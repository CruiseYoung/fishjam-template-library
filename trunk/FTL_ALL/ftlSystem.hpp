#ifndef FTL_SYSTEM_HPP
#define FTL_SYSTEM_HPP
#pragma once

#ifdef USE_EXPORT
#  include "ftlsystem.h"
#endif

#include <ShlObj.h>

namespace FTL
{
    CFOSInfo::CFOSInfo()
    {
        BOOL bRet = FALSE;
		ZeroMemory(&m_OsInfo, sizeof(m_OsInfo));
        m_OsInfo.dwOSVersionInfoSize = sizeof(m_OsInfo);
        API_VERIFY(::GetVersionEx(&m_OsInfo));
        FTLTRACEEX(tlTrace,_T("GetVersionEx :{ dwPlatformId=%d, dwMajorVersion=%d, dwMinorVersion=%d,dwBuildNumber=%d,szCSDVersion=%s}\r\n"),
            m_OsInfo.dwPlatformId,m_OsInfo.dwMajorVersion,m_OsInfo.dwMinorVersion,m_OsInfo.dwBuildNumber,m_OsInfo.szCSDVersion);
    }

    CFOSInfo::OSType CFOSInfo::GetOSType() const
    {
        OSType osType = ostLowUnknown;
        DWORD dwOsVersion = m_OsInfo.dwPlatformId * 10000 + m_OsInfo.dwMajorVersion * 100 + m_OsInfo.dwMinorVersion;
        switch(dwOsVersion)
        {
        case 00000: //TODO
            FTLASSERT(FALSE);
            osType = ostLowUnknown;//ostWin32;
            break;
        case 10400:		
            osType = ostWin95;
            break;
        case 10410://OK
            {
                if (_tcscmp(m_OsInfo.szCSDVersion,_T(" A ")) == 0)
                {
                    osType = ostWin98_SE;
                }
                else
                {
                    osType = ostWin98;
                }
                break;
            }
        case 10490://OK
            osType = ostWinMe;
            break;
        case 20351:
            osType = ostWinNT3;
            break;
        case 20400:
            osType = ostWinNT4;
            break;
        case 20500://OK
            osType = ostWin2000;
            break;
        case 20501://OK
            osType = ostWinXP;
            break;
        case 20502:
            osType = ostWin2003;
            break;
        case 20600:
            osType = ostVista;
            break;
        case 20601:
            osType = ostWindows7;
            break;
        default:
            {
                _ASSERT(FALSE);
                if (dwOsVersion > 20601)
                {
                    osType = ostHighUnknown;
                }
                else
                {
                    osType = ostLowUnknown;
                }
            }
            break;
        }
        return osType;
    }

    // if NT without ServicePack, return TRUE but csCSDVersion is ""
    BOOL CFOSInfo::GetWinNTServicePackName(LPTSTR pszNTVersion,DWORD nSize)	const
    {
        HRESULT hr = E_FAIL;
        COM_VERIFY(StringCchCopy(pszNTVersion, nSize, m_OsInfo.szCSDVersion));
        return TRUE;
    }

    BOOL CFOSInfo::IsGreaterWinNT() const
    {
        BOOL bRet = (m_OsInfo.dwPlatformId >= VER_PLATFORM_WIN32_NT);
        return bRet;
    }

    DWORD CFOSInfo::GetNumberOfProcessors() const
    {
        SYSTEM_INFO sysinfo = {0};
        ::GetSystemInfo(&sysinfo);
        return sysinfo.dwNumberOfProcessors;
    }

    BOOL CFOSInfo::GetPhysicalBytes(DWORDLONG* pAvailablePhysicalBytes, DWORDLONG *pTotalPhysicalBytes) const
    {
        BOOL bRet = FALSE;
        HMODULE hKernel = LoadLibrary(_T("kernel32.dll"));
        API_VERIFY(NULL != hKernel);
        if(pAvailablePhysicalBytes)
        {
            *pAvailablePhysicalBytes = 0;
        }
        if (pTotalPhysicalBytes)
        {
            *pTotalPhysicalBytes = 0;
        }

        if (hKernel)
        {
            typedef BOOL (WINAPI *FuncGlobalMemoryStatusEx)(LPMEMORYSTATUSEX);
            FuncGlobalMemoryStatusEx pfn = (FuncGlobalMemoryStatusEx)GetProcAddress(hKernel, "GlobalMemoryStatusEx");
            API_VERIFY(NULL != pfn);
            if (pfn)
            {
                MEMORYSTATUSEX memoryStatus = {0};
                //memset(&memoryStatus, 0, sizeof(MEMORYSTATUSEX));
                memoryStatus.dwLength = sizeof(MEMORYSTATUSEX);
                API_VERIFY((*pfn)(&memoryStatus));
                if (bRet)
                {
                    if(pAvailablePhysicalBytes)
                    {
                        *pAvailablePhysicalBytes = memoryStatus.ullAvailPhys;
                    }
                    if (pTotalPhysicalBytes)
                    {
                        *pTotalPhysicalBytes = memoryStatus.ullTotalPhys;
                    }
                }
            }
            FreeLibrary(hKernel);
        }
        if (!bRet)
        {
            MEMORYSTATUS memoryStatus = {0};
            //memset(&memoryStatus, 0, sizeof(MEMORYSTATUS));
            memoryStatus.dwLength = sizeof(MEMORYSTATUS);
            GlobalMemoryStatus(&memoryStatus);
            if(pAvailablePhysicalBytes)
            {
                *pAvailablePhysicalBytes = memoryStatus.dwTotalPhys;
            }
            if (pTotalPhysicalBytes)
            {
                *pTotalPhysicalBytes = memoryStatus.dwTotalPhys;
            }
            bRet = TRUE;
        }
        return bRet;
    }

    BOOL CFOSInfo::GetVolumeVisibleName(LPCTSTR pszVolume, LPTSTR pszBuf, DWORD bufSize) const
    {
        HRESULT hr = E_FAIL;
        BOOL bRet = FALSE;

        IShellFolder* pDesktop = 0;
        COM_VERIFY(::SHGetDesktopFolder(&pDesktop));
        if (SUCCEEDED(hr))
        {
            WCHAR wPath[] = L"@:\\";
            LPITEMIDLIST pidl = NULL;
            ULONG eaten = 0;
            ULONG attributes = 0;

            USES_CONVERSION;

            wPath[0] = CT2W(pszVolume)[0];
            COM_VERIFY(pDesktop->ParseDisplayName(0, 0, wPath, &eaten, &pidl, &attributes));
            if (SUCCEEDED(hr))
            {
                STRRET strret = {0};
                if (SUCCEEDED(pDesktop->GetDisplayNameOf(pidl, SHGDN_INFOLDER, &strret)))
                {
                    LPTSTR str = 0;
                    if (SUCCEEDED(StrRetToStr(&strret, pidl, &str)))
                    {
                        COM_VERIFY(StringCchCopy(pszBuf,bufSize,str));
                        CoTaskMemFree(str);
                        bRet = TRUE;
                    }
                }
                IMalloc* pMalloc = 0;
                if (SUCCEEDED(SHGetMalloc(&pMalloc)))
                {
                    pMalloc->Free(pidl);
                    pMalloc->Release();
                }
            }
            pDesktop->Release();
        }
        if(!bRet)
        {
            SetLastError(HRESULT_CODE(hr));
        }
        return bRet;
    }

	SystemParamProperty::SystemParamProperty()
	{
		Init();
	}
	SystemParamProperty::~SystemParamProperty()
	{

	}
	BOOL SystemParamProperty::Init()
	{
		BOOL bRet = FALSE;
		API_VERIFY(::SystemParametersInfo(SPI_GETKEYBOARDSPEED, 0, &m_nKeyboardSpeed, 0));

		return bRet;
	}

	LPCTSTR SystemParamProperty::GetPropertyString()
	{

	}

	BOOL CFSystemUtil::GetVersionFromString(VERSIONINFO& ver, LPCTSTR pszVersion)
	{
		if ( NULL  == pszVersion )
		{
			return FALSE;
		}

		const TCHAR szToken[] = _T(",.");
		const int  MAX_VERSION_SIZE	= 64 ;
		DWORD dwTemp[4] = { 0 };
		//memset(dwTemp, 0x00, sizeof(dwTemp));

		TCHAR *pszTok = NULL;
		TCHAR szVersion[MAX_VERSION_SIZE] = { 0 };
		StringCchCopy(szVersion, _countof(szVersion), pszVersion) ;
		pszTok = _tcstok(szVersion, szToken) ;
		if (pszTok == NULL)
		{
			return FALSE;
		}

		*dwTemp = DWORD(_ttoi(pszTok)) ;

		for (int nIndex = 1; nIndex < 4; nIndex++)
		{
			pszTok = _tcstok(NULL, szToken) ;
			if (pszTok == NULL)
			{
				return FALSE;
			}

			*(dwTemp + nIndex) = DWORD(_ttoi(pszTok)) ;
		}

		ver.dwVer0 = dwTemp[0];
		ver.dwVer1 = dwTemp[1];
		ver.dwVer2 = dwTemp[2];
		ver.dwVer3 = dwTemp[3];

		return TRUE;
	}

	BOOL CFSystemUtil::GetCurrentUserID( LPTSTR pszUserName, int iSize)
	{
		const TCHAR SUBKEY_LOGON_INFO[] = _T( "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Authentication\\LogonUI" );
		CRegKey RegUsers;
		if ( RegUsers.Open( HKEY_LOCAL_MACHINE, SUBKEY_LOGON_INFO, KEY_READ ) != ERROR_SUCCESS )
		{
			return FALSE;
		}

		ULONG ul = MAX_PATH;
		TCHAR szBuf[MAX_PATH] = { 0 };
		if ( RegUsers.QueryStringValue( _T("LastLoggedOnUser"), szBuf, &ul ) != ERROR_SUCCESS )
		{
			return FALSE;
		}

		HRESULT hr = E_FAIL;
		ZeroMemory( pszUserName, iSize );
		COM_VERIFY(StringCchCopy( pszUserName, iSize, ::PathFindFileName( szBuf )));

		return TRUE;

	}

	LPCTSTR CFSystemUtil::GetClipboardFormatString(UINT uFormat, LPTSTR lpszFormatName, int cchMaxCount)
	{
		CHECK_POINTER_RETURN_VALUE_IF_FAIL(lpszFormatName, NULL);
		lpszFormatName[0] = TEXT('\0');
		switch (uFormat)
		{
			HANDLE_CASE_TO_STRING(lpszFormatName, cchMaxCount, CF_TEXT);
			HANDLE_CASE_TO_STRING(lpszFormatName, cchMaxCount, CF_BITMAP);
			HANDLE_CASE_TO_STRING(lpszFormatName, cchMaxCount, CF_METAFILEPICT);
			HANDLE_CASE_TO_STRING(lpszFormatName, cchMaxCount, CF_SYLK);
			HANDLE_CASE_TO_STRING(lpszFormatName, cchMaxCount, CF_DIF);
			HANDLE_CASE_TO_STRING(lpszFormatName, cchMaxCount, CF_TIFF);
			HANDLE_CASE_TO_STRING(lpszFormatName, cchMaxCount, CF_OEMTEXT);
			HANDLE_CASE_TO_STRING(lpszFormatName, cchMaxCount, CF_DIB);
			HANDLE_CASE_TO_STRING(lpszFormatName, cchMaxCount, CF_PALETTE);
			HANDLE_CASE_TO_STRING(lpszFormatName, cchMaxCount, CF_PENDATA);
			HANDLE_CASE_TO_STRING(lpszFormatName, cchMaxCount, CF_RIFF);
			HANDLE_CASE_TO_STRING(lpszFormatName, cchMaxCount, CF_WAVE);
			HANDLE_CASE_TO_STRING(lpszFormatName, cchMaxCount, CF_UNICODETEXT);
			HANDLE_CASE_TO_STRING(lpszFormatName, cchMaxCount, CF_ENHMETAFILE);
			HANDLE_CASE_TO_STRING(lpszFormatName, cchMaxCount, CF_HDROP);
			HANDLE_CASE_TO_STRING(lpszFormatName, cchMaxCount, CF_LOCALE);
#if(WINVER >= 0x0500)
			HANDLE_CASE_TO_STRING(lpszFormatName, cchMaxCount, CF_DIBV5);
#endif /* WINVER >= 0x0500 */

			HANDLE_CASE_TO_STRING(lpszFormatName, cchMaxCount, CF_OWNERDISPLAY);
			HANDLE_CASE_TO_STRING(lpszFormatName, cchMaxCount, CF_DSPTEXT);
			HANDLE_CASE_TO_STRING(lpszFormatName, cchMaxCount, CF_DSPBITMAP);
			HANDLE_CASE_TO_STRING(lpszFormatName, cchMaxCount, CF_DSPMETAFILEPICT);
			HANDLE_CASE_TO_STRING(lpszFormatName, cchMaxCount, CF_DSPENHMETAFILE);
			HANDLE_CASE_TO_STRING(lpszFormatName, cchMaxCount, CF_PRIVATEFIRST);
			HANDLE_CASE_TO_STRING(lpszFormatName, cchMaxCount, CF_PRIVATELAST);
			HANDLE_CASE_TO_STRING(lpszFormatName, cchMaxCount, CF_GDIOBJFIRST);
			HANDLE_CASE_TO_STRING(lpszFormatName, cchMaxCount, CF_GDIOBJLAST);
		default:
			{
				int nRet = GetClipboardFormatName(uFormat, lpszFormatName, cchMaxCount);
				if (0 == nRet)
				{
					API_ASSERT(FALSE);
					StringCchPrintf(lpszFormatName, cchMaxCount, TEXT("Unknown:%d"), uFormat);
				}
			}
			break;
		}
		return lpszFormatName;
	}

    BOOL CFSystemUtil::Is64BitWindows()
    {
        BOOL bIsWow64 = FALSE;
#if defined(_WIN64)
        //64λ����ֻ����64λϵͳ������
        bIsWow64 = TRUE;
#elif defined(_WIN32)
        //32λ�����ͬʱ�� 32/64 λϵͳ������
        HMODULE hKernel32 = GetModuleHandle(TEXT("kernel32.dll"));
        FTLASSERT(hKernel32);

        typedef BOOL (WINAPI *LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);
        LPFN_ISWOW64PROCESS fnIsWow64Process = (LPFN_ISWOW64PROCESS)GetProcAddress(hKernel32, "IsWow64Process");
        if (fnIsWow64Process)
        {
            if (!fnIsWow64Process(GetCurrentProcess(),&bIsWow64))
            {
                bIsWow64 = FALSE;
            }
        }
#endif
        return bIsWow64;
    }

    BOOL CFSystemUtil::SuspendProcess(DWORD dwProcessID, BOOL fSuspend, DWORD skipedThreadId/* = ::GetCurrentThreadId()*/)
    {
        BOOL bRet = FALSE;
        //Get the list of threads in the system
        HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD,dwProcessID);
        if(hSnapshot != INVALID_HANDLE_VALUE)
        {
            //Walk the list of threads
            THREADENTRY32    te = {sizeof(te)};
            //ö�ٽ����������߳�
            API_VERIFY(Thread32First(hSnapshot, &te));
            for(; bRet; bRet = Thread32Next(hSnapshot, &te))
            {
                //Is this thread in the desired process and not the skipped thread(UI)
                if(te.th32OwnerProcessID == dwProcessID && te.th32ThreadID != skipedThreadId)
                {
                    //�߳�ID��ȫ�֣�Windows���������߳�Handle�ǽ����еı����������� ID -> Handle(��ȫ����)��ֻ��ѭ��ö��

                    //Attemp to covert the thread ID into a handle.
                    //Note: Win98 not support
                    HANDLE hThread = ::OpenThread (THREAD_SUSPEND_RESUME,FALSE,te.th32ThreadID);
                    if(hThread != NULL)
                    {
                        if(fSuspend)
                        {
                            API_VERIFY(-1 != ::SuspendThread(hThread));
                        }
                        else
                        {
                            API_VERIFY(-1 != ::ResumeThread(hThread));
                        }
                        SAFE_CLOSE_HANDLE(hThread,NULL);
                    }

                }
            }
            SAFE_CLOSE_HANDLE(hSnapshot,NULL);
        }
        return bRet;
    }

#ifdef UNICODE
#define CLIPBOARDFMT    CF_UNICODETEXT
#else
#define CLIPBOARDFMT    CF_TEXT
#endif

    BOOL CFSystemUtil::CopyTextToClipboard( LPCTSTR szMem , HWND hWndOwner )
    {
        FTLASSERT ( FALSE == IsBadStringPtr ( szMem , MAX_PATH ) ) ;
        if ( TRUE == IsBadStringPtr ( szMem , MAX_PATH ) )
        {
            return ( FALSE ) ;
        }

        BOOL bRet = TRUE ;
        BOOL bOpenedCB = FALSE ;
        if ( TRUE == ::OpenClipboard ( hWndOwner ) )
        {
            bOpenedCB = TRUE ;
            if ( TRUE == ::EmptyClipboard ( ) )
            {
                // Do the goofy memory stuff.
                HGLOBAL hGlob = ::GlobalAlloc ( GMEM_MOVEABLE         ,
                    (_tcslen ( szMem ) + 1) *
                    sizeof ( TCHAR )       );
                if ( NULL != hGlob )
                {
                    TCHAR * szClipMem = (TCHAR*)GlobalLock ( hGlob ) ;
                    FTLASSERT ( NULL != szMem ) ;
                    StringCchCopy(szClipMem , _tcslen ( szMem ), szMem);
                    //_tcscpy ( szClipMem, szMem) ;

                    GlobalUnlock ( hGlob ) ;
                    if ( NULL == ::SetClipboardData ( CLIPBOARDFMT , hGlob ) )
                    {
                        FTLASSERT ( !"SetClipboardData failed!!" ) ;
                    }
                }
                else
                {
                    FTLASSERT ( !"Unable to GlobalAlloc memory!!" ) ;
                    bRet = FALSE ;
                }
            }
            else
            {
                FTLASSERT ( !"Unable to empty the clipboard!!" ) ;
                bRet = FALSE ;
            }
        }
        else
        {
            FTLASSERT ( !"Unable to open the clipboard!!" ) ;
            bRet = FALSE ;
        }
        if ( TRUE == bOpenedCB )
        {
            CloseClipboard ( ) ;
        }
        return ( bRet ) ;
    }

    BOOL CFSystemUtil::IsLittleSystem()
    {
        FTLASSERT(sizeof(int) == 4);
        BOOL bisLittleSystem = TRUE;

        int tmpNumber = 0x12345678;
        const unsigned char bigNumberBuf[sizeof(int)] = {0x12, 0x34, 0x56, 0x78};
        const unsigned char littleNumberBuf[sizeof(int)] = {0x78, 0x56, 0x34, 0x12};

        if (memcmp(littleNumberBuf, (unsigned char*)&tmpNumber, sizeof(int)) == 0)
        {
            bisLittleSystem = TRUE;
        }
        else
        {
            FTLASSERT(memcmp(bigNumberBuf, (unsigned char*)&tmpNumber, sizeof(int)) == 0);
            bisLittleSystem = FALSE;
        }
        return bisLittleSystem;
    }

    int CFSystemUtil::DosLineToUnixLine(const char *src, char *dest, int maxlen)
    {
        int		len = 0;
        maxlen--;	// ȥ��ĩβ��\0

        //���src��û�е������ҳ��ȺϷ�
        while (*src != '\0' && len < maxlen)
            if ((dest[len] = *src++) != '\r')
                len++;

        //�����Ͻ�����\0
        dest[len] = 0;

        return	len;
    }

    //��Unix�Ľ�����ת��ΪDos�Ľ�����(�� "\n" �ĳ� "\r\n" )
    int CFSystemUtil::UnixLineToDosLine(const char *src, char *dest, int maxlen)
    {
        int		len = 0;
        char	*tmpbuf = NULL;

        //�����ʼ����Ŀ�ķ��ĵ�ַ��ͬ�������Ȱ�src������һ����ʱ������tmpbuf��
        if (src == dest)
		{
#pragma warning(push)
#pragma warning(disable : 4996)
            tmpbuf = _strdup(src);
#pragma warning(pop)
			src = tmpbuf;
		}
        maxlen--;	//ȥ�� \0��ռ�õ�λ��

        while (*src != '\0' && len < maxlen)
        {
            if ((dest[len] = *src++) == '\n')
            {
                dest[len++] = '\r';
                if (len < maxlen)
                    dest[len] = '\n';
            }
            len++;
        }
        dest[len] = 0;
        if (tmpbuf)
            free(tmpbuf);

        return	len;
    }

    CFSystemMetricsProperty::CFSystemMetricsProperty()
    {
        m_dwOldGetProperty = 0;
        
        m_cxScreen = GetSystemMetrics(SM_CXSCREEN);				//����ʾ��Ļ���(1280����/1440����)���ȼ��� GetDeviceCaps( hdcPrimaryMonitor, HORZRES).
        m_cyScreen = GetSystemMetrics(SM_CYSCREEN);				//����ʾ��Ļ�߶�(1024����/900����)         
        m_cxVScroll = GetSystemMetrics(SM_CXVSCROLL);			//��ֱ�������Ŀ��(XP -- 16����, Win7 -- 17����)
        m_cyHScroll = GetSystemMetrics(SM_CYHSCROLL);			//ˮƽ�������ĸ߶�(XP -- 16����, Win7 -- 17����)
        m_cyCaption = GetSystemMetrics(SM_CYCAPTION);			//�������ĸ߶�(XP -- 19����, Win7 -- 22 ����)
        m_cxBorder = GetSystemMetrics(SM_CXBORDER);				//����߿�Ŀ��(1����)
        m_cyBorder = GetSystemMetrics(SM_CYBORDER);				//����߿�ĸ߶�(1����)
        m_cxDlgFrame = GetSystemMetrics(SM_CXDLGFRAME);			//�Ի���߿�Ŀ��(3����)
        m_cyDlgFrame = GetSystemMetrics(SM_CYDLGFRAME);			//�Ի���߿�ĸ߶�(3����)
        m_cyVThumb = GetSystemMetrics(SM_CYVTHUMB);				//��ֱ�������� thumb box �ĸ߶�(XP -- 16����, Win7 -- 17����)
        m_cxHThumb = GetSystemMetrics(SM_CXHTHUMB);				//ˮƽ�������� thumb box �Ŀ��(XP -- 16����, Win7 -- 17����)
        m_cxIcon = GetSystemMetrics(SM_CXICON);					//Icon��ȱʡ���(32����)
        m_cyIcon = GetSystemMetrics(SM_CYICON);					//Icon��ȱʡ�߶�(32����)
        m_cxCursor = GetSystemMetrics(SM_CXCURSOR);				//Cursor�Ŀ��(32����)
        m_cyCursor = GetSystemMetrics(SM_CYCURSOR);				//Cursor�ĸ߶�(32����)
        m_cyMenu = GetSystemMetrics(SM_CYMENU);					//�˵��ĸ߶�(XP -- 19����, Win7 -- 20����)
        m_cxFullScreen = GetSystemMetrics(SM_CXFULLSCREEN);		//����ʾ����ȫ��ʱ�Ŀͻ������(1280����)
        m_cyFullScreen = GetSystemMetrics(SM_CYFULLSCREEN);		//����ʾ����ȫ��ʱ�Ŀͻ����߶�(975) -- ���������·�ʱ
        m_cyKanjiWindow = GetSystemMetrics(SM_CYKANJIWINDOW);	//double byte character �汾��Windows����Ļ�׶� Kanji ����ĸ߶�(0����)
        m_MousePresent = GetSystemMetrics(SM_MOUSEPRESENT);		//��0(1)��ʾ�����,0��ʾû�����
        m_cyVscroll = GetSystemMetrics(SM_CYVSCROLL);			//��ֱ�������м�ͷλͼ�ĸ߶�(XP -- 16����, Win7 -- 17����)
        m_cxHscroll = GetSystemMetrics(SM_CXHSCROLL);			//ˮƽ�������м�ͷλͼ�Ŀ��(XP -- 16����, Win7 -- 17����)
        m_Debug = GetSystemMetrics(SM_DEBUG);					//��0(1)��ʾ��װ���ǵ��԰�� User.exe(0)
        m_SwapButton = GetSystemMetrics(SM_SWAPBUTTON);			//��0��ʾ�������������Ҽ�
        m_Reserved1 = GetSystemMetrics(SM_RESERVED1);			//0
        m_Reserved2 = GetSystemMetrics(SM_RESERVED2);			//0
        m_Reserved3 = GetSystemMetrics(SM_RESERVED3);			//0
        m_Reserved4 = GetSystemMetrics(SM_RESERVED4);			//0
        m_cxMin = GetSystemMetrics(SM_CXMIN);					//�������С���(XP -- 112����, Win7 -- 132����)
        m_cyMin = GetSystemMetrics(SM_CYMIN);					//�������С�߶�(XP -- 27����, Win7 -- 38����)
        m_cxSize = GetSystemMetrics(SM_CXSIZE);					//����Caption���������ϰ�ť�Ŀ��(XP -- 18����, Win7 -- 35����)
        m_cySize = GetSystemMetrics(SM_CYSIZE);					//����Caption���������ϰ�ť�ĸ߶�(XP -- 18����, Win7 -- 21����)
        m_cxFrame = GetSystemMetrics(SM_CXFRAME);				//�ɸı��С�Ŀ���ı߽�Ŀ��(XP -- 4����, Win7 -- 8����) -- ���½ǵ��϶����Ŀ��?
        m_cyFrame = GetSystemMetrics(SM_CYFRAME);				//�ɸı��С�Ŀ���ı߽�ĸ߶�(XP -- 4����, Win7 -- 8����) -- ���½ǵ��϶����ĸ߶�?
        m_cxMinTrack = GetSystemMetrics(SM_CXMINTRACK);			//Windows���ϵ��ĵ���С���(XP -- 112����, Win7 -- 132����) -- ��ͨ�� WM_GETMINMAXINFO ������
        m_cyMinTrack = GetSystemMetrics(SM_CYMINTRACK);			//Windows���ϵ��ĵ���С�߶�(XP -- 27����, Win7 -- 38����) -- ��ͨ�� WM_GETMINMAXINFO ������
        m_cxDoubleClk = GetSystemMetrics(SM_CXDOUBLECLK);		//˫����Ч����Ŀ��(4����)
        m_cyDoubleClk = GetSystemMetrics(SM_CYDOUBLECLK);		//˫����Ч����ĸ߶�(4����)
        m_cxIconSpacing = GetSystemMetrics(SM_CXICONSPACING);	//large icon viewʱGrid Cell�Ŀ��(75����)
        m_cyIconSpacing = GetSystemMetrics(SM_CYICONSPACING);	//large icon viewʱGrid Cell�ĸ߶�(75����)
        m_MenuDropAlignment = GetSystemMetrics(SM_MENUDROPALIGNMENT);	//��0��ʾ�����˵������Menu-Bar Item���Ҷ��룬 0��ʾ������� (0)
        m_PenWindows = GetSystemMetrics(SM_PENWINDOWS);			//��0��ʾ��װ�� Pen computing extensions(0)
        m_DBCSEnabled = GetSystemMetrics(SM_DBCSENABLED);		//��0��ʾUser32.dll֧�� DBCS(1), IMM֧��(�����������뷨)
        m_CMouseButtons = GetSystemMetrics(SM_CMOUSEBUTTONS);	//��ʾ����ϰ�ť�ĸ���(3)

#if(WINVER >= 0x0400)
        m_cxFixedFrame = GetSystemMetrics(SM_CXFIXEDFRAME);		//ͬ SM_CXDLGFRAME(3)
        m_cyFixedframe = GetSystemMetrics(SM_CYFIXEDFRAME);		//ͬ SM_CYDLGFRAME(3)
        m_cxSizeFrame = GetSystemMetrics(SM_CXSIZEFRAME);		//ͬ SM_CXFRAME(4)
        m_cySizeFrame = GetSystemMetrics(SM_CYSIZEFRAME);		//ͬ SM_CYFRAME(4)

        m_Secure = GetSystemMetrics(SM_SECURE);					//This system metric should be ignored(ʼ�շ���0)
        m_cxEdge = GetSystemMetrics(SM_CXEDGE);					//3D�߿�Ŀ��(2����)
        m_cyEdge = GetSystemMetrics(SM_CYEDGE);					//3D�߿�ĸ߶�(2����)
        m_cxMinSpacing = GetSystemMetrics(SM_CXMINSPACING);		//��С�������ڱ���ϵĿ��(160����) -- ���ĵ���С����������������С��?
        m_cyMinSpacing = GetSystemMetrics(SM_CYMINSPACING);		//��С�������ڱ���ϵĸ߶�(XP--24����, Win7--27����) -- ���ĵ���С����������������С��?
        m_cxSMIcon = GetSystemMetrics(SM_CXSMICON);				//СIcon�Ľ�����(16����)
        m_cySMIcon = GetSystemMetrics(SM_CYSMICON);				//СIcon�Ľ���߶�(16����)
        m_cySMCaption = GetSystemMetrics(SM_CYSMCAPTION);		//С������(Caption)�ĸ߶�(XP--16����,Win7--20����)
        m_cxSMSize = GetSystemMetrics(SM_CXSMSIZE);				//С�������ϵİ�ť���(XP--12����,Win7--19����)
        m_cySMSize = GetSystemMetrics(SM_CYSMSIZE);				//С�������ϵİ�ť�߶�(XP--15����,Win7--19����)
        m_cxMenuSize = GetSystemMetrics(SM_CXMENUSIZE);			//MenuBar�ϰ�ť(����ĵ��������Ӵ���Ĺرհ�ť)�Ŀ��(18����)
        m_cyMenuSize = GetSystemMetrics(SM_CYMENUSIZE);			//MenuBar�ϰ�ť(����ĵ��������Ӵ���Ĺرհ�ť)�ĸ�(18����)
        m_Arrange = GetSystemMetrics(SM_ARRANGE);				//ָʾϵͳ���������С������( 8 -- ARW_HIDE )
        m_cxMinimized = GetSystemMetrics(SM_CXMINIMIZED);		//��С������Ŀ��(160����)
        m_cyMinimized = GetSystemMetrics(SM_CYMINIMIZED);		//��С������ĸ߶�(XP--24����,Win7--27����)
        m_cxMaxTrack = GetSystemMetrics(SM_CXMAXTRACK);			//�б��⡢�ɸı��С�߿�Ĵ����ȱʡ�����,ͨ���Ǵ����ܴﵽ�����ֵ
																//  (1292���� = SM_CXFULLSCREEN + (SM_CXEDGE + SM_CXFRAME) * 2 )�����ж����Ļ���������� 2736
																//            = 1280            + (     2    +      4    ) * 2 )��
        m_cyMaxTrack = GetSystemMetrics(SM_CYMAXTRACK);			//�б��⡢�ɸı��С�߿�Ĵ����ȱʡ���߶�,ͨ���Ǵ����ܴﵽ�����ֵ(1036 = 1024 + (2+4)*2 )
        m_cxMaximized = GetSystemMetrics(SM_CXMAXIMIZED);		//����ʾ���ϣ���󻯵Ķ��㴰��Ŀ��(1288 = 1280 + 4 * 2)
        m_cyMaximized = GetSystemMetrics(SM_CYMAXIMIZED);		//����ʾ���ϣ���󻯵Ķ��㴰��ĸ߶�(1002 = )
        m_Network = GetSystemMetrics(SM_NETWORK);				//���������Ƿ���ڵ�����λ( 0x03 )
        m_CleanBoot = GetSystemMetrics(SM_CLEANBOOT);			//����ϵͳ�������(0 -- ��������)
        m_cxDrag = GetSystemMetrics(SM_CXDRAG);					//��ʼ��קʱ����С���(4����)
        m_cyDrag = GetSystemMetrics(SM_CYDRAG);					//��ʼ��קʱ����С�߶�(4����)
#endif /* WINVER >= 0x0400 */

        m_ShowSounds = GetSystemMetrics(SM_SHOWSOUNDS);//           70
#if(WINVER >= 0x0400)
        m_cxMenuCheck = GetSystemMetrics(SM_CXMENUCHECK);		//ȱʡ�Ĳ˵�ѡ��λͼ�Ŀ��(13����) -- Use instead of GetMenuCheckMarkDimensions()
        m_cyMenuCheck = GetSystemMetrics(SM_CYMENUCHECK);		//ȱʡ�Ĳ˵�ѡ��λͼ�ĸ߶�(13����)
        m_SlowMachine = GetSystemMetrics(SM_SLOWMACHINE);		//��0��ʾϵͳ��һ�����ٵĴ�����(0)
        m_MideaStenabled = GetSystemMetrics(SM_MIDEASTENABLED);	//��0��ʾ�� Hebrew and Arabic languages 
#endif /* WINVER >= 0x0400 */

#if (WINVER >= 0x0500) || (_WIN32_WINNT >= 0x0400)
        m_MouseWheelPresent = GetSystemMetrics(SM_MOUSEWHEELPRESENT);	//��0��ʾ��������(1)
#endif

#if(WINVER >= 0x0500)
        m_XVirtualScreen = GetSystemMetrics(SM_XVIRTUALSCREEN);		//������Ļ����߽�(0)
        m_YVirtualScreen = GetSystemMetrics(SM_YVIRTUALSCREEN);		//������Ļ�Ķ��߽�(0)
        m_cxVirtualScreen = GetSystemMetrics(SM_CXVIRTUALSCREEN);	//������Ļ�Ŀ��(1280���أ��ж����Ļʱ������ 2720 = 1280 + 1440)
        m_cyVirtualScreen = GetSystemMetrics(SM_CYVIRTUALSCREEN);	//������Ļ�ĸ߶�(1024���أ��ж����Ļ����ֱ����ʱʱ���ܸ���)
        m_CMonitors = GetSystemMetrics(SM_CMONITORS);				//����������ʾ����ʾ���ĸ���(1��2����Ļʱ2)
        m_SameDisplayFormat = GetSystemMetrics(SM_SAMEDISPLAYFORMAT);	//��0��ʾ���е���ʾ������ͬ����ɫ��ʽ(Win7--1)
#endif /* WINVER >= 0x0500 */

#if(_WIN32_WINNT >= 0x0500)
        m_IMMEnabled = GetSystemMetrics(SM_IMMENABLED);			//��0��ʾ Input Method Manager/Input Method Editor ��Ч(Win7--1)
#endif /* _WIN32_WINNT >= 0x0500 */
#if(_WIN32_WINNT >= 0x0501)
        m_cxFocusBorder = GetSystemMetrics(SM_CXFOCUSBORDER);	//DrawFocusRect ����Focus����ʱ�����ұ߽�Ŀ��(1����)
        m_cyFocusBorder = GetSystemMetrics(SM_CYFOCUSBORDER);	//DrawFocusRect ����Focus����ʱ���ϡ��±߽�ĸ߶�(1����)
#endif /* _WIN32_WINNT >= 0x0501 */

#if(_WIN32_WINNT >= 0x0501)
        m_TabletPc = GetSystemMetrics(SM_TABLETPC);				//(0)
        m_MediaCenter = GetSystemMetrics(SM_MEDIACENTER);		//(XP--0, Win7--1)
        m_Starter = GetSystemMetrics(SM_STARTER);				//(0)
        m_Serverr2 = GetSystemMetrics(SM_SERVERR2);				//(0)
#endif /* _WIN32_WINNT >= 0x0501 */

#if(_WIN32_WINNT >= 0x0600)
        m_MouseHorizontalWheelPresent = GetSystemMetrics(SM_MOUSEHORIZONTALWHEELPRESENT);	//(?)
        m_cxPaddedBorder = GetSystemMetrics(SM_CXPADDEDBORDER);	//(?)
#endif /* _WIN32_WINNT >= 0x0600 */

        m_CMetrics = GetSystemMetrics(SM_CMETRICS);				//(0)

#if(WINVER >= 0x0500)
        m_RemoteSession = GetSystemMetrics(SM_REMOTESESSION);	//�Ƿ���ͨ�� Terminal Services(Զ���ն�)��������
#  if(_WIN32_WINNT >= 0x0501)
        m_ShuttingDown = GetSystemMetrics(SM_SHUTTINGDOWN);		//��ǰSession�Ƿ��� Shutting Down������
#  endif /* _WIN32_WINNT >= 0x0501 */
#  if(WINVER >= 0x0501)
        m_RemoteControl = GetSystemMetrics(SM_REMOTECONTROL);	//�� SM_REMOTESESSION ������?
#  endif /* WINVER >= 0x0501 */
#  if(WINVER >= 0x0501)
        m_CaretBlinkingEnabled = GetSystemMetrics(SM_CARETBLINKINGENABLED);// 0x2002
#  endif /* WINVER >= 0x0501 */
#endif /* WINVER >= 0x0500 */

    }

    CFSystemMetricsProperty::~CFSystemMetricsProperty()
    {
        
    }

	LPCTSTR CFSystemMetricsProperty::GetPropertyString(DWORD dwPropertyGet /* = SYSTEM_METRICS_PROPERTY_GET_DEFAULT */)
	{
		UNREFERENCED_PARAMETER(dwPropertyGet);
		m_strFormater.Format(TEXT("%d"), 100);
		return m_strFormater.GetString();
	}

    template <typename T, typename F> // = FTL::ObjecteDeleter<T*> 
    BOOL CFPluginMgrT<T,F>::Init(LPCTSTR pszPluginPath, LPCSTR pszProcName, LPCTSTR pszExtName /*= TEXT("*.dll")*/)
    {
        CHECK_POINTER_ISSTRING_PTR_RETURN_VALUE_IF_FAIL(pszPluginPath, FALSE);
        //CHECK_POINTER_ISSTRING_PTR_RETURN_VALUE_IF_FAIL(pszProcName, FALSE);
        CHECK_POINTER_ISSTRING_PTR_RETURN_VALUE_IF_FAIL(pszExtName, FALSE);

        BOOL bRet = FALSE;
        CFStringFormater strFormater;
        strFormater.Format(TEXT("%s\\%s"), pszPluginPath, pszExtName);

        WIN32_FIND_DATA findData = {0};
        HANDLE hFind = INVALID_HANDLE_VALUE;
        API_VERIFY( INVALID_HANDLE_VALUE != (hFind = ::FindFirstFile(strFormater, &findData)));
        if (INVALID_HANDLE_VALUE != hFind )
        {
            do 
            {
                //TODO: isDot
                CFStringFormater strFullPath;
                strFullPath.Format(TEXT("%s\\%s"), pszPluginPath, findData.cFileName);
                LoadPlugin( strFullPath, pszProcName);
            } 
            while (::FindNextFile(hFind, &findData));

            API_VERIFY(::FindClose(hFind));
            hFind = INVALID_HANDLE_VALUE;
        }
        return bRet;
    }

    template <typename T, typename F > // = FTL::ObjecteDeleter<T*>
    void CFPluginMgrT<T,F>::UnInit()
    {
        for_each(m_plugins.begin(), m_plugins.end(), F() );
        for_each(m_modules.begin(), m_modules.end(), FreeLibrary );

        m_plugins.clear();
        m_modules.clear();
    }

    template <typename T, typename F> // = FTL::ObjecteDeleter<T*> 
    BOOL CFPluginMgrT<T,F>::LoadPlugin(LPCTSTR pszePlugPath, LPCSTR pszProcName)
    {
        BOOL bRet = FALSE;
        HMODULE hModule = NULL;
        T* pInstance = NULL;
        API_VERIFY(NULL != (hModule = ::LoadLibrary(pszePlugPath)));
        if (NULL != hModule)
        {
            FunGetInstance pFun = (FunGetInstance)::GetProcAddress(hModule, pszProcName);
            if (pFun)
            {
                pInstance = (*pFun)();
                if (pInstance)
                {
                    m_plugins.push_back(pInstance);
                    m_modules.push_back(hModule);
                    bRet = TRUE;
                }
            }
        }
        if (!pInstance)
        {
            ::FreeModule(hModule);
            bRet = FALSE;
        }
        FTLTRACE(TEXT("Load Plugin Path=%s, Result=%d\n"), pszePlugPath, bRet);
        return bRet;
    }

    template <typename T, typename F> // = FTL::ObjecteDeleter<T*> 
    CFPluginMgrT<T,F>::~CFPluginMgrT()
    {
        UnInit();
    }
}

#endif //FTL_SYSTEM_HPP