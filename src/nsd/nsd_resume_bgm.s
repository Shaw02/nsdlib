;=======================================================================
;
;	NES Sound Driver Libraly		Library Functions
;
;=======================================================================

	.setcpu		"6502"

	.export		_nsd_resume_bgm

	.import		nsd_work
	.importzp	nsd_work_zp

	.include	"nes.inc"
	.include	"nsddef.inc"
	.include	"macro.inc"


;=======================================================================
;	void	__fastcall__	nsd_resume_bgm(void );
;-----------------------------------------------------------------------
;<<Contents>>
;	Resume the BGM
;<<Input>>
;	nothing
;<<Output>>
;	nothing
;=======================================================================
.proc	_nsd_resume_bgm: near

	lda	#~nsd_flag::BGM
	and	__flag
	sta	__flag			;BGM Enable

	lda	__sweep_ch1
	sta	APU_PULSE1RAMP
	lda	__sweep_ch2
	sta	APU_PULSE2RAMP

	lda	#$00
	sta	__apu_frequency1
	sta	__apu_frequency2
	sta	__apu_frequency3

.ifdef	MMC5
	sta	__mmc5_frequency1
	sta	__mmc5_frequency2
.endif

	;-----------------------
	;Init the channel structure
	ldx	#nsd::TR_BGM1
Loop:
	;é¸îgêîê›íËÇïKÇ∏åƒÇ‘ÇÊÇ§Ç…ÅB
	lda	#$FF
	sta	__frequency,x
	sta	__frequency + 1,x

	inx						;[2]2
	inx						;[2]4
	cpx	#nsd::TR_BGM1 + nsd::BGM_Track * 2		;[2]6
	bcc	Loop					;[2]8	Total = 10 clock

	lda	__chflag + nsd::TR_BGM3
	and	#nsd_chflag::KeyOff
	cmp	#nsd_chflag::KeyOff
	bne	@E
	lda	#$FF
	sta	APU_TRICTRL1
@E:
	rts
.endproc
