// ComicService.cpp : Implementation of WinMain


#include "stdafx.h"
#include "resource.h"
#include "ComicService.h"
#include "ComicServiceObj.h"
//#include <stdio.h>

CComicServiceModule::CComicServiceModule()
{
#ifdef _DEBUG
    m_status.dwControlsAccepted = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_PAUSE_CONTINUE | SERVICE_ACCEPT_SHUTDOWN;
#else
    m_status.dwControlsAccepted = 0; //disable stop in control panel
#endif
}

HRESULT CComicServiceModule::InitializeSecurity() throw()
{
    // TODO : Call CoInitializeSecurity and provide the appropriate security settings for 
    // your service
    // Suggested - PKT Level Authentication, 
    // Impersonation Level of RPC_C_IMP_LEVEL_IDENTIFY 
    // and an appropiate Non NULL Security Descriptor.

    HRESULT hr = E_FAIL;
    COM_VERIFY(CoInitializeSecurity(NULL, -1, NULL, NULL, 
        RPC_C_AUTHN_LEVEL_NONE, RPC_C_IMP_LEVEL_IDENTIFY, NULL, EOAC_NONE, NULL));

    return hr;
}

HRESULT CComicServiceModule::RegisterAppId(bool bService/* = false */) throw()
{
    HRESULT hr = __super::RegisterAppId(bService);
    BOOL bRet = FALSE;
    FTLTRACE(TEXT("RegisterAppId, bService=%d, hr=0x%x\n"), bService, hr);

    if (bService && IsInstalled())
    {
        SC_HANDLE hSCM = ::OpenSCManager(NULL, NULL, SERVICE_CHANGE_CONFIG);
        if (hSCM)
        {
            SC_HANDLE hService = ::OpenService(hSCM, m_szServiceName, SERVICE_CHANGE_CONFIG);
            if (hService)
            {
                API_VERIFY(ChangeServiceConfig(hService, SERVICE_NO_CHANGE, SERVICE_AUTO_START,
                    0, NULL, NULL, NULL, NULL, NULL, NULL, m_szServiceName));

                SERVICE_DESCRIPTION Description = { 0 };
                TCHAR szDescription[1024] = {0};
                StringCchCopy(szDescription, _countof(szDescription), TEXT("Naver Comic Viewer Helper Service"));
                Description.lpDescription = szDescription;

                API_VERIFY(ChangeServiceConfig2(hService, SERVICE_CONFIG_DESCRIPTION, &Description));

                API_VERIFY(CloseServiceHandle(hService));
            }
            else
            {
                hr = AtlHresultFromLastError();
            }
            API_VERIFY(CloseServiceHandle(hSCM));
        }
        else
        {
            hr = AtlHresultFromLastError();
        }
    }
    COM_VERIFY(hr);

    return hr;
}

HRESULT CComicServiceModule::PreMessageLoop(int nShowCmd) throw()
{
    HRESULT hr = S_OK;
    COM_VERIFY(__super::PreMessageLoop(nShowCmd));

    FTLTRACE(TEXT("PreMessageLoop, nShowCmd=%d, hr=0x%x\n"), nShowCmd, hr);
    return hr;
}

void CComicServiceModule::RunMessageLoop() throw()
{
    //CComObject<CComicServiceObj> *pComicServiceObj;
    //CComObject<CComicServiceObj>::CreateInstance(&pComicServiceObj);
    //pComicServiceObj->AddRef();
    __super::RunMessageLoop();
    //pComicServiceObj->Release();
}

HRESULT CComicServiceModule::PostMessageLoop() throw()
{
    HRESULT hr = S_OK;
    COM_VERIFY(__super::PostMessageLoop());

    FTLTRACE(TEXT("PostMessageLoop, hr=0x%x\n"), hr);

    return hr;
}

void CComicServiceModule::OnPause() throw()
{
    OutputDebugString(TEXT("On Comic Service Pause\n"));
    SetServiceStatus(SERVICE_PAUSED);
    __super::OnPause();
}
void CComicServiceModule::OnContinue() throw()
{
    OutputDebugString(TEXT("On Comic Service Continue\n"));
    SetServiceStatus(SERVICE_RUNNING);
    __super::OnContinue();
}

void CComicServiceModule::OnUnknownRequest(DWORD dwOpcode) throw()
{
    switch (dwOpcode)
    {
    case COMIC_SERVICE_STOP:
        //OutputDebugString(TEXT("In OnUnknownRequest for COMIC_SERVICE_STOP\n"));
        SetServiceStatus(SERVICE_STOP_PENDING);
        ::PostThreadMessage(m_dwThreadID, WM_QUIT, 0, 0);
        break;
    default:
        __super::OnUnknownRequest(dwOpcode);
        break;
    }
}


void CComicServiceModule::OnStop() throw()
{
    OutputDebugString(TEXT("On Comic Service Stop\n"));
    __super::OnStop();
}


CComicServiceModule _AtlModule;

//
extern "C" int WINAPI _tWinMain(HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/, 
                                LPTSTR /*lpCmdLine*/, int nShowCmd)
{
    int nRet = 0;
    nRet = _AtlModule.WinMain(nShowCmd);
    return nRet;
}
