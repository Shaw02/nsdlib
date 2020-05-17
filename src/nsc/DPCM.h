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
class DPCM :
	public FileInput, public MusicItem
{
//メンバー変数
private:
	unsigned	char	_DPCM_size;

//メンバー関数
public:
						DPCM(MMLfile* MML, const char* dmcfile, size_t _id, const _CHAR _strName[] = _T("==== [ DPCM ]===="));
						~DPCM(void);
	unsigned	char	getDPCMsize(void){return(_DPCM_size);};
				void	getAsm(MusicFile* MUS);
};
