/*******************************************************************************

			NES Sound Driver & Library	(NSD.lib)	MML Compiler

	Copyright (c) 2012 A.Watanabe (S.W.), All rights reserved.
	 For conditions of distribution and use, see copyright notice in "nsc.cpp".

*******************************************************************************/

#pragma once

/****************************************************************/
/*																*/
/*			�萔��`											*/
/*																*/
/****************************************************************/
typedef	struct {
		string				name;
		size_t				line;
} Macro_Stack;

/****************************************************************/
/*																*/
/*			�N���X��`											*/
/*																*/
/****************************************************************/
class MMLfile	// :
//	public FileInput
{
//�����o�[�ϐ�
private:
				string				nowCommand;			//���݃R���p�C�����̃R�}���h
	vector	<	FileInput*	>		ptcFiles;			//MML�t�@�C��
				FileInput*			nowFile;			//���݂̃t�@�C��
				size_t				iFiles;				//���݂̃t�@�C��No.

	map		< string,		string>	ptcMac;				//Macro������̕ۑ�
	map		< string,		int>	lv_Mac;				//Macro�̊K�w

	vector	<	Macro_Stack	>		s_macro;			//�l�X�g���̃}�N�����X�^�b�N
				Macro_Stack			nowMacro;
				size_t				p_macro;			//���l�X�g�ځH

				bool				f_macro;			//�}�N���������I�������ɗ��t���O
				bool				f_2to1;				//�}���`�o�C�g������ϊ������H
				bool				f_error;			//�G���[�����̗L��
public:
	map		<	size_t,	Patch*>		ptcPatch;			//Patch

				size_t				offset_Ei;			//
				size_t				offset_Ev;			//
				size_t				offset_En;			//
				size_t				offset_Em;			//
				int					iReleaseVolume;		//
				int					iRepeatMode;		//
				int					iTieMode;			//
				int					timebase;			//
				bool				octave_reverse;		//Octave Reverse
				bool				q_reverse;			//q, Q q_reverse
				int					rest;				// r �R�}���h�̋���
				int					wait;				// w �R�}���h�̋���
	unsigned	int					QMax;				// qMax
	unsigned	char				priority;

//�����o�[�֐�
public:
	MMLfile(string&	strFileName);
	~MMLfile(void);

				bool	eof(void);				//���݂̃t�@�C����EOF�`�F�b�N
				bool	eom(void);				//�l�l�k�̏I���`�F�b�N

				void	include();				//���|�C���^�ɂ���t�@�C����#include����
	
				void	SetMacro(int i_Lv);
				void	DeleteMacro(int i_Lv);
				void	CallMacro(void);
				size_t	GetMacroNest(void){return(p_macro);};

				void	SetPatch(void);
				bool	ChkPatch(size_t _no);

	fstream::pos_type	tellg(void);					//���݂̃t�@�C���̃|�C���^�擾
				void	StreamPointerMove(fstream::pos_type iSize);	//���݂̃t�@�C���̃|�C���^�ړ�
private:		void	Back_one(void);					//1�����߂�
public:			void	Back(void);						//1�����߂��i�S�p�E���p�ϊ��Ή��j

private:		char	read_char(void);				//1Byte�ǂݍ���
public:			char	cRead(void);					//1Byte�ǂݍ��݁i�S�p�E���p�ϊ��Ή��j
				bool	GetChar_With_Chk_RightCurlyBrace(char* cData);		//1Byte�ǂݍ��� '}'�`�F�b�N�t��
				char	GetChar(void);					//1Byte�ǂݍ��݁iwith EOF & Commend check�j
				void	Chk_Comma(void);				//`,'�̃`�F�b�N
				void	Chk_LeftCurlyBrace(void);		//'{'������܂Ń|�C���^��i�߂�
				void	Chk_EOF(void);					//[EOF]�`�F�b�N
				void	GetString(string* _str, bool f_ESC);	//""�t ������ �ǂݍ���
				int		GetNum(void);					//()�t  ���l  �ǂݍ���
				int		GetInt_With_Chk_Range(const _CHAR msg[], int min, int max);	//���l�ǂݍ���
				int		GetInt(void);					//���l�ǂݍ���
				bool	chkSigh(void);					//�����`�F�b�N
				int		GetHex(void);					//16�i���ǂݍ���
				int		GetDec(void);					//10�i���ǂݍ���
				int		GetOct(void);					// 8�i���ǂݍ���
				int		GetBin(void);					// 2�i���ǂݍ���
				int		GetLength(int DefaultLength);	//�����ǂݍ��݁i���Z�t���j
				int		readLength(int DefaultLength);	//�����ǂݍ���

				int		GetCommandID(const Command_Info _command[], size_t _size);	//�R�}���hID���擾

				size_t	GetLine(void){return(nowFile->GetLine());};
				void	SetLine(size_t i){nowFile->SetLine(i);};

				void	ErrUnknownCmd();
				void	Err(const _CHAR msg[]);
				void	Warning(const _CHAR msg[]);
				void	Err(const string& str);
				void	Warning(const string& str);
				void	Chk_Range(const _CHAR msg[], int min, int max, int value);

				bool	isError(){return(f_error);};
};
