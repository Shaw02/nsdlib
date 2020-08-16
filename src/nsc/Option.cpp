/*******************************************************************************

			NES Sound Driver & Library	(NSD.lib)	MML Compiler

	Copyright (c) 2012 A.Watanabe (S.W.), All rights reserved.
	 For conditions of distribution and use, see copyright notice in "nsc.cpp".

*******************************************************************************/

#include "StdAfx.h"
#include "Option.h"


#ifdef _WIN32
// WINDOWSのみ、'/'に対応する。
#define OPTCHK (argv[iCount][0]=='/')||(argv[iCount][0]=='-')
#else
// unix系ではスラッシュはディレクトリの区切りに使われる為、失敗する事がある
#define OPTCHK (argv[iCount][0]=='-')
#endif

//==============================================================
//		オプション処理
//--------------------------------------------------------------
//	●引数
//			int		argc	オプション文字列の数
//			_TCHAR*	argv[]	オプション文字列
//	●返値
//			無し
//	●備考
//			オプションにファイル名が指定されない場合は、ヘルプ表示して終了
//==============================================================
OPSW::OPSW(int argc, char* argv[]):
	//初期化設定
	iDebug(0),
	fErr(false),
	saveNSF(false),
	saveASM(false),
	flag_Optimize(false),
	flag_OptObj(true),
	flag_OptSeq(true),
	flag_SearchPass(false),
	fHelp(0)		//ヘルプは、デフォルトは表示しない。
{

	//----------------------------------
	//■Local 変数
	
	int		iCount;				//whileのカウント用
	int		iResult;			//汎用

	//Option処理用
	int		iOptionChk;			//オプションチェック用　ポインタ
	char	cOption;			//オプションチェック用　文字
	char	iFlagFilnameExt;	//拡張子あったかのフラグ

	//検索パス
	string	str_code	= "";
	string	str_dmc		= "";
	string	str_inc		= "";

	//----------------------------------
	//■オプション処理
	iCount=1;	//コマンド名は飛ばす
	while(iCount!=argc)
	{
		//--------------
		//オプションスイッチにハイフン(orスラッシュ)があるか確認
		if(OPTCHK){

			//--------------
			//◆Option Switch	（スラッシュがあった場合の処理）
			switch(argv[iCount][1]){
				//--------
				//Help表示
				case 'h' :
				case 'H' :
				case '?' :
					fHelp=1;
					break;
				//--------
				//アセンブリ言語へ
				case 'o' :
				case 'O' :
					{
						bool fTemp;
						switch(argv[iCount][3]){
							case '0' :
							case '+' :
								fTemp = true;
								break;
							case '1' :
							case '-' :
								fTemp = false;
								break;
							default :
								opError(_T("-o"));
								break;
						}
						switch(argv[iCount][2]){
							case 'd' :
							case 'D' :
								flag_Optimize = fTemp;
								break;
							case 'o' :
							case 'O' :
								flag_OptObj = fTemp;
								break;
							case 's' :
							case 'S' :
								flag_OptSeq = fTemp;
								break;
							default :
								opError(_T("-o"));
								break;
						}
						break;
					}
				//--------
				//アセンブリ言語へ
				case 'a' :
				case 'A' :
					saveASM = true;
					break;
				//--------
				//ＮＳＦへ
				case 'n' :
				case 'N' :
					saveNSF = true;
					break;
				//--------
				//標準エラー出力へ
				case 'e' :
				case 'E' :
					fErr = true;
					break;
				//--------
				//Tick Count
		//		case 't' :
		//		case 'T' :
		//			flag_TickCount = false;
		//			break;
				//--------
				//Search Pass
				case 's' :
				case 'S' :
					flag_SearchPass = true;
					break;
				//--------
				//Debugの指定
				case 'D' :
					//0x01:	Phase [1] : Creating Class Object process
					//0x10:	Phase [2] : Optimizing process
					//0x20:	Phase [3] : Setting Address
					//0x40:	Phase [4] : Outputing Music File
					//0x80:	Phase [5] : Delete Class Object
					int	i;
					iResult=sscanf(argv[iCount],"-D%d",&i);
					iDebug = i;
					if((iResult==NULL)||(iResult==EOF)){
						opError(_T("-D"));
						break;
					};
					break;
				//--------
				//Debugの指定
				case 'l' :
				case 'L' :
					//既に指定されている？
					if(strCodeName.empty()){
						iFlagFilnameExt=0;		//拡張子の有無　Reset
						iOptionChk=0;
						while((cOption=argv[iCount][iOptionChk+2])!=0)
						{
							strCodeName+=cOption;
							if(cOption=='.'){iFlagFilnameExt=1;};
							iOptionChk++;
						};
						if(iFlagFilnameExt==0){
							strCodeName+=".bin";
						};
					} else {
						opError(_T("-l Code ファイルが2回以上指定されました。"));
						break;
					};
					break;
				//--------
				//ROMコードの検索パス
				case 'c' :
				case 'C' :
					str_code += &argv[iCount][2];
					str_code.append(1, _PATH_SPLIT);
					break;
				//--------
				//⊿PCMの検索パス
				case 'p' :
				case 'P' :
					str_dmc += &argv[iCount][2];
					str_dmc.append(1, _PATH_SPLIT);
					break;
				//--------
				//インクルードファイルの検索パス
				case 'i' :
				case 'I' :
					str_inc += &argv[iCount][2];
					str_inc.append(1, _PATH_SPLIT);
					break;
				//--------
				//ファイルの指定
				case 'f' :
				case 'F' :
					//先に、ファイル名が書いてあるかチェック。
					if(argv[iCount][3]==0){
						opError(_T("/F ファイル名が書いてありません。"));
						break;
					};
					switch(argv[iCount][2]){
					//--------
					//MMLファイルの指定
					case 'A' :
					case 'a' :
						//既に指定されている？
						if(strASMname.empty()){
							iFlagFilnameExt=0;		//拡張子の有無　Reset
							iOptionChk=0;
							while((cOption=argv[iCount][iOptionChk+3])!=0)
							{
								strASMname+=cOption;
								if(cOption=='.'){iFlagFilnameExt=1;};
								iOptionChk++;
							};
							if(iFlagFilnameExt==0){
								strASMname+=".s";
							};
						} else {
							opError(_T("-fa ASM ファイルが2回以上指定されました。"));
							break;
						};
						break;
					case 'N' :
					case 'n' :
						//既に指定されている？
						if(strNSFname.empty()){
							iFlagFilnameExt=0;		//拡張子の有無　Reset
							iOptionChk=0;
							while((cOption=argv[iCount][iOptionChk+3])!=0)
							{
								strNSFname+=cOption;
								if(cOption=='.'){iFlagFilnameExt=1;};
								iOptionChk++;
							};
							if(iFlagFilnameExt==0){
								strNSFname+=".nsf";
							};
						} else {
							opError(_T("-fn NSF ファイルが2回以上指定されました。"));
							break;
						};
						break;
					default :
						opError(_T("-f"));
						break;
					};
				break;
				//--------
				//デフォルト
				default :
					opError(_T(""));
					break;
			};

		} else{

			//--------------
			//◆ファイル名	（スラッシュが無かった場合の処理）
			//既に指定されている？
			if(strMMLname.empty()){
				iFlagFilnameExt=0;		//拡張子の有無　Reset
				iOptionChk=0;		
				while((cOption=argv[iCount][iOptionChk])!=0)
				{
					strMMLname+=cOption;
					if(cOption=='.'){iFlagFilnameExt=1;};
					iOptionChk++;
				};
				if(iFlagFilnameExt==0){
					strMMLname+=".mml";
				};
			} else {
				opError(_T("MMLファイルが2回以上指定されました。"));
				break;
			};

		};

		//--------------
		//◆次のオプション
		iCount++;
	};

	//----------------------------------
	//◆オプションで指定された事を処理する。

	//--------------
	//ヘルプ表示
	//ファイル名が書かれなかった場合も、ヘルプを表示する。
	if((fHelp==1)||(strMMLname.empty())){print_help();};

	if(strASMname.empty()){
		iOptionChk=0;		
		while((cOption=strMMLname[iOptionChk])!='.')
		{
			strASMname+=cOption;
			iOptionChk++;
		};
		strASMname+=".s";
	};

	if(strNSFname.empty()){
		iOptionChk=0;		
		while((cOption=strMMLname[iOptionChk])!='.')
		{
			strNSFname+=cOption;
			iOptionChk++;
		};
		strNSFname+=".nsf";
	};

	//----------------------------------
	//◆検索パスの設定

	//MMLのディレクトリを取得
	iCount	= 0;
	iResult	= 0;
	while(iCount < (int)strMMLname.size()){
		if((strMMLname[iCount] == '\\') || (strMMLname[iCount] == '/')){
			iResult = iCount;
		}
		iCount++;
	}

	//優先順位１　カレントパス
	//優先順位２　MMLファイルが存在するパス
	if(iResult > 0){
		string	str_mmldir = "";
		iCount	= 0;
		while(iCount < iResult){
			str_mmldir += strMMLname[iCount];
			iCount++;
		}
		m_pass_code.add(&str_mmldir);
		m_pass_dmc.add(&str_mmldir);
		m_pass_inc.add(&str_mmldir);
	}

	//優先順位３　環境変数で指定のパス
	m_pass_code.add(getenv("PATH"));
	m_pass_dmc.add(getenv("DMC_INCLUDE"));

	//優先順位４　オプションで指定のパス
	m_pass_code.add(&str_code);
	m_pass_dmc.add(&str_dmc);
	m_pass_inc.add(&str_inc);



	//----------------------------------
	//◆

	//	to do	その他のオプションを追加したときは、この辺に追記する。

	//----------
	//Debug用 表示
//	cout << "MML = " << strMMLname << endl;
//	cout << "NSF = " << strNSFname << endl;
//	cout << "BIN = " << strBINname << endl;
//	cout << "ASM = " << strASMname << endl;
//	cout << "C   = " << strCname << endl;

/*
	cout << "code:" << endl;
	m_pass_code.debug();

	cout << "dmc:" << endl;
	m_pass_dmc.debug();

	cout << "inc:" << endl;
	m_pass_inc.debug();
*/

};
//==============================================================
//		ヘルプメッセージ
//--------------------------------------------------------------
//	●引数
//			なし
//	●返値
//			無し
//==============================================================
void	OPSW::print_help(){

	_COUT	<<	_T("MML Compiler for NES Sound Driver & Library\n")
				_T("\n")
				_T("  Usage : nsc [ -options ] [file(.mml)]\n")
				_T("\n")
				_T("  -A			Compile to assembly langage.\n")
				_T("  -N			Compile to NSF music format.\n")
				_T("  -E			Error/Warning messages out the stadard error.\n")
			//	_T("  -T			Disable to output the tick counting result.\n")
				_T("  -Od[+/-]		Optimize the NSF bank struct of the delta-PCM.\n")
				_T("  -Oo[+/-]		Optimize the object data.\n")
				_T("  -Os[+/-]		Optimize the sequence data.\n")
				_T("  -S			Enable outout the search pass result.\n")
				_T("  -L[file(.bin)]	Filename of the rom code for NSF.\n")
				_T("  -FA[file(.s  )]	Filename of the output assembly langage file.\n")
				_T("  -FN[file(.nsf)]	Filename of the output NSF music format.\n")
				_T("  -C[dir]		Search pass of the rom code for NSF.\n")
				_T("  -P[dir]		Search pass of the delta-pcm.\n")
				_T("  -I[dir]		Search pass of the include file.\n")
				_T("  -H			Print the this help.")	<<	endl;

	nsc_exit(EXIT_SUCCESS);

};
//==============================================================
//		デストラクト
//--------------------------------------------------------------
//	●引数
//			なし
//	●返値
//			無し
//==============================================================
OPSW::~OPSW(){


};
//==============================================================
//		エラー処理	（プロセスも終了する）
//--------------------------------------------------------------
//	●引数
//			const	_CHAR	*stErrMsg	エラーメッセージ
//	●返値
//			無し
//==============================================================
void OPSW::opError(const _CHAR *stErrMsg){

	if(fErr == true){
		_CERR << _T("オプションが不正です。：") << stErrMsg << endl;
	} else {
		_COUT << _T("オプションが不正です。：") << stErrMsg << endl;
	}
	nsc_exit(EXIT_FAILURE);

};
