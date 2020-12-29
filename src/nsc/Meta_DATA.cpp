/*******************************************************************************

			NES Sound Driver & Library	(NSD.lib)	MML Compiler

	Copyright (c) 2012 A.Watanabe (S.W.), All rights reserved.
	 For conditions of distribution and use, see copyright notice in "nsc.cpp".

*******************************************************************************/

#include "StdAfx.h"
#include "Meta_DATA.h"

//==============================================================
//		コンストラクタ
//--------------------------------------------------------------
//	●引数
//					string* nsf_data	NSFデータ
//		const		char	_strName[]	クラスの名前
//	●返値
//				無し
//==============================================================
Meta_DATA::Meta_DATA(string* nsf_data, const char _strName[]):
	MetaItem(_strName)
{
	setMetaData(nsf_data);
}

//==============================================================
//		デストラクタ
//--------------------------------------------------------------
//	●引数
//				無し
//	●返値
//				無し
//==============================================================
Meta_DATA::~Meta_DATA(void)
{
}
