/*******************************************************************************

			NES Sound Driver & Library	(NSD.lib)	MML Compiler

	Copyright (c) 2012 A.Watanabe (S.W.), All rights reserved.
	 For conditions of distribution and use, see copyright notice in "nsc.cpp".

*******************************************************************************/

#include "StdAfx.h"
#include "MetaItem.h"

/****************************************************************/
/*					グローバル変数（クラスだけど・・・）		*/
/****************************************************************/
extern	OPSW*			cOptionSW;	//オプション情報へのポインタ変数

//==============================================================
//		コンストラクタ
//--------------------------------------------------------------
//	●引数
//		const	char	_strName[]	identifier
//	●返値
//				無し
//==============================================================
MetaItem::MetaItem(const char _strName[]):
	m_identifier(_strName),
	m_size(0),
	m_offset(0)
{
	//Debug message　（うざい程出力するので注意。）
	if(cOptionSW->iDebug & DEBUG_Meta_Create){
		_COUT << _T("Create Meta Object : ") << m_identifier << endl;
	}
}

//==============================================================
//		デストラクタ
//--------------------------------------------------------------
//	●引数
//				無し
//	●返値
//				無し
//==============================================================
MetaItem::~MetaItem(void)
{
	clear();

	//Debug message　（うざい程出力するので注意。）
	if(cOptionSW->iDebug & DEBUG_Meta_Delete){
		_COUT << _T("Delete Meta Object : ") << m_identifier << endl;
	}
}

//==============================================================
//		クリア
//--------------------------------------------------------------
//	●引数
//				無し
//	●返値
//				無し
//==============================================================
void	MetaItem::clear(void)
{
	//----------------------
	//Local変数
	list<	MetaItem*>::iterator	itItem;

	//----------------------
	//clear
	m_data.clear();

	//----------------------
	//Delete Class
	if(!ptcItem.empty()){
		itItem = ptcItem.begin();
		while(itItem != ptcItem.end()){
			delete *itItem;
			itItem++;
		}
		ptcItem.clear();
	}

	m_size = 0;
}

//==============================================================
//		コードサイズの取得
//--------------------------------------------------------------
//	●引数
//				無し
//	●返値
//		size_t
//==============================================================
size_t		MetaItem::getSize()
{
	return(m_size);
}

//==============================================================
//		コードのオフセットアドレスの取得
//--------------------------------------------------------------
//	●引数
//				無し
//	●返値
//		size_t
//==============================================================
size_t	MetaItem::getOffset()
{
	return(m_offset);
}

//==============================================================
//		オフセットアドレスの設定
//--------------------------------------------------------------
//	●引数
//		size_t	_offset
//	●返値
//				無し
//==============================================================
size_t	MetaItem::SetOffset(size_t _offset)
{
	//----------------------
	//Local変数
	list<	MetaItem*>::iterator	itItem;
	size_t	i = 0;

	//Debug message　（うざい程出力するので注意。）
	if(cOptionSW->iDebug & DEBUG_Meta_Set){
		_COUT << _T("Object Address [0x") << hex << setw(4) << setfill(_T('0')) << _offset << _T("]: ");
		while(i < m_data.size()){
			_COUT	<<	hex	<<	setw(2)	<<	setfill(_T('0'))	<<	(unsigned int)(m_data[i] & 0xFF)	<<	_T(" ");
			i++;
		}
		_COUT  << dec	<< _T(": ") << m_identifier << endl;
	}

	m_offset = _offset;
	_offset	+= m_data.size();

	if(!ptcItem.empty()){
		itItem = ptcItem.begin();
		while(itItem != ptcItem.end()){
			_offset = (*itItem)->SetOffset(_offset);
			itItem++;
		}
	}

	//このオブジェクトのサイズ（最適化後）
	m_size = _offset - m_offset;

	//Size と ID の分を加算
	_offset += 8;

	return(_offset);
}

//==============================================================
//		コードの取得
//--------------------------------------------------------------
//	●引数
//					size_t	添え字
//	●返値
//		unsigned	char	内容
//==============================================================
unsigned	char	MetaItem::getData(size_t n)
{
	unsigned	char	iData;
	
	if((n<0) || (n>=m_size)){
		iData = 0xFF;
	} else {
		iData = m_data[n];
	}

	return(iData);
}

//==============================================================
//		コードの取得
//--------------------------------------------------------------
//	●引数
//		string*		_str
//	●返値
//				無し
//==============================================================
void	MetaItem::getData(string* _str)
{
	//----------------------
	//Local変数
	list<	MetaItem*>::iterator	itItem;

	_str->append(m_data);

	if(!ptcItem.empty()){
		itItem = ptcItem.begin();
		while(itItem != ptcItem.end()){
			(*itItem)->getMetaData(_str);
			itItem++;
		}
	}
}

//==============================================================
//		コードの取得
//--------------------------------------------------------------
//	●引数
//		string*		_str
//	●返値
//				無し
//==============================================================
void	MetaItem::getMetaData(string* _str)
{
	string	strSize;

	strSize.resize(4);

	//SIZE
	strSize[0] = (unsigned char)(m_size      ) & 0xFF;
	strSize[1] = (unsigned char)(m_size >>  8) & 0xFF;
	strSize[2] = (unsigned char)(m_size >> 16) & 0xFF;
	strSize[3] = (unsigned char)(m_size >> 24) & 0xFF;
	_str->append(strSize);

	//ID
	_str->append(m_identifier, 4);

	//Data
	getMetaData(_str);
}

//==============================================================
//		コードの設定
//--------------------------------------------------------------
//	●引数
//		string*		_str
//	●返値
//				無し
//==============================================================
void	MetaItem::setMetaData(string* _str)
{
	m_data.clear();
	m_data.assign(*_str);
	m_size = m_data.size();
}

