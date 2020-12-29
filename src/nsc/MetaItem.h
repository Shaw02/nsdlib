/*******************************************************************************

			NES Sound Driver & Library	(NSD.lib)	MML Compiler

	Copyright (c) 2012 A.Watanabe (S.W.), All rights reserved.
	 For conditions of distribution and use, see copyright notice in "nsc.cpp".

*******************************************************************************/

#pragma once

/****************************************************************/
/*			プロトタイプ										*/
/****************************************************************/
class	MusicFile;


/****************************************************************/
/*																*/
/*			クラス定義											*/
/*																*/
/****************************************************************/
class MetaItem
{
//メンバー変数
protected:
	const		char*		m_identifier;		//オブジェクトの名称
				size_t		m_size;				//Chunkのサイズ
				string		m_data;				//実体
	list<MetaItem*>			ptcItem;			//構造化
				size_t		m_offset;			//SNDファイルポインタ

//メンバー関数
public:
				MetaItem(const char m_identifier[]="");
	virtual		~MetaItem(void);

				void	clear(void);
				size_t	getSize();
				size_t	getOffset();
				size_t	SetOffset(size_t _offset);

				void	setItem(MetaItem* _item);
				void	setItem_front(MetaItem* _item);

	unsigned	char	getData(size_t n);
	virtual		size_t	getData(string* _str);
	virtual		void	getMetaData(string* _str);
	virtual		void	setMetaData(string* _str);
				void	push_back(char _ch);
				void	append(string* _str);
				void	append(int _i);
};
