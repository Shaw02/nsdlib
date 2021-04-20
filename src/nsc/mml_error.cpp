/*******************************************************************************

			NES Sound Driver & Library	(NSD.lib)	MML Compiler

	Copyright (c) 2012 A.Watanabe (S.W.), All rights reserved.
	 For conditions of distribution and use, see copyright notice in "nsc.cpp".

*******************************************************************************/

#include "StdAfx.h"
#include "mml_error.h"

/****************************************************************/
/*					グローバル変数（クラスだけど・・・）		*/
/****************************************************************/
extern	OPSW*			cOptionSW;	//オプション情報へのポインタ変数

#ifdef _OPENMP
	extern	omp_lock_t		lock_cout;
#endif

//==============================================================
//		コンストラクタ
//--------------------------------------------------------------
//	●引数
//		const _CHAR 	_message
//	●返値
//							無し
//==============================================================
mml_error_base::mml_error_base(const _CHAR* _Message)
{
	m_location.clear();
	m_message = _Message;
}

//==============================================================
//		コンストラクタ
//--------------------------------------------------------------
//	●引数
//			MMLfile&		MML
//		const _CHAR 	_message
//	●返値
//							無し
//==============================================================
mml_error_base::mml_error_base(MMLfile& MML, const _STRING& _Message)
{
	stringstream loc;

	MML.SetError();
	MML.GetLocation(&loc);
	m_location = loc.str();
	m_message = _Message.c_str();
}

//==============================================================
//		コンストラクタ
//--------------------------------------------------------------
//	●引数
//			MMLfile&		MML
//		const _CHAR 	_message
//	●返値
//							無し
//==============================================================
mml_error_base::mml_error_base(MMLfile& MML, const _CHAR* _Message)
{
	stringstream loc;

	MML.SetError();
	MML.GetLocation(&loc);
	m_location = loc.str();
	m_message = _Message;
}

//==============================================================
//		デストラクタ
//--------------------------------------------------------------
//	●引数
//				無し
//	●返値
//				無し
//==============================================================
mml_error_base::~mml_error_base(void)
{
}

//==============================================================
//		例外内容の出力
//--------------------------------------------------------------
//	●引数
//				無し
//	●返値
//				無し
//==============================================================
void	mml_error_base::out_what()
{
	_OMP_SET_LOCK(lock_cout)
	if(cOptionSW->fErr == true){
		cerr << "[ ERROR ] " << m_location;
		_CERR << m_message << endl;
	} else {
		cout << "[ ERROR ] " << m_location;
		_COUT << m_message << endl;
	}
	_OMP_UNSET_LOCK(lock_cout)
}

//--------------------------------------------------------------
void	mml_ios_failure::out_what()
{
	_OMP_SET_LOCK(lock_cout)
	if(cOptionSW->fErr == true){
		cerr << "[ ERROR ] " << m_location << what() << endl;
		_CERR << _T("\t\t\t\t") << m_message << endl;
	} else {
		cout << "[ ERROR ] " << m_location << what() << endl;
		_COUT << _T("\t\t\t\t") << m_message << endl;
	}
	_OMP_UNSET_LOCK(lock_cout)
}
