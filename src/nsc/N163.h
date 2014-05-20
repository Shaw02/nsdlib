#pragma once
#include "MusicItem.h"

/****************************************************************/
/*																*/
/*			クラス定義											*/
/*																*/
/****************************************************************/
class N163 :
	public MusicItem
{
//メンバー変数
private:
	unsigned	int		m_id;
				bool	f_Use;				//MML中で使われるか？

//static	const	Command_Info	Command[];	//コマンドの情報

//メンバー関数
public:
				N163(MMLfile* MML, unsigned int _id, const wchar_t _strName[] = L"==== [ N163 ]====");
				~N163(void);
		void	getAsm(MusicFile* MUS);
		void	setUse(void){f_Use = true;};
		bool	chkUse(void){return(f_Use);};
};
