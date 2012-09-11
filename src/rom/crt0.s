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
;0x8000		BYTE	BGM数
;0x8001		BYTE	SE数
;0x8002		WORD	⊿PCM構造体のポインタ
;0x8004-	WORD	BGM､SEデータのポインタ
;～～			BGM,SE,エンベロープ、音色データ ＆ ⊿PCM構造体
;0xC000-		⊿PCM実体
;0xE000-		プログラムコード
;
;	NSF Headerは、MMLコンパイラが作る。
;

; ------------------------------------------------------------------------
; Define for C Langage
; ------------------------------------------------------------------------

	.export		_exit
	.export		__STARTUP__ : absolute = 1      ; Mark as startup
	.import		initlib, donelib, callmain
	.import		push0, _main, zerobss, copydata
	.import		ppubuf_flush

	; Linker generated symbols
	.import		__RAM_START__, __RAM_SIZE__
	.import		__SRAM_START__, __SRAM_SIZE__
	.import		__ROM0_START__, __ROM0_SIZE__
	.import		__STARTUP_LOAD__,__STARTUP_RUN__, __STARTUP_SIZE__
	.import		__CODE_LOAD__,__CODE_RUN__, __CODE_SIZE__
	.import		__RODATA_LOAD__,__RODATA_RUN__, __RODATA_SIZE__

	.include	"zeropage.inc"
	.include	"nes.inc"

	.include	"..\..\include\nsd.inc"

; ------------------------------------------------------------------------
; 変数
; ------------------------------------------------------------------------
.zeropage

_tmp:	.word	0		;Temp
_ptr:	.word	0		;Pointer

.bss

_eff:	.byte	0		;SE start number

; ------------------------------------------------------------------------
; Place the startup code in a special segment.
; ------------------------------------------------------------------------
.segment	"STARTUP"

start:
	sei
	cld

	lda	#$00
	sta	$2000
	sta	$2001

	sei			; setup the CPU and System-IRQ
	cld			;
	ldx	#$FF		;
	txs			;s <- 0xFF

	; Clear the BSS data
	jsr	zerobss

	; initialize data
	jsr	copydata

	; setup the stack
	lda	#<(__SRAM_START__ + __SRAM_SIZE__)
	sta	sp
	lda	#>(__SRAM_START__ + __SRAM_SIZE__)
	sta	sp+1		; Set argument stack ptr

	jsr	_init

	; Call module constructors
	jsr	initlib

	lda	#$80		; PPU NMI on
	sta	$2000

	; Push arguments and call main()
;	jsr	callmain

	; Call module destructors. This is also the _exit entry.
_exit:	jsr	donelib		; Run module destructors

	; Reset the NES
	jmp	start

; ------------------------------------------------------------------------
; System V-Blank Interupt
; ------------------------------------------------------------------------

nmi:
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

	; Interrupt exit
irq2:
irq1:
timerirq:
irq:
	rti

; ------------------------------------------------------------------------
; 
; ------------------------------------------------------------------------
.proc	_nsf_main

	jsr	_nsd_main

	rts
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
	sta	$0000,x	;Stackはクリアしない。
	sta	$0200,x
	sta	$0300,x
	sta	$0400,x
	sta	$0500,x
	sta	$0600,x
	sta	$0700,x
	sta	$6000,x
	sta	$6100,x
	sta	$6200,x
	sta	$6300,x
	sta	$6400,x
	sta	$6500,x
	sta	$6600,x
	sta	$6700,x
	sta	$6800,x
	sta	$6900,x
	sta	$6A00,x
	sta	$6B00,x
	sta	$6C00,x
	sta	$6D00,x
	sta	$6E00,x
	sta	$6F00,x
	sta	$7000,x
	sta	$7100,x
	sta	$7200,x
	sta	$7300,x
	sta	$7400,x
	sta	$7500,x
	sta	$7600,x
	sta	$7700,x
	sta	$7800,x
	sta	$7900,x
	sta	$7A00,x
	sta	$7B00,x
	sta	$7C00,x
	sta	$7D00,x
	sta	$7E00,x
	sta	$7F00,x
	inx
	bne	@L

	rts
.endproc

; ------------------------------------------------------------------------
; 
; ------------------------------------------------------------------------
.proc	_init

	jsr	_nsd_init

	lda	#<(__ROM0_START__)
	sta	_ptr
	lda	#>(__ROM0_START__)
	sta	_ptr+1		;_ptr = __ROM0_START__

	ldy	#0
	lda	(_ptr),y
	iny			;ax = BGM and SE's qty
	sta	_eff

	lda	(_ptr),y
	iny
	sta	_tmp
	lda	(_ptr),y
	tax			;ax = Pointer of ⊿PCM infomation Struct
	lda	_tmp

	;■■■	to do	⊿PCMのロード

	rts
.endproc

; ------------------------------------------------------------------------
; 
; ------------------------------------------------------------------------
.proc	_play_music

	pha
	lda	#<(__ROM0_START__)
	sta	_ptr
	lda	#>(__ROM0_START__)
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

.segment "VECTORS"
	.word	_nsf_init	; $fff0		MMLコンパイラが参照して、NSFヘッダーに埋め込む。
	.word	_nsf_main	; $fff2		MMLコンパイラが参照して、NSFヘッダーに埋め込む。
	.word	irq2		; $fff4 ?
	.word	irq1		; $fff6 ?
	.word	timerirq	; $fff8 ?
	.word	nmi		; $fffa vblank nmi
	.word	start		; $fffc reset
	.word	irq		; $fffe irq / brk

