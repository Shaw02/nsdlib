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
class MusicTrack :
	public MusicItem
{
//�����o�[�ϐ�
public:
	//----------------------------------
	//�����h���C�o�@�V�~�����[�g�pWORK
	NSD_WORK	nsd;

private:
	//----------------------------------
	//Tick �J�E���g�p
				int		iTickTotal;
				int		iTickLoop;

	//----------------------------------
	//�R���p�C������
				size_t	offset_now;				//���݂̃I�t�Z�b�g
				bool	compile_flag;			//���݃R���p�C�����H

				bool	jump_flag;				// J

	//----------------------------------
	//����
				int		opt_DefaultLength;		//�����ȗ����̏����p

	//----------------------------------
	//�N�I���^�C�Y
				int		QMax;					//QMax
				int		gatetime_q;				//q
				int		gatetime_Q;				//Q

	//----------------------------------
	//�m�[�g
	mml_note*			_old_note;

	//----------------------------------
	//��������p
				char	KeySignature[8];		//����(c,d,e,f,g,a,b,r)
				char	nowKey;					//���݂̒�
				char	nowScale;				//���݂̃X�P�[���i���[�h�j
				
	//----------------------------------
	//�ڒ�
				int		iKeyShift;				//k (�R���p�C�����x��)

	//----------------------------------
	//�^���G�R�[
				bool	echo_already;			//���ɐ����������H
				bool	echo_vol_ret;			//�G�R�[�̉��ʂ����ɖ߂������H

				bool	echo_flag;				//�^���G�R�[ �t���O
				bool	echo_slur;				//�^���G�R�[ �X���[�łȂ���H
				int		echo_length;			//�^���G�R�[ ����
	unsigned	char	echo_volume;			//�^���G�R�[ ����
	unsigned	char	echo_value;				//�^���G�R�[ ���O�H
				char	oldNote[256];			//�^���G�R�[�p�����O�o�b�t�@
	unsigned	char	pt_oldNote;				//�^���G�R�[�p�����O�o�b�t�@�@�|�C���^

	//----------------------------------
	//�p�b�`
				bool	f_Patch;				//�p�b�`�������H
	unsigned	int		i_Patch;

	//----------------------------------
	//�������[�v�E���s�[�g�֌W

				bool	is_loop;				//	L	�R�}���h�o���������H
				bool	is_repeat_a_s;			//	[	�R�}���h���o���������H
				bool	is_repeat_a_b;			//	:	�R�}���h���o���������H
				bool	is_repeat_b_s;			//	|:	�R�}���h���o���������H
				bool	is_repeat_b_b;			//	\	�R�}���h���o���������H
				int		count_repeat_a;
	mml_repeat*			_old_repeat;

			vector<	int			>			repeat_type;		//�ǂ̃��s�[�g���g���Ă��邩�H
			vector<	int			>::iterator	it_repeat_type;

	//To control of the Repert(C)
			unsigned	int								sp_repeat_c;		//�X�^�b�N�|�C���^�i�l�X�g�j
	list<	unsigned	int				>				st_ct_repeat_c;		//���s�[�g��
	list<	list<	MusicItem*>::iterator>				st_it_repeat_c_s;	//���s�[�g�J�n�_
	list<	list<	MusicItem*>::iterator>				st_it_repeat_c_b;	//���s�[�g����_
	list<	list<	MusicItem*>::iterator>				st_it_repeat_c_e;	//���s�[�g�I���_
	list<	unsigned	int				>::iterator		it_ct_repeat_c;
	list<	list<	MusicItem*>::iterator>::iterator	it_it_repeat_c_s;
	list<	list<	MusicItem*>::iterator>::iterator	it_it_repeat_c_b;
	list<	list<	MusicItem*>::iterator>::iterator	it_it_repeat_c_e;

	//----------------------------------
	//�I�u�W�F�N�g

	//�A�h���X�������C�x���g�I�u�W�F�N�g�̃|�C���^�̈ꗗ
	vector<	mml_Address*	>	vec_ptc_FDSC;		//FDS Carrer
	vector<	mml_Address*	>	vec_ptc_FDSM;		//FDS Modlator
	vector<	mml_Address*	>	vec_ptc_OPLL;		//VRC7, OPLL
	vector<	mml_Address*	>	vec_ptc_Wave;		//N163
	vector<	mml_Address*	>	vec_ptc_SE;			//���ʉ��R�}���h�ꗗ
	vector<	mml_Address*	>	vec_ptc_Sub;		//�T�u���[�`���R�}���h�ꗗ
	vector<	mml_Address*	>	vec_ptc_Env;		//�G���x���[�v�R�}���h�ꗗ

	vector<	mml_Address*	>	vec_ptc_Loop_End;			//End of Track with LOOP
	vector<	mml_Address*	>	vec_ptc_Repert_A_End;		//Repert(A) End    poiont
	vector<	mml_Address*	>	vec_ptc_Repert_A_Branch;	//Repert(A) Branch poiont
	vector<	mml_Address*	>	vec_ptc_Repert_B_End;		//Repert(B) End    poiont

	//�eID�ԍ����̎Q�Ɛ悷��C�x���g�I�u�W�F�N�g�̃|�C���^�̈ꗗ
	map<	size_t, MusicEvent*	>	ptc_Loop;		//Loop
	map<	size_t, mml_repeat*	>	ptc_Repert_A;	//Repert(A) Start poiont
	map<	size_t, mml_Address*>	ptc_Repert_A_E;	//Repert(A) End   point
	map<	size_t, mml_repeat*	>	ptc_Repert_B;	//Repert(B) Start poiont

	//ID�ԍ����ǂ��܂łӂ�����
			size_t	cnt_Loop;						// L �R�}���h
			size_t	cnt_Repert_A;					// [ �R�}���h
			size_t	cnt_Repert_B;					// |:�R�}���h


//�����o�[�֐�
public:
			MusicTrack(size_t _id, MMLfile* MML, const _CHAR _strName[] = _T("---- [ Music Track ] ----"));
			~MusicTrack(void);

				int		TickCount(MusicFile* MUS, NSD_WORK* work);
				int		TickCount(MusicFile* MUS);
				void	TickCount_Envelope(MusicFile* MUS, mml_Address* adrObj, size_t _no);
				int		GetTickTotal(void){	return(iTickTotal);};
				int		GetTickLoop(void){	return(iTickLoop);};

				void	Fix_Address(MusicFile* MUS);
				void	SetEvent(MusicItem* _item);			//�C�x���g�̒ǉ�
				void	SetEvent_front(MusicItem* _item);	//�C�x���g�̒ǉ�

				//���̃g���b�N���R���p�C�����邩�ǂ���
				bool	GetCompileFlag(void){return(compile_flag);};
				void	SetCompileFlag(bool _flag){compile_flag = _flag;};

				//----------------------------------
				//���̃g���b�N�ɂ��������l�l�k�R�}���h
				void	SetEnd(MMLfile* MML);			//�L�q�u���b�N�I��
				void	SetLoop(MMLfile* MML);			//�������[�v

				void	SetRepeat_B_Start();
				void	SetRepeat_B_Branch(MMLfile* MML);
				void	SetRepeat_B_End(MMLfile* MML);

				void	SetEvent_Repeat_B_Start();
				void	SetEvent_Repeat_B_Branch();
				void	SetEvent_Repeat_B_End();

				void	SetRepeat_Start(MMLfile* MML);
				void	SetRepeat_End(MMLfile* MML);
				void	SetRepeat_Branch(MMLfile* MML);

				void	SetRepeat_A_Start(MMLfile* MML);
				void	SetRepeat_A_End(MMLfile* MML);

				void	SetEvent_Repeat_A_Start(unsigned char _cnt);
				void	SetEvent_Repeat_A_Branch();
				void	SetEvent_Repeat_A_End();

				void	SetRepeat_C_Start(MMLfile* MML);
				void	SetRepeat_C_End(MMLfile* MML);

				void	SetSE(MMLfile* MML);
				void	SetSubroutine(size_t _no);
				void	SetSubWithParch(size_t _no,bool _f);

				void	SetPatch(MMLfile* MML);	
				void	SetPatch();				//@P off
				void	CallPatch(MMLfile* MML, char _note);

				void	SetEnvelop_Evoi(size_t _no);
				void	SetEnvelop_Evol(size_t _no);
				void	SetEnvelop_Em(size_t _no);
				void	SetEnvelop_En(size_t _no);
				void	SetVoice(int _no);				//E@ off
				void	SetEnvelop_Evol();				//Ev off
				void	SetEnvelop_Em();				//Em off
				void	SetEnvelop_En();				//En off

				void	SetSweep(MMLfile* MML);
				void	SetSweep(unsigned char _c);

				void	SetFDSC(MMLfile* MML);			//@FC
				void	SetFDSM(MMLfile* MML);			//@FM
				void	SetVRC7(MMLfile* MML);			//@V
				void	SetN163(MMLfile* MML);			//@N
				void	SetN163_Load(MMLfile* MML);		//@NL
				void	SetN163_Set(MMLfile* MML);		//@NS

				void	SetJump(MMLfile* MML);			//�W�����v

				void	Set_q(int i);
				void	Set_u(int i);
				void	SetGatetime_Q(MMLfile* MML);
				void	SetGatetime(MMLfile* MML);
				void	SetGatetime_u(MMLfile* MML);

				void	SetReleaseMode(MMLfile* MML);
				void	SetReleaseVoice(MMLfile* MML);
				void	SetReleaseVolume(MMLfile* MML);

				void	SetKeyFlag(char _c, char _d, char _e, char _f, char _g, char _a, char _b);
				void	SetKey(int _key, int _scale);

				void	SetMajor();
				void	SetMinor();
				void	SetHMinor(MMLfile* MML);
				void	SetMMinor(MMLfile* MML);
				void	SetScale(MMLfile* MML);
				void	SetKeySignature(MMLfile* MML);	//�����̐ݒ�

				void	SetEcho(void);
				void	SetEcho(MMLfile* MML);
				void	SetEchoBuffer(MMLfile* MML,int note);
				void	ResetEcho();
				void	EchoVolRet();
				void	GenerateEcho(MMLfile* MML, int Length, int GateTime, bool	Slur);
				char	calc_note(MMLfile*	MML,int note);
				int		calc_length(MMLfile* MML);
				int		calc_gate(MMLfile* MML);
				bool	calc_slur(MMLfile* MML);
				void	SetNote(MMLfile* MML, int _key, int Length, int GateTime, bool Slur);
				void	SetNote(MMLfile* MML, int note);
				void	SetRest(MMLfile* MML, int mode);
				void	SetTai(MMLfile* MML);
				void	SetLength(MMLfile* MML);

				void	SetProtament(MMLfile* MML);
				void	SetProtament(MMLfile* MML, unsigned char iTempo);

				void	SetKeyShift(MMLfile* MML);
				void	SetKeyShift_Relative(MMLfile* MML);

				void	SetTranspose(int _no);
				void	SetTranspose_Relative(int _no);

				void	SetOctave(MMLfile* MML);
				void	SetOctaveInc();
				void	SetOctaveDec();
				void	SetOctaveOne_Inc();
				void	SetOctaveOne_Dec();
		
				void	SetVolume(MMLfile* MML);
				void	SetVolumeInc(MMLfile* MML);
				void	SetVolumeDec(MMLfile* MML);

				void	Reset_opt(void){
					//���s�[�g�I�����ɁA�f�t�H���g�����������Z�b�g����
					//���򎞂� l �R�}���h���������ꍇ�A���̒l����F�X�ƌv�Z���Ă��܂����߁B
					opt_DefaultLength	= -1;
				}
};
