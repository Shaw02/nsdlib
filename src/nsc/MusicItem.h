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
class MusicItem
{
//メンバー変数
protected:
	const		_CHAR*		strName;		//オブジェクトの名称
	list<MusicItem*>		ptcItem;		//構造化
				string		code;
				size_t		iSize;			//このオブジェクトのバイナリサイズ
				size_t		iOffset;		//SNDファイルポインタ

				size_t		m_id;			//ID番号
				bool		f_id;			//ID番号がセットされた事を示すflag

				bool		f_necessary;	//最適化フラグ


//メンバー関数
public:
	MusicItem(const _CHAR _strName[]=_T(""));
	MusicItem(size_t _id, const _CHAR _strName[]=_T(""));
	~MusicItem(void);

				void	clear(void);
				void	clear_Optimize();
				size_t	getSize();
				size_t	getOffset();
				size_t	SetOffset(size_t _offset);

	unsigned	char	getCode(size_t n);
	virtual		void	getCode(string* _str);
	virtual		void	setCode(string* _str);
	virtual		void	getAsm(MusicFile* MUS);

				void	set_id(size_t _id);
				size_t	get_id(void);
				bool	get_flag(void);

				void	setUse(void){f_necessary = true;};	//最適化：不可
				bool	chkUse(void){return(f_necessary);};	//最適化フラグの取得
};
