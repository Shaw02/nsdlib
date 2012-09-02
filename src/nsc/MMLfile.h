#pragma once

/****************************************************************/
/*																*/
/*			定数定義											*/
/*																*/
/****************************************************************/
#define	MML_timebase	24

/****************************************************************/
/*																*/
/*			クラス定義											*/
/*																*/
/****************************************************************/
class MMLfile	// :
//	public FileInput
{
//メンバー変数
private:
	vector	<	FileInput*	>		ptcFiles;			//MMLファイル
				FileInput*			nowFile;			//現在のファイル
	unsigned	int					iFiles;				//現在のファイルNo.
public:
				bool				octave_reverse;		//Octave Reverse

//メンバー関数
public:
	MMLfile(const char*	strFileName);
	~MMLfile(void);

				void	include();				//現ポインタにあるファイルを#includeする
	
				bool	eof(void);				//現在のファイルのEOFチェック
				bool	eom(void);				//ＭＭＬの終了チェック

				int		tellg(void);						//現在のファイルのポインタ取得
				void	StreamPointerAdd(__int32 iSize);	//現在のファイルのポインタ移動
				void	StreamPointerMove(__int32 iSize);	//現在のファイルのポインタ移動
				void	Back(void);

				char	cRead(void);		//1Byte読み込み
				char	GetChar(void);		//1Byte読み込み（with EOF & Commend check）
				string	GetString(void);	//""付 文字列 読み込み
				int		GetNum(void);		//()付  数値  読み込み
				int		GetInt(void);		//数値読み込み
				int		GetLength(void);	//音長読み込み（演算付き）
				int		readLength(void);	//音長読み込み

				int		GetCommandID(const Command_Info _command[], unsigned int _size);	//コマンドIDを取得

	void		Err(const char* msg);
	void		Warning(const char* msg);
};
