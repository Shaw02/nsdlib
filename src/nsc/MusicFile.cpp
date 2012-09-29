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
MusicFile::MusicFile(MMLfile* MML, string _code, const char _strName[]):
	MusicItem(_strName),
	cDPCMinfo(NULL),
	Header(MML, _code)
{
	//----------------------
	//Local変数


//	定数定義
enum	Command_ID_MusicFile {
	//for NSF output
	id_Title,
	id_Composer,
	id_Copyright,
	id_OffsetPCM,
	id_Code,

	//for ASM output
	id_SegmentSEQ,
	id_SegmentPCM,
	id_Label,

	//General
	id_include,
	id_OctaveReverse,
	id_bgm_num,
	id_se_num,

	//Block
	id_DPCM,
	id_Envelop,
	id_Macro,
	id_Sub,
	id_BGM,
	id_SE
};

//	これらは、MML構文で使えるコマンド。
const	static	Command_Info	Command[] = {
		//for NSF output
		{	"#Title",			id_Title		},
		{	"#title",			id_Title		},
		{	"#Composer",		id_Composer		},
		{	"#composer",		id_Composer		},
		{	"#Copyright",		id_Copyright	},
		{	"#copyright",		id_Copyright	},
		{	"#OffsetPCM",		id_OffsetPCM	},	//Offset Address of ⊿PCM
		{	"#offsetPCM",		id_OffsetPCM	},	//Offset Address of ⊿PCM
		{	"#Code",			id_Code			},
		{	"#code",			id_Code			},
		//for ASM output
		{	"#Segment",			id_SegmentSEQ	},	//Segment name for Sequence
		{	"#segment",			id_SegmentSEQ	},
		{	"#SegmentSEQ",		id_SegmentSEQ	},	//Segment name for Sequence
		{	"#segmentSEQ",		id_SegmentSEQ	},
		{	"#SegmentPCM",		id_SegmentPCM	},	//Segment name for ⊿PCM
		{	"#segmentPCM",		id_SegmentPCM	},
		{	"#Label",			id_Label		},
		{	"#label",			id_Label		},
		//General
		{	"#Include",			id_include		},
		{	"#include",			id_include		},
		{	"#OctaveReverse",	id_OctaveReverse},
		{	"#octaveReverse",	id_OctaveReverse},
		{	"#BGM",				id_bgm_num		},
		{	"#bgm",				id_bgm_num		},
		{	"#SE",				id_se_num		},
		{	"#se",				id_se_num		},
		//Block
		{	"DPCM",				id_DPCM			},
		{	"Envelope",			id_Envelop		},
		{	"envelope",			id_Envelop		},
		{	"Envelop",			id_Envelop		},
		{	"envelop",			id_Envelop		},
		{	"E",				id_Envelop		},
		{	"$",				id_Macro		},
		{	"Sub",				id_Sub			},
		{	"sub",				id_Sub			},
		{	"BGM",				id_BGM			},
		{	"bgm",				id_BGM			},
		{	"SE",				id_SE			},
		{	"se",				id_SE			},
	};

	unsigned	int			i;
	unsigned	char		cData;
				Envelop*	_env;
				BGM*		_bgm;
				SE*			_se;
				Sub*		_sub;
	string		msg;

	map<	int,	Envelop*	>::iterator	itEnvelop;
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
			//for NSF output
			case(id_Code):
				Header.Set_RomCode(MML);
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
			case(id_OffsetPCM):
				Header.Set_OffsetPCM(MML);
				break;
			//for ASM output
			case(id_SegmentSEQ):
				Header.Set_SegmentSEQ(MML);
				break;
			case(id_SegmentPCM):
				Header.Set_SegmentPCM(MML);
				break;
			case(id_Label):
				Header.Set_Label(MML);
				break;
			//General
			case(id_include):
				MML->include();
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
			//MML
			case(id_DPCM):
				if(cDPCMinfo != NULL){
					MML->Err("DPCMブロックは１つまでです。");
				}
				cDPCMinfo = new DPCMinfo(MML);
				ptcItem.push_back(cDPCMinfo);
				break;
			case(id_Envelop):
				i = MML->GetNum();
				//重複チェック
				if(ptcEnv.count(i) != 0){
					MML->Err("Envelop()ブロックで同じ番号が指定されました。");
				}
				_env = new Envelop(MML, i);
				ptcItem.push_back(_env);
				ptcEnv[i] = _env;
				iSize += _env->getSize();	//BGMのサイズを更新
				break;
			case(id_Macro):
				//■■■　to do
				break;
			case(id_Sub):
				i = MML->GetNum();
				//重複チェック
				if(ptcSub.count(i) != 0){
					MML->Err("Sub()ブロックで同じ番号が指定されました。");
				}
				//範囲チェック
				_sub = new Sub(MML, i);
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
				_bgm = new BGM(MML, i);
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
				_se = new SE(MML, i);
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
//		ΔPCMのオフセットアドレスを計算
//--------------------------------------------------------------
//	●引数
//				無し
//	●返値
//		unsigned	int	ΔPCMの合計サイズ
//==============================================================
unsigned	int		MusicFile::SetDPCMOffset(void)
{
	unsigned	int	i;

	dpcm_code.clear();
	if(cDPCMinfo != NULL){
		cDPCMinfo->getDPCMCode(&dpcm_code);
		i = cDPCMinfo->setDPCMoffset(Header.offsetPCM);
	} else {
		i = Header.offsetPCM;
	}
	return(i - Header.offsetPCM);
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
	map<int,Sub*	>::iterator	itSub;

	unsigned	int			iBGM	= 0;
	unsigned	int			iSE		= 0;

	while(iBGM < Header.iBGM){
		ptcBGM[iBGM]->Fix_Address(&ptcSub, &ptcEnv);
		iBGM++;
	}
	while(iSE < Header.iSE){
		ptcSE[iSE]->Fix_Address(&ptcSub, &ptcEnv);
		iSE++;
	}

	if(!ptcSub.empty()){
		itSub = ptcSub.begin();
		while(itSub != ptcSub.end()){
			itSub->second->Fix_Address(&ptcSub, &ptcEnv);
			itSub++;
		}
	}
}

/*
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

}
*/		
//==============================================================
//		ＮＳＦの作成
//--------------------------------------------------------------
//	●引数
//		const char*	strFileName	コード
//	●返値
//				無し
//==============================================================
void	MusicFile::make_bin(unsigned int rom_size)
{
				string		_str;
	unsigned	int			i		= 2;
	unsigned	int			iBGM	= 0;
	unsigned	int			iSE		= 0;
	unsigned	__int16*	pt;

	unsigned	int			_size	= 4 + (Header.iBGM + Header.iSE)*2;


	//曲バイナリーの作成
	_str.clear();
	_str.resize(_size);

	pt = (unsigned __int16*)_str.c_str();

	_str[0] = Header.iBGM;
	_str[1] = Header.iSE;

	if(cDPCMinfo != NULL){
		pt[1]	= 0x8000 + rom_size - 0x80 + _size + cDPCMinfo->getOffset();	//ΔPCM info のアドレス
	} else {
		pt[1]	= 0;
	}

	while(iBGM < Header.iBGM){
		pt[i] = 0x8000 + rom_size - 0x80 + _size + ptcBGM[iBGM]->getOffset();
		i++;
		iBGM++;
	}
	while(iSE < Header.iSE){
		pt[i] = 0x8000 + rom_size - 0x80 + _size + ptcSE[iSE]->getOffset();
		i++;
		iSE++;
	}

	getCode(&_str);
	code = _str;

	//ΔPCMの作成
	dpcm_code.clear();
	if(cDPCMinfo != NULL){
		cDPCMinfo->getDPCMCode(&dpcm_code);
	}

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

	unsigned	int		i,j;
	unsigned	int		bin_size;
	unsigned	int		mus_size;
	unsigned	int		pcm_size;
	unsigned	char	mus_bank;
	unsigned	char	pcm_bank;
				char*	romimg		= new char[0x8000+0x80];
	NSF_Header*			nsf			= (NSF_Header*)romimg;
	FileInput*			_romcode	= new FileInput();




	//NSF用コードの転送
	_romcode->fileopen(Header.romcode.c_str());
	bin_size = _romcode->GetSize();
	_romcode->read(romimg, bin_size);
	_romcode->close();
	delete		_romcode;

	//シーケンスのバイナリを生成
	make_bin(bin_size);

	mus_size = bin_size - 0x80 + code.size();
	mus_bank = mus_size >> 12;
	if((mus_size & 0x0FFF) != 0){
		mus_bank++;
	}

	//サイズチェック
	if((0x8000 + mus_size) > Header.offsetPCM){
		cout << "コード・シーケンスのサイズが許容値を越えました。" << endl;
		cout << "　許容値：" << Header.offsetPCM - 0x8000 << "[Byte]" << endl;
		cout << "　サイズ：" << mus_size << "[Byte]" << endl;
		exit(-1);
	}

	//⊿PCM
	pcm_size = dpcm_code.size();
	pcm_bank = pcm_size >> 12;
	if((pcm_size & 0x0FFF) != 0){
		pcm_bank++;
	}
	//⊿PCMサイズチェック
	if(	(Header.offsetPCM + pcm_size) > 0x10000	){
		cout << "⊿PCMのサイズが許容値を越えました。" << endl;
		cout << "　許容値：" << 0x10000 - Header.offsetPCM << "[Byte]" << endl;
		cout << "　サイズ：" << pcm_size << "[Byte]" << endl;
		exit(-1);
	}

	//NSFヘッダーの更新
	nsf->MusicNumber		= Header.iBGM + Header.iSE;
	memcpy(&nsf->Title, Header.title.c_str(), 32);
	memcpy(&nsf->Composer, Header.composer.c_str(), 32);
	memcpy(&nsf->Copyright, Header.copyright.c_str(), 32);

	//----------------------
	//ＮＳＦ書き込み
	fileopen(strFileName);

	if(cDPCMinfo == NULL){
		write(romimg, bin_size);			//NSFヘッダー ＆ コードの書き込み
		write(code.c_str(), code.size());	//シーケンスの書き込み
	} else {
		//ヘッダーにバンク情報を書く。
		i = 0;
		while(i < mus_bank){
			nsf->Bank[i] = i;
			i++;
		}
		while(i < ((Header.offsetPCM - 0x8000)>>12)){
			nsf->Bank[i] = 0;
			i++;
		}
		j = 0;
		while(i < 8){
			if(j < pcm_bank){
				nsf->Bank[i] = mus_bank + j;
			} else {
				nsf->Bank[i] = 0;
			}
			i++;
			j++;
		}
		//コード＆シーケンス
		write(romimg, bin_size);			//NSFヘッダー ＆ コードの書き込み
		write(code.c_str(), code.size());	//シーケンスの書き込み
		while(mus_size < ((unsigned int)mus_bank<<12)){
			put(0);		//0 padding
			mus_size++;
		}
		//ΔPCM
		write(dpcm_code.c_str(), pcm_size);		//⊿PCMの書き込み
		while(pcm_size < ((unsigned int)pcm_bank<<12)){
			put(0);		//0 padding
			pcm_size++;
		}
	}

	close();

	//----------------------
	//Exit
	delete[]	romimg;
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
	unsigned	int			iBGM	= 0;
	unsigned	int			iSE		= 0;

	//----------------------
	//File open
	fileopen(strFileName);

	//Header
	*this <<	";===============================================================\n"
				";		Music file for NES Sound Driver & Library\n"
				";			for assembly language (ca65.exe)\n"
				";===============================================================\n"
				<<endl;

	//Export of Sequence
	while(iBGM < Header.iBGM){
		*this	<<	"	.export		"	<<	Header.Label	<<	"BGM"	<<	iBGM	<<	endl;
		iBGM++;
	}
	while(iSE < Header.iSE){
		*this	<<	"	.export		"	<<	Header.Label	<<	"SE"	<<	iSE	<<	endl;
		iSE++;
	}

	if(cDPCMinfo != NULL){
		*this	<<	"	.export		"	<<	Header.Label	<<	"DPCMinfo"	<<	endl;
	}

	//MML
	*this <<	"\n\n.segment	"	<<	'"'	<<	Header.segmentSEQ	<<	'"' << endl;

	getAsm(this);

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
