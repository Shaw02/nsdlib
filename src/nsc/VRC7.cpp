/*******************************************************************************

			NES Sound Driver & Library	(NSD.lib)	MML Compiler

	Copyright (c) 2012 A.Watanabe (S.W.), All rights reserved.
	 For conditions of distribution and use, see copyright notice in "nsc.cpp".

*******************************************************************************/

#include "StdAfx.h"
#include "VRC7.h"

//==============================================================
//		�R���X�g���N�^
//--------------------------------------------------------------
//	������
//		MMLfile*			MML			MML�t�@�C���̃I�u�W�F�N�g
//					size_t	_id			VRC7���F�ԍ�
//		const		_CHAR	_strName[]	�I�u�W�F�N�g��
//	���Ԓl
//					����
//==============================================================
VRC7::VRC7(MMLfile* MML, size_t _id, const _CHAR _strName[]):
	MusicItem(_id, _strName)
{
	//----------------------
	//Local�ϐ�
				int		i;
				bool	_mset	= false;
	unsigned	char	_mode	= 0;
	unsigned	char	_pt		= 0;
	unsigned	char	_opll[24];
				char	cData;

//	�萔��`
enum	Command_ID_mml {
	VRC7_Normal,
	VRC7_Resister,
	VRC7_Num,
	VRC7_Comma
};

//	�����́AMML�\���Ŏg����R�}���h�B
const	static	Command_Info	Command[] = {
		{	"@R",	VRC7_Resister	},
		{	"@",	VRC7_Normal		},
		{	"0",	VRC7_Num		},
		{	"1",	VRC7_Num		},
		{	"2",	VRC7_Num		},
		{	"3",	VRC7_Num		},
		{	"4",	VRC7_Num		},
		{	"5",	VRC7_Num		},
		{	"6",	VRC7_Num		},
		{	"7",	VRC7_Num		},
		{	"8",	VRC7_Num		},
		{	"9",	VRC7_Num		},
		{	"$",	VRC7_Num		},
		{	"%",	VRC7_Num		},
		{	",",	VRC7_Comma		}
};


	//------------------------------
	//�N���X�̏����ݒ�

	iSize = 8;
	code.resize(iSize);

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

			case(VRC7_Normal):
				if(_mset == true){
					MML->Err(_T("@, @R�R�}���h�͂P�񂾂��w�肵�ĉ������B"));
				}
				_mset = true;
				_mode = 0;
				break;

			case(VRC7_Resister):
				if(_mset == true){
					MML->Err(_T("@, @R�R�}���h�͂P�񂾂��w�肵�ĉ������B"));
				}
				_mset = true;
				_mode = 1;
				break;

			case(VRC7_Num):
				if(_mset == false){
					MML->Err(_T("�悸��@, @R�R�}���h���L�q���ĉ������B"));
				}
				MML->Back();
				i = MML->GetInt_With_Chk_Range(_T("�p�����[�^"),0,255);
				if(_mode == 0){
					if(_pt >= 24){
						MML->Err(_T("VRC7(�p�����[�^�x�[�X)�̈�����24�𒴂��܂����B"));
					} else {
						_opll[_pt] = (unsigned char)i;
					}
				} else {
					if(_pt >= 8){
						MML->Err(_T("VRC7(���W�X�^�x�[�X)�̈�����8�𒴂��܂����B"));
					} else {
						code[_pt] = (unsigned char)i;
					}
				}
				_pt++;
				break;
			case(VRC7_Comma):
				break;

			//unknown command
			default:
				MML->ErrUnknownCmd();
				break;
		}
	}

	if(_mode == 0){
		if(_pt < 24){
			MML->Err(_T("VRC7(�p�����[�^�x�[�X)�̈�����24�ɖ����Ȃ��ł��B"));
		}

		//TL FB
		//0  1
		//AR DR SL RR KL MT AM VB EG KR DT
		//2  3  4  5  6  7  8  9  10 11 12
		//AR DR SL RR KL MT AM VB EG KR DT
		//13 14 15 16 17 18 19 20 21 22 23

		code[0] = ((_opll[ 8] & 0x01) << 7) | ((_opll[ 9] & 0x01) << 6) | ((_opll[10] & 0x01) << 5) | ((_opll[11] & 0x01) << 4) | (_opll[ 7] & 0x0F);
		code[1] = ((_opll[19] & 0x01) << 7) | ((_opll[20] & 0x01) << 6) | ((_opll[21] & 0x01) << 5) | ((_opll[22] & 0x01) << 4) | (_opll[18] & 0x0F);
		code[2] = ((_opll[ 6] & 0x03) << 6) |  (_opll[ 0] & 0x3F);
		code[3] = ((_opll[17] & 0x03) << 6) | ((_opll[23] & 0x01) << 4) | ((_opll[12] & 0x01) << 3) |  (_opll[ 1] & 0x07);
		code[4] = ((_opll[ 2] & 0x0F) << 4) |  (_opll[ 3] & 0x0F);
		code[5] = ((_opll[13] & 0x0F) << 4) |  (_opll[14] & 0x0F);
		code[6] = ((_opll[ 4] & 0x0F) << 4) |  (_opll[ 5] & 0x0F);
		code[7] = ((_opll[15] & 0x0F) << 4) |  (_opll[16] & 0x0F);
	} else {
		if(_pt < 8){
			MML->Err(_T("VRC7(���W�X�^�x�[�X)�̈�����8�ɖ����Ȃ��ł��B"));
		}
	}
}

//==============================================================
//		�f�X�g���N�^
//--------------------------------------------------------------
//	������
//				����
//	���Ԓl
//				����
//==============================================================
VRC7::~VRC7(void)
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
void	VRC7::getAsm(MusicFile* MUS)
{
	*MUS << MUS->Header.Label << "VRC7" << m_id << ":" << endl;
	MusicItem::getAsm(MUS);
}
