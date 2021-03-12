/***************************************************************************

				NES Sound Driver & Library	(NSD.lib)
					Music Macro Language (MML) Compiler

****************************************************************************

   Copyright (c) 2012 A.Watanabe (S.W.)
   All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:

     1. Redistributions of source code must retain the above copyright
        notice, this list of conditions and the following disclaimer.

     2. Redistributions in binary form must reproduce the above copyright
        notice, this list of conditions and the following disclaimer in the
        documentation and/or other materials provided with the distribution.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER
   OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

***************************************************************************/

// nsc.cpp : �R���\�[�� �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
//

#include "StdAfx.h"

/****************************************************************/
/*					�O���[�o��									*/
/****************************************************************/
		OPSW*			cOptionSW	= NULL;	//�ǂ�����ł��A�N�Z�X����B

#ifdef _OPENMP
		omp_lock_t		lock_cout;			//COUT, CERR�̔r������p
#endif

//==============================================================
//		�G���[			������ To Do:	�p�~�\��
//--------------------------------------------------------------
//	������
//		int			�G���[�R�[�h
//	���Ԓl
//					����
//==============================================================
void nsc_exit(int no)
{
	throw no;
}

//==============================================================
//		�G���[�o��
//--------------------------------------------------------------
void nsc_ErrMsg(int no)
{
	if(no != 0){
		//��������̃G���[�̏ꍇ
		_OMP_SET_LOCK(lock_cout)
		cerr << "Error!: " << strerror(no) << endl;
		_OMP_UNSET_LOCK(lock_cout)
	}
}

//--------------------------------------------------------------
void nsc_ErrMsg(const exception& e)
{
	_OMP_SET_LOCK(lock_cout)
	if(cOptionSW->fErr == true){
		cerr << "Error!: " << e.what() << endl;
	} else {
		cout << "Error!: " << e.what() << endl;
	}
	_OMP_UNSET_LOCK(lock_cout)
}

//--------------------------------------------------------------
void nsc_ErrMsg(const string& s)
{
	_OMP_SET_LOCK(lock_cout)
	if(cOptionSW->fErr == true){
		cerr << "Error!: " << s.c_str() << endl;
	} else {
		cout << "Error!: " << s.c_str() << endl;
	}
	_OMP_UNSET_LOCK(lock_cout)
}

//--------------------------------------------------------------
void nsc_ErrMsg(const _CHAR* stErrMsg)
{
	_OMP_SET_LOCK(lock_cout)

	if(cOptionSW->fErr == true){
		_CERR << _T("Error!: ") << stErrMsg << endl;
	} else {
		_COUT << _T("Error!: ") << stErrMsg << endl;
	}
	_OMP_UNSET_LOCK(lock_cout)
}

//==============================================================
//		���C���֐�
//--------------------------------------------------------------
//	������
//		int		argc	�R�}���h���C������	
//		_TCHAR*	argv[]	�R�}���h���C������
//	���Ԓl
//		int			�G���[�R�[�h
//==============================================================
int	main(int argc, char* argv[])
{
	size_t		i;
	int			iResult	= EXIT_SUCCESS;
	MMLfile		*cMML	= NULL;
	MusicFile	*cSND	= NULL;

#ifdef	_MSC_VER
		locale::global(std::locale(""));
#else
//		locale::global(std::locale(""));
//		 �� gcc�ł́A�g���Ȃ����悤�Ȃ̂ŁA�b���C�u�����̕����g���B
		setlocale(LC_ALL, "");
#endif

	_OMP_INIT_LOCK(lock_cout)

	try{

		//==================================
		_COUT	<<	_T("MML Compiler for NES Sound Driver & Library (NSD.Lib)\n")
					_T("    Version 1.31\n")
					_T("        Copyright (c) 2012-2021 S.W.\n")	<<	endl;

		//==================================
		//�I�v�V�����̏���
		cOptionSW	= new OPSW(argc,argv);

		//MML�t�@�C���̃N���X�I�u�W�F�N�g�쐬
		cMML	= new MMLfile(cOptionSW->strMMLname);


		//==================================
		//MML�\����͂��Ȃ���A�N���X�I�u�W�F�N�g�̍쐬
		_COUT << _T("------------------------------------------------------------") << endl;
		_COUT << _T("*Object creating process") << endl;

		//�ȃf�[�^�I�u�W�F�N�g�̍쐬
		cSND	= new MusicFile(cMML, cOptionSW->strCodeName);

		_COUT << endl;


		//==================================
		//Optimize & Tick Count
		_COUT << _T("------------------------------------------------------------") << endl;
		_COUT << _T("*Optimize & Tick counting process") << endl;

		cSND->TickCount();

		_COUT << endl;



		//==================================
		//�A�h���X�̉���
		_COUT << _T("------------------------------------------------------------") << endl;
		_COUT << _T("*Address settlement process") << endl;

		//�A�h���X�̌v�Z �� �T�C�Y�̏o��
		i = cSND->SetOffset(0);
		cout << "  Music Size = " << setfill(' ')  << setw(5) << i << " [Byte]" << endl;

		i = cSND->SetDPCMOffset(i);
		cout << "  DPCM Size  = " << setfill(' ')  << setw(5) << i << " [Byte]" << endl;

		//�A�h���X�������ɂ��I�y�R�[�h�̃A�h���X����
		cSND->Fix_Address();

		_COUT << endl;



		//==================================
		//�ۑ�
		//NSF
		if((cOptionSW->saveNSF == true) || ((cOptionSW->saveNSF == false)&&(cOptionSW->saveNSFe == false)&&(cOptionSW->saveASM == false))){
			cSND->saveNSF(cOptionSW->strNSFname);
		}

		//NSFe
		if(cOptionSW->saveNSFe == true){
			cSND->saveNSFe(cOptionSW->strNSFename);
		}

		//Assembly
		if(cOptionSW->saveASM == true){
			cSND->saveASM(cOptionSW->strASMname);
		}

		_COUT << endl;

	} catch (int no) {
		iResult	= no;	//�����������Ƃ��͕\�����Ȃ��B
	} catch (const exception& e){
		iResult	= EXIT_FAILURE;
		nsc_ErrMsg(e);
	} catch (const _CHAR* stErrMsg) {
		iResult	= EXIT_FAILURE;
		nsc_ErrMsg(stErrMsg);
	} catch (const string& str) {
		iResult	= EXIT_FAILURE;
		nsc_ErrMsg(str);
	}

	//==================================
	//�N���X�̍폜
	if (cSND)
		delete	cSND;
	if (cMML)
		delete	cMML;
	if (cOptionSW)
		delete	cOptionSW;

	_OMP_DESTROY_LOCK(lock_cout)

	return(iResult);
}
