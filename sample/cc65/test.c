
#define	__NES__

#include <nes.h>
#include <conio.h>

#include	"..\..\include\nsd.h"


//曲データ
extern	const	char		Test_BGM0[];		//Ｃ言語なので、シンボル先頭の'_'は不要。
extern	const	nsd_dpcm	Test_DPCMinfo[];	//Ｃ言語なので、シンボル先頭の'_'は不要。

//メインルーチン
void	main()
{

	nsd_init();				//音源ドライバの初期化。	必ず呼ぶ。
	nsd_set_dpcm(Test_DPCMinfo);	//⊿PCM制御構造体のセット

	nsd_play_bgm(Test_BGM0);		//演奏開始

	while(1){
		waitvblank();
		nsd_main();			//V-Blank毎に呼び出す。（ここに書かなくても、NMI割り込みでやっても良い。）
	}

}

