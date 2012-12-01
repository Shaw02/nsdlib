#pragma once

class FileOutput :
	public ofstream
{
public:
		FileOutput(void);
		~FileOutput(void);
void	fileopen(const char*	strFileName);
void	StreamPointerAdd(long iSize);
void	StreamPointerMove(long iSize);
};
