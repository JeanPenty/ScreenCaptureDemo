#pragma once


// CBackGroundDlg 对话框

class CBackGroundDlg : public CDialog
{
	DECLARE_DYNAMIC(CBackGroundDlg)

public:
	CBackGroundDlg(CWnd* pParent = NULL, CBitmap* pbmp = NULL);   // 标准构造函数
	virtual ~CBackGroundDlg();

// 对话框数据
	enum { IDD = IDD_BACKGROUNDDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

private:
	int m_cx;
	int m_cy;
	CBitmap*	m_pBitmap;
	Image*		m_pImgMask;
	Image*		m_pImgDot;

	CPoint		m_ptDown;
	bool		m_bDraw;
	bool		m_bFirstDraw;
	bool		m_bMoveAll;

	bool		m_bLeftTopMove;
	bool		m_bTopMove;
	bool		m_bRightTopMove;
	bool		m_bRightMove;
	bool		m_bRightBottomMove;
	bool		m_bBottomMove;
	bool		m_bLeftBottomMove;
	bool		m_bLeftMove;

	CRect		m_rcSelected;
	CRect		m_rcRemote;

	HCURSOR		m_hCurSelect;
	HCURSOR		m_hCurLeft, 
		m_hCurRight, 
		m_hCurTop, 
		m_hCurBottom, 
		m_hCurLeftTop, 
		m_hCurRightTop, 
		m_hCurLeftBottom, 
		m_hCurRightBottom;

	RectF		m_rcLeft, m_rcRight, m_rcTop, m_rcBottom;
public:
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);

	void	DrawCaptureFrame();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
};
