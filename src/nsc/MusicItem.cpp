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
//		MusicFile*	SND
//		int			iOffset
//	●返値
//				無し
//==============================================================
MusicItem::MusicItem(const char _strName[]):
	iOffset(0),
	strName(_strName)
{
	//Debug message　（うざい程出力するので注意。）
	if(cOptionSW->cDebug & 0x01){
		cout << "Create Music Object : " << strName << endl;
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
	//----------------------
	//Local変数
	vector<	MusicItem*>::iterator	itItem;

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

//==============================================================
//		コードサイズの取得
//--------------------------------------------------------------
//	●引数
//				無し
//	●返値
//				無し
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
//				無し
//==============================================================
unsigned	int		MusicItem::getOffset()
{
	return(iOffset);
}

//==============================================================
//		オフセットアドレスの設定
//--------------------------------------------------------------
//	●引数
//				無し
//	●返値
//				無し
//==============================================================
unsigned	int		MusicItem::SetOffset(unsigned	int _offset)
{
	//----------------------
	//Local変数
	vector<	MusicItem*>::iterator	itItem;

	//Debug message　（うざい程出力するので注意。）
	if(cOptionSW->cDebug & 0x02){
		cout << "Object Address [0x" << hex << _offset << dec << "]: " << strName << endl;
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
//				無し
//	●返値
//				無し
//==============================================================
void	MusicItem::getCode(string* _str)
{
	//----------------------
	//Local変数
	vector<	MusicItem*>::iterator	itItem;

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
//		コードの取得
//--------------------------------------------------------------
//	●引数
//				無し
//	●返値
//				無し
//==============================================================
void	MusicItem::getAsm(MusicFile* MUS)
{
	//----------------------
	//Local変数
	unsigned	int	i = 0;
	vector<	MusicItem*>::iterator	itItem;

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
