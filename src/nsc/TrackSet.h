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

private:
//static	const	Command_Info	Command[];	//コマンドの情報
	map<int,	MusicTrack*	>	ptcTrack;	//トラック・オブジェクトのポインタ
				MusicTrack*		nowTrack;	//コンパイル中のトラック番号
	unsigned	int				iTrack;		//コンパイル中のトラック番号
	unsigned	int				maxTrack;	//トラック番号の最大値
				bool			fSub;		//サブルーチン記述ブロックかどうかのフラグ

//メンバー関数
public:
				TrackSet(MMLfile* MML, unsigned int _id, bool _sub, const wchar_t _strName[] = L"==== [ Track Set ]====");
				~TrackSet(void);

		void	getAsm(MusicFile* MUS);
		void	Fix_Address(MusicFile* MUS);

	MusicTrack*	makeTrack(int _track);
	MusicTrack*	getTrack(int _track);

		void	SetEvent(MusicItem* _item);		//イベントの追加

		void	SetOctave(MMLfile* MML);
		void	SetOctaveInc();
		void	SetOctaveDec();
		void	SetOctaveOne_Inc();
		void	SetOctaveOne_Dec();

		void	SetVolume(MMLfile* MML);
		void	SetVolumeInc();
		void	SetVolumeDec();

		void	SetReleaseMode(MMLfile* MML);
		void	SetReleaseVoice(MMLfile* MML);
		void	SetReleaseVolume(MMLfile* MML);

		void	SetGatetime(MMLfile* MML);
		void	SetGatetime_u(MMLfile* MML);

		void	SetProtament(MMLfile* MML);
		void	SetSweep(MMLfile* MML);

		void	SetPoke(MMLfile* MML);
		void	SetVRC7_Write(MMLfile* MML);
		void	SetN163Channel(MMLfile* MML);
		void	Set_FME7_Frequency(MMLfile* MML);
};
