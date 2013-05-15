#pragma once

/*************************************************************************************************
* ������ CriticalSection �� Mutex �����ܲ��100���ѭ����
* 1.���̷߳�ʽ�£�CSֻҪԼ80���룬��Mutex��ҪԼ2000����
* 2.˫�߳̽�����ʽ�£�CSֻҪԼ170���룬��Spinֵ�޹أ���Mutex��ҪԼ4000���룻
*   ���ǣ��ڹ�˾ʹ�õ�˫�˻����ϣ�CS��Ҫ 1000~5000 ���룿�����������Spinֵ��������������ʱ��
**************************************************************************************************/


// CBasePage �Ի���

class CBasePage : public CPropertyPage
{
	DECLARE_DYNAMIC(CBasePage)

public:
	CBasePage();
	virtual ~CBasePage();

// �Ի�������
	enum { IDD = IDD_PAGE_BASE };
private:
    UINT m_performanceSpinCountNumber;
    BOOL m_nPerformanceType;
    CFElapseCounter m_ElapseCounter;
    CRITICAL_SECTION        m_perCS;
    HANDLE                  m_perMutex;
    static DWORD WINAPI TestPerformanceProc(LPVOID pParam);
    static void loopForMutex(HANDLE hMutex);
    static void loopForCS(LPCRITICAL_SECTION pCS);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	DECLARE_MESSAGE_MAP()
    void SetElapseButtonStatus(FTL::RunningStatus status, BOOL bEnabled);
public:
    afx_msg void OnBnClickedBtnStart();
    afx_msg void OnBnClickedBtnPauseResume();
    afx_msg void OnBnClickedBtnGetElapsetime();
    afx_msg void OnBnClickedBtnStop();
    afx_msg void OnBnClickedBtnExeCounter();
    afx_msg void OnBnClickedBtnPerformanceStart();
    afx_msg void OnBnClickedCheckCsSpincount();
    afx_msg void OnBnClickedRadioPerformanceCs();
    afx_msg void OnBnClickedRadioPerformanceMutex();
	afx_msg void OnBnClickedBtnApiVerify();
};
