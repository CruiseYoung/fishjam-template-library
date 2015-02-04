#include "StdAfx.h"
#include "TimerSink.h"

#ifdef _DEBUG
    #define new DEBUG_NEW
#endif


CTimerSink::CTimerSink(void)
{
    m_pWndResult = NULL;
}

CTimerSink::~CTimerSink(void)
{
}

void CTimerSink::SetResultWnd(CWnd * pEdit)
{
    m_pWndResult = pEdit;
}

STDMETHODIMP CTimerSink::QueryInterface(const struct _GUID &iid,void ** ppv)
{
    *ppv=this;
    return S_OK;
}

STDMETHODIMP_(ULONG) CTimerSink::AddRef(void)
{	return 1;	}

STDMETHODIMP_(ULONG) CTimerSink::Release(void)
{	return 0;	}

STDMETHODIMP CTimerSink::GetTypeInfoCount(unsigned int *)
{	return E_NOTIMPL;	}

STDMETHODIMP CTimerSink::GetTypeInfo(unsigned int,unsigned long,struct ITypeInfo ** )
{	return E_NOTIMPL;	}

STDMETHODIMP CTimerSink::GetIDsOfNames(const IID &,LPOLESTR *,UINT,LCID,DISPID *)
{	return E_NOTIMPL;	}

STDMETHODIMP CTimerSink::Invoke(
                               long dispID,
                               const struct _GUID &,
                               unsigned long,
                               unsigned short,
struct tagDISPPARAMS * pParams,
struct tagVARIANT *,
struct tagEXCEPINFO *,
    unsigned int *)
{
    switch(dispID)
    {
    case 1:	//���ݲ�ͬ��dispID,��ɲ�ͬ�Ļص�����
        if( !m_pWndResult )
        {
            AfxMessageBox( _T("û�е��� SetResultWnd() ������ʾ����Ĵ���") );
        }
        else
        {
            COleDateTime t(pParams->rgvarg[0]);
            CString str = t.Format( _T("%H:%M:%S") );
            m_pWndResult->SetWindowText( str );
        }
        break;
    default:
        AfxMessageBox( _T("��ô���ܣ�������û���������ĺ���ѽ") );
        break;
    }
    return S_OK;
}
