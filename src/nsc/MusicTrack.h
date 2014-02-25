#pragma once

/****************************************************************/
/*																*/
/*			クラス定義											*/
/*																*/
/****************************************************************/
class MusicTrack :
	public MusicItem
{
//メンバー変数
private:

				bool	compile_flag;		//現在コンパイル中？
	unsigned	int		offset_now;			//現在のオフセット

				bool	loop_flag;
	unsigned	int		offset_loop;		// L コマンドのオフセット
	unsigned	int		offset_repeat_a_s;	//前回の [  コマンドのオフセット
	unsigned	int		offset_repeat_a_b;	//前回の :  コマンドのオフセット
	unsigned	int		offset_repeat_b_s;	//前回の |: コマンドのオフセット
	unsigned	int		offset_repeat_b_b;	//前回の \  コマンドのオフセット
				int		count_repeat_a;
	unsigned	int		DefaultLength;

				bool	echo_flag;			//疑似エコー フラグ
				bool	echo_slur;			//疑似エコー スラーでつなげる？
				int		echo_length;		//疑似エコー 長さ
	unsigned	char	echo_volume;		//疑似エコー 音量
	unsigned	char	echo_value;			//疑似エコー 何前？
				char	oldNote[256];		//疑似エコー用バッファ

	unsigned	char	pt_oldNote;			//前回の音程
				char	volume;				//音量
				char	octave;				//オクターブ
				char	octave1;			//
				char	octave1_old;		//

	unsigned	int		gatetime_q;
	unsigned	int		gatetime_Q;
	unsigned	int		QMax;

				int		opt_DefaultLength;
				int		opt_gatetime_q;			//
				int		opt_gatetime_u;			//
				int		opt_volume;

				bool	jump_flag;			//ジャンプフラグ

	//調号制御用
				char	KeySignature[8];	//調号(c,d,e,f,g,a,b,r)
				char	nowKey;				//現在の調
				char	nowScale;			//現在のスケール（モード）
				
	mml_note*			_old_note;
	mml_Address*		_old_repeatA_Branch;
	mml_repeat*			_old_repeat;

	vector<	mml_Address*	>	ptcFDSC;		//FDS Carrer
	vector<	mml_Address*	>	ptcFDSM;		//FDS Modlator
	vector<	mml_Address*	>	ptcOPLL;		//VRC7, OPLL
	vector<	mml_Address*	>	ptcWave;		//N163
	vector<	mml_Address*	>	ptcSE;			//効果音コマンド一覧
	vector<	mml_Address*	>	ptcSub;			//サブルーチンコマンド一覧
	vector<	mml_Address*	>	ptcEnv;			//エンベロープコマンド一覧

//メンバー関数
public:
			MusicTrack(MMLfile* MML, const wchar_t _strName[] = L"==== [ Music Track ]====");
			~MusicTrack(void);

				void	Fix_Address(MusicFile* MUS);
				void	SetEvent(MusicItem* _item);		//イベントの追加

				bool	GetCompileFlag(void){return(compile_flag);};
				void	SetCompileFlag(bool _flag){compile_flag = _flag;};

				size_t	SetEnd(void);
				void	SetLoop();

				void	SetRepeat_A_Start(MMLfile* MML);
				void	SetRepeat_A_Branch(MMLfile* MML);
				void	SetRepeat_A_End(MMLfile* MML);
				void	SetRepeat_B_Start();
				void	SetRepeat_B_Branch(MMLfile* MML);
				void	SetRepeat_B_End(MMLfile* MML);

				void	SetSE(MMLfile* MML);
				void	SetSubroutine(MMLfile* MML);
				void	SetEnvelop(unsigned char _opcode, MMLfile* MML, int _offset);
				void	SetFDSC(MMLfile* MML);
				void	SetFDSM(MMLfile* MML);
				void	SetVRC7(MMLfile* MML);
				void	SetN163(MMLfile* MML);

				void	SetEcho(void);
				void	SetEcho(MMLfile* MML);

				void	SetJump(MMLfile* MML);

				void	Set_q(int i);
				void	SetGatetime_Q(MMLfile* MML);
				void	SetGatetime(MMLfile* MML);
				void	SetGatetime_u(MMLfile* MML);

				void	SetKeyFlag(char _c, char _d, char _e, char _f, char _g, char _a, char _b);
				void	SetKey(int _key, int _scale);

				void	SetMajor();
				void	SetMinor();
				void	SetHMinor(MMLfile* MML);
				void	SetMMinor(MMLfile* MML);
				void	SetScale(MMLfile* MML);
				void	SetKeySignature(MMLfile* MML);	//調号の設定

				void	ResetEcho();
				void	GenerateEcho(int Length, int GateTime, bool	Slur);
				void	SetNote(MMLfile* MML, int note);
				void	SetRest(MMLfile* MML, int mode);
				void	SetTai(MMLfile* MML);
				void	SetLength(MMLfile* MML);

				void	SetOctave(MMLfile* MML);
				void	SetOctaveInc();
				void	SetOctaveDec();
				void	SetOctaveOne_Inc();
				void	SetOctaveOne_Dec();
		
				void	SetVolume(char _v){volume = _v;	opt_volume = volume;};
				void	IncVolume(void){	volume++;	if(volume>15){volume = 15;}	opt_volume = volume;	};
				void	DecVolume(void){	volume--;	if(volume<0){volume = 0;}	opt_volume = volume;	};
	unsigned	int		GetDefaultLength(void){return(DefaultLength);};

				void	Reset_opt(void){
					opt_volume			= -1;
					opt_gatetime_q		= -1;
					opt_gatetime_u		= -1;
					opt_DefaultLength	= -1;
				}
				int		Get_opt_volume(void){		return(opt_volume);	};
};
