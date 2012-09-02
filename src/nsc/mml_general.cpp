#include "StdAfx.h"
#include ".\mml_general.h"

//==============================================================
//		コンストラクタ（引数0Byte）
//--------------------------------------------------------------
//	●引数
//		unsigned	int		code
//		const		char	_strName[]	クラスの名前
//	●返値
//				無し
//==============================================================
mml_general::mml_general(unsigned char _code, const char _strName[]):
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
//		unsigned	int		code
//		MMLfile*			MML
//		const		char	_strName[]	クラスの名前
//	●返値
//				無し
//==============================================================
mml_general::mml_general(unsigned char _code, MMLfile* MML, const char _strName[]="General"):
	MusicEvent(_strName)
{
	int		_data = MML->GetInt();

	if( (_data<-128) || (_data>255) ){
		MML->Err("パラメータの値が範囲を越えました。");
	}

	iSize = 2;
	code.resize(iSize);
	code[0] = _code;
	code[1] = (_data & 0xFF);
}

//==============================================================
//			コンストラクタ（引数1Byte）
//--------------------------------------------------------------
//	●引数
//		unsigned	int		code
//		const		char	_data
//		const		char	_strName[]	クラスの名前
//	●返値
//				無し
//==============================================================
mml_general::mml_general(unsigned char _code, const char _data, const char _strName[]/*="General"*/):
	MusicEvent(_strName)
{
	iSize = 2;
	code.resize(iSize);
	code[0] = _code;
	code[1] = _data;
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
