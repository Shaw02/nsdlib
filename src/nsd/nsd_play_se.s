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

	pha
	lda	#nsd_flag::Disable
	ora	__flag
	sta	__flag			;サウンド処理禁止
	pla

	sta	__ptr
	stx	__ptr + 1

.ifdef	DPCMBank
	stx	__tmp + 1		;保存
	jsr	_nsd_ptr_bank
.endif


	lda	__channel
	pha				;BGMから呼ばれるので、保存しておく。

	ldy	#0

	;-----------------------
Channel:
	lda	(__ptr),y		; a = number of track
	iny
	shl	a, 1
	add	#nsd::TR_SE
	sta	__channel

	;-----------------------
Priority:
	lda	(__ptr),y		;
	iny
	and	#$03
	shl	a,2
	sta	__tmp			; __tmp  = 優先度

	lda	#nsd_flag::SE
	bit	__flag			;
	bne	@S			;1(SE再生無し)だったら、再生可

	ldx	#0			;0(SE再生中)だったら、優先度比較
	lda	__flag
	and	#nsd_flag::Priority	; 0000-1100<2>
	cmp	__tmp
	bcc	@L			;優先度判定

@S:
	lda	__flag
	and	#<~nsd_flag::Priority
	ora	__tmp
	sta	__flag			;優先度更新
	ldx	#1			;今回の効果音を優先する
@L:
	stx	__tmp			; ＝ 0 だったら前回の方が優先度高い。

	;-----------------------
	;Init the channel structure
	ldx	#nsd::TR_SE

Loop:
	cpx	__channel
	bcs	Loop_End

	lda	__tmp
	bne	@Done			;優先度が高かったら無条件で再生

	lda	__Sequence_ptr + 1,x
	beq	@Done1			;トラックが使われてなかったら無条件で再生

	iny
	iny
	jmp	@None

@Done1:
	lda	__tmp
@Done:
	pha				;効果音優先度情報の保存
	lda	(__ptr),y
	iny
	sta	__tmp
	ora	(__ptr),y
	beq	@L
	jsr	_nsd_play

	lda	#$08
.ifdef	SE
	cpx	#nsd::TR_SE_Pluse1
	bne	@L0
	sta	APU_PULSE1RAMP
@L0:
.endif
	cpx	#nsd::TR_SE_Pluse2
	bne	@L1
	sta	APU_PULSE2RAMP
@L1:

@L:
	iny
	pla
	sta	__tmp			;効果音優先度情報の復帰

@None:
	inx
	inx
	cpx	#nsd::TR_SE + nsd::SE_Track * 2
	bcc	Loop
Loop_End:


	;-----------------------
	;無条件初期化

.ifdef	SE
	lda	#$FF
	sta	__se_tri_time		;三角波 時間
.endif

	;-----------------------

	pla
	sta	__channel

	lda	#<~(nsd_flag::SE + nsd_flag::Disable)
	and	__flag
	sta	__flag			;SE & Main Enable

	rts
.endproc
