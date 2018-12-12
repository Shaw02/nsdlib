;=======================================================================
;
;		Start-up program
;
;						by A.Watanabe
;
;=======================================================================

	.export		exit

	.import		_main
	.import		NMI_main
	.import		IRQ_main

	.import		ppudrv_init
	.import		_nsd_init

	.import		Bank_Change_Prg

	; Linker generated symbols
	.import		__STACK_START__,	__STACK_SIZE__
	.import		__RAM_START__,		__RAM_SIZE__
	.import		__SRAM_START__,		__SRAM_SIZE__
	.import		__ROM0_START__,		__ROM0_SIZE__
	.import		__STARTUP_LOAD__,	__STARTUP_RUN__,	__STARTUP_SIZE__
	.import		__CODE_LOAD__,		__CODE_RUN__,		__CODE_SIZE__
	.import		__RODATA_LOAD__,	__RODATA_RUN__,		__RODATA_SIZE__
	.import		__DATA_LOAD__,		__DATA_RUN__,		__DATA_SIZE__

	.include	"sample.inc"


; ------------------------------------------------------------------------
; Place the startup code in a special segment.

.segment	"STARTUP"

start:

; setup the CPU and System and Sound Driver nsd.lib

	sei
	cld

	DISP_OFF

;===============================
;	メモリ初期化
;===============================
Clear_Memory:

	lda	#0
	ldx	#<(__STACK_START__ + __STACK_SIZE__ - 1)
	txs
	tax
@L0:
	sta	$0000,x
;	sta	$0100,x
;	sta	$0200,x
;	sta	$0300,x
;	sta	$0400,x
;	sta	$0500,x
;	sta	$0600,x
;	sta	$0700,x

	inx
	bne	@L0

;===============================
;	ワーク領域初期化
;===============================
Work_Init:

	;初期化済データのRAM転送

	LDAX	#__DATA_LOAD__		; Source pointer
	STAX	__r0

	LDAX	#__DATA_RUN__	 	; Source pointer
	STAX	__r2

	ldx	#<~__DATA_SIZE__
	lda	#>~__DATA_SIZE__	; Use -(__DATASIZE__+1)
	sta	__r4
	ldy	#$00
	beq	@L1
; Copy loop

@L2:	lda	(__r0),y
	sta	(__r2),y
	iny
	bne	@L1
	inc	__r0 + 1
	inc	__r2 + 1

; Bump the high counter byte

@L1:	inx
	bne	@L2
@L3:	inc	__r4
	bne	@L2

;===============================
;	サウンド初期化
;===============================
Sound_Init:

	LDA	#$40
	STA	APU_PAD2

	; Call initialize sound driver
	jsr	_nsd_init

;===============================
;	画面初期化
;===============================
;Disp_Init:

	; Call initialize PPU
	jsr	_ppu_init

;===============================
;	メインルーチン呼出し
;===============================
; Push arguments and call main()

	jsr	_main

; Call module destructors. This is also the _exit entry.

exit:

; Reset the NES

	jmp	start

; ------------------------------------------------------------------------
; Init PPU
; ------------------------------------------------------------------------
.proc	_ppu_init

	;---------------
	; PPU Control
	lda	#%10101000		;V-Blank NMI: enable
	sta	PPU_CTRL1

	lda	#%00011110
	sta	PPU_CTRL2

	;---------------
	; Wait for vblank
@wait:	lda	PPU_STATUS
	bpl	@wait

	;---------------
	; reset scrolling
	lda	#0
	sta	PPU_VRAM_ADDR1
	sta	PPU_VRAM_ADDR1

	;---------------
	; Make all sprites invisible
	lda	#$00
	ldy	#$f0
	sta	PPU_SPR_ADDR
	ldx	#$40
@loop:	sty	PPU_SPR_IO
	sta	PPU_SPR_IO
	sta	PPU_SPR_IO
	sty	PPU_SPR_IO
	dex
	bne	@loop

	rts

.endproc

; ------------------------------------------------------------------------
; hardware vectors
; ------------------------------------------------------------------------

.segment "VECTORS"

	.word	NMI_main	; $fffa vblank nmi
	.word	start		; $fffc reset
	.word	IRQ_main	; $fffe irq / brk
