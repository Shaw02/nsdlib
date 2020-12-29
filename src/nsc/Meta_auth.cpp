/*******************************************************************************

			NES Sound Driver & Library	(NSD.lib)	MML Compiler

	Copyright (c) 2012 A.Watanabe (S.W.), All rights reserved.
	 For conditions of distribution and use, see copyright notice in "nsc.cpp".

*******************************************************************************/

#include "StdAfx.h"
#include "Meta_auth.h"

//==============================================================
//		コンストラクタ
//--------------------------------------------------------------
//	●引数
//		MusicHeader*		Header		ヘッダー情報
//		const		char	_strName[]	クラスの名前
//	●返値
//				無し
//==============================================================
Meta_auth::Meta_auth(MusicHeader* Header, const char _strName[]):
	MetaItem(_strName)
{
	m_data.clear();
	m_size = 0;

	append(&Header->title);
	append(&Header->composer);
	append(&Header->copyright);
	append(&Header->maker);
}

//==============================================================
//		デストラクタ
//--------------------------------------------------------------
//	●引数
//				無し
//	●返値
//				無し
//==============================================================
Meta_auth::~Meta_auth(void)
{
}
