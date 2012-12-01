#pragma once

/****************************************************************/
/*																*/
/*			定数定義											*/
/*																*/
/****************************************************************/

#define	nsd_EndOfTrack			0x00
#define	nsd_Jump				0x01
#define	nsd_Call				0x02
#define	nsd_Repeat_A_Start		0x03
#define	nsd_Repeat_A_Branch		0x04
#define	nsd_Repeat_A_End		0x05
#define	nsd_Poke				0x07
#define	nsd_Tempo				0x08
#define	nsd_Length				0x09
#define	nsd_GateTime_q			0x0A
#define	nsd_GateTime_u			0x0B
#define	nsd_Relative_Tempo		0x0C
#define	nsd_GateMode_0			0x0D
#define	nsd_GateMode_1			0x0E
#define	nsd_GateMode_2			0x0F
#define	nsd_Envelop_Voice		0x10
#define	nsd_Envelop_Volume		0x11
#define	nsd_Envelop_Frequency	0x12
#define	nsd_Envelop_Note		0x13
#define	nsd_Detune_Cent			0x14
#define	nsd_Derune_Register		0x15
#define	nsd_Sweep				0x16
#define	nsd_Portamento			0x17
#define	nsd_Repeat_B_Start		0x18
#define	nsd_Repeat_B_Branch		0x19
#define	nsd_Repeat_B_End		0x1A
#define	nsd_Voice				0x1B
#define	nsc_VRC7				0x1C
#define	nsc_VRC7_reg			0x1D
#define	nsc_N163				0x1E
#define	nsc_N163_Channel		0x1F
#define	nsd_Volume_Down			0x20
#define	nsd_Volume_Up			0x21
#define	nsc_FME7_frequency		0x26
#define	nsd_Octave_Down			0x28
#define	nsd_Octave_Up			0x29
#define	nsd_Transpose			0x2A
#define	nsd_Relative_Transpose	0x2B
#define	nsd_Octave_Down_1		0x2C
#define	nsd_Octave_Up_1			0x2D
#define	nsd_Transpose_1			0x2E
#define	nsd_Release_Voice		0x30	// - 0x37
#define	nsd_Octave				0x38	// - 0x3F
#define	nsd_Length_96			0x40
#define	nsd_Length_72			0x41
#define	nsd_Length_48			0x42
#define	nsd_Length_36			0x43
#define	nsd_Length_32			0x44
#define	nsd_Length_24			0x45
#define	nsd_Length_18			0x46
#define	nsd_Length_16			0x47
#define	nsd_Length_12			0x48
#define	nsd_Length_9			0x49
#define	nsd_Length_8			0x4A
#define	nsd_Length_6			0x4B
#define	nsd_Length_4			0x4C
#define	nsd_Length_3			0x4D
#define	nsd_Length_2			0x4E
#define	nsd_Length_1			0x4F
#define	nsd_GateTime_Byte		0x50
#define	nsd_Volume				0x60
#define	nsd_Release_Volume		0x70
#define	nsd_Note				0x80

#define	nsd_Note_Slur			0x10
#define	nsd_Note_Length			0x20
#define	nsd_Note_Gate			0x40

/****************************************************************/
/*																*/
/*			クラス定義											*/
/*																*/
/****************************************************************/
class MusicEvent:
	public MusicItem
{
//メンバー関数
public:
	MusicEvent(const wchar_t _strName[]=L"Event");
	~MusicEvent(void);
};
