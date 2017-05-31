;=======================================================================
;
;	.nsf ROM CODE for nsd.lib and nsc.exe
;
;				Programmed by A.Watanabe
;
;=======================================================================

	.export		__STARTUP__ : absolute = 1      ; Mark as startup

	; Linker generated symbols
	.import		__RAM_START__, __RAM_SIZE__
	.import		__SRAM_START__, __SRAM_SIZE__
	.import		__ROM0_START__, __ROM0_SIZE__, __ROM0_LAST__
	.import		__STARTUP_LOAD__,__STARTUP_RUN__, __STARTUP_SIZE__
	.import		__CODE_LOAD__,__CODE_RUN__, __CODE_SIZE__
	.import		__RODATA_LOAD__,__RODATA_RUN__, __RODATA_SIZE__

	.include	"nes.inc"
	.include	"nsd.inc"
	.include	"macro.inc"

.ifdef	DPCMBank
	.import		_nsd_irq
	.import		_nsd_ptr_bank
.endif

; ------------------------------------------------------------------------
; 	変数
; ------------------------------------------------------------------------

.bss

_eff:	.byte	0		;SE start number
_play:	.byte	0

; ------------------------------------------------------------------------
; 	256 bytes NSF header
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
	.addr	_nmi_main
	.res	32,	$0
	.res	32,	$0
	.res	32,	$0
	.word	$411A			;6E	60Hz
.ifdef	DPCMBank
	.byte	3,4,2,5,6,7,1,0		;70	
.else
	.byte	0,0,0,0,0,0,0,0		;70	bank
.endif
	.word	$4E20			;78
	.byte	0			;7A
	.byte	FDS_Flag + VRC6_Flag + VRC7_Flag + MMC5_Flag + N163_Flag + PSG_Flag
	.byte	0,0,0,0			;78

; ------------------------------------------------------------------------
.segment	"DRVINFO"
DRV_Name:	.byte	$4E, $53, $44, $4C, $20, $20
DRV_Version:	.byte	1
		.byte	28
		.byte	0,0,0,0,0,0,0,0

.ifdef	DPCMBank
.segment	"STARTUP"
DRV_Name2:	.byte	$4E, $53, $44, $4C, $20, $20
DRV_Version2:	.byte	1
		.byte	28
.endif

; ------------------------------------------------------------------------
; 	Init for TNS-HFC Series
; ------------------------------------------------------------------------
.segment	"STARTUP"
.ifdef	DPCMBank
.proc	_nsf_init2

	;リセット連打によるハング防止

	sei
	cld

@wait1:	lda	$2002
	bpl	@wait1
@wait2:	lda	$2002
	bpl	@wait2

	;Bank change (CODE)
	lda	#$00
	sta	$5FFF

	jmp	$0103

.endproc
.endif

; ------------------------------------------------------------------------
; 	Init
; ------------------------------------------------------------------------
.proc	_nsf_init

	sei
	cld

	pha

	lda	#nsd_flag::BGM + nsd_flag::SE
	sta	__flag		;BGM, SE処理を禁止（RAM未初期化対策）

.ifdef	DPCMBank

	;Bank change (CODE)
	lda	#$00
	sta	$5FFF

	;まず、JMP命令を書き込み
	lda	#$4C
	sta	$0100
	sta	$0103

	;TNS-HFCシリーズが書き換えたVECTORのコピー
	lda	$FFFA
	sta	$0101
	lda	$FFFB
	sta	$0102

	lda	$FFFC
	sta	$0104
	lda	$FFFD
	sta	$0105

	;Bank change (CODE)
	lda	#$00
	sta	$5FFA

	lda	#$02
	sta	$5FFF

	;Bank change (MUS)
	ldx	#$03
	stx	$5FF8
	inx
	stx	$5FF9

.endif

.ifdef	ExRAM

	;Bank change (MUS)
	ldx	#$03
	stx	$5FF8
	inx
	stx	$5FF9

	;コピー
	;Copy bank
	ldx	#0
Loop:
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
	jne	Loop

	;Bank change
	ldx	#5
	stx	$5FF8
	inx
	stx	$5FF9
	inx
	stx	$5FFA
	inx
	stx	$5FFB

.endif

	;Memory clear
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
	inx
	bne	@L

	;変数設定
	sta	_play		;フレームオーバー防止用変数

	lda	#$40
	sta	APU_PAD2		; SOFTCLK (RW)

	;nsd.lib初期化
	jsr	_nsd_init

_Play:
	;⊿PCMデータの設定
.ifdef	DPCMBank
	lda	#$00
	sta	__ptr
	sta	__ptr+1		;__ptr = __ROM0_START__
	jsr	_nsd_ptr_bank
.else
	lda	#<(__ROM0_LAST__)
	sta	__ptr
	lda	#>(__ROM0_LAST__)
	sta	__ptr+1		;__ptr = __ROM0_START__
.endif
	ldy	#0
	lda	(__ptr),y
	iny			;ax = BGM and SE's qty
	iny
	sta	_eff

	lda	(__ptr),y
	iny
	sta	__tmp
	lda	(__ptr),y
	tax			;ax = Pointer of ⊿PCM infomation Struct
	lda	__tmp
	jsr	_nsd_set_dpcm

	;曲データの設定
.ifdef	DPCMBank
	cli
	lda	#$00
	sta	__ptr
	sta	__ptr+1		;__ptr = __ROM0_START__
	jsr	_nsd_ptr_bank
.else
	lda	#<(__ROM0_LAST__)
	sta	__ptr
	lda	#>(__ROM0_LAST__)
	sta	__ptr+1		;__ptr = __ROM0_START__
.endif
	pla

	cmp	_eff
	php
	clc
	adc	#2
	asl	a
	tay			; y = _mus * 2
	lda	(__ptr),y
	sta	__tmp
	iny
	lda	(__ptr),y	; ax = Pointer of BGM/SE
	tax
	lda	__tmp
	plp

	bcs	@L
	jsr	_nsd_play_bgm
	jmp	@E
@L:	jsr	_nsd_play_se
@E:	rts
.endproc

; ------------------------------------------------------------------------
; 	実機ROM用	IRQ	(DPCM)
; ------------------------------------------------------------------------
.ifdef	DPCMBank

.zeropage

_IRQ_SVA:	.byte	0
_IRQ_SVX:	.byte	0
_IRQ_SVY:	.byte	0

.code
.proc	_irq_main

	sta	_IRQ_SVA	;save register
	stx	_IRQ_SVX
	sty	_IRQ_SVY

	jsr	_nsd_irq

	ldy	_IRQ_SVY
	ldx	_IRQ_SVX
	lda	_IRQ_SVA	;save register

	rti
.endproc
.endif

; ------------------------------------------------------------------------
; 	実機ROM用	NMI	(Vblank)
; ------------------------------------------------------------------------
.zeropage

_NMI_SVA:	.byte	0
_NMI_SVX:	.byte	0
_NMI_SVY:	.byte	0

.code
.proc	_nmi_main

	pha			;register push

	inc	_play
	lda	_play
	cmp	#1
	bne	@exit		;演奏中だったら終わり。

	stx	_NMI_SVX
	sty	_NMI_SVY
@loop:
	jsr	_nsd_main
	dec	_play
	bne	@loop		;frame over?

	ldy	_NMI_SVY
	ldx	_NMI_SVX

@exit:
	pla			;register pop

	rts
.endproc

; ------------------------------------------------------------------------
; 	Hardware vectors
; ------------------------------------------------------------------------

.ifdef	DPCMBank

.segment	"VECTORS"
	.word	$0100		
	.word	_nsf_init2	
	.word	_irq_main	; $fffe irq / brk

.segment	"VECTORS2"
	.word	$0100		
	.word	_nsf_init2	
	.word	_irq_main	; $fffe irq / brk

.endif
