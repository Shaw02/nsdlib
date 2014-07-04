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
				bool	f_Use;				//MML中で使われるか？
	unsigned	char	_DPCM_size;

//static	const	Command_Info	Command[];	//コマンドの情報

//メンバー関数
public:
				DPCM(FileInput* DPCMfile, unsigned int _id, const wchar_t _strName[] = L"==== [ DPCM ]====");
				~DPCM(void);
	unsigned	char	getDPCMsize(void){return(_DPCM_size);};
				void	getAsm(MusicFile* MUS);
};
