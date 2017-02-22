
// CaptureScreenDemoDlg.h : ͷ�ļ�
//

#pragma once

#include "BackGroundDlg.h"

// CCaptureScreenDemoDlg �Ի���
class CCaptureScreenDemoDlg : public CDialog
{
// ����
public:
	CCaptureScreenDemoDlg(CWnd* pParent = NULL);	// ��׼���캯��
	~CCaptureScreenDemoDlg(void);

// �Ի�������
	enum { IDD = IDD_CAPTURESCREENDEMO_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnCapture();
	LRESULT OnRecvCaptureScreen(WPARAM wParam, LPARAM lParam);

private:
	CBackGroundDlg*		m_pdlgBkgnd;
	CBitmap*			m_pBitmap;
	Image*				m_pImgMask;
	HBITMAP CopyScreenToBitmap(LPRECT lpRect,BOOL bSave =FALSE );
};
