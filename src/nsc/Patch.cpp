/*******************************************************************************

			NES Sound Driver & Library	(NSD.lib)	MML Compiler

	Copyright (c) 2012 A.Watanabe (S.W.), All rights reserved.
	 For conditions of distribution and use, see copyright notice in "nsc.cpp".

*******************************************************************************/

#include "StdAfx.h"
#include "Patch.h"

/****************************************************************/
/*					�O���[�o���ϐ��i�N���X�����ǁE�E�E�j		*/
/****************************************************************/
extern	OPSW*			cOptionSW;	//�I�v�V�������ւ̃|�C���^�ϐ�

//==============================================================
//		�R���X�g���N�^
//--------------------------------------------------------------
//	������
//		MMLfile*			MML			MML�t�@�C���̃I�u�W�F�N�g
//					size_t	_id			�p�b�`�ԍ�
//	���Ԓl
//					����
//==============================================================
Patch::Patch(MMLfile* MML, size_t _id):
	m_id(_id),
	f_error(false)
{
	//----------------------
	//Local�ϐ�


//	�萔��`
enum	Command_ID_mml {
	Patch_MacroSet,
	Patch_Macro,

	Patch_C,
	Patch_Cis,
	Patch_D,
	Patch_Dis,
	Patch_E,
	Patch_F,
	Patch_Fis,
	Patch_G,
	Patch_Gis,
	Patch_A,
	Patch_Ais,
	Patch_B,
	Patch_Note,

	Patch_Off_Evoi,
	Patch_Off_Evol,
	Patch_Off_Em,
	Patch_Off_En,

	Patch_Evoi,
	Patch_Evol,
	Patch_Em,
	Patch_En,

	Patch_Gate_q,
	Patch_Gate_u,

	Patch_Voice,
	Patch_KeyShift,
	Patch_n163set,
	Patch_Sweep,
	Patch_Sub

};

//	�����́AMML�\���Ŏg����R�}���h�B
const	static	Command_Info	Command[] = {
		{	"$$",	Patch_MacroSet	},
		{	"$",	Patch_Macro		},

		{	"c#",	Patch_Cis	},
		{	"d#",	Patch_Dis	},
		{	"f#",	Patch_Fis	},
		{	"g#",	Patch_Gis	},
		{	"a#",	Patch_Ais	},

		{	"c+",	Patch_Cis	},
		{	"d+",	Patch_Dis	},
		{	"f+",	Patch_Fis	},
		{	"g+",	Patch_Gis	},
		{	"a+",	Patch_Ais	},

		{	"d-",	Patch_Cis	},
		{	"e-",	Patch_Dis	},
		{	"g-",	Patch_Fis	},
		{	"a-",	Patch_Gis	},
		{	"b-",	Patch_Ais	},

		{	"c",	Patch_C		},
		{	"d",	Patch_D		},
		{	"e",	Patch_E		},
		{	"f",	Patch_F		},
		{	"g",	Patch_G		},
		{	"a",	Patch_A		},
		{	"b",	Patch_B		},

		{	"��#",		Patch_Cis	},
		{	"��#",		Patch_Dis	},
		{	"�ӂ�#",	Patch_Fis	},
		{	"��#",		Patch_Fis	},
		{	"��#",		Patch_Gis	},
		{	"��#",		Patch_Ais	},

		{	"��-",	Patch_Cis	},
		{	"��-",	Patch_Dis	},
		{	"��-",	Patch_Fis	},
		{	"��-",	Patch_Gis	},
		{	"��-",	Patch_Ais	},

		{	"��",	Patch_C		},
		{	"��",	Patch_D		},
		{	"��",	Patch_E		},
		{	"�ӂ�",	Patch_F		},
		{	"��",	Patch_F		},
		{	"��",	Patch_G		},
		{	"��",	Patch_A		},
		{	"��",	Patch_B		},

		{	"�h#",		Patch_Cis	},
		{	"��#",		Patch_Dis	},
		{	"�t�@#",	Patch_Fis	},
		{	"�t#",		Patch_Fis	},
		{	"�\#",		Patch_Gis	},
		{	"��#",		Patch_Ais	},

		{	"��-",	Patch_Cis	},
		{	"�~-",	Patch_Dis	},
		{	"�\-",	Patch_Fis	},
		{	"��-",	Patch_Gis	},
		{	"�V-",	Patch_Ais	},

		{	"�h",	Patch_C		},
		{	"��",	Patch_D		},
		{	"�~",	Patch_E		},
		{	"�t�@",	Patch_F		},
		{	"�t",	Patch_F		},
		{	"�\",	Patch_G		},
		{	"��",	Patch_A		},
		{	"�V",	Patch_B		},

		{	"n",	Patch_Note	},


		{	"E@*",	Patch_Off_Evoi	},
		{	"Ev*",	Patch_Off_Evol	},
		{	"Em*",	Patch_Off_Em	},
		{	"En*",	Patch_Off_En	},

		{	"E@",	Patch_Evoi		},
		{	"Ev",	Patch_Evol		},
		{	"Em",	Patch_Em		},
		{	"En",	Patch_En		},

		{	"q",	Patch_Gate_q	},
		{	"u",	Patch_Gate_u	},

		{	"s",	Patch_Sweep		},
		{	"@NS",	Patch_n163set	},
		{	"@",	Patch_Voice		},
		{	"_",	Patch_KeyShift	},
		{	"S",	Patch_Sub		}

};

				int		i;
				char	cData;

	//------------------------------
	//�N���X�̏����ݒ�
	setN(MML, 0);	//�m�[�g�ԍ� 0 �̏��͕K���쐬����B


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

			case(Patch_Macro):
				MML->CallMacro();
				break;

			case(Patch_MacroSet):
				MML->SetMacro(1);
				break;

			case(Patch_C):
				setKey(MML, 0);
				break;

			case(Patch_Cis):
				setKey(MML, 1);
				break;

			case(Patch_D):
				setKey(MML, 2);
				break;

			case(Patch_Dis):
				setKey(MML, 3);
				break;

			case(Patch_E):
				setKey(MML, 4);
				break;

			case(Patch_F):
				setKey(MML, 5);
				break;

			case(Patch_Fis):
				setKey(MML, 6);
				break;

			case(Patch_G):
				setKey(MML, 7);
				break;

			case(Patch_Gis):
				setKey(MML, 8);
				break;

			case(Patch_A):
				setKey(MML, 9);
				break;

			case(Patch_Ais):
				setKey(MML, 10);
				break;

			case(Patch_B):
				setKey(MML, 11);
				break;

			case(Patch_Note):
				setN(MML, MML->GetInt());
				break;

			case(Patch_Voice):
				if(m_now_Patch->fEvoi == true){
					MML->Err(_T("���F�G���x���[�v�Ɠ����ɒ�`���邱�Ƃ͂ł��܂���B"));
				}
				if(m_now_Patch->fVoi == true){
					MML->Err(_T("���F�̂Q�d��`�ł��B"));
				}
				i = MML->GetInt();
				if( (i<0) || (i>255) ){
					MML->Err(_T("���F��0�`255�͈̔͂Ŏw�肵�Ă��������B"));
				}
				m_now_Patch->iVoi		= i;
				m_now_Patch->fVoi		= true;
				m_now_Patch->sw_Evoi	= false;
				break;

			case(Patch_Off_Evoi):
				MML->Err(_T("���F�G���x���[�v�́A@�R�}���h�Ŗ����ɂł��܂��B"));
				break;

			case(Patch_Off_Evol):
				if(m_now_Patch->fEvol == true){
					MML->Err(_T("���ʃG���x���[�v�̂Q�d��`�ł��B"));
				}
				m_now_Patch->fEvol		= true;
				m_now_Patch->sw_Evol	= false;
				break;

			case(Patch_Off_Em):
				if(m_now_Patch->fEm == true){
					MML->Err(_T("�����G���x���[�v�̂Q�d��`�ł��B"));
				}
				m_now_Patch->fEm		= true;
				m_now_Patch->sw_Em		= false;
				break;

			case(Patch_Off_En):
				if(m_now_Patch->fEn == true){
					MML->Err(_T("�m�[�g�G���x���[�v�̂Q�d��`�ł��B"));
				}
				m_now_Patch->fEn		= true;
				m_now_Patch->sw_En		= false;
				break;

			case(Patch_Evoi):
				if(m_now_Patch->fEvoi == true){
					MML->Err(_T("���F�G���x���[�v�̂Q�d��`�ł��B"));
				}
				if(m_now_Patch->fVoi == true){
					MML->Err(_T("���F�Ɠ����ɒ�`���邱�Ƃ͂ł��܂���B"));
				}
				m_now_Patch->iEvoi		= MML->GetInt();
				m_now_Patch->fEvoi		= true;
				m_now_Patch->sw_Evoi	= true;
				break;

			case(Patch_Evol):
				if(m_now_Patch->fEvol == true){
					MML->Err(_T("���ʃG���x���[�v�̂Q�d��`�ł��B"));
				}
				m_now_Patch->iEvol		= MML->GetInt();
				m_now_Patch->fEvol		= true;
				m_now_Patch->sw_Evol	= true;
				break;

			case(Patch_Em):
				if(m_now_Patch->fEm == true){
					MML->Err(_T("�����G���x���[�v�̂Q�d��`�ł��B"));
				}
				m_now_Patch->iEm		= MML->GetInt();
				m_now_Patch->fEm		= true;
				m_now_Patch->sw_Em		= true;
				break;

			case(Patch_En):
				if(m_now_Patch->fEn == true){
					MML->Err(_T("�m�[�g�G���x���[�v�̂Q�d��`�ł��B"));
				}
				m_now_Patch->iEn		= MML->GetInt();
				m_now_Patch->fEn		= true;
				m_now_Patch->sw_En		= true;
				break;

			case(Patch_Gate_q):
				if(m_now_Patch->fGate_q == true){
					MML->Err(_T("q�R�}���h�̂Q�d��`�ł��B"));
				}
				m_now_Patch->iGate_q	= MML->GetInt();
				m_now_Patch->fGate_q	= true;
				break;

			case(Patch_Gate_u):
				if(m_now_Patch->fGate_u == true){
					MML->Err(_T("u�R�}���h�̂Q�d��`�ł��B"));
				}
				cData = MML->GetChar();
				if(cData == '0'){
					i = 0;
				} else {
					MML->Back();
					i = MML->GetLength(-1);
				}
				m_now_Patch->iGate_u	= i;
				m_now_Patch->fGate_u	= true;
				break;

			case(Patch_KeyShift):
				if(m_now_Patch->fKey == true){
					MML->Err(_T("�ڒ��̂Q�d��`�ł��B"));
				}
				m_now_Patch->iKey		= MML->GetInt();
				m_now_Patch->fKey		= true;
				break;

			case(Patch_Sweep):
				if(m_now_Patch->fSweep == true){
					MML->Err(_T("s�R�}���h�̂Q�d��`�ł��B"));
				} else {
					int		iSpeed;
					int		iDepth;
					char	c;

					iSpeed	= MML->GetInt();
					cData	= MML->GetChar();
					if(cData != ','){
						if( (iSpeed < 0) || (iSpeed > 255) ){
							MML->Err(_T("s�R�}���h��0�`255�͈̔͂Ŏw�肵�Ă��������B"));
						}
						MML->Back();
						c = (unsigned char)iSpeed;
					} else {
						if( (iSpeed < 0) || (iSpeed > 15) ){
							MML->Err(_T("s�R�}���h�̑�1�p�����[�^��0�`15�͈̔͂Ŏw�肵�Ă��������B"));
						}
						iDepth = MML->GetInt();
						if( (iDepth < 0) || (iDepth > 15) ){
							MML->Err(_T("s�R�}���h�̑�2�p�����[�^��0�`15�͈̔͂Ŏw�肵�Ă��������B"));
						}
						c = (unsigned char)(((iSpeed & 0x0F) << 4) | (iDepth & 0x0F));
					}
					m_now_Patch->iSweep		= c;
					m_now_Patch->fSweep		= true;
				}
				break;

			case(Patch_n163set):
				if(m_now_Patch->fVoi == true){
					MML->Err(_T("@�R�}���h�����Ɏw�肳��Ă��܂��B"));
				}
				if(m_now_Patch->fEvoi == true){
					MML->Err(_T("E@�R�}���h�����Ɏw�肳��Ă��܂��B"));
				}
				i = MML->GetInt();
				if((i<0) || (i>252)){
					MML->Err(_T("n16x�̔g�`�J�n�_��0�`252�͈̔͂Ŏw�肵�Ă��������B"));
				}
				if((i % 4) != 0){
					MML->Err(_T("n16x�̔g�`�J�n�_��4�̔{���Ŏw�肵�Ă��������B"));
				}
				m_now_Patch->iVoi		= i/4;
				m_now_Patch->fVoi		= true;
				m_now_Patch->sw_Evoi	= false;
				cData = MML->GetChar();
				if(cData == ','){
					if(m_now_Patch->fSweep == true){
						MML->Err(_T("s�R�}���h�����Ɏw�肳��Ă��܂��B"));
					}
					i = MML->GetInt();
					if((i<4) || (i>256)){
						MML->Err(_T("n16x�̃T���v������4�`256�͈̔͂Ŏw�肵�Ă��������B"));
					}
					if((i % 4) != 0){
						MML->Err(_T("n16x�̃T���v������4�̔{���Ŏw�肵�Ă��������B"));
					}
					m_now_Patch->iSweep		= (unsigned char)(64 - (i/4));
					m_now_Patch->fSweep		= true;
				} else {
					MML->Back();
				}
				break;

			case(Patch_Sub):
				if(m_now_Patch->fSub == true){
					MML->Err(_T("�T�u���[�`���̂Q�d��`�ł��B"));
				}
				m_now_Patch->iSub		= MML->GetInt();
				m_now_Patch->fSub		= true;

				cData = MML->GetChar();
				if(cData == ','){	//�œK�������t���O
					i = MML->GetInt();
					switch(i){
						case(0):
							m_now_Patch->fSub_opt=false;
							break;
						case(1):
							m_now_Patch->fSub_opt=true;
							break;
						default:
							MML->Err(_T("�T�u���[�`���̍œK���t���O��0�`1�͈̔͂Ŏw�肵�Ă��������B"));
							break;
					}
				} else {
					MML->Back();
				}
				break;
		
		//unknown command
			default:
				MML->ErrUnknownCmd();
				break;
		}
	}

	//Local Macro�̉��
	MML->DeleteMacro(1);

	//Debug message�@�i���������o�͂���̂Œ��ӁB�j
	if(cOptionSW->iDebug & DEBUG_Macros){
		DebugMsg();
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
Patch::~Patch(void)
{
	//----------------------
	//Delete Class
	for(map<size_t,patch_scrap*>::iterator it=m_Patch.begin(), e=m_Patch.end(); it!=e; ++it){
		delete it->second;
	}
	m_Patch.clear();
}

//==============================================================
//		�f�o�b�O�p
//--------------------------------------------------------------
//	������
//				����
//	���Ԓl
//				����
//==============================================================
void	Patch::DebugMsg(void)
{
	//----------------------
	//Delete Class
	cout	<< "==== [ Patch(" << m_id << ") ] ====" << endl;

	for(map<size_t,patch_scrap*>::iterator it=m_Patch.begin(), e=m_Patch.end(); it!=e; ++it){
		m_now_Patch = it->second;

		cout	<<	"  n" << it->first;
		if(get_fGate_q()){ cout	<<	"  q"	<<	get_iGate_q();	};
		if(get_fGate_u()){ cout	<<	"  u"	<<	get_iGate_u();	};
		if(get_fSub()){	cout	<<	"  S"	<<	get_iSub()	<<	","	<<	get_fSub_opt();		};
		if(get_fKey()){	cout	<<	"  _"	<<	get_iKey();		};
		if(get_fSweep()){cout	<<	"  s"	<<	(int)get_iSweep();	};
		if(get_fVoi()){	cout	<<	"  @"	<<	get_iVoi();		};
		if(get_fEvoi()){cout	<<	" E@"	<<	get_iEvoi();	};

		if(get_fEvol()){
			if(get_sw_Evol()){
				cout	<<	" Ev*";
			} else {
				cout	<<	" Ev"	<<	get_iEvol();
			}
		};

		if(get_fEm()){
			if(get_sw_Em()){
				cout	<<	" Em*";
			} else {
				cout	<<	" Em"	<<	get_iEm();
			}
		};

		if(get_fEn()){
			if(get_sw_En()){
				cout	<<	" En*";
			} else {
				cout	<<	" En"	<<	get_iEn();
			}
		};

		cout	<<	endl;
	}
}

//==============================================================
//		�ݒ�
//--------------------------------------------------------------
//	������
//		MMLfile*	MML		MML�t�@�C���̃I�u�W�F�N�g
//			int	key		�L�[�ԍ��i0:C / 1:Cis / ...�j
//	���Ԓl
//				����
//==============================================================
void	Patch::setKey(MMLfile* MML, int key)
{
	setN(MML, ((MML->GetInt()-1) * 12) + key);
}

//==============================================================
//		�ݒ�
//--------------------------------------------------------------
//	������
//		MMLfile*		MML		MML�t�@�C���̃I�u�W�F�N�g
//				int		note	�m�[�g�ԍ�
//	���Ԓl
//				����
//==============================================================
void	Patch::setN(MMLfile* MML, int note)
{

	if((note<0) || (note>255)){
		MML->Err(_T("���K�͈̔͂𒴂��Ă��܂��B"));
	}

	//�p�b�`�̐ݒ�
	m_kn = note;

	if(m_Patch.count(m_kn) != 0){
		//�����p�b�`�̃��[�h
		m_now_Patch =	m_Patch[m_kn];
	} else {
		//�p�b�`�̐V�K�쐬
		m_now_Patch			= new	patch_scrap;
		m_Patch[m_kn]		= m_now_Patch;

		m_now_Patch->fVoi	= false;
		m_now_Patch->fEvoi	= false;
		m_now_Patch->fEvol	= false;
		m_now_Patch->fEm	= false;
		m_now_Patch->fEn	= false;
		m_now_Patch->fKey	= false;
		m_now_Patch->fSweep	= false;
		m_now_Patch->fSub	= false;
		m_now_Patch->fSub_opt= false;
		m_now_Patch->fGate_q = false;
		m_now_Patch->fGate_u = false;

		m_now_Patch->sw_Evoi= false;
		m_now_Patch->sw_Evol= false;
		m_now_Patch->sw_Em	= false;
		m_now_Patch->sw_En	= false;
	}

}

//==============================================================
//		
//--------------------------------------------------------------
//	������
//				int		note	�m�[�g�ԍ�
//	���Ԓl
//				
//==============================================================
void	Patch::setNote(int i)
{
	m_kn = i;

	while(m_kn > 0){
		if(m_Patch.count(m_kn) != 0){
			break;
		} else {
			m_kn--;
		}
	}

	m_now_Patch = m_Patch[m_kn];

}
