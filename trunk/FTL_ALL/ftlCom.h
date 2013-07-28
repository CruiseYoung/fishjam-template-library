///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file   ftlCom.h
/// @brief  Fishjam Template Library Com Header File.
/// @author fujie
/// @version 0.6 
/// @date 03/30/2008
/// @defgroup ftlcom ftl com function and class
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef FTL_COM_H
#define FTL_COM_H
#pragma once

#include <atlbase.h>
#include <atlcom.h>
#include <atlctl.h>
//COM���� .rgs --  val DllSurrogate = s ''
//COM�ӿ� .rgs -- 
/*
	  Elevation
	  {
		  val Enabled = d 1
	  }
	  val LocalizedString = s  '@%MODULE%,-101'
*/

//����: COM�и��ֺ���ǩ������ͨ��MSDN�� OleView.exe �鿴��Ӧ�� tlb(�� mshtml.tlb) ���ļ�

/*************************************************************************************************************
* INITGUID -- ���� guiddef.h �� �ж�������� GUID(ʹ�ÿ�����Ѿ�����õ�),���õķ�ʽ��ֱ�Ӱ��� InitGuid.h
*
* ��������Ե��ַ�����ʾ
*   C++ �ַ�������null������ASCII��Unicode�ַ�����
*   Visual Basic�ַ�����һ��ASCII�ַ�������ϱ�ʾ���ȵ�ǰ׺��
*   Java�ַ�������0������Unicode�ַ�����
*
* 
* COM�Ļ����ַ�����������OLECHAR����ƽ̨�޹ص��ַ���ʾ������C++���ּ�ΪBSTR
*   BSTR(Basic STRing) <== ������(BSTRָ���ǰ4���ֽ�,����������NULL�ַ����ַ������ֽ������� SysAllocString(L"Hello")����Ϊ10)�Ŀ��ַ�(16bit)���飬
*     ���Դ���NULL�ֽڽ��зֶΣ����� �����ͻ��� �� ����Marshalʱ����Ч -- ����Ҫɨ����ȷ���ַ������ȣ�
*     ע��ʹ��BSTRʱ�����������ǶNULL�ַ����ַ���ʱ���п��ܳ��� -- BSTR �ᱻ��������Ϊ OLECHAR*(����NULL��β���ַ���)��
*       ��ʵ�������߲�ͬ��ǰ���Ƿ��г��ȡ��Ƿ������ǶNULL�ַ� �ȣ�
*       ����CComBSTR::Append(BSTR) ���� -- ��ȷ�ķ����� CComBSTR::AppendBSTR(BSTR).
*           CComBSTR::Append(LPCOLESTR �� LPCSTR) ��ȷ
*           CComBSTR bstrTest(BSTR); ���� -- ���BSTR�а���NULL�ַ����ᱻ��Ϊ�ǲ��� LPCOLESTR �����أ��Ӷ���ʧ���ݣ�
*           CComBSTR bstrTest(SysStringLen(BSTR), BSTR); ��ȷ -- ָ������
*           CComBSTR bstrTest = BSTR; ���� -- ��һ��������ǶNULL�ַ���BSTR��ֵ��һ��CComBSTR������Զ�޷���������
*           CComBSTR bstrTest; bstrTest.AppendBSTR(BSTR); ����, AppendBSTR ��ǰ������ Empty
*     SysAllocString���ص�ָ��ָ��BSTR�ĵ�һ���ַ�(������BSTR���ڴ�ĵ�һ���ֽ� -- ����λ��)����˿�ֱ�����ڴ󲿷���Ҫ LPCWSTR �� const OLECHAR*  �ĵط���
*       ���ǲ������� ��const �ĵط�����������ճɴ���
*     ���е�BSTR������ʹ��SysFreeString()�ͷ�,���������ڴ�й©��
*     ��һ�����캯�������Խ���REFGUID������ת��Ϊ�׶����ַ�����ʽ
*     ʹ�ù���COM�ӿ����Ƽ�ʹ�ã�������ʱ�������⣩
*       1.COM�ӿڶ���(�Ƽ�) -- ���Բ����Զ���Marshal
*       2.��Ҫ����IDL�ṹ���ж���BSTR��Ա(����ʹ��) -- ����ṹ��ĸ��ƺ��ͷŴ����鷳�����ֱ��ʹ����󳤶ȵ�TCHAR����
*       3.��ĳ�Ա�����ͷ�COM�ӿڵĺ���������ʹ��BSTR,�ֲ�����Ҫ�����ͷ�
*       4.�ڶ�BSTR�����޸ģ������������ͷ�ʱ��������ʹ��BSTR��ר�ú��� -- SysAllocString / SysAllocStringByteLen / SysFreeString / SysStringLen ��
*       5.��VB����NULLָ����һ����BSTR�ַ����ĺϷ�ֵ����SysStringLen�Ⱥ�����ΪNULL���Ϸ������Ҫ��ȷ��ȡBSTR���ȵĻ�����Ҫ��
*         UINT nLength = bstrInput ? SysStringLen(bstrInput) : 0;
*     ת��(�Ա�������˵�� BSTR �� OLECHAR* �� LPCOLESTR )
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
*         CComBSTR bstrBad(NULL) -- ����CComBSTR(int nSize)���ɳ���Ϊ0���ַ������ȼ���CComBSTR bstr2(L(""))�� CComBSTR::m_str ��Ҫ�ͷ�
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
* 
* CComVariant(VARIANT)
*   VariantInit -- ��ʼ��,�����ó� (vt = VT_EMPTY )
*   VariantCopy -- ����������ȷ�Ľ���ǳ���������
*   VariantCopyInd -- VARIANT������ѡ��ر�ʾ���һ��ļ����(VT_BYREF ���� VT_ARRAY?)�� �����������ȥ��VARIANT����һ������
*   VariantClear -- ����������ȷ���ͷţ������BSTR��SAFEARRAY�����ͷ�������ÿ��BSTR��Ȼ���ͷ����鱾��
*   ע�⣺CComVariant ����ַ����м������ع��죬��һ��ע�⣬���ܾͻ����(�������ATL.pdf��д�˻��� BSTR �����Ĺ��죬��VS2008��û��)
*     1.CComVariant(LPCOLESTR lpszSrc) 
*     2.CComVariant(LPCSTR lpszSrc)
* 
* �����д���GUID: CoCreateGuid, Ȼ���ͨ���� Data1~Data4 ���ʾ��������(�ɵ������������)
*
* COM���ӵ�ͻ��˵���(�μ�CFBhoObject::Invoke)��
*   ���� Invoke ������Ȼ����Դ�ӡ�����Ϣ���Ӷ�֪���������������ӵ��¼�
*
* ͨ��COM���ж�̬Ȩ������(������UAC�µ���ͨ�û����ɶ�̬ͨ��Ȩ����������COM�� -- COM Elevation Moniker)
*   http://blogs.msdn.com/b/vistacompatteam/archive/2006/09/28/cocreateinstanceasadmin-or-createelevatedcomobject-sample.aspx
*   COM������ע�������Ȩ������֧�ֱ�ʶ��
*     Lib��rgs�ļ���
*        APPID { val DllSurrogate = s '' }			-- COM������������´����� dllhost.exe ������
*        �������ö����Ƶ� AccessPermission ?
*     Object��rgs �ļ��У�
*        val LocalizedString = s'@%MODULE%,-101'	-- �������Ի�������ʾ���ַ���������ᱨ CO_E_MISSING_DISPLAYNAME
*        Elevation { 
*          val Enabled = d 1						-- ���� LUA-Enabled������᷵�� CO_E_ELEVATION_DISABLED
*          val IconReference= -s'@%MODULE%,-201'	-- ��ѡ�ĳ���ͼ��(ʵ����Ч? ����ǩ���Ż���Ч?)
*        } 
*   �ͻ��ˣ�ͨ�� CreateElevatedComObject ��������
*   �����׼�û�����(��������õĻ�������Ա����û�����⣬�����û����ʻ� E_ACCESSDENIED)
*     1.dcomcnfg -> Component Services\Computers\My Computer\DCOM Config -> �ҵ����� -> Properties -> Security
*       Access Permissions �� Add "Interactive"
*       TODO:�����ô���ֱ��������?
* 
*   Over-The-Shoulder (OTS) Elevation -- ����ʲô���͵�������
*   
* 
//http://blogs.msdn.com/b/vistacompatteam/archive/2006/09/28/cocreateinstanceasadmin-or-createelevatedcomobject-sample.aspx
#pragma TODO(The COM Elevation Moniker)
*     COM��( AppID/DllSurrogate, Elevation/Enabled=1, LocalizedString="xxx", )

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

		//����һ�����й�Ȩ��������COM����(��COM������������ض���Ҫ��?)
		FTLINLINE static HRESULT CreateElevatedComObject(HWND hwnd, REFCLSID rclsid, REFIID riid, __out void ** ppv);
    };

    FTLEXPORT class CFVariantInfo : public CFConvertInfoT<CFVariantInfo, const VARIANT&>
    {
        DISABLE_COPY_AND_ASSIGNMENT(CFVariantInfo);
    public:
		//bComPtrDetect �����Ƿ�ͨ�� ComDetect ���COM�ӿ�֧�ֵĽӿ�(�� IUnknown)
        FTLINLINE explicit CFVariantInfo(const VARIANT& info, BOOL bComPtrDetect = FALSE);
        FTLINLINE virtual LPCTSTR ConvertInfo();
	public:
		FTLINLINE VOID GetTypeInfo(CFStringFormater& formaterType);
		FTLINLINE VOID GetValueInfo(CFStringFormater& formaterValue);
	protected:
		BOOL m_bComPtrDetect;
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

	//Explorer ���ӵ��¼�ת����
	//�������� Explorer �ͻ��˵� Invoke ������Ȼ��������ת���õ� dispIdMember ��Ӧ���ַ���, �μ� CFBhoObject::Invoke
	//ͨ����Ҫ���˵� DISPID_STATUSTEXTCHANGE(102), DISPID_COMMANDSTATECHANGE(105, DISPID_PROGRESSCHANGE(108),
    //�ο����ӣ��� http://www.codeproject.com/Articles/7147/WTL-Browser
    FTLEXPORT class CFIExplorerDispidInfo : public CFConvertInfoT<CFIExplorerDispidInfo, DISPID>
    {
        DISABLE_COPY_AND_ASSIGNMENT(CFIExplorerDispidInfo);
    public:
        FTLINLINE explicit CFIExplorerDispidInfo(DISPID id, DISPPARAMS* pDispParams);
        FTLINLINE virtual LPCTSTR ConvertInfo();
	protected:
		DISPPARAMS*	m_pDispParams;
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
		enum {
			INVALID_INTERFACE_DUMPER_PARAM = -1
		};
		FTLINLINE explicit CFInterfaceDumperBase(IUnknown* pObj, IInformationOutput* pInfoOutput, 
			int nIndent, LONG_PTR param = INVALID_INTERFACE_DUMPER_PARAM );
		FTLINLINE HRESULT GetObjInfo(IInformationOutput* pInfoOutput);
        virtual ~CFInterfaceDumperBase();
	protected:
		IUnknown*           m_pObj;
		IInformationOutput*	m_pInfoOutput;
		int                 m_nIndent;
		LONG_PTR			m_nParam;
	};

    //�μ� codeproject �ϵ� CImageDataObject -- ԭ����ʵ���ƺ��в���Bug
    class CFImageDataObject 
        : public CComObjectRootEx<CComMultiThreadModel>
        , public IDataObjectImpl<CFImageDataObject>
    {
    public:
        BEGIN_COM_MAP(CFImageDataObject)
            COM_INTERFACE_ENTRY(IDataObject)
        END_COM_MAP()

        FTLINLINE HRESULT IDataObject_GetData(FORMATETC *pformatetcIn, STGMEDIUM *pmedium);
    public:
        FTLINLINE CFImageDataObject();
        FTLINLINE ~CFImageDataObject();
        IDataAdviseHolder* m_spDataAdviseHolder;
    };
}//namespace FTL

#endif //FTL_COM_H

#ifndef USE_EXPORT
#  include "ftlCom.hpp"
#endif