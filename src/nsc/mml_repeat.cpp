/*******************************************************************************

			NES Sound Driver & Library	(NSD.lib)	MML Compiler

	Copyright (c) 2012 A.Watanabe (S.W.), All rights reserved.
	 For conditions of distribution and use, see copyright notice in "nsc.cpp".

*******************************************************************************/

#include "StdAfx.h"
#include "mml_repeat.h"

//==============================================================
//		�R���X�g���N�^�ifor Repert A�j
//--------------------------------------------------------------
//	������
//		const		char	_strName[]	�N���X�̖��O
//		unsigned	char	count		���s�[�g��
//	���Ԓl
//				����
//==============================================================
mml_repeat::mml_repeat(size_t _id, unsigned char count, const _CHAR _strName[]):
	MusicEvent(_id, _strName)
{
	iSize = 2;
	code.resize(iSize);
	code[0] = nsd_Repeat_A_Start;
	code[1] = count;
}

//==============================================================
//		�R���X�g���N�^�ifor Repert B�j
//--------------------------------------------------------------
//	������
//		const		char	_strName[]	�N���X�̖��O
//	���Ԓl
//				����
//==============================================================
mml_repeat::mml_repeat(size_t _id, const _CHAR _strName[]):
	MusicEvent(_id, _strName)
{
	iSize = 1;
	code.resize(iSize);
	code[0] = nsd_Repeat_B_Start;
}

//==============================================================
//		�f�X�g���N�^
//--------------------------------------------------------------
//	������
//				����
//	���Ԓl
//				����
//==============================================================
mml_repeat::~mml_repeat(void)
{
}

//==============================================================
//		�񐔐ݒ�
//--------------------------------------------------------------
//	������
//		unsigned	char	count	���s�[�g��
//	���Ԓl
//				����
//==============================================================
void	mml_repeat::set_count(unsigned char count)
{
	if(	iSize != 2){
		throw out_of_range("mml_repeat::set_count()");
	}
	code[1] = count;
}
//==============================================================
//		�񐔐ݒ�
//--------------------------------------------------------------
//	������
//				����
//	���Ԓl
//		unsigned	char	count	���s�[�g��
//==============================================================
unsigned	char	mml_repeat::get_count(void)
{
	unsigned	char	count;

	if(	iSize != 2){
		throw out_of_range("mml_repeat::get_count()");
	}
	count = code[1];
	return(count);
}
