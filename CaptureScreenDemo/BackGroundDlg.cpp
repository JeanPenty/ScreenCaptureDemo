// BackGroundDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "CaptureScreenDemo.h"
#include "BackGroundDlg.h"


// CBackGroundDlg 对话框

IMPLEMENT_DYNAMIC(CBackGroundDlg, CDialog)

CBackGroundDlg::CBackGroundDlg(CWnd* pParent /*=NULL*/, CBitmap* pbmp /*= NULL*/)
	: CDialog(CBackGroundDlg::IDD, pParent)
	, m_pBitmap(pbmp)
{
	m_bDraw = false;
	m_bFirstDraw = true;
	m_bMoveAll = false;

	m_rcSelected = NULL;

	m_hCurSelect = LoadCursor(NULL, IDC_SIZEALL);
	m_hCurRightTop = LoadCursor(NULL, IDC_SIZENESW);
	m_hCurRightBottom = LoadCursor(NULL, IDC_SIZENWSE);
	m_hCurTop = LoadCursor(NULL, IDC_SIZENS);
	m_hCurBottom = LoadCursor(NULL, IDC_SIZENS);
	m_hCurLeft = LoadCursor(NULL, IDC_SIZEWE);
	m_hCurRight = LoadCursor(NULL, IDC_SIZEWE);
	m_hCurLeftTop = LoadCursor(NULL, IDC_SIZENWSE);
	m_hCurLeftBottom = LoadCursor(NULL, IDC_SIZENESW);

	m_bLeftTopMove = false;
	m_bTopMove = false;
	m_bRightTopMove = false;
	m_bRightMove = false;
	m_bRightBottomMove = false;
	m_bBottomMove = false;
	m_bLeftBottomMove = false;
	m_bLeftMove = false;

	m_pImgMask = NULL;
	m_pImgDot = NULL;
}

CBackGroundDlg::~CBackGroundDlg()
{
}

void CBackGroundDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CBackGroundDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()


// CBackGroundDlg 消息处理程序

BOOL CBackGroundDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化

	m_cx = GetSystemMetrics(SM_CXSCREEN);
	m_cy = GetSystemMetrics(SM_CYSCREEN);

	::SetWindowPos(this->GetSafeHwnd(), 
		HWND_TOPMOST, 0,0, m_cx, m_cy, SWP_NOCOPYBITS);

	m_pImgMask = Gdiplus::Image::FromFile(L"SC_MASK.png");
	m_pImgDot = Gdiplus::Image::FromFile(L"SC_DOT.png");

	//设置窗口透明
// 	SetWindowLong(this->GetSafeHwnd(),
// 		GWL_EXSTYLE,
// 		GetWindowLong(this->GetSafeHwnd(), GWL_EXSTYLE)^WS_EX_LAYERED);
// 	HINSTANCE hInst = LoadLibrary(L"User32.DLL"); //显式加载DLL
// 	if(hInst!=NULL) 
// 	{            
// 		typedef BOOL (WINAPI *MYFUNC)(HWND,COLORREF,BYTE,DWORD);          
// 		MYFUNC pFun = NULL;     
// 		pFun=(MYFUNC)GetProcAddress(hInst, "SetLayeredWindowAttributes");//取得SetLayeredWindowAttributes函数指针
// 		if(pFun!=NULL)
// 			pFun(this->GetSafeHwnd(),0,150,LWA_ALPHA);     
// 		FreeLibrary(hInst); 
// 	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
void CBackGroundDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialog::OnPaint()
	HDC hDC = GetDC()->GetSafeHdc();
	BITMAP bmp;
	m_pBitmap->GetBitmap(&bmp);

	CDC dcCompatible;
	dcCompatible.CreateCompatibleDC(GetDC());
	dcCompatible.SelectObject(m_pBitmap);

	CRect rect;
	GetClientRect(&rect);
	BitBlt(hDC,0,0,rect.Width(),rect.Height(),dcCompatible.GetSafeHdc(),0,0,SRCCOPY);


	Graphics graph(GetDC()->GetSafeHdc());

	RectF rcDrawRect;
	rcDrawRect.X = 0;
	rcDrawRect.Y = 0;
	rcDrawRect.Width = (REAL)rect.Width();
	rcDrawRect.Height = (REAL)rect.Height();

	graph.DrawImage(m_pImgMask, rcDrawRect, 0,0, 2, 2, UnitPixel);
}

void CBackGroundDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (PtInRect(m_rcSelected, point))
	{
		m_bMoveAll = true;
		SetCursor(m_hCurSelect);
	}

	if (PtInRect(CRect(m_rcSelected.left - 3, 
		m_rcSelected.top - 3, 
		m_rcSelected.left + 2, 
		m_rcSelected.top + 2), point))		//左上鼠标光标
	{
		SetCursor(m_hCurLeftTop);
		m_bLeftTopMove = true;
	}

	if (PtInRect(CRect(m_rcSelected.right - (m_rcSelected.right - m_rcSelected.left)/2,
		m_rcSelected.top - 3,
		m_rcSelected.right - (m_rcSelected.right - m_rcSelected.left)/2 + 5,
		m_rcSelected.top + 2), point))		//上边鼠标光标
	{
		SetCursor(m_hCurTop);
		m_bTopMove = true;
	}

	if (PtInRect(CRect(m_rcSelected.right - 2, 
		m_rcSelected.top - 3,
		m_rcSelected.right + 3,
		m_rcSelected.top + 2), point))		//右上鼠标光标
	{
		SetCursor(m_hCurRightTop);
		m_bRightTopMove = true;
	}

	if (PtInRect(CRect(m_rcSelected.right - 2, 
		m_rcSelected.bottom - (m_rcSelected.bottom - m_rcSelected.top)/2,
		m_rcSelected.right + 3, 
		m_rcSelected.bottom - (m_rcSelected.bottom - m_rcSelected.top)/2 + 5), point))		//右边鼠标光标
	{
		SetCursor(m_hCurRight);
		m_bRightMove = true;
	}

	if (PtInRect(CRect(m_rcSelected.right - 2, 
		m_rcSelected.bottom - 2,
		m_rcSelected.right + 3, 
		m_rcSelected.bottom + 3), point))		//右下鼠标光标
	{
		SetCursor(m_hCurRightBottom);
		m_bRightBottomMove = true;
	}

	if (PtInRect(CRect(m_rcSelected.right - (m_rcSelected.right - m_rcSelected.left)/2, 
		m_rcSelected.bottom - 2,
		m_rcSelected.right - (m_rcSelected.right - m_rcSelected.left)/2 + 5, 
		m_rcSelected.bottom + 3), point))		//下边鼠标光标
	{
		SetCursor(m_hCurBottom);
		m_bBottomMove = true;
	}

	if (PtInRect(CRect(m_rcSelected.left - 3, 
		m_rcSelected.bottom - 2,
		m_rcSelected.left + 2, 
		m_rcSelected.bottom + 3), point))		//左下鼠标光标
	{
		SetCursor(m_hCurLeftBottom);
		m_bLeftBottomMove = true;
	}

	if (PtInRect(CRect(m_rcSelected.left -3, 
		m_rcSelected.bottom - (m_rcSelected.bottom - m_rcSelected.top)/2,
		m_rcSelected.left + 2,
		m_rcSelected.bottom - (m_rcSelected.bottom - m_rcSelected.top)/2 + 5), point))		//左边鼠标光标
	{
		SetCursor(m_hCurLeft);
		m_bLeftMove = true;
	}

	m_ptDown = point;
	m_bDraw = true;
	CDialog::OnLButtonDown(nFlags, point);
}

void CBackGroundDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (m_ptDown != point)
	{
		m_bFirstDraw = false;

		if (m_bMoveAll)
		{
			SetCursor(m_hCurSelect);
			CRect rcArea;
			if (point.x >= m_ptDown.x)
			{
				rcArea.left = m_rcSelected.left + (point.x - m_ptDown.x);
				rcArea.right = m_rcSelected.right + (point.x - m_ptDown.x);

				if (rcArea.right >= GetSystemMetrics(SM_CXSCREEN))
				{
					rcArea.right = GetSystemMetrics(SM_CXSCREEN);
					rcArea.left = rcArea.right - m_rcSelected.Width();
				}
			}
			else if (point.x <= m_ptDown.x)
			{
				rcArea.left = m_rcSelected.left - (m_ptDown.x - point.x);
				rcArea.right = m_rcSelected.right - (m_ptDown.x - point.x);

				if (rcArea.left <= 0)
				{
					rcArea.left = 0;
					rcArea.right = rcArea.left + m_rcSelected.Width();
				}
			}

			if (point.y >= m_ptDown.y)
			{
				rcArea.top = m_rcSelected.top + (point.y - m_ptDown.y);
				rcArea.bottom = m_rcSelected.bottom + (point.y - m_ptDown.y);

				if (rcArea.bottom >= GetSystemMetrics(SM_CYSCREEN))
				{
					rcArea.bottom = GetSystemMetrics(SM_CYSCREEN);
					rcArea.top = rcArea.bottom - m_rcSelected.Height();
				}
			}
			else if (point.y <= m_ptDown.y)
			{
				rcArea.top = m_rcSelected.top - (m_ptDown.y - point.y);
				rcArea.bottom = m_rcSelected.bottom - (m_ptDown.y - point.y);
				if (rcArea.top <= 0)
				{
					rcArea.top = 0;
					rcArea.bottom = rcArea.top + m_rcSelected.Height();
				}
			}
			m_rcSelected = rcArea;
		}
	}

	if (m_bLeftTopMove)
	{
		m_rcSelected = m_rcRemote;
	}
	if (m_bTopMove)
	{
		m_rcSelected = m_rcRemote;
	}
	if (m_bRightTopMove)
	{
		m_rcSelected = m_rcRemote;
	}
	if (m_bRightMove)
	{
		m_rcSelected = m_rcRemote;
	}
	if (m_bRightBottomMove)
	{
		m_rcSelected = m_rcRemote;
	}
	if (m_bBottomMove)
	{
		m_rcSelected = m_rcRemote;
	}
	if (m_bLeftBottomMove)
	{
		m_rcSelected = m_rcRemote;
	}
	if (m_bLeftMove)
	{
		m_rcSelected = m_rcRemote;
	}

	if (PtInRect(CRect(m_rcSelected.left - 3, 
		m_rcSelected.top - 3, 
		m_rcSelected.left + 2, 
		m_rcSelected.top + 2), point))		//左上鼠标光标
	{
		SetCursor(m_hCurLeftTop);
	}

	if (PtInRect(CRect(m_rcSelected.right - (m_rcSelected.right - m_rcSelected.left)/2,
		m_rcSelected.top - 3,
		m_rcSelected.right - (m_rcSelected.right - m_rcSelected.left)/2 + 5,
		m_rcSelected.top + 2), point))		//上边鼠标光标
	{
		SetCursor(m_hCurTop);
	}

	if (PtInRect(CRect(m_rcSelected.right - 2, 
		m_rcSelected.top - 3,
		m_rcSelected.right + 3,
		m_rcSelected.top + 2), point))		//右上鼠标光标
	{
		SetCursor(m_hCurRightTop);
	}

	if (PtInRect(CRect(m_rcSelected.right - 2, 
		m_rcSelected.bottom - (m_rcSelected.bottom - m_rcSelected.top)/2,
		m_rcSelected.right + 3, 
		m_rcSelected.bottom - (m_rcSelected.bottom - m_rcSelected.top)/2 + 5), point))		//右边鼠标光标
	{
		SetCursor(m_hCurRight);
	}

	if (PtInRect(CRect(m_rcSelected.right - 2, 
		m_rcSelected.bottom - 2,
		m_rcSelected.right + 3, 
		m_rcSelected.bottom + 3), point))		//右下鼠标光标
	{
		SetCursor(m_hCurRightBottom);
	}

	if (PtInRect(CRect(m_rcSelected.right - (m_rcSelected.right - m_rcSelected.left)/2, 
		m_rcSelected.bottom - 2,
		m_rcSelected.right - (m_rcSelected.right - m_rcSelected.left)/2 + 5, 
		m_rcSelected.bottom + 3), point))		//下边鼠标光标
	{
		SetCursor(m_hCurBottom);
	}

	if (PtInRect(CRect(m_rcSelected.left - 3, 
		m_rcSelected.bottom - 2,
		m_rcSelected.left + 2, 
		m_rcSelected.bottom + 3), point))		//左下鼠标光标
	{
		SetCursor(m_hCurLeftBottom);
	}

	if (PtInRect(CRect(m_rcSelected.left -3, 
		m_rcSelected.bottom - (m_rcSelected.bottom - m_rcSelected.top)/2,
		m_rcSelected.left + 2,
		m_rcSelected.bottom - (m_rcSelected.bottom - m_rcSelected.top)/2 + 5), point))		//左边鼠标光标
	{
		SetCursor(m_hCurLeft);
	}

	m_bDraw = false;
	m_bMoveAll = false;
	m_bLeftTopMove = false;
	m_bTopMove = false;
	m_bRightTopMove = false;
	m_bRightMove = false;
	m_bRightBottomMove = false;
	m_bBottomMove = false;
	m_bLeftBottomMove = false;
	m_bLeftMove = false;

	CDialog::OnLButtonUp(nFlags, point);
}

BOOL CBackGroundDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if ( pMsg->message == WM_KEYDOWN )
	{
		if (pMsg->wParam == VK_RETURN)		
		{
			return TRUE;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}


void CBackGroundDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (GetSystemMetrics(SM_CXSCREEN) <= point.x || 
		GetSystemMetrics(SM_CYSCREEN) <= point.y)
	{
		return;
	}

	if (PtInRect(m_rcSelected, point))		//全部鼠标光标
	{
		SetCursor(m_hCurSelect);
	}

	if (PtInRect(CRect(m_rcSelected.left - 3, 
		m_rcSelected.top - 3, 
		m_rcSelected.left + 2, 
		m_rcSelected.top + 2), point))		//左上鼠标光标
	{
		SetCursor(m_hCurLeftTop);
	}

	if (PtInRect(CRect(m_rcSelected.right - (m_rcSelected.right - m_rcSelected.left)/2,
		m_rcSelected.top - 3,
		m_rcSelected.right - (m_rcSelected.right - m_rcSelected.left)/2 + 5,
		m_rcSelected.top + 2), point))		//上边鼠标光标
	{
		SetCursor(m_hCurTop);
	}

	if (PtInRect(CRect(m_rcSelected.right - 2, 
		m_rcSelected.top - 3,
		m_rcSelected.right + 3,
		m_rcSelected.top + 2), point))		//右上鼠标光标
	{
		SetCursor(m_hCurRightTop);
	}

	if (PtInRect(CRect(m_rcSelected.right - 2, 
		m_rcSelected.bottom - (m_rcSelected.bottom - m_rcSelected.top)/2,
		m_rcSelected.right + 3, 
		m_rcSelected.bottom - (m_rcSelected.bottom - m_rcSelected.top)/2 + 5), point))		//右边鼠标光标
	{
		SetCursor(m_hCurRight);
	}

	if (PtInRect(CRect(m_rcSelected.right - 2, 
		m_rcSelected.bottom - 2,
		m_rcSelected.right + 3, 
		m_rcSelected.bottom + 3), point))		//右下鼠标光标
	{
		SetCursor(m_hCurRightBottom);
	}

	if (PtInRect(CRect(m_rcSelected.right - (m_rcSelected.right - m_rcSelected.left)/2, 
		m_rcSelected.bottom - 2,
		m_rcSelected.right - (m_rcSelected.right - m_rcSelected.left)/2 + 5, 
		m_rcSelected.bottom + 3), point))		//下边鼠标光标
	{
		SetCursor(m_hCurBottom);
	}

	if (PtInRect(CRect(m_rcSelected.left - 3, 
		m_rcSelected.bottom - 2,
		m_rcSelected.left + 2, 
		m_rcSelected.bottom + 3), point))		//左下鼠标光标
	{
		SetCursor(m_hCurLeftBottom);
	}

	if (PtInRect(CRect(m_rcSelected.left -3, 
		m_rcSelected.bottom - (m_rcSelected.bottom - m_rcSelected.top)/2,
		m_rcSelected.left + 2,
		m_rcSelected.bottom - (m_rcSelected.bottom - m_rcSelected.top)/2 + 5), point))		//左边鼠标光标
	{
		SetCursor(m_hCurLeft);
	}

	CRect rect;
	GetClientRect(&rect);

	if (m_bFirstDraw)
	{
		if (m_bDraw)
		{
			HDC hDC = GetDC()->GetSafeHdc();
		 	HBITMAP hMemBitMap = 
		 		CreateCompatibleBitmap(hDC, rect.Width(), rect.Height());
			CDC dcCompatible;
			dcCompatible.CreateCompatibleDC(GetDC());
			dcCompatible.SelectObject(hMemBitMap);

			CRect rcArea;
			Graphics graph(dcCompatible.GetSafeHdc());
			int nXDown = m_ptDown.x;
			int nYDown = m_ptDown.y;
			int nXCur = point.x;
			int nYCur = point.y;
			if (nXDown <= nXCur && nYDown <= nYCur)
			{
				rcArea.left = m_ptDown.x;
				rcArea.top = m_ptDown.y;
				rcArea.right = point.x;
				rcArea.bottom = point.y;
			}
			else if (nXDown <= nXCur && nYDown >= nYCur)
			{
				rcArea.left = m_ptDown.x;
				rcArea.top = point.y;
				rcArea.right = point.x;
				rcArea.bottom = m_ptDown.y;
			}
			else if (nXCur <= nXDown && nYCur >= nYDown)
			{
				rcArea.left = point.x;
				rcArea.top = m_ptDown.y;
				rcArea.right = m_ptDown.x;
				rcArea.bottom = point.y;
			}
			else
			{
				rcArea.left = point.x;
				rcArea.top = point.y;
				rcArea.right = m_ptDown.x;
				rcArea.bottom = m_ptDown.y;
			}

			m_rcSelected = rcArea;

			/*
			@	画灰色方块，画选中区域之外的
			@	方法一：
			@	使用区域，整个为一个区域、选中为一个区域，计算出这两个区域之后再补集操作得到需要画灰色方块的区域
			@	向此区域中画灰色Mask
			@	方法二：
			@	将原矩形分割成上、下、左、右、四部分，分块画灰色Mask
			*/
// 			Image* image = 
// 				Gdiplus::Image::FromFile(L"SC_MASK.png");
			
			HBITMAP hBmp = (HBITMAP)m_pBitmap->GetSafeHandle();
			Bitmap bmp(hBmp, NULL);
			graph.DrawImage(&bmp, 0,0,rect.Width(), rect.Height());

			//画灰色的四个区域
			RectF rcLeft, rcRight, rcTop, rcBottom;

			{		//左边
				rcLeft.X = 0;
				rcLeft.Y = 0;
				rcLeft.Width = (REAL)m_rcSelected.left;
				rcLeft.Height = (REAL)rect.Height();

				graph.DrawImage(m_pImgMask, rcLeft, 0,0, 1, 1, UnitPixel);
			}

			{		//右边
				rcRight.X = (REAL)m_rcSelected.right;
				rcRight.Y = 0;
				rcRight.Width = (REAL)rect.right - m_rcSelected.right;
				rcRight.Height = (REAL)rect.Height();
				graph.DrawImage(m_pImgMask, rcRight, 0,0, 1, 1, UnitPixel);
			}

			{		//上边
				rcTop.Y = 0;
				rcTop.X = (REAL)m_rcSelected.left;
				rcTop.Width = (REAL)m_rcSelected.Width();
				rcTop.Height = (REAL)m_rcSelected.top;
				graph.DrawImage(m_pImgMask, rcTop, 0,0, 1, 1, UnitPixel);
			}

			{		//下边
				rcBottom.X = (REAL)m_rcSelected.left;
				rcBottom.Y = (REAL)m_rcSelected.bottom;
				rcBottom.Width = (REAL)m_rcSelected.Width();
				rcBottom.Height = (REAL)rect.bottom - m_rcSelected.bottom;
				graph.DrawImage(m_pImgMask, rcBottom, 0,0, 1, 1, UnitPixel);
			}
			
			//画8个拉伸的点
// 			Image* imageDot = 
// 				Gdiplus::Image::FromFile(L"SC_DOT.png");

			//top left
			graph.DrawImage(m_pImgDot, m_rcSelected.left - 3, m_rcSelected.top - 3, 5, 5);
			//bottom left
			graph.DrawImage(m_pImgDot, m_rcSelected.left - 3, m_rcSelected.bottom - 2, 5, 5);
			//top right
			graph.DrawImage(m_pImgDot, m_rcSelected.right - 2, m_rcSelected.top - 3, 5, 5);
			//bottom right
			graph.DrawImage(m_pImgDot, m_rcSelected.right - 2, m_rcSelected.bottom - 2, 5, 5);
			//left
			graph.DrawImage(m_pImgDot, m_rcSelected.left -3, m_rcSelected.bottom - (m_rcSelected.bottom - m_rcSelected.top)/2, 5, 5);
			//right
			graph.DrawImage(m_pImgDot, m_rcSelected.right - 2, m_rcSelected.bottom - (m_rcSelected.bottom - m_rcSelected.top)/2, 5, 5);
			//top 
			graph.DrawImage(m_pImgDot, m_rcSelected.right - (m_rcSelected.right - m_rcSelected.left)/2, m_rcSelected.top - 3, 5, 5);
			//bottom
			graph.DrawImage(m_pImgDot, m_rcSelected.right - (m_rcSelected.right - m_rcSelected.left)/2, m_rcSelected.bottom - 2, 5, 5);

			//画一个线边框
			Pen hPen(Color(255, 100, 100, 100), 1);
			graph.DrawLine(&hPen, m_rcSelected.left, m_rcSelected.top, m_rcSelected.right, m_rcSelected.top);
			graph.DrawLine(&hPen, m_rcSelected.right, m_rcSelected.top, m_rcSelected.right, m_rcSelected.bottom);
			graph.DrawLine(&hPen, m_rcSelected.left, m_rcSelected.bottom, m_rcSelected.right, m_rcSelected.bottom);
			graph.DrawLine(&hPen, m_rcSelected.left, m_rcSelected.top, m_rcSelected.left, m_rcSelected.bottom);

			RectF rcTitleSize;
			if (m_rcSelected.top > 20)
			{

				rcTitleSize.X = (REAL)m_rcSelected.left;
				rcTitleSize.Y = (REAL)m_rcSelected.top - 20;
				rcTitleSize.Width = 245;
				rcTitleSize.Height = 20;
			}
			else
			{
				rcTitleSize.X = (REAL)m_rcSelected.left;
				rcTitleSize.Y = (REAL)m_rcSelected.top;
				rcTitleSize.Width = 245;
				rcTitleSize.Height = 20;
			}
			graph.DrawImage(m_pImgMask, rcTitleSize, 0,0, 1, 1, UnitPixel);

			CString csTitle;
			csTitle.Format(L"起始位置:%d.%d 矩形大小 %d*%d", 
				m_rcSelected.left, m_rcSelected.top,
				m_rcSelected.right - m_rcSelected.left,
				m_rcSelected.bottom - m_rcSelected.top);

			FontFamily fontFamily(L"楷体");
			Gdiplus::Font font(&fontFamily, 14, FontStyleRegular, UnitPixel);
			StringFormat strFormat;
			strFormat.SetAlignment(StringAlignmentCenter);
			strFormat.SetLineAlignment(StringAlignmentCenter);
			graph.DrawString(csTitle, -1, 
				&font, 
				PointF(rcTitleSize.X, rcTitleSize.Y+2),
				&SolidBrush(Color(255, 255,255,255)));

			BitBlt(hDC,0,0,rect.Width(),rect.Height(),dcCompatible.GetSafeHdc(),0,0,SRCCOPY);
		}
	}
	else
	{
		if ( m_bMoveAll && m_bDraw)
		{
			SetCursor(m_hCurSelect);
			HDC hDC = GetDC()->GetSafeHdc();
		 	HBITMAP hMemBitMap = 
		 		CreateCompatibleBitmap(hDC, rect.Width(), rect.Height());
			CDC dcCompatible;
			dcCompatible.CreateCompatibleDC(GetDC());
			dcCompatible.SelectObject(hMemBitMap);
			Graphics graph(dcCompatible.GetSafeHdc());

			CRect rcArea;
			if (point.x >= m_ptDown.x)
			{
				rcArea.left = m_rcSelected.left + (point.x - m_ptDown.x);
				rcArea.right = m_rcSelected.right + (point.x - m_ptDown.x);

				if (rcArea.right >= GetSystemMetrics(SM_CXSCREEN))
				{
					rcArea.right = GetSystemMetrics(SM_CXSCREEN);
					rcArea.left = rcArea.right - m_rcSelected.Width();
				}
			}
			else if (point.x <= m_ptDown.x)
			{
				rcArea.left = m_rcSelected.left - (m_ptDown.x - point.x);
				rcArea.right = m_rcSelected.right - (m_ptDown.x - point.x);

				if (rcArea.left <= 0)
				{
					rcArea.left = 0;
					rcArea.right = rcArea.left + m_rcSelected.Width();
				}
			}

			if (point.y >= m_ptDown.y)
			{
				rcArea.top = m_rcSelected.top + (point.y - m_ptDown.y);
				rcArea.bottom = m_rcSelected.bottom + (point.y - m_ptDown.y);

				if (rcArea.bottom >= GetSystemMetrics(SM_CYSCREEN))
				{
					rcArea.bottom = GetSystemMetrics(SM_CYSCREEN);
					rcArea.top = rcArea.bottom - m_rcSelected.Height();
				}
			}
			else if (point.y <= m_ptDown.y)
			{
				rcArea.top = m_rcSelected.top - (m_ptDown.y - point.y);
				rcArea.bottom = m_rcSelected.bottom - (m_ptDown.y - point.y);
				if (rcArea.top <= 0)
				{
					rcArea.top = 0;
					rcArea.bottom = rcArea.top + m_rcSelected.Height();
				}
			}

// 			Image* image = 
// 				Gdiplus::Image::FromFile(L"SC_MASK.png");
			
			HBITMAP hBmp = (HBITMAP)m_pBitmap->GetSafeHandle();
			Bitmap bmp(hBmp, NULL);
			graph.DrawImage(&bmp, 0,0,rect.Width(), rect.Height());

			//画灰色的四个区域
			RectF rcLeft, rcRight, rcTop, rcBottom;

			{		//左边
				rcLeft.X = 0;
				rcLeft.Y = 0;
				rcLeft.Width = (REAL)rcArea.left;
				rcLeft.Height = (REAL)rect.Height();

				graph.DrawImage(m_pImgMask, rcLeft, 0,0, 1, 1, UnitPixel);
			}

			{		//右边
				rcRight.X = (REAL)rcArea.right;
				rcRight.Y = 0;
				rcRight.Width = (REAL)rect.right - rcArea.right;
				rcRight.Height = (REAL)rect.Height();
				graph.DrawImage(m_pImgMask, rcRight, 0,0, 1, 1, UnitPixel);
			}

			{		//上边
				rcTop.Y = 0;
				rcTop.X = (REAL)rcArea.left;
				rcTop.Width = (REAL)rcArea.Width();
				rcTop.Height = (REAL)rcArea.top;
				graph.DrawImage(m_pImgMask, rcTop, 0,0, 1, 1, UnitPixel);
			}

			{		//下边
				rcBottom.X = (REAL)rcArea.left;
				rcBottom.Y = (REAL)rcArea.bottom;
				rcBottom.Width = (REAL)rcArea.Width();
				rcBottom.Height = (REAL)rect.bottom - rcArea.bottom;
				graph.DrawImage(m_pImgMask, rcBottom, 0,0, 1, 1, UnitPixel);
			}
			
			//画8个拉伸的点
// 			Image* imageDot = 
// 				Gdiplus::Image::FromFile(L"SC_DOT.png");

			//top left
			graph.DrawImage(m_pImgDot, rcArea.left - 3, rcArea.top - 3, 5, 5);
			//bottom left
			graph.DrawImage(m_pImgDot, rcArea.left - 3, rcArea.bottom - 2, 5, 5);
			//top right
			graph.DrawImage(m_pImgDot, rcArea.right - 2, rcArea.top - 3, 5, 5);
			//bottom right
			graph.DrawImage(m_pImgDot, rcArea.right - 2, rcArea.bottom - 2, 5, 5);
			//left
			graph.DrawImage(m_pImgDot, rcArea.left -3, rcArea.bottom - (rcArea.bottom - rcArea.top)/2, 5, 5);
			//right
			graph.DrawImage(m_pImgDot, rcArea.right - 2, rcArea.bottom - (rcArea.bottom - rcArea.top)/2, 5, 5);
			//top 
			graph.DrawImage(m_pImgDot, rcArea.right - (rcArea.right - rcArea.left)/2, rcArea.top - 3, 5, 5);
			//bottom
			graph.DrawImage(m_pImgDot, rcArea.right - (rcArea.right - rcArea.left)/2, rcArea.bottom - 2, 5, 5);

			//画一个线边框
			Pen hPen(Color(255, 100, 100, 100), 1);
			graph.DrawLine(&hPen, rcArea.left, rcArea.top, rcArea.right, rcArea.top);
			graph.DrawLine(&hPen, rcArea.right, rcArea.top, rcArea.right, rcArea.bottom);
			graph.DrawLine(&hPen, rcArea.left, rcArea.bottom, rcArea.right, rcArea.bottom);
			graph.DrawLine(&hPen, rcArea.left, rcArea.top, rcArea.left, rcArea.bottom);

			RectF rcTitleSize;
			if (rcArea.top > 20)
			{

				rcTitleSize.X = (REAL)rcArea.left;
				rcTitleSize.Y = (REAL)rcArea.top - 20;
				rcTitleSize.Width = 245;
				rcTitleSize.Height = 20;
			}
			else
			{
				rcTitleSize.X = (REAL)rcArea.left;
				rcTitleSize.Y = (REAL)rcArea.top;
				rcTitleSize.Width = 245;
				rcTitleSize.Height = 20;
			}
			graph.DrawImage(m_pImgMask, rcTitleSize, 0,0, 1, 1, UnitPixel);

			CString csTitle;
			csTitle.Format(L"起始位置:%d.%d 矩形大小 %d*%d", 
				rcArea.left, rcArea.top,
				rcArea.right - rcArea.left,
				rcArea.bottom - rcArea.top);

			FontFamily fontFamily(L"楷体");
			Gdiplus::Font font(&fontFamily, 14, FontStyleRegular, UnitPixel);
			StringFormat strFormat;
			strFormat.SetAlignment(StringAlignmentCenter);
			strFormat.SetLineAlignment(StringAlignmentCenter);
			graph.DrawString(csTitle, -1, 
				&font, 
				PointF(rcTitleSize.X, rcTitleSize.Y+2),
				&SolidBrush(Color(255, 255,255,255)));

			BitBlt(hDC,0,0,rect.Width(),rect.Height(),dcCompatible.GetSafeHdc(),0,0,SRCCOPY);
		}
		else if (m_bLeftTopMove && m_bDraw)
		{
			HDC hDC = GetDC()->GetSafeHdc();
		 	HBITMAP hMemBitMap = 
		 		CreateCompatibleBitmap(hDC, rect.Width(), rect.Height());
			CDC dcCompatible;
			dcCompatible.CreateCompatibleDC(GetDC());
			dcCompatible.SelectObject(hMemBitMap);
			Graphics graph(dcCompatible.GetSafeHdc());

			CRect rcArea;
			if (point.x <= m_rcSelected.right)
			{
				rcArea.left = point.x;
				rcArea.right = m_rcSelected.right;
			}
			else if (point.x >= m_rcSelected.right)
			{
				rcArea.left = m_rcSelected.right;
				rcArea.right = point.x;
			}

			if (point.y >= m_rcSelected.bottom)
			{
				rcArea.top = m_rcSelected.bottom;
				rcArea.bottom = point.y;
			}
			else if (point.y <= m_rcSelected.bottom)
			{
				rcArea.top = point.y;
				rcArea.bottom = m_rcSelected.bottom;
			}

			m_rcRemote = rcArea;
// 			Image* image = 
// 				Gdiplus::Image::FromFile(L"SC_MASK.png");
			
			HBITMAP hBmp = (HBITMAP)m_pBitmap->GetSafeHandle();
			Bitmap bmp(hBmp, NULL);
			graph.DrawImage(&bmp, 0,0,rect.Width(), rect.Height());

			//画灰色的四个区域
			RectF rcLeft, rcRight, rcTop, rcBottom;

			{		//左边
				rcLeft.X = 0;
				rcLeft.Y = 0;
				rcLeft.Width = (REAL)rcArea.left;
				rcLeft.Height = (REAL)rect.Height();

				graph.DrawImage(m_pImgMask, rcLeft, 0,0, 1, 1, UnitPixel);
			}

			{		//右边
				rcRight.X = (REAL)rcArea.right;
				rcRight.Y = 0;
				rcRight.Width = (REAL)rect.right - rcArea.right;
				rcRight.Height = (REAL)rect.Height();
				graph.DrawImage(m_pImgMask, rcRight, 0,0, 1, 1, UnitPixel);
			}

			{		//上边
				rcTop.Y = 0;
				rcTop.X = (REAL)rcArea.left;
				rcTop.Width = (REAL)rcArea.Width();
				rcTop.Height = (REAL)rcArea.top;
				graph.DrawImage(m_pImgMask, rcTop, 0,0, 1, 1, UnitPixel);
			}

			{		//下边
				rcBottom.X = (REAL)rcArea.left;
				rcBottom.Y = (REAL)rcArea.bottom;
				rcBottom.Width = (REAL)rcArea.Width();
				rcBottom.Height = (REAL)rect.bottom - rcArea.bottom;
				graph.DrawImage(m_pImgMask, rcBottom, 0,0, 1, 1, UnitPixel);
			}
			
			//画8个拉伸的点
// 			Image* imageDot = 
// 				Gdiplus::Image::FromFile(L"SC_DOT.png");

			//top left
			graph.DrawImage(m_pImgDot, rcArea.left - 3, rcArea.top - 3, 5, 5);
			//bottom left
			graph.DrawImage(m_pImgDot, rcArea.left - 3, rcArea.bottom - 2, 5, 5);
			//top right
			graph.DrawImage(m_pImgDot, rcArea.right - 2, rcArea.top - 3, 5, 5);
			//bottom right
			graph.DrawImage(m_pImgDot, rcArea.right - 2, rcArea.bottom - 2, 5, 5);
			//left
			graph.DrawImage(m_pImgDot, rcArea.left -3, rcArea.bottom - (rcArea.bottom - rcArea.top)/2, 5, 5);
			//right
			graph.DrawImage(m_pImgDot, rcArea.right - 2, rcArea.bottom - (rcArea.bottom - rcArea.top)/2, 5, 5);
			//top 
			graph.DrawImage(m_pImgDot, rcArea.right - (rcArea.right - rcArea.left)/2, rcArea.top - 3, 5, 5);
			//bottom
			graph.DrawImage(m_pImgDot, rcArea.right - (rcArea.right - rcArea.left)/2, rcArea.bottom - 2, 5, 5);

			//画一个线边框
			Pen hPen(Color(255, 100, 100, 100), 1);
			graph.DrawLine(&hPen, rcArea.left, rcArea.top, rcArea.right, rcArea.top);
			graph.DrawLine(&hPen, rcArea.right, rcArea.top, rcArea.right, rcArea.bottom);
			graph.DrawLine(&hPen, rcArea.left, rcArea.bottom, rcArea.right, rcArea.bottom);
			graph.DrawLine(&hPen, rcArea.left, rcArea.top, rcArea.left, rcArea.bottom);

			RectF rcTitleSize;
			if (rcArea.top > 20)
			{

				rcTitleSize.X = (REAL)rcArea.left;
				rcTitleSize.Y = (REAL)rcArea.top - 20;
				rcTitleSize.Width = 245;
				rcTitleSize.Height = 20;
			}
			else
			{
				rcTitleSize.X = (REAL)rcArea.left;
				rcTitleSize.Y = (REAL)rcArea.top;
				rcTitleSize.Width = 245;
				rcTitleSize.Height = 20;
			}
			graph.DrawImage(m_pImgMask, rcTitleSize, 0,0, 1, 1, UnitPixel);

			CString csTitle;
			csTitle.Format(L"起始位置:%d.%d 矩形大小 %d*%d", 
				rcArea.left, rcArea.top,
				rcArea.right - rcArea.left,
				rcArea.bottom - rcArea.top);

			FontFamily fontFamily(L"楷体");
			Gdiplus::Font font(&fontFamily, 14, FontStyleRegular, UnitPixel);
			StringFormat strFormat;
			strFormat.SetAlignment(StringAlignmentCenter);
			strFormat.SetLineAlignment(StringAlignmentCenter);
			graph.DrawString(csTitle, -1, 
				&font, 
				PointF(rcTitleSize.X, rcTitleSize.Y+2),
				&SolidBrush(Color(255, 255,255,255)));

			BitBlt(hDC,0,0,rect.Width(),rect.Height(),dcCompatible.GetSafeHdc(),0,0,SRCCOPY);
		}
		else if (m_bTopMove && m_bDraw)
		{
			HDC hDC = GetDC()->GetSafeHdc();
			HBITMAP hMemBitMap = 
				CreateCompatibleBitmap(hDC, rect.Width(), rect.Height());
			CDC dcCompatible;
			dcCompatible.CreateCompatibleDC(GetDC());
			dcCompatible.SelectObject(hMemBitMap);
			Graphics graph(dcCompatible.GetSafeHdc());

			CRect rcArea;
			rcArea.left = m_rcSelected.left;
			rcArea.right = m_rcSelected.right;

			if (point.y >= m_rcSelected.bottom)
			{
				rcArea.top = m_rcSelected.bottom;
				rcArea.bottom = point.y;
			}
			else if (point.y <= m_rcSelected.bottom)
			{
				rcArea.top = point.y;
				rcArea.bottom = m_rcSelected.bottom;
			}

			m_rcRemote = rcArea;
// 			Image* image = 
// 				Gdiplus::Image::FromFile(L"SC_MASK.png");

			HBITMAP hBmp = (HBITMAP)m_pBitmap->GetSafeHandle();
			Bitmap bmp(hBmp, NULL);
			graph.DrawImage(&bmp, 0,0,rect.Width(), rect.Height());

			//画灰色的四个区域
			RectF rcLeft, rcRight, rcTop, rcBottom;

			{		//左边
				rcLeft.X = 0;
				rcLeft.Y = 0;
				rcLeft.Width = (REAL)rcArea.left;
				rcLeft.Height = (REAL)rect.Height();

				graph.DrawImage(m_pImgMask, rcLeft, 0,0, 1, 1, UnitPixel);
			}

			{		//右边
				rcRight.X = (REAL)rcArea.right;
				rcRight.Y = 0;
				rcRight.Width = (REAL)rect.right - rcArea.right;
				rcRight.Height = (REAL)rect.Height();
				graph.DrawImage(m_pImgMask, rcRight, 0,0, 1, 1, UnitPixel);
			}

			{		//上边
				rcTop.Y = 0;
				rcTop.X = (REAL)rcArea.left;
				rcTop.Width = (REAL)rcArea.Width();
				rcTop.Height = (REAL)rcArea.top;
				graph.DrawImage(m_pImgMask, rcTop, 0,0, 1, 1, UnitPixel);
			}

			{		//下边
				rcBottom.X = (REAL)rcArea.left;
				rcBottom.Y = (REAL)rcArea.bottom;
				rcBottom.Width = (REAL)rcArea.Width();
				rcBottom.Height = (REAL)rect.bottom - rcArea.bottom;
				graph.DrawImage(m_pImgMask, rcBottom, 0,0, 1, 1, UnitPixel);
			}

			//画8个拉伸的点
// 			Image* imageDot = 
// 				Gdiplus::Image::FromFile(L"SC_DOT.png");

			//top left
			graph.DrawImage(m_pImgDot, rcArea.left - 3, rcArea.top - 3, 5, 5);
			//bottom left
			graph.DrawImage(m_pImgDot, rcArea.left - 3, rcArea.bottom - 2, 5, 5);
			//top right
			graph.DrawImage(m_pImgDot, rcArea.right - 2, rcArea.top - 3, 5, 5);
			//bottom right
			graph.DrawImage(m_pImgDot, rcArea.right - 2, rcArea.bottom - 2, 5, 5);
			//left
			graph.DrawImage(m_pImgDot, rcArea.left -3, rcArea.bottom - (rcArea.bottom - rcArea.top)/2, 5, 5);
			//right
			graph.DrawImage(m_pImgDot, rcArea.right - 2, rcArea.bottom - (rcArea.bottom - rcArea.top)/2, 5, 5);
			//top 
			graph.DrawImage(m_pImgDot, rcArea.right - (rcArea.right - rcArea.left)/2, rcArea.top - 3, 5, 5);
			//bottom
			graph.DrawImage(m_pImgDot, rcArea.right - (rcArea.right - rcArea.left)/2, rcArea.bottom - 2, 5, 5);

			//画一个线边框
			Pen hPen(Color(255, 100, 100, 100), 1);
			graph.DrawLine(&hPen, rcArea.left, rcArea.top, rcArea.right, rcArea.top);
			graph.DrawLine(&hPen, rcArea.right, rcArea.top, rcArea.right, rcArea.bottom);
			graph.DrawLine(&hPen, rcArea.left, rcArea.bottom, rcArea.right, rcArea.bottom);
			graph.DrawLine(&hPen, rcArea.left, rcArea.top, rcArea.left, rcArea.bottom);

			RectF rcTitleSize;
			if (rcArea.top > 20)
			{

				rcTitleSize.X = (REAL)rcArea.left;
				rcTitleSize.Y = (REAL)rcArea.top - 20;
				rcTitleSize.Width = 245;
				rcTitleSize.Height = 20;
			}
			else
			{
				rcTitleSize.X = (REAL)rcArea.left;
				rcTitleSize.Y = (REAL)rcArea.top;
				rcTitleSize.Width = 245;
				rcTitleSize.Height = 20;
			}
			graph.DrawImage(m_pImgMask, rcTitleSize, 0,0, 1, 1, UnitPixel);

			CString csTitle;
			csTitle.Format(L"起始位置:%d.%d 矩形大小 %d*%d", 
				rcArea.left, rcArea.top,
				rcArea.right - rcArea.left,
				rcArea.bottom - rcArea.top);

			FontFamily fontFamily(L"楷体");
			Gdiplus::Font font(&fontFamily, 14, FontStyleRegular, UnitPixel);
			StringFormat strFormat;
			strFormat.SetAlignment(StringAlignmentCenter);
			strFormat.SetLineAlignment(StringAlignmentCenter);
			graph.DrawString(csTitle, -1, 
				&font, 
				PointF(rcTitleSize.X, rcTitleSize.Y+2),
				&SolidBrush(Color(255, 255,255,255)));

			BitBlt(hDC,0,0,rect.Width(),rect.Height(),dcCompatible.GetSafeHdc(),0,0,SRCCOPY);
		}
		else if (m_bRightTopMove && m_bDraw)
		{
			HDC hDC = GetDC()->GetSafeHdc();
			HBITMAP hMemBitMap = 
				CreateCompatibleBitmap(hDC, rect.Width(), rect.Height());
			CDC dcCompatible;
			dcCompatible.CreateCompatibleDC(GetDC());
			dcCompatible.SelectObject(hMemBitMap);
			Graphics graph(dcCompatible.GetSafeHdc());

			CRect rcArea;
			if (point.x >= m_rcSelected.left)
			{
				rcArea.left = m_rcSelected.left;
				rcArea.right = point.x;
			}
			if (point.x <= m_rcSelected.left)
			{
				rcArea.left = point.x;
				rcArea.right = m_rcSelected.left;
			}
			if (point.y >= m_rcSelected.bottom)
			{
				rcArea.top = m_rcSelected.bottom;
				rcArea.bottom = point.y;
			}
			else if (point.y <= m_rcSelected.bottom)
			{
				rcArea.top = point.y;
				rcArea.bottom = m_rcSelected.bottom;
			}

			m_rcRemote = rcArea;
// 			Image* m_pImgMask = 
// 				Gdiplus::Image::FromFile(L"SC_MASK.png");

			HBITMAP hBmp = (HBITMAP)m_pBitmap->GetSafeHandle();
			Bitmap bmp(hBmp, NULL);
			graph.DrawImage(&bmp, 0,0,rect.Width(), rect.Height());

			//画灰色的四个区域
			RectF rcLeft, rcRight, rcTop, rcBottom;

			{		//左边
				rcLeft.X = 0;
				rcLeft.Y = 0;
				rcLeft.Width = (REAL)rcArea.left;
				rcLeft.Height = (REAL)rect.Height();

				graph.DrawImage(m_pImgMask, rcLeft, 0,0, 1, 1, UnitPixel);
			}

			{		//右边
				rcRight.X = (REAL)rcArea.right;
				rcRight.Y = 0;
				rcRight.Width = (REAL)rect.right - rcArea.right;
				rcRight.Height = (REAL)rect.Height();
				graph.DrawImage(m_pImgMask, rcRight, 0,0, 1, 1, UnitPixel);
			}

			{		//上边
				rcTop.Y = 0;
				rcTop.X = (REAL)rcArea.left;
				rcTop.Width = (REAL)rcArea.Width();
				rcTop.Height = (REAL)rcArea.top;
				graph.DrawImage(m_pImgMask, rcTop, 0,0, 1, 1, UnitPixel);
			}

			{		//下边
				rcBottom.X = (REAL)rcArea.left;
				rcBottom.Y = (REAL)rcArea.bottom;
				rcBottom.Width = (REAL)rcArea.Width();
				rcBottom.Height = (REAL)rect.bottom - rcArea.bottom;
				graph.DrawImage(m_pImgMask, rcBottom, 0,0, 1, 1, UnitPixel);
			}

			//画8个拉伸的点
// 			Image* imageDot = 
// 				Gdiplus::Image::FromFile(L"SC_DOT.png");

			//top left
			graph.DrawImage(m_pImgDot, rcArea.left - 3, rcArea.top - 3, 5, 5);
			//bottom left
			graph.DrawImage(m_pImgDot, rcArea.left - 3, rcArea.bottom - 2, 5, 5);
			//top right
			graph.DrawImage(m_pImgDot, rcArea.right - 2, rcArea.top - 3, 5, 5);
			//bottom right
			graph.DrawImage(m_pImgDot, rcArea.right - 2, rcArea.bottom - 2, 5, 5);
			//left
			graph.DrawImage(m_pImgDot, rcArea.left -3, rcArea.bottom - (rcArea.bottom - rcArea.top)/2, 5, 5);
			//right
			graph.DrawImage(m_pImgDot, rcArea.right - 2, rcArea.bottom - (rcArea.bottom - rcArea.top)/2, 5, 5);
			//top 
			graph.DrawImage(m_pImgDot, rcArea.right - (rcArea.right - rcArea.left)/2, rcArea.top - 3, 5, 5);
			//bottom
			graph.DrawImage(m_pImgDot, rcArea.right - (rcArea.right - rcArea.left)/2, rcArea.bottom - 2, 5, 5);

			//画一个线边框
			Pen hPen(Color(255, 100, 100, 100), 1);
			graph.DrawLine(&hPen, rcArea.left, rcArea.top, rcArea.right, rcArea.top);
			graph.DrawLine(&hPen, rcArea.right, rcArea.top, rcArea.right, rcArea.bottom);
			graph.DrawLine(&hPen, rcArea.left, rcArea.bottom, rcArea.right, rcArea.bottom);
			graph.DrawLine(&hPen, rcArea.left, rcArea.top, rcArea.left, rcArea.bottom);

			RectF rcTitleSize;
			if (rcArea.top > 20)
			{

				rcTitleSize.X = (REAL)rcArea.left;
				rcTitleSize.Y = (REAL)rcArea.top - 20;
				rcTitleSize.Width = 245;
				rcTitleSize.Height = 20;
			}
			else
			{
				rcTitleSize.X = (REAL)rcArea.left;
				rcTitleSize.Y = (REAL)rcArea.top;
				rcTitleSize.Width = 245;
				rcTitleSize.Height = 20;
			}
			graph.DrawImage(m_pImgMask, rcTitleSize, 0,0, 1, 1, UnitPixel);

			CString csTitle;
			csTitle.Format(L"起始位置:%d.%d 矩形大小 %d*%d", 
				rcArea.left, rcArea.top,
				rcArea.right - rcArea.left,
				rcArea.bottom - rcArea.top);

			FontFamily fontFamily(L"楷体");
			Gdiplus::Font font(&fontFamily, 14, FontStyleRegular, UnitPixel);
			StringFormat strFormat;
			strFormat.SetAlignment(StringAlignmentCenter);
			strFormat.SetLineAlignment(StringAlignmentCenter);
			graph.DrawString(csTitle, -1, 
				&font, 
				PointF(rcTitleSize.X, rcTitleSize.Y+2),
				&SolidBrush(Color(255, 255,255,255)));

			BitBlt(hDC,0,0,rect.Width(),rect.Height(),dcCompatible.GetSafeHdc(),0,0,SRCCOPY);
		}
		else if (m_bRightMove && m_bDraw)
		{
			HDC hDC = GetDC()->GetSafeHdc();
			HBITMAP hMemBitMap = 
				CreateCompatibleBitmap(hDC, rect.Width(), rect.Height());
			CDC dcCompatible;
			dcCompatible.CreateCompatibleDC(GetDC());
			dcCompatible.SelectObject(hMemBitMap);
			Graphics graph(dcCompatible.GetSafeHdc());

			CRect rcArea;
			rcArea.top = m_rcSelected.top;
			rcArea.bottom = m_rcSelected.bottom;

			if (point.x >= m_rcSelected.left)
			{
				rcArea.left = m_rcSelected.left;
				rcArea.right = point.x;
			}
			else if (point.x <= m_rcSelected.left)
			{
				rcArea.left = point.x;
				rcArea.right = m_rcSelected.left;
			}

			m_rcRemote = rcArea;
// 			Image* image = 
// 				Gdiplus::Image::FromFile(L"SC_MASK.png");

			HBITMAP hBmp = (HBITMAP)m_pBitmap->GetSafeHandle();
			Bitmap bmp(hBmp, NULL);
			graph.DrawImage(&bmp, 0,0,rect.Width(), rect.Height());

			//画灰色的四个区域
			RectF rcLeft, rcRight, rcTop, rcBottom;

			{		//左边
				rcLeft.X = 0;
				rcLeft.Y = 0;
				rcLeft.Width = (REAL)rcArea.left;
				rcLeft.Height = (REAL)rect.Height();

				graph.DrawImage(m_pImgMask, rcLeft, 0,0, 1, 1, UnitPixel);
			}

			{		//右边
				rcRight.X = (REAL)rcArea.right;
				rcRight.Y = 0;
				rcRight.Width = (REAL)rect.right - rcArea.right;
				rcRight.Height = (REAL)rect.Height();
				graph.DrawImage(m_pImgMask, rcRight, 0,0, 1, 1, UnitPixel);
			}

			{		//上边
				rcTop.Y = 0;
				rcTop.X = (REAL)rcArea.left;
				rcTop.Width = (REAL)rcArea.Width();
				rcTop.Height = (REAL)rcArea.top;
				graph.DrawImage(m_pImgMask, rcTop, 0,0, 1, 1, UnitPixel);
			}

			{		//下边
				rcBottom.X = (REAL)rcArea.left;
				rcBottom.Y = (REAL)rcArea.bottom;
				rcBottom.Width = (REAL)rcArea.Width();
				rcBottom.Height = (REAL)rect.bottom - rcArea.bottom;
				graph.DrawImage(m_pImgMask, rcBottom, 0,0, 1, 1, UnitPixel);
			}

			//画8个拉伸的点
// 			Image* imageDot = 
// 				Gdiplus::Image::FromFile(L"SC_DOT.png");

			//top left
			graph.DrawImage(m_pImgDot, rcArea.left - 3, rcArea.top - 3, 5, 5);
			//bottom left
			graph.DrawImage(m_pImgDot, rcArea.left - 3, rcArea.bottom - 2, 5, 5);
			//top right
			graph.DrawImage(m_pImgDot, rcArea.right - 2, rcArea.top - 3, 5, 5);
			//bottom right
			graph.DrawImage(m_pImgDot, rcArea.right - 2, rcArea.bottom - 2, 5, 5);
			//left
			graph.DrawImage(m_pImgDot, rcArea.left -3, rcArea.bottom - (rcArea.bottom - rcArea.top)/2, 5, 5);
			//right
			graph.DrawImage(m_pImgDot, rcArea.right - 2, rcArea.bottom - (rcArea.bottom - rcArea.top)/2, 5, 5);
			//top 
			graph.DrawImage(m_pImgDot, rcArea.right - (rcArea.right - rcArea.left)/2, rcArea.top - 3, 5, 5);
			//bottom
			graph.DrawImage(m_pImgDot, rcArea.right - (rcArea.right - rcArea.left)/2, rcArea.bottom - 2, 5, 5);

			//画一个线边框
			Pen hPen(Color(255, 100, 100, 100), 1);
			graph.DrawLine(&hPen, rcArea.left, rcArea.top, rcArea.right, rcArea.top);
			graph.DrawLine(&hPen, rcArea.right, rcArea.top, rcArea.right, rcArea.bottom);
			graph.DrawLine(&hPen, rcArea.left, rcArea.bottom, rcArea.right, rcArea.bottom);
			graph.DrawLine(&hPen, rcArea.left, rcArea.top, rcArea.left, rcArea.bottom);

			RectF rcTitleSize;
			if (rcArea.top > 20)
			{

				rcTitleSize.X = (REAL)rcArea.left;
				rcTitleSize.Y = (REAL)rcArea.top - 20;
				rcTitleSize.Width = 245;
				rcTitleSize.Height = 20;
			}
			else
			{
				rcTitleSize.X = (REAL)rcArea.left;
				rcTitleSize.Y = (REAL)rcArea.top;
				rcTitleSize.Width = 245;
				rcTitleSize.Height = 20;
			}
			graph.DrawImage(m_pImgMask, rcTitleSize, 0,0, 1, 1, UnitPixel);

			CString csTitle;
			csTitle.Format(L"起始位置:%d.%d 矩形大小 %d*%d", 
				rcArea.left, rcArea.top,
				rcArea.right - rcArea.left,
				rcArea.bottom - rcArea.top);

			FontFamily fontFamily(L"楷体");
			Gdiplus::Font font(&fontFamily, 14, FontStyleRegular, UnitPixel);
			StringFormat strFormat;
			strFormat.SetAlignment(StringAlignmentCenter);
			strFormat.SetLineAlignment(StringAlignmentCenter);
			graph.DrawString(csTitle, -1, 
				&font, 
				PointF(rcTitleSize.X, rcTitleSize.Y+2),
				&SolidBrush(Color(255, 255,255,255)));

			BitBlt(hDC,0,0,rect.Width(),rect.Height(),dcCompatible.GetSafeHdc(),0,0,SRCCOPY);
		}
		else if (m_bRightBottomMove && m_bDraw)
		{
			HDC hDC = GetDC()->GetSafeHdc();
			HBITMAP hMemBitMap = 
				CreateCompatibleBitmap(hDC, rect.Width(), rect.Height());
			CDC dcCompatible;
			dcCompatible.CreateCompatibleDC(GetDC());
			dcCompatible.SelectObject(hMemBitMap);
			Graphics graph(dcCompatible.GetSafeHdc());

			CRect rcArea;
			if (point.x >= m_rcSelected.left)
			{
				rcArea.left = m_rcSelected.left;
				rcArea.right = point.x;
			}
			if (point.x <= m_rcSelected.left)
			{
				rcArea.left = point.x;
				rcArea.right = m_rcSelected.left;
			}
			if (point.y >= m_rcSelected.top)
			{
				rcArea.top = m_rcSelected.top;
				rcArea.bottom = point.y;
			}
			else if (point.y <= m_rcSelected.top)
			{
				rcArea.top = point.y;
				rcArea.bottom = m_rcSelected.top;
			}

			m_rcRemote = rcArea;
// 			Image* image = 
// 				Gdiplus::Image::FromFile(L"SC_MASK.png");

			HBITMAP hBmp = (HBITMAP)m_pBitmap->GetSafeHandle();
			Bitmap bmp(hBmp, NULL);
			graph.DrawImage(&bmp, 0,0,rect.Width(), rect.Height());

			//画灰色的四个区域
			RectF rcLeft, rcRight, rcTop, rcBottom;

			{		//左边
				rcLeft.X = 0;
				rcLeft.Y = 0;
				rcLeft.Width = (REAL)rcArea.left;
				rcLeft.Height = (REAL)rect.Height();

				graph.DrawImage(m_pImgMask, rcLeft, 0,0, 1, 1, UnitPixel);
			}

			{		//右边
				rcRight.X = (REAL)rcArea.right;
				rcRight.Y = 0;
				rcRight.Width = (REAL)rect.right - rcArea.right;
				rcRight.Height = (REAL)rect.Height();
				graph.DrawImage(m_pImgMask, rcRight, 0,0, 1, 1, UnitPixel);
			}

			{		//上边
				rcTop.Y = 0;
				rcTop.X = (REAL)rcArea.left;
				rcTop.Width = (REAL)rcArea.Width();
				rcTop.Height = (REAL)rcArea.top;
				graph.DrawImage(m_pImgMask, rcTop, 0,0, 1, 1, UnitPixel);
			}

			{		//下边
				rcBottom.X = (REAL)rcArea.left;
				rcBottom.Y = (REAL)rcArea.bottom;
				rcBottom.Width = (REAL)rcArea.Width();
				rcBottom.Height = (REAL)rect.bottom - rcArea.bottom;
				graph.DrawImage(m_pImgMask, rcBottom, 0,0, 1, 1, UnitPixel);
			}

			//画8个拉伸的点
// 			Image* imageDot = 
// 				Gdiplus::Image::FromFile(L"SC_DOT.png");

			//top left
			graph.DrawImage(m_pImgDot, rcArea.left - 3, rcArea.top - 3, 5, 5);
			//bottom left
			graph.DrawImage(m_pImgDot, rcArea.left - 3, rcArea.bottom - 2, 5, 5);
			//top right
			graph.DrawImage(m_pImgDot, rcArea.right - 2, rcArea.top - 3, 5, 5);
			//bottom right
			graph.DrawImage(m_pImgDot, rcArea.right - 2, rcArea.bottom - 2, 5, 5);
			//left
			graph.DrawImage(m_pImgDot, rcArea.left -3, rcArea.bottom - (rcArea.bottom - rcArea.top)/2, 5, 5);
			//right
			graph.DrawImage(m_pImgDot, rcArea.right - 2, rcArea.bottom - (rcArea.bottom - rcArea.top)/2, 5, 5);
			//top 
			graph.DrawImage(m_pImgDot, rcArea.right - (rcArea.right - rcArea.left)/2, rcArea.top - 3, 5, 5);
			//bottom
			graph.DrawImage(m_pImgDot, rcArea.right - (rcArea.right - rcArea.left)/2, rcArea.bottom - 2, 5, 5);

			//画一个线边框
			Pen hPen(Color(255, 100, 100, 100), 1);
			graph.DrawLine(&hPen, rcArea.left, rcArea.top, rcArea.right, rcArea.top);
			graph.DrawLine(&hPen, rcArea.right, rcArea.top, rcArea.right, rcArea.bottom);
			graph.DrawLine(&hPen, rcArea.left, rcArea.bottom, rcArea.right, rcArea.bottom);
			graph.DrawLine(&hPen, rcArea.left, rcArea.top, rcArea.left, rcArea.bottom);

			RectF rcTitleSize;
			if (rcArea.top > 20)
			{

				rcTitleSize.X = (REAL)rcArea.left;
				rcTitleSize.Y = (REAL)rcArea.top - 20;
				rcTitleSize.Width = 245;
				rcTitleSize.Height = 20;
			}
			else
			{
				rcTitleSize.X = (REAL)rcArea.left;
				rcTitleSize.Y = (REAL)rcArea.top;
				rcTitleSize.Width = 245;
				rcTitleSize.Height = 20;
			}
			graph.DrawImage(m_pImgMask, rcTitleSize, 0,0, 1, 1, UnitPixel);

			CString csTitle;
			csTitle.Format(L"起始位置:%d.%d 矩形大小 %d*%d", 
				rcArea.left, rcArea.top,
				rcArea.right - rcArea.left,
				rcArea.bottom - rcArea.top);

			FontFamily fontFamily(L"楷体");
			Gdiplus::Font font(&fontFamily, 14, FontStyleRegular, UnitPixel);
			StringFormat strFormat;
			strFormat.SetAlignment(StringAlignmentCenter);
			strFormat.SetLineAlignment(StringAlignmentCenter);
			graph.DrawString(csTitle, -1, 
				&font, 
				PointF(rcTitleSize.X, rcTitleSize.Y+2),
				&SolidBrush(Color(255, 255,255,255)));

			BitBlt(hDC,0,0,rect.Width(),rect.Height(),dcCompatible.GetSafeHdc(),0,0,SRCCOPY);
		}
		else if (m_bBottomMove && m_bDraw)
		{
			HDC hDC = GetDC()->GetSafeHdc();
			HBITMAP hMemBitMap = 
				CreateCompatibleBitmap(hDC, rect.Width(), rect.Height());
			CDC dcCompatible;
			dcCompatible.CreateCompatibleDC(GetDC());
			dcCompatible.SelectObject(hMemBitMap);
			Graphics graph(dcCompatible.GetSafeHdc());

			CRect rcArea;
			rcArea.left = m_rcSelected.left;
			rcArea.right = m_rcSelected.right;

			if (point.y >= m_rcSelected.top)
			{
				rcArea.top = m_rcSelected.top;
				rcArea.bottom = point.y;
			}
			else if (point.y <= m_rcSelected.top)
			{
				rcArea.top = point.y;
				rcArea.bottom = m_rcSelected.top;
			}

			m_rcRemote = rcArea;
// 			Image* image = 
// 				Gdiplus::Image::FromFile(L"SC_MASK.png");

			HBITMAP hBmp = (HBITMAP)m_pBitmap->GetSafeHandle();
			Bitmap bmp(hBmp, NULL);
			graph.DrawImage(&bmp, 0,0,rect.Width(), rect.Height());

			//画灰色的四个区域
			RectF rcLeft, rcRight, rcTop, rcBottom;

			{		//左边
				rcLeft.X = 0;
				rcLeft.Y = 0;
				rcLeft.Width = (REAL)rcArea.left;
				rcLeft.Height = (REAL)rect.Height();

				graph.DrawImage(m_pImgMask, rcLeft, 0,0, 1, 1, UnitPixel);
			}

			{		//右边
				rcRight.X = (REAL)rcArea.right;
				rcRight.Y = 0;
				rcRight.Width = (REAL)rect.right - rcArea.right;
				rcRight.Height = (REAL)rect.Height();
				graph.DrawImage(m_pImgMask, rcRight, 0,0, 1, 1, UnitPixel);
			}

			{		//上边
				rcTop.Y = 0;
				rcTop.X = (REAL)rcArea.left;
				rcTop.Width = (REAL)rcArea.Width();
				rcTop.Height = (REAL)rcArea.top;
				graph.DrawImage(m_pImgMask, rcTop, 0,0, 1, 1, UnitPixel);
			}

			{		//下边
				rcBottom.X = (REAL)rcArea.left;
				rcBottom.Y = (REAL)rcArea.bottom;
				rcBottom.Width = (REAL)rcArea.Width();
				rcBottom.Height = (REAL)rect.bottom - rcArea.bottom;
				graph.DrawImage(m_pImgMask, rcBottom, 0,0, 1, 1, UnitPixel);
			}

			//画8个拉伸的点
// 			Image* imageDot = 
// 				Gdiplus::Image::FromFile(L"SC_DOT.png");

			//top left
			graph.DrawImage(m_pImgDot, rcArea.left - 3, rcArea.top - 3, 5, 5);
			//bottom left
			graph.DrawImage(m_pImgDot, rcArea.left - 3, rcArea.bottom - 2, 5, 5);
			//top right
			graph.DrawImage(m_pImgDot, rcArea.right - 2, rcArea.top - 3, 5, 5);
			//bottom right
			graph.DrawImage(m_pImgDot, rcArea.right - 2, rcArea.bottom - 2, 5, 5);
			//left
			graph.DrawImage(m_pImgDot, rcArea.left -3, rcArea.bottom - (rcArea.bottom - rcArea.top)/2, 5, 5);
			//right
			graph.DrawImage(m_pImgDot, rcArea.right - 2, rcArea.bottom - (rcArea.bottom - rcArea.top)/2, 5, 5);
			//top 
			graph.DrawImage(m_pImgDot, rcArea.right - (rcArea.right - rcArea.left)/2, rcArea.top - 3, 5, 5);
			//bottom
			graph.DrawImage(m_pImgDot, rcArea.right - (rcArea.right - rcArea.left)/2, rcArea.bottom - 2, 5, 5);

			//画一个线边框
			Pen hPen(Color(255, 100, 100, 100), 1);
			graph.DrawLine(&hPen, rcArea.left, rcArea.top, rcArea.right, rcArea.top);
			graph.DrawLine(&hPen, rcArea.right, rcArea.top, rcArea.right, rcArea.bottom);
			graph.DrawLine(&hPen, rcArea.left, rcArea.bottom, rcArea.right, rcArea.bottom);
			graph.DrawLine(&hPen, rcArea.left, rcArea.top, rcArea.left, rcArea.bottom);

			RectF rcTitleSize;
			if (rcArea.top > 20)
			{

				rcTitleSize.X = (REAL)rcArea.left;
				rcTitleSize.Y = (REAL)rcArea.top - 20;
				rcTitleSize.Width = 245;
				rcTitleSize.Height = 20;
			}
			else
			{
				rcTitleSize.X = (REAL)rcArea.left;
				rcTitleSize.Y = (REAL)rcArea.top;
				rcTitleSize.Width = 245;
				rcTitleSize.Height = 20;
			}
			graph.DrawImage(m_pImgMask, rcTitleSize, 0,0, 1, 1, UnitPixel);

			CString csTitle;
			csTitle.Format(L"起始位置:%d.%d 矩形大小 %d*%d", 
				rcArea.left, rcArea.top,
				rcArea.right - rcArea.left,
				rcArea.bottom - rcArea.top);

			FontFamily fontFamily(L"楷体");
			Gdiplus::Font font(&fontFamily, 14, FontStyleRegular, UnitPixel);
			StringFormat strFormat;
			strFormat.SetAlignment(StringAlignmentCenter);
			strFormat.SetLineAlignment(StringAlignmentCenter);
			graph.DrawString(csTitle, -1, 
				&font, 
				PointF(rcTitleSize.X, rcTitleSize.Y+2),
				&SolidBrush(Color(255, 255,255,255)));

			BitBlt(hDC,0,0,rect.Width(),rect.Height(),dcCompatible.GetSafeHdc(),0,0,SRCCOPY);
		}
		else if (m_bLeftBottomMove && m_bDraw)
		{
			HDC hDC = GetDC()->GetSafeHdc();
			HBITMAP hMemBitMap = 
				CreateCompatibleBitmap(hDC, rect.Width(), rect.Height());
			CDC dcCompatible;
			dcCompatible.CreateCompatibleDC(GetDC());
			dcCompatible.SelectObject(hMemBitMap);
			Graphics graph(dcCompatible.GetSafeHdc());

			CRect rcArea;
			if (point.x >= m_rcSelected.right)
			{
				rcArea.left = m_rcSelected.right;
				rcArea.right = point.x;
			}
			if (point.x <= m_rcSelected.right)
			{
				rcArea.left = point.x;
				rcArea.right = m_rcSelected.right;
			}
			if (point.y >= m_rcSelected.top)
			{
				rcArea.top = m_rcSelected.top;
				rcArea.bottom = point.y;
			}
			else if (point.y <= m_rcSelected.top)
			{
				rcArea.top = point.y;
				rcArea.bottom = m_rcSelected.top;
			}

			m_rcRemote = rcArea;
// 			Image* image = 
// 				Gdiplus::Image::FromFile(L"SC_MASK.png");

			HBITMAP hBmp = (HBITMAP)m_pBitmap->GetSafeHandle();
			Bitmap bmp(hBmp, NULL);
			graph.DrawImage(&bmp, 0,0,rect.Width(), rect.Height());

			//画灰色的四个区域
			RectF rcLeft, rcRight, rcTop, rcBottom;

			{		//左边
				rcLeft.X = 0;
				rcLeft.Y = 0;
				rcLeft.Width = (REAL)rcArea.left;
				rcLeft.Height = (REAL)rect.Height();

				graph.DrawImage(m_pImgMask, rcLeft, 0,0, 1, 1, UnitPixel);
			}

			{		//右边
				rcRight.X = (REAL)rcArea.right;
				rcRight.Y = 0;
				rcRight.Width = (REAL)rect.right - rcArea.right;
				rcRight.Height = (REAL)rect.Height();
				graph.DrawImage(m_pImgMask, rcRight, 0,0, 1, 1, UnitPixel);
			}

			{		//上边
				rcTop.Y = 0;
				rcTop.X = (REAL)rcArea.left;
				rcTop.Width = (REAL)rcArea.Width();
				rcTop.Height = (REAL)rcArea.top;
				graph.DrawImage(m_pImgMask, rcTop, 0,0, 1, 1, UnitPixel);
			}

			{		//下边
				rcBottom.X = (REAL)rcArea.left;
				rcBottom.Y = (REAL)rcArea.bottom;
				rcBottom.Width = (REAL)rcArea.Width();
				rcBottom.Height = (REAL)rect.bottom - rcArea.bottom;
				graph.DrawImage(m_pImgMask, rcBottom, 0,0, 1, 1, UnitPixel);
			}

			//画8个拉伸的点
// 			Image* imageDot = 
// 				Gdiplus::Image::FromFile(L"SC_DOT.png");

			//top left
			graph.DrawImage(m_pImgDot, rcArea.left - 3, rcArea.top - 3, 5, 5);
			//bottom left
			graph.DrawImage(m_pImgDot, rcArea.left - 3, rcArea.bottom - 2, 5, 5);
			//top right
			graph.DrawImage(m_pImgDot, rcArea.right - 2, rcArea.top - 3, 5, 5);
			//bottom right
			graph.DrawImage(m_pImgDot, rcArea.right - 2, rcArea.bottom - 2, 5, 5);
			//left
			graph.DrawImage(m_pImgDot, rcArea.left -3, rcArea.bottom - (rcArea.bottom - rcArea.top)/2, 5, 5);
			//right
			graph.DrawImage(m_pImgDot, rcArea.right - 2, rcArea.bottom - (rcArea.bottom - rcArea.top)/2, 5, 5);
			//top 
			graph.DrawImage(m_pImgDot, rcArea.right - (rcArea.right - rcArea.left)/2, rcArea.top - 3, 5, 5);
			//bottom
			graph.DrawImage(m_pImgDot, rcArea.right - (rcArea.right - rcArea.left)/2, rcArea.bottom - 2, 5, 5);

			//画一个线边框
			Pen hPen(Color(255, 100, 100, 100), 1);
			graph.DrawLine(&hPen, rcArea.left, rcArea.top, rcArea.right, rcArea.top);
			graph.DrawLine(&hPen, rcArea.right, rcArea.top, rcArea.right, rcArea.bottom);
			graph.DrawLine(&hPen, rcArea.left, rcArea.bottom, rcArea.right, rcArea.bottom);
			graph.DrawLine(&hPen, rcArea.left, rcArea.top, rcArea.left, rcArea.bottom);

			RectF rcTitleSize;
			if (rcArea.top > 20)
			{

				rcTitleSize.X = (REAL)rcArea.left;
				rcTitleSize.Y = (REAL)rcArea.top - 20;
				rcTitleSize.Width = 245;
				rcTitleSize.Height = 20;
			}
			else
			{
				rcTitleSize.X = (REAL)rcArea.left;
				rcTitleSize.Y = (REAL)rcArea.top;
				rcTitleSize.Width = 245;
				rcTitleSize.Height = 20;
			}
			graph.DrawImage(m_pImgMask, rcTitleSize, 0,0, 1, 1, UnitPixel);

			CString csTitle;
			csTitle.Format(L"起始位置:%d.%d 矩形大小 %d*%d", 
				rcArea.left, rcArea.top,
				rcArea.right - rcArea.left,
				rcArea.bottom - rcArea.top);

			FontFamily fontFamily(L"楷体");
			Gdiplus::Font font(&fontFamily, 14, FontStyleRegular, UnitPixel);
			StringFormat strFormat;
			strFormat.SetAlignment(StringAlignmentCenter);
			strFormat.SetLineAlignment(StringAlignmentCenter);
			graph.DrawString(csTitle, -1, 
				&font, 
				PointF(rcTitleSize.X, rcTitleSize.Y+2),
				&SolidBrush(Color(255, 255,255,255)));

			BitBlt(hDC,0,0,rect.Width(),rect.Height(),dcCompatible.GetSafeHdc(),0,0,SRCCOPY);
		}
		else if (m_bLeftMove && m_bDraw)
		{
			HDC hDC = GetDC()->GetSafeHdc();
			HBITMAP hMemBitMap = 
				CreateCompatibleBitmap(hDC, rect.Width(), rect.Height());
			CDC dcCompatible;
			dcCompatible.CreateCompatibleDC(GetDC());
			dcCompatible.SelectObject(hMemBitMap);
			Graphics graph(dcCompatible.GetSafeHdc());

			CRect rcArea;
			rcArea.top = m_rcSelected.top;
			rcArea.bottom = m_rcSelected.bottom;

			if (point.x >= m_rcSelected.right)
			{
				rcArea.left = m_rcSelected.right;
				rcArea.right = point.x;
			}
			else if (point.x <= m_rcSelected.right)
			{
				rcArea.left = point.x;
				rcArea.right = m_rcSelected.right;
			}

			m_rcRemote = rcArea;
// 			Image* image = 
// 				Gdiplus::Image::FromFile(L"SC_MASK.png");

			HBITMAP hBmp = (HBITMAP)m_pBitmap->GetSafeHandle();
			Bitmap bmp(hBmp, NULL);
			graph.DrawImage(&bmp, 0,0,rect.Width(), rect.Height());

			//画灰色的四个区域
			RectF rcLeft, rcRight, rcTop, rcBottom;

			{		//左边
				rcLeft.X = 0;
				rcLeft.Y = 0;
				rcLeft.Width = (REAL)rcArea.left;
				rcLeft.Height = (REAL)rect.Height();

				graph.DrawImage(m_pImgMask, rcLeft, 0,0, 1, 1, UnitPixel);
			}

			{		//右边
				rcRight.X = (REAL)rcArea.right;
				rcRight.Y = 0;
				rcRight.Width = (REAL)rect.right - rcArea.right;
				rcRight.Height = (REAL)rect.Height();
				graph.DrawImage(m_pImgMask, rcRight, 0,0, 1, 1, UnitPixel);
			}

			{		//上边
				rcTop.Y = 0;
				rcTop.X = (REAL)rcArea.left;
				rcTop.Width = (REAL)rcArea.Width();
				rcTop.Height = (REAL)rcArea.top;
				graph.DrawImage(m_pImgMask, rcTop, 0,0, 1, 1, UnitPixel);
			}

			{		//下边
				rcBottom.X = (REAL)rcArea.left;
				rcBottom.Y = (REAL)rcArea.bottom;
				rcBottom.Width = (REAL)rcArea.Width();
				rcBottom.Height = (REAL)rect.bottom - rcArea.bottom;
				graph.DrawImage(m_pImgMask, rcBottom, 0,0, 1, 1, UnitPixel);
			}

			//画8个拉伸的点
// 			Image* imageDot = 
// 				Gdiplus::Image::FromFile(L"SC_DOT.png");

			//top left
			graph.DrawImage(m_pImgDot, rcArea.left - 3, rcArea.top - 3, 5, 5);
			//bottom left
			graph.DrawImage(m_pImgDot, rcArea.left - 3, rcArea.bottom - 2, 5, 5);
			//top right
			graph.DrawImage(m_pImgDot, rcArea.right - 2, rcArea.top - 3, 5, 5);
			//bottom right
			graph.DrawImage(m_pImgDot, rcArea.right - 2, rcArea.bottom - 2, 5, 5);
			//left
			graph.DrawImage(m_pImgDot, rcArea.left -3, rcArea.bottom - (rcArea.bottom - rcArea.top)/2, 5, 5);
			//right
			graph.DrawImage(m_pImgDot, rcArea.right - 2, rcArea.bottom - (rcArea.bottom - rcArea.top)/2, 5, 5);
			//top 
			graph.DrawImage(m_pImgDot, rcArea.right - (rcArea.right - rcArea.left)/2, rcArea.top - 3, 5, 5);
			//bottom
			graph.DrawImage(m_pImgDot, rcArea.right - (rcArea.right - rcArea.left)/2, rcArea.bottom - 2, 5, 5);

			//画一个线边框
			Pen hPen(Color(255, 100, 100, 100), 1);
			graph.DrawLine(&hPen, rcArea.left, rcArea.top, rcArea.right, rcArea.top);
			graph.DrawLine(&hPen, rcArea.right, rcArea.top, rcArea.right, rcArea.bottom);
			graph.DrawLine(&hPen, rcArea.left, rcArea.bottom, rcArea.right, rcArea.bottom);
			graph.DrawLine(&hPen, rcArea.left, rcArea.top, rcArea.left, rcArea.bottom);

			RectF rcTitleSize;
			if (rcArea.top > 20)
			{

				rcTitleSize.X = (REAL)rcArea.left;
				rcTitleSize.Y = (REAL)rcArea.top - 20;
				rcTitleSize.Width = 245;
				rcTitleSize.Height = 20;
			}
			else
			{
				rcTitleSize.X = (REAL)rcArea.left;
				rcTitleSize.Y = (REAL)rcArea.top;
				rcTitleSize.Width = 245;
				rcTitleSize.Height = 20;
			}
			graph.DrawImage(m_pImgMask, rcTitleSize, 0,0, 1, 1, UnitPixel);

			CString csTitle;
			csTitle.Format(L"起始位置:%d.%d 矩形大小 %d*%d", 
				rcArea.left, rcArea.top,
				rcArea.right - rcArea.left,
				rcArea.bottom - rcArea.top);

			FontFamily fontFamily(L"楷体");
			Gdiplus::Font font(&fontFamily, 14, FontStyleRegular, UnitPixel);
			StringFormat strFormat;
			strFormat.SetAlignment(StringAlignmentCenter);
			strFormat.SetLineAlignment(StringAlignmentCenter);
			graph.DrawString(csTitle, -1, 
				&font, 
				PointF(rcTitleSize.X, rcTitleSize.Y+2),
				&SolidBrush(Color(255, 255,255,255)));

			BitBlt(hDC,0,0,rect.Width(),rect.Height(),dcCompatible.GetSafeHdc(),0,0,SRCCOPY);
		}
	}

	CDialog::OnMouseMove(nFlags, point);
}

BOOL CBackGroundDlg::OnEraseBkgnd(CDC* pDC)
{
	BITMAP	bmp;
	m_pBitmap->GetBitmap(&bmp);
	CDC dcCompatible;
	dcCompatible.CreateCompatibleDC(pDC);
	dcCompatible.SelectObject(m_pBitmap);

	CRect rect;
	GetClientRect(&rect);
	pDC->BitBlt(0,0,rect.Width(),rect.Height(),&dcCompatible,0,0,SRCCOPY);

	return TRUE;
	//return CDialog::OnEraseBkgnd(pDC);
}
void CBackGroundDlg::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//双击保存截图
	CString csPictureName = _T("ScreenCapture-jinpeng-");
	CString csExt=_T(".png");
	CString csSaveFile=_T("");
	CTime tCurrentTime = CTime::GetCurrentTime();
	CString csTime = tCurrentTime.Format(_T("%Y%m%d%H%M%S"));
	csPictureName += csTime;
	CFileDialog fd(FALSE,_T("PNG"),csPictureName,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		_T("PNG 图片格式(*.png)|*.png|BMP 图片格式(*.bmp)|*.bmp|JPG 图片格式(*.jpg)|*.jpg||"),this);
	if(fd.DoModal()==IDCANCEL)
		return;
	csExt=fd.GetFileExt();//获取文件后缀
	csExt.Trim();//去掉字符左边和右边的空格
	csExt.MakeLower();//大写转小写
	csSaveFile=fd.GetPathName();//返回文件路径（盘符+路径+文件名）然后用_splitpath()进行分割，可以直接分割为盘符\路径\文件名\扩展名，我们一般是通过GetPathName()返回的结果解析出路径来

	CDC *dc=GetDC();
	CDC BuffDC;
	BuffDC.CreateCompatibleDC(dc);
	CBitmap BuffBmp;
	BuffBmp.CreateCompatibleBitmap(dc,m_rcSelected.Width(),m_rcSelected.Height());
	BuffDC.SelectObject(&BuffBmp);

	CDC pdc;
	pdc.CreateCompatibleDC(&BuffDC);
	pdc.SelectObject(m_pBitmap);
	BuffDC.BitBlt(0,0,m_rcSelected.Width(),m_rcSelected.Height(),&pdc,m_rcSelected.left,m_rcSelected.top,SRCCOPY);
	pdc.DeleteDC();

	CImage image;
	HRESULT hr;
	image.Attach(BuffBmp);//attaches hBitmap to CImage object 
	if (csExt=="bmp")
	{
		hr=image.Save(csSaveFile,Gdiplus::ImageFormatBMP);

	}
	else if(csExt=="jpg")
		hr=image.Save(csSaveFile,Gdiplus::ImageFormatJPEG);
	else
		hr=image.Save(csSaveFile,Gdiplus::ImageFormatPNG);

	if(FAILED(hr)){MessageBox(_T("保存失败！"));}//保存失败
	BuffDC.DeleteDC();
	BuffBmp.DeleteObject();
	ReleaseDC(dc);

	EndDialog(IDOK);
	CDialog::OnLButtonDblClk(nFlags, point);
}
