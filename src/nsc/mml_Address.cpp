/*******************************************************************************

			NES Sound Driver & Library	(NSD.lib)	MML Compiler

	Copyright (c) 2012 A.Watanabe (S.W.), All rights reserved.
	 For conditions of distribution and use, see copyright notice in "nsc.cpp".

*******************************************************************************/

#include "StdAfx.h"
#include "mml_Address.h"

/****************************************************************/
/*					�O���[�o���ϐ��i�N���X�����ǁE�E�E�j		*/
/****************************************************************/
extern	OPSW*			cOptionSW;	//�I�v�V�������ւ̃|�C���^�ϐ�

#ifdef _OPENMP
	extern	omp_lock_t		lock_cout;
#endif

//==============================================================
//		�R���X�g���N�^
//--------------------------------------------------------------
//	������
//		unsigned	char 	_code		�R�[�h
//		const		char	_strName[]	�N���X�̖��O
//	���Ԓl
//				����
//==============================================================
mml_Address::mml_Address(unsigned char _code, const _CHAR _strName[]):
	MusicEvent(_strName)
{
	iSize = 3;
	code.resize(iSize);
	code[0] = _code;
	code[1] = 0;
	code[2] = 0;
}

//==============================================================
//		�R���X�g���N�^
//--------------------------------------------------------------
//	������
//		unsigned	char 	_code		�R�[�h
//		const		char	_strName[]	�N���X�̖��O
//	���Ԓl
//				����
//==============================================================
mml_Address::mml_Address(size_t _id, unsigned char _code, const _CHAR _strName[]):
	MusicEvent(_id, _strName)
{
	iSize = 3;
	code.resize(iSize);
	code[0] = _code;
	code[1] = 0;
	code[2] = 0;
}

//==============================================================
//		�R���X�g���N�^
//--------------------------------------------------------------
//	������
//		unsigned	char 	_code		�R�[�h
//		unsigned	char 	_data		����1
//		const		char	_strName[]	�N���X�̖��O
//	���Ԓl
//				����
//==============================================================
mml_Address::mml_Address(size_t _id, unsigned char _code, unsigned char _data, const _CHAR _strName[]):
	MusicEvent(_id, _strName)
{
	iSize = 4;
	code.resize(iSize);
	code[0] = _code;
	code[1] = _data;
	code[2] = 0;
	code[3] = 0;
}

//==============================================================
//		�f�X�g���N�^
//--------------------------------------------------------------
//	������
//				����
//	���Ԓl
//				����
//==============================================================
mml_Address::~mml_Address(void)
{
}

//==============================================================
//		�A�h���X�̐ݒ�
//--------------------------------------------------------------
//	������
//		unsigned	int		_addr	�A�h���X
//	���Ԓl
//				����
//	������
//		���̊֐��͕���ɌĂяo����܂��B
//==============================================================
void	mml_Address::set_Address(size_t _addr)
{
	try {
		switch(iSize){
			case(0):
				break;
			case(3):
				code[1] = (unsigned char)((_addr     ) & 0xFF);
				code[2] = (unsigned char)((_addr >> 8) & 0xFF);
				break;
			case(4):
				code[2] = (unsigned char)((_addr     ) & 0xFF);
				code[3] = (unsigned char)((_addr >> 8) & 0xFF);
				break;
			default:
				throw invalid_argument("mml_Address::set_Address()");
				break;
		}
	
		if(cOptionSW->iDebug & DEBUG_FixAddress){
			_OMP_SET_LOCK(lock_cout)
			_COUT << _T("    Fixed: Object Address [0x") << hex << setw(4) << setfill(_T('0')) << iOffset << _T("]: ");
			for(size_t i=0, e=code.size(); i < e; ++i){
				_COUT	<<	hex	<<	setw(2)	<<	setfill(_T('0'))	<<	(unsigned int)(code[i] & 0xFF)	<<	_T(" ");
			}
			_COUT  << dec	<< _T(": ") << strName;
			if(f_id == true){
				_COUT	<< _T("(") << m_id << _T(")");
			}
			#ifdef _OPENMP
			_COUT << " (Thread No=" << omp_get_thread_num() <<")";
			#endif
			_COUT << endl;
			_OMP_UNSET_LOCK(lock_cout)
		}
	}
	catch (const exception& e) {
		nsc_ErrMsg(e);
	}
}
