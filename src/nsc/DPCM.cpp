/*******************************************************************************

			NES Sound Driver & Library	(NSD.lib)	MML Compiler

	Copyright (c) 2012 A.Watanabe (S.W.), All rights reserved.
	 For conditions of distribution and use, see copyright notice in "nsc.cpp".

*******************************************************************************/

#include "StdAfx.h"
#include "DPCM.h"

/****************************************************************/
/*					�O���[�o���ϐ��i�N���X�����ǁE�E�E�j		*/
/****************************************************************/
extern	OPSW*			cOptionSW;	//�I�v�V�������ւ̃|�C���^�ϐ�

//==============================================================
//		�R���X�g���N�^
//--------------------------------------------------------------
//	������
//		FileInput*			DPCMfile	��PCM�i*.dmc�j�̃t�@�C����
//					size_t	_id			�ԍ�
//		const		_CHAR	_strName[]	���̃I�u�W�F�N�g�̖��O
//	���Ԓl
//					����
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
		//Local�ϐ�
		std::streamsize	_size	= GetSize();

		if(_size > 0x0FF1){
			MML->Warning(_T("��PCM�̃t�@�C���T�C�Y���傫�����܂��B4081Byte�ɃJ�b�g���܂��B"));
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

		//��PCM���̂�]��
		read((char*)code.c_str(), _size);

		//Padding
		for(size_t i = _size; i<iSize; i++){
			code[i] = (unsigned char)0xAA;
		}
		close();
	}
}

//==============================================================
//		�f�X�g���N�^
//--------------------------------------------------------------
//	������
//				����
//	���Ԓl
//				����
//==============================================================
DPCM::~DPCM(void)
{
}

//==============================================================
//		�R�[�h�̎擾
//--------------------------------------------------------------
//	������
//		MusicFile*	MUS		�R�[�h���o�͂���ȃf�[�^�t�@�C���E�I�u�W�F�N�g
//	���Ԓl
//				����
//==============================================================
void	DPCM::getAsm(MusicFile* MUS)
{
	*MUS << ".align	$40\n" << MUS->Header.Label << "DPCM" << m_id << ":" << endl;
	MusicItem::getAsm(MUS);
}
