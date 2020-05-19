/*******************************************************************************

			NES Sound Driver & Library	(NSD.lib)	MML Compiler

	Copyright (c) 2012 A.Watanabe (S.W.), All rights reserved.
	 For conditions of distribution and use, see copyright notice in "nsc.cpp".

*******************************************************************************/

#include "StdAfx.h"
#include "mml_repeat.h"

//==============================================================
//		コンストラクタ（for Repert A）
//--------------------------------------------------------------
//	●引数
//		const		char	_strName[]	クラスの名前
//		unsigned	char	count		リピート回数
//	●返値
//				無し
//==============================================================
mml_repeat::mml_repeat(size_t _id, unsigned char count, const _CHAR _strName[]):
	MusicEvent(_id, _strName)
{
	iSize = 2;
	code.resize(iSize);
	code[0] = nsd_Repeat_A_Start;
	code[1] = count;
}

//==============================================================
//		コンストラクタ（for Repert B）
//--------------------------------------------------------------
//	●引数
//		const		char	_strName[]	クラスの名前
//	●返値
//				無し
//==============================================================
mml_repeat::mml_repeat(size_t _id, const _CHAR _strName[]):
	MusicEvent(_id, _strName)
{
	iSize = 1;
	code.resize(iSize);
	code[0] = nsd_Repeat_B_Start;
}

//==============================================================
//		デストラクタ
//--------------------------------------------------------------
//	●引数
//				無し
//	●返値
//				無し
//==============================================================
mml_repeat::~mml_repeat(void)
{
}

//==============================================================
//		回数設定
//--------------------------------------------------------------
//	●引数
//		unsigned	char	count	リピート回数
//	●返値
//				無し
//==============================================================
void	mml_repeat::set_count(unsigned char count)
{
	if(	iSize != 2){
		_CERR << _T("mml_repeat::set_count()関数で、エラーが発生しました。") << endl;
		nsc_exit(EXIT_FAILURE);
	}
	code[1] = count;
}
//==============================================================
//		回数設定
//--------------------------------------------------------------
//	●引数
//				無し
//	●返値
//		unsigned	char	count	リピート回数
//==============================================================
unsigned	char	mml_repeat::get_count(void)
{
	unsigned	char	count;

	if(	iSize != 2){
		_CERR << _T("mml_repeat::set_count()関数で、エラーが発生しました。") << endl;
		nsc_exit(EXIT_FAILURE);
	}
	count = code[1];
	return(count);
}
