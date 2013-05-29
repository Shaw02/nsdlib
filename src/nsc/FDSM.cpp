#include "StdAfx.h"
#include "FDSM.h"

//==============================================================
//		�R���X�g���N�^
//--------------------------------------------------------------
//	������
//		MMLfile*			MML			MML�t�@�C���̃I�u�W�F�N�g
//		unsigned	int		_id			FDSM�ԍ�
//		const		wchar_t	_strName[]	�I�u�W�F�N�g��
//	���Ԓl
//					����
//==============================================================
FDSM::FDSM(MMLfile* MML, unsigned int _id, const wchar_t _strName[]):
	MusicItem(_strName),
	m_id(_id)
{
	//----------------------
	//Local�ϐ�


//	�萔��`
enum	Command_ID_mml {
	FDSM_Num,
	FDSM_Commma
};

//	�����́AMML�\���Ŏg����R�}���h�B
const	static	Command_Info	Command[] = {
		{	"%",	FDSM_Num	},
		{	"$",	FDSM_Num	},
		{	"0",	FDSM_Num	},
		{	"1",	FDSM_Num	},
		{	"2",	FDSM_Num	},
		{	"3",	FDSM_Num	},
		{	"4",	FDSM_Num	},
		{	"5",	FDSM_Num	},
		{	"6",	FDSM_Num	},
		{	"7",	FDSM_Num	},
		{	"8",	FDSM_Num	},
		{	"9",	FDSM_Num	},
		{	",",	FDSM_Commma	}
};

				int		i;
	unsigned	char	cData;
	unsigned	int		ptFDSM		= 0;
				string	WAVE;

	//------------------------------
	//�N���X�̏����ݒ�
	WAVE.clear();

	//------------------------------
	//�R���p�C��

	// { �̌���
	while(MML->cRead() != '{'){
		if(MML->eof()){
			MML->Err(L"�u���b�N�̊J�n������{��������܂���B");
		}
	}

	code.resize(0);

	// } ������܂ŁA�L�q�u���b�N�����R���p�C������B
	while((cData = MML->GetChar()) != '}'){
		
		// } ������O�ɁA[EOF]��������G���[
		if( MML->eof() ){
			MML->Err(L"�u���b�N�̏I�[������`}'������܂���B");
		}

		//�P�߂�
		MML->Back();


		//�e�R�}���h���̏���
		switch(MML->GetCommandID(Command, sizeof(Command)/sizeof(Command_Info))){

			case(FDSM_Num):
				MML->Back();
				i = MML->GetInt();
				if( (i<0) || (i>7)){
					MML->Err(L"FDSM�̔g�`�p�^�[����0�`7�͈̔͂Ŏw�肵�ĉ������B");
				}
				WAVE.append((char)1, (char)i & 0x07);
				ptFDSM++;
				break;

			case(FDSM_Commma):
				break;

			//unknown command
			default:
				MML->Err(L"unknown command");
				break;
		}
	}
	if(ptFDSM < 32){
		MML->Err(L"�g�`�p�^�[����32�̐������L�q���Ă��������B32�ɖ����Ȃ��ł��B");
	}
	if(ptFDSM > 32){
		MML->Err(L"�g�`�p�^�[����32�̐������L�q���Ă��������B32�𒴂��Ă��܂��B");
	}
	i = 0;
	while(ptFDSM>0){
		code.append((char)1, WAVE[i]);
		i++;
		ptFDSM--;
	}


	iSize = code.size();
}

//==============================================================
//		�f�X�g���N�^
//--------------------------------------------------------------
//	������
//				����
//	���Ԓl
//				����
//==============================================================
FDSM::~FDSM(void)
{
}
//==============================================================
//		�R�[�h�̎擾
//--------------------------------------------------------------
//	������
//		MusicFile*	MUS		�R�[�h���o�͂���ȃf�[�^�t�@�C���E�I�u�W�F�N�g
//	���Ԓl
//				����
//==============================================================
void	FDSM::getAsm(MusicFile* MUS)
{
	*MUS << MUS->Header.Label.c_str() << "FDSM" << m_id << ":" << endl;
	MusicItem::getAsm(MUS);
}