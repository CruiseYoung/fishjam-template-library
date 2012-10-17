#include "Stdafx.h"
#include "DirectSoundDemoApp.h"
#include "MainFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CtestDDrawApp

BEGIN_MESSAGE_MAP(CDirectSoundDemoApp, CWinApp)
    ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()


// DirectSoundDemoApp ����

CDirectSoundDemoApp::CDirectSoundDemoApp()
{
    // TODO: �ڴ˴���ӹ�����룬
    // ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}

CDirectSoundDemoApp::~CDirectSoundDemoApp()
{

}
// Ψһ��һ�� CTimeSaverApp ����

CDirectSoundDemoApp theApp;


// CTimeSaverApp ��ʼ��

BOOL CDirectSoundDemoApp::InitInstance()
{
    // ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
    // ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
    //����Ҫ InitCommonControls()�����򣬽��޷��������ڡ�
    InitCommonControls();

    CWinApp::InitInstance();

    AfxEnableControlContainer();
    //SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));

    CMainFrame *pMainFrame = new CMainFrame();
    BOOL bRet = FALSE;
    API_VERIFY(pMainFrame->Create(NULL,TEXT("DirectSoundDemoApp")));
    if(FALSE == bRet)
    {
        delete pMainFrame;
        return FALSE;
    }

    m_pMainWnd = pMainFrame;
    m_pMainWnd->ShowWindow(SW_SHOW);
    m_pMainWnd->UpdateWindow();

    return TRUE;
}
