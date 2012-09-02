#pragma once

class FileOutput :
	public ofstream
{
public:
		FileOutput(void);
		~FileOutput(void);
void	fileopen(const char*	strFileName);
void	StreamPointerAdd(__int32 iSize);
void	StreamPointerMove(__int32 iSize);
};
