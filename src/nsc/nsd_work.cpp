/*******************************************************************************

			NES Sound Driver & Library	(NSD.lib)	MML Compiler

	Copyright (c) 2012 A.Watanabe (S.W.), All rights reserved.
	 For conditions of distribution and use, see copyright notice in "nsc.cpp".

*******************************************************************************/

#include "stdafx.h"
#include "nsd_work.h"

//==============================================================
//		コンストラクタ
//--------------------------------------------------------------
//	●引数
//					無し
//	●返値
//					無し
//==============================================================
NSD_WORK::NSD_WORK(void) {

	init();
};

//==============================================================
//		コンストラクタ
//--------------------------------------------------------------
//	●引数
//					無し
//	●返値
//					無し
//==============================================================
NSD_WORK::~NSD_WORK(void) {};

//==============================================================
//		コンストラクタ
//--------------------------------------------------------------
//	●引数
//					無し
//	●返値
//					無し
//==============================================================
void	NSD_WORK::init(void) {

	//音源ドライバ初期値を設定
	gatemode		= -1;
	length			= 24;
	gate_q			= 0;
	gate_u			= 0;
	octave			= 3;
	octave1			= 0;
	detune_cent		= 0;
	detune_reg		= 0;
	trans			= 0;
	voice			= -1;
	voice_rel		= -1;
	volume			= 15;
	volume_rel		= 2;
	sw_Evoi			= false;
	sw_Evol			= false;
	sw_Em			= false;
	sw_En			= false;
	sweep			= -1;
	sw_fds_career	= false;
	sw_fds_modlator	= false;
	fds_volume		= -1;
	fds_frequency	= -1;
	sw_vrc7_voice	= false;
	sw_n163_voice	= false;
	psg_switch		= -1;
	psg_frequency	= -1;
}

//==============================================================
//		コンストラクタ
//--------------------------------------------------------------
//	●引数
//		NSD_WORK*	work	コピー元
//	●返値
//					無し
//==============================================================
void	NSD_WORK::set(NSD_WORK* work) {

	gatemode		= work->gatemode;
	length			= work->length;
	gate_q			= work->gate_q;
	gate_u			= work->gate_u;
	octave			= work->octave;
	octave1			= work->octave1;
	detune_cent		= work->detune_cent;
	detune_reg		= work->detune_reg;
	trans			= work->trans;
	voice			= work->voice;
	voice_rel		= work->voice_rel;
	volume			= work->volume;
	volume_rel		= work->volume_rel;
	env_volume		= work->env_volume;
	env_voice		= work->env_voice;
	env_frequency	= work->env_frequency;
	env_note		= work->env_note;
	sw_Evoi			= work->sw_Evoi;
	sw_Evol			= work->sw_Evol;
	sw_Em			= work->sw_Em;
	sw_En			= work->sw_En;
	sweep			= work->sweep;
	fds_career		= work->fds_career;
	fds_modlator	= work->fds_modlator;
	sw_fds_career	= work->sw_fds_career;
	sw_fds_modlator	= work->sw_fds_modlator;
	fds_volume		= work->fds_volume;
	fds_frequency	= work->fds_frequency;
	vrc7_voice		= work->vrc7_voice;
	sw_vrc7_voice	= work->sw_vrc7_voice;
	n163_voice		= work->n163_voice;
	sw_n163_voice	= work->sw_n163_voice;
	psg_switch		= work->psg_switch;
	psg_frequency	= work->psg_frequency;
}


