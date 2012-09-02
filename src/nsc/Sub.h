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

	Sub(MMLfile* MML, char _strName[] = "==== [ Sub ]====");

	~Sub();

};// END CLASS DEFINITION Sub

#endif // __SUB__
