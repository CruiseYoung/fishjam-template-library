// dllmain.cpp : Implementation of DllMain.

#include "stdafx.h"
#include "resource.h"
#include "FanXianTB_i.h"
#include "dllmain.h"
#include "dlldatax.h"

CFanXianTBModule _AtlModule;

// DLL Entry Point
extern "C" BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
#ifdef _MERGE_PROXYSTUB
	if (!PrxDllMain(hInstance, dwReason, lpReserved))
		return FALSE;
#endif
	hInstance;

	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		{
			//������Դ������(explorer.exe), ֻ����IE(iexplore.exe)
			if (CFSystemUtil::IsSpecialProcessName(TEXT("explorer.exe"), NULL))
			{
				FTLTRACE(TEXT("IsSpecialProcessName return for explorer.exe\n"));
				return FALSE;
			}
		}
		break;
	case DLL_PROCESS_DETACH:
		break;
	}

	return _AtlModule.DllMain(dwReason, lpReserved); 
}
