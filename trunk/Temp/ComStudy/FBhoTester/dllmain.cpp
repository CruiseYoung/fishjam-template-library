// dllmain.cpp : Implementation of DllMain.

#include "stdafx.h"
#include "resource.h"
#include "FBhoTester_i.h"
#include "dllmain.h"
#include "dlldatax.h"

CFBhoTesterModule _AtlModule;

// DLL Entry Point
extern "C" BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
    if (DLL_PROCESS_ATTACH == dwReason)
    {
        //��ֹ�ļ����������BHO -- ��ֻ��IE���������
        TCHAR pszLoader[MAX_PATH];
        GetModuleFileName(NULL,pszLoader, MAX_PATH);
        _tcslwr(pszLoader);
        if (_tcsstr(pszLoader, TEXT("explorer.exe")))
        {
            return FALSE;
        }
    }
#ifdef _MERGE_PROXYSTUB
	if (!PrxDllMain(hInstance, dwReason, lpReserved))
		return FALSE;
#endif
	hInstance;
	return _AtlModule.DllMain(dwReason, lpReserved); 
}
