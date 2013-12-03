#pragma once


// CSystemPage �Ի���

class CSystemPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CSystemPage)

public:
	CSystemPage();
	virtual ~CSystemPage();

// �Ի�������
	enum { IDD = IDD_PAGE_SYSTEM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
    virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedBtnTempFpuRcReset();
	afx_msg void OnBnClickedBtnSystemMetrics();
    afx_msg void OnBnClickedBtnCreateProcessAsUser();
    afx_msg void OnBnClickedBtnCreateProcessAndWaitAllChild();
    afx_msg void OnBnClickedBtnGetRegValueExportString();
    afx_msg void OnBnClickedBtnTestOpenReg();
    afx_msg void OnBnClickedBtnCheckRunningOnVirtualBox();
    afx_msg void OnBnClickedBtnCheckRunningOnVirtualPC();
    afx_msg void OnBnClickedBtnCheckRunningOnVMWare();
    afx_msg void OnBnClickedBtnCheckRunningMachineType();


};
