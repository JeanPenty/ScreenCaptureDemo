
// CaptureScreenDemoDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "CaptureScreenDemo.h"
#include "CaptureScreenDemoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


HHOOK g_Keyboard_Hook = 0;
HWND  g_hWnd;

/*
@	全局键盘钩子回调函数
*/
LRESULT CALLBACK Keyboard_Proc (int code, WPARAM wParam, LPARAM lParam)
{
	PKBDLLHOOKSTRUCT p = (PKBDLLHOOKSTRUCT)lParam;
	if (wParam == WM_KEYDOWN && p->vkCode == 'A')
	{
		if (GetKeyState(VK_SHIFT) < 0 && GetKeyState(VK_CONTROL) < 0)
		{
			PostMessage(g_hWnd, WM_CAPTURE_SCREEN, 0,0);
			return 1;
		}
	}

	return CallNextHookEx (g_Keyboard_Hook, code, wParam, lParam);

};
// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CCaptureScreenDemoDlg 对话框




CCaptureScreenDemoDlg::CCaptureScreenDemoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCaptureScreenDemoDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_pdlgBkgnd = NULL;
	m_pBitmap = NULL;
} 

CCaptureScreenDemoDlg::~CCaptureScreenDemoDlg(void)
{
	if (m_pdlgBkgnd)
	{
		m_pdlgBkgnd->DestroyWindow();
		m_pdlgBkgnd = NULL;
	}

	//卸载钩子
	UnhookWindowsHookEx (g_Keyboard_Hook);
}

void CCaptureScreenDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CCaptureScreenDemoDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_CAPTURE, &CCaptureScreenDemoDlg::OnBnClickedBtnCapture)
	ON_MESSAGE(WM_CAPTURE_SCREEN, OnRecvCaptureScreen)
END_MESSAGE_MAP()


// CCaptureScreenDemoDlg 消息处理程序

BOOL CCaptureScreenDemoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	ModifyStyleEx(WS_EX_APPWINDOW,WS_EX_TOOLWINDOW);
	SetWindowPos(NULL,0,0,0,0,NULL);
	ShowWindow(SW_HIDE);	

	g_hWnd = this->m_hWnd;

	//装载钩子
	g_Keyboard_Hook = 
		SetWindowsHookEx(WH_KEYBOARD_LL,
		&Keyboard_Proc,
		0,
		0);
	if (g_Keyboard_Hook == NULL)
	{
		MessageBox(L"SetWindowsHookEx failed!");
	}

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CCaptureScreenDemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CCaptureScreenDemoDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CCaptureScreenDemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CCaptureScreenDemoDlg::OnBnClickedBtnCapture()
{
	// TODO: 在此添加控件通知处理程序代码
	CRect rect(0, 0,GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
	m_pBitmap = CBitmap::FromHandle(CopyScreenToBitmap(&rect));

	m_pdlgBkgnd = new CBackGroundDlg(GetDesktopWindow(), m_pBitmap);
	m_pdlgBkgnd->DoModal();
}

//拷贝屏幕到位图中
HBITMAP CCaptureScreenDemoDlg::CopyScreenToBitmap(LPRECT lpRect,BOOL bSave)
//lpRect 代表选定区域
{
	HDC       hScrDC, hMemDC;      
	// 屏幕和内存设备描述表
	HBITMAP    hBitmap, hOldBitmap;   
	// 位图句柄
	int       nX, nY, nX2, nY2;      
	// 选定区域坐标
	int       nWidth, nHeight;
	
	// 确保选定区域不为空矩形
	if (IsRectEmpty(lpRect))
		return NULL;
	//为屏幕创建设备描述表
	hScrDC = CreateDC(L"DISPLAY", NULL, NULL, NULL);

	//为屏幕设备描述表创建兼容的内存设备描述表
	hMemDC = CreateCompatibleDC(hScrDC);
	// 获得选定区域坐标
	nX = lpRect->left;
	nY = lpRect->top;
	nX2 = lpRect->right;
	nY2 = lpRect->bottom;

	//确保选定区域是可见的
	if (nX < 0)
		nX = 0;
	if (nY < 0)
		nY = 0;
	if (nX2 > GetSystemMetrics(SM_CXSCREEN))
		nX2 = GetSystemMetrics(SM_CXSCREEN);
	if (nY2 > GetSystemMetrics(SM_CYSCREEN))
		nY2 = GetSystemMetrics(SM_CYSCREEN);
	nWidth = nX2 - nX;
	nHeight = nY2 - nY;
	// 创建一个与屏幕设备描述表兼容的位图
	hBitmap = CreateCompatibleBitmap(hScrDC, nWidth, nHeight);
	// 把新位图选到内存设备描述表中
	hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);
	// 把屏幕设备描述表拷贝到内存设备描述表中
	if(bSave)
	{
		CDC dcCompatible;
		dcCompatible.CreateCompatibleDC(CDC::FromHandle(hMemDC));
		dcCompatible.SelectObject(m_pBitmap);
        
		BitBlt(hMemDC, 0, 0, nWidth, nHeight,
			dcCompatible, nX, nY, SRCCOPY);
	}
	else
	{
		BitBlt(hMemDC, 0, 0, nWidth, nHeight,
			hScrDC, nX, nY, SRCCOPY);
	}

	hBitmap = (HBITMAP)SelectObject(hMemDC, hOldBitmap);
	
	
	//得到屏幕位图的句柄
	//*************
	CDC *pMemDc = CDC::FromHandle(hMemDC);

		//m_pngMask.DrawImage(pMemDc,0,0,m_xScreen,m_yScreen,0,0,8,8);
		int x=nX;
		int y=nY;
		int w=nWidth;
		int h=nHeight;

	//清除 
	DeleteDC(hScrDC);
	DeleteDC(hMemDC);
	// 返回位图句柄
	if(bSave)
	{				
		if (OpenClipboard()) 
		{
        //清空剪贴板
        EmptyClipboard();
        //把屏幕内容粘贴到剪贴板上,
        //hBitmap 为刚才的屏幕位图句柄
        SetClipboardData(CF_BITMAP, hBitmap);
        //关闭剪贴板
        CloseClipboard();
      }
	}
	return hBitmap;
}

LRESULT CCaptureScreenDemoDlg::OnRecvCaptureScreen(WPARAM wParam, LPARAM lParam)
{
	OnBnClickedBtnCapture();
	return 0;
}