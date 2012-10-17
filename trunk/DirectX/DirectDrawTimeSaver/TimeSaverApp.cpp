#include "Stdafx.h"
#include "TimeSaverApp.h"
#include "MainFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CtestDDrawApp

BEGIN_MESSAGE_MAP(CTimeSaverApp, CWinApp)
    ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()


// CTimeSaverApp ����

CTimeSaverApp::CTimeSaverApp()
{
    // TODO: �ڴ˴���ӹ�����룬
    // ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}

CTimeSaverApp::~CTimeSaverApp()
{

}
// Ψһ��һ�� CTimeSaverApp ����

CTimeSaverApp theApp;


// CTimeSaverApp ��ʼ��

BOOL CTimeSaverApp::InitInstance()
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
    API_VERIFY(pMainFrame->Create(NULL,TEXT("TimerSaver")));
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
