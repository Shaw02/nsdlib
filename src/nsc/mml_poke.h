#pragma once
#include "musicevent.h"

/****************************************************************/
/*																*/
/*			クラス定義											*/
/*																*/
/****************************************************************/
class mml_poke :
	public MusicEvent
{
//メンバー関数
public:
	mml_poke(unsigned int _addr , unsigned char _data,  const _CHAR _strName[]=_T("Poke"));
	~mml_poke(void);
};
