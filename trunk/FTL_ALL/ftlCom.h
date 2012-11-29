///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file   ftlCom.h
/// @brief  Functional Template Library Base Header File.
/// @author fujie
/// @version 0.6 
/// @date 03/30/2008
/// @defgroup ftlcom ftl com function and class
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef FTL_COM_H
#define FTL_COM_H
#pragma once

#pragma TODO(Delete BSTR In COMStudy)
/*************************************************************************************************************
* INITGUID -- ���� guiddef.h �� �ж�������� GUID(ʹ�ÿ�����Ѿ�����õ�),���õķ�ʽ��ֱ�Ӱ��� InitGuid.h
*
* ��������Ե��ַ�����ʾ
*   C++ �ַ�������null������ASCII��Unicode�ַ�����
*   Visual Basic�ַ�����һ��ASCII�ַ�������ϱ�ʾ���ȵ�ǰ׺��
*   Java�ַ�������0������Unicode�ַ�����
* 
* COM�Ļ����ַ�����������OLECHAR����ƽ̨�޹ص��ַ���ʾ������C++���ּ�ΪBSTR
*   BSTR(Basic STRing) <== ������(BSTRָ���ǰ4���ֽ�,����������NULL�ַ����ַ������ֽ������� SysAllocString(L"Hello")����Ϊ10)�Ŀ��ַ�(16bit)���飬
*     ���Դ���NULL�ֽڽ��зֶΣ����ҽ���Marshalʱ����Ч -- ����Ҫɨ����ȷ���ַ������ȣ�
*     SysAllocString���ص�ָ��ָ��BSTR�ĵ�һ���ַ�(������BSTR���ڴ�ĵ�һ���ֽ� -- ����λ��)����˿�ֱ�����ڴ󲿷���Ҫ LPCWSTR �� const OLECHAR*  �ĵط���
*       ���ǲ������� ��const �ĵط�����������ճɴ���
*     ���е�BSTR������ʹ��SysFreeString()�ͷ�,���������ڴ�й©��
*     ʹ�ù���COM�ӿ����Ƽ�ʹ�ã�������ʱ�������⣩
*       1.COM�ӿڶ���(�Ƽ�) -- ���Բ����Զ���Marshal
*       2.��Ҫ����IDL�ṹ���ж���BSTR��Ա(����ʹ��) -- ����ṹ��ĸ��ƺ��ͷŴ����鷳�����ֱ��ʹ����󳤶ȵ�TCHAR����
*       3.��ĳ�Ա�����ͷ�COM�ӿڵĺ���������ʹ��BSTR,�ֲ�����Ҫ�����ͷ�
*       4.�ڶ�BSTR�����޸ģ������������ͷ�ʱ��������ʹ��BSTR��ר�ú��� -- SysAllocString / SysAllocStringByteLen / SysFreeString / SysStringLen ��
*     ת��
*       1.LPSTR   => BSTR  -- a.WCHAR wstr[MAX_WSTR_LEN]; MultiByteToWideChar(CP_ACP, 0, str, strlen(str)+1, wstr, xxx); BSTR bstr1 = ::SysAllocString(wstr);
*                             b._bstr_t("xxxx");
*       2.BSTR    => LPSTR -- a.WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)bstr,xxx); 
*                             b._bstr_t bstr1(bstr); const char* str = static_cast<const char*> bstr1;
*       3.BSTR    => LPWSTR(��Ҫ�޸�) -- UINT unLen = ::SysStringLen(bstr1); LPWSTR psz1 = new WCHAR[len+1]; ::_tcsncpy(psz1, bstr1, unLen);
*       4.BSTR    => LPCWSTR(���޸ģ�ֻ��)  -- LPCWSTR psz2 = static_cast<LPCWSTR>bstr2; ��ע�⣺���Free bstr2 ��Ͳ�����ʹ�� psz2
*       5.CString => BSTR  -- BSTR bstrTest = str1.AllocSysString();  xxxxxx;  ::SysFreeString(bstrTest);  
*                          �� CComBSTR bstr1(static_cast<LPCTSTR>(str1));  �����ֹ��ͷ�
*       6.BSTR    => CString -- BSTR bstrTest = SysAllocString("XXX"); CString str1(bstrTest); SysFreeString(bstrTest); 
*                               ���⣺CString ������?(ATL/WTL/MFC)
*       
*     ������: 
*       CComBSTR -- ֻ��һ��m_str��Ա�������ڷ���������Ĭ�Ϲ�������ֵ����ΪNULL�������Զ�����BSTR��Ҳ����Empty�ֶ��ͷţ�
*         ͨ����Ҫ��ֵ����Ϊ�մ�������ͻ�����ʹ��δ��ֵ���ִ��׳��쳣���ܽ���ANSI����ֻ�����UNICODE
*         CComBSTR bstrBad(NULL) -- ����CComBSTR(int nSize)���ɳ���Ϊ0���ַ������ȼ���CComBSTR bstr2(L(����))�� CComBSTR::m_str ��Ҫ�ͷ�
*         CComBSTR bstrGood / CComBSTR bstrGood(static_cast< LPCOLESTR> (NULL))�ŵ��� CComBSTR(LPCOLESTR pSrc) ��CComBSTR::m_str ����NULL
*       _bstr_t -- �����ü�����Ҫ���쳣�����ʺ��ڿͻ������ṩ�˵�wchar_t��char��ת�������������ɼ򵥵�BSTR�ַ����ȽϵȲ���
*       std::string/wstring -- ���������ƣ�����ֲ����á��Ƽ�ʹ��
*       comutil.h �� _com_util ���ƿռ��ж����� ConvertStringToBSTR �� ConvertBSTRToString ת������
*     ע�⣺
*       1.Automation��cache BSTRʹ�õĿռ䣬������������������ -- ��ˣ�Ӧ�þ�����СBSTR�����淶Χ
*       2.��һ���Ѿ���ʼ���õ�CComBSTR�ĵ�ַ����һ��������Ϊ[out]�����ᵼ���ڴ�й© -- �����ȵ���Empty��������ַ��������ݣ�
*         ��������ͬ�Ĵ��룬�������������[in, out]���Ͳ�����й©����Ϊ��������Ӧ�û��ڸ���֮ǰ��Freeԭ�еĴ�
*       3.��һ��CComBSTR���͵����Ա�������ظ�����[out]��������Ҫ�� Copy �� Detach
*       4.������CComBSTR����ѭ������Ƶ���ַ����޸Ĳ�����BSTR��Ч�ʽϵ�
*************************************************************************************************************/

#ifndef FTL_BASE_H
#  error ftlcom.h requires ftlbase.h to be included first
#endif

#include <exdispid.h>
#include <atlcomcli.h>
#include <atlsafe.h>

namespace FTL
{
//Class
    
//Function
    class CFComUtility
    {
    public:
        FTLINLINE static BOOL IsComponentRegistered(REFCLSID clsid);
        FTLINLINE static BOOL RegisterComponent(LPCTSTR pszFilePath,BOOL bRegister);
        FTLINLINE static HRESULT DisplayObjectPropertyPage(HWND hWndOwner,LPCOLESTR lpszCaption, IUnknown *pUnk);
    };

    FTLEXPORT class CFVariantInfo : public CFConvertInfoT<CFVariantInfo, const VARIANT&>
    {
        DISABLE_COPY_AND_ASSIGNMENT(CFVariantInfo);
    public:
        FTLINLINE explicit CFVariantInfo(const VARIANT& info);
        FTLINLINE virtual LPCTSTR ConvertInfo();
	public:
		FTLINLINE VOID GetTypeInfo(CFStringFormater& formaterType);
		FTLINLINE VOID GetValueInfo(CFStringFormater& formaterValue);
    };

	FTLEXPORT class CFPropVariantInfo : public CFConvertInfoT<CFPropVariantInfo, const PROPVARIANT&>
	{
		DISABLE_COPY_AND_ASSIGNMENT(CFPropVariantInfo);
	public:
		FTLINLINE explicit CFPropVariantInfo(const PROPVARIANT& info);
		FTLINLINE virtual LPCTSTR ConvertInfo();
	public:
		FTLINLINE VOID GetTypeInfo(CFStringFormater& formaterType);
		FTLINLINE VOID GetValueInfo(CFStringFormater& formaterValue);
	};

    class CFSideBySide //��ָ���ļ��в���COM����(��sbs�п���)
    {
    public:
        FTLINLINE static HRESULT /*__stdcall*/ SbsCreateInstance(LPCTSTR szModule, REFCLSID rclsid, LPUNKNOWN pUnkOuter, 
            DWORD dwClsContext, REFIID riid, LPVOID FAR* ppv);
        FTLINLINE static HRESULT /*__stdcall*/ SbsCreateInstanceFromModuleHandle(HMODULE hModule, REFCLSID rclsid, LPUNKNOWN pUnkOuter,
            DWORD dwClsContext, REFIID riid, LPVOID FAR* ppv);
    };

    FTLEXPORT class CFIExplorerDispidInfo : public CFConvertInfoT<CFIExplorerDispidInfo, DISPID>
    {
        DISABLE_COPY_AND_ASSIGNMENT(CFIExplorerDispidInfo);
    public:
        FTLINLINE explicit CFIExplorerDispidInfo(DISPID id);
        FTLINLINE virtual LPCTSTR ConvertInfo();
    };

    //���߳�֮�䴫��COM�ӿڵĸ�����
    template<typename T>
    class QueryInterfaceForBackgroundThread
    {
    public:
        QueryInterfaceForBackgroundThread(IUnknown* pUnk);
        ~QueryInterfaceForBackgroundThread();
		T* GetBackThreadInterface();
        //operator T*();
    private:
        IStream*	m_pStream;
        T*	        m_pInterface;
    };

    class IInformationOutput
    {
    public:
        virtual HRESULT SetIndent(int nIndent) = 0;
		virtual int GetIndent() = 0;
        virtual HRESULT OutputInfoName(LPCTSTR pszInfoName) = 0;
		virtual HRESULT OnOutput(LPCTSTR pszPrompInfo) = 0;
        virtual HRESULT OnOutput(LPCTSTR pszKey, LPCTSTR pValue) = 0;
		virtual HRESULT OnOutput(LPCTSTR pszKey, BSTR* pValue) = 0;
		virtual HRESULT OnOutput(LPCTSTR pszKey, VARIANT_BOOL value) = 0;
        virtual HRESULT OnOutput(LPCTSTR pszKey, LONG nValue) = 0;
		virtual HRESULT OnOutput(LPCTSTR pszKey, DWORD dwValue) = 0;
		virtual HRESULT OnOutput(LPCTSTR pszKey, DOUBLE fValue) = 0;
		virtual HRESULT OnOutput(LPCTSTR pszKey, int iValue) = 0;
		virtual HRESULT OnOutput(LPCTSTR pszKey, HWND hWnd) = 0;
		virtual HRESULT OnOutput(LPCTSTR pszKey, GUID* pGuid) = 0;
		virtual HRESULT OnOutput(LPCTSTR pszKey, VARIANT* pValue) = 0;
        virtual HRESULT OnOutput(LPCTSTR pszKey, LONG nTotal, LONG nIndex, VARIANT* pValue) = 0;
    };

    class CFOutputWindowInfoOutput : public IInformationOutput
    {
    public:
		FTLINLINE static CFOutputWindowInfoOutput* Instance();
        FTLINLINE CFOutputWindowInfoOutput();
        FTLINLINE ~CFOutputWindowInfoOutput();
        FTLINLINE virtual HRESULT SetIndent(int nIndent);
		FTLINLINE virtual int GetIndent();
        FTLINLINE virtual HRESULT OutputInfoName(LPCTSTR pszInfoName);
		FTLINLINE virtual HRESULT OnOutput(LPCTSTR pszPrompInfo);
        FTLINLINE virtual HRESULT OnOutput(LPCTSTR pszKey, LPCTSTR pValue);
        FTLINLINE virtual HRESULT OnOutput(LPCTSTR pszKey, BSTR* pValue);
		FTLINLINE virtual HRESULT OnOutput(LPCTSTR pszKey, VARIANT_BOOL value);
        FTLINLINE virtual HRESULT OnOutput(LPCTSTR pszKey, LONG nValue);
		FTLINLINE virtual HRESULT OnOutput(LPCTSTR pszKey, DWORD dwValue);
		FTLINLINE virtual HRESULT OnOutput(LPCTSTR pszKey, DOUBLE fValue);
		FTLINLINE virtual HRESULT OnOutput(LPCTSTR pszKey, int iValue);
		FTLINLINE virtual HRESULT OnOutput(LPCTSTR pszKey, HWND hWnd);
		FTLINLINE virtual HRESULT OnOutput(LPCTSTR pszKey, GUID* pGuid);
		FTLINLINE virtual HRESULT OnOutput(LPCTSTR pszKey, VARIANT* pValue);
        FTLINLINE virtual HRESULT OnOutput(LPCTSTR pszKey, LONG nTotal, LONG nIndex, VARIANT* pValue);
    protected:
        int     m_nIndent;
        TCHAR*  m_pszIndentSpace;
        FTLINLINE virtual VOID    OutputIndentSpace();
    };

	template <typename T>
	class CFInterfaceDumperBase
	{
		DISABLE_COPY_AND_ASSIGNMENT(CFInterfaceDumperBase);
	public:
		FTLINLINE explicit CFInterfaceDumperBase(IUnknown* pObj, IInformationOutput* pInfoOutput, int nIndent);
		FTLINLINE HRESULT GetObjInfo(IInformationOutput* pInfoOutput);
        virtual ~CFInterfaceDumperBase();
	protected:
		IUnknown*           m_pObj;
		IInformationOutput*	m_pInfoOutput;
		int                 m_nIndent;
	};

}//namespace FTL

#endif //FTL_COM_H

#ifndef USE_EXPORT
#  include "ftlCom.hpp"
#endif