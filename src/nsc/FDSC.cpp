/*******************************************************************************

			NES Sound Driver & Library	(NSD.lib)	MML Compiler

	Copyright (c) 2012 A.Watanabe (S.W.), All rights reserved.
	 For conditions of distribution and use, see copyright notice in "nsc.cpp".

*******************************************************************************/

#include "StdAfx.h"
#include "FDSC.h"

//==============================================================
//		コンストラクタ
//--------------------------------------------------------------
//	●引数
//		MMLfile*			MML			MMLファイルのオブジェクト
//					size_t	_id			FDSC番号
//		const		_CHAR	_strName[]	オブジェクト名
//	●返値
//					無し
//==============================================================
FDSC::FDSC(MMLfile* MML, size_t _id, const _CHAR _strName[]):
	MusicItem(_id, _strName)
{
	//----------------------
	//Local変数


//	定数定義
enum	Command_ID_mml {
	FDSC_Num,
	FDSC_Commma
};

//	これらは、MML構文で使えるコマンド。
const	static	Command_Info	Command[] = {
		{	"%",	FDSC_Num	},
		{	"$",	FDSC_Num	},
		{	"0",	FDSC_Num	},
		{	"1",	FDSC_Num	},
		{	"2",	FDSC_Num	},
		{	"3",	FDSC_Num	},
		{	"4",	FDSC_Num	},
		{	"5",	FDSC_Num	},
		{	"6",	FDSC_Num	},
		{	"7",	FDSC_Num	},
		{	"8",	FDSC_Num	},
		{	"9",	FDSC_Num	},
		{	",",	FDSC_Commma	}
};

				int		i;
				char	cData;
	unsigned	int		ptFDSC		= 0;
				string	WAVE;

	//------------------------------
	//クラスの初期設定
	WAVE.clear();
	code.resize(0);

	//------------------------------
	//コンパイル

	// { の検索
	MML->ChkBlockStart();

	// } が来るまで、記述ブロック内をコンパイルする。
	while(MML->GetChar_With_ChkEOF(&cData)){

		//１つ戻る
		MML->Back();

		//各コマンド毎の処理
		switch(MML->GetCommandID(Command, sizeof(Command)/sizeof(Command_Info))){

			case(FDSC_Num):
				MML->Back();
				i = MML->GetInt();
				if( (i<0) || (i>63)){
					MML->Err(_T("FDSCの波形パターンは0～63の範囲で指定して下さい。"));
				}
				WAVE.append((char)1, (char)i & 0x3F);
				ptFDSC++;
				break;

			case(FDSC_Commma):
				break;

			//unknown command
			default:
				MML->ErrUnknownCmd();
				break;
		}
	}
	if(ptFDSC < 64){
		MML->Err(_T("波形パターンは64個の数字を記述してください。64個に満たないです。"));
	}
	if(ptFDSC > 64){
		MML->Err(_T("波形パターンは64個の数字を記述してください。64個を超えています。"));
	}
	i = 0;
	while(ptFDSC>0){
		code.append((char)1, WAVE[i]);
		i++;
		ptFDSC--;
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
FDSC::~FDSC(void)
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
void	FDSC::getAsm(MusicFile* MUS)
{
	*MUS << MUS->Header.Label.c_str() << "FDSC" << m_id << ":" << endl;
	MusicItem::getAsm(MUS);
}
