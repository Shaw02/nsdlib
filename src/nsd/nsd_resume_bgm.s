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
	.include	"nsd.inc"

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

	rts
.endproc
