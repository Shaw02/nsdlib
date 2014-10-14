// 静的モデル

#ifndef __SE__
#define __SE__

// Include files
#include "MMLfile.h"
#include "TrackSet.h"
/****************************************************************/
/*																*/
/*			クラス定義											*/
/*																*/
/****************************************************************/
class SE : public TrackSet
{

public:

	SE(MMLfile* MML, unsigned int _id,	 _CHAR _strName[] = _T("SE"));
	~SE();
	void	getAsm(MusicFile* MUS);


};// END CLASS DEFINITION SE

#endif // __SE__
