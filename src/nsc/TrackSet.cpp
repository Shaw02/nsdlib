/*******************************************************************************

			NES Sound Driver & Library	(NSD.lib)	MML Compiler

	Copyright (c) 2012 A.Watanabe (S.W.), All rights reserved.
	 For conditions of distribution and use, see copyright notice in "nsc.cpp".

*******************************************************************************/

#include "StdAfx.h"
#include "TrackSet.h"

//==============================================================
//		コンストラクタ
//--------------------------------------------------------------
//	●引数
//		MMLfile*			MML			MMLファイルのオブジェクト
//		unsigned	int		_id			BGM番号
//		bool				_sub		このオブジェクトは、サブルーチン？
//		const		_CHAR	_strName[]	オブジェクト名
//	●返値
//					無し
//==============================================================
TrackSet::TrackSet(MMLfile* MML, unsigned int _id, bool _sub, bool _se, const _CHAR _strName[]):
	MusicItem(_id, _strName),
	iTempo(120)
{
	//----------------------
	//Local変数

//	定数定義
enum	Command_ID_mml {
	mml_Track,
	mml_Scale,
	mml_Major,
	mml_Minor,
	mml_HMinor,
	mml_MMinor,
	mml_KeySignature,
	mml_Macro,
	mml_MacroSet,
	mml_CallSE,
	mml_Subroutine,

	mml_Loop,
	mml_Repeat_A_Start,
	mml_Repeat_A_Branch,
	mml_Repeat_A_End,
	mml_Repeat_B_Start,
	mml_Repeat_B_Branch,
	mml_Repeat_B_End,
	mml_Repeat_C_Start,
//	mml_Repeat_C_Branch,
	mml_Repeat_C_End,

	mml_Tempo,
	mml_Tempo_Relative,
	mml_Tempo_Up,
	mml_Tempo_Down,

	mml_La,
	mml_Si,
	mml_Do,
	mml_Re,
	mml_Mi,
	mml_Fa,
	mml_Sol,
	mml_Rest,
	mml_Wait,
	mml_Tai,

	mml_Length,
	mml_Gate_Q,
	mml_Gate_q,
	mml_Gate_u,

	mml_Echo,
	mml_Echo_Off,
	mml_Echo_Reset,
	mml_Echo_C,
	mml_Echo_D,
	mml_Echo_E,
	mml_Echo_F,
	mml_Echo_G,
	mml_Echo_A,
	mml_Echo_B,
	mml_Echo_R,

	mml_Jump,
	mml_Jump_drv,

	mml_Envelop_Voice,
	mml_Envelop_Volume,
	mml_Envelop_Frequency,
	mml_Envelop_Note,

	mml_Envelop_Off_Voice,
	mml_Envelop_Off_Volume,
	mml_Envelop_Off_Frequency,
	mml_Envelop_Off_Note,

	mml_Patch,
	mml_Patch_Off,

	mml_Release_mdoe,
	mml_Release_Voice,
	mml_Release_Volume,

	mml_Voice,
	mml_FDSC,
	mml_FDSM,
	mml_FDSF,
	mml_FDSV,
	mml_VRC7,
	mml_N163,
	mml_N163_Set,
	mml_N163_Load,
	mml_N163_Channel,
	mml_FME7_frequency,

	mml_Octave,
	mml_Octave_Up,
	mml_Octave_Down,
	mml_Octave_Up1,
	mml_Octave_Down1,
	mml_Detune_Cent,
	mml_Detune_Cent_Relative,
	mml_Detune_Register,
	mml_Detune_Register_Relative,
	mml_Transpose,
	mml_Transpose_Relative,
	mml_KeyShift,
	mml_KeyShift_Relative,
	mml_Protament,
	mml_Protament2,
	mml_Sweep,

	mml_Volume,
	mml_Volume_Up,
	mml_Volume_Down,

	mml_VRC7_Write,
	mml_Memory_Write,

	mml_Priority,

	mml_Bar
};

//	これらは、MML構文で使えるコマンド。

const	static	unsigned	char	RS_UTF8[]	=	{0x5c, 0x00};				//REVERSE SOLIDUS
const	static	unsigned	char	RS_UTF8W[]	=	{0xEF, 0xBC, 0xBC, 0x00};	//REVERSE SOLIDUS
const	static	unsigned	char	Yen_UTF8[]	=	{0xC2, 0xA5, 0x00};			//Yen
const	static	unsigned	char	Yen_UTF8W[]	=	{0xEF, 0xBF, 0xA5, 0x00};	//Yen

const	static	Command_Info	Command[] = {
		{	"TR",		mml_Track				},
		{	"トラック",	mml_Track				},
		{	"Scale",	mml_Scale				},
		{	"scale",	mml_Scale				},
		{	"Mode",		mml_Scale				},
		{	"mode",		mml_Scale				},
		{	"Major",	mml_Major				},
		{	"major",	mml_Major				},
		{	"Minor",	mml_Minor				},
		{	"minor",	mml_Minor				},
		{	"HMinor",	mml_HMinor				},
		{	"hminor",	mml_HMinor				},
		{	"MMinor",	mml_MMinor				},
		{	"mminor",	mml_MMinor				},

		{	"K",		mml_KeySignature		},
		{	"調",		mml_KeySignature		},
		{	"SE",		mml_CallSE				},
		{	"S",		mml_Subroutine			},
		{	"$$",		mml_MacroSet			},
		{	"$",		mml_Macro				},

		{	"L",		mml_Loop				},
		{	"[:",		mml_Repeat_C_Start		},
	//	{	":",		mml_Repeat_C_Branch		},
		{	":]",		mml_Repeat_C_End		},
		{	"|:",		mml_Repeat_B_Start		},
		{	(char*)RS_UTF8,		mml_Repeat_B_Branch		},
		{	(char*)Yen_UTF8,	mml_Repeat_B_Branch		},
		{	(char*)RS_UTF8W,	mml_Repeat_B_Branch		},
		{	(char*)Yen_UTF8W,	mml_Repeat_B_Branch		},
		{	":|",		mml_Repeat_B_End		},
		{	"[",		mml_Repeat_A_Start		},
		{	":",		mml_Repeat_A_Branch		},
		{	"]",		mml_Repeat_A_End		},

		{	"∞",		mml_Loop				},
		{	"｜：",		mml_Repeat_B_Start		},
		{	"￥",		mml_Repeat_B_Branch		},
		{	"＼",		mml_Repeat_B_Branch		},
		{	"：｜",		mml_Repeat_B_End		},
		{	"【",		mml_Repeat_A_Start		},
		{	"：",		mml_Repeat_A_Branch		},
		{	"】",		mml_Repeat_A_End		},

		{	"t_",		mml_Tempo_Relative,		},
		{	"t",		mml_Tempo,				},
		{	"テンポ",	mml_Tempo,				},
		{	"→",		mml_Tempo_Up,			},
		{	"←",		mml_Tempo_Down,			},

		{	"a",	mml_La					},
		{	"b",	mml_Si					},
		{	"c",	mml_Do					},
		{	"d",	mml_Re					},
		{	"e",	mml_Mi					},
		{	"f",	mml_Fa					},
		{	"g",	mml_Sol					},
		{	"r",	mml_Rest				},
		{	"w",	mml_Wait				},
		{	"^",	mml_Tai					},

		{	"ら",	mml_La					},
		{	"し",	mml_Si					},
		{	"ど",	mml_Do					},
		{	"れ",	mml_Re					},
		{	"み",	mml_Mi					},
		{	"ふぁ",	mml_Fa					},
		{	"ふ",	mml_Fa					},
		{	"そ",	mml_Sol					},
		{	"ん",	mml_Rest				},
		{	"っ",	mml_Rest				},
		{	"ー",	mml_Tai					},

		{	"ラ",	mml_La					},
		{	"シ",	mml_Si					},
		{	"ド",	mml_Do					},
		{	"レ",	mml_Re					},
		{	"ミ",	mml_Mi					},
		{	"ファ",	mml_Fa					},
		{	"フ",	mml_Fa					},
		{	"ソ",	mml_Sol					},
		{	"ン",	mml_Rest				},
		{	"ッ",	mml_Rest				},
		{	"－",	mml_Tai					},

		{	"イ",	mml_La					},
		{	"ロ",	mml_Si					},
		{	"ハ",	mml_Do					},
		{	"ニ",	mml_Re					},
		{	"ホ",	mml_Mi					},
		{	"ヘ",	mml_Fa					},
		{	"ト",	mml_Sol					},

		{	"l",		mml_Length				},
		{	"音符",		mml_Length				},
		{	"Q",		mml_Gate_Q				},
		{	"q",		mml_Gate_q				},
		{	"ゲート",	mml_Gate_q				},
		{	"u",		mml_Gate_u				},

		{	"ECc",			mml_Echo_C				},
		{	"ECd",			mml_Echo_D				},
		{	"ECe",			mml_Echo_E				},
		{	"ECf",			mml_Echo_F				},
		{	"ECg",			mml_Echo_G				},
		{	"ECa",			mml_Echo_A				},
		{	"ECb",			mml_Echo_B				},
		{	"ECr",			mml_Echo_R				},
		{	"EC-",			mml_Echo_Reset			},
		{	"EC*",			mml_Echo_Off			},
		{	"EC",			mml_Echo				},
		{	"エコーオフ",	mml_Echo_Off			},
		{	"エコー",		mml_Echo				},

		{	"J",			mml_Jump				},
		{	"j",			mml_Jump_drv			},

		{	"E@*",	mml_Envelop_Off_Voice		},
		{	"Ev*",	mml_Envelop_Off_Volume		},
		{	"Em*",	mml_Envelop_Off_Frequency	},
		{	"En*",	mml_Envelop_Off_Note		},

		{	"E@",	mml_Envelop_Voice		},
		{	"Ev",	mml_Envelop_Volume		},
		{	"Em",	mml_Envelop_Frequency	},
		{	"En",	mml_Envelop_Note		},

		{	"エンベロープオフ",	mml_Envelop_Off_Volume		},
		{	"エンベロープ",		mml_Envelop_Volume			},
		{	"ビブラートオフ",	mml_Envelop_Off_Frequency	},
		{	"ビブラート",		mml_Envelop_Frequency		},

		{	"Rm",	mml_Release_mdoe		},
		{	"R@",	mml_Release_Voice		},
		{	"Rv",	mml_Release_Volume		},

		{	"@P*",	mml_Patch_Off			},
		{	"@P",	mml_Patch				},

		{	"@FC",	mml_FDSC				},
		{	"@FM",	mml_FDSM				},
		{	"@FF",	mml_FDSF				},
		{	"@FV",	mml_FDSV				},
		{	"@V",	mml_VRC7				},
		{	"@NS",	mml_N163_Set			},
		{	"@NL",	mml_N163_Load			},
		{	"@N",	mml_N163				},
		{	"@",	mml_Voice				},
		{	"音色",	mml_Voice				},
		{	"NC",	mml_N163_Channel		},
		{	"F",	mml_FME7_frequency		},

		{	"o",	mml_Octave				},
		{	"音階",	mml_Octave				},
		{	">",	mml_Octave_Up			},
		{	"上",	mml_Octave_Up			},
		{	"↑",	mml_Octave_Up			},
		{	"<",	mml_Octave_Down			},
		{	"下",	mml_Octave_Down			},
		{	"↓",	mml_Octave_Down			},
		{	"`",	mml_Octave_Up1			},
		{	"‘",	mml_Octave_Up1			},
		{	"\"",	mml_Octave_Down1		},
		{	"”",	mml_Octave_Down1		},

		{	"D%_",			mml_Detune_Register_Relative		},
		{	"D%",			mml_Detune_Register					},
		{	"D_",			mml_Detune_Cent_Relative			},
		{	"D",			mml_Detune_Cent						},
		{	"ディチューン",	mml_Detune_Cent			},
		{	"__",			mml_Transpose_Relative	},
		{	"_",			mml_Transpose			},
		{	"k_",			mml_KeyShift_Relative	},
		{	"k",			mml_KeyShift			},
		{	"TrackKey",		mml_Transpose			},
		{	"P",			mml_Protament			},
		{	"ポルタメント",	mml_Protament			},
		{	"{",			mml_Protament2			},
		{	"s",			mml_Sweep				},

		{	"v",		mml_Volume				},
		{	"音量",		mml_Volume				},
		{	")",		mml_Volume_Up			},
		{	"大きく",	mml_Volume_Up			},
		{	"大",		mml_Volume_Up			},
		{	"(",		mml_Volume_Down			},
		{	"小さく",	mml_Volume_Down			},
		{	"小",		mml_Volume_Down			},

		{	"yV",	mml_VRC7_Write			},
		{	"y",	mml_Memory_Write		},

		{	"#Priority",	mml_Priority	},
		{	"#priority",	mml_Priority	},

		{	"　",	mml_Bar					},
		{	"|",	mml_Bar					},
		{	"｜",	mml_Bar					}
};

	unsigned	char	cData;
				int		i;

	//------------------------------
	//クラスの初期設定
	fSub		= _sub;		//サブルーチンのフラグ
	fSE			= _se;		//
	iTrack		= 0;		//コンパイル中のトラック（Default = 0）
	maxTrack	= 0;		//最大トラック番号

	if(fSE == true){
		Priority	= MML->priority;
	} else {
		Priority	= 0;
	}

	//まずは、１つだけトラック（0番）のオブジェクトを作る。
	nowTrack	= makeTrack(MML, iTrack);
	nowTrack->SetCompileFlag(true);


	//------------------------------
	//コンパイル

	// { の検索
	while(MML->cRead() != '{'){
		if(MML->eof()){
			MML->Err(_T("ブロックの開始を示す{が見つかりません。"));
		}
	}

	// } が来るまで、記述ブロック内をコンパイルする。
	while(1){

		cData = MML->GetChar();
		if(cData == '}'){
			TrackChk(MML);
			if(nowTrack == NULL){
				break;
			}
		} else {
			// } が来る前に、[EOF]が来たらエラー
			if( MML->eof() ){
				MML->Err(_T("ブロックの終端を示す`}'がありません。"));
			}

			//１つ戻る
			MML->Back();
		}

		//各コマンド毎の処理
		switch(MML->GetCommandID(Command, sizeof(Command)/sizeof(Command_Info))){

			case(mml_Track):
				if(fSub == true){
					MML->Warning(_T("Subブロック内ではトラック指定はできません。無視します。"));
				} else {
					TrackProc(MML);
				}
				break;

			case(mml_Scale):
				nowTrack->SetScale(MML);
				break;

			case(mml_Major):
				nowTrack->SetMajor();
				break;

			case(mml_Minor):
				nowTrack->SetMinor();
				break;

			case(mml_HMinor):
				nowTrack->SetHMinor(MML);
				break;

			case(mml_MMinor):
				nowTrack->SetMMinor(MML);
				break;

			case(mml_KeySignature):
				nowTrack->SetKeySignature(MML);
				break;

			case(mml_Macro):
				MML->CallMacro();
				break;

			case(mml_MacroSet):
				MML->SetMacro(1);
				break;

			case(mml_CallSE):
				nowTrack->SetSE(MML);
				break;

			case(mml_Subroutine):
				nowTrack->SetSubroutine(MML->GetInt());
				break;

			case(mml_Loop):
				if(fSub == true){
					MML->Warning(_T("Subブロック内では無限ループはできません。無視します。"));
				} else {
					nowTrack->SetLoop(MML);
				}
				break;

			case(mml_Repeat_B_Start):
				nowTrack->SetRepeat_B_Start();
				break;

			case(mml_Repeat_B_Branch):
				nowTrack->SetRepeat_B_Branch(MML);
				break;

			case(mml_Repeat_B_End):
				nowTrack->SetRepeat_B_End(MML);
				break;

			case(mml_Repeat_A_Branch):
				nowTrack->SetRepeat_Branch(MML);
				break;

			case(mml_Repeat_A_Start):
				if((MML->iRepeatMode == 0) && (fSub == false)){
					nowTrack->SetRepeat_Start(MML);		//0 && main
				} else if(MML->iRepeatMode == 1){
					nowTrack->SetRepeat_A_Start(MML);	//1
				} else {
					nowTrack->SetRepeat_C_Start(MML);	//2 || (0 && sub)
				}
				break;

			case(mml_Repeat_A_End):
				if((MML->iRepeatMode == 0) && (fSub == false)){
					nowTrack->SetRepeat_End(MML);		//0 && main
				} else if(MML->iRepeatMode == 1){
					nowTrack->SetRepeat_A_End(MML);	//1
				} else {
					nowTrack->SetRepeat_C_End(MML);	//2 || (0 && sub)
				}
				break;

			case(mml_Repeat_C_Start):
				if((MML->iRepeatMode == 2) || ((MML->iRepeatMode == 0) && (fSub == true))){
					nowTrack->SetRepeat_A_Start(MML);	//2 || (0 && sub)
				} else {
					nowTrack->SetRepeat_C_Start(MML);	//1 || (0 && main)
				}
				break;

			case(mml_Repeat_C_End):
				if((MML->iRepeatMode == 2) || ((MML->iRepeatMode == 0) && (fSub == true))){
					nowTrack->SetRepeat_A_End(MML);
				} else {
					nowTrack->SetRepeat_C_End(MML);
				}
				break;

		//	case(mml_Repeat_C_Branch):
		//		nowTrack->SetRepeat_C_Branch(MML);
		//		break;

			case(mml_Tempo):
				SetTempo(MML);
				break;

			case(mml_Tempo_Relative):
				SetRelativeTempo(MML);
				break;

			case(mml_Tempo_Up):
				TempoUp();
				break;

			case(mml_Tempo_Down):
				TempoDown();
				break;

			case(mml_La):
				nowTrack->SetNote(MML, 5);
				break;

			case(mml_Si):
				nowTrack->SetNote(MML, 6);
				break;

			case(mml_Do):
				nowTrack->SetNote(MML, 0);
				break;

			case(mml_Re):
				nowTrack->SetNote(MML, 1);
				break;

			case(mml_Mi):
				nowTrack->SetNote(MML, 2);
				break;

			case(mml_Fa):
				nowTrack->SetNote(MML, 3);
				break;

			case(mml_Sol):
				nowTrack->SetNote(MML, 4);
				break;

			case(mml_Rest):
				nowTrack->SetRest(MML, MML->rest);
				break;

			case(mml_Wait):
				nowTrack->SetRest(MML, MML->wait | 0x80);
				break;

			case(mml_Tai):
				nowTrack->SetTai(MML);
				break;

			case(mml_Length):
				nowTrack->SetLength(MML);
				break;

			case(mml_Gate_Q):
				if(MML->q_reverse == true){
					nowTrack->SetGatetime(MML);
				} else {
					nowTrack->SetGatetime_Q(MML);
				}
				break;

			case(mml_Gate_q):
				if(MML->q_reverse == true){
					nowTrack->SetGatetime_Q(MML);
				} else {
					nowTrack->SetGatetime(MML);
				}
				break;

			case(mml_Gate_u):
				nowTrack->SetGatetime_u(MML);
				break;

			case(mml_Echo_C):
				nowTrack->SetEchoBuffer(MML, 0);
				break;

			case(mml_Echo_D):
				nowTrack->SetEchoBuffer(MML, 1);
				break;

			case(mml_Echo_E):
				nowTrack->SetEchoBuffer(MML, 2);
				break;

			case(mml_Echo_F):
				nowTrack->SetEchoBuffer(MML, 3);
				break;

			case(mml_Echo_G):
				nowTrack->SetEchoBuffer(MML, 4);
				break;

			case(mml_Echo_A):
				nowTrack->SetEchoBuffer(MML, 5);
				break;

			case(mml_Echo_B):
				nowTrack->SetEchoBuffer(MML, 6);
				break;

			case(mml_Echo_R):
				nowTrack->SetEchoBuffer(MML, -1);
				break;

			case(mml_Echo_Reset):
				nowTrack->ResetEcho();
				break;

			case(mml_Echo_Off):
				nowTrack->SetEcho();
				break;

			case(mml_Echo):
				nowTrack->SetEcho(MML);
				break;

			case(mml_Jump):
				nowTrack->SetJump(MML);
				break;

			case(mml_Jump_drv):
				SetJumpDrv(MML);
				break;

			case(mml_Envelop_Voice):
				nowTrack->SetEnvelop_Evoi(MML->GetInt() + MML->offset_Ei);
			//	nowTrack->SetEnvelop(nsd_Envelop_Voice, MML, MML->offset_Ei);
				break;

			case(mml_Envelop_Volume):
				nowTrack->SetEnvelop_Evol(MML->GetInt() + MML->offset_Ev);
			//	nowTrack->SetEnvelop(nsd_Envelop_Volume, MML, MML->offset_Ev);
				break;

			case(mml_Envelop_Frequency):
				nowTrack->SetEnvelop_Em(MML->GetInt() + MML->offset_Em);
			//	nowTrack->SetEnvelop(nsd_Envelop_Frequency, MML, MML->offset_Em);
				break;

			case(mml_Envelop_Note):
				nowTrack->SetEnvelop_En(MML->GetInt() + MML->offset_En);
			//	nowTrack->SetEnvelop(nsd_Envelop_Note, MML, MML->offset_En);
				break;

			case(mml_Envelop_Off_Voice):
				MML->Err(_T("音色エンベロープは、@コマンドで無効にできます。"));
				break;

			case(mml_Envelop_Off_Volume):
				nowTrack->SetEnvelop_Evol();
			//	SetEvent(new mml_Address(nsd_Envelop_Volume));
				break;

			case(mml_Envelop_Off_Frequency):
				nowTrack->SetEnvelop_Em();
			//	SetEvent(new mml_Address(nsd_Envelop_Frequency));
				break;

			case(mml_Envelop_Off_Note):
				nowTrack->SetEnvelop_En();
			//	SetEvent(new mml_Address(nsd_Envelop_Note));
				break;

			case(mml_Patch):
				nowTrack->SetPatch(MML);
				break;

			case(mml_Patch_Off):
				nowTrack->SetPatch();
				break;

			case(mml_Release_mdoe):
				nowTrack->SetReleaseMode(MML);
				break;

			case(mml_Release_Voice):
				nowTrack->SetReleaseVoice(MML);
				break;

			case(mml_Release_Volume):
				nowTrack->SetReleaseVolume(MML);
				break;

			case(mml_Voice):
				nowTrack->SetVoice(MML->GetInt());
			//	SetEvent(new mml_general(nsd_Voice, MML, _T("Voice")));
				break;

			case(mml_FDSC):
				nowTrack->SetFDSC(MML);
				break;

			case(mml_FDSM):
				nowTrack->SetFDSM(MML);
				break;

			case(mml_FDSF):
				Set_FDS_Frequency(MML);
				break;

			case(mml_FDSV):
				Set_FDS_Volume(MML);
				break;

			case(mml_VRC7):
				nowTrack->SetVRC7(MML);
				break;

			case(mml_N163):
				nowTrack->SetN163(MML);
				break;

			case(mml_N163_Set):
				nowTrack->SetN163_Set(MML);
				break;

			case(mml_N163_Load):
				nowTrack->SetN163_Load(MML);
				break;

			case(mml_N163_Channel):
				Set_N163_Channel(MML);
				break;

			case(mml_FME7_frequency):
				Set_FME7_Frequency(MML);
				break;

			case(mml_Octave):
				nowTrack->SetOctave(MML);
				break;

			case(mml_Octave_Up):
				if(MML->octave_reverse == true){
					nowTrack->SetOctaveDec();
				} else {
					nowTrack->SetOctaveInc();
				}
				break;

			case(mml_Octave_Down):
				if(MML->octave_reverse == true){
					nowTrack->SetOctaveInc();
				} else {
					nowTrack->SetOctaveDec();
				}
				break;

			case(mml_Octave_Up1):
				nowTrack->SetOctaveOne_Inc();
				break;

			case(mml_Octave_Down1):
				nowTrack->SetOctaveOne_Dec();
				break;

			case(mml_Detune_Cent):
				SetEvent(new mml_general(nsd_Detune_Cent, MML, _T("Detune Cent")));
				break;

			case(mml_Detune_Register):
				SetEvent(new mml_general(nsd_Derune_Register, MML, _T("Derune Register")));
				break;

			case(mml_Detune_Cent_Relative):
				SetEvent(new mml_general(nsd_SubCommand, (const char)nsd_sub_Detune_Cent, (char)MML->GetInt(), _T("Relative Detune Cent")));
				break;

			case(mml_Detune_Register_Relative):
				SetEvent(new mml_general(nsd_SubCommand, (const char)nsd_sub_Derune_Register, (char)MML->GetInt(), _T("Relative Detune Register")));
				break;
	
			case(mml_Transpose):
				i = MML->GetInt();
				if( (i < -128) || (i > 127) ){
					MML->Err(_T("移調は-127～128の範囲で指定してください。"));
				}
				nowTrack->SetTranspose(i);
				//SetEvent(new mml_general(nsd_Transpose, MML, _T("Transpose")));
				break;

			case(mml_Transpose_Relative):
				nowTrack->SetTranspose_Relative(MML->GetInt());
				//SetEvent(new mml_general(nsd_Relative_Transpose, MML, _T("Relative Transpose")));
				break;

			case(mml_KeyShift):
				nowTrack->SetKeyShift(MML);
				break;

			case(mml_KeyShift_Relative):
				nowTrack->SetKeyShift_Relative(MML);
				break;

			case(mml_Protament):
				nowTrack->SetProtament(MML);
				break;

			case(mml_Protament2):
				nowTrack->SetProtament(MML, (unsigned char)iTempo);
				break;

			case(mml_Sweep):
				nowTrack->SetSweep(MML);
				break;

			case(mml_Volume):
				nowTrack->SetVolume(MML);
				break;

			case(mml_Volume_Up):
				nowTrack->SetVolumeInc(MML);
				break;

			case(mml_Volume_Down):
				nowTrack->SetVolumeDec(MML);
				break;

			case(mml_VRC7_Write):
				Set_VRC7_Write(MML);
				break;

			case(mml_Memory_Write):
				Set_Poke(MML);
				break;

			case(mml_Priority):
				Set_Priority(MML);
				break;

			case(mml_Bar):
				break;

			//unknown command
			default:
				MML->Err(_T("unknown command"));
				break;
		}
	}

	//Local Macroの解放
	MML->DeleteMacro(1);

	if(fSub == true){
		//サブルーチンブロックの場合
		code.resize(0);
		i = (int)ptcTrack[iTrack]->SetEnd(MML);

	} else {
		//それ以外の場合
		i = 2 + ((maxTrack + 1) * 2);		//トラック情報を書くヘッダーのサイズを計算。
		code.resize(i);						//ヘッダ用にコードサイズを確保

		code[0] = (unsigned char)maxTrack + 1;				//トラック数
		code[1] = Priority;									//各種フラグ

		// to do ■■■アドレス解決ルーチンへ移動
		//各トラックに終端を書いて、曲データのアドレス情報を作成
		iTrack = 0;
		while(iTrack <= maxTrack){
			unsigned	int	n	= (int)ptcTrack[iTrack]->SetEnd(MML);
			if(n==0){
				code[iTrack *2 + 2]	= 0;
				code[iTrack *2 + 3]	= 0;
			} else {
				code[iTrack *2 + 2]	= (unsigned char)((i   ) & 0xFF);
				code[iTrack *2 + 3]	= (unsigned char)((i>>8) & 0xFF);
			}
			i += n;
			iTrack++;
		}
	}
	iSize = i;

}

//==============================================================
//		デストラクタ
//--------------------------------------------------------------
//	●引数
//				無し
//	●返値
//				無し
//==============================================================
TrackSet::~TrackSet(void)
{
}

//==============================================================
//		クリア
//--------------------------------------------------------------
//	●引数
//				無し
//	●返値
//				無し
//==============================================================
void	TrackSet::clear_Optimize()
{
	maxTrack = -1;
	MusicItem::clear_Optimize();
}

//==============================================================
//		コードの取得
//--------------------------------------------------------------
//	●引数
//		MusicFile*	MUS		コードを出力する曲データファイル・オブジェクト
//	●返値
//				無し
//==============================================================
void	TrackSet::getAsm(MusicFile* MUS)
{
	//----------------------
	//Local変数
	int	i = 0;
	list<	MusicItem*>::iterator	itItem;

	if(fSub == false){
		*MUS << "	.byte	$" << hex << setw(2) << setfill('0') << (int)(code[0] & 0xFF) << ", $" << (int)(code[1] & 0xFF) << endl;
		while(i <= maxTrack){
			if(i==0){
				*MUS << "	.word	$";
			} else {
				*MUS << " ,$";
			}
			*MUS << hex << setw(4) << setfill('0') << (int)((code[i*2+2] & 0xFF) | ((code[i*2+3] & 0xFF)<<8));
			i++;
		}
		*MUS << dec << endl;
	}

	if(!ptcItem.empty()){
		itItem = ptcItem.begin();
		while(itItem != ptcItem.end()){
			(*itItem)->getAsm(MUS);
			itItem++;
		}
	}
}

//==============================================================
//		カウントしながら、最適化情報収集
//--------------------------------------------------------------
//	●引数
//				MusicFile*	MUS
//				int			iStart
//				int			iEnd
//	●返値
//				無し
//==============================================================
void	TrackSet::TickCountPrint(MusicFile* MUS, int iStart, int iEnd)
{
	int		i		= iStart;
	int		iTick;

	cout	<< "Track ";
	while(i < iEnd){
		i++;
		cout << "| TR(" << setfill(' ')  << setw(2) << i << ") ";
	}

	i = iStart;
	cout << endl <<	"Loop  ";
	while(i < iEnd){
		ptcTrack[i]->nsd.init();		//エミュレータ・パラメータ初期化
		ptcTrack[i]->TickCount(MUS);	//エミュレート
		iTick = ptcTrack[i]->GetTickLoop();
		if(iTick == -1){
			cout << "| no-loop";
		} else {
			cout << "|" << setw(8) << setfill(' ') << iTick;
		}
		i++;
	}

	i = iStart;
	cout << endl <<	"Total ";;
	while(i < iEnd){
		cout << "|" << setw(8) << setfill(' ') << ptcTrack[i]->GetTickTotal();
		i++;
	}
	cout << endl;
	
}

//==============================================================
//		カウントしながら、最適化情報収集
//--------------------------------------------------------------
//	●引数
//				MusicFile*	MUS
//	●返値
//				無し
//==============================================================
void	TrackSet::TickCount(MusicFile* MUS)
{

	int		iTrack = 0;
	int		j;
	int		_maxTrack = maxTrack + 1;

	while(iTrack < _maxTrack){
		j = iTrack + 8;
		if(j >= _maxTrack){
			j = _maxTrack;
		}
		TickCountPrint(MUS,iTrack,j);
		cout << endl;
		iTrack = j;
	}
}

//==============================================================
//		不要な定義があるか検索
//--------------------------------------------------------------
//	●引数
//		MusicFile*	MUS		曲データファイル・オブジェクト
//	●返値
//				無し
//==============================================================
void	TrackSet::OptimizeDefineCheck(MusicFile* MUS)
{
	iTrack = 0;
	while(iTrack <= maxTrack){
		ptcTrack[iTrack]->OptimizeDefineCheck(MUS);
		iTrack++;
	}

}

//==============================================================
//		アドレス情報を決定する。
//--------------------------------------------------------------
//	●引数
//		MusicFile*	MUS		曲データファイル・オブジェクト
//	●返値
//				無し
//==============================================================
void	TrackSet::Fix_Address(MusicFile* MUS)
{
	iTrack = 0;
	while(iTrack <= maxTrack){
		ptcTrack[iTrack]->Fix_Address(MUS);
		iTrack++;
	}

}

//==============================================================
//		トラック
//--------------------------------------------------------------
//	●引数
//		MMLfile*			MML			MMLファイルのオブジェクト
//	●返値
//		無し
//==============================================================
void	TrackSet::TrackChk(MMLfile* MML)
{
	int		i = iTrack + 1;

	if(MML->GetMacroNest() > 0){
		MML->Err(_T("マクロ中でトラックの指定はできません。"));
	}

	//------------------
	//続きのトラックのチェック
//	i		= iTrack + 1;
	iTrack	= 0;
	while(i <= maxTrack){
		if(ptcTrack[i]->GetCompileFlag() == true){
			iTrack = i;
			break;
		}
		i++;
	}

	if(iTrack != 0){
		//------------------
		//続きのトラックがある場合

		//ポインタと行番号を復帰
		MML->StreamPointerMove(TrackPt);
		MML->SetLine(TrackLine);
		nowTrack = getTrack(MML, iTrack);
	} else {
		nowTrack = NULL;
	}
}

//==============================================================
//		トラックの処理
//--------------------------------------------------------------
//	●引数
//		MMLfile*			MML			MMLファイルのオブジェクト
//	●返値
//		無し
//==============================================================
void	TrackSet::TrackProc(MMLfile* MML)
{
					int		i;
	unsigned		char	cData;

	//------------------
	//続きのトラックのチェック

	TrackChk(MML);

	if(nowTrack == NULL){
		//------------------
		//続きのトラックがない場合

		//フラグを全てリセット
		i = 0;
		while(i <= maxTrack){
			ptcTrack[i]->SetCompileFlag(false);
			i++;
		}
		
		//コンパイルするトラックを列挙
		do{
			iTrack = MML->GetInt() - 1;
			if( (iTrack <= -1) ){
				MML->Err(_T("トラック番号で指定できる範囲を超えています。"));
			}
			nowTrack = getTrack(MML, iTrack);
			nowTrack->SetCompileFlag(true);
			cData = MML->GetChar();
		} while(cData == ',');
		MML->Back();

		//ポインタと行番号を退避
		TrackPt		= MML->tellg();
		TrackLine	= MML->GetLine();

		//最初の
		i = 0;
		while(i <= maxTrack){
			if(ptcTrack[i]->GetCompileFlag() == true){
				iTrack = i;
				nowTrack = getTrack(MML, iTrack);
				break;
			}
			i++;
		}

	}

}

//==============================================================
//		トラックの作成
//--------------------------------------------------------------
//	●引数
//		unsigned	int	_track		トラック番号
//	●返値
//		MusicTrack*					作ったトラック・オブジェクトのポインタ
//==============================================================
MusicTrack*	TrackSet::makeTrack(MMLfile* MML, int _track)
{
	//トラックのオブジェクトを生成。
	MusicTrack*	newTrack	= new MusicTrack(_track, MML);

	//生成したアイテムは保存
	ptcItem.push_back(newTrack);		//基底クラス"MusicItem"側で開放する。
	ptcTrack[_track] = newTrack;

	if(MML->iReleaseVolume != 2){		//ドライバー側のデフォルトは２
		newTrack->SetEvent(new mml_general(nsd_Release_Volume + (unsigned char)MML->iReleaseVolume, _T("Release Volume")));
	}

	//ポインタを渡す
	return(newTrack);
}

//==============================================================
//		トラック・オブジェクトのポインタを取得
//--------------------------------------------------------------
//	●引数
//		unsigned	int	_track	トラック番号
//	●返値
//		MusicTrack*				トラック・オブジェクトのポインタ
//	●返値
//		指定された番号のトラック・オブジェクトのポインタを取得する
//		無かった場合は新たにトラックを作って、
//		トラック番号が最大値を超えていたら最大値を更新する。
//==============================================================
MusicTrack*	TrackSet::getTrack(MMLfile* MML, int _track)
{
			int	i			 = maxTrack;	// i = 今ある、最終トラックの番号
	MusicTrack*	_getTrack;

	//最終トラック番号が指定値未満だったら、繰り返す。
	while(i < _track){
		i++;
		//トラックが無かったら作る
		if(ptcTrack.count(i) == 0){
			_getTrack	= makeTrack(MML, i);
		} else {
			_CERR << _T("MusicTrack* TrackSet::getTrack()関数でエラーが発生しました。") << endl;
			nsc_exit(EXIT_FAILURE);
		}
	}
	maxTrack = i;	//トラックの最大値を記憶。

	//ポインタを取得する。
	_getTrack = ptcTrack[_track];

	return(_getTrack);
}

//==============================================================
//		イベント
//--------------------------------------------------------------
//	●引数
//		MusicItem* _item
//	●返値
//		無し
//==============================================================
void	TrackSet::SetEvent(MusicItem* _item)
{
	nowTrack->SetEvent(_item);
}

//==============================================================
//			効果音優先度設定
//--------------------------------------------------------------
//	●引数
//		MMLfile*	MML		MMLファイルのオブジェクト
//	●返値
//				無し
//==============================================================
void	TrackSet::Set_Priority(MMLfile* MML)
{
	int	i = MML->GetInt();

	//se?
	if(fSE == false){
		MML->Warning(_T("SEブロック以外では優先度指定はできません。無視します。"));
	} else {
		if( (i <= 3) && (i >=0) ){
			Priority = (char)i;
		} else {
			MML->Err(_T("効果音の優先度は、は0～3の範囲で指定して下さい。"));
		}
	}

}

//==============================================================
//		ジャンプ
//--------------------------------------------------------------
//	●引数
//		MusicItem* _item
//	●返値
//		無し
//==============================================================
void	TrackSet::SetJumpDrv(MMLfile* MML)
{
	int	i	= MML->GetInt();

	switch(i){
		case(0):
			SetEvent(new mml_general(nsd_SubCommand, (const char)nsd_sub_Jump_off, _T("Jump Off")));
			break;
		case(1):
			SetEvent(new mml_general(nsd_SubCommand, (const char)nsd_sub_Jump_on, _T("Jump On")));
			break;
		default:
			MML->Err(_T("jコマンドが指定可能な範囲を超えました。"));
	}
}

//==============================================================
//		テンポ
//--------------------------------------------------------------
//	●引数
//		MMLfile*			MML			MMLファイルのオブジェクト
//	●返値
//		無し
//==============================================================
void	TrackSet::SetTempo(MMLfile* MML)
{
	int	iValue	= MML->GetInt();

	iTempo = ((iValue * MML->timebase) / 24);
	
	if((iTempo<0) || (iTempo>255)){
		MML->Err(_T("テンポが指定可能な範囲を超えました。"));
	}
	SetEvent(new mml_general(nsd_Tempo, (unsigned char)iTempo, _T("Tempo")));
}

//==============================================================
//		相対テンポ
//--------------------------------------------------------------
//	●引数
//		MMLfile*			MML			MMLファイルのオブジェクト
//	●返値
//		無し
//==============================================================
void	TrackSet::SetRelativeTempo(MMLfile* MML)
{
	int	iValue	= MML->GetInt();

	iTempo += iValue;

	SetEvent(new mml_general(nsd_Relative_Tempo, (char)iValue, _T("Relative Tempo")));

}

//==============================================================
//		テンポアップ
//--------------------------------------------------------------
//	●引数
//		MMLfile*			MML			MMLファイルのオブジェクト
//	●返値
//		無し
//==============================================================
void	TrackSet::TempoUp()
{
	iTempo += 4;

	SetEvent(new mml_general(nsd_Relative_Tempo, (char)4, _T("Relative Tempo")));

}

//==============================================================
//		テンポダウン
//--------------------------------------------------------------
//	●引数
//		MMLfile*			MML			MMLファイルのオブジェクト
//	●返値
//		無し
//==============================================================
void	TrackSet::TempoDown()
{
	iTempo -= 4;

	SetEvent(new mml_general(nsd_Relative_Tempo, (char)-4, _T("Relative Tempo")));

}

//==============================================================
//		アドレス指定の書込み
//--------------------------------------------------------------
//	●引数
//		MMLfile*	MML		MMLファイルのオブジェクト
//	●返値
//		無し
//==============================================================
void	TrackSet::Set_Poke(MMLfile* MML)
{
	unsigned	int		addr;
	unsigned	int		data;
	unsigned	char	cData;

	addr = MML->GetInt();
	if( (addr < 0x0000) || (addr > 0xFFFF) ){
		MML->Err(_T("yコマンドのアドレスは$0000～$FFFFの範囲で指定して下さい。"));
	}

	cData = MML->GetChar();
	if(cData != ','){
		MML->Err(_T("yコマンドのパラメータが足りません。２つ指定してください。"));
	}

	data = MML->GetInt();
	if(data > 255){
		MML->Err(_T("yコマンドのデータは$00～$FFの範囲で指定して下さい。"));
	}
	SetEvent(new mml_poke(addr, (unsigned char)data));
}

//==============================================================
//			FDS	キャリア周波数設定
//--------------------------------------------------------------
//	●引数
//		MMLfile*	MML		MMLファイルのオブジェクト
//	●返値
//				無し
//==============================================================
void	TrackSet::Set_FDS_Frequency(MMLfile* MML)
{
	int	i = MML->GetInt();

	if( (i <= 0x0FFF) && (i >=0) ){
		unsigned	char	c0 = (unsigned char)( i       & 0xFF);
		unsigned	char	c1 = (unsigned char)((i >> 8) & 0xFF);
		SetEvent(new mml_general(nsd_FDS_Frequency,c0,c1,_T("FDS career frequency")));
	} else {
		MML->Err(_T("FDSのキャリア周波数は0～4095の範囲で指定して下さい。"));
	}
}

//==============================================================
//			FDS	Master volume
//--------------------------------------------------------------
//	●引数
//		MMLfile*	MML		MMLファイルのオブジェクト
//	●返値
//				無し
//==============================================================
void	TrackSet::Set_FDS_Volume(MMLfile* MML)
{
	int		i = MML->GetInt();

	if((i<=3 ) && (i>=0)){
		SetEvent(new mml_general(nsd_FDS_Volume,(unsigned char)i,_T("FDS Master volume")));
	} else {
		MML->Err(_T("FDSのマスター音量は0～3の範囲で指定して下さい。"));
	}
}

//==============================================================
//		VRC7	レジスタ書き込み
//--------------------------------------------------------------
//	●引数
//		MMLfile*	MML		MMLファイルのオブジェクト
//	●返値
//		無し
//==============================================================
void	TrackSet::Set_VRC7_Write(MMLfile* MML)
{
	unsigned	char	cData;

				int		_Reg;
				int		_Dat;

	_Reg = MML->GetInt();
	if( (_Reg < 0) || (_Reg > 0x40) ){
		MML->Err(_T("VRC7レジスタ操作の第1パラメータは0～63の範囲で指定してください。"));
	}

	cData = MML->GetChar();
	if(cData != ','){
		MML->Err(_T("yV コマンドのパラメータが足りません。２つ指定してください。"));
	}

	_Dat = MML->GetInt();
	if( (_Dat < 0) || (_Dat > 255) ){
		MML->Err(_T("VRC7レジスタ操作の第2パラメータは0～255の範囲で指定してください。"));
	}

	SetEvent(new mml_general(nsc_VRC7_reg,(unsigned char)_Reg,(unsigned char)_Dat, _T("VRC7 Register Write")));
}

//==============================================================
//		NC	n163チャンネル数設定
//--------------------------------------------------------------
//	●引数
//		MMLfile*	MML		MMLファイルのオブジェクト
//	●返値
//				無し
//==============================================================
void	TrackSet::Set_N163_Channel(MMLfile* MML)
{
	int	i = MML->GetInt();

	if( (i <= 8) && (i >=1) ){
		SetEvent(new mml_general(nsc_N163_Channel,(unsigned char)(i-1),_T("n163 channel")));
	} else {
		MML->Err(_T("n163のチャンネル数は1～8の範囲で指定してください。"));
	}

}

//==============================================================
//			SUNSOFT 5b エンベロープ周波数設定
//--------------------------------------------------------------
//	●引数
//		MMLfile*	MML		MMLファイルのオブジェクト
//	●返値
//				無し
//==============================================================
void	TrackSet::Set_FME7_Frequency(MMLfile* MML)
{
	int	i = MML->GetInt();

	if( (i <= 0xFFFF) && (i >=0) ){
		unsigned	char	c0 = (unsigned char)( i       & 0xFF);
		unsigned	char	c1 = (unsigned char)((i >> 8) & 0xFF);
		SetEvent(new mml_general(nsc_FME7_frequency,c0,c1,_T("FME7 envelop frequency")));
	} else {
		MML->Err(_T("SUNSOFT 5bのハードウェアエンベロープ周波数は0～65535の範囲で指定して下さい。"));
	}
}
