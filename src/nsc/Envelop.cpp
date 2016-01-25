/*******************************************************************************

			NES Sound Driver & Library	(NSD.lib)	MML Compiler

	Copyright (c) 2012 A.Watanabe (S.W.), All rights reserved.
	 For conditions of distribution and use, see copyright notice in "nsc.cpp".

*******************************************************************************/

#include "StdAfx.h"
#include "Envelop.h"

//==============================================================
//		コンストラクタ
//--------------------------------------------------------------
//	●引数
//		MMLfile*			MML			MMLファイルのオブジェクト
//		unsigned	int		_id			エンベロープ番号
//		const		_CHAR	_strName[]	オブジェクト名
//	●返値
//					無し
//==============================================================
Envelop::Envelop(MMLfile* MML, unsigned int _id, const _CHAR _strName[]):
	MusicItem(_id, _strName),
	f_Use(false),
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
		{	"ｄ",	Env_Hold	},
		{	"Ｄ",	Env_Hold	},
		{	"|",	Env_Loop	},
		{	"｜",	Env_Loop	},
		{	"l",	Env_Loop	},
		{	"L",	Env_Loop	},
		{	"ｌ",	Env_Loop	},
		{	"Ｌ",	Env_Loop	},
		{	"r",	Env_Release	},
		{	"R",	Env_Release	},
		{	"ｒ",	Env_Release	},
		{	"Ｒ",	Env_Release	},
		{	"　",	Env_Commma	},
		{	"，",	Env_Commma	},
		{	"、",	Env_Commma	},
		{	",",	Env_Commma	}
};

	unsigned	char	cData;
				int		i;

				int		iValue	=255;	//今の設定値
				int		iLength	=0;		//ランレングス圧縮用

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
			MML->Err(_T("ブロックの開始を示す{が見つかりません。"));
		}
	}

	// } が来るまで、記述ブロック内をコンパイルする。
	while((cData = MML->GetChar()) != '}'){
		
		// } が来る前に、[EOF]が来たらエラー
		if( MML->eof() ){
			MML->Err(_T("ブロックの終端を示す`}'がありません。"));
		}

		//１つ戻る
		MML->Back();


		//各コマンド毎の処理
		switch(MML->GetCommandID(Command, sizeof(Command)/sizeof(Command_Info))){

			case(Env_Num):
				MML->Back();
				i = MML->GetInt();
				if( (i<-64) || (i>127)){
					MML->Err(_T("エンベロープは-64～127の範囲で指定して下さい。"));
				}

				if(iValue == i){
					iLength++;		//同じだったら、ランレングス圧縮する
				} else {
					if(iLength>0){
						setHold(iLength-1);	//今までの時間を加算する。
						iLength	= 0;		//ランレングス圧縮初期化
					}
					iValue	= i;

					code.append((char)1, (char)i & 0x7F);
					ptEnvelop++;
				}
				break;

			case(Env_Sweep):
				if(iLength>0){
					setHold(iLength-1);
					iLength	= 0;	//ランレングス圧縮初期化
				}
				iValue = sweep(MML);
				break;

			case(Env_Hold):
				i = MML->GetInt();
				if( (i<0) || (i>255)){
					MML->Err(_T("維持時間は0～255の範囲で指定して下さい。"));
				}
			//	setHold(i + iLength);	//今までの時間を加算する。
				iLength += i+1;			//ランレングスに加算する。
				break;

			case(Env_Loop):
				iValue	=255;			//ランレングス圧縮初期化
				if(iLength>0){
					setHold(iLength-1);	//ホールド時間出力
					iLength	= 0;		//ランレングス圧縮初期化
				}
				if(Release == false){
					if(ptEnvelop > 0x3F){
						MML->Err(_T("ループ位置を指定できる範囲(64Byte)を超えました。"));
					}
					Loop_Normal		= ptEnvelop;
				} else {
					if(ptEnvelop > 0x3F){
						MML->Err(_T("リリース時のループ位置を指定できる範囲(64Byte)を超えました。"));
					}
					Loop_Release	= ptEnvelop;
				}
				break;

			case(Env_Release):
				iValue	=255;			//ランレングス圧縮初期化
				if(iLength>0){
					setHold(iLength-1);	//ホールド時間出力
					iLength	= 0;		//ランレングス圧縮初期化
				}
				SetLoop(MML, Loop_Normal);
				code[0] = (unsigned char)ptEnvelop;
				Release = true;
				break;

			case(Env_Commma):
				break;

			//unknown command
			default:
				MML->Err(_T("unknown command"));
				break;
		}
	}

	iValue	=255;			//ランレングス圧縮初期化
	if(iLength>0){
		setHold(iLength-1);	//ホールド時間出力
		iLength	= 0;		//ランレングス圧縮初期化
	}

	if(Release == true){
		SetLoop(MML, Loop_Release);
	} else {
		SetLoop(MML, Loop_Normal);
	}

	if(code.size() > 256){
		MML->Err(_T("エンベロープの定義長が255Byteを越えました。"));
	}

	iSize = code.size();
}

void Envelop::SetLoop(MMLfile* MML, int LoopPoint)
{
	if(LoopPoint == -1){
		if(Release == true){
			MML->Warning(_T("リリース時のループポイントがありません。最後の値をループします。"));
			if(ptEnvelop > 0x3F){
				MML->Err(_T("リリース時のループ位置を指定できる範囲(63Byte)を超えました。"));
			}
		} else {
			MML->Warning(_T("ループポイントがありません。最後の値をループします。"));
			if(ptEnvelop > 0x3F){
				MML->Err(_T("ループ位置を指定できる範囲(63Byte)を超えました。"));
			}
		}
		code.append((char)1, (char)(ptEnvelop-1 | 0xC0));
	} else {
		if(ptEnvelop == Loop_Normal){
			MML->Err(_T("Lコマンドでパターン定義を終わることはできません。"));
		}
		code.append((char)1, (char)(LoopPoint | 0xC0));
	}
	ptEnvelop++;
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
		code.append((char)1, (unsigned char)0x8F);
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
	//●MML読み込み

	//
	iStart = MML->GetInt();
	if( (iStart<-64) || (iStart>127)){
		MML->Err(_T("開始点は-64～127の範囲で指定して下さい。"));
	}

	cData = MML->GetChar();
	if(cData != ','){
		MML->Err(_T("パラメータが足りません。"));
	}

	iEnd = MML->GetInt();
	if( (iEnd<-64) || (iEnd>127)){
		MML->Err(_T("終了点は-64～127の範囲で指定して下さい。"));
	}

	cData = MML->GetChar();
	if(cData != ','){
		MML->Err(_T("パラメータが足りません。"));
	}

	iLength = MML->GetInt();
	if( (iLength<1) || (iLength>255)){
		MML->Err(_T("長さは1～255の範囲で指定して下さい。"));
	}

	cData = MML->GetChar();
	if((cData != ')') && (cData != '}')){
		MML->Err(_T("パラメータが足りません。"));
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
	return(now);
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
