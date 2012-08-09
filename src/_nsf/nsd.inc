
.scope	nsd
	BGM_Track	= 5
	SE_Track	= 2
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
	flag		.byte					;04 flag
	channel		.byte					;05 channel (y resister)
	Sequence_ptr	.word			nsd::Track	;06 Address of playing sequence
	Length		.tag	NSD_Length_Cnt	nsd::Track	;16 Length Counter
	Envelop		.tag	NSD_Envelop	nsd::Track	;26 Envelop counter
.Endstruct

;=======================================================================
;	Working  Struct define in RAM area
;-----------------------------------------------------------------------

.Struct		NSD_Flag
	flag		.byte		;flag
	gatemode	.byte		;gatemode
.Endstruct

.Struct		NSD_length
	length		.byte		;l command value
	tai		.byte		;Tai & Slur
.Endstruct

.Struct		NSD_gatetime
	q		.byte		;q command value
	u		.byte		;u command value
.Endstruct

.Struct		NSD_note
	note		.byte		;note command value (bit 7 = `H' : rest)
	octave		.byte		;o command value
.Endstruct

.Struct		NSD_Detune
	cent		.byte		;D command value
	fine		.byte		;D% command value
.Endstruct

.Struct		NSD_Por_Lv
	target		.byte		;target
	inc		.byte		;add value
.Endstruct

.Struct		NSD_Por_Sp
	decay		.byte		;
	rate		.byte		;rate (speed)
.Endstruct

.Struct		NSD_Por_Co
	count		.byte		;rate control
	lv		.byte		;now depth
.Endstruct

.Struct		NSD_Rep_Trans
	count		.byte		;counter of repeat
	trans		.byte		;transpose
.Endstruct

.Struct		NSD_volume
	volume		.byte		;v command value
	volume_set	.byte		;deveice setting value
.Endstruct

.Struct		NSD_voice
	voice		.byte		;@ command value
	voice_set	.byte		;deveice setting value
.Endstruct

.Struct		NSD_Env_V_Ptr
	Volume		.byte					;Volume
	Voice		.byte					;Voice
.Endstruct

.Struct		NSD_Env_F_Ptr
	Frequency	.byte					;Freeuqncy
	Note		.byte					;Note
.Endstruct

.Struct		NSD_work
	Flag		.tag	NSD_Flag	nsd::Track	;flag
	Length		.tag	NSD_length	nsd::Track	;note length [tick]
	Gatetime	.tag	NSD_gatetime	nsd::Track	;gate time [tick]
	Note		.tag	NSD_note	nsd::Track	;octave and note command
	Detune		.tag	NSD_Detune	nsd::Track	;detune
	Por_Lv		.tag	NSD_Por_Lv	nsd::Track	;Portamento
	Por_Sp		.tag	NSD_Por_Sp	nsd::Track	;Portamento
	Por_Co		.tag	NSD_Por_Co	nsd::Track	;Portamento
	Rep_Trans	.tag	NSD_Rep_Trans	nsd::Track	;Repeat and Transpose
	Volume		.tag	NSD_volume	nsd::Track	;volume
	Voice		.tag	NSD_voice	nsd::Track	;voice
	SubRoutine	.word			nsd::Track	;Pointer of sub-routine return address
	Frequency	.word			nsd::Track	;Setting frequency
	Frequency_Set	.word			nsd::Track	;Setting frequency for Device
	Envelop_Volume	.word			nsd::Track	;Pointer of envelop
	Envelop_Voice	.word			nsd::Track	;Pointer of envelop
	Envelop_Freq	.word			nsd::Track	;Pointer of envelop
	Envelop_Note	.word			nsd::Track	;Pointer of envelop
	Env_V_Ptr	.tag	NSD_Env_V_Ptr	nsd::Track	;
	Env_F_Ptr	.tag	NSD_Env_F_Ptr	nsd::Track	;
.Endstruct

;=======================================================================
;	Defines for work structures
;-----------------------------------------------------------------------

.scope	nsd_flag
	BGM	= $01
	SE	= $02
.endscope

;Zero Page Works
.define	__ptr		nsd_work_zp + NSD_work_zp::_ptr
.define	__tmp		nsd_work_zp + NSD_work_zp::_tmp
.define	__flag		nsd_work_zp + NSD_work_zp::flag
.define	__channel	nsd_work_zp + NSD_work_zp::channel
.define	__Sequence_ptr	nsd_work_zp + NSD_work_zp::Sequence_ptr
.define	__Length_ctr	nsd_work_zp + NSD_work_zp::Length + NSD_Length_Cnt::counter
.define	__Gate		nsd_work_zp + NSD_work_zp::Length + NSD_Length_Cnt::gate
.define	__Envelop_V	nsd_work_zp + NSD_work_zp::Envelop + NSD_Envelop::V
.define	__Envelop_F	nsd_work_zp + NSD_work_zp::Envelop + NSD_Envelop::F
.define	__Tempo		nsd_work_zp + NSD_work_zp::Envelop + NSD_Envelop::V + nsd::TR_BGM3
.define	__Tempo_ctr	nsd_work_zp + NSD_work_zp::Envelop + NSD_Envelop::V + nsd::TR_BGM5

.scope	nsd_chflag
	SE1	= $40
	SE2	= $80
.endscope

;RAM Works
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
.define	__por_inc	nsd_work + NSD_work::Por_Lv + NSD_Por_Lv::inc
.define	__por_decay	nsd_work + NSD_work::Por_Sp + NSD_Por_Sp::decay
.define	__por_rate	nsd_work + NSD_work::Por_Sp + NSD_Por_Sp::rate
.define	__por_ctr	nsd_work + NSD_work::Por_Co + NSD_Por_Co::count
.define	__por_lv	nsd_work + NSD_work::Por_Co + NSD_Por_Co::lv
.define	__trans		nsd_work + NSD_work::Rep_Trans + NSD_Rep_Trans::trans
.define	__repeat_ctr	nsd_work + NSD_work::Rep_Trans + NSD_Rep_Trans::count
.define	__volume	nsd_work + NSD_work::Volume + NSD_volume::volume
.define	__volume_set	nsd_work + NSD_work::Volume + NSD_volume::volume_set
.define	__voice		nsd_work + NSD_work::Voice + NSD_voice::voice
.define	__voice_set	nsd_work + NSD_work::Voice + NSD_voice::voice_set
.define	__frequency	nsd_work + NSD_work::Frequency
.define	__frequency_set	nsd_work + NSD_work::Frequency_Set
.define	__subroutine	nsd_work + NSD_work::SubRoutine
.define	__env_volume	nsd_work + NSD_work::Envelop_Volume
.define	__env_voice	nsd_work + NSD_work::Envelop_Voice
.define	__env_frequency	nsd_work + NSD_work::Envelop_Freq
.define	__env_note	nsd_work + NSD_work::Envelop_Note
.define	__env_vol_ptr	nsd_work + NSD_work::Env_V_Ptr + NSD_Env_V_Ptr::Volume
.define	__env_voi_ptr	nsd_work + NSD_work::Env_V_Ptr + NSD_Env_V_Ptr::Voice
.define	__env_freq_ptr	nsd_work + NSD_work::Env_F_Ptr + NSD_Env_F_Ptr::Frequency
.define	__env_note_ptr	nsd_work + NSD_work::Env_F_Ptr + NSD_Env_F_Ptr::Note

;=======================================================================
;	Macros
;-----------------------------------------------------------------------

.MACPACK generic

.macro  ldax    arg
	.if (.match (.left (1, {arg}), #))
		; immediate mode
		lda     #<(.right (.tcount ({arg})-1, {arg}))
		ldx     #>(.right (.tcount ({arg})-1, {arg}))
	.else
		; assume absolute or zero page
		lda     arg
		ldx     1+(arg)
	.endif
.endmacro

.macro  addw    Arg
	clc
	.if (.match (.left (1, {arg}), #))
		; immediate mode
		adc     #<(.right (.tcount ({arg})-1, {arg}))
		adc     #>(.right (.tcount ({arg})-1, {arg}))
	.else
		; assume absolute or zero page
		adc     arg
		adc     1+(arg)
	.endif
.endmacro

.macro  subw    Arg
	sec
	.if (.match (.left (1, {arg}), #))
		; immediate mode
		sbc     #<(.right (.tcount ({arg})-1, {arg}))
		sbc     #>(.right (.tcount ({arg})-1, {arg}))
	.else
		; assume absolute or zero page
		sbc     arg
		sbc     1+(arg)
	.endif
.endmacro

.macro	shl	arg, c
	.repeat	c
		asl	arg
	.endrepeat
.endmacro

.macro	shr	arg, c
	.repeat	c
		lsr	arg
	.endrepeat
.endmacro

