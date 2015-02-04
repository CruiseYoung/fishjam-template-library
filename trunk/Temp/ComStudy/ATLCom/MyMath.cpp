// MyMath.cpp : Implementation of CMyMath

#include "stdafx.h"
#include "MyMath.h"
#include ".\mymath.h"

// CMyMath

CMyMath::CMyMath()
{
    for( int i=0; i < MAX_CALLBACK_COUNT; i++)	// ��ʼ�����еĻص��ӿ�Ϊ NULL
    {
        m_pCallBack[i] = NULL;
    }
    m_nNow = 0;
    m_bDirty = FALSE;
}

CMyMath::~CMyMath()
{
}

STDMETHODIMP CMyMath::InterfaceSupportsErrorInfo(REFIID riid)
{
    static const IID* arr[] = 
    {
        &IID_IMyMath2
    };

    for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
    {
        if (InlineIsEqualGUID(*arr[i],riid))
            return S_OK;
    }
    return S_FALSE;
}

STDMETHODIMP CMyMath::Add(LONG n1, LONG n2, LONG* pResult)
{
    if (pResult)
    {
        *pResult = n1 + n2;
        return S_OK;
    }
    else
    {
        return E_INVALIDARG;
    }
}

STDMETHODIMP CMyMath::Mul(LONG n1, LONG n2, LONG* pResult)
{
    HRESULT hr = E_FAIL;
    if (pResult)
    {
        *pResult = n1 * n2;
        return S_OK;
    }
    else
    {
#if 1
//����1 -- ԭ��
        //���� ISupportErrorInfo �����÷���
        CComQIPtr< ICreateErrorInfo> spCEI;     // ���� ICreateErrorInfo �ӿ�ָ��
        if (SUCCEEDED(::CreateErrorInfo( &spCEI )))
        {
            spCEI->SetGUID( IID_IMyMath2 );		// ��������Ľӿ�IID, �����ϵͳ����Ĵ���Ҳ����ʹ�� GUID_NULL

            LPOLESTR pProgID = NULL;
            hr = ::ProgIDFromCLSID( CLSID_MyMath, &pProgID );	// ͨ�õķ�����ȡ�� ProgID
            if (SUCCEEDED(hr))
            {
                spCEI->SetSource(pProgID);
                ::CoTaskMemFree( pProgID );		// �ͷ��ڴ�
            }
            //��
            //spCEI->SetSource( L"ATLCom.MyMath" );	// ProgID ������ rgs �ļ��п���
            

            // ���������ͬʱ�ṩ�˰����ļ�����ô�Ϳ��ԣ�
            spCEI->SetHelpContext( 0 );		// ���ð����ļ��������
            //spCEI->SetHelpFile( L"xxx.hlp" );	// ���ð����ļ����ļ���

            spCEI->SetDescription( L"����Ĳ���" );

            CComQIPtr < IErrorInfo > spErrInfo = spCEI; // QueryInterface �õ� IErrorInfo
            if( spErrInfo )
            {
                ::SetErrorInfo( 0, spErrInfo );	// ��ʱ�����߾Ϳ��Եõ�������Ϣ��
            }
        }
        return E_INVALIDARG;
#else
//����2 -- ʹ��ATL�ṩ�İ�װ����
        return Error( L"����Ĳ���",IID_IMyMath2,E_INVALIDARG);
#endif 
    }
}

STDMETHODIMP CMyMath::Sum(LONG nArraySize, LONG nArray[], LONG* plResult)
{
    *plResult = 0;
    while (nArraySize)
    {
        *plResult += nArray[--nArraySize];
    }
    return S_OK;
}

STDMETHODIMP CMyMath::GetPrim(LONG *plCount,LONG **ppLData)
{
    return E_NOTIMPL;
}

STDMETHODIMP CMyMath::DemoMarshalLongOperation(BSTR strParam)
{
    USES_CONVERSION;
    //ObjectLock lock(this);

    //this->Lock();

    ATLTRACE(TEXT(">>> Enter CMyMath::DemoMarshalLongOperation, lParam=%s,ThreadID=%d,CoCurProcess = %d\n"),OLE2CT(strParam),
        GetCurrentThreadId(),CoGetCurrentProcess());
    //Just Do Slepp
    Sleep(3000);

    ATLTRACE(TEXT("<<< Leave CMyMath::DemoMarshalLongOperation, lParam=%s,ThreadID=%d,CoCurProcess = %d\n"),OLE2CT(strParam),
        GetCurrentThreadId(),CoGetCurrentProcess());

    //this->Unlock();

    return S_OK;
}


STDMETHODIMP CMyMath::Upper(BSTR str, BSTR* pVal)
{
    *pVal = NULL;	// ��Զ��ʼ������ֵ�Ǹ���ϰ��

    CComBSTR s(str);
    s.ToUpper();	// ת��Ϊ��д

    *pVal = s.Copy();

    return S_OK;
}

STDMETHODIMP CMyMath::Sub(LONG n1, LONG n2)
{
   LONG nResult = n1 - n2;
   for (int i = 0; i< MAX_CALLBACK_COUNT; i++)
   {
       if( m_pCallBack[i] )						// ����ص��ӿ���Ч
           m_pCallBack[i]->Fire_Result( nResult );	// �򷢳��¼�/֪ͨ
   }
    return S_OK;
}

STDMETHODIMP CMyMath::MyAdvise(IMyMathCallBack* pCallBack, LONG* pdwCookie)
{
    if( NULL == pCallBack )		// ��Ȼ����һ����ָ�룿��
        return E_INVALIDARG;

    for( int i=0; i<10; i++)	// Ѱ��һ������ýӿ�ָ���λ��
    {
        if( NULL == m_pCallBack[i] )	// �ҵ���
        {
            m_pCallBack[i] = pCallBack;	// ���浽������
            m_pCallBack[i]->AddRef();	// ָ������� +1

            *pdwCookie = i+1;			// cookie ���������±�+1
            // +1 ��Ŀ���Ǳ���ʹ��0����Ϊ0��ʾ��Ч״̬

            return S_OK;
        }
    }
    return E_OUTOFMEMORY;	// ����10�����ӣ��ڴ治������
}

STDMETHODIMP CMyMath::MyUnAdvise(LONG dwCookie)
{
    if( dwCookie<1 || dwCookie>10 )		// ����˭�ɵ�ѽ���Ҹ�����
        return E_INVALIDARG;

    if( NULL == m_pCallBack[ dwCookie -1 ] )	// �������󣬻�ýӿ�ָ���Ѿ���Ч��
        return E_INVALIDARG;

    m_pCallBack[ dwCookie - 1 ]->Release();	// ָ������� -1
    m_pCallBack[ dwCookie -1 ] = NULL;		// �ճ����±������Ԫ��

    return S_OK;
}

// IPersist
STDMETHODIMP CMyMath::GetClassID(/*[out]*/CLSID * pClassID)
{
    *pClassID = GetObjectCLSID();
    return S_OK;
}
// IPersistStream
STDMETHODIMP CMyMath::IsDirty(void)
{
    if( m_bDirty )	
        return S_OK;
    else			
        return S_FALSE;
}

STDMETHODIMP CMyMath::Load(/*[in]*/IStream *pStm)
{
    return pStm->Read( &m_nNow, sizeof(m_nNow), NULL);
}

STDMETHODIMP CMyMath::Save(/*[in]*/IStream *pStm,/*[in]*/BOOL fClearDirty)
{
    if( fClearDirty )
    {
        m_bDirty = FALSE;
    }
    return pStm->Write( &m_nNow,sizeof(m_nNow), NULL );
    return S_OK;
}

STDMETHODIMP CMyMath::GetSizeMax(/*[out]*/ULARGE_INTEGER *pcbSize)
{
    //pcbSize->LowPart = ��Ҫ�������ݳ��ȵĵ�λ;
    //pcbSize->HighPart = ��Ҫ�������ݳ��ȵĸ�λ;// һ�㶼��0���ѵ�������ݳ��ȶ������� 4G��
    pcbSize->LowPart = sizeof(m_nNow);
    pcbSize->HighPart = 0;

    return S_OK;
}

// IPersistStreamInit
STDMETHODIMP CMyMath::InitNew(void)
{
    //�ڲ���������Ĭ�ϳ�ʼ��;
    //���û�����ڲ���ʾ���ݱ仯�ı���;
    m_nNow = 0;
    m_bDirty = TRUE;

    return S_OK;
}
STDMETHODIMP CMyMath::GetNext(LONG* pVal)
{
    while(1)	// ���� m_nNow ��ĵ�һ������
    {
        m_nNow ++;
        long i;
        for( i = 2; i<m_nNow; i++ )
        {
            if( m_nNow % i == 0 )	break;	//�ܹ�����������������
        }
        if( i == m_nNow )	break;	// �ҵ��ˣ�ֹͣ����
    }

    *pVal = m_nNow;
    m_bDirty = TRUE;	// �ڲ����������ݣ��Ѿ��ı���

    return S_OK;
}

STDMETHODIMP CMyMath::Compute(mathOPERATION enumOp, LONG lOp1, LONG lOp2, LONG* plResult)
{
    switch (enumOp)
    {
    case mathAdd:
        return Add(lOp1,lOp2,plResult);
    case mathSub:
        *plResult = lOp1 - lOp2;
        return S_OK;
    case mathMul:
        return Mul(lOp1,lOp2,plResult);
    case mathDiv:
        if(lOp2 == 0)
        {
            return Error("divide by zero",IID_IMyMath2,E_INVALIDARG);
        }
        *plResult = lOp1/lOp2;
        return S_OK;
    default:
        return E_INVALIDARG;
    }
    return S_OK;
}

STDMETHODIMP CMyMath::GetComponentInfo(COMPONENT_INFO** ppInfo)
{
    *ppInfo = (COMPONENT_INFO*)CoTaskMemAlloc(sizeof(COMPONENT_INFO));
    if (*ppInfo)
    {
        ZeroMemory(*ppInfo,sizeof(COMPONENT_INFO));
        (*ppInfo)->Major = 1;
        (*ppInfo)->Minor = 0;
        TCHAR buf[100] = {0};
        if (LoadString(_AtlBaseModule.GetResourceInstance(),IDS_AUTHOR,buf,_countof(buf)))
        {
            USES_CONVERSION;
            (*ppInfo)->Author = SysAllocString(T2W(buf));
        }
        return S_OK;
    }
    else
    {
        return E_OUTOFMEMORY;
    }
}
STDMETHODIMP CMyMath::LengthyComOperation(LONG nTimeout)
{
    // TODO: Add your implementation code here
    Sleep(nTimeout);
    return S_OK;
}
