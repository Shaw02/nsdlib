;=======================================================================
;
;	NES Sound Driver Libraly		Math Calculator
;
;=======================================================================

	.setcpu		"6502"

	.export		_nsd_div192
	.export		_nsd_mul

	.importzp	nsd_work_zp
	.import		nsd_work

	.include	"nes.inc"
	.include	"nsddef.inc"
	.include	"macro.inc"


;=======================================================================
;	void	__fastcall__	_nsd_div192(unsigned int ax);
;-----------------------------------------------------------------------
;<<Contents>>
;	Divide `ax' by 192 (0xC0) = 16 * 12
;	âÒïúñ@Ç…ÇÊÇÈ192å≈íËèúéZ
;<<Input>>
;	ax	value
;<<Output>>
;	a =	ax mod 192
;	x =	ax  /  192
;=======================================================================
.proc	_nsd_div192
.code
	pha		; save a
	txa
	tay		; y = x
	lda	#0	; a = octave

	;if (ax > 0x0600) then
	cpy	#$06
	bcc	Div_8
	tya
	sbc	#6	;cy = `H'
	tay		; y -= 6
	lda	#8

Div_8:	;if (ax > 0x0300) then
	cpy	#$03
	bcc	Div_4
	adc	#3	; + cy = 4
	dey
	dey
	dey	; y -= 3

Div_4:	
	tax		; x = octave
	pla		; restore a

	;if (ax > 0x0180) then
	cpy	#$01
	bcc	Div_2
	bne	@L2
	cmp	#$80
	bcc	Div_2
@L2:	sub	#$80
	bcs	@L
	dey
@L:	dey
	inx
	inx	; x += 2

Div_2:	;if (ax > 0x00C0) then
	cpy	#$00
	bne	@L2
	cmp	#$C0
	bcc	exit
@L2:	sub	#$C0
	bcs	@L
	dey
@L:	inx

exit:
	rts
.endproc

;=======================================================================
;	void	__fastcall__	_nsd_mul(unsigned char a, unsigned char x);
;-----------------------------------------------------------------------
;<<Contents>>
;	multiplication	8bit = 4bit * 6bit
;<<Input>>
;	a	value 1 (lower 4bit)	volume side	(0 - 15)
;	x	value 2 (lower 6bit)	envelope	(0 - 63)
;<<Output>>
;	a =	a Å~ (x Å{ 1) ÅÄ 16
;=======================================================================
.proc	_nsd_mul
.code

	and	#$0F
	eor	#$FF
	sta	__tmp

	inx
	txa
	sta	__tmp + 1		; = ^ (envelop+1)

	lda	#0
	lsr	__tmp
	bcs	@L1
	adc	__tmp + 1
@L1:
	lsr	a
	lsr	__tmp
	bcs	@L2
	adc	__tmp + 1
@L2:
	lsr	a
	lsr	__tmp
	bcs	@L3
	adc	__tmp + 1
@L3:
	lsr	a
	lsr	__tmp
	bcs	@L4
	adc	__tmp + 1
@L4:
	lsr	a
;	lsr	__tmp
;	bcs	@L5
;	adc	__tmp + 1
;@L5:
exit:
	rts
.endproc
