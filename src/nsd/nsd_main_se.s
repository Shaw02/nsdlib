;=======================================================================
;
;	NES Sound Driver Libraly		Main Routine
;
;=======================================================================

	.setcpu		"6502"

	.export		_nsd_main_se

	.importzp	nsd_work_zp
	.import		nsd_work

	.import		nsd_sequence
	.import		nsd_envelop

	.include	"nes.inc"
	.include	"nsddef.inc"
	.include	"macro.inc"


.code

;=======================================================================
;	void    __fastcall__    nsd_main_se(void );
;-----------------------------------------------------------------------
;<<Contents>>
;	main process of SE
;<<Input>>
;	nothing
;<<Output>>
;	nothing
;=======================================================================
.proc	_nsd_main_se: near

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
