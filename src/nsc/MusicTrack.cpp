/*******************************************************************************

			NES Sound Driver & Library	(NSD.lib)	MML Compiler

	Copyright (c) 2012 A.Watanabe (S.W.), All rights reserved.
	 For conditions of distribution and use, see copyright notice in "nsc.cpp".

*******************************************************************************/

#include "StdAfx.h"
#include "MusicTrack.h"

/****************************************************************/
/*					�O���[�o���ϐ��i�N���X�����ǁE�E�E�j		*/
/****************************************************************/
extern	OPSW*			cOptionSW;	//�I�v�V�������ւ̃|�C���^�ϐ�

//==============================================================
//		�R���X�g���N�^
//--------------------------------------------------------------
//	������
//		const		_CHAR	_strName[]	�I�u�W�F�N�g��
//	���Ԓl
//					����
//==============================================================
MusicTrack::MusicTrack(size_t _id, MMLfile* MML, const _CHAR _strName[]):
	MusicItem(_id, _strName),
	offset_now(0),
	compile_flag(false),
	jump_flag(false),
	iKeyShift(0),
	echo_already(false),
	echo_vol_ret(false),
	echo_flag(false),
	echo_length(-1),
	pt_oldNote(0),
	f_Patch(false),
	is_loop(false),				//���[�v
	is_repeat_a_s(false),		//���s�[�g�`
	is_repeat_a_b(false),		//���s�[�g�`
	is_repeat_b_s(false),		//���s�[�g�a
	is_repeat_b_b(false),		//���s�[�g�a
	sp_repeat_c(0),
	cnt_Loop(0),
	cnt_Repert_A(0),
	cnt_Repert_B(0)

{
	nsd.init();
	nsd.length = -1;

	ResetEcho();

	Reset_opt();

	//�N�I���^�C�Y
	QMax = MML->QMax;
	gatetime_Q = QMax;

	//�����iDefault = C-Dur�j
	nowKey		=0;
	nowScale	=0;
	SetKey(nowKey, nowScale);

	//���̃I�u�W�F�N�g�͕K���g���i�œK���ΏۊO�j�B
	setUse();

	//�ی�O�̃������A�N�Z�X�ɂ���O�����΍�iMML�R���p�C���W�J�̃��s�[�g�ׁ̈j
	//Visual C++.net 2008 �ł́A�������Ă����Ȃ��ƁA��O����������͗l�B
	//�܂�A�ʂ�ۑ΍�
	{
		list<MusicItem*>::iterator	pt_itMusic	=	ptcItem.end();

		//���s�[�g�^�C�v
		repeat_type.push_back(0);

		//���s�[�g(C)
		st_ct_repeat_c.push_back(0);
		st_it_repeat_c_s.push_back(pt_itMusic);
		st_it_repeat_c_b.push_back(pt_itMusic);
		st_it_repeat_c_e.push_back(pt_itMusic);

		//�C�x���g
		SetEvent(new MusicItem(_T("Null")));
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
MusicTrack::~MusicTrack(void)
{
}

//==============================================================
//		�J�E���g
//--------------------------------------------------------------
//	������
//			MusicFile*		MUS		
//			NSD_WORK*		work	�T�u���[�`�����̃p�����[�^
//	���Ԓl
//			unsigned	int			Tick��
//==============================================================
int	MusicTrack::TickCount(MusicFile* MUS, NSD_WORK* work)
{
	int	itick;

	nsd = *work;				//�T�u���[�`��������A�p�����[�^���擾
	itick = TickCount(MUS);
	*work = nsd;				//�T�u���[�`�����ցA�p�����[�^��ʒm

	return(itick);
}

//==============================================================
//		�J�E���g
//--------------------------------------------------------------
//	������
//			MusicFile*	MUS			
//	���Ԓl
//						int			Tick��
//==============================================================
int	MusicTrack::TickCount(MusicFile* MUS)
{

	//----------------------
	//Local �萔
	static	const	int				TBL_length[] = {96, 72, 48, 36, 32, 24, 18, 16, 12, 9, 8, 6, 4, 3, 2, 1};

	//----------------------
	//Local �ϐ�
	list<	MusicItem*>::iterator	itItem;
	unsigned	char				iCode;

	//���s�[�g�p
	list<	MusicItem*>::iterator	itLoop_start;
				int					iLoop_count	= 0;

	list<	MusicItem*>::iterator	itRepeatA_start;
	list<	MusicItem*>::iterator	itRepeatA_end;
				int					iRepeatA_count	= 0;

	list<	MusicItem*>::iterator	itRepeatB_start;
	list<	MusicItem*>::iterator	itRepeatB_end;
				int					iRepeatB_count	= 0;

	//Subroutine for Patch �œK���p
				size_t				no_sub = 0;
				bool				sw_sub = false;

	//�ėp
				mml_Address*		adrObj;
				size_t				_no;
				int					i;
				bool				_f;

				int					iTickCount	= 0;

	try {

		//Tick ������
		iTickTotal	=	0;		//
		iTickLoop	=	0;

		//----------------------
		//�v�����t

		//�V�[�P���X�f�[�^���v�����t
		itItem = ptcItem.begin();
		while(itItem != ptcItem.end()){

			//L �R�}���h�̃I�u�W�F�N�g���ǂ�������
			if(is_loop == true){
				if( (*itItem) == ptc_Loop[0]){
					itLoop_start = itItem;
					if(iLoop_count == 0){
						iTickLoop = iTickCount;
					}
					(*itItem)->setUse();
					itItem++;
					continue;
				}
			}

			//�R���p�C���p�̋[��Command �̏ꍇ
			if((*itItem)->getSize() == 0){
				(*itItem)->setUse();
				itItem++;
				continue;
			}

			//�e�R�}���h����
			iCode		= (*itItem)->getCode((size_t)0);

			//0x00 - 0x2F
			if(iCode < 0x30){

				//command
				adrObj = (mml_Address*)(*itItem);
				switch(iCode){
					//----------------------------------------------
					case(nsd_EndOfTrack):			//0x00
						iTickTotal = iTickCount;
						adrObj->setUse();
						break;

					case(nsd_Jump):					//0x01
						_no = adrObj->get_id();
						if(adrObj->chkUse() == false){
							if( ptc_Loop.count(_no) == 0){
								MUS->Err(_T("L"),_no);
							} else {
								adrObj->setUse();
								vec_ptc_Loop_End.push_back(adrObj);		//�A�h���X��������I�u�W�F�N�g�Ƃ��ēo�^
							}
						}
						//���񃋁[�v�̏ꍇ�A�߂�B
						if((iLoop_count == 0) && (is_loop == true)){
							iTickTotal = iTickCount;
							itItem = itLoop_start;
							iLoop_count++;
						}
						break;

					//----------------------------------------------
					case(nsd_Call):					//0x02
						{
							bool	f_isPatch	= ((mml_CallSub*)adrObj)->isPatch();
							_no = adrObj->get_id();
							if((no_sub != _no) || (sw_sub == false) || (cOptionSW->flag_OptSeq == false) || (((mml_CallSub*)adrObj)->isDisableOptimize() == true) || (f_isPatch == false)){
								if(f_isPatch == true){
									no_sub = _no;
									sw_sub = true;
								}
								if( MUS->ptcSub.count(_no) == 0){
									MUS->Err(_T("Sub"),_no);
								} else {
									iTickCount += MUS->ptcSub[_no]->TickCount(MUS, &nsd);	//�T�u���[�`������V�~�����[�g
									MUS->ptcSub[_no]->setUse();								//�T�u���[�`������g���t���O�𗧂Ă�
									if(adrObj->chkUse() == false){
										adrObj->setUse();
										vec_ptc_Sub.push_back(adrObj);		//�A�h���X��������I�u�W�F�N�g�Ƃ��ēo�^
									}
								}
							}
							
						}
						break;

					//----------------------------------------------
					case(nsd_Call_SE):				//0x06
						_no = adrObj->get_id();
						if(adrObj->chkUse() == false){
							if( MUS->ptcSE.count(_no) == 0){
								MUS->Err(_T("SE"),_no);
							} else {
								adrObj->setUse();
								vec_ptc_SE.push_back(adrObj);		//�A�h���X��������I�u�W�F�N�g�Ƃ��ēo�^
							}
						}
						break;

					//----------------------------------------------
					//Repeat A
					case(nsd_Repeat_A_Start):		//0x03
						{
						mml_repeat*	_event = (mml_repeat*)(*itItem);
						iRepeatA_count	= _event->get_count();
						itRepeatA_start	= itItem;	//�o����̂͌����_�ŗǂ�
						(*itItem)->setUse();
						}
						break;

					//----------------------------------------------
					case(nsd_Repeat_A_Branch):		//0x04
						_no = adrObj->get_id();
						if(adrObj->chkUse() == false){
							if( ptc_Repert_A_E.count(_no) == 0){
								MUS->Err(_T("]"),_no);
							} else {
								adrObj->setUse();
								vec_ptc_Repert_A_Branch.push_back(adrObj);	//�A�h���X��������I�u�W�F�N�g�Ƃ��ēo�^
							}
						}
						if(iRepeatA_count == 1){
							itItem = itRepeatA_end;	//�I���_�ɍs��
						}
						break;

					//----------------------------------------------
					case(nsd_Repeat_A_End):			//0x05
						_no = adrObj->get_id();
						if(adrObj->chkUse() == false){
							if( ptc_Repert_A.count(_no) == 0){
								MUS->Err(_T("["),_no);
							} else {
								adrObj->setUse();
								vec_ptc_Repert_A_End.push_back(adrObj);	//�A�h���X��������I�u�W�F�N�g�Ƃ��ēo�^
							}
						}
						itRepeatA_end = itItem;		//�I���_���L��
						iRepeatA_count--;
						if(iRepeatA_count != 0){
							itItem = itRepeatA_start;
						}
						break;

					//----------------------------------------------
					//Repeat B
					case(nsd_Repeat_B_Start):		//0x18
						itRepeatB_start = itItem;	//�o����̂͌����_�ŗǂ�
						iRepeatB_count	= 0;
						(*itItem)->setUse();
						break;

					//----------------------------------------------
					case(nsd_Repeat_B_Branch):		//0x19
						if(iRepeatB_count != 0){
							itItem = itRepeatB_end;	//������
						} else {
							(*itItem)->setUse();
						}
						break;

					//----------------------------------------------
					case(nsd_Repeat_B_End):			//0x1A
						_no = adrObj->get_id();
						if(adrObj->chkUse() == false){
							if( ptc_Repert_B.count(_no) == 0){
								MUS->Err(_T("|:"),_no);
							} else {
								adrObj->setUse();
								vec_ptc_Repert_B_End.push_back(adrObj);	//�A�h���X��������I�u�W�F�N�g�Ƃ��ēo�^
							}
						}
						itRepeatB_end = itItem;		//�������L��
						itItem = itRepeatB_start;
						iRepeatB_count++;
						break;

					//----------------------------------------------
					//Length
					case(nsd_Length):				//0x09
						i = (*itItem)->getCode(1);
						if(nsd.length != i){
							nsd.length = i;
							(*itItem)->setUse();
						}
						break;

					//----------------------------------------------
					//Gate
					case(nsd_GateTime_q):			//0x0A
						i = (*itItem)->getCode(1);
						if(nsd.gate_q != i){
							nsd.gate_q = i;
							(*itItem)->setUse();
						}
						break;

					case(nsd_GateTime_u):			//0x0B
						i = (*itItem)->getCode(1);
						if(nsd.gate_u != i){
							nsd.gate_u = i;
							(*itItem)->setUse();
						}
						break;

					case(nsd_GateMode_0):			//0x0D
						if(nsd.gatemode != 0){
							nsd.gatemode = 0;
							(*itItem)->setUse();
						}
						break;

					case(nsd_GateMode_1):			//0x0E
						if(nsd.gatemode != 1){
							nsd.gatemode = 1;
							(*itItem)->setUse();
						}
						break;

					case(nsd_GateMode_2):			//0x0F
						if(nsd.gatemode != 2){
							nsd.gatemode = 2;
							(*itItem)->setUse();
						}
						break;

					//----------------------------------------------
					//Define
					case(nsd_Voice):				//0x1B
						i = (*itItem)->getCode(1);
						if((nsd.voice != i) || (nsd.sw_Evoi == true)){
							(*itItem)->setUse();
						}
						nsd.sw_Evoi = false;
						nsd.voice = i;
						break;

					//----------------------------------------------
					case(nsd_Envelop_Voice):		//0x10
						_f = adrObj->get_flag();
						if(_f == true){
							_no = adrObj->get_id();
							//�ԍ������݂ƈ������A�œK�����Ȃ��B
							if((nsd.env_voice != _no) || (nsd.sw_Evoi == false) || (cOptionSW->flag_OptSeq == false)){
								nsd.env_voice = _no;
								TickCount_Envelope(MUS, adrObj, _no);
							}
						} else {
							//�t���O�����݂ƈ������A�œK�����Ȃ��B
							if(nsd.sw_Evoi == true){
								adrObj->setUse();
							}
						}
						nsd.sw_Evoi = _f;
						break;

					//----------------------------------------------
					case(nsd_Envelop_Volume):		//0x11
						_f = adrObj->get_flag();
						if(_f == true){
							_no = adrObj->get_id();
							//�ԍ������݂ƈ������A�œK�����Ȃ��B
							if((nsd.env_volume != _no) || (nsd.sw_Evol == false) || (cOptionSW->flag_OptSeq == false)){
								nsd.env_volume = _no;
								TickCount_Envelope(MUS, adrObj, _no);
							}
						} else {
							//�t���O�����݂ƈ������A�œK�����Ȃ��B
							if(nsd.sw_Evol == true){
								adrObj->setUse();
							}
						}
						nsd.sw_Evol = _f;
						break;

					//----------------------------------------------
					case(nsd_Envelop_Frequency):	//0x12
						_f = adrObj->get_flag();
						if(_f == true){
							_no = adrObj->get_id();
							//�ԍ������݂ƈ������A�œK�����Ȃ��B
							if((nsd.env_frequency != _no) || (nsd.sw_Em == false) || (cOptionSW->flag_OptSeq == false)){
								nsd.env_frequency = _no;
								TickCount_Envelope(MUS, adrObj, _no);
							}
						} else {
							//�t���O�����݂ƈ������A�œK�����Ȃ��B
							if(nsd.sw_Em == true){
								adrObj->setUse();
							}
						}
						nsd.sw_Em = _f;
						break;

					//----------------------------------------------
					case(nsd_Envelop_Note):			//0x13
						_f = adrObj->get_flag();
						if(_f == true){
							_no = adrObj->get_id();
							//�ԍ������݂ƈ������A�œK�����Ȃ��B
							if((nsd.env_note != _no) || (nsd.sw_En == false) || (cOptionSW->flag_OptSeq == false)){
								nsd.env_note = _no;
								TickCount_Envelope(MUS, adrObj, _no);
							}
						} else {
							//�t���O�����݂ƈ������A�œK�����Ȃ��B
							if(nsd.sw_En == true){
								adrObj->setUse();
							}
						}
						nsd.sw_En = _f;
						break;

					//----------------------------------------------
					case(nsd_Sweep):				//0x16
						i = (char)((*itItem)->getCode(1));
						if(nsd.sweep != i){
							nsd.sweep = i;
							(*itItem)->setUse();
						}
						break;

					//----------------------------------------------
					case(nsc_VRC7):					//0x1C
						_no = adrObj->get_id();
						if((nsd.vrc7_voice != _no) || (nsd.sw_vrc7_voice == false) || (cOptionSW->flag_OptSeq == false)){
							nsd.vrc7_voice = _no;
							nsd.sw_vrc7_voice = true;
							if(adrObj->chkUse() == false){
								if( MUS->ptcVRC7.count(_no) == 0){
									MUS->Err(_T("VRC7"),_no);
								} else {
									MUS->ptcVRC7[_no]->setUse();		//�g���t���O�𗧂Ă�
									adrObj->setUse();
									vec_ptc_OPLL.push_back(adrObj);		//�A�h���X��������I�u�W�F�N�g�Ƃ��ēo�^
								}
							}
						}
						break;

					//----------------------------------------------
					case(nsc_N163):					//0x1E
						_no = adrObj->get_id();
						if((nsd.n163_voice != _no) || (nsd.sw_n163_voice == false) || (cOptionSW->flag_OptSeq == false)){
							nsd.n163_voice = _no;
							nsd.sw_n163_voice = true;
							if(adrObj->chkUse() == false){
								if( MUS->ptcN163.count(_no) == 0){
									MUS->Err(_T("N163"),_no);
								} else {
									MUS->ptcN163[_no]->setUse();		//�g���t���O�𗧂Ă�
									adrObj->setUse();
									vec_ptc_Wave.push_back(adrObj);		//�A�h���X��������I�u�W�F�N�g�Ƃ��ēo�^
								}
							}
						}
						break;

					//----------------------------------------------
					case(nsd_Volume_Down):			//0x20
						nsd.volume--;
						if(nsd.volume < 0){
							nsd.volume = 0;
						}
						(*itItem)->setUse();
						break;

					case(nsd_Volume_Up):			//0x21
						nsd.volume++;
						if(nsd.volume > 15){
							nsd.volume = 15;
						}
						(*itItem)->setUse();
						break;

					//----------------------------------------------
					case(nsd_FDS_Career):			//0x22
						_no = adrObj->get_id();
						if((nsd.fds_career != _no) || (nsd.sw_fds_career == false) || (cOptionSW->flag_OptSeq == false)){
							nsd.fds_career = _no;
							nsd.sw_fds_career = true;
							if(adrObj->chkUse() == false){
								if( MUS->ptcFDSC.count(_no) == 0){
									MUS->Err(_T("FDSC"),_no);
								} else {
									MUS->ptcFDSC[_no]->setUse();		//�g���t���O�𗧂Ă�
									adrObj->setUse();
									vec_ptc_FDSC.push_back(adrObj);		//�A�h���X��������I�u�W�F�N�g�Ƃ��ēo�^
								}
							}
						}
						break;

					//----------------------------------------------
					case(nsd_FDS_Modlator):			//0x23
						_no = adrObj->get_id();
						if((nsd.fds_modlator != _no) || (nsd.sw_fds_modlator == false) || (cOptionSW->flag_OptSeq == false)){
							nsd.fds_modlator = _no;
							nsd.sw_fds_modlator = true;
							if(adrObj->chkUse() == false){
								if( MUS->ptcFDSM.count(_no) == 0){
									MUS->Err(_T("FDSM"),_no);
								} else {
									MUS->ptcFDSM[_no]->setUse();		//�g���t���O�𗧂Ă�
									adrObj->setUse();
									vec_ptc_FDSM.push_back(adrObj);		//�A�h���X��������I�u�W�F�N�g�Ƃ��ēo�^
								}
							}
						}
						break;

					//----------------------------------------------
					case(nsd_FDS_Frequency):		//0x24
						i = ((*itItem)->getCode(1)) + (((*itItem)->getCode(2)) << 8);
						if(nsd.fds_frequency != i){
							nsd.fds_frequency = i;
							(*itItem)->setUse();
						}
						break;

					//----------------------------------------------
					case(nsd_FDS_Volume):			//0x25
						i = (*itItem)->getCode(1);
						if(nsd.fds_volume != i){
							nsd.fds_volume = i;
							(*itItem)->setUse();
						}
						break;

					//----------------------------------------------
					case(nsc_FME7_frequency):		//0x26
						i = ((*itItem)->getCode(1)) + (((*itItem)->getCode(2)) << 8);
						if(nsd.psg_frequency != i){
							nsd.psg_frequency = i;
							(*itItem)->setUse();
						}
						break;

					//----------------------------------------------
					case(nsd_Octave_Down):			//0x28
						nsd.octave--;
						if(nsd.octave < 0){
							nsd.octave = 0;
						}
						(*itItem)->setUse();
						break;

					case(nsd_Octave_Up):			//0x29
						nsd.octave++;
						if(nsd.octave*12 >= 128){
							nsd.octave--;
						}
						(*itItem)->setUse();
						break;

					//----------------------------------------------
					case(nsd_Transpose):			//0x2A
						i = (char)((*itItem)->getCode(1));
						if(nsd.trans != i){
							nsd.trans = i;
							(*itItem)->setUse();
						}
						break;

					case(nsd_Relative_Transpose):	//0x2B
						nsd.trans += (char)((*itItem)->getCode(1));
						(*itItem)->setUse();
						break;

					//----------------------------------------------
					//Detune
					case(nsd_Detune_Cent):			//0x14
						i = (char)((*itItem)->getCode(1));
						if(nsd.detune_cent != i){
							nsd.detune_cent = i;
							(*itItem)->setUse();
						}
						break;

					case(nsd_Derune_Register):		//0x15
						i = (char)((*itItem)->getCode(1));
						if(nsd.detune_reg != i){
							nsd.detune_reg = i;
							(*itItem)->setUse();
						}
						break;

					//----------------------------------------------
					//Sub Command
					case(nsd_SubCommand):			//0x2F
						switch((*itItem)->getCode(1))
						{
							case(nsd_sub_Detune_Cent):		//0x2F-02
								nsd.detune_cent += (char)((*itItem)->getCode(2));
								(*itItem)->setUse();
								break;
							case(nsd_sub_Derune_Register):	//0x2F-03
								nsd.detune_reg += (char)((*itItem)->getCode(2));
								(*itItem)->setUse();
								break;
							default:
								(*itItem)->setUse();
								break;
						}
						break;

					//----------------------------------------------
					//Default
					default:
						(*itItem)->setUse();
						break;
				}

			} else if(iCode < 0x38){				//0x30 - 0x37
				i = (int)iCode & 0x07;
				if(nsd.voice_rel != i){
					nsd.voice_rel = i;
					(*itItem)->setUse();
				}

			} else if(iCode < 0x40){				//0x38 - 0x3F
				i = (int)iCode & 0x07;
				if(nsd.octave != i){
					nsd.octave = i;
					(*itItem)->setUse();
				}

			} else if(iCode < 0x50){				//0x40 - 0x4F
				i = TBL_length[iCode & 0x0F];
				if(nsd.length != i){
					nsd.length = i;
					(*itItem)->setUse();
				}

			} else if(iCode < 0x60){				//0x50 - 0x5F
				i = (int)iCode & 0x0F;
				if(nsd.gate_q != i){
					nsd.gate_q = i;
					(*itItem)->setUse();
				}

			} else if(iCode < 0x70){				//0x60 - 0x6F
				i = (int)iCode & 0x0F;
				if(nsd.volume != i){
					nsd.volume = i;
					(*itItem)->setUse();
				}

			} else if(iCode < 0x80){				//0x70 - 0x7F
				i = (int)iCode & 0x0F;
				if(nsd.volume_rel != i){
					nsd.volume_rel = i;
					(*itItem)->setUse();
				}

			} else {								//0x80 - 0xFF
				//note
				iCode &= 0x7F;
				if(iCode & nsd_Note_Length){
					iTickCount += (int)(*itItem)->getCode(1);
				} else {
					iTickCount += nsd.length;
				}
				(*itItem)->setUse();
			}
			itItem++;
		}

	//�V�~�����[�g�ł��Ȃ�������
	//	if(MUS->isError() == true){
	//	}

		if(is_loop == false){
			iTickLoop = -1;
		}

	}
	catch (const exception& e) {
		nsc_ErrMsg(e);
	}

	return(iTickTotal);
}

//--------------------------------------------------------------
void	MusicTrack::TickCount_Envelope(MusicFile* MUS, mml_Address* adrObj, size_t _no)
{
	if(adrObj->chkUse() == false){
		//�w��ԍ��̒�`�����邩�`�F�b�N
		if( MUS->ptcEnv.count(_no) == 0){
			MUS->Err(_T("Envelope"),_no);
		} else {
			MUS->ptcEnv[_no]->setUse();		//�g���t���O�𗧂Ă�
			adrObj->setUse();
			vec_ptc_Env.push_back(adrObj);	//�A�h���X��������I�u�W�F�N�g�Ƃ��ēo�^
		}
	}

}

//==============================================================
//		�A�h���X�������肷��B
//--------------------------------------------------------------
//	������
//		MusicFile*	MUS		�ȃf�[�^�t�@�C���E�I�u�W�F�N�g
//	���Ԓl
//				����
//==============================================================
void	MusicTrack::Fix_Address(MusicFile* MUS)
{
	//----------------------
	//Local�ϐ�	�i���[�v�񐔂͕��񉻑O�Ɏ擾����j
	const	int	sz_vec_ptc_Loop_End			= (const int)vec_ptc_Loop_End.size();
	const	int	sz_vec_ptc_Repert_A_End		= (const int)vec_ptc_Repert_A_End.size();
	const	int	sz_vec_ptc_Repert_A_Branch	= (const int)vec_ptc_Repert_A_Branch.size();
	const	int	sz_vec_ptc_Repert_B_End		= (const int)vec_ptc_Repert_B_End.size();
	const	int	sz_vec_ptc_SE				= (const int)vec_ptc_SE.size();
	const	int	sz_vec_ptc_Sub				= (const int)vec_ptc_Sub.size();
	const	int	sz_vec_ptc_Env				= (const int)vec_ptc_Env.size();
	const	int	sz_vec_ptc_FDSC				= (const int)vec_ptc_FDSC.size();
	const	int	sz_vec_ptc_FDSM				= (const int)vec_ptc_FDSM.size();
	const	int	sz_vec_ptc_OPLL				= (const int)vec_ptc_OPLL.size();
	const	int	sz_vec_ptc_Wave				= (const int)vec_ptc_Wave.size();

	if(cOptionSW->iDebug & DEBUG_FixAddress){
		_COUT << _T("  Object : ") << strName;
		if(f_id == true){
			_COUT	<< _T("(") << m_id << _T(")");
		}
		_COUT << endl;
	}

	_OMP_PARALLEL
	{
		//----------------------
		//LOOP
		_OMP_FOR_NOWAIT
		for(int i=0; i<sz_vec_ptc_Loop_End; ++i){
			size_t	_no			= vec_ptc_Loop_End[i]->get_id();
			size_t	_com_offset	= vec_ptc_Loop_End[i]->getOffset();
			size_t	_sub_offset;
			if( ptc_Loop.count(_no) == 0){
				MUS->Err(_T("L"),_no);
			} else {
				_sub_offset = ptc_Loop[_no]->getOffset();
				_sub_offset += ptc_Loop[_no]->getSize();	//�s��́A���̃R�}���h
				vec_ptc_Loop_End[i]->set_Address(_sub_offset - _com_offset - 1);
			}
		}

		//----------------------
		//Repeat (A)
		_OMP_FOR_NOWAIT
		for(int i=0; i<sz_vec_ptc_Repert_A_End; ++i){
			size_t	_no			= vec_ptc_Repert_A_End[i]->get_id();
			size_t	_com_offset	= vec_ptc_Repert_A_End[i]->getOffset();
			size_t	_sub_offset;
			if( ptc_Repert_A.count(_no) == 0){
				MUS->Err(_T("["),_no);
			} else {
				_sub_offset = ptc_Repert_A[_no]->getOffset();
				_sub_offset += ptc_Repert_A[_no]->getSize();	//�s��́A���̃R�}���h
				vec_ptc_Repert_A_End[i]->set_Address(_sub_offset - _com_offset - 1);
			}
		}

		//----------------------
		//Repeat (A) Branch
		_OMP_FOR_NOWAIT
		for(int i=0; i<sz_vec_ptc_Repert_A_Branch; ++i){
			size_t	_no			= vec_ptc_Repert_A_Branch[i]->get_id();
			size_t	_com_offset	= vec_ptc_Repert_A_Branch[i]->getOffset();
			size_t	_sub_offset;
			if( ptc_Repert_A_E.count(_no) == 0){
				MUS->Err(_T("]"),_no);
			} else {
				_sub_offset = ptc_Repert_A_E[_no]->getOffset();
				_sub_offset += ptc_Repert_A_E[_no]->getSize();	//�s��́A���̃R�}���h
				vec_ptc_Repert_A_Branch[i]->set_Address(_sub_offset - _com_offset - 1);
			}
		}

		//----------------------
		//Repeat (B)
		_OMP_FOR_NOWAIT
		for(int i=0; i<sz_vec_ptc_Repert_B_End; ++i){
			size_t	_no			= vec_ptc_Repert_B_End[i]->get_id();
			size_t	_com_offset	= vec_ptc_Repert_B_End[i]->getOffset();
			size_t	_sub_offset;
			if( ptc_Repert_B.count(_no) == 0){
				MUS->Err(_T("|:"),_no);
			} else {
				_sub_offset = ptc_Repert_B[_no]->getOffset();
				_sub_offset += ptc_Repert_B[_no]->getSize();	//�s��́A�����̃R�}���h
				vec_ptc_Repert_B_End[i]->set_Address(_sub_offset - _com_offset - 1);
			}
		}

		//----------------------
		//SE
		_OMP_FOR_NOWAIT
		for(int i=0; i<sz_vec_ptc_SE; ++i){
			size_t	_no			= vec_ptc_SE[i]->get_id();		//No.�̎擾
			size_t	_com_offset	= vec_ptc_SE[i]->getOffset();
			size_t	_sub_offset;
			if( MUS->ptcSE.count(_no) == 0){
				MUS->Err(_T("SE"),_no);
			} else {
				_sub_offset = MUS->ptcSE[_no]->getOffset();		//�w��No.�����݂���I�t�Z�b�g
				vec_ptc_SE[i]->set_Address(_sub_offset - _com_offset - 1);
			}
		}

		//----------------------
		//Surbortine
		_OMP_FOR_NOWAIT
		for(int i=0; i<sz_vec_ptc_Sub; ++i){
			size_t	_no			= vec_ptc_Sub[i]->get_id();		//No.�̎擾
			size_t	_com_offset	= vec_ptc_Sub[i]->getOffset();
			size_t	_sub_offset;
			if( MUS->ptcSub.count(_no) == 0){
				MUS->Err(_T("Sub"),_no);
			} else {
				_sub_offset = MUS->ptcSub[_no]->getOffset();	//�w��No.�����݂���I�t�Z�b�g
				vec_ptc_Sub[i]->set_Address(_sub_offset - _com_offset - 1);
			}
		}

		//----------------------
		//Envelope
		_OMP_FOR_NOWAIT
		for(int i=0; i<sz_vec_ptc_Env; ++i){
			size_t	_no			= vec_ptc_Env[i]->get_id();		//No.�̎擾
			size_t	_com_offset	= vec_ptc_Env[i]->getOffset();
			size_t	_sub_offset;
			if( MUS->ptcEnv.count(_no) == 0){
				MUS->Err(_T("Envelope"),_no);
			} else {
				_sub_offset = MUS->ptcEnv[_no]->getOffset();	//�w��No.�����݂���I�t�Z�b�g
				vec_ptc_Env[i]->set_Address(_sub_offset - _com_offset - 1);
			}
		}

		//----------------------
		//FDSC
		_OMP_FOR_NOWAIT
		for(int i=0; i<sz_vec_ptc_FDSC; ++i){
			size_t	_no			= vec_ptc_FDSC[i]->get_id();		//No.�̎擾
			size_t	_com_offset	= vec_ptc_FDSC[i]->getOffset();
			size_t	_sub_offset;
			if( MUS->ptcFDSC.count(_no) == 0){
				MUS->Err(_T("FDSC"),_no);
			} else {
				_sub_offset = MUS->ptcFDSC[_no]->getOffset();		//�w��No.�����݂���I�t�Z�b�g
				vec_ptc_FDSC[i]->set_Address(_sub_offset - _com_offset - 1);
			}
		}

		//----------------------
		//FDSM
		_OMP_FOR_NOWAIT
		for(int i=0; i<sz_vec_ptc_FDSM; ++i){
			size_t	_no			= vec_ptc_FDSM[i]->get_id();		//No.�̎擾
			size_t	_com_offset	= vec_ptc_FDSM[i]->getOffset();
			size_t	_sub_offset;
			if( MUS->ptcFDSM.count(_no) == 0){
				MUS->Err(_T("FDSM"),_no);
			} else {
				_sub_offset = MUS->ptcFDSM[_no]->getOffset();		//�w��No.�����݂���I�t�Z�b�g
				vec_ptc_FDSM[i]->set_Address(_sub_offset - _com_offset - 1);
			}
		}

		//----------------------
		//OPLL
		_OMP_FOR_NOWAIT
		for(int i=0; i<sz_vec_ptc_OPLL; ++i){
			size_t	_no			= vec_ptc_OPLL[i]->get_id();		//No.�̎擾
			size_t	_com_offset	= vec_ptc_OPLL[i]->getOffset();
			size_t	_sub_offset;
			if( MUS->ptcVRC7.count(_no) == 0){
				MUS->Err(_T("VRC7"),_no);
			} else {
				_sub_offset = MUS->ptcVRC7[_no]->getOffset();		//�w��No.�����݂���I�t�Z�b�g
				vec_ptc_OPLL[i]->set_Address(_sub_offset - _com_offset - 1);
			}
		}

		//----------------------
		//N163
		_OMP_FOR		//�Ō�͑S�X���b�h�̏I����҂�(nowait��t���Ȃ�)
		for(int i=0; i<sz_vec_ptc_Wave; ++i){
			size_t	_no			= vec_ptc_Wave[i]->get_id();		//No.�̎擾
			size_t	_com_offset	= vec_ptc_Wave[i]->getOffset();
			size_t	_sub_offset;
			if( MUS->ptcN163.count(_no) == 0){
				MUS->Err(_T("N163"),_no);
			} else {
				_sub_offset = MUS->ptcN163[_no]->getOffset();		//�w��No.�����݂���I�t�Z�b�g
				vec_ptc_Wave[i]->set_Address(_sub_offset - _com_offset - 2);	//N163�́A����������̂ŁA-2�ɂȂ�B
			}
		}
	}
}

//==============================================================
//		�V�[�P���X�E�I�u�W�F�N�g�̒ǉ�
//--------------------------------------------------------------
//	������
//		MusicItem* _item	�V�[�P���X�E�I�u�W�F�N�g
//	���Ԓl
//		����
//==============================================================
void	MusicTrack::SetEvent(MusicItem* _item)
{
	//�T�C�Y�̕��A�I�t�Z�b�g�𑝂₷�B
	offset_now += (unsigned int)_item->getSize();

	//�����object�̃|�C���^��ۑ����Ă����B
	ptcItem.push_back(_item);
}

//==============================================================
//		�V�[�P���X�E�I�u�W�F�N�g�̒ǉ�
//--------------------------------------------------------------
//	������
//		MusicItem* _item	�V�[�P���X�E�I�u�W�F�N�g
//	���Ԓl
//		����
//==============================================================
void	MusicTrack::SetEvent_front(MusicItem* _item)
{
	//�T�C�Y�̕��A�I�t�Z�b�g�𑝂₷�B
	offset_now += (unsigned int)_item->getSize();

	//�����object�̃|�C���^��ۑ����Ă����B
	ptcItem.push_front(_item);
}

//==============================================================
//		�L�q�u���b�N�̏I���iEnd of Track�̒ǉ��j
//--------------------------------------------------------------
//	������
//		����
//	���Ԓl
//		����
//==============================================================
void	MusicTrack::SetEnd(MMLfile* MML)
{
	mml_Address*	_event;

	//���s�[�g�̃`�F�b�N
	if(is_repeat_a_s == true){
		MML->Err(_T("���s�[�g(A)���I����Ă��܂���B"));
	}
	if(sp_repeat_c != 0){
		MML->Err(_T("���s�[�g(C)���I����Ă��܂���B"));
	}

	if(offset_now != 0){
		//�I�u�W�F�N�g�̍쐬
		if(is_loop == false){
			SetEvent(new mml_general(nsd_EndOfTrack,_T("End of Track")));
		} else {
			EchoVolRet();	//�^���G�R�[�̕��A
			_event = new mml_Address(cnt_Loop - 1, nsd_Jump, _T("End of Track with LOOP"));
			SetEvent(_event);
		}
	}
	iSize = offset_now;
}

//==============================================================
//		L	�������[�v
//--------------------------------------------------------------
//	������
//				����
//	���Ԓl
//				����
//==============================================================
void	MusicTrack::SetLoop(MMLfile* MML)
{
	MusicEvent*	_loop;

	if(is_loop == false){
		is_loop	= true;				//L�R�}���h���������B

		//�I�u�W�F�N�g�̍쐬
		_loop = new MusicEvent(cnt_Loop, _T("Loop Point"));

		//�I�u�W�F�N�g��ۑ�
		SetEvent(_loop);
		ptc_Loop[cnt_Loop] = _loop;

		//ID�ԍ��p�J�E���^�̃C���N�������g
		cnt_Loop++;
	} else {
		MML->Err(_T("�g���b�N���� L �R�}���h���d�����Ă��܂��B"));
	}

}

//==============================================================
//		|:	���s�[�g(B)	�J�n
//--------------------------------------------------------------
//	������
//		MMLfile*	MML		MML�t�@�C���̃I�u�W�F�N�g
//	���Ԓl
//				����
//==============================================================
void	MusicTrack::SetRepeat_B_Start()
{
	SetEvent_Repeat_B_Start();

	is_repeat_b_s = true;			// :| �R�}���h���o��
	is_repeat_b_b = false;			// \  �R�}���h�̏o���t���O�̓��Z�b�g

}
//--------------------------------------------------------------
void	MusicTrack::SetEvent_Repeat_B_Start()
{
	//�I�u�W�F�N�g�̍쐬
	mml_repeat*	_repeat = new mml_repeat(cnt_Repert_B);

	//�I�u�W�F�N�g��ۑ�
	SetEvent(_repeat);
	ptc_Repert_B[cnt_Repert_B] = _repeat;

	//ID�ԍ��p�J�E���^�̃C���N�������g
	cnt_Repert_B++;
}

//==============================================================
//		\	���s�[�g(B)	����
//--------------------------------------------------------------
//	������
//		MMLfile*	MML		MML�t�@�C���̃I�u�W�F�N�g
//	���Ԓl
//				����
//==============================================================
void	MusicTrack::SetRepeat_B_Branch(MMLfile* MML)
{
	EchoVolRet();	//�^���G�R�[�̕��A

	if(is_repeat_b_s == true){
		if(is_repeat_b_b == false){
			is_repeat_b_b = true;		//�R�}���h�����������������B
			SetEvent_Repeat_B_Branch();
		} else {
			MML->Err(_T("���s�[�g(B)���� \\ �R�}���h���d�����Ă��܂��B"));
		}
	} else {
		MML->Err(_T("���s�[�g(B)�̊J�n |: �R�}���h������܂���B"));
	}
}
//--------------------------------------------------------------
void	MusicTrack::SetEvent_Repeat_B_Branch()
{
	SetEvent(new mml_general(nsd_Repeat_B_Branch, _T("Repeat(B) Branch")));
}
//==============================================================
//		:|	���s�[�g(B)	�I��
//--------------------------------------------------------------
//	������
//		MMLfile*	MML		MML�t�@�C���̃I�u�W�F�N�g
//	���Ԓl
//				����
//==============================================================
void	MusicTrack::SetRepeat_B_End(MMLfile* MML)
{
	EchoVolRet();	//�^���G�R�[�̕��A

	if(is_repeat_b_s == true){
		if(is_repeat_b_b == true){
		//	��To Do:	����������Repeat(A)�Ƃ��蓾�Ȃ��l�X�g�ƂȂ��Ă��Ȃ����H �i�ȉ��A�������j
		//	if(offset_repeat_b_s < offset_repeat_a_s){
		//		MML->Err(_T("���s�[�g(A)�̋�Ԃ̓r���ł��B"));
		//	}
			SetEvent_Repeat_B_End();
		} else {
			MML->Err(_T("���s�[�g(B)���� \\ �R�}���h������܂���ł����B�K������_ \\ �͎w�肵�Ă��������B"));
		}
	} else {
		MML->Err(_T("���s�[�g(B)�̊J�n |: �R�}���h������܂���B"));
	}
}
//--------------------------------------------------------------
void	MusicTrack::SetEvent_Repeat_B_End()
{
	//�I�u�W�F�N�g�̍쐬
	mml_Address* _event = new mml_Address(cnt_Repert_B - 1, nsd_Repeat_B_End, _T("Repeat(B) End"));

	//�I�u�W�F�N�g��ۑ�
	SetEvent(_event);

	Reset_opt();
}

//==============================================================
//		:	���s�[�g (A) (C)	�J�n
//--------------------------------------------------------------
//	������
//		MMLfile*	MML		MML�t�@�C���̃I�u�W�F�N�g
//	���Ԓl
//				����
//==============================================================
void	MusicTrack::SetRepeat_Start(MMLfile* MML)
{
	if(is_repeat_a_s == false){
		SetRepeat_A_Start(MML);
	} else {
		SetRepeat_C_Start(MML);
	}
}

//==============================================================
//		[	���s�[�g(A)	�J�n
//--------------------------------------------------------------
//	������
//		MMLfile*	MML		MML�t�@�C���̃I�u�W�F�N�g
//	���Ԓl
//				����
//==============================================================
void	MusicTrack::SetRepeat_A_Start(MMLfile* MML)
{
	unsigned	char	cData	= MML->GetChar();

	MML->Back();

	if((cData < '0') || (cData > '9')){
		count_repeat_a = -1;
	} else {
		count_repeat_a = MML->GetInt_With_Chk_Range(_T("���s�[�g��"),2,255);
	}

	if(is_repeat_a_s == false){
		is_repeat_a_s = true;	// [ �R�}���h�̏o��
		is_repeat_a_b = false;	// : �R�}���h�̏o���t���O�̓��Z�b�g

		SetEvent_Repeat_A_Start((unsigned char)count_repeat_a);

		//���s�[�g�^�C�v�̋L��
		repeat_type.push_back(1);
		it_repeat_type	=	repeat_type.end();
		it_repeat_type--;
	} else {
		MML->Err(_T("���s�[�g(A)�̃l�X�g�͂ł��܂���B"));
	}
}
//--------------------------------------------------------------
void	MusicTrack::SetEvent_Repeat_A_Start(unsigned char _cnt)
{
	//�I�u�W�F�N�g�̍쐬
	mml_repeat*	_event = new mml_repeat(cnt_Repert_A, _cnt);

	//�I�u�W�F�N�g��ۑ�
	SetEvent(_event);
	ptc_Repert_A[cnt_Repert_A] = _event;

	//ID�ԍ��p�J�E���^�̃C���N�������g
	cnt_Repert_A++;
}
//==============================================================
//		[:	���s�[�g(C)	�J�n
//--------------------------------------------------------------
//	������
//		MMLfile*	MML		MML�t�@�C���̃I�u�W�F�N�g
//	���Ԓl
//				����
//==============================================================
void	MusicTrack::SetRepeat_C_Start(MMLfile* MML)
{

	list<MusicItem*>::iterator	pt_itMusic	=	ptcItem.end();	//�����_�̃R���p�C����
	unsigned	char	cData	= MML->GetChar();
				int		repeat_cnt;

	MML->Back();

	if((cData < '0') || (cData > '9')){
		repeat_cnt = -1;
	} else {
		repeat_cnt = MML->GetInt_With_Chk_Range(_T("���s�[�g��"),2,255);
	}

	//�X�^�b�N�̍쐬
	pt_itMusic--;
	st_ct_repeat_c.push_back(repeat_cnt);		//���s�[�g��
	st_it_repeat_c_s.push_back(pt_itMusic);		//�J�n�_
	st_it_repeat_c_b.push_back(pt_itMusic);		//����_
	st_it_repeat_c_e.push_back(pt_itMusic);		//�I���_
	sp_repeat_c++;

	//�Ō���̃C�e���[�^���擾
	it_ct_repeat_c		= st_ct_repeat_c.end();
	it_it_repeat_c_s	= st_it_repeat_c_s.end();
	it_it_repeat_c_b	= st_it_repeat_c_b.end();
	it_it_repeat_c_e	= st_it_repeat_c_e.end();

	it_ct_repeat_c--;
	it_it_repeat_c_s--;
	it_it_repeat_c_b--;
	it_it_repeat_c_e--;

	//���s�[�g�^�C�v�̋L��
	repeat_type.push_back(3);
	it_repeat_type	=	repeat_type.end();
	it_repeat_type--;

}

//==============================================================
//		:	���s�[�g (A) (C)	����
//--------------------------------------------------------------
//	������
//		MMLfile*	MML		MML�t�@�C���̃I�u�W�F�N�g
//	���Ԓl
//				����
//==============================================================
void	MusicTrack::SetRepeat_Branch(MMLfile* MML)
{
	EchoVolRet();	//�^���G�R�[�̕��A

	switch(*it_repeat_type){
		case(1):
			if(is_repeat_a_s == true){
				if(is_repeat_a_b == false){
					is_repeat_a_b = true;
					SetEvent_Repeat_A_Branch();
				} else {
					MML->Err(_T("���s�[�g(A)���� : �R�}���h���d�����Ă��܂��B"));
				}
			} else {
				MML->Err(_T("���s�[�g(A)�̊J�n [ �R�}���h������܂���B"));
			}
			break;
		case(3):
			{
				list<MusicItem*>::iterator	pt_itMusic	=	ptcItem.end();
				//���s�[�g(C)�͎g���Ă���H
				if(sp_repeat_c > 0){
					//����_�͏o�������H
					if((*it_it_repeat_c_b) == (*it_it_repeat_c_s)){
						pt_itMusic--;
						(*it_it_repeat_c_b) = pt_itMusic;
					} else {
						MML->Err(_T("���s�[�g(C)���� : �R�}���h���d�����Ă��܂��B"));
					}
				} else {
					MML->Err(_T("���s�[�g(C)�̊J�n [: �R�}���h������܂���B"));
				}
			}
			break;
		default:
			MML->Err(_T("���s�[�g�J�n�R�}���h������܂���B"));
			break;
	}

}
//--------------------------------------------------------------
void	MusicTrack::SetEvent_Repeat_A_Branch()
{
	//�I�u�W�F�N�g�̍쐬
	mml_Address* _event = new mml_Address(cnt_Repert_A - 1, nsd_Repeat_A_Branch, _T("Repeat(A) Branch"));

	//�I�u�W�F�N�g��ۑ�
	SetEvent(_event);

}

//==============================================================
//		:	���s�[�g (A) (C)	�I��
//--------------------------------------------------------------
//	������
//		MMLfile*	MML		MML�t�@�C���̃I�u�W�F�N�g
//	���Ԓl
//				����
//==============================================================
void	MusicTrack::SetRepeat_End(MMLfile* MML)
{

	switch(*it_repeat_type){
		case(1):
			SetRepeat_A_End(MML);
			break;
		case(3):
			SetRepeat_C_End(MML);
			break;
		default:
			MML->Err(_T("���s�[�g�J�n�R�}���h������܂���B"));
			break;
	}

}

//==============================================================
//		]	���s�[�g(A)	�I��
//--------------------------------------------------------------
//	������
//		MMLfile*	MML		MML�t�@�C���̃I�u�W�F�N�g
//	���Ԓl
//				����
//==============================================================
void	MusicTrack::SetRepeat_A_End(MMLfile* MML)
{
	EchoVolRet();	//�^���G�R�[�̕��A

	if((is_repeat_a_s == false) || ((*it_repeat_type) != 1)){
		MML->Err(_T("���s�[�g(A)�̊J�n [ �R�}���h������܂���B"));

	} else {

		unsigned	char	cData	= MML->GetChar();

		MML->Back();

		if((cData < '0') || (cData > '9')){
			//�����������ꍇ
			if(count_repeat_a == -1){
				MML->Err(_T("���s�[�g�񐔂̋L�q������܂���B"));
			}
		} else {
			//����������ꍇ
			if(count_repeat_a != -1){
				MML->Err(_T("���s�[�g�񐔂������ɋL�q����Ă��܂��B"));
			}
			count_repeat_a = MML->GetInt_With_Chk_Range(_T("���s�[�g��"),2,255);
		}

		ptc_Repert_A[cnt_Repert_A - 1]->set_count((unsigned char)count_repeat_a);

		SetEvent_Repeat_A_End();

		is_repeat_a_s = false;

		//���s�[�g�^�C�v�̕��A
		it_repeat_type--;
		repeat_type.pop_back();
	}

}
//--------------------------------------------------------------
void	MusicTrack::SetEvent_Repeat_A_End()
{
	//�I�u�W�F�N�g�̍쐬
	mml_Address* _event = new mml_Address(cnt_Repert_A - 1, nsd_Repeat_A_End, _T("Repeat(A) End"));

	//�I�u�W�F�N�g��ۑ�
	SetEvent(_event);
	ptc_Repert_A_E[cnt_Repert_A - 1] = _event;

	Reset_opt();
}

//==============================================================
//		:]	���s�[�g(C)	�I��
//--------------------------------------------------------------
//	������
//		MMLfile*	MML		MML�t�@�C���̃I�u�W�F�N�g
//	���Ԓl
//				����
//==============================================================
void	MusicTrack::SetRepeat_C_End(MMLfile* MML)
{
	EchoVolRet();	//�^���G�R�[�̕��A

	if((sp_repeat_c == 0) || ((*it_repeat_type) != 3)){
		MML->Err(_T("���s�[�g(C)�̊J�n [: �R�}���h������܂���B"));
	} else {

		list<MusicItem*>::iterator	pt_itMusic = ptcItem.end();
		unsigned	char	cData		= MML->GetChar();
					int		repeat_cnt	= (*it_ct_repeat_c);

		MML->Back();

		if((cData < '0') || (cData > '9')){
			//�����������ꍇ
			if(repeat_cnt == -1){
				MML->Err(_T("���s�[�g�񐔂̋L�q������܂���B"));
			}
		} else {
			//����������ꍇ
			if(repeat_cnt != -1){
				MML->Err(_T("���s�[�g�񐔂������ɋL�q����Ă��܂��B"));
			}
			repeat_cnt = MML->GetInt_With_Chk_Range(_T("���s�[�g��"),2,255);
			(*it_ct_repeat_c)	= repeat_cnt;
		}

		pt_itMusic--;
		(*it_it_repeat_c_e) = pt_itMusic;

		//--------------------------
		//���s�[�g�̓W�J
		{
			unsigned	int				iRepeatCount = (*it_ct_repeat_c) - 1;

			while(iRepeatCount>0){
				pt_itMusic	=	(*it_it_repeat_c_s);
				if(pt_itMusic != *it_it_repeat_c_e){
					do{
						unsigned char	cOpCode;
						pt_itMusic++;
						cOpCode		=	(*pt_itMusic)->getCode((size_t)0);
						switch(cOpCode){
							//--------------------------
							//Call Subroutine 
							case(nsd_Call):
								{
									//���̃N���X���L�̃����o�[�ϐ������邽�߁A�ʂɃR�s�[����B
									mml_CallSub* _event = new mml_CallSub(*(mml_CallSub*)(*pt_itMusic));
									SetEvent(_event);
								}
								break;
							//--------------------------
							//Repeat
							case(nsd_Repeat_A_Start):
								{
									mml_repeat* _event = (mml_repeat*)(*pt_itMusic);
									SetEvent_Repeat_A_Start(_event->get_count());
								}
								break;
							case(nsd_Repeat_A_Branch):
								SetEvent_Repeat_A_Branch();
								break;
							case(nsd_Repeat_A_End):
								SetEvent_Repeat_A_End();
								break;
							case(nsd_Repeat_B_Start):
								SetEvent_Repeat_B_Start();
								break;
							case(nsd_Repeat_B_Branch):
								SetEvent_Repeat_B_Branch();
								break;
							case(nsd_Repeat_B_End):
								SetEvent_Repeat_B_End();
								break;
							default:
								{
									MusicEvent* _event = new MusicEvent(*(MusicEvent*)*pt_itMusic);
									SetEvent(_event);
								}
								break;
						}

						//�Ō�̃��s�[�g�ŁA����R�}���h���������ꍇ
						if((iRepeatCount == 1) && (pt_itMusic == *it_it_repeat_c_b)){
							pt_itMusic = *it_it_repeat_c_e;
						}
					} while(pt_itMusic != *it_it_repeat_c_e);
				}
				iRepeatCount--;
			}
		}

		//--------------------------
		//�Ō���̃C�e���[�^���擾
		it_ct_repeat_c--;
		it_it_repeat_c_s--;
		it_it_repeat_c_b--;
		it_it_repeat_c_e--;

		//�X�^�b�N�̔j��
		st_ct_repeat_c.pop_back();
		st_it_repeat_c_s.pop_back();
		st_it_repeat_c_b.pop_back();
		st_it_repeat_c_e.pop_back();
		sp_repeat_c--;

		Reset_opt();

		//���s�[�g�^�C�v�̕��A
		it_repeat_type--;
		repeat_type.pop_back();
	}
}

//==============================================================
//		SE	���ʉ��Ăяo��
//--------------------------------------------------------------
//	������
//		MMLfile*	MML		MML�t�@�C���̃I�u�W�F�N�g
//	���Ԓl
//				����
//==============================================================
void	MusicTrack::SetSE(MMLfile* MML)
{
	size_t	_no = MML->GetInt();
	if(jump_flag==false){
		mml_Address*		_event = new mml_Address(_no, nsd_Call_SE, _T("Call SE"));
		SetEvent(_event);
	}
}

//==============================================================
//		S	�T�u���[�`���Ăяo��
//--------------------------------------------------------------
//	������
//		MMLfile*	MML		MML�t�@�C���̃I�u�W�F�N�g
//	���Ԓl
//				����
//==============================================================
void	MusicTrack::SetSubroutine(size_t _no)
{
	if(jump_flag==false){
		mml_CallSub*	_event = new mml_CallSub(_no);
		SetEvent(_event);
	};
	Reset_opt();
}

//==============================================================
//		S	�T�u���[�`���Ăяo���i�p�b�`����j
//--------------------------------------------------------------
//	������
//		size_t	_no,			�ďoNo
//		bool	_f_Disable_Opt	�œK�����������H
//	���Ԓl
//				����
//==============================================================
void	MusicTrack::SetSubWithParch(size_t _no,bool _f_Disable_Opt)
{
	if(jump_flag==false){
		mml_CallSub*	_event = new mml_CallSub(_no, _T("Call Subroutine for Patch"));
		_event->setPatch(_f_Disable_Opt);
		SetEvent(_event);
	};
}

//==============================================================
//		�p�b�`
//--------------------------------------------------------------
//	������
//		MMLfile*	MML		MML�t�@�C���̃I�u�W�F�N�g
//	���Ԓl
//				����
//==============================================================
void	MusicTrack::SetPatch(MMLfile* MML)
{
	f_Patch = true;
	i_Patch	= MML->GetInt();

	if(MML->ChkPatch(i_Patch) == false){
		MML->Err(_T("���݂��Ȃ��p�b�`�ł��B"));
	}
}

//--------------------------------------------------------------
void	MusicTrack::SetPatch()
{
	f_Patch = false;
}

//==============================================================
//		�p�b�`�̓W�J
//--------------------------------------------------------------
//	������
//		MMLfile*	MML		MML�t�@�C���̃I�u�W�F�N�g
//		char		 _note	�m�[�g�ԍ�
//	���Ԓl
//				����
//==============================================================
void	MusicTrack::CallPatch(MMLfile* MML, char _note)
{
	if(f_Patch == true){
		//�m�[�g�ԍ��̐ݒ�
		MML->ptcPatch[i_Patch]->setNote(_note);

		if(	MML->ptcPatch[i_Patch]->get_fSub() == true){
			SetSubWithParch(MML->ptcPatch[i_Patch]->get_iSub(), MML->ptcPatch[i_Patch]->get_fSub_opt());
		}

		if(	MML->ptcPatch[i_Patch]->get_fGate_q() == true){
			Set_q(MML->ptcPatch[i_Patch]->get_iGate_q());
		}

		if(	MML->ptcPatch[i_Patch]->get_fGate_u() == true){
			Set_u(MML->ptcPatch[i_Patch]->get_iGate_u());
		}

		if(	MML->ptcPatch[i_Patch]->get_fSweep() == true){
			SetSweep(MML->ptcPatch[i_Patch]->get_iSweep());
		}

		if(	MML->ptcPatch[i_Patch]->get_fKey() == true){
			SetTranspose(MML->ptcPatch[i_Patch]->get_iKey());
		}

		if(	MML->ptcPatch[i_Patch]->get_fVoi() == true){
			SetVoice(MML->ptcPatch[i_Patch]->get_iVoi());
		}

		if(	MML->ptcPatch[i_Patch]->get_fEvoi() == true){
			if(MML->ptcPatch[i_Patch]->get_sw_Evoi() == true){
				SetEnvelop_Evoi(MML->ptcPatch[i_Patch]->get_iEvoi() + MML->offset_Ei);
			}
		}

		if(	MML->ptcPatch[i_Patch]->get_fEvol() == true){
			if(MML->ptcPatch[i_Patch]->get_sw_Evol() == true){
				SetEnvelop_Evol(MML->ptcPatch[i_Patch]->get_iEvol() + MML->offset_Ev);
			} else {
				SetEnvelop_Evol();
			}
		}

		if(	MML->ptcPatch[i_Patch]->get_fEm() == true){
			if(MML->ptcPatch[i_Patch]->get_sw_Em() == true){
				SetEnvelop_Em(MML->ptcPatch[i_Patch]->get_iEm() + MML->offset_Em);
			} else {
				SetEnvelop_Em();
			}
		}

		if(	MML->ptcPatch[i_Patch]->get_fEn() == true){
			if(MML->ptcPatch[i_Patch]->get_sw_En() == true){
				SetEnvelop_En(MML->ptcPatch[i_Patch]->get_iEn() + MML->offset_En);
			} else {
				SetEnvelop_En();
			}
		}

	}
}

//==============================================================
//		E	�G���x���[�v�ݒ�
//--------------------------------------------------------------
//	������
//		MMLfile*	MML		MML�t�@�C���̃I�u�W�F�N�g
//	���Ԓl
//				����
//==============================================================
void	MusicTrack::SetEnvelop_Evoi(size_t _no)
{
	nsd.env_volume		= _no;
	nsd.sw_Evoi			= true;
	SetEvent(new mml_Address(_no, nsd_Envelop_Voice, _T("Voice Envelope")));
}

//--------------------------------------------------------------
void	MusicTrack::SetEnvelop_Evol(size_t _no)
{
	nsd.env_voice		= _no;
	nsd.sw_Evol			= true;
	SetEvent(new mml_Address(_no, nsd_Envelop_Volume, _T("Volume Envelope")));
}

//--------------------------------------------------------------
void	MusicTrack::SetEnvelop_Em(size_t _no)
{
	nsd.env_frequency	= _no;
	nsd.sw_Em			= true;
	SetEvent(new mml_Address(_no, nsd_Envelop_Frequency, _T("Frequency Envelope")));
}

//--------------------------------------------------------------
void	MusicTrack::SetEnvelop_En(size_t _no)
{
	nsd.env_note		= _no;
	nsd.sw_En			= true;
	SetEvent(new mml_Address(_no, nsd_Envelop_Note, _T("Note Envelope")));
}

//--------------------------------------------------------------
void	MusicTrack::SetVoice(int _no)
{
	nsd.voice			= _no;
	nsd.sw_Evoi			= false;
	SetEvent(new mml_general(nsd_Voice, (unsigned char)_no, _T("Voice")));
}

//--------------------------------------------------------------
void	MusicTrack::SetEnvelop_Evol()
{
	nsd.sw_Evol	= false;
	SetEvent(new mml_Address(nsd_Envelop_Volume, _T("Volume Envelope Off")));
}

//--------------------------------------------------------------
void	MusicTrack::SetEnvelop_Em()
{
	nsd.sw_Em	= false;
	SetEvent(new mml_Address(nsd_Envelop_Frequency, _T("Frequency Envelope Off")));
}

//--------------------------------------------------------------
void	MusicTrack::SetEnvelop_En()
{
	nsd.sw_En	= false;
	SetEvent(new mml_Address(nsd_Envelop_Note, _T("Note Envelope Off")));
}

//==============================================================
//		�����ŗL�p�����[�^�i�X�C�[�v�j
//--------------------------------------------------------------
//	������
//		MMLfile*	MML		MML�t�@�C���̃I�u�W�F�N�g
//	���Ԓl
//		����
//==============================================================
void	MusicTrack::SetSweep(MMLfile* MML)
{
				int		iSpeed;
				int		iDepth;
	unsigned	char	cData;

	iSpeed = MML->GetInt();		//�����̐��Ń����W���قȂ�̂ŁA��Ń`�F�b�N����

	cData = MML->GetChar();
	if(cData != ','){
		MML->Chk_Range(_T("s�R�}���h"),0,255,iSpeed);
		MML->Back();
		SetSweep((unsigned char)iSpeed);
	} else {
		MML->Chk_Range(_T("s�R�}���h�̑�1�p�����[�^"),0,15,iSpeed);
		iDepth = MML->GetInt();
		MML->Chk_Range(_T("s�R�}���h�̑�2�p�����[�^"),0,15,iDepth);
		SetSweep((unsigned char)(((iSpeed & 0x0F) << 4) | (iDepth & 0x0F)));
	}
}
//--------------------------------------------------------------
void	MusicTrack::SetSweep(unsigned char _c)
{
	SetEvent(new mml_general(nsd_Sweep, _c, _T("Sweep")));
}

//==============================================================
//		@FC	FDS
//--------------------------------------------------------------
//	������
//		MMLfile*	MML		MML�t�@�C���̃I�u�W�F�N�g
//	���Ԓl
//				����
//==============================================================
void	MusicTrack::SetFDSC(MMLfile* MML)
{
	size_t			_no		= MML->GetInt();
	mml_Address*	_event	= new mml_Address(_no, nsd_FDS_Career, _T("FDS career wave table"));

	SetEvent(_event);
}

//==============================================================
//		@FM	FDS
//--------------------------------------------------------------
//	������
//		MMLfile*	MML		MML�t�@�C���̃I�u�W�F�N�g
//	���Ԓl
//				����
//==============================================================
void	MusicTrack::SetFDSM(MMLfile* MML)
{
	size_t			_no		= MML->GetInt();
	mml_Address*	_event	= new mml_Address(_no, nsd_FDS_Modlator, _T("FDS modulator wave table"));

	SetEvent(_event);
}

//==============================================================
//		@V	VRC7���[�U��`���F�ݒ�
//--------------------------------------------------------------
//	������
//		MMLfile*	MML		MML�t�@�C���̃I�u�W�F�N�g
//	���Ԓl
//				����
//==============================================================
void	MusicTrack::SetVRC7(MMLfile* MML)
{
	size_t			_no		= MML->GetInt();
	mml_Address*	_event	= new mml_Address(_no, nsc_VRC7, _T("VRC7 user instruments"));

	SetEvent(_event);
}

//==============================================================
//		@N	n163���F�ݒ�
//--------------------------------------------------------------
//	������
//		MMLfile*	MML		MML�t�@�C���̃I�u�W�F�N�g
//	���Ԓl
//				����
//==============================================================
void	MusicTrack::SetN163(MMLfile* MML)
{
	unsigned	char	cNum	= (unsigned char)MML->GetInt_With_Chk_Range(_T("n16x�̔g�`�J�n�A�h���X"),0,63);		//���F�ԍ�
				size_t	_no;
	mml_Address*		_event;

	MML->Chk_Comma();
	_no = MML->GetInt();
	_event	= new mml_Address(_no, (unsigned char)nsc_N163, cNum, _T("n163 wave table"));
	SetEvent(_event);
}

//--------------------------------------------------------------
//		@NL
//--------------------------------------------------------------
void	MusicTrack::SetN163_Load(MMLfile* MML)
{
			int		i	= MML->GetInt_With_Chk_Range(_T("n16x�̔g�`�J�n�_"),0,252);
			size_t	_no;
	mml_Address*	_event;

	if((i % 4) != 0){
		MML->Err(_T("n16x�̔g�`�J�n�_��4�̔{���Ŏw�肵�Ă��������B"));
	}

	MML->Chk_Comma();
	_no = MML->GetInt();
	_event = new mml_Address(_no, (unsigned char)nsc_N163,(unsigned char)(i/4),_T("n163 wave table"));
	SetEvent(_event);
}

//==============================================================
//		@NS	n163���F�ݒ�
//--------------------------------------------------------------
//	������
//		MMLfile*	MML		MML�t�@�C���̃I�u�W�F�N�g
//	���Ԓl
//				����
//==============================================================
void	MusicTrack::SetN163_Set(MMLfile* MML)
{
				int		i = MML->GetInt_With_Chk_Range(_T("n16x�̔g�`�J�n�_"),0,252);
	unsigned	char	cData;

	if((i % 4) != 0){
		MML->Err(_T("n16x�̔g�`�J�n�_��4�̔{���Ŏw�肵�Ă��������B"));
	}
	SetVoice(i/4);

	cData = MML->GetChar();
	if(cData == ','){
		i = MML->GetInt_With_Chk_Range(_T("n16x�̃T���v����"),4,256);
		if((i % 4) != 0){
			MML->Err(_T("n16x�̃T���v������4�̔{���Ŏw�肵�Ă��������B"));
		}
		SetSweep((unsigned char)(64 - (i/4)));
	} else {
		MML->Back();
	}
}

//==============================================================
//		J	�W�����v
//--------------------------------------------------------------
//	������
//		MMLfile*	MML		MML�t�@�C���̃I�u�W�F�N�g
//	���Ԓl
//				����
//==============================================================
void	MusicTrack::SetJump(MMLfile* MML)
{
	int		_value = MML->GetInt_With_Chk_Range(_T("J�R�}���h"),0,1);

	switch(_value){
		case(0):
			jump_flag = false;
			break;
		case(1):
			jump_flag = true;
			break;
		default:
			throw	out_of_range("MusicTrack::SetJump(MMLfile* MML)");
	}

}

//==============================================================
//		�Q�[�g�^�C��(q)
//--------------------------------------------------------------
//	������
//		int		q		�N�I���^�C�Y
//	���Ԓl
//		����
//==============================================================
void	MusicTrack::Set_q(int i)
{
	nsd.gate_q = i;
	if( (i <= 15) && (i >= 0) ){
		SetEvent(new mml_general(nsd_GateTime_Byte + (unsigned char)i, _T("Gatetime(q) Byte")));
	} else {
		SetEvent(new mml_general(nsd_GateTime_q, (unsigned char)i, _T("Gatetime(q)")));
	}
}

//==============================================================
//		�Q�[�g�^�C��(u)
//--------------------------------------------------------------
//	������
//		int		u		�N�I���^�C�Y
//	���Ԓl
//		����
//==============================================================
void	MusicTrack::Set_u(int i)
{
	nsd.gate_u = i;
	SetEvent(new mml_general(nsd_GateTime_u, (unsigned char)i, _T("GateTime(u)")));
}

//==============================================================
//		�Q�[�g�^�C��(Q)
//--------------------------------------------------------------
//	������
//		MMLfile*	MML		MML�t�@�C���̃I�u�W�F�N�g
//	���Ԓl
//		����
//==============================================================
void	MusicTrack::SetGatetime_Q(MMLfile* MML)
{
	int	i = MML->GetInt_With_Chk_Range(_T("�Q�[�g�N�I���^�C�Y(Q)"),1,QMax);

	gatetime_Q = i;
	if(gatetime_Q == QMax){
		gatetime_q = 0;
		Set_q(gatetime_q);
	}
}

//==============================================================
//		�Q�[�g�^�C��(q)
//--------------------------------------------------------------
//	������
//		MMLfile*	MML		MML�t�@�C���̃I�u�W�F�N�g
//	���Ԓl
//		����
//==============================================================
void	MusicTrack::SetGatetime(MMLfile* MML)
{
	gatetime_q = MML->GetInt_With_Chk_Range(_T("�Q�[�g�N�I���^�C�Y(q)"),0,255);

	gatetime_Q = QMax;
	Set_q(gatetime_q);
}

//==============================================================
//		�Q�[�g�^�C��(u)
//--------------------------------------------------------------
//	������
//		MMLfile*	MML		MML�t�@�C���̃I�u�W�F�N�g
//	���Ԓl
//		����
//==============================================================
void	MusicTrack::SetGatetime_u(MMLfile* MML)
{
					int		i;
	unsigned		char	cData;

	cData = MML->GetChar();
	if(cData == '0'){
		i = 0;
	} else {
		MML->Back();
		i = MML->GetLength(-1);
	}
	Set_u(i);
}

//==============================================================
//		�����[�X���[�h
//--------------------------------------------------------------
//	������
//		MMLfile*	MML		MML�t�@�C���̃I�u�W�F�N�g
//	���Ԓl
//		����
//==============================================================
void	MusicTrack::SetReleaseMode(MMLfile* MML)
{
	mml_general*	_event	= NULL;

	int	i = MML->GetInt_With_Chk_Range(_T("�����[�X���[�h"),0,2);

	switch(i){
		case(0):
			_event = new mml_general(nsd_GateMode_0,  _T("GateMode 0"));
			break;
		case(1):
			_event = new mml_general(nsd_GateMode_1,  _T("GateMode 1"));
			break;
		case(2):
			_event = new mml_general(nsd_GateMode_2,  _T("GateMode 2"));
			break;
		default:
			throw	out_of_range("MusicTrack::SetReleaseMode()");
	}
	SetEvent(_event);
}

//==============================================================
//		�����[�X���F
//--------------------------------------------------------------
//	������
//		MMLfile*	MML		MML�t�@�C���̃I�u�W�F�N�g
//	���Ԓl
//		����
//==============================================================
void	MusicTrack::SetReleaseVoice(MMLfile* MML)
{
	int	i = MML->GetInt_With_Chk_Range(_T("�����[�X���̉��F"),0,7);

	SetEvent(new mml_general(nsd_Release_Voice + (unsigned char)i, _T("Release Voice")));
}

//==============================================================
//		�����[�X����
//--------------------------------------------------------------
//	������
//		MMLfile*	MML		MML�t�@�C���̃I�u�W�F�N�g
//	���Ԓl
//		����
//==============================================================
void	MusicTrack::SetReleaseVolume(MMLfile* MML)
{
	int	i = MML->GetInt_With_Chk_Range(_T("����"),0,15);

	SetEvent(new mml_general(nsd_Release_Volume + (unsigned char)i, _T("Release Volume")));
}

//==============================================================
//		�����̐ݒ�
//--------------------------------------------------------------
//	������
//		char	_c �` _b	�ݒ�l
//	���Ԓl
//		����
//==============================================================
void	MusicTrack::SetKeyFlag(char _c, char _d, char _e, char _f, char _g, char _a, char _b)
{
	KeySignature[0] = _c;	//c
	KeySignature[1] = _d;	//d
	KeySignature[2] = _e;	//e
	KeySignature[3] = _f;	//f
	KeySignature[4] = _g;	//g
	KeySignature[5] = _a;	//a
	KeySignature[6] = _b;	//b
}

//==============================================================
//		�����̐ݒ�
//--------------------------------------------------------------
//	������
//		int	_key	����ID
//	���Ԓl
//		����
//==============================================================
void	MusicTrack::SetKey(int _key, int _scale)
{

	static	const	char	Shift[16]={3,6,2,5,1,4,0,3,6,2,5,1,4,0,3,6};

	//��
	static	const	char	Key[16][7]={
	//	  c  d  e  f  g  a  b			n	ID		Key			Shift
		{-1,-1,-1, 0,-1,-1,-1},		//	0	-6		Gs			3
		{ 0,-1,-1, 0,-1,-1,-1},		//	1	-5		Ds			6
		{ 0,-1,-1, 0, 0,-1,-1},		//	2	-4		As			2
		{ 0, 0,-1, 0, 0,-1,-1},		//	3	-3		Es			5
		{ 0, 0,-1, 0, 0, 0,-1},		//	4	-2		Bs			1
		{ 0, 0, 0, 0, 0, 0,-1},		//	5	-1		F			4
		{ 0, 0, 0, 0, 0, 0, 0},		//	6	 0		C			0
		{ 0, 0, 0, 1, 0, 0, 0},		//	7	 1		G			3
		{ 1, 0, 0, 1, 0, 0, 0},		//	8	 2		D			6
		{ 1, 0, 0, 1, 1, 0, 0},		//	9	 3		A			2
		{ 1, 1, 0, 1, 1, 0, 0},		//	10	 4		E			5
		{ 1, 1, 0, 1, 1, 1, 0},		//	11	 5		H			1
		{ 1, 1, 1, 1, 1, 1, 0},		//	12	 6		Fis			4
		{ 1, 1, 1, 1, 1, 1, 1},		//	13	 7		Cis			0
		{ 1, 1, 1, 2, 1, 1, 1},		//	14	 8		Gis			3
		{ 2, 1, 1, 2, 1, 1, 1}		//	15	 9		Dis			6
	};

	//��
	static	const	char	Scale[80][7]={
	//	  c  d  e  f  g  a  b		ID		Scale (Key = C)		Name
		{ 0, 0, 0, 0, 0, 0, 0},	//	0		c d e f g a b		Ionian
		{ 0, 0,-1, 0, 0, 0,-1},	//	1		c d e-f g a b-		Dorian
		{ 0,-1,-1, 0, 0,-1,-1},	//	2		c d-e-f g a-b-		Phrygian
		{ 0, 0, 0, 1, 0, 0, 0},	//	3		c d e f+g a b		Lydian
		{ 0, 0, 0, 0, 0, 0,-1},	//	4		c d e f g a b-		Mixo-Lydian
		{ 0, 0,-1, 0, 0,-1,-1},	//	5		c d e-f g a-b-		Aeolian
		{ 0,-1,-1, 0,-1,-1,-1},	//	6		c d-e-f g-a-b-		Locrian
		{ 0, 1, 1, 1, 0, 1, 0},	//	7		c d+e+f+g a+b		Minor blues
		{ 0, 0,-1,-1, 0, 0, 0},	//	8		c d e-f-g a b		Major blues
		{ 0, 0, 0, 0, 0, 0, 0},	//	9		Dummy

		{ 0, 0,-1, 0, 0,-1, 0},	//	10		c d e-f g a-b		Harmonic minor
		{ 0,-1,-1, 0,-1, 0,-1},	//	11		c d-e-f g-a b-		Locrian natural13
		{ 0, 0, 0, 0, 1, 0, 0},	//	12		c d e f g+a b		Ionian Augmented
		{ 0, 0,-1, 1, 0, 0,-1},	//	13		c d e-f g a b-		Dorian ��11
		{ 0,-1, 0, 0, 0,-1,-1},	//	14		c d-e f g a-b-		Harmonic minor perfect 5th below (Spanish Gypsy)
		{ 0, 1, 0, 1, 0, 0, 0},	//	15		c d+e f+g a b		Lydian ��9
		{ 0,-1,-1,-1,-1,-1,-2},	//	16		c d-e-f-g-a-a		Altered ��7 (Ultra Locrian)
		{ 0, 0, 0, 0, 0, 0, 0},	//	17		Dummy
		{ 0, 0, 0, 0, 0, 0, 0},	//	18		Dummy
		{ 0, 0, 0, 0, 0, 0, 0},	//	19		Dummy

		{ 0, 0,-1, 0, 0, 0, 0},	//	20		c d e-f g a b		Melodic minor
		{ 0,-1,-1, 0, 0, 0,-1},	//	21		c d-e-f g a b-		Dorian ��9
		{ 0, 0, 0, 1, 1, 0, 0},	//	22		c d e f+g+a b		Lydian Augmented
		{ 0, 0, 0, 1, 0, 0,-1},	//	23		c d e f+g a b-		Mixo-Lydian ��11 (Lydian Dominant)
		{ 0, 0, 0, 0, 0,-1,-1},	//	24		c d e f g a-b-		Mixo-Lydian ��13
		{ 0, 0,-1, 0,-1,-1,-1},	//	25		c d e-f g-a-b-		Aeolian ��5 (Semi Locrian)
		{ 0,-1,-1,-1,-1,-1,-1},	//	26		c d-e-f-g-a-b-		Altered (Super Locrian)
		{-1,-1,-1, 0, 0,-1,-1},	//	27		c-d-e-f g a-b-		G Altered
		{ 0, 0, 0, 0, 0, 0, 0},	//	28		Dummy
		{ 0, 0, 0, 0, 0, 0, 0},	//	29		Dummy

		//c  d  e  f  g  a  b
		{ 0, 0, 0, 0, 0,-1, 0},	//	30		c d e f g a-b		Harmonic Major
		{ 0, 0,-1, 0,-1, 0,-1},	//	31		c d e-f g-a b-		Dorian ��5
		{ 0,-1,-1,-1, 0,-1,-1},	//	32		c d-e-f-g a-b-		Phrygian ��4
		{ 0, 0,-1, 1, 0, 0, 0},	//	33		c d e-f+g a b		Lydian ��3
		{ 0,-1, 0, 0, 0, 0,-1},	//	34		c d-e f g a b-		Mixo-Lydian ��9
		{ 0, 1, 0, 1, 1, 0, 0},	//	35		c d+e f+g+a b		Lydian ��2 ��5
		{ 0,-1,-1, 0,-1,-1,-2},	//	36		c d-e-f g-a-a		Locrian ���7
		{ 0, 0, 0, 0, 0, 0, 0},	//	37		Dummy
		{ 0, 0, 0, 0, 0, 0, 0},	//	38		Dummy
		{ 0, 0, 0, 0, 0, 0, 0},	//	39		Dummy

		{ 0,-1,-1, 0, 0, 0, 0},	//	40		c d-e-f g a b		Neapolitan
		{ 0, 0, 0, 1, 1, 1, 0},	//	41		c d e f+g+a+b		Leading whole-tone
		{ 0, 0, 0, 1, 1, 0,-1},	//	42		c d e f+g+a b-		Lydian Augmented Dominant
		{ 0, 0, 0, 1, 0,-1,-1},	//	43		c d e f+g a-b-		Lydian Dominant ��6 
		{ 0, 0, 0, 0,-1,-1,-1},	//	44		c d e f g-a-b-		Major Locrian
		{ 0, 0,-1,-1,-1,-1,-1},	//	45		c d e-f-g-a-b-		Semi Locrian ��4 
		{ 0,-1,-2,-1,-1,-1,-1},	//	46		c d-d f-g-a-b-		Super Locrian ���3
		{ 0, 0, 0, 0, 0, 0, 0},	//	47		Dummy
		{ 0, 0, 0, 0, 0, 0, 0},	//	48		Dummy
		{ 0, 0, 0, 0, 0, 0, 0},	//	49		Dummy

		{ 0,-1,-1, 0, 0,-1, 0},	//	50		c d-e-f g a-b		Neapolitan minor
		{ 0, 0, 0, 1, 0, 1, 0},	//	51		c d e f+g a+b		Lydian ��6 
		{ 0, 0, 0, 0, 1, 0,-1},	//	52		c d e f g+a b-		Mixo-Lydian Augmented
		{ 0, 0,-1, 1, 0,-1,-1},	//	53		c d e-f+g a-b-		Hungarian Gypsy 
		{ 0,-1, 0, 0,-1,-1,-1},	//	54		c d-e f g-a-b-		Locrian Dominant
		{ 0, 1, 0, 0, 0, 0, 0},	//	55		c d+e f g a b		Ionian ��2
		{ 0,-1,-2,-1,-1,-1,-2},	//	56		c d-d f-g-a-a		Ultra Locrian ���3
		{ 0, 0, 0, 0, 0, 0, 0},	//	57		Dummy
		{ 0, 0, 0, 0, 0, 0, 0},	//	58		Dummy
		{ 0, 0, 0, 0, 0, 0, 0},	//	59		Dummy

		{ 0,-1, 0, 0, 0,-1, 0},	//	60		c d-e f g a-b		Gypsy
		{ 0, 1, 0, 1, 0, 1, 0},	//	61		c d+e f+g a+b		Lydian ��2 ��6
		{ 0,-1,-1,-1, 0,-1,-2},	//	62		c d-e-f-g a-a		Ultra Phrygian
		{ 0, 0,-1, 1, 0,-1, 0},	//	63		c d e-f+g a-b		Hungarian minor
		{ 0,-1, 0, 0,-1, 0,-1},	//	64		c d-e f g-a b-		Oriental
		{ 0, 1, 0, 0, 1, 0, 0},	//	65		c d+e f g+a b		Ionian Augmented ��2
		{ 0,-1,-2, 0,-1,-1,-2},	//	66		c d-d f g-a-a		Locrian ���3 ���7
		{ 0, 0, 0, 0, 0, 0, 0},	//	67		Dummy
		{ 0, 0, 0, 0, 0, 0, 0},	//	68		Dummy
		{ 0, 0, 0, 0, 0, 0, 0},	//	69		Dummy

		{ 0, 1, 0, 1, 0, 0,-1},	//	70		c d+e f+g a b-		Hungarian major
		{ 0,-1,-1,-1,-1,-2,-2},	//	71		c d-e-f-g-g a		Super Locrian ���6 ���7 
		{ 0, 0,-1, 0,-1,-1, 0},	//	72		c d e-f g-a-b		Harmonic minor ��5
		{ 0,-1,-1,-1,-1, 0,-1},	//	73		c d-e-f-g-a b-		Super Locrian ��6 
		{ 0, 0,-1, 0, 1, 0, 0},	//	74		c d e-f g+a b		Melodic minor ��5
		{ 0,-1,-1, 1, 0, 0,-1},	//	75		c d-e-f+g a b-		Dorian ��9 ��11
		{ 0, 0, 1, 1, 1, 0, 0},	//	76		c d e+f+g+a b		Lydian Augmented ��3
		{ 0, 0, 0, 0, 0, 0, 0},	//	77		Dummy
		{ 0, 0, 0, 0, 0, 0, 0},	//	78		Dummy
		{ 0, 0, 0, 0, 0, 0, 0}	//	79		Dummy

	};

	char	shift;
	char	shift_scale[7];

	nowKey	 = (char)_key;
	nowScale = 0;

	shift = Shift[nowKey + 6];
	shift_scale[0] = (char)Scale[_scale][(shift + 0) % 7];
	shift_scale[1] = (char)Scale[_scale][(shift + 1) % 7];
	shift_scale[2] = (char)Scale[_scale][(shift + 2) % 7];
	shift_scale[3] = (char)Scale[_scale][(shift + 3) % 7];
	shift_scale[4] = (char)Scale[_scale][(shift + 4) % 7];
	shift_scale[5] = (char)Scale[_scale][(shift + 5) % 7];
	shift_scale[6] = (char)Scale[_scale][(shift + 6) % 7];

	SetKeyFlag(	Key[nowKey + 6][0] + shift_scale[0],
				Key[nowKey + 6][1] + shift_scale[1],
				Key[nowKey + 6][2] + shift_scale[2],
				Key[nowKey + 6][3] + shift_scale[3],
				Key[nowKey + 6][4] + shift_scale[4],
				Key[nowKey + 6][5] + shift_scale[5],
				Key[nowKey + 6][6] + shift_scale[6]
	);
}

//==============================================================
//		�X�P�[���̐ݒ�
//--------------------------------------------------------------
//	������
//		MMLfile*	MML		MML�t�@�C���̃I�u�W�F�N�g
//	���Ԓl
//		����
//==============================================================
void	MusicTrack::SetScale(MMLfile* MML)
{
	nowScale	= (char)MML->GetNum();
	SetKey(nowKey, nowScale);
}

//--------------------------------------------------------------
void	MusicTrack::SetMajor()
{
	nowScale = 0;
	SetKey(nowKey, nowScale);
}
//--------------------------------------------------------------
void	MusicTrack::SetMinor()
{
	nowScale = 5;
	SetKey(nowKey, nowScale);
}
//--------------------------------------------------------------
void	MusicTrack::SetHMinor(MMLfile* MML)
{
	char	cData		= MML->GetChar();
			nowScale	= 10;

	MML->Back();

	if(cData=='('){
		nowScale	+= (char)(MML->GetNum());
	}

	SetKey(nowKey, nowScale);
}

//--------------------------------------------------------------
void	MusicTrack::SetMMinor(MMLfile* MML)
{
	char	cData		= MML->GetChar();
			nowScale	= 20;

	MML->Back();

	if(cData=='('){
		nowScale	+= (char)(MML->GetNum());
	}

	SetKey(nowKey, nowScale);
}

//==============================================================
//		�����̐ݒ�
//--------------------------------------------------------------
//	������
//		MMLfile*	MML		MML�t�@�C���̃I�u�W�F�N�g
//	���Ԓl
//		����
//==============================================================
void	MusicTrack::SetKeySignature(MMLfile*	MML)
{
	//�����R�}���h��ID
	enum	KeySignature_ID {
		ks_Sharp,
		ks_Flat,
		ks_Natural,
		ks_c,
		ks_d,
		ks_e,
		ks_f,
		ks_g,
		ks_a,
		ks_b,
		ks_r,

		ks_0,
		ks_s1,
		ks_s2,
		ks_s3,
		ks_s4,
		ks_s5,
		ks_s6,
		ks_s7,
		ks_f1,
		ks_f2,
		ks_f3,
		ks_f4,
		ks_f5,
		ks_f6,
		ks_f7,

		ks_m0,
		ks_ms1,
		ks_ms2,
		ks_ms3,
		ks_ms4,
		ks_ms5,
		ks_ms6,
		ks_ms7,
		ks_ms8,
		ks_ms9,

		ks_mf1,
		ks_mf2,
		ks_mf3,
		ks_mf4,
		ks_mf5,
		ks_mf6,
	};

	//�����R�}���h���̒�`
	static	const	Command_Info	KS_Command[] = {
		{	"C-Dur",	ks_0		},	//
		{	"Ds-Dur",	ks_f5		},	//bbbbb
		{	"Des-Dur",	ks_f5		},	//bbbbb
		{	"D-Dur",	ks_s2		},	//##
		{	"Es-Dur",	ks_f3		},	//bbb
		{	"E-Dur",	ks_s4		},	//####
		{	"F-Dur",	ks_f1		},	//b
		{	"Fis-Dur",	ks_s6		},	//######
		{	"Gs-Dur",	ks_f6		},	//bbbbbb
		{	"Ges-Dur",	ks_f6		},	//bbbbbb
		{	"G-Dur",	ks_s1		},	//#
		{	"As-Dur",	ks_f4		},	//bbbb
		{	"A-Dur",	ks_s3		},	//###
		{	"B-Dur",	ks_f2		},	//bb
		{	"Bs-Dur",	ks_f2		},	//bb
		{	"H-Dur",	ks_s5		},	//#####

		{	"c-moll",	ks_m0		},	//bbb
		{	"cis-moll",	ks_ms7		},	//####
		{	"d-moll",	ks_ms2		},	//b
		{	"dis-moll",	ks_ms9		},	//######
		{	"es-moll",	ks_mf3		},	//bbbbbb
		{	"e-moll",	ks_ms4		},	//#
		{	"f-moll",	ks_mf1		},	//bbbb
		{	"fis-moll",	ks_ms6		},	//###
		{	"g-moll",	ks_ms1		},	//bb
		{	"gis-moll",	ks_ms8		},	//#####
		{	"a-moll",	ks_ms3		},	//
		{	"b-moll",	ks_mf2		},	//bbbbb
		{	"bs-moll",	ks_mf2		},	//bbbbb
		{	"h-moll",	ks_ms5		},	//##

		//���y���_�I�ɐ������Ȃ����ߋ��݊����ׁ̈B���n�̒��Ȃ̂Ɂ�ł͌���Ȃ��I
		{	"ds-moll",	ks_mf5		},	//####
		{	"des-moll",	ks_mf5		},	//####
		{	"gs-moll",	ks_mf6		},	//###
		{	"ges-moll",	ks_mf6		},	//###
		{	"as-moll",	ks_mf4		},	//#####

		{	"+",		ks_Sharp	},
		{	"�{",		ks_Sharp	},
		{	"#",		ks_Sharp	},
		{	"��",		ks_Sharp	},
		{	"-",		ks_Flat		},
		{	"�|",		ks_Flat		},
		{	"��",		ks_Flat		},
		{	"=",		ks_Natural	},
		{	"��",		ks_Natural	},
		{	"*",		ks_Natural	},
		{	"��",		ks_Natural	},

		{	"c",		ks_c		},
		{	"d",		ks_d		},
		{	"e",		ks_e		},
		{	"f",		ks_f		},
		{	"g",		ks_g		},
		{	"a",		ks_a		},
		{	"b",		ks_b		},
		{	"r",		ks_r		},

		{	"�h",		ks_c		},
		{	"��",		ks_d		},
		{	"�~",		ks_e		},
		{	"�t�@",		ks_f		},
		{	"�t",		ks_f		},
		{	"�\",		ks_g		},
		{	"��",		ks_a		},
		{	"�V",		ks_b		},
		{	"��",		ks_r		},
		{	"�b",		ks_r		},

		{	"��",		ks_c		},
		{	"��",		ks_d		},
		{	"��",		ks_e		},
		{	"�ӂ�",		ks_f		},
		{	"��",		ks_f		},
		{	"��",		ks_g		},
		{	"��",		ks_a		},
		{	"��",		ks_b		},
		{	"��",		ks_r		},
		{	"��",		ks_r		},

		{	"�n",		ks_c		},
		{	"�j",		ks_d		},
		{	"�z",		ks_e		},
		{	"�w",		ks_f		},
		{	"�g",		ks_g		},
		{	"�C",		ks_a		},
		{	"��",		ks_b		}

	};

	char	cData;
	char	sign = 0;

	// { �̌���
	MML->Chk_LeftCurlyBrace();

	// } ������܂ŁA�L�q�u���b�N�����R���p�C������B
	while(MML->GetChar_With_Chk_RightCurlyBrace(&cData)){

		//�P�߂�
		MML->Back();

		//�e�R�}���h���̏���
		switch(MML->GetCommandID(KS_Command, sizeof(KS_Command)/sizeof(Command_Info))){
			case(ks_c):
				KeySignature[0] = sign;
				break;
			case(ks_d):
				KeySignature[1] = sign;
				break;
			case(ks_e):
				KeySignature[2] = sign;
				break;
			case(ks_f):
				KeySignature[3] = sign;
				break;
			case(ks_g):
				KeySignature[4] = sign;
				break;
			case(ks_a):
				KeySignature[5] = sign;
				break;
			case(ks_b):
				KeySignature[6] = sign;
				break;
			case(ks_r):
				KeySignature[7] = sign;
				break;
			case(ks_Natural):
				sign = 0;
				break;
			case(ks_Sharp):
				sign = 1;
				break;
			case(ks_Flat):
				sign = -1;
				break;

			case(ks_0):
				nowKey		= 0;
				nowScale	= 0;
				SetKey(nowKey, nowScale);
				break;
			case(ks_s1):
				nowKey		= 1;
				nowScale	= 0;
				SetKey(nowKey, nowScale);
				break;
			case(ks_s2):
				nowKey		= 2;
				nowScale	= 0;
				SetKey(nowKey, nowScale);
				break;
			case(ks_s3):
				nowKey		= 3;
				nowScale	= 0;
				SetKey(nowKey, nowScale);
				break;
			case(ks_s4):
				nowKey		= 4;
				nowScale	= 0;
				SetKey(nowKey, nowScale);
				break;
			case(ks_s5):
				nowKey		= 5;
				nowScale	= 0;
				SetKey(nowKey, nowScale);
				break;
			case(ks_s6):
				nowKey		= 6;
				nowScale	= 0;
				SetKey(nowKey, nowScale);
				break;
			case(ks_f1):
				nowKey		= -1;
				nowScale	= 0;
				SetKey(nowKey, nowScale);
				break;
			case(ks_f2):
				nowKey		= -2;
				nowScale	= 0;
				SetKey(nowKey, nowScale);
				break;
			case(ks_f3):
				nowKey		= -3;
				nowScale	= 0;
				SetKey(nowKey, nowScale);
				break;
			case(ks_f4):
				nowKey		= -4;
				nowScale	= 0;
				SetKey(nowKey, nowScale);
				break;
			case(ks_f5):
				nowKey		= -5;
				nowScale	= 0;
				SetKey(nowKey, nowScale);
				break;
			case(ks_f6):
				nowKey		= -6;
				nowScale	= 0;
				SetKey(nowKey, nowScale);
				break;

			case(ks_m0):
				nowKey		= 0;
				nowScale	= 5;
				SetKey(nowKey, nowScale);
				break;
			case(ks_ms1):
				nowKey		= 1;
				nowScale	= 5;
				SetKey(nowKey, nowScale);
				break;
			case(ks_ms2):
				nowKey		= 2;
				nowScale	= 5;
				SetKey(nowKey, nowScale);
				break;
			case(ks_ms3):
				nowKey		= 3;
				nowScale	= 5;
				SetKey(nowKey, nowScale);
				break;
			case(ks_ms4):
				nowKey		= 4;
				nowScale	= 5;
				SetKey(nowKey, nowScale);
				break;
			case(ks_ms5):
				nowKey		= 5;
				nowScale	= 5;
				SetKey(nowKey, nowScale);
				break;
			case(ks_ms6):
				nowKey		= 6;
				nowScale	= 5;
				SetKey(nowKey, nowScale);
				break;
			case(ks_ms7):
				nowKey		= 7;
				nowScale	= 5;
				SetKey(nowKey, nowScale);
				break;
			case(ks_ms8):
				nowKey		= 8;
				nowScale	= 5;
				SetKey(nowKey, nowScale);
				break;
			case(ks_ms9):
				nowKey		= 9;
				nowScale	= 5;
				SetKey(nowKey, nowScale);
				break;



			case(ks_mf1):
				nowKey		= -1;
				nowScale	= 5;
				SetKey(nowKey, nowScale);
				break;
			case(ks_mf2):
				nowKey		= -2;
				nowScale	= 5;
				SetKey(nowKey, nowScale);
				break;
			case(ks_mf3):
				nowKey		= -3;
				nowScale	= 5;
				SetKey(nowKey, nowScale);
				break;
			case(ks_mf4):
				nowKey		= -4;
				nowScale	= 5;
				SetKey(nowKey, nowScale);
				break;
			case(ks_mf5):
				nowKey		= -5;
				nowScale	= 5;
				SetKey(nowKey, nowScale);
				break;
			case(ks_mf6):
				nowKey		= -6;
				nowScale	= 5;
				SetKey(nowKey, nowScale);
				break;

			default:
				MML->Err(_T("���� K{} �R�}���h�̈����Ŗ��m�̕������w�肳��܂����B"));
				break;
		}
	}

	//for Debug
/*
	_COUT << _T("[0] c = ") << (int)KeySignature[0] << endl;
	_COUT << _T("[1] d = ") << (int)KeySignature[1] << endl;
	_COUT << _T("[2] e = ") << (int)KeySignature[2] << endl;
	_COUT << _T("[3] f = ") << (int)KeySignature[3] << endl;
	_COUT << _T("[4] g = ") << (int)KeySignature[4] << endl;
	_COUT << _T("[5] a = ") << (int)KeySignature[5] << endl;
	_COUT << _T("[6] b = ") << (int)KeySignature[6] << endl;
*/
}

//==============================================================
//		�ȗ����̉���
//--------------------------------------------------------------
//	������
//		MMLfile*	MML		MML�t�@�C���̃I�u�W�F�N�g
//	���Ԓl
//		����
//==============================================================
void	MusicTrack::SetLength(MMLfile* MML)
{
	nsd.length			= MML->GetLength(nsd.length);
	opt_DefaultLength	= nsd.length;
	mml_general*		_event;

	switch(nsd.length){
		case(96):
			_event = new mml_general(nsd_Length_96, _T("Length 96"));
			break;
		case(72):
			_event = new mml_general(nsd_Length_72, _T("Length 72"));
			break;
		case(48):
			_event = new mml_general(nsd_Length_48, _T("Length 48"));
			break;
		case(36):
			_event = new mml_general(nsd_Length_36, _T("Length 36"));
			break;
		case(32):
			_event = new mml_general(nsd_Length_32, _T("Length 32"));
			break;
		case(24):
			_event = new mml_general(nsd_Length_24, _T("Length 24"));
			break;
		case(18):
			_event = new mml_general(nsd_Length_18, _T("Length 18"));
			break;
		case(16):
			_event = new mml_general(nsd_Length_16, _T("Length 16"));
			break;
		case(12):
			_event = new mml_general(nsd_Length_12, _T("Length 12"));
			break;
		case(9):
			_event = new mml_general(nsd_Length_9,  _T("Length 9"));
			break;
		case(8):
			_event = new mml_general(nsd_Length_8,  _T("Length 8"));
			break;
		case(6):
			_event = new mml_general(nsd_Length_6,  _T("Length 6"));
			break;
		case(4):
			_event = new mml_general(nsd_Length_4,  _T("Length 4"));
			break;
		case(3):
			_event = new mml_general(nsd_Length_3,  _T("Length 3"));
			break;
		case(2):
			_event = new mml_general(nsd_Length_2,  _T("Length 2"));
			break;
		case(1):
			_event = new mml_general(nsd_Length_1,  _T("Length 1"));
			break;
		default:
			_event = new mml_general(nsd_Length, (unsigned char)nsd.length, _T("Length"));
			break;
	}
	SetEvent(_event);
}

//==============================================================
//		�m�[�g�i���o�[�E�Վ��L���̉���
//--------------------------------------------------------------
//	������
//		MMLfile*	MML		MML�t�@�C���̃I�u�W�F�N�g
//		int			nn		�m�[�g�i0:C 1:D 2:E �c 7:B�j
//	���Ԓl
//		char
//==============================================================
char	MusicTrack::calc_note(MMLfile*	MML,int note)
{
	unsigned		char	cData = MML->GetChar();
	static	const	char	note_code[]={0,2,4,5,7,9,11};
					char	_key = note_code[note];

	//�Վ��L��
	//If Natural then skip
	if((cData != '=') && (cData != '*')){
		//�i�`����������Ȃ��ꍇ
		_key += KeySignature[note];
		while((cData == '#') || (cData == '+') || (cData == '-')){
			switch(cData){
				case('#'):
				case('+'):
					_key++;
					break;
				case('-'):
					_key--;
					break;
				default:
					break;
			}
			cData = MML->GetChar();	//���̕���
		}
		//�Վ��L���ł͂Ȃ��Ȃ����̂ŁA�|�C���^��߂��B
		MML->Back();
	}
	return(_key);
}

//==============================================================
//			�����v�Z
//--------------------------------------------------------------
//	������
//		MMLfile*	MML		MML�t�@�C���̃I�u�W�F�N�g
//	���Ԓl
//				����
//==============================================================
int		MusicTrack::calc_length(MMLfile* MML)
{
	unsigned	char	cData = MML->GetChar();
				int		Length;

	MML->Back();
	if(((cData >= '0') && (cData <= '9')) || (cData == '%') || (cData == '.') || (cData == '~') || ((MML->iTieMode == 1) && (cData == '^'))){
		Length = MML->GetLength(nsd.length);
	} else {
		Length = -1;
	}
	return(Length);
}

//==============================================================
//			�N�I���^�C�Y�v�Z
//--------------------------------------------------------------
//	������
//		MMLfile*	MML		MML�t�@�C���̃I�u�W�F�N�g
//	���Ԓl
//				����
//==============================================================
int		MusicTrack::calc_gate(MMLfile* MML)
{
	unsigned	char	cData = MML->GetChar();
				int		GateTime;

	//�Q�[�g�^�C��
	if(cData == ','){
		cData = MML->GetChar();
		if(cData == '0'){
			GateTime = 0;
		} else {
			MML->Back();
			GateTime = MML->GetLength(nsd.length);
		}
	} else {
		MML->Back();
		GateTime = -1;
	}
	return(GateTime);
}

//==============================================================
//			�X���[�v�Z
//--------------------------------------------------------------
//	������
//		MMLfile*	MML		MML�t�@�C���̃I�u�W�F�N�g
//	���Ԓl
//				����
//==============================================================
bool	MusicTrack::calc_slur(MMLfile* MML)
{
	unsigned	char	cData = MML->GetChar();
				bool	slur;

	if(cData == '&') {
		slur = true;
	} else {
		slur = false;
		MML->Back();
	}
	return(slur);
}

//==============================================================
//		EC*	�^���G�R�[����
//--------------------------------------------------------------
//	������
//		MMLfile*	MML		MML�t�@�C���̃I�u�W�F�N�g
//	���Ԓl
//				����
//==============================================================
void	MusicTrack::SetEcho(void)
{
	//�����ŉ��ʂ�߂��B
	EchoVolRet();

	echo_flag = false;
}

//==============================================================
//		EC	�^���G�R�[�ݒ�
//--------------------------------------------------------------
//	������
//		MMLfile*	MML		MML�t�@�C���̃I�u�W�F�N�g
//	���Ԓl
//				����
//==============================================================
void	MusicTrack::SetEcho(MMLfile* MML)
{
	unsigned	char	cData;
				int		_value;
				int		_volume;

	//�����ŉ��ʂ�߂��B
	EchoVolRet();

	_value = MML->GetInt_With_Chk_Range(_T("EC�R�}���h�̑�P�p�����[�^"),0,255);

	MML->Chk_Comma();
	_volume = MML->GetInt_With_Chk_Range(_T("EC�R�}���h�̑�Q�p�����[�^"),-1,15);

	echo_flag = true;
	echo_value	= (unsigned char)_value;
	if(_volume == -1){
		echo_slur = true;
	} else {
		echo_slur = false;
		echo_volume	= (unsigned char)_volume;
	}

	cData = MML->GetChar();
	if(cData != ','){
		MML->Back();
		echo_length = -1;
	} else {
		echo_length	= MML->GetLength(nsd.length);
	}
}

//==============================================================
//		�^���G�R�[�̃o�b�t�@�ɏ�������
//--------------------------------------------------------------
//	������
//				MMLfile*	MML
//				int			note	-1 (r), 0(C) ~ 7(B)
//	���Ԓl
//				����
//==============================================================
void	MusicTrack::SetEchoBuffer(MMLfile* MML,int note)
{
	int		iEchoNote;

	if(note == -1){
		//�x�����������ޏꍇ
		iEchoNote = -1;

	} else {
		//�m�[�g���������ޏꍇ
		unsigned	char	cData;
					int		iOctave;

		iEchoNote = calc_note(MML, note);

		cData = MML->GetChar();
		MML->Back();

		if((cData >= '0') && (cData <= '9')){
			iOctave = MML->GetInt() - 1;
		} else {
			iOctave = nsd.octave;
		}
		iEchoNote += iKeyShift;
		iEchoNote += iOctave*12;
		MML->Chk_Range(_T("�m�[�g�i���o�["),0,255,iEchoNote);
	}

	pt_oldNote++;
	oldNote[pt_oldNote]	= (char)iEchoNote;
}

//==============================================================
//		�^���G�R�[�̃��Z�b�g
//--------------------------------------------------------------
//	������
//				����
//	���Ԓl
//				����
//==============================================================
void	MusicTrack::ResetEcho()
{

	for(size_t i=0; i<256; i++){
		oldNote[i] = -1;
	}

//	echo_flag	= false;
	pt_oldNote	= 0;
}

//==============================================================
//	�^���G�R�[�̐���
//--------------------------------------------------------------
//	������
//		int		Length
//		int		GateTime
//		bool	Slur
//	���Ԓl
//				����
//==============================================================
void	MusicTrack::GenerateEcho(MMLfile* MML, int Length, int GateTime, bool	Slur)
{
	char	old_note = oldNote[(pt_oldNote - echo_value) & 0xFF];
//	char	now_note = oldNote[pt_oldNote];

	char	now_octave = (char)nsd.octave;
	char	old_octave = (old_note / 12);

//	int		i = 0;

	CallPatch(MML, old_note);

	if(echo_slur == false){
		//Echo volume
		SetEvent(new mml_general(nsd_Volume + echo_volume, _T("Echo Volume")));
		echo_vol_ret = true;
	} else {
		//�O�̃m�[�g���A�����̏ꍇ�̂�
		if(echo_already == false){
			_old_note->SetTai();
		}
	}

	//Echo note
	if(old_octave < now_octave){
		while(old_octave < now_octave){
			old_octave++;
			SetEvent(new mml_general(nsd_Octave_Down_1, _T("One time octave down")));
		}
	} else if(old_octave > now_octave){
		while(old_octave > now_octave){
			old_octave--;
			SetEvent(new mml_general(nsd_Octave_Up_1, _T("One time octave up")));
		}
	}

	if(Length == opt_DefaultLength){
		Length = -1;
	}
	_old_note = new mml_note(old_note % 12, Length, GateTime, Slur, _T("Echo Note"));
	SetEvent(_old_note);

//	EchoVolRet();

	echo_already	= true;

}

//==============================================================
//		�^���G�R�[�̉��ʕ��A
//--------------------------------------------------------------
//	������
//				����
//	���Ԓl
//				����
//==============================================================
void	MusicTrack::EchoVolRet()
{
	//�����ŉ��ʂ�߂��B
	if(echo_vol_ret == true){
		//volume return
		SetEvent(new mml_general(nsd_Volume + (char)nsd.volume, _T("Volume")));
		echo_vol_ret = false;
	}
}

//==============================================================
//		�����̃C�x���g�쐬
//--------------------------------------------------------------
//	������
//		int		_key		�m�[�g�i���o�[
//		int		Length		����
//		int		GateTime	�Q�[�g�^�C��
//		bool	Slur		�X���[
//	���Ԓl
//				����
//==============================================================
void	MusicTrack::SetNote(MMLfile* MML, int _key, int Length, int GateTime, bool Slur)
{
	int		Length_0	= Length;		//������
	char	_note_no;

	//�������̎擾
	if(Length_0 == -1){
		Length_0 = nsd.length;
	}

	//�N�I���^�C�Y
	if(gatetime_Q != QMax){
		Set_q((Length_0 * (QMax - gatetime_Q)) / QMax);
	}

	//�ڒ�
	_key	+= iKeyShift;
	_note_no = (char)(_key + (nsd.octave + nsd.octave1)*12) & 0xFF;

	//�^���G�R�[�̃o�b�t�@��������
	pt_oldNote++;
	oldNote[pt_oldNote]	= _note_no;
	nsd.octave1			= 0;

	echo_already		= false;

	//�����ŉ��ʂ�߂��B
	if(echo_flag == true){
		EchoVolRet();
	}

	//�w��ł���͈͂𒴂����ꍇ�B
	while(_key < 0){
		_key += 12;
		SetEvent(new mml_general(nsd_Octave_Down_1, _T("One time octave down")));
	}

	while(_key >= 12){
		_key -= 12;
		SetEvent(new mml_general(nsd_Octave_Up_1, _T("One time octave up")));
	}

	CallPatch(MML, _note_no);

	if((echo_flag == true) && (oldNote[(pt_oldNote - echo_value) & 0xFF] != -1) && (echo_length != -1) && (Length_0 > echo_length) && (Slur == false)){
		//�^���G�R�[ ����

		Length_0 -= echo_length;
		if(Length_0 == opt_DefaultLength){
			Length_0 = -1;
		}

		_old_note = new mml_note(_key, Length_0, GateTime, Slur, _T("Note"));

		//�C�x���g�I�u�W�F�N�g�̍쐬
		SetEvent(_old_note);

		GenerateEcho(MML,echo_length,GateTime,Slur);
	} else {
		if(Length == opt_DefaultLength){
			Length = -1;
		}
		//�^���G�R�[ �Ȃ�
		_old_note = new mml_note(_key, Length, GateTime, Slur, _T("Note"));

		//�C�x���g�I�u�W�F�N�g�̍쐬
		SetEvent(_old_note);

	}
}

//==============================================================
//		�����̃C�x���g�쐬
//--------------------------------------------------------------
//	������
//		MMLfile*	MML		MML�t�@�C���̃I�u�W�F�N�g
//		int			note	�m�[�g�i0:C 1:D 2:E �c 7:B�j
//	���Ԓl
//				����
//==============================================================
void	MusicTrack::SetNote(MMLfile*	MML,int note)
{
	int		_key		= calc_note(MML, note);
	int		Length		= calc_length(MML);
	int		GateTime	= calc_gate(MML);
	bool	Slur		= calc_slur(MML);

	if(jump_flag==false){
		SetNote(MML, _key, Length, GateTime, Slur);
	}

}

//==============================================================
//		�x���̃C�x���g�쐬
//--------------------------------------------------------------
//	������
//		MMLfile*	MML		MML�t�@�C���̃I�u�W�F�N�g
//		int			mode	���[�h
//	���Ԓl
//				����
//==============================================================
void	MusicTrack::SetRest(MMLfile*	MML, int mode)
{
					char	old_note = oldNote[(pt_oldNote - echo_value) & 0xFF];

	unsigned		char	cData;
	unsigned		char	_code = 0x0F;
					int		Length = -1;
					int		GateTime = -1;
					bool	Slur = false;


	//�x���̃��[�h
	cData = MML->GetChar();
	switch(cData){
		case('-'):
			_code = 0x0D;	//mode 0
			break;
		case('#'):
		case('+'):
			_code = 0x0E;	//mode 1
			break;
		case('='):
			_code = 0x0F;	//mode 2
			break;
		case('*'):
			_code = 0x0C;	//mode 3
			break;
		default:
			MML->Back();
			if(mode & 0x80){
				switch(mode){
					case(0x80):
						_code = 0x0D;
						break;
					case(0x81):
						_code = 0x0E;
						break;
					case(0x82):
						_code = 0x0F;
						break;
					case(0x83):
						_code = 0x0C;
						break;
					default:
						MML->Err(_T("MusicTrack::SetRest()�֐����ŁA#wait�R�}���h��mode�̒l�����m�̒l�ł��B"));
						break;
				}
			} else {
				switch(KeySignature[7]){
					case(-1):
						_code = 0x0D;
						break;
					case(+1):
						_code = 0x0E;
						break;
					default:
						switch(mode){
							case(0):
								_code = 0x0D;
								break;
							case(1):
								_code = 0x0E;
								break;
							case(2):
								_code = 0x0F;
								break;
							case(3):
								_code = 0x0C;
								break;
							default:
								MML->Err(_T("MusicTrack::SetRest()�֐����ŁA#rest�R�}���h��mode�̒l�����m�̒l�ł��B"));
								break;
						}
						break;
				}
			}
			break;
	}

	Length		= calc_length(MML);		//����
	GateTime	= calc_gate(MML);		//�Q�[�g�^�C�� or �^�C
	Slur		= calc_slur(MML);		//�^�C�E�X���[

	if(jump_flag==false){
		//�N�I���^�C�Y
		if(gatetime_Q != QMax){
			Set_q((Length * (QMax - gatetime_Q)) / QMax);
		}

		if((echo_flag == false) || (_code != 0x0F) || (old_note == -1) || (echo_length != -1)){
			if(Length == opt_DefaultLength){
				Length = -1;
			}
			_old_note = new mml_note(_code, Length, GateTime, Slur, _T("Rest"));
			SetEvent(_old_note);
		} else {
			GenerateEcho(MML,Length,GateTime,Slur);
		}
	}
}

//==============================================================
//		�^�C�̃I�u�W�F�N�g�쐬�i�x���ō��j
//--------------------------------------------------------------
//	������
//		MMLfile*	MML		MML�t�@�C���̃I�u�W�F�N�g
//	���Ԓl
//		����
//==============================================================
void	MusicTrack::SetTai(MMLfile* MML)
{
	if(jump_flag==false){
		_old_note->SetTai();
	}
	SetRest(MML,2);
}

//==============================================================
//			�|���^�����g�i�ȑf�Łj
//--------------------------------------------------------------
//	������
//		MMLfile*	MML		MML�t�@�C���̃I�u�W�F�N�g
//		char		iTempo	�e���|
//	���Ԓl
//		����
//==============================================================
void	MusicTrack::SetProtament(MMLfile* MML, unsigned char iTempo)
{
	unsigned	char	cData;

				int		o_rel	= 0;
				int		o_rel2	= 0;

				int		iNote_2;	//�m�[�g�i���o�[
				int		note_2;		//����code
				int		_key_2;		//����
				int		Length_2;	//����
				int		GateTime_2;	//�Q�[�g�^�C��
				bool	Slur_2;		//�X���[

				int		iNote;		//�m�[�g�i���o�[
				int		note;		//����code
				int		_key;		//����
				int		Length;		//����
				int		GateTime;	//�Q�[�g�^�C��
				bool	Slur;		//�X���[

	//------
	//�|���^�����g���̉���
	cData = MML->GetChar();
	switch(cData){
		case('c'):
			note	= 0;
			break;
		case('d'):
			note	= 1;
			break;
		case('e'):
			note	= 2;
			break;
		case('f'):
			note	= 3;
			break;
		case('g'):
			note	= 4;
			break;
		case('a'):
			note	= 5;
			break;
		case('b'):
			note	= 6;
			break;
		default:
			MML->Err(_T("a,b,c�`g�̉������L�q���Ă��������B"));
			break;
	}

	_key		= calc_note(MML, note);
	Length		= calc_length(MML);
	GateTime	= calc_gate(MML);
	Slur		= calc_slur(MML);

	//�m�[�g�i���o�[�̎擾
	iNote=		((char)_key + (nsd.octave + nsd.octave1)*12) & 0xFF;

	//------
	//�I�N�^�[�u����
	cData = MML->GetChar();
	while((cData == '>') || (cData == '<') || (cData == '`') || (cData == '"')){
		switch(cData){
			case('<'):
				o_rel--;
				break;
			case('>'):
				o_rel++;
				break;
			case('`'):
				o_rel2++;
				break;
			case('"'):
				o_rel2--;
				break;
			default:
				break;
		}
		cData = MML->GetChar();	//���̕���
	}
	//�Վ��L���ł͂Ȃ��Ȃ����̂ŁA�|�C���^��߂��B
	MML->Back();

	//------
	//�|���^�����g��̉���
	cData = MML->GetChar();
	switch(cData){
		case('c'):
			note_2	= 0;
			break;
		case('d'):
			note_2	= 1;
			break;
		case('e'):
			note_2	= 2;
			break;
		case('f'):
			note_2	= 3;
			break;
		case('g'):
			note_2	= 4;
			break;
		case('a'):
			note_2	= 5;
			break;
		case('b'):
			note_2	= 6;
			break;
		default:
			MML->Err(_T("a,b,c�`g�̉������L�q���Ă��������B"));
			break;
	}

	_key_2		= calc_note(MML, note_2);
	iNote_2		= ((char)_key_2 + (nsd.octave + nsd.octave1 + o_rel + o_rel2)*12) & 0xFF;

	//------
	//�|���^�����g�I��
	cData = MML->GetChar();
	if(cData != '}'){
		MML->Err(_T("�|���^�����g�R�}���h�́A}�ŕ��ĉ������B"));
	}

	Length_2	= calc_length(MML);
	if(Length_2 != -1){
		if(Length != -1){
			MML->Err(_T("�|���^�����g�R�}���h�ŁA������2�ӏ��ɋL�q����Ă��܂��B"));
		} else {
			Length = Length_2;
		}
	}

	GateTime_2	= calc_gate(MML);
	if(GateTime_2 != -1){
		if(GateTime != -1){
			MML->Err(_T("�|���^�����g�R�}���h�ŁA�Q�[�g�^�C����2�ӏ��ɋL�q����Ă��܂��B"));
		} else {
			GateTime = GateTime_2;
		}
	}

	Slur_2		= calc_slur(MML);
	if(Slur_2 == true){
		Slur = true;
	}

	//------
	//�|���^�����g�̌v�Z
	int		Length_0	= Length;		//������

	int	pol_length;
	int pol_target;
	int	pol_rate;
	int	pol_depth;

	//�������̎擾
	if(Length_0 == -1){
		Length_0 = nsd.length;
	}

	pol_length	= (Length_0 * 150) / (iTempo);
	pol_target	= iNote_2 - iNote;

	if(abs(pol_target) * 16 >= pol_length){
		pol_rate	= 1;
		pol_depth	= (pol_target * 16) / pol_length;
	//	if((pol_depth * pol_length) != (pol_target * 16)){
			if(pol_target>=0){
				pol_depth++;
			} else {
				pol_depth--;
			}
	//	}
	} else {
		if(pol_target < 0){
			pol_rate	= pol_length / (-pol_target * 16);
			pol_depth	= -1;
		} else {
			pol_rate	= pol_length / (+pol_target * 16);
			pol_depth	= 1;
		}
	}

//	printf("length :%d\n", pol_length);
//	printf("Rate   :%d\n", pol_rate);
//	printf("Depth  :%d\n", pol_depth);
//	printf("Target :%d\n", pol_target);

	//------
	//�I�u�W�F�N�g�̏�������
	if(jump_flag == false){

		//�|���^�����g
		SetEvent(new mml_general(nsd_Portamento, (unsigned char)1, (unsigned char)pol_rate, (unsigned char)pol_depth, (unsigned char)pol_target, _T("Portamento")));

		//�|���^�����g���̉���
		SetNote(MML, _key, Length, GateTime, Slur);
	}

	//�I�N�^�[�u�R�}���h�̏����o��
	while( o_rel > 0){
		if(MML->octave_reverse == true){
			SetOctaveDec();
		} else {
			SetOctaveInc();
		}
		o_rel--;
	}
	while( o_rel < 0){
		if(MML->octave_reverse == true){
			SetOctaveInc();
		} else {
			SetOctaveDec();
		}
		o_rel++;
	}

}

//==============================================================
//			�|���^�����g�iP�R�}���h�Łj
//--------------------------------------------------------------
//	������
//		MMLfile*	MML		MML�t�@�C���̃I�u�W�F�N�g
//	���Ԓl
//		����
//==============================================================
void	MusicTrack::SetProtament(MMLfile* MML)
{
	int		_Decay;
	int		_Rate;
	int		_Depth;
	int		_Target;

	_Decay = MML->GetInt_With_Chk_Range(_T("�|���^�����g�̑�1�p�����[�^"),0,255);
	_Decay++;

	MML->Chk_Comma();
	_Rate = MML->GetInt_With_Chk_Range(_T("�|���^�����g�̑�2�p�����[�^"),1,256);

	MML->Chk_Comma();
	_Depth = MML->GetInt_With_Chk_Range(_T("�|���^�����g�̑�3�p�����[�^"),-128,127);
//	_Decay++;

	MML->Chk_Comma();
	_Target = MML->GetInt_With_Chk_Range(_T("�|���^�����g�̑�4�p�����[�^"),-128,127);

	if(jump_flag == false){
        SetEvent(new mml_general(nsd_Portamento, (unsigned char)_Decay, (unsigned char)_Rate, (unsigned char)_Depth, (unsigned char)_Target, _T("Portamento")));
	}
}

//==============================================================
//		�ڒ��Q
//--------------------------------------------------------------
//	������
//		MMLfile*	MML		MML�t�@�C���̃I�u�W�F�N�g
//	���Ԓl
//		����
//==============================================================
void	MusicTrack::SetKeyShift(MMLfile* MML)
{
	iKeyShift = MML->GetInt_With_Chk_Range(_T("�L�[�V�t�g"),-128,127);
}

//==============================================================
//		�ڒ��Q
//--------------------------------------------------------------
//	������
//		MMLfile*	MML		MML�t�@�C���̃I�u�W�F�N�g
//	���Ԓl
//		����
//==============================================================
void	MusicTrack::SetKeyShift_Relative(MMLfile* MML)
{
	iKeyShift += MML->GetInt();
	MML->Chk_Range(_T("�L�[�V�t�g�͈̔�"),-128,127,iKeyShift);
}

//==============================================================
//		�ڒ��P
//--------------------------------------------------------------
//	������
//		MMLfile*	MML		MML�t�@�C���̃I�u�W�F�N�g
//	���Ԓl
//		����
//==============================================================
void	MusicTrack::SetTranspose(int _no)
{
	nsd.trans	= _no;
	SetEvent(new mml_general(nsd_Transpose, (char)nsd.trans, _T("Transpose")));
}

//==============================================================
//		�ڒ��P
//--------------------------------------------------------------
//	������
//		MMLfile*	MML		MML�t�@�C���̃I�u�W�F�N�g
//	���Ԓl
//		����
//==============================================================
void	MusicTrack::SetTranspose_Relative(int _no)
{
	nsd.trans += _no;
	SetEvent(new mml_general(nsd_Relative_Transpose, (char)_no, _T("Relative Transpose")));
}

//==============================================================
//		�I�N�^�[�u
//--------------------------------------------------------------
//	������
//		MMLfile*	MML		MML�t�@�C���̃I�u�W�F�N�g
//	���Ԓl
//		����
//==============================================================
void	MusicTrack::SetOctave(MMLfile* MML)
{
	nsd.octave = (char)(MML->GetInt_With_Chk_Range(_T("�I�N�^�[�u"),1,8) - 1);

	SetEvent(new mml_general(nsd_Octave + (unsigned char)nsd.octave, _T("Octave")));
}

//--------------------------------------------------------------
void	MusicTrack::SetOctaveInc()
{
	SetEvent(new mml_general(nsd_Octave_Up, _T("Octave Up")));
	if(nsd.octave<10){
		nsd.octave++;
	}
}

//--------------------------------------------------------------
void	MusicTrack::SetOctaveDec()
{
	SetEvent(new mml_general(nsd_Octave_Down, _T("Octave Down")));
	if(nsd.octave>0){
		nsd.octave--;
	}
}

//--------------------------------------------------------------
void	MusicTrack::SetOctaveOne_Inc()
{
	if(jump_flag == false){
		SetEvent(new mml_general(nsd_Octave_Up_1, _T("One time octave up")));
		nsd.octave1++;
	}
}

//--------------------------------------------------------------
void	MusicTrack::SetOctaveOne_Dec()
{
	if(jump_flag == false){
		SetEvent(new mml_general(nsd_Octave_Down_1, _T("One time octave down")));
		nsd.octave1--;
	}
}

//==============================================================
//		����
//--------------------------------------------------------------
//	������
//		MMLfile*	MML		MML�t�@�C���̃I�u�W�F�N�g
//	���Ԓl
//		����
//==============================================================
void	MusicTrack::SetVolume(MMLfile* MML)
{
	nsd.volume		= (char)MML->GetInt_With_Chk_Range(_T("����"),0,15);
	echo_vol_ret	= false;
	SetEvent(new mml_general(nsd_Volume + (char)nsd.volume, _T("Volume")));
}

//------
void	MusicTrack::SetVolumeInc(MMLfile* MML)
{
	unsigned	char	cData = MML->GetChar();
				int		iValue;

	if((cData >= '0') && (cData <= '9')){
		MML->Back();
		iValue = MML->GetInt_With_Chk_Range(_T("���Ή���"),0,15);
	} else {
		MML->Back();
		iValue = 1;
	}

	EchoVolRet();

	while(iValue > 0){
		SetEvent(new mml_general(nsd_Volume_Up, _T("Volume up")));
		nsd.volume++;
		iValue--;
	}
	if(nsd.volume>15){
		nsd.volume = 15;
	}
}

//------
void	MusicTrack::SetVolumeDec(MMLfile* MML)
{
	unsigned	char	cData = MML->GetChar();
				int		iValue;

	if((cData >= '0') && (cData <= '9')){
		MML->Back();
		iValue = MML->GetInt_With_Chk_Range(_T("���Ή���"),0,15);
	} else {
		MML->Back();
		iValue = 1;
	}

	EchoVolRet();

	while(iValue > 0){
		SetEvent(new mml_general(nsd_Volume_Down, _T("Volume down")));
		nsd.volume--;
		iValue--;
	}
	if(nsd.volume<0){
		nsd.volume = 0;
	}
}
