/*******************************************************************************

			NES Sound Driver & Library	(NSD.lib)	MML Compiler

	Copyright (c) 2012 A.Watanabe (S.W.), All rights reserved.
	 For conditions of distribution and use, see copyright notice in "nsc.cpp".

*******************************************************************************/

#pragma once
#include "TrackSet.h"

/****************************************************************/
/*																*/
/*			ƒNƒ‰ƒX’è‹`											*/
/*																*/
/****************************************************************/
class BGM :
	public TrackSet
{
public:
	BGM(MMLfile* MML, unsigned int _id, const _CHAR _strName[] = _T("BGM"));
	~BGM(void);
	void	getAsm(MusicFile* MUS);
};
