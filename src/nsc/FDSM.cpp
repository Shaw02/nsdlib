#include "StdAfx.h"
#include "FDSM.h"

//==============================================================
//		コンストラクタ
//--------------------------------------------------------------
//	●引数
//		MMLfile*	MML		MMLファイルのオブジェクト
//		bool		_sub	このオブジェクトは、サブルーチン？
//	●返値
//					無し
//==============================================================
FDSM::FDSM(MMLfile* MML, unsigned int _id, const wchar_t _strName[]):
	MusicItem(_strName),
	m_id(_id)
{
	//----------------------
	//Local変数


//	定数定義
enum	Command_ID_mml {
	FDSM_Num,
	FDSM_Commma
};

//	これらは、MML構文で使えるコマンド。
const	static	Command_Info	Command[] = {
		{	"%",	FDSM_Num	},
		{	"$",	FDSM_Num	},
		{	"0",	FDSM_Num	},
		{	"1",	FDSM_Num	},
		{	"2",	FDSM_Num	},
		{	"3",	FDSM_Num	},
		{	"4",	FDSM_Num	},
		{	"5",	FDSM_Num	},
		{	"6",	FDSM_Num	},
		{	"7",	FDSM_Num	},
		{	"8",	FDSM_Num	},
		{	"9",	FDSM_Num	},
		{	",",	FDSM_Commma	}
};

				int		i;
	unsigned	char	cData;
	unsigned	int		ptFDSM		= 0;
				string	WAVE;

	//------------------------------
	//クラスの初期設定
	WAVE.clear();

	//------------------------------
	//コンパイル

	// { の検索
	while(MML->cRead() != '{'){
		if(MML->eof()){
			MML->Err(L"ブロックの開始を示す{が見つかりません。");
		}
	}

	code.resize(0);

	// } が来るまで、記述ブロック内をコンパイルする。
	while((cData = MML->GetChar()) != '}'){
		
		// } が来る前に、[EOF]が来たらエラー
		if( MML->eof() ){
			MML->Err(L"ブロックの終端を示す`}'がありません。");
		}

		//１つ戻る
		MML->Back();


		//各コマンド毎の処理
		switch(MML->GetCommandID(Command, sizeof(Command)/sizeof(Command_Info))){

			case(FDSM_Num):
				MML->Back();
				i = MML->GetInt();
				if( (i<0) || (i>7)){
					MML->Err(L"FDSMの波形定義は、0～7の範囲で指定して下さい。");
				}
				WAVE.append((char)1, (char)i & 0x07);
				ptFDSM++;
				break;

			case(FDSM_Commma):
				break;

			//unknown command
			default:
				MML->Err(L"unknown command");
				break;
		}
	}
	if(ptFDSM < 32){
		MML->Err(L"サンプル長が32を超えています。");
	}
	if(ptFDSM > 32){
		MML->Err(L"サンプル長が32未満です。");
	}
	i = 0;
	while(ptFDSM>0){
		code.append((char)1, WAVE[i]);
		i++;
		ptFDSM--;
	}


	iSize = code.size();
}

//==============================================================
//		デストラクタ
//--------------------------------------------------------------
//	●引数
//				無し
//	●返値
//				無し
//==============================================================
FDSM::~FDSM(void)
{
}
//==============================================================
//		コードの取得
//--------------------------------------------------------------
//	●引数
//				無し
//	●返値
//				無し
//==============================================================
void	FDSM::getAsm(MusicFile* MUS)
{
	*MUS << MUS->Header.Label.c_str() << "FDSM" << m_id << ":" << endl;
	MusicItem::getAsm(MUS);
}
