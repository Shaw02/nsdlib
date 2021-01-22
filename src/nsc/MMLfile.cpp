/*******************************************************************************

			NES Sound Driver & Library	(NSD.lib)	MML Compiler

	Copyright (c) 2012 A.Watanabe (S.W.), All rights reserved.
	 For conditions of distribution and use, see copyright notice in "nsc.cpp".

*******************************************************************************/

#include "StdAfx.h"
#include "MMLfile.h"

/****************************************************************/
/*					�O���[�o���ϐ��i�N���X�����ǁE�E�E�j		*/
/****************************************************************/
extern	OPSW*			cOptionSW;	//�I�v�V�������ւ̃|�C���^�ϐ�

#ifdef _OPENMP
	extern	omp_lock_t		lock_cout;
#endif

/*
Memo:
	read_char();		//�t�@�C�� or �}�N������A1Byte�ǂݍ���
	�@��cRead()			//Shift-Jis�𔼊p�ɕϊ�����@�\�t��
	    ��GetChar()		//�R�����g�E����R�[�h�̖����@�\�t��
		����GetString()
	    ����GetNum()
	    ����Get****()
	    ��SetMacro()
	    ��CallMacro()
		��GetCommandID()
*/
//==============================================================
//		�R���X�g���N�^
//--------------------------------------------------------------
//	������
//		const	char*	strFileName		�t�@�C����
//	���Ԓl
//				����
//==============================================================
MMLfile::MMLfile(string&	strFileName):
p_macro(0),
f_macro(false),
f_2to1(false),
f_error(false),
offset_Ei(0),
offset_Ev(0),
offset_En(0),
offset_Em(0),
iReleaseVolume(2),
iRepeatMode(0),
iTieMode(0),
timebase(24),
octave_reverse(false),
q_reverse(false),
rest(2),
wait(0),
QMax(8),
priority(0)
{
	try {
		//File open
		nowFile	= new FileInput();
		nowFile->fileopen(strFileName.c_str());

		//�ǂݍ��ݎ��s�H
		f_error = nowFile->isError();
		if(f_error == true){
			throw ios_base::failure(strFileName + ": " + strerror(errno));
		} else {
			ptcFiles.push_back(nowFile);
			iFiles = 0;
		}

	} catch (int no) {
		nsc_ErrMsg(no);
		f_error = true;	//�G���[�����������B
	} catch (const exception& e){
		nsc_ErrMsg(e);
		f_error = true;	//�G���[�����������B
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
MMLfile::~MMLfile(void)
{

	//----------------------
	//Delete Class

	//�J�����t�@�C����S������
	for(vector<FileInput*>::iterator it=ptcFiles.begin(), e=ptcFiles.end(); it!=e; ++it){
		if(cOptionSW->iDebug & DEBUG_Close_Inc){
			_COUT << _T("Close file :");
			cout << (*it)->GetFilename()->c_str() << endl;
		}
		(*it)->close();
		delete *it;
	}
	ptcFiles.clear();

	//�}�N����S���������B
	for(map<string,string>::iterator it=ptcMac.begin(), e=ptcMac.end(); it!=e; ++it){
		it->second.clear();
	}
	ptcMac.clear();
	lv_Mac.clear();

	//�p�b�`��S���������B
	for(map<size_t,Patch*>::iterator it=ptcPatch.begin(), e=ptcPatch.end(); it!=e; ++it){
		delete	it->second;
	}
	ptcPatch.clear();

}

//==============================================================
//			���݃R���p�C���������̃t�@�C����[EOF]�`�F�b�N
//--------------------------------------------------------------
//	������
//			����
//	���Ԓl
//			bool		true	[EOF]�����o
//						false	�܂��I��肶��Ȃ��B
//==============================================================
bool	MMLfile::eof(void)
{
	return( nowFile->eof() );
}

//==============================================================
//			MML�t�@�C���̏I���`�F�b�N
//--------------------------------------------------------------
//	������
//			����
//	���Ԓl
//			bool		true	���t�@�C����[EOF]�����o
//						false	�܂��I��肶��Ȃ��B
//==============================================================
bool	MMLfile::eom(void)
{
	if(( iFiles == 0 ) && ( eof() )){
		return(true);
	}
	return(false);
}

//==============================================================
//		�C���N���[�h�t�@�C��
//--------------------------------------------------------------
//	������
//			����
//	���Ԓl
//			����
//	������
//			���݂̃t�@�C���|�C���^�ɏ����Ă���t�@�C�����C���N���[�h����
//==============================================================
void	MMLfile::include()
{
	//----------------------
	//Local�ϐ�
	string		_name = "";
	FileInput*	_incFile;

	GetString(&_name, false);

	//----------------------
	//�����t�@�C�����J����Ă��Ȃ����`�F�b�N
	for(vector<FileInput*>::iterator it=ptcFiles.begin(), e=ptcFiles.end(); it!=e; ++it){
		if( *(*it)->GetFilename() == _name ){
			string	errMsg = "The same file \"";
			errMsg += _name + "\" has already been open.";
			Err(errMsg);
		}
	}

	//----------------------
	//File open
	_incFile	= new FileInput();

	_incFile->fileopen(_name.c_str(), &cOptionSW->m_pass_inc);
	if(_incFile->isError() == true){
		f_error = true;
		delete _incFile;		//�ǂݍ��݂Ɏ��s�����̂ŁA�����ŃN���X�����������B
		string	errMsg = "\"";
		errMsg += _name + "\" :" + strerror(errno);
		Err(errMsg);
	} else {
		nowFile = _incFile;
		ptcFiles.push_back(_incFile);
		iFiles++; 
	}
}

//==============================================================
//		�}�N���̐ݒ�
//--------------------------------------------------------------
//	������
//		int		i_Lv	�ݒ肷�郌�x��
//	���Ԓl
//				����
//	������
//			���݂̃t�@�C���|�C���^�ɏ����Ă���}�N�����`����B
//==============================================================
void	MMLfile::SetMacro(int i_Lv)
{
	char	cData;
	string	macro_name		="";
	string	macro_contents	="";

	//Debug�p
	if(cOptionSW->iDebug & DEBUG_Macros){
		cout << "Set Macro (Lv=" << i_Lv << ") : ";
	}

	int		iKakko	= 0;

	//------------------
	//�}�N�����̎擾
	while((cData = cRead()) > 0x20){
		if(cData == '{'){
			Back();
			break;
		}
		macro_name += cData;
	};

	//------------------
	//�}�N�����̏d���`�F�b�N
	if(ptcMac.count(macro_name) != 0){
		Err(_T("���ɂ��̃}�N�����͑��݂��Ă��܂��B"));
	}

	//------------------
	//�}�N�����e�̎擾
	Chk_LeftCurlyBrace();

	while((GetChar_With_Chk_RightCurlyBrace(&cData)) || (iKakko != 0)){
		if(cData == '{'){
			iKakko++;
		} else if(cData == '}'){
			iKakko--;
		}
		macro_contents += cData;
	}
	macro_contents += " ";

	//------------------
	//�}�N�����e�̐ݒ�
	ptcMac[macro_name] = macro_contents;
	lv_Mac[macro_name] = i_Lv;

	//Debug�p
	if(cOptionSW->iDebug & DEBUG_Macros){
		cout << "SetMacro ptcMac[" << macro_name << "] = \"" << macro_contents << "\"" << endl;
	}
}

//==============================================================
//		�}�N���̐ݒ�
//--------------------------------------------------------------
//	������
//		int		i_Lv	�폜���郌�x��
//	���Ԓl
//				����
//	������
//			�Y�����x���̃}�N�����폜����
//==============================================================
void	MMLfile::DeleteMacro(int i_Lv)
{

	//----------------------
	//Local�ϐ�
	string	macro_name;
	int		macro_lv;

	//Debug�p
	if(cOptionSW->iDebug & DEBUG_Macros){
		cout << "Delete Macro (Lv=" << i_Lv << ") : " << endl;
	}
		
	//----------------------
	//���YLv�̃}�N�����������B
	map<string,string>::iterator itMac = ptcMac.begin();
	while(itMac != ptcMac.end()){
		macro_name	= itMac->first;
		macro_lv	= lv_Mac[macro_name];
		itMac++;
		if(i_Lv == macro_lv){
			ptcMac.erase(macro_name);
			lv_Mac.erase(macro_name);
			//Debug�p
			if(cOptionSW->iDebug & DEBUG_Macros){
				cout << "	ptcMac[" << macro_name << "]" << endl;
			}
		}
	}
}

//==============================================================
//		�}�N���̌Ăяo��
//--------------------------------------------------------------
//	������
//				����
//	���Ԓl
//				����
//	������
//			���݂̃t�@�C���|�C���^�ɏ����Ă���}�N�����Ăяo��
//==============================================================
void	MMLfile::CallMacro(void)
{
	char							cData;
	size_t							i		= 0;
	size_t							n		= 0;
	string							_name	= "";
	vector<const char*>				vecMac;

	//Debug�p
	if(cOptionSW->iDebug & DEBUG_Macros){
		cout << "Call Macro : ";
	}

	//------------------
	//��`���ꂽ�S�}�N�����̃|�C���^�擾
	if(!ptcMac.empty()){
		for(map<string,string>::iterator itMac=ptcMac.begin(), e=ptcMac.end(); itMac != e; itMac++){
			vecMac.push_back(itMac->first.c_str());
		}
	}

	//------------------
	//�}�N�����̏ƍ�
	//���o�^����Ă���}�N���̒��ŁA��Ԓ�����v����}�N������T���B
	i = 0;
	do{
		cData = cRead();
		_name += cData;
		n = 0;			//�q�b�g��
		if(cData > 0x20){
			vector<const char*>::iterator	it = vecMac.begin();
			while(it != vecMac.end()){
				char c = (*it)[i];
				if(c == cData){
					n++;
					it++;
				} else {
					it = vecMac.erase(it);
				}
			}
		}
		i++;
	} while(n>0);		//�q�b�g����0�ɂȂ�܂ŁA�J��Ԃ��B

	Back();										//�|�C���^���P�߂��B
	_name = _name.substr(0, _name.length()-1);	//�P�������炷�B

	//------------------
	//�}�N�����̑��݃`�F�b�N
	if(ptcMac.count(_name) == 0){
		Err(_T("���̃}�N�����͑��݂��Ă��܂���B"));
	}

	//------------------
	//�}�N�����̏d���`�F�b�N
	for(i=0; i < p_macro; i++){
		if(s_macro[i].name == _name){
			Err(_T("�}�N�����œ����}�N�����Ăяo���Ă��܂��B"));
		}
	}

	//------------------
	//�l�X�g��������ۑ�
	if(p_macro > 0){
		s_macro[p_macro-1].name = nowMacro.name;
		s_macro[p_macro-1].line = nowMacro.line;
	}

	//Debug�p
	if(cOptionSW->iDebug & DEBUG_Macros){
		cout << "ptcMac[" << _name << "]�@(nest = " << p_macro << " )" << endl;
	}

	nowMacro.name = _name;
	nowMacro.line = 0;

	s_macro.push_back(nowMacro);
	p_macro++;
}

//==============================================================
//		�p�b�`�̐ݒ�
//--------------------------------------------------------------
//	������
//				����
//	���Ԓl
//				����
//	������
//			���݂̃t�@�C���|�C���^�ɏ����Ă���}�N�����`����B
//==============================================================
void	MMLfile::SetPatch(void)
{

	size_t			i		= GetNum();

	//�d���`�F�b�N
	if(ptcPatch.count(i) != 0){
		Err(_T("Patch()�u���b�N�œ����ԍ����w�肳��܂����B"));
	}

	ptcPatch[i] =  new Patch(this, i);

}

//==============================================================
//		�p�b�`�̗L���`�F�b�N
//--------------------------------------------------------------
//	������
//			size_t _no	�p�b�`�ԍ�
//	���Ԓl
//			bool				���邩�ǂ���
//	������
//			
//==============================================================
bool	MMLfile::ChkPatch(size_t _no)
{
	bool	result;

	if(ptcPatch.count(_no) == 0){
		result = false;
	} else {
		result = true;
	}
	return(result);
}

//==============================================================
//			���݃R���p�C���������̃t�@�C���|�C���^�̎擾
//--------------------------------------------------------------
//	������
//						����
//	���Ԓl
//		std::streamoff	���݂̃t�@�C���|�C���^
//==============================================================
std::streamoff	MMLfile::tellg(void)
{
	std::streamoff	i;

	if(p_macro > 0){
		i = nowMacro.line;
	} else {
		i = nowFile->tellg();
	}

	return(i);
}

//==============================================================
//			���݃R���p�C���������̃t�@�C���|�C���^���ΓI�Ɉړ�
//--------------------------------------------------------------
//	������
//			std::streamoff	iSize	�ړ��l
//	���Ԓl
//			����
//==============================================================
void	MMLfile::StreamPointerMove(std::streamoff iSize)
{
	if(p_macro > 0){
		nowMacro.line = iSize;
	} else {
		nowFile->StreamPointerMove(iSize);
	}
}

//==============================================================
//			���݃R���p�C���������̃t�@�C���|�C���^����߂�
//--------------------------------------------------------------
//	������
//			����
//	���Ԓl
//			����
//==============================================================
void	MMLfile::Back_one(void)
{
	if(f_macro == true){
		f_macro = false;
		if(p_macro > 0){
			s_macro[p_macro-1].name = nowMacro.name;
			s_macro[p_macro-1].line = nowMacro.line;
		}
		p_macro++;
		nowMacro.name = s_macro[p_macro-1].name;
		nowMacro.line = s_macro[p_macro-1].line;
		nowMacro.line--;
	} else {
		if(p_macro > 0){
			nowMacro.line--;
		} else {
			nowFile->Back();
		}
	}
}

void	MMLfile::Back(void)
{
	if(f_2to1==true){
		Back_one();
		Back_one();		//�O��̓ǂݍ��݂��}���`�o�C�g������������A�Q�߂��B
	} else {
		Back_one();
	}

}

//==============================================================
//			�PByte�ǂݍ���
//--------------------------------------------------------------
//	������
//			����
//	���Ԓl
//			char	�ǂݍ��ݒl
//==============================================================
char	MMLfile::read_char(void)
{
	char	cData;

	//���O�̓ǂݍ��݂ŁA�}�N���̏I�[�ɍs�����B
	if(f_macro == true){
		f_macro	= false;		
		s_macro.pop_back();		//�}�N���X�^�b�N���P�߂��B
	}

	if(p_macro > 0){
		//�}�N������ǂݍ��ޏꍇ
		cData = ptcMac[nowMacro.name][nowMacro.line];
		nowMacro.line++;
		//�}�N���I�[�H
		if(ptcMac[nowMacro.name].size() == nowMacro.line){
			s_macro[p_macro-1].name = nowMacro.name;
			s_macro[p_macro-1].line = nowMacro.line;
			p_macro--;
			f_macro	= true;
			if(p_macro > 0){
				nowMacro.name = s_macro[p_macro-1].name;
				nowMacro.line = s_macro[p_macro-1].line;
			}
		}
	} else {
		//�t�@�C������ǂݍ��ޏꍇ
		cData		= nowFile->cRead();
	}

	return(cData);

}

//==============================================================
//			�PByte�ǂݍ���
//--------------------------------------------------------------
//	������
//			����
//	���Ԓl
//			char	�ǂݍ��ݒl
//==============================================================
char	MMLfile::cRead(void)
{
	unsigned	char	cData = read_char();
	unsigned	char	cDataMSB;

	switch(cData){
		case(0x81):
			cDataMSB = read_char();
			f_2to1 = true;
			switch(cDataMSB){
				case(0x67):
					cData = '"';
					break;
				case(0x68):
					cData = '"';
					break;
				case(0x69):
					cData = '(';
					break;
				case(0x6A):
					cData = ')';
					break;
				case(0x6F):
					cData = '{';
					break;
				case(0x70):
					cData = '}';
					break;
				case(0x7B):
					cData = '+';
					break;
				case(0x7C):
					cData = '-';
					break;
				case(0x94):	//��
					cData = '#';
					break;
				case(0xF3):	//��
					cData = '-';
					break;
				default:
					f_2to1 = false;
					Back();
					break;
			}
			break;

		case(0x82):
			cDataMSB = read_char();
			f_2to1 = true;
			switch(cDataMSB){
				case(0x4F):
					cData = '0';
					break;
				case(0x50):
					cData = '1';
					break;
				case(0x51):
					cData = '2';
					break;
				case(0x52):
					cData = '3';
					break;
				case(0x53):
					cData = '4';
					break;
				case(0x54):
					cData = '5';
					break;
				case(0x55):
					cData = '6';
					break;
				case(0x56):
					cData = '7';
					break;
				case(0x57):
					cData = '8';
					break;
				case(0x58):
					cData = '9';
					break;
				default:
					f_2to1 = false;
					Back();
					break;
			}
			break;

		default:
			f_2to1 = false;
			break;
	}

	return(cData);
}

//==============================================================
//			�PByte�ǂݍ��݁i�`�F�b�N�t���j
//--------------------------------------------------------------
//	������
//			����
//	���Ԓl
//			char	�ǂݍ��ݒl
//	������
//		�P�����̓ǂݍ��݁B
//		�A���A�ȉ��̋L�q���������ꍇ�́A�������΂��B
//			�E0x20�ȉ��̃L�����N�^�[�R�[�h
//			�E�R�����g
//		�܂��A�C���N���[�h�t�@�C������[EOF]�����o������A�O�̃t�@�C���ɖ߂�
//==============================================================
char	MMLfile::GetChar(void)		//1Byte�̓ǂݍ���
{
	unsigned	char	cData;

	do{
		cData = cRead();

		//[EOM]�H
		if( eom() )
		{
			return(0);

		//[EOF]�H
		} else if( eof() ){
			nowFile->close();
			delete	nowFile;
			ptcFiles.pop_back();
			nowFile = ptcFiles.back();
			iFiles--;
			//continue

		//0x20�ȉ��̐���R�[�h
		} else if(cData <= 0x20){
			//continue

		//�A�Z���u������^�C�v�̃R�����g�H
		} else if(cData == ';'){
			do{
				if( eof() ){
					break;
				}
				cData = cRead();		//���̃o�C�g��ǂݍ���
			} while(cData != 0x0A);
		
		//�b����^�C�v�̃R�����g�H
		} else 	if(cData == '/'){
			cData = cRead();		//���̃o�C�g��ǂݍ���
			switch(cData){

				//�P�s�R�����g
				case('/'):
					do{
						if( eof() ){
							break;
						}
						cData = cRead();		//���̃o�C�g��ǂݍ���
					}while(cData != 0x0A);
					break;

				//�͈̓R�����g
				case('*'):
					do{
						do{
							cData = cRead();		//���̃o�C�g��ǂݍ���
							Chk_EOF();
						}while(cData != '*');
						cData = cRead();
						Back();					//StreamPointerAdd(-1);
					} while(cData != '/');
							cRead();			//StreamPointerAdd(+1); ���������B
					break;

				//����ȊO
				default:
					Err(_T("'/'���P����������܂���ł����B"));
					break;
			}

		//[EOF]�ł��A�R�����g�ł������ꍇ�B
		} else {
			break;		//while(1) �� break;
		}

	} while(true);

	return(cData);
}

//==============================================================
//			','�����邩�`�F�b�N
//--------------------------------------------------------------
void	MMLfile::Chk_Comma(void)
{
	char cData = GetChar();
	if(cData != ','){
		Err(_T("���̃p�����[�^������','��������܂���ł����B"));
	}
}

//==============================================================
//			'{'�����邩�`�F�b�N
//--------------------------------------------------------------
void	MMLfile::Chk_LeftCurlyBrace(void)
{
	// { �̌���
	while(1){
		char cData = GetChar();
		if(cData == '{'){
			break;
		} else if(cData == '='){
			//=�͋�����B
			continue;
		} else {
			//����ȊO�̓G���[�ɂ���
			Err(_T("�u���b�N�̊J�n������'{'��������܂���ł����B"));
		}
	}
}

//==============================================================
//			[EOF]�̃`�F�b�N
//--------------------------------------------------------------
void	MMLfile::Chk_EOF(void)
{
	// } ������O�ɁA[EOF]��������G���[
	if(eof()){
		Err(_T("�\������[EOF]�����o���܂����B"));
	}
}

//==============================================================
//			[EOF]�̑O��'}'�����邩�`�F�b�N
//--------------------------------------------------------------
//	������
//			chae	cData*	�ǂݍ��񂾃f�[�^���i�[����|�C���^
//	���Ԓl
//			bool			�ǂݍ��񂾃f�[�^��`}'�ł�������false
//==============================================================
bool	MMLfile::GetChar_With_Chk_RightCurlyBrace(char* cData)
{
	//�ǂݍ���
	*cData = GetChar();

	Chk_EOF();

	//`}'�̃`�F�b�N
	return(*cData != '}');
}

//==============================================================
//			������ǂݍ���
//--------------------------------------------------------------
//	������
//			bool	f_ESC	�G�X�P�[�v�V�[�P���X�̉�
//							true: ESC�V�[�P���X ok
//	���Ԓl
//			string			�ǂݍ��񂾕�����
//	������
//			MML����"��"�ň͂܂ꂽ��������擾����B
//==============================================================
void	MMLfile::GetString(string* _str, bool	f_ESC)
{
	int		i;
	char	cData;

	cData = GetChar();
	if(cData != '"'){
			Err(_T("������J�n������\"��������܂���ł����B"));
	}

	while('"' != (cData = cRead())){
		Chk_EOF();
		if((f_ESC == true) && (cData == '\\')){
			cData = GetChar();
			Chk_EOF();
			switch(cData){
				case('a'):
					cData = '\a';
					break;
				case('b'):
					cData = '\b';
					break;
				case('f'):
					cData = '\f';
					break;
				case('n'):
					cData = '\n';
					break;
				case('r'):
					cData = '\r';
					break;
				case('t'):
					cData = '\t';
					break;
				case('v'):
					cData = '\v';
					break;
				case('\''):
					cData = '\'';
					break;
				case('\"'):
					cData = '\"';
					break;
				case('\\'):
					cData = '\\';
					break;
				case('\?'):
					cData = '\?';
					break;
				case('x'):
					i = GetHex();
					cData = (char)(i & 0xFF);
					break;
				case('0'):
				case('1'):
				case('2'):
				case('3'):
				case('4'):
				case('5'):
				case('6'):
				case('7'):
					Back();
					i = GetOct();
					cData = (char)(i & 0xFF);
					break;
				default:
					*_str += '\\';
					break;
			}
		}
		*_str += cData;
	}
}

//==============================================================
//			���l�ǂݍ���
//--------------------------------------------------------------
//	������
//			����
//	���Ԓl
//			int		�ǂݍ��񂾐��l
//	������
//			MML����(��)�ň͂܂ꂽ��������擾����B
//==============================================================
int	MMLfile::GetNum(void)
{
	char	cData;
	int		iResult;

	cData = GetChar();
	if(cData != '('){
		Err(_T("���l�J�n������(��������܂���ł����B"));
	}

	iResult = GetInt();

	cData = GetChar();
	if(cData != ')'){
		Err(_T("���l�I��������)��������܂���ł����B"));
	}

	return(iResult);
}

//==============================================================
//			���l�ǂݍ���
//--------------------------------------------------------------
//	������
//			����
//	���Ԓl
//			int		�ǂݍ��񂾐��l
//==============================================================
int		MMLfile::GetInt(void)
{
				bool	neg;				//����
				int		iResult;			//���Z����
	unsigned	char	cData = GetChar();

	//16�i��
	if(cData == '$'){
		neg		= chkSigh();
		iResult = GetHex();

	//�Q�i��
	} else if(cData == '%'){
		neg		= chkSigh();
		iResult = GetBin();

	//10�i��
	} else if(((cData >= '0') && (cData <= '9')) || (cData == '-') || (cData == '+')){
		Back();		//�|�C���^���P�߂�
		neg		= chkSigh();
		iResult = GetDec();

	} else {
		Err(_T("���l�ȊO���w�肳��܂����B"));
	}

	//����
	if(neg == true){
		iResult = -iResult;
	}

	return(iResult);
}

//--------------------------------------------------------------
bool	MMLfile::chkSigh(void)
{
				bool	neg;
	unsigned	char	cData = GetChar();

	switch(cData){
		case('-'):
			neg = true;
			break;
		case('+'):
			neg = false;
			break;
		default:
			Back();		//�|�C���^���P�߂�
			neg = false;
			break;
	}
	return(neg);
}

//--------------------------------------------------------------
int	MMLfile::GetHex(void)
{
				int		iResult = 0;			//���Z����
	unsigned	char	cData = GetChar();

	while(((cData >= '0') && (cData <= '9')) || ((cData >= 'a') && (cData <= 'f')) || ((cData >= 'A') && (cData <= 'F'))){
		iResult <<= 4;
		if((cData >= '0') && (cData <= '9')){
			iResult += (unsigned int)cData - 0x30;
		} else if((cData >= 'A') && (cData <= 'F')){
			iResult += (unsigned int)cData - 0x41 + 10;
		} else if((cData >= 'a') && (cData <= 'f')){
			iResult += (unsigned int)cData - 0x61 + 10;
		}
		cData	 = cRead();
	}

	//�|�C���^���P�߂�
	Back();							//	StreamPointerAdd(-1);

	return(iResult);
}

//--------------------------------------------------------------
int	MMLfile::GetDec(void)
{
				int		iResult = 0;			//���Z����
	unsigned	char	cData = GetChar();		

	while((cData >= '0') && (cData <= '9')){
		iResult *= 10;
		iResult += (unsigned int)cData - 0x30;
		cData	 = cRead();
	}

	//�|�C���^���P�߂�
	Back();							//	StreamPointerAdd(-1);

	return(iResult);
}

//--------------------------------------------------------------
int	MMLfile::GetOct(void)
{
				int		iResult = 0;			//���Z����
	unsigned	char	cData = GetChar();		

	while((cData >= '0') && (cData <= '7')){
		iResult <<= 3;
		iResult += (unsigned int)cData - 0x30;
		cData	 = cRead();
	}

	//�|�C���^���P�߂�
	Back();							//	StreamPointerAdd(-1);

	return(iResult);
}

//--------------------------------------------------------------
int	MMLfile::GetBin(void)
{
				int		iResult = 0;			//���Z����
	unsigned	char	cData = GetChar();		

	while((cData >= '0') && (cData <= '1')){
		iResult <<= 1;
		iResult += (unsigned int)cData - 0x30;
		cData	 = cRead();
	}

	//�|�C���^���P�߂�
	Back();							//	StreamPointerAdd(-1);

	return(iResult);
}

//==============================================================
//			�����ǂݍ���
//--------------------------------------------------------------
//	������
//			int		�L�ڂ��Ȃ��ꍇ�̉���[tick]
//	���Ԓl
//			int		����[tick]
//	������
//		������ǂݍ��ށi�t�_�t���j
//==============================================================
int	MMLfile::readLength(int DefaultLength){

	char	cData;				//�ǂݍ��ݗp
	int		iLength;			//���� [tick]
	int		iDot;				//�t�_�v�Z�p [tick]
	int		iMod;
	int		i;

	//�ǂݍ���
	cData = GetChar();

	//�|�C���^���P�߂�
	Back();							//StreamPointerAdd(-1);

	//Length
	if(((cData >= '0') && (cData <= '9')) || (cData == '.')){
		if((cData >= '0') && (cData <= '9')){
			i = GetInt();
			if(i==0){
				Err(_T("������0�͎g���܂���B"));
			}
			iLength = (timebase * 4) / i;
			iMod	= (timebase * 4) % i;
			if(iMod != 0){
				Warning(_T("�����̌v�Z�Ŋ���؂�܂���ł����B�����_�͐؎̂Ă��܂��B"));
			}
		} else {
			//�t�_�����L�q�����ꍇ�B
			if(DefaultLength == -1){
				Err(_T("�������L�q���ĉ������B"));
			}
			iLength = DefaultLength;
		}

		iDot	= iLength;
		//�t�_
		while((cData = cRead()) == '.'){
			if(iLength == -1){
				Err(_T("�����̋L�q�Ȃ��ɕt�_���g���ꍇ�́A�O������l �R�}���h���L�q���ĉ������B"));
			}
			iMod = (iDot & 0x01);
			iDot >>= 1;
			if(iMod != 0){
				Warning(_T("�t�_�̌v�Z�Ŋ���؂�܂���ł����B�����_�͐؎̂Ă��܂��B"));
			}
			iLength += iDot;
		};
		Back();							//StreamPointerAdd(-1);



	//Tick
	} else if (cData == '%'){
		GetChar();	//1�i�߂�B
		iLength = GetInt();

	} else {
		//�����������Ȃ��ꍇ
		if(DefaultLength == -1){
			Err(_T("�������L�q���ĉ������B"));
		}
		iLength = -1;
	}

	return(iLength);
}

//==============================================================
//			�����ǂݍ���
//--------------------------------------------------------------
//	������
//			int		�L�ڂ��Ȃ��ꍇ�̉���[tick]
//	���Ԓl
//			int		�ǂݍ��񂾐��l
//					������������ĂȂ��ꍇ�́A -1 ��Ԃ��B
//==============================================================
int		MMLfile::GetLength(int DefaultLength)	//
{
	char	cData;				//�ǂݍ��ݗp
	int		iLength;			//���� [tick]
	int		iCalc;
	bool	add;

	//�����ǂݍ���
	iLength = readLength(DefaultLength);

	//�����̉����Z
	cData = cRead();
	while((cData == '-') || (cData == '~') || (cData == '+') || ((iTieMode == 1) && (cData == '^'))){
		if(iLength == -1){
			iLength = DefaultLength;
		}

		//��Ōv�Z���镄���̃`�F�b�N
		if((cData == '+') || (cData == '^')){
			add = true;
		} else {
			add = false;
		}
		iCalc = readLength(DefaultLength);

		if(iCalc == -1){
		//	Warning(_T("�����̉����Z�l�ɐ��l�ȊO���w�肳��Ă��܂��B"));
			iCalc = DefaultLength;
		}

		if(add == true){
			iLength += iCalc;	// '+' �� '^' �͉��Z
		} else {
			iLength -= iCalc;	// '-' �� '~' �͌��Z
		}
		cData = cRead();
	};
	Back();							//StreamPointerAdd(-1);

	if(iLength == -1){
		return(iLength);
	}

	if((iLength < 1) || (iLength > 255)){
		Err(_T("�����́A%1�i96�j�`%255�i1+1+2+8�j�̊ԂŎw�肵�ĉ������B255[tick]�𒴂���ꍇ�̓^�C`&', `^'���g���ĉ������B"));
	}

	return(iLength);
}

//==============================================================
//		�R�}���hID�̎擾
//--------------------------------------------------------------
//	������
//		Command_Info*		_command
//		size_t				_size
//	���Ԓl
//		int			�R�}���h�R�[�h�@�i-1��unknown command�j
//==============================================================
int	MMLfile::GetCommandID(const Command_Info _command[], size_t _size)
{
	std::streamoff	ptCmdEnd	= tellg();
			size_t	i = 0;					//�����p
			size_t	n = 0;					//�q�b�g���p
			int		iResult = -1;

	map<const char*, int>	mapCmdInfo;

	nowCommand.clear();

	//�����p�̃I�u�W�F�N�g�쐬
	for(i=0; i<_size; i++){
		mapCmdInfo[_command[i].str] = _command[i].id;
	}

	//����
	i = 0;
	do{
		map<const char*, int>::iterator	it = mapCmdInfo.begin();
		char cData = cRead();
		n = 0;
		while(it != mapCmdInfo.end()){
			char c = (it->first)[i];
			if(c == cData){
				n++;
				it++;
			} else if(c == 0){
				Back();
				ptCmdEnd = tellg();				//�q�b�g�����Ƃ���̃t�@�C���|�C���^���L��
				cRead();
				nowCommand.assign(it->first);	//�q�b�g�����R�}���h��
				iResult = it->second;			//�q�b�g�����R�}���hID
				mapCmdInfo.erase(it++);
			} else {
				mapCmdInfo.erase(it++);
			}
		}
		i++;
	} while((n>0) && (f_macro == false));	//�q�b�g����0�ɂȂ�܂ŁA�J��Ԃ��B

	if(f_macro == true){
		Back();
	}
	StreamPointerMove(ptCmdEnd);

	return(iResult);
}

//==============================================================
//		�G���[����
//--------------------------------------------------------------
void	MMLfile::ErrUnknownCmd()
{
	char	cData = cRead();

	nowCommand.assign(1, cData);
	Err(_T("Unknown Command"));
}

//==============================================================
//		�G���[����
//--------------------------------------------------------------
//	������
//		const	_CHAR	msg[]	�G���[���b�Z�[�W
//	���Ԓl
//				����
//==============================================================
void	MMLfile::Err(const _CHAR msg[])
{
	_OMP_SET_LOCK(lock_cout)
	f_error = true;

	//�G���[���e��\��
	if(cOptionSW->fErr == true){
		//���݂̃t�@�C�����ƁA�s����\��
		cerr << "[ ERROR ] " << nowFile->GetFilename()->c_str() << " (Line = " << nowFile->GetLine() << "): " << nowCommand << " : ";
		_CERR << msg << endl;
	} else {
		//���݂̃t�@�C�����ƁA�s����\��
		cout << "[ ERROR ] " << nowFile->GetFilename()->c_str() << " (Line = " << nowFile->GetLine() << "): " << nowCommand << " : ";
		_COUT << msg << endl;
	}
	_OMP_UNSET_LOCK(lock_cout)

	//�ُ�I��
	nsc_exit(EXIT_FAILURE);
}

//--------------------------------------------------------------
void	MMLfile::Err(const string& str)
{
	_OMP_SET_LOCK(lock_cout)
	f_error = true;

	//�G���[���e��\��
	if(cOptionSW->fErr == true){
		//���݂̃t�@�C�����ƁA�s����\��
		cerr << "[ ERROR ] " << nowFile->GetFilename()->c_str() << " (Line = " << nowFile->GetLine() << "): " << nowCommand << " : " << str.c_str() << endl;
	} else {
		//���݂̃t�@�C�����ƁA�s����\��
		cout << "[ ERROR ] " << nowFile->GetFilename()->c_str() << " (Line = " << nowFile->GetLine() << "): " << nowCommand << " : " << str.c_str() << endl;
	}
	_OMP_UNSET_LOCK(lock_cout)

	//�ُ�I��
	nsc_exit(EXIT_FAILURE);
}

//==============================================================
//		���[�j���O����
//--------------------------------------------------------------
//	������
//		const	_CHAR	msg[]	���[�j���O���b�Z�[�W
//	���Ԓl
//				����
//==============================================================
void	MMLfile::Warning(const _CHAR msg[])
{
	_OMP_SET_LOCK(lock_cout)
	//���[�j���O���e��\��
	if(cOptionSW->fErr == true){
		//���݂̃t�@�C�����ƁA�s����\��
		cerr << "[WARNING] " << nowFile->GetFilename()->c_str() << " (Line = " << nowFile->GetLine() << "): " << nowCommand << " : ";
		_CERR << msg << endl;
	} else {
		//���݂̃t�@�C�����ƁA�s����\��
		cout << "[WARNING] " << nowFile->GetFilename()->c_str() << " (Line = " << nowFile->GetLine() << "): " << nowCommand << " : ";
		_COUT << msg << endl;
	}
	_OMP_UNSET_LOCK(lock_cout)
}

//--------------------------------------------------------------
void	MMLfile::Warning(const string& str)
{
	_OMP_SET_LOCK(lock_cout)
	//���[�j���O���e��\��
	if(cOptionSW->fErr == true){
		//���݂̃t�@�C�����ƁA�s����\��
		cerr << "[WARNING] " << nowFile->GetFilename()->c_str() << " (Line = " << nowFile->GetLine() << "): " << nowCommand << " : " << str.c_str() << endl;
	} else {
		//���݂̃t�@�C�����ƁA�s����\��
		cout << "[WARNING] " << nowFile->GetFilename()->c_str() << " (Line = " << nowFile->GetLine() << "): " << nowCommand << " : " << str.c_str() << endl;
	}
	_OMP_UNSET_LOCK(lock_cout)
}
