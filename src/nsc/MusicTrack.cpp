#include "StdAfx.h"
#include "MusicTrack.h"

/****************************************************************/
/*					グローバル変数（クラスだけど・・・）		*/
/****************************************************************/
extern	OPSW*			cOptionSW;	//オプション情報へのポインタ変数

//==============================================================
//		コンストラクタ
//--------------------------------------------------------------
//	●引数
//		const		wchar_t	_strName[]	オブジェクト名
//	●返値
//					無し
//==============================================================
MusicTrack::MusicTrack(MMLfile* MML, const wchar_t _strName[]):
	MusicItem(_strName),
	offset_now(0),
	offset_loop(0),				//無限ループ
	offset_repeat_a_s(0),		//リピートＡ
	offset_repeat_a_b(0),		//リピートＡ
	offset_repeat_b_s(0),		//リピートＢ
	offset_repeat_b_b(0),		//リピートＢ
	sp_repeat_c(0),
	DefaultLength(-1),
	volume(15),
	octave(5),
	octave1(0),
	echo_length(-1),
	echo_already(false),
	echo_vol_ret(false),
	echo_flag(false),
	loop_flag(false),
	compile_flag(false),
	pt_oldNote(0),
	iKeyShift(0),
	iTranspose(0),
	f_Patch(false),
	jump_flag(false)
{
	unsigned	int	i = 0;

	while(i < 256){
		oldNote[i] = -1;
		i++;
	}

	Reset_opt();

	QMax = MML->QMax;
	gatetime_Q = QMax;

	//調号（Default = C-Dur）
	nowKey		=0;
	nowScale	=0;
	SetKey(nowKey, nowScale);

	//保護外のメモリアクセスによる例外発生対策（MMLコンパイラ展開のリピートの為）
	//Visual C++.net 2008 では、こうしておかないと、例外が発生する模様。
	//つまり、ぬるぽ対策
	{
		list<MusicItem*>::iterator	pt_itMusic	=	ptcItem.end();

		//リピートタイプ
		repeat_type.push_back(0);

		//リピート(C)
		st_ct_repeat_c.push_back(0);
		st_it_repeat_c_s.push_back(pt_itMusic);
		st_it_repeat_c_b.push_back(pt_itMusic);
		st_it_repeat_c_e.push_back(pt_itMusic);

		//イベント
		SetEvent(new MusicItem(L"Null"));
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
MusicTrack::~MusicTrack(void)
{
}

//==============================================================
//		カウント
//--------------------------------------------------------------
//	●引数
//				無し
//	●返値
//			int	Tick数
//==============================================================
unsigned int	MusicTrack::TickCount(MusicFile* MUS, unsigned int iLength)
{
	//----------------------
	//Local変数
	list<	MusicItem*>::iterator	itItem;
	unsigned	char				iCode;

	list<	MusicItem*>::iterator	itRepeatA_start;
	list<	MusicItem*>::iterator	itRepeatA_end;
				int					iRepeatA_count	= 0;
				bool				f_RepeatA	=	false;

	list<	MusicItem*>::iterator	itRepeatB_start;
	list<	MusicItem*>::iterator	itRepeatB_end;
				int					iRepeatB_count	= 0;
				bool				f_RepeatB	=	false;

	DefaultLength	=	iLength;

	offset_now	=	0;		//バイナリー上のオフセット
	iTickTotal	=	0;		//
	iTickLoop	=	0;

	//----------------------
	//プリ演奏

	//シーケンスデータは存在しているか？
	if(!ptcItem.empty()){
		//存在していたら、プリ演奏
		itItem = ptcItem.begin();
		while(itItem != ptcItem.end()){
			if(offset_loop == offset_now){
				iTickLoop = iTickTotal;
			}
			iCode		=	(*itItem)->getCode(0);
			if((f_RepeatA == false) && (f_RepeatB == false)){
				offset_now	+=	(*itItem)->getSize();
			}
			if(iCode < 0x80){
				//command
				switch(iCode){
					case(nsd_Call):
						{
							mml_Address*	ptAdd	= (mml_Address*)(*itItem);
							Sub*			cSub	= MUS->ptcSub[ptAdd->get_id()];

							iTickTotal		+= cSub->TickCount(MUS, DefaultLength);
							DefaultLength	=  cSub->GetDefaultLength();
						}
						break;

					//----------
					//Repeat A
					case(nsd_Repeat_A_Start):
						itRepeatA_start	= itItem;	//覚えるのは現時点で良い
						iRepeatA_count	= (*itItem)->getCode(1);
						break;

					case(nsd_Repeat_A_Branch):
						if(iRepeatA_count == 1){
							itItem = itRepeatA_end;	//終了点に行く
							f_RepeatA = false;
						}
						break;

					case(nsd_Repeat_A_End):
						itRepeatA_end = itItem;		//終了点を記憶
						iRepeatA_count--;
						if(iRepeatA_count != 0){
							itItem = itRepeatA_start;
							f_RepeatA = true;
						} else {
							f_RepeatA = false;
						}
						break;

					//----------
					//Repeat B
					case(nsd_Repeat_B_Start):
						itRepeatB_start = itItem;	//覚えるのは現時点で良い
						iRepeatB_count	= 0;
						break;

					case(nsd_Repeat_B_Branch):
						if(iRepeatB_count != 0){
							itItem = itRepeatB_end;	//分岐先へ
						}
						f_RepeatB = false;
						break;

					case(nsd_Repeat_B_End):
						itRepeatB_end = itItem;		//分岐先を記憶
						itItem = itRepeatB_start;
						iRepeatB_count++;
						f_RepeatB = true;
						break;

					//----------
					//Length
					case(nsd_Length):
						DefaultLength = (*itItem)->getCode(1);
						break;
					case(nsd_Length_96):
						DefaultLength = 96;
						break;
					case(nsd_Length_72):
						DefaultLength = 72;
						break;
					case(nsd_Length_48):
						DefaultLength = 48;
						break;
					case(nsd_Length_36):
						DefaultLength = 36;
						break;
					case(nsd_Length_32):
						DefaultLength = 32;
						break;
					case(nsd_Length_24):
						DefaultLength = 24;
						break;
					case(nsd_Length_18):
						DefaultLength = 18;
						break;
					case(nsd_Length_16):
						DefaultLength = 16;
						break;
					case(nsd_Length_12):
						DefaultLength = 12;
						break;
					case(nsd_Length_9):
						DefaultLength = 9;
						break;
					case(nsd_Length_8):
						DefaultLength = 8;
						break;
					case(nsd_Length_6):
						DefaultLength = 6;
						break;
					case(nsd_Length_4):
						DefaultLength = 4;
						break;
					case(nsd_Length_3):
						DefaultLength = 3;
						break;
					case(nsd_Length_2):
						DefaultLength = 2;
						break;
					case(nsd_Length_1):
						DefaultLength = 1;
						break;
					default:
						break;
				}
			} else {
				//note
				iCode &= 0x7F;
				if(iCode & nsd_Note_Length){
					iTickTotal += (int)(*itItem)->getCode(1);
				} else {
					iTickTotal += DefaultLength;
				}
			}
			itItem++;
		}
	}

	if(loop_flag == false){
		iTickLoop = -1;
	}
	return(iTickTotal);
}

//==============================================================
//		使う（使わない）オブジェクトの検索
//--------------------------------------------------------------
//	●引数
//		MusicFile*	MUS		曲データファイル・オブジェクト
//	●返値
//				無し
//==============================================================
void	MusicTrack::Optimize(MusicFile* MUS)
{
	//----------------------
	//Local変数
	vector<	mml_Address*	>::iterator	itSub;
	vector<	mml_Address*	>::iterator	itEnv;
	vector<	mml_Address*	>::iterator	itFDSC;
	vector<	mml_Address*	>::iterator	itFDSM;
	vector<	mml_Address*	>::iterator	itVRC7;
	vector<	mml_Address*	>::iterator	itN163;
	unsigned	int	_no;

	//----------------------
	//Surbortine

	if(!ptcSub.empty()){
		itSub = ptcSub.begin();
		while(itSub != ptcSub.end()){
			_no			= (*itSub)->get_id();		//サブルーチンNo.の取得
			if( MUS->ptcSub.count(_no) == 0){
				if(cOptionSW->fErr == true){
					wcerr << L"Sub(" << _no << L")番が存在しません。" << endl;
				} else {
					wcout << L"Sub(" << _no << L")番が存在しません。" << endl;
				}
				nsc_exit(EXIT_FAILURE);
			}
			MUS->ptcSub[_no]->setUse();	//使うフラグを立てる
			itSub++;
		}
	}

	//----------------------
	//Envelope
	if(!ptcEnv.empty()){
		itEnv = ptcEnv.begin();
		while(itEnv != ptcEnv.end()){
			_no			= (*itEnv)->get_id();		//エンベロープNo.の取得
			if( MUS->ptcEnv.count(_no) == 0){
				if(cOptionSW->fErr == true){
					wcerr << L"Envelope(" << _no << L")番が存在しません。" << endl;
				} else {
					wcout << L"Envelope(" << _no << L")番が存在しません。" << endl;
				}
				nsc_exit(EXIT_FAILURE);
			}
			MUS->ptcEnv[_no]->setUse();	//使うフラグを立てる
			itEnv++;
		}
	}

	//----------------------
	//FDSC
	if(!ptcFDSC.empty()){
		itFDSC = ptcFDSC.begin();
		while(itFDSC != ptcFDSC.end()){
			_no			= (*itFDSC)->get_id();		//エンベロープNo.の取得
			if( MUS->ptcFDSC.count(_no) == 0){
				if(cOptionSW->fErr == true){
					wcerr << L"FDSC(" << _no << L")番が存在しません。" << endl;
				} else {
					wcout << L"FDSC(" << _no << L")番が存在しません。" << endl;
				}
				nsc_exit(EXIT_FAILURE);
			}
			MUS->ptcFDSC[_no]->setUse();	//使うフラグを立てる
			itFDSC++;
		}
	}

	//----------------------
	//FDSM
	if(!ptcFDSM.empty()){
		itFDSM = ptcFDSM.begin();
		while(itFDSM != ptcFDSM.end()){
			_no			= (*itFDSM)->get_id();		//エンベロープNo.の取得
			if( MUS->ptcFDSM.count(_no) == 0){
				if(cOptionSW->fErr == true){
					wcerr << L"FDSM(" << _no << L")番が存在しません。" << endl;
				} else {
					wcout << L"FDSM(" << _no << L")番が存在しません。" << endl;
				}
				nsc_exit(EXIT_FAILURE);
			}
			MUS->ptcFDSM[_no]->setUse();	//使うフラグを立てる
			itFDSM++;
		}
	}

	//----------------------
	//OPLL
	if(!ptcOPLL.empty()){
		itVRC7 = ptcOPLL.begin();
		while(itVRC7 != ptcOPLL.end()){
			_no			= (*itVRC7)->get_id();		//エンベロープNo.の取得
			if( MUS->ptcVRC7.count(_no) == 0){
				if(cOptionSW->fErr == true){
					wcerr << L"VRC7(" << _no << L")番が存在しません。" << endl;
				} else {
					wcout << L"VRC7(" << _no << L")番が存在しません。" << endl;
				}
				nsc_exit(EXIT_FAILURE);
			}
			MUS->ptcVRC7[_no]->setUse();	//使うフラグを立てる
			itVRC7++;
		}
	}

	//----------------------
	//N163
	if(!ptcWave.empty()){
		itN163 = ptcWave.begin();
		while(itN163 != ptcWave.end()){
			_no			= (*itN163)->get_id();		//エンベロープNo.の取得
			if( MUS->ptcN163.count(_no) == 0){
				if(cOptionSW->fErr == true){
					wcerr << L"N163(" << _no << L")番が存在しません。" << endl;
				} else {
					wcout << L"N163(" << _no << L")番が存在しません。" << endl;
				}
				nsc_exit(EXIT_FAILURE);
			}
			MUS->ptcN163[_no]->setUse();	//使うフラグを立てる
			itN163++;
		}
	}
	
}

//==============================================================
//		アドレス情報を決定する。
//--------------------------------------------------------------
//	●引数
//		MusicFile*	MUS		曲データファイル・オブジェクト
//	●返値
//				無し
//==============================================================
void	MusicTrack::Fix_Address(MusicFile* MUS)
{
	//----------------------
	//Local変数
	vector<	mml_Address*	>::iterator	itSE;
	vector<	mml_Address*	>::iterator	itSub;
	vector<	mml_Address*	>::iterator	itEnv;
	vector<	mml_Address*	>::iterator	itFDSC;
	vector<	mml_Address*	>::iterator	itFDSM;
	vector<	mml_Address*	>::iterator	itVRC7;
	vector<	mml_Address*	>::iterator	itN163;
	unsigned	int	_no;
	unsigned	int	_sub_offset;
	unsigned	int	_com_offset;

	//----------------------
	//SE
	if(!ptcSE.empty()){
		itSE = ptcSE.begin();
		while(itSE != ptcSE.end()){
			_no			= (*itSE)->get_id();		//サブルーチンNo.の取得
			_com_offset	= (*itSE)->getOffset();
			if( MUS->ptcSE.count(_no) == 0){
				if(cOptionSW->fErr == true){
					wcerr << L"SE(" << _no << L")番が存在しません。" << endl;
				} else {
					wcout << L"SE(" << _no << L")番が存在しません。" << endl;
				}
				nsc_exit(EXIT_FAILURE);
			}
			_sub_offset = MUS->ptcSE[_no]->getOffset();	//指定サブルーチンが存在するオフセット
			(*itSE)->set_Address(_sub_offset - _com_offset - 1);
			itSE++;
		}
	}

	//----------------------
	//Surbortine
	if(!ptcSub.empty()){
		itSub = ptcSub.begin();
		while(itSub != ptcSub.end()){
			_no			= (*itSub)->get_id();		//サブルーチンNo.の取得
			_com_offset	= (*itSub)->getOffset();
			if( MUS->ptcSub.count(_no) == 0){
				if(cOptionSW->fErr == true){
					wcerr << L"Sub(" << _no << L")番が存在しません。" << endl;
				} else {
					wcout << L"Sub(" << _no << L")番が存在しません。" << endl;
				}
				nsc_exit(EXIT_FAILURE);
			}
			_sub_offset = MUS->ptcSub[_no]->getOffset();	//指定サブルーチンが存在するオフセット
			(*itSub)->set_Address(_sub_offset - _com_offset - 1);
			itSub++;
		}
	}

	//----------------------
	//Envelope
	if(!ptcEnv.empty()){
		itEnv = ptcEnv.begin();
		while(itEnv != ptcEnv.end()){
			_no			= (*itEnv)->get_id();		//エンベロープNo.の取得
			_com_offset	= (*itEnv)->getOffset();
			if( MUS->ptcEnv.count(_no) == 0){
				if(cOptionSW->fErr == true){
					wcerr << L"Envelope(" << _no << L")番が存在しません。" << endl;
				} else {
					wcout << L"Envelope(" << _no << L")番が存在しません。" << endl;
				}
				nsc_exit(EXIT_FAILURE);
			}
			_sub_offset = MUS->ptcEnv[_no]->getOffset();	//指定エンベロープが存在するオフセット
			(*itEnv)->set_Address(_sub_offset - _com_offset - 1);
			itEnv++;
		}
	}

	//----------------------
	//FDSC
	if(!ptcFDSC.empty()){
		itFDSC = ptcFDSC.begin();
		while(itFDSC != ptcFDSC.end()){
			_no			= (*itFDSC)->get_id();		//エンベロープNo.の取得
			_com_offset	= (*itFDSC)->getOffset();
			if( MUS->ptcFDSC.count(_no) == 0){
				if(cOptionSW->fErr == true){
					wcerr << L"FDSC(" << _no << L")番が存在しません。" << endl;
				} else {
					wcout << L"FDSC(" << _no << L")番が存在しません。" << endl;
				}
				nsc_exit(EXIT_FAILURE);
			}
			_sub_offset = MUS->ptcFDSC[_no]->getOffset();	//指定エンベロープが存在するオフセット
			(*itFDSC)->set_Address(_sub_offset - _com_offset - 1);
			itFDSC++;
		}
	}

	//----------------------
	//FDSM
	if(!ptcFDSM.empty()){
		itFDSM = ptcFDSM.begin();
		while(itFDSM != ptcFDSM.end()){
			_no			= (*itFDSM)->get_id();		//エンベロープNo.の取得
			_com_offset	= (*itFDSM)->getOffset();
			if( MUS->ptcFDSM.count(_no) == 0){
				if(cOptionSW->fErr == true){
					wcerr << L"FDSM(" << _no << L")番が存在しません。" << endl;
				} else {
					wcout << L"FDSM(" << _no << L")番が存在しません。" << endl;
				}
				nsc_exit(EXIT_FAILURE);
			}
			_sub_offset = MUS->ptcFDSM[_no]->getOffset();	//指定エンベロープが存在するオフセット
			(*itFDSM)->set_Address(_sub_offset - _com_offset - 1);
			itFDSM++;
		}
	}

	//----------------------
	//OPLL
	if(!ptcOPLL.empty()){
		itVRC7 = ptcOPLL.begin();
		while(itVRC7 != ptcOPLL.end()){
			_no			= (*itVRC7)->get_id();		//エンベロープNo.の取得
			_com_offset	= (*itVRC7)->getOffset();
			if( MUS->ptcVRC7.count(_no) == 0){
				if(cOptionSW->fErr == true){
					wcerr << L"VRC7(" << _no << L")番が存在しません。" << endl;
				} else {
					wcout << L"VRC7(" << _no << L")番が存在しません。" << endl;
				}
				nsc_exit(EXIT_FAILURE);
			}
			_sub_offset = MUS->ptcVRC7[_no]->getOffset();	//指定エンベロープが存在するオフセット
			(*itVRC7)->set_Address(_sub_offset - _com_offset - 1);
			itVRC7++;
		}
	}

	//----------------------
	//N163
	if(!ptcWave.empty()){
		itN163 = ptcWave.begin();
		while(itN163 != ptcWave.end()){
			_no			= (*itN163)->get_id();		//エンベロープNo.の取得
			_com_offset	= (*itN163)->getOffset();
			if( MUS->ptcN163.count(_no) == 0){
				if(cOptionSW->fErr == true){
					wcerr << L"N163(" << _no << L")番が存在しません。" << endl;
				} else {
					wcout << L"N163(" << _no << L")番が存在しません。" << endl;
				}
				nsc_exit(EXIT_FAILURE);
			}
			_sub_offset = MUS->ptcN163[_no]->getOffset();	//指定エンベロープが存在するオフセット
			(*itN163)->set_Address(_sub_offset - _com_offset - 2);
			itN163++;
		}
	}

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
	offset_now += (unsigned int)_item->getSize();

	//作ったobjectのポインタを保存しておく。
	ptcItem.push_back(_item);

}

//==============================================================
//		記述ブロックの終了（End of Trackの追加）
//--------------------------------------------------------------
//	●引数
//		無し
//	●返値
//		size_t
//==============================================================
size_t	MusicTrack::SetEnd(MMLfile* MML)
{
	mml_Address*	_event;

	//リピートのチェック
	if(offset_repeat_a_s != 0){
		MML->Err(L"リピート(A)が終わっていません。");
	}
	if(sp_repeat_c != 0){
		MML->Err(L"リピート(C)が終わっていません。");
	}

	//オブジェクトの作成
	if(loop_flag == false){
		SetEvent(new mml_general(nsd_EndOfTrack,L"End of Track"));
	} else {
		_event = new mml_Address(nsd_Jump, L"End of Track with LOOP");
		_event->set_Address(offset_loop - offset_now - 1);
		SetEvent(_event);
	}
	iSize = offset_now;

	return(iSize);
}

//==============================================================
//		L	無限ループ
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

	Reset_opt();
}

//==============================================================
//		|:	リピート(B)	開始
//--------------------------------------------------------------
//	●引数
//		MMLfile*	MML		MMLファイルのオブジェクト
//	●返値
//				無し
//==============================================================
void	MusicTrack::SetRepeat_B_Start()
{
	SetEvent(new mml_general(nsd_Repeat_B_Start, L"Repeat(B) Start"));
	offset_repeat_b_s = offset_now;	// :| コマンドでは、次のコマンドに戻る。
	offset_repeat_b_b = 0;

	Reset_opt();
}

//==============================================================
//		\	リピート(B)	分岐
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
			SetEvent(new mml_general(nsd_Repeat_B_Branch, L"Repeat(B) Branch"));
		} else {
			MML->Err(L"リピート(B)内で \\ コマンドが重複しています。");
		}
	} else {
		MML->Err(L"リピート(B)の開始 |: コマンドがありません。");
	}
}

//==============================================================
//		:|	リピート(B)	終了
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
				MML->Err(L"リピート(A)の区間の途中です。");
			}
			_event = new mml_Address(nsd_Repeat_B_End, L"Repeat(B) End");
			_event->set_Address(offset_repeat_b_s - offset_now - 1);
			SetEvent(_event);
		} else {
			MML->Err(L"リピート(B)内で \\ コマンドがありませんでした。必ず分岐点 \\ は指定してください。");
		}
	} else {
		MML->Err(L"リピート(B)の開始 |: コマンドがありません。");
	}

	Reset_opt();
}

//==============================================================
//		:	リピート (A) (C)	開始
//--------------------------------------------------------------
//	●引数
//		MMLfile*	MML		MMLファイルのオブジェクト
//	●返値
//				無し
//==============================================================
void	MusicTrack::SetRepeat_Start(MMLfile* MML)
{
	if(offset_repeat_a_s == 0){
		SetRepeat_A_Start(MML);
	} else {
		SetRepeat_C_Start(MML);
	}
}

//==============================================================
//		[	リピート(A)	開始
//--------------------------------------------------------------
//	●引数
//		MMLfile*	MML		MMLファイルのオブジェクト
//	●返値
//				無し
//==============================================================
void	MusicTrack::SetRepeat_A_Start(MMLfile* MML)
{
	unsigned	char	cData	= MML->GetChar();

	MML->Back();

	if((cData < '0') || (cData > '9')){
		count_repeat_a = -1;
	} else {
		count_repeat_a = MML->GetInt();
		if( (count_repeat_a > 255) || (count_repeat_a < 2) ){
			MML->Err(L"リピート回数は2～255の範囲で指定して下さい。");
		}
	}

	if(offset_repeat_a_s != 0){
		MML->Err(L"リピート(A)のネストはできません。");
	} else {
		_old_repeat = new mml_repeat();

		SetEvent(_old_repeat);
		offset_repeat_a_s = offset_now;	// ] コマンドでは、次のコマンドに戻る。
		offset_repeat_a_b = 0;

		Reset_opt();
	}
		
	//リピートタイプの記憶
	repeat_type.push_back(1);
	it_repeat_type	=	repeat_type.end();
	it_repeat_type--;
}

//==============================================================
//		[:	リピート(C)	開始
//--------------------------------------------------------------
//	●引数
//		MMLfile*	MML		MMLファイルのオブジェクト
//	●返値
//				無し
//==============================================================
void	MusicTrack::SetRepeat_C_Start(MMLfile* MML)
{

	list<MusicItem*>::iterator	pt_itMusic	=	ptcItem.end();
	unsigned	char	cData	= MML->GetChar();
				int		repeat_cnt;

	MML->Back();

	if((cData < '0') || (cData > '9')){
		repeat_cnt = -1;
	} else {
		repeat_cnt = MML->GetInt();
		if( (repeat_cnt > 255) || (repeat_cnt < 2) ){
			MML->Err(L"リピート回数は2～255の範囲で指定して下さい。");
		}
	}

	//スタックの作成
	pt_itMusic--;
	st_ct_repeat_c.push_back(repeat_cnt);
	st_it_repeat_c_s.push_back(pt_itMusic);
	st_it_repeat_c_b.push_back(pt_itMusic);
	st_it_repeat_c_e.push_back(pt_itMusic);
	sp_repeat_c++;

	//最後尾のイテレータを取得
	it_ct_repeat_c		= st_ct_repeat_c.end();
	it_it_repeat_c_s	= st_it_repeat_c_s.end();
	it_it_repeat_c_b	= st_it_repeat_c_b.end();
	it_it_repeat_c_e	= st_it_repeat_c_e.end();

	it_ct_repeat_c--;
	it_it_repeat_c_s--;
	it_it_repeat_c_b--;
	it_it_repeat_c_e--;

	//リピートタイプの記憶
	repeat_type.push_back(3);
	it_repeat_type	=	repeat_type.end();
	it_repeat_type--;
		
}

//==============================================================
//		:	リピート (A) (C)	分岐
//--------------------------------------------------------------
//	●引数
//		MMLfile*	MML		MMLファイルのオブジェクト
//	●返値
//				無し
//==============================================================
void	MusicTrack::SetRepeat_Branch(MMLfile* MML)
{

	list<MusicItem*>::iterator	pt_itMusic	=	ptcItem.end();

	switch(*it_repeat_type){
		case(1):
			if(offset_repeat_a_s != 0){
				if(offset_repeat_a_b == 0){
					offset_repeat_a_b = offset_now + 1;	//引数の位置
					_old_repeatA_Branch = new mml_Address(nsd_Repeat_A_Branch, L"Repeat(A) Branch");
					SetEvent(_old_repeatA_Branch);
				} else {
					MML->Err(L"リピート(A)内で : コマンドが重複しています。");
				}
			} else {
				MML->Err(L"リピート(A)の開始 [ コマンドがありません。");
			}
			break;
		case(3):
			if(sp_repeat_c > 0){
				if((*it_it_repeat_c_b) == (*it_it_repeat_c_s)){
					pt_itMusic--;
					(*it_it_repeat_c_b) = pt_itMusic;
				} else {
					MML->Err(L"リピート(C)内で : コマンドが重複しています。");
				}
			} else {
				MML->Err(L"リピート(C)の開始 [: コマンドがありません。");
			}
			break;
		default:
			MML->Err(L"リピート開始コマンドがありません。");
			break;
	}

}

//==============================================================
//		:	リピート (A) (C)	終了
//--------------------------------------------------------------
//	●引数
//		MMLfile*	MML		MMLファイルのオブジェクト
//	●返値
//				無し
//==============================================================
void	MusicTrack::SetRepeat_End(MMLfile* MML)
{

	switch(*it_repeat_type){
		case(1):
			SetRepeat_A_End(MML);
			break;
		case(3):
			SetRepeat_C_End(MML);
			break;
		default:
			MML->Err(L"リピート開始コマンドがありません。");
			break;
	}

}

//==============================================================
//		]	リピート(A)	終了
//--------------------------------------------------------------
//	●引数
//		MMLfile*	MML		MMLファイルのオブジェクト
//	●返値
//				無し
//==============================================================
void	MusicTrack::SetRepeat_A_End(MMLfile* MML)
{

	if((offset_repeat_a_s == 0) || ((*it_repeat_type) != 1)){
		MML->Err(L"リピート(A)の開始 [ コマンドがありません。");

	} else {
		
		mml_Address*		_event;
		unsigned	char	cData	= MML->GetChar();

		MML->Back();

		if((cData < '0') || (cData > '9')){
			//引数が無い場合
			if(count_repeat_a == -1){
				MML->Err(L"リピート回数の記述がありません。");
			}
		} else {
			//引数がある場合
			if(count_repeat_a != -1){
				MML->Err(L"リピート回数が両方に記述されています。");
			}
			count_repeat_a = MML->GetInt();
			if( (count_repeat_a > 255) || (count_repeat_a < 2) ){
				MML->Err(L"リピート回数は2～255の範囲で指定して下さい。");
			}
		}

		_old_repeat->set_count((unsigned char)count_repeat_a);

		_event = new mml_Address(nsd_Repeat_A_End, L"Repeat(A) End");
		_event->set_Address(offset_repeat_a_s - offset_now - 1);
		SetEvent(_event);
		//条件分岐があったら。
		if(offset_repeat_a_b != 0){
			_old_repeatA_Branch->set_Address(offset_now - offset_repeat_a_b);
		}
		offset_repeat_a_s = 0;

		Reset_opt();

		//リピートタイプの復帰
		it_repeat_type--;
		repeat_type.pop_back();
	}

}

//==============================================================
//		:]	リピート(C)	終了
//--------------------------------------------------------------
//	●引数
//		MMLfile*	MML		MMLファイルのオブジェクト
//	●返値
//				無し
//==============================================================
void	MusicTrack::SetRepeat_C_End(MMLfile* MML)
{

	if((sp_repeat_c == 0) || ((*it_repeat_type) != 3)){
		MML->Err(L"リピート(C)の開始 [: コマンドがありません。");
	} else {

		list<MusicItem*>::iterator	pt_itMusic = ptcItem.end();
		unsigned	char	cData		= MML->GetChar();
					int		repeat_cnt	= (*it_ct_repeat_c);

		MML->Back();

		if((cData < '0') || (cData > '9')){
			//引数が無い場合
			if(repeat_cnt == -1){
				MML->Err(L"リピート回数の記述がありません。");
			}
		} else {
			//引数がある場合
			if(repeat_cnt != -1){
				MML->Err(L"リピート回数が両方に記述されています。");
			}
			repeat_cnt = MML->GetInt();
			if( (repeat_cnt > 255) || (repeat_cnt < 2) ){
				MML->Err(L"リピート回数は2～255の範囲で指定して下さい。");
			} else {
				(*it_ct_repeat_c)	= repeat_cnt;
			}
		}

		pt_itMusic--;
		(*it_it_repeat_c_e) = pt_itMusic;

		//--------------------------
		//リピートの展開
		{
			unsigned	int				iRepeatCount = (*it_ct_repeat_c) - 1;
			unsigned	char			cOpCode;
			string						sOpCode;

			while(iRepeatCount>0){
				pt_itMusic	=	(*it_it_repeat_c_s);
				if(pt_itMusic != *it_it_repeat_c_e){
					do{
						pt_itMusic++;
						cOpCode		=	(*pt_itMusic)->getCode(0);
						sOpCode.clear();
										(*pt_itMusic)->getCode(&sOpCode);
						switch(cOpCode){
							case(nsd_Call):
								ptcSub.push_back(CopyAddressEvent(cOpCode, &sOpCode, pt_itMusic));
								break;
							case(nsd_Call_SE):
								ptcSE.push_back(CopyAddressEvent(cOpCode, &sOpCode, pt_itMusic));
								break;
							case(nsd_Envelop_Voice):
							case(nsd_Envelop_Volume):
							case(nsd_Envelop_Frequency):
							case(nsd_Envelop_Note):
								CopyEnvEvent(cOpCode, &sOpCode, pt_itMusic);
								break;
							case(nsc_VRC7):
								ptcOPLL.push_back(CopyAddressEvent(cOpCode, &sOpCode, pt_itMusic));
								break;
							case(nsc_N163):
								ptcWave.push_back(CopyAddressEvent(cOpCode, &sOpCode, pt_itMusic));
								break;
							case(nsd_FDS_Career):
								ptcFDSC.push_back(CopyAddressEvent(cOpCode, &sOpCode, pt_itMusic));
								break;
							case(nsd_FDS_Modlator):
								ptcFDSM.push_back(CopyAddressEvent(cOpCode, &sOpCode, pt_itMusic));
								break;
							default:
								{
									mml_general*	_event	=	new	mml_general(cOpCode);
									_event->setCode(&sOpCode);
									SetEvent(_event);
								}
								break;
						}

						if((iRepeatCount == 1) && (pt_itMusic == *it_it_repeat_c_b)){
							pt_itMusic = *it_it_repeat_c_e;
						}
					} while(pt_itMusic != *it_it_repeat_c_e);
				}
				iRepeatCount--;
			}

		}

		//--------------------------
		//最後尾のイテレータを取得
		it_ct_repeat_c--;
		it_it_repeat_c_s--;
		it_it_repeat_c_b--;
		it_it_repeat_c_e--;

		//スタックの破棄
		st_ct_repeat_c.pop_back();
		st_it_repeat_c_s.pop_back();
		st_it_repeat_c_b.pop_back();
		st_it_repeat_c_e.pop_back();
		sp_repeat_c--;

		//リピートタイプの復帰
		it_repeat_type--;
		repeat_type.pop_back();

	}

}

mml_Address*	MusicTrack::CopyAddressEvent(unsigned char cOpCode, string* sOpCode, list<MusicItem*>::iterator pt_itMusic)
{
	mml_Address*	_event		=	new mml_Address(cOpCode);
	mml_Address*	ptAdrItem	=	(mml_Address*)(*pt_itMusic);
	_event->setCode(sOpCode);
	_event->set_id(ptAdrItem->get_id());
	SetEvent(_event);
	return(_event);
}

void	MusicTrack::CopyEnvEvent(unsigned char cOpCode, string* sOpCode, list<MusicItem*>::iterator pt_itMusic)
{
	mml_Address*	_event		=	new mml_Address(cOpCode);
	mml_Address*	ptAdrItem	=	(mml_Address*)(*pt_itMusic);
	_event->setCode(sOpCode);
	if(ptAdrItem->get_flag() == true){
		_event->set_id(ptAdrItem->get_id());
		ptcEnv.push_back(_event);
	}
	SetEvent(_event);
}

//==============================================================
//		SE	効果音呼び出し
//--------------------------------------------------------------
//	●引数
//		MMLfile*	MML		MMLファイルのオブジェクト
//	●返値
//				無し
//==============================================================
void	MusicTrack::SetSE(MMLfile* MML)
{
	unsigned	int		_no = MML->GetInt();
	if(jump_flag==false){
		mml_Address*		_event = new mml_Address(nsd_Call_SE, L"Call SE");
		_event->set_id(_no);
		SetEvent(_event);
		ptcSE.push_back(_event);
	}
}

//==============================================================
//		S	サブルーチン呼び出し
//--------------------------------------------------------------
//	●引数
//		MMLfile*	MML		MMLファイルのオブジェクト
//	●返値
//				無し
//==============================================================
void	MusicTrack::SetSubroutine(unsigned int _no)
{
	if(jump_flag==false){
		mml_Address*		_event = new mml_Address(nsd_Call, L"Subroutine");
		_event->set_id(_no);
		SetEvent(_event);
		ptcSub.push_back(_event);
	};
	Reset_opt();
}

void	MusicTrack::SetSubWuthParch(unsigned int _no)
{
	if((_no != iSub) || (f_opt_Sub == false)){
		iSub = _no;
		f_opt_Sub	= true;
		if(jump_flag==false){
			mml_Address*		_event = new mml_Address(nsd_Call, L"Subroutine for Patch");
			_event->set_id(_no);
			SetEvent(_event);
			ptcSub.push_back(_event);
		};
	}
}
//==============================================================
//		E	エンベロープ設定
//--------------------------------------------------------------
//	●引数
//		MMLfile*	MML		MMLファイルのオブジェクト
//	●返値
//				無し
//==============================================================
/*
void	MusicTrack::SetEnvelop(unsigned char _opcode, MMLfile* MML, int _offset)
{
	mml_Address*		_event = new mml_Address(_opcode, L"Envelop");
	unsigned	int		_no = MML->GetInt() + _offset;

	_event->set_id(_no);
	SetEvent(_event);
	ptcEnv.push_back(_event);
}
*/

//--------------------------------------------------------------
void	MusicTrack::SetEnvelop_Evoi(unsigned int _no)
{
	mml_Address*		_event;

	if((_no != iEvoi) || (f_opt_Evoi == false) || (sw_Evoi == false)){
		iEvoi		= _no;
		sw_Evoi		= true;
		f_opt_Evoi	= true;		//最適化フラグ
		_event		= new mml_Address(nsd_Envelop_Voice, L"Voice Envelope");

		_event->set_id(_no);
		SetEvent(_event);
		ptcEnv.push_back(_event);
	}
}

//--------------------------------------------------------------
void	MusicTrack::SetEnvelop_Evol(unsigned int _no)
{
	mml_Address*		_event;

	if((_no != iEvol) || (f_opt_Evol == false) || (sw_Evol == false)){
		iEvol		= _no;
		sw_Evol		= true;
		f_opt_Evol	= true;		//最適化フラグ
		_event		= new mml_Address(nsd_Envelop_Volume, L"Volume Envelope");

		_event->set_id(_no);
		SetEvent(_event);
		ptcEnv.push_back(_event);
	}
}

//--------------------------------------------------------------
void	MusicTrack::SetEnvelop_Em(unsigned int _no)
{
	mml_Address*		_event;

	if((_no != iEm) || (f_opt_Em == false) || (sw_Em == false)){
		iEm			= _no;
		sw_Em		= true;
		f_opt_Em	= true;		//最適化フラグ
		_event		= new mml_Address(nsd_Envelop_Frequency, L"Frequency Envelope");

		_event->set_id(_no);
		SetEvent(_event);
		ptcEnv.push_back(_event);
	}
}

//--------------------------------------------------------------
void	MusicTrack::SetEnvelop_En(unsigned int _no)
{
	mml_Address*		_event;

	if((_no != iEn) || (f_opt_En == false) || (sw_En == false)){
		iEn			= _no;
		sw_En		= true;
		f_opt_En	= true;		//最適化フラグ
		_event		= new mml_Address(nsd_Envelop_Note, L"Note Envelope");

		_event->set_id(_no);
		SetEvent(_event);
		ptcEnv.push_back(_event);
	}
}

//--------------------------------------------------------------
void	MusicTrack::SetVoice(unsigned int _no)
{
	if((_no != iVoi) || (f_opt_Evoi == false) || (sw_Evoi == true)){
		iVoi = _no;
		sw_Evoi		= false;
		f_opt_Evoi	= true;		//最適化フラグ
		SetEvent(new mml_general(nsd_Voice, (unsigned char)_no, L"Voice"));
	}
}

//--------------------------------------------------------------
void	MusicTrack::SetEnvelop_Evol()
{
	if((f_opt_Evol == false) || (sw_Evol == true)){
		sw_Evol		= false;
		f_opt_Evol	= true;		//最適化フラグ
		SetEvent(new mml_Address(nsd_Envelop_Volume, L"Volume Envelope Off"));
	}
}

//--------------------------------------------------------------
void	MusicTrack::SetEnvelop_Em()
{
	if((f_opt_Em == false) || (sw_Em == true)){
		sw_Em		= false;
		f_opt_Em	= true;		//最適化フラグ
		SetEvent(new mml_Address(nsd_Envelop_Frequency, L"Frequency Envelope Off"));
	}
}

//--------------------------------------------------------------
void	MusicTrack::SetEnvelop_En()
{
	if((f_opt_En == false) || (sw_En == true)){
		sw_En		= false;
		f_opt_En	= true;		//最適化フラグ
		SetEvent(new mml_Address(nsd_Envelop_Note, L"Note Envelope Off"));
	}
}

//==============================================================
//		音源固有パラメータ
//--------------------------------------------------------------
//	●引数
//		char	c	
//	●返値
//				無し
//==============================================================
void	MusicTrack::SetSweep(unsigned char c)
{
	if((f_opt_Sweep == false) || ((unsigned char)iSweep != c)){
		iSweep		= c;
		f_opt_Sweep	= true;		//最適化フラグ
		SetEvent(new mml_general(nsd_Sweep, c, L"Sweep"));
	}
}

//==============================================================
//		パッチ
//--------------------------------------------------------------
//	●引数
//		MMLfile*	MML		MMLファイルのオブジェクト
//	●返値
//				無し
//==============================================================
void	MusicTrack::SetPatch(MMLfile* MML)
{
	f_Patch = true;
	i_Patch	= MML->GetInt();

	if(MML->ChkPatch(i_Patch) == false){
		MML->Err(L"存在しないパッチです。");
	}
}

//--------------------------------------------------------------
void	MusicTrack::SetPatch()
{
	f_Patch = false;
}

//==============================================================
//		パッチの展開
//--------------------------------------------------------------
//	●引数
//		MMLfile*	MML		MMLファイルのオブジェクト
//		char		 _note	ノート番号
//	●返値
//				無し
//==============================================================
void	MusicTrack::CallPatch(MMLfile* MML, char _note)
{
	if(f_Patch == true){
		//ノート番号の設定
		MML->ptcPatch[i_Patch]->setNote(_note);

		if(	MML->ptcPatch[i_Patch]->get_fSub() == true){
			SetSubWuthParch(MML->ptcPatch[i_Patch]->get_iSub());
		}

		if(	MML->ptcPatch[i_Patch]->get_fGate_q() == true){
			Set_q(MML->ptcPatch[i_Patch]->get_iGate_q());
		}

		if(	MML->ptcPatch[i_Patch]->get_fGate_u() == true){
			Set_u(MML->ptcPatch[i_Patch]->get_iGate_u());
		}

		if(	MML->ptcPatch[i_Patch]->get_fSweep() == true){
			SetSweep(MML->ptcPatch[i_Patch]->get_iSweep());
		}

		if(	MML->ptcPatch[i_Patch]->get_fKey() == true){
			SetTranspose(MML->ptcPatch[i_Patch]->get_iKey());
		}

		if(	MML->ptcPatch[i_Patch]->get_fVoi() == true){
			SetVoice(MML->ptcPatch[i_Patch]->get_iVoi());
		}

		if(	MML->ptcPatch[i_Patch]->get_fEvoi() == true){
			if(MML->ptcPatch[i_Patch]->get_sw_Evoi() == true){
				SetEnvelop_Evoi(MML->ptcPatch[i_Patch]->get_iEvoi() + MML->offset_Ei);
			}
		}

		if(	MML->ptcPatch[i_Patch]->get_fEvol() == true){
			if(MML->ptcPatch[i_Patch]->get_sw_Evol() == true){
				SetEnvelop_Evol(MML->ptcPatch[i_Patch]->get_iEvol() + MML->offset_Ev);
			} else {
				SetEnvelop_Evol();
			}
		}

		if(	MML->ptcPatch[i_Patch]->get_fEm() == true){
			if(MML->ptcPatch[i_Patch]->get_sw_Em() == true){
				SetEnvelop_Em(MML->ptcPatch[i_Patch]->get_iEm() + MML->offset_Em);
			} else {
				SetEnvelop_Em();
			}
		}

		if(	MML->ptcPatch[i_Patch]->get_fEn() == true){
			if(MML->ptcPatch[i_Patch]->get_sw_En() == true){
				SetEnvelop_En(MML->ptcPatch[i_Patch]->get_iEn() + MML->offset_En);
			} else {
				SetEnvelop_En();
			}
		}

	}
}

//==============================================================
//		@FC	FDS	
//--------------------------------------------------------------
//	●引数
//		MMLfile*	MML		MMLファイルのオブジェクト
//	●返値
//				無し
//==============================================================
void	MusicTrack::SetFDSC(MMLfile* MML)
{
	mml_Address*		_event = new mml_Address(nsd_FDS_Career, L"FDS career wave table");
	unsigned	int		_no = MML->GetInt();

	_event->set_id(_no);
	SetEvent(_event);
	ptcFDSC.push_back(_event);
}

//==============================================================
//		@FM	FDS	
//--------------------------------------------------------------
//	●引数
//		MMLfile*	MML		MMLファイルのオブジェクト
//	●返値
//				無し
//==============================================================
void	MusicTrack::SetFDSM(MMLfile* MML)
{
	mml_Address*		_event = new mml_Address(nsd_FDS_Modlator, L"FDS modulator wave table");
	unsigned	int		_no = MML->GetInt();

	_event->set_id(_no);
	SetEvent(_event);
	ptcFDSM.push_back(_event);
}

//==============================================================
//		@V	VRC7ユーザ定義音色設定
//--------------------------------------------------------------
//	●引数
//		MMLfile*	MML		MMLファイルのオブジェクト
//	●返値
//				無し
//==============================================================
void	MusicTrack::SetVRC7(MMLfile* MML)
{
	mml_Address*		_event = new mml_Address(nsc_VRC7, L"VRC7 user instruments");
	unsigned	int		_no = MML->GetInt();

	_event->set_id(_no);
	SetEvent(_event);
	ptcOPLL.push_back(_event);
}

//==============================================================
//		@N	n163音色設定
//--------------------------------------------------------------
//	●引数
//		MMLfile*	MML		MMLファイルのオブジェクト
//	●返値
//				無し
//==============================================================
void	MusicTrack::SetN163(MMLfile* MML)
{
	unsigned	char	cData;
	mml_Address*		_event = new mml_Address(nsc_N163,(unsigned char)MML->GetInt(),L"n163 wave table");

	cData = MML->GetChar();
	if(cData != ','){
		MML->Err(L"@N コマンドのパラメータが足りません。２つ指定してください。");
	}

	_event->set_id(MML->GetInt());
	SetEvent(_event);
	ptcWave.push_back(_event);
}

//--------------------------------------------------------------
//		@NL
//--------------------------------------------------------------
void	MusicTrack::SetN163_Load(MMLfile* MML)
{
				int		i		= MML->GetInt();
	unsigned	char	cData	= MML->GetChar();
	mml_Address*		_event;

	if(cData != ','){
		MML->Err(L"@N コマンドのパラメータが足りません。２つ指定してください。");
	}

	if((i<0) || (i>252)){
		MML->Err(L"n16xの波形開始点は0～252の範囲で指定してください。");
	}
	if((i % 4) != 0){
		MML->Err(L"n16xの波形開始点は4の倍数で指定してください。");
	}

	_event = new mml_Address(nsc_N163,(unsigned char)(i/4),L"n163 wave table");
	_event->set_id(MML->GetInt());
	SetEvent(_event);
	ptcWave.push_back(_event);
}

//==============================================================
//		@NS	n163音色設定
//--------------------------------------------------------------
//	●引数
//		MMLfile*	MML		MMLファイルのオブジェクト
//	●返値
//				無し
//==============================================================
void	MusicTrack::SetN163_Set(MMLfile* MML)
{
				int		i = MML->GetInt();
	unsigned	char	cData;

	if((i<0) || (i>252)){
		MML->Err(L"n16xの波形開始点は0～252の範囲で指定してください。");
	}
	if((i % 4) != 0){
		MML->Err(L"n16xの波形開始点は4の倍数で指定してください。");
	}
	SetVoice(i/4);

	cData = MML->GetChar();
	if(cData == ','){
		i = MML->GetInt();
		if((i<4) || (i>256)){
			MML->Err(L"n16xのサンプル長は4～256の範囲で指定してください。");
		}
		if((i % 4) != 0){
			MML->Err(L"n16xのサンプル長は4の倍数で指定してください。");
		}
		SetSweep(64 - (i/4));
	} else {
		MML->Back();
	}
}

//==============================================================
//		J	ジャンプ
//--------------------------------------------------------------
//	●引数
//		MMLfile*	MML		MMLファイルのオブジェクト
//	●返値
//				無し
//==============================================================
void	MusicTrack::SetJump(MMLfile* MML)
{
	int		_value = MML->GetInt();

	switch(_value){
		case(0):
			jump_flag = false;
			break;
		case(1):
			jump_flag = true;
			break;
		default:
			MML->Err(L"Jコマンドは0～1の範囲で指定してください。");
	}

}

//==============================================================
//		ゲートタイム(q)
//--------------------------------------------------------------
//	●引数
//		int		q		クオンタイズ
//	●返値
//		無し
//==============================================================
void	MusicTrack::Set_q(int i)
{
	if(opt_gatetime_q != i){
		opt_gatetime_q = i;
		if( (i <= 15) && (i >= 0) ){
			SetEvent(new mml_general(nsd_GateTime_Byte + (unsigned char)i, L"Gatetime(q) Byte"));
		} else {
			SetEvent(new mml_general(nsd_GateTime_q, (unsigned char)i, L"Gatetime(q)"));
		}
	}

}

//==============================================================
//		ゲートタイム(u)
//--------------------------------------------------------------
//	●引数
//		int		u		クオンタイズ
//	●返値
//		無し
//==============================================================
void	MusicTrack::Set_u(int i)
{
	if(opt_gatetime_u != i){
		opt_gatetime_u = i;
		SetEvent(new mml_general(nsd_GateTime_u, (unsigned char)i, L"GateTime(u)"));
	}	
}

//==============================================================
//		ゲートタイム(Q)
//--------------------------------------------------------------
//	●引数
//		MMLfile*	MML		MMLファイルのオブジェクト
//	●返値
//		無し
//==============================================================
void	MusicTrack::SetGatetime_Q(MMLfile* MML)
{
	int	i = MML->GetInt();
	if((i<1) || (i>QMax)){
		MML->Err(L"ゲートタイムQは1～#QMaxの範囲で指定して下さい。");
	} else {
		gatetime_Q = i;
		if(gatetime_Q == QMax){
			gatetime_q = 0;
			Set_q(gatetime_q);
		}
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
	gatetime_q = MML->GetInt();
	if(gatetime_q > 255){
		MML->Err(L"ゲートタイムqは0～255の範囲で指定して下さい。");
	} else {
		gatetime_Q = QMax;
		Set_q(gatetime_q);
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
					int		i;
	unsigned		char	cData;

	cData = MML->GetChar();
	if(cData == '0'){
		i = 0;
	} else {
		MML->Back();
		i = MML->GetLength(-1);
	}
	Set_u(i);
}

//==============================================================
//		調号の設定
//--------------------------------------------------------------
//	●引数
//		char	_c ～ _b	設定値
//	●返値
//		無し
//==============================================================
void	MusicTrack::SetKeyFlag(char _c, char _d, char _e, char _f, char _g, char _a, char _b)
{
	KeySignature[0] = _c;	//c
	KeySignature[1] = _d;	//d
	KeySignature[2] = _e;	//e
	KeySignature[3] = _f;	//f
	KeySignature[4] = _g;	//g
	KeySignature[5] = _a;	//a
	KeySignature[6] = _b;	//b
}

//==============================================================
//		調号の設定
//--------------------------------------------------------------
//	●引数
//		int	_key	調のID
//	●返値
//		無し
//==============================================================
void	MusicTrack::SetKey(int _key, int _scale)
{

	static	const	char	Shift[16]={3,6,2,5,1,4,0,3,6,2,5,1,4,0,3,6};

	//調
	static	const	char	Key[16][7]={
	//	  c  d  e  f  g  a  b			n	ID		Key			Shift
		{-1,-1,-1, 0,-1,-1,-1},		//	0	-6		Gs			3
		{ 0,-1,-1, 0,-1,-1,-1},		//	1	-5		Ds			6
		{ 0,-1,-1, 0, 0,-1,-1},		//	2	-4		As			2
		{ 0, 0,-1, 0, 0,-1,-1},		//	3	-3		Es			5
		{ 0, 0,-1, 0, 0, 0,-1},		//	4	-2		Bs			1
		{ 0, 0, 0, 0, 0, 0,-1},		//	5	-1		F			4
		{ 0, 0, 0, 0, 0, 0, 0},		//	6	 0		C			0
		{ 0, 0, 0, 1, 0, 0, 0},		//	7	 1		G			3
		{ 1, 0, 0, 1, 0, 0, 0},		//	8	 2		D			6
		{ 1, 0, 0, 1, 1, 0, 0},		//	9	 3		A			2
		{ 1, 1, 0, 1, 1, 0, 0},		//	10	 4		E			5
		{ 1, 1, 0, 1, 1, 1, 0},		//	11	 5		H			1
		{ 1, 1, 1, 1, 1, 1, 0},		//	12	 6		Fis			4
		{ 1, 1, 1, 1, 1, 1, 1},		//	13	 7		Cis			0		
		{ 1, 1, 1, 2, 1, 1, 1},		//	14	 8		Gis			3
		{ 2, 1, 1, 2, 1, 1, 1}		//	15	 9		Dis			6
	};

	//調
	static	const	char	Scale[28][7]={
	//	  c  d  e  f  g  a  b		ID		Scale (Key = C)		Name
		{ 0, 0, 0, 0, 0, 0, 0},	//	0		c d e f g a b		Ionian
		{ 0, 0,-1, 0, 0, 0,-1},	//	1		c d e-f g a b-		Dorian
		{ 0,-1,-1, 0, 0,-1,-1},	//	2		c d-e-f g a-b-		Phrygian
		{ 0, 0, 0, 1, 0, 0, 0},	//	3		c d e f+g a b		Lydian
		{ 0, 0, 0, 0, 0, 0,-1},	//	4		c d e f g a b-		Mixo-Lydian
		{ 0, 0,-1, 0, 0,-1,-1},	//	5		c d e-f g a-b-		Aeolian
		{ 0,-1,-1, 0,-1,-1,-1},	//	6		c d-e-f g-a-b-		Locrian
		{ 0, 1, 1, 1, 0, 1, 0},	//	7		c d+e+f+g a+b
		{ 0, 0, 0, 0, 0, 0, 0},	//	8		Dummy
		{ 0, 0, 0, 0, 0, 0, 0},	//	9		Dummy
		{ 0, 0,-1, 0, 0,-1, 0},	//	10		c d e-f g a-b		Harmonic minor
		{ 0,-1,-1, 0,-1, 0,-1},	//	11		c d-e-f g-a b-		Locrian natural13
		{ 0, 0, 0, 0, 1, 0, 0},	//	12		c d e f g+a b		Ionian ＃5
		{ 0, 0,-1, 1, 0, 0,-1},	//	13		c d e-f g a b-		Dorian ＃11
		{ 0,-1, 0, 0, 0,-1,-1},	//	14		c d-e f g a-b-		Harmonic minor perfect 5th below
		{ 0, 1, 0, 1, 0, 0, 0},	//	15		c d+e f+g a b		Lydian ＃9
		{ 0,-1,-1,-1,-1,-1,-2},	//	16		c d-e-f-g-a-a		Altered ♭7
		{ 0, 0, 0, 0, 0, 0, 0},	//	17		Dummy
		{ 0, 0, 0, 0, 0, 0, 0},	//	18		Dummy
		{ 0, 0, 0, 0, 0, 0, 0},	//	19		Dummy
		{ 0, 0,-1, 0, 0, 0, 0},	//	20		c d e-f g a b		Melodic minor
		{ 0,-1,-1, 0, 0, 0,-1},	//	21		c d-e-f g a b-		Dorian ♭9
		{ 0, 0, 0, 1, 1, 0, 0},	//	22		c d e f+g+a b		Lydian ＃5
		{ 0, 0, 0, 1, 0, 0,-1},	//	23		c d e f+g a b-		Mixo-Lydian ＃11 (Lydian ♭7)
		{ 0, 0, 0, 0, 0,-1,-1},	//	24		c d e f g a-b-		Mixo-Lydian ♭13
		{ 0, 0,-1, 0,-1,-1,-1},	//	25		c d e-f g-a-b-		Aeolian ♭5 (Super Locrian)
		{ 0,-1,-1,-1,-1,-1,-1},	//	26		c d-e-f-g-a-b-		Altered
		{-1,-1,-1, 0, 0,-1,-1}	//	27		c-d-e-f g a-b-		G Altered
	};

	char	shift;
	char	shift_scale[7];

	nowKey	 = (char)_key;
	nowScale = 0;

	shift = Shift[nowKey + 6];
	shift_scale[0] = (char)Scale[_scale][(shift + 0) % 7];
	shift_scale[1] = (char)Scale[_scale][(shift + 1) % 7];
	shift_scale[2] = (char)Scale[_scale][(shift + 2) % 7];
	shift_scale[3] = (char)Scale[_scale][(shift + 3) % 7];
	shift_scale[4] = (char)Scale[_scale][(shift + 4) % 7];
	shift_scale[5] = (char)Scale[_scale][(shift + 5) % 7];
	shift_scale[6] = (char)Scale[_scale][(shift + 6) % 7];

	SetKeyFlag(	Key[nowKey + 6][0] + shift_scale[0],
				Key[nowKey + 6][1] + shift_scale[1],
				Key[nowKey + 6][2] + shift_scale[2],
				Key[nowKey + 6][3] + shift_scale[3],
				Key[nowKey + 6][4] + shift_scale[4],
				Key[nowKey + 6][5] + shift_scale[5],
				Key[nowKey + 6][6] + shift_scale[6]
	);
}

//==============================================================
//		スケールの設定
//--------------------------------------------------------------
//	●引数
//		MMLfile*	MML		MMLファイルのオブジェクト
//	●返値
//		無し
//==============================================================
void	MusicTrack::SetScale(MMLfile* MML)
{
	nowScale	= (char)MML->GetNum();
	SetKey(nowKey, nowScale);
}

//--------------------------------------------------------------
void	MusicTrack::SetMajor()
{
	nowScale = 0;
	SetKey(nowKey, nowScale);
}
//--------------------------------------------------------------
void	MusicTrack::SetMinor()
{
	nowScale = 5;
	SetKey(nowKey, nowScale);
}
//--------------------------------------------------------------
void	MusicTrack::SetHMinor(MMLfile* MML)
{
	char	cData		= MML->GetChar();
			nowScale	= 10;

	MML->Back();

	if(cData=='('){
		nowScale	+= (char)MML->GetNum();
	}

	SetKey(nowKey, nowScale);
}

//--------------------------------------------------------------
void	MusicTrack::SetMMinor(MMLfile* MML)
{
	char	cData		= MML->GetChar();
			nowScale	= 20;

	MML->Back();

	if(cData=='('){
		nowScale	+= (char)MML->GetNum();
	}

	SetKey(nowKey, nowScale);
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
		ks_r,

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
		ks_f7,

		ks_m0,
		ks_ms1,
		ks_ms2,
		ks_ms3,
		ks_ms4,
		ks_ms5,
		ks_ms6,
		ks_ms7,
		ks_ms8,
		ks_ms9,

		ks_mf1,
		ks_mf2,
		ks_mf3,
		ks_mf4,
		ks_mf5,
		ks_mf6,
	};

	//調号コマンド内の定義
	static	const	Command_Info	KS_Command[] = {
		{	"C-Dur",	ks_0		},	//
		{	"Ds-Dur",	ks_f5		},	//bbbbb
		{	"Des-Dur",	ks_f5		},	//bbbbb
		{	"D-Dur",	ks_s2		},	//##
		{	"Es-Dur",	ks_f3		},	//bbb
		{	"E-Dur",	ks_s4		},	//####
		{	"F-Dur",	ks_f1		},	//b
		{	"Fis-Dur",	ks_s6		},	//######
		{	"Gs-Dur",	ks_f6		},	//bbbbbb
		{	"Ges-Dur",	ks_f6		},	//bbbbbb
		{	"G-Dur",	ks_s1		},	//#
		{	"As-Dur",	ks_f4		},	//bbbb
		{	"A-Dur",	ks_s3		},	//###
		{	"B-Dur",	ks_f2		},	//bb
		{	"Bs-Dur",	ks_f2		},	//bb
		{	"H-Dur",	ks_s5		},	//#####

		{	"c-moll",	ks_m0		},	//bbb
		{	"cis-moll",	ks_ms7		},	//####
		{	"d-moll",	ks_ms2		},	//b
		{	"dis-moll",	ks_ms9		},	//######
		{	"es-moll",	ks_mf3		},	//bbbbbb
		{	"e-moll",	ks_ms4		},	//#
		{	"f-moll",	ks_mf1		},	//bbbb
		{	"fis-moll",	ks_ms6		},	//###
		{	"g-moll",	ks_ms1		},	//bb
		{	"gis-moll",	ks_ms8		},	//#####
		{	"a-moll",	ks_ms3		},	//
		{	"b-moll",	ks_mf2		},	//bbbbb
		{	"bs-moll",	ks_mf2		},	//bbbbb
		{	"h-moll",	ks_ms5		},	//##

		//音楽理論的に正しくないが過去互換性の為。＃系の調なのに♭では言わない！
		{	"ds-moll",	ks_mf5		},	//####
		{	"des-moll",	ks_mf5		},	//####
		{	"gs-moll",	ks_mf6		},	//###
		{	"ges-moll",	ks_mf6		},	//###
		{	"as-moll",	ks_mf4		},	//#####

		{	"+",		ks_Sharp	},
		{	"＋",		ks_Sharp	},
		{	"#",		ks_Sharp	},
		{	"＃",		ks_Sharp	},
		{	"-",		ks_Flat		},
		{	"－",		ks_Flat		},
		{	"♭",		ks_Flat		},
		{	"=",		ks_Natural	},
		{	"＝",		ks_Natural	},
		{	"*",		ks_Natural	},
		{	"＊",		ks_Natural	},

		{	"c",		ks_c		},
		{	"d",		ks_d		},
		{	"e",		ks_e		},
		{	"f",		ks_f		},
		{	"g",		ks_g		},
		{	"a",		ks_a		},
		{	"b",		ks_b		},
		{	"r",		ks_r		},

		{	"ド",		ks_c		},
		{	"レ",		ks_d		},
		{	"ミ",		ks_e		},
		{	"ファ",		ks_f		},
		{	"フ",		ks_f		},
		{	"ソ",		ks_g		},
		{	"ラ",		ks_a		},
		{	"シ",		ks_b		},
		{	"ン",		ks_r		},
		{	"ッ",		ks_r		},

		{	"ど",		ks_c		},
		{	"れ",		ks_d		},
		{	"み",		ks_e		},
		{	"ふぁ",		ks_f		},
		{	"ふ",		ks_f		},
		{	"そ",		ks_g		},
		{	"ら",		ks_a		},
		{	"し",		ks_b		},
		{	"ん",		ks_r		},
		{	"っ",		ks_r		},

		{	"ハ",		ks_c		},
		{	"ニ",		ks_d		},
		{	"ホ",		ks_e		},
		{	"ヘ",		ks_f		},
		{	"ト",		ks_g		},
		{	"イ",		ks_a		},
		{	"ロ",		ks_b		}

	};

	unsigned	char	cData;
				char	sign = 0;

	while(MML->cRead() != '{'){
		if(MML->eof()){
			MML->Err(L"調号コマンド・ブロックの開始を示す{が見つかりません。");
		}
	}

	// } が来るまで、記述ブロック内をコンパイルする。
	while((cData = MML->GetChar()) != '}'){
		
		// } が来る前に、[EOF]が来たらエラー
		if( MML->eof() ){
			MML->Err(L"調号コマンド・ブロックの終端を示す`}'がありません。");
		}

		//１つ戻る
		MML->Back();

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
			case(ks_r):
				KeySignature[7] = sign;
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

			case(ks_0):
				nowKey		= 0;
				nowScale	= 0;
				SetKey(nowKey, nowScale);
				break;
			case(ks_s1):
				nowKey		= 1;
				nowScale	= 0;
				SetKey(nowKey, nowScale);
				break;
			case(ks_s2):
				nowKey		= 2;
				nowScale	= 0;
				SetKey(nowKey, nowScale);
				break;
			case(ks_s3):
				nowKey		= 3;
				nowScale	= 0;
				SetKey(nowKey, nowScale);
				break;
			case(ks_s4):
				nowKey		= 4;
				nowScale	= 0;
				SetKey(nowKey, nowScale);
				break;
			case(ks_s5):
				nowKey		= 5;
				nowScale	= 0;
				SetKey(nowKey, nowScale);
				break;
			case(ks_s6):
				nowKey		= 6;
				nowScale	= 0;
				SetKey(nowKey, nowScale);
				break;
			case(ks_f1):
				nowKey		= -1;
				nowScale	= 0;
				SetKey(nowKey, nowScale);
				break;
			case(ks_f2):
				nowKey		= -2;
				nowScale	= 0;
				SetKey(nowKey, nowScale);
				break;
			case(ks_f3):
				nowKey		= -3;
				nowScale	= 0;
				SetKey(nowKey, nowScale);
				break;
			case(ks_f4):
				nowKey		= -4;
				nowScale	= 0;
				SetKey(nowKey, nowScale);
				break;
			case(ks_f5):
				nowKey		= -5;
				nowScale	= 0;
				SetKey(nowKey, nowScale);
				break;
			case(ks_f6):
				nowKey		= -6;
				nowScale	= 0;
				SetKey(nowKey, nowScale);
				break;

			case(ks_m0):
				nowKey		= 0;
				nowScale	= 5;
				SetKey(nowKey, nowScale);
				break;
			case(ks_ms1):
				nowKey		= 1;
				nowScale	= 5;
				SetKey(nowKey, nowScale);
				break;
			case(ks_ms2):
				nowKey		= 2;
				nowScale	= 5;
				SetKey(nowKey, nowScale);
				break;
			case(ks_ms3):
				nowKey		= 3;
				nowScale	= 5;
				SetKey(nowKey, nowScale);
				break;
			case(ks_ms4):
				nowKey		= 4;
				nowScale	= 5;
				SetKey(nowKey, nowScale);
				break;
			case(ks_ms5):
				nowKey		= 5;
				nowScale	= 5;
				SetKey(nowKey, nowScale);
				break;
			case(ks_ms6):
				nowKey		= 6;
				nowScale	= 5;
				SetKey(nowKey, nowScale);
				break;
			case(ks_ms7):
				nowKey		= 7;
				nowScale	= 5;
				SetKey(nowKey, nowScale);
				break;
			case(ks_ms8):
				nowKey		= 8;
				nowScale	= 5;
				SetKey(nowKey, nowScale);
				break;
			case(ks_ms9):
				nowKey		= 9;
				nowScale	= 5;
				SetKey(nowKey, nowScale);
				break;



			case(ks_mf1):
				nowKey		= -1;
				nowScale	= 5;
				SetKey(nowKey, nowScale);
				break;
			case(ks_mf2):
				nowKey		= -2;
				nowScale	= 5;
				SetKey(nowKey, nowScale);
				break;
			case(ks_mf3):
				nowKey		= -3;
				nowScale	= 5;
				SetKey(nowKey, nowScale);
				break;
			case(ks_mf4):
				nowKey		= -4;
				nowScale	= 5;
				SetKey(nowKey, nowScale);
				break;
			case(ks_mf5):
				nowKey		= -5;
				nowScale	= 5;
				SetKey(nowKey, nowScale);
				break;
			case(ks_mf6):
				nowKey		= -6;
				nowScale	= 5;
				SetKey(nowKey, nowScale);
				break;

			default:
				MML->Err(L"調号 K{} コマンドの引数で未知の文字が指定されました。");
				break;
		}
	}

	//for Debug
/*
	wcout << L"[0] c = " << (int)KeySignature[0] << endl;
	wcout << L"[1] d = " << (int)KeySignature[1] << endl;
	wcout << L"[2] e = " << (int)KeySignature[2] << endl;
	wcout << L"[3] f = " << (int)KeySignature[3] << endl;
	wcout << L"[4] g = " << (int)KeySignature[4] << endl;
	wcout << L"[5] a = " << (int)KeySignature[5] << endl;
	wcout << L"[6] b = " << (int)KeySignature[6] << endl;
*/
}

//==============================================================
//		省略時の音長
//--------------------------------------------------------------
//	●引数
//		MMLfile*	MML		MMLファイルのオブジェクト
//	●返値
//		無し
//==============================================================
void	MusicTrack::SetLength(MMLfile* MML)
{
	DefaultLength		= MML->GetLength(DefaultLength);
	opt_DefaultLength	= DefaultLength;
	mml_general*	_event;

	switch(DefaultLength){
		case(96):
			_event = new mml_general(nsd_Length_96, L"Length 96");
			break;
		case(72):
			_event = new mml_general(nsd_Length_72, L"Length 72");
			break;
		case(48):
			_event = new mml_general(nsd_Length_48, L"Length 48");
			break;
		case(36):
			_event = new mml_general(nsd_Length_36, L"Length 36");
			break;
		case(32):
			_event = new mml_general(nsd_Length_32, L"Length 32");
			break;
		case(24):
			_event = new mml_general(nsd_Length_24, L"Length 24");
			break;
		case(18):
			_event = new mml_general(nsd_Length_18, L"Length 18");
			break;
		case(16):
			_event = new mml_general(nsd_Length_16, L"Length 16");
			break;
		case(12):
			_event = new mml_general(nsd_Length_12, L"Length 12");
			break;
		case(9):
			_event = new mml_general(nsd_Length_9,  L"Length 9");
			break;
		case(8):
			_event = new mml_general(nsd_Length_8,  L"Length 8");
			break;
		case(6):
			_event = new mml_general(nsd_Length_6,  L"Length 6");
			break;
		case(4):
			_event = new mml_general(nsd_Length_4,  L"Length 4");
			break;
		case(3):
			_event = new mml_general(nsd_Length_3,  L"Length 3");
			break;
		case(2):
			_event = new mml_general(nsd_Length_2,  L"Length 2");
			break;
		case(1):
			_event = new mml_general(nsd_Length_1,  L"Length 1");
			break;
		default:
			_event = new mml_general(nsd_Length, (unsigned char)DefaultLength, L"Length");
			break;
	}
	SetEvent(_event);
}

//==============================================================
//		ノートナンバー・臨時記号の解決
//--------------------------------------------------------------
//	●引数
//		MMLfile*	MML		MMLファイルのオブジェクト
//		int			note	ノート（0:C 1:D 2:E … 7:B）
//	●返値
//		char		
//==============================================================
char	MusicTrack::calc_note(MMLfile*	MML,int note)
{
	unsigned		char	cData = MML->GetChar();
	static	const	char	note_code[]={0,2,4,5,7,9,11};
					char	_key = note_code[note];

	//臨時記号
	//If Natural then skip
	if((cData != '=') && (cData != '*')){
		//ナチュラルじゃない場合
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
	return(_key);
}

//==============================================================
//		
//--------------------------------------------------------------
//	●引数
//		MMLfile*	MML		MMLファイルのオブジェクト
//	●返値
//				無し
//==============================================================
int		MusicTrack::calc_length(MMLfile* MML)
{
	unsigned	char	cData = MML->GetChar();
				int		Length;

	MML->Back();
	if(((cData >= '0') && (cData <= '9')) || (cData == '%') || (cData == '.') || (cData == '~') || ((MML->iTieMode == 1) && (cData == '^'))){
		Length = MML->GetLength(DefaultLength);
	} else {
		Length = -1;
	}
	return(Length);
}

//==============================================================
//		
//--------------------------------------------------------------
//	●引数
//		MMLfile*	MML		MMLファイルのオブジェクト
//	●返値
//				無し
//==============================================================
int		MusicTrack::calc_gate(MMLfile* MML)
{
	unsigned	char	cData = MML->GetChar();
				int		GateTime;

	//ゲートタイム
	if(cData == ','){
		cData = MML->GetChar();
		if(cData == '0'){
			GateTime = 0;
		} else {
			MML->Back();
			GateTime = MML->GetLength(DefaultLength);
		}
	} else {
		MML->Back();
		GateTime = -1;
	}
	return(GateTime);
}

//==============================================================
//		
//--------------------------------------------------------------
//	●引数
//		MMLfile*	MML		MMLファイルのオブジェクト
//	●返値
//				無し
//==============================================================
bool	MusicTrack::calc_slur(MMLfile* MML)
{
	unsigned	char	cData = MML->GetChar();
				bool	slur;

	if(cData == '&') {
		slur = true;
	} else {
		slur = false;
		MML->Back();
	}
	return(slur);
}

//==============================================================
//		EC*	疑似エコー解除
//--------------------------------------------------------------
//	●引数
//		MMLfile*	MML		MMLファイルのオブジェクト
//	●返値
//				無し
//==============================================================
void	MusicTrack::SetEcho(void)
{
	//ここで音量を戻す。
	EchoVolRet();

	echo_flag = false;
}

//==============================================================
//		EC	疑似エコー設定
//--------------------------------------------------------------
//	●引数
//		MMLfile*	MML		MMLファイルのオブジェクト
//	●返値
//				無し
//==============================================================
void	MusicTrack::SetEcho(MMLfile* MML)
{
	unsigned	char	cData;
				int		_value;
				int		_volume;

	//ここで音量を戻す。
	EchoVolRet();

	_value = MML->GetInt();
	if((_value<0) || (_value>255)){
		MML->Err(L"ECコマンドの第１パラメータは0～255の範囲で指定してください。");
	}

	cData = MML->GetChar();
	if(cData != ','){
		MML->Err(L"EC コマンドのパラメータが足りません。２つ指定してください。");
	}

	_volume = MML->GetInt();
	if((_volume<-1) || (_volume>15)){
		MML->Err(L"ECコマンドの第２パラメータは-1～15の範囲で指定してください。");
	}

	echo_flag = true;
	echo_value	= (unsigned char)_value;
	if(_volume == -1){
		echo_slur = true;
	} else {
		echo_slur = false;
		echo_volume	= (unsigned char)_volume;
	}

	cData = MML->GetChar();
	if(cData != ','){
		MML->Back();
		echo_length = -1;
	} else {
		echo_length	= MML->GetLength(DefaultLength);
	}
}

//==============================================================
//		疑似エコーのリセット
//--------------------------------------------------------------
//	●引数
//				無し
//	●返値
//				無し
//==============================================================
void	MusicTrack::ResetEcho()
{
	unsigned	int	i = 0;

	while(i < 256){
		oldNote[i] = -1;
		i++;
	}
//	echo_flag	= false;
	pt_oldNote	= 0;
}

//==============================================================
//	疑似エコーの生成
//--------------------------------------------------------------
//	●引数
//		int		Length
//		int		GateTime
//		bool	Slur
//	●返値
//		
//==============================================================
void	MusicTrack::GenerateEcho(MMLfile* MML, int Length, int GateTime, bool	Slur)
{
	char	old_note = oldNote[(pt_oldNote - echo_value) & 0xFF];
	char	now_note = oldNote[pt_oldNote];

	char	now_octave = octave;
	char	old_octave = (old_note / 12);

//	int		i = 0;

	CallPatch(MML, old_note);

	if(echo_slur == false){
		//Echo volume
		SetEvent(new mml_general(nsd_Volume + echo_volume, L"Echo Volume"));
		echo_vol_ret = true;
	} else {
		//前のノートが、音符の場合のみ
		if(echo_already == false){
			_old_note->SetTai();
		}
	}

	//Echo note
	if(old_octave < now_octave){
		while(old_octave < now_octave){
			old_octave++;
			SetEvent(new mml_general(nsd_Octave_Down_1, L"One time octave down"));
		}
	} else if(old_octave > now_octave){
		while(old_octave > now_octave){
			old_octave--;
			SetEvent(new mml_general(nsd_Octave_Up_1, L"One time octave up"));
		}
	}

	if(Length == opt_DefaultLength){
		Length = -1;
	}
	_old_note = new mml_note(old_note % 12, Length, GateTime, Slur, L"Echo Note");
	SetEvent(_old_note);

//	EchoVolRet();

	echo_already	= true;

}

//==============================================================
//		疑似エコーの音量復帰
//--------------------------------------------------------------
//	●引数
//				無し
//	●返値
//				無し
//==============================================================
void	MusicTrack::EchoVolRet()
{
	//ここで音量を戻す。
	if(echo_vol_ret == true){
		//volume return
		SetEvent(new mml_general(nsd_Volume + volume, L"Volume"));
		echo_vol_ret = false;
	}
}


//==============================================================
//		音符のイベント作成
//--------------------------------------------------------------
//	●引数
//		int		_key		ノートナンバー
//		int		Length		音長
//		int		GateTime	ゲートタイム
//		bool	Slur		スラー
//	●返値
//				無し
//==============================================================
void	MusicTrack::SetNote(MMLfile* MML, int _key, int Length, int GateTime, bool Slur)
{
	int		Length_0	= Length;		//実音長
	char	_note_no;

	//実音長の取得
	if(Length_0 == -1){
		Length_0 = DefaultLength;
	}

	//クオンタイズ
	if(gatetime_Q != QMax){
		Set_q((Length_0 * (QMax - gatetime_Q)) / QMax);
	}

	//移調
	_key	+= iKeyShift;
	_note_no = ((char)_key + (octave + octave1)*12) & 0xFF;

	//疑似エコーのバッファ書き込み
	pt_oldNote++;
	oldNote[pt_oldNote]	= _note_no;
	octave1_old			= octave1;
	octave1				= 0;

	echo_already		= false;

	//ここで音量を戻す。
	if(echo_flag == true){
		EchoVolRet();
	}

	//指定できる範囲を超えた場合。
	while(_key < 0){
		_key += 12;
		SetEvent(new mml_general(nsd_Octave_Down_1, L"One time octave down"));
	}

	while(_key >= 12){
		_key -= 12;
		SetEvent(new mml_general(nsd_Octave_Up_1, L"One time octave up"));
	}

	CallPatch(MML, _note_no);

	if((echo_flag == true) && (oldNote[(pt_oldNote - echo_value) & 0xFF] != -1) && (echo_length != -1) && (Length_0 > echo_length) && (Slur == false)){
		//疑似エコー あり

		Length_0 -= echo_length;
		if(Length_0 == opt_DefaultLength){
			Length_0 = -1;
		}

		_old_note = new mml_note(_key, Length_0, GateTime, Slur, L"Note");

		//イベントオブジェクトの作成
		SetEvent(_old_note);

		GenerateEcho(MML,echo_length,GateTime,Slur);
	} else {
		if(Length == opt_DefaultLength){
			Length = -1;
		}
		//疑似エコー なし
		_old_note = new mml_note(_key, Length, GateTime, Slur, L"Note");

		//イベントオブジェクトの作成
		SetEvent(_old_note);

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
	int		_key		= calc_note(MML, note);
	int		Length		= calc_length(MML);
	int		GateTime	= calc_gate(MML);
	bool	Slur		= calc_slur(MML);

	if(jump_flag==false){
		SetNote(MML, _key, Length, GateTime, Slur);
	}

}

//==============================================================
//		休符のイベント作成
//--------------------------------------------------------------
//	●引数
//		MMLfile*	MML		MMLファイルのオブジェクト
//		int			mode	
//	●返値
//				無し
//==============================================================
void	MusicTrack::SetRest(MMLfile*	MML, int mode)
{
					char	old_note = oldNote[(pt_oldNote - echo_value) & 0xFF];
	
	unsigned		char	cData;
	unsigned		char	_code = 0x0F;
					int		Length = -1;
					int		GateTime = -1;
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
		case('='):
		case('*'):
			_code = 0x0F;
			break;
		default:
			MML->Back();
			if(mode & 0x80){
				_code = 0x0D + ((char)mode & 0x03);
			} else {
				switch(KeySignature[7]){
					case(-1):
						_code = 0x0D;
						break;
					case(+1):
						_code = 0x0E;
						break;
					default:
						_code = 0x0D + ((char)mode & 0x03);
						break;
				}
			}
			break;
	}

	Length		= calc_length(MML);		//長さ
	GateTime	= calc_gate(MML);		//ゲートタイム or タイ
	Slur		= calc_slur(MML);		//タイ・スラー

	if(jump_flag==false){
		//クオンタイズ
		if(gatetime_Q != QMax){
			Set_q((Length * (QMax - gatetime_Q)) / QMax);
		}

		if((echo_flag == false) || (_code != 0x0F) || (old_note == -1) || (echo_length != -1)){
			if(Length == opt_DefaultLength){
				Length = -1;
			}
			_old_note = new mml_note(_code, Length, GateTime, Slur, L"Rest");
			SetEvent(_old_note);
		} else {
			GenerateEcho(MML,Length,GateTime,Slur);
		}
	}
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
	if(jump_flag==false){
		_old_note->SetTai();
	}
	SetRest(MML,2);
}

//==============================================================
//			ポルタメント（簡素版）
//--------------------------------------------------------------
//	●引数
//		MMLfile*	MML		MMLファイルのオブジェクト
//		char		iTempo	テンポ
//	●返値
//		無し
//==============================================================
void	MusicTrack::SetProtament(MMLfile* MML, unsigned char iTempo)
{
	unsigned	char	cData;

				int		o_rel	= 0;
				int		o_rel2	= 0;

				int		iNote_2;	//ノートナンバー
				int		note_2;		//音名code
				int		_key_2;		//音名
				int		Length_2;	//音長
				int		GateTime_2;	//ゲートタイム
				bool	Slur_2;		//スラー

				int		iNote;		//ノートナンバー
				int		note;		//音名code
				int		_key;		//音名
				int		Length;		//音長
				int		GateTime;	//ゲートタイム
				bool	Slur;		//スラー

	//------
	//ポルタメント元の音符
	cData = MML->GetChar();
	switch(cData){
		case('c'):
			note	= 0;
			break;
		case('d'):
			note	= 1;
			break;
		case('e'):
			note	= 2;
			break;
		case('f'):
			note	= 3;
			break;
		case('g'):
			note	= 4;
			break;
		case('a'):
			note	= 5;
			break;
		case('b'):
			note	= 6;
			break;
		default:
			MML->Err(L"a,b,c～gの音名を記述してください。");
			break;
	}

	_key		= calc_note(MML, note);
	Length		= calc_length(MML);
	GateTime	= calc_gate(MML);
	Slur		= calc_slur(MML);

	//ノートナンバーの取得
	iNote=		((char)_key + (octave + octave1)*12) & 0xFF;

	//------
	//オクターブ処理
	cData = MML->GetChar();
	while((cData == '>') || (cData == '<') || (cData == '`') || (cData == '"')){
		switch(cData){
			case('<'):
				o_rel--;
				break;
			case('>'):
				o_rel++;
				break;
			case('`'):
				o_rel2++;
				break;
			case('"'):
				o_rel2--;
				break;
			default:
				break;
		}
		cData = MML->GetChar();	//次の文字
	}
	//臨時記号ではなくなったので、ポインタを戻す。
	MML->Back();

	//------
	//ポルタメント先の音符
	cData = MML->GetChar();
	switch(cData){
		case('c'):
			note_2	= 0;
			break;
		case('d'):
			note_2	= 1;
			break;
		case('e'):
			note_2	= 2;
			break;
		case('f'):
			note_2	= 3;
			break;
		case('g'):
			note_2	= 4;
			break;
		case('a'):
			note_2	= 5;
			break;
		case('b'):
			note_2	= 6;
			break;
		default:
			MML->Err(L"a,b,c～gの音名を記述してください。");
			break;
	}

	_key_2		= calc_note(MML, note_2);
	iNote_2		= ((char)_key_2 + (octave + octave1 + o_rel + o_rel2)*12) & 0xFF;

	//------
	//ポルタメント終了
	cData = MML->GetChar();
	if(cData != '}'){
		MML->Err(L"ポルタメントコマンドは、}で閉じて下さい。");
	}

	Length_2	= calc_length(MML);
	if(Length_2 != -1){
		if(Length != -1){
			MML->Err(L"ポルタメントコマンドで、音長が2箇所に記述されています。");
		} else {
			Length = Length_2;
		}
	}

	GateTime_2	= calc_gate(MML);
	if(GateTime_2 != -1){
		if(GateTime != -1){
			MML->Err(L"ポルタメントコマンドで、ゲートタイムが2箇所に記述されています。");
		} else {
			GateTime = GateTime_2;
		}
	}

	Slur_2		= calc_slur(MML);
	if(Slur_2 == true){
		Slur = true;
	}

	//------
	//ポルタメントの計算
	int		Length_0	= Length;		//実音長

	int	pol_length;
	int pol_target;
	int	pol_rate;
	int	pol_depth;

	//実音長の取得
	if(Length_0 == -1){
		Length_0 = DefaultLength;
	}

	pol_length	= (Length_0 * 150) / iTempo;
	pol_target	= iNote_2 - iNote;

	if(abs(pol_target) * 16 >= pol_length){
		pol_rate	= 1;
		pol_depth	= (pol_target * 16) / pol_length;
		if((pol_depth * pol_length) != (pol_target * 16)){
			if(pol_target>=0){
				pol_depth++;
			} else {
				pol_depth--;
			}
		}
	} else {
		if(pol_target < 0){
			pol_rate	= pol_length / (-pol_target * 16);
			pol_depth	= -1;
		} else {
			pol_rate	= pol_length / (+pol_target * 16);
			pol_depth	= 1;
		}
	}

//	printf("length :%d\n", pol_length);
//	printf("Rate   :%d\n", pol_rate);
//	printf("Depth  :%d\n", pol_depth);
//	printf("Target :%d\n", pol_target);

	//------
	//オブジェクトの書き込み
	if(jump_flag == false){

		//ポルタメント
		SetEvent(new mml_general(nsd_Portamento, (unsigned char)1, (unsigned char)pol_rate, (unsigned char)pol_depth, (unsigned char)pol_target, L"Portamento"));

		//ポルタメント元の音符
		SetNote(MML, _key, Length, GateTime, Slur);
	}

	//オクターブコマンドの書き出し
	while( o_rel > 0){
		if(MML->octave_reverse == true){
			SetOctaveDec();
		} else {
			SetOctaveInc();
		}
		o_rel--;
	}
	while( o_rel < 0){
		if(MML->octave_reverse == true){
			SetOctaveInc();
		} else {
			SetOctaveDec();
		}
		o_rel++;
	}

}

//==============================================================
//			ポルタメント（Pコマンド版）
//--------------------------------------------------------------
//	●引数
//		MMLfile*	MML		MMLファイルのオブジェクト
//	●返値
//		無し
//==============================================================
void	MusicTrack::SetProtament(MMLfile* MML)
{
	unsigned	char	cData;

				int		_Decay;
				int		_Rate;
				int		_Depth;
				int		_Target;

	_Decay = MML->GetInt();
	if( (_Decay < 0) || (_Decay > 255) ){
		MML->Err(L"ポルタメントの第1パラメータは0～255の範囲で指定してください。");
	}
	_Decay++;

	cData = MML->GetChar();
	if(cData != ','){
		MML->Err(L"P コマンドのパラメータが足りません。４つ指定してください。");
	}

	_Rate = MML->GetInt();
	if( (_Rate < 1) || (_Rate > 256) ){
		MML->Err(L"ポルタメントの第2パラメータは1～256の範囲で指定してください。");
	}

	cData = MML->GetChar();
	if(cData != ','){
		MML->Err(L"P コマンドのパラメータが足りません。４つ指定してください。");
	}

	_Depth = MML->GetInt();
	if( (_Depth < -128) || (_Depth > 127) ){
		MML->Err(L"ポルタメントの第3パラメータは-128～127の範囲で指定してください。");
	}
	_Decay++;

	cData = MML->GetChar();
	if(cData != ','){
		MML->Err(L"P コマンドのパラメータが足りません。４つ指定してください。");
	}

	_Target = MML->GetInt();
	if( (_Target < -128) || (_Target > 127) ){
		MML->Err(L"ポルタメントの第4パラメータは-128～127の範囲で指定してください。");
	}

	if(jump_flag == false){
        SetEvent(new mml_general(nsd_Portamento, (unsigned char)_Decay, (unsigned char)_Rate, (unsigned char)_Depth, (unsigned char)_Target, L"Portamento"));
	}
}

//==============================================================
//		移調２
//--------------------------------------------------------------
//	●引数
//		MMLfile*	MML		MMLファイルのオブジェクト
//	●返値
//		無し
//==============================================================
void	MusicTrack::SetKeyShift(MMLfile* MML)
{
	iKeyShift = MML->GetInt();
	if( (iKeyShift < -128) || (iKeyShift > 127) ){
		MML->Err(L"キーシフトは-127～128の範囲で指定してください。");
	}
}

//==============================================================
//		移調２
//--------------------------------------------------------------
//	●引数
//		MMLfile*	MML		MMLファイルのオブジェクト
//	●返値
//		無し
//==============================================================
void	MusicTrack::SetKeyShift_Relative(MMLfile* MML)
{
	iKeyShift += MML->GetInt();
	if( (iKeyShift < -128) || (iKeyShift > 127) ){
		MML->Err(L"キーシフトの範囲が-127～128を超えました。");
	}

}

//==============================================================
//		移調１
//--------------------------------------------------------------
//	●引数
//		MMLfile*	MML		MMLファイルのオブジェクト
//	●返値
//		無し
//==============================================================
void	MusicTrack::SetTranspose(int _no)
{
	if((iTranspose != _no) || (f_opt_Key == false)){
		f_opt_Key	= true;
		iTranspose	= _no;
		SetEvent(new mml_general(nsd_Transpose, iTranspose, L"Transpose"));
	}
}

//==============================================================
//		移調１
//--------------------------------------------------------------
//	●引数
//		MMLfile*	MML		MMLファイルのオブジェクト
//	●返値
//		無し
//==============================================================
void	MusicTrack::SetTranspose_Relative(int _no)
{
	iTranspose += _no;
	SetEvent(new mml_general(nsd_Relative_Transpose, _no, L"Relative Transpose"));
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
	octave = MML->GetInt() - 1;

	if( (octave <= 7) && (octave >=0) ){
		if(opt_octave != octave){
			SetEvent(new mml_general(nsd_Octave + (unsigned char)octave, L"Octave"));
			opt_octave = octave;
		}
	} else {
		MML->Err(L"オクターブは1～8の範囲で指定してください。");
	}
}

//--------------------------------------------------------------
void	MusicTrack::SetOctaveInc()
{
	SetEvent(new mml_general(nsd_Octave_Up, L"Octave Up"));
	octave++;
	if(opt_octave != -1){
		opt_octave = octave;
	}
}

//--------------------------------------------------------------
void	MusicTrack::SetOctaveDec()
{
	SetEvent(new mml_general(nsd_Octave_Down, L"Octave Down"));
	octave--;
	if(opt_octave != -1){
		opt_octave = octave;
	}
}

//--------------------------------------------------------------
void	MusicTrack::SetOctaveOne_Inc()
{
	if(jump_flag == false){
		SetEvent(new mml_general(nsd_Octave_Up_1, L"One time octave up"));
		octave1++;
	}
}

//--------------------------------------------------------------
void	MusicTrack::SetOctaveOne_Dec()
{
	if(jump_flag == false){
		SetEvent(new mml_general(nsd_Octave_Down_1, L"One time octave down"));
		octave1--;
	}
}


