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

//==============================================================
//		エラー
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
				int	iResult	= EXIT_SUCCESS;
	MMLfile			*cMML	= NULL;
    MusicFile		*cSND	= NULL;


#ifdef	_WIN32
	locale::global(std::locale(""));
#else
//	setlocale(LC_ALL, "ja_JP.UTF-8");
	setlocale(LC_ALL, "");
#endif


//	locale::global(std::locale(""));	//g++ だと、ランタイム エラーになる。


	try {
		//==================================
		_COUT	<<	_T("MML Compiler for NES Sound Driver & Library (NSD.Lib)\n")
					_T("    Version 1.26\n")
					_T("        Copyright (c) 2012-2015 S.W.\n")	<<	endl;



		//==================================
		//クラスの作成
		cOptionSW	= new OPSW(argc,argv);							//オプション処理
		if(cOptionSW->cDebug & 0x01){
			_COUT << _T("\n============ [ 1st phase : Object Creating ] ============") << endl;
		}
		_COUT << _T("----------------------------------------") << endl;
		_COUT << _T("*Object creating process") << endl;

		cMML = new MMLfile(cOptionSW->strMMLname.c_str());
		cSND = new MusicFile(cMML, cOptionSW->strCodeName);



		//==================================
		//アドレスの解決
		if(cOptionSW->cDebug & 0x02){
			_COUT << _T("\n============ [ 2nd phase : Address Setting ] ============") << endl;
		}
		_COUT << _T("----------------------------------------") << endl;
		_COUT << _T("*Address settlement process") << endl;


		//ＭＭＬから呼ばれるオブジェクトの検索 ＆ 呼ばれないオブジェクトの削除
		cSND->Optimize();

		//アドレスの計算
		i = cSND->SetOffset(0);
		cout << "  Music Size = " << setfill(' ')  << setw(5) << i << " [Byte]" << endl;
		i = cSND->SetDPCMOffset(i);
		cout << "  DPCM Size  = " << setfill(' ')  << setw(5) << i << " [Byte]" << endl;

		//アドレスを引数にもつオペコードのアドレス解決
		cSND->Fix_Address();

		//==================================
		//保存
		if(cOptionSW->cDebug & 0x04){
			_COUT << _T("\n============ [ 3rd phase : Music File Outputing ] ============") << endl;
		}

			if((cOptionSW->saveNSF == true) || ((cOptionSW->saveNSF == false)&&(cOptionSW->saveASM == false))){
			cSND->saveNSF(cOptionSW->strNSFname.c_str());
		}

		if(cOptionSW->saveASM == true){
			cSND->saveASM(cOptionSW->strASMname.c_str());
		}



		//==================================
		//Tick Count
		if(cOptionSW->flag_TickCount == true){

			if(cOptionSW->cDebug & 0x04){
				_COUT << _T("\n============ [ 4th phase : Tick Counting ] ============") << endl;
			}
			_COUT << _T("----------------------------------------") << endl;
			_COUT << _T("*Tick counting process") << endl;

			cSND->TickCount();

		} else {
			_COUT	<<	_T("tickのカウントは無効化されました。")	<<	endl;
		}

	} catch (int no) {
		if (no != EXIT_SUCCESS){
            _COUT	<<	_T("Error!:") << no << endl;
			iResult	= EXIT_FAILURE;
		}
	}

	//==================================
	//クラスの削除
	if (cSND)
		delete	cSND;
	if (cMML)
		delete	cMML;
	if (cOptionSW)
		delete	cOptionSW;

	return(iResult);
}
