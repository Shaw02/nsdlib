;=======================================================================
;
;	NES Sound Driver Libraly		Library Functions
;
;=======================================================================

	.setcpu		"6502"

	.export		_nsd_stop_se

	.import		_nsd_stop

	.import		_nsd_snd_init
	.import		_nsd_snd_sweep
	.import		_nsd_snd_volume
	.import		_nsd_snd_keyoff
	.import		nsd_work
	.importzp	nsd_work_zp

	.include	"nes.inc"
	.include	"nsd.inc"

.code
;=======================================================================
;	void	__fastcall__	nsd_stop_se(void );
;-----------------------------------------------------------------------
;<<Contents>>
;	Stop the SE
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
	jsr	_nsd_stop
	inx
	inx
	cpx	#nsd::TR_SE1 + nsd::SE_Track * 2
	bcc	Loop	

	rts
.endproc
