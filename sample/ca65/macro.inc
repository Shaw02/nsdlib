

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
;	a >>= c	�i�Z�p�V�t�g�j	���A�L�������[�^����
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
;	arg(mem16) >>= c	�i�Z�p�V�t�g�j
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
;	�a�f�A�h���X�v�Z
;---------------------------------------
;	ptx	X ���W
;	pty	Y ���W
;	scn	�X�N���[���ԍ�
;---------------------------------------
.define	ADDR_BG(ptx,pty,scn)	$2000 + (ptx) + ((pty)*32) + ((scn)*$400)


;---------------------------------------
;	�X�v���C�g�A�h���X�v�Z
;---------------------------------------
;	number	�X�v���C�g�ԍ��i0�`63�j
;	member	�A�N�Z�X���郁���o�[
;		�@ptx	�w���W
;		�@pty	�x���W
;		�@num	�L�����N�^�[
;		�@att	�����i�p���b�g�┽�]�Ȃǁj
;---------------------------------------
.define	ADDR_SPR(number,member)	_ppu_sprite_buff + ((number)*4) + SPR_ONE::member


;---------------------------------------
;	VBLANK �҂�
;---------------------------------------
.macro	WAIT_VBLANK
	.local	Skip
Skip:	lda	PPU_STATUS
	bpl	Skip
.endmacro

;---------------------------------------
;	NMI ���荞�ݑ҂�
;---------------------------------------
.macro	WAIT_NMI
	.local	Skip
	lda	__cc
Skip:	cmp	__cc
	beq	Skip
.endmacro

;---------------------------------------
;	�W�����v
;---------------------------------------
.macro	JMP_AX
	STAX	_JMP_Address
	jmp	(_JMP_Address)
.endmacro

;---------------------------------------
;	�R�[��
;---------------------------------------
.macro	JSR_AX
	jsr	jsr_ax
.endmacro

;---------------------------------------
;	�o���N�ԃR�[��
;---------------------------------------
.macro	FARJSR	bank, adr
	LDAX	adr
	ldy	bank
	jsr	jsr_axy
.endmacro

;---------------------------------------
;	��ʕ\��
;	������������҂��ƁI�I
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
;	��ʕ\��
;---------------------------------------
.macro	DISP_ON
	WAIT_VBLANK			;���������܂ő҂�
	lda	#sysdef::PPU_Ctrl1_set	;���荞�݂͊J�n����B
	sta	PPU_CTRL1
	sta	__Flag_2000
	lda	#sysdef::PPU_Ctrl2_set	;MMC3 IRQ�̏������̂��߂ɁA
	sta	__Flag_2001		;BG, Spr���ɕ\����on�ɂ���B
	sta	PPU_CTRL2		;
.endmacro

;---------------------------------------
;	��ʂ������܂��B
;	�i�����O�̏�Ԃ́A�ێ��j
;---------------------------------------
.macro	DISP_OFF
	lda	#%00000000
	sta	PPU_CTRL1
	sta	PPU_CTRL2
	WAIT_VBLANK			;��ʂ�������܂ő҂�
.endmacro

;---------------------------------------
;	��ʂ����ɖ߂��܂��B
;	�i�����O�̏�ԂɁA���A�j
;---------------------------------------
.macro	DISP_RET
	WAIT_VBLANK			;���������܂ő҂�
	lda	__Flag_2000
	sta	PPU_CTRL1
	lda	__Flag_2001
	sta	PPU_CTRL2
.endmacro

;---------------------------------------
;	�L�[�擾
;---------------------------------------
;�����݉�����Ă���L�[
.macro	GET_PAD0
	lda	__PAD0
.endmacro

.macro	GET_PAD1
	lda	__PAD1
.endmacro

;�������ꂽ�L�[
.macro	GET_PAD0_REMOVE
	lda	__PAD0_REL
.endmacro

.macro	GET_PAD1_REMOVE
	lda	__PAD1_REL
.endmacro

;�������ꂽ�L�[
.macro	GET_PAD0_PRESS
	lda	__PAD0_TRG
.endmacro

.macro	GET_PAD1_PRESS
	lda	__PAD1_TRG
.endmacro
