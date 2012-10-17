// StorageFileStructureView.cpp : CStorageFileStructureView ���ʵ��
//

#include "stdafx.h"
#include "StorageFile.h"

#include "StorageFileDoc.h"
#include "StorageFileStructureView.h"
#include ".\storagefilestructureview.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CStorageFileStructureView

IMPLEMENT_DYNCREATE(CStorageFileStructureView, CTreeView)

BEGIN_MESSAGE_MAP(CStorageFileStructureView, CTreeView)
END_MESSAGE_MAP()

// CStorageFileStructureView ����/����

CStorageFileStructureView::CStorageFileStructureView()
{
    m_bSubclassed = FALSE;
}

CStorageFileStructureView::~CStorageFileStructureView()
{
}

BOOL CStorageFileStructureView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸� CREATESTRUCT cs ���޸Ĵ������
	// ��ʽ

	return CTreeView::PreCreateWindow(cs);
}

// CStorageFileStructureView ����

void CStorageFileStructureView::OnDraw(CDC* pDC)
{
	CStorageFileDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

    //pDC->DrawText(TEXT("CStorageFileStructureView"),-1,NULL,DT_CENTER|DT_VCENTER);
	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
}


// CStorageFileStructureView ���

#ifdef _DEBUG
void CStorageFileStructureView::AssertValid() const
{
	CTreeView::AssertValid();
}

void CStorageFileStructureView::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}

CStorageFileDoc* CStorageFileStructureView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CStorageFileDoc)));
	return (CStorageFileDoc*)m_pDocument;
}
#endif //_DEBUG


// CStorageFileStructureView ��Ϣ�������

void CStorageFileStructureView::OnInitialUpdate()
{
    BOOL bRet = FALSE;
    CTreeView::OnInitialUpdate();

    //if(TRUE == GetTreeCtrl().IsWindowVisible()) 
    //{ 
    //    Detach(); 
    //    API_VERIFY(m_StorageTreeCtrl.SubclassWindow(GetTreeCtrl().GetSafeHwnd())); 
    //    //InitTree(); 
    //} 

    //CTreeCtrl& tree = GetTreeCtrl(); // �õ�CTreeCtrl���������V4Eitug
    //HWND hwndTreeCtrl = tree.m_hWnd; // ��¼�����ľ�����������ϻ��õ�
    //tree.UnsubclassWindow(); // ��Ϊ��������Ѿ������໯���ģ���Spy++���Կ���������Ҫ�Ƚ���UnsubclassWindow
    //m_StorageTreeCtrl.SubclassWindow(hwndTreeCtrl); // ����ʵ���ҵ����໯
    //m_bSubclassed = TRUE; // ���ñ�־
    //// ����������Ҫ������ܶ�API���û���ΪCMyTreeView�ڲ��Ŀؼ����ھ����Ч
    //m_hWnd = m_StorageTreeCtrl.m_hWnd; // ����CMyTreeView��m_hWnd��Ա����

}


