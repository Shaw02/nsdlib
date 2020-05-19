/*******************************************************************************

			NES Sound Driver & Library	(NSD.lib)	MML Compiler

	Copyright (c) 2012 A.Watanabe (S.W.), All rights reserved.
	 For conditions of distribution and use, see copyright notice in "nsc.cpp".

*******************************************************************************/

#pragma once

/****************************************************************/
/*																*/
/*			定数定義											*/
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
	unsigned	char	Null1;				//7C
	unsigned	char	Null2;				//7D
	unsigned	char	Null3;				//7E
	unsigned	char	Null4;				//7F
};

/****************************************************************/
/*																*/
/*			クラス定義											*/
/*																*/
/****************************************************************/
class MusicFile :
	public FileOutput, public MusicItem
{
//メンバー変数
public:
			MusicHeader			Header;		//Header
			DPCMinfo*			cDPCMinfo;	//⊿PCM

	map<size_t, FDSC*	>	ptcFDSC;	//FDS  wave table (career)
	map<size_t, FDSM*	>	ptcFDSM;	//FDS  wave table (modulator)
	map<size_t, VRC7*	>	ptcVRC7;	//VRC7 User Instrument
	map<size_t, N163*	>	ptcN163;	//N163 wave table
	map<size_t, Envelop*>	ptcEnv;		//Envelop
	map<size_t, BGM*	>	ptcBGM;		//BGM
	map<size_t, SE*		>	ptcSE;		//SE 
	map<size_t, Sub*	>	ptcSub;		//Subroutine Sequence

private:
//static	const	Command_Info	Command[];
					string		dpcm_code;
//					size_t		nsf_size;

//メンバー関数
public:
	MusicFile(MMLfile* MML, string _code, const _CHAR _strName[]=_T("================ [ Music ] ================"));
	~MusicFile(void);

	void	TickCount(void);

	size_t	SetDPCMOffset(size_t iMusSize);

	void	Fix_Address(void);

	//バイナリーを作る
	void	make_bin(size_t rom_size, size_t ptOffset);

	//保存フェーズ
	void	saveNSF(const char*	strFileName);
	void	saveASM(const char*	strFileName);

	void	Err(const _CHAR msg[]);
	void	Warning(const _CHAR msg[]);

};
