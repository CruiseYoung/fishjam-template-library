#ifndef FTL_CRASH_HANDLER_HPP
#define FTL_CRASH_HANDLER_HPP
#pragma once

#ifdef USE_EXPORT
#  include "ftlCrashHandler.h"
#endif
#include <atlfile.h>
#include "ftlFile.h"
//#include <atldlgs.h>
#include <WindowsX.h>
namespace FTL
{
    // Dailog creation functions
    template<typename T>
    HWND CFResourcelessDlg<T>::Create(HWND hWndParent/* = ::GetActiveWindow()*/, LPARAM dwInitParam/* = NULL*/)
    {
        m_bModal	= false;
        T* pT		= static_cast<T*>(this);
        m_iSize		= 0;
        m_pTemplate	= NULL;
        m_iOffset	= 0;
        m_pOffset	= NULL;
        int dummy	= 0;
        m_pOffset	= &dummy;

        // Get the size of the template
        pT->CreateDlg();

        // Create the template buffer
        m_pTemplate = (BYTE*)malloc(m_iSize);

        HWND retVal = 0;
        if (m_pTemplate)
        {
            // Add window data
            _Module.AddCreateWndData(&m_thunk.cd, (CDialogImplBase*)this);

            m_pOffset = &m_iOffset;
            pT->CreateDlg();

            // Set the number of controls, windows does not support more than 255 controls, create them in OnInitDialog()
            FTLASSERT(m_uiNumCtrls < 255);
            ((DLGTEMPLATE*)m_pTemplate)->cdit = m_uiNumCtrls;

            // Display the dialog
            retVal = ::CreateDialogIndirectParam(GetModuleHandle(NULL), (DLGTEMPLATE*)m_pTemplate, hWndParent, (DLGPROC)T::StartDialogProc, dwInitParam);

            // Clean up
            free(m_pTemplate);

            ATLASSERT(m_hWnd == retVal);
        }
        return retVal;
    }
    template<typename T>
    BOOL CFResourcelessDlg<T>::DestroyWindow()
    {
        FTLASSERT(::IsWindow(m_hWnd));
        FTLASSERT(!m_bModal);
        return ::DestroyWindow(m_hWnd);
    }
    template<typename T>
    INT_PTR CFResourcelessDlg<T>::DoModal(HWND hWndParent/* = ::GetActiveWindow()*/, LPARAM dwInitParam/* = NULL*/)
    {
        m_bModal	= true;
        T* pT		= static_cast<T*>(this);
        m_iSize		= 0;
        m_pTemplate = NULL;
        m_iOffset	= 0;
        m_pOffset	= NULL;
        int dummy	= 0;
        m_pOffset	= &dummy;

        // Get the size of the template
        pT->CreateDlg();

        // Create the template buffer
        m_pTemplate = (BYTE*)malloc(m_iSize);
        INT_PTR retVal = 0;
        if (m_pTemplate)
        {
            ZeroMemory(m_pTemplate,m_iSize);

            // Add window data
			ATLASSERT(_pModule);
			if (_pModule)
			{
				_pModule->AddCreateWndData(&m_thunk.cd, (CDialogImplBase*)this);
			}
#if 0
            _Module.AddCreateWndData(&m_thunk.cd, (CDialogImplBase*)this);
#endif
            m_pOffset = &m_iOffset;
            pT->CreateDlg();

            // Set the number of controls, windows does not support more than 255 controls, create them in OnInitDialog()
            FTLASSERT(m_uiNumCtrls < 255);
            ((DLGTEMPLATE*)m_pTemplate)->cdit = m_uiNumCtrls;

            // Display the dialog,����� CreateDialogIndirectParam ���Ƿ�ģ̬�Ի���
            retVal = ::DialogBoxIndirectParam(GetModuleHandle(NULL), (DLGTEMPLATE*)m_pTemplate, hWndParent, (DLGPROC)T::StartDialogProc, dwInitParam);

            // Clean up
            free(m_pTemplate);
        }
        return retVal;
    }
    // Accessor functions
    template<typename T>
    bool CFResourcelessDlg<T>::IsModal() const	        
    {
        return m_bModal;
    }
    template<typename T>
    UINT CFResourcelessDlg<T>::NumberOfControls()	const
    {
        return m_uiNumCtrls;
    }
    template<typename T>
    void CFResourcelessDlg<T>::EndDialog(INT_PTR iResult)
    {
        FTLASSERT(::IsWindow(m_hWnd));
        if (m_bModal)
            ::EndDialog(m_hWnd, iResult);
        else
            DestroyWindow();
    }
    template<typename T>
    LRESULT CFResourcelessDlg<T>::OnCloseCmd(UINT, int iId, HWND, BOOL&)
    {
        EndDialog(iId);
        return 0;
    }
    // Template create functions
    template<typename T>
    void CFResourcelessDlg<T>::CreateDlgTemplate(ATL::_U_STRINGorID pszTitle, 
        DWORD dwStyle, DWORD dwExStyle, 
        short x, short y, short cx, short cy, 
        short sFontSize/* = 0*/, 
        ATL::_U_STRINGorID pszFontName/* = (UINT)0*/,
        ATL::_U_STRINGorID pszMenu/* = (UINT)0*/, 
        ATL::_U_STRINGorID pszWndClass/* = (UINT)0*/)
    {
        DLGTEMPLATE* pDlgTemplate	= (DLGTEMPLATE*)(m_pTemplate + m_iOffset);
        int size					= sizeof(*pDlgTemplate);
        WORD* pWrite				= NULL;

        FTLASSERT(!pszFontName.m_lpstr == !(dwStyle & DS_SETFONT));
        FTLASSERT((dwStyle & DS_SHELLFONT) != DS_SHELLFONT);
        FTLASSERT(!m_iOffset);

        // Set the DLGTEMPLATE data
        if (pDlgTemplate)
        {
            pDlgTemplate->style				= dwStyle;
            pDlgTemplate->dwExtendedStyle	= dwExStyle;
            pDlgTemplate->x					= x;
            pDlgTemplate->y					= y;
            pDlgTemplate->cx				= cx;
            pDlgTemplate->cy				= cy;

            // Set the pointer
            pWrite = (WORD*)(pDlgTemplate + 1);
        }

        // Set the menu, window class name, title, font size and font name
        size += WriteString(pWrite, pszMenu.m_lpstr, true);
        size += WriteString(pWrite, pszWndClass.m_lpstr);
        size += WriteString(pWrite, pszTitle.m_lpstr);

        size += 2;
        if (pWrite && pszFontName.m_lpstr)
            *pWrite++ = sFontSize;
        size += WriteString(pWrite, pszFontName.m_lpstr);

        // Align the pointer to DWORD
        int tmpsize = size;
        size = (size + 3) & (~3);
        if (pWrite)
        {
            memset(pWrite, 0, size - tmpsize);
        }
        // Set the number of controls to zero
        m_uiNumCtrls = 0;

        m_iSize += size;
        *m_pOffset += size;
    }
    template<typename T>
    void CFResourcelessDlg<T>::AddDlgItem(ATL::_U_STRINGorID pszTitle, 
        DWORD dwStyle, DWORD dwExStyle, 
        short x, short y, short cx, short cy, 
        short id, 
        ATL::_U_STRINGorID pszWndClass/* = (UINT)0*/, 
        short sCreateDataSize/* = 0*/, 
        void* pCreateData/* = NULL*/)
    {
        DLGITEMTEMPLATE* pDlgItemTemplate = (DLGITEMTEMPLATE*)(m_pTemplate + m_iOffset);
        int size		= sizeof(*pDlgItemTemplate);
        WORD* pWrite	= NULL;

        FTLASSERT(!pCreateData == !sCreateDataSize);

        if (pDlgItemTemplate)
        {
            pDlgItemTemplate->style				= dwStyle|WS_VISIBLE|WS_CHILD;
            pDlgItemTemplate->dwExtendedStyle	= dwExStyle;
            pDlgItemTemplate->x					= x;
            pDlgItemTemplate->y					= y;
            pDlgItemTemplate->cx				= cx;
            pDlgItemTemplate->cy				= cy;
            pDlgItemTemplate->id				= id;

            pWrite = (WORD*)(pDlgItemTemplate + 1);
        }

        // Set the WndClass & Title
        size += WriteString(pWrite, pszWndClass.m_lpstr, true);
        size += WriteString(pWrite, pszTitle.m_lpstr);

        // Set create data
        size += sCreateDataSize + 2;
        if (pWrite)
        {
            if (pCreateData)
            {
                // Write data size & data
                *pWrite = sCreateDataSize + 2;
                memcpy(pWrite + 1, pCreateData, sCreateDataSize);
                // Increase the pointer
                pWrite += *pWrite;
            }
            else
            {
                // Set data to zero and increase
                *pWrite++ = 0;
            }
        }

        // Align the pointer to DWORD
        int tmpsize = size;
        size = (size + 3) & (~3);
        if (pWrite)
            memset(pWrite, 0, size - tmpsize);

        // Increase the number of controls
        m_uiNumCtrls++;

        m_iSize += size;
        *m_pOffset += size;
    }
    template<typename T>
    void CFResourcelessDlg<T>::AddButton(ATL::_U_STRINGorID pszTitle, DWORD dwStyle, DWORD dwExStyle, short x, short y, short cx, short cy, short wId)
    {
        AddDlgItem(pszTitle, dwStyle, dwExStyle, x, y, cx, cy, wId, DLG_BUTTON);
    }
    template<typename T>
    void CFResourcelessDlg<T>::AddEditBox(ATL::_U_STRINGorID pszTitle, DWORD dwStyle, DWORD dwExStyle, short x, short y, short cx, short cy, short wId)
    {
        AddDlgItem(pszTitle, dwStyle, dwExStyle, x, y, cx, cy, wId, DLG_EDIT);
    }
    template<typename T>
    void CFResourcelessDlg<T>::AddStatic(ATL::_U_STRINGorID pszTitle, DWORD dwStyle, DWORD dwExStyle, short x, short y, short cx, short cy, short wId)
    {
        AddDlgItem(pszTitle, dwStyle, dwExStyle, x, y, cx, cy, wId, DLG_STATIC);
    }
    template<typename T>
    void CFResourcelessDlg<T>::AddListBox(ATL::_U_STRINGorID pszTitle, DWORD dwStyle, DWORD dwExStyle, short x, short y, short cx, short cy, short wId)
    {
        AddDlgItem(pszTitle, dwStyle, dwExStyle, x, y, cx, cy, wId, DLG_LIST);
    }
    template<typename T>
    void CFResourcelessDlg<T>::AddScrollBar(ATL::_U_STRINGorID pszTitle, DWORD dwStyle, DWORD dwExStyle, short x, short y, short cx, short cy, short wId)
    {
        AddDlgItem(pszTitle, dwStyle, dwExStyle, x, y, cx, cy, wId, DLG_SCROLLBAR);
    }
    template<typename T>
    void CFResourcelessDlg<T>::AddCombo(ATL::_U_STRINGorID pszTitle, DWORD dwStyle, DWORD dwExStyle, short x, short y, short cx, short cy, short wId)
    {
        AddDlgItem(pszTitle, dwStyle, dwExStyle, x, y, cx, cy, wId, DLG_COMBO);
    }
    template<typename T>
    int CFResourcelessDlg<T>::WriteString(WORD* &dest, LPCTSTR pszString, bool bWriteResource/* = false*/)
    {
        int len = 1;
        if (pszString || bWriteResource)
        {
            if (bWriteResource && IS_INTRESOURCE(pszString))
            {
                if (pszString == 0)
                    len = 1;
                else
                    len = 2;
                if (dest)
                {
                    if (pszString == 0)
                        *dest++ = 0;
                    else
                    {
                        *dest++ = 0xFFFF;
                        *dest++ = (WORD)pszString;
                    }
                }
            }
            else
            {
                if (IS_INTRESOURCE(pszString))
                {
                    // Load a string from a resource
                    LPTSTR buffer = (LPTSTR)malloc(1024 /sizeof(TCHAR));
                    AtlLoadString((UINT)(DWORD_PTR)(pszString), buffer, 1024 / sizeof(TCHAR));
#ifdef UNICODE
                    len = (int)(_tcslen(pszString) + 1);
                    if (dest)
                    {
#pragma warning (disable : 4996)
                        wcsncpy((LPWSTR)dest,pszString,len);
#pragma warning(default : 4996 )
                        dest += len;
                    }
#else
                    len = MultiByteToWideChar(CP_THREAD_ACP, 0, buffer, -1, NULL, 0);
                    if (dest)
                    {
                        MultiByteToWideChar(CP_THREAD_ACP, 0, buffer, -1, (LPWSTR)dest, 1024);
                        dest += len;
                    }
#endif
                    free(buffer);
                }
                else
                {
#ifdef UNICODE
                    len = (int)(_tcslen(pszString) + 1);
                    if (dest)
                    {
#pragma warning (disable : 4996)
                        wcsncpy((LPWSTR)dest,pszString,len);
#pragma warning(default : 4996 )
                        dest += len;
                    }
#else
                    len = MultiByteToWideChar(CP_THREAD_ACP, 0, pszString, -1, NULL, 0);
                    if (dest)
                    {
                        MultiByteToWideChar(CP_THREAD_ACP, 0, pszString, -1, (LPWSTR)dest, 1024);
                        dest += len;
                    }
#endif
                }
            }
        }
        else if (dest)
        {
            *dest++ = 0;
        }
        return len * sizeof(WORD);
    }

    CFStackWalker::CFStackWalker(int Options/* = OptionDefualt*/,
        LPCTSTR pszSymPath/* = NULL*/,
        DWORD dwProcessId/* = GetCurrentProcessId()*/,
        HANDLE hProcess/* = GetCurrentProcess()*/)
    {
        m_bModulesLoaded = FALSE;
        m_bSymEngInit = FALSE;
        m_Options = Options;
        m_dwProcessId = dwProcessId;
        m_hProcess = hProcess; //OpenProcess (PROCESS_ALL_ACCESS,FALSE,dwProcessId);// 

        if (NULL == pszSymPath)
        {
            //m_pszSymPath = new TCHAR[1];
            //m_pszSymPath[0] = NULL;
            m_pszSymPath = NULL;
        }
        else
        {
            size_t len = _tcslen(pszSymPath);
            m_pszSymPath = new TCHAR[len + 1];
            StringCchCopy(m_pszSymPath,len + 1,pszSymPath);
        }
    }

    CFStackWalker::~CFStackWalker()
    {
        ClearCallStack();
        CleanSymEng();
        SAFE_DELETE_ARRAY(m_pszSymPath);
    }

    BOOL CFStackWalker::InitSymEng()
    {
        BOOL bRet = TRUE;
        if (FALSE == m_bSymEngInit)
        {
            CFStringFormater strFormater(1024);
            if (m_pszSymPath != NULL && m_pszSymPath[0] != 0)
            {
                strFormater.AppendFormat(TEXT("%s;"),m_pszSymPath);
            }

            if ( (m_Options & SymBuildPath) != 0)
            {
                strFormater.AppendFormat(_T(".;"));

                // Now first add the (optional) provided sympath:
                TCHAR szTemp[1024] = {0};
                // Now add the current directory:
                if (GetCurrentDirectory(_countof(szTemp), szTemp) > 0)
                {
                    szTemp[_countof(szTemp)-1] = 0;
                    strFormater.AppendFormat(_T("%s;"),szTemp);
                }

                // Now add the path for the main-module:
                ZeroMemory(szTemp,_countof(szTemp) * sizeof(TCHAR));
                if (GetModuleFileName(NULL, szTemp, _countof(szTemp)) > 0)
                {
                    szTemp[_countof(szTemp)-1] = 0;
                    for (LPTSTR p = (szTemp+_tcslen(szTemp)-1); p >= szTemp; --p)
                    {
                        // locate the rightmost path separator
                        if ( (*p == _T('\\')) || (*p == _T('/')) || (*p == _T(':')) )
                        {
                            *p = 0;
                            break;
                        }
                    }  // for (search for path separator...)
                    if (_tcslen(szTemp) > 0)
                    {
                        strFormater.AppendFormat(TEXT("%s;"),szTemp);
                    }
                }

                ZeroMemory(szTemp,_countof(szTemp) * sizeof(TCHAR));
                if (GetEnvironmentVariable(_T("_NT_SYMBOL_PATH"), szTemp, _countof(szTemp)) > 0)
                {
                    szTemp[_countof(szTemp)-1] = 0;
                    strFormater.AppendFormat(TEXT("%s;"),szTemp);
                }

                ZeroMemory(szTemp,_countof(szTemp) * sizeof(TCHAR));
                if (GetEnvironmentVariable(_T("_NT_ALTERNATE_SYMBOL_PATH"), szTemp, _countof(szTemp)) > 0)
                {
                    szTemp[_countof(szTemp)-1] = 0;
                    strFormater.AppendFormat(TEXT("%s;"),szTemp);
                }

                ZeroMemory(szTemp,_countof(szTemp) * sizeof(TCHAR));
                if (GetEnvironmentVariable(_T("SYSTEMROOT"), szTemp, _countof(szTemp)) > 0)
                {
                    szTemp[_countof(szTemp)-1] = 0;
                    strFormater.AppendFormat(TEXT("%s;%s\\System32;"),szTemp,szTemp);
                }
            }

            USES_CONVERSION;

            DWORD symOptions = SymGetOptions();
            symOptions |= SYMOPT_LOAD_LINES;
            //symOptions |= SYMOPT_FAIL_CRITICAL_ERRORS;
            //symOptions |= SYMOPT_NO_PROMPTS;
            symOptions = SymSetOptions(symOptions);

            API_VERIFY(SymInitialize(m_hProcess,T2A((LPTSTR)(LPCTSTR)strFormater),TRUE));
            m_bSymEngInit = bRet;
        }
        return bRet;
    }

    BOOL CFStackWalker::CleanSymEng()
    {
        BOOL bRet = TRUE;
        if (m_bSymEngInit)
        {
            API_VERIFY(SymCleanup(m_hProcess));
            m_bSymEngInit = FALSE;
        }
        return bRet;
    }

    BOOL CFStackWalker::GetModuleListByToolHelp32(HANDLE hProcess, DWORD pid)
    {
        BOOL bRet = FALSE;
        HANDLE hSnap = CreateToolhelp32Snapshot( TH32CS_SNAPMODULE, pid);
        API_VERIFY(INVALID_HANDLE_VALUE != hSnap);
        if (INVALID_HANDLE_VALUE == hSnap )
        {
            return FALSE;
        }

        MODULEENTRY32 me32 = {0};
        me32.dwSize = sizeof(me32);
        API_VERIFY(Module32First( hSnap, &me32));
        int cnt = 0;
        while (bRet)
        {
            LoadModuleInfo(hProcess, me32.szExePath, me32.szModule, (DWORD)(DWORD_PTR) me32.modBaseAddr, me32.modBaseSize);
            cnt++;
            API_VERIFY_EXCEPT1(Module32Next(hSnap, &me32),ERROR_NO_MORE_FILES);
        }
        SAFE_CLOSE_HANDLE(hSnap,NULL);
        return cnt > 0;
    }

    BOOL CFStackWalker::GetModuleListByPSAPI(HANDLE hProcess)
    {
        BOOL bRet = FALSE;
        HMODULE *pModules = NULL;
        DWORD cbNeeded = 0;
        MODULEINFO mi = {0};
        DWORD ModuleCount = 0; 
        API_VERIFY(EnumProcessModules(hProcess, NULL, 0, &cbNeeded));
        if (bRet)
        {
            DWORD cnt = cbNeeded / sizeof(HMODULE);
            pModules = new HMODULE[cnt];
            API_VERIFY(EnumProcessModules(hProcess, pModules, cbNeeded, &cbNeeded));
            if (bRet)
            {
                for (DWORD i = 0; i < cnt; i++ )
                {
                    API_VERIFY(GetModuleInformation(hProcess, pModules[i], &mi, sizeof(mi)));
                    TCHAR moduleFileName[MAX_PATH] = {0};
                    API_VERIFY(GetModuleFileNameEx(hProcess, pModules[i],moduleFileName , _countof(moduleFileName)) > 0);
                    TCHAR moduleBaseName[MAX_PATH] = {0};
                    API_VERIFY(GetModuleBaseName(hProcess, pModules[i], moduleBaseName, _countof(moduleBaseName)) > 0 );

                    API_VERIFY(LoadModuleInfo(hProcess,moduleFileName , moduleBaseName, (DWORD)(DWORD_PTR) mi.lpBaseOfDll, mi.SizeOfImage));
                    ModuleCount++;
                }
            }
            SAFE_DELETE_ARRAY(pModules);
        }
        return ModuleCount != 0;
    }

    BOOL CFStackWalker::LoadModuleInfo(HANDLE hProcess, LPCTSTR img, LPCTSTR mod, DWORD baseAddr, DWORD size)
    {
        BOOL bRet = FALSE;
        USES_CONVERSION;
        DWORD64 dwResult = SymLoadModule(hProcess, 0, T2A((LPTSTR)img), T2A((LPTSTR)mod), baseAddr, size);
#pragma TODO(����Ϊʲô��ʧ�� �� Error=2(ERROR_FILE_NOT_FOUND)
        UNREFERENCED_PARAMETER(dwResult);
        //API_VERIFY(dwResult != 0);
#if 0
        ULONGLONG fileVersion = 0;
        if ( (m_Options & RetrieveFileVersion) != 0)
        {
            VS_FIXEDFILEINFO *fInfo = NULL;
            DWORD dwHandle = NULL;
            DWORD dwSize = GetFileVersionInfoSize(img, &dwHandle);
            if (dwSize > 0)
            {
                LPBYTE vData = new BYTE[dwSize];
                if (vData != NULL)
                {
                    if (GetFileVersionInfo(img, dwHandle, dwSize, (LPVOID)vData) != 0)
                    {
                        UINT len;
                        TCHAR szSubBlock[] = _T("\\");
                        if (VerQueryValue(vData, szSubBlock, (LPVOID*) &fInfo, &len) == 0)
                        {
                            fInfo = NULL;
                        }
                        else
                        {
                            fileVersion = ((ULONGLONG)fInfo->dwFileVersionLS) + ((ULONGLONG)fInfo->dwFileVersionMS << 32);
                        }
                    }
                    SAFE_DELETE_ARRAY(vData);
                }
            }
        }
#endif 

        // Retrive some additional-infos about the module
        IMAGEHLP_MODULE  Module = {0};
        Module.SizeOfStruct = sizeof(IMAGEHLP_MODULE);
        TCHAR szSymType[20] = {0};
        API_VERIFY(SymGetModuleInfo(hProcess, baseAddr, &Module));
        if(bRet)
        {
            switch(Module.SymType)
            {
                HANDLE_CASE_TO_STRING(szSymType, _countof(szSymType), SymNone);
                HANDLE_CASE_TO_STRING(szSymType, _countof(szSymType), SymCoff);
                HANDLE_CASE_TO_STRING(szSymType, _countof(szSymType), SymCv);
                HANDLE_CASE_TO_STRING(szSymType, _countof(szSymType), SymPdb);
                HANDLE_CASE_TO_STRING(szSymType, _countof(szSymType), SymExport);
                HANDLE_CASE_TO_STRING(szSymType, _countof(szSymType), SymDeferred);
                HANDLE_CASE_TO_STRING(szSymType, _countof(szSymType), SymSym);
                HANDLE_CASE_TO_STRING(szSymType, _countof(szSymType), SymDia);
                HANDLE_CASE_TO_STRING(szSymType, _countof(szSymType), SymVirtual);
                HANDLE_CASE_TO_STRING(szSymType, _countof(szSymType), NumSymTypes);
            default:
                _ASSERT(FALSE);
                break;
            }
        }
        return bRet;
    }

    BOOL __stdcall CFStackWalker::myReadProcessMemoryProc(
        HANDLE      hProcess,
        DWORD64     qwBaseAddress,
        PVOID       lpBuffer,
        DWORD       nSize,
        LPDWORD     lpNumberOfBytesRead
        )
    {
        SIZE_T st = 0;
        BOOL bRet = ReadProcessMemory(hProcess, (LPCVOID) qwBaseAddress, lpBuffer, nSize, &st);
        *lpNumberOfBytesRead = (DWORD) st;
        return bRet;
    }


    BOOL CFStackWalker::GetCallStackArray(HANDLE hThread, const CONTEXT *pContent)
    {
        FTLASSERT(pContent);
        BOOL bRet = FALSE;
        m_Context = *pContent;
        
        IMAGEHLP_SYMBOL64 *pSym = NULL;
        IMAGEHLP_MODULE64 Module = {0};
        IMAGEHLP_LINE64 Line = {0};
        int frameNum = 0;

        API_VERIFY(InitSymEng());
        if (FALSE == GetModuleListByToolHelp32(m_hProcess,m_dwProcessId))
        {   
            API_VERIFY(GetModuleListByPSAPI(m_hProcess));
        }

        // init STACKFRAME for first call
        STACKFRAME64 sf = {0}; // in/out stack frame
        DWORD imageType = 0;
#ifdef _M_IX86  //_X86_
        // normally, call ImageNtHeader() and use machine info from PE header
        imageType = IMAGE_FILE_MACHINE_I386;
        sf.AddrPC.Offset = m_Context.Eip;
        sf.AddrPC.Mode = AddrModeFlat;
        sf.AddrFrame.Offset = m_Context.Ebp;
        sf.AddrFrame.Mode = AddrModeFlat;
        sf.AddrStack.Offset = m_Context.Esp;
        sf.AddrStack.Mode = AddrModeFlat;
#elif _M_X64  //_AMD64_
        imageType = IMAGE_FILE_MACHINE_AMD64;
        sf.AddrPC.Offset = m_Context.Rip;
        sf.AddrPC.Mode = AddrModeFlat;
        sf.AddrFrame.Offset = m_Context.Rbp;
        sf.AddrFrame.Mode = AddrModeFlat;
        sf.AddrStack.Offset = m_Context.Rsp;
        sf.AddrStack.Mode = AddrModeFlat;
#elif _M_IA64  //_IA64_
        imageType = IMAGE_FILE_MACHINE_IA64;
        sf.AddrPC.Offset = m_Context.StIIP;
        sf.AddrPC.Mode = AddrModeFlat;
        sf.AddrFrame.Offset = m_Context.IntSp;
        sf.AddrFrame.Mode = AddrModeFlat;
        sf.AddrBStore.Offset = m_Context.RsBSP;
        sf.AddrBStore.Mode = AddrModeFlat;
        sf.AddrStack.Offset = m_Context.IntSp;
        sf.AddrStack.Mode = AddrModeFlat;
#else
#  error "Platform not supported!"
#endif
        pSym = (IMAGEHLP_SYMBOL64 *) new BYTE[sizeof(IMAGEHLP_SYMBOL64) + STACKWALK_MAX_NAMELEN];
        _ASSERT(pSym);
        if (pSym)
        {
            ZeroMemory(pSym,sizeof(IMAGEHLP_SYMBOL64) + STACKWALK_MAX_NAMELEN);
            pSym->SizeOfStruct = sizeof(IMAGEHLP_SYMBOL64);
            pSym->MaxNameLength = STACKWALK_MAX_NAMELEN;

            ZeroMemory(&Line, sizeof(Line));
            Line.SizeOfStruct = sizeof(Line);

            ZeroMemory(&Module, sizeof(Module));
            Module.SizeOfStruct = sizeof(Module);

            for (frameNum = 0; ; ++frameNum )
            {
                // get next stack frame (StackWalk64(), SymFunctionTableAccess64(), SymGetModuleBase64())
                // if this returns ERROR_INVALID_ADDRESS (487) or ERROR_NOACCESS (998), you can
                // assume that either you are done, or that the stack is so hosed that the next
                // deeper frame could not be found.
                // CONTEXT need not to be supplied if imageTyp is IMAGE_FILE_MACHINE_I386!
                API_VERIFY(StackWalk64(imageType, m_hProcess, hThread, &sf, &m_Context, 
                    &CFStackWalker::myReadProcessMemoryProc, SymFunctionTableAccess64, SymGetModuleBase64, NULL));

                //DWORD64 dwModBase = SymGetModuleBase64 ( m_hProcess ,
                //	sf.AddrPC.Offset  ) ;
                //if ( 0 == dwModBase )
                //{
                //	OnDbgHelpErr(_T("SymGetModuleBase64"), GetLastError(), sf.AddrPC.Offset);
                //	break;
                //}

                if (sf.AddrPC.Offset == sf.AddrReturn.Offset)
                {
                    FTLASSERT(FALSE);
                    //OnDbgHelpErr(_T("StackWalk64-Endless-Callstack!"), 0, sf.AddrPC.Offset);
                    break;
                }

                CallStackEntry csEntry = {0};
                csEntry.offset = sf.AddrPC.Offset;
                csEntry.SegCs = m_Context.SegCs;

                if (sf.AddrPC.Offset != 0)
                {
                    // we seem to have a valid PC
                    // show procedure info (SymGetSymFromAddr64())
                    pSym->Address = sf.AddrPC.Offset;
                    if(SymGetSymFromAddr64(m_hProcess, sf.AddrPC.Offset, &(csEntry.offsetFromSmybol), pSym))
                    {
                        // TODO: Mache dies sicher...!
                        USES_CONVERSION;
                        StringCchCopy(csEntry.name,_countof(csEntry.name),A2T(pSym->Name));
                        CHAR undName[STACKWALK_MAX_NAMELEN] = {0};
                        CHAR undFullName[STACKWALK_MAX_NAMELEN] = {0};
                        // UnDecorateSymbolName()
                        UnDecorateSymbolName( pSym->Name, undName, STACKWALK_MAX_NAMELEN, UNDNAME_NAME_ONLY );
                        UnDecorateSymbolName( pSym->Name, undFullName, STACKWALK_MAX_NAMELEN, UNDNAME_COMPLETE );
                        StringCchCopy(csEntry.undName,_countof(csEntry.undName), A2T(undName));
                        StringCchCopy(csEntry.undFullName,_countof(csEntry.undFullName),A2T(undFullName));
                    }

                    // show line number info, NT5.0-method (SymGetLineFromAddr64())
                    if (RetrieveLine == (RetrieveLine & m_Options))
                    { // yes, we have SymGetLineFromAddr64()
                        if(SymGetLineFromAddr64(m_hProcess, sf.AddrPC.Offset, &(csEntry.offsetFromLine), &Line))
                        {   //ERROR_INVALID_ADDRESS
                            csEntry.lineNumber = Line.LineNumber;
                            USES_CONVERSION;
                            StringCchCopy(csEntry.lineFileName,_countof(csEntry.lineFileName), A2T(Line.FileName));
                        }
                    } // yes, we have SymGetLineFromAddr64()


                    // show module info (SymGetModuleInfo64())

                    ZeroMemory(&Module,sizeof(IMAGEHLP_MODULE64));
                    Module.SizeOfStruct = sizeof(IMAGEHLP_MODULE64);
                    if(SymGetModuleInfo64(m_hProcess,sf.AddrPC.Offset,&Module))
                    { // got module info OK
                        csEntry.symType = Module.SymType;
                        switch ( Module.SymType )
                        {
                            HANDLE_CASE_TO_STRING(csEntry.symTypeString,_countof(csEntry.symTypeString),SymNone);
                            HANDLE_CASE_TO_STRING(csEntry.symTypeString,_countof(csEntry.symTypeString),SymCoff);
                            HANDLE_CASE_TO_STRING(csEntry.symTypeString,_countof(csEntry.symTypeString),SymCv);
                            HANDLE_CASE_TO_STRING(csEntry.symTypeString,_countof(csEntry.symTypeString),SymPdb);
                            HANDLE_CASE_TO_STRING(csEntry.symTypeString,_countof(csEntry.symTypeString),SymExport);
                            HANDLE_CASE_TO_STRING(csEntry.symTypeString,_countof(csEntry.symTypeString),SymDeferred);
                            HANDLE_CASE_TO_STRING(csEntry.symTypeString,_countof(csEntry.symTypeString),SymSym);
                            HANDLE_CASE_TO_STRING(csEntry.symTypeString,_countof(csEntry.symTypeString),SymDia);
                            HANDLE_CASE_TO_STRING(csEntry.symTypeString,_countof(csEntry.symTypeString),SymVirtual);
                            HANDLE_CASE_TO_STRING(csEntry.symTypeString,_countof(csEntry.symTypeString),NumSymTypes);
                        default:
                            _ASSERT(FALSE);
                            break;
                        }

                        USES_CONVERSION;
                        StringCchCopy(csEntry.moduleName,_countof(csEntry.moduleName), A2T(Module.ModuleName));
                        csEntry.baseOfImage = Module.BaseOfImage;
                        StringCchCopy(csEntry.loadedImageName,_countof(csEntry.loadedImageName), A2T(Module.LoadedImageName));
                    } // got module info OK
                    //else
                    //{
                    //    OnDbgHelpErr(_T("SymGetModuleInfo64"), GetLastError(), sf.AddrPC.Offset);
                    //}

                    csEntry.Params[0] = sf.Params[0];
                    csEntry.Params[1] = sf.Params[1];
                    csEntry.Params[2] = sf.Params[2];
                    csEntry.Params[3] = sf.Params[3];

                } // we seem to have a valid PC
                CallStackEntryType et = nextEntry;
                if (frameNum == 0)
                    et = firstEntry;
                AddCallStackEntry(et,csEntry);

                if (sf.AddrReturn.Offset == 0)
                {
                    AddCallStackEntry(lastEntry,csEntry);
                    SetLastError(ERROR_SUCCESS);
                    break;
                }
            } // for ( frameNum )
            SAFE_DELETE_ARRAY(pSym);
            //CString strResultFormater;

            //CFStringFormater strResultFormater(1024);
            //for (size_t i = 0; i < m_CallStatcks.size(); i++)
            //{
            //    strResultFormater.AppendFormat(TEXT("%s\n"),m_CallStatcks[i]);
            //}
            //MessageBox(NULL,strResultFormater,NULL,MB_OK);
        }
        return TRUE;
    }

    INT CFStackWalker::GetStackTraceNum() const
    {
        return static_cast<INT>(m_CallStatcks.size());
    }

    LPCTSTR CFStackWalker::GetStackTraceStringByIndex(INT index) const
    {
        return m_CallStatcks[index];
    }


    void CFStackWalker::AddCallStackEntry(CallStackEntryType eType, CallStackEntry &entry)
    {
        if (firstEntry == eType)
        {
            ClearCallStack();
        }
        HRESULT hr = E_FAIL;
        CFStringFormater    strFormater(STACKWALK_MAX_NAMELEN);
        if ((eType != lastEntry) && (entry.offset != 0) )
        {
            if (entry.name[0] == 0)
            {
                COM_VERIFY(StringCchCopy(entry.name,_countof(entry.name), _T("(function-name not available)")));
            }
            if (entry.undName[0] != 0)
            {
                COM_VERIFY(StringCchCopy(entry.name, _countof(entry.name), entry.undName));
            }
            if (entry.undFullName[0] != 0)
            {
                COM_VERIFY(StringCchCopy(entry.name,_countof(entry.name),entry.undFullName));
            }
            if (entry.lineFileName[0] == 0)
            {
                COM_VERIFY(StringCchCopy(entry.lineFileName,_countof(entry.lineFileName), _T("(filename not available)")));
                if (entry.moduleName[0] == 0)
                {
                    COM_VERIFY(StringCchCopy(entry.moduleName,_countof(entry.moduleName),_T("(module-name not available)")));
                }
                strFormater.Format(_T("%s (Addr:%p): %s!%s"),
                    entry.lineFileName,(LPVOID) entry.offset, entry.loadedImageName, entry.name);
            }
            else
            {
                strFormater.Format(_T("%s (%d): %s!%s"), 
                    entry.lineFileName, entry.lineNumber,entry.loadedImageName, entry.name);
            }
            m_CallStatcks.push_back(strFormater.Detach());
        }
    }

    void CFStackWalker::ClearCallStack()
    {
        for (CallStackIterator iter = m_CallStatcks.begin();
            iter != m_CallStatcks.end(); ++iter)
        {
            delete [](*iter);
        }
        m_CallStatcks.clear();
    }

    CFCrashHandlerDialog::CFCrashHandlerDialog(PEXCEPTION_POINTERS pExcption)
    {
        m_pException = pExcption;
    }
    CFCrashHandlerDialog::~CFCrashHandlerDialog()
    {

    }
    void CFCrashHandlerDialog::CreateDlg()
    {
        CreateDlgTemplate(TEXT("Crash Handler"), 
            DS_SETFONT | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME,
            //DS_SETFONT|DS_MODALFRAME|WS_MINIMIZEBOX|WS_MAXIMIZEBOX|WS_POPUP|WS_CAPTION|WS_SYSMENU|WS_THICKFRAME|WS_OVERLAPPEDWINDOW,
            //WS_EX_DLGMODALFRAME | WS_EX_APPWINDOW | WS_EX_OVERLAPPEDWINDOW , 
            0, 
            0, 0, 420, 274, 
            8, TEXT("MS Shell Dlg"), TEXT(""), TEXT(""));

        AddListBox(TEXT(""), LBS_NOINTEGRALHEIGHT|LBS_DISABLENOSCROLL|WS_VSCROLL|WS_HSCROLL|WS_TABSTOP, 
			0, 7, 21, 406, 221, IDC_LIST_STACK);

		AddStatic(TEXT("Address:"), 0, 0, 7, 7, 38, 11, IDC_STATIC);
		AddStatic(TEXT("%s:%s"), 0, 0, 48, 7, 106, 11, IDC_STATIC_ADDRESS);
		AddStatic(TEXT("Reason Info"), 0, 0, 167, 7, 246, 11, IDC_STATIC_REASON);
		AddStatic(TEXT("Reason:"), 0, 0, 129, 7, 37, 11, IDC_STATIC);

		AddButton(TEXT("MiniDump"), 0, 0, 50, 253, 50, 14, IDC_BTN_CREATE_MINIDUMP);
		AddButton(TEXT("SaveStack"), 0, 0, 120, 253, 50, 14, IDC_BTN_SAVE_STACK);
		AddButton(TEXT("Debug"), WS_DISABLED, 0, 190, 253, 50, 14, IDC_BTN_DEBUG);
		AddButton(TEXT("Close"), 0| BS_DEFPUSHBUTTON , 0, 261, 253, 50, 14, IDOK);

        // End generated dialog        
    }

	BOOL CFCrashHandlerDialog::_GetCrashFilePrefix(LPTSTR pszBuffer, DWORD dwSize)
	{
		BOOL bRet = FALSE;
        TCHAR szModuleName[MAX_PATH] = {0};
        GetModuleFileName(NULL, szModuleName, _countof(szModuleName));
        LPCTSTR pszFileName = PathFindFileName(szModuleName);
        if (pszFileName)
        {
            FILETIME curFileTime = {0};
            SYSTEMTIME	curSysTime = {0};
            GetSystemTimeAsFileTime(&curFileTime);
            bRet = FileTimeToSystemTime(&curFileTime, &curSysTime);
            if (bRet)
            {

                HRESULT hr = StringCchPrintf(pszBuffer, dwSize, TEXT("%s-%04d%02d%02d-%02d%02d%02d"), 
                    pszFileName,
                    curSysTime.wYear, curSysTime.wMonth, curSysTime.wDay,
                    curSysTime.wHour, curSysTime.wMinute, curSysTime.wSecond);
                bRet = SUCCEEDED(hr);
            }
        }
		return bRet;
	}

	LRESULT CFCrashHandlerDialog::OnSaveStackClick(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
        //::MessageBox(m_hWnd, TEXT("Before Save Stack"), TEXT("Test"), MB_OK);

		TCHAR szCrashFile[MAX_PATH] = {0};
		_GetCrashFilePrefix(szCrashFile, _countof(szCrashFile));
		StringCchCat(szCrashFile, _countof(szCrashFile), TEXT("_StackList.txt"));

		CFileDialog dlgSave(FALSE, TEXT("txt"), szCrashFile, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, TEXT("Text File\0*.txt\0\0"));
		if (dlgSave.DoModal() == IDOK)
		{
			CFAnsiFile fileDump(tfeUnknown);
#ifdef __AFXDLGS_H__
			if (fileDump.Create(dlgSave.GetPathName()))
#else
			if (fileDump.Create(dlgSave.m_szFileName))
#endif 
			{
                fileDump.WriteFileHeader();

				TCHAR szInfo[128] = {0};
				//WORD wdStart = 0xFEFF;
				//fileDump.Write(&wdStart, sizeof(wdStart));
				//HWND hWndAddress = GetDlgItem(IDC_STATIC_ADDRESS);
				GetDlgItemText(IDC_STATIC_ADDRESS, szInfo, _countof(szInfo));
				CString strInfo;
				strInfo.Format(TEXT("New Address:%s\r\n"), szInfo);
				fileDump.WriteString(strInfo, NULL);

				GetDlgItemText(IDC_STATIC_REASON, szInfo, _countof(szInfo));
				strInfo.Format(TEXT("Reason:%s\r\n"), szInfo);
				fileDump.WriteString(strInfo, NULL);

				HWND hListStack = GetDlgItem(IDC_LIST_STACK);
				FTLASSERT(::IsWindow(hListStack));
				if (::IsWindow(hListStack))
				{
					int nMaxBuff = 1024;
					TCHAR* pszBuf = new TCHAR[nMaxBuff];
					ZeroMemory(pszBuf, sizeof(TCHAR) * nMaxBuff);

					int nCount = ListBox_GetCount(hListStack);
					for (int i = 0; i < nCount; i++)
					{
						int nSize = ListBox_GetTextLen(hListStack, i);
						if (nSize > nMaxBuff - 1)
						{
							delete [] pszBuf;
							nMaxBuff = nSize + 1;
							pszBuf = new TCHAR[nMaxBuff];
							
						}
						ListBox_GetText(hListStack, i, pszBuf);
						//DWORD dwCount = nSize * sizeof(TCHAR);
						HRESULT hr = fileDump.WriteString(pszBuf, NULL); 
                        FTLASSERT(SUCCEEDED(hr));
						fileDump.WriteString(TEXT("\r\n"), NULL);
					}
					delete [] pszBuf;
					fileDump.Close();
				}
			}
		}
		return 0;
	}

	LRESULT CFCrashHandlerDialog::OnCreateMiniDumpClick(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		BOOL bRet = FALSE;

		TCHAR szCrashFile[MAX_PATH] = {0};
		_GetCrashFilePrefix(szCrashFile, _countof(szCrashFile));
		StringCchCat(szCrashFile, _countof(szCrashFile), TEXT("_Dump.dmp"));

		CFileDialog dlgSave(FALSE, TEXT("dmp"), szCrashFile, 
			OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
			TEXT("MiniDump Files\0*.dmp\0\0"));
		if (dlgSave.DoModal() == IDOK)
		{
			CAtlFile fileDump;
#ifdef __AFXDLGS_H__
			if (SUCCEEDED(fileDump.Create(dlgSave.GetPathName(), GENERIC_WRITE, 0, CREATE_ALWAYS)))
#else
			if (SUCCEEDED(fileDump.Create(dlgSave.m_szFileName, GENERIC_WRITE, 0, CREATE_ALWAYS)))
#endif 
			{
				MINIDUMP_EXCEPTION_INFORMATION eInfo;
				eInfo.ThreadId = GetCurrentThreadId(); //����Ҫ����Ϣ���ȥ
				eInfo.ExceptionPointers = m_pException;
				eInfo.ClientPointers = FALSE;

				//Dump��������С�͵�, ��ʡ�ռ�. ���Բο�MSDN���ɸ���ϸ��Dump.
				bRet = MiniDumpWriteDump(
					GetCurrentProcess(),
					GetCurrentProcessId(),
					fileDump.m_h,
					MiniDumpNormal,
					&eInfo,
					NULL,
					NULL);

				fileDump.Close();
			}

		}
		return 0;
	}

    LPTSTR CFCrashHandlerDialog::GetFaultReason(DWORD ExceptionCode)
    {
        switch(ExceptionCode)
        {
            //�߳���ͼ��һ�����ַ���ж���д����û�����ʵ��Ĵ�ȡ������������쳣��
            //HANDLE_CASE_TO_STRING(m_FaultReason ,_countof(m_FaultReason), EXCEPTION_ACCESS_VIOLATION); 
        case EXCEPTION_ACCESS_VIOLATION:
            {
                StringCchPrintf(m_FaultReason,_countof(m_FaultReason),
                    TEXT("EXCEPTION_ACCESS_VIOLATION,Attempt to %s to %p"),
                    0 == m_pException->ExceptionRecord->ExceptionInformation[0] ? _T("Read"): _T("Write"),
                    m_pException->ExceptionRecord->ExceptionInformation[1]);
                break;
            }

            //�߳���ͼ����д��֧�ֶ��루alignment����Ӳ���ϵ�δ��������ݡ�
            HANDLE_CASE_TO_STRING(m_FaultReason ,_countof(m_FaultReason), EXCEPTION_DATATYPE_MISALIGNMENT);

            //����һ���ϵ�
            HANDLE_CASE_TO_STRING(m_FaultReason ,_countof(m_FaultReason), EXCEPTION_BREAKPOINT);

            //һ�������ݾ�����������ָ����Ƹ�֪һ��ָ����ִ����ϡ�
            HANDLE_CASE_TO_STRING(m_FaultReason ,_countof(m_FaultReason), EXCEPTION_SINGLE_STEP);

            //�߳���ͼ��ȡһ��Խ�������Ԫ�أ���Ӧ��Ӳ��֧�ֱ߽��顣
            HANDLE_CASE_TO_STRING(m_FaultReason ,_countof(m_FaultReason), EXCEPTION_ARRAY_BOUNDS_EXCEEDED);

            //��������е�һ������������������������ֵ��һ��̫С��ֵ�����ܱ�ʾ��׼�ĸ���ֵ��
            HANDLE_CASE_TO_STRING(m_FaultReason ,_countof(m_FaultReason), EXCEPTION_FLT_DENORMAL_OPERAND);

            //�߳���ͼ�ø�����0����һ������
            HANDLE_CASE_TO_STRING(m_FaultReason ,_countof(m_FaultReason), EXCEPTION_FLT_DIVIDE_BY_ZERO);

            //��������Ľ�����ܾ�ȷ��ʾ��ʮ����С����
            HANDLE_CASE_TO_STRING(m_FaultReason ,_countof(m_FaultReason), EXCEPTION_FLT_INEXACT_RESULT);

            //��ʾ�κ�û���ڴ��г��������������쳣��
            HANDLE_CASE_TO_STRING(m_FaultReason ,_countof(m_FaultReason), EXCEPTION_FLT_INVALID_OPERATION);

            //��������Ľ�������������ֵ��
            HANDLE_CASE_TO_STRING(m_FaultReason ,_countof(m_FaultReason), EXCEPTION_FLT_OVERFLOW);

            //���ڸ���������ջ��������硣
            HANDLE_CASE_TO_STRING(m_FaultReason ,_countof(m_FaultReason), EXCEPTION_FLT_STACK_CHECK);

            //��������Ľ��С�������ֵ
            HANDLE_CASE_TO_STRING(m_FaultReason ,_countof(m_FaultReason), EXCEPTION_FLT_UNDERFLOW);

            //�߳���ͼ������0����һ������
            HANDLE_CASE_TO_STRING(m_FaultReason ,_countof(m_FaultReason), EXCEPTION_INT_DIVIDE_BY_ZERO);

            //һ�����������Ľ������������ֵ�涨�ķ�Χ��
            HANDLE_CASE_TO_STRING(m_FaultReason ,_countof(m_FaultReason), EXCEPTION_INT_OVERFLOW);

            //�߳�ִ��һ��ָ�������ڵ�ǰ����ģʽ�в�����
            HANDLE_CASE_TO_STRING(m_FaultReason ,_countof(m_FaultReason), EXCEPTION_PRIV_INSTRUCTION);

            //�����ļ�ϵͳ��һ���豸�������򷵻�һ����������ɲ�������Ҫ���ҳ���ϡ�
            HANDLE_CASE_TO_STRING(m_FaultReason ,_countof(m_FaultReason), EXCEPTION_IN_PAGE_ERROR);

            //�߳�ִ����һ����Ч��ָ�����쳣���ض���CPU�ṹ�����壻�ڲ�ͬ��CPU�ϣ�ִ��һ����Чָ�������һ���ݾ�����
            HANDLE_CASE_TO_STRING(m_FaultReason ,_countof(m_FaultReason), EXCEPTION_ILLEGAL_INSTRUCTION);

            //һ���쳣��������һ�����ܼ������쳣���� EXCEPTION_CONTINUE_EXECUTION��
            HANDLE_CASE_TO_STRING(m_FaultReason ,_countof(m_FaultReason), EXCEPTION_NONCONTINUABLE_EXCEPTION);

            //�߳������˷������������ջ�ռ䡣
            HANDLE_CASE_TO_STRING(m_FaultReason ,_countof(m_FaultReason), EXCEPTION_STACK_OVERFLOW);

            //һ���쳣����������һ����Ч���쳣����ֵ������ _HANDLE,_CONTINUE_SEARCH,_CONTINUE_EXCEPTION ����֮һ��
            HANDLE_CASE_TO_STRING(m_FaultReason ,_countof(m_FaultReason), EXCEPTION_INVALID_DISPOSITION);

            //һ���߳���ͼ��ȡһ������ PAGE_GUARD �������Ե��ڴ�ҳ��
            HANDLE_CASE_TO_STRING(m_FaultReason ,_countof(m_FaultReason), EXCEPTION_GUARD_PAGE);

            //��һ������������һ����Ч�����
            HANDLE_CASE_TO_STRING(m_FaultReason ,_countof(m_FaultReason), EXCEPTION_INVALID_HANDLE);

#define EXCEPTION_CONTROL_C                                     ((DWORD )0x40010005L)
#define EXCEPTION_CONTROL_BREAK                                 ((DWORD )0X40010008L)

#define EXCEPTION_NOT_ENOUGH_QUOTA                              ((DWORD )0xC0000017L)
#define EXCEPTION_UNABLE_TO_LOCATE_DLL                          ((DWORD )0xC0000135L)
#define EXCEPTION_ORDINAL_NOT_FOUND                             ((DWORD )0xC0000138L)
#define EXCEPTION_ENTRY_POINT_NOT_FOUND                         ((DWORD )0xC0000139L)
            //�ڵ���CrashHandle��ʱ���֣���һ���˳��쳣�������󣬵ڶ��η���
#define EXCEPTION_UNKNOWN_0xC0150010                            ((DWORD )0xC0150010L)

#define EXCEPTION_DLL_INITIALIZATION_FAILED                     ((DWORD )0xC0000142L)
            //�ӳټ���ʱ���ֵĴ��󣨼� DelayImp.h ��
            // If this is a Delay-load problem, ExceptionInformation[0] points to a DelayLoadInfo structure that has detailed error info
            // PDelayLoadInfo pdli = PDelayLoadInfo(pExPtrs->ExceptionRecord->ExceptionInformation[0]);
#define EXCEPTION_MODULE_NOT_FOUND                              ((DWORD )0xc06d007eL)
#define EXCEPTION_PROCEDURE_NOT_FOUND                           ((DWORD )0xc06d007fL)

#define EXCEPTION_MICROSOFT_CPLUSPLUS_EXCEPTION	                ((DWORD )0xE06D7363L)

            HANDLE_CASE_TO_STRING(m_FaultReason ,_countof(m_FaultReason), EXCEPTION_CONTROL_C);
            HANDLE_CASE_TO_STRING(m_FaultReason ,_countof(m_FaultReason), EXCEPTION_CONTROL_BREAK);
            HANDLE_CASE_TO_STRING(m_FaultReason ,_countof(m_FaultReason), EXCEPTION_NOT_ENOUGH_QUOTA);
            HANDLE_CASE_TO_STRING(m_FaultReason ,_countof(m_FaultReason), EXCEPTION_UNABLE_TO_LOCATE_DLL);
            HANDLE_CASE_TO_STRING(m_FaultReason ,_countof(m_FaultReason), EXCEPTION_ORDINAL_NOT_FOUND);
            HANDLE_CASE_TO_STRING(m_FaultReason ,_countof(m_FaultReason), EXCEPTION_ENTRY_POINT_NOT_FOUND);
            HANDLE_CASE_TO_STRING(m_FaultReason ,_countof(m_FaultReason), EXCEPTION_UNKNOWN_0xC0150010);
            HANDLE_CASE_TO_STRING(m_FaultReason ,_countof(m_FaultReason), EXCEPTION_DLL_INITIALIZATION_FAILED);
            HANDLE_CASE_TO_STRING(m_FaultReason ,_countof(m_FaultReason), EXCEPTION_MODULE_NOT_FOUND);
            HANDLE_CASE_TO_STRING(m_FaultReason ,_countof(m_FaultReason), EXCEPTION_PROCEDURE_NOT_FOUND);

            //Visual C++ ���������ò���ϵͳ�Ľṹ���쳣����ʵ�� C++ �쳣����
            //�� throw C++ �쳣ʱ��ת��Ϊ RaiseException(EXCEPTION_MICROSOFT_CPLUSPLUS_EXCEPTION,...)
            HANDLE_CASE_TO_STRING(m_FaultReason ,_countof(m_FaultReason), EXCEPTION_MICROSOFT_CPLUSPLUS_EXCEPTION);
        default:
            {
                StringCchPrintf(m_FaultReason,_countof(m_FaultReason),
                    TEXT("Unknown Exception Code 0x%08x"),m_pException->ExceptionRecord->ExceptionCode);
            }
            break;
        }
        return m_FaultReason;
    }

    LRESULT CFCrashHandlerDialog::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
    {
        BOOL bRet = FALSE;
        CFStringFormater strFormater;
        if (m_pException)
        {
#ifdef WIN32
            //strFormater.Format(_T ( "%04X:%08X" ),
            //  m_pException->ContextRecord->SegCs,
            //  m_pException->ExceptionRecord->ExceptionAddress);
            strFormater.Format(_T ( "%08X" ),
                m_pException->ExceptionRecord->ExceptionAddress);
#else

            strFormater.Format(_T ( "%016X" ),
                m_pException->ExceptionRecord->ExceptionAddress);
#endif
            FTLTRACE(_T("Crash %08x at %s"),m_pException->ExceptionRecord->ExceptionCode,strFormater);
            SetDlgItemText(IDC_STATIC_ADDRESS,strFormater);
            SetDlgItemText(IDC_STATIC_REASON,GetFaultReason(m_pException->ExceptionRecord->ExceptionCode));

            HWND hListStack = GetDlgItem(IDC_LIST_STACK);
            FTLASSERT(::IsWindow(hListStack));
            if (::IsWindow(hListStack))
            {
                CFStackWalker stackWalker;
                API_VERIFY(stackWalker.GetCallStackArray(GetCurrentThread(),m_pException->ContextRecord));
                INT nTraceNum = stackWalker.GetStackTraceNum();
                for (INT i = 0; i < nTraceNum; i++)
                {
                    LPCTSTR pszStack = stackWalker.GetStackTraceStringByIndex(i);
                    //ListBox_AddString(hListStack,pszStack);
                    ::SendMessage(hListStack,LB_ADDSTRING,0L,(LPARAM)(LPCTSTR)(pszStack));
                }

                ListBox_SetHorizontalExtent(hListStack, 1024); //add for magin
            }
        }
        return 0;
    }

    LRESULT CFCrashHandlerDialog::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        FTLTRACE(TEXT("CFCrashHandlerDialog::OnDestroy\n"));
        bHandled = FALSE;
        return 0;
    }

    LRESULT CFCrashHandlerDialog::OnSize(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
    {
        BOOL bRet = FALSE;
        
        UINT nType = wParam;
        WORD nNewWidth = LOWORD(lParam);
        WORD nNewHeight = HIWORD(lParam);
        
        HWND hListStack = GetDlgItem(IDC_LIST_STACK);
        FTLASSERT(::IsWindow(hListStack));

        API_VERIFY(::SetWindowPos(hListStack, NULL, 0, 0, nNewWidth - 13, nNewHeight - 70, SWP_NOZORDER | SWP_NOMOVE));
        
        //HWND hBtnMiniDump = GetDlgItem(IDC_BTN_CREATE_MINIDUMP);
        //HWND hBtnSaveStack = GetDlgItem(IDC_BTN_SAVE_STACK);
        //HWND hBtnDebug = GetDlgItem(IDC_BTN_DEBUG);
        //HWND hBtnClose = GetDlgItem(IDOK);

        HWND hButtons[] = {
            GetDlgItem(IDC_BTN_CREATE_MINIDUMP),
            GetDlgItem(IDC_BTN_SAVE_STACK),
            GetDlgItem(IDC_BTN_DEBUG),
            GetDlgItem(IDOK)
        };

        for (int i = 0; i < _countof(hButtons); i++)
        {
            RECT rcOld = {0};

            API_VERIFY(::GetWindowRect(hButtons[i], &rcOld));
            API_VERIFY(ScreenToClient(&rcOld));
            API_VERIFY(::SetWindowPos(hButtons[i], NULL, rcOld.left, nNewHeight - 30, 0, 0, SWP_NOZORDER | SWP_NOSIZE));
        }

        return 0;
    }


    CFCrashHandler::CFCrashHandler()
    {
        FTLASSERT(NULL == s_pSingleCrashHandler);
        s_pSingleCrashHandler = this;
		s_pCriticalSection = new CRITICAL_SECTION;
		if (s_pCriticalSection)
		{
			InitializeCriticalSection(s_pCriticalSection);
		}
        m_pfnOrigFilt = NULL;
    }
    CFCrashHandler::~CFCrashHandler()
    {
        if (NULL != m_pfnOrigFilt)
        {
            RestoreCrashHandlerFilter();
        }
		if (s_pCriticalSection)
		{
			DeleteCriticalSection(s_pCriticalSection);
			delete s_pCriticalSection;
			s_pCriticalSection = NULL;
		}
        s_pSingleCrashHandler = NULL;
    }
    BOOL CFCrashHandler::SetDefaultCrashHandlerFilter()
    {
        FTLASSERT(NULL == m_pfnOrigFilt);
        m_pfnOrigFilt = SetUnhandledExceptionFilter(DefaultCrashHandlerFilter);
        return TRUE;
    }
    BOOL CFCrashHandler::RestoreCrashHandlerFilter()
    {
        FTLASSERT(NULL != m_pfnOrigFilt);
        SetUnhandledExceptionFilter(m_pfnOrigFilt);
        m_pfnOrigFilt = NULL;
        return TRUE;
    }


    __declspec(selectany) CFCrashHandler* CFCrashHandler::s_pSingleCrashHandler = NULL;
	__declspec(selectany) CRITICAL_SECTION* CFCrashHandler::s_pCriticalSection = NULL;
    LONG __stdcall CFCrashHandler::DefaultCrashHandlerFilter( PEXCEPTION_POINTERS pExPtrs )
    {
		OutputDebugString(_T("Enter CFCrashHandler::DefaultCrashHandlerFilter\r\n"));

		FTLASSERT(NULL != s_pSingleCrashHandler);
#if 0
		EnterCriticalSection( s_pCriticalSection ); 
#endif 
        if ( EXCEPTION_STACK_OVERFLOW == pExPtrs->ExceptionRecord->ExceptionCode )
        {
            OutputDebugString(_T("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\r\n"));
            OutputDebugString(_T("EXCEPTION_STACK_OVERFLOW occurred\r\n"));
            OutputDebugString(_T("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\r\n"));
        }
        LONG lRet = EXCEPTION_EXECUTE_HANDLER;
        //s_pExPtrs = pExPtrs;
        CFSystemUtil::SuspendProcess(GetCurrentProcessId(),TRUE,GetCurrentThreadId());

#pragma TODO(���ַ����Ƿ����)
		CComModule* pLocalModule = NULL;
		if (!_pModule)
		{
			//MessageBox(NULL, TEXT("Before new ComModule"), TEXT("Info"), MB_OK);
			pLocalModule = new CComModule();
		}

		CFCrashHandlerDialog dlg(pExPtrs);
		//dlg.SetFaultReason()
		switch (dlg.DoModal())
		{
		case CFCrashHandlerDialog::IDC_BTN_CREATE_MINIDUMP:
			break;
		case CFCrashHandlerDialog::IDC_BTN_DEBUG:
			break;
		default:
			lRet = EXCEPTION_EXECUTE_HANDLER;//EXCEPTION_CONTINUE_SEARCH;
			break;
		}
		SAFE_DELETE(pLocalModule);

#if 0
		LeaveCriticalSection(s_pCriticalSection); 
#endif

        //if (s_pfnOrigFilt)
        //{
        //    (*s_pfnOrigFilt)(pExPtrs);
        //}
        CFSystemUtil::SuspendProcess(GetCurrentProcessId(),FALSE,GetCurrentThreadId());
		OutputDebugString(_T("Leave CFCrashHandler::DefaultCrashHandlerFilter\r\n"));
        return ( lRet ) ;
    }

}//namespace FTL


#endif //FTL_CRASH_HANDLER_HPP