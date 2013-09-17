#pragma once

//��ֱ��֧�� CWindow*,����֧�� CWindowImplRoot<CWindow>��
//���� delete ʱ����(_BLOCK_TYPE_IS_VALID)��Ϊʲô��

typedef CWindowImplRoot<CWindow> CTabCtrlWindow;

class CTabCtrlEx : public CWindowImpl<CTabCtrlEx, CTabCtrl>
{
public:
    CTabCtrlEx();
    virtual ~CTabCtrlEx();
    void SetRectangle();

    int AddWindow(CTabCtrlWindow* pWnd,LPCTSTR pszTitle);
    int InsertWindow(int nItem, CTabCtrlWindow* pWnd,LPCTSTR pszTitle);
    BOOL RemoveWindow(int nItem);
    CTabCtrlWindow* GetWindow(int nItem);
    
    BEGIN_MSG_MAP(CTabCtrlEx)
        MSG_WM_LBUTTONDOWN(OnLButtonDown)
        MSG_WM_SIZE(OnSize)
    END_MSG_MAP()

protected:
    CSimpleArray<CTabCtrlWindow *> m_AlltabPages;
    int m_CurrentIndex;
public:
    LRESULT OnSize(UINT state, CSize Size);
    LRESULT OnLButtonDown(UINT nFlags, CPoint pt);
};
