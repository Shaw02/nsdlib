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
	mml_general(unsigned char _code, const char _strName[]/*="General"*/);
	mml_general(unsigned char _code, MMLfile* MML, const char _strName[]/*="General"*/);
	mml_general(unsigned char _code, const char _data, const char _strName[]/*="General"*/);
	~mml_general(void);
};
