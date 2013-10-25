///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file   ftlSystem.hpp
/// @brief  Fishjam Template Library System Implemention File.
/// @author fujie
/// @version 0.6 
/// @date 03/30/2008
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef FTL_SYSTEM_HPP
#define FTL_SYSTEM_HPP
#pragma once

#ifdef USE_EXPORT
#  include "ftlsystem.h"
#endif
#include "ftlConversion.h"

#include <ShlObj.h>
#include <atlbase.h>
#ifdef _DEBUG
#  include <winternl.h>
#endif

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
		case 20602:
			osType = ostWindows8;
			break;
        default:
            {
                _ASSERT(FALSE);
                if (dwOsVersion > 20602)
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
		DWORD dwProcessCount1 = 0; 
		DWORD dwProcessCount2 = 0; 

		//��һ�ַ��� -- ֻ�ܵõ�����CPU?
        SYSTEM_INFO sysinfo = {0};
        ::GetSystemInfo(&sysinfo);
		dwProcessCount1 = sysinfo.dwNumberOfProcessors;
		
		//�ڶ��ַ��� -- ���Եõ����߳� ?
		BOOL bRet = FALSE;
		DWORD_PTR dwProcessAffinityMask = 0, dwSystemAffinityMask = 0;
		API_VERIFY(GetProcessAffinityMask(GetCurrentProcess(), &dwProcessAffinityMask, &dwSystemAffinityMask));
		if (bRet)
		{
			for (; dwProcessAffinityMask; dwProcessAffinityMask >>= 1)
			{
				if (dwProcessAffinityMask & 0x1) 
				{
					dwProcessCount2++;
				}
			}
		}

		API_ASSERT(dwProcessCount1 == dwProcessCount2);
        return dwProcessCount2;
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

    LONG CFRegUtil::GetRegValueExportString(HKEY hKey, LPCTSTR pszValueName, CAtlString& strResult, DWORD* pRegType)
    {
        LONG lRet = ERROR_SUCCESS;
        BOOL bRet= FALSE;
        DWORD dwRegType = REG_NONE;
        DWORD nBytesCount = 0;
        REG_VERIFY(::RegQueryValueEx(hKey, pszValueName, NULL, &dwRegType, NULL, &nBytesCount));
        if (ERROR_SUCCESS == lRet)
        {
            CFMemAllocator<BYTE> memValue(nBytesCount);
            REG_VERIFY(::RegQueryValueEx(hKey, pszValueName, NULL, &dwRegType, memValue, &nBytesCount));
            if (ERROR_SUCCESS == lRet)
            {
                if (pRegType)
                {
                    *pRegType = dwRegType;
                }
                strResult.Empty();
                CAtlString strBinary;
                LONG nStrBinaryCount = 0;

                switch (dwRegType)
                {
                case REG_SZ:
                    {
                        CAtlString strTemp;
                        strTemp.Format(TEXT("%s"), (LPCTSTR)memValue.GetMemory());
                        strTemp.Replace(TEXT("\\"), TEXT("\\\\"));  // \ => \\  
                        strTemp.Replace(TEXT("\""), TEXT("\\\""));  // " => \" 
                        strResult.Format(TEXT("\"%s\""), strTemp);
                    }
                    break;
                case REG_EXPAND_SZ:
                case REG_BINARY:
                case REG_MULTI_SZ:
                case REG_QWORD_LITTLE_ENDIAN:  //REG_QWORD
                    {
                        API_VERIFY(CFConvUtil::HexFromBinary(memValue.GetMemory(), nBytesCount, NULL, &nStrBinaryCount, _T(',')));
                        CFMemAllocator<TCHAR> bufString(nStrBinaryCount);
                        API_VERIFY(CFConvUtil::HexFromBinary(memValue.GetMemory(), nBytesCount, bufString.GetMemory(), &nStrBinaryCount, _T(',')));
                        
                        if (REG_BINARY == dwRegType)
                        {
                            strResult = _T("hex:");
                        }
                        else
                        {
                            strResult.Format(TEXT("hex(%x):"), dwRegType);
                        }
                        strResult.Append(bufString.GetMemory());

                        break;
                    }
                case REG_DWORD_LITTLE_ENDIAN: //REG_DWORD:
                    {
                        strResult.Format(TEXT("dword:%08x"), *((DWORD*)memValue.GetMemory()));
                        break;
                    }
                case REG_DWORD_BIG_ENDIAN:
                case REG_LINK:
                case REG_RESOURCE_LIST:
                case REG_FULL_RESOURCE_DESCRIPTOR:
                case REG_RESOURCE_REQUIREMENTS_LIST:
                default:
                    FTLTRACEEX(tlWarning, TEXT("%s Unsupport RegType\n"), __FILE__LINE__);
                    FTLASSERT(FALSE);
                    lRet = ERROR_UNSUPPORTED_TYPE;
                    break;
                }
            }
        }
        return lRet;
    }
    

	///////////////////////////////////////// SystemParamProperty ///////////////////////////////////////////////
	SystemParamProperty::SystemParamProperty()
	{
		//Init();
		GetParametersInfo();
	}
	SystemParamProperty::~SystemParamProperty()
	{

	}
	BOOL SystemParamProperty::GetParametersInfo()
	{
		BOOL bRet = FALSE;

		API_VERIFY(::SystemParametersInfo(SPI_GETBEEP, 0, &m_bBeep, 0));			//����������Ƿ��Ǵ�
		API_VERIFY(::SystemParametersInfo(SPI_GETMOUSE, 0, &m_MouseInfo[0], 0));	//����2����ֵ�ͼ�������
		API_VERIFY(::SystemParametersInfo(SPI_GETBORDER, 0, &m_nBorder, 0));		//���ڱ߽�Ŵ��ȵı߽�Ŵ�����
		API_VERIFY(::SystemParametersInfo(SPI_GETKEYBOARDSPEED, 0, &m_dwKeyboardSpeed, 0));	//�����ظ������ٶ��������,0(Լ30��/��)~31(Լ25��/��)
		API_VERIFY(::SystemParametersInfo(SPI_LANGDRIVER, 0, &m_dwUnknown, 0));		//δʵ��
		API_VERIFY(::SystemParametersInfo(SPI_ICONHORIZONTALSPACING, 0, &m_nIconHorizontalSpacing, 0));	//��LargeIconView��ʱ��Icon�Ŀ��,���ñ������SM_CXICON
		API_VERIFY(::SystemParametersInfo(SPI_ICONVERTICALSPACING, 0, &m_nIconVerticalSpacing, 0));		//��LargeIconView��ʱ��Icon�ĸ߶�,���ñ������SM_CXICON
		API_VERIFY(::SystemParametersInfo(SPI_GETSCREENSAVETIMEOUT, 0, &m_nScreenSaveTimeout, 0));		//������ʱ��ʱ�䣬��λΪ��
#if 0
		API_VERIFY(::SystemParametersInfo(SPI_GETSCREENSAVEACTIVE, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETGRIDGRANULARITY, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETKEYBOARDDELAY, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETICONTITLEWRAP, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETMENUDROPALIGNMENT, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETICONTITLELOGFONT, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETFASTTASKSWITCH, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETDRAGFULLWINDOWS, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETNONCLIENTMETRICS, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETMINIMIZEDMETRICS, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETICONMETRICS, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETWORKAREA, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETHIGHCONTRAST, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETKEYBOARDPREF, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETSCREENREADER, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETANIMATION, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETFONTSMOOTHING, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETLOWPOWERTIMEOUT, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETPOWEROFFTIMEOUT, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETLOWPOWERACTIVE, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETPOWEROFFACTIVE, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETDEFAULTINPUTLANG, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETWINDOWSEXTENSION, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETMOUSETRAILS, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETFILTERKEYS, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETTOGGLEKEYS, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETMOUSEKEYS, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETSHOWSOUNDS, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETSTICKYKEYS, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETACCESSTIMEOUT, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETSERIALKEYS, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETSOUNDSENTRY, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETSNAPTODEFBUTTON, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETMOUSEHOVERWIDTH, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETMOUSEHOVERHEIGHT, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETMOUSEHOVERTIME, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETWHEELSCROLLLINES, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETMENUSHOWDELAY, 0, &XXXXX, 0));
#if (_WIN32_WINNT >= 0x0600)
		API_VERIFY(::SystemParametersInfo(SPI_GETWHEELSCROLLCHARS, 0, &XXXXX, 0));
#endif //_WIN32_WINNT >= 0x0600

		API_VERIFY(::SystemParametersInfo(SPI_GETSHOWIMEUI, 0, &XXXXX, 0));
#if(WINVER >= 0x0500)
		API_VERIFY(::SystemParametersInfo(SPI_GETMOUSESPEED, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETSCREENSAVERRUNNING, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETDESKWALLPAPER, 0, &XXXXX, 0));
#endif /* WINVER >= 0x0500 */

#if(WINVER >= 0x0600)
		API_VERIFY(::SystemParametersInfo(SPI_GETAUDIODESCRIPTION, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETSCREENSAVESECURE, 0, &XXXXX, 0));
#endif /* WINVER >= 0x0600 */

#if(_WIN32_WINNT >= 0x0601)
		API_VERIFY(::SystemParametersInfo(SPI_GETHUNGAPPTIMEOUT, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETWAITTOKILLTIMEOUT, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETWAITTOKILLSERVICETIMEOUT, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETMOUSEDOCKTHRESHOLD, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETPENDOCKTHRESHOLD, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETWINARRANGING, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETMOUSEDRAGOUTTHRESHOLD, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETPENDRAGOUTTHRESHOLD, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETMOUSESIDEMOVETHRESHOLD, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETPENSIDEMOVETHRESHOLD, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETDRAGFROMMAXIMIZE, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETSNAPSIZING, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETDOCKMOVING, 0, &XXXXX, 0));
#endif /* _WIN32_WINNT >= 0x0601 */

#if(WINVER >= 0x0500)
		API_VERIFY(::SystemParametersInfo(SPI_GETACTIVEWINDOWTRACKING, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETMENUANIMATION, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETCOMBOBOXANIMATION, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETLISTBOXSMOOTHSCROLLING, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETGRADIENTCAPTIONS, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETKEYBOARDCUES, 0, &m_bKeyboardCues, 0)); //��ݼ��ַ����Ƿ�һֱ��ʾ�»��ߣ���ΪFALSE����Ҫ����"Alt"�����ʾ
		API_VERIFY(::SystemParametersInfo(SPI_GETMENUUNDERLINES, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETACTIVEWNDTRKZORDER, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETHOTTRACKING, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETMENUFADE, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETSELECTIONFADE, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETTOOLTIPANIMATION, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETTOOLTIPFADE, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETCURSORSHADOW, 0, &XXXXX, 0));
#if(_WIN32_WINNT >= 0x0501)
		API_VERIFY(::SystemParametersInfo(SPI_GETMOUSESONAR, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETMOUSECLICKLOCK, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETMOUSEVANISH, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETFLATMENU, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETDROPSHADOW, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETBLOCKSENDINPUTRESETS, 0, &XXXXX, 0));
#endif /* _WIN32_WINNT >= 0x0501 */
		API_VERIFY(::SystemParametersInfo(SPI_GETUIEFFECTS, 0, &XXXXX, 0));
#if(_WIN32_WINNT >= 0x0600)
		API_VERIFY(::SystemParametersInfo(SPI_GETDISABLEOVERLAPPEDCONTENT, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETCLIENTAREAANIMATION, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETCLEARTYPE, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETSPEECHRECOGNITION, 0, &XXXXX, 0));
#endif /* _WIN32_WINNT >= 0x0600 */
		API_VERIFY(::SystemParametersInfo(SPI_GETFOREGROUNDLOCKTIMEOUT, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETACTIVEWNDTRKTIMEOUT, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETFOREGROUNDFLASHCOUNT, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETCARETWIDTH, 0, &XXXXX, 0));
#if(_WIN32_WINNT >= 0x0501)
		API_VERIFY(::SystemParametersInfo(SPI_GETMOUSECLICKLOCKTIME, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETFONTSMOOTHINGTYPE, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETFONTSMOOTHINGCONTRAST, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETFOCUSBORDERWIDTH, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETFOCUSBORDERHEIGHT, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETFONTSMOOTHINGORIENTATION, 0, &XXXXX, 0));
#endif /* _WIN32_WINNT >= 0x0501 */

#if(_WIN32_WINNT >= 0x0600)
		API_VERIFY(::SystemParametersInfo(SPI_GETMINIMUMHITRADIUS, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETMESSAGEDURATION, 0, &XXXXX, 0));
#endif /* _WIN32_WINNT >= 0x0600 */

#endif /* WINVER >= 0x0500 */
		//API_VERIFY(::SystemParametersInfo(SPI_XXX, 0, &XXXXX, 0));

		//API_VERIFY(::SystemParametersInfo(SPI_GETKEYBOARDSPEED, 0, &m_nKeyboardSpeed, 0));
		//API_VERIFY(::SystemParametersInfo(SPI_GETWORKAREA, NULL, &rcArea ,NULL));
#endif 

		return bRet;
	}
	///////////////////////////////////////// SystemParamProperty ///////////////////////////////////////////////

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
        ATL::CRegKey RegUsers;
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
			HANDLE_CASE_TO_STRING(lpszFormatName, cchMaxCount, CF_HDROP); //DragQueryFile��ȡ
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

    BOOL CFSystemUtil::EnableProcessPrivilege(HANDLE hProcess, LPCTSTR lpPrivilegeName /* = SE_DEBUG_NAME */,BOOL bEnabled /*= TRUE*/)
    {
        BOOL bRet = FALSE;
        TOKEN_PRIVILEGES TokenPrivileges = { 0 };
        API_VERIFY(LookupPrivilegeValue (NULL, lpPrivilegeName, &TokenPrivileges.Privileges[0].Luid));
        if (bRet)
        {
            HANDLE hToken = NULL;
            API_VERIFY(OpenProcessToken(hProcess, TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken));
            if (bRet)
            {
                TokenPrivileges.PrivilegeCount = 1;
                TokenPrivileges.Privileges[0].Attributes = bEnabled ? SE_PRIVILEGE_ENABLED : 0; //SE_PRIVILEGE_REMOVED
                API_VERIFY(AdjustTokenPrivileges (hToken,FALSE,&TokenPrivileges,
                    sizeof (TokenPrivileges),NULL,NULL));
                bRet = (GetLastError () == ERROR_SUCCESS);
                CloseHandle(hToken);
            }
        }
        return bRet;
    }


	BOOL CFSystemUtil::IsSpecialProcessName(LPCTSTR pszProcessName, HMODULE hModule /* = NULL */)
	{
		BOOL bRet = FALSE;
		TCHAR szModuleFilePath[MAX_PATH] = {0};
		API_VERIFY(::GetModuleFileName(hModule, szModuleFilePath, _countof(szModuleFilePath)) > 0);

		//�����ȣ�����ָ�����ֵĽ���
		if(lstrcmpi(pszProcessName, PathFindFileName(szModuleFilePath)) == 0)
		{
			bRet = TRUE;
		}
		else
		{
			bRet = FALSE;
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
        BOOL bRet = TRUE ;
		API_VERIFY(::OpenClipboard ( hWndOwner ))
        if ( bRet )
        {
            API_VERIFY(::EmptyClipboard( ));
			if(bRet)
            {
				const int nLen = _tcslen (szMem);

				//TODO:GlobalAlloc ������ڴ治�ͷ�?����˵ͨ���´ε� EmptyClipboard �ͷ�?
                HGLOBAL hGlob = ::GlobalAlloc ( GHND, ( nLen + 1) * sizeof ( TCHAR ));
                if ( NULL != hGlob )
                {
                    TCHAR * szClipMem = (TCHAR*)GlobalLock ( hGlob ) ;
                    FTLASSERT ( NULL != szMem ) ;
                    StringCchCopy(szClipMem , nLen, szMem);
                    GlobalUnlock ( hGlob ) ;
                    API_VERIFY(NULL != ::SetClipboardData (CLIPBOARDFMT, hGlob));

					//TODO:����ͬʱ֧�� CF_UNICODETEXT��CF_TEXT
                }
                else
                {
                    bRet = FALSE ;
                }
            }
			CloseClipboard();
        }
        return ( bRet ) ;
    }

	ULONGLONG CFSystemUtil::GetTickCount64()
	{
		typedef ULONGLONG (WINAPI* GetTickCount64Proc)(void);
		typedef ULONG (WINAPI* NtQuerySystemInformationProc)(int /*SYSTEM_INFORMATION_CLASS SystemInformationClass*/,
			PVOID /*SystemInformation*/, 
			ULONG /*SystemInformationLength*/, 
			PULONG /*ReturnLength*/);

		GetTickCount64Proc pVistaGetTickCount64 = (GetTickCount64Proc)
			GetProcAddress(GetModuleHandle(_T("kernel32.dll")), "GetTickCount64");
		if (pVistaGetTickCount64)
		{
			return pVistaGetTickCount64();
		}
		else
		{
			typedef struct _SYSTEM_TIME_OF_DAY_INFORMATION
			{
				LARGE_INTEGER BootTime;
				LARGE_INTEGER CurrentTime;
				LARGE_INTEGER TimeZoneBias;
				ULONG CurrentTimeZoneId;
			} SYSTEM_TIME_OF_DAY_INFORMATION, *PSYSTEM_TIME_OF_DAY_INFORMATION;

			NtQuerySystemInformationProc pNtQuerySystemInformation = (NtQuerySystemInformationProc)
				GetProcAddress(GetModuleHandle(_T("ntdll.dll")), ("NtQuerySystemInformation"));

			SYSTEM_TIME_OF_DAY_INFORMATION  st ={0};
			ULONG                           oSize = 0;
			if((NULL == pNtQuerySystemInformation)
				|| 0 != (pNtQuerySystemInformation(3, &st, sizeof(st), &oSize))
				||(oSize!= sizeof(st)))
			{
				return GetTickCount();
			}
			return (st.CurrentTime.QuadPart - st.BootTime.QuadPart)/10000;
		}
	}


	DWORD CFSystemUtil::GetPidFromProcessName(LPCTSTR pszProcesName)
	{
		DWORD dwPidResult = 0;

		BOOL bRet = FALSE;
		HANDLE hSnapProcHandle = NULL;
		API_VERIFY((hSnapProcHandle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0)) != INVALID_HANDLE_VALUE);
		if (bRet)
		{
			PROCESSENTRY32 ProcEntry = { 0 };
			ProcEntry.dwSize = sizeof(PROCESSENTRY32);
			API_VERIFY(Process32First(hSnapProcHandle, &ProcEntry));
			while (bRet)
			{
				LPCTSTR pszFileName = PathFindFileName(ProcEntry.szExeFile);
				if (pszFileName)
				{
					if(lstrcmpi(pszProcesName, pszFileName) == 0)
					{
						dwPidResult = ProcEntry.th32ProcessID;
						break;
					}
				}
				API_VERIFY(Process32Next(hSnapProcHandle, &ProcEntry));
			}
			CloseHandle(hSnapProcHandle);
		}
		//OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, False, PID);

		return dwPidResult;
	}

    typedef struct  
    {  
        DWORD       ExitStatus;                     //PVOID Reserved1
        DWORD       PebBaseAddress;                 //PPEB PebBaseAddress;
        DWORD       AffinityMask;                   //PVOID Reserved2[0]
        DWORD       BasePriority;                   //PVOID Reserved2[1];
        ULONG_PTR   UniqueProcessId;                //ULONG_PTR UniqueProcessId;
        ULONG       InheritedFromUniqueProcessId;   //Reserved3
    }   PROCESS_BASIC_INFORMATION_FOR_PPID;  

#ifdef _DEBUG
    //C_ASSERT(sizeof(PROCESS_BASIC_INFORMATION_FOR_PPID) == sizeof(PROCESS_BASIC_INFORMATION));
#endif 

    typedef LONG (WINAPI *NtQueryInformationProcessProc)(HANDLE, UINT, PVOID, ULONG, PULONG);  
    DWORD CFSystemUtil::GetParentProcessId(DWORD dwPID, BOOL bCheckParentExist /* = TRUE*/)
    {
        DWORD   dwParentPID = (DWORD)-1;  
        BOOL    bRet = FALSE;
        LONG    nStatus = 0;
        HANDLE  hProcess = NULL;
        PROCESS_BASIC_INFORMATION_FOR_PPID pbi = {0};  

        HMODULE hModuleNtDll = GetModuleHandle(TEXT("ntdll"));
        NtQueryInformationProcessProc pNtQueryInformationProcess = 
            (NtQueryInformationProcessProc)GetProcAddress( hModuleNtDll, "NtQueryInformationProcess");  
        if (pNtQueryInformationProcess)
        {
            // Get process handle  
            hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, dwPID);  
            if (hProcess)
            {
                nStatus = (pNtQueryInformationProcess)(hProcess,  
                    0, //ProcessBasicInformation,  
                    (PVOID)&pbi,  
                    sizeof(PROCESS_BASIC_INFORMATION_FOR_PPID),  
                    NULL  
                    );
                FTLASSERT(0 == nStatus);    //STATUS_SUCCESS
                if (0 == nStatus)
                {
                    if (bCheckParentExist)
                    {
                        HANDLE hParentProcess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, pbi.InheritedFromUniqueProcessId); 
                        if (hParentProcess)
                        {
                            dwParentPID = pbi.InheritedFromUniqueProcessId;
                            CloseHandle(hParentProcess);
                        }
                    }
                    else
                    {
                        dwParentPID = pbi.InheritedFromUniqueProcessId;
                    }
                    
                }
                else
                {
                    FTLTRACEEX(FTL::tlError, TEXT("%s, CFSystemUtil::GetParentProcessId, Error Code=0x%x\n"),
                        __FILE__LINE__,  nStatus);
                }
                CloseHandle (hProcess);  
            }
        }
        return dwParentPID;  
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

#if defined(_M_IX86)
	// IsInsideVPC's exception filter  
	DWORD __forceinline IsInsideVPC_exceptionFilter(LPEXCEPTION_POINTERS ep)  
	{  
    	PCONTEXT ctx = ep->ContextRecord;  

    	ctx->Ebx = (DWORD)(-1); // Not running VPC  
    	ctx->Eip += 4; // skip past the "call VPC" opcodes  
    	return (DWORD)EXCEPTION_CONTINUE_EXECUTION; // we can safely resume execution since we skipped faulty instruction  
	}  

	BOOL CFSystemUtil::IsInsideVPC()
	{
	    bool rc = false;  

	    __try  
	    {  
	        _asm push ebx  
	        _asm mov  ebx, 0 // Flag  
	        _asm mov  eax, 1 // VPC function number  

	        // call VPC   
	        _asm __emit 0Fh  
	        _asm __emit 3Fh  
	        _asm __emit 07h  
	        _asm __emit 0Bh  

	        _asm test ebx, ebx  
	        _asm setz [rc]  
	        _asm pop ebx  
	    }  
	    // The except block shouldn't get triggered if VPC is running!!  
	    __except(IsInsideVPC_exceptionFilter(GetExceptionInformation()))  
	    {  
	    }  

	    return rc;  
	}

	FTLINLINE bool _IsInsideVMWare()
	{
		bool r;
		_asm
		{
			push   edx
			push   ecx
			push   ebx

			mov    eax, 'VMXh'
			mov    ebx, 0 // any value but MAGIC VALUE
			mov    ecx, 10 // get VMWare version
			mov    edx, 'VX' // port number
			in     eax, dx // read port
			// on return EAX returns the VERSION
			cmp    ebx, 'VMXh' // is it a reply from VMWare?
			setz   [r] // set return value

			pop    ebx
			pop    ecx
			pop    edx
		}
		return r;
	}

	BOOL CFSystemUtil::IsInsideVMWare()
	{
		BOOL bRet = FALSE;
		__try
		{
			bRet = !!_IsInsideVMWare();
		}
		__except(1) // 1 = EXCEPTION_EXECUTE_HANDLER
		{
			
		}
		return bRet;
	}
#endif 

    BOOL CFSystemUtil::IsRunningOnRemoteDesktop()
    {
        BOOL bRet = FALSE;
        BOOL bRunningOnRemoteDesktop = FALSE;
        DWORD dwSessionId = 0;
        API_VERIFY(ProcessIdToSessionId(GetCurrentProcessId(), &dwSessionId));
        if (bRet)
        {
            if (WTSGetActiveConsoleSessionId() != dwSessionId)
            {
                bRunningOnRemoteDesktop = TRUE;
            }
        }
        return bRunningOnRemoteDesktop;
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
        HANDLE hFind = ::FindFirstFile(strFormater, &findData);
        API_VERIFY_EXCEPT1( (INVALID_HANDLE_VALUE != (hFind)), ERROR_PATH_NOT_FOUND);
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