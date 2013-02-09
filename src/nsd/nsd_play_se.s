;=======================================================================
;
;	NES Sound Driver Libraly		Library Functions
;
;=======================================================================

	.setcpu		"6502"

	.export		_nsd_play_se

	.import		_nsd_play
	.import		_nsd_stop_se

	.import		nsd_work
	.importzp	nsd_work_zp

	.include	"nes.inc"
	.include	"nsd.inc"

.code
;=======================================================================
;	void	__fastcall__	nsd_play_se(void* se );
;-----------------------------------------------------------------------
;<<Contents>>
;	Play start the SE
;<<Input>>
;	ax	pointer of SE
;<<Output>>
;	nothing
;=======================================================================
.proc	_nsd_play_se: near

	pha
	txa
	pha

	jsr	_nsd_stop_se		;stop the BGM (disable BGM)

	pla
	sta	__ptr + 1
	pla
	sta	__ptr

	ldy	#0

	lda	(__ptr),y		; a = number of track
	shl	a, 1
	add	#nsd::TR_SE1
	sta	__tmp
	iny
	iny

	;-----------------------
	;Init the channel structure
	ldx	#nsd::TR_SE1
	lda	#nsd_chflag::SE1
	sta	__chflag,x
	jsr	_nsd_play

	ldx	#nsd::TR_SE2
	lda	#nsd_chflag::SE2
	sta	__chflag,x
	jsr	_nsd_play

	;-----------------------
	;common
	lda	#$08
	sta	APU_PULSE2RAMP		;ch 2 only

	;Tempo is none

	lda	#~nsd_flag::SE
	and	__flag
	sta	__flag			;SE Enable

	rts
.endproc
