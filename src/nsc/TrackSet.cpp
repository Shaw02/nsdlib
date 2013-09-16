#include "StdAfx.h"
#include "trackset.h"

//==============================================================
//		コンストラクタ
//--------------------------------------------------------------
//	●引数
//		MMLfile*			MML			MMLファイルのオブジェクト
//		unsigned	int		_id			BGM番号
//		bool				_sub		このオブジェクトは、サブルーチン？
//		const		wchar_t	_strName[]	オブジェクト名
//	●返値
//					無し
//==============================================================
TrackSet::TrackSet(MMLfile* MML, unsigned int _id, bool _sub, const wchar_t _strName[]):
	MusicItem(_strName),
	m_id(_id)
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
	mml_CallSE,
	mml_Subroutine,

	mml_Loop,
	mml_Repeat_A_Start,
	mml_Repeat_A_Branch,
	mml_Repeat_A_End,
	mml_Repeat_B_Start,
	mml_Repeat_B_Branch,
	mml_Repeat_B_End,

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
	mml_Gate_q,
	mml_Gate_u,

	mml_Echo,
	mml_Echo_Off,

	mml_Envelop_Voice,
	mml_Envelop_Volume,
	mml_Envelop_Frequency,
	mml_Envelop_Note,

	mml_Envelop_Off_Voice,
	mml_Envelop_Off_Volume,
	mml_Envelop_Off_Frequency,
	mml_Envelop_Off_Note,

	mml_Release_mdoe,
	mml_Release_Voice,
	mml_Release_Volume,

	mml_FDSC,
	mml_FDSM,
	mml_FDSF,
	mml_FDSV,
	mml_VRC7,
	mml_N163,
	mml_Voice,
	mml_N163_Channel,
	mml_FME7_frequency,

	mml_Octave,
	mml_Octave_Up,
	mml_Octave_Down,
	mml_Octave_Up1,
	mml_Octave_Down1,
	mml_Detune_Cent,
	mml_Detune_Register,
	mml_Transpose,
	mml_Transpose_Relative,
	mml_Protament,
	mml_Sweep,

	mml_Volume,
	mml_Volume_Up,
	mml_Volume_Down,

	mml_VRC7_Write,
	mml_Memory_Write,

	mml_Bar
};

//	これらは、MML構文で使えるコマンド。
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
		{	"$",		mml_Macro				},

		{	"L",		mml_Loop				},
		{	"|:",		mml_Repeat_B_Start		},
		{	"\\",		mml_Repeat_B_Branch		},
		{	":|",		mml_Repeat_B_End		},
		{	"[",		mml_Repeat_A_Start		},
		{	":",		mml_Repeat_A_Branch		},
		{	"]",		mml_Repeat_A_End		},

		{	"∞",		mml_Loop				},
		{	"｜：",		mml_Repeat_B_Start		},
		{	"￥",		mml_Repeat_B_Branch		},
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
		{	"q",		mml_Gate_q				},
		{	"ゲート",	mml_Gate_q				},
		{	"u",		mml_Gate_u				},

		{	"EC*",			mml_Echo_Off			},
		{	"EC",			mml_Echo				},
		{	"エコーオフ",	mml_Echo_Off			},
		{	"エコー",		mml_Echo				},

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

		{	"@FC",	mml_FDSC				},
		{	"@FM",	mml_FDSM				},
		{	"@FF",	mml_FDSF				},
		{	"@FV",	mml_FDSV				},
		{	"@V",	mml_VRC7				},
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

		{	"D%",			mml_Detune_Register		},
		{	"D",			mml_Detune_Cent			},
		{	"ディチューン",	mml_Detune_Cent			},
		{	"__",			mml_Transpose_Relative	},
		{	"_",			mml_Transpose			},
		{	"TrackKey",		mml_Transpose			},
		{	"P",			mml_Protament			},
		{	"ポルタメント",	mml_Protament			},
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

		{	"　",	mml_Bar					},
		{	"|",	mml_Bar					},
		{	"｜",	mml_Bar					}
};

	unsigned	char	cData;
				int		i;

	//------------------------------
	//クラスの初期設定
	fSub		= _sub;		//サブルーチンのフラグ
	iTrack		= 0;		//コンパイル中のトラック（Default = 0）
	maxTrack	= 0;		//最大トラック番号

	//まずは、１つだけトラック（0番）のオブジェクトを作る。
	nowTrack	= makeTrack(iTrack);
	nowTrack->SetCompileFlag(true);


	//------------------------------
	//コンパイル

	// { の検索
	while(MML->cRead() != '{'){
		if(MML->eof()){
			MML->Err(L"ブロックの開始を示す{が見つかりません。");
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
				MML->Err(L"ブロックの終端を示す`}'がありません。");
			}

			//１つ戻る
			MML->Back();
		}

		//各コマンド毎の処理
		switch(MML->GetCommandID(Command, sizeof(Command)/sizeof(Command_Info))){

			case(mml_Track):
				if(fSub == true){
					MML->Warning(L"Subブロック内ではトラック指定はできません。無視します。");
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

			case(mml_CallSE):
				nowTrack->SetSE(MML);
				break;

			case(mml_Subroutine):
				nowTrack->SetSubroutine(MML);
				break;

			case(mml_Loop):
				if(fSub == true){
					MML->Warning(L"Subブロック内では無限ループはできません。無視します。");
				} else {
					nowTrack->SetLoop();
				}
				break;

			case(mml_Repeat_A_Start):
				nowTrack->SetRepeat_A_Start(MML);
				break;

			case(mml_Repeat_A_Branch):
				nowTrack->SetRepeat_A_Branch(MML);
				break;

			case(mml_Repeat_A_End):
				nowTrack->SetRepeat_A_End(MML);
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

			case(mml_Tempo):
				SetTempo(MML);
				break;

			case(mml_Tempo_Relative):
				SetEvent(new mml_general(nsd_Relative_Tempo, MML, L"Relative Tempo"));
				break;

			case(mml_Tempo_Up):
				SetEvent(new mml_general(nsd_Relative_Tempo, 4, L"Relative Tempo"));
				break;

			case(mml_Tempo_Down):
				SetEvent(new mml_general(nsd_Relative_Tempo, -4, L"Relative Tempo"));
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

			case(mml_Gate_q):
				SetGatetime(MML);
				break;

			case(mml_Gate_u):
				SetGatetime_u(MML);
				break;

			case(mml_Echo_Off):
				nowTrack->SetEcho();
				break;

			case(mml_Echo):
				nowTrack->SetEcho(MML);
				break;

			case(mml_Envelop_Voice):
				nowTrack->SetEnvelop(nsd_Envelop_Voice, MML, MML->offset_Ei);
				break;

			case(mml_Envelop_Volume):
				nowTrack->SetEnvelop(nsd_Envelop_Volume, MML, MML->offset_Ev);
				break;

			case(mml_Envelop_Frequency):
				nowTrack->SetEnvelop(nsd_Envelop_Frequency, MML, MML->offset_Em);
				break;

			case(mml_Envelop_Note):
				nowTrack->SetEnvelop(nsd_Envelop_Note, MML, MML->offset_En);
				break;

			case(mml_Envelop_Off_Voice):
				MML->Err(L"音色エンベロープは、@コマンドで無効にできます。");
				break;

			case(mml_Envelop_Off_Volume):
				SetEvent(new mml_Address(nsd_Envelop_Volume));
				break;

			case(mml_Envelop_Off_Frequency):
				SetEvent(new mml_Address(nsd_Envelop_Frequency));
				break;

			case(mml_Envelop_Off_Note):
				SetEvent(new mml_Address(nsd_Envelop_Note));
				break;

			case(mml_Release_mdoe):
				SetReleaseMode(MML);
				break;

			case(mml_Release_Voice):
				SetReleaseVoice(MML);
				break;

			case(mml_Release_Volume):
				SetReleaseVolume(MML);
				break;

			case(mml_Voice):
				SetEvent(new mml_general(nsd_Voice, MML, L"Voice"));
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

			case(mml_N163_Channel):
				SetN163Channel(MML);
				break;

			case(mml_FME7_frequency):
				Set_FME7_Frequency(MML);
				break;

			case(mml_Octave):
				SetOctave(MML);
				break;

			case(mml_Octave_Up):
				if(MML->octave_reverse == true){
					SetOctaveDec();
				} else {
					SetOctaveInc();
				}
				break;

			case(mml_Octave_Down):
				if(MML->octave_reverse == true){
					SetOctaveInc();
				} else {
					SetOctaveDec();
				}
				break;

			case(mml_Octave_Up1):
				SetOctaveOne_Inc();
				break;

			case(mml_Octave_Down1):
				SetOctaveOne_Dec();
				break;

			case(mml_Detune_Cent):
				SetEvent(new mml_general(nsd_Detune_Cent, MML, L"Detune Cent"));
				break;

			case(mml_Detune_Register):
				SetEvent(new mml_general(nsd_Derune_Register, MML, L"Derune Register"));
				break;

			case(mml_Transpose):
				SetEvent(new mml_general(nsd_Transpose, MML, L"Transpose"));
				break;

			case(mml_Transpose_Relative):
				SetEvent(new mml_general(nsd_Relative_Transpose, MML, L"Relative Transpose"));
				break;

			case(mml_Protament):
				SetProtament(MML);
				break;

			case(mml_Sweep):
				SetSweep(MML);
				break;

			case(mml_Volume):
				SetVolume(MML);
				break;

			case(mml_Volume_Up):
				SetVolumeInc();
				break;

			case(mml_Volume_Down):
				SetVolumeDec();
				break;

			case(mml_VRC7_Write):
				SetVRC7_Write(MML);
				break;

			case(mml_Memory_Write):
				SetPoke(MML);
				break;

			case(mml_Bar):
				break;

			//unknown command
			default:
				MML->Err(L"unknown command");
				break;
		}
	}

	if(fSub == true){
		//サブルーチンブロックの場合
		code.resize(0);
		i = (int)ptcTrack[iTrack]->SetEnd();

	} else {
		//それ以外の場合
		i = 2 + ((maxTrack + 1) * 2);		//トラック情報を書くヘッダーのサイズを計算。
		code.resize(i);						//ヘッダ用にコードサイズを確保

		code[0] = (unsigned char)maxTrack + 1;				//トラック数
		code[1] = 0;						//

		//各トラックに終端を書いて、曲データのアドレス情報を作成
		iTrack = 0;
		while(iTrack <= maxTrack){
			code[iTrack *2 + 2]	= (unsigned char)((i   ) & 0xFF);
			code[iTrack *2 + 3]	= (unsigned char)((i>>8) & 0xFF);
			i += (int)ptcTrack[iTrack]->SetEnd();
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
	vector<	MusicItem*>::iterator	itItem;

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
		MML->Err(L"マクロ中でトラックの指定はできません。");
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
		nowTrack = getTrack(iTrack);
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
				MML->Err(L"トラック番号で指定できる範囲を超えています。");
			}
			nowTrack = getTrack(iTrack);
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
				nowTrack = getTrack(iTrack);
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
MusicTrack*	TrackSet::makeTrack(int _track)
{
	//トラックのオブジェクトを生成。
	MusicTrack*	newTrack	= new MusicTrack();

	//生成したアイテムは保存
	ptcItem.push_back(newTrack);		//基底クラス"MusicItem"側で開放する。
	ptcTrack[_track] = newTrack;

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
MusicTrack*	TrackSet::getTrack(int _track)
{
			int	i			 = maxTrack;	// i = 今ある、最終トラックの番号
	MusicTrack*	_getTrack;

	//最終トラック番号が指定値未満だったら、繰り返す。
	while(i < _track){
		i++;
		//トラックが無かったら作る
		if(ptcTrack.count(i) == 0){
			_getTrack	= makeTrack(i);
		} else {
			wcout << L"MusicTrack* TrackSet::getTrack()関数でエラーが発生しました。" << endl;
			exit(-1);
		}
	}
	maxTrack = i;	//トラックの最大値を記憶。

	//ポインタを取得する。
	_getTrack = ptcTrack[_track];

	return(_getTrack);
}

//==============================================================
//		オクターブ
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
//		オクターブ
//--------------------------------------------------------------
//	●引数
//		MMLfile*			MML			MMLファイルのオブジェクト
//	●返値
//		無し
//==============================================================
void	TrackSet::SetTempo(MMLfile* MML)
{
	int	iTempo	= MML->GetInt();

	iTempo = (iTempo * MML->timebase) / 24;
	if((iTempo<0) || (iTempo>255)){
		MML->Err(L"テンポが指定可能な範囲を超えました。");
	}

	SetEvent(new mml_general(nsd_Tempo, (unsigned char)iTempo, L"Tempo"));
}

//==============================================================
//		オクターブ
//--------------------------------------------------------------
//	●引数
//		MMLfile*	MML		MMLファイルのオブジェクト
//	●返値
//		無し
//==============================================================
void	TrackSet::SetOctave(MMLfile* MML)
{
	unsigned	int	iOctave = MML->GetInt() - 2;

	if( (iOctave <= 7) && (iOctave >=0) ){
		SetEvent(new mml_general(nsd_Octave + (unsigned char)iOctave, L"Octave"));
		nowTrack->SetOctave((unsigned char)iOctave);
	} else {
		MML->Err(L"オクターブは2～9の範囲で指定してください。o1の領域は相対オクターブをご利用ください。");
	}
}

void	TrackSet::SetOctaveInc()
{
	SetEvent(new mml_general(nsd_Octave_Up, L"Octave Up"));
	nowTrack->IncOctave();
}

void	TrackSet::SetOctaveDec()
{
	SetEvent(new mml_general(nsd_Octave_Down, L"Octave Down"));
	nowTrack->DecOctave();
}

void	TrackSet::SetOctaveOne_Inc()
{
	SetEvent(new mml_general(nsd_Octave_Up_1, L"One time octave up"));
	nowTrack->IncOctave1();
}

void	TrackSet::SetOctaveOne_Dec()
{
	SetEvent(new mml_general(nsd_Octave_Down_1, L"One time octave down"));
	nowTrack->DecOctave1();
}

//==============================================================
//		音量
//--------------------------------------------------------------
//	●引数
//		MMLfile*	MML		MMLファイルのオブジェクト
//	●返値
//		無し
//==============================================================
void	TrackSet::SetVolume(MMLfile* MML)
{
	unsigned	int	i = MML->GetInt();

	if( (i <= 15) && (i >= 0) ){
		SetEvent(new mml_general(nsd_Volume + (unsigned char)i, L"Volume"));
		nowTrack->SetVolume((unsigned char)i);
	} else {
		MML->Err(L"音量は0～15の範囲で指定してください。");
	}
}

void	TrackSet::SetVolumeInc()
{
	SetEvent(new mml_general(nsd_Volume_Up, L"Volume up"));
	nowTrack->IncVolume();
}

void	TrackSet::SetVolumeDec()
{
	SetEvent(new mml_general(nsd_Volume_Down, L"Volume down"));
	nowTrack->DecVolume();
}

//==============================================================
//		リリースモード
//--------------------------------------------------------------
//	●引数
//		MMLfile*	MML		MMLファイルのオブジェクト
//	●返値
//		無し
//==============================================================
void	TrackSet::SetReleaseMode(MMLfile* MML)
{
	mml_general*	_event;
	unsigned	int	i = MML->GetInt();

	switch(i){
		case(0):
			_event = new mml_general(nsd_GateMode_0,  L"GateMode 0");
			break;
		case(1):
			_event = new mml_general(nsd_GateMode_1,  L"GateMode 1");
			break;
		case(2):
			_event = new mml_general(nsd_GateMode_2,  L"GateMode 2");
			break;
		default:
			MML->Err(L"リリースモードは0～2の範囲で指定してください。");
			break;
	}
	SetEvent(_event);
}

//==============================================================
//		リリース音色
//--------------------------------------------------------------
//	●引数
//		MMLfile*	MML		MMLファイルのオブジェクト
//	●返値
//		無し
//==============================================================
void	TrackSet::SetReleaseVoice(MMLfile* MML)
{
	unsigned	int	i = MML->GetInt();

	if( (i <= 7) && (i >= 0) ){
		SetEvent(new mml_general(nsd_Release_Voice + (unsigned char)i, L"Release Voice"));
	} else {
		MML->Err(L"リリース音色は0～7の範囲で指定してください。");
	}
}

//==============================================================
//		リリース音量
//--------------------------------------------------------------
//	●引数
//		MMLfile*	MML		MMLファイルのオブジェクト
//	●返値
//		無し
//==============================================================
void	TrackSet::SetReleaseVolume(MMLfile* MML)
{
	unsigned	int	i = MML->GetInt();

	if( (i <= 15) && (i >= 0) ){
		SetEvent(new mml_general(nsd_Release_Volume + (unsigned char)i, L"Release Volume"));
	} else {
		MML->Err(L"音量は0～15の範囲で指定してください。");
	}
}

//==============================================================
//		ゲートタイム(q)
//--------------------------------------------------------------
//	●引数
//		MMLfile*	MML		MMLファイルのオブジェクト
//	●返値
//		無し
//==============================================================
void	TrackSet::SetGatetime(MMLfile* MML)
{
	unsigned	int	i = MML->GetInt();

	if( (i <= 15) && (i >= 0) ){
		SetEvent(new mml_general(nsd_GateTime_Byte + (unsigned char)i, L"Gatetime(q) Byte"));
	} else if( i <= 255) {
		SetEvent(new mml_general(nsd_GateTime_q, (unsigned char)i, L"Gatetime(q)"));
	} else {
		MML->Err(L"ゲートタイムqは0～255の範囲で指定して下さい。");
	}
}

//==============================================================
//		ゲートタイム(u)
//--------------------------------------------------------------
//	●引数
//		MMLfile*	MML		MMLファイルのオブジェクト
//	●返値
//		無し
//==============================================================
void	TrackSet::SetGatetime_u(MMLfile* MML)
{
	unsigned		int	i;
	unsigned		char	cData;

	//休符のモード
	cData = MML->GetChar();
	if(cData == '0'){
		i = 0;
	} else {
		MML->Back();
		i = MML->GetLength(nowTrack->GetDefaultLength());
	}
	SetEvent(new mml_general(nsd_GateTime_u, (unsigned char)i, L"GateTime(u)"));
	
}

//==============================================================
//		
//--------------------------------------------------------------
//	●引数
//		MMLfile*	MML		MMLファイルのオブジェクト
//	●返値
//		無し
//==============================================================
void	TrackSet::SetProtament(MMLfile* MML)
{
	unsigned	char	cData;

				int		_Decay;
				int		_Rate;
				int		_Depth;
				int		_Target;

	_Decay = MML->GetInt();
	if( (_Decay < 0) || (_Decay > 255) ){
		MML->Err(L"ポルタメントの第1パラメータは0～255の範囲で指定してください。");
	}
	_Decay++;

	cData = MML->GetChar();
	if(cData != ','){
		MML->Err(L"P コマンドのパラメータが足りません。４つ指定してください。");
	}

	_Rate = MML->GetInt();
	if( (_Rate < 1) || (_Rate > 256) ){
		MML->Err(L"ポルタメントの第2パラメータは1～256の範囲で指定してください。");
	}

	cData = MML->GetChar();
	if(cData != ','){
		MML->Err(L"P コマンドのパラメータが足りません。４つ指定してください。");
	}

	_Depth = MML->GetInt();
	if( (_Depth < -128) || (_Depth > 127) ){
		MML->Err(L"ポルタメントの第3パラメータは-128～127の範囲で指定してください。");
	}
	_Decay++;

	cData = MML->GetChar();
	if(cData != ','){
		MML->Err(L"P コマンドのパラメータが足りません。４つ指定してください。");
	}

	_Target = MML->GetInt();
	if( (_Target < -128) || (_Target > 127) ){
		MML->Err(L"ポルタメントの第4パラメータは-128～127の範囲で指定してください。");
	}
	SetEvent(new mml_general(nsd_Portamento, (unsigned char)_Decay, (unsigned char)_Rate, (unsigned char)_Depth, (unsigned char)_Target, L"Portamento"));
}

//==============================================================
//		スイープ
//--------------------------------------------------------------
//	●引数
//		MMLfile*	MML		MMLファイルのオブジェクト
//	●返値
//		無し
//==============================================================
void	TrackSet::SetSweep(MMLfile* MML)
{
				int		iSpeed;
				int		iDepth;
	unsigned	char	_data;
	unsigned	char	cData;

	iSpeed = MML->GetInt();

	cData = MML->GetChar();
	if(cData != ','){
		if( (iSpeed < 0) || (iSpeed > 255) ){
			MML->Err(L"sコマンドは0～255の範囲で指定してください。");
		}
		MML->Back();
		_data = (unsigned char)iSpeed;
	} else {
		if( (iSpeed < 0) || (iSpeed > 15) ){
			MML->Err(L"sコマンドの第1パラメータは0～15の範囲で指定してください。");
		}
		iDepth = MML->GetInt();
		if( (iDepth < 0) || (iDepth > 15) ){
			MML->Err(L"sコマンドの第2パラメータは0～15の範囲で指定してください。");
		}
		_data = (unsigned char)(((iSpeed & 0x0F) << 4) | (iDepth & 0x0F));
	}

	SetEvent(new mml_general(nsd_Sweep, _data, L"Sweep"));
}

//==============================================================
//		
//--------------------------------------------------------------
//	●引数
//		MMLfile*	MML		MMLファイルのオブジェクト
//	●返値
//		無し
//==============================================================
void	TrackSet::SetPoke(MMLfile* MML)
{
	unsigned	int	addr;
	unsigned	int	data;
	unsigned	char	cData;

	addr = MML->GetInt();
	if( (addr < 0x0000) || (addr > 0xFFFF) ){
		MML->Err(L"yコマンドのアドレスは$0000～$FFFFの範囲で指定して下さい。");
	}

	cData = MML->GetChar();
	if(cData != ','){
		MML->Err(L"yコマンドのパラメータが足りません。２つ指定してください。");
	}

	data = MML->GetInt();
	if(data > 255){
		MML->Err(L"yコマンドのデータは$00～$FFの範囲で指定して下さい。");
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
		SetEvent(new mml_general(nsd_FDS_Frequency,c0,c1,L"FDS career frequency"));
	} else {
		MML->Err(L"FDSのキャリア周波数は0～4095の範囲で指定して下さい。");
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
		SetEvent(new mml_general(nsd_FDS_Volume,(unsigned char)i,L"FDS Master volume"));
	} else {
		MML->Err(L"FDSのマスター音量は0～3の範囲で指定して下さい。");
	}
}

//==============================================================
//		
//--------------------------------------------------------------
//	●引数
//		MMLfile*	MML		MMLファイルのオブジェクト
//	●返値
//		無し
//==============================================================
void	TrackSet::SetVRC7_Write(MMLfile* MML)
{
	unsigned	char	cData;

				int		_Reg;
				int		_Dat;

	_Reg = MML->GetInt();
	if( (_Reg < 0) || (_Reg > 0x40) ){
		MML->Err(L"VRC7レジスタ操作の第1パラメータは0～63の範囲で指定してください。");
	}

	cData = MML->GetChar();
	if(cData != ','){
		MML->Err(L"P コマンドのパラメータが足りません。４つ指定してください。");
	}

	_Dat = MML->GetInt();
	if( (_Dat < 0) || (_Dat > 255) ){
		MML->Err(L"VRC7レジスタ操作の第2パラメータは0～255の範囲で指定してください。");
	}

	SetEvent(new mml_general(nsc_VRC7_reg,(unsigned char)_Reg,(unsigned char)_Dat, L"VRC7 Register Write"));
}

//==============================================================
//		NC	n163チャンネル数設定
//--------------------------------------------------------------
//	●引数
//		MMLfile*	MML		MMLファイルのオブジェクト
//	●返値
//				無し
//==============================================================
void	TrackSet::SetN163Channel(MMLfile* MML)
{
	int	i = MML->GetInt();

	if( (i <= 8) && (i >=1) ){
		SetEvent(new mml_general(nsc_N163_Channel,(unsigned char)(i-1),L"n163 channel"));
	} else {
		MML->Err(L"n163のチャンネル数は1～8の範囲で指定してください。");
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
		SetEvent(new mml_general(nsc_FME7_frequency,c0,c1,L"FME7 envelop frequency"));
	} else {
		MML->Err(L"SUNSOFT 5bのハードウェアエンベロープ周波数は0～65535の範囲で指定して下さい。");
	}
}

