/*******************************************************************************

			NES Sound Driver & Library	(NSD.lib)	MML Compiler

	Copyright (c) 2012 A.Watanabe (S.W.), All rights reserved.
	 For conditions of distribution and use, see copyright notice in "nsc.cpp".

*******************************************************************************/

#pragma once
#include "mml_Address.h"

/****************************************************************/
/*																*/
/*			クラス定義											*/
/*																*/
/****************************************************************/
class mml_CallSub :
	public mml_Address
{
//メンバー変数
	bool	by_Patch;

//メンバー関数
public:
	mml_CallSub(size_t _id, const _CHAR _strName[]=_T("Call Subroutine"));
	~mml_CallSub(void);

	void	setPatch(){	by_Patch = true; };
	void	setPatch(bool _f){	by_Patch = _f; };
	bool	getPatch(){	return(by_Patch); };
};
