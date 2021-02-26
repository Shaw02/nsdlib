/*******************************************************************************

			NES Sound Driver & Library	(NSD.lib)	MML Compiler

	Copyright (c) 2012 A.Watanabe (S.W.), All rights reserved.
	 For conditions of distribution and use, see copyright notice in "nsc.cpp".

*******************************************************************************/

#pragma once

class FileOutput :
	public ofstream
{
protected:
//�����o�[�ϐ�
		bool	f_error;					//�G���[�����̗L��

public:
//�����o�[�֐�
					FileOutput(void);
					~FileOutput(void);
			void	fileopen(const char*	strFileName);
			void	StreamPointerAdd(fstream::off_type iSize);
			void	StreamPointerMove(fstream::pos_type iSize);
			bool	isError(){return(f_error);};
};
