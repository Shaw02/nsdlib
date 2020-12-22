/*******************************************************************************

			NES Sound Driver & Library	(NSD.lib)	MML Compiler

	Copyright (c) 2012 A.Watanabe (S.W.), All rights reserved.
	 For conditions of distribution and use, see copyright notice in "nsc.cpp".

*******************************************************************************/

#pragma once

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
	MusicHeader				Header;		//Header
	DPCMinfo*				cDPCMinfo;	//⊿PCM

	map<size_t, FDSC*	>	ptcFDSC;	//FDS  wave table (career)
	map<size_t, FDSM*	>	ptcFDSM;	//FDS  wave table (modulator)
	map<size_t, VRC7*	>	ptcVRC7;	//VRC7 User Instrument
	map<size_t, N163*	>	ptcN163;	//N163 wave table
	map<size_t, Envelop*>	ptcEnv;		//Envelop
	map<size_t, BGM*	>	ptcBGM;		//BGM
	map<size_t, SE*		>	ptcSE;		//SE 
	map<size_t, Sub*	>	ptcSub;		//Subroutine Sequence

private:
	string					dpcm_code;

//メンバー関数
public:
	MusicFile(MMLfile* MML, string _code, const _CHAR _strName[]=_T("================ [ Music ] ================"));
	~MusicFile(void);

	void	TickCount(void);

	size_t	SetDPCMOffset(size_t iMusSize);

	void	Fix_Address(void);

	size_t	read_bin(string* _str, NSF_Header* nsf_hed);				//.binを読み込む
	size_t	make_mus(string* _str, size_t rom_size, size_t ptOffset);	//バイナリーを作る
	size_t	make_dpcm(string* _str);
	size_t	make_bin(NSF_Header* NSF_Hed, string* NSF_Data);

	//保存フェーズ
	void	saveNSF(const char*	strFileName);
	void	saveNSFe(const char*	strFileName);
	void	saveASM(const char*	strFileName);

	void	Err(const _CHAR msg[]);
	void	Warning(const _CHAR msg[]);

};
