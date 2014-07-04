#include "StdAfx.h"
#include "MusicItem.h"

/****************************************************************/
/*					グローバル変数（クラスだけど・・・）		*/
/****************************************************************/
extern	OPSW*			cOptionSW;	//オプション情報へのポインタ変数

//==============================================================
//		コンストラクタ
//--------------------------------------------------------------
//	●引数
//		const		wchar_t	_strName[]	オブジェクト名
//	●返値
//				無し
//==============================================================
MusicItem::MusicItem(const wchar_t _strName[]):
	iOffset(0),
	iSize(0),
	strName(_strName)
{
	//Debug message　（うざい程出力するので注意。）
	if(cOptionSW->cDebug & 0x01){
		wcout << L"Create Music Object : " << strName << endl;
	}
}

MusicItem::MusicItem(int _id, const wchar_t _strName[]):
	iOffset(0),
	iSize(0),
	strName(_strName)
{
	//Debug message　（うざい程出力するので注意。）
	if(cOptionSW->cDebug & 0x01){
		wcout << L"Create Music Object : ====[ " << strName << L"(";
		cout << _id;
		wcout << L") ]====" << endl;
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
MusicItem::~MusicItem(void)
{
	clear();

	//Debug message　（うざい程出力するので注意。）
	if(cOptionSW->cDebug & 0x80){
		wcout << L"Delete Music Object : " << strName << endl;
	}
}

//==============================================================
//		クリア
//--------------------------------------------------------------
//	●引数
//				無し
//	●返値
//				無し
//==============================================================
void	MusicItem::clear(void)
{
	//----------------------
	//Local変数
	list<	MusicItem*>::iterator	itItem;

	//----------------------
	//clear
	code.clear();

	//----------------------
	//Delete Class
	if(!ptcItem.empty()){
		itItem = ptcItem.begin();
		while(itItem != ptcItem.end()){
			delete *itItem;
			itItem++;
		}
		ptcItem.clear();
	}
}

void	MusicItem::clear(int _id)
{

	//Debug message　（うざい程出力するので注意。）
	if(cOptionSW->cDebug & 0x40){
		wcout << L"Clear Music Object : ====[ " << strName << L"(" << _id << L") ]====" << endl;
	}

	clear();
}

//==============================================================
//		コードサイズの取得
//--------------------------------------------------------------
//	●引数
//				無し
//	●返値
//		size_t
//==============================================================
size_t		MusicItem::getSize()
{
	return(iSize);
}

//==============================================================
//		コードのオフセットアドレスの取得
//--------------------------------------------------------------
//	●引数
//				無し
//	●返値
//		unsigned	int	
//==============================================================
unsigned	int		MusicItem::getOffset()
{
	return(iOffset);
}

//==============================================================
//		オフセットアドレスの設定
//--------------------------------------------------------------
//	●引数
//		unsigned	int		_offset
//	●返値
//				無し
//==============================================================
unsigned	int		MusicItem::SetOffset(unsigned	int _offset)
{
	//----------------------
	//Local変数
	list<	MusicItem*>::iterator	itItem;
	int		i	=	0;

	//Debug message　（うざい程出力するので注意。）
	if(cOptionSW->cDebug & 0x02){
		wcout << L"Object Address [0x" << hex << setw(4) << setfill(L'0') << _offset << L"]: ";
		while(i < code.size()){
			wcout	<<	hex	<<	setw(2)	<<	setfill(L'0')	<<	(unsigned int)(code[i] & 0xFF)	<<	L" ";
			i++;
		}
		wcout  << dec	<< L": " << strName << endl;
	}

	iOffset = _offset;
	_offset	+= (unsigned int)code.size();

	if(!ptcItem.empty()){
		itItem = ptcItem.begin();
		while(itItem != ptcItem.end()){
			_offset = (*itItem)->SetOffset(_offset);
			itItem++;
		}
	}

	return(_offset);
}

//==============================================================
//		コードの取得
//--------------------------------------------------------------
//	●引数
//		unsigned	int	n	添え字
//	●返値
//		unsigned	char	内容
//==============================================================
unsigned	char	MusicItem::getCode(int n)
{
	unsigned	char	iCode;
	
	if((n<0) || (n>iSize)){
		iCode = -1;
	} else {
		iCode = code[n];
	}

	return(iCode);
}

//==============================================================
//		コードの取得
//--------------------------------------------------------------
//	●引数
//		string*		_str
//	●返値
//				無し
//==============================================================
void	MusicItem::getCode(string* _str)
{
	//----------------------
	//Local変数
	list<	MusicItem*>::iterator	itItem;

	_str->append(code);

	if(!ptcItem.empty()){
		itItem = ptcItem.begin();
		while(itItem != ptcItem.end()){
			(*itItem)->getCode(_str);
			itItem++;
		}
	}
}

//==============================================================
//		コードの設定
//--------------------------------------------------------------
//	●引数
//		string*		_str
//	●返値
//				無し
//==============================================================
void	MusicItem::setCode(string* _str)
{
	code.clear();
	code.assign(*_str);
	iSize = code.size();
}

//==============================================================
//		コードの取得
//--------------------------------------------------------------
//	●引数
//		MusicFile*	MUS		コードを出力する曲データファイル・オブジェクト
//	●返値
//				無し
//==============================================================
void	MusicItem::getAsm(MusicFile* MUS)
{
	//----------------------
	//Local変数
	unsigned	int	i = 0;
	list<	MusicItem*>::iterator	itItem;

	if(code.size() > 0){
		while(i < code.size()){
			if(i==0){
				*MUS << "	.byte	$";
			} else {
				*MUS << " ,$";
			}
			*MUS << hex << setw(2) << setfill('0') << (int)(code[i] & 0xFF);
			i++;
		}
		*MUS << dec << endl;
	}

	if(!ptcItem.empty()){
		itItem = ptcItem.begin();
		while(itItem != ptcItem.end()){
			(*itItem)->getAsm(MUS);
			itItem++;
		}
	}
}
