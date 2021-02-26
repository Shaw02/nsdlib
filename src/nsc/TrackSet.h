/*******************************************************************************

			NES Sound Driver & Library	(NSD.lib)	MML Compiler

	Copyright (c) 2012 A.Watanabe (S.W.), All rights reserved.
	 For conditions of distribution and use, see copyright notice in "nsc.cpp".

*******************************************************************************/

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
				string			label;		//曲名
				string			author;		//作曲者
				int				time;		//演奏時間[msec]
				int				fade;		//フェードアウト時間[msec]

				int				iTempo;		//テンポ
				bool			fJump;		//j1コマンドがあったか？

	map<size_t,	MusicTrack*	>	ptcTrack;	//トラック・オブジェクトのポインタ
				MusicTrack*		nowTrack;	//コンパイル中のトラック
				size_t			iTrack;		//コンパイル中のトラック番号
				size_t			maxTrack;	//トラック番号の最大値
				bool			fSub;		//サブルーチン記述ブロックかどうかのフラグ
				bool			fSE;		//SEかどうかのフラグ
		fstream::pos_type		TrackPt;	//
				size_t			TrackLine;	//
	unsigned	char			Priority;	//効果音優先順位

//メンバー関数
public:
				TrackSet(MMLfile* MML, size_t _id, bool _sub, bool _se, const _CHAR _strName[] = _T("==== [ Track Set ]===="));
				~TrackSet(void);

		void	TickCount(MusicFile* MUS);
		void	TickCountPrint(MusicFile* MUS, size_t iStart, size_t iEnd);
		void	getAsm(MusicFile* MUS);

		void	Fix_Address(MusicFile* MUS);

		void	TrackChk(MMLfile* MML);
		void	TrackProc(MMLfile* MML);
	MusicTrack*	makeTrack(MMLfile* MML, size_t _track);
	MusicTrack*	getTrack(MMLfile* MML, size_t _track);

		void	SetEvent(MusicItem* _item);		//イベントの追加

		//----------------------------------
		//曲全体に効くＭＭＬコマンド

		//効果音の優先順位
		void	Set_Priority(MMLfile* MML);

		//早送り
		void	SetJumpDrv(MMLfile* MML);

		//テンポ操作
		void	SetTempo(MMLfile* MML);
		void	SetRelativeTempo(MMLfile* MML);
		void	TempoUp();
		void	TempoDown();

		//レジスタ操作
		void	Set_Poke(MMLfile* MML);				//メモリ書き込み
		void	Set_VRC7_Write(MMLfile* MML);		//VRC7	レジスタ書き込み

		void	Set_FDS_Frequency(MMLfile* MML);	//FDS	モジュレータ周波数
		void	Set_FDS_Volume(MMLfile* MML);		//FDS	マスター音量
		void	Set_N163_Channel(MMLfile* MML);		//N16x	チャンネル数
		void	Set_FME7_Frequency(MMLfile* MML);	//SN5B	エンベロープ周波数

		//----------------------------------
		//MetaItem
		bool	isLabel(void){if(label.size() == 0){return(false);} else {return(true);};}
		bool	isAuthor(void){if(author.size() == 0){return(false);} else {return(true);};}
		bool	isTime(void){if(time < 0){return(false);} else {return(true);};}
		bool	isFade(void){if(time < 0){return(false);} else {return(true);};}

		string*	getLabel(void){return(&label);};
		string*	getAuthor(void){return(&author);};
		int		getTime(void){return(time);};
		int		getFade(void){return(fade);};
};
