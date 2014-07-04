#include "StdAfx.h"
#include "Patch.h"

/****************************************************************/
/*					グローバル変数（クラスだけど・・・）		*/
/****************************************************************/
extern	OPSW*			cOptionSW;	//オプション情報へのポインタ変数

//==============================================================
//		コンストラクタ
//--------------------------------------------------------------
//	●引数
//		MMLfile*			MML			MMLファイルのオブジェクト
//					int		_id			パッチ番号
//	●返値
//					無し
//==============================================================
Patch::Patch(MMLfile* MML, int _id):
	m_id(_id)
{
	//----------------------
	//Local変数


//	定数定義
enum	Command_ID_mml {
	Patch_C,
	Patch_Cis,
	Patch_D,
	Patch_Dis,
	Patch_E,
	Patch_F,
	Patch_Fis,
	Patch_G,
	Patch_Gis,
	Patch_A,
	Patch_Ais,
	Patch_B,
	Patch_Note,

	Patch_Off_Evoi,
	Patch_Off_Evol,
	Patch_Off_Em,
	Patch_Off_En,

	Patch_Evoi,
	Patch_Evol,
	Patch_Em,
	Patch_En,

	Patch_Voice,
	Patch_KeyShift,
	Patch_n163set,
	Patch_Sweep,
	Patch_Sub

};

//	これらは、MML構文で使えるコマンド。
const	static	Command_Info	Command[] = {
		{	"c#",	Patch_Cis	},
		{	"d#",	Patch_Dis	},
		{	"f#",	Patch_Fis	},
		{	"g#",	Patch_Gis	},
		{	"a#",	Patch_Ais	},

		{	"c+",	Patch_Cis	},
		{	"d+",	Patch_Dis	},
		{	"f+",	Patch_Fis	},
		{	"g+",	Patch_Gis	},
		{	"a+",	Patch_Ais	},

		{	"d-",	Patch_Cis	},
		{	"e-",	Patch_Dis	},
		{	"g-",	Patch_Fis	},
		{	"a-",	Patch_Gis	},
		{	"b-",	Patch_Ais	},

		{	"c",	Patch_C		},
		{	"d",	Patch_D		},
		{	"e",	Patch_E		},
		{	"f",	Patch_F		},
		{	"g",	Patch_G		},
		{	"a",	Patch_A		},
		{	"b",	Patch_B		},

		{	"ど#",		Patch_Cis	},
		{	"れ#",		Patch_Dis	},
		{	"ふぁ#",	Patch_Fis	},
		{	"ふ#",		Patch_Fis	},
		{	"そ#",		Patch_Gis	},
		{	"ら#",		Patch_Ais	},

		{	"れ-",	Patch_Cis	},
		{	"み-",	Patch_Dis	},
		{	"そ-",	Patch_Fis	},
		{	"ら-",	Patch_Gis	},
		{	"し-",	Patch_Ais	},

		{	"ど",	Patch_C		},
		{	"れ",	Patch_D		},
		{	"み",	Patch_E		},
		{	"ふぁ",	Patch_F		},
		{	"ふ",	Patch_F		},
		{	"そ",	Patch_G		},
		{	"ら",	Patch_A		},
		{	"し",	Patch_B		},

		{	"ド#",		Patch_Cis	},
		{	"レ#",		Patch_Dis	},
		{	"ファ#",	Patch_Fis	},
		{	"フ#",		Patch_Fis	},
		{	"ソ#",		Patch_Gis	},
		{	"ラ#",		Patch_Ais	},

		{	"レ-",	Patch_Cis	},
		{	"ミ-",	Patch_Dis	},
		{	"ソ-",	Patch_Fis	},
		{	"ラ-",	Patch_Gis	},
		{	"シ-",	Patch_Ais	},

		{	"ド",	Patch_C		},
		{	"レ",	Patch_D		},
		{	"ミ",	Patch_E		},
		{	"ファ",	Patch_F		},
		{	"フ",	Patch_F		},
		{	"ソ",	Patch_G		},
		{	"ラ",	Patch_A		},
		{	"シ",	Patch_B		},

		{	"n",	Patch_Note	},


		{	"E@*",	Patch_Off_Evoi	},
		{	"Ev*",	Patch_Off_Evol	},
		{	"Em*",	Patch_Off_Em	},
		{	"En*",	Patch_Off_En	},

		{	"E@",	Patch_Evoi		},
		{	"Ev",	Patch_Evol		},
		{	"Em",	Patch_Em		},
		{	"En",	Patch_En		},

		{	"s",	Patch_Sweep		},
		{	"@NS",	Patch_n163set	},
		{	"@",	Patch_Voice		},
		{	"_",	Patch_KeyShift	},
		{	"S",	Patch_Sub		}

};

				int		i;
	unsigned	char	cData;

	//------------------------------
	//クラスの初期設定
	setN(MML, 0);	//ノート番号 0 の情報は必ず作成する。


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

			case(Patch_C):
				setKey(MML, 0);
				break;

			case(Patch_Cis):
				setKey(MML, 1);
				break;

			case(Patch_D):
				setKey(MML, 2);
				break;

			case(Patch_Dis):
				setKey(MML, 3);
				break;

			case(Patch_E):
				setKey(MML, 4);
				break;

			case(Patch_F):
				setKey(MML, 5);
				break;

			case(Patch_Fis):
				setKey(MML, 6);
				break;

			case(Patch_G):
				setKey(MML, 7);
				break;

			case(Patch_Gis):
				setKey(MML, 8);
				break;

			case(Patch_A):
				setKey(MML, 9);
				break;

			case(Patch_Ais):
				setKey(MML, 10);
				break;

			case(Patch_B):
				setKey(MML, 11);
				break;

			case(Patch_Note):
				setN(MML, MML->GetInt());
				break;

			case(Patch_Voice):
				if(m_now_Patch->fEvoi == true){
					MML->Err(L"音色エンベロープと同時に定義することはできません。");
				}
				if(m_now_Patch->fVoi == true){
					MML->Err(L"音色の２重定義です。");
				}
				i = MML->GetInt();
				if( (i<0) || (i>255) ){
					MML->Err(L"音色は0～255の範囲で指定してください。");
				}
				m_now_Patch->iVoi		= i;
				m_now_Patch->fVoi		= true;
				m_now_Patch->sw_Evoi	= false;
				break;

			case(Patch_Off_Evoi):
				MML->Err(L"音色エンベロープは、@コマンドで無効にできます。");
				break;

			case(Patch_Off_Evol):
				if(m_now_Patch->fEvol == true){
					MML->Err(L"音量エンベロープの２重定義です。");
				}
				m_now_Patch->fEvol		= true;
				m_now_Patch->sw_Evol	= false;
				break;

			case(Patch_Off_Em):
				if(m_now_Patch->fEm == true){
					MML->Err(L"音程エンベロープの２重定義です。");
				}
				m_now_Patch->fEm		= true;
				m_now_Patch->sw_Em		= false;
				break;

			case(Patch_Off_En):
				if(m_now_Patch->fEn == true){
					MML->Err(L"ノートエンベロープの２重定義です。");
				}
				m_now_Patch->fEn		= true;
				m_now_Patch->sw_En		= false;
				break;

			case(Patch_Evoi):
				if(m_now_Patch->fEvoi == true){
					MML->Err(L"音色エンベロープの２重定義です。");
				}
				if(m_now_Patch->fVoi == true){
					MML->Err(L"音色と同時に定義することはできません。");
				}
				m_now_Patch->iEvoi		= MML->GetInt();
				m_now_Patch->fEvoi		= true;
				m_now_Patch->sw_Evoi	= true;
				break;

			case(Patch_Evol):
				if(m_now_Patch->fEvol == true){
					MML->Err(L"音量エンベロープの２重定義です。");
				}
				m_now_Patch->iEvol		= MML->GetInt();
				m_now_Patch->fEvol		= true;
				m_now_Patch->sw_Evol	= true;
				break;

			case(Patch_Em):
				if(m_now_Patch->fEm == true){
					MML->Err(L"音程エンベロープの２重定義です。");
				}
				m_now_Patch->iEm		= MML->GetInt();
				m_now_Patch->fEm		= true;
				m_now_Patch->sw_Em		= true;
				break;

			case(Patch_En):
				if(m_now_Patch->fEn == true){
					MML->Err(L"ノートエンベロープの２重定義です。");
				}
				m_now_Patch->iEn		= MML->GetInt();
				m_now_Patch->fEn		= true;
				m_now_Patch->sw_En		= true;
				break;

			case(Patch_KeyShift):
				if(m_now_Patch->fKey == true){
					MML->Err(L"移調の２重定義です。");
				}
				m_now_Patch->iKey		= MML->GetInt();
				m_now_Patch->fKey		= true;
				break;

			case(Patch_Sweep):
				if(m_now_Patch->fSweep == true){
					MML->Err(L"sコマンドの２重定義です。");
				} else {
					int		iSpeed;
					int		iDepth;
					char	c;

					iSpeed	= MML->GetInt();
					cData	= MML->GetChar();
					if(cData != ','){
						if( (iSpeed < 0) || (iSpeed > 255) ){
							MML->Err(L"sコマンドは0～255の範囲で指定してください。");
						}
						MML->Back();
						c = (unsigned char)iSpeed;
					} else {
						if( (iSpeed < 0) || (iSpeed > 15) ){
							MML->Err(L"sコマンドの第1パラメータは0～15の範囲で指定してください。");
						}
						iDepth = MML->GetInt();
						if( (iDepth < 0) || (iDepth > 15) ){
							MML->Err(L"sコマンドの第2パラメータは0～15の範囲で指定してください。");
						}
						c = (unsigned char)(((iSpeed & 0x0F) << 4) | (iDepth & 0x0F));
					}
					m_now_Patch->iSweep		= c;
					m_now_Patch->fSweep		= true;
				}
				break;

			case(Patch_n163set):
				if(m_now_Patch->fVoi == true){
					MML->Err(L"@コマンドが既に指定されています。");
				}
				if(m_now_Patch->fEvoi == true){
					MML->Err(L"E@コマンドが既に指定されています。");
				}
				i = MML->GetInt();
				if((i<0) || (i>252)){
					MML->Err(L"n16xの波形開始点は0～252の範囲で指定してください。");
				}
				if((i % 4) != 0){
					MML->Err(L"n16xの波形開始点は4の倍数で指定してください。");
				}
				m_now_Patch->iVoi		= i/4;
				m_now_Patch->fVoi		= true;
				m_now_Patch->sw_Evoi	= false;
				cData = MML->GetChar();
				if(cData == ','){
					if(m_now_Patch->fSweep == true){
						MML->Err(L"sコマンドが既に指定されています。");
					}
					i = MML->GetInt();
					if((i<4) || (i>256)){
						MML->Err(L"n16xのサンプル長は4～256の範囲で指定してください。");
					}
					if((i % 4) != 0){
						MML->Err(L"n16xのサンプル長は4の倍数で指定してください。");
					}
					m_now_Patch->iSweep		= 64 - (i/4);
					m_now_Patch->fSweep		= true;
				} else {
					MML->Back();
				}
				break;

			case(Patch_Sub):
				if(m_now_Patch->fSub == true){
					MML->Err(L"サブルーチンの２重定義です。");
				}
				m_now_Patch->iSub		= MML->GetInt();
				m_now_Patch->fSub		= true;
				break;
		
		//unknown command
			default:
				MML->Err(L"unknown command");
				break;
		}
	}

	//Debug message　（うざい程出力するので注意。）
	if(cOptionSW->cDebug & 0x01){
		DebugMsg();
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
Patch::~Patch(void)
{
	//----------------------
	//Local変数
	map<unsigned int, patch_scrap*>::iterator	itPatch;
	
	//----------------------
	//Delete Class
	if(!m_Patch.empty()){
		itPatch = m_Patch.begin();
		while(itPatch != m_Patch.end()){
			delete itPatch->second;
			itPatch++;
		}
		m_Patch.clear();
	}
}

//==============================================================
//		デバッグ用
//--------------------------------------------------------------
//	●引数
//				無し
//	●返値
//				無し
//==============================================================
void	Patch::DebugMsg(void)
{
	//----------------------
	//Local変数
	map<unsigned int, patch_scrap*>::iterator	itPatch;
	
	//----------------------
	//Delete Class
	cout	<< "==== [ Patch(" << m_id << ") ] ====" << endl;

	if(!m_Patch.empty()){
		itPatch = m_Patch.begin();
		while(itPatch != m_Patch.end()){
			m_now_Patch = itPatch->second;

			cout	<<	"  n" << itPatch->first;
			if(get_fSub()){	cout	<<	"  S"	<<	get_iSub();		};
			if(get_fKey()){	cout	<<	"  _"	<<	get_iKey();		};
			if(get_fSweep()){cout	<<	"  s"	<<	(int)get_iSweep();	};
			if(get_fVoi()){	cout	<<	"  @"	<<	get_iVoi();		};
			if(get_fEvoi()){cout	<<	" E@"	<<	get_iEvoi();	};

			if(get_fEvol()){
				if(get_sw_Evol()){
					cout	<<	" Ev*";
				} else {
					cout	<<	" Ev"	<<	get_iEvol();
				}
			};

			if(get_fEm()){
				if(get_sw_Em()){
					cout	<<	" Em*";
				} else {
					cout	<<	" Em"	<<	get_iEm();
				}
			};

			if(get_fEn()){
				if(get_sw_En()){
					cout	<<	" En*";
				} else {
					cout	<<	" En"	<<	get_iEn();
				}
			};

			cout	<<	endl;
			itPatch++;
		}
	}
}

//==============================================================
//		設定
//--------------------------------------------------------------
//	●引数
//		MMLfile*	MML		MMLファイルのオブジェクト
//			int	key		キー番号（0:C / 1:Cis / ...）
//	●返値
//				無し
//==============================================================
void	Patch::setKey(MMLfile* MML, int key)
{
	setN(MML, ((MML->GetInt()-1) * 12) + key);
}

//==============================================================
//		設定
//--------------------------------------------------------------
//	●引数
//		MMLfile*		MML		MMLファイルのオブジェクト
//				int		note	ノート番号
//	●返値
//				無し
//==============================================================
void	Patch::setN(MMLfile* MML, int note)
{

	if((note<0) || (note>255)){
		MML->Err(L"音階の範囲を超えています。");
	}

	//パッチの設定
	m_kn = note;

	if(m_Patch.count(m_kn) != 0){
		//既存パッチのロード
		m_now_Patch =	m_Patch[m_kn];
	} else {
		//パッチの新規作成
		m_now_Patch			= new	patch_scrap;
		m_Patch[m_kn]		= m_now_Patch;

		m_now_Patch->fVoi	= false;
		m_now_Patch->fEvoi	= false;
		m_now_Patch->fEvol	= false;
		m_now_Patch->fEm	= false;
		m_now_Patch->fEn	= false;
		m_now_Patch->fSub	= false;

		m_now_Patch->sw_Evoi= false;
		m_now_Patch->sw_Evol= false;
		m_now_Patch->sw_Em	= false;
		m_now_Patch->sw_En	= false;
	}

}

//==============================================================
//		
//--------------------------------------------------------------
//	●引数
//				int		note	ノート番号
//	●返値
//				
//==============================================================
void	Patch::setNote(int i)
{
	bool	f_set = false;

	m_kn = i;

	while(m_kn > 0){
		if(m_Patch.count(m_kn) != 0){
			f_set = true;
			break;
		} else {
			m_kn--;
		}
	}

	m_now_Patch = m_Patch[m_kn];

}
