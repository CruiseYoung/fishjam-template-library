#include "Stdafx.h"
#include "Direct3DDemoApp.h"
#include "MainFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDirect3DDemoApp

BEGIN_MESSAGE_MAP(CDirect3DDemoApp, CWinApp)
    ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()


// CDirect3DDemoApp ����

CDirect3DDemoApp::CDirect3DDemoApp()
{
    // TODO: �ڴ˴���ӹ�����룬
    // ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}

CDirect3DDemoApp::~CDirect3DDemoApp()
{

}
// Ψһ��һ�� CtestDDrawApp ����

CDirect3DDemoApp theApp;


// CtestDDrawApp ��ʼ��

BOOL CDirect3DDemoApp::InitInstance()
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
    API_VERIFY(pMainFrame->Create(NULL,TEXT("Direct3D Demo")));
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
