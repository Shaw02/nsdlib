;=======================================================================
;
;	NES Sound Driver Libraly		Library Functions
;
;=======================================================================

	.setcpu		"6502"

	.export		_nsd_pause_bgm
	.export		_nsd_pause

	.import		_nsd_snd_volume
	.import		_nsd_snd_keyoff

	.import		nsd_work
	.importzp	nsd_work_zp

	.include	"nes.inc"
	.include	"nsd.inc"


;=======================================================================
;	void	__fastcall__	nsd_pause_bgm(void );
;-----------------------------------------------------------------------
;<<Contents>>
;	Pause the BGM
;<<Input>>
;	nothing
;<<Output>>
;	nothing
;=======================================================================
.proc	_nsd_pause_bgm: near

	lda	#nsd_flag::BGM
	ora	__flag
	sta	__flag			;BGM Disable

	;-----------------------
	;Init the channel structure
	ldx	#nsd::TR_BGM1
Loop:
	jsr	_nsd_pause				;[6]20 clock

	inx						;[2]8
	inx						;[2]10
	cpx	#nsd::TR_BGM1 + nsd::BGM_Track * 2	;[2]12
	bcc	Loop					;[2]14

	rts
.endproc

;=======================================================================
;	Puase
;-----------------------------------------------------------------------
;<<Contents>>
;	Pause
;<<Input>>
;	nothing
;<<Output>>
;	nothing
;=======================================================================
.proc	_nsd_pause

	jsr	_nsd_snd_keyoff		;[6]26 clock

	lda	#$00			;[2]8
	jsr	_nsd_snd_volume		;[6]14

	;to do VRC7

	rts				;[6]6
.endproc
