#pragma once


// CCrashHandlerPage �Ի���

class CCrashHandlerPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CCrashHandlerPage)

public:
	CCrashHandlerPage();
	virtual ~CCrashHandlerPage();

// �Ի�������
	enum { IDD = IDD_PAGE_CRASH_HANDLER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedBtnCrash();
    afx_msg void OnBnClickedBtnDisResourcelessDlg();
};
