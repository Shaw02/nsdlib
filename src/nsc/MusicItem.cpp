/*******************************************************************************

			NES Sound Driver & Library	(NSD.lib)	MML Compiler

	Copyright (c) 2012 A.Watanabe (S.W.), All rights reserved.
	 For conditions of distribution and use, see copyright notice in "nsc.cpp".

*******************************************************************************/

#include "StdAfx.h"
#include "MusicItem.h"

/****************************************************************/
/*					グローバル変数（クラスだけど・・・）		*/
/****************************************************************/
extern	OPSW*			cOptionSW;	//オプション情報へのポインタ変数

#ifdef _OPENMP
	extern	omp_lock_t		lock_cout;
#endif

//==============================================================
//		コンストラクタ
//--------------------------------------------------------------
//	●引数
//		const		_CHAR	_strName[]	オブジェクト名
//	●返値
//				無し
//==============================================================
MusicItem::MusicItem(const _CHAR _strName[]):
	strName(_strName),
	iSize(0),
	iOffset(0),
	m_id(0),
	f_id(false),
	f_necessary(false)
{
	//Debug message　（うざい程出力するので注意。）
	if(cOptionSW->iDebug & DEBUG_Create){
		_COUT << _T("Create Music Object : ") << strName << endl;
	}
}

MusicItem::MusicItem(size_t _id, const _CHAR _strName[]):
	strName(_strName),
	iSize(0),
	iOffset(0),
	m_id(_id),
	f_id(true),
	f_necessary(false)
{
	//Debug message　（うざい程出力するので注意。）
	if(cOptionSW->iDebug & DEBUG_Create){
		_COUT << _T("Create Music Object : ") << strName << _T("(");
		cout << m_id;
		_COUT << _T(")") << endl;
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
MusicItem::~MusicItem(void)
{
	clear();

	//Debug message　（うざい程出力するので注意。）
	if(cOptionSW->iDebug & DEBUG_Delete){
		_OMP_SET_LOCK(lock_cout)
		_COUT << _T("Delete Music Object : ") << strName;
		if(f_id == true){
			_COUT << _T("(") << m_id << _T(")");
		}
		#ifdef _OPENMP
		_COUT << " (Thread No=" << omp_get_thread_num() <<")";
		#endif
		_COUT << endl;
		_OMP_UNSET_LOCK(lock_cout)
	}
}

//==============================================================
//		クリア
//--------------------------------------------------------------
//	●引数
//				無し
//	●返値
//				無し
//==============================================================
void	MusicItem::clear(void)
{

	//----------------------
	//clear
	code.clear();

	//----------------------
	//Delete Class
	for(list<MusicItem*>::iterator it=ptcItem.begin(), e=ptcItem.end(); it!=e; ++it){
		delete *it;
	}
	ptcItem.clear();

	iSize = 0;
}

//==============================================================
//		最適化時のクリア
//--------------------------------------------------------------
void	MusicItem::clear_Optimize()
{

	if(cOptionSW->iDebug & DEBUG_Optimize){
		_OMP_SET_LOCK(lock_cout)
		_COUT << _T("Optimize Object  : ") << strName;
		if(f_id == true){
			_COUT	<< _T("(") << m_id << _T(")");
		}
		#ifdef _OPENMP
		_COUT << " (Thread No=" << omp_get_thread_num() <<")";
		#endif
		_COUT << endl;
		_OMP_UNSET_LOCK(lock_cout)
	}

	if(chkUse() == false){
		//----------------------
		//このオブジェクトごと、ごっそりクリアする。
		//Debug message　（うざい程出力するので注意。）
		if(cOptionSW->iDebug & DEBUG_Optimize){
			_OMP_SET_LOCK(lock_cout)
			_COUT << _T("Optimizing : ") << strName;
			if(f_id == true){
				_COUT	<< _T("(") << m_id << _T(")");
			}
			#ifdef _OPENMP
			_COUT << " (Thread No=" << omp_get_thread_num() <<")";
			#endif
			_COUT << endl;
			_OMP_UNSET_LOCK(lock_cout)
		}
		clear();
	} else {
		//----------------------
		//子オブジェクトを最適化するか評価する。
		for(list<MusicItem*>::iterator it=ptcItem.begin(), e=ptcItem.end(); it!=e; ++it){
			(*it)->clear_Optimize();
		}
	}
}

//==============================================================
//		コードサイズの取得
//--------------------------------------------------------------
//	●引数
//				無し
//	●返値
//		size_t
//==============================================================
size_t		MusicItem::getSize()
{
	return(iSize);
}

//==============================================================
//		オフセットアドレスの設定
//--------------------------------------------------------------
//	●引数
//		size_t	_offset	このオブジェクトのオフセットアドレス
//	●返値
//		size_t			次のオブジェクトのオフセットアドレス
//==============================================================
size_t	MusicItem::SetOffset(size_t _offset)
{
	//Debug message　（うざい程出力するので注意。）
	if(cOptionSW->iDebug & DEBUG_SetAddress){
		_COUT << _T("Object Address [0x") << hex << setw(4) << setfill(_T('0')) << _offset << _T("]: ");
		for(size_t i=0, e=code.size(); i < e; ++i){
			_COUT	<<	hex	<<	setw(2)	<<	setfill(_T('0'))	<<	(unsigned int)(code[i] & 0xFF)	<<	_T(" ");
		}
		_COUT  << dec	<< _T(": ") << strName;
		if(f_id == true){
			_COUT	<< _T("(") << m_id << _T(")");
		}
		_COUT << endl;
	}

	iOffset = _offset;
	_offset	+= code.size();

	for(list<MusicItem*>::iterator it=ptcItem.begin(), e=ptcItem.end(); it != e; ++it ){
		_offset = (*it)->SetOffset(_offset);
	}

	//このオブジェクトのサイズ（最適化後）
	iSize = _offset - iOffset;

	return(_offset);
}

//==============================================================
//		コードの取得
//--------------------------------------------------------------
//	●引数
//		unsigned	int	n	添え字
//	●返値
//		unsigned	char	内容
//==============================================================
unsigned	char	MusicItem::getCode(size_t n)
{
	unsigned	char	iCode;
	
	if((n<0) || (n>=iSize)){
		iCode = 0xFF;
	} else {
		iCode = code[n];
	}

	return(iCode);
}

//==============================================================
//		コードの取得
//--------------------------------------------------------------
//	●引数
//		string*		_str
//	●返値
//				無し
//==============================================================
void	MusicItem::getCode(string* _str)
{
	_str->append(code);

	for(list<MusicItem*>::iterator it=ptcItem.begin(), e=ptcItem.end(); it!=e; ++it){
		(*it)->getCode(_str);
	}
}

//==============================================================
//		コードの設定
//--------------------------------------------------------------
//	●引数
//		string*		_str
//	●返値
//				無し
//==============================================================
void	MusicItem::setCode(string* _str)
{
	code.clear();
	code.assign(*_str);
	iSize = code.size();
}

//==============================================================
//		コードの取得
//--------------------------------------------------------------
//	●引数
//		MusicFile*	MUS		コードを出力する曲データファイル・オブジェクト
//	●返値
//				無し
//==============================================================
void	MusicItem::getAsm(MusicFile* MUS)
{
	if(code.size() > 0){
		for(size_t i=0; i<code.size(); ++i){
			if(i==0){
				*MUS << "	.byte	$";
			} else {
				*MUS << " ,$";
			}
			*MUS << hex << setw(2) << setfill('0') << (int)(code[i] & 0xFF);
		}
		*MUS << dec << endl;
	}

	for(list<MusicItem*>::iterator it=ptcItem.begin(), e=ptcItem.end(); it!=e; ++it){
		(*it)->getAsm(MUS);
	}
}

//==============================================================
//		idの設定
//--------------------------------------------------------------
//	●引数
//		size_t	_id		番号
//	●返値
//				無し
//==============================================================
void	MusicItem::set_id(size_t _id)
{
	f_id = true;
	m_id = _id;
}

//==============================================================
//		idの取得
//--------------------------------------------------------------
//	●引数
//				無し
//	●返値
//		unsigned	int		番号
//==============================================================
size_t	MusicItem::get_id(void)
{
	return(m_id);
}
//==============================================================
//		flagの取得
//--------------------------------------------------------------
//	●引数
//				無し
//	●返値
//		unsigned	int		番号
//==============================================================
bool	MusicItem::get_flag(void)
{
	return(f_id);
}
