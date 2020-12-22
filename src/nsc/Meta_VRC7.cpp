/*******************************************************************************

			NES Sound Driver & Library	(NSD.lib)	MML Compiler

	Copyright (c) 2012 A.Watanabe (S.W.), All rights reserved.
	 For conditions of distribution and use, see copyright notice in "nsc.cpp".

*******************************************************************************/

#include "StdAfx.h"
#include "Meta_VRC7.h"

//==============================================================
//		コンストラクタ
//--------------------------------------------------------------
//	●引数
//		unsigned	char	_vrc7		VRC7の換装設定
//		const		char	_strName[]	クラスの名前
//	●返値
//				無し
//==============================================================
Meta_VRC7::Meta_VRC7(unsigned char _vrc7, const char _strName[]):
	MetaItem(_strName)
{
	m_size = 1;
	m_data.resize(m_size);

	m_data[0] = _vrc7;
}

//==============================================================
//		デストラクタ
//--------------------------------------------------------------
//	●引数
//				無し
//	●返値
//				無し
//==============================================================
Meta_VRC7::~Meta_VRC7(void)
{
}
