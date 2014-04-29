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



