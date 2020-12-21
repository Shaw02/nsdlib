/*******************************************************************************

			NES Sound Driver & Library	(NSD.lib)	MML Compiler

	Copyright (c) 2012 A.Watanabe (S.W.), All rights reserved.
	 For conditions of distribution and use, see copyright notice in "nsc.cpp".

*******************************************************************************/

#pragma once
#include "MetaItem.h"

/****************************************************************/
/*																*/
/*			構造体定義											*/
/*																*/
/****************************************************************/
struct	NSF_Header{
				char	Name[5];			//00	"NESM",0x1A
	unsigned	char	Version;			//05
	unsigned	char	MusicNumber;		//06
	unsigned	char	StartMusicNumber;	//07
	unsigned	short	LoadAddress;		//08
	unsigned	short	InitAddress;		//0A
	unsigned	short	MainAddress;		//0C
				char	Title[32];			//0E
				char	Composer[32];		//2E
				char	Copyright[32];		//4E
	unsigned	short	Frequency_NTSC;		//6E
	unsigned	char	Bank[8];			//70
	unsigned	short	Frequency_PAL;		//78
	unsigned	char	Video;				//7A
	unsigned	char	External;			//7B
	unsigned	char	Flags;				//7C
	unsigned	char	szNSF_Data[3];		//7D-7F
};

#define	nsf_flag_IRQ_support 		0x10
#define	nsf_flag_non_Ret_Init		0x20
#define	nsf_flag_non_Play 			0x40
#define	nsf_flag_MetaData			0x80

/****************************************************************/
/*																*/
/*			クラス定義											*/
/*																*/
/****************************************************************/
class MusicHeader :
	public MetaItem
{
//メンバー変数
public:
				bool	bank;
				bool	op_code;		//option で code 指定された？
				size_t	iBGM;			//BGM数
				size_t	iSE;			//SE数
				size_t	offsetPCM;

				//for .s
				string	Label;
				string	segmentSEQ;
				string	segmentPCM;

				//for NSF
				bool	f_VRC7_chg;		//#VRC7を使ったか？

				int		iExternal;		//拡張音源フラグ
				string	title;
				string	copyright;
				string	composer;
				string	maker;
				string	text;
				string	romcode;
	vector<unsigned char>		plst;
	vector<unsigned char>		psfx;
	map<unsigned char, short>	mixe;

//メンバー関数
public:
				MusicHeader(string _code);
				~MusicHeader(void);
		void	Set_Title(MMLfile* MML);
		void	Set_Copyright(MMLfile* MML);
		void	Set_Composer(MMLfile* MML);
		void	Set_Maker(MMLfile* MML);
		void	Set_Text(MMLfile* MML);

		void	Set_SegmentSEQ(MMLfile* MML);
		void	Set_SegmentPCM(MMLfile* MML);
		void	Set_Label(MMLfile* MML);
		void	Set_OffsetPCM(MMLfile* MML);
		void	Set_RomCode(MMLfile* MML);
		void	Set_Number_BGM(MMLfile* MML);
		void	Set_Number_SE(MMLfile* MML);
		void	Set_External(MMLfile* MML);
		void	Set_Bank(void);
};
