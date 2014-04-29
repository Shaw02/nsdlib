;=======================================================================
;
;	NES Sound Driver Libraly		Library Functions
;
;=======================================================================

	.setcpu		"6502"

	.export		_nsd_play_se

	.import		_nsd_play
	.import		_nsd_stop_se

.ifdef	DPCMBank
	.import		_nsd_ptr_bank
.endif

	.import		nsd_work
	.importzp	nsd_work_zp

	.include	"nes.inc"
	.include	"nsddef.inc"
	.include	"macro.inc"


.code
;=======================================================================
;	void	__fastcall__	nsd_play_se(void* se );
;-----------------------------------------------------------------------
;<<Contents>>
;	Play start the SE
;<<Input>>
;	ax	pointer of SE
;<<Output>>
;	nothing
;=======================================================================
.proc	_nsd_play_se: near

	sta	__ptr
	stx	__ptr + 1

.ifdef	DPCMBank
	stx	__tmp + 1	;保存
	jsr	_nsd_ptr_bank
.endif

	lda	#nsd_flag::Disable
	ora	__flag
	sta	__flag			;サウンド処理禁止

	lda	__channel
	pha				;BGMから呼ばれるので、保存しておく。

	ldy	#0

	;-----------------------
	lda	(__ptr),y		; a = number of track
	iny
	shl	a, 1
	add	#nsd::TR_SE1
	sta	__channel

	;-----------------------
	lda	(__ptr),y		; a = 優先度
	iny
	and	#$03
	shl	a,2
	sta	__tmp + 1

	;-----------------------
	;Init the channel structure
SE1:	ldx	#nsd::TR_SE1
	cpx	__channel
	bcs	Exit

	lda	__Sequence_ptr + 1,x
	beq	@Done			;トラックが使われてなかったら無条件で再生

	lda	__flag
	and	#nsd_flag::Priority	; 0000-1100<2>
	cmp	__tmp + 1
	bcc	SE2			;優先度判定

@Done:
	lda	__flag
	and	#~nsd_flag::Priority
	ora	__tmp + 1
	sta	__flag			;優先度更新

	lda	(__ptr),y
	iny
	sta	__tmp
	lda	(__ptr),y
	ora	__tmp
	bne	@L			;ポインターが0だったら、トラック無し。
	iny
	jmp	SE2
@L:

	lda	#nsd_chflag::SE1
	jsr	_nsd_play
	iny

	lda	#$08
	sta	APU_PULSE2RAMP		;ch 2 only

	;-----------------------
SE2:	ldx	#nsd::TR_SE2
	cpx	__channel
	bcs	Exit

	lda	__Sequence_ptr + 1,x
	beq	@Done			;トラックが使われてなかったら無条件で再生

	lda	__flag
	and	#nsd_flag::Priority
	cmp	__tmp + 1
	bcc	Exit			;優先度判定

@Done:
	lda	(__ptr),y
	iny
	sta	__tmp
	lda	(__ptr),y
	ora	__tmp
	bne	@L			;ポインターが0だったら、トラック無し。
	iny
	jmp	Exit
@L:
	lda	#nsd_chflag::SE2
	jsr	_nsd_play
	iny

	;-----------------------
Exit:
	;Tempo is none

	pla
	sta	__channel

	lda	#~(nsd_flag::SE + nsd_flag::Disable)
	and	__flag
	sta	__flag			;SE & Main Enable

	rts
.endproc
