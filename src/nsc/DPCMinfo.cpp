#include "StdAfx.h"
#include "DPCMinfo.h"

//==============================================================
//		コンストラクタ
//--------------------------------------------------------------
//	●引数
//		MMLfile*	MML		MMLファイルのオブジェクト
//		bool		_sub	このオブジェクトは、サブルーチン？
//	●返値
//					無し
//==============================================================
DPCMinfo::DPCMinfo(MMLfile* MML, const wchar_t _strName[]):
	MusicItem(_strName)
{
	//----------------------
	//Local変数


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

		{	"n",	DPCM_Note	}
};

	unsigned	char	cData;

	//------------------------------
	//クラスの初期設定
	max_number	= 0;
	m_id		= 0;

	//------------------------------
	//コンパイル

	// { の検索
	while(MML->cRead() != '{'){
		if(MML->eof()){
			MML->Err(L"ブロックの開始を示す{が見つかりません。");
		}
	}

	// } が来るまで、記述ブロック内をコンパイルする。
	while((cData = MML->GetChar()) != '}'){
		
		// } が来る前に、[EOF]が来たらエラー
		if( MML->eof() ){
			MML->Err(L"ブロックの終端を示す`}'がありません。");
		}

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
				setNote(MML, MML->GetInt());
				break;

			//unknown command
			default:
				MML->Err(L"unknown command");
				break;
		}
	}

	//ここで確保しておく。
	iSize = (max_number+1)*4;
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
	//Local変数
	map<string, DPCM*>::iterator	itItem;

	//----------------------
	//Delete Class
	if(!ptcDPCM.empty()){
		itItem = ptcDPCM.begin();
		while(itItem != ptcDPCM.end()){
			delete itItem->second;
			itItem++;
		}
		ptcDPCM.clear();
	}
}
//==============================================================
//		設定
//--------------------------------------------------------------
//	●引数
//		MMLfile*	MML		MMLファイルのオブジェクト
//	●返値
//				無し
//==============================================================
void	DPCMinfo::setKey(MMLfile* MML, int key)
{
	setNote(MML, ((MML->GetInt()-1) * 12) + key);
}

void	DPCMinfo::setNote(MMLfile* MML, int note)
{
	unsigned	char	cData;
				int		play_frequency;
				int		mode			= 0;
				int		start_volume	= 0x40;
	DPCM*		_DPCM;
	FileInput	_DPCM_file;

	if((note<0) || (note>127)){
		MML->Err(L"音階の範囲を超えています。");
	}

	if(max_number < note){
		max_number = note;
	}

	//ファイル名
	cData = MML->GetChar();
	if(cData != ','){
		MML->Err(L"⊿PCM定義のパラメータが足りません。");
	}

	infoDPCM[note].file = MML->GetString();
	if(ptcDPCM.count(infoDPCM[note].file) == 0){
		//新しいファイルだったら、DPCMオブジェクトを生成する。
		_DPCM_file.fileopen(infoDPCM[note].file.c_str());
		if(_DPCM_file.GetSize() > 4081){
			MML->Err(L"⊿PCMは、4081Byte以下にしてください。");
		}
		_DPCM = new DPCM(&_DPCM_file, m_id);
		m_id++;
		_DPCM_file.close();
		ptcDPCM[infoDPCM[note].file] = _DPCM;
	} else {
		_DPCM = ptcDPCM[infoDPCM[note].file];
	}

	//再生周波数
	cData = MML->GetChar();
	if(cData != ','){
		MML->Err(L"⊿PCM定義のパラメータが足りません。");
	}
	play_frequency = MML->GetInt();
	if((play_frequency<0) || (play_frequency>15)){
		MML->Err(L"⊿PCMの周波数は0～15の範囲で指定して下さい。");
	}

	//モード
	cData = MML->GetChar();
	if(cData != ','){
		MML->Err(L"⊿PCM定義のパラメータが足りません。");
	}

	mode = MML->GetInt();
	if((mode<0) || (mode>1)){
		MML->Err(L"⊿PCMの周波数は0～1の範囲で指定して下さい。");
	}
	infoDPCM[note].ctrl = (mode<<6) + play_frequency;

	//初期値
	cData = MML->GetChar();
	if(cData == ','){
		start_volume = MML->GetInt();	
		if((start_volume<-1) || (start_volume>127)){
			MML->Err(L"⊿PCMの周波数は-1～127の範囲で指定して下さい。");
		}
		infoDPCM[note].DA = start_volume;
	} else {
		MML->Back();
		infoDPCM[note].DA = 0;
	}
}
//==============================================================
//		コードの取得
//--------------------------------------------------------------
//	●引数
//		unsigned	int _offset		オフセットアドレス計算用
//		string*			_str		コードを入れるオブジェクト
//	●返値
//				無し
//	●処理
//		コードを取得しながら、各ΔPCMのバイナリを得る。
//		（得たコードは、NSF出力の時しか使わないが。。。）
//==============================================================
unsigned	int	DPCMinfo::setDPCMoffset(unsigned	int _offset)
{
	map<string, DPCM*>::iterator	itDPCM;
	DPCM*	_DPCM;
	size_t	_size;

	unsigned	int	i=0;

	if(m_id > 0){
		//DPCM
		if(!ptcDPCM.empty()){
			itDPCM = ptcDPCM.begin();
			while(itDPCM != ptcDPCM.end()){
				_DPCM = itDPCM->second;
				_DPCM->SetOffset(_offset);
				_size	= (_DPCM->getSize() & 0xFFC0) + 0x0040;
				_offset	+= (unsigned int)_size;
				itDPCM++;
			}
		}
	}

	//ΔPCMinfo構造体の生成
	while(i <= max_number){
		if(infoDPCM[i].file.empty()){
			code[i*4 + 0] = 0;
			code[i*4 + 1] = 0;
			code[i*4 + 2] = 0;
			code[i*4 + 3] = 0;
		} else {
			_DPCM = ptcDPCM[infoDPCM[i].file];
			code[i*4 + 0] = infoDPCM[i].ctrl;
			code[i*4 + 1] = infoDPCM[i].DA;
			code[i*4 + 2] = (_DPCM->getOffset() - 0xC000) >> 6;
			code[i*4 + 3] = _DPCM->getDPCMsize();
		}
		i++;
	}

	return(_offset);
}

void	DPCMinfo::getDPCMCode(string* _str)
{
	map<string, DPCM*>::iterator	itDPCM;
	DPCM*	_DPCM;
	size_t	_size;
	size_t	psize;

	if(m_id > 0){
		//DPCM
		if(!ptcDPCM.empty()){
			itDPCM = ptcDPCM.begin();
			while(itDPCM != ptcDPCM.end()){
				_DPCM = itDPCM->second;
				_DPCM->getCode(_str);
				_size	= _DPCM->getSize();
				psize	= (_size & 0xFFC0) + 0x0040;
				_str->append(psize - _size,(char)0x00);
				itDPCM++;
			}
		}
	}
}
//==============================================================
//		コードの取得
//--------------------------------------------------------------
//	●引数
//				無し
//	●返値
//				無し
//==============================================================
void	DPCMinfo::getAsm(MusicFile* MUS)
{
	map<string, DPCM*>::iterator	itDPCM;

	if(m_id > 0){
		//DPCMinfo
		*MUS << MUS->Header.Label.c_str() << "DPCMinfo" << ":" << endl;
		MusicItem::getAsm(MUS);

		//DPCM
		if(!ptcDPCM.empty()){
			*MUS <<	"\n\n.segment	"	<<	'"'	<<	MUS->Header.segmentPCM	<<	'"' << endl;
			itDPCM = ptcDPCM.begin();
			while(itDPCM != ptcDPCM.end()){
				itDPCM->second->getAsm(MUS);
				itDPCM++;
			}
			*MUS <<	"\n\n.segment	"	<<	'"'	<<	MUS->Header.segmentSEQ	<<	'"' << endl;
		}
	}
}
