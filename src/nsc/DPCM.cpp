#include "StdAfx.h"
#include ".\DPCM.h"

//==============================================================
//		コンストラクタ
//--------------------------------------------------------------
//	●引数
//		MMLfile*	MML		MMLファイルのオブジェクト
//		bool		_sub	このオブジェクトは、サブルーチン？
//	●返値
//					無し
//==============================================================
DPCM::DPCM(FileInput* DPCMfile, unsigned int _id, const char _strName[]):
	MusicItem(_strName),
	m_id(_id)
{
	//----------------------
	//Local変数
	unsigned	int		_size		= DPCMfile->GetSize();
	unsigned	int		i = 0;
	unsigned	char	cData;

	if((_size & 0x000F) != 0x01){
		iSize = (_size & 0x0FF0) + 0x0011;
	} else {
		iSize = _size;
	}
	_DPCM_size = (char)(iSize >> 4);

	code.resize(iSize);

	//�儕CM実体を転送
	while(i < _size){
		cData = DPCMfile->cRead();
		code[i] = cData;
		i++;
	}
	//Padding
	while(i < iSize){
		code[i] = (unsigned char)0xAA;
		i++;
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
//				無し
//	●返値
//				無し
//==============================================================
void	DPCM::getAsm(MusicFile* MUS)
{
	*MUS << ".align	$40\n" << MUS->Header.Label.c_str() << "DPCM" << m_id << ":" << endl;
	MusicItem::getAsm(MUS);
}
