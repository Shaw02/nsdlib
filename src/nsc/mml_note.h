/*******************************************************************************

			NES Sound Driver & Library	(NSD.lib)	MML Compiler

	Copyright (c) 2012 A.Watanabe (S.W.), All rights reserved.
	 For conditions of distribution and use, see copyright notice in "nsc.cpp".

*******************************************************************************/

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
