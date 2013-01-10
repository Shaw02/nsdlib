#pragma once
#include "MusicItem.h"

/****************************************************************/
/*																*/
/*			クラス定義											*/
/*																*/
/****************************************************************/
class FDSM :
	public MusicItem
{
//メンバー変数
private:
	unsigned	int	m_id;
//static	const	Command_Info	Command[];	//コマンドの情報

//メンバー関数
public:
				FDSM(MMLfile* MML, unsigned int _id, const wchar_t _strName[] = L"==== [ FDSM ]====");
				~FDSM(void);
		void	getAsm(MusicFile* MUS);
};
