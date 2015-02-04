// ComCallerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ComCaller.h"
#include "ComCallerDlg.h"

#include "StorageFileUtil.h"
#include "..\ATLCom\ATLCom_i.c"
#include ".\comcallerdlg.h"
#include "..\AtlServiceDemo\AtlServiceDemo_i.h"
#include "..\AtlServiceDemo\AtlServiceDemo_i.c"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//����ú�Ļ�����ͨ�� CoMarshal ����ָ�룬�������߳��л�õĽӿ�ʵ�����Ǵ���Ľӿ�(��ַ��һ��)
#define DEMO_COM_MARSHAL

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CComCallerDlg dialog

CComCallerDlg::CComCallerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CComCallerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    m_dwCallbackCookie = 0;
    m_dwCPCookie = 0;
}

void CComCallerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CComCallerDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
    ON_BN_CLICKED(IDC_BTN_VIEW_STORAGE_FILE, OnBnClickedBtnViewStorageFile)
    ON_BN_CLICKED(IDC_BTN_DRAW_PIC, OnBnClickedBtnDrawPic)
    ON_BN_CLICKED(IDC_BTN_CALL_SERVICE_EXE_MULTI, OnBnClickedBtnCallServiceExeMulti)

    ON_BN_CLICKED(IDC_BTN_CALL_MYMATH_ADD, OnBnClickedBtnCallMymathAdd)
    ON_BN_CLICKED(IDC_BTN_CALL_MYMATH2_MUL, OnBnClickedBtnCallMymath2Mul)
    ON_WM_CLOSE()
    ON_BN_CLICKED(IDC_BTN_VIEW_DISPATCH, OnBnClickedBtnViewDispatch)
    ON_BN_CLICKED(IDC_BTN_IMPORT_UPPER, OnBnClickedBtnImportUpper)
    ON_BN_CLICKED(IDC_BTN_DISPATCH_API, OnBnClickedBtnDispatchApi)
    ON_BN_CLICKED(IDC_BTN_USE_CCOMDISPATCHDRIVER, OnBnClickedBtnUseCcomdispatchdriver)
    ON_BN_CLICKED(IDC_BTN_ERROR_HANDLE, OnBnClickedBtnErrorHandle)
    ON_BN_CLICKED(IDC_BTN_SUB_AND_CALLBACK, OnBnClickedBtnSubAndCallback)
    ON_BN_CLICKED(IDC_BTN_TIMER_CONNECTPOINT, OnBnClickedBtnTimerConnectpoint)
    ON_BN_CLICKED(IDC_BTN_PERSIST, OnBnClickedBtnPersist)
    ON_BN_CLICKED(IDC_BTN_DEMO_MARSHAL, OnBnClickedBtnDemoMarshal)
    ON_WM_DESTROY()
    ON_BN_CLICKED(IDC_BTN_SUM_ARRAY, OnBnClickedBtnSumArray)
    ON_BN_CLICKED(IDC_BTN_GET_COMPONENT_INFO, OnBnClickedBtnGetComponentInfo)
    ON_BN_CLICKED(IDC_BTN_DETECT_INTERFACE, OnBnClickedBtnDetectInterface)
    ON_BN_CLICKED(IDC_BTN_MONIKER, OnBnClickedBtnMoniker)
    ON_BN_CLICKED(IDC_BTN_WIA, OnBnClickedBtnWia)
    ON_BN_CLICKED(IDC_BTN_LENGTHY_COM_OPERATION, OnBnClickedBtnLengthyComOperation)
    ON_BN_CLICKED(IDC_BTN_EVENT_HOOK, &CComCallerDlg::OnBnClickedBtnEventHook)
END_MESSAGE_MAP()

// �����ٵľͿ��ԣ���Ϊ������������ڳ������ǰ�ǲ����˳���
STDMETHODIMP_(ULONG) CComCallerDlg::AddRef()
{
    return 1;
}

STDMETHODIMP_(ULONG) CComCallerDlg::Release()
{
    return 0;
}

STDMETHODIMP CComCallerDlg::QueryInterface(const struct _GUID &iid,void ** ppv)
{
    *ppv=this;
    return S_OK;
}

STDMETHODIMP CComCallerDlg::Fire_Result(LONG nResult)
{
    CString str;
    str.Format(TEXT("Com Sub Fire_Result: %d"),nResult);
    SetWindowText(str);
    return S_OK;
}

// CComCallerDlg message handlers

BOOL CComCallerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

    HRESULT hr = E_FAIL;
    COM_VERIFY(m_spMyMath2.CoCreateInstance(CLSID_MyMath));
    COM_VERIFY(m_spMyTimer.CoCreateInstance(CLSID_MyTimer));

    m_spMyMath2->MyAdvise(this,&m_dwCallbackCookie);
    //COM_DETECT_INTERFACE_FROM_LIST(m_spMyMath2);
    //COM_DETECT_INTERFACE_FROM_LIST(m_spMyTimer);

    CComPtr<IUnknown> spUnk;
    COM_VERIFY(spUnk.CoCreateInstance(CLSID_ComActiveXObj));
    if (spUnk)
    {
        //COM_DETECT_INTERFACE_FROM_REGISTER(spUnk);
        //COM_DETECT_INTERFACE_FROM_LIST(spUnk);
    }

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CComCallerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CComCallerDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CComCallerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CComCallerDlg::OnClose()
{
    CDialog::OnClose();
}

void CComCallerDlg::OnBnClickedBtnViewStorageFile()
{
    //::CoInitialize(NULL);	// COM ��ʼ��
    // �����MFC���򣬿���ʹ��AfxOleInit()���


    CStorageFileUtil storage;
    storage.CreateDemoStorageFile(TEXT("c:\\a.stg"));
    storage.DumpStorageFileInfo(TEXT("c:\\a.stg"));

    //CoUninitialize();
}

void CComCallerDlg::OnBnClickedBtnDrawPic()
{
    ::CoInitialize(NULL);  // COM ��ʼ��
    HRESULT hr;
    CFile file;
    CWnd* pBtnDraw = GetDlgItem(IDC_BTN_DRAW_PIC);
    _ASSERT(pBtnDraw);

    CDC* pDC = pBtnDraw->GetDC();

    file.Open( TEXT(".\\res\\ComCaller.ico"), CFile::modeRead | CFile::shareDenyNone );  // �����ļ�����
    DWORD dwSize = (DWORD)file.GetLength();
    HGLOBAL hMem = ::GlobalAlloc( GMEM_MOVEABLE, dwSize );
    LPVOID lpBuf = ::GlobalLock( hMem );
    file.Read( lpBuf, dwSize );
    file.Close();
    ::GlobalUnlock( hMem );

    IStream * pStream = NULL;
    IPicture * pPicture = NULL;

    // �� HGLOBAL �õ� IStream������ TRUE ��ʾ�ͷ� IStream ��ͬʱ���ͷ��ڴ�
    hr = ::CreateStreamOnHGlobal( hMem, TRUE, &pStream );
    ASSERT ( SUCCEEDED(hr) );

    hr = ::OleLoadPicture( pStream, dwSize, TRUE, IID_IPicture, ( LPVOID * )&pPicture );
    ASSERT(hr==S_OK);

    long nWidth,nHeight;  // ��ߣ�MM_HIMETRIC ģʽ����λ��0.01����
    pPicture->get_Width( &nWidth );    // ��
    pPicture->get_Height( &nHeight );  // ��

    ////////ԭ����ʾ//////
    //CSize sz( nWidth, nHeight );
    //pDC->HIMETRICtoDP( &sz );  // ת�� MM_HIMETRIC ģʽ��λΪ MM_TEXT ���ص�λ
    //pPicture->Render(pDC->m_hDC,0,0,sz.cx,sz.cy,
    //    0,nHeight,nWidth,-nHeight,NULL);

    ////////����ť���ڳߴ���ʾ////////
    CRect rect;	
    pBtnDraw->GetClientRect(&rect);
    pPicture->Render(pDC->m_hDC,0,0,rect.Width(),rect.Height(),
    	0,nHeight,nWidth,-nHeight,NULL);

    if ( pPicture ) pPicture->Release();// �ͷ� IPicture ָ��
    if ( pStream ) pStream->Release();  // �ͷ� IStream ָ�룬ͬʱ�ͷ��� hMem

    ::CoUninitialize();

    pBtnDraw->ReleaseDC(pDC);
}

void CComCallerDlg::OnBnClickedBtnCallServiceExeMulti()
{
    HRESULT hr = E_FAIL;
    CComPtr<IAtlServiceDemoObject> spServiceDemoObject;
    COM_VERIFY(spServiceDemoObject.CoCreateInstance(CLSID_AtlServiceDemoObject));
    LONG result = 0;
    if(spServiceDemoObject)
    {
        COM_VERIFY(spServiceDemoObject->Multi(10,20,&result));
        CString str;
        str.Format(TEXT("Service Exe::Multi -- 10 * 20 = %d"), result);
        AfxMessageBox(str);
    }
}

void CComCallerDlg::OnBnClickedBtnCallMymathAdd()
{
    HRESULT hr = E_FAIL;

    CComPtr<IMyMath> spMyMath;
    COM_VERIFY(spMyMath.CoCreateInstance(CLSID_MyMath));
    LONG result = 0;
    if (spMyMath)
    {
        COM_VERIFY(spMyMath->Add(10,20,&result));

        CString str;
        str.Format(TEXT("MyMath::Add -- 10 + 20 = %d"),result);
        AfxMessageBox(str);
    }
}

void CComCallerDlg::OnBnClickedBtnCallMymath2Mul()
{
    HRESULT hr = E_FAIL;
    CComPtr<IMyMath>    spMyMath;
    CComQIPtr<IMyMath2> spMyMath2;
    COM_VERIFY(spMyMath.CoCreateInstance(CLSID_MyMath));
    spMyMath2 = spMyMath;

    if (spMyMath && spMyMath2)
    {
        LONG resultAdd = 0;
        LONG resultMul = 0;

        COM_VERIFY(spMyMath->Add(10,20,&resultAdd));
        COM_VERIFY(spMyMath2->Mul(10,20,&resultMul));

        CString str;
        str.Format(TEXT("MyMath2::Add -- 10 + 20 = %d\r\nMyMath2::Mul -- 10 * 20 = %d"),
            resultAdd,resultMul);
        AfxMessageBox(str);

        spMyMath.Release();
        spMyMath2.Release();
    }
}

void CComCallerDlg::OnBnClickedBtnViewDispatch()
{
    CComPtr<IMyMath> spMyMath;
    spMyMath.CoCreateInstance(CLSID_MyMath);
    LONG l = 0;
    spMyMath->Add(10,20,&l);

    //HRESULT hr = E_FAIL;
    //CFileDialog dlg(TRUE,NULL,NULL,OFN_HIDEREADONLY,TEXT("Executable File(*.exe;*.tlb)|*.exe;*.tlb|All Files(*.*)|*.*"));
    //if (dlg.DoModal() == IDOK)
    //{
    //}
}

//ʹ�� import �������Ϳ�
#ifdef _DEBUG
    #import "..\ATLCom\Debug\ATLCom.tlb"
#else
    #import "..\ATLCom\Release\ATLCom.tlb"
#endif

void CComCallerDlg::OnBnClickedBtnImportUpper()
{
    HRESULT hr = E_FAIL;
    ATLComLib::IMyDispatchPtr spMyDispathc;
    //COM_VERIFY(spMyDispathc.CreateInstance(CLSID_MyMath));
    COM_VERIFY(spMyDispathc.CreateInstance(TEXT("ATLCom.MyMath")));

    _bstr_t  bstrInput(TEXT("hello"));
    _bstr_t  bstrOutput;
    bstrOutput = spMyDispathc->Upper(bstrInput);
    USES_CONVERSION;

    CString str;
    str.Format(TEXT("Upper('hello') = %s"),OLE2CT(bstrOutput));
    AfxMessageBox(str);
}

void CComCallerDlg::OnBnClickedBtnDispatchApi()
{
    CLSID clsid;				// ͨ�� ProgID �õ� CLSID
    HRESULT hr = E_FAIL;
    COM_VERIFY(::CLSIDFromProgID( L"ATLCom.MyMath", &clsid ));
    ASSERT( SUCCEEDED( hr ) );	// ���ʧ�ܣ�˵��û��ע�����

    IDispatch * pDisp = NULL;	// �� CLSID ������������õ� IDispatch ָ��
    COM_VERIFY(::CoCreateInstance( clsid, NULL, CLSCTX_ALL, IID_IDispatch, (LPVOID *)&pDisp ));

    LPOLESTR pwFunName = L"Upper";	// ׼��ȡ�� Upper ��������� DispID
    DISPID dispID;					// ȡ�õ���ţ�׼�����浽����
    COM_VERIFY(pDisp->GetIDsOfNames(		// ���ݺ�������ȡ����ŵĺ���
        IID_NULL,
        &pwFunName,					// �������Ƶ�����
        1,							// �������������е�Ԫ�ظ���
        LOCALE_SYSTEM_DEFAULT,		// ʹ��ϵͳĬ�ϵ����Ի���
        &dispID ));					// ����ֵ
    ASSERT( SUCCEEDED( hr ) );		// ���ʧ�ܣ�˵�����������û�� ADD ����

    VARIANTARG v[1];					// ���� Upper("hello") ��������Ҫ�Ĳ���
    v[0].vt = VT_BSTR;	v[0].bstrVal = _bstr_t("hello");	// ��һ��������BSTR("hello")

    DISPPARAMS dispParams = { v, NULL, _countof(v), 0 };	// �Ѳ�����װ������ṹ��
    VARIANT vResult;			// �������صļ�����

    hr = pDisp->Invoke(			// ���ú���
        dispID,					// ������ dispID ָ��
        IID_NULL,
        LOCALE_SYSTEM_DEFAULT,	// ʹ��ϵͳĬ�ϵ����Ի���
        DISPATCH_METHOD,		// ���õ��Ƿ�������������
        &dispParams,			// ����
        &vResult,				// ����ֵ
        NULL,					// �������쳣����
        NULL);					// �����Ǵ�����
    ASSERT( SUCCEEDED( hr ) );	// ���ʧ�ܣ�˵���������ݴ���
    ASSERT(vResult.vt == VT_BSTR);

    USES_CONVERSION;
    CString str;			// ��ʾһ�½��
    str.Format(TEXT("Upper('hello') = %s"), OLE2T(vResult.bstrVal));
    AfxMessageBox( str );

    pDisp->Release();		// �ͷŽӿ�ָ��
    
}

void CComCallerDlg::OnBnClickedBtnUseCcomdispatchdriver()
{
    CLSID clsid;				// ͨ�� ProgID ȡ������� CLSID
    HRESULT hr = ::CLSIDFromProgID( L"ATLCom.MyMath", &clsid );
    ASSERT( SUCCEEDED( hr ) );	// ���ʧ�ܣ�˵��û��ע�����

    CComPtr < IUnknown > spUnk;	// �� CLSID �����������ȡ�� IUnknown ָ��
    hr = ::CoCreateInstance( clsid, NULL, CLSCTX_ALL, IID_IUnknown, (LPVOID *)&spUnk );
    ASSERT( SUCCEEDED( hr ) );

    CComDispatchDriver spDisp( spUnk );	// ���� IDispatch ������ָ��(ʵ�ʾ��� CComQIPtr<IDispatch> )
    CComVariant v1("hello"), vResult;	// ����
    hr = spDisp.Invoke1(	// ����2�������ĺ���
        L"Upper",			// �������� Add
        &v1,				// ��һ��������ֵΪ"hello"
        &vResult);			// ����ֵ
    ASSERT( SUCCEEDED( hr ) );	// ���ʧ�ܣ�˵������û�� ADD ���������߲�������
    ASSERT(vResult.vt == VT_BSTR);
    
    spDisp.Release();

    USES_CONVERSION;
    CString str;			// ��ʾһ�½��
    str.Format(TEXT("Upper('hello') = %s"), OLE2T(vResult.bstrVal));
    AfxMessageBox( str );

}

void CComCallerDlg::OnBnClickedBtnErrorHandle()
{
    HRESULT hr = E_FAIL;
    CComPtr<IMyMath2>   spMyMath2;
    COM_VERIFY(spMyMath2.CoCreateInstance(CLSID_MyMath));
    COM_VERIFY(spMyMath2->Mul(10,20,NULL));
    if (FAILED(hr))
    {
        CComQIPtr < ISupportErrorInfo > spSEI = spMyMath2;	// ����Ƿ��ṩ�� ISupportErrorInfo �ӿڣ�
        if( spSEI )	// ���֧�֣���ô
        {
            COM_VERIFY(spSEI->InterfaceSupportsErrorInfo( IID_IMyMath2 ));	// �Ƿ�֧�� Ixxx �ӿڵĴ�����
            if( SUCCEEDED( hr ) )
            {	// ֧�֣�̫���ˡ�ȡ��������Ϣ
                CComQIPtr < IErrorInfo > spErrInfo;		// ���� IErrorInfo �ӿ�
                hr = ::GetErrorInfo( 0, &spErrInfo );	// ȡ�ýӿ�
                if( SUCCEEDED( hr ) )
                {
                    CComBSTR bstrDes;
                    spErrInfo->GetDescription( &bstrDes );	// ȡ�ô�������
                    AfxMessageBox(_bstr_t(bstrDes));
                    //......	// ������ȡ����������Ϣ
                    
                }
            }
        }
    }
}


void CComCallerDlg::OnBnClickedBtnSubAndCallback()
{
    //������Ҫͨ���ص��ķ�ʽ���أ���ò�Ҫʹ�þֲ������ķ�ʽ����COM���󣬷��������Ϊ�����ڳ�����
    HRESULT hr = E_FAIL;
    SetWindowText(TEXT(""));
    COM_VERIFY(m_spMyMath2->Sub(20,10));
}


//���ӵ��ʾ������
void CComCallerDlg::OnBnClickedBtnTimerConnectpoint()
{
    HRESULT hr = E_FAIL;
    
    CComQIPtr<IConnectionPointContainer> spContainer( m_spMyTimer );
    ASSERT( spContainer );		// ���û���ṩ���ӵ㹦��

    if (!m_spCPTimer)
    {
        COM_VERIFY(spContainer->FindConnectionPoint(__uuidof(_IMyTimerEvents), &m_spCPTimer ));
        ASSERT( m_spCPTimer );			// û���ҵ����ӵ�ӿ�

        COM_VERIFY(m_spCPTimer->Advise( &m_TimerSink, &m_dwCPCookie ));
        ASSERT( SUCCEEDED( hr ) );	// ����ʧ��
        m_TimerSink.SetResultWnd( this );
        COM_VERIFY(m_spMyTimer->SetTimer(1000));
    }
    else
    {
        COM_VERIFY(m_spMyTimer->KillTimer());
        COM_VERIFY(m_spCPTimer->Unadvise(m_dwCPCookie));
        m_spCPTimer.Release();
    }

}

void CComCallerDlg::OnBnClickedBtnPersist()
{
    //���Ҫ���桢�ָ��Ļ���ʹ�� CStorageFileUtil �ķ��������� IStorage���������е� IStream �ж�д�־û���Ϣ
    HRESULT hr = E_FAIL;
    LONG lVal = 0;
    COM_VERIFY(m_spMyMath2->GetNext(&lVal));
    CComQIPtr<IPersistStreamInit>    spPSI = m_spMyMath2;
    ASSERT(spPSI);
    //spPSI->Save(..);
    //spPSI->Load(...);

    SetDlgItemInt(IDC_BTN_PERSIST,lVal);
}

DWORD ApartMentMsgWaitForMultipleObject(HANDLE *hHandle,DWORD dwWaitCout, DWORD dwMilliseconds)
{
    BOOL bQuit = FALSE;
    DWORD dwRet;

    while(!bQuit)
    {
        int rc;
        rc = ::MsgWaitForMultipleObjects(dwWaitCout,//��Ҫ�ȴ��Ķ�������
            hHandle,// ��������
            FALSE, //�ȴ����еĶ���
            (DWORD)dwMilliseconds,//�ȴ���ʱ��
            (DWORD)(QS_ALLINPUT | QS_ALLPOSTMESSAGE)//�¼�����
            );
        //�ȴ����¼�����
        if( rc ==  WAIT_OBJECT_0 )
        {
            dwRet = rc;
            bQuit = TRUE;
        }
        //����windows��Ϣ
        else if( rc == WAIT_OBJECT_0 + dwWaitCout )			
        {
            MSG msg;
            while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
            {
                TranslateMessage (&msg);
                DispatchMessage(&msg);
            }			
        } 
    }
    return dwRet;
}

DWORD WINAPI MyThreadProc( LPVOID lpThreadParameter )
{
    /*******************************************************************************************************
    * ���Խ���
    *   �μ� ComThreadingModel.h �еġ����ʵ����
    *   
    * ���Բ��� -- ע�⣺ThreadingModel ������ CComXXXThreadModel������
    *   һ��ThreadingModelΪ Apartment(CComSingleThreadModel)
    *     1.���̺߳����̶߳��� COINIT_APARTMENTTHREADED -- �� CoInitialize(NULL);
    *       a.������ DEMO_COM_MARSHAL ��ʱ(ֱ�Ӵ���) -- �������룬    ��ַ��ͬ��  ��ͬ�߳� <== ����Υ����STA��ģ��
    *       b.���� DEMO_COM_MARSHAL ��(����Marshal)  -- ��������ִ�У���ַ����ͬ��ͬһ�߳�
    *     2.���̺߳����̶߳��� COINIT_MULTITHREADED
    *       a.������ DEMO_COM_MARSHAL ��ʱ(ֱ�Ӵ���) -- ��������ִ�У���ַ��ͬ��  ͬһ�߳� <== ��ʱ�����߳���ͬһMTA�У�Marshalֻ�Ǽ򵥴���ָ��
    *       b.���� DEMO_COM_MARSHAL ��(����Marshal)  -- ��������ִ�У���ַ��ͬ��  ͬһ�߳�
    *     3.���߳��� COINIT_APARTMENTTHREADED(STA) �����߳��� COINIT_MULTITHREADED(MTA)
    *       a.������ DEMO_COM_MARSHAL ��ʱ(ֱ�Ӵ���) -- �������룬    ��ַ��ͬ��  ��ͬ�߳�
    *       b.���� DEMO_COM_MARSHAL ��(����Marshal)  -- ��������ִ�У���ַ����ͬ��ͬһ�߳�
    *     4.���߳��� COINIT_MULTITHREADED(MTA)�����߳��� COINIT_APARTMENTTHREADED(STA)
    *       a.������ DEMO_COM_MARSHAL ��ʱ(ֱ�Ӵ���) -- ��ַ��ͬ�����̵߳���ʱ����RPC_E_WRONG_THREAD(0x8001010E)
    *           <== ��STA�̲߳��ܵ��������߳�(MTA)�����Ķ���
    *       b.���� DEMO_COM_MARSHAL ��(����Marshal)  -- ��������ִ�У���ַ����ͬ��ͬһ�߳�
    *   ����ThreadingModelΪ Free(CComMultiThreadModel)
    *     1.���̺߳����̶߳��� COINIT_APARTMENTTHREADED -- �� CoInitialize(NULL);
    *       a.������ DEMO_COM_MARSHAL ��ʱ(ֱ�Ӵ���) -- ��ַ��ͬ�����̵߳���ʱ����RPC_E_WRONG_THREAD(0x8001010E)
    *       b.���� DEMO_COM_MARSHAL ��(����Marshal)  -- �������룬    ��ַ����ͬ����ͬ�߳�
    *     2.���̺߳����̶߳��� COINIT_MULTITHREADED
    *       a.������ DEMO_COM_MARSHAL ��ʱ(ֱ�Ӵ���) -- �������룬    ��ַ��ͬ��  ��ͬ�߳�
    *       b.���� DEMO_COM_MARSHAL ��(����Marshal)  -- �������룬    ��ַ��ͬ��  ��ͬ�߳�
    *     3.���߳��� COINIT_APARTMENTTHREADED(STA) �����߳��� COINIT_MULTITHREADED(MTA)
    *       a.������ DEMO_COM_MARSHAL ��ʱ(ֱ�Ӵ���) -- ��ַ��ͬ�����̵߳���ʱ����RPC_E_WRONG_THREAD(0x8001010E)
    *       b.���� DEMO_COM_MARSHAL ��(����Marshal)  -- �������룬    ��ַ����ͬ����ͬ�߳�
    *     4.���߳��� COINIT_MULTITHREADED(MTA)�����߳��� COINIT_APARTMENTTHREADED(STA)
    *       a.������ DEMO_COM_MARSHAL ��ʱ(ֱ�Ӵ���) -- �������룬    ��ַ��ͬ��  ��ͬ�߳�
    *       b.���� DEMO_COM_MARSHAL ��(����Marshal)  -- �������룬    ��ַ����ͬ����ͬ�߳�
    *   �ġ�ThreadingModelΪ Both(CComMultiThreadModel) -- 
    *     0.���߳�ʹ�� COINIT_APARTMENTTHREADED ʱ���������� USE_FREE_THREADED_MARSHALER ������ 
    *          Free-Threaded Marshaler ֧�֣������(Access violation reading location) -- ���⣺Vista�²���Freeʱ����
    *     1.���̺߳����̶߳��� COINIT_APARTMENTTHREADED -- �� CoInitialize(NULL);
    *       a.������ DEMO_COM_MARSHAL ��ʱ(ֱ�Ӵ���) -- 
    *       b.���� DEMO_COM_MARSHAL ��(����Marshal)  -- 
    *     2.���̺߳����̶߳��� COINIT_MULTITHREADED
    *       a.������ DEMO_COM_MARSHAL ��ʱ(ֱ�Ӵ���) -- 
    *       b.���� DEMO_COM_MARSHAL ��(����Marshal)  -- 
    *     3.���߳��� COINIT_APARTMENTTHREADED(STA) �����߳��� COINIT_MULTITHREADED(MTA)
    *       a.������ DEMO_COM_MARSHAL ��ʱ(ֱ�Ӵ���) -- 
    *       b.���� DEMO_COM_MARSHAL ��(����Marshal)  -- 
    *     4.���߳��� COINIT_MULTITHREADED(MTA)�����߳��� COINIT_APARTMENTTHREADED(STA)
    *       a.������ DEMO_COM_MARSHAL ��ʱ(ֱ�Ӵ���) -- 
    *       b.���� DEMO_COM_MARSHAL ��(����Marshal)  -- 
    *   ����ThreadingModelΪ Neutral(CComMultiThreadModel) -- ע�⣺����ʱû��ѡ�� COM+ ֧��
    *     1.���̺߳����̶߳��� COINIT_APARTMENTTHREADED -- �� CoInitialize(NULL);
    *       a.������ DEMO_COM_MARSHAL ��ʱ(ֱ�Ӵ���) -- ��ַ��ͬ�����̵߳���ʱ����RPC_E_WRONG_THREAD(0x8001010E)
    *       b.���� DEMO_COM_MARSHAL ��(����Marshal)  -- �������룬    ��ַ��ͬ��  ��ͬ�߳�
    *     2.���̺߳����̶߳��� COINIT_MULTITHREADED
    *       a.������ DEMO_COM_MARSHAL ��ʱ(ֱ�Ӵ���) -- �������룬    ��ַ��ͬ��  ��ͬ�߳�
    *       b.���� DEMO_COM_MARSHAL ��(����Marshal)  -- �������룬    ��ַ��ͬ��  ��ͬ�߳�
    *     3.���߳��� COINIT_APARTMENTTHREADED(STA) �����߳��� COINIT_MULTITHREADED(MTA)
    *       a.������ DEMO_COM_MARSHAL ��ʱ(ֱ�Ӵ���) -- ��ַ��ͬ�����̵߳���ʱ����RPC_E_WRONG_THREAD(0x8001010E)
    *       b.���� DEMO_COM_MARSHAL ��(����Marshal)  -- �������룬    ��ַ��ͬ��  ��ͬ�߳�
    *     4.���߳��� COINIT_MULTITHREADED(MTA)�����߳��� COINIT_APARTMENTTHREADED(STA)
    *       a.������ DEMO_COM_MARSHAL ��ʱ(ֱ�Ӵ���) -- ��ַ��ͬ�����̵߳���ʱ����RPC_E_WRONG_THREAD(0x8001010E)
    *       b.���� DEMO_COM_MARSHAL ��(����Marshal)  -- �������룬    ��ַ��ͬ��  ��ͬ�߳�
    *******************************************************************************************************/
    HRESULT hr = E_FAIL;
    COM_VERIFY(CoInitializeEx(NULL,COINIT_MULTITHREADED));
    {
        Sleep(1000);

        CComPtr<IMyMath2> spMyMath2;
#ifdef DEMO_COM_MARSHAL
        IStream* pStream = (IStream*)lpThreadParameter;
        COM_VERIFY(CoGetInterfaceAndReleaseStream(pStream,IID_IMyMath2,(void**)&spMyMath2));
#else
        spMyMath2 = (IMyMath2*)lpThreadParameter;
#endif
        ATLTRACE(TEXT("Sub Thread,ThreadID=%d,CoCurProcess=%d,spMyMath = 0x%08x\n"),
            GetCurrentThreadId(), CoGetCurrentProcess(),spMyMath2);

        LONG lResult = 0;
        COM_VERIFY(spMyMath2->DemoMarshalLongOperation(CComBSTR(TEXT("Sub Thread"))));
    }
    CoUninitialize();
    return 0;
}

void CComCallerDlg::OnBnClickedBtnDemoMarshal()
{
    //��ʾ�Խӿڽ���Marshal�Ĳ��� -- Ĭ�����Ϊ���� STA
    //  �������ʹ��Marshal������ֱ�Ӵ���ָ�룬�������߳��е��� DemoMarshalLongOperation ������
    HRESULT hr = E_FAIL;
    LONG lResult = 0;

    //COM_VERIFY(CreateStreamOnHGlobal(NULL,TRUE,&spStream));
    //SHCreateMemStream

    ATLTRACE(TEXT("Main Thread,ThreadID=%d,CoCurProcess=%d,spMyMath2 = 0x%08x\n"),
        GetCurrentThreadId(), CoGetCurrentProcess(),m_spMyMath2);
#ifdef DEMO_COM_MARSHAL
    IStream* pStream = NULL;        //����CoGetInterfaceAndReleaseStream���ͷţ�������ﲻ��Ҫ���ͷ��� 
    COM_VERIFY(CoMarshalInterThreadInterfaceInStream(IID_IMyMath2,m_spMyMath2,&pStream));
    HANDLE hThread = CreateThread(NULL,0,MyThreadProc,pStream,0,NULL);
#else
    HANDLE hThread = CreateThread(NULL,0,MyThreadProc,m_spMyMath2,0,NULL);
#endif

    COM_VERIFY(m_spMyMath2->DemoMarshalLongOperation(CComBSTR(TEXT("Main Thread"))));
    //WaitForSingleObject(hThread,INFINITE);  //����Wait���ʹ����Ϣѭ���޷���������˻��������� ProcessExplorer �鿴
    //ApartMentMsgWaitForMultipleObject(&hThread,1,INFINITE);  
    CloseHandle(hThread);
}

void CComCallerDlg::OnDestroy()
{
    // TODO: Add your message handler code here and/or call default
    HRESULT hr = E_FAIL;
    if (0 != m_dwCPCookie && m_spCPTimer)
    {
        m_spCPTimer->Unadvise(m_dwCPCookie);
        m_spCPTimer.Release();
        m_dwCPCookie = 0;
    }

    if (0 != m_dwCallbackCookie && m_spMyMath2)
    {
        COM_VERIFY(m_spMyMath2->MyUnAdvise(m_dwCallbackCookie));
        m_spMyMath2.Release();
    }
    m_spMyTimer.Release();

    __super::OnDestroy();

    // TODO: Add your message handler code here
}

void CComCallerDlg::OnBnClickedBtnSumArray()
{
    LONG lArray[] = {1,2,3,4,5,6,7,8,9,10};
    LONG lResult = 0;
    HRESULT hr = E_FAIL;
    COM_VERIFY(m_spMyMath2->Sum(_countof(lArray),lArray,&lResult));
    SetDlgItemInt(IDC_BTN_SUM_ARRAY,lResult,TRUE);
}

void CComCallerDlg::OnBnClickedBtnGetComponentInfo()
{
    HRESULT hr = E_FAIL;
    CComQIPtr<IComponentInfo>  spInfo = m_spMyMath2;
    if (spInfo)
    {
        COMPONENT_INFO* pInfo = NULL;
        COM_VERIFY(spInfo->GetComponentInfo(&pInfo));
        if (SUCCEEDED(hr))
        {
            USES_CONVERSION;
            CString strInfo;
            strInfo.Format(TEXT("Author = %s\n Major=%d \n Minor = %d"),
                OLE2CT(pInfo->Author),pInfo->Major,pInfo->Minor);
            SysFreeString(pInfo->Author);
            CoTaskMemFree(pInfo);
            AfxMessageBox(strInfo);
        }
    }
}

void CComCallerDlg::OnBnClickedBtnDetectInterface()
{
    HRESULT hr = E_FAIL;
    IUnknown *pTestUnk = NULL;
    hr = ::CoCreateInstance(CLSID_MyMath, NULL, CLSCTX_INPROC_SERVER,
        IID_IUnknown, (void**)&pTestUnk);
    if (pTestUnk)
    {
        TRACE(TEXT("Before COM_DETECT_INTERFACE_FROM_REGISTER\n"));
        
        COM_DETECT_INTERFACE_FROM_REGISTER(pTestUnk);

        TRACE(TEXT("Before COM_DETECT_INTERFACE_FROM_LIST\n"));
        COM_DETECT_INTERFACE_FROM_LIST(pTestUnk);

        //CComQIPtr<IWebBrowserApp> spWebApp = pTestUnk;
        ////IWebBrowserApp* spWebApp;
        //COM_VERIFY(spWebApp->put_Visible(VARIANT_TRUE));
        //CComVariant varFlags(navOpenInNewWindow);
        //CComVariant varTargetFrameName(_BLANK);
        //CComVariant varPostData()

        //�д�
        //COM_VERIFY(spWebApp->Navigate(CComBSTR("http://www.baidu.com"),&varFlags,&varTargetFrameName,NULL,NULL));
        pTestUnk->Release();
    }

    CComPtr<IUnknown> InternetExplorer;
    COM_VERIFY(InternetExplorer.CoCreateInstance(CLSID_InternetExplorer));
    COM_DETECT_INTERFACE_FROM_REGISTER(InternetExplorer);
    COM_DETECT_INTERFACE_FROM_LIST(InternetExplorer);

}

void CComCallerDlg::OnBnClickedBtnMoniker()
{
    //����֮ǰ�����COM�������Ϣ�ļ�����COM�����Ŀǰ�������⣺��ô����ɹ�ʹ�õ���Ϣ�ļ���
    HRESULT hr = E_FAIL;
    CFileDialog dlg(TRUE);
    if (dlg.DoModal() == IDOK)
    {
        USES_CONVERSION;
        CComPtr<IMoniker>   spMoniker;
        CComPtr<IMyMath2>   spMyMath2;
        //IMoniker*   spMoniker = NULL;
        //IMyMath2*     spMyMath2 = NULL;
        COM_VERIFY(CreateFileMoniker(T2COLE(dlg.GetFileName()),&spMoniker));
        if (SUCCEEDED(hr))
        {
            
            COM_VERIFY(BindMoniker(spMoniker, 0, IID_IMyMath2, (void **)&spMyMath2));
            if(SUCCEEDED(hr)) 
            {
                LONG lResult = 0;
                COM_VERIFY(spMyMath2->Add(1,2,&lResult));
                Sleep(2000);
            }
        }
    }
    

}

//WIA(Windows Image Acquisition) -- ����Ӧ�ó���ӿ�(API)Ҳ���豸��������ӿ�(DDI)
void CComCallerDlg::OnBnClickedBtnWia()
{
    
}

void CComCallerDlg::OnBnClickedBtnLengthyComOperation()
{
    //http://support.microsoft.com/kb/248019/en-us
    //���MFC�ͻ��˵���COMServer�е��߳������Ļ�������֡�OLE Server Busy���Ի���ͨ�����´�����Է�ֹ����
    //���⣺ʼ��û�г���"OLE Server Busy"�Ի���

	COleMessageFilter* pFilter = AfxOleGetMessageFilter();
    AfxOleGetMessageFilter()->SetMessagePendingDelay(1000); //set the wait period on outgoing COM calls
    //AfxOleGetMessageFilter()->EnableNotRespondingDialog(FALSE); //disable the Not Responding dialog box
    //AfxOleGetMessageFilter()->EnableBusyDialog(FALSE);//disable the busy dialog box from appearing after the COM call times out
    
    m_spMyMath2->LengthyComOperation(5000);

}

[event_source(native)]
class CMyEventSource {
public:
    __event void OnValueChanged(int nNewValue);
};

[event_receiver(native)]
class CMyEventReceiver
{
public:
    void OnValueChangedHandler(int nValue) {
        TRACE(TEXT("[%d] In CMyEventReceiver1::OnValueChangedHandler, this=%p\n"), 
            GetCurrentThreadId(),this);
    }

    void HookEvent(CMyEventSource* pSource) {
        BOOL bRet = FALSE;
        __hook(&CMyEventSource::OnValueChanged, pSource, &CMyEventReceiver::OnValueChangedHandler, this);
    }
    void UnHookEvent(CMyEventSource* pSource) {
        __unhook(&CMyEventSource::OnValueChanged, pSource, &CMyEventReceiver::OnValueChangedHandler);
    }
};


void CComCallerDlg::OnBnClickedBtnEventHook()
{
    CMyEventSource source;
    CMyEventReceiver receiver1;
    CMyEventReceiver receiver2;

    receiver1.HookEvent(&source);
    receiver2.HookEvent(&source);
    TRACE(TEXT("[%d] Before raise OnValueChanged\n"), GetCurrentThreadId());
    __raise source.OnValueChanged(123);
    TRACE(TEXT("[%d] After raise OnValueChanged\n"), GetCurrentThreadId());

    receiver2.UnHookEvent(&source);
    receiver1.UnHookEvent(&source);

}
