// StorageFileDoc.h :  CStorageFileDoc ��Ľӿ�
//


#pragma once

class CStorageFileDoc : public CDocument
{
protected: // �������л�����
	CStorageFileDoc();
	DECLARE_DYNCREATE(CStorageFileDoc)

// ����
public:

// ����
public:

// ��д
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// ʵ��
public:
	virtual ~CStorageFileDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
};


