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
class mml_general :
	public MusicEvent
{
//メンバー関数
public:
	mml_general(unsigned char _code, const _CHAR _strName[]=_T("General"));
	mml_general(unsigned char _code, MMLfile* MML, const _CHAR _strName[]=_T("General"));
	mml_general(unsigned char _code, char _data, const _CHAR _strName[]=_T("General"));
	mml_general(unsigned char _code, char _n1, char _n2, const _CHAR _strName[]=_T("General"));
	mml_general(unsigned char _code, char _n1, char _n2, char _n3, char _n4, const _CHAR _strName[]=_T("General"));
	~mml_general(void);
};
