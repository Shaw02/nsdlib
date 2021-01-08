/*******************************************************************************

			NES Sound Driver & Library	(NSD.lib)	MML Compiler

	Copyright (c) 2012 A.Watanabe (S.W.), All rights reserved.
	 For conditions of distribution and use, see copyright notice in "nsc.cpp".

*******************************************************************************/

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
//		const		_CHAR	_strName[]	オブジェクト名
//	●返値
//				無し
//==============================================================
MusicFile::MusicFile(MMLfile* MML, string _code, const _CHAR _strName[]):
	MusicItem(_strName),
	Header(_code),
	cDPCMinfo(NULL)
{
	//----------------------
	//Local変数


//	定数定義
enum	Command_ID_MusicFile {
	//for NSF output
	id_Title,
	id_Composer,
	id_Copyright,
	id_Maker,
	id_Text,
	id_plst,
	id_psfx,
	id_mixe,
	id_vrc7_chg,
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
	id_QReverse,
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
		{	"#Maker",			id_Maker		},
		{	"#maker",			id_Maker		},
		{	"#Ripper",			id_Maker		},
		{	"#ripper",			id_Maker		},
		{	"製作者",			id_Maker		},
		{	"#Text",			id_Text			},
		{	"#text",			id_Text			},
		{	"テキスト",			id_Text			},
		{	"#PlayList",		id_plst			},
		{	"#Plst",			id_plst			},
		{	"#plst",			id_plst			},
		{	"音楽順番",			id_plst			},
		{	"曲順",				id_plst			},
		{	"#PlaySfx",			id_psfx			},
		{	"#Psfx",			id_psfx			},
		{	"#psfx",			id_psfx			},
		{	"効果音順番",		id_psfx			},
		{	"#Mixe",			id_mixe			},
		{	"#mixe",			id_mixe			},
		{	"#Mix",				id_mixe			},
		{	"#mix",				id_mixe			},
		{	"#VRC7",			id_vrc7_chg		},
		{	"#OffsetPCM",		id_OffsetPCM	},	//Offset Address of ⊿PCM
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
		{	"#QReverse",		id_QReverse		},
		{	"#qReverse",		id_QReverse		},
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

				size_t		i;
//	unsigned	char		cData;
	string		msg;

	map<size_t,	Envelop*	>::iterator	itEnvelop;
	map<size_t,	BGM*		>::iterator	itBGM;
	map<size_t,	SE*			>::iterator	itSE;
	map<size_t,	Sub*		>::iterator	itSub;

//	iSize = 0;

	//このオブジェクトは必ず使う（最適化対象外）。
	setUse();

	//メタデータチェック用のフラグ初期化
	f_is_track_time		= false;
	f_is_track_fade		= false;
	f_is_track_label	= false;
	f_is_track_auth		= false;

	do{
		//１文字読み込み（コメントチェック、includeファイルの終端チェックもあり）
//		cData = MML->GetChar();
		MML->GetChar();

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
			case(id_Maker):
				Header.Set_Maker(MML);
				break;
			case(id_Text):
				Header.Text_Append(MML);
				break;
			case(id_plst):
				Header.Set_plst(MML);
				break;
			case(id_psfx):
				Header.Set_psfx(MML);
				break;
			case(id_mixe):
				Header.Set_mixe(MML);
				break;
			case(id_vrc7_chg):
				Header.Ser_VRC7(MML);
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
			case(id_QReverse):
				MML->q_reverse = true;			//これは、MMLファイルの属性。
				break;
			case(id_releaseVolume):
				MML->iReleaseVolume	=  MML->GetInt();
				if((MML->iReleaseVolume<0) || (MML->iReleaseVolume>15)){
					MML->Err(_T("#ReleaseVolumeコマンドは、0～15の範囲で指定してください。"));
				}
				break;
			case(id_repeatMode):
				MML->iRepeatMode	=  MML->GetInt();
				if((MML->iRepeatMode<0) || (MML->iRepeatMode>2)){
					MML->Err(_T("#RepeatModeコマンドは、0～2の範囲で指定してください。"));
				}
				break;
			case(id_TieMode):
				MML->iTieMode	=  MML->GetInt();
				if((MML->iTieMode<0) || (MML->iTieMode>1)){
					MML->Err(_T("#TieModeコマンドは、0～1の範囲で指定してください。"));
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
				i = MML->GetInt();
				if((i<0) || (i>3)){
					MML->Err(_T("#priorityコマンドは、0～3の範囲で指定してください。"));
				} else {
					MML->priority = (unsigned char)i;
				}
				break;
			case(id_QMax):
				MML->QMax = MML->GetInt();
				break;
			case(id_rest):
				MML->rest = MML->GetInt();
				if((MML->rest<0) || (MML->rest>3)){
					MML->Err(_T("#Restコマンドは、0～3の範囲で指定してください。"));
				}
				break;
			case(id_wait):
				MML->wait = MML->GetInt();
				if((MML->rest<0) || (MML->rest>3)){
					MML->Err(_T("#Waitコマンドは、0～3の範囲で指定してください。"));
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
					MML->Err(_T("DPCMブロックは１つまでです。"));
				}
				cDPCMinfo = new DPCMinfo(MML, Header.bank);
				if(cDPCMinfo->isError() == true){
					f_error = true;
				} else {
					ptcItem.push_back(cDPCMinfo);
				}
				break;
			case(id_FDSC):
				{
					FDSC*	_fdsc;
					i = MML->GetNum();
					//重複チェック
					if(ptcFDSC.count(i) != 0){
						MML->Err(_T("FDSC()ブロックで同じ番号が指定されました。"));
					}
					_fdsc = new FDSC(MML, i);
					ptcItem.push_back(_fdsc);
					ptcFDSC[i] = _fdsc;
				}
				break;
			case(id_FDSM):
				{
					FDSM*	_fdsm;
					i = MML->GetNum();
					//重複チェック
					if(ptcFDSM.count(i) != 0){
						MML->Err(_T("FDSM()ブロックで同じ番号が指定されました。"));
					}
					_fdsm = new FDSM(MML, i);
					ptcItem.push_back(_fdsm);
					ptcFDSM[i] = _fdsm;
				}
				break;
			case(id_VRC7):
				{
					VRC7*	_vrc7;
					i = MML->GetNum();
					//重複チェック
					if(ptcVRC7.count(i) != 0){
						MML->Err(_T("VRC7()ブロックで同じ番号が指定されました。"));
					}
					_vrc7 = new VRC7(MML, i);
					ptcItem.push_back(_vrc7);
					ptcVRC7[i] = _vrc7;
				}
				break;
			case(id_N163):
				{
					N163*	_n163;
					i = MML->GetNum();
					//重複チェック
					if(ptcN163.count(i) != 0){
						MML->Err(_T("N163()ブロックで同じ番号が指定されました。"));
					}
					_n163 = new N163(MML, i);
					ptcItem.push_back(_n163);
					ptcN163[i] = _n163;
				}
				break;
			case(id_Envelop):
				{
					Envelop*	_env;
					i = MML->GetNum();
					//重複チェック
					if(ptcEnv.count(i) != 0){
						MML->Err(_T("Envelope()ブロックで同じ番号が指定されました。"));
					}
					_env = new Envelop(MML, i);
					ptcItem.push_back(_env);
					ptcEnv[i] = _env;
				}
				break;
			case(id_Vibrato):
				{
					Envelop*	_env;
					MML->offset_Em = 1000000;
					i = MML->GetNum() + MML->offset_Em;
					//重複チェック
					if(ptcEnv.count(i) != 0){
						MML->Err(_T("ビブラート()ブロックで同じ番号が指定されました。"));
					}
					_env = new Envelop(MML, i);
					ptcItem.push_back(_env);
					ptcEnv[i] = _env;
				}
				break;
			case(id_Sub):
				{
					Sub*	_sub;
					i = MML->GetNum();
					//重複チェック
					if(ptcSub.count(i) != 0){
						MML->Err(_T("Sub()ブロックで同じ番号が指定されました。"));
					}
					//範囲チェック
					_sub = new Sub(MML, i);
					ptcItem.push_back(_sub);
					ptcSub[i] = _sub;
				}
				break;
			case(id_BGM):
				{
					BGM*	_bgm;
					i = MML->GetNum();
					//重複チェック
					if(ptcBGM.count(i) != 0){
						MML->Err(_T("BGM()ブロックで同じ番号が指定されました。"));
					}
					//範囲チェック
					if((Header.iBGM <= i) || (i<0)){
						MML->Err(_T("BGM()ブロックで指定できる範囲を超えています。\n#BGMの数値を確認してください。"));
					}
					_bgm = new BGM(MML, i);
					ptcItem.push_back(_bgm);
					ptcBGM[i] = _bgm;
					//メタデータがあったかチェック
					if(_bgm->isLabel() == true){
						f_is_track_label = true;
					}
					if(_bgm->isAuthor() == true){
						f_is_track_auth	= true;
					}
					if(_bgm->isTime() == true){
						f_is_track_time	= true;
					}
					if(_bgm->isFade() == true){
						f_is_track_fade	= true;
					}
				}
				break;
			case(id_SE):
				{
					SE*	_se;
					i = MML->GetNum();
					//重複チェック
					if(ptcSE.count(i) != 0){
						MML->Err(_T("SE()ブロックで同じ番号が指定されました。"));
					}
					//範囲チェック
					if((Header.iSE <= i) || (i<0)){
						MML->Err(_T("SE()ブロックで指定できる範囲を超えています。\n#SEの数値を確認してください。"));
					}
					_se = new SE(MML, i);
					ptcItem.push_back(_se);
					ptcSE[i] = _se;
					//メタデータがあったかチェック
					if(_se->isLabel() == true){
						f_is_track_label = true;
					}
					if(_se->isAuthor() == true){
						f_is_track_auth	= true;
					}
					if(_se->isTime() == true){
						f_is_track_time	= true;
					}
					if(_se->isFade() == true){
						f_is_track_fade	= true;
					}
				}
				break;
			case(id_Null):
				break;
			default:
				MML->Err(_T("unknown command"));
				break;
		}
		
	} while( !MML->eom() );

	//==============================
	//Check
	if( Header.iBGM + Header.iSE > 255){
		Err(_T("BGMとSEの数が合計で255を越えました。"));
	}

	i = 0;
	while(i < Header.iBGM){
		if(ptcBGM.count(i) == 0){
			Err(_T("BGMデータが足りません。"));
		};
		i++;
	}

	i = 0;
	while(i < Header.iSE){
		if(ptcSE.count(i) == 0){
			Err(_T("SE データが足りません。"));
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
//		使用しないオブジェクトの検索＆削除
//		しながら、Tickをカウント
//		且つ、メタデータ情報を収集
//--------------------------------------------------------------
//	●引数
//				無し
//	●返値
//				無し
//==============================================================
void	MusicFile::TickCount(void)
{
	map<size_t, FDSC*		>::iterator	itFDSC;		//FDS  wave table (career)
	map<size_t, FDSM*		>::iterator	itFDSM;		//FDS  wave table (modulator)
	map<size_t, VRC7*		>::iterator	itVRC7;		//VRC7 User Instrument
	map<size_t, N163*		>::iterator	itN163;		//N163 wave table
	map<size_t, Envelop*	>::iterator	itEnv;		//Envelop
	map<size_t, Sub*		>::iterator	itSub;		//Subroutine

				size_t		iBGM	= 0;
				size_t		iSE		= 0;

	//==============================
	//Tick Count & 最適化のための情報収集

	while(iBGM < Header.iBGM){
		BGM* _BGM = ptcBGM[iBGM];
		_COUT << _T("---- BGM(") << iBGM << _T(") ----") <<endl;
		_BGM->TickCount(this);				//カウンティングしながら、不要なコマンドが無いかチェック
		if(f_is_track_label == true){
			Header.Set_tlbl(_BGM->getLabel());
		}
		if(f_is_track_auth == true){
			Header.Set_taut(_BGM->getAuthor());
		}
		if(f_is_track_time == true){
			Header.Set_time(_BGM->getTime());
		}
		if(f_is_track_fade == true){
			Header.Set_fade(_BGM->getFade());
		}
		iBGM++;
	}

	while(iSE < Header.iSE){
		SE*	_SE = ptcSE[iSE];
		_COUT << _T("---- SE(") << iSE << _T(") ----") <<endl;
		_SE->TickCount(this);				//カウンティングしながら、不要なコマンドが無いかチェック
		if(f_is_track_label == true){
			Header.Set_tlbl(_SE->getLabel());
		}
		if(f_is_track_auth == true){
			Header.Set_taut(_SE->getAuthor());
		}
		if(f_is_track_time == true){
			Header.Set_time(_SE->getTime());
		}
		if(f_is_track_fade == true){
			Header.Set_fade(_SE->getFade());
		}
		iSE++;
	}


	//==============================
	//Metadata

	Header.Set_text();	//text
	Header.Set_auth();	//auth
	Header.Set_NEND();	//NEND


	//==============================
	//最適化

	//----------------------
	//不要なコマンドの削除
	if(cOptionSW->flag_OptSeq == true){		//コマンドの最適化が無効だったら、最適化しない。

		iBGM	= 0;
		while(iBGM < Header.iBGM){
			ptcBGM[iBGM]->clear_Optimize();
			iBGM++;
		}

		iSE		= 0;
		while(iSE < Header.iSE){
			ptcSE[iSE]->clear_Optimize();
			iSE++;
		}

		if(!ptcSub.empty()){
			itSub = ptcSub.begin();
			while(itSub != ptcSub.end()){
				itSub->second->clear_Optimize();
				itSub++;
			}
		}
	}

	//----------------------
	//使っていない定義の削除
	if(cOptionSW->flag_OptObj == true){		//定義の最適化が無効だったら、最適化しない。

		//エンベロープ
		if(!ptcEnv.empty()){
			itEnv = ptcEnv.begin();
			while(itEnv != ptcEnv.end()){
				itEnv->second->clear_Optimize();
				itEnv++;
			}
		}

		//FDSC
		if(!ptcFDSC.empty()){
			itFDSC = ptcFDSC.begin();
			while(itFDSC != ptcFDSC.end()){
				itFDSC->second->clear_Optimize();
				itFDSC++;
			}
		}

		//FDSM
		if(!ptcFDSM.empty()){
			itFDSM = ptcFDSM.begin();
			while(itFDSM != ptcFDSM.end()){
				itFDSM->second->clear_Optimize();
				itFDSM++;
			}
		}

		//VRC7
		if(!ptcVRC7.empty()){
			itVRC7 = ptcVRC7.begin();
			while(itVRC7 != ptcVRC7.end()){
				itVRC7->second->clear_Optimize();
				itVRC7++;
			}
		}

		//N163
		if(!ptcN163.empty()){
			itN163 = ptcN163.begin();
			while(itN163 != ptcN163.end()){
				itN163->second->clear_Optimize();
				itN163++;
			}
		}
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
size_t	MusicFile::SetDPCMOffset(size_t iMusSize)
{
				size_t	i;
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
//		アドレス情報を決定する。
//--------------------------------------------------------------
//	●引数
//				無し
//	●返値
//				無し
//==============================================================
void	MusicFile::Fix_Address(void)
{
	map<size_t,Sub*	>::iterator	itSub;

	size_t	iBGM	= 0;
	size_t	iSE		= 0;


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

//==============================================================
//		ＢＩＮの読み込み
//--------------------------------------------------------------
//	●引数
//		string* 		_str		コード生成先
//		NSF_Header*		nsf_hed		NSFヘッダーの読み込み先
//	●返値
//		size_t						読み込んだ.binファイル（NSFヘッダー分を除く）
//==============================================================
size_t	MusicFile::read_bin(string* _str, NSF_Header* nsf_hed)
{
	size_t		bin_size;			//.binファイルのサイズ
	size_t		code_size;			//うち、ドライバー（コード）のサイズ
	FileInput*	_romcode	= new FileInput();	

	//----------------------
	//ファイルを開く
	_romcode->fileopen(Header.romcode.c_str(), &(cOptionSW->m_pass_code));
	if(_romcode->isError() == true){
		f_error = true;	//_romcodeを new しっぱなしなので、ここでは throw しない。

	} else {

		char*	nsf_code	= new char[0xC000];			//ここに.binファイルを読み込む

		//----------------------
		//NSF用コードの転送
		bin_size = _romcode->GetSize();
		code_size = bin_size - sizeof(NSF_Header);
		_romcode->read((char *)nsf_hed, sizeof(NSF_Header));
		_romcode->read(nsf_code, code_size);
		_romcode->close();

		//----------------------
		//NSFヘッダーの更新
		if(Header.title.size() < 32){
			Header.title.resize(32);
		}
		memcpy(&nsf_hed->Title, Header.title.c_str(), 32);

		if(Header.composer.size() < 32){
			Header.composer.resize(32);
		}
		memcpy(&nsf_hed->Composer, Header.composer.c_str(), 32);

		if(Header.copyright.size() < 32){
			Header.copyright.resize(32);
		}
		memcpy(&nsf_hed->Copyright, Header.copyright.c_str(), 32);

		nsf_hed->Version		= (unsigned char)(cOptionSW->iNSF_version);
		nsf_hed->MusicNumber	= (unsigned char)((Header.iBGM + Header.iSE) & 0xFF);
		if(Header.iExternal != -1){
			nsf_hed->External	= (unsigned char)Header.iExternal;
		}

		//----------------------
		//Exit
		_str->append(nsf_code, code_size);

		delete[]	nsf_code;

	}
	//----------------------
	//Exit
	delete		_romcode;

	return(code_size);

}

//==============================================================
//		シーケンスデータの作成
//--------------------------------------------------------------
//	●引数
//		string* _str		コード生成先
//		size_t	code_size	*.binのサイズ（NSFヘッダー分を除く）
//		size_t	ptOffset	ROMのオフセットアドレス
//								bank非対応の場合は、0x8000
//								bank　対応の場合は、0x0000
//	●返値
//		size_t				シーケンスデータのサイズ
//	●処理
//		NSF、NSFe用の曲データ構造を作る。
//		Addr	Type	Contents
//		-------+-------+--------------------------
//		0000	BYTE	BGM データ数
//		0001	BYTE	SE  データ数
//		0002	WORD	nsd_dpcm構造体のアドレス
//		0004	WORD[]	BGM, SEのアドレス、
//		xxxx	----	BGM, SEのシーケンスデータ
//==============================================================
size_t	MusicFile::make_mus(string* _str, size_t code_size, size_t ptOffset)
{
	unsigned	int			i		= 2;
				size_t		iBGM	= 0;
				size_t		iSE		= 0;
	unsigned	short*		pt;

				size_t		_size	= 4 + (Header.iBGM + Header.iSE)*2;
				size_t		size0	= _str->size();		//初期サイズ

	//----------------------
	//バイナリをクリア。
	code.clear();
	//領域の確保
	code.resize(_size);

	//曲アドレス情報（NSF, NSEe用）の作成
	pt = (unsigned short*)code.c_str();

	code[0] = (char)(Header.iBGM & 0xFF);
	code[1] = (char)(Header.iSE  & 0xFF);

	if(Header.bank == false){

		if(cDPCMinfo != NULL){
			pt[1]	= (unsigned short)(ptOffset + code_size + _size + cDPCMinfo->getOffset());	//ΔPCM info のアドレス
		} else {
			pt[1]	= 0;
		}

		while(iBGM < Header.iBGM){
			pt[i] = (unsigned short)(ptOffset + code_size + _size + ptcBGM[iBGM]->getOffset());
			i++;
			iBGM++;
		}
		while(iSE < Header.iSE){
			pt[i] = (unsigned short)(ptOffset + code_size + _size + ptcSE[iSE]->getOffset());
			i++;
			iSE++;
		}

	} else {

		if(cDPCMinfo != NULL){
			pt[1] = (unsigned short)(ptOffset + _size + cDPCMinfo->getOffset());	//ΔPCM info のアドレス
		} else {
			pt[1]	= 0;
		}

		while(iBGM < Header.iBGM){
			pt[i] = (unsigned short)(ptOffset + _size + ptcBGM[iBGM]->getOffset());
			i++;
			iBGM++;
		}
		while(iSE < Header.iSE){
			pt[i] = (unsigned short)(ptOffset + _size + ptcSE[iSE]->getOffset());
			i++;
			iSE++;
		}

	}

	getCode(_str);

	return(_str->size() - size0);

}

//==============================================================
//		⊿ＰＣＭの作成
//--------------------------------------------------------------
//	●引数
//		string* _str		コード生成先
//	●返値
//		size_t				⊿PCMデータのサイズ
//	●処理
//
//==============================================================
size_t	MusicFile::make_dpcm(string* _str)
{
	//ΔPCMの作成
	_str->append(dpcm_code);

	return(dpcm_code.size());
}

//==============================================================
//		ＮＳＦの作成
//--------------------------------------------------------------
//	●引数
//		NSF_Header*		NSF_Hed		ヘッダー部
//		string*			NSF_Data	データ部
//	●返値
//		size_t						データ部のサイズ
//	●処理
//
//--------------------------------------------------------------
size_t	MusicFile::make_bin(NSF_Header* NSF_Hed, string* NSF_Data)
{

	unsigned	int		i,j;

				size_t	iSizeLimit;		//サイズチェック用

				size_t	code_size;
				size_t	mus_size;
				size_t	pcm_size;
				size_t	NSF_size;
	unsigned	char	code_bank = 3;	//バンク対応.binのバンク数
	unsigned	char	mus_bank;
	unsigned	char	pcm_bank;

				char	chk_Bank;		//NSFヘッダーのバンクチェック用

				bool	flag_Optimize	= cOptionSW->flag_Optimize;

	//----------------------
	//BINファイルの読みこみ
	code_size = read_bin(NSF_Data, NSF_Hed);
	if(isError() == true){
		//BINファイルの読み込みに失敗していたら終了する
		throw EXIT_FAILURE;

	} else {

		//----------------------
		//Binが、バンク対応か？
		//chk_Bank == 0	・・・　非対応bin
		//chk_Bank != 0	・・・　　対応bin
		chk_Bank  = NSF_Hed->Bank[0];
		chk_Bank |= NSF_Hed->Bank[1];
		chk_Bank |= NSF_Hed->Bank[2];
		chk_Bank |= NSF_Hed->Bank[3];
		chk_Bank |= NSF_Hed->Bank[4];
		chk_Bank |= NSF_Hed->Bank[5];
		chk_Bank |= NSF_Hed->Bank[6];
		chk_Bank |= NSF_Hed->Bank[7];

		//----------------------
		//シーケンスの作成
		if(chk_Bank == 0){

			//------------------------------
			//Bank 非対応bin
			if(Header.bank == true){
				Err(_T("指定の.binファイルは、⊿PCMのバンクに対応していません。\n⊿PCMのバンクに対応した.binファイルを指定してください。"));
			}

			//サイズの上限
			iSizeLimit = Header.offsetPCM - 0x8000;

			//シーケンスのバイナリを生成
			mus_size = make_mus(NSF_Data, code_size, 0x8000);
			mus_size += code_size;						//.bin と 曲オブジェクトを足し算

			//出力
			_COUT << _T("[CODE & MUSIC]") << endl;
		} else {

			//------------------------------
			//Bank対応bin？
			if(Header.bank == false){
				Err(_T("指定の.binファイルは、⊿PCMのバンクに対応しています。\n#Bankコマンドを指定してください。"));
			}
			if(cOptionSW->iNSF_version >=2){
				NSF_Hed->Flags |= nsf_flag_IRQ_support;
			}

			//サイズの上限
			iSizeLimit = 0x10000;

			//シーケンスのバイナリを生成
			mus_size = make_mus(NSF_Data, code_size, 0x0000);

			//出力
			_COUT << _T("[CODE]") << endl;
			_COUT << _T("  Bank = 3") << endl;
			_COUT << _T("  Size = 12288 [Byte]") << endl;

			_COUT << _T("[MUSIC]") << endl;
		}

		//バンク数の計算
		mus_bank = (unsigned char)(mus_size >> 12);
		if((mus_size & 0x0FFF) != 0){
			mus_bank++;
		}

		_COUT << _T("  Bank = ") << (unsigned int)mus_bank << endl;
		_COUT << _T("  Size = ") << (unsigned int)mus_size << _T(" [Byte] / ") << iSizeLimit << _T(" [Byte]") << endl;

		//サイズチェック
		if(mus_size > iSizeLimit){
			Err(_T("コード・シーケンスのサイズが許容値を越えました。"));
		}

		//----------------------
		//GAPの生成
		if(cDPCMinfo != NULL){
			if(chk_Bank == 0){
				//0でpaddingする。
				if(flag_Optimize == true){
					//シーケンスデータのバンク領域
					//⊿PCMのバンク内の開始オフセットまで
					NSF_Data->resize((size_t)(mus_bank<<12) + (Header.offsetPCM & 0x0FFF));		
				} else {
					//⊿PCMの開始位置まで
					NSF_Data->resize(Header.offsetPCM - 0x8000);
				}
			} else {
				//シーケンスデータのバンク領域
				//⊿PCMの開始位置まで
				NSF_Data->resize((size_t)((mus_bank + code_bank)<<12) + (Header.offsetPCM - 0xC000));
			}
		}

		//----------------------
		//⊿PCM
		_COUT << _T("[DPCM]") << endl;

		pcm_size = make_dpcm(NSF_Data);
		pcm_bank = (unsigned char)((pcm_size + (Header.offsetPCM & 0x0FFF)) >> 12);
		if((pcm_size & 0x0FFF) != 0){
			pcm_bank++;
		}

		//⊿PCMサイズチェック
		_COUT << _T("  Bank = ") << (size_t)pcm_bank << endl;
		_COUT << _T("  Size = ") << (size_t)pcm_size << _T(" [Byte]");
		if(chk_Bank == 0){
			_COUT << _T(" / ") << 0x10000 - Header.offsetPCM << _T(" [Byte]") << endl;

			if(	(Header.offsetPCM + pcm_size) > 0x10000	){
				Err(_T("⊿PCMのサイズが許容値を越えました。"));
			}
		} else {
			_COUT << endl;
			i = mus_bank + pcm_bank + code_bank;

			if(i > 255){
				Err(_T("バンク数の合計が255を越えました。"));
			}
		}

		//⊿PCMが有る && バンク構成の最適化が有効 && bank非対応の.binを使う場合、
		//NSFヘッダーに、バンク情報を書き込む
		if((chk_Bank == 0) && (cDPCMinfo != NULL) && (flag_Optimize == true)){
			i = 0;
			while(i < mus_bank){
				NSF_Hed->Bank[i] = (unsigned char)i;
				i++;
			}
			while(i < ((Header.offsetPCM - 0x8000)>>12)){
				NSF_Hed->Bank[i] = 0;
				i++;
			}
			j = 0;
			while(i < 8){
				if(j < pcm_bank){
					NSF_Hed->Bank[i] = mus_bank + (unsigned char)j;
				} else {
					NSF_Hed->Bank[i] = 0;
				}
				i++;
				j++;
			}
		}

		//----------------------
		//GAPの生成
		if(chk_Bank == 0){
			NSF_size = NSF_Data->size();
			if((cDPCMinfo != NULL) && (flag_Optimize == true)){
				//バンク内を0でpaddingする。
				if((NSF_size & 0x0FFF) != 0){
					NSF_size = (NSF_size & 0xF000) + 0x1000;
					NSF_Data->resize(NSF_size);
				}
			}
		} else {
			//バンク内を0でpaddingする。
			NSF_size = (size_t)((mus_bank + pcm_bank + code_bank)<<12);
			//BANK対応の.binを使う場合で、且つ、32kByte未満の場合、32kByteにする。
			if(NSF_size < 0x8000){
				NSF_size = 0x8000;
			}
			NSF_Data->resize(NSF_size);
		}

		//----------------------
		//NSF2の場合、サイズをヘッダーに書く
		if(cOptionSW->iNSF_version >=2){
			NSF_Hed->szNSF_Data[0] = (char)( NSF_size      & 0xFF);
			NSF_Hed->szNSF_Data[1] = (char)((NSF_size>> 8) & 0xFF);
			NSF_Hed->szNSF_Data[2] = (char)((NSF_size>>16) & 0xFF);
		}
	}

	//----------------------
	//Exit
	return(NSF_size);

}

//==============================================================
//		ＮＳＦ形式への保存
//--------------------------------------------------------------
//	●引数
//		const	char*	strFileName		ファイル名
//	●返値
//				無し
//==============================================================
void	MusicFile::saveNSF(const char*	strFileName)
{
	NSF_Header*			NSF_Hed			= new NSF_Header;
				string	NSF_Data;
				size_t	NSF_size;

				string	Meta_data;
				size_t	Meta_size;

	try{

		_COUT << _T("----------------------------------------") << endl;
		_COUT << _T("*NSF build process") << endl;

		//==============================
		//----------------------
		//クリア
		NSF_Data.clear();
		Meta_data.clear();

		//----------------------
		//NSFの、ヘッダーとデータ部の作成
		NSF_size = make_bin(NSF_Hed, &NSF_Data);

		//==============================
		//Meta Data
		Meta_size = Header.getData(&Meta_data);

		//NENDだけであれば、metadataの埋め込みは不要
		if(Meta_size <= 8){
			Meta_size = 0;
		}

		//サイズ表示
		_COUT << _T("[Meta Data]") << endl;
		_COUT << _T("  Size = ") << (unsigned int)Meta_size << _T(" [Byte]") << endl;

		if(cOptionSW->iNSF_version >=2){
			//Meta Dataがある場合
			if(Meta_size > 0){
				NSF_Hed->Flags |= nsf_flag_MetaData;
			}
		}

		//==============================
		//NSF書き込み
		//----------------------
		//Open File
		fileopen(strFileName);
		if(isError() == true){
			throw EXIT_FAILURE;
		} else {
			//----------------------
			//Write File
			write((char *)NSF_Hed, sizeof(NSF_Header));			//NSFヘッダーの書き込み
			write(NSF_Data.c_str(), NSF_size);					//データの書き込み
			if((cOptionSW->iNSF_version>=2) && (Meta_size > 0)){
				write(Meta_data.c_str(), Meta_size);			//Meta dataの書き込み
			}

			//----------------------
			//Close file
			close();
		}

	} catch (int no){
		_COUT	<<	_T("Error!:") << no << _T(":NSFファイルを生成中にエラーが発生しました。") << endl;
	}

		//==============================
		//Exit
		delete[]	NSF_Hed;
}

//==============================================================
//		ＮＳＦｅ形式への保存
//--------------------------------------------------------------
//	●引数
//		const	char*	strFileName		ファイル名
//	●返値
//				無し
//==============================================================
void	MusicFile::saveNSFe(const char*	strFileName)
{

	const		char	NSFE_Head[4]	= {0x4E, 0x53, 0x46, 0x45};		//NSFE

	NSF_Header*			NSF_Hed			= new NSF_Header;
				string	NSF_Data;

				string	Meta_data;
				size_t	Meta_size;

	try{

		_COUT << _T("----------------------------------------") << endl;
		_COUT << _T("*NSFe build process") << endl;

		//==============================
		//----------------------
		//クリア
		NSF_Data.clear();
		Meta_data.clear();

		//----------------------
		//NSFの、ヘッダーとデータ部の作成
		make_bin(NSF_Hed, &NSF_Data);		//バンク構成の最適化はここでやっている。

		//==============================
		//メタデータの生成
		//----------------------
		//DATA
		Header.Set_DATA(&NSF_Data);

		//----------------------
		//NSFe用のmetadeta(INFO, NSF2, BANK)
		Header.Set_NSFe_footer(NSF_Hed);

		//----------------------
		//バイナリに変換
		Meta_size = Header.getData(&Meta_data);

		//サイズ表示
		_COUT << _T("[Meta Data]") << endl;
		_COUT << _T("  Size = ") << (unsigned int)Meta_size << _T(" [Byte]") << endl;

		//==============================
		//NSFE書き込み
		//----------------------
		//Open File
		fileopen(strFileName);
		if(isError() == true){
			throw EXIT_FAILURE;
		} else {
			//----------------------
			//Write File
			write(NSFE_Head, sizeof(NSFE_Head));			//NSFヘッダーの書き込み
			write(Meta_data.c_str(), Meta_size);			//Meta dataの書き込み

			//----------------------
			//Close file
			close();
		}

	} catch (int no){
		_COUT	<<	_T("Error!:") << no << _T(":NSFeファイルを生成中にエラーが発生しました。") << endl;
	}

	//==============================
	//Exit
	delete[]	NSF_Hed;

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
	size_t	iBGM	= 0;
	size_t	iSE		= 0;

	try{

		//----------------------
		//File open
		fileopen(strFileName);
		if(isError() == true){
			throw EXIT_FAILURE;
		} else {

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

	} catch (int no){
		_COUT	<<	_T("Error!:") << no << _T(":Sファイルを生成中にエラーが発生しました。") << endl;
	}
}

//==============================================================
//		エラー処理
//--------------------------------------------------------------
//	●引数
//		const	_CHAR	msg[]	エラーメッセージ
//	●返値
//				無し
//==============================================================
void	MusicFile::Err(const _CHAR msg[])
{
	f_error = true;

	if(cOptionSW->fErr == true){
		_CERR << _T("[ ERROR ] : ") << msg << endl;
	} else {
		_COUT << _T("[ ERROR ] : ") << msg << endl;
	}

	//異常終了
	nsc_exit(EXIT_FAILURE);
}

//==============================================================
//		ワーニング処理
//--------------------------------------------------------------
//	●引数
//		const	_CHAR	msg[]	ワーニングメッセージ
//	●返値
//				無し
//==============================================================
void	MusicFile::Warning(const _CHAR msg[])
{
	//現在のファイル名と、行数を表示
	if(cOptionSW->fErr == true){
		_CERR << _T("[WARNING] : ") << msg << endl;
	} else {
		_COUT << _T("[WARNING] : ") << msg << endl;
	}
}
