#pragma once
#include "musicevent.h"

/****************************************************************/
/*																*/
/*			クラス定義											*/
/*																*/
/****************************************************************/
class mml_general :
	public MusicEvent
{
//メンバー関数
public:
	mml_general(unsigned char _code, const wchar_t _strName[]=L"General");
	mml_general(unsigned char _code, MMLfile* MML, const wchar_t _strName[]=L"General");
	mml_general(unsigned char _code, const char _data, const wchar_t _strName[]=L"General");
	mml_general(unsigned char _code, const char _n1, const char _n2, const wchar_t _strName[]=L"General");
	mml_general(unsigned char _code, const char _n1, const char _n2, const char _n3, const char _n4, const wchar_t _strName[]=L"General");
	~mml_general(void);
};
