
;---------------------------------------
;General Define
.scope	nsd
	APU_Track	= 5		;

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

	TR_MMC5		= TR_VRC7 + VRC7_Track * 2
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

	BGM_Track	= APU_Track + FDS_Track + VRC6_Track + VRC7_Track + MMC5_Track + N163_Track + PSG_Track
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

;---------------------------------------
;FDS define

FDS_Wave_Table		= $4040		; to $407F
FDS_Volume		= $4080
FDS_FTUNE		= $4082
FDS_CTUNE		= $4083
FDS_Sweep_Envelope	= $4084
FDS_Sweep_Bias		= $4085
FDS_Mod_FTUNE		= $4086
FDS_Mod_CTUNE		= $4087
FDS_Mod_Append		= $4088
FDS_Write_Enable	= $4089
FDS_Envelope_Speed	= $408A
FDS_Volume_Gain		= $4090		;$4080's value
FDS_Sweep_Gain		= $4092		;$4084's value

;---------------------------------------
;VRC6 define

VRC6_Frequency		= $9003		;.... .ABH	Normally you should write $00 to this register on startup to initialize it.
VRC6_Pulse1_CTRL	= $9000		;MDDD vvvv	mode DT vol
VRC6_Pulse1_FTUNE	= $9001		;FFFF FFFF
VRC6_Pulse1_CTUNE	= $9002		;E... FFFF
VRC6_Pulse2_CTRL	= $A000
VRC6_Pulse2_FTUNE	= $A001
VRC6_Pulse2_CTUNE	= $A002
VRC6_SAW_CTRL		= $B000		;..AA AAAA	volume
VRC6_SAW_FTUNE		= $B001		;FFFF FFFF
VRC6_SAW_CTUNE		= $B002		;E... FFFF

;---------------------------------------
;VRC7 define

VRC7_Resister		= $9010		;wait  6 clk
VRC7_Data		= $9030		;wait 42 clk

VRC7_Patch0		= $00
VRC7_Patch1		= $01
VRC7_Patch2		= $02
VRC7_Patch3		= $03
VRC7_Patch4		= $04
VRC7_Patch5		= $05
VRC7_Patch6		= $06
VRC7_Patch7		= $07

VRC7_Frequency		= $10
VRC7_Octave		= $20
VRC7_Volume		= $30

;---------------------------------------
;MMC5 define

MMC5_Pulse1_CTRL	= $5000         ; Pulse_ #1 Control Register (W)
MMC5_Pulse1_FTUNE	= $5002         ; Pulse_ #1 Fine Tune (FT) Register (W)
MMC5_Pulse1_CTUNE	= $5003         ; Pulse_ #1 Coarse Tune (CT) Register (W)
MMC5_Pulse2_CTRL	= $5004         ; Pulse_ #2 Control Register (W)
MMC5_Pulse2_FTUNE	= $5006         ; Pulse_ #2 Fine Tune Register (W)
MMC5_Pulse2_CTUNE	= $5007         ; Pulse_ #2 Coarse Tune Register (W)
MMC5_CHANCTRL		= $5015         ;

;---------------------------------------
;N163 define

N163_Resister		= $F800
N163_Data		= $4800

N163_Frequency_Low	= $40
N163_Frequency_Middle	= $42
N163_Frequency_High	= $44
N163_Waveform		= $46
N163_Volume		= $47
N163_EnableChannel	= $7F

;---------------------------------------
;PSG define

PSG_Register		= $C000
PSG_Data		= $E000

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

; 6 + ( 6 [Byte] * 28 [ch] ) = 174

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

.Struct		NSD_Por_Lv
	target		.byte		;`P' target
	depth		.byte		;`P' add value
.Endstruct

.Struct		NSD_Por_Co
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
	volume_env	.byte		;now envelop volume
.Endstruct

.Struct		NSD_voice
	voice		.byte		;`@R' command value (upper 4bit)
	voice_set	.byte		;deveice setting value
.Endstruct

.Struct		NSD_Env_V_Ptr
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
	Length		.tag	NSD_length	nsd::Track	;note length [tick]
	Gatetime	.tag	NSD_gatetime	nsd::Track	;gate time [tick]
	Note		.tag	NSD_note	nsd::Track	;octave and note command
	Detune		.tag	NSD_Detune	nsd::Track	;detune
	Por_Lv		.tag	NSD_Por_Lv	nsd::Track	;Portamento
	Por_Co		.tag	NSD_Por_Co	nsd::Track	;Portamento
	Por_now		.word			nsd::Track	;Portamento
	Trans		.tag	NSD_Trans	nsd::Track	;Repeat and Transpose
	Repeat		.tag	NSD_Repeat	nsd::Track	;Repeat and Transpose
	Volume		.tag	NSD_volume	nsd::Track	;volume
	Voice		.tag	NSD_voice	nsd::Track	;voice
	Env_F_Now	.tag	NSD_Env_F_Now	nsd::Track	;Now value of envelop
	Env_V_Ptr	.tag	NSD_Env_V_Ptr	nsd::Track	;Address of envelop
	Env_F_Ptr	.tag	NSD_Env_F_Ptr	nsd::Track	;Address of envelop
	Envelop_Volume	.word			nsd::Track	;Pointer of envelop
	Envelop_Voice	.word			nsd::Track	;Pointer of envelop
	Envelop_Freq	.word			nsd::Track	;Pointer of envelop
	Envelop_Note	.word			nsd::Track	;Pointer of envelop
	Frequency	.word			nsd::Track	;Setting frequency
	Frequency_Set	.word			nsd::Track	;Setting frequency for Device
	SubRoutine	.word			nsd::Track	;Pointer of sub-routine return address
	Repeat2		.word			nsd::Track	;Pointer of repeat2 goto point
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
.define	__Envelop_V	nsd_work_zp + NSD_work_zp::Envelop + NSD_Envelop::V	;upper 4bit Voice / lower 4bit:Volume
.define	__Envelop_F	nsd_work_zp + NSD_work_zp::Envelop + NSD_Envelop::F	;upper 4bit:Note  / lower 4bit:Frequency
.define	__Tempo		nsd_work_zp + NSD_work_zp::Envelop + NSD_Envelop::V + nsd::TR_BGM3
.define	__Tempo_ctr	nsd_work_zp + NSD_work_zp::Envelop + NSD_Envelop::V + nsd::TR_BGM5

.scope	nsd_chflag
	SE1	= $40
	SE2	= $80
	KeyOff	= $03		;00	Key Off	: Vol = 0
				;01	Key Off	: Release command
				;10	Key Off	:  envelop
				;11	Note On	: (envelop)
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
.define	__frequency_set	nsd_work + NSD_work::Frequency_Set
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

.define	__dpcm_info	nsd_work + NSD_work::Env_V_Ptr + nsd::TR_BGM5


;=======================================================================
;		Struct of DPCM					*
;-----------------------------------------------------------------------
.Struct		nsd_dpcm
	Control		.byte		;I/O 0x4010
	DA		.byte		;I/O 0x4011
	Address		.byte		;I/O 0x4012
	Size		.byte		;I/O 0x4013
.Endstruct



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

