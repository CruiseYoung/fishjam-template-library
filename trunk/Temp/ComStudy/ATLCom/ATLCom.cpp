// ATLCom.cpp : Implementation of DLL Exports.

#include "stdafx.h"
#include "resource.h"
#include "ATLCom.h"

class CATLComModule : public CAtlDllModuleT< CATLComModule >
{
public :
	DECLARE_LIBID(LIBID_ATLComLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_ATLCOM, "{BF5B28A0-CEF0-49B4-B7D8-DE5AC1471092}")
};

CATLComModule _AtlModule;


// DLL Entry Point
// DLL ��ڵ�
extern "C" BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	hInstance;
    return _AtlModule.DllMain(dwReason, lpReserved); 
}


// Used to determine whether the DLL can be unloaded by OLE
// ����ȷ�� DLL �Ƿ���� OLE ж��
STDAPI DllCanUnloadNow(void)
{
    return _AtlModule.DllCanUnloadNow();
}


// Returns a class factory to create an object of the requested type
// ����һ���๤���Դ������������͵Ķ���
STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
    return _AtlModule.DllGetClassObject(rclsid, riid, ppv);
}


// DllRegisterServer - Adds entries to the system registry
// DllRegisterServer - ������ӵ�ϵͳע���
STDAPI DllRegisterServer(void)
{
    // registers object, typelib and all interfaces in typelib
    // ע��������Ϳ�����Ϳ��е����нӿ�
    HRESULT hr = _AtlModule.DllRegisterServer();
	return hr;
}


// DllUnregisterServer - Removes entries from the system registry
// DllUnregisterServer - �����ϵͳע������Ƴ�
STDAPI DllUnregisterServer(void)
{
	HRESULT hr = _AtlModule.DllUnregisterServer();
	return hr;
}
