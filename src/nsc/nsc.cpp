// nsc.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"

/****************************************************************/
/*					グローバル									*/
/****************************************************************/
		OPSW*			cOptionSW;	//どっからでもアクセスする。

//==============================================================
//		メイン関数
//--------------------------------------------------------------
//	●引数
//		int		argc	コマンドライン引数	
//		_TCHAR*	argv[]	コマンドライン引数
//	●返値
//		int			エラーコード
//==============================================================
int	main(int argc, char* argv[])
{
	unsigned	int	i;

#ifdef	_WIN32
	locale::global(std::locale("japanese"));
#else
	setlocale(LC_ALL, "ja_JP.UTF-8");
#endif

	//==================================
	//クラスの作成
	cOptionSW	= new OPSW(argc,argv);							//オプション処理
	if(cOptionSW->cDebug & 0x01){
		wcout << L"\n============ [ 1st phase : Object Creating ] ============" << endl;
	}
	wcout << L"----------------------------------------" << endl;
	wcout << L"*Object creating process" << endl;

	MMLfile*	cMML		= new MMLfile(cOptionSW->strMMLname.c_str());
	MusicFile*	cSND		= new MusicFile(cMML, cOptionSW->strCodeName);

	//==================================
	//アドレスの解決
	if(cOptionSW->cDebug & 0x02){
		wcout << L"\n============ [ 2nd phase : Address Setting ] ============" << endl;
	}
	wcout << L"----------------------------------------" << endl;
	wcout << L"*Address settlement process" << endl;

	i = cSND->SetOffset(0);
	cout << "  Music Size = " << setfill(' ')  << setw(5) << i << " [Byte]" << endl;
	i = cSND->SetDPCMOffset(i);
	cout << "  DPCM Size  = " << setfill(' ')  << setw(5) << i << " [Byte]" << endl;

	cSND->Fix_Address();

	//==================================
	//保存
	if(cOptionSW->cDebug & 0x04){
		wcout << L"\n============ [ 3rd phase : Music File Outputing ] ============" << endl;
	}

		if((cOptionSW->saveNSF == true) || ((cOptionSW->saveNSF == false)&&(cOptionSW->saveASM == false))){
		cSND->saveNSF(cOptionSW->strNSFname.c_str(), cOptionSW->opt);
	}

	if(cOptionSW->saveASM == true){
		cSND->saveASM(cOptionSW->strASMname.c_str());
	}

	//==================================
	//Tick Count
	if(cOptionSW->flag_TickCount == true){

		if(cOptionSW->cDebug & 0x04){
			wcout << L"\n============ [ 4th phase : Tick Counting ] ============" << endl;
		}
		wcout << L"----------------------------------------" << endl;
		wcout << L"*Tick counting process" << endl;

		cSND->TickCount();

	}

	//==================================
	//クラスの削除
	delete	cSND;
	delete	cMML;
	delete	cOptionSW;

	return(0);
}
