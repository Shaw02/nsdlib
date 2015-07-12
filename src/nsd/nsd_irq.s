;=======================================================================
;
;	NES Sound Driver & library (NSD.lib)	IRQ Main Routine
;
;-----------------------------------------------------------------------
;
;	Copyright (c) 2012 A.Watanabe (S.W.), All rights reserved.
;	For conditions of distribution and use, see copyright notice
;	  in "nsd.h" or "nsd.inc".
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

;	;DPCM DMAによる割り込みか？
;	lda	APU_CHANCTRL
;	bpl	DPCM_END

	;---------------
	;⊿PCM再生が終わるのを待つ。
@wait:
	lda	APU_CHANCTRL
	and	#%00010000
	bne	@wait

	lda	#$0F
	sta	APU_CHANCTRL

	;---------------
	;現在発音している⊿PCM構造体の要素を取得
	ldx	#nsd::TR_BGM5
	jsr	_nsd_dpcm_calc
.ifdef	DPCMBank
	jsr	_nsd_ptr_bank
.endif

	;---------------
	;これから発音する⊿PCM構造体の要素を取得
	ldy	#5
	lda	(__ptr),y
	sta	__note,x		;次に発音するノート番号
	jsr	_nsd_dpcm_calc
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

DPCM_END:

Exit:
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
