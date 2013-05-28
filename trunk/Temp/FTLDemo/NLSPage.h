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
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
private:
	BOOL _InitControls();
	BOOL _RefreshCodePages(DWORD dwFlags);
	BOOL _TestConverion();
public:
	CComboBox	m_cmbCodePages;
	static BOOL CALLBACK _InnerCodePageEnumProc(LPTSTR lpCodePageString);
private:
public:
	afx_msg void OnBnClickedRadioCodepageSupported();
	afx_msg void OnBnClickedRadioCodepageInstalled();
};
