/*******************************************************************************

			NES Sound Driver & Library	(NSD.lib)	MML Compiler

	Copyright (c) 2012 A.Watanabe (S.W.), All rights reserved.
	 For conditions of distribution and use, see copyright notice in "nsc.cpp".

*******************************************************************************/

#pragma once
#include "MusicItem.h"

/****************************************************************/
/*																*/
/*			クラス定義											*/
/*																*/
/****************************************************************/
class N163 :
	public MusicItem
{
//メンバー変数
private:

//static	const	Command_Info	Command[];	//コマンドの情報

//メンバー関数
public:
				N163(MMLfile* MML, size_t _id, const _CHAR _strName[] = _T("N163"));
				~N163(void);
		void	getAsm(MusicFile* MUS);
};
