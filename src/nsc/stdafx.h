// stdafx.h : 標準のシステム インクルード ファイルのインクルード ファイル、または
// 参照回数が多く、かつあまり変更されない、プロジェクト専用のインクルード ファイル
// を記述します。
//

#pragma once

#include <stdlib.h>
#include <string.h>

#ifdef	_WIN32
	#include <locale>
#else
	#include <locale.h>
#endif



#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>

#include <iomanip>

//#include <tchar.h>


/****************************************************************/
/*			プロトタイプ										*/
/****************************************************************/

using namespace std;

class	MusicFile;
class	MMLfile;
class	Sub;

typedef struct {
	char*	str;
	int		id;
} Command_Info;

#include "FileInput.h"		//ファイル入力用
#include "FileOutput.h"		//ファイル出力用

#include "Option.h"			//オプション

#include "MusicItem.h"

#include "FDSC.h"
#include "FDSM.h"
#include "VRC7.h"
#include "N163.h"

#include "DPCM.h"
#include "DPCMinfo.h"

#include "Envelop.h"

#include "MusicEvent.h"			//各イベント
#include "mml_general.h"		//汎用
#include "mml_repeat.h"		//汎用
#include "mml_poke.h"			//メモリ書き込み
#include "mml_note.h"			//音符・休符
#include "mml_Address.h"

#include "MusicHeader.h"		//ヘッダー

#include "MusicTrack.h"			//トラック情報
#include "TrackSet.h"
#include "BGM.h"
#include "SE.h"
#include "Sub.h"

#include "MMLfile.h"		//MMLファイル
#include "MusicFile.h"		//SNDファイル

// TODO: プログラムに必要な追加ヘッダーをここで参照してください。

/*

	■	to do

	・#BGM, #SE	で指定した範囲で、無い番号があったらエラー
	・NSFヘッダーの文字列　32文字以下の時の処理

*/