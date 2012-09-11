
	.setcpu		"6502"

	.export		_nsd_main

	.importzp	nsd_work_zp
	.import		nsd_work

	.import		nsd_sequence
	.import		nsd_envelop

	.include	"nes.inc"
	.include	"nsd.inc"

.code

;=======================================================================
;	void    __fastcall__    nsd_main(void );
;-----------------------------------------------------------------------
;<<Contents>>
;	main process of BGM and SE
;<<Input>>
;	nothing
;<<Output>>
;	nothing
;=======================================================================
.proc	_nsd_main: near

	lda	#nsd_flag::BGM
	bit	__flag
	bne	BGM_Exit		;BGM disable ?

	;-------------------------------
	;Tempo
	lda	__Tempo_ctr
	add	__Tempo
	sta	__Tempo_ctr
	bcc	BGM_SEQ_Exit

	;-------------------------------
	;BGM
BGM_Begin:
	.repeat	nsd::BGM_Track, I
		ldx	#I*2
		jsr	nsd_sequence
	.endrepeat

	lda	__Tempo_ctr
	sub	#150
	sta	__Tempo_ctr
	cmp	#106
	bcc	BGM_Begin

BGM_SEQ_Exit:

	;-------
	;Envelop
	.repeat	nsd::BGM_Track, I
		ldx	#I*2
		jsr	nsd_envelop
	.endrepeat

BGM_Exit:


	;-------------------------------
	;SE

	lda	#nsd_flag::SE
	bit	__flag
	bne	SE_Exit			;SE disable ?

	.repeat	nsd::SE_Track, I
		ldx	#I * 2 + nsd::BGM_Track * 2
		jsr	nsd_sequence
	.endrepeat

	;-------
	;Envelop
	.repeat	nsd::SE_Track, I
		ldx	#I * 2 + nsd::BGM_Track * 2
		jsr	nsd_envelop
	.endrepeat

SE_Exit:

	;-------------------------------
	;Exit
	rts
.endproc
