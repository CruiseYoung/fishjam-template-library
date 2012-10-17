// StorageFileStructureView.h : CStorageFileStructureView ��Ľӿ�
//


#pragma once
#include "StorageFileDoc.h"
#include "StorageTreeCtrl.h"

class CStorageFileStructureView : public CTreeView
{
protected: // �������л�����
	CStorageFileStructureView();
	DECLARE_DYNCREATE(CStorageFileStructureView)

// ����
public:
	CStorageFileDoc* GetDocument() const;

// ����
public:

// ��д
	public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// ʵ��
public:
	virtual ~CStorageFileStructureView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
    CStorageTreeCtrl    m_StorageTreeCtrl;
    BOOL                m_bSubclassed;
// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
public:
    virtual void OnInitialUpdate();
    //CTreeCtrl& GetTreeCtrl() const
    //{
    //    if (m_bSubclassed) 
    //        return (CTreeCtrl&)m_StorageTreeCtrl; // �����໯�ˣ������µ�����V4Eitug
    //    else 
    //        return CTreeView::GetTreeCtrl(); // ��δ���໯������Ĭ�ϴ���V4Eitug
    //}
};

#ifndef _DEBUG  // StorageFileStructureView.cpp �ĵ��԰汾
inline CStorageFileDoc* CStorageFileStructureView::GetDocument() const
   { return reinterpret_cast<CStorageFileDoc*>(m_pDocument); }
#endif

