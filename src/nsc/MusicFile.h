#pragma once

struct	NSF_Header{
				char	Name[5];			//00	"NESM",0x1A
	unsigned	char	Version;			//05
	unsigned	char	MusicNumber;		//06
	unsigned	char	StartMusicNumber;	//07
	unsigned	__int16	LoadAddress;		//08
	unsigned	__int16	InitAddress;		//0A
	unsigned	__int16	MainAddress;		//0C
				char	Title[32];			//0E
				char	Composer[32];		//2E
				char	Copyright[32];		//4E
	unsigned	__int16	Frequency_NTSC;		//6E
	unsigned	char	Bank[8];			//70
	unsigned	__int16	Frequency_PAL;		//78
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

private:
//static	const	Command_Info	Command[];

	map<	int, Envelop*	>	ptcEnv;		//Envelop
	map<	int, BGM*		>	ptcBGM;		//BGM
	map<	int, SE*		>	ptcSE;		//SE 
	map<	int, Sub*		>	ptcSub;		//Subroutine Sequence
	map< string, MusicTrack*>	ptcMac;		//Macro
	map< string, MusicItem*>	ptcDPCM;	//⊿PCM
//								DPCMinfo;

	unsigned	int				nsf_size;

//メンバー関数
public:
	MusicFile(MMLfile* MML, string _code, const char _strName[]="==== [ Music ] ====");
	~MusicFile(void);

	void	Fix_Address(void);

	//バイナリーを作る
	void	make_bin(unsigned int rom_size);

	//保存フェーズ
//	void	saveBIN(const char*	strFileName);
	void	saveNSF(const char*	strFileName);
	void	saveASM(const char*	strFileName);
//	void	saveC(const char*	strFileName);

	void	Err(const char* msg);
	void	Warning(const char* msg);

};
