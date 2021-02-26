/*******************************************************************************

			NES Sound Driver & Library	(NSD.lib)	MML Compiler

	Copyright (c) 2012 A.Watanabe (S.W.), All rights reserved.
	 For conditions of distribution and use, see copyright notice in "nsc.cpp".

*******************************************************************************/

#include "StdAfx.h"
#include "FileOutput.h"

//==============================================================
//		コンストラクタ
//--------------------------------------------------------------
//	●引数
//				無し
//	●返値
//				無し
//==============================================================
FileOutput::FileOutput(void):
	f_error(false)
{
//#ifdef	_WIN32
	imbue( std::locale::classic() );
//#endif
}

//==============================================================
//		デストラクタ
//--------------------------------------------------------------
//	●引数
//				無し
//	●返値
//				無し
//==============================================================
FileOutput::~FileOutput(void)
{
}

//--------------------------------
//ファイルを開く　エラー処理付き
//--------------------------------
void	FileOutput::fileopen(const char*	strFileName){

	//File open
	open(strFileName,ios_base::out | ios_base::binary);
	if(good()==false){
		f_error	= true;
	}
};

//--------------------------------
//相対シーク
//--------------------------------
void	FileOutput::StreamPointerAdd(fstream::off_type iSize){
	seekp(iSize,ios::cur);
};

//--------------------------------
//絶対シーク
//--------------------------------
void	FileOutput::StreamPointerMove(fstream::pos_type iSize){
	seekp(iSize);
};
