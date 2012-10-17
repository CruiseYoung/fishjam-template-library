// MainFrm.cpp : CMainFrame ���ʵ��
//

#include "stdafx.h"
#include "StorageFile.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // ״̬��ָʾ��
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};


// CMainFrame ����/����

CMainFrame::CMainFrame()
{
	// TODO: �ڴ���ӳ�Ա��ʼ������
}

CMainFrame::~CMainFrame()
{
}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("δ�ܴ���������\n");
		return -1;      // δ�ܴ���
	}
	//if (!m_wndDlgBar.Create(this, IDR_MAINFRAME, 
	//	CBRS_ALIGN_TOP, AFX_IDW_DIALOGBAR))
	//{
	//	TRACE0("δ�ܴ����Ի���\n");
	//	return -1;		// δ�ܴ���
	//}

	if (!m_wndReBar.Create(this)
		  || !m_wndReBar.AddBar(&m_wndToolBar)
		//|| !m_wndReBar.AddBar(&m_wndDlgBar)
        )
	{
		TRACE0("δ�ܴ��� Rebar\n");
		return -1;      // δ�ܴ���
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("δ�ܴ���״̬��\n");
		return -1;      // δ�ܴ���
	}
	// TODO: �������Ҫ������ʾ���򽫴��Ƴ�
	m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY);

	return 0;
}

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT /*lpcs*/,
	CCreateContext* pContext)
{
    if (!m_wndSplitter.CreateStatic(this, 1, 2))
        return FALSE;

    if (!m_wndSplitter.CreateView(0, 0, pContext->m_pNewViewClass, 
        CSize(100, 100), pContext) ||
        !m_wndSplitter.CreateView(0, 1, RUNTIME_CLASS(CHexEditView), 
        CSize(0, 0), pContext))
    {
        m_wndSplitter.DestroyWindow();
        return FALSE;
    }
    return TRUE;

}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: �ڴ˴�ͨ���޸� CREATESTRUCT cs ���޸Ĵ������
	// ��ʽ

	return TRUE;
}


// �Զ���Ļ�ȡ���ں���
CStorageFileStructureView* CMainFrame::GetStorageStructurePane()
{
    CWnd* pWnd = ((CMainFrame*)AfxGetMainWnd())->m_wndSplitter.GetPane(0, 0);
    CStorageFileStructureView* pView = DYNAMIC_DOWNCAST(CStorageFileStructureView, pWnd);
    return pView;
}
CHexEditView* CMainFrame::GetHexPane()
{
    CWnd* pWnd = ((CMainFrame*)AfxGetMainWnd())->m_wndSplitter.GetPane(0, 1);
    CHexEditView* pView = DYNAMIC_DOWNCAST(CHexEditView, pWnd);
    return pView;
}

// CMainFrame ���

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG


// CMainFrame ��Ϣ�������

