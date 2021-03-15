/*******************************************************************************

			NES Sound Driver & Library	(NSD.lib)	MML Compiler

	Copyright (c) 2012 A.Watanabe (S.W.), All rights reserved.
	 For conditions of distribution and use, see copyright notice in "nsc.cpp".

*******************************************************************************/

#include "StdAfx.h"
#include "N163.h"

//==============================================================
//		�R���X�g���N�^
//--------------------------------------------------------------
//	������
//		MMLfile*			MML			MML�t�@�C���̃I�u�W�F�N�g
//					size_t	_id			N163�ԍ�
//		const		_CHAR	_strName[]	�I�u�W�F�N�g��
//	���Ԓl
//					����
//==============================================================
N163::N163(MMLfile* MML, size_t _id, const _CHAR _strName[]):
	MusicItem(_id, _strName)
{
	//----------------------
	//Local�ϐ�


//	�萔��`
enum	Command_ID_mml {
	N163_Num,
	N163_Commma
};

//	�����́AMML�\���Ŏg����R�}���h�B
const	static	Command_Info	Command[] = {
		{	"%",	N163_Num	},
		{	"$",	N163_Num	},
		{	"0",	N163_Num	},
		{	"1",	N163_Num	},
		{	"2",	N163_Num	},
		{	"3",	N163_Num	},
		{	"4",	N163_Num	},
		{	"5",	N163_Num	},
		{	"6",	N163_Num	},
		{	"7",	N163_Num	},
		{	"8",	N163_Num	},
		{	"9",	N163_Num	},
		{	",",	N163_Commma	}
};

				int		i;
				char	cData;
	unsigned	int		ptN163		= 0;
				string	WAVE;

	//------------------------------
	//�N���X�̏����ݒ�
	WAVE.clear();
	code.resize(0);

	//------------------------------
	//�R���p�C��

	// { �̌���
	MML->Chk_LeftCurlyBrace();

	// } ������܂ŁA�L�q�u���b�N�����R���p�C������B
	while(MML->GetChar_With_Chk_RightCurlyBrace(&cData)){

		//�P�߂�
		MML->Back();

		//�e�R�}���h���̏���
		switch(MML->GetCommandID(Command, sizeof(Command)/sizeof(Command_Info))){

			case(N163_Num):
				MML->Back();
				i = MML->GetInt_With_Chk_Range(_T("n163�̔g�`�p�^�[��"),0,15);
				WAVE.append((char)1, (char)i & 0x0F);
				ptN163++;
				break;

			case(N163_Commma):
				break;

			//unknown command
			default:
				MML->ErrUnknownCmd();
				break;
		}
	}
	if(ptN163 > 256){
		MML->Err(_T("�T���v������256���z���Ă��܂��B"));
	}
	if((ptN163 & 0x03) != 0){
		MML->Err(_T("�T���v������4�̔{���ŋL�q���ĉ������B"));
	}
	ptN163 >>= 1;
	code.append((char)1, (unsigned char)ptN163);
	i = 0;
	while(ptN163>0){
		code.append((char)1, WAVE[i*2+0] | (WAVE[i*2+1]<<4) );
		i++;
		ptN163--;
	}


	iSize = code.size();
}

//==============================================================
//		�f�X�g���N�^
//--------------------------------------------------------------
//	������
//				����
//	���Ԓl
//				����
//==============================================================
N163::~N163(void)
{
}
//==============================================================
//		�R�[�h�̎擾
//--------------------------------------------------------------
//	������
//		MusicFile*	MUS		�R�[�h���o�͂���ȃf�[�^�t�@�C���E�I�u�W�F�N�g
//	���Ԓl
//				����
//==============================================================
void	N163::getAsm(MusicFile* MUS)
{
	*MUS << MUS->Header.Label.c_str() << "N163" << m_id << ":" << endl;
	MusicItem::getAsm(MUS);
}
