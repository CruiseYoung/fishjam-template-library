#pragma once

#define FULLSCREEN_RUNNING  //��ȫ��ģʽ�봰��ģʽ���л�

#include <ddraw.h>

class CMainFrame : public CFrameWnd
{
    DECLARE_DYNCREATE(CMainFrame)
public:
    CMainFrame(void);
    ~CMainFrame(void);
    DECLARE_MESSAGE_MAP()
public:
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
private:
    UINT_PTR                m_TimerID;
    IDirectDraw7*           m_pDDraw;
    IDirectDrawSurface7*    m_pDDSPrimary;  //��ҳ��
#ifdef FULLSCREEN_RUNNING
    IDirectDrawSurface7*    m_pDDSBack;     //��̨����ҳ��
#endif
    IDirectDrawSurface7*    m_pDDSBackground; //����ҳ��

public:
    afx_msg void OnTimer(UINT nIDEvent);
    afx_msg void OnDestroy();
    afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};
