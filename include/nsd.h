/***************************************************************************

				NES Sound Driver & Library	(NSD.lib)

****************************************************************************

   Copyright (c) 2012 A.Watanabe (S.W.)
   All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:

     1. Redistributions of source code must retain the above copyright
        notice, this list of conditions and the following disclaimer.

     2. Redistributions in binary form must reproduce the above copyright
        notice, this list of conditions and the following disclaimer in the
        documentation and/or other materials provided with the distribution.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER
   OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

***************************************************************************/

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
void	__fastcall__	nsd_main_bgm(void);
void	__fastcall__	nsd_main_se(void);


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
void	__fastcall__	nsd_set_dpcm(const nsd_dpcm* dpcm);


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
	Summary :	Pause the BGM
	Arguments :	None
	Return :	None
----------------------------------------------*/
void	__fastcall__	nsd_pause_bgm(void);


/*----------------------------------------------
	Summary :	Resume the BGM
	Arguments :	None
	Return :	None
----------------------------------------------*/
void	__fastcall__	nsd_resume_bgm(void);


/*----------------------------------------------
	Summary :	Pause ans Save
	Arguments :	None
	Return :	None
----------------------------------------------*/
void	__fastcall__	nsd_save(void* buff);


/*----------------------------------------------
	Summary :	Pause ans Load
	Arguments :	None
	Return :	None
----------------------------------------------*/
void	__fastcall__	nsd_load(void* buff);


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
