/*******************************************************************************

			NES Sound Driver & Library	(NSD.lib)	MML Compiler

	Copyright (c) 2012 A.Watanabe (S.W.), All rights reserved.
	 For conditions of distribution and use, see copyright notice in "nsc.cpp".

*******************************************************************************/

#pragma once

/****************************************************************/
/*																*/
/*			�N���X��`											*/
/*																*/
/****************************************************************/
class MusicFile :
	public FileOutput, public MusicItem
{
//�����o�[�ϐ�
public:
	MusicHeader				Header;		//Header
	DPCMinfo*				cDPCMinfo;	//��PCM

	map<size_t, FDSC*	>	ptcFDSC;	//FDS  wave table (career)
	map<size_t, FDSM*	>	ptcFDSM;	//FDS  wave table (modulator)
	map<size_t, VRC7*	>	ptcVRC7;	//VRC7 User Instrument
	map<size_t, N163*	>	ptcN163;	//N163 wave table
	map<size_t, Envelop*>	ptcEnv;		//Envelop
	map<size_t, BGM*	>	ptcBGM;		//BGM
	map<size_t, SE*		>	ptcSE;		//SE 
	map<size_t, Sub*	>	ptcSub;		//Subroutine Sequence

private:
	string					dpcm_code;

				bool		f_is_track_time;	//#time		�R�}���h�����������H
				bool		f_is_track_fade;	//#fade		�R�}���h�����������H
				bool		f_is_track_label;	//#label	�R�}���h�����������H
				bool		f_is_track_auth;	//#composer	�R�}���h�����������H

//�����o�[�֐�
public:
	MusicFile(MMLfile* MML, string _code, const _CHAR _strName[]=_T("================ [ Music ] ================"));
	~MusicFile(void);

	void	TickCount(void);

	size_t	SetDPCMOffset(size_t iMusSize);

	void	Fix_Address(void);

	std::streamsize	read_bin(string* _str, NSF_Header* nsf_hed);		//.bin��ǂݍ���
	size_t	make_mus(string* _str, size_t rom_size, size_t ptOffset);	//�o�C�i���[�����
	size_t	make_dpcm(string* _str);
	size_t	make_bin(NSF_Header* NSF_Hed, string* NSF_Data);

	//�ۑ��t�F�[�Y
	void	saveNSF(string& strFileName);
	void	saveNSFe(string& strFileName);
	void	saveASM(string& strFileName);

	void	Err(const _CHAR msg[]);
	void	Err(const _CHAR msg[], size_t no);
	void	Warning(const _CHAR msg[]);

};
