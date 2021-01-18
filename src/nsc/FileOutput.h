/*******************************************************************************

			NES Sound Driver & Library	(NSD.lib)	MML Compiler

	Copyright (c) 2012 A.Watanabe (S.W.), All rights reserved.
	 For conditions of distribution and use, see copyright notice in "nsc.cpp".

*******************************************************************************/

#pragma once

class FileOutput :
	public ofstream
{
protected:
//メンバー変数
		bool	f_error;					//エラー発生の有無

public:
//メンバー関数
					FileOutput(void);
					~FileOutput(void);
			void	fileopen(const char*	strFileName);
			void	StreamPointerAdd(std::streamoff iSize);
			void	StreamPointerMove(std::streamoff iSize);
			bool	isError(){return(f_error);};
};
