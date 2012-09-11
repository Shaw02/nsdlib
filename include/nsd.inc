;=======================================================================|
;									|
;		NES Sound Driver & library				|
;				define file				|
;					for Assemble Language (ca65)	|
;									|
;					Programmed by			|
;						A.Watanabe		|
;									|
;=======================================================================|

;****************************************************************
;*		Struct of DPCM					*
;****************************************************************
.Struct		nsd_dpcm
	Control		.byte		;I/O 0x4010
	DA		.byte		;I/O 0x4011
	Address		.byte		;I/O 0x4012
	Size		.byte		;I/O 0x4013
.Endstruct



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
;	.import		_nsd_set_dpcm


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


;****************************************************************
