#include "StdAfx.h"
#include "Envelop.h"

//==============================================================
//		コンストラクタ
//--------------------------------------------------------------
//	●引数
//		MMLfile*			MML			MMLファイルのオブジェクト
//		unsigned	int		_id			エンベロープ番号
//		const		wchar_t	_strName[]	オブジェクト名
//	●返値
//					無し
//==============================================================
Envelop::Envelop(MMLfile* MML, unsigned int _id, const wchar_t _strName[]):
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
		{	"(",	Env_Sweep	},
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

	//------------------------------
	//クラスの初期設定

	Loop_Normal		= -1;
	Loop_Release	= -1;
	Release			= false;
	ptEnvelop		= 1;

	code.resize(0);
	code.append((char)1, (char)0);



	//------------------------------
	//コンパイル

	// { の検索
	while(MML->cRead() != '{'){
		if(MML->eof()){
			MML->Err(L"ブロックの開始を示す{が見つかりません。");
		}
	}

	// } が来るまで、記述ブロック内をコンパイルする。
	while((cData = MML->GetChar()) != '}'){
		
		// } が来る前に、[EOF]が来たらエラー
		if( MML->eof() ){
			MML->Err(L"ブロックの終端を示す`}'がありません。");
		}

		//１つ戻る
		MML->Back();


		//各コマンド毎の処理
		switch(MML->GetCommandID(Command, sizeof(Command)/sizeof(Command_Info))){

			case(Env_Num):
				MML->Back();
				i = MML->GetInt();
				if( (i<-64) || (i>127)){
					MML->Err(L"エンベロープは、-64～127の範囲で指定して下さい。");
				}
				code.append((char)1, (char)i & 0x7F);
				ptEnvelop++;
				break;

			case(Env_Sweep):
				sweep(MML);
				break;

			case(Env_Hold):
				i = MML->GetInt();
				if( (i<0) || (i>255)){
					MML->Err(L"維持時間は、0～255の範囲で指定して下さい。");
				}
				setHold(i);
				break;

			case(Env_Loop):
				if(ptEnvelop > 0x3F){
					MML->Err(L"ループ位置を指定できる範囲を超えました。");
				}
				if(Release == false){
					Loop_Normal		= ptEnvelop;
				} else {
					Loop_Release	= ptEnvelop;
				}
				break;

			case(Env_Release):
				if(Loop_Normal == -1){
					MML->Warning(L"ループポイントがありません。最後の値をループします。");
					code.append((char)1, (char)(ptEnvelop-1 | 0xC0));
				} else {
					if(ptEnvelop == Loop_Normal){
						MML->Err(L"Lコマンドの直後にRコマンドを置くことはできません。");
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
				MML->Err(L"unknown command");
				break;
		}
	}

	if(Release == true){
		if(Loop_Release == -1){
			MML->Warning(L"リリース時のループポイントがありません。最後の値をループします。");
			code.append((char)1, (char)(ptEnvelop-1 | 0xC0));
		} else {
			if(ptEnvelop == Loop_Release){
				MML->Err(L"Lコマンドでパターン定義を終わることはできません。");
			}
			code.append((char)1, (char)(Loop_Release | 0xC0));
		}
	} else {
		if(Loop_Normal == -1){
			MML->Warning(L"ループポイントがありません。最後の値をループします。");
			code.append((char)1, (char)(ptEnvelop-1 | 0xC0));
		} else {
			if(ptEnvelop == Loop_Normal){
				MML->Err(L"Lコマンドでパターン定義を終わることはできません。");
			}
			code.append((char)1, (char)(Loop_Normal | 0xC0));
		}
	}

	if(code.size() > 256){
		MML->Err(L"エンベロープの定義長が256Byteを越えました。");
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
//		維持時間のセット
//--------------------------------------------------------------
//	●引数
//		int		length		維持時間
//	●返値
//				無し
//==============================================================
void	Envelop::setHold(int length)
{
	while(length>15){
		code.append((char)1, (char)0x8F);
		ptEnvelop++;
		length -= 16;		//15を書けば、16フレーム維持
	}
	code.append((char)1, (char)((length & 0x0F) | 0x80));
	ptEnvelop++;
}

//==============================================================
//		スイープ
//--------------------------------------------------------------
//	●引数
//		MMLfile*	MML		MMLファイルのオブジェクト
//	●返値
//				無し
//==============================================================
void	Envelop::sweep(MMLfile* MML)
{
	unsigned	char	cData;

	int		iStart;
	int		iEnd;
	int		iLength;
	int		iDelta;

	int		now;
	int		cnt;

	int		i=0;
	int		temp;

	//--------------------------
	//●MML読み込み

	//
	iStart = MML->GetInt();
	if( (iStart<-64) || (iStart>127)){
		MML->Err(L"開始点は、-64～127の範囲で指定して下さい。");
	}

	cData = MML->GetChar();
	if(cData != ','){
		MML->Err(L"パラメータが足りません。");
	}

	iEnd = MML->GetInt();
	if( (iEnd<-64) || (iEnd>127)){
		MML->Err(L"終了点は、-64～127の範囲で指定して下さい。");
	}

	cData = MML->GetChar();
	if(cData != ','){
		MML->Err(L"パラメータが足りません。");
	}

	iLength = MML->GetInt();
	if( (iLength<1) || (iLength>255)){
		MML->Err(L"長さは、1～255の範囲で指定して下さい。");
	}

	cData = MML->GetChar();
	if((cData != ')') && (cData != '}')){
		MML->Err(L"パラメータが足りません。");
	}

	//--------------------------
	//●テーブル作成

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

}

//==============================================================
//		コードの取得
//--------------------------------------------------------------
//	●引数
//		MusicFile*	MUS		コードを出力する曲データファイル・オブジェクト
//	●返値
//				無し
//==============================================================
void	Envelop::getAsm(MusicFile* MUS)
{
	*MUS << MUS->Header.Label.c_str() << "Envelope" << m_id << ":" << endl;
	MusicItem::getAsm(MUS);
}
