
#define	__NES__

#include <nes.h>
#include <conio.h>

#include	"..\..\include\nsd.h"


//曲データ
extern	const	char		Test_BGM0[];		//Ｃ言語なので、シンボル先頭の'_'は不要。
extern	const	nsd_dpcm	Test_DPCMinfo[];	//Ｃ言語なので、シンボル先頭の'_'は不要。




//--------------------------------------
//キー読み込み用の定義
#define		APU_PAD1	(char*)0x4016			//Joypad #1 (RW)
#define		APU_PAD2	(char*)0x4017			//Joypad #2 / SOFTCLK (RW)

#define		JOY_A		0x80
#define		JOY_B		0x40
#define		JOY_Select	0x20
#define		JOY_Start	0x10
#define		JOY_Up		0x08
#define		JOY_Down	0x04
#define		JOY_Right	0x02
#define		JOY_Left	0x01



//--------------------------------------
//キー読み込み
//
//	！！注意！！
//		コンパイラオプションで最適化すると、正常に動作せず
//

char	read_key()
{
	char	key = 0;

	*APU_PAD1	= 0x01;
	*APU_PAD1	= 0x00;

	key   = *APU_PAD1 & 0x01;
	key <<= 1;
	key  |= *APU_PAD1 & 0x01;
	key <<= 1;
	key  |= *APU_PAD1 & 0x01;
	key <<= 1;
	key  |= *APU_PAD1 & 0x01;
	key <<= 1;
	key  |= *APU_PAD1 & 0x01;
	key <<= 1;
	key  |= *APU_PAD1 & 0x01;
	key <<= 1;
	key  |= *APU_PAD1 & 0x01;
	key <<= 1;
	key  |= *APU_PAD1 & 0x01;

	return(key);
}



//--------------------------------------
//キー読み込み（但し、更新されない場合は 0x00 ）
char	inkey()
{
	static	char	old_key;
			char	key = read_key();

	if(old_key != key){
		old_key = key;
	} else {
		key = 0;
	}

	return(key);

}



//--------------------------------------
//メインルーチン
void	main()
{

	unsigned	char	key			= 0;
	signed		char	cursol		= 0;
	signed		char	old_cursol	= -1;

	//memo: BG color
	//0		black
	//1		white
	//2		red
	//3		cyan
	//4		violett
	//5		green
	//6		blue
	//7		yellow
	//8		orange
	//9		brown
	//A		light red
	//B		dark grey
	//C		middle grey
	//D		light green
	//E		light blue
	//F		light gray

	bgcolor(0x06);

	gotoxy(2,2);
	cprintf("nsd.lib");

	gotoxy(2,3);
	cprintf("C language sample.");

	gotoxy(2,5);
	cprintf("call nsd_init();");
	nsd_init();						//音源ドライバの初期化。	必ず呼ぶ。

	gotoxy(2,6);
	cprintf("call nsd_set_dpcm();");
	nsd_set_dpcm(Test_DPCMinfo);	//⊿PCM制御構造体のセット

	gotoxy(8,11);
	cprintf("Play music");
	gotoxy(8,12);
	cprintf("Stop music");
	gotoxy(8,13);
	cprintf("Pause music");
	gotoxy(8,14);
	cprintf("Resume music");

	while(1){

		//キー読み込み
		key = inkey();


		//カーソル処理
		if(key & JOY_Down){
			old_cursol = cursol;
			cursol++;
			if(cursol > 3){
				cursol = 3;
			}
		}

		if(key & JOY_Up){
			old_cursol = cursol;
			cursol--;
			if(cursol < 0){
				cursol = 0;
			}
		}

		if(old_cursol != cursol){
			gotoxy(6,11 + old_cursol);
			cprintf(" ");
			gotoxy(6,11 + cursol);
			cprintf(">");
			old_cursol = cursol;
		}

		//Ａボタン処理
		if(key & JOY_A){
			switch(cursol){
				case(0):
					nsd_play_bgm(Test_BGM0);	//演奏開始
					break;
				case(1):
					nsd_stop_bgm();				//演奏停止
					break;
				case(2):
					nsd_pause_bgm();			//演奏開始
					break;
				case(3):
					nsd_resume_bgm();			//演奏停止
					break;
				default:
					break;
			}
		}

		nsd_main();					//V-Blank毎に呼び出す。（ここに書かなくても、NMI割り込みでやっても良い。）

		//垂直同期待ち
		waitvsync();

	}

}

