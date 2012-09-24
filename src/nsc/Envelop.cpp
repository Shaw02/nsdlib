#include "StdAfx.h"
#include ".\Envelop.h"

//==============================================================
//		コンストラクタ
//--------------------------------------------------------------
//	●引数
//		MMLfile*	MML		MMLファイルのオブジェクト
//		bool		_sub	このオブジェクトは、サブルーチン？
//	●返値
//					無し
//==============================================================
Envelop::Envelop(MMLfile* MML, unsigned int _id, const char _strName[]):
	MusicItem(_strName),
	m_id(_id)
{
	//----------------------
	//Local変数


//	定数定義
enum	Command_ID_mml {
	Env_Num,
	Env_Sweep,
	Env_Hold,
	Env_Loop,
	Env_Release,
	Env_Commma
};

//	これらは、MML構文で使えるコマンド。
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
		{	"d",	Env_Hold	},
		{	"D",	Env_Hold	},
		{	"|",	Env_Loop	},
		{	"l",	Env_Loop	},
		{	"L",	Env_Loop	},
		{	"r",	Env_Release	},
		{	"R",	Env_Release	},
		{	",",	Env_Commma	}
};

	unsigned	char	cData;
				int		i;
				int		Loop_Normal		= -1;
				int		Loop_Release	= -1;
				bool	Release			= false;
	unsigned	int		ptEnvelop		= 1;

	//------------------------------
	//クラスの初期設定


	//------------------------------
	//コンパイル

	// { の検索
	while(MML->cRead() != '{'){
		if(MML->eof()){
			MML->Err("ブロックの開始を示す{が見つかりません。");
		}
	}

	code.resize(0);
	code.append((char)1, (char)0);

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

			case(Env_Num):
				MML->Back();
				i = MML->GetInt();
				if( (i<-64) || (i>63)){
					MML->Err("エンベロープは、-64～63の範囲で指定して下さい。");
				}
				code.append((char)1, (char)i & 0x7F);
				ptEnvelop++;
				break;

			case(Env_Sweep):
				break;

			case(Env_Hold):
				i = MML->GetInt();
				if( (i<0) || (i>255)){
					MML->Err("維持時間は、0～255の範囲で指定して下さい。");
				}
				while(i>15){
					code.append((char)1, (char)0x8F);
					i -= 16;		//15を書けば、16フレーム維持
				ptEnvelop++;
				}
				code.append((char)1, (char)((i & 0x0F) | 0x80));
				ptEnvelop++;
				break;

			case(Env_Loop):
				if(ptEnvelop > 0x3F){
					MML->Err("ループ位置を指定できる範囲を超えました。");
				}
				if(Release == false){
					Loop_Normal		= ptEnvelop;
				} else {
					Loop_Release	= ptEnvelop;
				}
				break;

			case(Env_Release):
				if(Loop_Normal == -1){
					MML->Warning("ループポイントがありません。最後の値をループします。");
					code.append((char)1, (char)(ptEnvelop-1 | 0xC0));
				} else {
					if(ptEnvelop == Loop_Normal){
						MML->Err("Lコマンドの直後にRコマンドを置くことはできません。");
					}
					code.append((char)1, (char)(Loop_Normal | 0xC0));
				}
				ptEnvelop++;
				code[0] = ptEnvelop;
				Release = true;
				break;

			case(Env_Commma):
				break;

			//unknown command
			default:
				MML->Err("unknown command");
				break;
		}
	}

	if(Release == true){
		if(Loop_Release == -1){
			MML->Warning("リリース時のループポイントがありません。最後の値をループします。");
			code.append((char)1, (char)(ptEnvelop-1 | 0xC0));
		} else {
			if(ptEnvelop == Loop_Release){
				MML->Err("Lコマンドでパターン定義を終わることはできません。");
			}
			code.append((char)1, (char)(Loop_Release | 0xC0));
		}
	} else {
		if(Loop_Normal == -1){
			MML->Warning("ループポイントがありません。最後の値をループします。");
			code.append((char)1, (char)(ptEnvelop-1 | 0xC0));
		} else {
			if(ptEnvelop == Loop_Normal){
				MML->Err("Lコマンドでパターン定義を終わることはできません。");
			}
			code.append((char)1, (char)(Loop_Normal | 0xC0));
		}
	}

	iSize = code.size();
}

//==============================================================
//		デストラクタ
//--------------------------------------------------------------
//	●引数
//				無し
//	●返値
//				無し
//==============================================================
Envelop::~Envelop(void)
{
}
//==============================================================
//		コードの取得
//--------------------------------------------------------------
//	●引数
//				無し
//	●返値
//				無し
//==============================================================
void	Envelop::getAsm(MusicFile* MUS)
{
	*MUS << MUS->Header.Label.c_str() << "Envelope" << m_id << ":" << endl;
	MusicItem::getAsm(MUS);
}
