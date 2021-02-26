/*******************************************************************************

			NES Sound Driver & Library	(NSD.lib)	MML Compiler

	Copyright (c) 2012 A.Watanabe (S.W.), All rights reserved.
	 For conditions of distribution and use, see copyright notice in "nsc.cpp".

*******************************************************************************/

#pragma once

class FileInput :
	public ifstream
{
protected:
//�����o�[�ϐ�
		bool	f_error;					//�G���[�����̗L��
				string		strFilename;
				size_t		nowLine;		//���݂̃��C��
	unsigned	char		readData;

//�����o�[�֐�
public:
					FileInput(void);
					~FileInput(void);
			void	fileopen(const char*	_strFileName);
			void	fileopen(const char*	_strFileName,SearchPass* _pass);
			void	StreamPointerAdd(fstream::off_type iSize);
			void	StreamPointerMove(fstream::pos_type iSize);
			void	Back(void);
			string*	GetFilename(void){return(&strFilename);};
unsigned	char	cRead();
std::streamsize		GetSize();
			size_t	GetLine(void){return(nowLine);};
			void	SetLine(size_t i){nowLine = i;};
			bool	isError(){return(f_error);};
};
