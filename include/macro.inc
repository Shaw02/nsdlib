;=======================================================================
;
;	NES Sound Driver & library (NSD.lib)	Macros
;
;-----------------------------------------------------------------------
;
;	Copyright (c) 2012 A.Watanabe (S.W.), All rights reserved.
;	For conditions of distribution and use, see copyright notice
;	  in "nsd.h" or "nsd.inc".
;
;=======================================================================

.ifndef	__MACRO_DEF__

.define	__MACRO_DEF__	1

.MACPACK generic
.MACPACK longbranch 

;---------------------------------------
;	ax <= arg(mem16/imm16)
;---------------------------------------
.macro	LDAX	arg
	.if (.match (.left (1, {arg}), #))
		; immediate mode
		lda     #<(.right (.tcount ({arg})-1, {arg}))
		ldx     #>(.right (.tcount ({arg})-1, {arg}))
	.else
		; assume absolute or zero page
		lda     arg
		ldx     1+(arg)
	.endif
.endmacro


;---------------------------------------
;	arg(mem16) <= ax
;---------------------------------------
.macro	STAX	arg
		; assume absolute or zero page
		sta     arg
		stx     1+(arg)
.endmacro


;---------------------------------------
;	ax <= ax + arg(mem16/imm16)
;---------------------------------------
.macro	ADDW	arg
	clc
	.if (.match (.left (1, {arg}), #))
		; immediate mode
		adc     #<(.right (.tcount ({arg})-1, {arg}))
		pha
		txa
		adc     #>(.right (.tcount ({arg})-1, {arg}))
		tax
		pla
	.else
		; assume absolute or zero page
		adc     arg
		pha
		txa
		adc     1+(arg)
		tax
		pla
	.endif
.endmacro


;---------------------------------------
;	ax <= ax - arg(mem16/imm16)
;---------------------------------------
.macro	SUBW	arg
	sec
	.if (.match (.left (1, {arg}), #))
		; immediate mode
		sbc     #<(.right (.tcount ({arg})-1, {arg}))
		pha
		txa
		sbc     #>(.right (.tcount ({arg})-1, {arg}))
		tax
		pla
	.else
		; assume absolute or zero page
		sbc     arg
		pha
		txa
		sbc     1+(arg)
		tax
		pla
	.endif
.endmacro


;---------------------------------------
;	arg(ax/mem16)++
;---------------------------------------
.macro	INCW	arg
	.local	Skip

	.if (.blank(arg)) .or (.xmatch ({arg}, ax))
		add	#1
		bne	Skip
		inx
	.else
		inc	arg
		bne	Skip
		inc	1+(arg)
	.endif
Skip:
.endmacro


;---------------------------------------
;	arg(ax/mem16)--
;---------------------------------------
.macro	DECW	arg
	.local	Skip
	.if (.blank(arg)) .or (.match ({arg}, ax))
		sub	#1
		bcc	Skip
		dex
	.else
		pha
		lda	arg
		sub	#1
		sta	arg
		bcc	Skip
		dec	1+(arg)
		pla
	.endif
Skip:
.endmacro


;---------------------------------------
;	arg(a/mem8) <<= c
;---------------------------------------
.macro	SHL	arg, c
	.repeat	c
		asl	arg
	.endrepeat
.endmacro

.macro	shl	arg, c
	.repeat	c
		asl	arg
	.endrepeat
.endmacro


;---------------------------------------
;	arg(a/mem8) >>= c
;---------------------------------------
.macro	SHR	arg, c
	.repeat	c
		lsr	arg
	.endrepeat
.endmacro

.macro	shr	arg, c
	.repeat	c
		lsr	arg
	.endrepeat
.endmacro


;---------------------------------------
;	a >>= c	（算術シフト）	※アキュムレータ限定
;---------------------------------------
.macro	SAR	arg, c

	.if (.match ({arg}, a))

		.repeat	c
			cmp	#$80
			ror	a
		.endrepeat
	.else
		pha
		lda	arg
		.repeat	c
			cmp	#$80
			ror	a
		.endrepeat
		sta	arg
		pla
	.endif

.endmacro


;---------------------------------------
;	arg(mem16) <<= c
;---------------------------------------
.macro	SHLW	arg, c
	.repeat	c
		asl	arg
		rol	1+(arg)
	.endrepeat
.endmacro


;---------------------------------------
;	arg(mem16) >>= c
;---------------------------------------
.macro	SHRW	arg, c
	.repeat	c
		ror	1+(arg)
		lsr	arg
	.endrepeat
.endmacro


;---------------------------------------
;	arg(mem16) >>= c	（算術シフト）
;---------------------------------------
.macro	SARW	arg, c
	pha
	lda	1+(arg)
	.repeat	c
		cmp	#$80
		ror	a
		ror	arg
	.endrepeat
	sta	1+(arg)
	pla
.endmacro



.endif
