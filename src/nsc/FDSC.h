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
	unsigned	int	m_id;
//static	const	Command_Info	Command[];	//コマンドの情報

//メンバー関数
public:
				FDSC(MMLfile* MML, unsigned int _id, const wchar_t _strName[] = L"==== [ FDSC ]====");
				~FDSC(void);
		void	getAsm(MusicFile* MUS);
};
