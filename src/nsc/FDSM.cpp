/*******************************************************************************

			NES Sound Driver & Library	(NSD.lib)	MML Compiler

	Copyright (c) 2012 A.Watanabe (S.W.), All rights reserved.
	 For conditions of distribution and use, see copyright notice in "nsc.cpp".

*******************************************************************************/

#include "StdAfx.h"
#include "FDSM.h"

//==============================================================
//		�R���X�g���N�^
//--------------------------------------------------------------
//	������
//		MMLfile*			MML			MML�t�@�C���̃I�u�W�F�N�g
//		unsigned	int		_id			FDSM�ԍ�
//		const		_CHAR	_strName[]	�I�u�W�F�N�g��
//	���Ԓl
//					����
//==============================================================
FDSM::FDSM(MMLfile* MML, size_t _id, const _CHAR _strName[]):
	MusicItem(_id, _strName)
{
	//----------------------
	//Local�ϐ�


//	�萔��`
enum	Command_ID_mml {
	FDSM_Num,
	FDSM_Commma
};

//	�����́AMML�\���Ŏg����R�}���h�B
const	static	Command_Info	Command[] = {
		{	"%",	FDSM_Num	},
		{	"$",	FDSM_Num	},
		{	"0",	FDSM_Num	},
		{	"1",	FDSM_Num	},
		{	"2",	FDSM_Num	},
		{	"3",	FDSM_Num	},
		{	"4",	FDSM_Num	},
		{	"5",	FDSM_Num	},
		{	"6",	FDSM_Num	},
		{	"7",	FDSM_Num	},
		{	"8",	FDSM_Num	},
		{	"9",	FDSM_Num	},
		{	",",	FDSM_Commma	}
};

				int		i;
				char	cData;
	unsigned	int		ptFDSM		= 0;
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

			case(FDSM_Num):
				MML->Back();
				i = MML->GetInt_With_Chk_Range(_T("FDSM�̔g�`�p�^�[��"),0,7);
				WAVE.append((char)1, (char)i & 0x07);
				ptFDSM++;
				break;

			case(FDSM_Commma):
				break;

			//unknown command
			default:
				MML->ErrUnknownCmd();
				break;
		}
	}
	if(ptFDSM < 32){
		MML->Err(_T("�g�`�p�^�[����32�̐������L�q���Ă��������B32�ɖ����Ȃ��ł��B"));
	}
	if(ptFDSM > 32){
		MML->Err(_T("�g�`�p�^�[����32�̐������L�q���Ă��������B32�𒴂��Ă��܂��B"));
	}
	i = 0;
	while(ptFDSM>0){
		code.append((char)1, WAVE[i]);
		i++;
		ptFDSM--;
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
FDSM::~FDSM(void)
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
void	FDSM::getAsm(MusicFile* MUS)
{
	*MUS << MUS->Header.Label.c_str() << "FDSM" << m_id << ":" << endl;
	MusicItem::getAsm(MUS);
}
