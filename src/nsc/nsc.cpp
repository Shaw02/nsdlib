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
int	__cdecl	_tmain(int argc, _TCHAR* argv[])
{

	//==================================
	//クラスの作成
	cOptionSW	= new OPSW(argc,argv);							//オプション処理
	if(cOptionSW->cDebug & 0x01){
		cout << "\n============ [ 1st phase : Create Object ] ============\n" << endl;
	}
	MMLfile*	cMML		= new MMLfile(cOptionSW->strMMLname.c_str());
	MusicFile*	cSND		= new MusicFile(cMML);

	//==================================
	//アドレスの解決
	if(cOptionSW->cDebug & 0x02){
		cout << "\n============ [ 2nd phase : Address Setting ] ============\n" << endl;
	}
	cSND->SetOffset(0);
	cSND->Fix_Address();

	//==================================
	//バイナリの作成
	cSND->make_binary();

	//==================================
	//保存
	if(cOptionSW->saveASM == true){
		cSND->saveASM(cOptionSW->strASMname.c_str());
	}
	if(cOptionSW->saveBIN == true){
		cSND->saveBIN(cOptionSW->strBINname.c_str());
	}
	if(cOptionSW->saveC == true){
		cSND->saveC(cOptionSW->strCname.c_str());
	}
	if(cOptionSW->saveNSF == true){
		cSND->saveNSF(cOptionSW->strNSFname.c_str());
	}


	//==================================
	//クラスの削除
	delete	cSND;
	delete	cMML;
	delete	cOptionSW;

	return(0);
}
