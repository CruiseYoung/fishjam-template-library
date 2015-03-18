// FTLDemo.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "FTLDemo.h"
#include "FTLDemoSheet.h"
#include <ftlCrashHandler.h>
#include <ftlGdi.h>
#include <ftlgdiplus.h>
#include <ftlSocket.h>
//#include "vld.h"
#include <ftlfile.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CFTLDemoApp

BEGIN_MESSAGE_MAP(CFTLDemoApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CFTLDemoApp ����

CFTLDemoApp::CFTLDemoApp()
{
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CFTLDemoApp ����

CFTLDemoApp theApp;
//CComModule _Module;     //Ϊ��ʹ��CFResourcelessDlg

// CFTLDemoApp ��ʼ��

BOOL CFTLDemoApp::InitInstance()
{
    //FUNCTION_BLOCK_INIT();
    BOOL bRet = FALSE;
    CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
    {
        //VLDEnable();
        //FAST_TRACE_OPTIONS traceOptions;
        //traceOptions.traceThreshold = tlTrace;
        //traceOptions.bWriteToFile = TRUE;
        //FTL::CFFastTrace::GetInstance().SetTraceOptions(&traceOptions);

        // ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
        // ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
        //����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
        INITCOMMONCONTROLSEX InitCtrls;
        InitCtrls.dwSize = sizeof(InitCtrls);
        // ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
        // �����ؼ��ࡣ
        InitCtrls.dwICC = ICC_WIN95_CLASSES;
        InitCommonControlsEx(&InitCtrls);

        CWinApp::InitInstance();

        AfxEnableControlContainer();

        //FTL::CFFileUtil::DumpAllAPISystemFolderPath();

        // ��׼��ʼ��
        // ���δʹ����Щ���ܲ�ϣ����С
        // ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
        // ����Ҫ���ض���ʼ������
        // �������ڴ洢���õ�ע�����
        SetRegistryKey(_T("Fishjam"));
        API_VERIFY(FTL::CFGdiUtil::SetProcessDPIAdware());
        FTLTRACE(TEXT("SetProcessDPIAdware:%d\n"), bRet);

		FTL::CFCrashHandler crashHandler;
        crashHandler.SetDefaultCrashHandlerFilter();

        FTL::CFAutoGdiplus   gdiPlusInitObject;
        FTL::CFWinsockEnvInit winsockInit;

        //CFTLDemoDlg dlg;
        //m_pMainWnd = &dlg;
        //INT_PTR nResponse = dlg.DoModal();
        CFTLDemoSheet sheet(TEXT("FTL Demo"));
        INT_PTR nResponse  = sheet.DoModal();
        if (nResponse == IDOK)
        {
        }
        else if (nResponse == IDCANCEL)
        {
        }

        // ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
        //  ����������Ӧ�ó������Ϣ�á�
    }
    CoUninitialize();
    //FUNCTION_BLOCK_UNINIT();
    return FALSE;
}

CString GetCurrentTimeString()
{
    SYSTEMTIME st = {0};
    GetLocalTime(&st);
    CString strTime;
    strTime.Format(TEXT("%02d:%02d:%02d:%03d"),st.wHour,st.wMinute,st.wSecond,st.wMilliseconds);
    return strTime;
}