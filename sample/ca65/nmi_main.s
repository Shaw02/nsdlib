
	.export		NMI_main

	.include	"sample.inc"


.segment	"LOWCODE"
;===============================================================
;			nmi_main(void);
;---------------------------------------------------------------
;	Summary :		NMI main Routine
;	Arguments :		None
;	Return :		None
;	Modifies :		None
;	Useage :		Please write this address to interrupt vector
;---------------------------------------------------------------
;	《メモ》
;	・NMI割り込みは、PPU関連の操作を先にやる
;	　PPUへのアクセスは、垂直同期帰還中にしかできないため。
;	・キーの読み込みも、NMI割り込みで行う。
;	　キー読み込み中に音源ドライバーが割り込むと、キー読み込みに失敗するため。
;===============================================================
.proc	NMI_main

;---------------------------------------
;register push
;---------------------------------------
	pha
	tya
	pha
	txa
	pha

;---------------------------------------
;call the callback routine
;---------------------------------------

	;---------------
	;VRAM更新コードの呼出し
;	lda	_NMI_CallBack + 1
;	beq	CallBack_Ret
;	jsr	jmp_NMI_CallBack

CallBack_Ret:

;---------------------------------------
; Call sound driver main routine
;---------------------------------------

	;---------------
	;サウンドドライバ呼出し

	jsr	_nsd_main

;---------------------------------------
; Read joy-pad
;---------------------------------------

Key:
	;---------------
	;前回のキーをコピー
	lda	__PAD0
	sta	__PAD0_OLD
	lda	__PAD1
	sta	__PAD1_OLD

	;---------------
	;変数初期化
	ldx	#$01
	stx	__PAD0
	stx	__PAD1
	stx	__n0
	stx	__n1
	stx	__n2
	stx	__n3

	;---------------
	;１回目読み込み
	stx	APU_PAD1	;4021 P/S_CONTROL端子 <= High (PARALLEL operation)
	dex
	stx	APU_PAD1	;4021 P/S_CONTROL端子 <= Low  (SERIAL   operation)

@loop1:
	lda	APU_PAD1
	lsr	a
	rol	__n0		;PAD1
	lsr	a
	rol	__n2		;PAD3（拡張PAD）

	lda	APU_PAD2
	lsr	a
	rol	__n1		;PAD2
	bcc	@loop1

	;---------------
	;２回目読み込み	（※⊿PCM ＆ チャタリングによるノイズ除去のため）
	inx
	stx	APU_PAD1	;4021 P/S_CONTROL端子 <= High (PARALLEL operation)
	dex
	stx	APU_PAD1	;4021 P/S_CONTROL端子 <= Low  (SERIAL   operation)

@loop2:
	lda	APU_PAD1
	lsr	a
	rol	__PAD0		;PAD1
	lsr	a
	rol	__n3		;PAD3（拡張PAD）

	lda	APU_PAD2
	lsr	a
	rol	__PAD1		;PAD2
	bcc	@loop2

	;---------------
	;⊿PCM ＆ チャタリングのノイズ除去 ＆ 立上がり・立下り検出

	;-------
	;PAD3（拡張PAD）
	lda	__n3
	and	__n2		;ノイズ除去（１回目、２回目共に`H'の場合に、`H'）
	sta	__n2

	;-------
	;PAD1 & PAD3
	lda	__n0
	and	__PAD0		;ノイズ除去（１回目、２回目共に`H'の場合に、`H'）
	ora	__n2		;PAD3（拡張PAD）の読み込みを、PAD1にマージ
	sta	__PAD0

	eor	__PAD0_OLD	;エッジ検出
	tax
	and	__PAD0
	sta	__PAD0_TRG	;立上がり
	txa
	and	__PAD0_OLD
	sta	__PAD0_REL	;立下り

	;-------
	;PAD2
	lda	__n1
	and	__PAD1		;ノイズ除去（１回目、２回目共に`H'の場合に、`H'）
	sta	__PAD1

	eor	__PAD1_OLD	;エッジ検出
	tax
	and	__PAD1
	sta	__PAD1_TRG	;立上がり
	txa
	and	__PAD1_OLD
	sta	__PAD1_REL	;立下り



;---------------------------------------
; Count-up
;---------------------------------------
Count:
@cc:
	inc	__cc
	lda	__cc
	cmp	#60
	bne	@exit
	lda	#0
	sta	__cc

@ss:
	ldx	__ss
	cpx	#$59
	beq	@ss_1
	inx
	stx	__ss
	txa
	and	#$0F
	cmp	#$0A
	bne	@exit
	txa
	add	#6
	sta	__ss
	bne	@exit
@ss_1:
	lda	#0
	sta	__ss

@mm:
	ldx	__mm
	cpx	#$59
	beq	@mm_1
	inx
	stx	__mm
	txa
	and	#$0F
	cmp	#$0A
	bne	@exit
	txa
	add	#6
	sta	__mm
	bne	@exit
@mm_1:
	lda	#0
	sta	__mm

@hh:
	inc	__hh

@exit:

;---------------------------------------
;register pop
;---------------------------------------
	pla
	tax
	pla
	tay
	pla

	rti
.endproc

.proc	jmp_NMI_CallBack
	jmp	(_NMI_CallBack)
.endproc

