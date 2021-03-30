/*******************************************************************************

			NES Sound Driver & Library	(NSD.lib)	MML Compiler

	Copyright (c) 2012 A.Watanabe (S.W.), All rights reserved.
	 For conditions of distribution and use, see copyright notice in "nsc.cpp".

*******************************************************************************/

#include "StdAfx.h"
#include "DPCMinfo.h"

//==============================================================
//		コンストラクタ
//--------------------------------------------------------------
//	●引数
//		MMLfile*			MML			MMLファイルのオブジェクト
//					bool	_bank		BANK対応のMMLか？
//		const		_CHAR	_strName[]	このオブジェクトの名前
//	●返値
//					無し
//==============================================================
DPCMinfo::DPCMinfo(MMLfile* MML, bool _bank, const _CHAR _strName[]):
	MusicItem(_strName),
	f_error(false)
{
	//----------------------
	//Local変数

	bank = _bank;

//	定数定義
enum	Command_ID_mml {
	DPCM_C,
	DPCM_Cis,
	DPCM_D,
	DPCM_Dis,
	DPCM_E,
	DPCM_F,
	DPCM_Fis,
	DPCM_G,
	DPCM_Gis,
	DPCM_A,
	DPCM_Ais,
	DPCM_B,
	DPCM_Note
};

//	これらは、MML構文で使えるコマンド。
const	static	Command_Info	Command[] = {
		{	"c#",	DPCM_Cis	},
		{	"d#",	DPCM_Dis	},
		{	"f#",	DPCM_Fis	},
		{	"g#",	DPCM_Gis	},
		{	"a#",	DPCM_Ais	},

		{	"c+",	DPCM_Cis	},
		{	"d+",	DPCM_Dis	},
		{	"f+",	DPCM_Fis	},
		{	"g+",	DPCM_Gis	},
		{	"a+",	DPCM_Ais	},

		{	"d-",	DPCM_Cis	},
		{	"e-",	DPCM_Dis	},
		{	"g-",	DPCM_Fis	},
		{	"a-",	DPCM_Gis	},
		{	"b-",	DPCM_Ais	},

		{	"c",	DPCM_C		},
		{	"d",	DPCM_D		},
		{	"e",	DPCM_E		},
		{	"f",	DPCM_F		},
		{	"g",	DPCM_G		},
		{	"a",	DPCM_A		},
		{	"b",	DPCM_B		},

		{	"ど#",		DPCM_Cis	},
		{	"れ#",		DPCM_Dis	},
		{	"ふぁ#",	DPCM_Fis	},
		{	"ふ#",		DPCM_Fis	},
		{	"そ#",		DPCM_Gis	},
		{	"ら#",		DPCM_Ais	},

		{	"れ-",	DPCM_Cis	},
		{	"み-",	DPCM_Dis	},
		{	"そ-",	DPCM_Fis	},
		{	"ら-",	DPCM_Gis	},
		{	"し-",	DPCM_Ais	},

		{	"ど",	DPCM_C		},
		{	"れ",	DPCM_D		},
		{	"み",	DPCM_E		},
		{	"ふぁ",	DPCM_F		},
		{	"ふ",	DPCM_F		},
		{	"そ",	DPCM_G		},
		{	"ら",	DPCM_A		},
		{	"し",	DPCM_B		},

		{	"ド#",		DPCM_Cis	},
		{	"レ#",		DPCM_Dis	},
		{	"ファ#",	DPCM_Fis	},
		{	"フ#",		DPCM_Fis	},
		{	"ソ#",		DPCM_Gis	},
		{	"ラ#",		DPCM_Ais	},

		{	"レ-",	DPCM_Cis	},
		{	"ミ-",	DPCM_Dis	},
		{	"ソ-",	DPCM_Fis	},
		{	"ラ-",	DPCM_Gis	},
		{	"シ-",	DPCM_Ais	},

		{	"ド",	DPCM_C		},
		{	"レ",	DPCM_D		},
		{	"ミ",	DPCM_E		},
		{	"ファ",	DPCM_F		},
		{	"フ",	DPCM_F		},
		{	"ソ",	DPCM_G		},
		{	"ラ",	DPCM_A		},
		{	"シ",	DPCM_B		},

		{	"n",	DPCM_Note	}

};

				char	cData;

	//------------------------------
	//クラスの初期設定
	max_number	= 0;
	m_id		= 0;

	//------------------------------
	//コンパイル

	// { の検索
	MML->Chk_LeftCurlyBrace();

	// } が来るまで、記述ブロック内をコンパイルする。
	while(MML->GetChar_With_Chk_RightCurlyBrace(&cData)){

		//１つ戻る
		MML->Back();

		//各コマンド毎の処理
		switch(MML->GetCommandID(Command, sizeof(Command)/sizeof(Command_Info))){

			case(DPCM_C):
				setKey(MML, 0);
				break;

			case(DPCM_Cis):
				setKey(MML, 1);
				break;

			case(DPCM_D):
				setKey(MML, 2);
				break;

			case(DPCM_Dis):
				setKey(MML, 3);
				break;

			case(DPCM_E):
				setKey(MML, 4);
				break;

			case(DPCM_F):
				setKey(MML, 5);
				break;

			case(DPCM_Fis):
				setKey(MML, 6);
				break;

			case(DPCM_G):
				setKey(MML, 7);
				break;

			case(DPCM_Gis):
				setKey(MML, 8);
				break;

			case(DPCM_A):
				setKey(MML, 9);
				break;

			case(DPCM_Ais):
				setKey(MML, 10);
				break;

			case(DPCM_B):
				setKey(MML, 11);
				break;

			case(DPCM_Note):
				setNote(MML, MML->GetInt_With_Chk_Range(_T("ノート番号"), 0,255));
				break;

			//unknown command
			default:
				MML->ErrUnknownCmd();
				break;
		}
	}

	//ここで確保しておく。
	if(bank == true){
		iSize = ((size_t)max_number + (size_t)1) * 6;
	} else {
		iSize = ((size_t)max_number + (size_t)1) * 4;
	}
	code.resize(iSize);
}

//==============================================================
//		デストラクタ
//--------------------------------------------------------------
//	●引数
//				無し
//	●返値
//				無し
//==============================================================
DPCMinfo::~DPCMinfo(void)
{
	//⊿PCMは、ここで破棄する。

	//----------------------
	//Delete Class
	for(map<string, DPCM*>::iterator it = ptcDPCM.begin(), e = ptcDPCM.end(); it != e; it++){
		delete it->second;
	}
	ptcDPCM.clear();
}
//==============================================================
//		設定
//--------------------------------------------------------------
//	●引数
//		MMLfile*	MML		MMLファイルのオブジェクト
//			int		key		キー番号（0:C / 1:Cis / ...）
//	●返値
//				無し
//==============================================================
void	DPCMinfo::setKey(MMLfile* MML, int key)
{
	int octave = MML->GetInt_With_Chk_Range(_T("オクターブ"), 1,12);
	setNote(MML, ((octave - 1) * 12) + key);
}

//==============================================================
//		設定
//--------------------------------------------------------------
//	●引数
//		MMLfile*		MML		MMLファイルのオブジェクト
//	●返値
//				無し
//==============================================================
void	DPCMinfo::setNote(MMLfile* MML, int note)
{
	unsigned	char	cData;
				int		play_frequency;
				int		mode			= 0;
				int		start_volume	= 0x40;
				int		next;
				int		start_offset;
				int		size_offset;
	DPCM*		_DPCM;

	if(max_number < note){
		max_number = (unsigned char)note;
	}

	//ファイル名
	MML->Chk_Comma();
	infoDPCM[note].file.clear();
	MML->GetString(&infoDPCM[note].file, false);
	if(ptcDPCM.count(infoDPCM[note].file) == 0){
		//新しいファイルだったら、DPCMオブジェクトを生成する。
		_DPCM = new DPCM(MML, infoDPCM[note].file.c_str(), m_id);
		if(_DPCM->isError() == true){
			f_error = true;	//読み込みに失敗した場合
		}
		ptcDPCM[infoDPCM[note].file] = _DPCM;
		m_id++;
	} else {
		_DPCM = ptcDPCM[infoDPCM[note].file];
	}

	//再生周波数
	MML->Chk_Comma();
	play_frequency = MML->GetInt_With_Chk_Range(_T("⊿PCMの周波数"),0,15);

	//モード
	MML->Chk_Comma();
	mode = MML->GetInt_With_Chk_Range(_T("⊿PCMのモード"),0,2);
	if((mode==2) && (bank==false)){
		MML->Err(_T("⊿PCMのモード2(IRQ)は、#Bankコマンドの指定が必要です。"));
	}
	infoDPCM[note].ctrl = (unsigned char)(mode<<6) + (unsigned char)play_frequency;

	//初期値
	cData = MML->GetChar();
	if(cData == ','){
		start_volume = MML->GetInt_With_Chk_Range(_T("⊿PCMの初期値"),-1,127);
		infoDPCM[note].DA = (unsigned char)start_volume;
	} else {
		MML->Back();
		infoDPCM[note].DA = 0;
	}

	//次のノート
	if(mode == 2){
		MML->Chk_Comma();
		next = MML->GetInt_With_Chk_Range(_T("⊿PCMの次のノート番号"),-1,255);
		infoDPCM[note].next = (unsigned char)next;
	} else {
		infoDPCM[note].next = 0;
	}

	//offset
	cData = MML->GetChar();
	if(cData == ','){
		start_offset = MML->GetInt_With_Chk_Range(_T("⊿PCMの発音開始オフセット"),0,255);
		infoDPCM[note].offset = (unsigned char)start_offset;
	} else {
		MML->Back();
		infoDPCM[note].offset = 0;
	}

	//size
	cData = MML->GetChar();
	if(cData == ','){
		size_offset = MML->GetInt_With_Chk_Range(_T("⊿PCMの発音サイズ"),0,255);
		infoDPCM[note].size = (unsigned char)size_offset;
	} else {
		MML->Back();
		infoDPCM[note].size = 0;
	}
}

//==============================================================
//		ΔPCMinfo構造体の生成（配置アドレスの解決）
//--------------------------------------------------------------
//	●引数
//					size_t	_offset		ΔPCM配置アドレス
//		unsigned	char	_MusBank	シーケンスのバンク数
//	●返値
//					size_t				ΔPCM配置終了アドレス
//	●処理
//		コードを取得しながら、各ΔPCMのバイナリを得る。
//		（得たコードは、NSF出力の時しか使わないが。。。）
//==============================================================
size_t	DPCMinfo::setDPCMoffset(size_t _offset, unsigned char _MusBank)
{

	DPCM*	_DPCM;
	size_t	_size;

	//⊿PCMの配置アドレスを解決。しながらNSF出力用の⊿PCM実体を作成。
	if(m_id > 0){
		for(map<string, DPCM*>::iterator it = ptcDPCM.begin(), e = ptcDPCM.end(); it != e; it++){
			_DPCM = it->second;
			_DPCM->SetOffset(_offset);
			_size	= (_DPCM->getSize() & 0xFFC0) + 0x0040;
			_offset	+= _size;
		}
	}

	//ΔPCMinfo構造体の生成
	if(bank == false){
		for(size_t i=0; i <= max_number; i++){
			if(infoDPCM[i].file.empty()){
				code[i*4 + 0] = 0;
				code[i*4 + 1] = 0;
				code[i*4 + 2] = 0;
				code[i*4 + 3] = 0;
			} else {
				_DPCM = ptcDPCM[infoDPCM[i].file];
				code[i*4 + 0] = infoDPCM[i].ctrl;
				code[i*4 + 1] = infoDPCM[i].DA;
				code[i*4 + 2] = (unsigned char)((_DPCM->getOffset() - 0xC000) >> 6) + infoDPCM[i].offset;
				if(infoDPCM[i].size == 0){
					if(_DPCM->getDPCMsize() >= (infoDPCM[i].offset*4)){
                        code[i*4 + 3] = _DPCM->getDPCMsize() - (infoDPCM[i].offset*4);
					} else {
						code[i*4 + 3] = 0;
					}
				} else {
					code[i*4 + 3] = infoDPCM[i].size;
				}
			}
		}
	} else {
		for(size_t i=0; i <= max_number; i++){
			if(infoDPCM[i].file.empty()){
				code[i*6 + 0] = 0;
				code[i*6 + 1] = 0;
				code[i*6 + 2] = 0;
				code[i*6 + 3] = 0;
				code[i*6 + 4] = 0;
				code[i*6 + 5] = 0;
			} else {
				_DPCM = ptcDPCM[infoDPCM[i].file];
				code[i*6 + 0] = infoDPCM[i].ctrl;
				code[i*6 + 1] = infoDPCM[i].DA;
				code[i*6 + 2] = (unsigned char)((_DPCM->getOffset() & 0x0FFF) >> 6) + infoDPCM[i].offset;
				if(infoDPCM[i].size == 0){
					if(_DPCM->getDPCMsize() >= (infoDPCM[i].offset*4)){
                        code[i*6 + 3] = _DPCM->getDPCMsize() - (infoDPCM[i].offset*4);
					} else {
						code[i*6 + 3] = 0;
					}
				} else {
					code[i*6 + 3] = infoDPCM[i].size;
				}
				code[i*6 + 4] = (unsigned char)((_DPCM->getOffset() - 0xC000) / 0x1000) + _MusBank;
				code[i*6 + 5] = infoDPCM[i].next;
			}
		}
	}

	return(_offset);
}

//==============================================================
//		コードの取得
//--------------------------------------------------------------
//	●引数
//		string*		_str		コードを入れるstringのポインタ
//	●返値
//		無し
//	●処理
//		コードを取得しながら、各ΔPCMのバイナリを得る。
//		（得たコードは、NSF出力の時しか使わないが。。。）
//==============================================================
void	DPCMinfo::getDPCMCode(string* _str)
{
	DPCM*	_DPCM;
	size_t	_size;
	size_t	psize;

	if(m_id > 0){
		//DPCM
		for(map<string, DPCM*>::iterator it = ptcDPCM.begin(), e = ptcDPCM.end(); it != e; it++){
			_DPCM = it->second;
			_DPCM->getCode(_str);
			_size	= _DPCM->getSize();
			psize	= (_size & 0xFFC0) + 0x0040;
			_str->append(psize - _size,(char)0x00);
		}
	}
}
//==============================================================
//		コードの取得
//--------------------------------------------------------------
//	●引数
//		MusicFile*	MUS		コードを出力する曲データファイル・オブジェクト
//	●返値
//				無し
//==============================================================
void	DPCMinfo::getAsm(MusicFile* MUS)
{
	if(m_id > 0){
		//DPCMinfo
		*MUS << MUS->Header.Label << "DPCMinfo" << ":" << endl;
		MusicItem::getAsm(MUS);

		//DPCM
		if(MUS->Header.segmentPCM != "") {
			*MUS <<	"\n\n.segment	"	<<	'"'	<<	MUS->Header.segmentPCM	<<	'"' << endl;
			for(map<string, DPCM*>::iterator it = ptcDPCM.begin(), e = ptcDPCM.end(); it != e; it++){
				it->second->getAsm(MUS);
			}
			*MUS <<	"\n\n.segment	"	<<	'"'	<<	MUS->Header.segmentSEQ	<<	'"' << endl;
		}
	}
}
