#include "StdAfx.h"
#include ".\trackset.h"

//==============================================================
//		コンストラクタ
//--------------------------------------------------------------
//	●引数
//		MMLfile*	MML		MMLファイルのオブジェクト
//		bool		_sub	このオブジェクトは、サブルーチン？
//	●返値
//					無し
//==============================================================
TrackSet::TrackSet(MMLfile* MML, unsigned int _id, bool _sub, const char _strName[]):
	MusicItem(_strName),
	m_id(_id)
{
	//----------------------
	//Local変数

//	定数定義
enum	Command_ID_mml {
	mml_Track,
	mml_KeySignature,
	mml_Macro,
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

	mml_La,
	mml_Si,
	mml_Do,
	mml_Re,
	mml_Mi,
	mml_Fa,
	mml_Sol,
	mml_Rest,
	mml_Tai,

	mml_Length,
	mml_Gate_q,
	mml_Gate_u,

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
	mml_VRC7,
	mml_N163,
	mml_Voice,
	mml_N163_Channel,

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
		{	"TR",	mml_Track				},
		{	"K",	mml_KeySignature,		},
		{	"S",	mml_Subroutine			},
		{	"$",	mml_Macro,				},

		{	"L",	mml_Loop				},
		{	"|:",	mml_Repeat_B_Start		},
		{	"\\",	mml_Repeat_B_Branch		},
		{	":|",	mml_Repeat_B_End		},
		{	"[",	mml_Repeat_A_Start		},
		{	":",	mml_Repeat_A_Branch		},
		{	"]",	mml_Repeat_A_End		},

		{	"t_",	mml_Tempo_Relative,		},
		{	"t",	mml_Tempo,				},

		{	"a",	mml_La					},
		{	"b",	mml_Si					},
		{	"c",	mml_Do					},
		{	"d",	mml_Re					},
		{	"e",	mml_Mi					},
		{	"f",	mml_Fa					},
		{	"g",	mml_Sol					},
		{	"r",	mml_Rest				},
		{	"^",	mml_Tai					},

		{	"l",	mml_Length				},
		{	"q",	mml_Gate_q				},
		{	"u",	mml_Gate_u				},

		{	"E@*",	mml_Envelop_Off_Voice		},
		{	"Ev*",	mml_Envelop_Off_Volume		},
		{	"Em*",	mml_Envelop_Off_Frequency	},
		{	"En*",	mml_Envelop_Off_Note		},

		{	"E@",	mml_Envelop_Voice		},
		{	"Ev",	mml_Envelop_Volume		},
		{	"Em",	mml_Envelop_Frequency	},
		{	"En",	mml_Envelop_Note		},

		{	"Rm",	mml_Release_mdoe		},
		{	"R@",	mml_Release_Voice		},
		{	"Rv",	mml_Release_Volume		},

		{	"@FC",	mml_FDSC				},
		{	"@FM",	mml_FDSM				},
		{	"@FF",	mml_FDSF				},
		{	"@V",	mml_VRC7				},
		{	"@N",	mml_N163				},
		{	"@",	mml_Voice				},
		{	"NC",	mml_N163_Channel		},

		{	"o",	mml_Octave				},
		{	">",	mml_Octave_Up			},
		{	"<",	mml_Octave_Down			},
		{	"`",	mml_Octave_Up1			},
		{	"\"",	mml_Octave_Down1		},
		{	"D%",	mml_Detune_Register		},
		{	"D",	mml_Detune_Cent			},
		{	"__",	mml_Transpose_Relative	},
		{	"_",	mml_Transpose			},
		{	"P",	mml_Protament			},
		{	"s",	mml_Sweep				},

		{	"v",	mml_Volume				},
		{	")",	mml_Volume_Up			},
		{	"(",	mml_Volume_Down			},

		{	"yV",	mml_VRC7_Write			},
		{	"y",	mml_Memory_Write		},

		{	"|",	mml_Bar					}
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



	//------------------------------
	//コンパイル

	// { の検索
	while(MML->cRead() != '{'){
		if(MML->eof()){
			MML->Err("ブロックの開始を示す{が見つかりません。");
		}
	}

	// } が来るまで、記述ブロック内をコンパイルする。
	while((cData = MML->GetChar()) != '}'){
		
		// } が来る前に、[EOF]が来たらエラー
		if( MML->eof() ){
			MML->Err("ブロックの終端を示す`}'がありません。");
		}

		//１つ戻る
		MML->Back();


		//各コマンド毎の処理
		switch(MML->GetCommandID(Command, sizeof(Command)/sizeof(Command_Info))){

			case(mml_Track):
				if(fSub == true){
					MML->Warning("Subブロックないではトラック指定はできません。無視します。");
				}
				iTrack = MML->GetInt() - 1;
				if( (iTrack < 0) ){
					MML->Err("トラック番号で指定できる範囲を超えています。");
				}
				nowTrack = getTrack(iTrack);
				break;

			case(mml_KeySignature):
				nowTrack->SetKeySignature(MML);
				break;

			case(mml_Macro):
				//■ to do 
				break;

			case(mml_Subroutine):
				nowTrack->SetSubroutine(MML);
				break;

			case(mml_Loop):
				if(fSub == true){
					MML->Warning("Subブロックないでは無限ループはできません。無視します。");
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
				nowTrack->SetRepeat_B_Start(MML);
				break;

			case(mml_Repeat_B_Branch):
				nowTrack->SetRepeat_B_Branch(MML);
				break;

			case(mml_Repeat_B_End):
				nowTrack->SetRepeat_B_End(MML);
				break;

			case(mml_Tempo):
				nowTrack->SetEvent(new mml_general(nsd_Tempo, MML, "Tempo"));
				break;

			case(mml_Tempo_Relative):
				nowTrack->SetEvent(new mml_general(nsd_Relative_Tempo, MML, "Relative Tempo"));
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
				nowTrack->SetRest(MML);
				break;

			case(mml_Tai):
				nowTrack->SetTai(MML);
				break;

			case(mml_Length):
				nowTrack->SetLength(MML);
				break;

			case(mml_Gate_q):
				nowTrack->SetGatetime(MML);
				break;

			case(mml_Gate_u):
				nowTrack->SetGatetime_u(MML);
				break;

			case(mml_Envelop_Voice):
				nowTrack->SetEnvelop(nsd_Envelop_Voice, MML);
				break;

			case(mml_Envelop_Volume):
				nowTrack->SetEnvelop(nsd_Envelop_Volume, MML);
				break;

			case(mml_Envelop_Frequency):
				nowTrack->SetEnvelop(nsd_Envelop_Frequency, MML);
				break;

			case(mml_Envelop_Note):
				nowTrack->SetEnvelop(nsd_Envelop_Note, MML);
				break;

			case(mml_Envelop_Off_Voice):
				MML->Err("音色エンベロープは、@コマンドで無効にできます。");
				break;

			case(mml_Envelop_Off_Volume):
				nowTrack->SetEvent(new mml_Address(nsd_Envelop_Volume));
				break;

			case(mml_Envelop_Off_Frequency):
				nowTrack->SetEvent(new mml_Address(nsd_Envelop_Frequency));
				break;

			case(mml_Envelop_Off_Note):
				nowTrack->SetEvent(new mml_Address(nsd_Envelop_Note));
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

			case(mml_FDSC):
				//■■■ to do
				break;

			case(mml_FDSM):
				//■■■ to do
				break;

			case(mml_FDSF):
				//■■■ to do
				break;

			case(mml_VRC7):
				nowTrack->SetVRC7(MML);
				break;

			case(mml_Voice):
				nowTrack->SetEvent(new mml_general(nsd_Voice, MML, "Voice"));
				break;

			case(mml_N163):
				nowTrack->SetN163(MML);
				break;

			case(mml_N163_Channel):
				nowTrack->SetN163Channel(MML);
				break;

			case(mml_Octave):
				nowTrack->SetOctave(MML);
				break;

			case(mml_Octave_Up):
				if(MML->octave_reverse == true){
					nowTrack->SetEvent(new mml_general(nsd_Octave_Down, "Octave Down"));
				} else {
					nowTrack->SetEvent(new mml_general(nsd_Octave_Up, "Octave Up"));
				}
				break;

			case(mml_Octave_Down):
				if(MML->octave_reverse == true){
					nowTrack->SetEvent(new mml_general(nsd_Octave_Up, "Octave Up"));
				} else {
					nowTrack->SetEvent(new mml_general(nsd_Octave_Down, "Octave Down"));
				}
				break;

			case(mml_Octave_Up1):
				nowTrack->SetEvent(new mml_general(nsd_Octave_Up_1, "One time octave up"));
				break;

			case(mml_Octave_Down1):
				nowTrack->SetEvent(new mml_general(nsd_Octave_Down_1, "One time octave down"));
				break;

			case(mml_Detune_Cent):
				nowTrack->SetEvent(new mml_general(nsd_Detune_Cent, MML, "Detune Cent"));
				break;

			case(mml_Detune_Register):
				nowTrack->SetEvent(new mml_general(nsd_Derune_Register, MML, "Derune Register"));
				break;

			case(mml_Transpose):
				nowTrack->SetEvent(new mml_general(nsd_Transpose, MML, "Transpose"));
				break;

			case(mml_Transpose_Relative):
				nowTrack->SetEvent(new mml_general(nsd_Relative_Transpose, MML, "Relative Transpose"));
				break;

			case(mml_Protament):
				nowTrack->SetProtament(MML);
				break;

			case(mml_Sweep):
				nowTrack->SetSweep(MML);
				break;

			case(mml_Volume):
				nowTrack->SetVolume(MML);
				break;

			case(mml_Volume_Up):
				nowTrack->SetEvent(new mml_general(nsd_Volume_Up, "Volume up"));
				break;

			case(mml_Volume_Down):
				nowTrack->SetEvent(new mml_general(nsd_Volume_Down, "Volume down"));
				break;

			case(mml_VRC7_Write):
				nowTrack->SetVRC7_Write(MML);
				break;
			case(mml_Memory_Write):
				nowTrack->SetPoke(MML);
				break;

			case(mml_Bar):
				break;

			//unknown command
			default:
				MML->Err("unknown command");
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

		code[0] = maxTrack + 1;				//トラック数
		code[1] = 0;						//

		//各トラックに終端を書いて、曲データのアドレス情報を作成
		iTrack = 0;
		while(iTrack <= maxTrack){
			code[iTrack *2 + 2]	= ((i   ) & 0xFF);
			code[iTrack *2 + 3]	= ((i>>8) & 0xFF);
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
//		アドレス情報を決定する。
//--------------------------------------------------------------
//	●引数
//				無し
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
//		トラックの作成
//--------------------------------------------------------------
//	●引数
//		unsigned int _track		トラック番号
//	●返値
//		MusicTrack*				作ったトラック・オブジェクトのポインタ
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
//		unsigned int _track		トラック番号
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
			cout << "MusicTrack* TrackSet::getTrack()関数でエラーが発生しました。" << endl;
			exit(-1);
		}
	}
	maxTrack = i;	//トラックの最大値を記憶。

	//ポインタを取得する。
	_getTrack = ptcTrack[_track];

	return(_getTrack);
}
//==============================================================
//		コードの取得
//--------------------------------------------------------------
//	●引数
//				無し
//	●返値
//				無し
//==============================================================
void	TrackSet::getAsm(MusicFile* MUS)
{
	//----------------------
	//Local変数
	unsigned	int	i = 0;
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
