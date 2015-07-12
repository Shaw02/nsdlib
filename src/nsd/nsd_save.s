;=======================================================================
;
;	NES Sound Driver & library (NSD.lib)	Library Functions
;
;-----------------------------------------------------------------------
;
;	Copyright (c) 2012 A.Watanabe (S.W.), All rights reserved.
;	For conditions of distribution and use, see copyright notice
;	  in "nsd.h" or "nsd.inc".
;
;=======================================================================

	.setcpu		"6502"

	.export		_nsd_save

	.import		nsd_work
	.importzp	nsd_work_zp

	.import		_nsd_pause_bgm
	.import		_nsd_stop_se

	.include	"nes.inc"
	.include	"nsddef.inc"
	.include	"macro.inc"

;=======================================================================
;	void	__fastcall__	nsd_save(void* buff );
;-----------------------------------------------------------------------
;<<Contents>>
;	Pause and Save state
;<<Input>>
;	ax	pointer of save buffer
;<<Output>>
;	nothing
;=======================================================================
.code
.proc	_nsd_save: near

	pha
	txa
	pha				;ï€ë∂

	jsr	_nsd_pause_bgm
	jsr	_nsd_stop_se

	pla
	sta	__tmp + 1
	pla
	sta	__tmp			;à¯êîÇ __tmp Ç÷

COPY_ZP:
	ldx	#.sizeof(NSD_work_zp)
	ldy	#0
@L:
	lda	nsd_work_zp,y
	sta	(__tmp),y
	iny
	dex
	bne	@L

COPY_WORK:
	lda	__tmp
	add	#.sizeof(NSD_work_zp)
	sta	__tmp
	bcc	@L2
	inc	__tmp + 1
@L2:

	LDAX	#nsd_work
	STAX	__ptr

	LDAX	#~.sizeof(NSD_work)
	sta	__channel

	ldy	#0
	beq	LOOP_CNT

LOOP:
	lda	(__ptr),y
	sta	(__tmp),y

	iny
	bne	LOOP_CNT
	inc	__ptr + 1
	inc	__tmp + 1

LOOP_CNT:
	inc	__channel
	bne	LOOP
	inx
	bne	LOOP

	rts
.endproc
