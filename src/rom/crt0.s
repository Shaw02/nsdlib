;=======================================================================
;
;	NSF ROM code for NSD.Lib
;
;				Programmed by A.Watanabe
;
;=======================================================================

;-------------------------------
;Memory Map of this NSF program code
;-------------------------------
;Address	Type	Contents
;0x8000		BYTE	BGM��
;0x8001		BYTE	SE��
;0x8002		WORD	��PCM�\���̂̃|�C���^
;0x8004-	WORD	BGM�SE�f�[�^�̃|�C���^
;�`�`			BGM,SE,�G���x���[�v�A���F�f�[�^ �� ��PCM�\����
;0xC000-		��PCM����
;0xE000-		�v���O�����R�[�h

; ------------------------------------------------------------------------
; Define for C Langage
; ------------------------------------------------------------------------

;	.export		_exit
	.export		__STARTUP__ : absolute = 1      ; Mark as startup
;	.import		initlib, donelib, callmain
;	.import		push0, _main, zerobss, copydata
;	.import		ppubuf_flush

	; Linker generated symbols
	.import		__RAM_START__, __RAM_SIZE__
	.import		__SRAM_START__, __SRAM_SIZE__
	.import		__ROM0_START__, __ROM0_SIZE__, __ROM0_LAST__
	.import		__STARTUP_LOAD__,__STARTUP_RUN__, __STARTUP_SIZE__
	.import		__CODE_LOAD__,__CODE_RUN__, __CODE_SIZE__
	.import		__RODATA_LOAD__,__RODATA_RUN__, __RODATA_SIZE__

;	.include	"zeropage.inc"
;	.include	"nes.inc"

	.include	"..\..\include\nsd.inc"


; ------------------------------------------------------------------------
; �ϐ�
; ------------------------------------------------------------------------
.zeropage

_ptr:	.word	0
_tmp:	.word	0



.bss

_eff:	.byte	0		;SE start number

; ------------------------------------------------------------------------
; 16 bytes INES header
; ------------------------------------------------------------------------

	.ifdef	FDS
		FDS_Flag	= $04		;
	.else
		FDS_Flag	= 0		;
	.endif

	.ifdef	VRC6
		VRC6_Flag	= $01		;
	.else
		VRC6_Flag	= 0		;
	.endif

	.if	.defined(VRC7) || .defined(OPLL)
		VRC7_Flag	= $02		;
	.else
		VRC7_Flag	= 0		;
	.endif

	.ifdef	MMC5
		MMC5_Flag	= $08		; (without DPCM)
	.else
		MMC5_Flag	= 0		;
	.endif

	.ifdef	N163
		N163_Flag	= $10		;
	.else
		N163_Flag	= 0		;
	.endif

	.ifdef	PSG
		PSG_Flag	= $20		;
	.else
		PSG_Flag	= 0		;
	.endif


.segment	"HEADER"

	.byte	$4E,$45,$53,$4D,$1A	;00	"NESM",0x1A
	.byte	1			;05	Version
	.byte	0			;06	Music number
	.byte	1			;07	Start Music number
	.addr	$8000			;08	Load address
	.addr	_nsf_init		;0A	Init routine address
	.addr	_nsf_main		;0C	Sound driver main routine address
	.res	32,	$0
	.res	32,	$0
	.res	32,	$0
	.word	$411A			;6E	60Hz
	.byte	0,0,0,0,0,0,0,0		;70	bank
	.word	$4E20			;78
	.byte	0			;7A
	.byte	FDS_Flag + VRC6_Flag + VRC7_Flag + MMC5_Flag + N163_Flag + PSG_Flag
	.byte	0,0,0,0			;78

; ------------------------------------------------------------------------
; 	���@ROM�p	Reset
; ------------------------------------------------------------------------
.segment	"STARTUP"

.ifdef	DPCMBank
; ------------------------------------------------------------------------
; 	���@ROM�p	IRQ	(DPCM)
; ------------------------------------------------------------------------
.proc	_irq_main

	pha			;register push
	tya
	pha
	txa
	pha

;	jsr	_nsd_irq_main

	pla			;register pop
	tax
	pla
	tay
	pla

	rti
.endproc

; ------------------------------------------------------------------------
; 	���@ROM�p	NMI	(Vblank)
; ------------------------------------------------------------------------
.proc	_nmi_main

	pha			;register push
	tya
	pha
	txa
	pha

	jsr	_nsd_main

	pla			;register pop
	tax
	pla
	tay
	pla

	rti

.endproc

.endif
; ------------------------------------------------------------------------
; 	NSF�p
; ------------------------------------------------------------------------
.proc	_nsf_main

	jmp	_nsd_main

.endproc

; ------------------------------------------------------------------------
; 
; ------------------------------------------------------------------------
.proc	_nsf_init
	pha

	jsr	_zero_mem

	jsr	_init

	pla
	jsr	_play_music

	rts
.endproc

; ------------------------------------------------------------------------
; 
; ------------------------------------------------------------------------
.proc	_zero_mem
	lda	#0
	ldx	#0

@L:
	sta	$0000,x	;Stack�̓N���A���Ȃ��B
	sta	$0200,x
	sta	$0300,x
	sta	$0400,x
	sta	$0500,x
	sta	$0600,x
	sta	$0700,x
	inx
	bne	@L

	;�R�s�[
.ifdef	DPCMBank
	ldx	#0
@L2:
	lda	$8000,x
	sta	$6000,x
	lda	$8100,x
	sta	$6100,x
	lda	$8200,x
	sta	$6200,x
	lda	$8300,x
	sta	$6300,x
	lda	$8400,x
	sta	$6400,x
	lda	$8500,x
	sta	$6500,x
	lda	$8600,x
	sta	$6600,x
	lda	$8700,x
	sta	$6700,x
	lda	$8800,x
	sta	$6800,x
	lda	$8900,x
	sta	$6900,x
	lda	$8A00,x
	sta	$6A00,x
	lda	$8B00,x
	sta	$6B00,x
	lda	$8C00,x
	sta	$6C00,x
	lda	$8D00,x
	sta	$6D00,x
	lda	$8E00,x
	sta	$6E00,x
	lda	$8F00,x
	sta	$6F00,x

	lda	$9000,x
	sta	$7000,x
	lda	$9100,x
	sta	$7100,x
	lda	$9200,x
	sta	$7200,x
	lda	$9300,x
	sta	$7300,x
	lda	$9400,x
	sta	$7400,x
	lda	$9500,x
	sta	$7500,x
	lda	$9600,x
	sta	$7600,x
	lda	$9700,x
	sta	$7700,x
	lda	$9800,x
	sta	$7800,x
	lda	$9900,x
	sta	$7900,x
	lda	$9A00,x
	sta	$7A00,x
	lda	$9B00,x
	sta	$7B00,x
	lda	$9C00,x
	sta	$7C00,x
	lda	$9D00,x
	sta	$7D00,x
	lda	$9E00,x
	sta	$7E00,x
	lda	$9F00,x
	sta	$7F00,x

	inx
	bne	@L
.endif

	rts
.endproc

; ------------------------------------------------------------------------
; 
; ------------------------------------------------------------------------
.proc	_init

	jsr	_nsd_init

	lda	#<(__ROM0_LAST__)
	sta	_ptr
	lda	#>(__ROM0_LAST__)
	sta	_ptr+1		;_ptr = __ROM0_START__

	ldy	#0
	lda	(_ptr),y
	iny			;ax = BGM and SE's qty
	iny
	sta	_eff

	lda	(_ptr),y
	iny
	sta	_tmp
	lda	(_ptr),y
	tax			;ax = Pointer of ��PCM infomation Struct
	lda	_tmp
	jsr	_nsd_set_dpcm

	rts
.endproc

; ------------------------------------------------------------------------
; 
; ------------------------------------------------------------------------
.proc	_play_music

	pha
	lda	#<(__ROM0_LAST__)
	sta	_ptr
	lda	#>(__ROM0_LAST__)
	sta	_ptr+1		;_ptr = __ROM0_START__
	pla

	cmp	_eff
	php
	clc
	adc	#2
	asl	a
	tay			; y = _mus * 2
	lda	(_ptr),y
	sta	_tmp
	iny
	lda	(_ptr),y	; ax = Pointer of BGM/SE
	tax
	lda	_tmp
	plp

	bcs	@L
	jmp	_nsd_play_bgm
@L:	jmp	_nsd_play_se
.endproc


; ------------------------------------------------------------------------
; hardware vectors
; ------------------------------------------------------------------------

.ifdef	DPCMBank
.segment	"VECTORS"
	.word	_nmi_main	; $fffa vblank nmi
	.word	_nsf_init	; $fffc reset
	.word	_irq_main	; $fffe irq / brk
.endif