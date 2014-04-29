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

.ifdef	DPCMBank
	.import		_nsd_ptr_bank
.endif

	.importzp	nsd_work_zp
	.import		nsd_work

	.include	"nes.inc"
	.include	"nsddef.inc"
	.include	"macro.inc"


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

Start:

	;DPCM IEQ check
DPCM_CHK:
	lda	APU_CHANCTRL
	bpl	DPCM_END

	and	#$7F
	sta	APU_CHANCTRL

	and	#%00010000		;カウンタが残っていた場合。
	beq	DPCM_IRQ
DPCM_END:

Exit:
	rts

DPCM_IRQ:
	lda	#$10			
	sta	APU_MODCTRL		;End of IRQ の発行

	;next DPCM
	ldx	#nsd::TR_BGM5
	jsr	_nsd_dpcm_calc		;現在の構造体から取得
.ifdef	DPCMBank
	jsr	_nsd_ptr_bank
.endif

	;次に発音するノート番号
	ldy	#5
	lda	(__ptr),y
	sta	__note,x

	jsr	_nsd_dpcm_calc		;これから発音する構造体から取得
.ifdef	DPCMBank
	jsr	_nsd_ptr_bank
.endif

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

	bne	Start

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
