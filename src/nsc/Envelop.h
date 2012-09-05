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
//static	const	Command_Info	Command[];	//コマンドの情報

//メンバー関数
public:
				Envelop(MMLfile* MML, const char _strName[] = "==== [ Envelop ]====");
				~Envelop(void);
};
