#include "StdAfx.h"
#include "MusicFile.h"

/****************************************************************/
/*					グローバル変数（クラスだけど・・・）		*/
/****************************************************************/
extern	OPSW*			cOptionSW;	//オプション情報へのポインタ変数

//==============================================================
//		コンストラクタ
//--------------------------------------------------------------
//	●引数
//		MMLfile*			MML			ＭＭＬファイルのオブジェクト
//		string				_code		リンクするコード（*.bin）のファイル名
//		const		wchar_t	_strName[]	オブジェクト名
//	●返値
//				無し
//==============================================================
MusicFile::MusicFile(MMLfile* MML, string _code, const wchar_t _strName[]):
	MusicItem(_strName),
	cDPCMinfo(NULL),
	Header(_code)
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
	id_External,
	id_Bank,

	//for ASM output
	id_SegmentSEQ,
	id_SegmentPCM,
	id_Label,
	id_Priority,

	//General
	id_include,
	id_timebase,
	id_OctaveReverse,
	id_bgm_num,
	id_se_num,
	id_releaseVolume,
	id_repeatMode,
	id_TieMode,
	id_offset_Ei,
	id_offset_Ev,
	id_offset_En,
	id_offset_Em,
	id_QMax,
	id_rest,
	id_wait,

	//Block
	id_DPCM,
	id_FDSC,
	id_FDSM,
	id_VRC7,
	id_N163,
	id_Envelop,
	id_Vibrato,
	id_Macro,
	id_Sub,
	id_BGM,
	id_SE,
	id_Patch,

	id_Null
};

//	これらは、MML構文で使えるコマンド。
const	static	Command_Info	Command[] = {
		//for NSF output
		{	"#Title",			id_Title		},
		{	"#title",			id_Title		},
		{	"曲名",				id_Title		},
		{	"#Composer",		id_Composer		},
		{	"#composer",		id_Composer		},
		{	"作曲者",			id_Composer		},
		{	"作曲",				id_Composer		},
		{	"#Copyright",		id_Copyright	},
		{	"#copyright",		id_Copyright	},
		{	"作者",				id_Copyright	},
		{	"著作権者",			id_Copyright	},
		{	"#OffsetPCM",		id_OffsetPCM	},	//Offset Address of ⊿PCM
		{	"#offsetPCM",		id_OffsetPCM	},	//Offset Address of ⊿PCM
		{	"#offsetPCM",		id_OffsetPCM	},	//Offset Address of ⊿PCM
		{	"#Code",			id_Code			},
		{	"#code",			id_Code			},
		{	"コード",			id_Code			},
		{	"#External",		id_External		},
		{	"#external",		id_External		},
		{	"#Bank",			id_Bank			},
		{	"#bank",			id_Bank			},
		{	"バンク",			id_Bank			},
		//for ASM output
		{	"#SegmentPCM",		id_SegmentPCM	},	//Segment name for ⊿PCM
		{	"#segmentPCM",		id_SegmentPCM	},
		{	"#SegmentSEQ",		id_SegmentSEQ	},	//Segment name for Sequence
		{	"#segmentSEQ",		id_SegmentSEQ	},
		{	"#Segment",			id_SegmentSEQ	},	//Segment name for Sequence
		{	"#segment",			id_SegmentSEQ	},
		{	"#Label",			id_Label		},
		{	"#label",			id_Label		},

		//General
		{	"#Include",			id_include		},
		{	"#include",			id_include		},
		{	"読む",				id_include		},
		{	"#Timebase",		id_timebase		},
		{	"#timebase",		id_timebase		},
		{	"#OctaveReverse",	id_OctaveReverse},
		{	"#octaveReverse",	id_OctaveReverse},
		{	"#BGM",				id_bgm_num		},
		{	"#bgm",				id_bgm_num		},
		{	"音楽数",			id_bgm_num		},
		{	"#SE",				id_se_num		},
		{	"#se",				id_se_num		},
		{	"効果音数",			id_se_num		},
		{	"#releaseVolume",	id_releaseVolume},
		{	"#ReleaseVolume",	id_releaseVolume},
		{	"#repeatMode",		id_repeatMode	},
		{	"#RepeatMode",		id_repeatMode	},
		{	"#TieMode",			id_TieMode		},
		{	"#tieMode",			id_TieMode		},	
		{	"#OffsetE@",		id_offset_Ei	},
		{	"#offsetE@",		id_offset_Ei	},
		{	"#OffsetEv",		id_offset_Ev	},
		{	"#offsetEv",		id_offset_Ev	},
		{	"#OffsetEm",		id_offset_Em	},
		{	"#offsetEm",		id_offset_Em	},
		{	"#OffsetEn",		id_offset_En	},
		{	"#offsetEn",		id_offset_En	},
		{	"#Priority",		id_Priority		},
		{	"#priority",		id_Priority		},
		{	"#QMax",			id_QMax			},
		{	"#Qmax",			id_QMax			},
		{	"#Rest",			id_rest			},
		{	"#rest",			id_rest			},
		{	"#Wait",			id_wait			},
		{	"#wait",			id_wait			},
		//Block
		{	"DPCM",				id_DPCM			},
		{	"FDSC",				id_FDSC			},
		{	"FDSM",				id_FDSM			},
		{	"VRC7",				id_VRC7			},
		{	"N163",				id_N163			},
		{	"Envelope",			id_Envelop		},
		{	"envelope",			id_Envelop		},
		{	"Envelop",			id_Envelop		},
		{	"envelop",			id_Envelop		},
		{	"エンベロープ",		id_Envelop		},
		{	"ビブラート",		id_Vibrato		},
		{	"Sub",				id_Sub			},
		{	"sub",				id_Sub			},
		{	"BGM",				id_BGM			},
		{	"bgm",				id_BGM			},
		{	"音楽",				id_BGM			},
		{	"SE",				id_SE			},
		{	"se",				id_SE			},
		{	"効果音",			id_SE			},
		{	"Patch",			id_Patch		},

		//for 1 command
		{	"D",				id_DPCM			},
		{	"FC",				id_FDSC			},
		{	"FM",				id_FDSM			},
		{	"O",				id_VRC7			},
		{	"V",				id_VRC7			},
		{	"N",				id_N163			},
		{	"E",				id_Envelop		},
		{	"S",				id_Sub			},
		{	"P",				id_Patch		},
		{	"$",				id_Macro		},

		//for mck/ppmck
		{	"@DPCM",			id_DPCM			},
		{	"@FM",				id_FDSC			},
		{	"@MW",				id_FDSM			},
		{	"@OP",				id_VRC7			},
		{	"@N",				id_N163			},
		{	"@E",				id_Envelop		},

		{	"　",				id_Null			},
	};

	unsigned	int			i;
	unsigned	char		cData;
				FDSC*		_fdsc;
				FDSM*		_fdsm;
				VRC7*		_vrc7;
				N163*		_n163;
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
		MML->Back();

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
			case(id_External):
				Header.Set_External(MML);
				break;
			case(id_Bank):
				Header.Set_Bank();
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
			case(id_bgm_num):
				Header.Set_Number_BGM(MML);
				break;
			case(id_se_num):
				Header.Set_Number_SE(MML);
				break;
			case(id_timebase):
				MML->timebase = MML->GetInt();	//これは、MMLファイルの属性。
				break;
			case(id_OctaveReverse):
				MML->octave_reverse = true;		//これは、MMLファイルの属性。
				break;
			case(id_releaseVolume):
				MML->iReleaseVolume	=  MML->GetInt();
				if((MML->iReleaseVolume<0) || (MML->iReleaseVolume>15)){
					MML->Err(L"#ReleaseVolumeコマンドは、0～15の範囲で指定してください。");
				}
				break;
			case(id_repeatMode):
				MML->iRepeatMode	=  MML->GetInt();
				if((MML->iRepeatMode<0) || (MML->iRepeatMode>2)){
					MML->Err(L"#RepeatModeコマンドは、0～2の範囲で指定してください。");
				}
				break;
			case(id_TieMode):
				MML->iTieMode	=  MML->GetInt();
				if((MML->iTieMode<0) || (MML->iTieMode>1)){
					MML->Err(L"#TieModeコマンドは、0～1の範囲で指定してください。");
				}
				break;
			case(id_offset_Ei):
				MML->offset_Ei = MML->GetInt();
				break;
			case(id_offset_Ev):
				MML->offset_Ev = MML->GetInt();
				break;
			case(id_offset_En):
				MML->offset_En = MML->GetInt();
				break;
			case(id_offset_Em):
				MML->offset_Em = MML->GetInt();
				break;
			case(id_Priority):
				MML->priority = MML->GetInt();
				if((MML->priority<0) || (MML->priority>3)){
					MML->Err(L"#priorityコマンドは、0～3の範囲で指定してください。");
				}
				break;
			case(id_QMax):
				MML->QMax = MML->GetInt();
				break;
			case(id_rest):
				MML->rest = MML->GetInt();
				if((MML->rest<0) || (MML->rest>2)){
					MML->Err(L"#Restコマンドは、0～2の範囲で指定してください。");
				}
				break;
			case(id_wait):
				MML->wait = MML->GetInt();
				if((MML->rest<0) || (MML->rest>2)){
					MML->Err(L"#Waitコマンドは、0～2の範囲で指定してください。");
				}
				break;
			case(id_Macro):
				MML->SetMacro(0);
				break;
			case(id_Patch):
				MML->SetPatch();
				break;
			//MML
			case(id_DPCM):
				if(cDPCMinfo != NULL){
					MML->Err(L"DPCMブロックは１つまでです。");
				}
				cDPCMinfo = new DPCMinfo(MML, Header.bank);
				ptcItem.push_back(cDPCMinfo);
				iSize += cDPCMinfo->getSize();	//BGMのサイズを更新
				break;
			case(id_FDSC):
				i = MML->GetNum();

				//重複チェック
				if(ptcFDSC.count(i) != 0){
					MML->Err(L"FDSC()ブロックで同じ番号が指定されました。");
				}
				_fdsc = new FDSC(MML, i);
				ptcItem.push_back(_fdsc);
				ptcFDSC[i] = _fdsc;
				iSize += _fdsc->getSize();	//BGMのサイズを更新
				break;
			case(id_FDSM):
				i = MML->GetNum();

				//重複チェック
				if(ptcFDSM.count(i) != 0){
					MML->Err(L"FDSM()ブロックで同じ番号が指定されました。");
				}
				_fdsm = new FDSM(MML, i);
				ptcItem.push_back(_fdsm);
				ptcFDSM[i] = _fdsm;
				iSize += _fdsm->getSize();	//BGMのサイズを更新
				break;
			case(id_VRC7):
				i = MML->GetNum();

				//重複チェック
				if(ptcVRC7.count(i) != 0){
					MML->Err(L"VRC7()ブロックで同じ番号が指定されました。");
				}
				_vrc7 = new VRC7(MML, i);
				ptcItem.push_back(_vrc7);
				ptcVRC7[i] = _vrc7;
				iSize += _vrc7->getSize();	//BGMのサイズを更新
				break;
			case(id_N163):
				i = MML->GetNum();

				//重複チェック
				if(ptcN163.count(i) != 0){
					MML->Err(L"N163()ブロックで同じ番号が指定されました。");
				}
				_n163 = new N163(MML, i);
				ptcItem.push_back(_n163);
				ptcN163[i] = _n163;
				iSize += _n163->getSize();	//BGMのサイズを更新
				break;
			case(id_Envelop):
				i = MML->GetNum();
				//重複チェック
				if(ptcEnv.count(i) != 0){
					MML->Err(L"Envelope()ブロックで同じ番号が指定されました。");
				}
				_env = new Envelop(MML, i);
				ptcItem.push_back(_env);
				ptcEnv[i] = _env;
				iSize += _env->getSize();	//BGMのサイズを更新
				break;
			case(id_Vibrato):
				MML->offset_Em = 1000000;
				i = MML->GetNum() + MML->offset_Em;
				//重複チェック
				if(ptcEnv.count(i) != 0){
					MML->Err(L"ビブラート()ブロックで同じ番号が指定されました。");
				}
				_env = new Envelop(MML, i);
				ptcItem.push_back(_env);
				ptcEnv[i] = _env;
				iSize += _env->getSize();	//BGMのサイズを更新
				break;
			case(id_Sub):
				i = MML->GetNum();
				//重複チェック
				if(ptcSub.count(i) != 0){
					MML->Err(L"Sub()ブロックで同じ番号が指定されました。");
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
					MML->Err(L"BGM()ブロックで同じ番号が指定されました。");
				}
				//範囲チェック
				if((Header.iBGM <= i) || (i<0)){
					MML->Err(L"BGM()ブロックで指定できる範囲を超えています。\n#BGMの数値を確認してください。");
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
					MML->Err(L"SE()ブロックで同じ番号が指定されました。");
				}
				//範囲チェック
				if((Header.iSE <= i) || (i<0)){
					MML->Err(L"SE()ブロックで指定できる範囲を超えています。\n#SEの数値を確認してください。");
				}
				_se = new SE(MML, i);
				ptcItem.push_back(_se);
				ptcSE[i] = _se;
				iSize += _se->getSize();	//BGMのサイズを更新
				break;
			case(id_Null):
				break;
			default:
				MML->Err(L"unknown command");
				break;
		}
		
	} while( !MML->eom() );

	//Check
	if( Header.iBGM + Header.iSE > 255){
		Err(L"BGMとSEの数が合計で255を越えました。");
	}

	i = 0;
	while(i < Header.iBGM){
		if(ptcBGM.count(i) == 0){
			Err(L"BGMデータが足りません。");
		};
		i++;
	}

	i = 0;
	while(i < Header.iSE){
		if(ptcSE.count(i) == 0){
			Err(L"SE データが足りません。");
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
//		デストラクタ
//--------------------------------------------------------------
//	●引数
//				無し
//	●返値
//				無し
//==============================================================
void	MusicFile::TickCount(void)
{
	unsigned	int			iBGM	= 0;
	unsigned	int			iSE		= 0;

	while(iBGM < Header.iBGM){
		cout << "---- BGM(" << iBGM << ") ----" <<endl;
		ptcBGM[iBGM]->TickCount(this);
		iBGM++;
	}

	while(iSE < Header.iSE){
		cout << "---- SE(" << iSE << ") ----" <<endl;
		ptcSE[iSE]->TickCount(this);
		iSE++;
	}


}

//==============================================================
//		ΔPCMのオフセットアドレスを計算
//--------------------------------------------------------------
//	●引数
//		unsigned	int	iMusSize	シーケンスのサイズ
//	●返値
//		unsigned	int	ΔPCMの合計サイズ
//==============================================================
unsigned	int		MusicFile::SetDPCMOffset(unsigned int iMusSize)
{
	unsigned	int		i;
	unsigned	char	mus_bank = (unsigned char)(iMusSize >> 12);

	if((iMusSize & 0x0FFF) != 0){
		mus_bank++;
	}


	dpcm_code.clear();
	if(cDPCMinfo != NULL){
		cDPCMinfo->getDPCMCode(&dpcm_code);
		i = cDPCMinfo->setDPCMoffset(Header.offsetPCM, mus_bank+3);
	} else {
		i = Header.offsetPCM;
	}
	return(i - Header.offsetPCM);
}

//==============================================================
//		使用しないオブジェクトの検索＆削除
//--------------------------------------------------------------
//	●引数
//				無し
//	●返値
//				無し
//==============================================================
void	MusicFile::Optimize(void)
{
	map<	int, FDSC*		>::iterator	itFDSC;		//FDS  wave table (career)
	map<	int, FDSM*		>::iterator	itFDSM;		//FDS  wave table (modulator)
	map<	int, VRC7*		>::iterator	itVRC7;		//VRC7 User Instrument
	map<	int, N163*		>::iterator	itN163;		//N163 wave table
	map<	int, Envelop*	>::iterator	itEnv;		//Envelop
	map<	int, Sub*		>::iterator	itSub;		//Subroutine

	unsigned	int			iBGM	= 0;
	unsigned	int			iSE		= 0;

	//----------------------
	//使う（使わない）オブジェクトの検索
	while(iBGM < Header.iBGM){
		ptcBGM[iBGM]->Optimize(this);
		iBGM++;
	}

	while(iSE < Header.iSE){
		ptcSE[iSE]->Optimize(this);
		iSE++;
	}

	//----------------------
	//使わないオブジェクトの削除

	//サブルーチン
	if(!ptcSub.empty()){
		itSub = ptcSub.begin();
		while(itSub != ptcSub.end()){
			if(itSub->second->chkUse() == true){
				//使うサブルーチンであれば、その中で使ってるオブジェクトをチェック
				itSub->second->Optimize(this);
			} else {
				//使わないサブルーチンであれば、オブジェクト削除。
				itSub->second->clear(itSub->first);
			}
			itSub++;
		}
	}

	//エンベロープ
	if(!ptcEnv.empty()){
		itEnv = ptcEnv.begin();
		while(itEnv != ptcEnv.end()){
			if(itEnv->second->chkUse() == false){
				//使わないサブルーチンであれば、オブジェクト削除。
				itEnv->second->clear(itEnv->first);
			}
			itEnv++;
		}
	}

	//FDSC
	if(!ptcFDSC.empty()){
		itFDSC = ptcFDSC.begin();
		while(itFDSC != ptcFDSC.end()){
			if(itFDSC->second->chkUse() == false){
				//使わないサブルーチンであれば、オブジェクト削除。
				itFDSC->second->clear(itFDSC->first);
			}
			itFDSC++;
		}
	}

	//FDSM
	if(!ptcFDSM.empty()){
		itFDSM = ptcFDSM.begin();
		while(itFDSM != ptcFDSM.end()){
			if(itFDSM->second->chkUse() == false){
				//使わないサブルーチンであれば、オブジェクト削除。
				itFDSM->second->clear(itFDSM->first);
			}
			itFDSM++;
		}
	}

	//VRC7
	if(!ptcVRC7.empty()){
		itVRC7 = ptcVRC7.begin();
		while(itVRC7 != ptcVRC7.end()){
			if(itVRC7->second->chkUse() == false){
				//使わないサブルーチンであれば、オブジェクト削除。
				itVRC7->second->clear(itVRC7->first);
			}
			itVRC7++;
		}
	}

	//N163
	if(!ptcN163.empty()){
		itN163 = ptcN163.begin();
		while(itN163 != ptcN163.end()){
			if(itN163->second->chkUse() == false){
				//使わないサブルーチンであれば、オブジェクト削除。
				itN163->second->clear(itN163->first);
			}
			itN163++;
		}
	}

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
		ptcBGM[iBGM]->Fix_Address(this);
		iBGM++;
	}

	while(iSE < Header.iSE){
		ptcSE[iSE]->Fix_Address(this);
		iSE++;
	}

	if(!ptcSub.empty()){
		itSub = ptcSub.begin();
		while(itSub != ptcSub.end()){
			itSub->second->Fix_Address(this);
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
//		size_t	rom_size	*.binのサイズ
//	●返値
//				無し
//==============================================================
void	MusicFile::make_bin(size_t rom_size, int ptOffset)
{
				string		_str;
	unsigned	int			i		= 2;
	unsigned	int			iBGM	= 0;
	unsigned	int			iSE		= 0;
	unsigned	short*		pt;

				size_t		_size	= 4 + (Header.iBGM + Header.iSE)*2;


	//曲バイナリーの作成
	_str.clear();
	_str.resize(_size);

	pt = (unsigned short*)_str.c_str();

	_str[0] = Header.iBGM;
	_str[1] = Header.iSE;

	if(Header.bank == false){

		if(cDPCMinfo != NULL){
			pt[1]	= ptOffset + (unsigned short)rom_size - 0x80 + (unsigned short)_size + (unsigned short)cDPCMinfo->getOffset();	//ΔPCM info のアドレス
		} else {
			pt[1]	= 0;
		}

		while(iBGM < Header.iBGM){
			pt[i] = ptOffset + (unsigned short)rom_size - 0x80 + (unsigned short)_size + (unsigned short)ptcBGM[iBGM]->getOffset();
			i++;
			iBGM++;
		}
		while(iSE < Header.iSE){
			pt[i] = ptOffset + (unsigned short)rom_size - 0x80 + (unsigned short)_size + (unsigned short)ptcSE[iSE]->getOffset();
			i++;
			iSE++;
		}

	} else {

		if(cDPCMinfo != NULL){
			pt[1]	= ptOffset + (unsigned short)_size + (unsigned short)cDPCMinfo->getOffset();	//ΔPCM info のアドレス
		} else {
			pt[1]	= 0;
		}

		while(iBGM < Header.iBGM){
			pt[i] = ptOffset + (unsigned short)_size + (unsigned short)ptcBGM[iBGM]->getOffset();
			i++;
			iBGM++;
		}
		while(iSE < Header.iSE){
			pt[i] = ptOffset + (unsigned short)_size + (unsigned short)ptcSE[iSE]->getOffset();
			i++;
			iSE++;
		}

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
//		const	char*	strFileName		ファイル名
//		bool			opt				最適化フラグ
//	●返値
//				無し
//==============================================================
void	MusicFile::saveNSF(const char*	strFileName)
{

	unsigned	int		i,j;
				size_t	bin_size;
				size_t	mus_size;
				size_t	pcm_size;
	unsigned	char	mus_bank;
	unsigned	char	pcm_bank;
				char*	romimg		= new char[0xC000+0x80];
	NSF_Header*			nsf			= (NSF_Header*)romimg;
	FileInput*			_romcode	= new FileInput();
				bool	dpcm_bank	= false;
				bool	opt			= cOptionSW->opt;

	//NSF用コードの転送
	_romcode->fileopen(Header.romcode.c_str(), &(cOptionSW->m_pass_code));
	bin_size = _romcode->GetSize();
	_romcode->read(romimg, bin_size);
	_romcode->close();
	delete		_romcode;

	//NSFヘッダーの更新
	memcpy(&nsf->Title, Header.title.c_str(), 32);
	memcpy(&nsf->Composer, Header.composer.c_str(), 32);
	memcpy(&nsf->Copyright, Header.copyright.c_str(), 32);
	nsf->MusicNumber	= Header.iBGM + Header.iSE;
	if(Header.iExternal != -1){
		nsf->External	= (unsigned char)Header.iExternal;
	}

	wcout << L"----------------------------------------" << endl;
	wcout << L"*NSF build process" << endl;


	if((nsf->Bank[0] == 0) && (nsf->Bank[1] == 0) && (nsf->Bank[2] == 0) && (nsf->Bank[3] == 0)){

		//------------------------------
		//Bank 非対応bin

		//シーケンスのバイナリを生成
		make_bin(bin_size, 0x8000);

		if(Header.bank == true){
			Err(L"指定の.binファイルは、⊿PCMのバンクに対応していません。\n⊿PCMのバンクに対応した.binファイルを指定してください。");
		}

		mus_size = bin_size - 0x80 + code.size();
		mus_bank = (unsigned char)(mus_size >> 12);
		if((mus_size & 0x0FFF) != 0){
			mus_bank++;
		}

		//サイズチェック
		wcout << L"[CODE & MUSIC]" << endl;
		wcout << L"  Bank = " << (unsigned int)mus_bank << endl;
		wcout << L"  Size = " << (unsigned int)mus_size << L" [Byte] / " << Header.offsetPCM - 0x8000 << L" [Byte]" << endl;

		if((0x8000 + mus_size) > Header.offsetPCM){
			Err(L"コード・シーケンスのサイズが許容値を越えました。");
		}

	} else {

		//------------------------------
		//Bank対応bin？

		unsigned	int	iSizeLimit = 0x10000;	//拡張RAMへの転送有り

		//シーケンスのバイナリを生成
		make_bin(bin_size, 0x0000);

		if(Header.bank == false){
			Err(L"指定の.binファイルは、⊿PCMのバンクに対応しています。\n#Bankコマンドを指定してください。");
		}

		dpcm_bank = true;
		mus_size = code.size();
		mus_bank = (unsigned char)(mus_size >> 12);
		if((mus_size & 0x0FFF) != 0){
			mus_bank++;
		}

		wcout << L"[CODE]" << endl;
		wcout << L"  Bank = 3" << endl;
		wcout << L"  Size = 12288 [Byte]" << endl;

		wcout << L"[MUSIC]" << endl;
		wcout << L"  Bank = " << (unsigned int)mus_bank << endl;
		wcout << L"  Size = " << (unsigned int)mus_size << L" [Byte] / " << iSizeLimit << L" [Byte]" << endl;

		//サイズチェック
		if(mus_size > iSizeLimit){
			Err(L"コード・シーケンスのサイズが許容値を越えました。");
		}

	}

	//⊿PCM
	wcout << L"[DPCM]" << endl;

	pcm_size = dpcm_code.size();
	pcm_bank = (unsigned char)((pcm_size + (Header.offsetPCM & 0x0FFF)) >> 12);
	if((pcm_size & 0x0FFF) != 0){
		pcm_bank++;
	}


	if(dpcm_bank == false){
		//⊿PCMサイズチェック
		wcout << L"  Bank = " << (unsigned int)pcm_bank << endl;
		wcout << L"  Size = " << (unsigned int)pcm_size << L" [Byte] / " << 0x10000 - Header.offsetPCM << L" [Byte]" << endl;

		if(	(Header.offsetPCM + pcm_size) > 0x10000	){
			Err(L"⊿PCMのサイズが許容値を越えました。");
		}

	} else {
		//⊿PCMサイズチェック
		wcout << L"  Bank = " << (unsigned int)pcm_bank << endl;
		wcout << L"  Size = " << (unsigned int)pcm_size << L" [Byte]" << endl;

		i = mus_bank + pcm_bank + 3;
		if(i > 255){
			Err(L"バンク数の合計が255を越えました。");
		}
	}

	//----------------------
	//ＮＳＦ書き込み
	fileopen(strFileName);

	if(dpcm_bank == false){
		if(cDPCMinfo == NULL){
			//⊿PCMを使わない場合
			write(romimg, bin_size);			//NSFヘッダー ＆ コードの書き込み
			write(code.c_str(), code.size());	//シーケンスの書き込み

		} else {
			//⊿PCMを使う場合
			if(opt == true){
				//最適化が有効であれば、ヘッダーにバンク情報を書く。
				i = 0;
				while(i < mus_bank){
					nsf->Bank[i] = (unsigned char)i;
					i++;
				}
				while(i < ((Header.offsetPCM - 0x8000)>>12)){
					nsf->Bank[i] = 0;
					i++;
				}
				j = 0;
				while(i < 8){
					if(j < pcm_bank){
						nsf->Bank[i] = mus_bank + (unsigned char)j;
					} else {
						nsf->Bank[i] = 0;
					}
					i++;
					j++;
				}
			}

			//コード＆シーケンス
			write(romimg, bin_size);			//NSFヘッダー ＆ コードの書き込み
			write(code.c_str(), code.size());	//シーケンスの書き込み

			if(opt == true){
				//GAP
				while(mus_size < ((unsigned int)mus_bank<<12)){
					put(0);		//0 padding
					mus_size++;
				}
				//GAP2
				mus_size = Header.offsetPCM & 0x0FFF;
				while(mus_size > 0 ){
					put(0);		//0 padding
					mus_size--;
				}
				//ΔPCM
				write(dpcm_code.c_str(), pcm_size);		//⊿PCMの書き込み
				while(pcm_size < ((unsigned int)pcm_bank<<12)){
					put(0);		//0 padding
					pcm_size++;
				}
			} else {
				//GAP
				while(mus_size < (Header.offsetPCM - 0x8000)){
					put(0);		//0 padding
					mus_size++;
				}
				//ΔPCM
				write(dpcm_code.c_str(), pcm_size);		//⊿PCMの書き込み
			}
		}
	} else {
		//Bank 対応bin
		write(&romimg[0x0000], bin_size);	//NSFヘッダー ＆ コードの書き込み
		write(code.c_str(), code.size());	//シーケンスの書き込み
		//GAP
		while(mus_size < ((unsigned int)mus_bank<<12)){
			put(0);		//0 padding
			mus_size++;
		}

		write(dpcm_code.c_str(), pcm_size);		//⊿PCMの書き込み
		//GAP
		while(pcm_size < ((unsigned int)pcm_bank<<12)){
			put(0);		//0 padding
			pcm_size++;
		}

		if(opt != true){
			//GAP（必ず、32kByte以上にする。）
			i = (mus_bank + pcm_bank + 3) << 12;
			while(i < 0x8000){
				put(0);		//0 padding
				i++;
			}
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
//		const	char*	strFileName		ファイル名
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
//		const	wchar_t	msg[]	エラーメッセージ
//	●返値
//				無し
//==============================================================
void	MusicFile::Err(const wchar_t msg[])
{
	if(cOptionSW->fErr == true){
		wcerr << L"[ ERROR ] : " << msg << endl;
	} else {
		wcout << L"[ ERROR ] : " << msg << endl;
	}

	//異常終了
	nsc_exit(EXIT_FAILURE);
}

//==============================================================
//		ワーニング処理
//--------------------------------------------------------------
//	●引数
//		const	wchar_t	msg[]	ワーニングメッセージ
//	●返値
//				無し
//==============================================================
void	MusicFile::Warning(const wchar_t msg[])
{
	//現在のファイル名と、行数を表示
	if(cOptionSW->fErr == true){
		wcerr << L"[WARNING] : " << msg << endl;
	} else {
		wcout << L"[WARNING] : " << msg << endl;
	}
}
