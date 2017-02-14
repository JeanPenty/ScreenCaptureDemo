
// CaptureScreenDemoDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CaptureScreenDemo.h"
#include "CaptureScreenDemoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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

	m_pImgMask = 
		Gdiplus::Image::FromFile(L"E:\\Projects\\CaptureScreenDemo\\CaptureScreenDemo\\SC_MASK.png");

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

// 	m_pngMask.DrawImage(pMemDc,0,0,nWidth,y,0,0,8,8);
// 	m_pngMask.DrawImage(pMemDc,0,y+h,nWidth,nHeight-y-h,0,0,8,8);
// 	m_pngMask.DrawImage(pMemDc,0,y,x,h,0,0,8,8);
// 	m_pngMask.DrawImage(pMemDc,x+w,y,nWidth-x-w,h,0,0,8,8);
// 
// 	pMemDc->Draw3dRect(x,y,w,h,RGB(0,0,0),RGB(0,0,0));
// 	//////////////////////////////////////////////////////////////////////////
// 	CRect rc; rc.left=x+2;rc.top=y-24;rc.right=x+268;rc.bottom=y-2;
// 	if(rc.top<0){rc.top=y+2;rc.bottom=y+24;}
// 	m_pngMask.DrawImage(pMemDc,rc.left,rc.top,rc.Width(),rc.Height(),0,0,8,8);
// 	
// 	//////////////////////////////////////////////////////////////////////////
// 	m_rcAction[0].left=x+w-232;m_rcAction[0].top=y+h+2;
// 	m_rcAction[0].right=m_rcAction[0].left+130;m_rcAction[0].bottom=m_rcAction[1].top+26;
// 	m_pngAction.DrawImage(pMemDc,m_rcAction[0].left,m_rcAction[0].top,130,26,0,(m_iActionMask & 0x00F)*26,130,26);
// 
// 	m_rcAction[1].left=x+w-102;m_rcAction[1].top=y+h+2;
// 	m_rcAction[1].right=m_rcAction[1].left+51;m_rcAction[1].bottom=m_rcAction[1].top+26;
// 	m_pngAction.DrawImage(pMemDc,m_rcAction[1].left,m_rcAction[1].top,51,26,130,((m_iActionMask & 0x0F0)>4)*26,51,26);
// 
// 	m_rcAction[2].left=x+w-51;m_rcAction[2].top=y+h+2;
// 	m_rcAction[2].right=m_rcAction[2].left+51;m_rcAction[2].bottom=m_rcAction[2].top+26;
// 	m_pngAction.DrawImage(pMemDc,m_rcAction[2].left,m_rcAction[2].top,51,26,181,((m_iActionMask & 0xF00)>8)*26,51,26);
// 	//////////////////////////////////////////////////////////////////////////
// 	x=m_rcSel[0].left;
// 	y=m_rcSel[0].top;
// 	w=m_rcSel[0].Width();
// 	h=m_rcSel[0].Height();
// 
// 
// 	m_pngDot.DrawImage(pMemDc,x-2,y-2);
// 	m_rcSel[1].left=x-2;m_rcSel[1].top=y-2;
// 
// 	m_pngDot.DrawImage(pMemDc,x+w/2-2,y-2);
// 	m_rcSel[2].left=x+w/2-2;m_rcSel[2].top=y-2;
// 
// 	m_pngDot.DrawImage(pMemDc,x+w-3,y-2);
// 	m_rcSel[3].left=x+w-3;m_rcSel[3].top=y-2;
// 
// 	m_pngDot.DrawImage(pMemDc,x-2,y+h/2-2);
// 	m_rcSel[4].left=x-2;m_rcSel[4].top=y+h/2-2;
// 
// 	m_pngDot.DrawImage(pMemDc,x+w-3,y+h/2-2);
// 	m_rcSel[5].left=x+w-3;m_rcSel[5].top=y+h/2-2;
// 
// 	m_pngDot.DrawImage(pMemDc,x-2,y+h-3);
// 	m_rcSel[6].left=x-2;m_rcSel[6].top=y+h-3;
// 
// 	m_pngDot.DrawImage(pMemDc,x+w/2-2,y+h-3);
// 	m_rcSel[7].left=x+w/2-2;m_rcSel[7].top=y+h-3;
// 
// 	m_pngDot.DrawImage(pMemDc,x+w-3,y+h-3);
// 	m_rcSel[8].left=x+w-3;m_rcSel[8].top=y+h-3;
// 
// 	for(int i=1;i<9;i++)
// 	{
// 	m_rcSel[i].right=m_rcSel[i].left+5;
// 	m_rcSel[i].bottom=m_rcSel[i].top+5;
// 	}

	//////////////////////////////////////////////////////////////////////////
// 	//�滭����
// 	CDC * pDC = CDC::FromHandle(hScrDC);
// 	pDC->BitBlt(0,0,nWidth,nHeight,pMemDc,0,0,SRCCOPY);
// 	//////////////////////////////////////////////////////////////////////////
	///

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
