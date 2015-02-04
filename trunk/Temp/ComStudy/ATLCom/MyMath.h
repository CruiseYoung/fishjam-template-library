// MyMath.h : Declaration of the CMyMath

#pragma once
#include "resource.h"       // main symbols

#include "ATLCom.h"
#include "ScriptSupport.h"          
#include "_IMyTimerEvents_CP.H"     //���ӵ��򵼻��Զ����ɸ��ļ�������

#define USE_FREE_THREADED_MARSHALER   //ʹ�á��ۺϡ��ķ�ʽ����ϵͳ�ṩ�������̹߳��������
//���ʹ����Bothģ�ͣ���Ҫ������������̹߳���������ۺ���һ��,����Ļ����ڿͻ���ʹ��ʱ���ܳ���
//  �磺�ͻ���ʹ�� COINIT_APARTMENTTHREADED ��ʼ��ʱ������֡�Access violation reading location���Ĵ���


#define MAX_CALLBACK_COUNT  10
//#define USE_IPERSISTSTREAMINITIMPL

// CMyMath

class ATL_NO_VTABLE CMyMath : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CMyMath, &CLSID_MyMath>,
	public IDispatchImpl<IMyDispatch, &IID_IMyDispatch, &LIBID_ATLComLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
    //public ISupportErrorInfoImpl<&CLSID_MyMath>,
    public ISupportErrorInfo,
    //public IMyMath,
    public IMyMath2,     //����IMyMath2 �̳���IMyMath����˼̳��б��в�����Ҫ IMyMath
#ifdef USE_IPERSISTSTREAMINITIMPL
    public IPersistStreamInitImpl<CMyMath>, //Ҳ���Դ�ATL�ṩ�Ľӿ�ʵ���м̳У�����ӱ�Ҫ��ӳ�伴��
#else
    public IPersistStreamInit,              // �ֹ���ӳ����Խӿ� -- ���ڼ�������ʱ����ͣ
#endif
    public IActiveScriptSiteImpl<CMyMath>,   //�Լ����� ScriptSite ʵ��ģ�棬��δ��ϸ����
    public IComponentInfo                   //ʹ���Զ���ṹ�Ľӿ�
{
public:
	CMyMath();
    ~CMyMath();

DECLARE_REGISTRY_RESOURCEID(IDR_MYMATH)


BEGIN_COM_MAP(CMyMath)
    //������ط�����ӿڣ��ͻ��˲��ܽ��� QueryInterface
    COM_INTERFACE_ENTRY(IMyMath)
    COM_INTERFACE_ENTRY(IMyMath2)
    COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IDispatch)
    COM_INTERFACE_ENTRY(IMyDispatch)

    // �ֹ���ӽӿ�ӳ������
    COM_INTERFACE_ENTRY(IPersistStreamInit)
    // ��ʾ���Ҫȡ�� IPersistStream ָ�룬�򷵻� IPersistStreamInit ָ��
    COM_INTERFACE_ENTRY_IID(IID_IPersistStream, IPersistStreamInit)
    // ��ʾ���Ҫȡ�� IPersist ָ�룬�򷵻� IPersistStremInit ָ��
    COM_INTERFACE_ENTRY_IID(IID_IPersist, IPersistStreamInit)
    COM_INTERFACE_ENTRY(IActiveScriptSite)
    COM_INTERFACE_ENTRY(IComponentInfo)
#ifdef USE_FREE_THREADED_MARSHALER
    COM_INTERFACE_ENTRY_AGGREGATE(IID_IMarshal,m_pUnkMarshaler.p)
#endif
END_COM_MAP()


// �ֹ��������ӳ������� IPersistXXXImpl ������ġ���д ActiveX ��ʱ��ATL �򵼻�������������ӳ���
#ifdef USE_IPERSISTSTREAMINITIMPL
BEGIN_PROP_MAP(CMyMath)
    // ������"��������", �ӿ��������(��IDL�ļ�), ����ҳ�Ի���
    PROP_ENTRY("str", 1, CLSID_NULL)
    PROP_ENTRY("integer", 2, CLSID_NULL)
    //PROP_ENTRY_EX
    //PROP_PAGE
    //PROP_DATA_ENTRY
END_PROP_MAP()
#endif

// ISupportsErrorInfo
    STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

	DECLARE_PROTECT_FINAL_CONSTRUCT()
    DECLARE_GET_CONTROLLING_UNKNOWN()

	HRESULT FinalConstruct()
	{
#ifdef USE_FREE_THREADED_MARSHALER
        return CoCreateFreeThreadedMarshaler(GetControllingUnknown(),
            &m_pUnkMarshaler.p);
#else
		return S_OK;
#endif
	}
	
	void FinalRelease() 
	{
#ifdef USE_FREE_THREADED_MARSHALER
        m_pUnkMarshaler.Release();
#endif
	}

private:
    // �洢�ص��ӿ�ָ�������
    IMyMathCallBack* m_pCallBack[MAX_CALLBACK_COUNT];
    // �����￪ʼ��������
    LONG m_nNow;

    // �ڲ������������Ƿ��Ѿ��仯��
    BOOL m_bDirty;

#ifdef USE_FREE_THREADED_MARSHALER
    CComPtr<IUnknown> m_pUnkMarshaler;
#endif
    
public:
#ifdef USE_IPERSISTSTREAMINITIMPL
    // �����Ա�������� IPersistXXXImpl ������ģ������ú� m_bDirty ��ͬ
    BOOL m_bRequiresSave;	// ��ʾ���������Ƿ��Ѿ��ı����Ҫ����
#endif

    STDMETHOD(Add)(LONG n1, LONG n2, LONG* pResult);
    STDMETHOD(Mul)(LONG n1, LONG n2, LONG* pResult);
    STDMETHOD(Sum)(LONG nArraySize, LONG nArray[], LONG* plResult);

    STDMETHOD(GetPrim)(LONG *plCount,LONG **ppLData);
    
    //ʹ��ָ��ö�ٵķ���
    STDMETHOD(Compute)(/*[in]*/mathOPERATION enumOp, /*[in]*/LONG lOp1, /*[in]*/LONG lOp2,/*[out,retval]*/ LONG* plResult);

    STDMETHOD(Upper)(BSTR str, BSTR* pVal);
    STDMETHOD(DemoMarshalLongOperation)(BSTR strParam);

    //CallBack
    STDMETHOD(Sub)(LONG n1, LONG n2);
    STDMETHOD(MyAdvise)(IMyMathCallBack* pCallBack, LONG* pdwCookie);
    STDMETHOD(MyUnAdvise)(LONG dwCookie);

    //������ʾ�����ԵĽӿڷ���
    STDMETHOD(GetNext)(LONG* pVal);

    STDMETHOD(GetComponentInfo)(COMPONENT_INFO** ppInfo);

    // IPersist
    STDMETHOD(GetClassID)(/*[out]*/CLSID * pClassID);
    // IPersistStream
    STDMETHOD(IsDirty)(void);
    STDMETHOD(Load)(/*[in]*/IStream *pStm);
    STDMETHOD(Save)(/*[in]*/IStream *pStm,/*[in]*/BOOL fClearDirty);
    STDMETHOD(GetSizeMax)(/*[out]*/ULARGE_INTEGER *pcbSize);
    // IPersistStreamInit
    STDMETHOD(InitNew)(void);
    STDMETHOD(LengthyComOperation)(LONG nTimeout);
};

OBJECT_ENTRY_AUTO(__uuidof(MyMath), CMyMath)
