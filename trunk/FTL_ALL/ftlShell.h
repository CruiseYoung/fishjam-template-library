
#ifndef FTL_SHELL_H
#define FTL_SHELL_H
#pragma once

#ifndef FTL_BASE_H
#  error ftlShell.h requires ftlbase.h to be included first
#endif

/*****************************************************************************************************
* DropFile -- WM_DROPFILES ��Ϣ
*   nCount = DragQueryFile(hDropInfo, -1, NULL, 0);
*   for( i < nCount; ) { 
*     nSize = DragQueryFile(hDropInfo, i NULL, 0); 
*     pBuf=new char[++nSize]; 
*     DragQueryFile(hDropInfo, i, pBuf, nSize);
*   }
*
* SHGetSpecialFolderLocation/SHGetSpecialFolderPath -- ��ȡϵͳ��ָ�����͵�Ŀ¼λ����Ϣ
* ShellBrowser ��������ֻ��Ҫ�� SHBrowseForFolder �� SHGetPathFromIDList ��������
* SHGetDesktopFolder -- ��ȡShell�������Ӧ��IShellFolder�ӿ�
*
* LPITEMIDLIST -- Shell �й�������Ψһ��ʶ? ����ͨ�� ILCombine �Ⱥ������ݣ�
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
*****************************************************************************************************/

namespace FTL
{
	
    class IShellChangeObserver
    {
    public:

    };

	//����ļ�ϵͳ�ļ��仯֪ͨ
    FTLEXPORT class CFShellChangeMonitor
    {
    public:
        FTLINLINE CFShellChangeMonitor();
        FTLINLINE ~CFShellChangeMonitor();
        FTLINLINE BOOL Create(LPCTSTR pszMonitorPath = NULL, BOOL bRecursive = TRUE);
        FTLINLINE BOOL Destroy();
    private:
        HWND m_hWndNotify;
        UINT m_uiNotifyMsg;
		ULONG m_uiChangeNotifyID;
        IShellFolder*  m_pShellFolder;
        BOOL _CreateNotifyWinows();
        static FTLINLINE LRESULT CALLBACK MainMonitorWndProc(HWND hwnd,UINT uMsg, WPARAM wParam,LPARAM lParam);

        //FTLINLINE BOOL _RegisterMonitor()   
    };

    struct ShellFileInfo
    {

    };
    FTLEXPORT class CFShellUtil
    {
    public:
        FTLINLINE static HRESULT GetFileShellInfo(LPCTSTR pszPath, ShellFileInfo& outInfo);

		//ʹ��ShellExecute�ķ������ļ�ִ�У������û�н������ӣ��򵯳�"OpenWith"�ĶԻ���
		//  TODO: �Ƿ���������׼�ĺ�������ɸù���
		FTLINLINE static HRESULT ExecuteOpenWithDialog(LPCTSTR pszFile, HWND hWndParent);
    };
}

#endif //FTL_SHELL_H

#ifndef USE_EXPORT
#  include "ftlShell.hpp"
#endif