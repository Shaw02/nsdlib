#include "StdAfx.h"
#include "MusicHeader.h"

//==============================================================
//		コンストラクタ
//--------------------------------------------------------------
//	●引数
//		MusicFile*	SND
//		int			iOffset
//	●返値
//				無し
//==============================================================
MusicHeader::MusicHeader(MMLfile* MML):
	iBGM(1),
	iSE(0),
	title(""),
	copyright(""),
	composer(""),
	romcode("nsd.bin"),
	segment("RODATA")
{
}

//==============================================================
//		デストラクタ
//--------------------------------------------------------------
//	●引数
//				無し
//	●返値
//				無し
//==============================================================
MusicHeader::~MusicHeader(void)
{
}

void	MusicHeader::Set_Title(MMLfile* MML)
{
	title = MML->GetString();
}

void	MusicHeader::Set_Copyright(MMLfile* MML)
{
	copyright = MML->GetString();
}

void	MusicHeader::Set_Composer(MMLfile* MML)
{
	composer = MML->GetString();
}

void	MusicHeader::Set_Segment(MMLfile* MML)
{
	segment = MML->GetString();
}

void	MusicHeader::Set_RomCode(MMLfile* MML)
{
	romcode = MML->GetString();
}

void	MusicHeader::Set_Number_BGM(MMLfile* MML)
{
	int	_n = MML->GetInt();

	if((_n > 255) || (_n < 0)){
		MML->Err("#BGMは0～255以下の値で指定してください。");
	}
	iBGM = _n;
}

void	MusicHeader::Set_Number_SE(MMLfile* MML)
{
	int	_n = MML->GetInt();

	if((_n > 255) || (_n < 0)){
		MML->Err("#SEは0～255以下の値で指定してください。");
	}
	iSE = _n;
}
