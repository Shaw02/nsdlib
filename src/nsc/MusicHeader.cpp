/*******************************************************************************

			NES Sound Driver & Library	(NSD.lib)	MML Compiler

	Copyright (c) 2012 A.Watanabe (S.W.), All rights reserved.
	 For conditions of distribution and use, see copyright notice in "nsc.cpp".

*******************************************************************************/

#include "StdAfx.h"
#include "MusicHeader.h"

//==============================================================
//		コンストラクタ
//--------------------------------------------------------------
//	●引数
//		const		_CHAR	_strName[]	オブジェクト名
//		string				_code
//	●返値
//				無し
//==============================================================
MusicHeader::MusicHeader(string _code) :
	MetaItem("NSFE"),
	bank(false),
	op_code(false),
	iBGM(1),
	iSE(0),
	offsetPCM(0x10000),
	iExternal(-1),
	Label("_nsd_"),
	title(""),
	copyright(""),
	composer(""),
	segmentSEQ("RODATA"),
	segmentPCM("PCMDATA")
{
	if(_code.empty()){
		op_code = false;
		romcode = "nsd.bin";
	} else {
		op_code = true;
		romcode = _code;
	}
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
	title = MML->GetString(true);
}

void	MusicHeader::Set_Copyright(MMLfile* MML)
{
	copyright = MML->GetString(true);
}

void	MusicHeader::Set_Composer(MMLfile* MML)
{
	composer = MML->GetString(true);
}

void	MusicHeader::Set_SegmentSEQ(MMLfile* MML)
{
	segmentSEQ = MML->GetString(false);
}

void	MusicHeader::Set_SegmentPCM(MMLfile* MML)
{
	segmentPCM = MML->GetString(false);
}

void	MusicHeader::Set_Label(MMLfile* MML)
{
	Label = MML->GetString(false);
}

void	MusicHeader::Set_OffsetPCM(MMLfile* MML)
{
	offsetPCM = MML->GetInt();

	if(bank==true){
		MML->Warning(_T("#Bank指定時は、#offsetPCMは無効です。"));
	} else {
		if((offsetPCM < 0xC000) || (offsetPCM > 0x10000)){
			MML->Err(_T("$C000 ～ $10000（⊿PCM未使用）の範囲で指定して下さい。"));
		}
		if((offsetPCM & 0x003F) != 0){
			MML->Warning(_T("⊿PCMの配置アドレスは64（$40）Byteでアライメントします。"));
			offsetPCM &= 0xFFC0;
			offsetPCM += 0x0040;
		}
	}
}

void	MusicHeader::Set_RomCode(MMLfile* MML)
{
	if(op_code == true){
		MML->Warning(_T("オプションスイッチでリンクするコードが指定されているので、#codeコマンドは無視します。"));
		MML->GetString(false);
	} else {
		romcode = MML->GetString(false);
	}
}

void	MusicHeader::Set_Number_BGM(MMLfile* MML)
{
	size_t	_n = MML->GetInt();

	if((_n > 255) || (_n < 0)){
		MML->Err(_T("#BGMは0～255の範囲で指定してください。"));
	}
	iBGM = _n;
}

void	MusicHeader::Set_Number_SE(MMLfile* MML)
{
	size_t	_n = MML->GetInt();

	if((_n > 255) || (_n < 0)){
		MML->Err(_T("#SEは0～255の範囲で指定してください。"));
	}
	iSE = _n;
}

void	MusicHeader::Set_External(MMLfile* MML)
{
	int	_n = MML->GetInt();

	if((_n > 0x3F) || (_n < 0)){
		MML->Err(_T("#Externalは$00～#3Fの範囲で指定してください。"));
	}
	iExternal = _n;
}

void	MusicHeader::Set_Bank(void)
{
	bank		= true;
	offsetPCM	= 0xC000;
}
