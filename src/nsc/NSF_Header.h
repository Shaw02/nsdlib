/*******************************************************************************

			NES Sound Driver & Library	(NSD.lib)	MML Compiler

	Copyright (c) 2012 A.Watanabe (S.W.), All rights reserved.
	 For conditions of distribution and use, see copyright notice in "nsc.cpp".

*******************************************************************************/

#pragma once

/****************************************************************/
/*																*/
/*			ç\ë¢ëÃíËã`											*/
/*																*/
/****************************************************************/
typedef struct {
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
} NSF_Header;

#define	nsf_flag_IRQ_support 		0x10
#define	nsf_flag_non_Ret_Init		0x20
#define	nsf_flag_non_Play 			0x40
#define	nsf_flag_MetaData			0x80

