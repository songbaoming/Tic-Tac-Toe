#define PERSON		1
#define COMPUTER	2
#define WM_COMDROP	(WM_USER+0x100)

//应用程序类
class CMyApp : public CWinApp
{
public:
	virtual BOOL InitInstance();
};

//框架窗口类
class CMainWindow : public CWnd
{
protected:
	static const CRect m_rcSquares[9];	//棋盘填子坐标
	static const int m_nLines[8][3];
	int m_nGameGrid[9];					//棋盘位置占用记录
	int m_nNextOne;						//棋手

	void DrawBoard(CDC *pdc);			//画棋盘
	void DrawO(CDC *pdc, int nPos);		//画O
	void DrawX(CDC *pdc, int nPos);		//画X
	int GetRectID(CPoint point);		//判断点击位置在哪个方框内
	int CalcValue(int nPos);
	bool InThisLine(int nPos, int nLine);
	void CheckForGameOver();
	int IsSomeoneWin();
	BOOL IsDraw();
	void Replay();
public:
	CMainWindow();
protected:
	virtual void PostNcDestroy();

	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg LRESULT	OnComputerDrop(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()

};