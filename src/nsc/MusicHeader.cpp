/*******************************************************************************

			NES Sound Driver & Library	(NSD.lib)	MML Compiler

	Copyright (c) 2012 A.Watanabe (S.W.), All rights reserved.
	 For conditions of distribution and use, see copyright notice in "nsc.cpp".

*******************************************************************************/

#include "StdAfx.h"
#include "MusicHeader.h"

/****************************************************************/
/*					グローバル変数（クラスだけど・・・）		*/
/****************************************************************/
extern	OPSW*			cOptionSW;	//オプション情報へのポインタ変数

//==============================================================
//		コンストラクタ
//--------------------------------------------------------------
//	●引数
//		const		_CHAR	_strName[]	オブジェクト名
//		string				_code
//	●返値
//				無し
//==============================================================
MusicHeader::MusicHeader(string _code) :
	MetaItem("NSFE"),
	bank(false),
	op_code(false),
	iBGM(1),
	iSE(0),
	offsetPCM(0x10000),
	Label("_nsd_"),
	segmentSEQ("RODATA"),
	segmentPCM("PCMDATA"),
	iExternal(-1),
	title(""),
	copyright(""),
	composer(""),
	maker(""),
	text(""),
	f_track_time(false),
	f_track_fade(false),
	f_track_label(false),
	f_track_auth(false),
	meta_DATA(NULL),		//2.2	DATA	NSFe MUST
	meta_NEND(NULL),		//2.3	NEND	NSFe MUST
	meta_BANK(NULL),		//2.4	BANK	NSFe optional / NSF MUSTNOT
	meta_NSF2(NULL),		//2.6	NSF2	NSFe optional /  NSF MUSTNOT
	meta_VRC7(NULL),		//2.7	VRC7
	meta_plst(NULL),		//2.8	plst
	meta_psfx(NULL),		//2.9	psfx
//	meta_time(NULL),		//2.10	time
//	meta_fade(NULL),		//2.11	fade
//	meta_tlbl(NULL),		//2.12	tlbl
//	meta_taut(NULL),		//2.13	taut
	meta_auth(NULL),		//2.14	auth
	meta_text(NULL),		//2.15	text
	meta_mixe(NULL)			//2.16	mixe
{
	if(_code.empty()){
		op_code = false;
		romcode = "nsd.bin";
	} else {
		op_code = true;
		romcode = _code;
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
MusicHeader::~MusicHeader(void)
{
}

//==============================================================
//	auth
//--------------------------------------------------------------
void	MusicHeader::Set_Title(MMLfile* MML)
{
	title = MML->GetString();
}

//--------------------------------------------------------------
void	MusicHeader::Set_Copyright(MMLfile* MML)
{
	copyright = MML->GetString();
}

//--------------------------------------------------------------
void	MusicHeader::Set_Composer(MMLfile* MML)
{
	composer = MML->GetString();
}

//--------------------------------------------------------------
void	MusicHeader::Set_Maker(MMLfile* MML)
{
	maker = MML->GetString();
}

//--------------------------------------------------------------
void	MusicHeader::Text_Append(MMLfile* MML)
{
	if(text.size() > 0){
		text.push_back(0x0D);
		text.push_back(0x0A);
	}
	text.append(MML->GetString());
}


//==============================================================
//	Metadata
//--------------------------------------------------------------
void	MusicHeader::Set_NSFe_footer(NSF_Header* _nsf_hed)
{
	char	chk_Bank;		//NSFヘッダーのバンクチェック用

	//----------------------
	//BANK
	chk_Bank  = _nsf_hed->Bank[0];
	chk_Bank |= _nsf_hed->Bank[1];
	chk_Bank |= _nsf_hed->Bank[2];
	chk_Bank |= _nsf_hed->Bank[3];
	chk_Bank |= _nsf_hed->Bank[4];
	chk_Bank |= _nsf_hed->Bank[5];
	chk_Bank |= _nsf_hed->Bank[6];
	chk_Bank |= _nsf_hed->Bank[7];
	if(chk_Bank != 0){
		meta_BANK = new Meta_BANK(_nsf_hed);
		setItem_front(meta_BANK);
	}

	//----------------------
	//NSF2
	if(cOptionSW->iNSF_version >=2){
		meta_NSF2 = new Meta_NSF2(_nsf_hed);
		setItem_front(meta_NSF2);
	}

	//----------------------
	//INFO
	meta_INFO = new Meta_INFO(_nsf_hed);
	setItem_front(meta_INFO);
}

//--------------------------------------------------------------
void	MusicHeader::Set_DATA(string* data)
{
	meta_DATA = new Meta_DATA(data);
	setItem_front(meta_DATA);
}

//--------------------------------------------------------------
void	MusicHeader::Set_NEND()
{
	meta_NEND = new Meta_NEND();
	setItem(meta_NEND);
}

//--------------------------------------------------------------
void	MusicHeader::Ser_VRC7(MMLfile* MML)
{
	int	_vrc7	= MML->GetInt();

	if(meta_VRC7 != NULL){
		MML->Warning(_T("#VRC7が重複しています。"));
	}

	if((_vrc7 < 0)||(_vrc7 > 1)){
		MML->Err(_T("#VRC7は、0か1で指定してください。"));
	}

	meta_VRC7 = new Meta_VRC7((char)_vrc7);
	setItem(meta_VRC7);
}

//--------------------------------------------------------------
void	MusicHeader::Set_plst(MMLfile* MML)
{
	//	定数定義
	enum	Command_ID_mixe {
		plst_Num,
		plst_Commma
	};

	//	これらは、MML構文で使えるコマンド。
	const	static	Command_Info	Command[] = {
		{	"%",		plst_Num	},
		{	"$",		plst_Num	},
		{	"0",		plst_Num	},
		{	"1",		plst_Num	},
		{	"2",		plst_Num	},
		{	"3",		plst_Num	},
		{	"4",		plst_Num	},
		{	"5",		plst_Num	},
		{	"6",		plst_Num	},
		{	"7",		plst_Num	},
		{	"8",		plst_Num	},
		{	"9",		plst_Num	},
		{	",",		plst_Commma	}
	};

				size_t	i;
	unsigned	char	cData;

	//------------------------------
	//コンパイル
	if(meta_plst != NULL){
		MML->Warning(_T("#plstが重複しています。"));
	}

	meta_plst = new Meta_plst();
	setItem(meta_plst);

	// { の検索
	while(MML->cRead() != '{'){
		if(MML->eof()){
			MML->Err(_T("ブロックの開始を示す{が見つかりません。"));
		}
	}

	// } が来るまで、記述ブロック内をコンパイルする。
	while((cData = MML->GetChar()) != '}'){
		
		// } が来る前に、[EOF]が来たらエラー
		if( MML->eof() ){
			MML->Err(_T("ブロックの終端を示す`}'がありません。"));
		}

		//１つ戻る
		MML->Back();

		//各コマンド毎の処理
		switch(MML->GetCommandID(Command, sizeof(Command)/sizeof(Command_Info))){
			case(plst_Num):
				MML->Back();
				i = MML->GetInt();
				if( (i<0) || (i>=iBGM)){
					MML->Err(_T("#plstは0 <= n < #BGMコマンドで指定した数の範囲で指定して下さい。"));
				}
				meta_plst->push_back((char)(i & 0xFF));
				break;
			case(plst_Commma):
				break;
			//unknown command
			default:
				MML->Err(_T("unknown command"));
				break;
		}
	}
}

//--------------------------------------------------------------
void	MusicHeader::Set_psfx(MMLfile* MML)
{
	//	定数定義
	enum	Command_ID_mixe {
		psfx_Num,
		psfx_Commma
	};

	//	これらは、MML構文で使えるコマンド。
	const	static	Command_Info	Command[] = {
		{	"%",		psfx_Num	},
		{	"$",		psfx_Num	},
		{	"0",		psfx_Num	},
		{	"1",		psfx_Num	},
		{	"2",		psfx_Num	},
		{	"3",		psfx_Num	},
		{	"4",		psfx_Num	},
		{	"5",		psfx_Num	},
		{	"6",		psfx_Num	},
		{	"7",		psfx_Num	},
		{	"8",		psfx_Num	},
		{	"9",		psfx_Num	},
		{	",",		psfx_Commma	}
	};

				size_t	i;
	unsigned	char	cData;

	//------------------------------
	//コンパイル
	if(meta_psfx != NULL){
		MML->Warning(_T("#psfxが重複しています。"));
	}

	meta_psfx = new Meta_psfx();
	setItem(meta_psfx);

	// { の検索
	while(MML->cRead() != '{'){
		if(MML->eof()){
			MML->Err(_T("ブロックの開始を示す{が見つかりません。"));
		}
	}

	// } が来るまで、記述ブロック内をコンパイルする。
	while((cData = MML->GetChar()) != '}'){
		
		// } が来る前に、[EOF]が来たらエラー
		if( MML->eof() ){
			MML->Err(_T("ブロックの終端を示す`}'がありません。"));
		}

		//１つ戻る
		MML->Back();

		//各コマンド毎の処理
		switch(MML->GetCommandID(Command, sizeof(Command)/sizeof(Command_Info))){
			case(psfx_Num):
				MML->Back();
				i = MML->GetInt();
				if( (i<0) || (i>=iSE)){
					MML->Err(_T("#psfxは0 <= n < #SEコマンドで指定した数の範囲で指定して下さい。"));
				}
				meta_psfx->push_back((char)((i+iBGM) & 0xFF));
				break;
			case(psfx_Commma):
				break;
			//unknown command
			default:
				MML->Err(_T("unknown command"));
				break;
		}
	}
}

//--------------------------------------------------------------
void	MusicHeader::Set_time()
{

}

//--------------------------------------------------------------
void	MusicHeader::Set_fade()
{

}

//--------------------------------------------------------------
void	MusicHeader::Set_tlbl()
{

}

//--------------------------------------------------------------
void	MusicHeader::Set_taut()
{

}

//--------------------------------------------------------------
void	MusicHeader::Set_auth()
{
	//曲名、作曲者、著作権者が32Byteを超える場合、 又は、 Textがある場合、 又は、 NSFe形式で保存する場合、authを作る。
	if((title.size() > 32) || (composer.size() > 32) || (copyright.size() > 32) || (maker.size() > 0) || (cOptionSW->saveNSFe == true)){
		meta_auth = new Meta_auth(this);
		setItem(meta_auth);
	}
}

//--------------------------------------------------------------
void	MusicHeader::Set_text()
{
	if(text.size() > 0){
		meta_text = new Meta_text(&text);
		setItem(meta_text);
	}
}

//--------------------------------------------------------------
void	MusicHeader::Set_mixe(MMLfile* MML)
{
	
//	定数定義
enum	Command_ID_mixe {
	mixe_APU1,
	mixe_APU2,
	mixe_VRC6,
	mixe_VRC7,
	mixe_FDS,
	mixe_MMC5,
	mixe_N163,
	mixe_SN5B,
	mixe_Num,
	mixe_Commma
};

//	これらは、MML構文で使えるコマンド。
const	static	Command_Info	Command[] = {
		{	"APU1",		mixe_APU1	},
		{	"APU2",		mixe_APU2	},
		{	"VRC6",		mixe_VRC6	},
		{	"VRC7",		mixe_VRC7	},
		{	"FDS",		mixe_FDS	},
		{	"MMC5",		mixe_MMC5	},
		{	"N160",		mixe_N163	},
		{	"N163",		mixe_N163	},
		{	"N16x",		mixe_N163	},
		{	"SN5B",		mixe_SN5B	},
		{	"PSG",		mixe_SN5B	},
		{	"%",		mixe_Num	},
		{	"$",		mixe_Num	},
		{	"-",		mixe_Num	},
		{	"0",		mixe_Num	},
		{	"1",		mixe_Num	},
		{	"2",		mixe_Num	},
		{	"3",		mixe_Num	},
		{	"4",		mixe_Num	},
		{	"5",		mixe_Num	},
		{	"6",		mixe_Num	},
		{	"7",		mixe_Num	},
		{	"8",		mixe_Num	},
		{	"9",		mixe_Num	},
		{	"=",		mixe_Commma	},
		{	",",		mixe_Commma	}
	};

				int		i;
	unsigned	char	cData;
	unsigned	char	id = 0;

	//------------------------------
	//コンパイル

	if(meta_mixe != NULL){
		MML->Warning(_T("#mixeが重複しています。"));
	} else {
		meta_mixe = new Meta_mixe();
		setItem(meta_mixe);
	}

	// { の検索
	while(MML->cRead() != '{'){
		if(MML->eof()){
			MML->Err(_T("ブロックの開始を示す{が見つかりません。"));
		}
	}

	// } が来るまで、記述ブロック内をコンパイルする。
	while((cData = MML->GetChar()) != '}'){
		
		// } が来る前に、[EOF]が来たらエラー
		if( MML->eof() ){
			MML->Err(_T("ブロックの終端を示す`}'がありません。"));
		}

		//１つ戻る
		MML->Back();

		//各コマンド毎の処理
		switch(MML->GetCommandID(Command, sizeof(Command)/sizeof(Command_Info))){
			case(mixe_APU1):
				id = mixe_APU1;
				break;
			case(mixe_APU2):
				id = mixe_APU2;
				break;
			case(mixe_VRC6):
				id = mixe_VRC6;
				break;
			case(mixe_VRC7):
				id = mixe_VRC7;
				break;
			case(mixe_FDS):
				id = mixe_FDS;
				break;
			case(mixe_MMC5):
				id = mixe_MMC5;
				break;
			case(mixe_N163):
				id = mixe_N163;
				break;
			case(mixe_SN5B):
				id = mixe_SN5B;
				break;
			case(mixe_Num):
				MML->Back();
				i = MML->GetInt();
				meta_mixe->append(id, i, MML);
				break;
			case(mixe_Commma):
				break;
			//unknown command
			default:
				MML->Err(_T("unknown command"));
				break;
		}
	}
}

//==============================================================
void	MusicHeader::Set_SegmentSEQ(MMLfile* MML)
{
	segmentSEQ = MML->GetString();
}

//==============================================================
void	MusicHeader::Set_SegmentPCM(MMLfile* MML)
{
	segmentPCM = MML->GetString();
}

//==============================================================
void	MusicHeader::Set_Label(MMLfile* MML)
{
	Label = MML->GetString();
}

//==============================================================
void	MusicHeader::Set_OffsetPCM(MMLfile* MML)
{
	offsetPCM = MML->GetInt();

	if(bank==true){
		MML->Warning(_T("#Bank指定時は、#offsetPCMは無効です。"));
	} else {
		if((offsetPCM < 0xC000) || (offsetPCM > 0x10000)){
			MML->Err(_T("$C000 ～ $10000（⊿PCM未使用）の範囲で指定して下さい。"));
		}
		if((offsetPCM & 0x003F) != 0){
			MML->Warning(_T("⊿PCMの配置アドレスは64（$40）Byteでアライメントします。"));
			offsetPCM &= 0xFFC0;
			offsetPCM += 0x0040;
		}
	}
}

//==============================================================
void	MusicHeader::Set_RomCode(MMLfile* MML)
{
	if(op_code == true){
		MML->Warning(_T("オプションスイッチでリンクするコードが指定されているので、#codeコマンドは無視します。"));
		MML->GetString();
	} else {
		romcode = MML->GetString();
	}
}

//==============================================================
void	MusicHeader::Set_Number_BGM(MMLfile* MML)
{
	size_t	_n = MML->GetInt();

	if((_n > 255) || (_n < 0)){
		MML->Err(_T("#BGMは0～255の範囲で指定してください。"));
	}
	iBGM = _n;
}

//==============================================================
void	MusicHeader::Set_Number_SE(MMLfile* MML)
{
	size_t	_n = MML->GetInt();

	if((_n > 255) || (_n < 0)){
		MML->Err(_T("#SEは0～255の範囲で指定してください。"));
	}
	iSE = _n;
}

//==============================================================
void	MusicHeader::Set_External(MMLfile* MML)
{
	int	_n = MML->GetInt();

	if((_n > 0x3F) || (_n < 0)){
		MML->Err(_T("#Externalは$00～#3Fの範囲で指定してください。"));
	}
	iExternal = _n;
}

//==============================================================
void	MusicHeader::Set_Bank(void)
{
	bank		= true;
	offsetPCM	= 0xC000;
}
