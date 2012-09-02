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
	unsigned	char	iBGM;
	unsigned	char	iSE;
				string	title;
				string	copyright;
				string	composer;
				string	segment;
				string	romcode;
//メンバー関数
public:
				MusicHeader(MMLfile* MML);
				~MusicHeader(void);
		void	Set_Title(MMLfile* MML);
		void	Set_Copyright(MMLfile* MML);
		void	Set_Composer(MMLfile* MML);
		void	Set_Segment(MMLfile* MML);
		void	Set_RomCode(MMLfile* MML);
		void	Set_Number_BGM(MMLfile* MML);
		void	Set_Number_SE(MMLfile* MML);
};
