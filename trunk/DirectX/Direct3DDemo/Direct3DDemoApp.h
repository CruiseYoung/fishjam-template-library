#pragma once

#ifndef __AFXWIN_H__
#  error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h��
#endif

#include "MainFrame.h"

//#include "resource.h"		// ������

class CDirect3DDemoApp : public CWinApp
{
public:
    CDirect3DDemoApp(void);
    ~CDirect3DDemoApp(void);

    // ��д
public:
    virtual BOOL InitInstance();
    // ʵ��
    DECLARE_MESSAGE_MAP()
private:
};

extern CDirect3DDemoApp theApp;
