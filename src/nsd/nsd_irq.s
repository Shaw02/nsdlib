;=======================================================================
;
;	NES Sound Driver Libraly		IRQ Main Routine
;
;=======================================================================

	.setcpu		"6502"

	.export		_nsd_irq
	.export		_nsd_bank_change

	.import		_nsd_snd_keyon
	.import		_nsd_dpcm_calc

	.importzp	nsd_work_zp
	.import		nsd_work

	.include	"nes.inc"
	.include	"nsd.inc"

.code

;=======================================================================
;	void    __fastcall__    nsd_irq(void );
;-----------------------------------------------------------------------
;<<Contents>>
;	IRQ process
;<<Input>>
;	nothing
;<<Output>>
;	nothing
;=======================================================================
.proc	_nsd_irq: near

	;register push
	lda	__tmp
	pha
	lda	__tmp + 1
	pha
	lda	__ptr
	pha
	lda	__ptr + 1
	pha

	;DPCM IEQ check
	lda	APU_CHANCTRL
	bpl	DPCM_Exit

	lda	#$0F
	sta	APU_CHANCTRL

	;next DPCM
	ldx	#nsd::TR_BGM5
	jsr	_nsd_dpcm_calc

	;éüÇ…î≠âπÇ∑ÇÈÉmÅ[Égî‘çÜ
	ldy	#5
	lda	(__ptr),y
	sta	__note,x

	;key on
;	jsr	_nsd_snd_keyon

	jsr	_nsd_dpcm_calc

	;bank number
	ldy	#4
	lda	(__ptr),y
	jsr	_nsd_bank_change

	;Set the DPCM resister
	;CTRL (note etc..)
	ldy	#0
	lda	(__ptr),y
	sta	APU_MODCTRL

	;volume
	iny

	;address
	iny
	lda	(__ptr),y
	sta	APU_MODADDR

	;length
	iny
	lda	(__ptr),y
	sta	APU_MODLEN

	lda	#$1F
	sta	APU_CHANCTRL

DPCM_Exit:

	;-------------------------------
	;Exit

	;register pop back
	pla
	sta	__ptr + 1
	pla
	sta	__ptr
	pla
	sta	__tmp + 1
	pla
	sta	__tmp

	rts
.endproc

;=======================================================================
;	void    __fastcall__    nsd_bank_change(void );
;-----------------------------------------------------------------------
;<<Contents>>
;	Bank Change for NSF
;<<Input>>
;	a	bank number for DPCM
;<<Output>>
;	nothing
;=======================================================================
.proc	_nsd_bank_change: near

	sta	$5FFC
	add	#1
	sta	$5FFD

	rts
.endproc
