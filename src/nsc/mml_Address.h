#pragma once
#include "musicevent.h"

/****************************************************************/
/*																*/
/*			クラス定義											*/
/*																*/
/****************************************************************/
class mml_Address :
	public MusicEvent
{
//メンバー変数
	unsigned	int	m_id;
//メンバー関数
public:
	mml_Address(unsigned char _code, const char _strName[]="Address");
	~mml_Address(void);

				void	set_Address(unsigned int _addr);
//	unsigned	int		get_Address(void);
				void	set_id(unsigned int _id);
	unsigned	int		get_id(void);
};
