#pragma once
#include "MusicItem.h"

/****************************************************************/
/*																*/
/*			クラス定義											*/
/*																*/
/****************************************************************/
class TrackSet :
	public MusicItem
{
//メンバー変数
protected:
	unsigned	int				m_id;		//ID
	unsigned	char			iTempo;

protected:
//static	const	Command_Info	Command[];	//コマンドの情報
	map<int,	MusicTrack*	>	ptcTrack;	//トラック・オブジェクトのポインタ
				MusicTrack*		nowTrack;	//コンパイル中のトラック
				int				iTrack;		//コンパイル中のトラック番号
				int				maxTrack;	//トラック番号の最大値
				bool			fSub;		//サブルーチン記述ブロックかどうかのフラグ
				bool			fSE;		//SEかどうかのフラグ
	unsigned	int				TrackPt;	//
	unsigned	int				TrackLine;	//
	unsigned	char			Priority;	//

//メンバー関数
public:
				TrackSet(MMLfile* MML, unsigned int _id, bool _sub, bool _se, const wchar_t _strName[] = L"==== [ Track Set ]====");
				~TrackSet(void);

		void	TickCount(MusicFile* MUS);
		void	getAsm(MusicFile* MUS);

		void	Optimize(MusicFile* MUS);
		void	Fix_Address(MusicFile* MUS);

		void	TrackChk(MMLfile* MML);
		void	TrackProc(MMLfile* MML);
	MusicTrack*	makeTrack(MMLfile* MML, int _track);
	MusicTrack*	getTrack(MMLfile* MML, int _track);

		void	SetEvent(MusicItem* _item);		//イベントの追加

		void	SetTempo(MMLfile* MML);
		void	SetRelativeTempo(MMLfile* MML);
		void	SetRelativeUp();
		void	SetRelativeDown();

		void	SetVolume(MMLfile* MML);
		void	SetVolumeInc(MMLfile* MML);
		void	SetVolumeDec(MMLfile* MML);

		void	SetReleaseMode(MMLfile* MML);
		void	SetReleaseVoice(MMLfile* MML);
		void	SetReleaseVolume(MMLfile* MML);

		void	SetSweep(MMLfile* MML);

		void	SetPoke(MMLfile* MML);
		void	Set_FDS_Frequency(MMLfile* MML);
		void	Set_FDS_Volume(MMLfile* MML);
		void	SetVRC7_Write(MMLfile* MML);
		void	SetN163Channel(MMLfile* MML);
		void	Set_FME7_Frequency(MMLfile* MML);

		void	SetPriority(MMLfile* MML);
};
