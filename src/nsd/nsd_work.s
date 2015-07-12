;=======================================================================
;
;	NES Sound Driver & library (NSD.lib)	Work area
;
;-----------------------------------------------------------------------
;
;	Copyright (c) 2012 A.Watanabe (S.W.), All rights reserved.
;	For conditions of distribution and use, see copyright notice
;	  in "nsd.h" or "nsd.inc".
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

