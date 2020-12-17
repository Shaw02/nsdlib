/*******************************************************************************

			NES Sound Driver & Library	(NSD.lib)	MML Compiler

	Copyright (c) 2012 A.Watanabe (S.W.), All rights reserved.
	 For conditions of distribution and use, see copyright notice in "nsc.cpp".

*******************************************************************************/

#include "StdAfx.h"
#include "Meta_BANK.h"

//==============================================================
//		コンストラクタ
//--------------------------------------------------------------
//	●引数
//		NSF_Header*			_nsf_hed	NSFヘッダー
//		const		char	_strName[]	クラスの名前
//	●返値
//				無し
//==============================================================
Meta_BANK::Meta_BANK(NSF_Header* _nsf_hed, const char _strName[]):
	MetaItem(_strName)
{
	m_size = 8;
	m_data.resize(m_size);

	memcpy((void *)m_data.c_str(), &_nsf_hed->Bank, m_size);
}

//==============================================================
//		デストラクタ
//--------------------------------------------------------------
//	●引数
//				無し
//	●返値
//				無し
//==============================================================
Meta_BANK::~Meta_BANK(void)
{
}
