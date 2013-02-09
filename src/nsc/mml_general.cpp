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
mml_general::mml_general(unsigned char _code, const wchar_t _strName[]):
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
mml_general::mml_general(unsigned char _code, MMLfile* MML, const wchar_t _strName[]):
	MusicEvent(_strName)
{
	int		_data = MML->GetInt();

	if( (_data<-128) || (_data>255) ){
		MML->Err(L"パラメータの値が範囲を越えました。");
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
//		unsigned	char	_code		コード
//		const		char	_data		引数
//		const		char	_strName[]	クラスの名前
//	●返値
//				無し
//==============================================================
mml_general::mml_general(unsigned char _code, const char _data, const wchar_t _strName[]):
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
mml_general::mml_general(unsigned char _code, const char _n1, const char _n2, const wchar_t _strName[]):
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
mml_general::mml_general(unsigned char _code, const char _n1, const char _n2, const char _n3, const char _n4, const wchar_t _strName[]):
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
