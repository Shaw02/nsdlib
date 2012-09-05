#include "StdAfx.h"
#include "MusicTrack.h"

//==============================================================
//		コンストラクタ
//--------------------------------------------------------------
//	●引数
//		MMLfile*	MML
//	●返値
//					無し
//==============================================================
MusicTrack::MusicTrack(const char _strName[]):
	MusicItem(_strName),
	offset_now(0),				//
	offset_loop(0),				//無限ループ
	offset_repeat_a_s(0),		//リピートＡ
	offset_repeat_a_b(0),		//リピートＡ
	offset_repeat_b_s(0),		//リピートＢ
	offset_repeat_b_b(0),		//リピートＢ
	loop_flag(false)
{
	//調号
	KeySignature[0]	= 0;
	KeySignature[1]	= 0;
	KeySignature[2]	= 0;
	KeySignature[3]	= 0;
	KeySignature[4]	= 0;
	KeySignature[5]	= 0;
	KeySignature[6]	= 0;
}

//==============================================================
//		デストラクタ
//--------------------------------------------------------------
//	●引数
//				無し
//	●返値
//				無し
//==============================================================
MusicTrack::~MusicTrack(void)
{
}

//==============================================================
//		
//--------------------------------------------------------------
//	●引数
//				無し
//	●返値
//				無し
//==============================================================
void	MusicTrack::SetLoop()
{
	offset_loop = offset_now;
	loop_flag	= true;
}

//==============================================================
//		
//--------------------------------------------------------------
//	●引数
//		MMLfile*	MML		MMLファイルのオブジェクト
//	●返値
//				無し
//==============================================================
void	MusicTrack::SetRepeat_A_Start(MMLfile* MML)
{
	SetEvent(new mml_general(nsd_Repeat_A_Start, MML, "Repeat(A) Start"));
	offset_repeat_a_s = offset_now;	// ] コマンドでは、次のコマンドに戻る。
	offset_repeat_a_b = 0;
}

//==============================================================
//		
//--------------------------------------------------------------
//	●引数
//		MMLfile*	MML		MMLファイルのオブジェクト
//	●返値
//				無し
//==============================================================
void	MusicTrack::SetRepeat_A_Branch(MMLfile* MML)
{
	if(offset_repeat_a_s != 0){
		if(offset_repeat_a_b == 0){
			offset_repeat_a_b = offset_now + 1;	//引数の位置
			_old_repeatA_Branch = new mml_Address(nsd_Repeat_A_Branch, "Repeat(A) Branch");
			SetEvent(_old_repeatA_Branch);
		} else {
			MML->Err("リピート(A)内で : コマンドが重複しています。");
		}
	} else {
		MML->Err("リピート(A)の開始 [ コマンドがありません。");
	}
}

//==============================================================
//		
//--------------------------------------------------------------
//	●引数
//		MMLfile*	MML		MMLファイルのオブジェクト
//	●返値
//				無し
//==============================================================
void	MusicTrack::SetRepeat_A_End(MMLfile* MML)
{
	mml_Address*	_event;

	if(offset_repeat_a_s != 0){
		_event = new mml_Address(nsd_Repeat_A_End, "Repeat(A) End");
		_event->set_Address(offset_repeat_a_s - offset_now - 1);
		SetEvent(_event);
		//条件分岐があったら。
		if(offset_repeat_a_b != 0){
			_old_repeatA_Branch->set_Address(offset_now - offset_repeat_a_b);
		}
		offset_repeat_a_s = 0;
	} else {
		MML->Err("リピート(A)の開始 [ コマンドがありません。");
	}

}

//==============================================================
//		
//--------------------------------------------------------------
//	●引数
//		MMLfile*	MML		MMLファイルのオブジェクト
//	●返値
//				無し
//==============================================================
void	MusicTrack::SetRepeat_B_Start(MMLfile* MML)
{
	SetEvent(new mml_general(nsd_Repeat_B_Start, "Repeat(B) Start"));
	offset_repeat_b_s = offset_now;	// :| コマンドでは、次のコマンドに戻る。
	offset_repeat_b_b = 0;
}

//==============================================================
//		
//--------------------------------------------------------------
//	●引数
//		MMLfile*	MML		MMLファイルのオブジェクト
//	●返値
//				無し
//==============================================================
void	MusicTrack::SetRepeat_B_Branch(MMLfile* MML)
{
	if(offset_repeat_b_s != 0){
		if(offset_repeat_b_b == 0){
			offset_repeat_b_b = offset_now;	//コマンドがあった事を示す。
			SetEvent(new mml_general(nsd_Repeat_B_Branch, "Repeat(B) Branch"));
		} else {
			MML->Err("リピート(B)内で \\ コマンドが重複しています。");
		}
	} else {
		MML->Err("リピート(B)の開始 |: コマンドがありません。");
	}
}

//==============================================================
//		
//--------------------------------------------------------------
//	●引数
//		MMLfile*	MML		MMLファイルのオブジェクト
//	●返値
//				無し
//==============================================================
void	MusicTrack::SetRepeat_B_End(MMLfile* MML)
{
	mml_Address*	_event;

	if(offset_repeat_b_s != 0){
		if(offset_repeat_b_b != 0){
			if(offset_repeat_b_s < offset_repeat_a_s){
				MML->Err("リピート(A)の区間の途中です。");
			}
			_event = new mml_Address(nsd_Repeat_B_End, "Repeat(B) End");
			_event->set_Address(offset_repeat_b_s - offset_now - 1);
			SetEvent(_event);
		} else {
			MML->Err("リピート(B)内で \\ コマンドがありませんでした。必ず分岐点 \\ は指定してください。");
		}
	} else {
		MML->Err("リピート(B)の開始 |: コマンドがありません。");
	}
}

//==============================================================
//		
//--------------------------------------------------------------
//	●引数
//		MMLfile*	MML		MMLファイルのオブジェクト
//	●返値
//				無し
//==============================================================
void	MusicTrack::SetEnvelop(unsigned char _opcode, MMLfile* MML)
{
	mml_Address*		_event = new mml_Address(_opcode, "Envelop");
	unsigned	int		_no = MML->GetInt();

	if((_no<0) || (_no>65535)){
		MML->Err("エンベロープ番号は、0～65535の範囲で指定してください。");
	}
	_event->set_Address( _no & 0xFFFF );
	SetEvent(_event);
	ptcEnvelop.push_back(_event);
}

//==============================================================
//		
//--------------------------------------------------------------
//	●引数
//		MMLfile*	MML		MMLファイルのオブジェクト
//	●返値
//				無し
//==============================================================
void	MusicTrack::SetSubroutine(MMLfile* MML)
{
	mml_Address*		_event = new mml_Address(nsd_Call, "Subroutine");
	unsigned	int		_no = MML->GetInt();

	if((_no<0) || (_no>65535)){
		MML->Err("サブルーチン番号は、0～65535の範囲で指定してください。");
	}
	_event->set_Address( _no & 0xFFFF );
	SetEvent(_event);
	ptcSubroutine.push_back(_event);
}

//==============================================================
//		アドレス情報を決定する。
//--------------------------------------------------------------
//	●引数
//				無し
//	●返値
//				無し
//==============================================================
void	MusicTrack::Fix_Address(map<int, Sub*>* ptcSub, map<int, Envelop*>* ptcEnv)
{
	//----------------------
	//Local変数
	vector<	mml_Address*	>::iterator	itSub;
	vector<	mml_Address*	>::iterator	itEnv;
	unsigned	int	_no;
	unsigned	int	_sub_offset;
	unsigned	int	_com_offset;

	//----------------------
	//
	if(!ptcSubroutine.empty()){
		itSub = ptcSubroutine.begin();
		while(itSub != ptcSubroutine.end()){
			_no			= (*itSub)->get_Address();		//サブルーチンNo.の取得
			_com_offset	= (*itSub)->getOffset();
			if( ptcSub->count(_no) == 0){
				printf("サブルーチン %d 番が存在しません。",_no);
				exit(-1);
			}
			_sub_offset = (*ptcSub)[_no]->getOffset();	//指定サブルーチンが存在するオフセット
			(*itSub)->set_Address(_sub_offset - _com_offset - 1);
			itSub++;
		}
	}

	//
	if(!ptcEnvelop.empty()){
		itEnv = ptcEnvelop.begin();
		while(itEnv != ptcEnvelop.end()){
			_no			= (*itEnv)->get_Address();		//エンベロープNo.の取得
			_com_offset	= (*itEnv)->getOffset();
			if( ptcEnv->count(_no) == 0){
				printf("エンベロープ %d 番が存在しません。",_no);
				exit(-1);
			}
			_sub_offset = (*ptcEnv)[_no]->getOffset();	//指定エンベロープが存在するオフセット
			(*itEnv)->set_Address(_sub_offset - _com_offset - 1);
			itEnv++;
		}
	}
}
//==============================================================
//		音符のイベント作成
//--------------------------------------------------------------
//	●引数
//		MMLfile*	MML		MMLファイルのオブジェクト
//		int			note	ノート（0:C 1:D 2:E … 7:B）
//	●返値
//				無し
//==============================================================
void	MusicTrack::SetNote(MMLfile*	MML,int note)
{
	unsigned		char	cData;
	static	const	char	note_code[]={0,2,4,5,7,9,11};
					int		_key = note_code[note];
	unsigned		int		Length = -1;
	unsigned		int		GateTime = -1;
					bool	Slur = false;

	//臨時記号
	cData = MML->GetChar();
	//If Natural then skip
	if((cData != '=') && (cData != '*')){
		_key += KeySignature[note];
		while((cData == '#') || (cData == '+') || (cData == '-')){
			switch(cData){
				case('#'):
				case('+'):
					_key++;
					break;
				case('-'):
					_key--;
					break;
				default:
					break;
			}
			cData = MML->GetChar();	//次の文字
		}
		//臨時記号ではなくなったので、ポインタを戻す。
		MML->Back();
	}

	cData = MML->GetChar();
	if(((cData > '0') && (cData < '9')) || (cData == '%')){
		MML->Back();
		Length = MML->GetLength();
	} else {
		MML->Back();
	}

	cData = MML->GetChar();
	if(cData == ','){
		GateTime = MML->GetLength();
	} else if(cData == '&') {
		Slur = true;
	} else {
		MML->Back();
	}

	//指定できる範囲を超えた場合。
	while(_key < 0){
		_key += 12;
		SetEvent(new mml_general(nsd_Octave_Down_1, "One time octave down"));
	}

	while(_key >= 12){
		_key -= 12;
		SetEvent(new mml_general(nsd_Octave_Up_1, "One time octave up"));
	}


	//イベントオブジェクトの作成
	_old_note = new mml_note(_key, Length, GateTime, Slur, "Note");
	SetEvent(_old_note);
}

//==============================================================
//		休符のイベント作成
//--------------------------------------------------------------
//	●引数
//		MMLfile*	MML		MMLファイルのオブジェクト
//	●返値
//				無し
//==============================================================
void	MusicTrack::SetRest(MMLfile*	MML)
{
	unsigned		char	cData;
	unsigned		char	_code = 0x0F;
	unsigned		int		Length = -1;
	unsigned		int		GateTime = -1;
					bool	Slur = false;

	//休符のモード
	cData = MML->GetChar();
	switch(cData){
		case('-'):
			_code = 0x0D;
			break;
		case('#'):
		case('+'):
			_code = 0x0E;
			break;
		default:
			MML->Back();
			break;
	}

	//長さ
	cData = MML->GetChar();
	if(((cData > '0') && (cData < '9')) || (cData == '%')){
		MML->Back();
		Length = MML->GetLength();
	} else {
		MML->Back();
	}

	//ゲートタイム or タイ
	cData = MML->GetChar();
	if(cData == ','){
		GateTime = MML->GetLength();
	} else if(cData == '&') {
		Slur = true;
	} else {
		MML->Back();
	}

	_old_note = new mml_note(_code, Length, GateTime, Slur, "Rest");
	SetEvent(_old_note);
}

//==============================================================
//		タイのオブジェクト作成（休符で作る）
//--------------------------------------------------------------
//	●引数
//		MMLfile*	MML		MMLファイルのオブジェクト
//	●返値
//		無し
//==============================================================
void	MusicTrack::SetTai(MMLfile* MML)
{
	_old_note->SetTai();
	SetRest(MML);
}

//==============================================================
//		省略時の音調
//--------------------------------------------------------------
//	●引数
//		MMLfile*	MML		MMLファイルのオブジェクト
//	●返値
//		無し
//==============================================================
void	MusicTrack::SetLength(MMLfile* MML)
{
	unsigned	int	iLength = MML->GetLength();
	mml_general*	_event;

	switch(iLength){
		case(96):
			_event = new mml_general(nsd_Length_96, "Length 96");
			break;
		case(72):
			_event = new mml_general(nsd_Length_72, "Length 72");
			break;
		case(48):
			_event = new mml_general(nsd_Length_48, "Length 48");
			break;
		case(36):
			_event = new mml_general(nsd_Length_36, "Length 36");
			break;
		case(32):
			_event = new mml_general(nsd_Length_32, "Length 32");
			break;
		case(24):
			_event = new mml_general(nsd_Length_24, "Length 24");
			break;
		case(18):
			_event = new mml_general(nsd_Length_18, "Length 18");
			break;
		case(16):
			_event = new mml_general(nsd_Length_16, "Length 16");
			break;
		case(12):
			_event = new mml_general(nsd_Length_12, "Length 12");
			break;
		case(9):
			_event = new mml_general(nsd_Length_9,  "Length 9");
			break;
		case(8):
			_event = new mml_general(nsd_Length_8,  "Length 8");
			break;
		case(6):
			_event = new mml_general(nsd_Length_6,  "Length 6");
			break;
		case(4):
			_event = new mml_general(nsd_Length_4,  "Length 4");
			break;
		case(3):
			_event = new mml_general(nsd_Length_3,  "Length 3");
			break;
		case(2):
			_event = new mml_general(nsd_Length_2,  "Length 2");
			break;
		case(1):
			_event = new mml_general(nsd_Length_1,  "Length 1");
			break;
		default:
			_event = new mml_general(nsd_Length, (iLength & 0xFF), "Length");
			break;
	}
	SetEvent(_event);
}

//==============================================================
//		オクターブ
//--------------------------------------------------------------
//	●引数
//		MMLfile*	MML		MMLファイルのオブジェクト
//	●返値
//		無し
//==============================================================
void	MusicTrack::SetOctave(MMLfile* MML)
{
	unsigned	int	iOctave = MML->GetInt();

	if( (iOctave <= 9) && (iOctave >=2) ){
		SetEvent(new mml_general(nsd_Octave + iOctave - 2, "Octave"));
	} else {
		MML->Err("オクターブは2～9の範囲で指定してください。o1の領域は、相対オクターブをご利用ください。");
	}
}

//==============================================================
//		ゲートタイム(q)
//--------------------------------------------------------------
//	●引数
//		MMLfile*	MML		MMLファイルのオブジェクト
//	●返値
//		無し
//==============================================================
void	MusicTrack::SetGatetime(MMLfile* MML)
{
	unsigned	int	i = MML->GetInt();

	if( (i <= 15) && (i >= 0) ){
		SetEvent(new mml_general(nsd_GateTime_Byte + i, "Gatetime(q) Byte"));
	} else if( i <= 255) {
		SetEvent(new mml_general(nsd_GateTime_q, i, "Gatetime(q)"));
	} else {
		MML->Err("パラメータの値が範囲を越えました。");
	}
}

//==============================================================
//		ゲートタイム(u)
//--------------------------------------------------------------
//	●引数
//		MMLfile*	MML		MMLファイルのオブジェクト
//	●返値
//		無し
//==============================================================
void	MusicTrack::SetGatetime_u(MMLfile* MML)
{
	unsigned		int	i;
	unsigned		char	cData;

	//休符のモード
	cData = MML->GetChar();
	if(cData == '0'){
		i = 0;
	} else {
		MML->Back();
		i = MML->GetLength();
	}
	SetEvent(new mml_general(nsd_GateTime_u, i & 0xFF , "GateTime(u)"));
	
}

//==============================================================
//		スイープ
//--------------------------------------------------------------
//	●引数
//		MMLfile*	MML		MMLファイルのオブジェクト
//	●返値
//		無し
//==============================================================
void	MusicTrack::SetSweep(MMLfile* MML)
{
	unsigned	int		iSpeed;
	unsigned	int		iDepth;
	unsigned	char	_data;
	unsigned	char	cData;

	iSpeed = MML->GetInt();
	if( (iSpeed < 0) || (iSpeed > 15) ){
		MML->Err("スイープで指定できる範囲を超えています。0～15の範囲で指定してください。");
	}

	cData = MML->GetChar();
	if(cData != ','){
		MML->Err("s コマンドのパラメータが足りません。２つ指定してください。");
	}

	iDepth = MML->GetInt();
	if( (iDepth < 0) || (iDepth > 15) ){
		MML->Err("スイープで指定できる範囲を超えています。0～15の範囲で指定してください。");
	}
	_data = ((iSpeed & 0x0F) << 4) | (iDepth & 0x0F);
	SetEvent(new mml_general(nsd_Sweep, _data, "Sweep"));
}

//==============================================================
//		音量
//--------------------------------------------------------------
//	●引数
//		MMLfile*	MML		MMLファイルのオブジェクト
//	●返値
//		無し
//==============================================================
void	MusicTrack::SetVolume(MMLfile* MML)
{
	unsigned	int	i = MML->GetInt();

	if( (i <= 15) && (i >= 0) ){
		SetEvent(new mml_general(nsd_Volume + i, "Volume"));
	} else {
		MML->Err("音量で指定できる範囲を超えています。0～15の範囲で指定してください。");
	}
}

//==============================================================
//		リリースモード
//--------------------------------------------------------------
//	●引数
//		MMLfile*	MML		MMLファイルのオブジェクト
//	●返値
//		無し
//==============================================================
void	MusicTrack::SetReleaseMode(MMLfile* MML)
{
	unsigned	int	i = MML->GetInt();
	mml_general*	_event;

	switch(i){
		case(0):
			_event = new mml_general(nsd_GateMode_0,  "GateMode 0");
			break;
		case(1):
			_event = new mml_general(nsd_GateMode_1,  "GateMode 1");
			break;
		case(2):
			_event = new mml_general(nsd_GateMode_2,  "GateMode 2");
			break;
		default:
			MML->Err("リリースモードで指定できる範囲を超えています。0～2の範囲で指定してください。");
			break;
	}
	SetEvent(_event);
}

//==============================================================
//		リリース音色
//--------------------------------------------------------------
//	●引数
//		MMLfile*	MML		MMLファイルのオブジェクト
//	●返値
//		無し
//==============================================================
void	MusicTrack::SetReleaseVoice(MMLfile* MML)
{
	unsigned	int	i = MML->GetInt();

	if( (i <= 7) && (i >= 0) ){
		SetEvent(new mml_general(nsd_Release_Voice + i, "Release Voice"));
	} else {
		MML->Err("リリース音色で指定できる範囲を超えています。0～7の範囲で指定してください。");
	}
}

//==============================================================
//		リリース音量
//--------------------------------------------------------------
//	●引数
//		MMLfile*	MML		MMLファイルのオブジェクト
//	●返値
//		無し
//==============================================================
void	MusicTrack::SetReleaseVolume(MMLfile* MML)
{
	unsigned	int	i = MML->GetInt();

	if( (i <= 15) && (i >= 0) ){
		SetEvent(new mml_general(nsd_Release_Volume + i, "Release Volume"));
	} else {
		MML->Err("音量で指定できる範囲を超えています。0～15の範囲で指定してください。");
	}
}

//==============================================================
//		
//--------------------------------------------------------------
//	●引数
//		MMLfile*	MML		MMLファイルのオブジェクト
//	●返値
//		無し
//==============================================================
void	MusicTrack::SetPoke(MMLfile* MML)
{
	unsigned	int	addr;
	unsigned	int	data;
	unsigned	char	cData;

	addr = MML->GetInt();
	if( (addr < 0x0000) || (addr > 0xFFFF) ){
		MML->Err("パラメータの値が範囲を越えました。");
	}

	cData = MML->GetChar();
	if(cData != ','){
		MML->Err("y コマンドのパラメータが足りません。２つ指定してください。");
	}

	data = MML->GetInt();
	if(data > 255){
		MML->Err("パラメータの値が範囲を越えました。");
	}
	SetEvent(new mml_poke(addr, data & 0xFF));
}

//==============================================================
//		調号の設定
//--------------------------------------------------------------
//	●引数
//		MMLfile*	MML		MMLファイルのオブジェクト
//	●返値
//		無し
//==============================================================
void	MusicTrack::SetKeySignature(MMLfile*	MML)
{
	//調号コマンドのID
	enum	KeySignature_ID {
		ks_Sharp,
		ks_Flat,
		ks_Natural,
		ks_c,
		ks_d,
		ks_e,
		ks_f,
		ks_g,
		ks_a,
		ks_b,
		ks_0,
		ks_s1,
		ks_s2,
		ks_s3,
		ks_s4,
		ks_s5,
		ks_s6,
		ks_s7,
		ks_f1,
		ks_f2,
		ks_f3,
		ks_f4,
		ks_f5,
		ks_f6,
		ks_f7
	};

	//調号コマンド内の定義
	static	const	Command_Info	KS_Command[] = {
		{	"+",		ks_Sharp	},
		{	"#",		ks_Sharp	},
		{	"-",		ks_Flat		},
		{	"=",		ks_Natural	},
		{	"*",		ks_Natural	},
		{	"c",		ks_c		},
		{	"d",		ks_d		},
		{	"e",		ks_e		},
		{	"f",		ks_f		},
		{	"g",		ks_g		},
		{	"a",		ks_a		},
		{	"b",		ks_b		},
		{	"C_Dur",	ks_0		},	//
		{	"Cis_Dur",	ks_f5		},	//bbbbb
		{	"D_Dur",	ks_s2		},	//##
		{	"Dis_Dur",	ks_f3		},	//bbb
		{	"E_Dur",	ks_s4		},	//####
		{	"F_Dur",	ks_f1		},	//b
		{	"Fis_Dur",	ks_s6		},	//######
		{	"Gs_Dur",	ks_f6		},	//bbbbbb
		{	"G_Dur",	ks_s1		},	//#
		{	"As_Dur",	ks_f4		},	//bbbb
		{	"A_Dur",	ks_s3		},	//###
		{	"B_Dur",	ks_f2		},	//bb
		{	"H_Dur",	ks_s5		}	//#####
	};

	unsigned	char	cData;
				char	sign = 0;

	while(MML->cRead() != '{'){
		if(MML->eof()){
			MML->Err("調号コマンド・ブロックの開始を示す{が見つかりません。");
		}
	}

	// } が来るまで、記述ブロック内をコンパイルする。
	while((cData = MML->GetChar()) != '}'){
		
		// } が来る前に、[EOF]が来たらエラー
		if( MML->eof() ){
			MML->Err("調号コマンド・ブロックの終端を示す`}'がありません。");
		}

		//１つ戻る
		MML->StreamPointerAdd(-1);

		switch(MML->GetCommandID(KS_Command, sizeof(KS_Command)/sizeof(Command_Info))){
			case(ks_c):
				KeySignature[0] = sign;
				break;
			case(ks_d):
				KeySignature[1] = sign;
				break;
			case(ks_e):
				KeySignature[2] = sign;
				break;
			case(ks_f):
				KeySignature[3] = sign;
				break;
			case(ks_g):
				KeySignature[4] = sign;
				break;
			case(ks_a):
				KeySignature[5] = sign;
				break;
			case(ks_b):
				KeySignature[6] = sign;
				break;
			case(ks_Natural):
				sign = 0;
				break;
			case(ks_Sharp):
				sign = 1;
				break;
			case(ks_Flat):
				sign = -1;
				break;
			default:
				MML->Err("調号 K{} コマンドの引数で未知の文字が指定されました。");
				break;
		}
	}

	//for Debug
/*
	cout << "[0] c = " << (int)KeySignature[0] << endl;
	cout << "[1] d = " << (int)KeySignature[1] << endl;
	cout << "[2] e = " << (int)KeySignature[2] << endl;
	cout << "[3] f = " << (int)KeySignature[3] << endl;
	cout << "[4] g = " << (int)KeySignature[4] << endl;
	cout << "[5] a = " << (int)KeySignature[5] << endl;
	cout << "[6] b = " << (int)KeySignature[6] << endl;
*/
}

//==============================================================
//		記述ブロックの終了（End of Trackの追加）
//--------------------------------------------------------------
//	●引数
//		無し
//	●返値
//		無し
//==============================================================
unsigned	int	MusicTrack::SetEnd(void)
{
	mml_Address*	_event;

	if(loop_flag == false){
		SetEvent(new mml_general(nsd_EndOfTrack,"End of Track"));
	} else {
		_event = new mml_Address(nsd_Jump, "End of Track with LOOP");
		_event->set_Address(offset_loop - offset_now - 1);
		SetEvent(_event);
	}
	iSize = offset_now;

	return(iSize);
}

//==============================================================
//		シーケンス・オブジェクトの追加
//--------------------------------------------------------------
//	●引数
//		MusicItem* _item	シーケンス・オブジェクト
//	●返値
//		無し
//==============================================================
void	MusicTrack::SetEvent(MusicItem* _item)
{
	//サイズの分、オフセットを増やす。
	offset_now += _item->getSize();

	//作ったobjectのポインタを保存しておく。
	ptcItem.push_back(_item);
}

