/*******************************************************************************

			NES Sound Driver & Library	(NSD.lib)	MML Compiler

	Copyright (c) 2012 A.Watanabe (S.W.), All rights reserved.
	 For conditions of distribution and use, see copyright notice in "nsc.cpp".

*******************************************************************************/

#pragma once

/****************************************************************/
/*																*/
/*			構造体定義											*/
/*																*/
/****************************************************************/
typedef	struct{

	//設定数値
	unsigned	int		iVoi;		//
				size_t	iEvoi;		//
				size_t	iEvol;		//
				size_t	iEm;		//
				size_t	iEn;		//
	unsigned	int		iKey;		//KeyShift
	unsigned	char	iSweep;		//
				size_t	iSub;		//
	unsigned	int		iGate_q;	//
	unsigned	int		iGate_u;	//

	//設定するかどうか（defailt = false）
				bool	fVoi;		//
				bool	fEvoi;		//
				bool	fEvol;		//
				bool	fEm;		//
				bool	fEn;		//
				bool	fKey;		//
				bool	fSweep;		//
				bool	fSub;		//
				bool	fSub_opt;	//trueで最適化無効
				bool	fGate_q;	//
				bool	fGate_u;	//

	//エンベロープのsw（有る場合 true）
				bool	sw_Evoi;	//
				bool	sw_Evol;	//
				bool	sw_Em;		//
				bool	sw_En;		//

} patch_scrap;

/****************************************************************/
/*																*/
/*			クラス定義											*/
/*																*/
/****************************************************************/
class Patch
{
//メンバー変数
protected:

					size_t		m_id;			//パッチ番号
	map<size_t, patch_scrap*>	m_Patch;		//パッチの情報
					size_t		m_kn;			//処理中のノート番号

		patch_scrap*			m_now_Patch;	//処理中のパッチのポインタ


//メンバー関数
public:
						Patch(	MMLfile* MML, size_t _id);		
						~Patch(void);					

	//Patch オブジェクト生成部
				void	setKey(	MMLfile* MML, int key);
				void	setN(	MMLfile* MML, int note);

				void	DebugMsg(void);

	//シーケンス記述ブロックから呼ぶ関数群
				void	setNote(int i);		//ノート番号のセット

	unsigned	int		get_iVoi(void){		return(m_now_Patch->iVoi);};
				size_t	get_iEvoi(void){	return(m_now_Patch->iEvoi);};
				size_t	get_iEvol(void){	return(m_now_Patch->iEvol);};
				size_t	get_iEm(void){		return(m_now_Patch->iEm);};
				size_t	get_iEn(void){		return(m_now_Patch->iEn);};
	unsigned	int		get_iKey(void){		return(m_now_Patch->iKey);};
	unsigned	char	get_iSweep(void){	return(m_now_Patch->iSweep);};
				size_t	get_iSub(void){		return(m_now_Patch->iSub);};
	unsigned	int		get_iGate_q(void){	return(m_now_Patch->iGate_q);};
	unsigned	int		get_iGate_u(void){	return(m_now_Patch->iGate_u);};

				bool	get_fVoi(void){		return(m_now_Patch->fVoi);};
				bool	get_fEvoi(void){	return(m_now_Patch->fEvoi);};
				bool	get_fEvol(void){	return(m_now_Patch->fEvol);};
				bool	get_fEm(void){		return(m_now_Patch->fEm);};
				bool	get_fEn(void){		return(m_now_Patch->fEn);};
				bool	get_fKey(void){		return(m_now_Patch->fKey);};
				bool	get_fSweep(void){	return(m_now_Patch->fSweep);};
				bool	get_fSub(void){		return(m_now_Patch->fSub);};
				bool	get_fSub_opt(void){	return(m_now_Patch->fSub_opt);};
				bool	get_fGate_q(void){	return(m_now_Patch->fGate_q);};
				bool	get_fGate_u(void){	return(m_now_Patch->fGate_u);};

				bool	get_sw_Evoi(void){	return(m_now_Patch->sw_Evoi);};
				bool	get_sw_Evol(void){	return(m_now_Patch->sw_Evol);};
				bool	get_sw_Em(void){	return(m_now_Patch->sw_Em);};
				bool	get_sw_En(void){	return(m_now_Patch->sw_En);};

};
