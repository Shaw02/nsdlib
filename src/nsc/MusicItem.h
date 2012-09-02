#pragma once

/****************************************************************/
/*			プロトタイプ										*/
/****************************************************************/
class	MusicFile;


/****************************************************************/
/*																*/
/*			クラス定義											*/
/*																*/
/****************************************************************/
class MusicItem
{
//メンバー変数
protected:
	const		char*	strName;		//オブジェクトの名称
	vector<MusicItem*>	ptcItem;		//構造化
				string	code;
	unsigned	int		iSize;
	unsigned	int		iOffset;		//SNDファイルポインタ
	

//メンバー関数
public:
	MusicItem(const char _strName[]="");
	~MusicItem(void);

	unsigned	int		getSize();
	unsigned	int		getOffset();
	unsigned	int		SetOffset(unsigned	int	_offset);

	virtual		void	getCode(string* _str);
};
