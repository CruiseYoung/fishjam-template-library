#pragma once

#ifndef __AFXWIN_H__
#  error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h��
#endif

#include "MainFrame.h"

//#include "resource.h"		// ������

class CTimeSaverApp : public CWinApp
{
public:
    CTimeSaverApp(void);
    ~CTimeSaverApp(void);

    // ��д
public:
    virtual BOOL InitInstance();
    // ʵ��
    DECLARE_MESSAGE_MAP()
private:
};

extern CTimeSaverApp theApp;
