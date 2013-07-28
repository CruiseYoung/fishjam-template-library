#ifndef FTL_CRASH_HANDLER_H
#define FTL_CRASH_HANDLER_H

#pragma once

#ifndef FTL_BASE_H
#  error ftlcrashhandler requires ftlbase.h to be included first
#endif

#include <atlwin.h>
// ����Դ�Ի�����ҪATL

namespace FTL
{
	/*********************************************************************************************
	* CFileDialog dlg(TRUE); dlg.DoModel(); ��Win7�±���
	*   http://social.msdn.microsoft.com/Forums/en-US/2de2670e-03df-495b-9a84-1049eddff9b6/cfiledialog-on-vindows-7-exception
	*********************************************************************************************/

    /*********************************************************************************************
	* ���ŷ�������
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
    * SetUnhandledExceptionFilter -- �ڷ���δ�����쳣ʱ����ϵͳ���ý��д���
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
    **********************************************************************************************/
}

//extern CServerAppModule _Module;
//extern CComModule _Module;
//extern CAppModule _Module;  //CFResourcelessDlg��Ҫʹ��



#include <vector>
//#include <atlbase.h>
//#include <atlwin.h>
#include <TlHelp32.h>
#include <DbgHelp.h>
#include <Psapi.h>
#pragma comment( lib, "DbgHelp.lib" )
#pragma comment( lib, "Psapi.lib" )
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

    // The "ugly" assembler-implementation is needed for systems before XP
    // If you have a new PSDK and you only compile for XP and later, then you can use 
    // the "RtlCaptureContext"
    // Currently there is no define which determines the PSDK-Version... 
    // So we just use the compiler-version (and assumes that the PSDK is 
    // the one which was installed by the VS-IDE)

    // INFO: If you want, you can use the RtlCaptureContext if you only target XP and later...
    //       But I currently use it in x64/IA64 environments...
    //#if defined(_M_IX86) && (_WIN32_WINNT <= 0x0500) && (_MSC_VER < 1400)

#if defined(_M_IX86)
#ifdef CURRENT_THREAD_VIA_EXCEPTION
    // TODO: The following is not a "good" implementation, 
    // because the call stack is only valid in the "__except" block...
#define GET_CURRENT_CONTEXT(c, contextFlags) \
    do { \
    memset(&c, 0, sizeof(CONTEXT)); \
    EXCEPTION_POINTERS *pExp = NULL; \
    __try { \
    throw 0; \
    } __except( ( (pExp = GetExceptionInformation()) ? EXCEPTION_EXECUTE_HANDLER : EXCEPTION_EXECUTE_HANDLER)) {} \
    if (pExp != NULL) \
    memcpy(&c, pExp->ContextRecord, sizeof(CONTEXT)); \
    c.ContextFlags = contextFlags; \
    } while(0);
#else
    // The following should be enough for walking the call stack...
#define GET_CURRENT_CONTEXT(c, contextFlags) \
    do { \
    memset(&c, 0, sizeof(CONTEXT)); \
    c.ContextFlags = contextFlags; \
    __asm    call x \
    __asm x: pop eax \
    __asm    mov c.Eip, eax \
    __asm    mov c.Ebp, ebp \
    __asm    mov c.Esp, esp \
    } while(0);
#endif
#else
    // The following is defined for x86 (XP and higher), x64 and IA64:
#define GET_CURRENT_CONTEXT(c, contextFlags) \
    do { \
    memset(&c, 0, sizeof(CONTEXT)); \
    c.ContextFlags = contextFlags; \
    RtlCaptureContext(&c); \
    } while(0);
#endif

    // The IMAGEHLP_MODULE wrapper class
    struct CImageHlp_Module : public IMAGEHLP_MODULE64
    {
        CImageHlp_Module ( )
        {
            memset ( this , NULL , sizeof ( IMAGEHLP_MODULE64 ) ) ;
            SizeOfStruct = sizeof ( IMAGEHLP_MODULE64 ) ;
        }
    } ;

    // The IMAGEHLP_LINE wrapper class
    struct CImageHlp_Line : public IMAGEHLP_LINE64
    {
        CImageHlp_Line ( )
        {
            memset ( this , NULL , sizeof ( IMAGEHLP_LINE64 ) ) ;
            SizeOfStruct = sizeof ( IMAGEHLP_LINE64 ) ;
        }
    } ;

    class CFCrashHandlerDialog;
    class CFStackWalker
    {
        //ע�⣺����಻���̰߳�ȫ�ģ���Ҫ�Ļ��������Լ�����ͬ��
    public:           
        typedef enum StackWalkOptions
        {
            RetrieveNone = 0,           //! No addition info will be retrieved,(only the address is available)
            RetrieveSymbol = 1,         //! Try to get the symbol-name
            RetrieveLine = 2,           //! Try to get the line for this symbol
            RetrieveModuleInfo = 4,     //! Try to retrieve the module-infos
            RetrieveParams = 8,         //! Try to retrieve the params info
            RetrieveFileVersion = 0x10, //! Also retrieve the version for the DLL/EXE
            RetrieveVerbose = 0x1F,     //! Contains all the above
            SymBuildPath = 0x100,       //! Generate a "good" symbol-search-path
            SymUseSymSrv = 0x200,       //! Also use the public Microsoft-Symbol-Server
            SymAll = 0x300,             //! Contains all the above "Sym"-options
            OptionsAll = 0xFFFF,        //! Contains all options
            OptionDefualt = OptionsAll  //! Default Option
        } StackWalkOptions;

        enum { STACKWALK_MAX_NAMELEN = 512 }; // max name length for found symbols


        // Entry for each CallStack-Entry
        typedef struct CallStackEntry
        {
            DWORD64 offset;  // if 0, we have no valid entry
            DWORD64 offsetFromSmybol;
            DWORD64 baseOfImage;
            DWORD64 Params[4];

            DWORD offsetFromLine;
            DWORD lineNumber;
            DWORD symType;
            DWORD SegCs;

            TCHAR name[STACKWALK_MAX_NAMELEN];
            TCHAR undName[STACKWALK_MAX_NAMELEN];
            TCHAR undFullName[STACKWALK_MAX_NAMELEN];
            TCHAR lineFileName[STACKWALK_MAX_NAMELEN];
            TCHAR moduleName[STACKWALK_MAX_NAMELEN];
            TCHAR loadedImageName[STACKWALK_MAX_NAMELEN];
            TCHAR symTypeString[STACKWALK_MAX_NAMELEN];
        } CallStackEntry;
        enum CallStackEntryType {firstEntry, nextEntry, lastEntry};

        //typedef BOOL (__stdcall *PReadProcessMemoryRoutine)(
        //    HANDLE      hProcess,
        //    DWORD64     qwBaseAddress,
        //    PVOID       lpBuffer,
        //    DWORD       nSize,
        //    LPDWORD     lpNumberOfBytesRead,
        //    LPVOID      pUserData  // optional data, which was passed in "ShowCallstack"
        //    );

        FTLINLINE CFStackWalker(int Options = OptionDefualt, LPCTSTR pszSymPath = NULL,
            DWORD dwProcessId = GetCurrentProcessId(),
            HANDLE hProcess = GetCurrentProcess());
        FTLINLINE virtual ~CFStackWalker();
        FTLINLINE BOOL GetCallStackArray(HANDLE hThread, const CONTEXT *pContent);
        FTLINLINE INT GetStackTraceNum() const;
        FTLINLINE LPCTSTR GetStackTraceStringByIndex(INT index) const;
    protected:
        FTLINLINE BOOL InitSymEng();
        FTLINLINE BOOL CleanSymEng();
        FTLINLINE BOOL GetModuleListByToolHelp32(HANDLE hProcess, DWORD pid);
        FTLINLINE BOOL GetModuleListByPSAPI(HANDLE hProcess);
        FTLINLINE BOOL LoadModuleInfo(HANDLE hProcess, LPCTSTR img, LPCTSTR mod, DWORD baseAddr, DWORD size);
        FTLINLINE static BOOL __stdcall myReadProcessMemoryProc(
            HANDLE      hProcess,
            DWORD64     qwBaseAddress,
            PVOID       lpBuffer,
            DWORD       nSize,
            LPDWORD     lpNumberOfBytesRead
            );
        FTLINLINE void AddCallStackEntry(CallStackEntryType eType, CallStackEntry &entry);
        FTLINLINE void ClearCallStack();
    protected:
        CONTEXT m_Context;
        BOOL    m_bModulesLoaded;
        BOOL    m_bSymEngInit;
        INT     m_Options;
        LPTSTR  m_pszSymPath;
        HANDLE  m_hProcess;
        DWORD   m_dwProcessId;
        typedef std::vector<LPTSTR>  CallStackArray;
        typedef CallStackArray::iterator    CallStackIterator;
        CallStackArray  m_CallStatcks;
    };

    class CFCrashHandler;
}


namespace FTL
{
    class CFCrashHandlerDialog : public CFResourcelessDlg<CFCrashHandlerDialog>
    {
    public:
        BEGIN_MSG_MAP(CFCrashHandlerDialog)
			COMMAND_HANDLER(IDC_BTN_CREATE_MINIDUMP, BN_CLICKED, OnCreateMiniDumpClick)
			COMMAND_HANDLER(IDC_BTN_SAVE_STACK, BN_CLICKED, OnSaveStackClick)
            MESSAGE_HANDLER(WM_INITDIALOG,OnInitDialog)
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
