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
private:
//static	const	Command_Info	Command[];	//コマンドの情報

	map<int,	MusicTrack*	>	ptcTrack;	//トラック・オブジェクトのポインタ
				MusicTrack*		nowTrack;	//コンパイル中のトラック番号
				int				iTrack;		//コンパイル中のトラック番号
				int				maxTrack;	//トラック番号の最大値
				bool			fSub;		//サブルーチン記述ブロックかどうかのフラグ

//メンバー関数
public:
				TrackSet(MMLfile* MML, bool _sub, const char _strName[] = "==== [ Track Set ]====");
				~TrackSet(void);

		void	Fix_Address(map<int, Sub*>* ptcSub, map<int, Envelop*>* ptcEnvelop);

	MusicTrack*	makeTrack(int _track);
	MusicTrack*	getTrack(int _track);
};
