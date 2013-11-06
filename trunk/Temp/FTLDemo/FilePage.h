#pragma once

#include <ftlFile.h>
// CFilePage dialog

#define UM_UPDATE_COPY_DIR_PROGRESS     (WM_USER + 101)
#define UM_UPDATE_COPY_DIR_ERROR        (WM_USER + 102)

class CFilePage : public CPropertyPage
    , public ICopyDirCallback
{
	DECLARE_DYNAMIC(CFilePage)

public:
	CFilePage();
	virtual ~CFilePage();

// Dialog Data
	enum { IDD = IDD_PAGE_FILE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support


	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedBtnFileChooseCopySrcDir();
    afx_msg void OnBnClickedBtnFileChooseCopyDstDir();
    afx_msg void OnBnClickedBtnFileFindSourceDir();
    afx_msg void OnBnClickedBtnFileStartCopyDir();
    afx_msg void OnBnClickedBtnFilePauseResumeCopyDir();
    afx_msg void OnBnClickedBtnFileStopCopyDir();

    LRESULT AFX_MSG_CALL OnCopyDirProgress(WPARAM wParam, LPARAM lParam);
    LRESULT AFX_MSG_CALL OnCopyDirError(WPARAM wParam, LPARAM lParam);

    virtual BOOL OnInitDialog();
protected:
    CString     m_strCopySrcDir;
    CString     m_strCopyDstDir;

    LONGLONG    m_nTotalSize;
    LONGLONG    m_nCopiedSize;
    CProgressCtrl   m_progressCopyDir;

    CFDirectoryCopier   m_DirCopier;

    //ICopyDirCallback
    virtual VOID OnBeginPrepareSourceFiles(LPCTSTR pszSrcDir, LPCTSTR pszDstDir);
    virtual VOID OnBegin(LONGLONG nTotalSize, LONG nFileCount);
    virtual VOID OnCopyFile(LPCTSTR pszSrcFile, LPCTSTR pszTargetFile, LONG nIndex, LONGLONG nFileSize, LONGLONG nCopiedSize);
    virtual VOID OnEnd(BOOL bSuccess, LONGLONG nTotalCopiedSize, LONG nCopiedFileCount);
    virtual VOID OnError(LPCTSTR pszSrcFile, LPCTSTR pszTargetFile, DWORD dwError);
};
