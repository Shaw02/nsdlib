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
	unsigned	int		m_id;
				int		Loop_Normal;
				int		Loop_Release;
				bool	Release;
				int		ptEnvelop;

//static	const	Command_Info	Command[];	//コマンドの情報

//メンバー関数
public:
				Envelop(MMLfile* MML, unsigned int _id, const wchar_t _strName[] = L"==== [ Envelop ]====");
				~Envelop(void);
		void	setHold(int length);
		void	sweep(MMLfile* MML);
		void	getAsm(MusicFile* MUS);
};
