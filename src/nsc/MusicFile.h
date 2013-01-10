#pragma once

struct	NSF_Header{
				char	Name[5];			//00	"NESM",0x1A
	unsigned	char	Version;			//05
	unsigned	char	MusicNumber;		//06
	unsigned	char	StartMusicNumber;	//07
	unsigned	short	LoadAddress;		//08
	unsigned	short	InitAddress;		//0A
	unsigned	short	MainAddress;		//0C
				char	Title[32];			//0E
				char	Composer[32];		//2E
				char	Copyright[32];		//4E
	unsigned	short	Frequency_NTSC;		//6E
	unsigned	char	Bank[8];			//70
	unsigned	short	Frequency_PAL;		//78
	unsigned	char	Video;				//7A
	unsigned	char	External;			//7B
	unsigned	char	Null1;				//7C
	unsigned	char	Null2;				//7D
	unsigned	char	Null3;				//7E
	unsigned	char	Null4;				//7F
};

/****************************************************************/
/*																*/
/*			クラス定義											*/
/*																*/
/****************************************************************/
class MusicFile :
	public FileOutput, public MusicItem
{
//メンバー変数
public:
			MusicHeader			Header;		//Header
	map<	int, FDSC*		>	ptcFDSC;	//FDS  wave table (career)
	map<	int, FDSM*		>	ptcFDSM;	//FDS  wave table (modulator)
	map<	int, VRC7*		>	ptcVRC7;	//VRC7 User Instrument
	map<	int, N163*		>	ptcN163;	//N163 wave table
	map<	int, Envelop*	>	ptcEnv;		//Envelop
	map<	int, BGM*		>	ptcBGM;		//BGM
	map<	int, SE*		>	ptcSE;		//SE 
	map<	int, Sub*		>	ptcSub;		//Subroutine Sequence
	map< string, MusicTrack*>	ptcMac;		//Macro
				DPCMinfo*		cDPCMinfo;

private:
//static	const	Command_Info	Command[];
					string		dpcm_code;
	unsigned	int				nsf_size;

//メンバー関数
public:
	MusicFile(MMLfile* MML, string _code, const wchar_t _strName[]=L"==== [ Music ] ====");
	~MusicFile(void);

	unsigned	int		SetDPCMOffset(void);

	void	Fix_Address(void);

	//バイナリーを作る
	void	make_bin(size_t rom_size);

	//保存フェーズ
	void	saveNSF(const char*	strFileName, bool opt);
	void	saveASM(const char*	strFileName);

	void	Err(const wchar_t msg[]);
	void	Warning(const wchar_t msg[]);

};
