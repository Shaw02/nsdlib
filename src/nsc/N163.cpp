/*******************************************************************************

			NES Sound Driver & Library	(NSD.lib)	MML Compiler

	Copyright (c) 2012 A.Watanabe (S.W.), All rights reserved.
	 For conditions of distribution and use, see copyright notice in "nsc.cpp".

*******************************************************************************/

#include "StdAfx.h"
#include "N163.h"

//==============================================================
//		コンストラクタ
//--------------------------------------------------------------
//	●引数
//		MMLfile*			MML			MMLファイルのオブジェクト
//					size_t	_id			N163番号
//		const		_CHAR	_strName[]	オブジェクト名
//	●返値
//					無し
//==============================================================
N163::N163(MMLfile* MML, size_t _id, const _CHAR _strName[]):
	MusicItem(_id, _strName)
{
	//----------------------
	//Local変数


//	定数定義
enum	Command_ID_mml {
	N163_Num,
	N163_Commma
};

//	これらは、MML構文で使えるコマンド。
const	static	Command_Info	Command[] = {
		{	"%",	N163_Num	},
		{	"$",	N163_Num	},
		{	"0",	N163_Num	},
		{	"1",	N163_Num	},
		{	"2",	N163_Num	},
		{	"3",	N163_Num	},
		{	"4",	N163_Num	},
		{	"5",	N163_Num	},
		{	"6",	N163_Num	},
		{	"7",	N163_Num	},
		{	"8",	N163_Num	},
		{	"9",	N163_Num	},
		{	",",	N163_Commma	}
};

				int		i;
				char	cData;
	unsigned	int		ptN163		= 0;
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

			case(N163_Num):
				MML->Back();
				i = MML->GetInt_With_Chk_Range(_T("n163の波形パターン"),0,15);
				WAVE.append((char)1, (char)i & 0x0F);
				ptN163++;
				break;

			case(N163_Commma):
				break;

			//unknown command
			default:
				MML->ErrUnknownCmd();
				break;
		}
	}
	if(ptN163 > 256){
		MML->Err(_T("サンプル長が256個を越えています。"));
	}
	if((ptN163 & 0x03) != 0){
		MML->Err(_T("サンプル長は4の倍数で記述して下さい。"));
	}
	ptN163 >>= 1;
	code.append((char)1, (unsigned char)ptN163);
	i = 0;
	while(ptN163>0){
		code.append((char)1, WAVE[i*2+0] | (WAVE[i*2+1]<<4) );
		i++;
		ptN163--;
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
N163::~N163(void)
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
void	N163::getAsm(MusicFile* MUS)
{
	*MUS << MUS->Header.Label.c_str() << "N163" << m_id << ":" << endl;
	MusicItem::getAsm(MUS);
}
