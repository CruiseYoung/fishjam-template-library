// MyTimer.h : Declaration of the CMyTimer

#pragma once
#include "resource.h"       // main symbols

#include "ATLCom.h"
#include "_IMyTimerEvents_CP.h"


// CMyTimer

class ATL_NO_VTABLE CMyTimer : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CMyTimer, &CLSID_MyTimer>,
    //ͨ�����ӵ����Զ�����ļ̳��б�
    public IConnectionPointContainerImpl<CMyTimer>,
    //public IProvideClassInfo2Impl<&CLSID_MyTimer,&DIID__IMyTimerEvents>,  //ʵ�����ӵ�ʱ��ͨ��ʵ�ָýӿڣ�����ͻ��˻����Advise
	public CProxy_IMyTimerEvents<CMyTimer>, 
	public IDispatchImpl<IMyTimer, &IID_IMyTimer, &LIBID_ATLComLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CMyTimer()
	{
        m_idTimer = 0;
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MYTIMER)


BEGIN_COM_MAP(CMyTimer)
	COM_INTERFACE_ENTRY(IMyTimer)
	COM_INTERFACE_ENTRY(IDispatch)
    COM_INTERFACE_ENTRY(IConnectionPointContainer)      //���ӵ����������ڲ������ӵ�
END_COM_MAP()

//�����ӵ��򵼼�������ӵ�ӳ��
BEGIN_CONNECTION_POINT_MAP(CMyTimer)
	CONNECTION_POINT_ENTRY(__uuidof(_IMyTimerEvents))
END_CONNECTION_POINT_MAP()

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}
	
	void FinalRelease() 
	{
	}
private:
    // ���� TIMER ��ID�ţ�ΪKillTimer()��
    UINT_PTR m_idTimer;

public:
    //���ӵ�Ĳ��Ժ���
    STDMETHOD(SetTimer)(LONG uElapse);
    STDMETHOD(KillTimer)(void);
};

OBJECT_ENTRY_AUTO(__uuidof(MyTimer), CMyTimer)
