
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
				bool		fErr;			//エラー出力先	true:標準エラー出力／false:標準出力
				bool		saveNSF;		//.nsf を出力するか
				bool		saveASM;		//.s   を出力するか
				bool		opt;			//最適化を有効にするか？
				bool		flag_TickCount;	//TickCountを無効にするか？
				bool		flag_SearchPass;//SearchPassの処理結果を出力するか
				char		fHelp;			//ヘルプを指定したか？
				string		strMMLname;		//指定したMMLファイル名
				string		strNSFname;		//指定したNSFファイル名
				string		strASMname;		//指定したASMファイル名
				string		strCodeName;	//ROM Codeの名前

				SearchPass	m_pass_code;	//検索パス	".bin"ファイル
				SearchPass	m_pass_dmc;		//検索パス	".dmc"ファイル
				SearchPass	m_pass_inc;		//検索パス	インクルードファイル

//メンバー関数
public:
		OPSW();								//初期化のみ
		OPSW(int argc, char* argv[]);		//引数内容から、クラスを初期化＆ファイルオープン
		~OPSW();							//ファイルクローズ
private:
void	opError(const wchar_t *stErrMsg);		//オプションエラー
void	print_help();						//help message
};
