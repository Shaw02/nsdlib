#pragma once
#include "MusicItem.h"

/****************************************************************/
/*																*/
/*			クラス定義											*/
/*																*/
/****************************************************************/
class VRC7 :
	public MusicItem
{
//メンバー変数
private:
	unsigned	int	m_id;
//static	const	Command_Info	Command[];	//コマンドの情報

//メンバー関数
public:
				VRC7(MMLfile* MML, unsigned int _id, const wchar_t _strName[] = L"==== [ VRC7 ]====");
				~VRC7(void);
		void	getAsm(MusicFile* MUS);
};
