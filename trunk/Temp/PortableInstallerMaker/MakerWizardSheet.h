#pragma once

/*******************************************************************************************
* _U_STRINGorID��һ��WTL�Ĺ����࣬�������Զ�ת��LPCTSTR����ԴID
* 
* #define _WTL_NEW_PAGE_NOTIFY_HANDLERS -- ʹ��WTL 7����Ϣ����ʽ(����)
* 
* WTL����ҳ����
*   �� CWindow ����(���ڽӿ���) + CWindowImplBaseT ����(��Ϣӳ�����ʹ��ڵ�����ʵ��) ��ͬʵ��
*
* ���Ա�Sheet:
*   1.CPropertySheetWindow -- ��װ��PSM_��Ϣ�Ĵ���
*     PROPSHEETHEADER m_psh;
*     ATL::CSimpleArray<HPROPSHEETPAGE> m_arrPages;
*   2.CPropertySheetImpl -- �ṩ����,�� AddPage/RemovePage/SetActivePage/SetWizardMode(��ģʽ) ��
*     �����Ŀ����ص�(����ʱ��)����
*        OnSheetInitialized() -- ��ʼ��
*        PropSheetCallback() -- PropSheetProc �����ľ�̬�ص�����(ע�⣺δʵ��)��
*          static int CALLBACK PropSheetCallback(HWND hWnd, UINT uMsg, LPARAM lParam);
* ����ҳPage
*   1.CPropertyPageWindow 
*   2.CPropertyPageImpl/CAxPropertyPageImpl(֧��ActiveX�ؼ�) -- ��CDialogImplBaseT�̳У�
*     ��Ҫָ���Ի�����Դ: Style(Child)��Border(Thin)��Title Bar(True)��Disabled(True),
*     ͨ��OnNotify��Ϣӳ�䴦��֪ͨ��ΪPSN_��WM_NOTIFY��Ϣ��
*     PROPSHEETPAGE m_psp;
*     �����Ŀ����ص�(����ʱ��)����(����μ� WTL.CPropertyPageImpl.OnNotify )
*       OnSetActive() -- ����ҳ���Ϊ����״̬����ģʽʱ��Ҫ����SetWizardButtons()������Ӧ�İ�ť
*       OnKillActive() - ����ҳ���Ϊ�Ǽ���״̬������TRUE��ʾ��ֹʧȥ����
*       OnQueryCancel() -- 
*       OnApply() -- ���� PSNRET_NOERROR ����ȷ������ɷ��� PSNRET_INVALID ��
*       OnWizardBack/OnWizardNext/OnWizardFinish -- ����ǰһ��/���е���һ��/�����򵼽���������TRUE��ֹ
*
* WTL�Ѿ��ṩ�Ļ����ࣺ
*   CPropertySheet/CPropertyPage/CAxPropertyPage -- ����Ҫ�û����Ƶļ�����
*   ʾ�����룺
*     CPropertySheet sheet(_T("About PSheets"));
*     CPropertyPage<IDD_ABOUTBOX> pgAbout;
*     sheet.AddPage(pgAbout);
*     sheet.DoModal();
*******************************************************************************************/

#include "MakerWelcomePage.h"
#include "MakerChooseSetupFilePage.h"
#include "MakerSetupProgressPage.h"
#include "MakerCompletePage.h"

class CMakerWizardSheet :
    public CWizard97SheetImpl<CMakerWizardSheet>,
    public CMakerWizardInfoRef
{
protected:
    // Typedefs
    typedef CMakerWizardSheet thisClass;
    typedef CWizard97SheetImpl<CMakerWizardSheet> baseClass;
    typedef CMakerWizardInfoRef infoRefClass;

    // Data members
    CMakerWelcomePage           m_pageWelcome;
    CMakerChooseSetupFilePage   m_pageChooseSetupFile;
    CMakerSetupProgressPage     m_pageSetupProgress;
    CMakerCompletePage          m_pageCompletion;

public:
    // Constructors
    CMakerWizardSheet(CMakerWizardInfo* pMakerWizardInfo, UINT uStartPage = 0, HWND hWndParent = NULL);

    // Message Handlers
    BEGIN_MSG_MAP(thisClass)
        MESSAGE_HANDLER(WM_HELP, OnHelp)
        CHAIN_MSG_MAP(baseClass)
    END_MSG_MAP()

    LRESULT OnHelp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
};
