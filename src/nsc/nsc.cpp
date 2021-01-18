/***************************************************************************

				NES Sound Driver & Library	(NSD.lib)
					Music Macro Language (MML) Compiler

****************************************************************************

   Copyright (c) 2012 A.Watanabe (S.W.)
   All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:

     1. Redistributions of source code must retain the above copyright
        notice, this list of conditions and the following disclaimer.

     2. Redistributions in binary form must reproduce the above copyright
        notice, this list of conditions and the following disclaimer in the
        documentation and/or other materials provided with the distribution.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER
   OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

***************************************************************************/

// nsc.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "StdAfx.h"

/****************************************************************/
/*					グローバル									*/
/****************************************************************/
		OPSW*			cOptionSW = NULL;	//どっからでもアクセスする。

#ifdef _OPENMP
		omp_lock_t		lock_cout;			//COUT, CERRの排他制御用
#endif

//==============================================================
//		エラー			■■■ To Do:	廃止予定
//--------------------------------------------------------------
//	●引数
//		int			エラーコード
//	●返値
//					無し
//==============================================================
void nsc_exit(int no)
{
	throw no;
}

//==============================================================
//		エラー出力
//--------------------------------------------------------------
void nsc_ErrMsg(int no)
{
	_OMP_SET_LOCK(lock_cout)
	if(cOptionSW->fErr == true){
		cerr << "Error!: " << strerror(no) << endl;
	} else {
		cout << "Error!: " << strerror(no) << endl;
	}
	_OMP_UNSET_LOCK(lock_cout)
}

//--------------------------------------------------------------
void nsc_ErrMsg(const exception& e)
{
	_OMP_SET_LOCK(lock_cout)
	if(cOptionSW->fErr == true){
		cerr << "Error!: " << e.what() << endl;
	} else {
		cout << "Error!: " << e.what() << endl;
	}
	_OMP_UNSET_LOCK(lock_cout)
}

//--------------------------------------------------------------
void nsc_ErrMsg(const _CHAR *stErrMsg)
{
	_OMP_SET_LOCK(lock_cout)
	if(cOptionSW->fErr == true){
		_CERR << _T("Error!: ") << stErrMsg << endl;
	} else {
		_COUT << _T("Error!: ") << stErrMsg << endl;
	}
	_OMP_UNSET_LOCK(lock_cout)
}

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

	int		iResult	= EXIT_SUCCESS;


#ifdef	_MSC_VER
		locale::global(std::locale(""));
#else
//		locale::global(std::locale(""));
//		 ↑ gccでは、使えないもようなので、Ｃライブラリの方を使う。
		setlocale(LC_ALL, "");
#endif

	_OMP_INIT_LOCK(lock_cout)

	//==================================
	_COUT	<<	_T("MML Compiler for NES Sound Driver & Library (NSD.Lib)\n")
				_T("    Version 1.30\n")
				_T("        Copyright (c) 2012-2021 S.W.\n")	<<	endl;

	//==================================
	//オプションの処理
	cOptionSW	= new OPSW(argc,argv);
	if(cOptionSW->isError() == true){
		iResult	= EXIT_FAILURE;
	}
	//オプションでエラーが発生している、若しくはヘルプを表示していたらコンパイルしない。
	if((iResult	!= EXIT_FAILURE) && (cOptionSW->fHelp == false)){

		//MMLファイルのクラスオブジェクト作成
		MMLfile	*cMML = new MMLfile(cOptionSW->strMMLname);

		//MMLファイルの読み込みに失敗したらコンパイルしない。
		if(cMML->isError() == true){
			iResult	= EXIT_FAILURE;
		} else {

			size_t		i;
			MusicFile* cSND = NULL;

			//==================================
			//MML構文解析しながら、クラスオブジェクトの作成
			_COUT << _T("------------------------------------------------------------") << endl;
			_COUT << _T("*Object creating process") << endl;

			//曲データオブジェクトの作成
			cSND = new MusicFile(cMML, cOptionSW->strCodeName);

			_COUT << endl;



			//==================================
			//Optimize & Tick Count
			_COUT << _T("------------------------------------------------------------") << endl;
			_COUT << _T("*Optimize & Tick counting process") << endl;

			cSND->TickCount();

			_COUT << endl;



			//==================================
			//アドレスの解決
			if((cSND->isError() == true) || (cMML->isError() == true)){
				//エラーが発生していたら保存しない。
				iResult = EXIT_FAILURE;

			} else {
				_COUT << _T("------------------------------------------------------------") << endl;
				_COUT << _T("*Address settlement process") << endl;

				//アドレスの計算 ＆ サイズの出力
				i = cSND->SetOffset(0);
				cout << "  Music Size = " << setfill(' ')  << setw(5) << i << " [Byte]" << endl;

				i = cSND->SetDPCMOffset(i);
				cout << "  DPCM Size  = " << setfill(' ')  << setw(5) << i << " [Byte]" << endl;

				//アドレスを引数にもつオペコードのアドレス解決
				cSND->Fix_Address();

				_COUT << endl;
			}



			//==================================
			//保存
			//NSF
			if((cSND->isError() == true) || (cMML->isError() == true)){
				//エラーが発生していたら保存しない。
				iResult = EXIT_FAILURE;

			} else {
				if((cOptionSW->saveNSF == true) || ((cOptionSW->saveNSF == false)&&(cOptionSW->saveNSFe == false)&&(cOptionSW->saveASM == false))){
					cSND->saveNSF(cOptionSW->strNSFname);
				}

				//NSFe
				if(cOptionSW->saveNSFe == true){
					cSND->saveNSFe(cOptionSW->strNSFename);
				}

				//Assembly
				if(cOptionSW->saveASM == true){
					cSND->saveASM(cOptionSW->strASMname);
				}

				_COUT << endl;
			}

			//==================================
			//クラスの削除
			cout << "delete cSND" << endl; 		//Debug用
			if (cSND)
				delete	cSND;
		}
		cout << "delete cMML" << endl; 			//Debug用
		if (cMML)
			delete	cMML;
	}
	cout << "delete cOptionSW" << endl; 		//Debug用
	if (cOptionSW)
		delete	cOptionSW;

	_OMP_DESTROY_LOCK(lock_cout)

	return(iResult);
}
