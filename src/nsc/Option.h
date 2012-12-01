
#pragma once

/****************************************************************/
/*																*/
/*			クラス定義											*/
/*																*/
/****************************************************************/
class OPSW {
//メンバー変数
public:
	unsigned	char		cDebug;			//デバッグ用
				bool		saveNSF;
				bool		saveASM;
				bool		opt;
				char		fHelp;			//ヘルプを指定したか？
				string		strMMLname;		//指定したMMLファイル名
				string		strNSFname;		//指定したNSFファイル名
				string		strASMname;		//指定したASMファイル名
				string		strCodeName;	//ROM Codeの名前

//メンバー関数
public:
		OPSW();								//初期化のみ
		OPSW(int argc, char* argv[]);		//引数内容から、クラスを初期化＆ファイルオープン
		~OPSW();							//ファイルクローズ
private:
void	opError(const wchar_t *stErrMsg);		//オプションエラー
void	print_help();						//help message
};
