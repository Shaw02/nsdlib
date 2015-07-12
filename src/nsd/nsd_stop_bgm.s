;=======================================================================
;
;	NES Sound Driver & library (NSD.lib)	Library Functions
;
;-----------------------------------------------------------------------
;
;	Copyright (c) 2012 A.Watanabe (S.W.), All rights reserved.
;	For conditions of distribution and use, see copyright notice
;	  in "nsd.h" or "nsd.inc".
;
;=======================================================================

	.setcpu		"6502"

	.export		_nsd_stop_bgm

	.import		_nsd_snd_voice
	.import		_nsd_snd_volume
	.import		_nsd_snd_keyoff

	.import		nsd_work
	.importzp	nsd_work_zp

	.include	"nes.inc"
	.include	"nsddef.inc"
	.include	"macro.inc"


.code

;=======================================================================
;	void	__fastcall__	nsd_stop_bgm(void );
;-----------------------------------------------------------------------
;<<Contents>>
;	Stop the BGM
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
;	jsr	_nsd_stop	

	jsr	_nsd_snd_keyoff		;[6]

	lda	#$00			;[2]
	sta	__Sequence_ptr,x	;[4]
	sta	__Sequence_ptr + 1,x	;[4]

	jsr	_nsd_snd_volume		;[6]

	;to do VRC7
	lda	#$FF			;[2]
	sta	__note,x		;[5]

	inx						;[2]2
	inx						;[2]4
	cpx	#nsd::TR_BGM1 + nsd::BGM_Track * 2	;[2]6
	bcc	Loop					;[2]8	Total = 10 clock

	rts
.endproc
