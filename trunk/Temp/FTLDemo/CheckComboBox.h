#pragma once

class CCheckComboBox : public CComboBox
{
public:
	CCheckComboBox();
	virtual ~CCheckComboBox();

    //overwrite
	virtual BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);

    //��д����� ���麯�� -- ע�⣺�Ƿ��麯������Ҫ����ȷ���Ƿ�������
    DWORD_PTR GetItemData(int nIndex) const;
    int SetItemData(int nIndex, DWORD_PTR dwItemData);
    
    int AddString(LPCTSTR lpszString);
    int DeleteString(UINT nIndex);
    int InsertString(int nIndex, LPCTSTR lpszString);
    void ResetContent();

    //CCheckComboBox function
	INT SetCheck(INT nIndex, BOOL bFlag);
	BOOL GetCheck(INT nIndex);
	void SelectAll(BOOL bCheck = TRUE);

protected:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCheckComboBox)
	protected:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CCheckComboBox)
	afx_msg LRESULT OnCtlColorListBox(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGetText(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGetTextLength(WPARAM wParam, LPARAM lParam);
	afx_msg void OnDropDown();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
protected:
    struct CheckItemInfo
    {
        BOOL        bChecked;
        DWORD_PTR   dwItemData;
    };

	// Routine to update the text
	void RecalcText();

	// The subclassed COMBOLBOX window (notice the 'L')
	HWND m_hListBox;

	// The string containing the text to display
	CString m_strText;
	BOOL m_bTextUpdated;

	// A flag used in MeasureItem, see comments there
	BOOL m_bItemHeightSet;

    //ResetContent ���ڲ���װ��������� CheckItemInfo �ṹ��Ϣ
    void _ResetCheckItemContent(BOOL bResetBaseItemData);
};