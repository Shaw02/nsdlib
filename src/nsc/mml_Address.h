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
class mml_Address :
	public MusicEvent
{
//メンバー変数

//メンバー関数
public:
	mml_Address(unsigned char _code, const _CHAR _strName[]=_T("Address"));
	mml_Address(size_t _id, unsigned char _code, const _CHAR _strName[]=_T("Address"));
	mml_Address(size_t _id, unsigned char _code, unsigned char _data, const _CHAR _strName[]=_T("Address"));
	~mml_Address(void);

				void	set_Address(size_t _addr);
};
