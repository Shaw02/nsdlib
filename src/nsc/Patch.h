#pragma once

/****************************************************************/
/*																*/
/*			構造体定義											*/
/*																*/
/****************************************************************/
typedef	struct{

	//設定数値
	unsigned	int		iVoi;		//
	unsigned	int		iEvoi;		//
	unsigned	int		iEvol;		//
	unsigned	int		iEm;		//
	unsigned	int		iEn;		//
	unsigned	int		iKey;		//KeyShift
	unsigned	char	iSweep;		//
	unsigned	int		iSub;		//

	//設定するかどうか（defailt = false）
				bool	fVoi;		//
				bool	fEvoi;		//
				bool	fEvol;		//
				bool	fEm;		//
				bool	fEn;		//
				bool	fKey;		//
				bool	fSweep;		//
				bool	fSub;		//

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

		unsigned	int					m_id;			//パッチ番号
	map<unsigned	int, patch_scrap*>	m_Patch;		//パッチの情報

		unsigned	int					m_kn;			//処理中のノート番号

		patch_scrap*					m_now_Patch;	//処理中のパッチのポインタ


//メンバー関数
public:
						Patch(	MMLfile* MML, int _id);		
						~Patch(void);					

	//Patch オブジェクト生成部
				void	setKey(	MMLfile* MML, int key);
				void	setN(	MMLfile* MML, int note);

				void	DebugMsg(void);

	//シーケンス記述ブロックから呼ぶ関数群
				void	setNote(int i);		//ノート番号のセット

	unsigned	int		get_iVoi(void){		return(m_now_Patch->iVoi);};
	unsigned	int		get_iEvoi(void){	return(m_now_Patch->iEvoi);};
	unsigned	int		get_iEvol(void){	return(m_now_Patch->iEvol);};
	unsigned	int		get_iEm(void){		return(m_now_Patch->iEm);};
	unsigned	int		get_iEn(void){		return(m_now_Patch->iEn);};
	unsigned	int		get_iKey(void){		return(m_now_Patch->iKey);};
	unsigned	char	get_iSweep(void){	return(m_now_Patch->iSweep);};
	unsigned	int		get_iSub(void){		return(m_now_Patch->iSub);};

				bool	get_fVoi(void){		return(m_now_Patch->fVoi);};
				bool	get_fEvoi(void){	return(m_now_Patch->fEvoi);};
				bool	get_fEvol(void){	return(m_now_Patch->fEvol);};
				bool	get_fEm(void){		return(m_now_Patch->fEm);};
				bool	get_fEn(void){		return(m_now_Patch->fEn);};
				bool	get_fKey(void){		return(m_now_Patch->fKey);};
				bool	get_fSweep(void){	return(m_now_Patch->fSweep);};
				bool	get_fSub(void){		return(m_now_Patch->fSub);};

				bool	get_sw_Evoi(void){	return(m_now_Patch->sw_Evoi);};
				bool	get_sw_Evol(void){	return(m_now_Patch->sw_Evol);};
				bool	get_sw_Em(void){	return(m_now_Patch->sw_Em);};
				bool	get_sw_En(void){	return(m_now_Patch->sw_En);};

};
