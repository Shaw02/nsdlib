#pragma once
#include "MusicItem.h"

/****************************************************************/
/*																*/
/*			クラス定義											*/
/*																*/
/****************************************************************/
class DPCM :
	public FileInput, public MusicItem
{
//メンバー変数
private:
	unsigned	int		m_id;
				bool	f_Use;				//MML中で使われるか？
	unsigned	char	_DPCM_size;

//メンバー関数
public:
						DPCM(MMLfile* MML, const char* dmcfile, unsigned int _id, const _CHAR _strName[] = _T("==== [ DPCM ]===="));
						~DPCM(void);
	unsigned	char	getDPCMsize(void){return(_DPCM_size);};
				void	getAsm(MusicFile* MUS);
};
