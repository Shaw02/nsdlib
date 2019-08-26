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
				size_t		iSize;

	unsigned	int			m_id;			//ID番号
				bool		f_id;			//ID番号がセットされた事を示すflag

	unsigned	int			iOffset;		//SNDファイルポインタ
				bool		f_Optimize;		//最適化フラグ


//メンバー関数
public:
	MusicItem(const _CHAR _strName[]=_T(""));
	MusicItem(unsigned int _id, const _CHAR _strName[]=_T(""));
	~MusicItem(void);

				void	clear(void);
				void	clear_Optimize();
				size_t	getSize();
	unsigned	int		getOffset();
	unsigned	int		SetOffset(unsigned	int	_offset);

	unsigned	char	getCode(unsigned int n);
	virtual		void	getCode(string* _str);
	virtual		void	setCode(string* _str);
	virtual		void	getAsm(MusicFile* MUS);

				void	set_id(unsigned int _id);
	unsigned	int		get_id(void);
				bool	get_flag(void);

				void	setUse(void){f_Optimize = true;};	//最適化：不可
				bool	chkUse(void){return(f_Optimize);};	//最適化フラグの取得
};
