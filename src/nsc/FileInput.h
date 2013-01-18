#pragma once

class FileInput :
	public ifstream
{
protected:
//メンバー変数
				string		strFilename;
	unsigned	int			iLine;		//現在のライン
	unsigned	char		readData;

//メンバー関数
public:
					FileInput(void);
					~FileInput(void);
			void	fileopen(const char*	_strFileName);
			void	StreamPointerAdd(long iSize);
			void	StreamPointerMove(long iSize);
			void	Back(void);
			string*	GetFilename(void){return(&strFilename);};
unsigned	char	cRead();
unsigned	int		GetSize();
unsigned	int		GetLine(void){return(iLine);};
			void	SetLine(unsigned int i){iLine = i;};

};
