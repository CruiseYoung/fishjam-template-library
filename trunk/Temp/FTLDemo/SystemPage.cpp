// SystemPage.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "FTLDemo.h"
#include "SystemPage.h"
#include <ftlsystem.h>

// CSystemPage �Ի���

IMPLEMENT_DYNAMIC(CSystemPage, CPropertyPage)

CSystemPage::CSystemPage()
	: CPropertyPage(CSystemPage::IDD)
{

}

CSystemPage::~CSystemPage()
{
}

void CSystemPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSystemPage, CPropertyPage)
    ON_BN_CLICKED(IDC_BTN_TEMP_FPU_RC_RESET, &CSystemPage::OnBnClickedBtnTempFpuRcReset)
	ON_BN_CLICKED(IDC_BTN_SYSTEM_METRICS, &CSystemPage::OnBnClickedBtnSystemMetrics)
END_MESSAGE_MAP()


// CSystemPage ��Ϣ�������

BOOL CSystemPage::OnInitDialog()
{
    BOOL bIs64Bit = CFSystemUtil::Is64BitWindows();
    SetDlgItemText(IDC_STATIC_IS64, bIs64Bit ? TEXT("TRUE") : TEXT("FALSE"));
    return TRUE;  // return TRUE unless you set the focus to a control
}

void CSystemPage::OnBnClickedBtnTempFpuRcReset()
{
    double f = 1.0 / 13.0;
    TRACE(TEXT("f = %f\n"),f);

    CFTempFpuRcReset rcReset;
    f = 1.0 / 13.0;
    TRACE(TEXT("f = %f\n"),f);
}

void CSystemPage::OnBnClickedBtnSystemMetrics()
{
	CFSystemMetricsProperty	systemMetricsProperty;
	FTLTRACE(TEXT("SystemMetricsProperty = %s\n"), systemMetricsProperty.GetPropertyString());
}
