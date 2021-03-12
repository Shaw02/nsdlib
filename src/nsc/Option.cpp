/*******************************************************************************

			NES Sound Driver & Library	(NSD.lib)	MML Compiler

	Copyright (c) 2012 A.Watanabe (S.W.), All rights reserved.
	 For conditions of distribution and use, see copyright notice in "nsc.cpp".

*******************************************************************************/

#include "StdAfx.h"
#include "Option.h"


#ifdef _WIN32
// WINDOWS�̂݁A'/'�ɑΉ�����B
#define OPTCHK (argv[iCount][0]=='/')||(argv[iCount][0]=='-')
#else
// unix�n�ł̓X���b�V���̓f�B���N�g���̋�؂�Ɏg����ׁA���s���鎖������
#define OPTCHK (argv[iCount][0]=='-')
#endif

//==============================================================
//		�I�v�V��������
//--------------------------------------------------------------
//	������
//			int		argc	�I�v�V����������̐�
//			_TCHAR*	argv[]	�I�v�V����������
//	���Ԓl
//			����
//	�����l
//			�I�v�V�����Ƀt�@�C�������w�肳��Ȃ��ꍇ�́A�w���v�\�����ďI��
//==============================================================
OPSW::OPSW(int argc, char* argv[]):
	//�������ݒ�
	fOptionError(false),
	fHelp(false),			//�w���v�́A�f�t�H���g�͕\�����Ȃ��B
	fErr(false),
	saveNSF(false),
	saveNSFe(false),
	saveASM(false),
	flag_Optimize(false),
	flag_OptObj(true),
	flag_OptSeq(true),
	flag_SearchPass(false),
	iNSF_version(2),
	iDebug(0)
{

	try {

		//----------------------------------
		//��Local �ϐ�
		
		int		iCount = 1;			//while�̃J�E���g�p�i�R�}���h���͔�΂��j
		int		iResult;			//�ėp

		//Option�����p
		int		iOptionChk;			//�I�v�V�����`�F�b�N�p�@�|�C���^
		char	cOption;			//�I�v�V�����`�F�b�N�p�@����
		char	iFlagFilnameExt;	//�g���q���������̃t���O

		//�����p�X
		string	str_code	= "";
		string	str_dmc		= "";
		string	str_inc		= "";

		//----------------------------------
		//���I�v�V��������
		while(iCount!=argc)
		{
			//--------------
			//�I�v�V�����X�C�b�`�Ƀn�C�t��(or�X���b�V��)�����邩�m�F
			if(OPTCHK){

				//--------------
				//��Option Switch	�i�X���b�V�����������ꍇ�̏����j
				switch(argv[iCount][1]){
					//--------
					//Help�\��
					case 'h' :
					case 'H' :
					case '?' :
						fHelp=true;
						break;
					//--------
					//�œK���ݒ�
					case 'o' :
					case 'O' :
						{
							bool fTemp;
							switch(argv[iCount][3]){
								case '0' :
								case '+' :
									fTemp = true;
									break;
								case '1' :
								case '-' :
									fTemp = false;
									break;
								default :
									throw string(argv[iCount]);
									break;
							}
							switch(argv[iCount][2]){
								case 'd' :
								case 'D' :
									flag_Optimize = fTemp;
									break;
								case 'o' :
								case 'O' :
									flag_OptObj = fTemp;
									break;
								case 's' :
								case 'S' :
									flag_OptSeq = fTemp;
									break;
								default :
									throw string(argv[iCount]);
									break;
							}
							break;
						}
					//--------
					//�A�Z���u�������
					case 'a' :
					case 'A' :
						saveASM = true;
						break;
					//--------
					//�m�r�e��
					case 'n' :
					case 'N' :
						saveNSF = true;
						break;
					//--------
					//NSFe��
					case 'x' :
					case 'X' :
						saveNSFe = true;
						break;
					//--------
					//�W���G���[�o�͂�
					case 'e' :
					case 'E' :
						fErr = true;
						break;
					//--------
					//�m�r�e��
					case 'v' :
					case 'V' :
						switch(argv[iCount][2]){
							case 0x00 :
							case '1' :
								iNSF_version = 1;
								break;
							case '2' :
								iNSF_version = 2;
								break;
							default :
								throw string(argv[iCount]);
								break;
						}
						break;
					//--------
					//Tick Count
			//		case 't' :
			//		case 'T' :
			//			flag_TickCount = false;
			//			break;
					//--------
					//Search Pass
					case 's' :
					case 'S' :
						flag_SearchPass = true;
						break;
					//--------
					//Debug�̎w��
					case 'D' :
						//0x01:	Phase [1] : Creating Class Object process
						//0x10:	Phase [2] : Optimizing process
						//0x20:	Phase [3] : Setting Address
						//0x40:	Phase [4] : Outputing Music File
						//0x80:	Phase [5] : Delete Class Object
						int	i;
						iResult=sscanf(argv[iCount],"-D%d",&i);
						iDebug = i;
						if((iResult==0)||(iResult==EOF)){
							throw string(argv[iCount]);
							break;
						};
						break;
					//--------
					//Debug�̎w��
					case 'l' :
					case 'L' :
						//���Ɏw�肳��Ă���H
						if(strCodeName.empty()){
							iFlagFilnameExt=0;		//�g���q�̗L���@Reset
							iOptionChk=0;
							while((cOption=argv[iCount][iOptionChk+2])!=0)
							{
								strCodeName+=cOption;
								if(cOption=='.'){iFlagFilnameExt=1;};
								iOptionChk++;
							};
							if(iFlagFilnameExt==0){
								strCodeName+=".bin";
							};
						} else {
							throw _T("-l Code �t�@�C����2��ȏ�w�肳��܂����B");
							break;
						};
						break;
					//--------
					//ROM�R�[�h�̌����p�X
					case 'c' :
					case 'C' :
						str_code += &argv[iCount][2];
						str_code.append(1, _PATH_SPLIT);
						break;
					//--------
					//��PCM�̌����p�X
					case 'p' :
					case 'P' :
						str_dmc += &argv[iCount][2];
						str_dmc.append(1, _PATH_SPLIT);
						break;
					//--------
					//�C���N���[�h�t�@�C���̌����p�X
					case 'i' :
					case 'I' :
						str_inc += &argv[iCount][2];
						str_inc.append(1, _PATH_SPLIT);
						break;
					//--------
					//�t�@�C���̎w��
					case 'f' :
					case 'F' :
						//��ɁA�t�@�C�����������Ă��邩�`�F�b�N�B
						if(argv[iCount][3]==0){
							throw _T("-f �t�@�C�����������Ă���܂���B");
							break;
						};
						switch(argv[iCount][2]){
						//--------
						//Assembly�t�@�C���̎w��
						case 'A' :
						case 'a' :
							//���Ɏw�肳��Ă���H
							if(strASMname.empty()){
								iFlagFilnameExt=0;		//�g���q�̗L���@Reset
								iOptionChk=0;
								while((cOption=argv[iCount][iOptionChk+3])!=0)
								{
									strASMname+=cOption;
									if(cOption=='.'){iFlagFilnameExt=1;};
									iOptionChk++;
								};
								if(iFlagFilnameExt==0){
									strASMname+=".s";
								};
							} else {
								throw _T("-fa ASM �t�@�C����2��ȏ�w�肳��܂����B");
								break;
							};
							break;
						//--------
						//NSF�t�@�C���̎w��
						case 'N' :
						case 'n' :
							//���Ɏw�肳��Ă���H
							if(strNSFname.empty()){
								iFlagFilnameExt=0;		//�g���q�̗L���@Reset
								iOptionChk=0;
								while((cOption=argv[iCount][iOptionChk+3])!=0)
								{
									strNSFname+=cOption;
									if(cOption=='.'){iFlagFilnameExt=1;};
									iOptionChk++;
								};
								if(iFlagFilnameExt==0){
									strNSFname+=".nsf";
								};
							} else {
								throw _T("-fn NSF �t�@�C����2��ȏ�w�肳��܂����B");
								break;
							};
							break;
						//--------
						//NSFe�t�@�C���̎w��
						case 'X' :
						case 'x' :
							//���Ɏw�肳��Ă���H
							if(strNSFename.empty()){
								iFlagFilnameExt=0;		//�g���q�̗L���@Reset
								iOptionChk=0;
								while((cOption=argv[iCount][iOptionChk+3])!=0)
								{
									strNSFename+=cOption;
									if(cOption=='.'){iFlagFilnameExt=1;};
									iOptionChk++;
								};
								if(iFlagFilnameExt==0){
									strNSFename+=".nsfe";
								};
							} else {
								throw _T("-fn NSFe �t�@�C����2��ȏ�w�肳��܂����B");
								break;
							};
							break;
						default :
							throw string(argv[iCount]);
							break;
						};
					break;
					//--------
					//�f�t�H���g
					default :
						throw string(argv[iCount]);
						break;
				};

			} else{

				//--------------
				//���t�@�C����	�i�X���b�V�������������ꍇ�̏����j
				//���Ɏw�肳��Ă���H
				if(strMMLname.empty()){
					iFlagFilnameExt=0;		//�g���q�̗L���@Reset
					iOptionChk=0;		
					while((cOption=argv[iCount][iOptionChk])!=0)
					{
						strMMLname+=cOption;
						if(cOption=='.'){iFlagFilnameExt=1;};
						iOptionChk++;
					};
					if(iFlagFilnameExt==0){
						strMMLname+=".mml";
					};
				} else {
					throw _T("MML�t�@�C����2��ȏ�w�肳��܂����B");
					break;
				};

			};

			//--------------
			//�����̃I�v�V����
			iCount++;
		};

		//----------------------------------
		//���I�v�V�����Ŏw�肳�ꂽ������������B

		//--------------
		//�w���v�\��
		//�t�@�C������������Ȃ������ꍇ���A�w���v��\������B
		if((fHelp==true)||(strMMLname.empty())){print_help();};

		if(strASMname.empty()){
			iOptionChk=0;		
			while((cOption=strMMLname[iOptionChk])!='.')
			{
				strASMname+=cOption;
				iOptionChk++;
			};
			strASMname+=".s";
		};

		if(strNSFname.empty()){
			iOptionChk=0;		
			while((cOption=strMMLname[iOptionChk])!='.')
			{
				strNSFname+=cOption;
				iOptionChk++;
			};
			strNSFname+=".nsf";
		};

		if(strNSFename.empty()){
			iOptionChk=0;		
			while((cOption=strMMLname[iOptionChk])!='.')
			{
				strNSFename+=cOption;
				iOptionChk++;
			};
			strNSFename+=".nsfe";
		};

		//----------------------------------
		//�������p�X�̐ݒ�

		//MML�̃f�B���N�g�����擾
		iResult	= 0;
		for(iCount=0; iCount<(int)strMMLname.size(); ++iCount){
			if((strMMLname[iCount] == '\\') || (strMMLname[iCount] == '/')){
				iResult = iCount;
			}
		}

		//�D�揇�ʂP�@�J�����g�p�X
		//�D�揇�ʂQ�@MML�t�@�C�������݂���p�X
		if(iResult > 0){
			string	str_mmldir = "";
			for(iCount=0; iCount<iResult; ++iCount){
				str_mmldir += strMMLname[iCount];
			}
			m_pass_code.add(&str_mmldir);
			m_pass_dmc.add(&str_mmldir);
			m_pass_inc.add(&str_mmldir);
		}

		//�D�揇�ʂR�@���ϐ��Ŏw��̃p�X
		m_pass_code.add(getenv("PATH"));
		m_pass_dmc.add(getenv("DMC_INCLUDE"));

		//�D�揇�ʂS�@�I�v�V�����Ŏw��̃p�X
		m_pass_code.add(&str_code);
		m_pass_dmc.add(&str_dmc);
		m_pass_inc.add(&str_inc);



		//----------------------------------
		//��

		//	to do	���̑��̃I�v�V������ǉ������Ƃ��́A���̕ӂɒǋL����B

		//----------
		//Debug�p �\��
	//	cout << "MML = " << strMMLname << endl;
	//	cout << "NSF = " << strNSFname << endl;
	//	cout << "NSFe = " << strNSFename << endl;
	//	cout << "BIN = " << strBINname << endl;
	//	cout << "ASM = " << strASMname << endl;
	//	cout << "C   = " << strCname << endl;

	/*
		cout << "code:" << endl;
		m_pass_code.debug();

		cout << "dmc:" << endl;
		m_pass_dmc.debug();

		cout << "inc:" << endl;
		m_pass_inc.debug();
	*/

	} catch (const _CHAR* stErrMsg) {
		if(fErr == true){
			_CERR	<<	_T("�s���ȃI�v�V�����F");
			_CERR	<<	stErrMsg << endl;
		} else {
			_COUT	<<	_T("�s���ȃI�v�V�����F");
			_COUT	<<	stErrMsg << endl;
		}
		fOptionError = true;	//�I�v�V���������ŃG���[�����������B
		throw EXIT_FAILURE;		//���X���[����
	} catch (const string& str) {
		if(fErr == true){
			_CERR	<<	_T("�s���ȃI�v�V�����F") << str.c_str() << endl;
		} else {
			_COUT	<<	_T("�s���ȃI�v�V�����F") << str.c_str() << endl;
		}
		fOptionError = true;	//�I�v�V���������ŃG���[�����������B
		throw EXIT_FAILURE;		//���X���[����
	} catch (...) {
		fOptionError = true;	//�I�v�V���������ŃG���[�����������B
		throw;					//���X���[����
	}
};

//==============================================================
//		�f�X�g���N�g
//--------------------------------------------------------------
//	������
//			�Ȃ�
//	���Ԓl
//			����
//==============================================================
OPSW::~OPSW(){

};

//==============================================================
//		�w���v���b�Z�[�W
//--------------------------------------------------------------
//	������
//			�Ȃ�
//	���Ԓl
//			����
//==============================================================
void	OPSW::print_help(){

	_COUT	<<	_T("MML Compiler for NES Sound Driver & Library\n")
				_T("\n")
				_T("  Usage : nsc [ -options ] [file(.mml)]\n")
				_T("\n")
				_T("  -A			Compile to assembly langage.\n")
				_T("  -N			Compile to NSF music format.\n")
				_T("  -X			Compile to NSFe music format.\n")
				_T("  -V[num]		Specify the NSF Version.\n")
			//	_T("  -T			Disable to output the tick counting result.\n")
				_T("  -Od[+/-]		Optimize the NSF bank struct of the delta-PCM.\n")
				_T("  -Oo[+/-]		Optimize the object data.\n")
				_T("  -Os[+/-]		Optimize the sequence data.\n")
				_T("  -E			Error/Warning messages out the stadard error.\n")
				_T("  -S			Enable outout the search pass result.\n")
				_T("  -L[file(.bin)]	Filename of the rom code for NSF.\n")
				_T("  -FA[file(.s  )]	Filename of the output assembly langage file.\n")
				_T("  -FN[file(.nsf)]	Filename of the output NSF music format.\n")
				_T("  -FX[file(.nsfe)]	Filename of the output NSFe music format.\n")
				_T("  -C[dir]		Search pass of the rom code for NSF.\n")
				_T("  -P[dir]		Search pass of the delta-pcm.\n")
				_T("  -I[dir]		Search pass of the include file.\n")
				_T("  -H			Print the this help.")	<<	endl;

	throw EXIT_SUCCESS;
};
