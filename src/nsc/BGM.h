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
	BGM(MMLfile* MML, unsigned int _id, const char _strName[] = "==== [ BGM ]====");
	~BGM(void);
	void	getAsm(MusicFile* MUS);
};
