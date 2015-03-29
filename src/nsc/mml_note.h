#pragma once
#include "MusicEvent.h"

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
	mml_note(int _code, int length, int gatetime, bool slur, const _CHAR _strName[]=_T("Note"));
	~mml_note(void);
	void	SetTai(void);
};
