
;************************************************************************
;*		スプライト						*
;************************************************************************

;１要素
.Struct	SPR_ONE
	pty	.byte
	num	.byte
	att	.byte
	ptx	.byte
.Endstruct

;全体
.Struct	SPR_TBL
	spr	.tag	SPR_ONE		64
.Endstruct

