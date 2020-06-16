/*******************************************************************************

			NES Sound Driver & Library	(NSD.lib)	MML Compiler

	Copyright (c) 2012 A.Watanabe (S.W.), All rights reserved.
	 For conditions of distribution and use, see copyright notice in "nsc.cpp".

*******************************************************************************/

#pragma once

class FileInput :
	public ifstream
{
protected:
//メンバー変数
				string		strFilename;
	unsigned	int			iLine;		//現在のライン
	unsigned	char		readData;

//メンバー関数
public:
					FileInput(void);
					~FileInput(void);
			void	fileopen(const char*	_strFileName);
			void	fileopen(const char*	_strFileName,SearchPass* _pass);
			void	StreamPointerAdd(std::streamoff iSize);
			void	StreamPointerMove(std::streamoff iSize);
			void	Back(void);
			string*	GetFilename(void){return(&strFilename);};
unsigned	char	cRead();
std::streamoff		GetSize();
unsigned	int		GetLine(void){return(iLine);};
			void	SetLine(unsigned int i){iLine = i;};

};
