// StorageFileDoc.cpp :  CStorageFileDoc ���ʵ��
//

#include "stdafx.h"
#include "StorageFile.h"

#include "StorageFileDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CStorageFileDoc

IMPLEMENT_DYNCREATE(CStorageFileDoc, CDocument)

BEGIN_MESSAGE_MAP(CStorageFileDoc, CDocument)
END_MESSAGE_MAP()


// CStorageFileDoc ����/����

CStorageFileDoc::CStorageFileDoc()
{
	// TODO: �ڴ����һ���Թ������

}

CStorageFileDoc::~CStorageFileDoc()
{
}

BOOL CStorageFileDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)

	return TRUE;
}




// CStorageFileDoc ���л�

void CStorageFileDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: �ڴ���Ӵ洢����
	}
	else
	{
		// TODO: �ڴ���Ӽ��ش���
	}
}


// CStorageFileDoc ���

#ifdef _DEBUG
void CStorageFileDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CStorageFileDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CStorageFileDoc ����
