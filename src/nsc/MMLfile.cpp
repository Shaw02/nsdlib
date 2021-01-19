/*******************************************************************************

			NES Sound Driver & Library	(NSD.lib)	MML Compiler

	Copyright (c) 2012 A.Watanabe (S.W.), All rights reserved.
	 For conditions of distribution and use, see copyright notice in "nsc.cpp".

*******************************************************************************/

#include "StdAfx.h"
#include "MMLfile.h"

/****************************************************************/
/*					グローバル変数（クラスだけど・・・）		*/
/****************************************************************/
extern	OPSW*			cOptionSW;	//オプション情報へのポインタ変数

#ifdef _OPENMP
	extern	omp_lock_t		lock_cout;
#endif

/*
Memo:
	read_char();		//ファイル or マクロから、1Byte読み込み
	　└cRead()			//Shift-Jisを半角に変換する機構付き
	    ├GetChar()		//コメント・制御コードの無視機能付き
		│├GetString()
	    │├GetNum()
	    │└Get****()
	    ├SetMacro()
	    ├CallMacro()
		└GetCommandID()
*/
//==============================================================
//		コンストラクタ
//--------------------------------------------------------------
//	●引数
//		const	char*	strFileName		ファイル名
//	●返値
//				無し
//==============================================================
MMLfile::MMLfile(string&	strFileName):
p_macro(0),
f_macro(false),
f_2to1(false),
f_error(false),
offset_Ei(0),
offset_Ev(0),
offset_En(0),
offset_Em(0),
iReleaseVolume(2),
iRepeatMode(0),
iTieMode(0),
timebase(24),
octave_reverse(false),
q_reverse(false),
rest(2),
wait(0),
QMax(8),
priority(0)
{
	try {
		//File open
		nowFile	= new FileInput();
		nowFile->fileopen(strFileName.c_str());

		//読み込み失敗？
		f_error = nowFile->isError();
		if(f_error == true){
			throw ios_base::failure(strFileName + ": " + strerror(errno));
		} else {
			ptcFiles.push_back(nowFile);
			iFiles = 0;
		}

	} catch (int no) {
		nsc_ErrMsg(no);
		f_error = true;	//エラーが発生した。
	} catch (const exception& e){
		nsc_ErrMsg(e);
		f_error = true;	//エラーが発生した。
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
MMLfile::~MMLfile(void)
{

	//----------------------
	//Delete Class

	//開いたファイルを全部閉じる
	for(vector<FileInput*>::iterator it=ptcFiles.begin(), e=ptcFiles.end(); it!=e; ++it){
		if(cOptionSW->iDebug & DEBUG_Close_Inc){
			_COUT << _T("Close file :");
			cout << (*it)->GetFilename()->c_str() << endl;
		}
		(*it)->close();
		delete *it;
	}
	ptcFiles.clear();

	//マクロを全部解放する。
	for(map<string,string>::iterator it=ptcMac.begin(), e=ptcMac.end(); it!=e; ++it){
		it->second.clear();
	}
	ptcMac.clear();
	lv_Mac.clear();

	//パッチを全部解放する。
	for(map<size_t,Patch*>::iterator it=ptcPatch.begin(), e=ptcPatch.end(); it!=e; ++it){
		delete	it->second;
	}
	ptcPatch.clear();

}

//==============================================================
//			現在コンパイル処理中のファイルの[EOF]チェック
//--------------------------------------------------------------
//	●引数
//			無し
//	●返値
//			bool		true	[EOF]を検出
//						false	まだ終わりじゃない。
//==============================================================
bool	MMLfile::eof(void)
{
	return( nowFile->eof() );
}

//==============================================================
//			MMLファイルの終了チェック
//--------------------------------------------------------------
//	●引数
//			無し
//	●返値
//			bool		true	元ファイルで[EOF]を検出
//						false	まだ終わりじゃない。
//==============================================================
bool	MMLfile::eom(void)
{
	if(( iFiles == 0 ) && ( eof() )){
		return(true);
	}
	return(false);
}

//==============================================================
//		インクルードファイル
//--------------------------------------------------------------
//	●引数
//			無し
//	●返値
//			無し
//	●処理
//			現在のファイルポインタに書いてあるファイルをインクルードする
//==============================================================
void	MMLfile::include()
{
	//----------------------
	//Local変数
	string		_name = "";
	FileInput*	_incFile;

	GetString(&_name, false);

	//----------------------
	//同じファイルが開かれていないかチェック
	for(vector<FileInput*>::iterator it=ptcFiles.begin(), e=ptcFiles.end(); it!=e; ++it){
		if( *(*it)->GetFilename() == _name ){
			Err(_T("既に同じファイルが#includeで開かれています。"));
		}
	}

	//----------------------
	//File open
	_incFile	= new FileInput();

	_incFile->fileopen(_name.c_str(), &cOptionSW->m_pass_inc);
	if(_incFile->isError() == true){
		f_error = true;
		delete _incFile;		//読み込みに失敗したので、ここでクラスを解放させる。
		Err(_T("インクルードするファイルが見つかりませんでした。"));
	} else {
		nowFile = _incFile;
		ptcFiles.push_back(_incFile);
		iFiles++; 
	}
}

//==============================================================
//		マクロの設定
//--------------------------------------------------------------
//	●引数
//		int		i_Lv	設定するレベル
//	●返値
//				無し
//	●処理
//			現在のファイルポインタに書いてあるマクロを定義する。
//==============================================================
void	MMLfile::SetMacro(int i_Lv)
{
	char	cData;
	string	macro_name		="";
	string	macro_contents	="";

	//Debug用
	if(cOptionSW->iDebug & DEBUG_Macros){
		cout << "Set Macro (Lv=" << i_Lv << ") : ";
	}

	int		iKakko	= 0;

	//------------------
	//マクロ名の取得
	while((cData = cRead()) > 0x20){
		if(cData == '{'){
			Back();
			break;
		}
		macro_name += cData;
	};

	//------------------
	//マクロ名の重複チェック
	if(ptcMac.count(macro_name) != 0){
		Err(_T("既にそのマクロ名は存在しています。"));
	}


	//------------------
	//マクロ内容の取得
	cData = GetChar();
	if(cData != '{'){
		Err(_T("マクロ定義開始を示す{が見つかりませんでした。"));
	}

	while(('}' != (cData = GetChar())) || (iKakko != 0)){
		if(eof()){
			Err(_T("文字列終了を示す}が見つかりませんでした。"));
		}
		if(cData == '{'){
			iKakko++;
		} else if(cData == '}'){
			iKakko--;
		}
		macro_contents += cData;
	}
	macro_contents += " ";

	//------------------
	//マクロ内容の設定
	ptcMac[macro_name] = macro_contents;
	lv_Mac[macro_name] = i_Lv;

	//Debug用
	if(cOptionSW->iDebug & DEBUG_Macros){
		cout << "SetMacro ptcMac[" << macro_name << "] = \"" << macro_contents << "\"" << endl;
	}
}

//==============================================================
//		マクロの設定
//--------------------------------------------------------------
//	●引数
//		int		i_Lv	削除するレベル
//	●返値
//				無し
//	●処理
//			該当レベルのマクロを削除する
//==============================================================
void	MMLfile::DeleteMacro(int i_Lv)
{

	//----------------------
	//Local変数
	string	macro_name;
	int		macro_lv;

	//Debug用
	if(cOptionSW->iDebug & DEBUG_Macros){
		cout << "Delete Macro (Lv=" << i_Lv << ") : " << endl;
	}
		
	//----------------------
	//当該Lvのマクロを解放する。
	map<string,string>::iterator itMac = ptcMac.begin();
	while(itMac != ptcMac.end()){
		macro_name	= itMac->first;
		macro_lv	= lv_Mac[macro_name];
		itMac++;
		if(i_Lv == macro_lv){
			ptcMac.erase(macro_name);
			lv_Mac.erase(macro_name);
			//Debug用
			if(cOptionSW->iDebug & DEBUG_Macros){
				cout << "	ptcMac[" << macro_name << "]" << endl;
			}
		}
	}
}

//==============================================================
//		マクロの呼び出し
//--------------------------------------------------------------
//	●引数
//				無し
//	●返値
//				無し
//	●処理
//			現在のファイルポインタに書いてあるマクロを呼び出す
//==============================================================
void	MMLfile::CallMacro(void)
{
	char							cData;
	size_t							i		= 0;
	size_t							n		= 0;
	string							_name	= "";
	vector<const char*>				vecMac;

	//Debug用
	if(cOptionSW->iDebug & DEBUG_Macros){
		cout << "Call Macro : ";
	}

	//------------------
	//定義された全マクロ名のポインタ取得
	if(!ptcMac.empty()){
		for(map<string,string>::iterator itMac=ptcMac.begin(), e=ptcMac.end(); itMac != e; itMac++){
			vecMac.push_back(itMac->first.c_str());
		}
	}

	//------------------
	//マクロ名の照合
	//※登録されているマクロの中で、一番長く一致するマクロ名を探す。
	i = 0;
	do{
		cData = cRead();
		_name += cData;
		n = 0;			//ヒット数
		if(cData > 0x20){
			vector<const char*>::iterator	it = vecMac.begin();
			while(it != vecMac.end()){
				char c = (*it)[i];
				if(c == cData){
					n++;
					it++;
				} else {
					it = vecMac.erase(it);
				}
			}
		}
		i++;
	} while(n>0);		//ヒット数が0になるまで、繰り返し。

	Back();										//ポインタを１つ戻す。
	_name = _name.substr(0, _name.length()-1);	//１文字減らす。

	//------------------
	//マクロ名の存在チェック
	if(ptcMac.count(_name) == 0){
		Err(_T("そのマクロ名は存在していません。"));
	}

	//------------------
	//マクロ名の重複チェック
	for(i=0; i < p_macro; i++){
		if(s_macro[i].name == _name){
			Err(_T("マクロ内で同じマクロを呼び出しています。"));
		}
	}

	//------------------
	//ネストだったら保存
	if(p_macro > 0){
		s_macro[p_macro-1].name = nowMacro.name;
		s_macro[p_macro-1].line = nowMacro.line;
	}

	//Debug用
	if(cOptionSW->iDebug & DEBUG_Macros){
		cout << "ptcMac[" << _name << "]　(nest = " << p_macro << " )" << endl;
	}

	nowMacro.name = _name;
	nowMacro.line = 0;

	s_macro.push_back(nowMacro);
	p_macro++;
}

//==============================================================
//		パッチの設定
//--------------------------------------------------------------
//	●引数
//				無し
//	●返値
//				無し
//	●処理
//			現在のファイルポインタに書いてあるマクロを定義する。
//==============================================================
void	MMLfile::SetPatch(void)
{

	size_t			i		= GetNum();

	//重複チェック
	if(ptcPatch.count(i) != 0){
		Err(_T("Patch()ブロックで同じ番号が指定されました。"));
	}

	ptcPatch[i] =  new Patch(this, i);

}

//==============================================================
//		パッチの有無チェック
//--------------------------------------------------------------
//	●引数
//			size_t _no	パッチ番号
//	●返値
//			bool				あるかどうか
//	●処理
//			
//==============================================================
bool	MMLfile::ChkPatch(size_t _no)
{
	bool	result;

	if(ptcPatch.count(_no) == 0){
		result = false;
	} else {
		result = true;
	}
	return(result);
}

//==============================================================
//			現在コンパイル処理中のファイルポインタの取得
//--------------------------------------------------------------
//	●引数
//						無し
//	●返値
//		std::streamoff	現在のファイルポインタ
//==============================================================
std::streamoff	MMLfile::tellg(void)
{
	std::streamoff	i;

	if(p_macro > 0){
		i = nowMacro.line;
	} else {
		i = nowFile->tellg();
	}

	return(i);
}

//==============================================================
//			現在コンパイル処理中のファイルポインタを絶対的に移動
//--------------------------------------------------------------
//	●引数
//			std::streamoff	iSize	移動値
//	●返値
//			無し
//==============================================================
void	MMLfile::StreamPointerMove(std::streamoff iSize)
{
	if(p_macro > 0){
		nowMacro.line = iSize;
	} else {
		nowFile->StreamPointerMove(iSize);
	}
}

//==============================================================
//			現在コンパイル処理中のファイルポインタを一つ戻す
//--------------------------------------------------------------
//	●引数
//			無し
//	●返値
//			無し
//==============================================================
void	MMLfile::Back_one(void)
{
	if(f_macro == true){
		f_macro = false;
		if(p_macro > 0){
			s_macro[p_macro-1].name = nowMacro.name;
			s_macro[p_macro-1].line = nowMacro.line;
		}
		p_macro++;
		nowMacro.name = s_macro[p_macro-1].name;
		nowMacro.line = s_macro[p_macro-1].line;
		nowMacro.line--;
	} else {
		if(p_macro > 0){
			nowMacro.line--;
		} else {
			nowFile->Back();
		}
	}
}

void	MMLfile::Back(void)
{
	if(f_2to1==true){
		Back_one();
		Back_one();		//前回の読み込みがマルチバイト文字だったら、２つ戻す。
	} else {
		Back_one();
	}

}

//==============================================================
//			１Byte読み込み
//--------------------------------------------------------------
//	●引数
//			無し
//	●返値
//			char	読み込み値
//==============================================================
char	MMLfile::read_char(void)
{
	char	cData;

	//直前の読み込みで、マクロの終端に行った。
	if(f_macro == true){
		f_macro	= false;		
		s_macro.pop_back();		//マクロスタックを１つ戻す。
	}

	if(p_macro > 0){
		//マクロから読み込む場合
		cData = ptcMac[nowMacro.name][nowMacro.line];
		nowMacro.line++;
		//マクロ終端？
		if(ptcMac[nowMacro.name].size() == nowMacro.line){
			s_macro[p_macro-1].name = nowMacro.name;
			s_macro[p_macro-1].line = nowMacro.line;
			p_macro--;
			f_macro	= true;
			if(p_macro > 0){
				nowMacro.name = s_macro[p_macro-1].name;
				nowMacro.line = s_macro[p_macro-1].line;
			}
		}
	} else {
		//ファイルから読み込む場合
		cData		= nowFile->cRead();
	}

	return(cData);

}

//==============================================================
//			１Byte読み込み
//--------------------------------------------------------------
//	●引数
//			無し
//	●返値
//			char	読み込み値
//==============================================================
char	MMLfile::cRead(void)
{
	unsigned	char	cData = read_char();
	unsigned	char	cDataMSB;

	switch(cData){
		case(0x81):
			cDataMSB = read_char();
			f_2to1 = true;
			switch(cDataMSB){
				case(0x67):
					cData = '"';
					break;
				case(0x68):
					cData = '"';
					break;
				case(0x69):
					cData = '(';
					break;
				case(0x6A):
					cData = ')';
					break;
				case(0x6F):
					cData = '{';
					break;
				case(0x70):
					cData = '}';
					break;
				case(0x7B):
					cData = '+';
					break;
				case(0x7C):
					cData = '-';
					break;
				case(0x94):	//＃
					cData = '#';
					break;
				case(0xF3):	//♭
					cData = '-';
					break;
				default:
					f_2to1 = false;
					Back();
					break;
			}
			break;

		case(0x82):
			cDataMSB = read_char();
			f_2to1 = true;
			switch(cDataMSB){
				case(0x4F):
					cData = '0';
					break;
				case(0x50):
					cData = '1';
					break;
				case(0x51):
					cData = '2';
					break;
				case(0x52):
					cData = '3';
					break;
				case(0x53):
					cData = '4';
					break;
				case(0x54):
					cData = '5';
					break;
				case(0x55):
					cData = '6';
					break;
				case(0x56):
					cData = '7';
					break;
				case(0x57):
					cData = '8';
					break;
				case(0x58):
					cData = '9';
					break;
				default:
					f_2to1 = false;
					Back();
					break;
			}
			break;

		default:
			f_2to1 = false;
			break;
	}

	return(cData);
}

//==============================================================
//			１Byte読み込み（チェック付き）
//--------------------------------------------------------------
//	●引数
//			無し
//	●返値
//			char	読み込み値
//	●処理
//		１文字の読み込み。
//		但し、以下の記述があった場合は、そこを飛ばす。
//			・0x20以下のキャラクターコード
//			・コメント
//		また、インクルードファイル中で[EOF]を検出したら、前のファイルに戻る
//==============================================================
char	MMLfile::GetChar(void)		//1Byteの読み込み
{
	unsigned	char	cData;

	do{
		cData = cRead();

		//[EOM]？
		if( eom() )
		{
			return(0);

		//[EOF]？
		} else if( eof() ){
			nowFile->close();
			delete	nowFile;
			ptcFiles.pop_back();
			nowFile = ptcFiles.back();
			iFiles--;
			//continue

		//0x20以下の制御コード
		} else if(cData <= 0x20){
			//continue

		//アセンブリ言語タイプのコメント？
		} else if(cData == ';'){
			do{
				if( eof() ){
					break;
				}
				cData = cRead();		//次のバイトを読み込み
			} while(cData != 0x0A);
		
		//Ｃ言語タイプのコメント？
		} else 	if(cData == '/'){
			cData = cRead();		//次のバイトを読み込み
			switch(cData){

				//１行コメント
				case('/'):
					do{
						if( eof() ){
							break;
						}
						cData = cRead();		//次のバイトを読み込み
					}while(cData != 0x0A);
					break;

				//範囲コメント
				case('*'):
					do{
						do{
							cData = cRead();		//次のバイトを読み込み
							if(eof()){
								Err(_T("コメント終端 */ が見つかりませんでした。"));
							}
						}while(cData != '*');
						cData = cRead();
						Back();					//StreamPointerAdd(-1);
					} while(cData != '/');
							cRead();			//StreamPointerAdd(+1); をしたい。
					break;

				//それ以外
				default:
					Err(_T("'/'が１つしか見つかりませんでした。"));
					break;
			}

		//[EOF]でも、コメントでも無い場合。
		} else {
			break;		//while(1) の break;
		}

	} while(true);

	return(cData);
}

//==============================================================
//			{があるかチェック
//--------------------------------------------------------------
void	MMLfile::ChkBlockStart(void)
{
	// { の検索
	while(1){
		char cData = GetChar();
		if(cData == '{'){
			break;
		} else if(cData == '='){
			//=は許可する。
			continue;
		} else {
			//それ以外はエラーにする
			Err(_T("ブロックの開始を示す{が見つかりませんでした。"));
		}
	}
}

//==============================================================
//			[EOF]のチェック
//--------------------------------------------------------------
void	MMLfile::ChkEOF(void)
{
	// } が来る前に、[EOF]が来たらエラー
	if(eof()){
		Err(_T("ブロックの終端を示す`}'がありませんでした。"));
	}
}

//==============================================================
//			[EOF]の前に}があるかチェック
//--------------------------------------------------------------
//	●引数
//			chae	cData*	読み込んだデータを格納するポインタ
//	●返値
//			bool			読み込んだデータが`}'であったらfalse
//==============================================================
bool	MMLfile::GetChar_With_ChkEOF(char* cData)
{
	//読み込み
	*cData = GetChar();

	ChkEOF();

	//`}'のチェック
	return(*cData != '}');
}

//==============================================================
//			文字列読み込み
//--------------------------------------------------------------
//	●引数
//			bool	f_ESC	エスケープシーケンスの可否
//							true: ESCシーケンス ok
//	●返値
//			string			読み込んだ文字列
//	●処理
//			MML中で"と"で囲まれた文字列を取得する。
//==============================================================
void	MMLfile::GetString(string* _str, bool	f_ESC)
{
	int		i;
	char	cData;

	cData = GetChar();
	if(cData != '"'){
			Err(_T("文字列開始を示す\"が見つかりませんでした。"));
	}

	while('"' != (cData = cRead())){
		if(eof()){
			Err(_T("文字列終了を示す\"が見つかりませんでした。"));
		}
		if((f_ESC == true) && (cData == '\\')){
			cData = GetChar();
			if(eof()){
				Err(_T("文字列終了を示す\"が見つかりませんでした。"));
			}
			switch(cData){
				case('a'):
					cData = '\a';
					break;
				case('b'):
					cData = '\b';
					break;
				case('f'):
					cData = '\f';
					break;
				case('n'):
					cData = '\n';
					break;
				case('r'):
					cData = '\r';
					break;
				case('t'):
					cData = '\t';
					break;
				case('v'):
					cData = '\v';
					break;
				case('\''):
					cData = '\'';
					break;
				case('\"'):
					cData = '\"';
					break;
				case('\\'):
					cData = '\\';
					break;
				case('\?'):
					cData = '\?';
					break;
				case('x'):
					i = GetHex();
					cData = (char)(i & 0xFF);
					break;
				case('0'):
				case('1'):
				case('2'):
				case('3'):
				case('4'):
				case('5'):
				case('6'):
				case('7'):
					Back();
					i = GetOct();
					cData = (char)(i & 0xFF);
					break;
				default:
					*_str += '\\';
					break;
			}
		}
		*_str += cData;
	}
}

//==============================================================
//			数値読み込み
//--------------------------------------------------------------
//	●引数
//			無し
//	●返値
//			int		読み込んだ数値
//	●処理
//			MML中で(と)で囲まれた文字列を取得する。
//==============================================================
int	MMLfile::GetNum(void)
{
	char	cData;
	int		iResult;

	cData = GetChar();
	if(cData != '('){
		Err(_T("数値開始を示す(が見つかりませんでした。"));
	}

	iResult = GetInt();

	cData = GetChar();
	if(cData != ')'){
		Err(_T("数値終了を示す)が見つかりませんでした。"));
	}

	return(iResult);
}

//==============================================================
//			数値読み込み
//--------------------------------------------------------------
//	●引数
//			無し
//	●返値
//			int		読み込んだ数値
//==============================================================
int		MMLfile::GetInt(void)
{
				bool	neg;				//符号
				int		iResult;			//演算結果
	unsigned	char	cData = GetChar();

	//16進数
	if(cData == '$'){
		neg		= chkSigh();
		iResult = GetHex();

	//２進数
	} else if(cData == '%'){
		neg		= chkSigh();
		iResult = GetBin();

	//10進数
	} else if(((cData >= '0') && (cData <= '9')) || (cData == '-') || (cData == '+')){
		Back();		//ポインタを１つ戻す
		neg		= chkSigh();
		iResult = GetDec();

	} else {
		Err(_T("数値以外が指定されました。"));
	}

	//符号
	if(neg == true){
		iResult = -iResult;
	}

	return(iResult);
}

//--------------------------------------------------------------
bool	MMLfile::chkSigh(void)
{
				bool	neg;
	unsigned	char	cData = GetChar();

	switch(cData){
		case('-'):
			neg = true;
			break;
		case('+'):
			neg = false;
			break;
		default:
			Back();		//ポインタを１つ戻す
			neg = false;
			break;
	}
	return(neg);
}

//--------------------------------------------------------------
int	MMLfile::GetHex(void)
{
				int		iResult = 0;			//演算結果
	unsigned	char	cData = GetChar();

	while(((cData >= '0') && (cData <= '9')) || ((cData >= 'a') && (cData <= 'f')) || ((cData >= 'A') && (cData <= 'F'))){
		iResult <<= 4;
		if((cData >= '0') && (cData <= '9')){
			iResult += (unsigned int)cData - 0x30;
		} else if((cData >= 'A') && (cData <= 'F')){
			iResult += (unsigned int)cData - 0x41 + 10;
		} else if((cData >= 'a') && (cData <= 'f')){
			iResult += (unsigned int)cData - 0x61 + 10;
		}
		cData	 = cRead();
	}

	//ポインタを１つ戻す
	Back();							//	StreamPointerAdd(-1);

	return(iResult);
}

//--------------------------------------------------------------
int	MMLfile::GetDec(void)
{
				int		iResult = 0;			//演算結果
	unsigned	char	cData = GetChar();		

	while((cData >= '0') && (cData <= '9')){
		iResult *= 10;
		iResult += (unsigned int)cData - 0x30;
		cData	 = cRead();
	}

	//ポインタを１つ戻す
	Back();							//	StreamPointerAdd(-1);

	return(iResult);
}

//--------------------------------------------------------------
int	MMLfile::GetOct(void)
{
				int		iResult = 0;			//演算結果
	unsigned	char	cData = GetChar();		

	while((cData >= '0') && (cData <= '7')){
		iResult <<= 3;
		iResult += (unsigned int)cData - 0x30;
		cData	 = cRead();
	}

	//ポインタを１つ戻す
	Back();							//	StreamPointerAdd(-1);

	return(iResult);
}

//--------------------------------------------------------------
int	MMLfile::GetBin(void)
{
				int		iResult = 0;			//演算結果
	unsigned	char	cData = GetChar();		

	while((cData >= '0') && (cData <= '1')){
		iResult <<= 1;
		iResult += (unsigned int)cData - 0x30;
		cData	 = cRead();
	}

	//ポインタを１つ戻す
	Back();							//	StreamPointerAdd(-1);

	return(iResult);
}

//==============================================================
//			音長読み込み
//--------------------------------------------------------------
//	●引数
//			int		記載がない場合の音長[tick]
//	●返値
//			int		音長[tick]
//	●処理
//		音長を読み込む（付点付き）
//==============================================================
int	MMLfile::readLength(int DefaultLength){

	char	cData;				//読み込み用
	int		iLength;			//音長 [tick]
	int		iDot;				//付点計算用 [tick]
	int		iMod;
	int		i;

	//読み込み
	cData = GetChar();

	//ポインタを１つ戻す
	Back();							//StreamPointerAdd(-1);

	//Length
	if(((cData >= '0') && (cData <= '9')) || (cData == '.')){
		if((cData >= '0') && (cData <= '9')){
			i = GetInt();
			if(i==0){
				Err(_T("音長に0は使えません。"));
			}
			iLength = (timebase * 4) / i;
			iMod	= (timebase * 4) % i;
			if(iMod != 0){
				Warning(_T("音長の計算で割り切れませんでした。小数点は切捨てします。"));
			}
		} else {
			//付点だけ記述される場合。
			if(DefaultLength == -1){
				Err(_T("音長を記述して下さい。"));
			}
			iLength = DefaultLength;
		}

		iDot	= iLength;
		//付点
		while((cData = cRead()) == '.'){
			if(iLength == -1){
				Err(_T("音長の記述なしに付点を使う場合は、前もってl コマンドを記述して下さい。"));
			}
			iMod = (iDot & 0x01);
			iDot >>= 1;
			if(iMod != 0){
				Warning(_T("付点の計算で割り切れませんでした。小数点は切捨てします。"));
			}
			iLength += iDot;
		};
		Back();							//StreamPointerAdd(-1);



	//Tick
	} else if (cData == '%'){
		GetChar();	//1つ進める。
		iLength = GetInt();

	} else {
		//引数を書かない場合
		if(DefaultLength == -1){
			Err(_T("音長を記述して下さい。"));
		}
		iLength = -1;
	}

	return(iLength);
}

//==============================================================
//			音長読み込み
//--------------------------------------------------------------
//	●引数
//			int		記載がない場合の音長[tick]
//	●返値
//			int		読み込んだ数値
//					音長がかかれてない場合は、 -1 を返す。
//==============================================================
int		MMLfile::GetLength(int DefaultLength)	//
{
	char	cData;				//読み込み用
	int		iLength;			//音長 [tick]
	int		iCalc;
	bool	add;

	//音長読み込み
	iLength = readLength(DefaultLength);

	//音長の加減算
	cData = cRead();
	while((cData == '-') || (cData == '~') || (cData == '+') || ((iTieMode == 1) && (cData == '^'))){
		if(iLength == -1){
			iLength = DefaultLength;
		}

		//後で計算する符号のチェック
		if((cData == '+') || (cData == '^')){
			add = true;
		} else {
			add = false;
		}
		iCalc = readLength(DefaultLength);

		if(iCalc == -1){
		//	Warning(_T("音長の加減算値に数値以外が指定されています。"));
			iCalc = DefaultLength;
		}

		if(add == true){
			iLength += iCalc;	// '+' と '^' は加算
		} else {
			iLength -= iCalc;	// '-' と '~' は減算
		}
		cData = cRead();
	};
	Back();							//StreamPointerAdd(-1);

	if(iLength == -1){
		return(iLength);
	}

	if((iLength < 1) || (iLength > 255)){
		Err(_T("音長は、%1（96）～%255（1+1+2+8）の間で指定して下さい。255[tick]を超える場合はタイ`&', `^'を使って下さい。"));
	}

	return(iLength);
}

//==============================================================
//		コマンドIDの取得
//--------------------------------------------------------------
//	●引数
//		Command_Info*		_command
//		size_t				_size
//	●返値
//		int			コマンドコード　（-1でunknown command）
//==============================================================
int	MMLfile::GetCommandID(const Command_Info _command[], size_t _size)
{
	std::streamoff	ptCmdEnd	= tellg();
			size_t	i = 0;					//走査用
			size_t	n = 0;					//ヒット数用
			int		iResult = -1;

	map<const char*, int>	mapCmdInfo;

	//走査用のオブジェクト作成
	for(i=0; i<_size; i++){
		mapCmdInfo[_command[i].str] = _command[i].id;
	}

	//走査
	i = 0;
	do{
		map<const char*, int>::iterator	it = mapCmdInfo.begin();
		char cData = cRead();
		n = 0;
		while(it != mapCmdInfo.end()){
			char c = (it->first)[i];
			if(c == cData){
				n++;
				it++;
			} else if(c == 0){
				Back();
				ptCmdEnd = tellg();		//ヒットしたところのファイルポインタを記憶
				cRead();
				iResult = it->second;	//ヒットしたコマンドID
				mapCmdInfo.erase(it++);
			} else {
				mapCmdInfo.erase(it++);
			}
		}
		i++;
	} while((n>0) && (f_macro == false));	//ヒット数が0になるまで、繰り返し。

	if(f_macro == true){
		Back();
	}
	StreamPointerMove(ptCmdEnd);

	return(iResult);
}

//==============================================================
//		エラー処理
//--------------------------------------------------------------
void	MMLfile::ErrUnknownCmd()
{
	Err(_T("Unknown Command"));
	cRead();		//ポインタを一つ進める。
}

//==============================================================
//		エラー処理
//--------------------------------------------------------------
//	●引数
//		const	_CHAR	msg[]	エラーメッセージ
//	●返値
//				無し
//==============================================================
void	MMLfile::Err(const _CHAR msg[])
{
	_OMP_SET_LOCK(lock_cout)
	f_error = true;

	//エラー内容を表示
	if(cOptionSW->fErr == true){
		//現在のファイル名と、行数を表示
		cerr << "[ ERROR ] " << nowFile->GetFilename()->c_str() << " (Line = " << nowFile->GetLine() << ") : ";
		_CERR << msg << endl;
	} else {
		//現在のファイル名と、行数を表示
		cout << "[ ERROR ] " << nowFile->GetFilename()->c_str() << " (Line = " << nowFile->GetLine() << ") : ";
		_COUT << msg << endl;
	}
	_OMP_UNSET_LOCK(lock_cout)

	//異常終了
	nsc_exit(EXIT_FAILURE);
}

//==============================================================
//		ワーニング処理
//--------------------------------------------------------------
//	●引数
//		const	_CHAR	msg[]	ワーニングメッセージ
//	●返値
//				無し
//==============================================================
void	MMLfile::Warning(const _CHAR msg[])
{
	_OMP_SET_LOCK(lock_cout)
	//ワーニング内容を表示
	if(cOptionSW->fErr == true){
		//現在のファイル名と、行数を表示
		cerr << "[WARNING] " << nowFile->GetFilename()->c_str() << " (Line = " << nowFile->GetLine() << ") : ";
		_CERR << msg << endl;
	} else {
		//現在のファイル名と、行数を表示
		cout << "[WARNING] " << nowFile->GetFilename()->c_str() << " (Line = " << nowFile->GetLine() << ") : ";
		_COUT << msg << endl;
	}
	_OMP_UNSET_LOCK(lock_cout)
}
