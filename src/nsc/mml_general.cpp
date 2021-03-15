/*******************************************************************************

			NES Sound Driver & Library	(NSD.lib)	MML Compiler

	Copyright (c) 2012 A.Watanabe (S.W.), All rights reserved.
	 For conditions of distribution and use, see copyright notice in "nsc.cpp".

*******************************************************************************/

#include "StdAfx.h"
#include "mml_general.h"

//==============================================================
//		�R���X�g���N�^�i����0Byte�j
//--------------------------------------------------------------
//	������
//		unsigned	char	_code		�R�[�h
//		const		char	_strName[]	�N���X�̖��O
//	���Ԓl
//				����
//==============================================================
mml_general::mml_general(unsigned char _code, const _CHAR _strName[]):
	MusicEvent(_strName)
{
	iSize = 1;
	code.resize(iSize);
	code[0] = _code;
}

//==============================================================
//		�R���X�g���N�^�i����1Byte�j
//--------------------------------------------------------------
//	������
//		unsigned	char	_code		�R�[�h
//		MMLfile*			MML			MML�t�@�C���̃I�u�W�F�N�g
//		const		char	_strName[]	�N���X�̖��O
//	���Ԓl
//				����
//==============================================================
mml_general::mml_general(unsigned char _code, MMLfile* MML, const _CHAR _strName[]):
	MusicEvent(_strName)
{
	int		_data = MML->GetInt_With_Chk_Range(_T("�p�����[�^"),-128,255);

	iSize = 2;
	code.resize(iSize);
	code[0] = (unsigned char)_code;
	code[1] = (unsigned char)_data;
}

//==============================================================
//			�R���X�g���N�^�i����1Byte�j
//--------------------------------------------------------------
//	������
//		unsigned	char	_code		�R�[�h
//		const		char	_data		����
//		const		char	_strName[]	�N���X�̖��O
//	���Ԓl
//				����
//==============================================================
mml_general::mml_general(unsigned char _code, char _data, const _CHAR _strName[]):
	MusicEvent(_strName)
{
	iSize = 2;
	code.resize(iSize);
	code[0] = _code;
	code[1] = _data;
}

//==============================================================
//			�R���X�g���N�^�i����2Byte�j
//--------------------------------------------------------------
//	������
//		unsigned	char	_code		�R�[�h
//		const		char	_n1			����1
//		const		char	_n2			����2
//		const		char	_strName[]	�N���X�̖��O
//	���Ԓl
//				����
//==============================================================
mml_general::mml_general(unsigned char _code, char _n1, char _n2, const _CHAR _strName[]):
	MusicEvent(_strName)
{
	iSize = 3;
	code.resize(iSize);
	code[0] = _code;
	code[1] = _n1;
	code[2] = _n2;
}

//==============================================================
//			�R���X�g���N�^�i����4Byte�j
//--------------------------------------------------------------
//	������
//		unsigned	char	_code		�R�[�h
//		const		char	_n1			����1
//		const		char	_n2			����2
//		const		char	_n3			����3
//		const		char	_n4			����4
//		const		char	_strName[]	�N���X�̖��O
//	���Ԓl
//				����
//==============================================================
mml_general::mml_general(unsigned char _code, char _n1, char _n2, char _n3, char _n4, const _CHAR _strName[]):
	MusicEvent(_strName)
{
	iSize = 5;
	code.resize(iSize);
	code[0] = _code;
	code[1] = _n1;
	code[2] = _n2;
	code[3] = _n3;
	code[4] = _n4;
}

//==============================================================
//		�f�X�g���N�^
//--------------------------------------------------------------
//	������
//				����
//	���Ԓl
//				����
//==============================================================
mml_general::~mml_general(void)
{
}
