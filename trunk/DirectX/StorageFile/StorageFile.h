// StorageFile.h : StorageFile Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h�� 
#endif

#include "resource.h"       // ������


// CStorageFileApp:
// �йش����ʵ�֣������ StorageFile.cpp
//

class CStorageFileApp : public CWinApp
{
public:
	CStorageFileApp();


// ��д
public:
	virtual BOOL InitInstance();

// ʵ��
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CStorageFileApp theApp;
