/*******************************************************************************

			NES Sound Driver & Library	(NSD.lib)	MML Compiler

	Copyright (c) 2012 A.Watanabe (S.W.), All rights reserved.
	 For conditions of distribution and use, see copyright notice in "nsc.cpp".

*******************************************************************************/

#pragma once
#include "MusicItem.h"

/****************************************************************/
/*																*/
/*			�N���X��`											*/
/*																*/
/****************************************************************/
class TrackSet :
	public MusicItem
{
//�����o�[�ϐ�
protected:
				string			label;		//�Ȗ�
				string			author;		//��Ȏ�
				int				time;		//���t����[msec]
				int				fade;		//�t�F�[�h�A�E�g����[msec]

				int				iTempo;		//�e���|
				bool			fJump;		//j1�R�}���h�����������H

	map<size_t,	MusicTrack*	>	ptcTrack;	//�g���b�N�E�I�u�W�F�N�g�̃|�C���^
				MusicTrack*		nowTrack;	//�R���p�C�����̃g���b�N
				size_t			iTrack;		//�R���p�C�����̃g���b�N�ԍ�
				size_t			maxTrack;	//�g���b�N�ԍ��̍ő�l
				bool			fSub;		//�T�u���[�`���L�q�u���b�N���ǂ����̃t���O
				bool			fSE;		//SE���ǂ����̃t���O
		fstream::pos_type		TrackPt;	//
				size_t			TrackLine;	//
	unsigned	char			Priority;	//���ʉ��D�揇��

//�����o�[�֐�
public:
				TrackSet(MMLfile* MML, size_t _id, bool _sub, bool _se, const _CHAR _strName[] = _T("==== [ Track Set ]===="));
				~TrackSet(void);

		void	TickCount(MusicFile* MUS);
		void	TickCountPrint(MusicFile* MUS, size_t iStart, size_t iEnd);
		void	getAsm(MusicFile* MUS);

		void	Fix_Address(MusicFile* MUS);

		void	TrackChk(MMLfile* MML);
		void	TrackProc(MMLfile* MML);
	MusicTrack*	makeTrack(MMLfile* MML, size_t _track);
	MusicTrack*	getTrack(MMLfile* MML, size_t _track);

		void	SetEvent(MusicItem* _item);		//�C�x���g�̒ǉ�

		//----------------------------------
		//�ȑS�̂Ɍ����l�l�k�R�}���h

		//���ʉ��̗D�揇��
		void	Set_Priority(MMLfile* MML);

		//������
		void	SetJumpDrv(MMLfile* MML);

		//�e���|����
		void	SetTempo(MMLfile* MML);
		void	SetRelativeTempo(MMLfile* MML);
		void	TempoUp();
		void	TempoDown();

		//���W�X�^����
		void	Set_Poke(MMLfile* MML);				//��������������
		void	Set_VRC7_Write(MMLfile* MML);		//VRC7	���W�X�^��������

		void	Set_FDS_Frequency(MMLfile* MML);	//FDS	���W�����[�^���g��
		void	Set_FDS_Volume(MMLfile* MML);		//FDS	�}�X�^�[����
		void	Set_N163_Channel(MMLfile* MML);		//N16x	�`�����l����
		void	Set_FME7_Frequency(MMLfile* MML);	//SN5B	�G���x���[�v���g��

		//----------------------------------
		//MetaItem
		bool	isLabel(void){if(label.size() == 0){return(false);} else {return(true);};}
		bool	isAuthor(void){if(author.size() == 0){return(false);} else {return(true);};}
		bool	isTime(void){if(time < 0){return(false);} else {return(true);};}
		bool	isFade(void){if(time < 0){return(false);} else {return(true);};}

		string*	getLabel(void){return(&label);};
		string*	getAuthor(void){return(&author);};
		int		getTime(void){return(time);};
		int		getFade(void){return(fade);};
};
