#pragma once

#ifndef __AFXWIN_H__
#  error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h��
#endif

#include "MainFrame.h"


class CDirectSoundDemoApp : public CWinApp
{
public:
    CDirectSoundDemoApp(void);
    ~CDirectSoundDemoApp(void);

    // ��д
public:
    virtual BOOL InitInstance();
    // ʵ��
    DECLARE_MESSAGE_MAP()
private:
};

extern CDirectSoundDemoApp theApp;