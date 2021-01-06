/*******************************************************************************

			NES Sound Driver & Library	(NSD.lib)	MML Compiler

	Copyright (c) 2012 A.Watanabe (S.W.), All rights reserved.
	 For conditions of distribution and use, see copyright notice in "nsc.cpp".

*******************************************************************************/

#pragma once

/****************************************************************/
/*																*/
/*			クラス定義											*/
/*																*/
/****************************************************************/

#define	DEBUG_Create		0x01	//0x01:	Phase [1] : Creating Class Object process
#define DEBUG_Meta_Create	0x02	//0x02:	Phase [4] : Outputing Music File
#define DEBUG_Macros		0x04	//0x04	  - Debug for Macro, Patch
#define	DEBUG_Optimize		0x08	//0x08:	Phase [2] : Optimizing process
#define	DEBUG_SetAddress	0x10	//0x10:	Phase [3] : Setting Address
#define	DEBUG_Meta_Set		0x20	//0x20:	Phase [3] : Setting Address
#define	DEBUG_Delete		0x40	//0x40:	Phase [5] : Delete Class Object
#define DEBUG_Meta_Delete	0x80	//0x80:	Phase [5] : Delete Class Object



/****************************************************************/
/*																*/
/*			クラス定義											*/
/*																*/
/****************************************************************/
class OPSW {
//メンバー変数
public:
				bool		fOptionError;	//オプションでエラーがあった場合
				bool		fHelp;			//ヘルプを指定したか？
				bool		fErr;			//エラー出力先	true:標準エラー出力／false:標準出力
				bool		saveNSF;		//.nsf を出力するか
				bool		saveNSFe;		//.nsfeを出力するか
				bool		saveASM;		//.s   を出力するか
				bool		flag_Optimize;	//最適化を有効にするか？
				bool		flag_OptObj;	//最適化を有効にするか？
				bool		flag_OptSeq;	//最適化を有効にするか？
		//		bool		flag_TickCount;	//TickCountを無効にするか？
				bool		flag_SearchPass;//SearchPassの処理結果を出力するか
				int			iNSF_version;	//NSFのヴァージョン

	unsigned	int			iDebug;			//デバッグ用
				string		strMMLname;		//指定したMMLファイル名
				string		strNSFname;		//指定したNSFファイル名
				string		strNSFename;	//指定したNSFeファイル名
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
void	opError(const _CHAR *stErrMsg);		//オプションエラー
void	print_help();						//help message
};
