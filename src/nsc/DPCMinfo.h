/*******************************************************************************

			NES Sound Driver & Library	(NSD.lib)	MML Compiler

	Copyright (c) 2012 A.Watanabe (S.W.), All rights reserved.
	 For conditions of distribution and use, see copyright notice in "nsc.cpp".

*******************************************************************************/

#pragma once
#include "MusicItem.h"

/****************************************************************/
/*																*/
/*			定数定義											*/
/*																*/
/****************************************************************/
typedef	struct nsd_dpcm_info{
				string	file;
	unsigned	char	ctrl;
	unsigned	char	DA;
	unsigned	char	next;
	unsigned	char	offset;
	unsigned	char	size;
	nsd_dpcm_info(){
		file	= "";
		ctrl	= 0;
		DA		= 0;
		next	= 0;
		offset	= 0;
		size	= 0;
	}
} nsd_dpcm_info;

typedef	struct{
	char		Control;			/*	I/O 0x4010	*/
	char		DA;					/*	I/O 0x4011	*/
	char		Address;			/*	I/O 0x4012	*/
	char		Size;				/*	I/O 0x4013	*/
} nsd_dpcm;

/****************************************************************/
/*																*/
/*			クラス定義											*/
/*																*/
/****************************************************************/
class DPCMinfo :
	public MusicItem
{
//メンバー変数
private:
	bool				f_error;				//エラー発生の有無
	bool				bank;
	unsigned	char	max_number;
	nsd_dpcm_info		infoDPCM[256];
	map< string, DPCM*>	ptcDPCM;	//⊿PCM
//static	const	Command_Info	Command[];	//コマンドの情報

//メンバー関数
public:
						DPCMinfo(MMLfile* MML, bool _bank, const _CHAR _strName[] = _T("==== [ DPCMinfo ]===="));
						~DPCMinfo(void);
				void	setKey(MMLfile* MML, int key);
				void	setNote(MMLfile* MML, int note);
				size_t	setDPCMoffset(size_t _offset, unsigned char _MusBank);
				void	getDPCMCode(string* _str);
				void	getAsm(MusicFile* MUS);
				bool	isError(){return(f_error);};
};
