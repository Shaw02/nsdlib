/*******************************************************************************

			NES Sound Driver & Library	(NSD.lib)	MML Compiler

	Copyright (c) 2012 A.Watanabe (S.W.), All rights reserved.
	 For conditions of distribution and use, see copyright notice in "nsc.cpp".

*******************************************************************************/

#pragma once
#include "MetaItem.h"

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
				int		iExternal;		//拡張音源フラグ
				string	title;
				string	copyright;
				string	composer;
				string	maker;
				string	romcode;
				string	text;

private:		//for Metadata
				Meta_INFO*	meta_INFO;		//2.1	INFO	NSFe MUST
				Meta_DATA*	meta_DATA;		//2.2	DATA	NSFe MUST
				Meta_NEND*	meta_NEND;		//2.3	NEND	NSFe MUST
				Meta_BANK*	meta_BANK;		//2.4	BANK	NSFe optional / NSF MUSTNOT
				Meta_NSF2*	meta_NSF2;		//2.6	NSF2	NSFe optional /  NSF MUSTNOT
				Meta_VRC7*	meta_VRC7;		//2.7	VRC7
				Meta_plst*	meta_plst;		//2.8	plst
				Meta_psfx*	meta_psfx;		//2.9	psfx
				Meta_time*	meta_time;		//2.10	time
				Meta_fade*	meta_fade;		//2.11	fade
				Meta_tlbl*	meta_tlbl;		//2.12	tlbl
				Meta_taut*	meta_taut;		//2.13	taut
				Meta_auth*	meta_auth;		//2.14	auth
				Meta_text*	meta_text;		//2.15	text
				Meta_mixe*	meta_mixe;		//2.16	mixe
				
//メンバー関数
public:
				MusicHeader(string& _code);
				~MusicHeader(void);
		void	Set_Title(MMLfile* MML);
		void	Set_Copyright(MMLfile* MML);
		void	Set_Composer(MMLfile* MML);
		void	Set_Maker(MMLfile* MML);
		void	Text_Append(MMLfile* MML);

		void	Set_NSFe_footer(NSF_Header* _nsf_hed);
		void	Set_DATA(string* data);
		void	Set_NEND();
		void	Ser_VRC7(MMLfile* MML);
		void	Set_plst(MMLfile* MML);
		void	Set_psfx(MMLfile* MML);
		void	Set_time(int _iTime);
		void	Set_fade(int _iFade);
		void	Set_tlbl(string* _str);
		void	Set_taut(string* _str);
		void	Set_auth();
		void	Set_text();
		void	Set_mixe(MMLfile* MML);

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
