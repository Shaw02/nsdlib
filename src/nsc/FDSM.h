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
class FDSM :
	public MusicItem
{
//メンバー変数
private:
	unsigned	int		m_id;

//static	const	Command_Info	Command[];	//コマンドの情報

//メンバー関数
public:
				FDSM(MMLfile* MML, unsigned int _id, const _CHAR _strName[] = _T("FDSM"));
				~FDSM(void);
		void	getAsm(MusicFile* MUS);
};
