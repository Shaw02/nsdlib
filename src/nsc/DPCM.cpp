/*******************************************************************************

			NES Sound Driver & Library	(NSD.lib)	MML Compiler

	Copyright (c) 2012 A.Watanabe (S.W.), All rights reserved.
	 For conditions of distribution and use, see copyright notice in "nsc.cpp".

*******************************************************************************/

#include "StdAfx.h"
#include "DPCM.h"

/****************************************************************/
/*					グローバル変数（クラスだけど・・・）		*/
/****************************************************************/
extern	OPSW*			cOptionSW;	//オプション情報へのポインタ変数

//==============================================================
//		コンストラクタ
//--------------------------------------------------------------
//	●引数
//		FileInput*			DPCMfile	⊿PCM（*.dmc）のファイル名
//					size_t	_id			番号
//		const		_CHAR	_strName[]	このオブジェクトの名前
//	●返値
//					無し
//==============================================================
DPCM::DPCM(MMLfile* MML, const char* dmcfile, size_t _id, const _CHAR _strName[]):
	MusicItem(_id, _strName),
	_DPCM_size(0)
{
	fileopen(dmcfile, &cOptionSW->m_pass_dmc);

	if(f_error == true){
		string	errMsg = "\"";
		errMsg += dmcfile;
		errMsg += "\" :";
		errMsg += strerror(errno);
		MML->Err(errMsg);
	} else {
		//----------------------
		//Local変数
		std::streamsize	_size	= GetSize();

		if(_size > 0x0FF1){
			MML->Warning(_T("⊿PCMのファイルサイズが大きすぎます。4081Byteにカットします。"));
			iSize = 0x0FF1;
			_size = 0x0FF1;
		} else {
			if((_size & 0x000F) != 0x01){
				iSize = (_size & 0x0FF0) + 0x0011;
			} else {
				iSize = _size;
			}
		}
		_DPCM_size = (unsigned char)(iSize >> 4);

		code.resize(iSize);

		//⊿PCM実体を転送
		read((char*)code.c_str(), _size);

		//Padding
		for(size_t i = _size; i<iSize; i++){
			code[i] = (unsigned char)0xAA;
		}
		close();
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
DPCM::~DPCM(void)
{
}

//==============================================================
//		コードの取得
//--------------------------------------------------------------
//	●引数
//		MusicFile*	MUS		コードを出力する曲データファイル・オブジェクト
//	●返値
//				無し
//==============================================================
void	DPCM::getAsm(MusicFile* MUS)
{
	*MUS << ".align	$40\n" << MUS->Header.Label << "DPCM" << m_id << ":" << endl;
	MusicItem::getAsm(MUS);
}
