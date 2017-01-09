/*******************************************************************************

			NES Sound Driver & Library	(NSD.lib)	MML Compiler

	Copyright (c) 2012 A.Watanabe (S.W.), All rights reserved.
	 For conditions of distribution and use, see copyright notice in "nsc.cpp".

*******************************************************************************/

#pragma once

/****************************************************************/
/*																*/
/*			構造体定義											*/
/*																*/
/****************************************************************/
class NSD_WORK{

public:
	int	gatemode;
	int	length;
	int	gate_q;
	int	gate_u;

	int	octave;
	int	octave1;		//One time octave
	int	detune_cent;
	int	detune_fine;

	int	trans;

	int	voice;
	int	volume;

	int	env_volume;
	int	env_voice;
	int	env_frequency;
	int	env_note;

	//APU
	int	sweep;
	int	apu_tri_time;

	//FDS
	int	fds_frequency;
	int	fds_sweepbias;

	//N163
	int	n163_num;
	int	n163_frequency;

	//PSG
	int	psg_switch;

			NSD_WORK(void){};
			~NSD_WORK(void){};

	void	init(void){
		//音源ドライバ初期値を設定
		gatemode		= -1;
		length			= 24;
		gate_q			= 0;
		gate_u			= 0;
		octave			= 3;
		octave1			= 0;
		detune_cent		= 0;
		detune_fine		= 0;
		trans			= 0;
		voice			= -1;
		volume			= 15;
		env_volume		= -1;
		env_voice		= -1;
		env_frequency	= -1;
		env_note		= -1;
		sweep			= -1;
		apu_tri_time	= -1;
		fds_frequency	= -1;
		fds_sweepbias	= -1;
		n163_num		= -1;
		n163_frequency	= -1;
		psg_switch		= -1;
	}

	void	set(NSD_WORK* work){
		gatemode		=	work->gatemode;
		length			=	work->length;
		gate_q			=	work->gate_q;
		gate_u			=	work->gate_u;
		octave			=	work->octave;
		octave1			=	work->octave1;
		detune_cent		=	work->detune_cent;
		detune_fine		=	work->detune_fine;
		trans			=	work->trans;
		voice			=	work->voice;
		volume			=	work->volume;
		env_volume		=	work->env_volume;
		env_voice		=	work->env_voice;
		env_frequency	=	work->env_frequency;
		env_note		=	work->env_note;
		sweep			=	work->sweep;
		apu_tri_time	=	work->apu_tri_time;
		fds_frequency	=	work->fds_frequency;
		fds_sweepbias	=	work->fds_sweepbias;
		n163_num		=	work->n163_num;
		n163_frequency	=	work->n163_frequency;
		psg_switch		=	work->psg_switch;
	}

	void	get(NSD_WORK* work){work->set(this);};
};
/****************************************************************/
/*																*/
/*			クラス定義											*/
/*																*/
/****************************************************************/
class MusicTrack :
	public MusicItem
{
//メンバー変数
public:
	//----------------------------------
	//音源ドライバ　シミュレート用WORK
	NSD_WORK	nsd;

private:
	//----------------------------------
	//Tick カウント用
				int		iTickTotal;
				int		iTickLoop;

	//----------------------------------
	//コンパイル制御
	unsigned	int		offset_now;				//現在のオフセット
				bool	compile_flag;			//現在コンパイル中？

				bool	jump_flag;				// J

	//----------------------------------
	//音長
	//			int		DefaultLength;			//l
				int		opt_DefaultLength;
	
	//----------------------------------
	//クオンタイズ
				int		QMax;					//QMax
				int		gatetime_q;				//q
				int		gatetime_Q;				//Q
				int		opt_gatetime_q;			//
				int		opt_gatetime_u;			//

	//----------------------------------
	//音量
//				char	volume;					//現在の音量
				int		opt_volume;

	//----------------------------------
	//オクターブ
//				char	octave;					//現在のオクターブ
//				char	octave1;				//一次的な相対オクターブ　計算用
				int		opt_octave;

	//----------------------------------
	//ノート
	mml_note*			_old_note;

	//----------------------------------
	//調号制御用
				char	KeySignature[8];		//調号(c,d,e,f,g,a,b,r)
				char	nowKey;					//現在の調
				char	nowScale;				//現在のスケール（モード）
				
	//----------------------------------
	//移調
				int		iKeyShift;				//k
				
//				int		iTranspose;				//_

	//----------------------------------
	//疑似エコー
				bool	echo_already;			//既に生成したか？
				bool	echo_vol_ret;			//エコーの音量を元に戻したか？

				bool	echo_flag;				//疑似エコー フラグ
				bool	echo_slur;				//疑似エコー スラーでつなげる？
				int		echo_length;			//疑似エコー 長さ
	unsigned	char	echo_volume;			//疑似エコー 音量
	unsigned	char	echo_value;				//疑似エコー 何前？
				char	oldNote[256];			//疑似エコー用リングバッファ
	unsigned	char	pt_oldNote;				//疑似エコー用リングバッファ　ポインタ

	//----------------------------------
	//パッチ
				bool	f_Patch;				//パッチ処理中？
	unsigned	int		i_Patch;

	//現在の状態（設定数値）
//	unsigned	int		iVoi;			//
//	unsigned	int		iEvoi;			//
//	unsigned	int		iEvol;			//
//	unsigned	int		iEm;			//
//	unsigned	int		iEn;			//
	unsigned	char	iSweep;			//
	unsigned	int		iSub;			//サブルーチン用

	//現在の状態（エンベロープのsw）
				bool	sw_Evoi;		//
				bool	sw_Evol;		//
				bool	sw_Em;			//
				bool	sw_En;			//

	//設定するかどうか（defailt = false）
				bool	f_opt_Voi;		//
				bool	f_opt_Evoi;		//
				bool	f_opt_Evol;		//
				bool	f_opt_Em;		//
				bool	f_opt_En;		//
				bool	f_opt_Key;		//
				bool	f_opt_Sweep;	//
				bool	f_opt_Sub;		//サブルーチン（パッチ用）

	//----------------------------------
	//無限ループ
				bool	loop_flag;				// L コマンド出現したか？
	unsigned	int		offset_loop;			// L コマンドのオフセット

	//----------------------------------
	//リピート関係
	mml_Address*		_old_repeatA_Branch;
	mml_repeat*			_old_repeat;

	unsigned	int		offset_repeat_a_s;		//前回の [  コマンドのオフセット
	unsigned	int		offset_repeat_a_b;		//前回の :  コマンドのオフセット
	unsigned	int		offset_repeat_b_s;		//前回の |: コマンドのオフセット
	unsigned	int		offset_repeat_b_b;		//前回の \  コマンドのオフセット
				int		count_repeat_a;

			vector<	int			>			repeat_type;		//どのリピートを使っているか？
			vector<	int			>::iterator	it_repeat_type;

			unsigned	int					sp_repeat_c;		//リピート(C)のスタックポインタ
	list<	unsigned	int				>	st_ct_repeat_c;
	list<	list<	MusicItem*>::iterator>	st_it_repeat_c_s;
	list<	list<	MusicItem*>::iterator>	st_it_repeat_c_b;
	list<	list<	MusicItem*>::iterator>	st_it_repeat_c_e;
	list<	unsigned	int				>::iterator		it_ct_repeat_c;
	list<	list<	MusicItem*>::iterator>::iterator	it_it_repeat_c_s;
	list<	list<	MusicItem*>::iterator>::iterator	it_it_repeat_c_b;
	list<	list<	MusicItem*>::iterator>::iterator	it_it_repeat_c_e;

	//----------------------------------
	//オブジェクト
	vector<	mml_Address*	>	ptcFDSC;		//FDS Carrer
	vector<	mml_Address*	>	ptcFDSM;		//FDS Modlator
	vector<	mml_Address*	>	ptcOPLL;		//VRC7, OPLL
	vector<	mml_Address*	>	ptcWave;		//N163
	vector<	mml_Address*	>	ptcSE;			//効果音コマンド一覧
	vector<	mml_Address*	>	ptcSub;			//サブルーチンコマンド一覧
	vector<	mml_Address*	>	ptcEnv;			//エンベロープコマンド一覧


//メンバー関数
public:
			MusicTrack(MMLfile* MML, const _CHAR _strName[] = _T("==== [ Music Track ]===="));
			~MusicTrack(void);

	unsigned	int		TickCount(MusicFile* MUS, NSD_WORK* work);
	unsigned	int		TickCount(MusicFile* MUS);
	unsigned	int		GetTickTotal(void){	return(iTickTotal);};
	unsigned	int		GetTickLoop(void){	return(iTickLoop);};

				void	OptimizeDefineCheck(MusicFile* MUS);
				void	Fix_Address(MusicFile* MUS);
				void	SetEvent(MusicItem* _item);		//イベントの追加

				//このトラックをコンパイルするかどうか
				bool	GetCompileFlag(void){return(compile_flag);};
				void	SetCompileFlag(bool _flag){compile_flag = _flag;};

				//----------------------------------
				//このトラックにだけ効くＭＭＬコマンド
				size_t	SetEnd(MMLfile* MML);			//記述ブロック終了
				void	SetLoop();						//無限ループ

				void	SetJump(MMLfile* MML);			//ジャンプ

				void	SetRepeat_Start(MMLfile* MML);
				void	SetRepeat_End(MMLfile* MML);
				void	SetRepeat_Branch(MMLfile* MML);

				void	SetRepeat_A_Start(MMLfile* MML);
				void	SetRepeat_A_End(MMLfile* MML);
				void	SetRepeat_C_Start(MMLfile* MML);
				void	SetRepeat_C_End(MMLfile* MML);

				void	SetRepeat_B_Start();
				void	SetRepeat_B_Branch(MMLfile* MML);
				void	SetRepeat_B_End(MMLfile* MML);
		mml_Address*	CopyAddressEvent(unsigned char cOpCode, string* sOpCode, list<MusicItem*>::iterator pt_itMusic);
				void	CopyEnvEvent(unsigned char cOpCode, string* sOpCode, list<MusicItem*>::iterator pt_itMusic);

				void	SetSE(MMLfile* MML);
				void	SetSubroutine(unsigned int _no);
				void	SetSubWithParch(unsigned int _no,bool _f);

				void	SetPatch(MMLfile* MML);	
				void	SetPatch();				//@P off
				void	CallPatch(MMLfile* MML, char _note);

				void	SetEnvelop_Evoi(unsigned int _no);
				void	SetEnvelop_Evol(unsigned int _no);
				void	SetEnvelop_Em(unsigned int _no);
				void	SetEnvelop_En(unsigned int _no);
				void	SetVoice(unsigned int _no);		//E@ off
				void	SetEnvelop_Evol();				//Ev off
				void	SetEnvelop_Em();				//Em off
				void	SetEnvelop_En();				//En off

				void	SetSweep(MMLfile* MML);
				void	SetSweep(unsigned char _c);

				void	SetFDSC(MMLfile* MML);			//@FC
				void	SetFDSM(MMLfile* MML);			//@FM
				void	SetVRC7(MMLfile* MML);			//@V
				void	SetN163(MMLfile* MML);			//@N
				void	SetN163_Load(MMLfile* MML);		//@NL
				void	SetN163_Set(MMLfile* MML);		//@NS

				void	Set_q(int i);
				void	Set_u(int i);
				void	SetGatetime_Q(MMLfile* MML);
				void	SetGatetime(MMLfile* MML);
				void	SetGatetime_u(MMLfile* MML);

				void	SetReleaseMode(MMLfile* MML);
				void	SetReleaseVoice(MMLfile* MML);
				void	SetReleaseVolume(MMLfile* MML);

				void	SetKeyFlag(char _c, char _d, char _e, char _f, char _g, char _a, char _b);
				void	SetKey(int _key, int _scale);

				void	SetMajor();
				void	SetMinor();
				void	SetHMinor(MMLfile* MML);
				void	SetMMinor(MMLfile* MML);
				void	SetScale(MMLfile* MML);
				void	SetKeySignature(MMLfile* MML);	//調号の設定

				void	SetEcho(void);
				void	SetEcho(MMLfile* MML);
				void	SetEchoBuffer(MMLfile* MML,int note);
				void	ResetEcho();
				void	EchoVolRet();
				void	GenerateEcho(MMLfile* MML, int Length, int GateTime, bool	Slur);
				char	calc_note(MMLfile*	MML,int note);
				int		calc_length(MMLfile* MML);
				int		calc_gate(MMLfile* MML);
				bool	calc_slur(MMLfile* MML);
				void	SetNote(MMLfile* MML, int _key, int Length, int GateTime, bool Slur);
				void	SetNote(MMLfile* MML, int note);
				void	SetRest(MMLfile* MML, int mode);
				void	SetTai(MMLfile* MML);
				void	SetLength(MMLfile* MML);

				void	SetProtament(MMLfile* MML);
				void	SetProtament(MMLfile* MML, unsigned char iTempo);

				void	SetKeyShift(MMLfile* MML);
				void	SetKeyShift_Relative(MMLfile* MML);

				void	SetTranspose(int _no);
				void	SetTranspose_Relative(int _no);

				void	SetOctave(MMLfile* MML);
				void	SetOctaveInc();
				void	SetOctaveDec();
				void	SetOctaveOne_Inc();
				void	SetOctaveOne_Dec();
		
				void	SetVolume(MMLfile* MML);
				void	SetVolumeInc(MMLfile* MML);
				void	SetVolumeDec(MMLfile* MML);

//	unsigned	int		GetDefaultLength(void){return(nsd.length);};

				void	Reset_opt(void){
					opt_octave			= -1;
					opt_volume			= -1;
					opt_gatetime_q		= -1;
					opt_gatetime_u		= -1;
					opt_DefaultLength	= -1;
					f_opt_Voi			= false;	//
					f_opt_Evoi			= false;	//
					f_opt_Evol			= false;	//
					f_opt_Em			= false;	//
					f_opt_En			= false;	//
					f_opt_Key			= false;	//
					f_opt_Sweep			= false;	//
					f_opt_Sub			= false;	//
			//		if((echo_flag == true) && (echo_slur == false)){
			//			echo_vol_ret	= true;		//
			//		}
				}
};
