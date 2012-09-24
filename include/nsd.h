/************************************************************************/
/*																		*/
/*				NES Sound Driver & Library								*/
/*								define file								*/
/*										for C Language	(cc65)			*/
/*																		*/
/*								Programmed by							*/
/*										A.Watanabe						*/
/*																		*/
/************************************************************************/

#ifndef _NSD_H_
#define _NSD_H_

/****************************************************************/
/*			Struct of ‡™PCM										*/
/****************************************************************/
typedef struct{
	char		Control;			/*	I/O 0x4010	*/
	char		DA;					/*	I/O 0x4011	*/
	char		Address;			/*	I/O 0x4012	*/
	char		Size;				/*	I/O 0x4013	*/
} nsd_dpcm;



/****************************************************************/
/*			Function of Library									*/
/****************************************************************/
/*----------------------------------------------
	Summary :	main routine of this sound driver
	Arguments :	None
	Return :	None
	Useage :	Call by 60[Hz].  e.g. V-Blank
----------------------------------------------*/
void	__fastcall__	nsd_main(void);


/*----------------------------------------------
	Summary :	Initraize this sound driver.
	Arguments :	None
	Return :	None
	Useage :	Call when start up.
----------------------------------------------*/
void	__fastcall__	nsd_init(void);


/*----------------------------------------------
	Summary :	Set the Delta PCM information.
	Arguments :	Pointer of the Delta PCM information.
	Return :	None
----------------------------------------------*/
void	__fastcall__	nsd_set_dpcm(nsd_dpcm* dpcm);


/*----------------------------------------------
	Summary :	Play the BGM
	Arguments :	Pointer of the BGM data.
	Return :	None
----------------------------------------------*/
void	__fastcall__	nsd_play_bgm(void* ptr);


/*----------------------------------------------
	Summary :	Stop the BGM
	Arguments :	None
	Return :	None
----------------------------------------------*/
void	__fastcall__	nsd_stop_bgm(void);


/*----------------------------------------------
	Summary :	Play the SE
	Arguments :	Pointer of the SE data.
	Return :	None
----------------------------------------------*/
void	__fastcall__	nsd_play_se(void* ptr);


/*----------------------------------------------
	Summary :	Stop the SE
	Arguments :	None
	Return :	None
----------------------------------------------*/
void	__fastcall__	nsd_stop_se(void);


/****************************************************************/
#endif _NSD_H_
