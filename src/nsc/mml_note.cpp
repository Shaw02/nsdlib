/*******************************************************************************

			NES Sound Driver & Library	(NSD.lib)	MML Compiler

	Copyright (c) 2012 A.Watanabe (S.W.), All rights reserved.
	 For conditions of distribution and use, see copyright notice in "nsc.cpp".

*******************************************************************************/

#include "StdAfx.h"
#include "mml_note.h"

//==============================================================
//		�R���X�g���N�^
//--------------------------------------------------------------
//	������
//		unsigned	int		code
//		unsigned	int		length
//		unsigned	int		gatetime
//					bool	slur
//		const		char	_strName[]	�N���X�̖��O
//	���Ԓl
//				����
//==============================================================
mml_note::mml_note(int _code, int length, int gatetime, bool slur, const _CHAR _strName[]):
	MusicEvent(_strName)
{
	unsigned	char	opcode = nsd_Note | (unsigned char)_code;
				size_t	i = 0;

	if((_code<0) || (_code>15)){
		throw out_of_range("mml_note::mml_note()");
	}

	iSize = 1;

	//���̃I�u�W�F�N�g�͕K���g���i�œK���ΏۊO�j�B
	setUse();

	if(slur == true){
		opcode |= nsd_Note_Slur;
	}

	if(length != -1){
		opcode |= nsd_Note_Length;
		iSize++;
	}

	if(gatetime != -1){
		opcode |= nsd_Note_Gate;
		iSize++;
	}

	code.resize(iSize);
	code[i] = opcode;
	i++;

	if(length != -1){
		code[i] = (unsigned char)length;
		i++;
	}
	if(gatetime != -1){
		code[i] = (unsigned char)gatetime;
		i++;
	}

}

//==============================================================
//		�f�X�g���N�^
//--------------------------------------------------------------
//	������
//				����
//	���Ԓl
//				����
//==============================================================
mml_note::~mml_note(void)
{
}

//==============================================================
//		�^�C
//--------------------------------------------------------------
//	������
//				����
//	���Ԓl
//				����
//==============================================================
void	mml_note::SetTai(void)
{
	code[0] |= nsd_Note_Slur;
}
