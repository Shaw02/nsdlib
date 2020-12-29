/*******************************************************************************

			NES Sound Driver & Library	(NSD.lib)	MML Compiler

	Copyright (c) 2012 A.Watanabe (S.W.), All rights reserved.
	 For conditions of distribution and use, see copyright notice in "nsc.cpp".

*******************************************************************************/

#include "StdAfx.h"
#include "Meta_INFO.h"

//==============================================================
//		コンストラクタ
//--------------------------------------------------------------
//	●引数
//		NSF_Header*			_nsf_hed	NSFヘッダー
//		const		char	_strName[]	クラスの名前
//	●返値
//				無し
//==============================================================
Meta_INFO::Meta_INFO(NSF_Header* _nsf_hed, const char _strName[]):
	MetaItem(_strName)
{
	m_size = 10;
	m_data.resize(m_size);
	m_data[0] = (unsigned char)(_nsf_hed->LoadAddress & 0xFF);
	m_data[1] = (unsigned char)(_nsf_hed->LoadAddress >> 8);
	m_data[2] = (unsigned char)(_nsf_hed->InitAddress & 0xFF);
	m_data[3] = (unsigned char)(_nsf_hed->InitAddress >> 8);
	m_data[4] = (unsigned char)(_nsf_hed->MainAddress & 0xFF);
	m_data[5] = (unsigned char)(_nsf_hed->MainAddress >> 8);
	m_data[6] = _nsf_hed->Video;
	m_data[7] = _nsf_hed->External;
	m_data[8] = _nsf_hed->MusicNumber;
	m_data[9] = _nsf_hed->StartMusicNumber;
}

//==============================================================
//		デストラクタ
//--------------------------------------------------------------
//	●引数
//				無し
//	●返値
//				無し
//==============================================================
Meta_INFO::~Meta_INFO(void)
{
}
