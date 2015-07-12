;===============================================================================|
;										|
;		NES Sound Driver & library	(NSD.lib)			|
;										|
;-------------------------------------------------------------------------------|
;										|
;  Copyright (c) 2012 A.Watanabe (S.W.)						|
;  All rights reserved.								|
;										|
;  Redistribution and use in source and binary forms, with or without		|
;  modification, are permitted provided that the following conditions		|
;  are met:									|
;										|
;    1. Redistributions of source code must retain the above copyright		|
;       notice, this list of conditions and the following disclaimer.		|
;										|
;    2. Redistributions in binary form must reproduce the above copyright	|
;       notice, this list of conditions and the following disclaimer in the	|
;       documentation and/or other materials provided with the distribution.	|
;										|
;  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS		|
;  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT		|
;  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR	|
;  A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER	|
;  OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,	|
;  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,		|
;  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR		|
;  PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF	|
;  LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING		|
;  NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS		|
;  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.			|
;										|
;===============================================================================|

	.include	"nsddef.inc"

;****************************************************************
;*		Function of Library				*
;****************************************************************
;-----------------------------------------------
;	void	__fastcall__	nsd_main(void);
;-----------------------------------------------
;	Summary :		main routine of this sound driver
;	Arguments :		None
;	Return :		None
;	Modifies :		A X Y
;	Useage :		Call by 60[Hz].  e.g. V-Blank
;-----------------------------------------------
	.import		_nsd_main
	.import		_nsd_main_bgm
	.import		_nsd_main_se


;-----------------------------------------------
;	void	__fastcall__	nsd_init(void);
;-----------------------------------------------
;	Summary :		Initraize this sound driver.
;	Arguments :		None
;	Return :		None
;	Modifies :		A X Y
;	Useage :		Call when start up.
;-----------------------------------------------
	.import		_nsd_init


;-----------------------------------------------
;	void	__fastcall__	nsd_set_dpcm(nsd_dpcm* dpcm);
;-----------------------------------------------
;	Summary :		Set the Delta PCM information.
;	Arguments :	AX	Pointer of the Delta PCM information.
;	Return :		None
;	Modifies :		A X Y
;-----------------------------------------------
	.import		_nsd_set_dpcm


;-----------------------------------------------
;	void	__fastcall__	nsd_play_bgm(void* ptr);
;-----------------------------------------------
;	Summary :		Play the BGM
;	Arguments :	AX	Pointer of the BGM data.
;	Return :		None
;	Modifies :		A X Y
;-----------------------------------------------
	.import		_nsd_play_bgm


;-----------------------------------------------
;	void	__fastcall__	nsd_stop_bgm(void);
;-----------------------------------------------
;	Summary :		Stop the BGM
;	Arguments :		None
;	Return :		None
;	Modifies :		A X Y
;-----------------------------------------------
	.import		_nsd_stop_bgm


;-----------------------------------------------
;	void	__fastcall__	nsd_pause_bgm(void);
;-----------------------------------------------
;	Summary :		Pause the BGM
;	Arguments :		None
;	Return :		None
;	Modifies :		A X Y
;-----------------------------------------------
	.import		_nsd_pause_bgm


;-----------------------------------------------
;	void	__fastcall__	nsd_resume_bgm(void);
;-----------------------------------------------
;	Summary :		Resume the BGM
;	Arguments :		None
;	Return :		None
;	Modifies :		A X Y
;-----------------------------------------------
	.import		_nsd_resume_bgm


;-----------------------------------------------
;	void	__fastcall__	_nsd_save(void* buff);
;-----------------------------------------------
;	Summary :		Save state and pause
;	Arguments :	AX	Pointer of Save buff
;	Return :		None
;	Modifies :		A X Y
;-----------------------------------------------
	.import		_nsd_save


;-----------------------------------------------
;	void	__fastcall__	_nsd_load(void* buff);
;-----------------------------------------------
;	Summary :		Pause and Load state
;	Arguments :	AX	Pointer of Save buff
;	Return :		None
;	Modifies :		A X Y
;-----------------------------------------------
	.import		_nsd_load


;-----------------------------------------------
;	void	__fastcall__	nsd_play_se(void* ptr);
;-----------------------------------------------
;	Summary :		Play the SE
;	Arguments :	AX	Pointer of the SE data.
;	Return :		None
;	Modifies :		A X Y
;-----------------------------------------------
	.import		_nsd_play_se


;-----------------------------------------------
;	void	__fastcall__	nsd_stop_se(void);
;-----------------------------------------------
;	Summary :		Stop the SE
;	Arguments :		None
;	Return :		None
;	Modifies :		A X Y
;-----------------------------------------------
	.import		_nsd_stop_se



;-----------------------------------------------
;	Work area of nsd.lib
;-----------------------------------------------
	.importzp	nsd_work_zp
	.import		nsd_work



