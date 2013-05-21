
#ifndef FTL_SHELL_H
#define FTL_SHELL_H
#pragma once

#ifndef FTL_BASE_H
#  error ftlShell.h requires ftlbase.h to be included first
#endif

/*****************************************************************************************************
* DropFile -- WM_DROPFILES 消息，需要设置 WS_EX_ACCEPTFILES 风格
*   nCount = DragQueryFile(hDropInfo, -1, NULL, 0);
*   for( i < nCount; ) { 
*     nSize = DragQueryFile(hDropInfo, i NULL, 0); 
*     pBuf=new char[++nSize]; 
*     DragQueryFile(hDropInfo, i, pBuf, nSize);
*   }
*
* SHGetSpecialFolderLocation/SHGetSpecialFolderPath -- 获取系统中指定类型的目录位置信息
* ShellBrowser 窗体现在只需要用 SHBrowseForFolder 和 SHGetPathFromIDList 函数即可
* SHGetDesktopFolder -- 获取Shell中桌面对应的IShellFolder接口
*
* LPITEMIDLIST -- Shell 中管理对象的唯一标识? 可以通过 ILCreateFromPath、ILClone、ILGetNext、ILCombine 等函数操纵，
*   使用 IShellFolder::BindToObject 绑定到目标目录上， EnumObjects 枚举其下的所有子对象
* LPENUMIDLIST
* 
* 
*   SHILCreateFromPath -- 从字符串的PATH中获得 LPITEMIDLIST
*   SHGetPathFromIDList-- 从LPITEMIDLIST中获得字符串的PATH
*
* 获取Shell的图标
*   SHFILEINFO shfi = {0};
*   UINT flags = SHGFI_ICON |SHGFI_SYSICONINDEX|SHGFI_SMALLICON; //SHGFI_LARGEICON
*   HIMAGELIST hi=(HIMAGELIST)SHGetFileInfo(NULL,0,&shfi,sizeof(SHFILEINFO),flags); //CImageList::FromHandle(hi
*
* 使用Shell打开指定文件(LPITEMIDLIST) -- 可以避免参数带空格的问题
*	SHELLEXECUTEINFO ShExecInfo = {0};
*   ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
*   ShExecInfo.fMask = SEE_MASK_IDLIST;
*   ShExecInfo.lpIDList= pidlItemTarget;
*   ShExecInfo.nShow = SW_MAXIMIZE;
*   ShellExecuteEx(&ShExecInfo);
*
* 获取文件版本信息
*   GetFileVersionInfoSize -> GetFileVersionInfo -> VerQueryValue(lpData, TEXT("\\"), &pFileInfo, (PUINT)&bufLen);
*   HIWORD(MS),LOWORD(MS),wBuild, HIWORD(LS)
*****************************************************************************************************/

namespace FTL
{
	
    class IShellChangeObserver
    {
    public:
		virtual void OnFileRename( LPCITEMIDLIST pIdl_Src, LPCITEMIDLIST pIdl_Dst ) {};
		virtual void OnFileCreate( LPCITEMIDLIST pIdl ){};
		virtual void OnFileDelete( LPCITEMIDLIST pIdl ) {};
		virtual void OnFileUpdated( LPCITEMIDLIST pIdl ) {};
		virtual void OnFreeSpace(LPCITEMIDLIST pIdl ) {};
		virtual void OnDirRename( LPCITEMIDLIST pIdl_Src, LPCITEMIDLIST pIdl_Dst ){};
		virtual void OnDirCreate( LPCITEMIDLIST pIdl ) {};
		virtual void OnDirDelete( LPCITEMIDLIST pIdl ) {};
		virtual void OnDirUpdated( LPCITEMIDLIST pIdl ){};
		virtual void OnMediaInserted( LPCITEMIDLIST pIdl ){};
		virtual void OnMediaRemoved( LPCITEMIDLIST pIdl ){};
		virtual void OnNetShare(LPCITEMIDLIST pIdl) {};
		virtual void OnNetUnShare(LPCITEMIDLIST pIdl) {};
		virtual void OnDriveAdded( LPCITEMIDLIST pIdl ) {};
		virtual void OnDriveAddGUI( LPCITEMIDLIST pIdl ) {};
		virtual void OnDriveRemoved( LPCITEMIDLIST pIdl ) {};
		virtual void OnChangeAttributes( LPCITEMIDLIST pIdl ) {};
		virtual void OnServerDisconnect(LPCITEMIDLIST pIdl ) {};
    };

	//检测文件系统文件变化通知
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

    };
    FTLEXPORT class CFShellUtil
    {
    public:
		//获取 SHCNE_XXX( 如 SHCNE_RENAMEITEM ) 等对应的字符串信息
		FTLINLINE static LPCTSTR GetShellChangeNotifyString(LONG nEvent, CFStringFormater& formater, LPCTSTR pszDivide = TEXT("|"));

		//获取 pItemIdList 对应的字符串
		FTLINLINE static HRESULT GetItemIdName(  LPCITEMIDLIST  pItemIdList, LPTSTR pFriendlyName, UINT cchBuf, 
			DWORD dwFlags = SHGDN_FORPARSING, IShellFolder* pSF = NULL );

        FTLINLINE static HRESULT GetFileShellInfo(LPCTSTR pszPath, ShellFileInfo& outInfo);

		//获取Shell的系统图标列表，之后可以通过 GetListCtrl().SetImageList(CImageList::FromHandle(hi),LVSIL_SMALL 或 LVSIL_NORMAL) 的方式使用
		//获取文件信息时，可以获取到其在系统图标中的s索引 ?
		FTLINLINE static HRESULT GetShellIconImageList(__out HIMAGELIST& rSmallIconList, __out HIMAGELIST& rLargeIconList);

		//CLSID_ShellLink 组件有 IShellLink(提供快捷方式的参数读写功能)、IPersistFile(提供快捷方式持续性文件的读写功能)等接口。
		FTLINLINE static HRESULT CreateLink(LPCTSTR szPathObj, LPCTSTR szPathLink, LPCTSTR szDesc, LPCTSTR szIconPath = NULL, int iIcon = -1);

		//使用ShellExecute的方法打开文件执行，但如果没有建立连接，则弹出"OpenWith"的对话框
		//  TODO: 是否有其他标准的函数来完成该功能
		FTLINLINE static HRESULT ExecuteOrOpenWithDialog(LPCTSTR pszFile, HWND hWndParent);

		FTLINLINE static HRESULT ExplorerToSpecialFile(LPCTSTR pszFilePath);
    };
}

#endif //FTL_SHELL_H

#ifndef USE_EXPORT
#  include "ftlShell.hpp"
#endif