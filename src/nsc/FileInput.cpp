/*******************************************************************************

			NES Sound Driver & Library	(NSD.lib)	MML Compiler

	Copyright (c) 2012 A.Watanabe (S.W.), All rights reserved.
	 For conditions of distribution and use, see copyright notice in "nsc.cpp".

*******************************************************************************/

#include "StdAfx.h"
#include "FileInput.h"

/****************************************************************/
/*					�O���[�o���ϐ��i�N���X�����ǁE�E�E�j		*/
/****************************************************************/
extern	OPSW*			cOptionSW;	//�I�v�V�������ւ̃|�C���^�ϐ�

//==============================================================
//		�f�X�g���N�^
//--------------------------------------------------------------
//	������
//				����
//	���Ԓl
//				����
//==============================================================
FileInput::FileInput(void):
	f_error(false),
	nowLine(1),
	readData(0)
{
}

//==============================================================
//		�f�X�g���N�^
//--------------------------------------------------------------
//	������
//				����
//	���Ԓl
//				����
//==============================================================
FileInput::~FileInput(void)
{
}

//--------------------------------
//�t�@�C�����J���@�G���[�����t��
//--------------------------------
void	FileInput::fileopen(const char*	_strFileName){

	errno = 0;	//�O���[�o���ϐ� errno ���O�ɏ�����
	clear();	//�t���O�̃N���A

	open(_strFileName,ios_base::in | ios_base::binary);
	if(good()==false){
		f_error	= true;
	}
	strFilename.append(_strFileName);
};

//--------------------------------
//�t�@�C�����J���@�G���[�����t��
//--------------------------------
void	FileInput::fileopen(const char*	_strFileName,SearchPass* _pass)
{
	f_error	= true;

	//�悸�́A���̂܂�
	errno = 0;	//�O���[�o���ϐ� errno ���O�ɏ�����
	clear();	//�t���O�̃N���A
	open(_strFileName,ios_base::in | ios_base::binary);
	if(cOptionSW->flag_SearchPass){
		perror(_strFileName);
	}

	if(good()==true){
		f_error	= false;
	} else {

		//�����p�X
		size_t	iSize	= _pass->count();
		string	name;
		string	workName= string(_strFileName);
		size_t	loc		= workName.rfind('/');

		//�w��̃t�@�C���Ƀp�X��������Ă�����A�����B
		if(loc != string::npos){
			workName.erase(0, loc);		//�t�@�C�����̂�
		}

		for(size_t i=0; i<iSize; ++i){

#ifdef _WIN32
			//Windows�̏ꍇ�́A���΃p�X���܂߂Č�������iUNIX�n�͕s�j
			//�w��̌����p�X����Ƃ������΃p�X����������B
			errno = 0;	//�O���[�o���ϐ� errno ���O�ɏ�����
			clear();	//�t���O�̃N���A

			name.assign(_pass->get(i));
			name.append(_strFileName);
			open(name.c_str(),ios_base::in | ios_base::binary);
			if(cOptionSW->flag_SearchPass){
				perror(name.c_str());
			}
			if(good()==true){
				f_error	= false;
				break;
			};
#endif
			//WINDOWS, UNIX�n����
			//�����p�X�{�t�@�C�����݂̂Ō����B
			errno = 0;	//�O���[�o���ϐ� errno ���O�ɏ�����
			clear();	//�t���O�̃N���A

			name.assign(_pass->get(i));
			name.append(workName);
			open(name.c_str(),ios_base::in | ios_base::binary);
			if(cOptionSW->flag_SearchPass){
				perror(name.c_str());
			}
			if(good()==true){
				f_error	= false;
				break;
			};
		};
	};

	if(f_error == false){
		strFilename.append(_strFileName);	
	}
}

//--------------------------------
//���΃V�[�N
//--------------------------------
void	FileInput::StreamPointerAdd(fstream::off_type iSize){
	seekg(iSize,ios::cur);
};

//--------------------------------
//��΃V�[�N
//--------------------------------
void	FileInput::StreamPointerMove(fstream::pos_type iSize){
	seekg(iSize);
};

//--------------------------------
//1Byte�߂�
//--------------------------------
void	FileInput::Back(void)
{
	StreamPointerAdd(-1);
	if(readData == 0x0A){
		nowLine--;
	}

	//�X�V
	read((char*)&readData, sizeof(unsigned char));
	StreamPointerAdd(-1);
}

//--------------------------------
//1Byte�ǂݍ���
//--------------------------------
unsigned	char	FileInput::cRead()
{
	read((char*)&readData, sizeof(unsigned char));
	if(readData == 0x0A){
		nowLine++;
	}

	return(readData);
};

//--------------------------------
//�T�C�Y
//--------------------------------
std::streamsize	FileInput::GetSize(){

	std::streamsize	szData;
	fstream::pos_type	iDataT = tellg();

	seekg(0		,ios::end);
	szData = tellg();
	seekg(iDataT);

	return(szData);
};
