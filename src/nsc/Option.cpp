#include "stdafx.h"
#include "Option.h"

//==============================================================
//		オプション処理
//--------------------------------------------------------------
//	●引数
//			int argc		オプション文字列の数
//			_TCHAR* argv[]	オプション文字列
//	●返値
//			SND.name[]		変換元のSNDファイル
//			MML.name[]		変換先のMMLファイル
//	●備考
//			オプションにファイル名が指定されない場合は、ヘルプ表示して終了
//==============================================================
OPSW::OPSW(int argc, _TCHAR* argv[]):
	//初期化設定
	fHelp(0),		//ヘルプは、デフォルトは表示しない。
	saveNSF(false),
	saveBIN(false),
	saveASM(false),
	saveC(false),
	cDebug(0)
{

	//----------------------------------
	//■Local 変数
	int		iCount;				//whileのカウント用
	int		iResult;			//汎用

	//Option処理用
	int		iOptionChk;			//オプションチェック用　ポインタ
	char	cOption;			//オプションチェック用　文字
	char	iFlagFilnameExt;	//拡張子あったかのフラグ

	//----------------------------------
	//■オプション処理
	iCount=1;	//コマンド名は飛ばす
	while(iCount!=argc)
	{
		//--------------
		//オプションスイッチにスラッシュがあるか確認
		if((argv[iCount][0]=='/')||(argv[iCount][0]=='-')){

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
				case 'a' :
				case 'A' :
					saveASM = true;
					break;
				//--------
				//バイナリーへ
				case 'b' :
				case 'B' :
					saveBIN = true;
					break;
				//--------
				//Ｃ言語へ
				case 'c' :
				case 'C' :
					saveC = true;
					break;
				//--------
				//Ｃ言語へ
				case 'n' :
				case 'N' :
					saveNSF = true;
					break;
				//--------
				//Debugの指定
				case 'D' :
					iResult=sscanf(argv[iCount],"/D%d",&cDebug);
					if((iResult==NULL)||(iResult==EOF)){
						opError("-D");
						break;
					};
					break;
				//--------
				//ファイルの指定
				case 'f' :
				case 'F' :
					//先に、ファイル名が書いてあるかチェック。
					if(argv[iCount][3]==0){
						opError("/F ファイル名が書いてありません。");
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
								strASMname+=".asm";
							};
						} else {
							opError("-fa ASM ファイルが2回以上指定されました。");
							break;
						};
						break;
					case 'B' :
					case 'b' :
						//既に指定されている？
						if(strBINname.empty()){
							iFlagFilnameExt=0;		//拡張子の有無　Reset
							iOptionChk=0;
							while((cOption=argv[iCount][iOptionChk+3])!=0)
							{
								strBINname+=cOption;
								if(cOption=='.'){iFlagFilnameExt=1;};
								iOptionChk++;
							};
							if(iFlagFilnameExt==0){
								strBINname+=".bin";
							};
						} else {
							opError("-fb BIN ファイルが2回以上指定されました。");
							break;
						};
						break;
					case 'C' :
					case 'c' :
						//既に指定されている？
						if(strCname.empty()){
							iFlagFilnameExt=0;		//拡張子の有無　Reset
							iOptionChk=0;
							while((cOption=argv[iCount][iOptionChk+3])!=0)
							{
								strCname+=cOption;
								if(cOption=='.'){iFlagFilnameExt=1;};
								iOptionChk++;
							};
							if(iFlagFilnameExt==0){
								strCname+=".c";
							};
						} else {
							opError("-fc C ファイルが2回以上指定されました。");
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
							opError("-fn NSF ファイルが2回以上指定されました。");
							break;
						};
						break;
					default :
						opError("-f");
						break;
					};
				break;
				//--------
				//デフォルト
				default :
					opError("");
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
				opError("MMLファイルが2回以上指定されました。");
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

	//--------------
	//ファイルの指定が無かった場合
	if(strBINname.empty()){
		iOptionChk=0;		
		while((cOption=strMMLname[iOptionChk])!='.')
		{
			strBINname+=cOption;
			iOptionChk++;
		};
		strBINname+=".bin";
	};

	if(strASMname.empty()){
		iOptionChk=0;		
		while((cOption=strMMLname[iOptionChk])!='.')
		{
			strASMname+=cOption;
			iOptionChk++;
		};
		strASMname+=".asm";
	};

	if(strCname.empty()){
		iOptionChk=0;		
		while((cOption=strMMLname[iOptionChk])!='.')
		{
			strCname+=cOption;
			iOptionChk++;
		};
		strCname+=".c";
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


	//--------------
	//

	//	to do	その他のオプションを追加したときは、この辺に追記する。

	//----------
	//Debug用 表示
//	cout << "MML = " << strMMLname << endl;
//	cout << "NSF = " << strNSFname << endl;
//	cout << "BIN = " << strBINname << endl;
//	cout << "ASM = " << strASMname << endl;
//	cout << "C   = " << strCname << endl;


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

	cout	<<	"MML Compiler for NES Sound Driver & Library\n"
				"\n"
				"  Usage : nsc [ -options ] [file(.mml)]\n"
				"\n"
				"  -a			Compile to assembly langage.\n"
				"  -b			Compile to binary.\n"
				"  -c			Compile to C langage.\n"
				"  -n			Compile to NSF.\n"
				"  -fa[file(.asm)]	Name the output assembly langage file.\n"
				"  -fb[file(.bin)]	Name the output binary file.\n"
				"  -fc[file(.c  )]	Name the output C langage file.\n"
				"  -fn[file(.nsf)]	Name the output NSF file.\n"
				"  -h			Print the this help."	<<	endl;

	exit(EXIT_SUCCESS);

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
//			char *stErrMsg	エラーメッセージ
//	●返値
//			無し
//==============================================================
void OPSW::opError(const char *stErrMsg){

	cerr << "オプションが不正です。：" << stErrMsg << endl;
	exit(EXIT_FAILURE);

};
