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
class MusicHeader
{
//メンバー変数
public:
				bool	bank;
				bool	op_code;		//option で code 指定された？
				size_t	iBGM;			//BGM数
				size_t	iSE;			//SE数
				size_t	offsetPCM;
				int		iExternal;		//拡張音源フラグ
				string	Label;
				string	title;
				string	copyright;
				string	composer;
				string	segmentSEQ;
				string	segmentPCM;
				string	romcode;
//メンバー関数
public:
				MusicHeader(string _code);
				~MusicHeader(void);
		void	Set_Title(MMLfile* MML);
		void	Set_Copyright(MMLfile* MML);
		void	Set_Composer(MMLfile* MML);
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
