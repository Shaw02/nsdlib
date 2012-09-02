#pragma once
#include "trackset.h"

/****************************************************************/
/*																*/
/*			ƒNƒ‰ƒX’è‹`											*/
/*																*/
/****************************************************************/
class BGM :
	public TrackSet
{
public:
	BGM(MMLfile* MML, const char _strName[] = "==== [ BGM ]====");
	~BGM(void);
};
