#include <sdkddkver.h>
#include <afxwin.h>
#include "Tic-Tac-Toe.h"
#include "resource.h"

CMyApp myApp;//程序实例
//实例初始化
BOOL CMyApp::InitInstance()
{
	m_pMainWnd = new CMainWindow;
	m_pMainWnd->ShowWindow(m_nCmdShow);
	m_pMainWnd->UpdateWindow();
	return TRUE;
}
//消息映射表
BEGIN_MESSAGE_MAP(CMainWindow, CWnd)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_MESSAGE(WM_COMDROP, OnComputerDrop)
END_MESSAGE_MAP()

const CRect CMainWindow::m_rcSquares[9] = {
	CRect( 16,  16, 112, 112),
	CRect(128,  16, 224, 112),
	CRect(240,  16, 336, 112),
	CRect( 16, 128, 112, 224),
	CRect(128, 128, 224, 224),
	CRect(240, 128, 336, 224),
	CRect( 16, 240, 112, 336),
	CRect(128, 240, 224, 336),
	CRect(240, 240, 336, 336)
};

const int CMainWindow::m_nLines[8][3] = {
	0, 1, 2,
	3, 4, 5,
	6, 7, 8,
	0, 3, 6,
	1, 4, 7,
	2, 5, 8,
	0, 4, 8,
	2, 4, 6
};

CMainWindow::CMainWindow()
{
	m_nNextOne = PERSON;
	::ZeroMemory(m_nGameGrid, 9 * sizeof(int));

	CString strWcl = AfxRegisterWndClass(
		CS_DBLCLKS,
		AfxGetApp()->LoadStandardCursor(IDC_ARROW),
		(HBRUSH)(COLOR_3DFACE + 1),
		AfxGetApp()->LoadIcon(IDI_APPICON));

	CreateEx(0, strWcl, TEXT("井字棋"),
		WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, NULL);

	CRect rect(0, 0, 352, 352);
	CalcWindowRect(&rect);
	SetWindowPos(NULL, 0, 0, rect.Width(), rect.Height(),
		SWP_NOZORDER | SWP_NOMOVE | SWP_NOREDRAW);
}

void CMainWindow::PostNcDestroy()
{
	delete this;
}

void CMainWindow::OnPaint()
{
	CPaintDC dc(this);//WM_PAINT 必须用 CPaintDC 才能清除 WM_PAINT，否则一直调用OnPaint

	DrawBoard(&dc);
}

void CMainWindow::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (m_nNextOne != PERSON)
		return;
	int pos = GetRectID(point);
	if (pos == -1 || m_nGameGrid[pos] != 0)	//单击了方框并且方框未曾使用
		return;

	m_nGameGrid[pos] = PERSON;
	CClientDC dc(this);
	DrawX(&dc, pos);
	m_nNextOne = COMPUTER;
	CheckForGameOver();
}

LRESULT CMainWindow::OnComputerDrop(WPARAM wParam, LPARAM lParam)
{
	if (m_nNextOne != COMPUTER)
		return 0;

	int maxVal = 0, nPos;
	for (int i = 0; i < 9; ++i)
	{
		if (m_nGameGrid[i])
			continue;
		int temp = CalcValue(i);
		if (temp>maxVal)
		{
			maxVal = temp;
			nPos = i;
		}
	}
	m_nGameGrid[nPos] = COMPUTER;
	CClientDC dc(this);
	DrawO(&dc, nPos);
	m_nNextOne = PERSON;
	CheckForGameOver();

	return 0;
}

void CMainWindow::DrawBoard(CDC *pdc)
{
	CPen pen(PS_SOLID, 16, RGB(0, 0, 0));
	CPen *pOldPen = pdc->SelectObject(&pen);

	pdc->MoveTo(120, 16);
	pdc->LineTo(120, 336);

	pdc->MoveTo(232, 16);
	pdc->LineTo(232, 336);

	pdc->MoveTo(16, 120);
	pdc->LineTo(336, 120);

	pdc->MoveTo(16, 232);
	pdc->LineTo(336, 232);

	for (int i = 0; i < 9; ++i)
	{
		if (m_nGameGrid[i] == PERSON)
			DrawX(pdc, i);
		else if (m_nGameGrid[i] == COMPUTER)
			DrawO(pdc, i);
	}
	pdc->SelectObject(pOldPen);
}

void CMainWindow::DrawO(CDC *pdc, int nPos)
{
	CPen pen(PS_SOLID, 16, RGB(0, 0, 255));
	CPen *pOldPen = pdc->SelectObject(&pen);
	pdc->SelectStockObject(NULL_BRUSH);

	CRect rect = m_rcSquares[nPos];
	rect.DeflateRect(16, 16);
	pdc->Ellipse(rect);

	pdc->SelectObject(pOldPen);
}

void CMainWindow::DrawX(CDC *pdc, int nPos)
{
	CPen pen(PS_SOLID, 16, RGB(255, 0, 0));
	CPen *pOldPen = pdc->SelectObject(&pen);

	CRect rect = m_rcSquares[nPos];
	rect.DeflateRect(16, 16);
	pdc->MoveTo(rect.left, rect.top);
	pdc->LineTo(rect.right, rect.bottom);
	pdc->MoveTo(rect.right, rect.top);
	pdc->LineTo(rect.left, rect.bottom);

	pdc->SelectObject(pOldPen);
}

int CMainWindow::GetRectID(CPoint point)
{
	for (int i = 0; i < 9; ++i)
	{
		if (m_rcSquares[i].PtInRect(point))
			return i;
	}
	return -1;
}

int CMainWindow::IsSomeoneWin()
{
	for (int i = 0; i < 8; ++i)
	{
		if (m_nGameGrid[m_nLines[i][0]] == PERSON &&
			m_nGameGrid[m_nLines[i][1]] == PERSON &&
			m_nGameGrid[m_nLines[i][2]] == PERSON)
			return PERSON;

		if (m_nGameGrid[m_nLines[i][0]] == COMPUTER &&
			m_nGameGrid[m_nLines[i][1]] == COMPUTER &&
			m_nGameGrid[m_nLines[i][2]] == COMPUTER)
			return COMPUTER;
	}

	return 0;
}

BOOL CMainWindow::IsDraw()
{
	for (int i = 0; i < 9; ++i)
	{
		if (m_nGameGrid[i] == 0)
			return FALSE;
	}
	return TRUE;
}

void CMainWindow::CheckForGameOver()
{
	int iWinner;

	if (iWinner = IsSomeoneWin())
	{
		CString str((iWinner == PERSON) ? TEXT("X 胜！") : TEXT("O 胜！"));
		MessageBox(str, TEXT("游戏结束"), MB_ICONEXCLAMATION | MB_OK);
		m_nNextOne = 0;
	}
	else if (IsDraw())
	{
		m_nNextOne = 0;
		MessageBox(TEXT("平局！"), TEXT("游戏结束"), MB_ICONEXCLAMATION | MB_OK);
	}
	else if (m_nNextOne == COMPUTER)
		PostMessage(WM_COMDROP, 0, 0);
}

void CMainWindow::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	CClientDC dc(this);
	if (RGB(0, 0, 0) == dc.GetPixel(point))
		Replay();

	CWnd::OnLButtonDblClk(nFlags, point);
}

void CMainWindow::Replay()
{
	m_nNextOne = PERSON;
	::ZeroMemory(m_nGameGrid, 9 * sizeof(int));
	Invalidate();
}

int CMainWindow::CalcValue(int nPos)
{
	int result = 0, nValue[7] = { 2, 0, 1, 3, 5, 0, 8 };//改变难度在于这个位置价值

	for (int i = 0; i < 8; ++i){
		if (!InThisLine(nPos, i))
			continue;

		int temp = 0;
		for (int j = 0; j < 3; ++j){
			switch (m_nGameGrid[m_nLines[i][j]]){
			case 0:
				break;
			case PERSON:
				temp += 2;
				break;
			case COMPUTER:
				temp += 3;
				break;
			}
		}
		result += nValue[temp];
	}

	return result;
}


bool CMainWindow::InThisLine(int nPos, int nLine)
{
	for (int i = 0; i < 3; ++i){
		if (m_nLines[nLine][i] == nPos)
			return true;
	}
	return false;
}
