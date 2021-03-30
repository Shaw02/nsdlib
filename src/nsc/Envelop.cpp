/*******************************************************************************

			NES Sound Driver & Library	(NSD.lib)	MML Compiler

	Copyright (c) 2012 A.Watanabe (S.W.), All rights reserved.
	 For conditions of distribution and use, see copyright notice in "nsc.cpp".

*******************************************************************************/

#include "StdAfx.h"
#include "Envelop.h"

//==============================================================
//		�R���X�g���N�^
//--------------------------------------------------------------
//	������
//		MMLfile*			MML			MML�t�@�C���̃I�u�W�F�N�g
//					size_t	_id			�G���x���[�v�ԍ�
//		const		_CHAR	_strName[]	�I�u�W�F�N�g��
//	���Ԓl
//					����
//==============================================================
Envelop::Envelop(MMLfile* MML, size_t _id, const _CHAR _strName[]):
	MusicItem(_id, _strName)
{
	//----------------------
	//Local�ϐ�


//	�萔��`
enum	Command_ID_mml {
	Env_Num,
	Env_Sweep,
	Env_Hold,
	Env_Loop,
	Env_Release,
	Env_Commma
};

//	�����́AMML�\���Ŏg����R�}���h�B
const	static	Command_Info	Command[] = {
		{	"$",	Env_Num		},
		{	"%",	Env_Num		},
		{	"-",	Env_Num		},
		{	"0",	Env_Num		},
		{	"1",	Env_Num		},
		{	"2",	Env_Num		},
		{	"3",	Env_Num		},
		{	"4",	Env_Num		},
		{	"5",	Env_Num		},
		{	"6",	Env_Num		},
		{	"7",	Env_Num		},
		{	"8",	Env_Num		},
		{	"9",	Env_Num		},
		{	"{",	Env_Sweep	},
		{	"(",	Env_Sweep	},
		{	"d",	Env_Hold	},
		{	"D",	Env_Hold	},
		{	"��",	Env_Hold	},
		{	"�c",	Env_Hold	},
		{	"|",	Env_Loop	},
		{	"�b",	Env_Loop	},
		{	"l",	Env_Loop	},
		{	"L",	Env_Loop	},
		{	"��",	Env_Loop	},
		{	"�k",	Env_Loop	},
		{	"r",	Env_Release	},
		{	"R",	Env_Release	},
		{	"��",	Env_Release	},
		{	"�q",	Env_Release	},
		{	"�@",	Env_Commma	},
		{	"�C",	Env_Commma	},
		{	"�A",	Env_Commma	},
		{	",",	Env_Commma	}
};

				char	cData;
				int		i;

				int		iValue	=255;	//���̐ݒ�l
				int		iLength	=0;		//���������O�X���k�p

	//------------------------------
	//�N���X�̏����ݒ�

	Loop_Normal		= -1;
	Loop_Release	= -1;
	Release			= false;
	ptEnvelop		= 1;

	code.resize(0);
	code.append((char)1, (char)0);



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

			case(Env_Num):
				MML->Back();
				i = MML->GetInt_With_Chk_Range(_T("�G���x���[�v�̒l"),-64,127);
				if(iValue == i){
					iLength++;		//������������A���������O�X���k����
				} else {
					if(iLength>0){
						setHold(iLength-1);	//���܂ł̎��Ԃ����Z����B
						iLength	= 0;		//���������O�X���k������
					}
					iValue	= i;

					code.append((char)1, (char)i & 0x7F);
					ptEnvelop++;
				}
				break;

			case(Env_Sweep):
				if(iLength>0){
					setHold(iLength-1);
					iLength	= 0;	//���������O�X���k������
				}
				iValue = sweep(MML);
				break;

			case(Env_Hold):
				i = MML->GetInt_With_Chk_Range(_T("�G���x���[�v�̈ێ�����"),0,255);
				iLength += i+1;			//���������O�X�ɉ��Z����B
				break;

			case(Env_Loop):
				iValue	=255;			//���������O�X���k������
				if(iLength>0){
					setHold(iLength-1);	//�z�[���h���ԏo��
					iLength	= 0;		//���������O�X���k������
				}
				if(Release == false){
					if(ptEnvelop > 0x3F){
						MML->Warning(_T("���[�v�ʒu���w��ł���͈�(64Byte)�𒴂��܂����B"));
					}
					Loop_Normal		= ptEnvelop;
				} else {
					if(ptEnvelop > 0x3F){
						MML->Warning(_T("�����[�X���̃��[�v�ʒu���w��ł���͈�(64Byte)�𒴂��܂����B"));
					}
					Loop_Release	= ptEnvelop;
				}
				break;

			case(Env_Release):
				iValue	=255;			//���������O�X���k������
				if(iLength>0){
					setHold(iLength-1);	//�z�[���h���ԏo��
					iLength	= 0;		//���������O�X���k������
				}
				SetLoop(MML, Loop_Normal);
				code[0] = (unsigned char)ptEnvelop;
				Release = true;
				break;

			case(Env_Commma):
				break;

			//unknown command
			default:
				MML->ErrUnknownCmd();
				break;
		}
	}

	iValue	=255;			//���������O�X���k������
	if(iLength>0){
		setHold(iLength-1);	//�z�[���h���ԏo��
		iLength	= 0;		//���������O�X���k������
	}

	if(Release == true){
		SetLoop(MML, Loop_Release);
	} else {
		SetLoop(MML, Loop_Normal);
	}

	if(code.size() > 256){
		MML->Err(_T("�G���x���[�v�̒�`����256Byte���z���܂����B"));
	}

	iSize = code.size();
}

void Envelop::SetLoop(MMLfile* MML, int LoopPoint)
{
	if(LoopPoint == -1){
		if(Release == true){
			MML->Warning(_T("�����[�X���̃��[�v�|�C���g������܂���B�Ō�̒l�����[�v���܂��B"));
			if(ptEnvelop > 0x3F){
				MML->Warning(_T("�����[�X���̃��[�v�ʒu���w��ł���͈�(63Byte)�𒴂��܂����B"));
			}
		} else {
			MML->Warning(_T("���[�v�|�C���g������܂���B�Ō�̒l�����[�v���܂��B"));
			if(ptEnvelop > 0x3F){
				MML->Warning(_T("���[�v�ʒu���w��ł���͈�(63Byte)�𒴂��܂����B"));
			}
		}
		code.append((char)1, (char)((ptEnvelop-1) | 0xC0));
	} else {
		if(ptEnvelop == Loop_Normal){
			MML->Err(_T("L�R�}���h�Ńp�^�[����`���I��邱�Ƃ͂ł��܂���B"));
		}
		code.append((char)1, (char)(LoopPoint | 0xC0));
	}
	ptEnvelop++;
}
//==============================================================
//		�f�X�g���N�^
//--------------------------------------------------------------
//	������
//				����
//	���Ԓl
//				����
//==============================================================
Envelop::~Envelop(void)
{
}

//==============================================================
//		�ێ����Ԃ̃Z�b�g
//--------------------------------------------------------------
//	������
//		int		length		�ێ�����
//	���Ԓl
//				����
//==============================================================
void	Envelop::setHold(int length)
{
	while(length>15){
		code.append((char)1, (unsigned char)0x8F);
		ptEnvelop++;
		length -= 16;		//15�������΁A16�t���[���ێ�
	}
	code.append((char)1, (char)((length & 0x0F) | 0x80));
	ptEnvelop++;
}

//==============================================================
//		�X�C�[�v
//--------------------------------------------------------------
//	������
//		MMLfile*	MML		MML�t�@�C���̃I�u�W�F�N�g
//	���Ԓl
//				����
//==============================================================
int	Envelop::sweep(MMLfile* MML)
{
	unsigned	char	cData;

	int		iStart;
	int		iEnd;
	int		iLength;
	int		iDelta;

	int		now	= 0;
	int		cnt	= 0;

	int		i=0;
	int		temp;

	//--------------------------
	//��MML�ǂݍ���

	//
	iStart = MML->GetInt_With_Chk_Range(_T("�J�n�l"),-64,127);

	MML->Chk_Comma();
	iEnd = MML->GetInt_With_Chk_Range(_T("�I���l"),-64,127);

	MML->Chk_Comma();
	iLength = MML->GetInt_With_Chk_Range(_T("����"),1,255);

	cData = MML->GetChar();
	if((cData != ')') && (cData != '}')){
		MML->Err(_T(") ��������܂���ł����B"));
	}

	//--------------------------
	//���e�[�u���쐬

	iDelta = iEnd - iStart;

	while(i<iLength){
		temp = iStart + (iDelta * i) / iLength;
		if(i == 0){
			now = temp;
			cnt	= 0;
		} else if (temp != now){
			code.append((char)1, (char)now & 0x7F);
			ptEnvelop++;
			if(cnt>=1){
				setHold(cnt-1);
			}
			now = temp;
			cnt	= 0;
		} else {
			cnt++;
		}
		i++;
	}

	code.append((char)1, (char)now & 0x7F);
	ptEnvelop++;
	if(cnt>=1){
		setHold(cnt-1);
	}
	return(now);
}

//==============================================================
//		�R�[�h�̎擾
//--------------------------------------------------------------
//	������
//		MusicFile*	MUS		�R�[�h���o�͂���ȃf�[�^�t�@�C���E�I�u�W�F�N�g
//	���Ԓl
//				����
//==============================================================
void	Envelop::getAsm(MusicFile* MUS)
{
	*MUS << MUS->Header.Label << "Envelope" << m_id << ":" << endl;
	MusicItem::getAsm(MUS);
}
