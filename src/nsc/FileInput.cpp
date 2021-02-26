/*******************************************************************************

			NES Sound Driver & Library	(NSD.lib)	MML Compiler

	Copyright (c) 2012 A.Watanabe (S.W.), All rights reserved.
	 For conditions of distribution and use, see copyright notice in "nsc.cpp".

*******************************************************************************/

#include "StdAfx.h"
#include "FileInput.h"

/****************************************************************/
/*					グローバル変数（クラスだけど・・・）		*/
/****************************************************************/
extern	OPSW*			cOptionSW;	//オプション情報へのポインタ変数

//==============================================================
//		デストラクタ
//--------------------------------------------------------------
//	●引数
//				無し
//	●返値
//				無し
//==============================================================
FileInput::FileInput(void):
	f_error(false),
	nowLine(1),
	readData(0)
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

	errno = 0;	//グローバル変数 errno を０に初期化
	clear();	//フラグのクリア

	open(_strFileName,ios_base::in | ios_base::binary);
	if(good()==false){
		f_error	= true;
	}
	strFilename.append(_strFileName);
};

//--------------------------------
//ファイルを開く　エラー処理付き
//--------------------------------
void	FileInput::fileopen(const char*	_strFileName,SearchPass* _pass)
{
	f_error	= true;

	//先ずは、そのまま
	errno = 0;	//グローバル変数 errno を０に初期化
	clear();	//フラグのクリア
	open(_strFileName,ios_base::in | ios_base::binary);
	if(cOptionSW->flag_SearchPass){
		perror(_strFileName);
	}

	if(good()==true){
		f_error	= false;
	} else {

		//検索パス
		size_t	iSize	= _pass->count();
		string	name;
		string	workName= string(_strFileName);
		size_t	loc		= workName.rfind('/');

		//指定のファイルにパスが書かれていたら、消す。
		if(loc != string::npos){
			workName.erase(0, loc);		//ファイル名のみ
		}

		for(size_t i=0; i<iSize; ++i){

#ifdef _WIN32
			//Windowsの場合は、相対パスも含めて検索する（UNIX系は不可）
			//指定の検索パスを基準とした相対パスも検索する。
			errno = 0;	//グローバル変数 errno を０に初期化
			clear();	//フラグのクリア

			name.assign(_pass->get(i));
			name.append(_strFileName);
			open(name.c_str(),ios_base::in | ios_base::binary);
			if(cOptionSW->flag_SearchPass){
				perror(name.c_str());
			}
			if(good()==true){
				f_error	= false;
				break;
			};
#endif
			//WINDOWS, UNIX系共通
			//検索パス＋ファイル名のみで検索。
			errno = 0;	//グローバル変数 errno を０に初期化
			clear();	//フラグのクリア

			name.assign(_pass->get(i));
			name.append(workName);
			open(name.c_str(),ios_base::in | ios_base::binary);
			if(cOptionSW->flag_SearchPass){
				perror(name.c_str());
			}
			if(good()==true){
				f_error	= false;
				break;
			};
		};
	};

	if(f_error == false){
		strFilename.append(_strFileName);	
	}
}

//--------------------------------
//相対シーク
//--------------------------------
void	FileInput::StreamPointerAdd(fstream::off_type iSize){
	seekg(iSize,ios::cur);
};

//--------------------------------
//絶対シーク
//--------------------------------
void	FileInput::StreamPointerMove(fstream::pos_type iSize){
	seekg(iSize);
};

//--------------------------------
//1Byte戻る
//--------------------------------
void	FileInput::Back(void)
{
	StreamPointerAdd(-1);
	if(readData == 0x0A){
		nowLine--;
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
		nowLine++;
	}

	return(readData);
};

//--------------------------------
//サイズ
//--------------------------------
std::streamsize	FileInput::GetSize(){

	std::streamsize	szData;
	fstream::pos_type	iDataT = tellg();

	seekg(0		,ios::end);
	szData = tellg();
	seekg(iDataT);

	return(szData);
};
