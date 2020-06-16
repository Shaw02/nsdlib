/*******************************************************************************

			NES Sound Driver & Library	(NSD.lib)	MML Compiler

	Copyright (c) 2012 A.Watanabe (S.W.), All rights reserved.
	 For conditions of distribution and use, see copyright notice in "nsc.cpp".

*******************************************************************************/

#pragma once

class FileOutput :
	public ofstream
{
public:
		FileOutput(void);
		~FileOutput(void);
void	fileopen(const char*	strFileName);
void	StreamPointerAdd(std::streamoff iSize);
void	StreamPointerMove(std::streamoff iSize);
};
