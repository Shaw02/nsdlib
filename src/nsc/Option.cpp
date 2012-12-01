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
OPSW::OPSW(int argc, char* argv[]):
	//初期化設定
	fHelp(0),		//ヘルプは、デフォルトは表示しない。
	saveNSF(false),
	saveASM(false),
	opt(false),
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
				case 'o' :
				case 'O' :
					opt = true;
					break;
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
				//Debugの指定
				case 'D' :
					iResult=sscanf(argv[iCount],"/D%d",&cDebug);
					if((iResult==NULL)||(iResult==EOF)){
						opError(L"-D");
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
						opError(L"-l Code ファイルが2回以上指定されました。");
						break;
					};
					break;
				//--------
				//ファイルの指定
				case 'f' :
				case 'F' :
					//先に、ファイル名が書いてあるかチェック。
					if(argv[iCount][3]==0){
						opError(L"/F ファイル名が書いてありません。");
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
							opError(L"-fa ASM ファイルが2回以上指定されました。");
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
							opError(L"-fn NSF ファイルが2回以上指定されました。");
							break;
						};
						break;
					default :
						opError(L"-f");
						break;
					};
				break;
				//--------
				//デフォルト
				default :
					opError(L"");
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
				opError(L"MMLファイルが2回以上指定されました。");
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

	wcout	<<	L"MML Compiler for NES Sound Driver & Library\n"
				L"\n"
				L"  Usage : nsc [ -options ] [file(.mml)]\n"
				L"\n"
				L"  -a			Compile to assembly langage.\n"
				L"  -n			Compile to NSF music format.\n"
				L"  -l[file(.bin)]	Filename of the rom code for NSF.\n"
				L"  -fa[file(.s  )]	Filename of the output assembly langage file.\n"
				L"  -fn[file(.nsf)]	Filename of the output NSF music format.\n"
				L"  -h			Print the this help."	<<	endl;

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
void OPSW::opError(const wchar_t *stErrMsg){

	wcerr << L"オプションが不正です。：" << stErrMsg << endl;
	exit(EXIT_FAILURE);

};
