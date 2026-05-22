;=======================================================================
;
;	NES Sound Driver & library (NSD.lib)	Library Functions
;
;-----------------------------------------------------------------------
;
;	Copyright (c) 2012 A.Watanabe (S.W.), All rights reserved.
;	For conditions of distribution and use, see copyright notice
;	  in "nsd.h" or "nsd.inc".
;
;=======================================================================

	.setcpu		"6502"

	.export		_nsd_set_master_volume

	.import		nsd_work
	.importzp	nsd_work_zp

	.include	"nes.inc"
	.include	"nsddef.inc"
	.include	"macro.inc"


.code
;=======================================================================
;	void	__fastcall__	_nsd_set_master_volume(char master_volume);
;-----------------------------------------------------------------------
;<<Contents>>
;	Set the Master Volume
;<<Input>>
;	a	Master Volume
;<<Output>>
;	nothing
;=======================================================================
.proc	_nsd_set_master_volume: near

	and	#$0F
	sta	__master_volume

	rts
.endproc
