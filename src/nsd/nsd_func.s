
	.setcpu		"6502"

	.export		_nsd_init
	.export		_nsd_play_bgm
	.export		_nsd_stop_bgm
;	.export		_nsd_play_se
	.export		_nsd_stop_se

	.import		_nsd_snd_init
	.import		nsd_work
	.importzp	nsd_work_zp

	.include	"nes.inc"
	.include	"nsd.inc"

.code

;=======================================================================
;	void    __fastcall__    nsd_init(void );
;-----------------------------------------------------------------------
;<<Contents>>
;	Init
;<<Input>>
;	nothing
;<<Output>>
;	nothing
;=======================================================================
.proc	_nsd_init: near

	jsr	_nsd_stop_bgm
	jsr	_nsd_stop_se
	jsr	_nsd_snd_init

	rts
.endproc

;=======================================================================
;	void    __fastcall__    nsd_play_bgm(void* bgm );
;-----------------------------------------------------------------------
;<<Contents>>
;	Start BGM
;<<Input>>
;	ax	pointer of BGM
;<<Output>>
;	nothing
;=======================================================================
.proc	_nsd_play_bgm: near

	sta	__ptr
	stx	__ptr + 1

	jsr	_nsd_stop_bgm

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
	sta	__Sequence_ptr + 1,x
	iny
	lda	#0
	jmp	@E
@L:
	lda	#0
	sta	__Sequence_ptr,x
	sta	__Sequence_ptr + 1,x
@E:
	sta	__chflag,x		;Å°Å°Å° to do for SE 
	sta	__tai,x
	sta	__Gate,x
	sta	__gate_q,x
	sta	__gate_u,x
	sta	__detune_cent,x
	sta	__detune_fine,x
	sta	__por_target,x
	sta	__trans,x
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
	sta	__env_voice,x
	sta	__env_voice + 1,x
	sta	__env_voi_ptr,x
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

	inx
	inx
	cpx	#nsd::TR_BGM1 + nsd::BGM_Track * 2
	bcs	Loop_End
	jmp	Loop
Loop_End:

	;-----------------------
	;common

	lda	#120
	sta	__Tempo

	lda	#~nsd_flag::BGM
	and	__flag
	sta	__flag			;BGM Enable

	rts
.endproc

;=======================================================================
;	void    __fastcall__    nsd_stop_bgm(void );
;-----------------------------------------------------------------------
;<<Contents>>
;	Init
;<<Input>>
;	nothing
;<<Output>>
;	nothing
;=======================================================================
.proc	_nsd_stop_bgm: near

	;-----------------------
	;common
	lda	#nsd_flag::BGM
	ora	__flag
	sta	__flag			;BGM Disable

	;-----------------------
	;Init the channel structure
	ldx	#nsd::TR_BGM1
Loop:
	lda	#$00
	sta	__Sequence_ptr,x
	sta	__Sequence_ptr + 1,x
	lda	#$FD
	sta	__note,x

	inx
	inx
	cpx	#nsd::TR_BGM1 + nsd::BGM_Track * 2
	bcc	Loop	

	rts
.endproc

;=======================================================================
;	void    __fastcall__    nsd_stop_se(void );
;-----------------------------------------------------------------------
;<<Contents>>
;	Init
;<<Input>>
;	nothing
;<<Output>>
;	nothing
;=======================================================================
.proc	_nsd_stop_se: near

	;-----------------------
	;common
	lda	#nsd_flag::SE
	ora	__flag
	sta	__flag			;SE Disable

	;-----------------------
	;Init the channel structure
	ldx	#nsd::TR_SE1
Loop:
	lda	#$00
	sta	__Sequence_ptr,x
	sta	__Sequence_ptr + 1,x
	lda	#$FD
	sta	__note,x

	inx
	inx
	cpx	#nsd::TR_SE1 + nsd::SE_Track * 2
	bcc	Loop	

	rts
.endproc

