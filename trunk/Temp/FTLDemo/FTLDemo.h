// FTLDemo.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CFTLDemoApp:
// �йش����ʵ�֣������ FTLDemo.cpp
//

class CFTLDemoApp : public CWinApp
{
public:
	CFTLDemoApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CFTLDemoApp theApp;

CString GetCurrentTimeString();