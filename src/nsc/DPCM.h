#pragma once
#include "MusicItem.h"

/****************************************************************/
/*																*/
/*			クラス定義											*/
/*																*/
/****************************************************************/
class DPCM :
	public MusicItem
{
//メンバー変数
private:
	unsigned	int		m_id;
	unsigned	char	_DPCM_size;
//static	const	Command_Info	Command[];	//コマンドの情報

//メンバー関数
public:
				DPCM(FileInput* DPCMfile, unsigned int _id, const char _strName[] = "==== [ DPCM ]====");
				~DPCM(void);
	unsigned	char	getDPCMsize(void){return(_DPCM_size);};
				void	getAsm(MusicFile* MUS);
};
