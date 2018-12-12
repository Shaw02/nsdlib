

;=======================================================================
;	Macros
;-----------------------------------------------------------------------

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
		lsr	1+(arg)
		ror	arg
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


;---------------------------------------
;	ＢＧアドレス計算
;---------------------------------------
;	ptx	X 座標
;	pty	Y 座標
;	scn	スクリーン番号
;---------------------------------------
.define	ADDR_BG(ptx,pty,scn)	$2000 + (ptx) + ((pty)*32) + ((scn)*$400)


;---------------------------------------
;	スプライトアドレス計算
;---------------------------------------
;	number	スプライト番号（0～63）
;	member	アクセスするメンバー
;		　ptx	Ｘ座標
;		　pty	Ｙ座標
;		　num	キャラクター
;		　att	属性（パレットや反転など）
;---------------------------------------
.define	ADDR_SPR(number,member)	_ppu_sprite_buff + ((number)*4) + SPR_ONE::member


;---------------------------------------
;	VBLANK 待ち
;---------------------------------------
.macro	WAIT_VBLANK
	.local	Skip
Skip:	lda	PPU_STATUS
	bpl	Skip
.endmacro

;---------------------------------------
;	NMI 割り込み待ち
;---------------------------------------
.macro	WAIT_NMI
	.local	Skip
	lda	__cc
Skip:	cmp	__cc
	beq	Skip
.endmacro

;---------------------------------------
;	ジャンプ
;---------------------------------------
.macro	JMP_AX
	STAX	_JMP_Address
	jmp	(_JMP_Address)
.endmacro

;---------------------------------------
;	コール
;---------------------------------------
.macro	JSR_AX
	jsr	jsr_ax
.endmacro

;---------------------------------------
;	バンク間コール
;---------------------------------------
.macro	FARJSR	bank, adr
	LDAX	adr
	ldy	bank
	jsr	jsr_axy
.endmacro

;---------------------------------------
;	画面表示
;	※垂直同期を待つこと！！
;---------------------------------------
.macro	DISP_SET
	sta	PPU_CTRL1
	sta	__Flag_2000
.endmacro

.macro	DISP_SET1
	sta	PPU_CTRL1
	sta	__Flag_2000
.endmacro

.macro	DISP_SET2
	sta	PPU_CTRL2
	sta	__Flag_2001
.endmacro
;---------------------------------------
;	画面表示
;---------------------------------------
.macro	DISP_ON
	WAIT_VBLANK			;垂直同期まで待つ
	lda	#sysdef::PPU_Ctrl1_set	;割り込みは開始する。
	sta	PPU_CTRL1
	sta	__Flag_2000
	lda	#sysdef::PPU_Ctrl2_set	;MMC3 IRQの初期化のために、
	sta	__Flag_2001		;BG, Spr共に表示をonにする。
	sta	PPU_CTRL2		;
.endmacro

;---------------------------------------
;	画面を消します。
;	（消す前の状態は、維持）
;---------------------------------------
.macro	DISP_OFF
	lda	#%00000000
	sta	PPU_CTRL1
	sta	PPU_CTRL2
	WAIT_VBLANK			;画面が消えるまで待つ
.endmacro

;---------------------------------------
;	画面を元に戻します。
;	（消す前の状態に、復帰）
;---------------------------------------
.macro	DISP_RET
	WAIT_VBLANK			;垂直同期まで待つ
	lda	__Flag_2000
	sta	PPU_CTRL1
	lda	__Flag_2001
	sta	PPU_CTRL2
.endmacro

;---------------------------------------
;	キー取得
;---------------------------------------
;●現在押されているキー
.macro	GET_PAD0
	lda	__PAD0
.endmacro

.macro	GET_PAD1
	lda	__PAD1
.endmacro

;●離されたキー
.macro	GET_PAD0_REMOVE
	lda	__PAD0_REL
.endmacro

.macro	GET_PAD1_REMOVE
	lda	__PAD1_REL
.endmacro

;●押されたキー
.macro	GET_PAD0_PRESS
	lda	__PAD0_TRG
.endmacro

.macro	GET_PAD1_PRESS
	lda	__PAD1_TRG
.endmacro

