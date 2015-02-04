#ifndef SCRIPT_SUPPORT_H
#define SCRIPT_SUPPORT_H
#pragma once
#include <ActivScp.h>

/****************************************************************************************************
* �ű�(JScript��VBScript��)��Ӧ�úܹ㷺 -- HTML��ASP��VBA����װ����XML������Shell������ȣ�
* 
* ��EXE������ VBScript��JavaScript �Ƚű���֧��(ʹ�ýű�����,�� VBScript��JScript) -- �磺
*   ͨ���ű�������������ƽ�溯����http://www.vckbase.com/document/viewdoc/?id=1462
*   ��MFCӦ�ó����м���ű�֧�֣� http://support.microsoft.com/kb/221992/en-us
*
* ��ǰ��Windowsƽ̨�ṩ��5������(�� CoCreateInstance(IActiveScript) ʱ�ṩ)
*   �ű�����                ProgID              CLSID 
* 1.VBScript                VBScript            {B54F3741-5B07-11CF-A4B0-00AA004A55E8} 
* 2.VBScript encoding       VBScript.Encode     {B54F3743-5B07-11cf-A4B0-00AA004A55E8} 
* 3.JScript                 JScript             {F414C260-6AC0-11CF-B6D1-00AA00BBBB58} 
* 4.JScript encoding        JScript.Encode      {F414C262-6AC0-11CF-B6D1-00AA00BBBB58} 
* 5.XMLScript               XML                 {989D1DC0-B162-11D1-B6EC-D27DDCF9A923}

* 1.ʵ�� IActiveScriptSite �ӿ�������ִ�нű�������--�ű������ͨ���ýӿڻ�ȡ�������Զ�������ָ�룻
*   a.GetItemInfo -- �ű�������ø÷���������κβ���ʶ�Ķ�����Ϣ����Ҫ�����Զ�������IDispatch�Ľӿ�ָ�룬
*     �ṩ�����������Ϳ��IUnknown�Ľӿ�ָ��
* 2.ʵ���Զ����ӿ� IDispatch�������ͽű��������Զ��������ṩ��󶨵ķ��������ԣ����нű���������չ(����ͨ�� call XXX �ķ�ʽ����)
* 3.���� <activscp.h> �ļ������� IActiveScript �ӿڣ�
*   a.ͨ�� SetScriptSite �����Լ�ʵ�ֵ� ScriptSite �ӿ�
*   b.ͨ�� AddNamedItem ���߽ű����洴���� Item���󣬲�ʹ��ɼ���
*   c.ͨ�� SetScriptState( SCRIPTSTATE_CONNECTED ) �������нű�
* 4.ͨ�� IActiveScript ��ȡ IActiveScriptParse �ӿ�
*   a.ͨ�� ParseScriptText �ṩ�ű��ı�
*
* The VBScript engine parses the text in the script for you and when it encounters a method call 
*   or property reference, it delegates the implementation to your provided interfaces.
*
* ����˳��
*   spActiveScript.CoCreateInstance( L"VBScript" ); -- ����(VBScript)�ű����棬�������ӿ�ָ��
*     =>IActiveScript::QueryInterface(IActiveScriptParse) -- ��ýű���������Ľӿ�ָ��
*       =>IActiveScript::SetScriptSite(IActiveScriptSite)
*         =>IActiveScriptParse::InitNew -- ��ʼ���ű�����Ľ�������
*           =>spActiveScript->AddNamedItem(L"MyObject", SCRIPTITEM_ISVISIBLE | SCRIPTITEM_ISSOURCE); 
*             ָ������ʹ���������Ŀ���� �� ��־��Ϣ
*             =>IActiveScriptParse::ParseScriptText -- �ύ��Ҫ���еĽű��ı�
*               =>IActiveScriptSite::GetItemInfo    -- �ص�ȡ����ű��������Զ�����������Ϳ�� IUnknown ָ��
*                 =>IActiveScript::SetScriptState(SCRIPTSTATE_CONNECTED) -- ����Ϊ����״̬����ʼ����
*                   =>�ű��п���ͨ�� call XXX �ķ�ʽ���� Site �ṩ�ķ�������֪ͨ -- Invoke��ʽ����
*                     => IActiveScript::Close() -- �ر����棬�ͷŽӿ�ָ��
*
* ʵ�ֶ��û�������ʽ������:
*   ���������� -- ��Ҫ�Ա��ʽ����ɨ�裬ȥ���ţ���������������ȼ�����2������Ȼ��������������沨�����ʽ��
*                 ��Ȼ��ͨ��ջ�ķ����������㡣
*   ���������� -- 
****************************************************************************************************/

template<typename T>
class ATL_NO_VTABLE IActiveScriptSiteImpl : public IActiveScriptSite
{
public:
    //�ű�������׼��ִ�нű������ʱ�򣬵��øú�����ѯ�ʽű���ʹ�õ����Ի�����
    //������� E_NOTIMPL,��ô����ͻ�ʹ�õ�ǰϵͳĬ��ʹ�õ�����
    STDMETHOD(GetLCID)(LCID *plcid) 
    {
        T* pT = static_cast<T*>(this);
        ATLTRACE2(atlTraceControls,2,_T("IActiveScriptSiteImpl::GetLCID\n"));
        return E_NOTIMPL;
    }

    //�ű�����ִ��ǰ�������������������Ϳ�(IDL�ļ��Ķ�������ʽ�������к����Ĳ�����Ϣ���߽ű�����ص��ķ���)
    //�������Ժ��ȡIDispatch�ӿ�(����ͨ��Invokeִ�нű��еĺ���)��IUnknownָ�롣
    //pstrName -- ���ڽű�����������������ű���Ŀ��������Ӧ��IUnknown��ITypeInfo ��ָ�룬
    //            ��ֵͨ�� IActiveScript::AddNamedItem ����ָ����
    STDMETHOD(GetItemInfo)(LPCOLESTR pstrName,DWORD dwReturnMask,IUnknown **ppiunkItem,ITypeInfo **ppti)
    {
        T* pT = static_cast<T*>(this);
        ATLTRACE2(atlTraceControls,2,_T("IActiveScriptSiteImpl::GetItemInfo\n"));
        if(ppti)
        {	//ȡ��������Ϣ�⣬���ű����к���������
            *ppti = NULL;
            if(dwReturnMask & SCRIPTINFO_ITYPEINFO)
            {
                CComPtr< ITypeLib > sptLib;
                _ASSERT(FALSE);
                //ע�⣬�˴���Ҫ����ͨ�õķ���
                ::LoadTypeLib( L"FunMap.tlb", &sptLib );	//װ����������Ϳ�
                if( sptLib )
                    sptLib->GetTypeInfo( 0, ppti );
            }
        }

        if(ppiunkItem)
        {	//ȡ�� IUnknown ָ�룬�� VBScript �����ĺ�������
            *ppiunkItem = NULL;
            if(dwReturnMask & SCRIPTINFO_IUNKNOWN)
            {
                _ASSERT(FALSE); //�˴�Ҳ��Ҫ����ͨ�õķ���
                if ( 0 == _wcsicmp( L"MyObject", pstrName ) )
                {
                    //��ǰ�Ĵ���������Active�ؼ���
                    //pT->ControlQueryInterface( IID_IUnknown, (LPVOID *)ppiunkItem );
                    pT->QueryInterface(IID_IUnknown,(LPVOID *)ppiunkItem);
                }
            }
        }
        return E_NOTIMPL;
    }

    //�ű�������Ҫͨ��Ψһ��һ���ַ������ʵ���ʱ�򱣴��װ���ĵ���״̬,������IE�е��ü��±��༭HTMLԴ�ļ���
    //������� E_NOTIMPL����ű�����Ĭ��ͬ��ʹ���ĵ�
    STDMETHOD(GetDocVersionString)(BSTR *pbstrVersion)
    {
        T* pT = static_cast<T*>(this);
        ATLTRACE2(atlTraceControls,2,_T("IActiveScriptSiteImpl::GetDocVersionString\n"));
        return E_NOTIMPL;
    }

    //�ű�����ִ�н�������OnStateChange ֮ǰ�������������ͬʱ SCRIPTSTATE_INITIALIZED �Ѿ��������
    //����pvarResult�д��ݽű���ִ�н�������ΪNULL��ʾ�ű�û��ִ�н����
    //����pexecpinfoΪNULL��ʾ�ű�ִ��û�д��󣬷�������Դ�����ṹ��ȡ�÷����쳣�� ������Ϣ��
    STDMETHOD(OnScriptTerminate)(const VARIANT *pvarResult,const EXCEPINFO *pexcepinfo)
    {
        T* pT = static_cast<T*>(this);
        ATLTRACE2(atlTraceControls,2,_T("IActiveScriptSiteImpl::OnScriptTerminate\n"));
        return E_NOTIMPL;
    }

    //�ű�������ִ�нű������У���״̬�����ı��ʱ�򣬵��øú�������Ϣ�ο� IActiveScript::GetScriptState
    STDMETHOD(OnStateChange)(SCRIPTSTATE ssScriptState)
    {
        T* pT = static_cast<T*>(this);
        ATLTRACE2(atlTraceControls,2,_T("IActiveScriptSiteImpl::OnStateChange\n"));
        return S_OK;
    }

    //���ű���������ʱ���ã����û������˲���ȷ�ĺ���ʽ����ű�ִ��ʱ����
    STDMETHOD(OnScriptError)(IActiveScriptError *pscripterror)
    {
        T* pT = static_cast<T*>(this);
        ATLTRACE2(atlTraceControls,2,_T("IActiveScriptSiteImpl::OnScriptError\n"));
        CComBSTR err,msg;
        EXCEPINFO ei = {0};
        HRESULT hr = E_FAIL;
        hr = pscripterror->GetSourceLineText( &err );
        hr = pscripterror->GetExceptionInfo(&ei);
        return S_OK;
    }

    //�ű���ʼִ��ʱ����
    STDMETHOD(OnEnterScript)(void)
    {
        T* pT = static_cast<T*>(this);
        ATLTRACE2(atlTraceControls,2,_T("IActiveScriptSiteImpl::OnEnterScript\n"));
        return S_OK;
    }

    //�ű�ִ�����ʱ����
    STDMETHOD(OnLeaveScript)(void)
    {
        T* pT = static_cast<T*>(this);
        ATLTRACE2(atlTraceControls,2,_T("IActiveScriptSiteImpl::OnLeaveScript\n"));
        return S_OK;
    }
};

//ʹ�ű�ʵ�����û��Ľ��潻�����ű������ͨ��IActiveScriptSite::QueryInterface() ����ѯ����ӿڲ�ʹ������
template<typename T>
class ATL_NO_VTABLE IActiveScriptSiteWindowImpl : public IActiveScriptSiteWindow
{
public:
    STDMETHOD(GetWindow)(HWND *phwnd)
    {
        T* pT = static_cast<T*>(this);
        ATLTRACE2(atlTraceControls,2,_T("IActiveScriptSiteWindowImpl::GetWindow\n"));
        return E_NOTIMPL;
    }

    STDMETHOD(EnableModeless)(BOOL fEnable)
    {
        T* pT = static_cast<T*>(this);
        ATLTRACE2(atlTraceControls,2,_T("IActiveScriptSiteWindowImpl::EnableModeless\n"));
        return E_NOTIMPL;
    }
};

#endif //SCRIPT_SUPPORT_H