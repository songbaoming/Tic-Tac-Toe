#define PERSON		1
#define COMPUTER	2
#define WM_COMDROP	(WM_USER+0x100)

//Ӧ�ó�����
class CMyApp : public CWinApp
{
public:
	virtual BOOL InitInstance();
};

//��ܴ�����
class CMainWindow : public CWnd
{
protected:
	static const CRect m_rcSquares[9];	//������������
	static const int m_nLines[8][3];
	int m_nGameGrid[9];					//����λ��ռ�ü�¼
	int m_nNextOne;						//����

	void DrawBoard(CDC *pdc);			//������
	void DrawO(CDC *pdc, int nPos);		//��O
	void DrawX(CDC *pdc, int nPos);		//��X
	int GetRectID(CPoint point);		//�жϵ��λ�����ĸ�������
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