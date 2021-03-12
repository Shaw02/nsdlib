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
	fOptionError(false),
	fHelp(false),			//ヘルプは、デフォルトは表示しない。
	fErr(false),
	saveNSF(false),
	saveNSFe(false),
	saveASM(false),
	flag_Optimize(false),
	flag_OptObj(true),
	flag_OptSeq(true),
	flag_SearchPass(false),
	iNSF_version(2),
	iDebug(0)
{

	try {

		//----------------------------------
		//■Local 変数
		
		int		iCount = 1;			//whileのカウント用（コマンド名は飛ばす）
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
						fHelp=true;
						break;
					//--------
					//最適化設定
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
									throw string(argv[iCount]);
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
									throw string(argv[iCount]);
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
					//NSFeへ
					case 'x' :
					case 'X' :
						saveNSFe = true;
						break;
					//--------
					//標準エラー出力へ
					case 'e' :
					case 'E' :
						fErr = true;
						break;
					//--------
					//ＮＳＦへ
					case 'v' :
					case 'V' :
						switch(argv[iCount][2]){
							case 0x00 :
							case '1' :
								iNSF_version = 1;
								break;
							case '2' :
								iNSF_version = 2;
								break;
							default :
								throw string(argv[iCount]);
								break;
						}
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
						if((iResult==0)||(iResult==EOF)){
							throw string(argv[iCount]);
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
							throw _T("-l Code ファイルが2回以上指定されました。");
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
							throw _T("-f ファイル名が書いてありません。");
							break;
						};
						switch(argv[iCount][2]){
						//--------
						//Assemblyファイルの指定
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
								throw _T("-fa ASM ファイルが2回以上指定されました。");
								break;
							};
							break;
						//--------
						//NSFファイルの指定
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
								throw _T("-fn NSF ファイルが2回以上指定されました。");
								break;
							};
							break;
						//--------
						//NSFeファイルの指定
						case 'X' :
						case 'x' :
							//既に指定されている？
							if(strNSFename.empty()){
								iFlagFilnameExt=0;		//拡張子の有無　Reset
								iOptionChk=0;
								while((cOption=argv[iCount][iOptionChk+3])!=0)
								{
									strNSFename+=cOption;
									if(cOption=='.'){iFlagFilnameExt=1;};
									iOptionChk++;
								};
								if(iFlagFilnameExt==0){
									strNSFename+=".nsfe";
								};
							} else {
								throw _T("-fn NSFe ファイルが2回以上指定されました。");
								break;
							};
							break;
						default :
							throw string(argv[iCount]);
							break;
						};
					break;
					//--------
					//デフォルト
					default :
						throw string(argv[iCount]);
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
					throw _T("MMLファイルが2回以上指定されました。");
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
		if((fHelp==true)||(strMMLname.empty())){print_help();};

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

		if(strNSFename.empty()){
			iOptionChk=0;		
			while((cOption=strMMLname[iOptionChk])!='.')
			{
				strNSFename+=cOption;
				iOptionChk++;
			};
			strNSFename+=".nsfe";
		};

		//----------------------------------
		//◆検索パスの設定

		//MMLのディレクトリを取得
		iResult	= 0;
		for(iCount=0; iCount<(int)strMMLname.size(); ++iCount){
			if((strMMLname[iCount] == '\\') || (strMMLname[iCount] == '/')){
				iResult = iCount;
			}
		}

		//優先順位１　カレントパス
		//優先順位２　MMLファイルが存在するパス
		if(iResult > 0){
			string	str_mmldir = "";
			for(iCount=0; iCount<iResult; ++iCount){
				str_mmldir += strMMLname[iCount];
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
	//	cout << "NSFe = " << strNSFename << endl;
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

	} catch (const _CHAR* stErrMsg) {
		if(fErr == true){
			_CERR	<<	_T("不正なオプション：");
			_CERR	<<	stErrMsg << endl;
		} else {
			_COUT	<<	_T("不正なオプション：");
			_COUT	<<	stErrMsg << endl;
		}
		fOptionError = true;	//オプション処理でエラーが発生した。
		throw EXIT_FAILURE;		//リスローする
	} catch (const string& str) {
		if(fErr == true){
			_CERR	<<	_T("不正なオプション：") << str.c_str() << endl;
		} else {
			_COUT	<<	_T("不正なオプション：") << str.c_str() << endl;
		}
		fOptionError = true;	//オプション処理でエラーが発生した。
		throw EXIT_FAILURE;		//リスローする
	} catch (...) {
		fOptionError = true;	//オプション処理でエラーが発生した。
		throw;					//リスローする
	}
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
				_T("  -X			Compile to NSFe music format.\n")
				_T("  -V[num]		Specify the NSF Version.\n")
			//	_T("  -T			Disable to output the tick counting result.\n")
				_T("  -Od[+/-]		Optimize the NSF bank struct of the delta-PCM.\n")
				_T("  -Oo[+/-]		Optimize the object data.\n")
				_T("  -Os[+/-]		Optimize the sequence data.\n")
				_T("  -E			Error/Warning messages out the stadard error.\n")
				_T("  -S			Enable outout the search pass result.\n")
				_T("  -L[file(.bin)]	Filename of the rom code for NSF.\n")
				_T("  -FA[file(.s  )]	Filename of the output assembly langage file.\n")
				_T("  -FN[file(.nsf)]	Filename of the output NSF music format.\n")
				_T("  -FX[file(.nsfe)]	Filename of the output NSFe music format.\n")
				_T("  -C[dir]		Search pass of the rom code for NSF.\n")
				_T("  -P[dir]		Search pass of the delta-pcm.\n")
				_T("  -I[dir]		Search pass of the include file.\n")
				_T("  -H			Print the this help.")	<<	endl;

	throw EXIT_SUCCESS;
};
