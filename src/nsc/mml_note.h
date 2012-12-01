#pragma once
#include "musicevent.h"

/****************************************************************/
/*																*/
/*			クラス定義											*/
/*																*/
/****************************************************************/
class mml_note :
	public MusicEvent
{
//メンバー関数
public:
	mml_note(int _code, int length, int gatetime, bool slur, const wchar_t _strName[]=L"Note");
	~mml_note(void);
	void	SetTai(void);
};
