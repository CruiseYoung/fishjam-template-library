
// GifLibDemoDlg.h : header file
//

#pragma once


// CGifLibDemoDlg dialog
class CGifLibDemoDlg : public CDialog
{
// Construction
public:
	CGifLibDemoDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_GIFLIBDEMO_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedButton1();
    BOOL _OverlayMouseToScreen(HDC hdc, LPRECT lpRect);
    afx_msg void OnBnClickedButton2();
    afx_msg void OnTimer(UINT_PTR nIDEvent);
    int m_nClipIndex;
    DWORD m_dwTicket;
    afx_msg void OnBnClickedBtnTimerClip();
    afx_msg void OnBnClickedBtnWuColorQuantizer();
};
