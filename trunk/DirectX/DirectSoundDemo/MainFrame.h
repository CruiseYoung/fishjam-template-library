#pragma once

#include <dsound.h>

class CMainFrame : public CFrameWnd
{
    DECLARE_DYNCREATE(CMainFrame)
public:
    CMainFrame(void);
    ~CMainFrame(void);
    DECLARE_MESSAGE_MAP()
public:
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnDestroy();
public:
private:
    IDirectSound*           m_pdSound;
	IDirectSoundBuffer*		m_pdSoundPrimaryBuffer; //������
    IDirectSoundBuffer*     m_pdSoundBuffer;  //��������
    IDirectSoundNotify*     m_pdSoundNotify;
    IDirectSoundCapture*    m_pdSoundCapture;
};
