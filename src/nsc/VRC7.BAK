#pragma once
#include "MusicItem.h"

/****************************************************************/
/*																*/
/*			クラス定義											*/
/*																*/
/****************************************************************/
class Envelop :
	public MusicItem
{
//メンバー変数
private:
	unsigned	int	m_id;
//static	const	Command_Info	Command[];	//コマンドの情報

//メンバー関数
public:
				Envelop(MMLfile* MML, unsigned int _id, const char _strName[] = "==== [ Envelop ]====");
				~Envelop(void);
		void	getAsm(MusicFile* MUS);
};
