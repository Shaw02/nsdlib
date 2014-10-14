#pragma once
#include "MusicItem.h"

/****************************************************************/
/*																*/
/*			クラス定義											*/
/*																*/
/****************************************************************/
class FDSC :
	public MusicItem
{
//メンバー変数
private:
	unsigned	int		m_id;
				bool	f_Use;				//MML中で使われるか？

//static	const	Command_Info	Command[];	//コマンドの情報

//メンバー関数
public:
				FDSC(MMLfile* MML, unsigned int _id, const _CHAR _strName[] = _T("FDSC"));
				~FDSC(void);
		void	setUse(void){f_Use = true;};
		bool	chkUse(void){return(f_Use);};
		void	getAsm(MusicFile* MUS);
};
