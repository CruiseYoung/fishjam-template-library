#include "StdAfx.h"
#include ".\comdebug.h"

CComDebug::CComDebug(void)
{
}

CComDebug::~CComDebug(void)
{
}

HRESULT CComDebug::RegisterCOMObject(LPCTSTR pszObjectPath, BOOL bRegister)
{
    HRESULT hr = E_FAIL;

    //typedef HRESULT (WINAPI * FREG)();
    //TCHAR szWorkPath[ MAX_PATH ];

    //::GetCurrentDirectory( sizeof(szWorkPath), szWorkPath );	// ���浱ǰ���̵Ĺ���Ŀ¼
    //::SetCurrentDirectory( ���Ŀ¼ );	// �л��������Ŀ¼����ֹ�����װ�ص�ʱ����Ҫͬʱ����һЩ����������DLL

    //HMODULE hDLL = ::LoadLibrary( ����ļ��� );	// ��̬װ�����
    //if(hDLL)
    //{
    //    FREG lpfunc = (FREG)::GetProcAddress( hDLL, _T("DllRegisterServer") );	// ȡ��ע�ắ��ָ��
    //    // ����Ƿ�ע�ᣬ����ȡ��"DllUnregisterServer"����ָ��
    //    if ( lpfunc )	lpfunc();	// ִ��ע�ᡣ����Ϊ�˼򵥣�û���жϷ���ֵ
    //    ::FreeLibrary(hDLL);
    //}

    //::SetCurrentDirectory(szWorkPath);	// �л���ԭ�ȵĽ��̹���Ŀ¼

    return hr;
}