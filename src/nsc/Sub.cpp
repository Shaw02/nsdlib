/*******************************************************************************

			NES Sound Driver & Library	(NSD.lib)	MML Compiler

	Copyright (c) 2012 A.Watanabe (S.W.), All rights reserved.
	 For conditions of distribution and use, see copyright notice in "nsc.cpp".

*******************************************************************************/

// �ÓI���f��
#include "StdAfx.h"
#include "Sub.h"

//==============================================================
//		�R���X�g���N�^
//--------------------------------------------------------------
//	������
//		MMLfile*	MML			MML�t�@�C���̃I�u�W�F�N�g
//		size_t		_id			�T�u���[�`���ԍ�
//		const _CHAR	_strName[]	�I�u�W�F�N�g��
//	���Ԓl
//					����
//==============================================================
Sub::Sub(MMLfile* MML, size_t _id, const _CHAR _strName[]/* = "==== [ Sub ]===="*/):
	TrackSet(MML, _id, true, false, _strName)
{
	_OMP_INIT_LOCK(lock_TickCount)
}

//==============================================================
//		�f�X�g���N�^
//--------------------------------------------------------------
//	������
//				����
//	���Ԓl
//				����
//==============================================================
Sub::~Sub()
{
	_OMP_DESTROY_LOCK(lock_TickCount)
}

//==============================================================
//		TickCount	�T�u���[�`���Ăяo��
//--------------------------------------------------------------
//	������
//				����
//	���Ԓl
//				����
//==============================================================
unsigned	int	Sub::TickCount(MusicFile* MUS, NSD_WORK* work)
{
	int	_iResult;

	//���̃g���b�N���Ăяo���Ă��鎞�ɁA���������Ȃ��B
	_OMP_SET_LOCK(lock_TickCount)
	_iResult = ptcTrack[0]->TickCount(MUS, work);
	_OMP_UNSET_LOCK(lock_TickCount)

	return(_iResult);
}

//==============================================================
//		�R�[�h�̎擾
//--------------------------------------------------------------
//	������
//		MusicFile* MUS		�R�[�h���o�͂���ȃf�[�^�t�@�C���E�I�u�W�F�N�g
//	���Ԓl
//				����
//==============================================================
void	Sub::getAsm(MusicFile* MUS)
{
	*MUS << MUS->Header.Label << "SUB" << m_id << ":" << endl;
	TrackSet::getAsm(MUS);
}
