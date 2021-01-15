/*******************************************************************************

			NES Sound Driver & Library	(NSD.lib)	MML Compiler

	Copyright (c) 2012 A.Watanabe (S.W.), All rights reserved.
	 For conditions of distribution and use, see copyright notice in "nsc.cpp".

*******************************************************************************/

// 静的モデル
#include "StdAfx.h"
#include "Sub.h"

//==============================================================
//		コンストラクタ
//--------------------------------------------------------------
//	●引数
//		MMLfile*	MML			MMLファイルのオブジェクト
//		size_t		_id			サブルーチン番号
//		const _CHAR	_strName[]	オブジェクト名
//	●返値
//					無し
//==============================================================
Sub::Sub(MMLfile* MML, size_t _id, const _CHAR _strName[]/* = "==== [ Sub ]===="*/):
	TrackSet(MML, _id, true, false, _strName)
{
#ifdef _OPENMP
	omp_init_lock(&lock_TickCount);
#endif
}

//==============================================================
//		デストラクタ
//--------------------------------------------------------------
//	●引数
//				無し
//	●返値
//				無し
//==============================================================
Sub::~Sub()
{
#ifdef _OPENMP
	omp_destroy_lock(&lock_TickCount);
#endif
}

//==============================================================
//		TickCount	サブルーチン呼び出し
//--------------------------------------------------------------
//	●引数
//				無し
//	●返値
//				無し
//==============================================================
unsigned	int	Sub::TickCount(MusicFile* MUS, NSD_WORK* work)
{
	int	_iResult;

#ifdef _OPENMP
	//他のトラックが呼び出している時に、競合させない。
	omp_set_lock(&lock_TickCount);
#endif

	_iResult = ptcTrack[0]->TickCount(MUS, work);

#ifdef _OPENMP
	omp_unset_lock(&lock_TickCount);
#endif

	return(_iResult);
}

//==============================================================
//		コードの取得
//--------------------------------------------------------------
//	●引数
//		MusicFile* MUS		コードを出力する曲データファイル・オブジェクト
//	●返値
//				無し
//==============================================================
void	Sub::getAsm(MusicFile* MUS)
{
	*MUS << MUS->Header.Label.c_str() << "SUB" << m_id << ":" << endl;
	TrackSet::getAsm(MUS);
}
