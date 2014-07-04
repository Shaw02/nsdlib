#include "StdAfx.h"
#include "VRC7.h"

//==============================================================
//		コンストラクタ
//--------------------------------------------------------------
//	●引数
//		MMLfile*			MML			MMLファイルのオブジェクト
//		unsigned	int		_id			VRC7音色番号
//		const		wchar_t	_strName[]	オブジェクト名
//	●返値
//					無し
//==============================================================
VRC7::VRC7(MMLfile* MML, unsigned int _id, const wchar_t _strName[]):
	MusicItem(_id, _strName),
	f_Use(false),
	m_id(_id)
{
	//----------------------
	//Local変数
				int		i;
				bool	_mset	= false;
	unsigned	char	_mode	= 0;
	unsigned	char	_pt		= 0;
	unsigned	char	_opll[24];
	unsigned	char	cData;

//	定数定義
enum	Command_ID_mml {
	VRC7_Normal,
	VRC7_Resister,
	VRC7_Num,
	VRC7_Comma
};

//	これらは、MML構文で使えるコマンド。
const	static	Command_Info	Command[] = {
		{	"@R",	VRC7_Resister	},
		{	"@",	VRC7_Normal		},
		{	"0",	VRC7_Num		},
		{	"1",	VRC7_Num		},
		{	"2",	VRC7_Num		},
		{	"3",	VRC7_Num		},
		{	"4",	VRC7_Num		},
		{	"5",	VRC7_Num		},
		{	"6",	VRC7_Num		},
		{	"7",	VRC7_Num		},
		{	"8",	VRC7_Num		},
		{	"9",	VRC7_Num		},
		{	"$",	VRC7_Num		},
		{	"%",	VRC7_Num		},
		{	",",	VRC7_Comma		}
};


	//------------------------------
	//クラスの初期設定

	iSize = 8;
	code.resize(iSize);

	//------------------------------
	//コンパイル

	// { の検索
	while(MML->cRead() != '{'){
		if(MML->eof()){
			MML->Err(L"ブロックの開始を示す{が見つかりません。");
		}
	}


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

			case(VRC7_Normal):
				if(_mset == true){
					MML->Err(L"@, @Rコマンドは１回だけ指定して下さい。");
				}
				_mset = true;
				_mode = 0;
				break;

			case(VRC7_Resister):
				if(_mset == true){
					MML->Err(L"@, @Rコマンドは１回だけ指定して下さい。");
				}
				_mset = true;
				_mode = 1;
				break;

			case(VRC7_Num):
				if(_mset == false){
					MML->Err(L"先ずは@, @Rコマンドを記述して下さい。");
				}
				MML->Back();
				i = MML->GetInt();
				if( (i<0) || (i>255) ){
					MML->Err(L"0～255の範囲で指定して下さい。");
				}

				if(_mode == 0){
					if(_pt >= 24){
						MML->Err(L"VRC7(パラメータベース)の引数が24個を超えました。");
					}
					_opll[_pt] = (unsigned char)i;
				} else {
					if(_pt >= 8){
						MML->Err(L"VRC7(レジスタベース)の引数が8個を超えました。");
					}
					code[_pt] = (unsigned char)i;
				}
				_pt++;
				break;
			case(VRC7_Comma):
				break;

			//unknown command
			default:
				MML->Err(L"unknown command");
				break;
		}
	}

	if(_mode == 0){
		if(_pt < 24){
			MML->Err(L"VRC7(パラメータベース)の引数が24個に満たないです。");
		}

		//TL FB
		//0  1
		//AR DR SL RR KL MT AM VB EG KR DT
		//2  3  4  5  6  7  8  9  10 11 12
		//AR DR SL RR KL MT AM VB EG KR DT
		//13 14 15 16 17 18 19 20 21 22 23

		code[0] = ((_opll[ 8] & 0x01) << 7) | ((_opll[ 9] & 0x01) << 6) | ((_opll[10] & 0x01) << 5) | ((_opll[11] & 0x01) << 4) | (_opll[ 7] & 0x0F);
		code[1] = ((_opll[19] & 0x01) << 7) | ((_opll[20] & 0x01) << 6) | ((_opll[21] & 0x01) << 5) | ((_opll[22] & 0x01) << 4) | (_opll[18] & 0x0F);
		code[2] = ((_opll[ 6] & 0x03) << 6) |  (_opll[ 0] & 0x3F);
		code[3] = ((_opll[17] & 0x03) << 6) | ((_opll[23] & 0x01) << 4) | ((_opll[12] & 0x01) << 3) |  (_opll[ 1] & 0x07);
		code[4] = ((_opll[ 2] & 0x0F) << 4) |  (_opll[ 3] & 0x0F);
		code[5] = ((_opll[13] & 0x0F) << 4) |  (_opll[14] & 0x0F);
		code[6] = ((_opll[ 4] & 0x0F) << 4) |  (_opll[ 5] & 0x0F);
		code[7] = ((_opll[15] & 0x0F) << 4) |  (_opll[16] & 0x0F);
	} else {
		if(_pt < 8){
			MML->Err(L"VRC7(レジスタベース)の引数が8個に満たないです。");
		}
	}
}

//==============================================================
//		デストラクタ
//--------------------------------------------------------------
//	●引数
//				無し
//	●返値
//				無し
//==============================================================
VRC7::~VRC7(void)
{
}
//==============================================================
//		コードの取得
//--------------------------------------------------------------
//	●引数
//		MusicFile*	MUS		コードを出力する曲データファイル・オブジェクト
//	●返値
//				無し
//==============================================================
void	VRC7::getAsm(MusicFile* MUS)
{
	*MUS << MUS->Header.Label.c_str() << "VRC7" << m_id << ":" << endl;
	MusicItem::getAsm(MUS);
}
