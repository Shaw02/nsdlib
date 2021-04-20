/*******************************************************************************

			NES Sound Driver & Library	(NSD.lib)	MML Compiler

	Copyright (c) 2012 A.Watanabe (S.W.), All rights reserved.
	 For conditions of distribution and use, see copyright notice in "nsc.cpp".

*******************************************************************************/

#include "StdAfx.h"
#include "MusicFile.h"

/****************************************************************/
/*					�O���[�o���ϐ��i�N���X�����ǁE�E�E�j		*/
/****************************************************************/
extern	OPSW*			cOptionSW;	//�I�v�V�������ւ̃|�C���^�ϐ�

#ifdef _OPENMP
	extern	omp_lock_t		lock_cout;
#endif

//==============================================================
//		�R���X�g���N�^
//--------------------------------------------------------------
//	������
//		MMLfile*			MML			�l�l�k�t�@�C���̃I�u�W�F�N�g
//		string				_code		�����N����R�[�h�i*.bin�j�̃t�@�C����
//		const		_CHAR	_strName[]	�I�u�W�F�N�g��
//	���Ԓl
//				����
//==============================================================
MusicFile::MusicFile(MMLfile* MML, string _code, const _CHAR _strName[]):
	MusicItem(_strName),
	Header(_code),
	cDPCMinfo(NULL)
{

	try {

	//----------------------
	//Local�ϐ�

//	�萔��`
enum	Command_ID_MusicFile {
	//for NSF output
	id_Title,
	id_Composer,
	id_Copyright,
	id_Maker,
	id_Text,
	id_plst,
	id_psfx,
	id_mixe,
	id_vrc7_chg,
	id_OffsetPCM,
	id_Code,
	id_External,
	id_Bank,

	//for ASM output
	id_SegmentSEQ,
	id_SegmentPCM,
	id_Label,
	id_Priority,

	//General
	id_include,
	id_timebase,
	id_OctaveReverse,
	id_QReverse,
	id_bgm_num,
	id_se_num,
	id_releaseVolume,
	id_repeatMode,
	id_TieMode,
	id_offset_Ei,
	id_offset_Ev,
	id_offset_En,
	id_offset_Em,
	id_QMax,
	id_rest,
	id_wait,

	//Block
	id_DPCM,
	id_FDSC,
	id_FDSM,
	id_VRC7,
	id_N163,
	id_Envelop,
	id_Vibrato,
	id_Macro,
	id_Sub,
	id_BGM,
	id_SE,
	id_Patch,

	id_Null
};

//	�����́AMML�\���Ŏg����R�}���h�B
const	static	Command_Info	Command[] = {
		//for NSF output
		{	"#Title",			id_Title		},
		{	"#title",			id_Title		},
		{	"�Ȗ�",				id_Title		},
		{	"#Composer",		id_Composer		},
		{	"#composer",		id_Composer		},
		{	"��Ȏ�",			id_Composer		},
		{	"���",				id_Composer		},
		{	"#Copyright",		id_Copyright	},
		{	"#copyright",		id_Copyright	},
		{	"���",				id_Copyright	},
		{	"���쌠��",			id_Copyright	},
		{	"#Maker",			id_Maker		},
		{	"#maker",			id_Maker		},
		{	"#Ripper",			id_Maker		},
		{	"#ripper",			id_Maker		},
		{	"�����",			id_Maker		},
		{	"#Text",			id_Text			},
		{	"#text",			id_Text			},
		{	"�e�L�X�g",			id_Text			},
		{	"#PlayList",		id_plst			},
		{	"#Plst",			id_plst			},
		{	"#plst",			id_plst			},
		{	"���y����",			id_plst			},
		{	"�ȏ�",				id_plst			},
		{	"#PlaySfx",			id_psfx			},
		{	"#Psfx",			id_psfx			},
		{	"#psfx",			id_psfx			},
		{	"���ʉ�����",		id_psfx			},
		{	"#Mixe",			id_mixe			},
		{	"#mixe",			id_mixe			},
		{	"#Mix",				id_mixe			},
		{	"#mix",				id_mixe			},
		{	"#VRC7",			id_vrc7_chg		},
		{	"#OffsetPCM",		id_OffsetPCM	},	//Offset Address of ��PCM
		{	"#offsetPCM",		id_OffsetPCM	},	//Offset Address of ��PCM
		{	"#Code",			id_Code			},
		{	"#code",			id_Code			},
		{	"�R�[�h",			id_Code			},
		{	"#External",		id_External		},
		{	"#external",		id_External		},
		{	"#Bank",			id_Bank			},
		{	"#bank",			id_Bank			},
		{	"�o���N",			id_Bank			},
		//for ASM output
		{	"#SegmentPCM",		id_SegmentPCM	},	//Segment name for ��PCM
		{	"#segmentPCM",		id_SegmentPCM	},
		{	"#SegmentSEQ",		id_SegmentSEQ	},	//Segment name for Sequence
		{	"#segmentSEQ",		id_SegmentSEQ	},
		{	"#Segment",			id_SegmentSEQ	},	//Segment name for Sequence
		{	"#segment",			id_SegmentSEQ	},
		{	"#Label",			id_Label		},
		{	"#label",			id_Label		},

		//General
		{	"#Include",			id_include		},
		{	"#include",			id_include		},
		{	"�ǂ�",				id_include		},
		{	"#Timebase",		id_timebase		},
		{	"#timebase",		id_timebase		},
		{	"#OctaveReverse",	id_OctaveReverse},
		{	"#octaveReverse",	id_OctaveReverse},
		{	"#QReverse",		id_QReverse		},
		{	"#qReverse",		id_QReverse		},
		{	"#BGM",				id_bgm_num		},
		{	"#bgm",				id_bgm_num		},
		{	"���y��",			id_bgm_num		},
		{	"#SE",				id_se_num		},
		{	"#se",				id_se_num		},
		{	"���ʉ���",			id_se_num		},
		{	"#releaseVolume",	id_releaseVolume},
		{	"#ReleaseVolume",	id_releaseVolume},
		{	"#repeatMode",		id_repeatMode	},
		{	"#RepeatMode",		id_repeatMode	},
		{	"#TieMode",			id_TieMode		},
		{	"#tieMode",			id_TieMode		},	
		{	"#OffsetE@",		id_offset_Ei	},
		{	"#offsetE@",		id_offset_Ei	},
		{	"#OffsetEv",		id_offset_Ev	},
		{	"#offsetEv",		id_offset_Ev	},
		{	"#OffsetEm",		id_offset_Em	},
		{	"#offsetEm",		id_offset_Em	},
		{	"#OffsetEn",		id_offset_En	},
		{	"#offsetEn",		id_offset_En	},
		{	"#Priority",		id_Priority		},
		{	"#priority",		id_Priority		},
		{	"#QMax",			id_QMax			},
		{	"#Qmax",			id_QMax			},
		{	"#Rest",			id_rest			},
		{	"#rest",			id_rest			},
		{	"#Wait",			id_wait			},
		{	"#wait",			id_wait			},
		//Block
		{	"DPCM",				id_DPCM			},
		{	"FDSC",				id_FDSC			},
		{	"FDSM",				id_FDSM			},
		{	"VRC7",				id_VRC7			},
		{	"N163",				id_N163			},
		{	"Envelope",			id_Envelop		},
		{	"envelope",			id_Envelop		},
		{	"Envelop",			id_Envelop		},
		{	"envelop",			id_Envelop		},
		{	"�G���x���[�v",		id_Envelop		},
		{	"�r�u���[�g",		id_Vibrato		},
		{	"Sub",				id_Sub			},
		{	"sub",				id_Sub			},
		{	"BGM",				id_BGM			},
		{	"bgm",				id_BGM			},
		{	"���y",				id_BGM			},
		{	"SE",				id_SE			},
		{	"se",				id_SE			},
		{	"���ʉ�",			id_SE			},
		{	"Patch",			id_Patch		},

		//for 1 command
		{	"D",				id_DPCM			},
		{	"FC",				id_FDSC			},
		{	"FM",				id_FDSM			},
		{	"O",				id_VRC7			},
		{	"V",				id_VRC7			},
		{	"N",				id_N163			},
		{	"E",				id_Envelop		},
		{	"S",				id_Sub			},
		{	"P",				id_Patch		},
		{	"$",				id_Macro		},

		//for mck/ppmck
		{	"@DPCM",			id_DPCM			},
		{	"@FM",				id_FDSC			},
		{	"@MW",				id_FDSM			},
		{	"@OP",				id_VRC7			},
		{	"@N",				id_N163			},
		{	"@E",				id_Envelop		},

		{	"�@",				id_Null			},
	};

					size_t		i;
		string		msg;

		map<size_t,	Envelop*	>::iterator	itEnvelop;
		map<size_t,	BGM*		>::iterator	itBGM;
		map<size_t,	SE*			>::iterator	itSE;
		map<size_t,	Sub*		>::iterator	itSub;

	//	iSize = 0;

		//���̃I�u�W�F�N�g�͕K���g���i�œK���ΏۊO�j�B
		setUse();

		//���^�f�[�^�`�F�b�N�p�̃t���O������
		f_is_track_time		= false;
		f_is_track_fade		= false;
		f_is_track_label	= false;
		f_is_track_auth		= false;

		do{
			//�P�����ǂݍ��݁i�R�����g�`�F�b�N�Ainclude�t�@�C���̏I�[�`�F�b�N������j
	//		cData = MML->GetChar();
			MML->GetChar();

			//[EOF]�`�F�b�N
			if( MML->eom() ){
				break;
			}

			//�P�߂�
			MML->Back();

			//�R�}���h������̃`�F�b�N
			switch(MML->GetCommandID(Command, sizeof(Command)/sizeof(Command_Info))){
				//for NSF output
				case(id_Code):
					Header.Set_RomCode(MML);
					break;
				case(id_Title):
					Header.Set_Title(MML);
					break;
				case(id_Composer):
					Header.Set_Composer(MML);
					break;
				case(id_Copyright):
					Header.Set_Copyright(MML);
					break;
				case(id_Maker):
					Header.Set_Maker(MML);
					break;
				case(id_Text):
					Header.Text_Append(MML);
					break;
				case(id_plst):
					Header.Set_plst(MML);
					break;
				case(id_psfx):
					Header.Set_psfx(MML);
					break;
				case(id_mixe):
					Header.Set_mixe(MML);
					break;
				case(id_vrc7_chg):
					Header.Ser_VRC7(MML);
					break;
				case(id_OffsetPCM):
					Header.Set_OffsetPCM(MML);
					break;
				case(id_External):
					Header.Set_External(MML);
					break;
				case(id_Bank):
					Header.Set_Bank();
					break;
				//for ASM output
				case(id_SegmentSEQ):
					Header.Set_SegmentSEQ(MML);
					break;
				case(id_SegmentPCM):
					Header.Set_SegmentPCM(MML);
					break;
				case(id_Label):
					Header.Set_Label(MML);
					break;
				//General
				case(id_include):
					MML->include();
					break;
				case(id_bgm_num):
					Header.Set_Number_BGM(MML);
					break;
				case(id_se_num):
					Header.Set_Number_SE(MML);
					break;
				case(id_timebase):
					MML->timebase = MML->GetInt();	//����́AMML�t�@�C���̑����B
					break;
				case(id_OctaveReverse):
					MML->octave_reverse = true;		//����́AMML�t�@�C���̑����B
					break;
				case(id_QReverse):
					MML->q_reverse = true;			//����́AMML�t�@�C���̑����B
					break;
				case(id_releaseVolume):
					MML->iReleaseVolume	= MML->GetInt_With_Chk_Range(_T("#ReleaseVolume�R�}���h"),0,15);
					break;
				case(id_repeatMode):
					MML->iRepeatMode = MML->GetInt_With_Chk_Range(_T("#RepeatMode�R�}���h"),0,2);
					break;
				case(id_TieMode):
					MML->iTieMode = MML->GetInt_With_Chk_Range(_T("#TieMode�R�}���h"),0,1);
					break;
				case(id_offset_Ei):
					MML->offset_Ei = MML->GetInt();
					break;
				case(id_offset_Ev):
					MML->offset_Ev = MML->GetInt();
					break;
				case(id_offset_En):
					MML->offset_En = MML->GetInt();
					break;
				case(id_offset_Em):
					MML->offset_Em = MML->GetInt();
					break;
				case(id_Priority):
					MML->priority = (unsigned char)MML->GetInt_With_Chk_Range(_T("#priority�R�}���h"),0,3);
					break;
				case(id_QMax):
					MML->QMax = MML->GetInt();
					break;
				case(id_rest):
					MML->rest = MML->GetInt_With_Chk_Range(_T("#Rest�R�}���h"),0,3);
					break;
				case(id_wait):
					MML->wait = MML->GetInt_With_Chk_Range(_T("#Wait�R�}���h"),0,3);
					break;
				case(id_Macro):
					MML->SetMacro(0);
					break;
				case(id_Patch):
					MML->SetPatch();
					break;
				//MML
				case(id_DPCM):
					if(cDPCMinfo != NULL){
						MML->Err(_T("���ɁADPCM����`����Ă��܂��B"));
					}
					cDPCMinfo = new DPCMinfo(MML, Header.bank);
					if(cDPCMinfo->isError() == true){
						f_error = true;
					} else {
						ptcItem.push_back(cDPCMinfo);
					}
					break;
				case(id_FDSC):
					{
						FDSC*	_fdsc;
						i = MML->GetNum();
						//�d���`�F�b�N
						if(ptcFDSC.count(i) != 0){
							_SSTREAM	errMsg;
							errMsg << _T("���ɁAFDSC(") << i << _T(")�͒�`����Ă��܂��B");
							MML->Err(errMsg.str().c_str());
						}
						_fdsc = new FDSC(MML, i);
						ptcItem.push_back(_fdsc);
						ptcFDSC[i] = _fdsc;
					}
					break;
				case(id_FDSM):
					{
						FDSM*	_fdsm;
						i = MML->GetNum();
						//�d���`�F�b�N
						if(ptcFDSM.count(i) != 0){
							_SSTREAM	errMsg;
							errMsg << _T("���ɁAFDSM(") << i << _T(")�͒�`����Ă��܂��B");
							MML->Err(errMsg.str().c_str());
						}
						_fdsm = new FDSM(MML, i);
						ptcItem.push_back(_fdsm);
						ptcFDSM[i] = _fdsm;
					}
					break;
				case(id_VRC7):
					{
						VRC7*	_vrc7;
						i = MML->GetNum();
						//�d���`�F�b�N
						if(ptcVRC7.count(i) != 0){
							_SSTREAM	errMsg;
							errMsg << _T("���ɁAVRC7(") << i << _T(")�͒�`����Ă��܂��B");
							MML->Err(errMsg.str().c_str());
						}
						_vrc7 = new VRC7(MML, i);
						ptcItem.push_back(_vrc7);
						ptcVRC7[i] = _vrc7;
					}
					break;
				case(id_N163):
					{
						N163*	_n163;
						i = MML->GetNum();
						//�d���`�F�b�N
						if(ptcN163.count(i) != 0){
							_SSTREAM	errMsg;
							errMsg << _T("���ɁAN163(") << i << _T(")�͒�`����Ă��܂��B");
							MML->Err(errMsg.str().c_str());
						}
						_n163 = new N163(MML, i);
						ptcItem.push_back(_n163);
						ptcN163[i] = _n163;
					}
					break;
				case(id_Envelop):
					{
						Envelop*	_env;
						i = MML->GetNum();
						//�d���`�F�b�N
						if(ptcEnv.count(i) != 0){
							_SSTREAM	errMsg;
							errMsg << _T("���ɁAEnvelope(") << i << _T(")�͒�`����Ă��܂��B");
							MML->Err(errMsg.str().c_str());
						}
						_env = new Envelop(MML, i);
						ptcItem.push_back(_env);
						ptcEnv[i] = _env;
					}
					break;
				case(id_Vibrato):
					{
						Envelop*	_env;
						MML->offset_Em = 1000000;
						i = MML->GetNum() + MML->offset_Em;
						//�d���`�F�b�N
						if(ptcEnv.count(i) != 0){
							_SSTREAM	errMsg;
							errMsg << _T("���ɁA�r�u���[�g(") << (i - MML->offset_Em) << _T(")�͒�`����Ă��܂��B");
							MML->Err(errMsg.str().c_str());
						}
						_env = new Envelop(MML, i);
						ptcItem.push_back(_env);
						ptcEnv[i] = _env;
					}
					break;
				case(id_Sub):
					{
						Sub*	_sub;
						i = MML->GetNum();
						//�d���`�F�b�N
						if(ptcSub.count(i) != 0){
							_SSTREAM	errMsg;
							errMsg << _T("���ɁASub(") << i << _T(")�͒�`����Ă��܂��B");
							MML->Err(errMsg.str().c_str());
						}
						//�͈̓`�F�b�N
						_sub = new Sub(MML, i);
						ptcItem.push_back(_sub);
						ptcSub[i] = _sub;
					}
					break;
				case(id_BGM):
					{
						BGM*	_bgm;
						i = MML->GetNum();
						//�͈̓`�F�b�N
						MML->Chk_Range(_T("BGM()"), 0, Header.iBGM-1, i);
						//�d���`�F�b�N
						if(ptcBGM.count(i) != 0){
							_SSTREAM	errMsg;
							errMsg << _T("���ɁABGM(") << i << _T(")�͒�`����Ă��܂��B");
							MML->Err(errMsg.str().c_str());
						}
						_bgm = new BGM(MML, i);
						ptcItem.push_back(_bgm);
						ptcBGM[i] = _bgm;
						//���^�f�[�^�����������`�F�b�N
						if(_bgm->isLabel() == true){
							f_is_track_label = true;
						}
						if(_bgm->isAuthor() == true){
							f_is_track_auth	= true;
						}
						if(_bgm->isTime() == true){
							f_is_track_time	= true;
						}
						if(_bgm->isFade() == true){
							f_is_track_fade	= true;
						}
					}
					break;
				case(id_SE):
					{
						SE*	_se;
						i = MML->GetNum();
						//�͈̓`�F�b�N
						MML->Chk_Range(_T("SE()"), 0, Header.iSE-1, i);
						//�d���`�F�b�N
						if(ptcSE.count(i) != 0){
							_SSTREAM	errMsg;
							errMsg << _T("���ɁASE(") << i << _T(")�͒�`����Ă��܂��B");
							MML->Err(errMsg.str().c_str());
						}
						_se = new SE(MML, i);
						ptcItem.push_back(_se);
						ptcSE[i] = _se;
						//���^�f�[�^�����������`�F�b�N
						if(_se->isLabel() == true){
							f_is_track_label = true;
						}
						if(_se->isAuthor() == true){
							f_is_track_auth	= true;
						}
						if(_se->isTime() == true){
							f_is_track_time	= true;
						}
						if(_se->isFade() == true){
							f_is_track_fade	= true;
						}
					}
					break;
				case(id_Null):
					break;
				default:
					MML->ErrUnknownCmd();
					break;
			}
			
		} while( !MML->eom() );

		//==============================
		//Check
		if( Header.iBGM + Header.iSE > 255){
			MML->Err(_T("BGM��SE�̐������v��255���z���܂����B"));
		}

		for(size_t n=0, e=Header.iBGM; n<e; ++n){
			if(ptcBGM.count(n) == 0){
				_SSTREAM errMsg;
				errMsg << _T("BGM(") << n << _T(")�Ԃ����݂��܂���B");
				MML->Err(errMsg.str().c_str());
			};
		}

		for(size_t n=0, e=Header.iSE; n<e; ++n){
			if(ptcSE.count(n) == 0){
				_SSTREAM errMsg;
				errMsg << _T("SE(") << n << _T(")�Ԃ����݂��܂���B");
				MML->Err(errMsg.str().c_str());
			};
		}

	} catch (mml_error& e){
		f_error = true;
		e.out_what();
	} catch (mml_ios_failure& e){
		f_error = true;
		e.out_what();
	}

	if((f_error == true) || (MML->isError())){
		throw _T("MML�̍\����͂����s���܂����B");
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
MusicFile::~MusicFile(void)
{
}

//==============================================================
//		�g�p���Ȃ��I�u�W�F�N�g�̌������폜
//		���Ȃ���ATick���J�E���g
//		���A���^�f�[�^�������W
//--------------------------------------------------------------
//	������
//				����
//	���Ԓl
//				����
//==============================================================
void	MusicFile::TickCount(void)
{
	//==============================
	//Metadata

	Header.Set_text();	//text
	Header.Set_auth();	//auth
	Header.Set_NEND();	//NEND


	//==============================
	//Tick Count & �œK���̂��߂̏����W
	//�i�����͕��񉻂��Ȃ��ŁA���Ԃɏ������鎖�j

	for(size_t n=0, e=Header.iBGM; n<e; ++n){
		if(ptcBGM.count(n) == 1){
			BGM* _BGM = ptcBGM[n];
			_COUT << _T("---- BGM(") << n << _T(") ----") <<endl;
			_BGM->TickCount(this);				//�J�E���e�B���O���Ȃ���A�s�v�ȃR�}���h���������`�F�b�N
			if(f_is_track_label == true){
				Header.Set_tlbl(_BGM->getLabel());
			}
			if(f_is_track_auth == true){
				Header.Set_taut(_BGM->getAuthor());
			}
			if(f_is_track_time == true){
				Header.Set_time(_BGM->getTime());
			}
			if(f_is_track_fade == true){
				Header.Set_fade(_BGM->getFade());
			}
		}
	}

	for(size_t n=0, e=Header.iSE; n<e; ++n){
		if(ptcSE.count(n) == 1){
			SE*	_SE = ptcSE[n];
			_COUT << _T("---- SE(") << n << _T(") ----") <<endl;
			_SE->TickCount(this);				//�J�E���e�B���O���Ȃ���A�s�v�ȃR�}���h���������`�F�b�N
			if(f_is_track_label == true){
				Header.Set_tlbl(_SE->getLabel());
			}
			if(f_is_track_auth == true){
				Header.Set_taut(_SE->getAuthor());
			}
			if(f_is_track_time == true){
				Header.Set_time(_SE->getTime());
			}
			if(f_is_track_fade == true){
				Header.Set_fade(_SE->getFade());
			}
		}
	}

	//�G���[���������Ă�����œK���͂��Ȃ��B
	if (f_error == true) {
		throw _T("�œK�������s���܂����B");		
	}

	//==============================
	//�œK��
	//�i�J�E���g������́A���񉻂��ėǂ��j

	//----------------------
	//�s�v�ȃR�}���h�̍폜
	if (cOptionSW->flag_OptSeq == true) {		//�R�}���h�̍œK����������������A�œK�����Ȃ��B

		_OMP_PARALLEL
		{
			_OMP_FOR_NOWAIT
			for (int n = 0; n < Header.iBGM; ++n) {
				ptcBGM[n]->clear_Optimize();
			}

			_OMP_FOR_NOWAIT
			for (int n = 0; n < Header.iSE; ++n) {
				ptcSE[n]->clear_Optimize();
			}

			_OMP_SINGLE
			for (map<size_t, Sub*>::iterator it = ptcSub.begin(), e = ptcSub.end(); it != e; ++it) {
				it->second->clear_Optimize();
			}
		}
	}

	//----------------------
	//�g���Ă��Ȃ���`�̍폜
	if (cOptionSW->flag_OptObj == true) {		//��`�̍œK����������������A�œK�����Ȃ��B

		_OMP_PARALLEL_SECTIONS
		{
			//�G���x���[�v
			_OMP_SECTION
			{
				for (map<size_t, Envelop*>::iterator it = ptcEnv.begin(), e = ptcEnv.end(); it != e; ++it) {
					it->second->clear_Optimize();
				}
			}

			//FDSC
			_OMP_SECTION
			{
				for (map<size_t, FDSC*>::iterator it = ptcFDSC.begin(), e = ptcFDSC.end(); it != e; ++it) {
					it->second->clear_Optimize();
				}
			}

			//FDSM
			_OMP_SECTION
			{
				for (map<size_t, FDSM*>::iterator it = ptcFDSM.begin(), e = ptcFDSM.end(); it != e; ++it) {
					it->second->clear_Optimize();
				}
			}

			//VRC7
			_OMP_SECTION
			{
				for (map<size_t, VRC7*>::iterator it = ptcVRC7.begin(), e = ptcVRC7.end(); it != e; ++it) {
					it->second->clear_Optimize();
				}
			}

			//N163
			_OMP_SECTION
			{
				for (map<size_t, N163*>::iterator it = ptcN163.begin(), e = ptcN163.end(); it != e; ++it) {
					it->second->clear_Optimize();
				}
			}
		}
	}
}

//==============================================================
//		��PCM�̃I�t�Z�b�g�A�h���X���v�Z
//--------------------------------------------------------------
//	������
//		unsigned	int	iMusSize	�V�[�P���X�̃T�C�Y
//	���Ԓl
//		unsigned	int	��PCM�̍��v�T�C�Y
//==============================================================
size_t	MusicFile::SetDPCMOffset(size_t iMusSize)
{
				size_t	i;
	unsigned	char	mus_bank = (unsigned char)(iMusSize >> 12);

	if((iMusSize & 0x0FFF) != 0){
		mus_bank++;
	}

	dpcm_code.clear();
	if(cDPCMinfo != NULL){
		cDPCMinfo->getDPCMCode(&dpcm_code);
		i = cDPCMinfo->setDPCMoffset(Header.offsetPCM, mus_bank+3);
	} else {
		i = Header.offsetPCM;
	}

	return(i - Header.offsetPCM);
}

//==============================================================
//		�A�h���X�������肷��B
//--------------------------------------------------------------
//	������
//				����
//	���Ԓl
//				����
//	������
//				Fix_Address�́A�e�g���b�N�ŁA���񉻂���B
//==============================================================
void	MusicFile::Fix_Address(void)
{
	for (size_t n = 0; n < Header.iBGM; ++n) {
		ptcBGM[n]->Fix_Address(this);	//���̐�ŕ���[��]
	}

	for (size_t n = 0; n < Header.iSE; ++n) {
		ptcSE[n]->Fix_Address(this);	//���̐�ŕ���[��]
	}

	for (map<size_t, Sub*>::iterator it = ptcSub.begin(), e = ptcSub.end(); it != e; ++it) {
		it->second->Fix_Address(this);	//���̐�ŕ���[��]
	}

	if (f_error == true) {
		throw _T("�A�h���X�����Ɏ��s���܂����B");	
	}
}

//==============================================================
//		�a�h�m�̓ǂݍ���
//--------------------------------------------------------------
//	������
//		string* 		_str		�R�[�h������
//		NSF_Header*		nsf_hed		NSF�w�b�_�[�̓ǂݍ��ݐ�
//	���Ԓl
//		size_t						�ǂݍ���.bin�t�@�C���iNSF�w�b�_�[���������j
//==============================================================
std::streamsize	MusicFile::read_bin(string* _str, NSF_Header* nsf_hed)
{
	std::streamsize		bin_size	= 0;	//.bin�t�@�C���̃T�C�Y
	std::streamsize		code_size	= 0;	//�����A�h���C�o�[�i�R�[�h�j�̃T�C�Y
	FileInput*			_romcode	= new FileInput();	

	//----------------------
	//�t�@�C�����J��
	_romcode->fileopen(Header.romcode.c_str(), &(cOptionSW->m_pass_code));
	if(_romcode->isError() == true){
		f_error = true;	//_romcode�� new �����ςȂ��Ȃ̂ŁA�����ł� throw ���Ȃ��B

	} else {

		char*	nsf_code	= new char[0xC000];			//������.bin�t�@�C����ǂݍ���

		//----------------------
		//NSF�p�R�[�h�̓]��
		bin_size = _romcode->GetSize();
		code_size = bin_size - sizeof(NSF_Header);
		_romcode->read((char *)nsf_hed, sizeof(NSF_Header));
		_romcode->read(nsf_code, code_size);
		_romcode->close();

		//----------------------
		//NSF�w�b�_�[�̍X�V
		if(Header.title.size() < 32){
			Header.title.resize(32);
		}
		memcpy(&nsf_hed->Title, Header.title.c_str(), 32);

		if(Header.composer.size() < 32){
			Header.composer.resize(32);
		}
		memcpy(&nsf_hed->Composer, Header.composer.c_str(), 32);

		if(Header.copyright.size() < 32){
			Header.copyright.resize(32);
		}
		memcpy(&nsf_hed->Copyright, Header.copyright.c_str(), 32);

		nsf_hed->Version		= (unsigned char)(cOptionSW->iNSF_version);
		nsf_hed->MusicNumber	= (unsigned char)((Header.iBGM + Header.iSE) & 0xFF);
		if(Header.iExternal != -1){
			nsf_hed->External	= (unsigned char)Header.iExternal;
		}

		//----------------------
		//Exit
		_str->append(nsf_code, code_size);

		delete[]	nsf_code;

	}
	//----------------------
	//Exit
	delete		_romcode;

	return(code_size);

}

//==============================================================
//		�V�[�P���X�f�[�^�̍쐬
//--------------------------------------------------------------
//	������
//		string* _str		�R�[�h������
//		size_t	code_size	*.bin�̃T�C�Y�iNSF�w�b�_�[���������j
//		size_t	ptOffset	ROM�̃I�t�Z�b�g�A�h���X
//								bank��Ή��̏ꍇ�́A0x8000
//								bank�@�Ή��̏ꍇ�́A0x0000
//	���Ԓl
//		size_t				�V�[�P���X�f�[�^�̃T�C�Y
//	������
//		NSF�ANSFe�p�̋ȃf�[�^�\�������B
//		Addr	Type	Contents
//		-------+-------+--------------------------
//		0000	BYTE	BGM �f�[�^��
//		0001	BYTE	SE  �f�[�^��
//		0002	WORD	nsd_dpcm�\���̂̃A�h���X
//		0004	WORD[]	BGM, SE�̃A�h���X�A
//		xxxx	----	BGM, SE�̃V�[�P���X�f�[�^
//==============================================================
size_t	MusicFile::make_mus(string* _str, size_t code_size, size_t ptOffset)
{
	unsigned	int			i		= 2;
	unsigned	short*		pt;

				size_t		_size	= 4 + (Header.iBGM + Header.iSE)*2;
				size_t		size0	= _str->size();		//�����T�C�Y

	//----------------------
	//�o�C�i�����N���A�B
	code.clear();
	//�̈�̊m��
	code.resize(_size);

	//�ȃA�h���X���iNSF, NSEe�p�j�̍쐬
	pt = (unsigned short*)code.c_str();

	code[0] = (char)(Header.iBGM & 0xFF);
	code[1] = (char)(Header.iSE  & 0xFF);

	if(Header.bank == false){

		if(cDPCMinfo != NULL){
			pt[1]	= (unsigned short)(ptOffset + code_size + _size + cDPCMinfo->getOffset());	//��PCM info �̃A�h���X
		} else {
			pt[1]	= 0;
		}

		for(size_t n=0; n<Header.iBGM; n++){
			pt[i] = (unsigned short)(ptOffset + code_size + _size + ptcBGM[n]->getOffset());
			i++;
		}

		for(size_t n=0; n<Header.iSE; n++){
			pt[i] = (unsigned short)(ptOffset + code_size + _size + ptcSE[n]->getOffset());
			i++;
		}

	} else {

		if(cDPCMinfo != NULL){
			pt[1] = (unsigned short)(ptOffset + _size + cDPCMinfo->getOffset());	//��PCM info �̃A�h���X
		} else {
			pt[1]	= 0;
		}

		for(size_t n=0; n<Header.iBGM; n++){
			pt[i] = (unsigned short)(ptOffset + _size + ptcBGM[n]->getOffset());
			i++;
		}

		for(size_t n=0; n<Header.iSE; n++){
			pt[i] = (unsigned short)(ptOffset + _size + ptcSE[n]->getOffset());
			i++;
		}

	}

	getCode(_str);

	return(_str->size() - size0);

}

//==============================================================
//		���o�b�l�̍쐬
//--------------------------------------------------------------
//	������
//		string* _str		�R�[�h������
//	���Ԓl
//		size_t				��PCM�f�[�^�̃T�C�Y
//	������
//
//==============================================================
size_t	MusicFile::make_dpcm(string* _str)
{
	//��PCM�̍쐬
	_str->append(dpcm_code);

	return(dpcm_code.size());
}

//==============================================================
//		�m�r�e�̍쐬
//--------------------------------------------------------------
//	������
//		NSF_Header*		NSF_Hed		�w�b�_�[��
//		string*			NSF_Data	�f�[�^��
//	���Ԓl
//		size_t						�f�[�^���̃T�C�Y
//	������
//
//--------------------------------------------------------------
size_t	MusicFile::make_bin(NSF_Header* NSF_Hed, string* NSF_Data)
{

	unsigned	int		i,j;

				size_t	iSizeLimit;		//�T�C�Y�`�F�b�N�p

				size_t	code_size;
				size_t	mus_size;
				size_t	pcm_size;
				size_t	NSF_size;
	unsigned	char	code_bank = 3;	//�o���N�Ή�.bin�̃o���N��
	unsigned	char	mus_bank;
	unsigned	char	pcm_bank;

				char	chk_Bank;		//NSF�w�b�_�[�̃o���N�`�F�b�N�p

				bool	flag_Optimize	= cOptionSW->flag_Optimize;

	//----------------------
	//BIN�t�@�C���̓ǂ݂���
	code_size = read_bin(NSF_Data, NSF_Hed);
	if(isError() == true){
		//BIN�t�@�C���̓ǂݍ��݂Ɏ��s���Ă�����I������
		throw ios_failure(Header.romcode, errno);

	} else {

		//----------------------
		//Bin���A�o���N�Ή����H
		//chk_Bank == 0	�E�E�E�@��Ή�bin
		//chk_Bank != 0	�E�E�E�@�@�Ή�bin
		chk_Bank  = NSF_Hed->Bank[0];
		chk_Bank |= NSF_Hed->Bank[1];
		chk_Bank |= NSF_Hed->Bank[2];
		chk_Bank |= NSF_Hed->Bank[3];
		chk_Bank |= NSF_Hed->Bank[4];
		chk_Bank |= NSF_Hed->Bank[5];
		chk_Bank |= NSF_Hed->Bank[6];
		chk_Bank |= NSF_Hed->Bank[7];

		//----------------------
		//�V�[�P���X�̍쐬
		if(chk_Bank == 0){

			//------------------------------
			//Bank ��Ή�bin
			if(Header.bank == true){
				throw mml_error(_T("�w���.bin�t�@�C���́A��PCM�̃o���N�ɑΉ����Ă��܂���B\n��PCM�̃o���N�ɑΉ�����.bin�t�@�C�����w�肵�Ă��������B"));
			}

			//�T�C�Y�̏��
			iSizeLimit = Header.offsetPCM - 0x8000;

			//�V�[�P���X�̃o�C�i���𐶐�
			mus_size = make_mus(NSF_Data, code_size, 0x8000);
			mus_size += code_size;						//.bin �� �ȃI�u�W�F�N�g�𑫂��Z

			//�o��
			_COUT << _T("[CODE & MUSIC]") << endl;
		} else {

			//------------------------------
			//Bank�Ή�bin�H
			if(Header.bank == false){
				throw mml_error(_T("�w���.bin�t�@�C���́A��PCM�̃o���N�ɑΉ����Ă��܂��B\n#Bank�R�}���h���w�肵�Ă��������B"));
			}
			if(cOptionSW->iNSF_version >=2){
				NSF_Hed->Flags |= nsf_flag_IRQ_support;
			}

			//�T�C�Y�̏��
			iSizeLimit = 0x10000;

			//�V�[�P���X�̃o�C�i���𐶐�
			mus_size = make_mus(NSF_Data, code_size, 0x0000);

			//�o��
			_COUT << _T("[CODE]") << endl;
			_COUT << _T("  Bank = 3") << endl;
			_COUT << _T("  Size = 12288 [Byte]") << endl;

			_COUT << _T("[MUSIC]") << endl;
		}

		//�o���N���̌v�Z
		mus_bank = (unsigned char)(mus_size >> 12);
		if((mus_size & 0x0FFF) != 0){
			mus_bank++;
		}

		_COUT << _T("  Bank = ") << (unsigned int)mus_bank << endl;
		_COUT << _T("  Size = ") << (unsigned int)mus_size << _T(" [Byte] / ") << iSizeLimit << _T(" [Byte]") << endl;

		//�T�C�Y�`�F�b�N
		if(mus_size > iSizeLimit){
			throw mml_error(_T("�R�[�h�E�V�[�P���X�̃T�C�Y�����e�l���z���܂����B"));
		}

		//----------------------
		//GAP�̐���
		if(cDPCMinfo != NULL){
			if(chk_Bank == 0){
				//0��padding����B
				if(flag_Optimize == true){
					//�V�[�P���X�f�[�^�̃o���N�̈�
					//��PCM�̃o���N���̊J�n�I�t�Z�b�g�܂�
					NSF_Data->resize(((size_t)mus_bank<<12) + (Header.offsetPCM & 0x0FFF));
				} else {
					//��PCM�̊J�n�ʒu�܂�
					NSF_Data->resize(Header.offsetPCM - 0x8000);
				}
			} else {
				//�V�[�P���X�f�[�^�̃o���N�̈�
				//��PCM�̊J�n�ʒu�܂�
				NSF_Data->resize((((size_t)mus_bank + (size_t)code_bank)<<12) + (Header.offsetPCM - 0xC000));
			}
		}

		//----------------------
		//��PCM
		_COUT << _T("[DPCM]") << endl;

		pcm_size = make_dpcm(NSF_Data);
		pcm_bank = (unsigned char)((pcm_size + (Header.offsetPCM & 0x0FFF)) >> 12);
		if((pcm_size & 0x0FFF) != 0){
			pcm_bank++;
		}

		//��PCM�T�C�Y�`�F�b�N
		_COUT << _T("  Bank = ") << (size_t)pcm_bank << endl;
		_COUT << _T("  Size = ") << (size_t)pcm_size << _T(" [Byte]");
		if(chk_Bank == 0){
			_COUT << _T(" / ") << 0x10000 - Header.offsetPCM << _T(" [Byte]") << endl;

			if(	(Header.offsetPCM + pcm_size) > 0x10000	){
				throw mml_error(_T("��PCM�̃T�C�Y�����e�l���z���܂����B"));
			}
		} else {
			_COUT << endl;
			i = mus_bank + pcm_bank + code_bank;

			if(i > 255){
				throw mml_error(_T("�o���N���̍��v��255���z���܂����B"));
			}
		}

		//��PCM���L�� && �o���N�\���̍œK�����L�� && bank��Ή���.bin���g���ꍇ�A
		//NSF�w�b�_�[�ɁA�o���N������������
		if((chk_Bank == 0) && (cDPCMinfo != NULL) && (flag_Optimize == true)){
			i = 0;
			while(i < mus_bank){
				NSF_Hed->Bank[i] = (unsigned char)i;
				i++;
			}
			while(i < ((Header.offsetPCM - 0x8000)>>12)){
				NSF_Hed->Bank[i] = 0;
				i++;
			}
			j = 0;
			while(i < 8){
				if(j < pcm_bank){
					NSF_Hed->Bank[i] = mus_bank + (unsigned char)j;
				} else {
					NSF_Hed->Bank[i] = 0;
				}
				i++;
				j++;
			}
		}

		//----------------------
		//GAP�̐���
		if(chk_Bank == 0){
			NSF_size = NSF_Data->size();
			if((cDPCMinfo != NULL) && (flag_Optimize == true)){
				//�o���N����0��padding����B
				if((NSF_size & 0x0FFF) != 0){
					NSF_size = (NSF_size & 0xF000) + 0x1000;
					NSF_Data->resize(NSF_size);
				}
			}
		} else {
			//�o���N����0��padding����B
			NSF_size = ((size_t)mus_bank + (size_t)pcm_bank + (size_t)code_bank)<<12;
			//BANK�Ή���.bin���g���ꍇ�ŁA���A32kByte�����̏ꍇ�A32kByte�ɂ���B
			if(NSF_size < 0x8000){
				NSF_size = 0x8000;
			}
			NSF_Data->resize(NSF_size);
		}

		//----------------------
		//NSF2�̏ꍇ�A�T�C�Y���w�b�_�[�ɏ���
		if(cOptionSW->iNSF_version >=2){
			NSF_Hed->szNSF_Data[0] = (char)( NSF_size      & 0xFF);
			NSF_Hed->szNSF_Data[1] = (char)((NSF_size>> 8) & 0xFF);
			NSF_Hed->szNSF_Data[2] = (char)((NSF_size>>16) & 0xFF);
		}
	}

	//----------------------
	//Exit
	return(NSF_size);

}

//==============================================================
//		�m�r�e�`���ւ̕ۑ�
//--------------------------------------------------------------
//	������
//		const	char*	strFileName		�t�@�C����
//	���Ԓl
//				����
//==============================================================
void	MusicFile::saveNSF(string&	strFileName)
{
	NSF_Header*			NSF_Hed			= new NSF_Header;
				string	NSF_Data;
				size_t	NSF_size;

				string	Meta_data;
				size_t	Meta_size;

	try{

		_COUT << _T("----------------------------------------") << endl;
		_COUT << _T("*NSF build process") << endl;

		//==============================
		//----------------------
		//�N���A
		NSF_Data.clear();
		Meta_data.clear();

		//----------------------
		//NSF�́A�w�b�_�[�ƃf�[�^���̍쐬
		NSF_size = make_bin(NSF_Hed, &NSF_Data);

		//==============================
		//Meta Data
		Meta_size = Header.getData(&Meta_data);

		//NEND�����ł���΁Ametadata�̖��ߍ��݂͕s�v
		if(Meta_size <= 8){
			Meta_size = 0;
		}

		//�T�C�Y�\��
		_COUT << _T("[Meta Data]") << endl;
		_COUT << _T("  Size = ") << (unsigned int)Meta_size << _T(" [Byte]") << endl;

		if(cOptionSW->iNSF_version >=2){
			//Meta Data������ꍇ
			if(Meta_size > 0){
				NSF_Hed->Flags |= nsf_flag_MetaData;
			}
		}

		//==============================
		//NSF��������
		//----------------------
		//Open File
		fileopen(strFileName.c_str());
		if(isError() == true){
			throw ios_failure(strFileName, errno);
		} else {
			//----------------------
			//Write File
			write((char *)NSF_Hed, sizeof(NSF_Header));			//NSF�w�b�_�[�̏�������
			write(NSF_Data.c_str(), NSF_size);					//�f�[�^�̏�������
			if((cOptionSW->iNSF_version>=2) && (Meta_size > 0)){
				write(Meta_data.c_str(), Meta_size);			//Meta data�̏�������
			}

			//----------------------
			//Close file
			close();
		}

	} catch (mml_error& e){
		e.out_what();
	} catch (ios_failure& e) {
		nsc_ErrMsg(e);
	}

	//==============================
	//Exit
	delete	NSF_Hed;
}

//==============================================================
//		�m�r�e���`���ւ̕ۑ�
//--------------------------------------------------------------
//	������
//		const	char*	strFileName		�t�@�C����
//	���Ԓl
//				����
//==============================================================
void	MusicFile::saveNSFe(string&	strFileName)
{

	const		char	NSFE_Head[4]	= {0x4E, 0x53, 0x46, 0x45};		//NSFE

	NSF_Header*			NSF_Hed			= new NSF_Header;
				string	NSF_Data;

				string	Meta_data;
				size_t	Meta_size;

	try{

		_COUT << _T("----------------------------------------") << endl;
		_COUT << _T("*NSFe build process") << endl;

		//==============================
		//----------------------
		//�N���A
		NSF_Data.clear();
		Meta_data.clear();

		//----------------------
		//NSF�́A�w�b�_�[�ƃf�[�^���̍쐬
		make_bin(NSF_Hed, &NSF_Data);		//�o���N�\���̍œK���͂����ł���Ă���B

		//==============================
		//���^�f�[�^�̐���
		//----------------------
		//DATA
		Header.Set_DATA(&NSF_Data);

		//----------------------
		//NSFe�p��metadeta(INFO, NSF2, BANK)
		Header.Set_NSFe_footer(NSF_Hed);

		//----------------------
		//�o�C�i���ɕϊ�
		Meta_size = Header.getData(&Meta_data);

		//�T�C�Y�\��
		_COUT << _T("[Meta Data]") << endl;
		_COUT << _T("  Size = ") << (unsigned int)Meta_size << _T(" [Byte]") << endl;

		//==============================
		//NSFE��������
		//----------------------
		//Open File
		fileopen(strFileName.c_str());
		if(isError() == true){
			throw ios_failure(strFileName, errno);
		} else {
			//----------------------
			//Write File
			write(NSFE_Head, sizeof(NSFE_Head));			//NSF�w�b�_�[�̏�������
			write(Meta_data.c_str(), Meta_size);			//Meta data�̏�������

			//----------------------
			//Close file
			close();
		}

	} catch (mml_error& e){
		e.out_what();
	} catch (ios_failure& e) {
		nsc_ErrMsg(e);
	}

	//==============================
	//Exit
	delete	NSF_Hed;

}

//==============================================================
//		�A�Z���u������\�[�X�ւ̕ۑ�
//--------------------------------------------------------------
//	������
//		const	char*	strFileName		�t�@�C����
//	���Ԓl
//				����
//==============================================================
void	MusicFile::saveASM(string&	strFileName)
{
	try{

		//----------------------
		//File open
		fileopen(strFileName.c_str());
		if(isError() == true){
			throw ios_failure(strFileName, errno);
		} else {

			//Header
			*this <<	";===============================================================\n"
						";		Music file for NES Sound Driver & Library\n"
						";			for assembly language (ca65.exe)\n"
						";===============================================================\n"
						<<endl;

			//Export of Sequence
			for(size_t n=0; n<Header.iBGM; n++){
				*this	<<	"	.export		"	<<	Header.Label	<<	"BGM"	<<	n	<<	endl;
			}

			for(size_t n=0; n<Header.iSE; n++){
				*this	<<	"	.export		"	<<	Header.Label	<<	"SE"	<<	n	<<	endl;
			}

			if(cDPCMinfo != NULL){
				*this	<<	"	.export		"	<<	Header.Label	<<	"DPCMinfo"	<<	endl;
			}

			//MML
			*this <<	"\n\n.segment	"	<<	'"'	<<	Header.segmentSEQ	<<	'"' << endl;

			getAsm(this);

			//----------------------
			//Close file
			close();
		}

	} catch (mml_error& e){
		e.out_what();
	} catch (ios_failure& e) {
		nsc_ErrMsg(e);
	}
}

//==============================================================
//		�G���[����
//--------------------------------------------------------------
//	������
//		const	_CHAR	msg[]	�G���[���b�Z�[�W
//	���Ԓl
//				����
//==============================================================
/*
void	MusicFile::Err(const _CHAR msg[])
{
	_OMP_SET_LOCK(lock_cout)
	f_error = true;
	if(cOptionSW->fErr == true){
		_CERR << _T("[ ERROR ] : ") << msg << endl;
	} else {
		_COUT << _T("[ ERROR ] : ") << msg << endl;
	}
	_OMP_UNSET_LOCK(lock_cout)

	throw EXIT_FAILURE;		//��{�I�ɒv���I�ȃG���[�Ȃ̂ŗ�O�𓊂���B
}
*/

//--------------------------------------------------------------
void	MusicFile::Err(const _CHAR msg[], size_t no)
{
	_OMP_SET_LOCK(lock_cout)
	f_error = true;
	if(cOptionSW->fErr == true){
		_CERR << _T("[ ERROR ] : ") << msg << _T("(") << no << _T(")�Ԃ����݂��܂���B") << endl;
	} else {
		_COUT << _T("[ ERROR ] : ") << msg << _T("(") << no << _T(")�Ԃ����݂��܂���B") << endl;
	}
	_OMP_UNSET_LOCK(lock_cout)
}

//==============================================================
//		���[�j���O����
//--------------------------------------------------------------
//	������
//		const	_CHAR	msg[]	���[�j���O���b�Z�[�W
//	���Ԓl
//				����
//==============================================================
void	MusicFile::Warning(const _CHAR msg[])
{
	_OMP_SET_LOCK(lock_cout)
	//���݂̃t�@�C�����ƁA�s����\��
	if(cOptionSW->fErr == true){
		_CERR << _T("[WARNING] : ") << msg << endl;
	} else {
		_COUT << _T("[WARNING] : ") << msg << endl;
	}
	_OMP_UNSET_LOCK(lock_cout)
}
