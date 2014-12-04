#ifndef FTL_CRASH_HANDLER_H
#define FTL_CRASH_HANDLER_H

#pragma once

#ifndef FTL_BASE_H
#  error ftlcrashhandler requires ftlbase.h to be included first
#endif

#include <atlwin.h>
//#include <ftlWindow.h>
// ����Դ�Ի�����ҪATL

namespace FTL
{
	/*********************************************************************************************
	* CFileDialog dlg(TRUE); dlg.DoModal(); ��Win7�±���
	*   http://social.msdn.microsoft.com/Forums/en-US/2de2670e-03df-495b-9a84-1049eddff9b6/cfiledialog-on-vindows-7-exception
	*********************************************************************************************/

    /*********************************************************************************************
	* ���ŷ�����(Symbol Server)
    *   �� WinDbg.exe/devenv.exe + DbgEng.dll + DbgHelp.dll(OS�ķ��ſ�ģ�飬�����ȡ�ͽ������Է���) + SymSvr.Dll(�ӷ��ŷ��������ҡ����غ͹�������ļ�) ����ɣ�
    *   �﷨Ϊ��symsrv*ServerDLL*[DownstreamStore*]ServerPath
    *     ���ʹ��ϵͳ�ṩ��SymSvr.Dll�����Լ򻯳ɣ�srv*[DownstreamStore*]ServerPath
	*   ����������%_NT_SYMBOL_PATH%=SRV*E:\OSSymbols*http://msdl.microsoft.com/download/symbols
	*   ���⣺VS�п����ڵ��Է���·����ָ�� http://msdl.microsoft.com/download/symbols ����
    *
	* CrashReport: ��������쳣��ʱ����ʾ���ʹ���ĶԻ���, ����Dump�ļ����͵�ָ���ĵ�ַ. http://code.google.com/p/crashrpt/
	* XCrashReport: ����������Ƶ�һ����Դ��Ŀ. http://www.codeproject.com/KB/debug/XCrashReportPt1.aspx
	* procdump: ����ʹ��ָ���������� dmp �ļ�(http://technet.microsoft.com/en-us/sysinternals/dd996900)
	*   procdump -ma <PID>  -- ʹ��ָ����������������dump�ļ������ڳ��������������Ի���ʱʹ��
	*
	* ��ȡDump��Ϣ -- http://support.microsoft.com/kb/315263
	* WINDBG����DMP������ http://bbs.icafe8.com/forum.php?mod=viewthread&tid=400104&fromuid=30123
    *
    * ���ο��������ŷ�����API(Symbol Server API, �� SymbolServer �Ⱥ���������μ� dbghelp.chm �ļ�),
	*
	* �������⣺
	*   1.post mortem debugging -- ���� dmp �ļ�ʱ�޷����öϵ�
	*   1.���� .dmp �ļ�����ʱ���� Output ������ ��.dmp': Loaded '*�� ���Բ鿴�ڿͻ��˼��ص��ڵ��Ի�����û�е��ļ�
	*     (��������Ǹ��ְ汾�� msvcrp* �� mfc*)��Ȼ����Ҫ���ض�Ӧ�ļ��������޷����ص��Է���
	* �޷�ͨ�����ŷ��������� mfc90u.pdb
	*   1.�п����ǲ�ͬ��OS?
	*
	* 
	*********************************************************************************************/

	/*********************************************************************************************
	* ÿ���쳣��Windows �����������ִ������ -- 
	*   �쳣������(VEH��SEH ��)
    * SetUnhandledExceptionFilter -- �ڷ���δ�����쳣ʱ����ϵͳ���ý��д���
    *   ���쳣�������ķ���ֵ��
    *     EXCEPTION_EXECUTE_HANDLER -- 
    *     EXCEPTION_CONTINUE_EXECUTION -- �������ͼ�Է����쳣�Ĵ�����ִ��һ�飬ͨ����Ҫ���޸�����߼����������ѭ��
    *     EXCEPTION_CONTINUE_SEARCH -- ����ϵͳͨ�����쳣����������Ϣ�Ի��� 
    * Vistaϵͳ�¿���ʹ�� RegisterApplicationRecoveryCallback ע��ָ��ص�������������
    * δ֪������߳���Windows��Ӧʱ��(Ĭ��5��)��ϵͳ���ã����Ա����û�δ��������ݲ��ָ�����Windows�ṩ��UI��
    * �� TIB �б���SEH������CONTEXT��Ĵ�����ֵ
    * ͨ��VirtualQuery��ѯEIP��ʹ��GetModuleName�ɻ�֪�쳣ģ����
    * StackWalk�Ļص�����dbghelpʵ�ֺõĺ���
    * SymFromAddr��ȡջλ�õĺ�����
    * SymGetLineFromAddr -- ��ȡԴ�����ļ�������
	*
	* RaiseException(EXCEPTION_BREAKPOINT, 0, 0, NULL);  -- �����׳��쳣
	* 
	* __try { ...} 
	* __except( 
	*           code=GetExceptionCode(), 
	*           MyMiniDump(hFile, GetExceptionInformation() ),
	*           EXCEPTION_EXECUTE_HANDLER 
	*          ) { �쳣������� } 
	*
	* 
	* DbgHelp�е�DumpAPI����: http://www.debuginfo.com/examples/src/effminidumps/MiniDump.cpp 
    *
    * TODO(δ���ǵ���Լ������Ҫȷ������������Ƿ���ȷ)
    *   EBP-C -- ��һ���ֲ�����
    *   EBP-8 -- ��Cookie����(һ���� 0xCCCCCCC, ֻ�е��԰����?)
    *   EBP-4 -- ��ȫCookie
    *   EBP -- ��������EBP ֵ
    *   EBP+4 -- �����ķ��ص�ַ
    *   EBP+8 -- ����N(��һ���������һ��?)
    **********************************************************************************************/
}

//extern CServerAppModule _Module;
//extern CComModule _Module;
//extern CAppModule _Module;  //CFResourcelessDlg��Ҫʹ��



#include <vector>
//#include <atlbase.h>
//#include <atlwin.h>
//#include <WindowsX.h>

#include <ftlsystem.h>  //SuspendProcess


namespace FTL
{
    FTLEXPORT template<typename T>
    class CFResourcelessDlg: public ATL::CDialogImplBase  //����Դ�Ի���������ʾCrashHandler�Ļ���
    {
    public:
        // Message map for IDOK & IDCANCEL
        BEGIN_MSG_MAP(CFResourcelessDlg)
            COMMAND_ID_HANDLER(IDOK, OnCloseCmd)
            COMMAND_ID_HANDLER(IDCANCEL, OnCloseCmd)
        END_MSG_MAP()

        // Dailog creation functions
        FTLINLINE HWND Create(HWND hWndParent = ::GetActiveWindow(), LPARAM dwInitParam = NULL);
        FTLINLINE virtual BOOL	DestroyWindow();
        FTLINLINE INT_PTR DoModal(HWND hWndParent = ::GetActiveWindow(), LPARAM dwInitParam = NULL);
        // Accessor functions
        FTLINLINE bool IsModal() const;
        FTLINLINE UINT NumberOfControls()	const;
    protected:
        // Misc functions
        FTLINLINE void EndDialog(INT_PTR iResult);
        FTLINLINE virtual	LRESULT	OnCloseCmd(UINT, int iId, HWND, BOOL&);
        // Template create functions
        FTLINLINE void CreateDlgTemplate(ATL::_U_STRINGorID pszTitle, DWORD dwStyle, DWORD dwExStyle, 
            short x, short y, short cx, short cy, short sFontSize = 0, 
            ATL::_U_STRINGorID pszFontName = (UINT)0, 
            ATL::_U_STRINGorID pszMenu = (UINT)0, 
            ATL::_U_STRINGorID pszWndClass = (UINT)0);
        FTLINLINE void AddDlgItem(ATL::_U_STRINGorID pszTitle, DWORD dwStyle, DWORD dwExStyle, 
            short x, short y, short cx, short cy, short id, 
            ATL::_U_STRINGorID pszWndClass = (UINT)0, 
            short sCreateDataSize = 0, 
            void* pCreateData = NULL);
        FTLINLINE void AddButton(ATL::_U_STRINGorID pszTitle, DWORD dwStyle, DWORD dwExStyle, 
            short x, short y, short cx, short cy, short wId);
        FTLINLINE void AddEditBox(ATL::_U_STRINGorID pszTitle, DWORD dwStyle, DWORD dwExStyle, 
            short x, short y, short cx, short cy, short wId);
        FTLINLINE void AddStatic(ATL::_U_STRINGorID pszTitle, DWORD dwStyle, DWORD dwExStyle, 
            short x, short y, short cx, short cy, short wId);
        FTLINLINE void AddListBox(ATL::_U_STRINGorID pszTitle, DWORD dwStyle, DWORD dwExStyle, 
            short x, short y, short cx, short cy, short wId);
        FTLINLINE void AddScrollBar(ATL::_U_STRINGorID pszTitle, DWORD dwStyle, DWORD dwExStyle, 
            short x, short y, short cx, short cy, short wId);
        FTLINLINE void AddCombo(ATL::_U_STRINGorID pszTitle, DWORD dwStyle, DWORD dwExStyle, 
            short x, short y, short cx, short cy, short wId);
    private:
        FTLINLINE int WriteString(WORD* &dest, LPCTSTR pszString, bool bWriteResource = false);
        enum	COMMONCONTROLS{
            DLG_BUTTON		= 0x80,
            DLG_EDIT		= 0x81,
            DLG_STATIC		= 0x82,
            DLG_LIST		= 0x83,
            DLG_SCROLLBAR	= 0x84,
            DLG_COMBO		= 0x85
        };
        WORD	m_uiNumCtrls;	// Number of controls on in the template
        int		m_iSize;		// Total size of the template
        int		m_iOffset;		// Offset
        int*	m_pOffset;		// Pointer to the offset
        BYTE*	m_pTemplate;	// Pointer to the template data
        bool	m_bModal;
    };

    class CFCrashHandlerDialog;
    class CFCrashHandler;
}


namespace FTL
{
    class CFCrashHandlerDialog : public CFResourcelessDlg<CFCrashHandlerDialog>
    {
    public:
        BEGIN_MSG_MAP(CFCrashHandlerDialog)
            //DUMP_WINDOWS_MSG(__FILE__LINE__, DEAFULT_DUMP_FILTER_MESSAGES, _countof(DEAFULT_DUMP_FILTER_MESSAGES), uMsg, wParam, lParam)

            COMMAND_HANDLER(IDC_BTN_CREATE_MINIDUMP, BN_CLICKED, OnCreateMiniDumpClick)
			COMMAND_HANDLER(IDC_BTN_SAVE_STACK, BN_CLICKED, OnSaveStackClick)
            MESSAGE_HANDLER(WM_INITDIALOG,OnInitDialog)
            MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
            MESSAGE_HANDLER(WM_SIZE, OnSize)
			CHAIN_MSG_MAP(CFResourcelessDlg<CFCrashHandlerDialog>)
            //MESSAGE_HANDLER(WM_SIZE, OnSize)
        END_MSG_MAP()
        enum 
        {
            IDC_STATIC_ADDRESS  = 1000,
            IDC_STATIC_REASON,
			IDC_LIST_STACK,
            IDC_BTN_DEBUG,
            IDC_BTN_CREATE_MINIDUMP,
			IDC_BTN_SAVE_STACK,
        };
        FTLINLINE CFCrashHandlerDialog(PEXCEPTION_POINTERS pExcption);
        FTLINLINE virtual ~CFCrashHandlerDialog();
        FTLINLINE LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
        FTLINLINE LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
        FTLINLINE LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

        //FTLINLINE LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
        FTLINLINE LPTSTR GetFaultReason(DWORD ExceptionCode);
        FTLINLINE void CreateDlg();
		FTLINLINE LRESULT OnCreateMiniDumpClick(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
		FTLINLINE LRESULT OnSaveStackClick(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
    protected:
		FTLINLINE BOOL _GetCrashFilePrefix(LPTSTR pszBuffer, DWORD dwSize);		
        PEXCEPTION_POINTERS m_pException;
        TCHAR m_FaultReason[MAX_BUFFER_LENGTH];
    };

    //enum MiniDumpType{
    //    mdtNormal         = 0x0000,
    //    mdtWithDataSegs   = 0x0001,
    //    mdtWithFullMemory = 0x0002,
    //    mdtWithHandleData = 0x0004,
    //};

    //ע�⣺���� SetUnhandledExceptionFilter �޷��������Ĳ��������ʹ���෽�����ÿ���
    //typedef long ( __stdcall *PFNCHFILTFN ) ( PEXCEPTION_POINTERS pExPtrs ) ;
	//TODO:��Ҫʹ�� AdjustTokenPrivileges ������ SE_DEBUG_NAME Ȩ�� ������ MiniDumpWriteDump ?
	//  https://www.evernote.com/shard/s90/sh/81c75ef2-416b-4b72-8ed2-f997c12da163/2eb0ee2d8fa5dd2ad832351371216788
	//  
    class CFCrashHandler
    {
    public:
        FTLINLINE CFCrashHandler();
        FTLINLINE ~CFCrashHandler();
        FTLINLINE BOOL SetDefaultCrashHandlerFilter();
        FTLINLINE BOOL RestoreCrashHandlerFilter();
        //BOOL SetCrashHandlerFilter ( PFNCHFILTFN pFn ) ;
        FTLINLINE static BOOL SnapProcessMiniDump(HANDLE hProcess, MINIDUMP_TYPE type, LPCTSTR pszDumpFileName);
        FTLINLINE static BOOL CreateProcessCrashDump(HANDLE hProcess, MINIDUMP_TYPE type, LPCTSTR pszDumpFileName,
            DWORD dwThread, EXCEPTION_POINTERS * pExceptInfo);
    private:
        //PFNCHFILTFN m_pfnCallBack;
        static CFCrashHandler* s_pSingleCrashHandler;
		static CRITICAL_SECTION	*s_pCriticalSection;

        LPTOP_LEVEL_EXCEPTION_FILTER m_pfnOrigFilt;
        FTLINLINE static LONG __stdcall DefaultCrashHandlerFilter( PEXCEPTION_POINTERS pExPtrs);
    };
}

#endif //FTL_CRASH_HANDLER_H

#ifndef USE_EXPORT
#  include "ftlCrashHandler.hpp"
#endif
