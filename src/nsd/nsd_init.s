;=======================================================================
;
;	NES Sound Driver Libraly		Library Functions
;
;=======================================================================

	.setcpu		"6502"

	.export		_nsd_init

	.import		_nsd_stop_bgm
	.import		_nsd_stop_se

	.import		_nsd_snd_init

	.import		nsd_work
	.importzp	nsd_work_zp

	.include	"nes.inc"
	.include	"nsd.inc"

.code

;=======================================================================
;	void	__fastcall__	nsd_init(void );
;-----------------------------------------------------------------------
;<<Contents>>
;	Initialize the sound driver
;<<Input>>
;	nothing
;<<Output>>
;	nothing
;=======================================================================
.proc	_nsd_init: near

	jsr	_nsd_stop_bgm
	jsr	_nsd_stop_se
	jsr	_nsd_snd_init

	rts
.endproc
