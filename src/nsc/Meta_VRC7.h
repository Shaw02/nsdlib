/*******************************************************************************

			NES Sound Driver & Library	(NSD.lib)	MML Compiler

	Copyright (c) 2012 A.Watanabe (S.W.), All rights reserved.
	 For conditions of distribution and use, see copyright notice in "nsc.cpp".

*******************************************************************************/

#pragma once
#include "MetaItem.h"

/****************************************************************/
/*																*/
/*			クラス定義											*/
/*																*/
/****************************************************************/
class Meta_VRC7 :
	public MetaItem
{
//メンバー変数
private:

//メンバー関数
public:
	Meta_VRC7(unsigned char _vrc7, const char _strName[] = "VRC7");
	~Meta_VRC7(void);
};
