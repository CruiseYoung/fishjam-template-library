#pragma once

#include <ftlNLS.h>

// CNLSPage �Ի���

class CNLSPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CNLSPage)

public:
	CNLSPage();
	virtual ~CNLSPage();

// �Ի�������
	enum { IDD = IDD_PAGE_NLS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
private:
    void SetButtonStatus(BOOL bServerEnabled);
public:
};
