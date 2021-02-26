/*******************************************************************************

			NES Sound Driver & Library	(NSD.lib)	MML Compiler

	Copyright (c) 2012 A.Watanabe (S.W.), All rights reserved.
	 For conditions of distribution and use, see copyright notice in "nsc.cpp".

*******************************************************************************/

#include "StdAfx.h"
#include "FileOutput.h"

//==============================================================
//		�R���X�g���N�^
//--------------------------------------------------------------
//	������
//				����
//	���Ԓl
//				����
//==============================================================
FileOutput::FileOutput(void):
	f_error(false)
{
//#ifdef	_WIN32
	imbue( std::locale::classic() );
//#endif
}

//==============================================================
//		�f�X�g���N�^
//--------------------------------------------------------------
//	������
//				����
//	���Ԓl
//				����
//==============================================================
FileOutput::~FileOutput(void)
{
}

//--------------------------------
//�t�@�C�����J���@�G���[�����t��
//--------------------------------
void	FileOutput::fileopen(const char*	strFileName){

	//File open
	open(strFileName,ios_base::out | ios_base::binary);
	if(good()==false){
		f_error	= true;
	}
};

//--------------------------------
//���΃V�[�N
//--------------------------------
void	FileOutput::StreamPointerAdd(fstream::off_type iSize){
	seekp(iSize,ios::cur);
};

//--------------------------------
//��΃V�[�N
//--------------------------------
void	FileOutput::StreamPointerMove(fstream::pos_type iSize){
	seekp(iSize);
};
