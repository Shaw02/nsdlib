// 静的モデル

#ifndef __SUB__
#define __SUB__

// Include files
#include "MMLfile.h"
#include "TrackSet.h"
/****************************************************************/
/*																*/
/*			クラス定義											*/
/*																*/
/****************************************************************/
class Sub : public TrackSet
{

//メンバー変数
private:
	bool	f_Use;				//MML中で使われるか？

public:

	Sub(MMLfile* MML, unsigned int _id, wchar_t _strName[] = L"Sub");
	~Sub();

	void				getAsm(MusicFile* MUS);

	unsigned	int		TickCount(MusicFile* MUS, unsigned int iLength);
	unsigned	int		GetDefaultLength(void);
				void	setUse(void){f_Use = true;};
				bool	chkUse(void){return(f_Use);};

};// END CLASS DEFINITION Sub

#endif // __SUB__
