/*******************************************************************************

			NES Sound Driver & Library	(NSD.lib)	MML Compiler

	Copyright (c) 2012 A.Watanabe (S.W.), All rights reserved.
	 For conditions of distribution and use, see copyright notice in "nsc.cpp".

*******************************************************************************/

// 静的モデル
#include "StdAfx.h"
#include "SE.h"

//==============================================================
//		コンストラクタ
//--------------------------------------------------------------
//	●引数
//		MMLfile*			MML			MMLファイルのオブジェクト
//					size_t	_id			SE番号
//		const		_CHAR	_strName[]	オブジェクト名
//	●返値
//					無し
//==============================================================
SE::SE(MMLfile* MML, size_t _id, const _CHAR _strName[]):
	TrackSet(MML, _id, false, true, _strName)
{
	//このオブジェクトは必ず使う（最適化対象外）。
	setUse();
}

//==============================================================
//		デストラクタ
//--------------------------------------------------------------
//	●引数
//				無し
//	●返値
//				無し
//==============================================================
SE::~SE()
{
}

//==============================================================
//		コードの取得
//--------------------------------------------------------------
//	●引数
//		MusicFile*	MUS		コードを出力する曲データファイル・オブジェクト
//	●返値
//				無し
//==============================================================
void	SE::getAsm(MusicFile* MUS)
{
	*MUS << MUS->Header.Label.c_str() << "SE" << m_id << ":" << endl;
	TrackSet::getAsm(MUS);
}
