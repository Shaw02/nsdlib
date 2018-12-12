
	.export		IRQ_main

	.include	"sample.inc"


.segment	"LOWCODE"
;===============================================================
;			IRQ_main(void);
;---------------------------------------------------------------
;	Summary :		IRQ main routine
;	Arguments :		None
;	Return :		None
;	Modifies :		None
;	Useage :		Please write this address to interrupt vector
;===============================================================
.proc	IRQ_main			;[7]

;---------------------------------------
;register push
;---------------------------------------
	pha				;[3]10
	tya				;[2]12
	pha				;[3]15
	txa				;[2]17
	pha				;[3]20

;---------------------------------------
;call the callback routine
;---------------------------------------

;	lda	_IRQ_CallBack + 1	;[4]24
;	beq	CallBack_Ret		;[2]26
;	jsr	jmp_IRQ_CallBack	;[6]32

CallBack_Ret:

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

.proc	jmp_IRQ_CallBack
	jmp	(_IRQ_CallBack)		;[5]37 clock
.endproc
