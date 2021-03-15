/*******************************************************************************

			NES Sound Driver & Library	(NSD.lib)	MML Compiler

	Copyright (c) 2012 A.Watanabe (S.W.), All rights reserved.
	 For conditions of distribution and use, see copyright notice in "nsc.cpp".

*******************************************************************************/

#include "StdAfx.h"
#include "FDSM.h"

//==============================================================
//		コンストラクタ
//--------------------------------------------------------------
//	●引数
//		MMLfile*			MML			MMLファイルのオブジェクト
//		unsigned	int		_id			FDSM番号
//		const		_CHAR	_strName[]	オブジェクト名
//	●返値
//					無し
//==============================================================
FDSM::FDSM(MMLfile* MML, size_t _id, const _CHAR _strName[]):
	MusicItem(_id, _strName)
{
	//----------------------
	//Local変数


//	定数定義
enum	Command_ID_mml {
	FDSM_Num,
	FDSM_Commma
};

//	これらは、MML構文で使えるコマンド。
const	static	Command_Info	Command[] = {
		{	"%",	FDSM_Num	},
		{	"$",	FDSM_Num	},
		{	"0",	FDSM_Num	},
		{	"1",	FDSM_Num	},
		{	"2",	FDSM_Num	},
		{	"3",	FDSM_Num	},
		{	"4",	FDSM_Num	},
		{	"5",	FDSM_Num	},
		{	"6",	FDSM_Num	},
		{	"7",	FDSM_Num	},
		{	"8",	FDSM_Num	},
		{	"9",	FDSM_Num	},
		{	",",	FDSM_Commma	}
};

				int		i;
				char	cData;
	unsigned	int		ptFDSM		= 0;
				string	WAVE;

	//------------------------------
	//クラスの初期設定
	WAVE.clear();
	code.resize(0);

	//------------------------------
	//コンパイル

	// { の検索
	MML->Chk_LeftCurlyBrace();

	// } が来るまで、記述ブロック内をコンパイルする。
	while(MML->GetChar_With_Chk_RightCurlyBrace(&cData)){

		//１つ戻る
		MML->Back();

		//各コマンド毎の処理
		switch(MML->GetCommandID(Command, sizeof(Command)/sizeof(Command_Info))){

			case(FDSM_Num):
				MML->Back();
				i = MML->GetInt_With_Chk_Range(_T("FDSMの波形パターン"),0,7);
				WAVE.append((char)1, (char)i & 0x07);
				ptFDSM++;
				break;

			case(FDSM_Commma):
				break;

			//unknown command
			default:
				MML->ErrUnknownCmd();
				break;
		}
	}
	if(ptFDSM < 32){
		MML->Err(_T("波形パターンは32個の数字を記述してください。32個に満たないです。"));
	}
	if(ptFDSM > 32){
		MML->Err(_T("波形パターンは32個の数字を記述してください。32個を超えています。"));
	}
	i = 0;
	while(ptFDSM>0){
		code.append((char)1, WAVE[i]);
		i++;
		ptFDSM--;
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
FDSM::~FDSM(void)
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
void	FDSM::getAsm(MusicFile* MUS)
{
	*MUS << MUS->Header.Label.c_str() << "FDSM" << m_id << ":" << endl;
	MusicItem::getAsm(MUS);
}
