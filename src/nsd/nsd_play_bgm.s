;=======================================================================
;
;	NES Sound Driver Libraly		Library Functions
;
;=======================================================================

	.setcpu		"6502"

	.export		_nsd_play_bgm
	.export		_nsd_play

	.import		_nsd_stop_bgm

	.import		nsd_work
	.importzp	nsd_work_zp

	.include	"nes.inc"
	.include	"nsd.inc"

.code

;=======================================================================
;	void	__fastcall__	nsd_play_bgm(void* bgm );
;-----------------------------------------------------------------------
;<<Contents>>
;	Play start the BGM
;<<Input>>
;	ax	pointer of BGM
;<<Output>>
;	nothing
;=======================================================================
.proc	_nsd_play_bgm: near

	pha
	txa
	pha

	jsr	_nsd_stop_bgm		;stop the BGM (disable BGM)

	pla
	sta	__ptr + 1
	pla
	sta	__ptr

	ldy	#0

	lda	(__ptr),y		; a = number of track
	shl	a, 1
	sta	__tmp
	iny
	iny

	;-----------------------
	;Init the channel structure
	ldx	#nsd::TR_BGM1

Loop:
	lda	#00
	sta	__chflag,x
	jsr	_nsd_play

	inx
	inx
	cpx	#nsd::TR_BGM1 + nsd::BGM_Track * 2
	bcs	Loop_End
	jmp	Loop
Loop_End:

	;-----------------------
	;common
	lda	#$08
	sta	APU_PULSE1RAMP
	sta	APU_PULSE2RAMP
	sta	__sweep_ch1
	sta	__sweep_ch2

	lda	#120
	sta	__Tempo

	lda	#$FF
	sta	__Tempo_ctr

.ifdef	OPLL
	lda	#OPLL_RHYTHM
	sta	OPLL_Resister

	lda	#$00
	sta	__opll_ryhthm
	tay
	sta	OPLL_Data
.endif

	lda	#~nsd_flag::BGM
	and	__flag
	sta	__flag			;BGM Enable

	rts
.endproc

;=======================================================================
;	Play
;-----------------------------------------------------------------------
;<<Contents>>
;	Play
;<<Input>>
;	__ptr	pointer
;	__tmp	channel
;<<Output>>
;	nothing
;=======================================================================
.proc	_nsd_play

	cpx	__tmp
	bcs	@L
	lda	(__ptr),y
	add	__ptr
	php
	sta	__Sequence_ptr,x
	iny
	lda	(__ptr),y
	plp
	adc	__ptr + 1
	sta	__Sequence_ptr + 1,x	;__Sequence_ptr = __ptr + (__ptr),y
	iny
	lda	#0
	jmp	@E
@L:
	lda	#0
	sta	__Sequence_ptr,x
	sta	__Sequence_ptr + 1,x
@E:
	sta	__tai,x
	sta	__Gate,x
	sta	__gate_q,x
	sta	__gate_u,x
	sta	__detune_cent,x
	sta	__detune_fine,x
	sta	__por_target,x
	sta	__trans,x
	sta	__trans_one,x
	sta	__subroutine,x
	sta	__subroutine + 1,x

	sta	__env_frequency,x
	sta	__env_frequency + 1,x
	sta	__env_freq_ptr,x
	sta	__env_note,x
	sta	__env_note + 1,x
	sta	__env_note_ptr,x

	;Ch3(Tri) or Ch5(DPCM) ?
	cpx	#nsd::TR_BGM3
	beq	Step
	cpx	#nsd::TR_BGM5
	beq	Step

	sta	__env_volume,x
	sta	__env_volume + 1,x
	sta	__env_vol_ptr,x
	sta	__env_voice + 1,x
	sta	__env_voi_ptr,x
	lda	#$02
	sta	__env_voice,x
	lda	#$00
	sta	__voice,x
	lda	#$2F
	sta	__volume,x
Step:

	lda	#1
	sta	__Length_ctr,x

	lda	#24
	sta	__length,x

	lda	#48
	sta	__octave,x

	lda	#$FF
	sta	__note,x
	lda	#$2
	sta	__gatemode,x

	rts
.endproc

