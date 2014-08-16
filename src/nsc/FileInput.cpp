#include "StdAfx.h"
#include "FileInput.h"

//==============================================================
//		デストラクタ
//--------------------------------------------------------------
//	●引数
//				無し
//	●返値
//				無し
//==============================================================
FileInput::FileInput(void):
	readData(0),
	iLine(1)
{
}

//==============================================================
//		デストラクタ
//--------------------------------------------------------------
//	●引数
//				無し
//	●返値
//				無し
//==============================================================
FileInput::~FileInput(void)
{
}

//--------------------------------
//ファイルを開く　エラー処理付き
//--------------------------------
void	FileInput::fileopen(const char*	_strFileName){

	open(_strFileName,ios_base::in | ios_base::binary);
	if(good()==false){
		perror(_strFileName);
		nsc_exit(EXIT_FAILURE);
	};
	strFilename = _strFileName;
};

//--------------------------------
//相対シーク
//--------------------------------
void	FileInput::StreamPointerAdd(long iSize){
	seekg((long)iSize,ios::cur);
};

//--------------------------------
//絶対シーク
//--------------------------------
void	FileInput::StreamPointerMove(long iSize){
		seekg((long)iSize,ios::beg);
};

void	FileInput::Back(void)
{
	StreamPointerAdd(-1);
	if(readData == 0x0A){
		iLine--;
	}

	//更新
	read((char*)&readData, sizeof(unsigned char));
	StreamPointerAdd(-1);
}
//--------------------------------
//1Byte読み込み
//--------------------------------
unsigned	char	FileInput::cRead()
{
	read((char*)&readData, sizeof(unsigned char));
	if(readData == 0x0A){
		iLine++;
	}

	return(readData);
};
//--------------------------------
//サイズ
//--------------------------------
unsigned	int	FileInput::GetSize(){

	unsigned	int	iData;
	unsigned	int	iDataT = tellg();

	seekg(0		,ios::end);
	iData = tellg();
	seekg(iDataT,ios::beg);

	return(iData);
};
