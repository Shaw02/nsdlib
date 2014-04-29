;=======================================================================
;
;	NES Sound Driver Libraly		Library Functions
;
;=======================================================================

	.setcpu		"6502"

	.export		_nsd_set_dpcm

	.import		nsd_work
	.importzp	nsd_work_zp

	.include	"nes.inc"
	.include	"nsddef.inc"
	.include	"macro.inc"


.code
;=======================================================================
;	void	__fastcall__	nsd_set_dpcm(nsd_dpcm* dpcm);
;-----------------------------------------------------------------------
;<<Contents>>
;	Set the Delta PCM information.
;<<Input>>
;	ax	Pointer of the Delta PCM information.
;<<Output>>
;	nothing
;=======================================================================
.proc	_nsd_set_dpcm: near

	sta	__dpcm_info + 0
	stx	__dpcm_info + 1

	rts
.endproc
