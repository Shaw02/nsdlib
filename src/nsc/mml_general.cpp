/*******************************************************************************

			NES Sound Driver & Library	(NSD.lib)	MML Compiler

	Copyright (c) 2012 A.Watanabe (S.W.), All rights reserved.
	 For conditions of distribution and use, see copyright notice in "nsc.cpp".

*******************************************************************************/

#include "StdAfx.h"
#include "mml_general.h"

//==============================================================
//		コンストラクタ（引数0Byte）
//--------------------------------------------------------------
//	●引数
//		unsigned	char	_code		コード
//		const		char	_strName[]	クラスの名前
//	●返値
//				無し
//==============================================================
mml_general::mml_general(unsigned char _code, const _CHAR _strName[]):
	MusicEvent(_strName)
{
	iSize = 1;
	code.resize(iSize);
	code[0] = _code;
}

//==============================================================
//		コンストラクタ（引数1Byte）
//--------------------------------------------------------------
//	●引数
//		unsigned	char	_code		コード
//		MMLfile*			MML			MMLファイルのオブジェクト
//		const		char	_strName[]	クラスの名前
//	●返値
//				無し
//==============================================================
mml_general::mml_general(unsigned char _code, MMLfile* MML, const _CHAR _strName[]):
	MusicEvent(_strName)
{
	int		_data = MML->GetInt_With_Chk_Range(_T("パラメータ"),-128,255);

	iSize = 2;
	code.resize(iSize);
	code[0] = (unsigned char)_code;
	code[1] = (unsigned char)_data;
}

//==============================================================
//			コンストラクタ（引数1Byte）
//--------------------------------------------------------------
//	●引数
//		unsigned	char	_code		コード
//		const		char	_data		引数
//		const		char	_strName[]	クラスの名前
//	●返値
//				無し
//==============================================================
mml_general::mml_general(unsigned char _code, char _data, const _CHAR _strName[]):
	MusicEvent(_strName)
{
	iSize = 2;
	code.resize(iSize);
	code[0] = _code;
	code[1] = _data;
}

//==============================================================
//			コンストラクタ（引数2Byte）
//--------------------------------------------------------------
//	●引数
//		unsigned	char	_code		コード
//		const		char	_n1			引数1
//		const		char	_n2			引数2
//		const		char	_strName[]	クラスの名前
//	●返値
//				無し
//==============================================================
mml_general::mml_general(unsigned char _code, char _n1, char _n2, const _CHAR _strName[]):
	MusicEvent(_strName)
{
	iSize = 3;
	code.resize(iSize);
	code[0] = _code;
	code[1] = _n1;
	code[2] = _n2;
}

//==============================================================
//			コンストラクタ（引数4Byte）
//--------------------------------------------------------------
//	●引数
//		unsigned	char	_code		コード
//		const		char	_n1			引数1
//		const		char	_n2			引数2
//		const		char	_n3			引数3
//		const		char	_n4			引数4
//		const		char	_strName[]	クラスの名前
//	●返値
//				無し
//==============================================================
mml_general::mml_general(unsigned char _code, char _n1, char _n2, char _n3, char _n4, const _CHAR _strName[]):
	MusicEvent(_strName)
{
	iSize = 5;
	code.resize(iSize);
	code[0] = _code;
	code[1] = _n1;
	code[2] = _n2;
	code[3] = _n3;
	code[4] = _n4;
}

//==============================================================
//		デストラクタ
//--------------------------------------------------------------
//	●引数
//				無し
//	●返値
//				無し
//==============================================================
mml_general::~mml_general(void)
{
}
