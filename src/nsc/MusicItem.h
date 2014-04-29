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
	const		wchar_t*	strName;		//オブジェクトの名称
	vector<MusicItem*>		ptcItem;		//構造化
				string		code;
				size_t		iSize;
	unsigned	int			iOffset;		//SNDファイルポインタ
	

//メンバー関数
public:
	MusicItem(const wchar_t _strName[]=L"");
	~MusicItem(void);

				size_t	getSize();
	unsigned	int		getOffset();
	unsigned	int		SetOffset(unsigned	int	_offset);

	unsigned	char	getCode(int n);
	virtual		void	getCode(string* _str);
	virtual		void	getAsm(MusicFile* MUS);
};
