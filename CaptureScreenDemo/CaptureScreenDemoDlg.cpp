
// CaptureScreenDemoDlg.cpp : ʵ���ļ�
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
@	ȫ�ּ��̹��ӻص�����
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
// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CCaptureScreenDemoDlg �Ի���




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

	//ж�ع���
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


// CCaptureScreenDemoDlg ��Ϣ�������

BOOL CCaptureScreenDemoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	ModifyStyleEx(WS_EX_APPWINDOW,WS_EX_TOOLWINDOW);
	SetWindowPos(NULL,0,0,0,0,NULL);
	ShowWindow(SW_HIDE);	

	g_hWnd = this->m_hWnd;

	//װ�ع���
	g_Keyboard_Hook = 
		SetWindowsHookEx(WH_KEYBOARD_LL,
		&Keyboard_Proc,
		0,
		0);
	if (g_Keyboard_Hook == NULL)
	{
		MessageBox(L"SetWindowsHookEx failed!");
	}

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CCaptureScreenDemoDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CCaptureScreenDemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CCaptureScreenDemoDlg::OnBnClickedBtnCapture()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CRect rect(0, 0,GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
	m_pBitmap = CBitmap::FromHandle(CopyScreenToBitmap(&rect));

	m_pdlgBkgnd = new CBackGroundDlg(GetDesktopWindow(), m_pBitmap);
	m_pdlgBkgnd->DoModal();
}

//������Ļ��λͼ��
HBITMAP CCaptureScreenDemoDlg::CopyScreenToBitmap(LPRECT lpRect,BOOL bSave)
//lpRect ����ѡ������
{
	HDC       hScrDC, hMemDC;      
	// ��Ļ���ڴ��豸������
	HBITMAP    hBitmap, hOldBitmap;   
	// λͼ���
	int       nX, nY, nX2, nY2;      
	// ѡ����������
	int       nWidth, nHeight;
	
	// ȷ��ѡ������Ϊ�վ���
	if (IsRectEmpty(lpRect))
		return NULL;
	//Ϊ��Ļ�����豸������
	hScrDC = CreateDC(L"DISPLAY", NULL, NULL, NULL);

	//Ϊ��Ļ�豸�����������ݵ��ڴ��豸������
	hMemDC = CreateCompatibleDC(hScrDC);
	// ���ѡ����������
	nX = lpRect->left;
	nY = lpRect->top;
	nX2 = lpRect->right;
	nY2 = lpRect->bottom;

	//ȷ��ѡ�������ǿɼ���
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
	// ����һ������Ļ�豸��������ݵ�λͼ
	hBitmap = CreateCompatibleBitmap(hScrDC, nWidth, nHeight);
	// ����λͼѡ���ڴ��豸��������
	hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);
	// ����Ļ�豸�����������ڴ��豸��������
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
	
	
	//�õ���Ļλͼ�ľ��
	//*************
	CDC *pMemDc = CDC::FromHandle(hMemDC);

		//m_pngMask.DrawImage(pMemDc,0,0,m_xScreen,m_yScreen,0,0,8,8);
		int x=nX;
		int y=nY;
		int w=nWidth;
		int h=nHeight;

	//��� 
	DeleteDC(hScrDC);
	DeleteDC(hMemDC);
	// ����λͼ���
	if(bSave)
	{				
		if (OpenClipboard()) 
		{
        //��ռ�����
        EmptyClipboard();
        //����Ļ����ճ������������,
        //hBitmap Ϊ�ղŵ���Ļλͼ���
        SetClipboardData(CF_BITMAP, hBitmap);
        //�رռ�����
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