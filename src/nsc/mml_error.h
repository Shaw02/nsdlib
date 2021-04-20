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
class mml_error_base
{
//�����o�[�ϐ�
protected:
			string	m_location;		//�G���[�̔����ʒu

	const	_CHAR*	m_message;		//�G���[���e

//�����o�[�֐�
public:
					mml_error_base(const _CHAR* _Message);
    				mml_error_base(MMLfile& MML, const _STRING& _Message);
    				mml_error_base(MMLfile& MML, const _CHAR* _Message);
	virtual			~mml_error_base(void);
	virtual	void	out_what();
};

/****************************************************************/
class mml_error : public mml_error_base, public exception
{
public:
					mml_error(const _CHAR* _Message):
						mml_error_base(_Message){};
    				mml_error(MMLfile& MML, const _STRING& _Message):
						mml_error_base(MML, _Message){};
    				mml_error(MMLfile& MML, const _CHAR* _Message):
						mml_error_base(MML, _Message){};
	virtual			~mml_error(void){};
};

//--------------------------------------------------------------
class mml_no_LeftCurlyBrace : public mml_error
{
public:
    				mml_no_LeftCurlyBrace(MMLfile& MML, const _CHAR* _Message = _T("�u���b�N�̊J�n������'{'��������܂���ł����B")):
						mml_error(MML, _Message){};
	virtual			~mml_no_LeftCurlyBrace(void){};
};

//--------------------------------------------------------------
class mml_no_expected_eof : public mml_error
{
public:
    				mml_no_expected_eof(MMLfile& MML, const _CHAR* _Message = _T("�\������[EOF]�����o���܂����B")):
						mml_error(MML, _Message){};
	virtual			~mml_no_expected_eof(void){};
};

//--------------------------------------------------------------
class mml_lack_parameter : public mml_error
{
public:
    				mml_lack_parameter(MMLfile& MML, const _CHAR* _Message = _T("���̃p�����[�^������','��������܂���ł����B")):
						mml_error(MML, _Message){};
	virtual			~mml_lack_parameter(void){};
};

//--------------------------------------------------------------
class ios_failure : public ios_base::failure
{
public:
    				ios_failure(const string& _Filename, int _ErrNo):
						ios_base::failure(_Filename + ": " + strerror(_ErrNo)){};
    				ios_failure(const string& _Filename, const char* _Message):
						ios_base::failure(_Filename + ": " + _Message){};
	virtual			~ios_failure(void){};
};

//--------------------------------------------------------------
class mml_ios_failure : public mml_error_base, public ios_failure
{
public:
    				mml_ios_failure(MMLfile& MML, const string& _Filename, int _ErrNo):
						ios_failure(_Filename, _ErrNo),
						mml_error_base(MML, _T("�t�@�C�����J���܂���ł����B")){};
    				mml_ios_failure(MMLfile& MML, const string& _Filename, const char* _Message):
						ios_failure(_Filename, _Message),
						mml_error_base(MML, _T("�t�@�C�����J���܂���ł����B")){};
	virtual			~mml_ios_failure(void){};
	void			out_what();
};
