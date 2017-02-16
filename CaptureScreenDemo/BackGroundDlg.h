#pragma once


// CBackGroundDlg 对话框

enum DrawShape
{
	EN_DRAW_NULL,
	EN_DRAW_ARROW,
	EN_DRAW_ELLIPSE,
	EN_DRAW_RECT,
};

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
	CBitmap*	m_pTempBitmap;
	Image*		m_pImgMask;
	Image*		m_pImgDot;

	Image*		m_pImgSave;
	Image*		m_pImgSaveGray;
	Image*		m_pImgFinish;
	Image*		m_pImgFinishGray;
	Image*		m_pImgClipboard;
	Image*		m_pImgClipboardGray;
	Image*		m_pImgArrow;
	Image*		m_pImgRectangle;
	Image*		m_pImgEllipse;
	Image*		m_pimgOperateBG;

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

	bool		m_bDrawOperate;

	CRect		m_rcSelected;
	CRect		m_rcRemote;

	CRect		m_rcSave, m_rcFinish, m_rcClipboard, m_rcArrow, m_rcRectangle, m_rcEllipse;

	HCURSOR		m_hCurSelect;
	HCURSOR		m_hCurLeft, 
		m_hCurRight, 
		m_hCurTop, 
		m_hCurBottom, 
		m_hCurLeftTop, 
		m_hCurRightTop, 
		m_hCurLeftBottom, 
		m_hCurRightBottom,
		m_hCurHand,
		m_hCurArrow;

	RectF		m_rcLeft, m_rcRight, m_rcTop, m_rcBottom;

	DrawShape   m_drawState;
public:
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);

	void	DrawCaptureFrame();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);

private:
	void MoveAll(CPoint point);
	void TrackLeftTop(CPoint point);
	void TrackTop(CPoint point);
	void TrackRightTop(CPoint point);
	void TrackRight(CPoint point);
	void TrackRightBottom(CPoint point);
	void TrackBottom(CPoint point);
	void TrackLeftBottom(CPoint point);
	void TrackLeft(CPoint point);

	void CalcOperateBarRect(CRect rcSelected, 
		RectF& rcOperateBar,
		RectF& rcFinish,
		RectF& rcSave,
		RectF& rcClipboard,
		RectF& rcArrow,
		RectF& rcEllipse,
		RectF& rcRect);

	void CalcGrayAreaRect(CRect rcArea,
		RectF& rcLeft,
		RectF& rcTop,
		RectF& rcRight,
		RectF& rcBottom);

	void SaveScreenCapture();
	void CopyScreenCaptureToClipboard();

	void DrawArrow(CPoint point);
	void LineArrow(CDC* pDC,CPoint p1, CPoint p2,double theta,double alpha,int length,COLORREF clr,int size);


	HBITMAP CopyClientToBitmap(int x,int y,int cx,int cy,bool bsave=false);
protected:
	HBITMAP CopyDCToBitmap(LPRECT lpRect,BOOL bSave =FALSE );
};
