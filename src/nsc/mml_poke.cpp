#include "StdAfx.h"
#include "mml_poke.h"

//==============================================================
//		コンストラクタ
//--------------------------------------------------------------
//	●引数
//		unsigned	int		_addr		アドレス
//		unsigned	char	_data		データ
//		const		char	_strName[]	クラスの名前
//	●返値
//				無し
//==============================================================
mml_poke::mml_poke(unsigned int _addr , unsigned char _data, const wchar_t _strName[]):
	MusicEvent(_strName)
{
	iSize = 4;
	code.resize(iSize);
	code[0] = nsd_Poke;
	code[1] = (unsigned char)(_addr     );
	code[2] = (unsigned char)(_addr >> 8);
	code[3] = _data;
}

//==============================================================
//		デストラクタ
//--------------------------------------------------------------
//	●引数
//				無し
//	●返値
//				無し
//==============================================================
mml_poke::~mml_poke(void)
{
}
