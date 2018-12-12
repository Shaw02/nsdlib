
	.setcpu		"6502"

	.include	"nes.inc"

	.include	"nsd.inc"

	.include	"struct.inc"
	.include	"macro.inc"
	.include	"work.inc"

;---------------------------------------
;	システムプログラム中の定数を定義します。
;---------------------------------------

.scope	sysdef

;PPU
PPU_Ctrl1_set		= PPU_CTRL1_VBLANK | PPU_CTRL1_SPTBL
PPU_Ctrl2_set		= PPU_CTRL2_SPDISP | PPU_CTRL2_BGDISP | PPU_CTRL2_SPCLIP | PPU_CTRL2_BGCLIP

.endscope




;---------------------------------------
;	ＩＯ
;---------------------------------------
; $2000 PPU Control Register 0
PPU_CTRL1_VBLANK	= $80
PPU_CTRL1_SLAVE		= $40
PPU_CTRL1_SP16		= $20
PPU_CTRL1_BGTBL		= $10
PPU_CTRL1_SPTBL		= $08
PPU_CTRL1_INC32		= $04
PPU_CTRL1_NAMETBL	= $03

; $2001 PPU Control Register 1
PPU_CTRL2_BGCOLOR	= $E0
PPU_CTRL2_SPDISP	= $10
PPU_CTRL2_BGDISP	= $08
PPU_CTRL2_SPCLIP	= $04
PPU_CTRL2_BGCLIP	= $02
PPU_CTRL2_COLORMODE	= $01

PAD_RIGHT	= $01
PAD_LEFT	= $02
PAD_DOWN	= $04
PAD_UP		= $08
PAD_START	= $10
PAD_SELECT	= $20
PAD_B		= $40
PAD_A		= $80
