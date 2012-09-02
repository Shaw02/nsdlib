#include "StdAfx.h"
#include "MusicFile.h"

//==============================================================
//		コンストラクタ
//--------------------------------------------------------------
//	●引数
//		MMLfile* MML	ＭＭＬファイルのオブジェクト
//	●返値
//				無し
//==============================================================
MusicFile::MusicFile(MMLfile* MML, const char _strName[]):
	MusicItem(_strName),
	Header(MML)
{
	//----------------------
	//Local変数


//	定数定義
enum	Command_ID_MusicFile {
	//NSF Header
	id_include,
	id_Title,
	id_Composer,
	id_Copyright,
	id_bgm_num,
	id_se_num,

	id_Segment,
	id_Code,

	id_OctaveReverse,

	//Block
	id_Envelop,
	id_Macro,
	id_Sub,
	id_BGM,
	id_SE
};

//	これらは、MML構文で使えるコマンド。
const	static	Command_Info	Command[] = {
		{	"#Include",			id_include		},
		{	"#include",			id_include		},
		{	"#Title",			id_Title		},
		{	"#title",			id_Title		},
		{	"#Composer",		id_Composer		},
		{	"#composer",		id_Composer		},
		{	"#Copyright",		id_Copyright	},
		{	"#copyright",		id_Copyright	},
		{	"#Segment",			id_Segment		},
		{	"#segment",			id_Segment		},
		{	"#Code",			id_Code			},
		{	"#code",			id_Code			},
		{	"#OctaveReverse",	id_OctaveReverse},
		{	"#octaveReverse",	id_OctaveReverse},
		{	"#BGM",				id_bgm_num		},
		{	"#bgm",				id_bgm_num		},
		{	"#SE",				id_se_num		},
		{	"#se",				id_se_num		},
		{	"Envelop",			id_Envelop		},
		{	"envelop",			id_Envelop		},
		{	"$",				id_Macro		},
		{	"Sub",				id_Sub			},
		{	"sub",				id_Sub			},
		{	"BGM",				id_BGM			},
		{	"bgm",				id_BGM			},
		{	"SE",				id_SE			},
		{	"se",				id_SE			},
	};

	unsigned	int		i;
	unsigned	char	cData;
				BGM*	_bgm;
				SE*		_se;
				Sub*	_sub;
	string		msg;

//	map<	int,	Envelop*	>::iterator	itEnvelop;
	map<	int,	BGM*		>::iterator	itBGM;
	map<	int,	SE*			>::iterator	itSE;
	map<	int,	Sub*		>::iterator	itSub;

	iSize = 0;

	do{
		
		//１文字読み込み（コメントチェック、includeファイルの終端チェックもあり）
		cData = MML->GetChar();

		//[EOF]チェック
		if( MML->eom() ){
			break;
		}

		//１つ戻る
		MML->StreamPointerAdd(-1);

		//コマンド文字列のチェック
		switch(MML->GetCommandID(Command, sizeof(Command)/sizeof(Command_Info))){
			case(id_include):
				MML->include();
				break;
			case(id_Title):
				Header.Set_Title(MML);
				break;
			case(id_Composer):
				Header.Set_Composer(MML);
				break;
			case(id_Copyright):
				Header.Set_Copyright(MML);
				break;
			case(id_Segment):
				Header.Set_Segment(MML);
				break;
			case(id_Code):
				Header.Set_RomCode(MML);
				break;
			case(id_OctaveReverse):
				MML->octave_reverse = true;		//これは、MMLファイルの属性。
				break;
			case(id_bgm_num):
				Header.Set_Number_BGM(MML);
				break;
			case(id_se_num):
				Header.Set_Number_SE(MML);
				break;
//			case(id_Envelop):
//				break;
			case(id_Sub):
				i = MML->GetNum();
				//重複チェック
				if(ptcSub.count(i) != 0){
					MML->Err("Sub()ブロックで同じ番号が指定されました。");
				}
				//範囲チェック
				_sub = new Sub(MML);
				ptcItem.push_back(_sub);
				ptcSub[i] = _sub;
				iSize += _sub->getSize();	//BGMのサイズを更新
				break;
			case(id_BGM):
				i = MML->GetNum();
				//重複チェック
				if(ptcBGM.count(i) != 0){
					MML->Err("BGM()ブロックで同じ番号が指定されました。");
				}
				//範囲チェック
				if((Header.iBGM <= i) || (i<0)){
					MML->Err("BGM()ブロックで指定できる範囲を超えています。\n#BGMの数値を確認してください。");
				}
				_bgm = new BGM(MML);
				ptcItem.push_back(_bgm);
				ptcBGM[i] = _bgm;
				iSize += _bgm->getSize();	//BGMのサイズを更新
				break;
			case(id_SE):
				i = MML->GetNum();
				//重複チェック
				if(ptcSE.count(i) != 0){
					MML->Err("SE()ブロックで同じ番号が指定されました。");
				}
				//範囲チェック
				if((Header.iSE <= i) || (i<0)){
					MML->Err("SE()ブロックで指定できる範囲を超えています。\n#SEの数値を確認してください。");
				}
				_se = new SE(MML);
				ptcItem.push_back(_se);
				ptcSE[i] = _se;
				iSize += _se->getSize();	//BGMのサイズを更新
				break;
			default:
				MML->Err("unknown command");
				break;
		}
		
	} while( !MML->eom() );

	//Check
	if( Header.iBGM + Header.iSE > 255){
		Err("BGMとSEの数が、合計で255を越えました。");
	}

	i = 0;
	while(i < Header.iBGM){
		if(ptcBGM.count(i) == 0){
			Err("BGMデータが足りません。");
		};
		i++;
	}

	i = 0;
	while(i < Header.iSE){
		if(ptcSE.count(i) == 0){
			Err("SE データが足りません。");
		};
		i++;
	}

}

//==============================================================
//		デストラクタ
//--------------------------------------------------------------
//	●引数
//				無し
//	●返値
//				無し
//==============================================================
MusicFile::~MusicFile(void)
{

}

//==============================================================
//		アドレス情報を決定する。
//--------------------------------------------------------------
//	●引数
//				無し
//	●返値
//				無し
//==============================================================
void	MusicFile::Fix_Address(void)
{
	unsigned	int			iBGM	= 0;
	unsigned	int			iSE		= 0;

	while(iBGM < Header.iBGM){
		ptcBGM[iBGM]->Fix_Address(&ptcSub);
		iBGM++;
	}
	while(iSE < Header.iSE){
		ptcSE[iSE]->Fix_Address(&ptcSub);
		iSE++;
	}

}

//==============================================================
//		
//--------------------------------------------------------------
//	●引数
//				無し
//	●返値
//				無し
//==============================================================
void	MusicFile::init_romimg(void)
{
	memset(romimg, 0, sizeof(romimg));

}

//==============================================================
//		曲バイナリイメージの作成
//--------------------------------------------------------------
//	●引数
//				無し
//	●返値
//				無し
//==============================================================
void	MusicFile::make_binary(void)
{
	string	_str;
	unsigned	int			i		= 2;
	unsigned	int			iBGM	= 0;
	unsigned	int			iSE		= 0;
	unsigned	__int16*	pt;

	unsigned	int		_size	= 4 + (Header.iBGM + Header.iSE)*2;

	_str.clear();
	_str.resize(_size);

	pt = (unsigned __int16*)_str.c_str();

	_str[0] = Header.iBGM;
	_str[1] = Header.iSE;
	pt[1]	=	0;			//ΔPCM info のアドレス

	while(iBGM < Header.iBGM){
		pt[i] = 0x8000 + _size + ptcBGM[iBGM]->getOffset();
		i++;
		iBGM++;
	}
	while(iSE < Header.iSE){
		pt[i] = 0x8000 + _size + ptcSE[iSE]->getOffset();
		i++;
		iSE++;
	}

	getCode(&_str);
	code = _str;
}
		
//==============================================================
//		ＮＳＦの作成
//--------------------------------------------------------------
//	●引数
//		const char*	strFileName	コード
//	●返値
//				無し
//==============================================================
void	MusicFile::make_nsf(const char*	strFileName)
{
	FileInput*	_romcode	= new FileInput();
	_romcode->fileopen(strFileName);

	unsigned	__int16*	_nsd_init	=	((unsigned	__int16*)romimg) + 0x7FF0/sizeof(unsigned	__int16);
	unsigned	__int16*	_nsd_main	=	((unsigned	__int16*)romimg) + 0x7FF2/sizeof(unsigned	__int16);

	//ROMイメージにバイナリーを転送
	init_romimg();
	memcpy((char *)romimg, code.c_str(), code.size());

	_romcode->read((char*)(romimg + 0x6000), 0x2000);
	_romcode->close();	

	//NSFヘッダーの作成
	nsf.Name[0]			= 'N';
	nsf.Name[1]			= 'E';
	nsf.Name[2]			= 'S';
	nsf.Name[3]			= 'M';
	nsf.Name[4]			= 0x1A;
	nsf.Version			= 1;
	nsf.MusicNumber		= Header.iBGM + Header.iSE;
	nsf.StartMusicNumber= 1;
	nsf.LoadAddress		= 0x8000;
	nsf.InitAddress		= *_nsd_init;
	nsf.MainAddress		= *_nsd_main;
	nsf.Frequency_NTSC	= 0x411A;
	nsf.Frequency_PAL	= 0x4E20;
	nsf.Video			= 0;
	nsf.External		= 0;
	nsf.Bank[0]			= 0;
	nsf.Bank[1]			= 0;
	nsf.Bank[2]			= 0;
	nsf.Bank[3]			= 0;
	nsf.Bank[4]			= 0;
	nsf.Bank[5]			= 0;
	nsf.Bank[6]			= 0;
	nsf.Bank[7]			= 0;
	nsf.Null1			= 0;
	nsf.Null2			= 0;
	nsf.Null3			= 0;
	nsf.Null4			= 0;

	memcpy(&nsf.Title, Header.title.c_str(), 32);
	memcpy(&nsf.Composer, Header.composer.c_str(), 32);
	memcpy(&nsf.Copyright, Header.copyright.c_str(), 32);

	delete	_romcode;
}

//==============================================================
//		バイナリファイルへの保存
//--------------------------------------------------------------
//	●引数
//		const char*	strFileName		ファイル名
//	●返値
//				無し
//==============================================================
void	MusicFile::saveBIN(const char*	strFileName)
{
	//----------------------
	//File open
	fileopen(strFileName);

	write(code.c_str(), code.size());
	//----------------------
	//Close file
	close();
}

//==============================================================
//		ＮＳＦ形式への保存
//--------------------------------------------------------------
//	●引数
//		const char*	strFileName		ファイル名
//	●返値
//				無し
//==============================================================
void	MusicFile::saveNSF(const char*	strFileName)
{
	//----------------------
	//File open
	fileopen(strFileName);

	make_nsf(Header.romcode.c_str());
	write((char*)&nsf, sizeof(nsf));
	write(romimg, sizeof(romimg));

	//----------------------
	//Close file
	close();
}

//==============================================================
//		アセンブリ言語ソースへの保存
//--------------------------------------------------------------
//	●引数
//		const char*	strFileName		ファイル名
//	●返値
//				無し
//==============================================================
void	MusicFile::saveASM(const char*	strFileName)
{
	//----------------------
	//File open
	fileopen(strFileName);

	//----------------------
	//Close file
	close();
}

//==============================================================
//		Ｃ言語ソースへの保存
//--------------------------------------------------------------
//	●引数
//		const char*	strFileName		ファイル名
//	●返値
//				無し
//==============================================================
void	MusicFile::saveC(const char*	strFileName)
{
	//----------------------
	//File open
	fileopen(strFileName);

	//----------------------
	//Close file
	close();
}

//==============================================================
//		エラー処理
//--------------------------------------------------------------
//	●引数
//				無し
//	●返値
//				無し
//==============================================================
void	MusicFile::Err(const char* msg)
{
	cout << "[ ERROR ] : " << msg << endl;

	//異常終了
	exit(-1);
}

//==============================================================
//		ワーニング処理
//--------------------------------------------------------------
//	●引数
//				無し
//	●返値
//				無し
//==============================================================
void	MusicFile::Warning(const char* msg)
{
	//現在のファイル名と、行数を表示
	cout << "[WARNING] : " << msg << endl;
}
