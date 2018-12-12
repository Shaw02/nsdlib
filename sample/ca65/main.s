
	.export		_main

	.import		_Test_DPCMinfo
	.import		_Test_BGM0

	.include	"sample.inc"

; ------------------------------------------------------------------------
; play
; ------------------------------------------------------------------------

.rodata

;Address of D-PCM information
dpcm_info:	.addr	_Test_DPCMinfo

;Address of BGM Sequence
bgm_00:		.addr	_Test_BGM0


.code
.proc	_bgm_play

	;---------------
	;Set D-PCM information to nsd.lib
	lda	dpcm_info
	ldx	dpcm_info + 1
	jsr	_nsd_set_dpcm

	;---------------
	;Play BGM
	lda	bgm_00
	ldx	bgm_00 + 1
	jsr	_nsd_play_bgm

	rts

.endproc

; ------------------------------------------------------------------------
; main
; ------------------------------------------------------------------------
.code

.proc	_main

	lda	#$0
	jsr	_bgm_play



LOOP:
	lda	__cc			;←この変数は、フレーム毎にカウントアップする。
	pha				;現在のフレームを取得
					;（ある程度だけ、フレームオーバーを回避する）










	pla				;ループ開始時のフレームカウント値を取得

	;---------------
	;フレーム更新待ち
Wait_Next_Flame:
	cmp	__cc
	bne	LOOP			;次のフレームになったら先頭へ
	beq	Wait_Next_Flame


	rts

.endproc
