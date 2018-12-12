;=======================================================================
;
;	NES Sound Driver & library (NSD.lib)	Main Routine
;
;-----------------------------------------------------------------------
;
;	Copyright (c) 2012 A.Watanabe (S.W.), All rights reserved.
;	For conditions of distribution and use, see copyright notice
;	  in "nsd.h" or "nsd.inc".
;
;=======================================================================

	.setcpu		"6502"

	.export		_nsd_main

	.importzp	nsd_work_zp
	.import		nsd_work

	.import		nsd_sequence
	.import		nsd_envelop

	.include	"nes.inc"
	.include	"nsddef.inc"
	.include	"macro.inc"


.code

;=======================================================================
;	void    __fastcall__    nsd_main(void );
;-----------------------------------------------------------------------
;<<Contents>>
;	main process of BGM and SE
;<<Input>>
;	nothing
;<<Output>>
;	nothing
;=======================================================================
.proc	_nsd_main: near

	lda	__flag		;無効の場合は、呼ばない。
	jmi	Exit		;（最上位 1bit = `H'の場合、終了）

.ifdef	DPCMBank
	sei

	;register push
	lda	__tmp
	pha
	lda	__tmp + 1
	pha
	lda	__ptr
	pha
	lda	__ptr + 1
	pha
.endif

	NSD_MAIN_SE
	NSD_MAIN_BGM

.ifdef	DPCMBank
	;register pop back
	pla
	sta	__ptr + 1
	pla
	sta	__ptr
	pla
	sta	__tmp + 1
	pla
	sta	__tmp

	cli
.endif

	;-------------------------------
	;Exit
Exit:	rts
.endproc
