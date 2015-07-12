/*******************************************************************************

			NES Sound Driver & Library	(NSD.lib)	MML Compiler

	Copyright (c) 2012 A.Watanabe (S.W.), All rights reserved.
	 For conditions of distribution and use, see copyright notice in "nsc.cpp".

*******************************************************************************/

#pragma once

/****************************************************************/
/*																*/
/*			クラス定義											*/
/*																*/
/****************************************************************/
class SearchPass
{
protected:
	int					m_count;			//登録数量
	map<int, string>	m_ptcPass;			//検索パスの一覧（優先順位：登録順）

public:
						SearchPass(void);
						~SearchPass(void);

	void				clear();					//登録の全クリア
	void				debug();					//デバッグ用

	void				add_one(string* _str);		//追加（１dir）
	void				add(string*	_str);			//追加（";"の区切り対応）
	void				add(char*	_str);			//追加（";"の区切り対応）

	const char*			get(int id);				//文字列の取得
	int					count(){return(m_count);};	//登録数量の取得


};
