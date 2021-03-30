/*******************************************************************************

			NES Sound Driver & Library	(NSD.lib)	MML Compiler

	Copyright (c) 2012 A.Watanabe (S.W.), All rights reserved.
	 For conditions of distribution and use, see copyright notice in "nsc.cpp".

*******************************************************************************/

// stdafx.h : 標準のシステム インクルード ファイルのインクルード ファイル、または
// 参照回数が多く、かつあまり変更されない、プロジェクト専用のインクルード ファイル
// を記述します。
//

#pragma once

#define _CRT_SECURE_NO_WARNINGS

//---------------------------------------------------------------
//	Microsoft Visual C++ では _Pragma() が使えない対策
#ifdef	_MSC_VER
	//Visual C++だったら
	#define _PRAGMA(x)	__pragma(x)
#else
	//C++11標準の方法
	#define _PRAGMA(x)	_Pragma(#x)
#endif


//---------------------------------------------------------------
//Unicode環境
#ifdef	_UNICODE
	#define	_CHAR		wchar_t
	#define _T(x)		L ## x
	#define _EOF		WEOF
	#define	_STRING		wstring
	#define	_SSTREAM	wstringstream
	#define	_COUT		wcout
	#define	_CERR		wcerr
#else
	#define	_CHAR		char
	#define _T(x)		x
	#define _EOF		EOF
	#define	_STRING		string
	#define	_SSTREAM	stringstream
	#define	_COUT		cout
	#define	_CERR		cerr
#endif


//---------------------------------------------------------------
//パス区切り
#ifdef	_WIN32
	// MS系は ;
	#define _PATH_SPLIT	';'
#else
	// UNIX系は :
	#define _PATH_SPLIT	':'
#endif


//---------------------------------------------------------------
//OpenMP
#ifdef _OPENMP
	#include	<omp.h>
	#define	_OMP_INIT_LOCK(x)			omp_init_lock(&x);
	#define	_OMP_SET_LOCK(x)			omp_set_lock(&x);
	#define	_OMP_UNSET_LOCK(x)			omp_unset_lock(&x);
	#define	_OMP_DESTROY_LOCK(x)		omp_destroy_lock(&x);
	#define	_OMP(x)						_PRAGMA(omp x)
#else
	#define	_OMP_INIT_LOCK(x)
	#define	_OMP_SET_LOCK(x)
	#define	_OMP_UNSET_LOCK(x)
	#define	_OMP_DESTROY_LOCK(x)
	#define	_OMP(x)	
#endif

#define	_OMP_FOR					_OMP(for)
#define	_OMP_FOR_NOWAIT				_OMP(for nowait)
#define	_OMP_SECTIONS				_OMP(sections)
#define	_OMP_SECTION				_OMP(section)
#define	_OMP_SINGLE					_OMP(single)
#define	_OMP_PARALLEL				_OMP(parallel)
#define	_OMP_PARALLEL_FOR			_OMP(parallel for)
#define	_OMP_PARALLEL_FOR_NOWAIT	_OMP(parallel for nowait)
#define	_OMP_PARALLEL_SECTIONS		_OMP(parallel sections)



//---------------------------------------------------------------
//標準ライブラリ
//C
#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <cwchar>
#include <cstring>
#include <clocale>	//gcc用にこっちも入れておく。

//C++
#include <algorithm>
#include <exception>
#include <stdexcept>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <list>
#include <map>
#include <string>
#include <locale>


/***************************************************************/
/*			プロジェクトの各ヘッダー							*/
/***************************************************************/


using namespace std;

//プロトタイプ宣言
void nsc_exit(int no);		//■■■■ To Do:	廃止予定
void nsc_ErrMsg(int no);
void nsc_ErrMsg(const exception& e);
void nsc_ErrMsg(const string& errMsg);
void nsc_ErrMsg(const _CHAR* errMsg);

class	MusicHeader;
class	MusicFile;
class	MMLfile;
class	Sub;

//構造体定義
typedef struct {
	const char*	str;
	int			id;
} Command_Info;

//ヘッダー
#include "SearchPass.h"			//検索パス

#include "FileInput.h"			//ファイル入力用
#include "FileOutput.h"			//ファイル出力用

#include "Option.h"				//オプション

#include "MusicItem.h"

#include "Patch.h"

#include "FDSC.h"
#include "FDSM.h"
#include "VRC7.h"
#include "N163.h"

#include "DPCM.h"
#include "DPCMinfo.h"

#include "Envelop.h"

#include "MusicEvent.h"			//各イベント
#include "mml_general.h"		//汎用
#include "mml_repeat.h"			//汎用
#include "mml_poke.h"			//メモリ書き込み
#include "mml_note.h"			//音符・休符
#include "mml_Address.h"
#include "mml_CallSub.h"

#include "NSF_Header.h"			//ヘッダー

#include "MetaItem.h"
#include "Meta_INFO.h"			//2.1	INFO	NSFe MUST
#include "Meta_DATA.h"			//2.2	DATA	NSFe MUST
#include "Meta_NEND.h"			//2.3	NEND	NSFe MUST
#include "Meta_BANK.h"			//2.4	BANK	NSFe optional / NSF MUSTNOT
#include "Meta_NSF2.h"			//2.6	NSF2	NSFe optional /  NSF MUSTNOT
#include "Meta_VRC7.h"			//2.7	VRC7
#include "Meta_plst.h"			//2.8	plst
#include "Meta_psfx.h"			//2.9	psfx
#include "Meta_time.h"			//2.10	time
#include "Meta_fade.h"			//2.11	fade
#include "Meta_tlbl.h"			//2.12	tlbl
#include "Meta_taut.h"			//2.13	taut
#include "Meta_auth.h"			//2.14	auth
#include "Meta_text.h"			//2.15	text
#include "Meta_mixe.h"			//2.16	mixe

#include "MusicHeader.h"		//ヘッダー

#include "nsd_work.h"
#include "MusicNote.h"
#include "MusicTrack.h"			//トラック情報
#include "TrackSet.h"
#include "BGM.h"
#include "SE.h"
#include "Sub.h"

#include "MMLfile.h"		//MMLファイル
#include "MusicFile.h"		//SNDファイル

// TODO: プログラムに必要な追加ヘッダーをここで参照してください。
