
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

	.import		nsd_work
	.importzp	nsd_work_zp

	.include	"zeropage.inc"
	.include	"nes.inc"
	.include	"nsd.inc"

; ------------------------------------------------------------------------
; Define for sound driver
; ------------------------------------------------------------------------

	.export		_init
	.export		_vblank_main

	;Sound driver function
	.import		_nsd_main
	.import		_nsd_snd_init
	.import		_nsd_snd_keyon
	.import		_nsd_snd_keyoff
	.import		_nsd_snd_voice
	.import		_nsd_snd_volume
	.import		_nsd_snd_frequency

; ------------------------------------------------------------------------
; 16 bytes INES header
; ------------------------------------------------------------------------

.segment        "HEADER"

	.byte	$4E,$45,$53,$4D,$1A	;00	"NESM",0x1A
	.byte	1			;05	Version
	.byte	7			;06	Music number
	.byte	1			;07	Start Music number
	.addr	$8000			;08	Load address
	.addr	_init			;0A	Init routine address
	.addr	_vblank_main		;0C	Sound driver main routine address
	.res	31,	$20
	.byte	0
	.res	31,	$20
	.byte	0
	.res	31,	$20
	.byte	0
	.word	$411A			;6E	60Hz
	.byte	0,0,0,0,0,0,0,0		;70	bank
	.byte	0,0,0,0,0,0,0,0		;78	

; ------------------------------------------------------------------------
; Place the startup code in a special segment.
; ------------------------------------------------------------------------

.segment	"STARTUP"

start:
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

	; Call module constructors
	jsr	initlib

	; Push arguments and call main()
	jsr	callmain

	; Call module destructors. This is also the _exit entry.
_exit:	jsr	donelib		; Run module destructors

	; Reset the NES
	jmp start

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
; Place the startup code in a special segment.
; ------------------------------------------------------------------------

.proc	_init
.bss
tmp:	.word	0
.segment	"STARTUP"
	sta	tmp

	;Sound Driver init
	jsr	_nsd_snd_init

	lda	tmp
	shl	a, 1
	sta	__channel

	lda	tmp
	jsr	_nsd_snd_voice

	lda	#$C0
	jsr	_nsd_snd_volume

	jsr	_nsd_snd_keyon

	rts
.endproc

.proc	_vblank_main
.bss
Freq:	.word	0
flag:	.byte	0
.segment	"STARTUP"

	lda	#$08
	sta	APU_PULSE1RAMP		; Pulse #1 Ramp Control Register (W)
	sta	APU_PULSE2RAMP		; Pulse #2 Ramp Control Register (W)

	lda	Freq + 1
	and	#$07
	sta	Freq + 1
	tax
	lda	Freq
	jsr	_nsd_snd_frequency
	lda	Freq
	clc
	adc	#1
	sta	Freq
	bcc	@L
	inc	Freq + 1
@L:
	rts
.endproc

; ------------------------------------------------------------------------
; hardware vectors
; ------------------------------------------------------------------------

.segment "VECTORS"

	.word	irq2		; $fff4 ?
	.word	irq1		; $fff6 ?
	.word	timerirq	; $fff8 ?
	.word	nmi		; $fffa vblank nmi
	.word	start		; $fffc reset
	.word	irq		; $fffe irq / brk

