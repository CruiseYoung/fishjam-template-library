//StockBar.h : Declaration of the CStockBar

//***************************************************************************//
//                                                                           //
//  This file was created using the DeskBand ATL Object Wizard 2.0           //
//  By Erik Thompson 2001                                                  //
//  Email questions and comments to erikt@radbytes.com						 //
//                                                                           //
//***************************************************************************//

#ifndef __STOCKBAR_H_
#define __STOCKBAR_H_

#include "resource.h"       // main symbols
#include "ReflectionWnd.h"

#ifndef CATID_AppContainerCompatible
#include <shlguid.h>
#include <exdispid.h>
//{59FB2056-D625-48D0-A944-1A85B5AB2640}
DEFINE_GUID(CATID_AppContainerCompatible, 
            0x59fb2056,0xd625,0x48d0,0xa9,0x44,0x1a,0x85,0xb5,0xab,0x26,0x40);
#endif


/////////////////////////////////////////////////////////////////////////////
// CStockBar
class ATL_NO_VTABLE CStockBar : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CStockBar, &CLSID_StockBar>,
	public IDeskBand,
	public IObjectWithSite,
	public IInputObject, 
	public IDispatchImpl<IStockBar, &IID_IStockBar, &LIBID_MOTLEYFOOLLib>
{
public:
	CStockBar();

DECLARE_REGISTRY_RESOURCEID(IDR_STOCKBAR)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_CATEGORY_MAP(CStockBar)
	//IMPLEMENTED_CATEGORY(CATID_InfoBand)
	//IMPLEMENTED_CATEGORY(CATID_CommBand)
	//IMPLEMENTED_CATEGORY(CATID_DeskBand)
 //   IMPLEMENTED_CATEGORY(CATID_SafeForScripting)
 //   IMPLEMENTED_CATEGORY(CATID_SafeForInitializing)
    IMPLEMENTED_CATEGORY(CATID_AppContainerCompatible)
    //��һ�ַ������� StockBar.rgs ������ ����ע����Ϣ
    //'Implemented Categories'
    //{
    //    {59fb2056-d625-48d0-a944-1a85b5ab2640}
    //}
END_CATEGORY_MAP()

BEGIN_COM_MAP(CStockBar)
	COM_INTERFACE_ENTRY(IStockBar)
	COM_INTERFACE_ENTRY(IInputObject)
	COM_INTERFACE_ENTRY(IOleWindow)
	COM_INTERFACE_ENTRY_IID(IID_IDockingWindow, IDockingWindow)
	COM_INTERFACE_ENTRY(IObjectWithSite)
	COM_INTERFACE_ENTRY_IID(IID_IDeskBand, IDeskBand)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

// IDeskBand
public:
	STDMETHOD(GetBandInfo)(DWORD dwBandID, DWORD dwViewMode, DESKBANDINFO* pdbi);

// IObjectWithSite
public:
	STDMETHOD(SetSite)(IUnknown* pUnkSite);
	STDMETHOD(GetSite)(REFIID riid, void **ppvSite);

// IOleWindow
public:
	STDMETHOD(GetWindow)(HWND* phwnd);
	STDMETHOD(ContextSensitiveHelp)(BOOL fEnterMode);

// IDockingWindow
public:
	STDMETHOD(CloseDW)(unsigned long dwReserved);
	STDMETHOD(ResizeBorderDW)(const RECT* prcBorder, IUnknown* punkToolbarSite, BOOL fReserved);
	STDMETHOD(ShowDW)(BOOL fShow);

// IInputObject
public:
	STDMETHOD(HasFocusIO)(void);
	STDMETHOD(TranslateAcceleratorIO)(LPMSG lpMsg);
	STDMETHOD(UIActivateIO)(BOOL fActivate, LPMSG lpMsg);

// IStockBar
public:
	void FocusChange(BOOL bHaveFocus);

protected:
	BOOL RegisterAndCreateWindow();
protected:
	DWORD m_dwBandID;
	DWORD m_dwViewMode;
	BOOL m_bShow;
	BOOL m_bEnterHelpMode;
	HWND m_hWndParent;
	CReflectionWnd m_ReflectWnd;
	IInputObjectSite* m_pSite;
};

#endif //__STOCKBAR_H_