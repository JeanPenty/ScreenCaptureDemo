
// CaptureScreenDemo.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CCaptureScreenDemoApp:
// �йش����ʵ�֣������ CaptureScreenDemo.cpp
//

class CCaptureScreenDemoApp : public CWinAppEx
{
public:
	CCaptureScreenDemoApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();

private:
	ULONG_PTR m_gidplusToken;
};

extern CCaptureScreenDemoApp theApp;