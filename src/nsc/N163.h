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
	unsigned	int	m_id;
//static	const	Command_Info	Command[];	//コマンドの情報

//メンバー関数
public:
				N163(MMLfile* MML, unsigned int _id, const wchar_t _strName[] = L"==== [ N163 ]====");
				~N163(void);
		void	getAsm(MusicFile* MUS);
};
