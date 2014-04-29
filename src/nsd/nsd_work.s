;=======================================================================
;
;	NES Sound Driver Libraly		Work area
;
;=======================================================================

	.include	"nes.inc"
	.include	"nsddef.inc"
	.include	"macro.inc"


	.exportzp	nsd_work_zp
	.export		nsd_work

.zeropage

nsd_work_zp:	.tag	NSD_work_zp	;zero-page area

.bss

nsd_work:	.tag	NSD_work	;ram area

