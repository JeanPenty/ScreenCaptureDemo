#pragma once


// CBackGroundDlg 对话框

enum DrawShape			//形状枚举
{
	EN_DRAW_NULL,		//未选中
	EN_DRAW_ARROW,		//箭头
	EN_DRAW_ELLIPSE,	//椭圆
	EN_DRAW_RECT,		//矩形
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
	bool		m_bDraw;			//鼠标按下
	bool		m_bFirstDraw;		//第一次绘制，选中截取区域
	bool		m_bMoveAll;			//整体移动

	bool		m_bLeftTopMove;		//左上角拖拽拉伸
	bool		m_bTopMove;			//上边拖拽拉伸
	bool		m_bRightTopMove;	//右上角拖拽拉伸
	bool		m_bRightMove;		//右边拖拽拉伸
	bool		m_bRightBottomMove;	//右下角拖拽拉伸
	bool		m_bBottomMove;		//下边拖拽拉伸
	bool		m_bLeftBottomMove;	//左下角拖拽拉伸
	bool		m_bLeftMove;		//左边拖拽拉伸

	bool		m_bDrawOperate;		//绘制矩形、椭圆、箭头或者其他

	CRect		m_rcSelected;		//截取的区域
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

	RectF		m_rcLeft, m_rcRight, m_rcTop, m_rcBottom;		//4块灰色区域

	DrawShape   m_drawState;									//涂鸦类型

	CToolTipCtrl m_toolTip;

	bool		m_bShowTip;
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

	void DrawRectangle(CPoint point);
	void DrawEllipse(CPoint point);
	HBITMAP CopyClientToBitmap(int x,int y,int cx,int cy,bool bsave=false);
protected:
	HBITMAP CopyDCToBitmap(LPRECT lpRect,BOOL bSave =FALSE );
};
