;=======================================================================|
;									|
;		NES Sound Driver & library				|
;				define file				|
;					for Assemble Language (ca65)	|
;									|
;					Programmed by			|
;						A.Watanabe		|
;									|
;=======================================================================|

;****************************************************************
;*		Function of Library				*
;****************************************************************
;-----------------------------------------------
;	void	__fastcall__	nsd_main(void);
;-----------------------------------------------
;	Summary :		main routine of this sound driver
;	Arguments :		None
;	Return :		None
;	Modifies :		A X Y
;	Useage :		Call by 60[Hz].  e.g. V-Blank
;-----------------------------------------------
	.import		_nsd_main


;-----------------------------------------------
;	void	__fastcall__	nsd_init(void);
;-----------------------------------------------
;	Summary :		Initraize this sound driver.
;	Arguments :		None
;	Return :		None
;	Modifies :		A X Y
;	Useage :		Call when start up.
;-----------------------------------------------
	.import		_nsd_init


;-----------------------------------------------
;	void	__fastcall__	nsd_set_dpcm(nsd_dpcm* dpcm);
;-----------------------------------------------
;	Summary :		Set the Delta PCM information.
;	Arguments :	AX	Pointer of the Delta PCM information.
;	Return :		None
;	Modifies :		A X Y
;-----------------------------------------------
	.import		_nsd_set_dpcm


;-----------------------------------------------
;	void	__fastcall__	nsd_play_bgm(void* ptr);
;-----------------------------------------------
;	Summary :		Play the BGM
;	Arguments :	AX	Pointer of the BGM data.
;	Return :		None
;	Modifies :		A X Y
;-----------------------------------------------
	.import		_nsd_play_bgm


;-----------------------------------------------
;	void	__fastcall__	nsd_stop_bgm(void);
;-----------------------------------------------
;	Summary :		Stop the BGM
;	Arguments :		None
;	Return :		None
;	Modifies :		A X Y
;-----------------------------------------------
	.import		_nsd_stop_bgm


;-----------------------------------------------
;	void	__fastcall__	nsd_pause_bgm(void);
;-----------------------------------------------
;	Summary :		Pause the BGM
;	Arguments :		None
;	Return :		None
;	Modifies :		A X Y
;-----------------------------------------------
	.import		_nsd_pause_bgm


;-----------------------------------------------
;	void	__fastcall__	nsd_resume_bgm(void);
;-----------------------------------------------
;	Summary :		Resume the BGM
;	Arguments :		None
;	Return :		None
;	Modifies :		A X Y
;-----------------------------------------------
	.import		_nsd_resume_bgm


;-----------------------------------------------
;	void	__fastcall__	nsd_play_se(void* ptr);
;-----------------------------------------------
;	Summary :		Play the SE
;	Arguments :	AX	Pointer of the SE data.
;	Return :		None
;	Modifies :		A X Y
;-----------------------------------------------
	.import		_nsd_play_se


;-----------------------------------------------
;	void	__fastcall__	nsd_stop_se(void);
;-----------------------------------------------
;	Summary :		Stop the SE
;	Arguments :		None
;	Return :		None
;	Modifies :		A X Y
;-----------------------------------------------
	.import		_nsd_stop_se



;-----------------------------------------------
;	Work area of nsd.lib
;-----------------------------------------------
	.importzp	nsd_work_zp
	.import		nsd_work





;****************************************************************
;*		Define for nsd.lib				*
;****************************************************************

;=======================================================================
;	General Define
;-----------------------------------------------------------------------

.scope	nsd
	APU_Track	= 5			;

	TR_FDS		= APU_Track * 2
	.ifdef	FDS
		FDS_Track	= 1		;
	.else
		FDS_Track	= 0		;
	.endif

	TR_VRC6		= TR_FDS + FDS_Track * 2
	.ifdef	VRC6
		VRC6_Track	= 3		;
	.else
		VRC6_Track	= 0		;
	.endif

	TR_VRC7		= TR_VRC6 + VRC6_Track * 2
	.ifdef	VRC7
		VRC7_Track	= 6		;
	.else
		VRC7_Track	= 0		;
	.endif

	TR_OPLL		= TR_VRC7 + VRC7_Track * 2
	.ifdef	OPLL
		OPLL_Track_FM		= 9
		OPLL_Track_Rhythm	= 5
		OPLL_Track	= OPLL_Track_FM + OPLL_Track_Rhythm
	.else
		OPLL_Track	= 0		;
	.endif

	TR_MMC5		= TR_OPLL + OPLL_Track * 2
	.ifdef	MMC5
		MMC5_Track	= 2		; (without DPCM)
	.else
		MMC5_Track	= 0		;
	.endif

	TR_N163		= TR_MMC5 + MMC5_Track * 2
	.ifdef	N163
		N163_Track	= 8		;
	.else
		N163_Track	= 0		;
	.endif

	TR_PSG		= TR_N163 + N163_Track * 2
	.ifdef	PSG
		PSG_Track	= 3		;
	.else
		PSG_Track	= 0		;
	.endif

	TR_NULL		= TR_PSG + PSG_Track * 2
	.ifdef	NULL
		NULL_Track	= 1		;
	.else
		NULL_Track	= 0		;
	.endif

	BGM_Track	= APU_Track + FDS_Track + VRC6_Track + VRC7_Track + OPLL_Track + MMC5_Track + N163_Track + PSG_Track + NULL_Track
	SE_Track	= 2		;

	Track		= BGM_Track + SE_Track

	TR_BGM1		= 0
	TR_BGM2		= 2
	TR_BGM3		= 4
	TR_BGM4		= 6
	TR_BGM5		= 8
	TR_SE1		= BGM_Track * 2 + 0
	TR_SE2		= BGM_Track * 2 + 2

.endscope





;=======================================================================
;	Working Struct define in Zero-page area
;-----------------------------------------------------------------------

.Struct		NSD_Envelop
	V		.byte		;Volume & Voice (ch3:Tempo / ch5:Tempo_ctr)
	F		.byte		;Frequency & Note
.Endstruct

.Struct		NSD_Length_Cnt
	counter		.byte		;now length of note
	gate		.byte		;length - u
.Endstruct

.Struct		NSD_work_zp
	_ptr		.word					;00 General pointer value
	_tmp		.word					;02 General value
	channel		.byte					;04 channel (x resister)
;	flag		.byte					;05 flag
	Sequence_ptr	.word			nsd::Track	;06 Address of playing sequence
.ifndef	HFX4
	LengthCnt	.tag	NSD_Length_Cnt	nsd::Track	;16 Length Counter
.endif
	Envelop		.tag	NSD_Envelop	nsd::Track	;26 Envelop counter
.Endstruct

; 6 + ( 6 [Byte] * 28 [ch] ) = 174

;---------------------------------------
;	Defines for work structures

.scope	nsd_flag
	BGM		= $01
	SE		= $02
	Priority	= $0C
	Disable		= $80
.endscope

;Zero Page Works
.define	__ptr		nsd_work_zp + NSD_work_zp::_ptr
.define	__tmp		nsd_work_zp + NSD_work_zp::_tmp
;.define	__flag		nsd_work_zp + NSD_work_zp::flag
.define	__channel	nsd_work_zp + NSD_work_zp::channel
.define	__Sequence_ptr	nsd_work_zp + NSD_work_zp::Sequence_ptr

.define	__Envelop_V	nsd_work_zp + NSD_work_zp::Envelop + NSD_Envelop::V	;upper 4bit Voice / lower 4bit:Volume
.define	__Envelop_F	nsd_work_zp + NSD_work_zp::Envelop + NSD_Envelop::F	;upper 4bit:Note  / lower 4bit:Frequency

.define	__flag		nsd_work_zp + NSD_work_zp::Envelop + NSD_Envelop::V + nsd::TR_BGM3
.define	__Tempo		nsd_work_zp + NSD_work_zp::Envelop + NSD_Envelop::F + nsd::TR_BGM5
.define	__Tempo_ctr	nsd_work_zp + NSD_work_zp::Envelop + NSD_Envelop::V + nsd::TR_BGM5





;=======================================================================
;	Working  Struct define in RAM area
;-----------------------------------------------------------------------

.Struct		NSD_Flag
	flag		.byte		;flag
	gatemode	.byte		;gate mode
.Endstruct

.Struct		NSD_length
	length		.byte		;`l' command value
	tai		.byte		;flag of Tai & Slur
.Endstruct

.Struct		NSD_gatetime
	q		.byte		;`q' command value
	u		.byte		;`u' command value
.Endstruct

.Struct		NSD_note
	note		.byte		;`note' command value (bit 7 = `H' : rest)
	octave		.byte		;`o' command value
.Endstruct

.Struct		NSD_Detune
	cent		.byte		;`D' command value
	fine		.byte		;`D%' command value
.Endstruct

.Struct		NSD_Por_Lv		;(ch5 : BGM1 & BGM2 Frequency)
	target		.byte		;`P' target
	depth		.byte		;`P' add value
.Endstruct

.Struct		NSD_Por_Co		;(ch5 : BGM3 & SE  Frequency)
	count		.byte		;rate control
	rate		.byte		;now depth
.Endstruct

.Struct		NSD_Trans
	trans		.byte		;`_' command value
	onetime		.byte		;`'', `"' command value
.Endstruct

.Struct		NSD_Repeat
	count1		.byte		;counter of repeat(A)
	count2		.byte		;counter of repeat(B)
.Endstruct

.Struct		NSD_volume
	volume		.byte		;`v' & `vR' command value
	volume_env	.byte		;now envelop volume	(ch3,5 : sweep value of ch1,2)
.Endstruct

.Struct		NSD_voice
	voice		.byte		;`@R' command value (upper 4bit)
	voice_set	.byte		;deveice setting value
.Endstruct

.Struct		NSD_Env_V_Ptr		;(ch5 : pointer of ⊿PCM information)
	Volume		.byte		;Volume
	Voice		.byte		;Voice
.Endstruct

.Struct		NSD_Env_F_Ptr
	Frequency	.byte		;Freeuqncy
	Note		.byte		;Note
.Endstruct

.Struct		NSD_Env_F_Now
	Frequency	.byte		;now envelop Freeuqncy
	Note		.byte		;now envelop Note
.Endstruct

.Struct		NSD_work
	Flag		.tag	NSD_Flag	nsd::Track	;flag
.ifdef	HFX4
	LengthCnt	.tag	NSD_Length_Cnt	nsd::Track	;16 Length Counter
.endif
	Length		.tag	NSD_length	nsd::Track	;note length [tick]
	Gatetime	.tag	NSD_gatetime	nsd::Track	;gate time [tick]
	Note		.tag	NSD_note	nsd::Track	;octave and note command
	Detune		.tag	NSD_Detune	nsd::Track	;detune
	Por_Lv		.tag	NSD_Por_Lv	nsd::Track	;Portamento / TR5: BGM1 & BGM2 frequency
	Por_Co		.tag	NSD_Por_Co	nsd::Track	;Portamento / TR5: BGM3 & SE1  frequency
	Por_now		.word			nsd::Track	;Portamento
	Trans		.tag	NSD_Trans	nsd::Track	;Transpose
	Repeat		.tag	NSD_Repeat	nsd::Track	;Repeat
	Volume		.tag	NSD_volume	nsd::Track	;volume	/ TR3: BGM1 sweep / TR5: BGM2 sweep
	Voice		.tag	NSD_voice	nsd::Track	;voice
	Env_F_Now	.tag	NSD_Env_F_Now	nsd::Track	;Now value of envelop
	Env_V_Ptr	.tag	NSD_Env_V_Ptr	nsd::Track	;Address of envelop / TR5:DPCM info
	Env_F_Ptr	.tag	NSD_Env_F_Ptr	nsd::Track	;Address of envelop
	Envelop_Volume	.word			nsd::Track	;Pointer of envelop
	Envelop_Voice	.word			nsd::Track	;Pointer of envelop
	Envelop_Freq	.word			nsd::Track	;Pointer of envelop
	Envelop_Note	.word			nsd::Track	;Pointer of envelop
	Frequency	.word			nsd::Track	;Setting frequency
	SubRoutine	.word			nsd::Track	;Pointer of sub-routine return address
	Repeat2		.word			nsd::Track	;Pointer of repeat2 goto point

.ifdef	MMC5
	MMC5_Frequency	.byte			nsd::MMC5_Track	;MMC5 frequency
.endif

.ifdef	FDS
	FDS_Frequency	.byte					;FDS frequency
	FDS_SweepBias	.byte					;FDS Sweep bias
.endif

.ifdef	VRC7
	VRC7_Frequency	.byte			nsd::VRC7_Track	;VRC7 frequecny
	VRC7_Freq_old	.byte			nsd::VRC7_Track
	VRC7_Reg	.word					;
.endif

.ifdef	OPLL
	OPLL_Frequency	.byte			nsd::OPLL_Track_FM	;OPLL frequency (FM)
	OPLL_Freq_old	.byte			nsd::OPLL_Track_FM
	OPLL_Reg	.word		
	OPLL_Rhythm	.byte						;OPLL Rhythm
.endif

.ifdef	N163
	N163_num	.byte					;N163 ch number
	N163_Frequency	.byte			nsd::N163_Track	;N163 frequency
.endif

.ifdef	PSG
	PSG_switch	.byte					;PSG switch
.endif

.Endstruct



;---------------------------------------
;	Defines for work structures

.scope	nsd_chflag
	SE2	= $80
	SE1	= $40
	Sustain	= $20		;for VRC7
	KeyOn	= $10		;for VRC7
	Envelop	= $10		;for PSG
	FDSVOL	= $0C
	KeyOff	= $03		;00	Key Off	: Vol = 0
				;01	Key Off	: Release command
				;10	Key Off	:  envelop
				;11	Note On	: (envelop)
.endscope

.scope	nsd_mode
	voice		= $08
	gatemode	= $03
.endscope


;RAM Works
.ifdef	HFX4
.define	__Length_ctr	nsd_work + NSD_work::LengthCnt + NSD_Length_Cnt::counter
.define	__Gate		nsd_work + NSD_work::LengthCnt + NSD_Length_Cnt::gate
.else
.define	__Length_ctr	nsd_work_zp + NSD_work_zp::LengthCnt + NSD_Length_Cnt::counter
.define	__Gate		nsd_work_zp + NSD_work_zp::LengthCnt + NSD_Length_Cnt::gate
.endif

.define	__chflag	nsd_work + NSD_work::Flag + NSD_Flag::flag
.define	__gatemode	nsd_work + NSD_work::Flag + NSD_Flag::gatemode
.define	__tai		nsd_work + NSD_work::Length + NSD_length::tai
.define	__length	nsd_work + NSD_work::Length + NSD_length::length
.define	__gate_q	nsd_work + NSD_work::Gatetime + NSD_gatetime::q
.define	__gate_u	nsd_work + NSD_work::Gatetime + NSD_gatetime::u
.define __note		nsd_work + NSD_work::Note + NSD_note::note
.define	__octave	nsd_work + NSD_work::Note + NSD_note::octave
.define	__detune_cent	nsd_work + NSD_work::Detune + NSD_Detune::cent
.define	__detune_fine	nsd_work + NSD_work::Detune + NSD_Detune::fine
.define	__por_target	nsd_work + NSD_work::Por_Lv + NSD_Por_Lv::target
.define	__por_depth	nsd_work + NSD_work::Por_Lv + NSD_Por_Lv::depth
.define	__por_ctr	nsd_work + NSD_work::Por_Co + NSD_Por_Co::count
.define	__por_rate	nsd_work + NSD_work::Por_Co + NSD_Por_Co::rate
.define __por_now	nsd_work + NSD_work::Por_now
.define	__trans		nsd_work + NSD_work::Trans + NSD_Trans::trans
.define	__trans_one	nsd_work + NSD_work::Trans + NSD_Trans::onetime
.define	__repeat_ctr	nsd_work + NSD_work::Repeat + NSD_Repeat::count1
.define	__repeat_ctr2	nsd_work + NSD_work::Repeat + NSD_Repeat::count2
.define	__volume	nsd_work + NSD_work::Volume + NSD_volume::volume
.define	__voice		nsd_work + NSD_work::Voice + NSD_voice::voice
.define	__voice_set	nsd_work + NSD_work::Voice + NSD_voice::voice_set
.define	__frequency	nsd_work + NSD_work::Frequency
.define	__subroutine	nsd_work + NSD_work::SubRoutine
.define	__repeat2	nsd_work + NSD_work::Repeat2
.define	__env_volume	nsd_work + NSD_work::Envelop_Volume
.define	__env_voice	nsd_work + NSD_work::Envelop_Voice
.define	__env_frequency	nsd_work + NSD_work::Envelop_Freq
.define	__env_note	nsd_work + NSD_work::Envelop_Note
.define	__env_vol_ptr	nsd_work + NSD_work::Env_V_Ptr + NSD_Env_V_Ptr::Volume
.define	__env_voi_ptr	nsd_work + NSD_work::Env_V_Ptr + NSD_Env_V_Ptr::Voice
.define	__env_freq_ptr	nsd_work + NSD_work::Env_F_Ptr + NSD_Env_F_Ptr::Frequency
.define	__env_note_ptr	nsd_work + NSD_work::Env_F_Ptr + NSD_Env_F_Ptr::Note
;	voiceに関しては、その後の演算は無いので、更新されない場合はエンベロープ処理を終了する。
.define	__env_vol_now	nsd_work + NSD_work::Volume + NSD_volume::volume_env
.define	__env_freq_now	nsd_work + NSD_work::Env_F_Now + NSD_Env_F_Now::Frequency
.define	__env_note_now	nsd_work + NSD_work::Env_F_Now + NSD_Env_F_Now::Note

.define	__sweep_ch1	nsd_work + NSD_work::Volume + NSD_volume::volume_env + nsd::TR_BGM3
.define	__sweep_ch2	nsd_work + NSD_work::Volume + NSD_volume::volume_env + nsd::TR_BGM5


.define	__apu_frequency1	nsd_work + NSD_work::Por_Lv + nsd::TR_BGM5		;.byte
.define	__apu_frequency2	nsd_work + NSD_work::Por_Lv + nsd::TR_BGM5 + 1		;.byte
.define	__apu_frequency3	nsd_work + NSD_work::Por_Co + nsd::TR_BGM5		;.byte
.define	__se_frequency		nsd_work + NSD_work::Por_Co + nsd::TR_BGM5 + 1		;.byte
.define	__dpcm_info		nsd_work + NSD_work::Env_V_Ptr + nsd::TR_BGM5		;.word

.ifdef	MMC5
.define	__mmc5_frequency1	nsd_work + NSD_work::MMC5_Frequency + 0
.define	__mmc5_frequency2	nsd_work + NSD_work::MMC5_Frequency + 1
.endif

.ifdef	FDS
.define	__fds_frequency		nsd_work + NSD_work::FDS_Frequency
.define	__fds_sweepbias		nsd_work + NSD_work::FDS_SweepBias
.endif

.ifdef	VRC7
.define	__vrc7_frequency	nsd_work + NSD_work::VRC7_Frequency
.define	__vrc7_freq_old		nsd_work + NSD_work::VRC7_Freq_old
.define	__vrc7_reg		nsd_work + NSD_work::VRC7_Reg
.endif

.ifdef	OPLL
.define	__opll_frequency	nsd_work + NSD_work::OPLL_Frequency
.define	__opll_freq_old		nsd_work + NSD_work::OPLL_Freq_old
.define	__opll_reg		nsd_work + NSD_work::OPLL_Reg
.define	__opll_ryhthm		nsd_work + NSD_work::OPLL_Rhythm
.endif

.ifdef	N163
.define	__n163_num		nsd_work + NSD_work::N163_num
.define	__n163_frequency	nsd_work + NSD_work::N163_Frequency
.endif

.ifdef	PSG
.define	__psg_switch		nsd_work + NSD_work::PSG_switch
.endif





;=======================================================================
;		Struct of DPCM					*
;-----------------------------------------------------------------------
.Struct		nsd_dpcm
	Control		.byte		;I/O 0x4010
	DA		.byte		;I/O 0x4011
	Address		.byte		;I/O 0x4012
	Size		.byte		;I/O 0x4013
.ifdef	DPCMBank
	Bank		.byte		;Bank number
	Next		.byte		;Next number(note)
.endif
.Endstruct


