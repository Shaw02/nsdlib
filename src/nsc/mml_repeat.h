#pragma once
#include "musicevent.h"

/****************************************************************/
/*																*/
/*			クラス定義											*/
/*																*/
/****************************************************************/
class mml_repeat :
	public MusicEvent
{
//メンバー関数
public:
	mml_repeat(const wchar_t _strName[]=L"Repeat(A) Start");
	~mml_repeat(void);
	void	set_count(unsigned char count);
};
