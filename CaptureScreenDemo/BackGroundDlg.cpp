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
	m_hCurHand = LoadCursor(NULL, IDC_HAND);
	m_hCurArrow = LoadCursor(NULL, IDC_ARROW);

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
	m_pImgFinish = NULL;
	m_pImgFinishGray = NULL;
	m_pImgSave = NULL;
	m_pImgSaveGray = NULL;
	m_pImgClipboard = NULL;
	m_pImgClipboardGray = NULL;

	m_bDrawOperate = false;

	m_drawState = EN_DRAW_NULL;

	m_bShowTip = false;
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

	ModifyStyleEx(WS_EX_APPWINDOW,WS_EX_TOOLWINDOW);

	EnableToolTips(TRUE);
	m_toolTip.Create(this);
	m_toolTip.Activate(TRUE);
	m_toolTip.AddTool(this, _T("operateBar"));

	m_cx = GetSystemMetrics(SM_CXSCREEN);
	m_cy = GetSystemMetrics(SM_CYSCREEN);

	::SetWindowPos(this->GetSafeHwnd(), 
		HWND_TOPMOST, 0,0, m_cx, m_cy, SWP_NOCOPYBITS);

	/*	2017-02-17	yangjinpeng
	@	加载工程中所用到的资源图片
	*/

	m_pImgMask = Gdiplus::Image::FromFile(L"SC_MASK.png");
	m_pImgDot = Gdiplus::Image::FromFile(L"SC_DOT.png");

	m_pImgFinish = Gdiplus::Image::FromFile(L"finish_capture_normal.png");
	m_pImgFinishGray = Gdiplus::Image::FromFile(L"finish_capture_gray.png");
	m_pImgSave = Gdiplus::Image::FromFile(L"save_normal.png");
	m_pImgSaveGray = Gdiplus::Image::FromFile(L"save_gray.png");
	m_pImgClipboard = Gdiplus::Image::FromFile(L"clipboard_normal.png");
	m_pImgClipboardGray = Gdiplus::Image::FromFile(L"clipboard_gray.png");
	m_pImgArrow = Gdiplus::Image::FromFile(L"arrow.png");
	m_pImgRectangle = Gdiplus::Image::FromFile(L"rect.png");
	m_pImgEllipse = Gdiplus::Image::FromFile(L"ellipse.png");
	m_pimgOperateBG = Gdiplus::Image::FromFile(L"OperateBar_BG.png");


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

	/*	2017-02-22	yangjinpeng
	@	如果鼠标按下区域在截取的区域内，有两种操作
	@	1、整体移动
	@	2、绘制其他图形或者涂鸦
	*/
	if (PtInRect(m_rcSelected, point) && !m_bDrawOperate)		//选择整块移动
	{
		m_bMoveAll = true;
		SetCursor(m_hCurSelect);		
	}
	else if (!PtInRect(m_rcSelected, point) && !m_bDrawOperate)	//排除掉在已截取的区域外的绘图
	{
		m_bDraw = false;
		SetCursor(m_hCurArrow);
	}
	else
		SetCursor(m_hCurArrow);

	if (PtInRect(CRect(m_rcSelected.left - 3, 
		m_rcSelected.top - 3, 
		m_rcSelected.left + 2, 
		m_rcSelected.top + 2), point))		//左上鼠标光标，左上角拉伸
	{
		SetCursor(m_hCurLeftTop);
		m_bLeftTopMove = true;
	}

	if (PtInRect(CRect(m_rcSelected.right - (m_rcSelected.right - m_rcSelected.left)/2,
		m_rcSelected.top - 3,
		m_rcSelected.right - (m_rcSelected.right - m_rcSelected.left)/2 + 5,
		m_rcSelected.top + 2), point))		//上边鼠标光标，上边拉伸
	{
		SetCursor(m_hCurTop);
		m_bTopMove = true;
	}

	if (PtInRect(CRect(m_rcSelected.right - 2, 
		m_rcSelected.top - 3,
		m_rcSelected.right + 3,
		m_rcSelected.top + 2), point))		//右上鼠标光标，右上角拉伸
	{
		SetCursor(m_hCurRightTop);
		m_bRightTopMove = true;
	}

	if (PtInRect(CRect(m_rcSelected.right - 2, 
		m_rcSelected.bottom - (m_rcSelected.bottom - m_rcSelected.top)/2,
		m_rcSelected.right + 3, 
		m_rcSelected.bottom - (m_rcSelected.bottom - m_rcSelected.top)/2 + 5), point))		//右边鼠标光标，右边拉伸
	{
		SetCursor(m_hCurRight);
		m_bRightMove = true;
	}

	if (PtInRect(CRect(m_rcSelected.right - 2, 
		m_rcSelected.bottom - 2,
		m_rcSelected.right + 3, 
		m_rcSelected.bottom + 3), point))		//右下鼠标光标，右下角拉伸
	{
		SetCursor(m_hCurRightBottom);
		m_bRightBottomMove = true;
	}

	if (PtInRect(CRect(m_rcSelected.right - (m_rcSelected.right - m_rcSelected.left)/2, 
		m_rcSelected.bottom - 2,
		m_rcSelected.right - (m_rcSelected.right - m_rcSelected.left)/2 + 5, 
		m_rcSelected.bottom + 3), point))		//下边鼠标光标，下边拉伸
	{
		SetCursor(m_hCurBottom);
		m_bBottomMove = true;
	}

	if (PtInRect(CRect(m_rcSelected.left - 3, 
		m_rcSelected.bottom - 2,
		m_rcSelected.left + 2, 
		m_rcSelected.bottom + 3), point))		//左下鼠标光标，左下角拉伸
	{
		SetCursor(m_hCurLeftBottom);
		m_bLeftBottomMove = true;
	}

	if (PtInRect(CRect(m_rcSelected.left -3, 
		m_rcSelected.bottom - (m_rcSelected.bottom - m_rcSelected.top)/2,
		m_rcSelected.left + 2,
		m_rcSelected.bottom - (m_rcSelected.bottom - m_rcSelected.top)/2 + 5), point))		//左边鼠标光标，左边拉伸
	{
		SetCursor(m_hCurLeft);
		m_bLeftMove = true;
	}

	//begin operate_bar
	if (PtInRect(m_rcFinish, point))			
	{
		//TODO:	截图完成，可在此处写相应的逻辑处理代码	
		SetCursor(m_hCurHand);		//设置鼠标为手型
		CopyScreenCaptureToClipboard();
	}

	if (PtInRect(m_rcSave, point))
	{
		SetCursor(m_hCurHand);
		//TODO:	可调用保存图片的函数对截取的屏幕进行保存
		SaveScreenCapture();
	}

	if (PtInRect(m_rcClipboard, point))
	{
		SetCursor(m_hCurHand);
		//TODO:	将截取的屏幕图像放到剪贴板
		CopyScreenCaptureToClipboard();
	}

	if (PtInRect(m_rcArrow, point))
	{
		SetCursor(m_hCurHand);			//选取了画的形状为箭头
		m_bDrawOperate = true;

		m_drawState = EN_DRAW_ARROW;
	}

	if (PtInRect(m_rcEllipse, point))
	{
		SetCursor(m_hCurHand);
		m_bDrawOperate = true;

		m_drawState = EN_DRAW_ELLIPSE;
	}

	if (PtInRect(m_rcRectangle, point))
	{
		SetCursor(m_hCurHand);
		m_bDrawOperate = true;

		m_drawState = EN_DRAW_RECT;
	}
	//end

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

		if (m_bMoveAll && !m_bDrawOperate)
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
		else
			SetCursor(m_hCurArrow);
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

	//begin operate_bar
	if (PtInRect(m_rcFinish, point))
	{
		SetCursor(m_hCurHand);
	}

	if (PtInRect(m_rcSave, point))
	{
		SetCursor(m_hCurHand);
	}

	if (PtInRect(m_rcClipboard, point))
	{
		SetCursor(m_hCurHand);
	}

	if (PtInRect(m_rcArrow, point))
	{
		SetCursor(m_hCurHand);
	}

	if (PtInRect(m_rcEllipse, point))
	{
		SetCursor(m_hCurHand);
	}

	if (PtInRect(m_rcRectangle, point))
	{
		SetCursor(m_hCurHand);
	}
	//end

	//将画完Shape的图像保存下来
	if (m_bDrawOperate && m_drawState != EN_DRAW_NULL)
	{
		CRect rect;
		GetClientRect(&rect);
		m_pBitmap = 
			CBitmap::FromHandle(
			CopyClientToBitmap(rect.left, 
			rect.top, 
			rect.Width(), 
			rect.Height())); 
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

	m_toolTip.RelayEvent(pMsg);

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

	if (PtInRect(m_rcSelected, point) && !m_bFirstDraw && !m_bDrawOperate)		//全部鼠标光标
	{
		SetCursor(m_hCurSelect);
	}
	else
		SetCursor(m_hCurArrow);

	//begin	operate_bar 
	if (PtInRect(m_rcFinish, point) && !m_bShowTip)
	{
// 		m_toolTip.UpdateTipText(L"完成", this);
// 		CToolInfo       sTinfo;                // 提示信息
// 		m_toolTip.GetToolInfo(sTinfo, this);
// 		sTinfo.uFlags = TTF_TRACK;     // 显示方式设置
// 		m_toolTip.SetToolInfo(&sTinfo);
// 
// 		m_toolTip.SendMessage(TTM_TRACKPOSITION, 0, 
// 			(LPARAM)MAKELONG(m_rcFinish.left, m_rcFinish.bottom));
// 		m_toolTip.SendMessage(TTM_TRACKACTIVATE, TRUE, (LPARAM)&sTinfo ); // tips显示
// 
// 		m_bShowTip = true;
		SetCursor(m_hCurHand);
	}
	else
	{
// 		m_toolTip.UpdateTipText(L"", this);
// 		CToolInfo       sTinfo;                // 提示信息
// 		m_toolTip.GetToolInfo(sTinfo, this);
// 		sTinfo.uFlags = TTF_TRACK;     // 显示方式设置
// 		m_toolTip.SetToolInfo(&sTinfo);
// 
// 		m_toolTip.SendMessage(TTM_TRACKPOSITION, 0, 
// 			(LPARAM)MAKELONG(m_rcFinish.left, m_rcFinish.bottom));
// 		m_toolTip.SendMessage(TTM_TRACKACTIVATE, TRUE, (LPARAM)&sTinfo ); // tips显示
// 		m_bShowTip = false;
	}
	
	if (PtInRect(m_rcSave, point) && !m_bShowTip)
	{
// 		m_toolTip.UpdateTipText(L"保存", this);
// 		CToolInfo       sTinfo;                // 提示信息
// 		m_toolTip.GetToolInfo(sTinfo, this);
// 		sTinfo.uFlags = TTF_TRACK;     // 显示方式设置
// 		m_toolTip.SetToolInfo(&sTinfo);
// 
// 		m_toolTip.SendMessage(TTM_TRACKPOSITION, 0, 
// 			(LPARAM)MAKELONG(m_rcSave.left, m_rcSave.bottom));
// 		m_toolTip.SendMessage(TTM_TRACKACTIVATE, TRUE, (LPARAM)&sTinfo ); // tips显示
// 
// 		m_bShowTip = true;
		SetCursor(m_hCurHand);
	}
	else
	{
// 		m_toolTip.UpdateTipText(L"", this);
// 		CToolInfo       sTinfo;                // 提示信息
// 		m_toolTip.GetToolInfo(sTinfo, this);
// 		sTinfo.uFlags = TTF_TRACK;     // 显示方式设置
// 		m_toolTip.SetToolInfo(&sTinfo);
// 
// 		m_toolTip.SendMessage(TTM_TRACKPOSITION, 0, 
// 			(LPARAM)MAKELONG(m_rcSave.left, m_rcSave.bottom));
// 		m_toolTip.SendMessage(TTM_TRACKACTIVATE, TRUE, (LPARAM)&sTinfo ); // tips显示
// 		m_bShowTip = false;
	}

	if (PtInRect(m_rcClipboard, point) && !m_bShowTip)
	{
// 		m_toolTip.UpdateTipText(L"复制到剪贴板", this);
// 		CToolInfo       sTinfo;                // 提示信息
// 		m_toolTip.GetToolInfo(sTinfo, this);
// 		sTinfo.uFlags = TTF_TRACK;     // 显示方式设置
// 		m_toolTip.SetToolInfo(&sTinfo);
// 
// 		m_toolTip.SendMessage(TTM_TRACKPOSITION, 0, 
// 			(LPARAM)MAKELONG(m_rcClipboard.left, m_rcClipboard.bottom));
// 		m_toolTip.SendMessage(TTM_TRACKACTIVATE, TRUE, (LPARAM)&sTinfo ); // tips显示
// 
// 		m_bShowTip = true;
		SetCursor(m_hCurHand);
	}
	else
	{
// 		m_toolTip.UpdateTipText(L"", this);
// 		CToolInfo       sTinfo;                // 提示信息
// 		m_toolTip.GetToolInfo(sTinfo, this);
// 		sTinfo.uFlags = TTF_TRACK;     // 显示方式设置
// 		m_toolTip.SetToolInfo(&sTinfo);
// 
// 		m_toolTip.SendMessage(TTM_TRACKPOSITION, 0, 
// 			(LPARAM)MAKELONG(m_rcClipboard.left, m_rcClipboard.bottom));
// 		m_toolTip.SendMessage(TTM_TRACKACTIVATE, TRUE, (LPARAM)&sTinfo ); // tips显示
// 		m_bShowTip = false;
	}

	if (PtInRect(m_rcArrow, point) && !m_bShowTip)
	{
// 		m_toolTip.UpdateTipText(L"箭头", this);
// 		CToolInfo       sTinfo;                // 提示信息
// 		m_toolTip.GetToolInfo(sTinfo, this);
// 		sTinfo.uFlags = TTF_TRACK;     // 显示方式设置
// 		m_toolTip.SetToolInfo(&sTinfo);
// 
// 		m_toolTip.SendMessage(TTM_TRACKPOSITION, 0, 
// 			(LPARAM)MAKELONG(m_rcArrow.left, m_rcArrow.bottom));
// 		m_toolTip.SendMessage(TTM_TRACKACTIVATE, TRUE, (LPARAM)&sTinfo ); // tips显示
// 
// 		m_bShowTip = true;
		SetCursor(m_hCurHand);
	}
	else
	{
// 		m_toolTip.UpdateTipText(L"", this);
// 		CToolInfo       sTinfo;                // 提示信息
// 		m_toolTip.GetToolInfo(sTinfo, this);
// 		sTinfo.uFlags = TTF_TRACK;     // 显示方式设置
// 		m_toolTip.SetToolInfo(&sTinfo);
// 
// 		m_toolTip.SendMessage(TTM_TRACKPOSITION, 0, 
// 			(LPARAM)MAKELONG(m_rcArrow.left, m_rcArrow.bottom));
// 		m_toolTip.SendMessage(TTM_TRACKACTIVATE, TRUE, (LPARAM)&sTinfo ); // tips显示
// 		m_bShowTip = false;
	}

	if (PtInRect(m_rcEllipse, point) && !m_bShowTip)
	{
// 		m_toolTip.UpdateTipText(L"椭圆", this);
// 		CToolInfo       sTinfo;                // 提示信息
// 		m_toolTip.GetToolInfo(sTinfo, this);
// 		sTinfo.uFlags = TTF_TRACK;     // 显示方式设置
// 		m_toolTip.SetToolInfo(&sTinfo);
// 
// 		m_toolTip.SendMessage(TTM_TRACKPOSITION, 0, 
// 			(LPARAM)MAKELONG(m_rcEllipse.left, m_rcEllipse.bottom));
// 		m_toolTip.SendMessage(TTM_TRACKACTIVATE, TRUE, (LPARAM)&sTinfo ); // tips显示
// 
// 		m_bShowTip = true;
		SetCursor(m_hCurHand);
	}
	else
	{
// 		m_toolTip.UpdateTipText(L"", this);
// 		CToolInfo       sTinfo;                // 提示信息
// 		m_toolTip.GetToolInfo(sTinfo, this);
// 		sTinfo.uFlags = TTF_TRACK;     // 显示方式设置
// 		m_toolTip.SetToolInfo(&sTinfo);
// 
// 		m_toolTip.SendMessage(TTM_TRACKPOSITION, 0, 
// 			(LPARAM)MAKELONG(m_rcEllipse.left, m_rcEllipse.bottom));
// 		m_toolTip.SendMessage(TTM_TRACKACTIVATE, TRUE, (LPARAM)&sTinfo ); // tips显示
// 
// 		m_bShowTip = false;
	}
	
	if (PtInRect(m_rcRectangle, point) && !m_bShowTip)
	{
// 		m_toolTip.UpdateTipText(L"矩形", this);
// 		CToolInfo       sTinfo;                // 提示信息
// 		m_toolTip.GetToolInfo(sTinfo, this);
// 		sTinfo.uFlags = TTF_TRACK;     // 显示方式设置
// 		m_toolTip.SetToolInfo(&sTinfo);
// 
// 		m_toolTip.SendMessage(TTM_TRACKPOSITION, 0, 
// 			(LPARAM)MAKELONG(m_rcRectangle.left, m_rcRectangle.bottom));
// 		m_toolTip.SendMessage(TTM_TRACKACTIVATE, TRUE, (LPARAM)&sTinfo ); // tips显示
// 
// 		m_bShowTip = true;
		SetCursor(m_hCurHand);
	}
	else
	{
// 		m_toolTip.UpdateTipText(L"", this);
// 		CToolInfo       sTinfo;                // 提示信息
// 		m_toolTip.GetToolInfo(sTinfo, this);
// 		sTinfo.uFlags = TTF_TRACK;     // 显示方式设置
// 		m_toolTip.SetToolInfo(&sTinfo);
// 
// 		m_toolTip.SendMessage(TTM_TRACKPOSITION, 0, 
// 			(LPARAM)MAKELONG(m_rcRectangle.left, m_rcRectangle.bottom));
// 		m_toolTip.SendMessage(TTM_TRACKACTIVATE, TRUE, (LPARAM)&sTinfo ); // tips显示
// 
// 		m_bShowTip = false;
	}
	//end

	if (PtInRect(CRect(m_rcSelected.left - 3, 
		m_rcSelected.top - 3, 
		m_rcSelected.left + 2, 
		m_rcSelected.top + 2), point) && !m_bFirstDraw)		//左上鼠标光标
	{
		SetCursor(m_hCurLeftTop);
	}

	if (PtInRect(CRect(m_rcSelected.right - (m_rcSelected.right - m_rcSelected.left)/2,
		m_rcSelected.top - 3,
		m_rcSelected.right - (m_rcSelected.right - m_rcSelected.left)/2 + 5,
		m_rcSelected.top + 2), point)  && !m_bFirstDraw)		//上边鼠标光标
	{
		SetCursor(m_hCurTop);
	}

	if (PtInRect(CRect(m_rcSelected.right - 2, 
		m_rcSelected.top - 3,
		m_rcSelected.right + 3,
		m_rcSelected.top + 2), point) && !m_bFirstDraw)		//右上鼠标光标
	{
		SetCursor(m_hCurRightTop);
	}

	if (PtInRect(CRect(m_rcSelected.right - 2, 
		m_rcSelected.bottom - (m_rcSelected.bottom - m_rcSelected.top)/2,
		m_rcSelected.right + 3, 
		m_rcSelected.bottom - (m_rcSelected.bottom - m_rcSelected.top)/2 + 5), point) && !m_bFirstDraw)		//右边鼠标光标
	{
		SetCursor(m_hCurRight);
	}

	if (PtInRect(CRect(m_rcSelected.right - 2, 
		m_rcSelected.bottom - 2,
		m_rcSelected.right + 3, 
		m_rcSelected.bottom + 3), point) && !m_bFirstDraw)		//右下鼠标光标
	{
		SetCursor(m_hCurRightBottom);
	}

	if (PtInRect(CRect(m_rcSelected.right - (m_rcSelected.right - m_rcSelected.left)/2, 
		m_rcSelected.bottom - 2,
		m_rcSelected.right - (m_rcSelected.right - m_rcSelected.left)/2 + 5, 
		m_rcSelected.bottom + 3), point) && !m_bFirstDraw)		//下边鼠标光标
	{
		SetCursor(m_hCurBottom);
	}

	if (PtInRect(CRect(m_rcSelected.left - 3, 
		m_rcSelected.bottom - 2,
		m_rcSelected.left + 2, 
		m_rcSelected.bottom + 3), point) && !m_bFirstDraw)		//左下鼠标光标
	{
		SetCursor(m_hCurLeftBottom);
	}

	if (PtInRect(CRect(m_rcSelected.left -3, 
		m_rcSelected.bottom - (m_rcSelected.bottom - m_rcSelected.top)/2,
		m_rcSelected.left + 2,
		m_rcSelected.bottom - (m_rcSelected.bottom - m_rcSelected.top)/2 + 5), point) && !m_bFirstDraw)		//左边鼠标光标
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
			@	使用区域，整个为一个区域、选中为一个区域，这两个区域之间的补集操作得到需要画灰色方块的区域
			@	向此区域中画灰色Mask
			@	方法二：
			@	将原矩形分割成上、下、左、右、四部分，分块画灰色Mask
			*/
			
			HBITMAP hBmp = (HBITMAP)m_pBitmap->GetSafeHandle();
			Bitmap bmp(hBmp, NULL);
			graph.DrawImage(&bmp, 0,0,rect.Width(), rect.Height());

			//画灰色的四个区域
			RectF rcLeft, rcRight, rcTop, rcBottom;
			CalcGrayAreaRect(rcArea, rcLeft, rcTop, rcRight, rcBottom);
			graph.DrawImage(m_pImgMask, rcLeft, 0,0, 1, 1, UnitPixel);
			graph.DrawImage(m_pImgMask, rcRight, 0,0, 1, 1, UnitPixel);
			graph.DrawImage(m_pImgMask, rcTop, 0,0, 1, 1, UnitPixel);
			graph.DrawImage(m_pImgMask, rcBottom, 0,0, 1, 1, UnitPixel);
			
			//画8个拉伸的点
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

			RectF rcOperateBar, rcFinish, rcSave, rcClipboard, rcArrow, rcEllipse, rcRect;
			CalcOperateBarRect(rcArea, rcOperateBar, rcFinish, rcSave, rcClipboard, rcArrow, rcEllipse, rcRect);
			graph.DrawImage(m_pimgOperateBG, rcOperateBar, 0,0, 1, 1, UnitPixel);
			graph.DrawImage(m_pImgFinish, rcFinish, 0,0, 30, 30, UnitPixel);
			graph.DrawImage(m_pImgSave, rcSave, 0,0, 30, 30, UnitPixel);
			graph.DrawImage(m_pImgClipboard, rcClipboard, 0,0, 30, 30, UnitPixel);
			graph.DrawImage(m_pImgArrow, rcArrow, 0,0, 30, 30, UnitPixel);
			graph.DrawImage(m_pImgEllipse, rcEllipse, 0,0, 30, 30, UnitPixel);
			graph.DrawImage(m_pImgRectangle, rcRect, 0,0, 30, 30, UnitPixel);

			BitBlt(hDC,0,0,rect.Width(),rect.Height(),dcCompatible.GetSafeHdc(),0,0,SRCCOPY);
		}
	}
	else
	{
		if ( m_bMoveAll && m_bDraw && !m_bDrawOperate)
		{
			MoveAll(point);				//整体移动
		}
		else if (m_bLeftTopMove && m_bDraw && !m_bDrawOperate)
		{
			TrackLeftTop(point);		//左上角拉伸
		}
		else if (m_bTopMove && m_bDraw && !m_bDrawOperate)
		{
			TrackTop(point);			//上边拉伸
		}
		else if (m_bRightTopMove && m_bDraw && !m_bDrawOperate)
		{
			TrackRightTop(point);		//右上角拉伸
		}
		else if (m_bRightMove && m_bDraw && !m_bDrawOperate)
		{
			TrackRight(point);			//右边拉伸
		}
		else if (m_bRightBottomMove && m_bDraw && !m_bDrawOperate)
		{
			TrackRightBottom(point);	//右下拉伸
		}
		else if (m_bBottomMove && m_bDraw && !m_bDrawOperate)
		{
			TrackBottom(point);			//下边拉伸
		}
		else if (m_bLeftBottomMove && m_bDraw && !m_bDrawOperate)
		{
			TrackLeftBottom(point);		//左下角拉伸
		}
		else if (m_bLeftMove && m_bDraw && !m_bDrawOperate)
		{
			TrackLeft(point);			//左边拉伸
		}

		if (m_bDraw && m_bDrawOperate && PtInRect(m_rcSelected, m_ptDown))
		{
			CPoint ptRemote = point;
			if (point.x <= m_rcSelected.left)
			{
				ptRemote.x = m_rcSelected.left;
			}

			if (point.x >= m_rcSelected.right)
			{
				ptRemote.x = m_rcSelected.right;
			}

			if (point.y <= m_rcSelected.top)
			{
				ptRemote.y = m_rcSelected.top;
			}

			if (point.y >= m_rcSelected.bottom)
			{
				ptRemote.y = m_rcSelected.bottom;
			}

			switch (m_drawState)
			{
			case EN_DRAW_ARROW:
				{
					DrawArrow(ptRemote);
				}
				break;

			case EN_DRAW_ELLIPSE:
				{
					DrawEllipse(ptRemote);
				}
				break;

			case EN_DRAW_RECT:
				{
					DrawRectangle(ptRemote);
				}
				break;

			default:
				break;
			}
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
	
	CDialog::OnLButtonDblClk(nFlags, point);
}


void CBackGroundDlg::MoveAll(CPoint point)
{
	//TODO:	整个截图移动
	CRect rect;
	GetClientRect(&rect);

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

	HBITMAP hBmp = (HBITMAP)m_pBitmap->GetSafeHandle();
	Bitmap bmp(hBmp, NULL);
	graph.DrawImage(&bmp, 0,0,rect.Width(), rect.Height());

	//画灰色的四个区域
	RectF rcLeft, rcRight, rcTop, rcBottom;
	CalcGrayAreaRect(rcArea, rcLeft, rcTop, rcRight, rcBottom);
	graph.DrawImage(m_pImgMask, rcLeft, 0,0, 1, 1, UnitPixel);
	graph.DrawImage(m_pImgMask, rcRight, 0,0, 1, 1, UnitPixel);
	graph.DrawImage(m_pImgMask, rcTop, 0,0, 1, 1, UnitPixel);
	graph.DrawImage(m_pImgMask, rcBottom, 0,0, 1, 1, UnitPixel);

	//画8个拉伸的点
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
		rcTitleSize.Y = (REAL)(rcArea.top - 20);
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

	RectF rcOperateBar, rcFinish, rcSave, rcClipboard, rcArrow, rcEllipse, rcRect;
	CalcOperateBarRect(rcArea, rcOperateBar, rcFinish, rcSave, rcClipboard, rcArrow, rcEllipse, rcRect);
	graph.DrawImage(m_pimgOperateBG, rcOperateBar, 0,0, 1, 1, UnitPixel);
	graph.DrawImage(m_pImgFinish, rcFinish, 0,0, 30, 30, UnitPixel);
	graph.DrawImage(m_pImgSave, rcSave, 0,0, 30, 30, UnitPixel);
	graph.DrawImage(m_pImgClipboard, rcClipboard, 0,0, 30, 30, UnitPixel);
	graph.DrawImage(m_pImgArrow, rcArrow, 0,0, 30, 30, UnitPixel);
	graph.DrawImage(m_pImgEllipse, rcEllipse, 0,0, 30, 30, UnitPixel);
	graph.DrawImage(m_pImgRectangle, rcRect, 0,0, 30, 30, UnitPixel);

	BitBlt(hDC,0,0,rect.Width(),rect.Height(),dcCompatible.GetSafeHdc(),0,0,SRCCOPY);
}

void CBackGroundDlg::TrackLeftTop(CPoint point)
{
	//TODO：左上角拉伸
	CRect rect;
	GetClientRect(&rect);

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
	HBITMAP hBmp = (HBITMAP)m_pBitmap->GetSafeHandle();
	Bitmap bmp(hBmp, NULL);
	graph.DrawImage(&bmp, 0,0,rect.Width(), rect.Height());

	//画灰色的四个区域
	RectF rcLeft, rcRight, rcTop, rcBottom;
	CalcGrayAreaRect(rcArea, rcLeft, rcTop, rcRight, rcBottom);
	graph.DrawImage(m_pImgMask, rcLeft, 0,0, 1, 1, UnitPixel);
	graph.DrawImage(m_pImgMask, rcRight, 0,0, 1, 1, UnitPixel);
	graph.DrawImage(m_pImgMask, rcTop, 0,0, 1, 1, UnitPixel);
	graph.DrawImage(m_pImgMask, rcBottom, 0,0, 1, 1, UnitPixel);

	//画8个拉伸的点
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

	RectF rcOperateBar, rcFinish, rcSave, rcClipboard, rcArrow, rcEllipse, rcRect;
	CalcOperateBarRect(rcArea, rcOperateBar, rcFinish, rcSave, rcClipboard, rcArrow, rcEllipse, rcRect);
	graph.DrawImage(m_pimgOperateBG, rcOperateBar, 0,0, 1, 1, UnitPixel);
	graph.DrawImage(m_pImgFinish, rcFinish, 0,0, 30, 30, UnitPixel);
	graph.DrawImage(m_pImgSave, rcSave, 0,0, 30, 30, UnitPixel);
	graph.DrawImage(m_pImgClipboard, rcClipboard, 0,0, 30, 30, UnitPixel);
	graph.DrawImage(m_pImgArrow, rcArrow, 0,0, 30, 30, UnitPixel);
	graph.DrawImage(m_pImgEllipse, rcEllipse, 0,0, 30, 30, UnitPixel);
	graph.DrawImage(m_pImgRectangle, rcRect, 0,0, 30, 30, UnitPixel);

	BitBlt(hDC,0,0,rect.Width(),rect.Height(),dcCompatible.GetSafeHdc(),0,0,SRCCOPY);
}

void CBackGroundDlg::TrackTop(CPoint point)
{
	//TODO：上边拉伸
	CRect rect;
	GetClientRect(&rect);

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
	HBITMAP hBmp = (HBITMAP)m_pBitmap->GetSafeHandle();
	Bitmap bmp(hBmp, NULL);
	graph.DrawImage(&bmp, 0,0,rect.Width(), rect.Height());

	//画灰色的四个区域
	RectF rcLeft, rcRight, rcTop, rcBottom;
	CalcGrayAreaRect(rcArea, rcLeft, rcTop, rcRight, rcBottom);
	graph.DrawImage(m_pimgOperateBG, rcLeft, 0,0, 1, 1, UnitPixel);
	graph.DrawImage(m_pImgMask, rcRight, 0,0, 1, 1, UnitPixel);
	graph.DrawImage(m_pImgMask, rcTop, 0,0, 1, 1, UnitPixel);
	graph.DrawImage(m_pImgMask, rcBottom, 0,0, 1, 1, UnitPixel);

	//画8个拉伸的点
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

	RectF rcOperateBar, rcFinish, rcSave, rcClipboard, rcArrow, rcEllipse, rcRect;
	CalcOperateBarRect(rcArea, rcOperateBar, rcFinish, rcSave, rcClipboard, rcArrow, rcEllipse, rcRect);
	graph.DrawImage(m_pimgOperateBG, rcOperateBar, 0,0, 1, 1, UnitPixel);
	graph.DrawImage(m_pImgFinish, rcFinish, 0,0, 30, 30, UnitPixel);
	graph.DrawImage(m_pImgSave, rcSave, 0,0, 30, 30, UnitPixel);
	graph.DrawImage(m_pImgClipboard, rcClipboard, 0,0, 30, 30, UnitPixel);
	graph.DrawImage(m_pImgArrow, rcArrow, 0,0, 30, 30, UnitPixel);
	graph.DrawImage(m_pImgEllipse, rcEllipse, 0,0, 30, 30, UnitPixel);
	graph.DrawImage(m_pImgRectangle, rcRect, 0,0, 30, 30, UnitPixel);

	BitBlt(hDC,0,0,rect.Width(),rect.Height(),dcCompatible.GetSafeHdc(),0,0,SRCCOPY);
}

void CBackGroundDlg::TrackRightTop(CPoint point)
{
	//TODO:	右上角拉伸
	CRect rect;
	GetClientRect(&rect);

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
	HBITMAP hBmp = (HBITMAP)m_pBitmap->GetSafeHandle();
	Bitmap bmp(hBmp, NULL);
	graph.DrawImage(&bmp, 0,0,rect.Width(), rect.Height());

	//画灰色的四个区域
	RectF rcLeft, rcRight, rcTop, rcBottom;
	CalcGrayAreaRect(rcArea, rcLeft, rcTop, rcRight, rcBottom);
	graph.DrawImage(m_pImgMask, rcLeft, 0,0, 1, 1, UnitPixel);
	graph.DrawImage(m_pImgMask, rcRight, 0,0, 1, 1, UnitPixel);
	graph.DrawImage(m_pImgMask, rcTop, 0,0, 1, 1, UnitPixel);
	graph.DrawImage(m_pImgMask, rcBottom, 0,0, 1, 1, UnitPixel);

	//画8个拉伸的点
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

	RectF rcOperateBar, rcFinish, rcSave, rcClipboard, rcArrow, rcEllipse, rcRect;
	CalcOperateBarRect(rcArea, rcOperateBar, rcFinish, rcSave, rcClipboard, rcArrow, rcEllipse, rcRect);
	graph.DrawImage(m_pimgOperateBG, rcOperateBar, 0,0, 1, 1, UnitPixel);
	graph.DrawImage(m_pImgFinish, rcFinish, 0,0, 30, 30, UnitPixel);
	graph.DrawImage(m_pImgSave, rcSave, 0,0, 30, 30, UnitPixel);
	graph.DrawImage(m_pImgClipboard, rcClipboard, 0,0, 30, 30, UnitPixel);
	graph.DrawImage(m_pImgArrow, rcArrow, 0,0, 30, 30, UnitPixel);
	graph.DrawImage(m_pImgEllipse, rcEllipse, 0,0, 30, 30, UnitPixel);
	graph.DrawImage(m_pImgRectangle, rcRect, 0,0, 30, 30, UnitPixel);

	BitBlt(hDC,0,0,rect.Width(),rect.Height(),dcCompatible.GetSafeHdc(),0,0,SRCCOPY);
}

void CBackGroundDlg::TrackRight(CPoint point)
{
	//TODO:	右边拉伸
	CRect rect;
	GetClientRect(&rect);

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
	HBITMAP hBmp = (HBITMAP)m_pBitmap->GetSafeHandle();
	Bitmap bmp(hBmp, NULL);
	graph.DrawImage(&bmp, 0,0,rect.Width(), rect.Height());

	//画灰色的四个区域
	RectF rcLeft, rcRight, rcTop, rcBottom;
	CalcGrayAreaRect(rcArea, rcLeft, rcTop, rcRight, rcBottom);
	graph.DrawImage(m_pImgMask, rcLeft, 0,0, 1, 1, UnitPixel);
	graph.DrawImage(m_pImgMask, rcRight, 0,0, 1, 1, UnitPixel);
	graph.DrawImage(m_pImgMask, rcTop, 0,0, 1, 1, UnitPixel);
	graph.DrawImage(m_pImgMask, rcBottom, 0,0, 1, 1, UnitPixel);

	//画8个拉伸的点
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

	RectF rcOperateBar, rcFinish, rcSave, rcClipboard, rcArrow, rcEllipse, rcRect;
	CalcOperateBarRect(rcArea, rcOperateBar, rcFinish, rcSave, rcClipboard, rcArrow, rcEllipse, rcRect);
	graph.DrawImage(m_pimgOperateBG, rcOperateBar, 0,0, 1, 1, UnitPixel);
	graph.DrawImage(m_pImgFinish, rcFinish, 0,0, 30, 30, UnitPixel);
	graph.DrawImage(m_pImgSave, rcSave, 0,0, 30, 30, UnitPixel);
	graph.DrawImage(m_pImgClipboard, rcClipboard, 0,0, 30, 30, UnitPixel);
	graph.DrawImage(m_pImgArrow, rcArrow, 0,0, 30, 30, UnitPixel);
	graph.DrawImage(m_pImgEllipse, rcEllipse, 0,0, 30, 30, UnitPixel);
	graph.DrawImage(m_pImgRectangle, rcRect, 0,0, 30, 30, UnitPixel);

	BitBlt(hDC,0,0,rect.Width(),rect.Height(),dcCompatible.GetSafeHdc(),0,0,SRCCOPY);
}

void CBackGroundDlg::TrackRightBottom(CPoint point)
{
	//TODO:	右下角拉伸
	CRect rect;
	GetClientRect(&rect);

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
	HBITMAP hBmp = (HBITMAP)m_pBitmap->GetSafeHandle();
	Bitmap bmp(hBmp, NULL);
	graph.DrawImage(&bmp, 0,0,rect.Width(), rect.Height());

	//画灰色的四个区域
	RectF rcLeft, rcRight, rcTop, rcBottom;
	CalcGrayAreaRect(rcArea, rcLeft, rcTop, rcRight, rcBottom);
	graph.DrawImage(m_pImgMask, rcLeft, 0,0, 1, 1, UnitPixel);
	graph.DrawImage(m_pImgMask, rcRight, 0,0, 1, 1, UnitPixel);
	graph.DrawImage(m_pImgMask, rcTop, 0,0, 1, 1, UnitPixel);
	graph.DrawImage(m_pImgMask, rcBottom, 0,0, 1, 1, UnitPixel);

	//画8个拉伸的点
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

	RectF rcOperateBar, rcFinish, rcSave, rcClipboard, rcArrow, rcEllipse, rcRect;
	CalcOperateBarRect(rcArea, rcOperateBar, rcFinish, rcSave, rcClipboard, rcArrow, rcEllipse, rcRect);
	graph.DrawImage(m_pimgOperateBG, rcOperateBar, 0,0, 1, 1, UnitPixel);
	graph.DrawImage(m_pImgFinish, rcFinish, 0,0, 30, 30, UnitPixel);
	graph.DrawImage(m_pImgSave, rcSave, 0,0, 30, 30, UnitPixel);
	graph.DrawImage(m_pImgClipboard, rcClipboard, 0,0, 30, 30, UnitPixel);
	graph.DrawImage(m_pImgArrow, rcArrow, 0,0, 30, 30, UnitPixel);
	graph.DrawImage(m_pImgEllipse, rcEllipse, 0,0, 30, 30, UnitPixel);
	graph.DrawImage(m_pImgRectangle, rcRect, 0,0, 30, 30, UnitPixel);

	BitBlt(hDC,0,0,rect.Width(),rect.Height(),dcCompatible.GetSafeHdc(),0,0,SRCCOPY);
}

void CBackGroundDlg::TrackBottom(CPoint point)
{
	//TODO:	下边拉伸
	CRect rect;
	GetClientRect(&rect);

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
	HBITMAP hBmp = (HBITMAP)m_pBitmap->GetSafeHandle();
	Bitmap bmp(hBmp, NULL);
	graph.DrawImage(&bmp, 0,0,rect.Width(), rect.Height());

	//画灰色的四个区域
	RectF rcLeft, rcRight, rcTop, rcBottom;
	CalcGrayAreaRect(rcArea, rcLeft, rcTop, rcRight, rcBottom);
	graph.DrawImage(m_pImgMask, rcLeft, 0,0, 1, 1, UnitPixel);
	graph.DrawImage(m_pImgMask, rcRight, 0,0, 1, 1, UnitPixel);
	graph.DrawImage(m_pImgMask, rcTop, 0,0, 1, 1, UnitPixel);
	graph.DrawImage(m_pImgMask, rcBottom, 0,0, 1, 1, UnitPixel);

	//画8个拉伸的点
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

	RectF rcOperateBar, rcFinish, rcSave, rcClipboard, rcArrow, rcEllipse, rcRect;
	CalcOperateBarRect(rcArea, rcOperateBar, rcFinish, rcSave, rcClipboard, rcArrow, rcEllipse, rcRect);
	graph.DrawImage(m_pimgOperateBG, rcOperateBar, 0,0, 1, 1, UnitPixel);
	graph.DrawImage(m_pImgFinish, rcFinish, 0,0, 30, 30, UnitPixel);
	graph.DrawImage(m_pImgSave, rcSave, 0,0, 30, 30, UnitPixel);
	graph.DrawImage(m_pImgClipboard, rcClipboard, 0,0, 30, 30, UnitPixel);
	graph.DrawImage(m_pImgArrow, rcArrow, 0,0, 30, 30, UnitPixel);
	graph.DrawImage(m_pImgEllipse, rcEllipse, 0,0, 30, 30, UnitPixel);
	graph.DrawImage(m_pImgRectangle, rcRect, 0,0, 30, 30, UnitPixel);

	BitBlt(hDC,0,0,rect.Width(),rect.Height(),dcCompatible.GetSafeHdc(),0,0,SRCCOPY);
}

void CBackGroundDlg::TrackLeftBottom(CPoint point)
{
	//TODO：左下角拉伸
	CRect rect;
	GetClientRect(&rect);

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
	HBITMAP hBmp = (HBITMAP)m_pBitmap->GetSafeHandle();
	Bitmap bmp(hBmp, NULL);
	graph.DrawImage(&bmp, 0,0,rect.Width(), rect.Height());

	//画灰色的四个区域
	RectF rcLeft, rcRight, rcTop, rcBottom;
	CalcGrayAreaRect(rcArea, rcLeft, rcTop, rcRight, rcBottom);
	graph.DrawImage(m_pImgMask, rcLeft, 0,0, 1, 1, UnitPixel);
	graph.DrawImage(m_pImgMask, rcRight, 0,0, 1, 1, UnitPixel);
	graph.DrawImage(m_pImgMask, rcTop, 0,0, 1, 1, UnitPixel);
	graph.DrawImage(m_pImgMask, rcBottom, 0,0, 1, 1, UnitPixel);

	//画8个拉伸的点
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

	RectF rcOperateBar, rcFinish, rcSave, rcClipboard, rcArrow, rcEllipse, rcRect;
	CalcOperateBarRect(rcArea, rcOperateBar, rcFinish, rcSave, rcClipboard, rcArrow, rcEllipse, rcRect);
	graph.DrawImage(m_pimgOperateBG, rcOperateBar, 0,0, 1, 1, UnitPixel);
	graph.DrawImage(m_pImgFinish, rcFinish, 0,0, 30, 30, UnitPixel);
	graph.DrawImage(m_pImgSave, rcSave, 0,0, 30, 30, UnitPixel);
	graph.DrawImage(m_pImgClipboard, rcClipboard, 0,0, 30, 30, UnitPixel);
	graph.DrawImage(m_pImgArrow, rcArrow, 0,0, 30, 30, UnitPixel);
	graph.DrawImage(m_pImgEllipse, rcEllipse, 0,0, 30, 30, UnitPixel);
	graph.DrawImage(m_pImgRectangle, rcRect, 0,0, 30, 30, UnitPixel);

	BitBlt(hDC,0,0,rect.Width(),rect.Height(),dcCompatible.GetSafeHdc(),0,0,SRCCOPY);
}

void CBackGroundDlg::TrackLeft(CPoint point)
{
	//TODO:	左边拉伸
	CRect rect;
	GetClientRect(&rect);

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
	HBITMAP hBmp = (HBITMAP)m_pBitmap->GetSafeHandle();
	Bitmap bmp(hBmp, NULL);
	graph.DrawImage(&bmp, 0,0,rect.Width(), rect.Height());

	//画灰色的四个区域
	RectF rcLeft, rcRight, rcTop, rcBottom;
	CalcGrayAreaRect(rcArea, rcLeft, rcTop, rcRight, rcBottom);
	graph.DrawImage(m_pImgMask, rcLeft, 0,0, 1, 1, UnitPixel);
	graph.DrawImage(m_pImgMask, rcRight, 0,0, 1, 1, UnitPixel);
	graph.DrawImage(m_pImgMask, rcTop, 0,0, 1, 1, UnitPixel);
	graph.DrawImage(m_pImgMask, rcBottom, 0,0, 1, 1, UnitPixel);

	//画8个拉伸的点
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

	RectF rcOperateBar, rcFinish, rcSave, rcClipboard, rcArrow, rcEllipse, rcRect;
	CalcOperateBarRect(rcArea, rcOperateBar, rcFinish, rcSave, rcClipboard, rcArrow, rcEllipse, rcRect);
	graph.DrawImage(m_pimgOperateBG, rcOperateBar, 0,0, 1, 1, UnitPixel);
	graph.DrawImage(m_pImgFinish, rcFinish, 0,0, 30, 30, UnitPixel);
	graph.DrawImage(m_pImgSave, rcSave, 0,0, 30, 30, UnitPixel);
	graph.DrawImage(m_pImgClipboard, rcClipboard, 0,0, 30, 30, UnitPixel);
	graph.DrawImage(m_pImgArrow, rcArrow, 0,0, 30, 30, UnitPixel);
	graph.DrawImage(m_pImgEllipse, rcEllipse, 0,0, 30, 30, UnitPixel);
	graph.DrawImage(m_pImgRectangle, rcRect, 0,0, 30, 30, UnitPixel);

	BitBlt(hDC,0,0,rect.Width(),rect.Height(),dcCompatible.GetSafeHdc(),0,0,SRCCOPY);
}

void CBackGroundDlg::CalcOperateBarRect(CRect rcSelected, 
									   RectF& rcOperateBar, 
									   RectF& rcFinish, 
									   RectF& rcSave, 
									   RectF& rcClipboard,
									   RectF& rcArrow,
									   RectF& rcEllipse,
									   RectF& rcRect)
{
	if (rcSelected.bottom + 40 >= GetSystemMetrics(SM_CYSCREEN))
	{
		rcOperateBar.X = (REAL)(rcSelected.right - 240);
		rcOperateBar.Y = (REAL)(rcSelected.bottom - 43);
		rcOperateBar.Width = 240;
		rcOperateBar.Height = 40;
	}
	else
	{
		rcOperateBar.X = (REAL)(rcSelected.right - 240);
		rcOperateBar.Y = (REAL)(rcSelected.bottom) + 3;
		rcOperateBar.Width = 240;
		rcOperateBar.Height = 40;
	}

	rcFinish.X = rcOperateBar.X + 200 + 5;
	rcFinish.Y = rcOperateBar.Y + 5;
	rcFinish.Width = 30;
	rcFinish.Height = 30;

	rcSave.X = rcOperateBar.X + 160 + 5;
	rcSave.Y = rcOperateBar.Y + 5;
	rcSave.Width = 30;
	rcSave.Height = 30;

	rcClipboard.X = rcOperateBar.X + 120 + 5;
	rcClipboard.Y = rcOperateBar.Y + 5;
	rcClipboard.Width = 30;
	rcClipboard.Height = 30;

	rcArrow.X = rcOperateBar.X + 80 + 5;
	rcArrow.Y = rcOperateBar.Y + 5;
	rcArrow.Width = 30;
	rcArrow.Height = 30;

	rcEllipse.X = rcOperateBar.X + 40 + 5;
	rcEllipse.Y = rcOperateBar.Y + 5;
	rcEllipse.Width = 30;
	rcEllipse.Height = 30;

	rcRect.X = rcOperateBar.X + 5;
	rcRect.Y = rcOperateBar.Y + 5;
	rcRect.Width = 30;
	rcRect.Height = 30;

	m_rcFinish.left = (LONG)rcFinish.X;
	m_rcFinish.top = (LONG)rcFinish.Y;
	m_rcFinish.right = (LONG)rcFinish.X + (LONG)rcFinish.Width;
	m_rcFinish.bottom = (LONG)rcFinish.Y + (LONG)rcFinish.Height;

	m_rcSave.left = (LONG)rcSave.X;
	m_rcSave.top = (LONG)rcSave.Y;
	m_rcSave.right = (LONG)rcSave.X + (LONG)rcSave.Width;
	m_rcSave.bottom = (LONG)rcSave.Y + (LONG)rcSave.Height;

	m_rcClipboard.left = (LONG)rcClipboard.X;
	m_rcClipboard.top = (LONG)rcClipboard.Y;
	m_rcClipboard.right = (LONG)rcClipboard.X + (LONG)rcClipboard.Width;
	m_rcClipboard.bottom = (LONG)rcClipboard.Y + (LONG)rcClipboard.Height;

	m_rcArrow.left = (LONG)rcArrow.X;
	m_rcArrow.top = (LONG)rcArrow.Y;
	m_rcArrow.right = (LONG)rcArrow.X + (LONG)rcArrow.Width;
	m_rcArrow.bottom = (LONG)rcArrow.Y + (LONG)rcArrow.Height;

	m_rcEllipse.left = (LONG)rcEllipse.X;
	m_rcEllipse.top = (LONG)rcEllipse.Y;
	m_rcEllipse.right = (LONG)rcEllipse.X + (LONG)rcEllipse.Width;
	m_rcEllipse.bottom = (LONG)rcEllipse.Y + (LONG)rcEllipse.Height;

	m_rcRectangle.left = (LONG)rcRect.X;
	m_rcRectangle.top = (LONG)rcRect.Y;
	m_rcRectangle.right = (LONG)rcRect.X + (LONG)rcRect.Width;
	m_rcRectangle.bottom = (LONG)rcRect.Y + (LONG)rcRect.Height;
}

void CBackGroundDlg::CalcGrayAreaRect(CRect rcArea,
									 RectF& rcLeft, 
									 RectF& rcTop, 
									 RectF& rcRight, 
									 RectF& rcBottom)
{
	CRect rect;
	GetClientRect(&rect);

	//左边
	rcLeft.X = 0;
	rcLeft.Y = 0;
	rcLeft.Width = (REAL)rcArea.left;
	rcLeft.Height = (REAL)rect.Height();


	//右边
	rcRight.X = (REAL)rcArea.right;
	rcRight.Y = 0;
	rcRight.Width = (REAL)rect.right - rcArea.right;
	rcRight.Height = (REAL)rect.Height();


	//上边
	rcTop.Y = 0;
	rcTop.X = (REAL)rcArea.left;
	rcTop.Width = (REAL)rcArea.Width();
	rcTop.Height = (REAL)rcArea.top;

	//下边
	rcBottom.X = (REAL)rcArea.left;
	rcBottom.Y = (REAL)rcArea.bottom;
	rcBottom.Width = (REAL)rcArea.Width();
	rcBottom.Height = (REAL)rect.bottom - rcArea.bottom;
}

void CBackGroundDlg::SaveScreenCapture()
{
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
	BuffDC.BitBlt(0,0,
		m_rcSelected.Width(),
		m_rcSelected.Height(),
		&pdc,m_rcSelected.left,m_rcSelected.top,SRCCOPY);
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
}

void CBackGroundDlg::CopyScreenCaptureToClipboard()
{
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

	if (OpenClipboard())
	{
		//清空剪切板
		EmptyClipboard();
		//把屏幕内容粘贴到剪切版上
		SetClipboardData(CF_BITMAP,BuffBmp.m_hObject);
		//关闭剪切版
		CloseClipboard();
	}

	BuffDC.DeleteDC();
	BuffBmp.DeleteObject();
	ReleaseDC(dc);

	EndDialog(IDOK);
}

void CBackGroundDlg::DrawArrow(CPoint point)
{
	CRect rt;
	GetClientRect(&rt);

	CDC* pDC = GetDC();
	CDC	BufferDC;
	BufferDC.CreateCompatibleDC(pDC);
	CBitmap buffBmp;
	buffBmp.CreateCompatibleBitmap(pDC, rt.Width(), rt.Height());
	BufferDC.SelectObject(&buffBmp);

	CDC dc;
	dc.CreateCompatibleDC(&BufferDC);
	dc.SelectObject(m_pBitmap);
	BufferDC.BitBlt(0, 0, rt.Width(), rt.Height(), &dc, 0, 0, SRCCOPY|CAPTUREBLT);
	dc.DeleteDC();

	PointF ptDown, ptRemote;
	ptDown.X = (REAL)m_ptDown.x;
	ptDown.Y = (REAL)m_ptDown.y;

	ptRemote.X = (REAL)point.x;
	ptRemote.Y = (REAL)point.y;

	COLORREF clr(RGB(255, 0,0));

	Gdiplus::Graphics graphics(BufferDC.m_hDC);
	SolidBrush hBrush(Color::Red);
	Pen hPen(&hBrush, 2);
	graphics.SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);  

	LineArrow(&BufferDC, m_ptDown, point, 30, 15, 40, clr, 1);

	pDC->BitBlt(rt.left,
		rt.top, 
		rt.Width(), 
		rt.Height(), 
		&BufferDC, 0,0, SRCCOPY|CAPTUREBLT);
	BufferDC.DeleteDC();
	buffBmp.DeleteObject();
	ReleaseDC(pDC);
}

void CBackGroundDlg::LineArrow(CDC* pDC,CPoint p1, CPoint p2,double theta,double alpha,int length,COLORREF clr,int size)
{
	theta = 3.1415926*theta/180;//转换为弧度
	alpha = 3.1415926*alpha/180;
	double Px,Py,P1x,P1y,P2x,P2y;
	double Pax,Pay,Pbx,Pby;
	//以P2为原点得到向量P2P1（P）
	Px=p1.x-p2.x;
	Py=p1.y-p2.y;
	//向量P旋转theta角得到向量P1
	P1x=Px*cos(theta)-Py*sin(theta);
	P1y=Px*sin(theta)+Py*cos(theta);
	//向量P旋转alpha角得到向量Pa
	Pax=Px*cos(alpha)-Py*sin(alpha);
	Pay=Px*sin(alpha)+Py*cos(alpha);
	//向量P旋转-theta角得到向量P2
	P2x=Px*cos(-theta)-Py*sin(-theta);
	P2y=Px*sin(-theta)+Py*cos(-theta);
	//向量P旋转-alpha角得到向量Pb
	Pbx=Px*cos(-alpha)-Py*sin(-alpha);
	Pby=Px*sin(-alpha)+Py*cos(-alpha);
	//伸缩向量至制定长度
	int len;
	if (size==1)
	{
		length = (int)(length/2);
		len	   = (int)(length-1.5);
	}
	else if (size == 2)
	{
		length = length*2/3+2;
		len = length - 2;
	}
	else
	{
		len = length -3;
	}

	double x1,x2;
	x1=sqrt(P1x*P1x+P1y*P1y);
	P1x=P1x*length/x1;
	P1y=P1y*length/x1;
	x2=sqrt(P2x*P2x+P2y*P2y);
	P2x=P2x*length/x2;
	P2y=P2y*length/x2;

	double xa,xb;
	xa=sqrt(Pax*Pax+Pay*Pay);
	Pax=Pax*(len)/x1;
	Pay=Pay*(len)/x1;
	xb=sqrt(Pbx*Pbx+Pby*Pby);
	Pbx=Pbx*(len)/x2;
	Pby=Pby*(len)/x2;
	//平移变量到直线的末端
	P1x=P1x+p2.x;
	P1y=P1y+p2.y;
	P2x=P2x+p2.x;
	P2y=P2y+p2.y;

	Pax=Pax+p2.x;
	Pay=Pay+p2.y;
	Pbx=Pbx+p2.x;
	Pby=Pby+p2.y;
	CPen pen;
	pen.CreatePen(PS_SOLID, 1, clr);
	CBrush brushRed(clr);
	Gdiplus::Graphics graphics(pDC->m_hDC); 
	SolidBrush GdiBrush(Gdiplus::Color(255,GetRValue(clr),GetGValue(clr),GetBValue(clr)));
	graphics.SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);  
	Point   pts[6];
	pts[0].X = (INT)p1.x;
	pts[0].Y = (INT)p1.y;
	pts[1].X = (INT)Pax;
	pts[1].Y = (INT)Pay;
	pts[2].X = (INT)P1x;
	pts[2].Y = (INT)P1y;
	pts[3].X = (INT)p2.x;
	pts[3].Y = (INT)p2.y;
	pts[4].X = (INT)P2x;
	pts[4].Y = (INT)P2y;
	pts[5].X = (INT)Pbx;
	pts[5].Y = (INT)Pby;
	graphics.FillPolygon(&GdiBrush,pts,6);
}

HBITMAP CBackGroundDlg::CopyClientToBitmap(int x,int y,int cx,int cy,bool bsave)
{
	HDC hSrcDC = ::GetDC(m_hWnd);
	HDC hMemDC = CreateCompatibleDC(hSrcDC);
	HBITMAP hBitmap = CreateCompatibleBitmap(hSrcDC, cx, cy);
	HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);
	//////////////////////////////////////////////////////////////////////////
	if (bsave)
	{
		CDC dcCompatible;
		dcCompatible.CreateCompatibleDC(CDC::FromHandle(hMemDC));
		dcCompatible.SelectObject(m_pBitmap);
		BitBlt(hMemDC,0,0,cx,cy,dcCompatible,x,y,SRCCOPY|CAPTUREBLT);
	}
	else
	{
		BitBlt(hMemDC,0,0,cx,cy,hSrcDC,x,y,SRCCOPY|CAPTUREBLT);
	}
	//得到屏幕位图的句柄
	//hBitmap=(HBITMAP)SelectObject(hMemDC,hOldBitmap);
	//清除
	DeleteDC(hSrcDC);
	DeleteDC(hMemDC);
	//返回位图句柄
	if(bsave)
	{
		if (OpenClipboard())
		{
			//清空剪切板
			EmptyClipboard();
			//把屏幕内容粘贴到剪切版上
			SetClipboardData(CF_BITMAP,hBitmap);
			//关闭剪切版
			CloseClipboard();
		}
	}
	return hBitmap;
}

void CBackGroundDlg::DrawRectangle(CPoint point)
{
	CRect rt;
	GetClientRect(&rt);

	CDC* pDC = GetDC();
	CDC	BufferDC;
	BufferDC.CreateCompatibleDC(pDC);
	CBitmap buffBmp;
	buffBmp.CreateCompatibleBitmap(pDC, rt.Width(), rt.Height());
	BufferDC.SelectObject(&buffBmp);

	CDC dc;
	dc.CreateCompatibleDC(&BufferDC);
	dc.SelectObject(m_pBitmap);
	BufferDC.BitBlt(0, 0, rt.Width(), rt.Height(), &dc, 0, 0, SRCCOPY|CAPTUREBLT);
	dc.DeleteDC();

	PointF ptDown, ptRemote;
	ptDown.X = (REAL)m_ptDown.x;
	ptDown.Y = (REAL)m_ptDown.y;

	ptRemote.X = (REAL)point.x;
	ptRemote.Y = (REAL)point.y;

	COLORREF clr(RGB(255, 0,0));

	Gdiplus::Graphics graphics(BufferDC.m_hDC);
	SolidBrush hBrush(Color::Red);
	Pen hPen(&hBrush, 2);
	graphics.SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);  

	RectF rcRectangle;
	if (point.x >= m_ptDown.x)
	{
		rcRectangle.X = (REAL)m_ptDown.x;
		rcRectangle.Width = (REAL)point.x - (REAL)m_ptDown.x;
	}
	else
	{
		rcRectangle.X = (REAL)point.x;
		rcRectangle.Width = (REAL)m_ptDown.x - (REAL)point.x;
	}

	if (point.y >= m_ptDown.y)
	{
		rcRectangle.Y = (REAL)m_ptDown.y;
		rcRectangle.Height = (REAL)point.y - (REAL)m_ptDown.y;
	}
	else
	{
		rcRectangle.Y = (REAL)point.y;
		rcRectangle.Height = (REAL)m_ptDown.y - (REAL)point.y;
	}

	graphics.DrawRectangle(&hPen, rcRectangle);

	pDC->BitBlt(rt.left,
		rt.top, 
		rt.Width(), 
		rt.Height(), 
		&BufferDC, 0,0, SRCCOPY|CAPTUREBLT);
	BufferDC.DeleteDC();
	buffBmp.DeleteObject();
	ReleaseDC(pDC);
}

void CBackGroundDlg::DrawEllipse(CPoint point)
{
	CRect rt;
	GetClientRect(&rt);

	CDC* pDC = GetDC();
	CDC	BufferDC;
	BufferDC.CreateCompatibleDC(pDC);
	CBitmap buffBmp;
	buffBmp.CreateCompatibleBitmap(pDC, rt.Width(), rt.Height());
	BufferDC.SelectObject(&buffBmp);

	CDC dc;
	dc.CreateCompatibleDC(&BufferDC);
	dc.SelectObject(m_pBitmap);
	BufferDC.BitBlt(0, 0, rt.Width(), rt.Height(), &dc, 0, 0, SRCCOPY|CAPTUREBLT);
	dc.DeleteDC();

	PointF ptDown, ptRemote;
	ptDown.X = (REAL)m_ptDown.x;
	ptDown.Y = (REAL)m_ptDown.y;

	ptRemote.X = (REAL)point.x;
	ptRemote.Y = (REAL)point.y;

	COLORREF clr(RGB(255, 0,0));

	Gdiplus::Graphics graphics(BufferDC.m_hDC);
	SolidBrush hBrush(Color::Red);
	Pen hPen(&hBrush, 2);
	graphics.SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);  

	graphics.DrawEllipse(&hPen, 
		m_ptDown.x,
		m_ptDown.y, 
		point.x - m_ptDown.x, 
		point.y - m_ptDown.y);

	pDC->BitBlt(rt.left,
		rt.top, 
		rt.Width(), 
		rt.Height(), 
		&BufferDC, 0,0, SRCCOPY|CAPTUREBLT);
	BufferDC.DeleteDC();
	buffBmp.DeleteObject();
	ReleaseDC(pDC);
}