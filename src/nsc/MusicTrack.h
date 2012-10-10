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
	unsigned	int		offset_now;			//現在のオフセット
	unsigned	int		offset_loop;		// L コマンドのオフセット
	unsigned	int		offset_repeat_a_s;	//前回の [  コマンドのオフセット
	unsigned	int		offset_repeat_a_b;	//前回の :  コマンドのオフセット
	unsigned	int		offset_repeat_b_s;	//前回の |: コマンドのオフセット
	unsigned	int		offset_repeat_b_b;	//前回の \  コマンドのオフセット
	unsigned	int		DefaultLength;
				bool	loop_flag;

				char	KeySignature[8];	//調号(c,d,e,f,g,a,b,r)

	mml_note*			_old_note;
	mml_Address*		_old_repeatA_Branch;

	vector<	mml_Address*	>	ptcOPLL;		//VRC7
	vector<	mml_Address*	>	ptcWave;		//VRC7
	vector<	mml_Address*	>	ptcSubroutine;	//サブルーチンコマンド一覧
	vector<	mml_Address*	>	ptcEnvelop;		//エンベロープコマンド一覧

//メンバー関数
public:
			MusicTrack(const char _strName[] = "==== [ Music Track ]====");
			~MusicTrack(void);

				void	SetEvent(MusicItem* _item);		//イベントの追加

				void	Fix_Address(MusicFile* MUS);

				size_t	SetEnd(void);
				void	SetVRC7(MMLfile* MML);
				void	SetN163(MMLfile* MML);
				void	SetN163Channel(MMLfile* MML);
				void	SetSubroutine(MMLfile* MML);
				void	SetEnvelop(unsigned char _opcode, MMLfile* MML);
				void	SetLoop();
				void	SetRepeat_A_Start(MMLfile* MML);
				void	SetRepeat_A_Branch(MMLfile* MML);
				void	SetRepeat_A_End(MMLfile* MML);
				void	SetRepeat_B_Start(MMLfile* MML);
				void	SetRepeat_B_Branch(MMLfile* MML);
				void	SetRepeat_B_End(MMLfile* MML);

				void	SetNote(MMLfile* MML, int note);
				void	SetRest(MMLfile* MML);
				void	SetTai(MMLfile* MML);
				void	SetLength(MMLfile* MML);
				void	SetOctave(MMLfile* MML);
				void	SetGatetime(MMLfile* MML);
				void	SetGatetime_u(MMLfile* MML);
				void	SetSweep(MMLfile* MML);
				void	SetVolume(MMLfile* MML);
				void	SetReleaseMode(MMLfile* MML);
				void	SetReleaseVoice(MMLfile* MML);
				void	SetReleaseVolume(MMLfile* MML);
				void	SetProtament(MMLfile* MML);
				void	SetVRC7_Write(MMLfile* MML);
				void	SetPoke(MMLfile* MML);
				void	SetKeySignature(MMLfile* MML);	//調号の設定
};
