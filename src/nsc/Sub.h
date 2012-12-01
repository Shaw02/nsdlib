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

public:

	Sub(MMLfile* MML, unsigned int _id, wchar_t _strName[] = L"==== [ Sub ]====");
	~Sub();
	void	getAsm(MusicFile* MUS);

};// END CLASS DEFINITION Sub

#endif // __SUB__
