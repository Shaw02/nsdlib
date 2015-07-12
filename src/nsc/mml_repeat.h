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
class mml_repeat :
	public MusicEvent
{
//メンバー関数
public:
	mml_repeat(const _CHAR _strName[]=_T("Repeat(A) Start"));
	~mml_repeat(void);
	void	set_count(unsigned char count);
};
