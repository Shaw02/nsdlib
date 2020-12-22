/*******************************************************************************

			NES Sound Driver & Library	(NSD.lib)	MML Compiler

	Copyright (c) 2012 A.Watanabe (S.W.), All rights reserved.
	 For conditions of distribution and use, see copyright notice in "nsc.cpp".

*******************************************************************************/

#include "StdAfx.h"
#include "Meta_mixe.h"

//==============================================================
//		コンストラクタ
//--------------------------------------------------------------
//	●引数
//		const		char	_strName[]	クラスの名前
//	●返値
//				無し
//==============================================================
Meta_mixe::Meta_mixe(const char _strName[]):
	MetaItem(_strName)
{
	m_size = 0;
	m_data.clear();
}

//==============================================================
//		デストラクタ
//--------------------------------------------------------------
//	●引数
//				無し
//	●返値
//				無し
//==============================================================
Meta_mixe::~Meta_mixe(void)
{
}

//==============================================================
//		デストラクタ
//--------------------------------------------------------------
//	●引数
//				unsigned char	_id		音源のID
//				MMLfile*		MML		MMLファイル
//	●返値
//				無し
//==============================================================
void	Meta_mixe::append(unsigned char _id, signed int _vol, MMLfile* MML)
{
	//エラー処理
	if(m_volume.count(_id) > 0){
		MML->Warning(_T("#mixeにて音源指定が重複しています。"));
	} else {
		if((_vol<-32768) || (_vol>32767)){
			MML->Warning(_T("#mixeの音量は、-32768～32767で指定してください。"));
		}
		m_volume[_id] = (short)_vol & 0xFFFF;
		m_data.push_back(_id);
		m_data.push_back((char)( _vol     & 0xFF));
		m_data.push_back((char)((_vol>>8) & 0xFF));
		m_size += 3;
	}
}
