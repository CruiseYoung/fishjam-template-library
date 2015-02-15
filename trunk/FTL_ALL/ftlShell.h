
#ifndef FTL_SHELL_H
#define FTL_SHELL_H
#pragma once

#ifndef FTL_BASE_H
#  error ftlShell.h requires ftlbase.h to be included first
#endif

/*****************************************************************************************************
* DropFile -- WM_DROPFILES ��Ϣ����Ҫ���� WS_EX_ACCEPTFILES ���
*   nCount = DragQueryFile(hDropInfo, -1, NULL, 0);
*   for( i < nCount; ) { 
*     nSize = DragQueryFile(hDropInfo, i NULL, 0); 
*     pBuf=new char[++nSize]; 
*     DragQueryFile(hDropInfo, i, pBuf, nSize);
*   }
*
* SHGetSpecialFolderPath/SHGetSpecialFolderLocation -- ��ȡϵͳ��ָ�����͵�Ŀ¼λ����Ϣ
* ShellBrowser ��������ֻ��Ҫ�� SHBrowseForFolder �� SHGetPathFromIDList ��������
* SHGetDesktopFolder -- ��ȡShell�������Ӧ��IShellFolder�ӿ�
*
* LPITEMIDLIST -- Shell �й�������Ψһ��ʶ? ����ͨ�� ILCreateFromPath��ILClone��ILGetNext��ILCombine �Ⱥ������ݣ�
*   ʹ�� IShellFolder::BindToObject �󶨵�Ŀ��Ŀ¼�ϣ� EnumObjects ö�����µ������Ӷ���
* LPENUMIDLIST
* 
* 
*   SHILCreateFromPath -- ���ַ�����PATH�л�� LPITEMIDLIST
*   SHGetPathFromIDList-- ��LPITEMIDLIST�л���ַ�����PATH
*
* ��ȡShell��ͼ��
*   SHFILEINFO shfi = {0};
*   UINT flags = SHGFI_ICON |SHGFI_SYSICONINDEX|SHGFI_SMALLICON; //SHGFI_LARGEICON
*   HIMAGELIST hi=(HIMAGELIST)SHGetFileInfo(NULL,0,&shfi,sizeof(SHFILEINFO),flags); //CImageList::FromHandle(hi
*
* ʹ��Shell��ָ���ļ�(LPITEMIDLIST) -- ���Ա���������ո������
*	SHELLEXECUTEINFO ShExecInfo = {0};
*   ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
*   ShExecInfo.fMask = SEE_MASK_IDLIST;
*   ShExecInfo.lpIDList= pidlItemTarget;
*   ShExecInfo.nShow = SW_MAXIMIZE;
*   ShellExecuteEx(&ShExecInfo);
*
* ��ȡ�ļ��汾��Ϣ
*   GetFileVersionInfoSize -> GetFileVersionInfo -> VerQueryValue(lpData, TEXT("\\"), &pFileInfo, (PUINT)&bufLen);
*   HIWORD(MS),LOWORD(MS),wBuild, HIWORD(LS)
*
*****************************************************************************************************/

/*****************************************************************************************************
* �ļ�ϵͳ�仯���
*   1.SHChangeNotifyRegister -- ��ָ���Ĵ�����ӵ�ϵͳ����Ϣ�������У����ղ����������ļ�ϵͳ����Shell��֪ͨ��Ϣ
*   2.FindFirstChangeNotification  -- ����һ������֪ͨ��������ó�ʼ����֪ͨ����������Ȼ�����Wait
*   3.ReadDirectoryChangesW -- 
*   4.ʵ��ICopyHook�ӿڵ�Shell��չ���󣬲�ע�ᵽ HKCR\Directory\shellex\CopyHookHandlers ��
* ע���ϵͳ�仯���
*   1.RegNotifyChangeKeyValue
*****************************************************************************************************/

namespace FTL
{
	
    class IShellChangeObserver
    {
    public:
		virtual void OnFileRename( LPCITEMIDLIST /* pIdl_Src */, LPCITEMIDLIST /* pIdl_Dst */ ) {};
		virtual void OnFileCreate( LPCITEMIDLIST /* pIdl */ ){};
		virtual void OnFileDelete( LPCITEMIDLIST /* pIdl */  ) {};
		virtual void OnFileUpdated( LPCITEMIDLIST /* pIdl */  ) {};
		virtual void OnFreeSpace(LPCITEMIDLIST /* pIdl */  ) {};
		virtual void OnDirRename( LPCITEMIDLIST /* pIdl_Src */, LPCITEMIDLIST /* pIdl_Dst */ ){
            FTLTRACE(TEXT("OnDirRename: \n"));
        };

		virtual void OnDirCreate( LPCITEMIDLIST /* pIdl */  ) {};
		virtual void OnDirDelete( LPCITEMIDLIST /* pIdl */  ) {};
		virtual void OnDirUpdated( LPCITEMIDLIST /* pIdl */  ){};
		virtual void OnMediaInserted( LPCITEMIDLIST /* pIdl */  ){};
		virtual void OnMediaRemoved( LPCITEMIDLIST /* pIdl */  ){};
		virtual void OnNetShare(LPCITEMIDLIST /* pIdl */ ) {};
		virtual void OnNetUnShare(LPCITEMIDLIST /* pIdl */ ) {};
		virtual void OnDriveAdded( LPCITEMIDLIST /* pIdl */  ) {};
		virtual void OnDriveAddGUI( LPCITEMIDLIST /* pIdl */  ) {};
		virtual void OnDriveRemoved( LPCITEMIDLIST /* pIdl */  ) {};
		virtual void OnChangeAttributes( LPCITEMIDLIST /* pIdl */  ) {};
		virtual void OnServerDisconnect(LPCITEMIDLIST /* pIdl */  ) {};
    };

	//����ļ�ϵͳ�ļ��仯֪ͨ
    FTLEXPORT class CFShellChangeMonitor
    {
    public:
        FTLINLINE CFShellChangeMonitor();
        FTLINLINE ~CFShellChangeMonitor();
		FTLINLINE BOOL SetChangeObserver(IShellChangeObserver* pChangeObserver)
		{
			m_pChangeObserver = pChangeObserver;
			return TRUE;
		}
        FTLINLINE BOOL Create(LPCTSTR pszMonitorPath = NULL, 
			LONG nEvent = SHCNE_ALLEVENTS | SHCNE_INTERRUPT,
			BOOL bRecursive = TRUE);
        FTLINLINE BOOL Destroy();
    private:
		IShellChangeObserver*	m_pChangeObserver;
        HWND m_hWndNotify;
        UINT m_uiNotifyMsg;
		ULONG m_uiChangeNotifyID;
        IShellFolder*  m_pShellFolder;
        FTLINLINE BOOL _CreateNotifyWinows();
        FTLINLINE static LRESULT CALLBACK _MainMonitorWndProc(HWND hwnd,UINT uMsg, WPARAM wParam,LPARAM lParam);
		FTLINLINE LRESULT _HandleMonitorEvent(LONG wEventId, LPCITEMIDLIST pIdlDst, LPCITEMIDLIST pIdlSrc);

        //FTLINLINE BOOL _RegisterMonitor()   
    };

    struct ShellFileInfo
    {
        //PIDLIST_ABSOLUTE    pIdl;
    };
    FTLEXPORT class CFShellUtil
    {
    public:
		//��ȡ SHCNE_XXX( �� SHCNE_RENAMEITEM ) �ȶ�Ӧ���ַ�����Ϣ
		FTLINLINE static LPCTSTR GetShellChangeNotifyString(LONG nEvent, CFStringFormater& formater, LPCTSTR pszDivide = TEXT("|"));

		//��ȡ pItemIdList ��Ӧ���ַ���
		FTLINLINE static HRESULT GetItemIdName(  LPCITEMIDLIST  pItemIdList, LPTSTR pFriendlyName, UINT cchBuf, 
			DWORD dwFlags = SHGDN_FORPARSING, IShellFolder* pSF = NULL );

        //��ȡ·����Ӧ�� LPITEMIDLIST
        FTLINLINE static HRESULT GetItemIDListFromPath( LPCTSTR szFullPath, LPITEMIDLIST* ppidl, IShellFolder* pSF = NULL);

        //TODO: ϵͳ�Ѿ��ṩ�� SHGetFileInfo ����
        FTLINLINE static HRESULT GetFileShellInfo(LPCTSTR pszPath, ShellFileInfo& outInfo);

		//��ȡShell��ϵͳͼ���б�֮�����ͨ�� GetListCtrl().SetImageList(CImageList::FromHandle(hi),LVSIL_SMALL �� LVSIL_NORMAL) �ķ�ʽʹ��
		//��ȡ�ļ���Ϣʱ�����Ի�ȡ������ϵͳͼ���е�s���� ?
		FTLINLINE static HRESULT GetShellIconImageList(__out HIMAGELIST& rSmallIconList, __out HIMAGELIST& rLargeIconList);

		//CLSID_ShellLink ����� IShellLink(�ṩ��ݷ�ʽ�Ĳ�����д����)��IPersistFile(�ṩ��ݷ�ʽ�������ļ��Ķ�д����)�Ƚӿڡ�
		FTLINLINE static HRESULT CreateLink(LPCTSTR szPathObj, LPCTSTR szPathLink, LPCTSTR szDesc, LPCTSTR szIconPath = NULL, int iIcon = -1);

		//ʹ��ShellExecute�ķ������ļ�ִ�У������û�н������ӣ��򵯳�"OpenWith"�ĶԻ���
		//  TODO: �Ƿ���������׼�ĺ�������ɸù���
		FTLINLINE static HRESULT ExecuteOrOpenWithDialog(LPCTSTR pszFile, HWND hWndParent);

		FTLINLINE static HRESULT ExplorerToSpecialFile(LPCTSTR pszFilePath);

        FTLINLINE static HRESULT LaunchIE(LPCTSTR szURL, int nCmdShow = SW_SHOW);

        //FTLINLINE static BOOL BrowserDirectory(CFStringFormater& strResult, LPCTSTR strTitle = NULL, UINT nFlags = BIF_NEWDIALOGSTYLE);
    };


    //BUG -- Win7 �£����� BIF_NEWDIALOGSTYLE ʱ���޷��Զ���λ��ʾ pszInit ��Ŀ¼
    //https://connect.microsoft.com/VisualStudio/feedback/details/518103/bffm-setselection-does-not-work-with-shbrowseforfolder-on-windows-7
    //atldlgs.h ���� CFolderDialog -- CFolderDialog dlg ( hWnd, NULL, BIF_USENEWUI | BIF_RETURNONLYFSDIRS ); dlg.SetInitialFolder( szDefaultPath ); if (IDOK ==dlg.DoModal() ){ ... }
    class CFDirBrowser
    {
    public:
        FTLINLINE CFDirBrowser(LPCTSTR lpszTitle = NULL, HWND hWndOwner = NULL, LPCTSTR pszInit = NULL, 
            UINT nFlags = BIF_NEWDIALOGSTYLE | 
            BIF_EDITBOX |
            BIF_STATUSTEXT |        //�ɰ���״̬����ͨ��������Ϣ�����������ı�
            BIF_RETURNONLYFSDIRS |  //�������ļ�ϵͳ��Ŀ¼��ѡ�С��ҵĵ��ԡ���ʱȷ�ϰ�ťΪ����״̬
            BIF_BROWSEINCLUDEURLS 
            );
        FTLINLINE BOOL DoModal();
        FTLINLINE LPCTSTR GetSelectPath() { return m_szPath; }
        BROWSEINFO  m_browseInfo;
    protected:
        TCHAR       m_szPath[MAX_PATH];
        TCHAR       m_szInitPath[MAX_PATH];
    protected:
        FTLINLINE static int CALLBACK DirBrowseCallbackProc(HWND hwnd, UINT uMsg,LPARAM lParam, LPARAM lpData);
        HWND        m_hTreeView;
        BOOL        m_bFirstEnsureSelectVisible;
        FTLINLINE HWND FindTreeViewCtrl(HWND hWnd);
        FTLINLINE BOOL EnsureSelectVisible();
    };
}

#endif //FTL_SHELL_H

#ifndef USE_EXPORT
#  include "ftlShell.hpp"
#endif