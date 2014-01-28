///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file   ftlbase.hpp
/// @brief  Fishjam Template Library Base Implemention File.
/// @author fujie
/// @version 0.6 
/// @date 03/30/2008
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef FTL_BASE_HPP
#define FTL_BASE_HPP
#pragma once

#ifdef USE_EXPORT
#  include "ftlbase.h"
#endif

#include "shlwapi.h"
#pragma comment( lib, "shlwapi.lib" )

namespace FTL
{
    template<typename T>
    BOOL IsSameNumber(const T& expected, const T& actual, const T& delta)
    {
        if (FTL_ABS(expected - actual) <= delta)
        {
            return TRUE;
        }
        return FALSE;
    }

	template<typename T>
	void SwapValue(T& value1, T& value2)
	{
		T temp =  value1;
		value1 = value2;
		value2 = temp;
	}

    CFLastErrorRecovery::CFLastErrorRecovery()
    {
        m_dwLastError = GetLastError();
    }
    CFLastErrorRecovery::~CFLastErrorRecovery()
    {
        SetLastError(m_dwLastError);
    }

	template<typename T>
	CFSharedVariableT<T>::CFSharedVariableT(
		InitializeSharedVariableProc pInitializeProc /* = NULL */, 
		FinalizeSharedVariableProc pFinalizeProc /* = NULL */,
		LPCTSTR pszShareName /* = NULL */)
	{
		DWORD dwShareInfoSize = sizeof(T);

		m_pShareValue = NULL;
		m_hMapping = NULL;
		m_bFirstCreate = FALSE;
		m_pFinalizeProc = pFinalizeProc;

		TCHAR szMapName[MAX_PATH] = {0};
		if (NULL == pszShareName)
		{
			TCHAR szModuleFileName[MAX_PATH] = {0};
			GetModuleFileName(NULL, szModuleFileName, _countof(szModuleFileName));
			PathRemoveExtension(szModuleFileName);
			LPCTSTR pszNamePos = PathFindFileName(szModuleFileName); // &szModuleFileName[nLength + 1];
			StringCchPrintf(szMapName, _countof(szMapName), TEXT("FTLShare_%s_%d"), pszNamePos, GetCurrentProcessId());
			pszShareName =  szMapName;
		}

		m_hMapping = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE,
			0, dwShareInfoSize, pszShareName);
		FTLASSERT(m_hMapping != NULL);
		if (m_hMapping)
		{
			m_bFirstCreate = (GetLastError() == 0); //Not ERROR_ALREADY_EXISTS
			m_pShareValue = reinterpret_cast<T*>
				(::MapViewOfFileEx(m_hMapping, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, dwShareInfoSize, NULL));
		}
		FTLASSERT(m_pShareValue);

		if (m_bFirstCreate && pInitializeProc)
		{
			if (m_pShareValue)
			{
				pInitializeProc(*m_pShareValue);
			}
		}
	}

	template<typename T>
	CFSharedVariableT<T>::~CFSharedVariableT()
	{
		if (m_pShareValue)
		{
			if (m_bFirstCreate && m_pFinalizeProc)
			{
				m_pFinalizeProc(*m_pShareValue);
			}

			UnmapViewOfFile(m_pShareValue);
			m_pShareValue = NULL;
		}
		if (m_hMapping)
		{
			CloseHandle(m_hMapping);
			m_hMapping = NULL;
		}
	}

	template<typename T>
	T& CFSharedVariableT<T>::GetShareValue()
	{
		FTLASSERT(m_pShareValue);
		return *m_pShareValue;
	}

	BOOL CALLBACK _FtlGlobalShareInfoInitialize(FTLGlobalShareInfo& rShareInfo)
	{
		BOOL bRet = TRUE;
		FTLASSERT(rShareInfo.dwTraceTlsIndex == 0);
		FTLASSERT(rShareInfo.dwBlockElapseTlsIndex == 0);

		rShareInfo.dwTraceTlsIndex = TlsAlloc();
		FTLASSERT(TLS_OUT_OF_INDEXES != rShareInfo.dwTraceTlsIndex);
		if (TLS_OUT_OF_INDEXES != rShareInfo.dwTraceTlsIndex)
		{
			TlsSetValue(rShareInfo.dwTraceTlsIndex, NULL);		
		}
		rShareInfo.dwBlockElapseTlsIndex = TlsAlloc();
		FTLASSERT(TLS_OUT_OF_INDEXES != rShareInfo.dwBlockElapseTlsIndex);
		if (TLS_OUT_OF_INDEXES != rShareInfo.dwBlockElapseTlsIndex)
		{
			TlsSetValue(rShareInfo.dwBlockElapseTlsIndex, NULL);
		}

		rShareInfo.nTraceSequenceNumber = 0;
		rShareInfo.nBlockElapseId = 0;
		return bRet;
	}

	BOOL CALLBACK _FtlGlobalShareInfoFinalize(FTLGlobalShareInfo& rShareInfo)
	{
		if (TLS_OUT_OF_INDEXES != rShareInfo.dwTraceTlsIndex)
		{
			//CFFastTrace::CFTFileWriter*
			FTLASSERT(NULL == TlsGetValue(rShareInfo.dwTraceTlsIndex));			//��ص���Դ�����Ѿ��ͷ�
			TlsFree(rShareInfo.dwTraceTlsIndex);
			rShareInfo.dwTraceTlsIndex = TLS_OUT_OF_INDEXES;
		}
		if (TLS_OUT_OF_INDEXES != rShareInfo.dwBlockElapseTlsIndex)
		{
			//BlockElapseInfo*
			FTLASSERT(NULL == TlsGetValue(rShareInfo.dwBlockElapseTlsIndex));	//��ص���Դ�����Ѿ��ͷ�
			TlsFree(rShareInfo.dwBlockElapseTlsIndex);
			rShareInfo.dwBlockElapseTlsIndex = TLS_OUT_OF_INDEXES;
		}
		return TRUE;
	}

    template<typename TBase, typename INFO_TYPE, LONG bufLen>
    CFConvertInfoT<TBase,INFO_TYPE,bufLen>::CFConvertInfoT(INFO_TYPE info)
        :m_Info(info)
    {
        ZeroMemory(m_bufInfo,sizeof(m_bufInfo));
    }
    template<typename TBase, typename INFO_TYPE, LONG bufLen>
    CFConvertInfoT<TBase,INFO_TYPE,bufLen>::~CFConvertInfoT()
    {

    }
    template<typename TBase, typename INFO_TYPE, LONG bufLen>
    LPCTSTR CFConvertInfoT<TBase,INFO_TYPE,bufLen>::GetConvertedInfo()
    {
        TBase* pThis = static_cast<TBase*>(this);
        return pThis->ConvertInfo();
    }

    template<typename TBase, typename INFO_TYPE, LONG bufLen>
    INFO_TYPE CFConvertInfoT<TBase,INFO_TYPE,bufLen>::GetInfo() const
    {
        return m_Info;
    }

    template<typename TBase, typename INFO_TYPE, LONG bufLen>
    void CFConvertInfoT<TBase,INFO_TYPE,bufLen>::SetInfo(INFO_TYPE info)
    {
        m_Info = info;
        m_bufInfo[0] = NULL;
    }

    CFAPIErrorInfo::CFAPIErrorInfo(DWORD dwError) : CFConvertInfoT<CFAPIErrorInfo,DWORD>(dwError)
    {
		m_LanguageID = MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US);
        //m_LanguageID = MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL);//��������
    }

	DWORD CFAPIErrorInfo::SetLanguageID(DWORD dwLanguageID)
	{
		DWORD dwOldLanguageID = m_LanguageID;
		m_LanguageID = dwLanguageID;
		return dwOldLanguageID;
	}

    LPCTSTR CFAPIErrorInfo::ConvertInfo()
    {
        if (NULL == m_bufInfo[0])
        {
            switch (m_Info)
            {
#ifndef STATUS_DLL_NOT_FOUND
#  define STATUS_DLL_NOT_FOUND      ((DWORD )0xC0000135L)
#endif 
				//Side By Side Error
			    HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),ERROR_SXS_CANT_GEN_ACTCTX);
				
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),STATUS_DLL_NOT_FOUND);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),EXCEPTION_ACCESS_VIOLATION);
            default:
                break;
            }
            if (NULL == m_bufInfo[0])
            {
                LPTSTR pszMsgBuf = NULL;
                DWORD dwCount = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER 
                    | FORMAT_MESSAGE_FROM_SYSTEM 
                    | FORMAT_MESSAGE_IGNORE_INSERTS,    //��Ϣ�����еĲ�������(���� %s һ��)�ᱻһֱ���Ժ�����ֱ��������������ı�
                    NULL,
                    m_Info,
                    m_LanguageID,
                    (LPTSTR)&pszMsgBuf,
                    0,
                    NULL);

				if (!pszMsgBuf)
				{
					static LPCTSTR pszErrorModule[] = {
						TEXT("wininet.dll"),
						TEXT("netmsg.dll"),
						//TEXT("p2p.dll"),  // HRESULT_FACILITY(hrError) == FACILITY_P2P
					};

					for (int i = 0; i < _countof(pszErrorModule); i++)
					{
						//Just get loaded module in the calling process 
						HMODULE hModule = GetModuleHandle(pszErrorModule[i]);//
						//LoadLibraryEx(pszErrorModule[i], NULL, DONT_RESOLVE_DLL_REFERENCES);
						if (hModule != NULL) 
						{
							dwCount = FormatMessage(FORMAT_MESSAGE_FROM_HMODULE |FORMAT_MESSAGE_ALLOCATE_BUFFER,
								//| FORMAT_MESSAGE_FROM_SYSTEM,
								hModule, m_Info, m_LanguageID,
								(LPTSTR)&pszMsgBuf, 0, NULL);
							//FreeLibrary(hModule);

							if (pszMsgBuf)
							{
								//find message
								break;
							}
						}
					}
				}

                if(0 != dwCount && NULL != pszMsgBuf )
                {
					int length = _tcsclen(pszMsgBuf);
					while (length > 0 
						&& (pszMsgBuf[length - 1] == _T('\r') || pszMsgBuf[length - 1] == _T('\n')))
					{
						pszMsgBuf[length - 1]=_T('\0');
						length--;
					}

					//lstrcpyn(m_bufInfo,pszMsgBuf,_countof(m_bufInfo) - 1);
					StringCchPrintf(m_bufInfo, _countof(m_bufInfo) - 1, TEXT("%s"), pszMsgBuf);
					LocalFree(pszMsgBuf);
                }
                else //�޷���ȡ����ֵ
                {
                    StringCchPrintf(m_bufInfo,_countof(m_bufInfo),TEXT("Unknown Error:0x%08x"),m_Info);
                }
            }
        }
        return m_bufInfo;
    }

#ifndef FACILITY_D3DX
#  define FACILITY_D3D                     0x876
#  define FACILITY_D3DX                    0x877
#  define FACILITY_DSound                  0x878
#endif 

    CFComErrorInfo::CFComErrorInfo(HRESULT hr) : CFConvertInfoT<CFComErrorInfo,HRESULT>(hr)
    {
    }

    LPCTSTR CFComErrorInfo::GetErrorFacility(HRESULT hr, LPTSTR pszFacility,DWORD dwLength)
    {
        int nFacility = HRESULT_FACILITY(hr);
        switch (nFacility)
        {
            HANDLE_CASE_TO_STRING(pszFacility,dwLength,FACILITY_NULL);         //0
            HANDLE_CASE_TO_STRING(pszFacility,dwLength,FACILITY_RPC);          //1
            HANDLE_CASE_TO_STRING(pszFacility,dwLength,FACILITY_DISPATCH);     //2
            HANDLE_CASE_TO_STRING(pszFacility,dwLength,FACILITY_STORAGE);      //3
            HANDLE_CASE_TO_STRING(pszFacility,dwLength,FACILITY_ITF);          //4
            HANDLE_CASE_TO_STRING(pszFacility,dwLength,FACILITY_WIN32);        //7
            HANDLE_CASE_TO_STRING(pszFacility,dwLength,FACILITY_WINDOWS);      //8
            HANDLE_CASE_TO_STRING(pszFacility,dwLength,FACILITY_SECURITY);     //9
            //HANDLE_CASE_TO_STRING(pszFacility,dwLength,FACILITY_SSPI);       //9
            HANDLE_CASE_TO_STRING(pszFacility,dwLength,FACILITY_CONTROL);      //10
            HANDLE_CASE_TO_STRING(pszFacility,dwLength,FACILITY_CERT);         //11
            HANDLE_CASE_TO_STRING(pszFacility,dwLength,FACILITY_INTERNET);     //12
            HANDLE_CASE_TO_STRING(pszFacility,dwLength,FACILITY_MEDIASERVER);  //13
            HANDLE_CASE_TO_STRING(pszFacility,dwLength,FACILITY_MSMQ);         //14
            HANDLE_CASE_TO_STRING(pszFacility,dwLength,FACILITY_SETUPAPI);     //15
            HANDLE_CASE_TO_STRING(pszFacility,dwLength,FACILITY_SCARD);        //16
            HANDLE_CASE_TO_STRING(pszFacility,dwLength,FACILITY_COMPLUS);      //17
            HANDLE_CASE_TO_STRING(pszFacility,dwLength,FACILITY_AAF);          //18
            HANDLE_CASE_TO_STRING(pszFacility,dwLength,FACILITY_URT);          //19
            HANDLE_CASE_TO_STRING(pszFacility,dwLength,FACILITY_ACS);          //20
            HANDLE_CASE_TO_STRING(pszFacility,dwLength,FACILITY_DPLAY);        //21
            HANDLE_CASE_TO_STRING(pszFacility,dwLength,FACILITY_UMI);          //22
            HANDLE_CASE_TO_STRING(pszFacility,dwLength,FACILITY_SXS);          //23
            HANDLE_CASE_TO_STRING(pszFacility,dwLength,FACILITY_WINDOWS_CE);   //24
            HANDLE_CASE_TO_STRING(pszFacility,dwLength,FACILITY_HTTP);         //25
            HANDLE_CASE_TO_STRING(pszFacility,dwLength,FACILITY_BACKGROUNDCOPY);//32
            HANDLE_CASE_TO_STRING(pszFacility,dwLength,FACILITY_CONFIGURATION);//33
            HANDLE_CASE_TO_STRING(pszFacility,dwLength,FACILITY_STATE_MANAGEMENT);//34
            HANDLE_CASE_TO_STRING(pszFacility,dwLength,FACILITY_METADIRECTORY);//35

            HANDLE_CASE_TO_STRING(pszFacility,dwLength,FACILITY_D3D);           //0x876
            HANDLE_CASE_TO_STRING(pszFacility,dwLength,FACILITY_D3DX);          //0x877
            HANDLE_CASE_TO_STRING(pszFacility,dwLength,FACILITY_DSound);        //0x878
        default:
            StringCchPrintf(pszFacility,dwLength,TEXT("Unknown[%d]"),nFacility);
            break;
        }
        return pszFacility;
    }

    LPCTSTR CFComErrorInfo::ConvertInfo()
    {
        if (NULL == m_bufInfo[0])
        {
            switch(m_Info)
            {
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), S_FALSE);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), E_FAIL);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), E_INVALIDARG);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), E_NOINTERFACE);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), E_NOTIMPL);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), DISP_E_MEMBERNOTFOUND);
				HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), E_UNEXPECTED);
                //HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), CONNECT_E_NOCONNECTION);
            default:
                TCHAR strFacility[20] = {0};
                GetErrorFacility(m_Info,strFacility,_countof(strFacility));

                CFAPIErrorInfo apiErrorInfo(m_Info);
                StringCchPrintf(m_bufInfo,_countof(m_bufInfo),
                    TEXT("%s, Facility:%s"),apiErrorInfo.GetConvertedInfo(),strFacility);
                break;
            }
        }
        return m_bufInfo;
    }

	CFWindowDumpInfo::CFWindowDumpInfo(HWND hWnd) 
		: CFConvertInfoT<CFWindowDumpInfo, HWND>(hWnd)
	{
	}

	LPCTSTR CFWindowDumpInfo::ConvertInfo()
	{
		BOOL bRet = TRUE;
		if (NULL == m_bufInfo[0])
		{
			TCHAR szClassName[64] = {0};
			API_VERIFY(0 != ::GetClassName(m_Info, szClassName, _countof(szClassName)));

			//Window Text May be Empty, So it will return zero
			TCHAR szWindowText[128] = {0};
			API_VERIFY_EXCEPT1(0 != ::GetWindowText(m_Info, szWindowText, _countof(szWindowText)), ERROR_SUCCESS);

			RECT rcWindow = {0};
			API_VERIFY(::GetWindowRect(m_Info, &rcWindow));

			StringCchPrintf(m_bufInfo,_countof(m_bufInfo), 
				TEXT("HWnd=0x%x, ClassName='%s', WindowText='%s', rcWindow=(%d,%d)-(%d,%d), %dx%d"),
				m_Info, szClassName, szWindowText, 
				rcWindow.left, rcWindow.top, rcWindow.right, rcWindow.bottom,
				rcWindow.right - rcWindow.left, rcWindow.bottom - rcWindow.top);
		}
		return m_bufInfo;
	}

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    template <typename T, UINT DefaultFixedCount/* = DEFAULT_MEMALLOCATOR_FIXED_COUNT*/, MemoryAllocType allocType  /*= matNew*/>
    CFMemAllocator<T, DefaultFixedCount, allocType>::CFMemAllocator()
        :m_pMem(NULL)
        ,m_allocType(allocType)
        ,m_nCount(DefaultFixedCount)
        ,m_bAlignment(FALSE)
    {
		ZeroMemory(m_FixedMem, sizeof(m_FixedMem));
        _Init(DefaultFixedCount, m_bAlignment);
    }

    template <typename T, UINT DefaultFixedCount/* = DEFAULT_MEMALLOCATOR_FIXED_COUNT*/, MemoryAllocType allocType  /*= matNew*/>
    CFMemAllocator<T, DefaultFixedCount, allocType>::CFMemAllocator(DWORD nCount, BOOL bAlignment)
        :m_pMem(NULL)
        ,m_allocType(allocType)
        ,m_nCount(nCount)
        ,m_bAlignment(bAlignment)
    {
        if (0 == nCount)
        {
            nCount = 1;
        }
    	ZeroMemory(m_FixedMem, sizeof(m_FixedMem));
        _Init(nCount, m_bAlignment);
    }

    template <typename T, UINT DefaultFixedCount/* = DEFAULT_MEMALLOCATOR_FIXED_COUNT*/, MemoryAllocType allocType  /*= matNew*/>
    VOID CFMemAllocator<T, DefaultFixedCount, allocType>::_Init(DWORD nCount, BOOL bAlignment)
    {
        if (bAlignment)
        {
            nCount = _AdjustAlignmentSize(nCount);
        }
        if (nCount > DefaultFixedCount)
        {
            m_nCount = nCount;
            switch(allocType)
            {
            case matNew:
                m_pMem = new T[nCount];
                ZeroMemory(m_pMem,sizeof(T) * nCount); //������ڴ棬��֤û����������--�Ƿ�������ָ�����⣿����Ӱ�죿
                break;
            case matVirtualAlloc:
                m_pMem = (T*)VirtualAlloc(NULL, sizeof(T) * nCount, MEM_COMMIT, PAGE_READWRITE);
                break;
            default:
                FTLASSERT(FALSE);
                break;
            }
        }
    }

    template <typename T, UINT DefaultFixedCount/* = DEFAULT_MEMALLOCATOR_FIXED_COUNT*/, MemoryAllocType allocType  /*= matNew*/>
    UINT CFMemAllocator<T, DefaultFixedCount, allocType>::_AdjustAlignmentSize(UINT nCount)
    {
        static UINT s_nAlignmentSize[] = {1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024,
            2048, 4096, 8192, 16384, 32768, 65536, 131072, 262144, 524288, 1048576, //1048576 = 1024 * 1024
            2097152, 4194304, 8388608, 16777216, 33554432, 67108864, 134217728, 268435456, 536870912, 1073741824, //1073741824 = 1024 * 1024 * 1024
            2147483648, UINT_MAX
        };
        INT nIndex = 0;
        while (nCount < s_nAlignmentSize[nIndex] && nIndex < _countof(s_nAlignmentSize))
        {
            nIndex++;
            
        }
        if (nIndex < _countof(s_nAlignmentSize))
        {
            nCount = s_nAlignmentSize[nIndex];
        }
        return nCount;
    }
    
    template <typename T, UINT DefaultFixedCount/* = DEFAULT_MEMALLOCATOR_FIXED_COUNT*/, MemoryAllocType allocType  /*= matNew*/>
    VOID CFMemAllocator<T, DefaultFixedCount, allocType>::_FreeMemory()
    {
        BOOL bRet = TRUE;
        if (m_pMem)
        {
            switch (allocType)
            {
            case matNew:
                SAFE_DELETE_ARRAY(m_pMem);
                break;
            case matVirtualAlloc:
                API_VERIFY(VirtualFree(m_pMem, 0, MEM_RELEASE));
                m_pMem = NULL;
                break;
            default:
                FTLASSERT(FALSE);
                break;
            }
        }
    }

    template <typename T, UINT DefaultFixedCount/* = DEFAULT_MEMALLOCATOR_FIXED_COUNT*/, MemoryAllocType allocType  /*= matNew*/>
    CFMemAllocator<T, DefaultFixedCount, allocType>::~CFMemAllocator()
    {
        _FreeMemory();
    }

    template <typename T, UINT DefaultFixedCount/* = DEFAULT_MEMALLOCATOR_FIXED_COUNT*/, MemoryAllocType allocType  /*= matNew*/>
    T* CFMemAllocator<T, DefaultFixedCount, allocType>::Detatch()
    {
        T* pTmpMem = m_pMem;
        m_pMem = NULL;
        m_nCount = DefaultFixedCount;
        return pTmpMem;
    }

    template <typename T, UINT DefaultFixedCount/* = DEFAULT_MEMALLOCATOR_FIXED_COUNT*/, MemoryAllocType allocType  /*= matNew*/>
    T* CFMemAllocator<T, DefaultFixedCount, allocType>::GetMemory( UINT nMaxSize )
    {
        if ( !m_pMem && nMaxSize <= DefaultFixedCount )
        {
            return m_FixedMem;
        }

        if ( m_nCount < nMaxSize || NULL == m_pMem )
        {
            _FreeMemory();
            m_nCount = _GetBlockSize( nMaxSize );
            _Init(m_nCount, m_bAlignment);
        }
		return m_pMem;
    }

	template <typename T, UINT DefaultFixedCount/* = DEFAULT_MEMALLOCATOR_FIXED_COUNT*/, MemoryAllocType allocType  /*= matNew*/>
	T* CFMemAllocator<T, DefaultFixedCount, allocType>::GetMemory()
	{
		if ( !m_pMem && m_nCount <= DefaultFixedCount )
		{
			return m_FixedMem;
		}
		return m_pMem;
	}

    template <typename T, UINT DefaultFixedCount/* = DEFAULT_MEMALLOCATOR_FIXED_COUNT*/, MemoryAllocType allocType  /*= matNew*/>
    UINT CFMemAllocator<T, DefaultFixedCount, allocType>::GetCount() const
    {
        return m_nCount;
    }

    template <typename T, UINT DefaultFixedCount/* = DEFAULT_MEMALLOCATOR_FIXED_COUNT*/, MemoryAllocType allocType  /*= matNew*/>
    UINT CFMemAllocator<T, DefaultFixedCount, allocType>::_GetBlockSize(UINT nMaxCount)
    {
        UINT TempSize = m_nCount;
        while ( TempSize < nMaxCount )
        {
            TempSize *= 2;
        }
        return TempSize;
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	CFStringFormater::CFStringFormater(DWORD dwInitAllocLength/* = MAX_BUFFER_LENGTH*/, 
		DWORD dwMaxBufferTimes /* = STRINGFORMATER_BUFFER_MAX_DEFAULT_TIMES */)
        : m_dwInitAllocLength(dwInitAllocLength)
		, m_dwMaxBufferTimes(dwMaxBufferTimes)
    {
        m_pBuf = NULL;
        m_dwTotalSpaceSize = 0;
    }
    CFStringFormater::~CFStringFormater()
    {
        SAFE_DELETE_ARRAY(m_pBuf);
    }

    BOOL CFStringFormater::Reset(DWORD dwNewSize /* = 0 */)
    {
		FTLASSERT(dwNewSize >= 0);
		if (dwNewSize == m_dwTotalSpaceSize)
		{
			//TODO:Zero old memory?
			//if (m_pBuf)
			//{
			//	m_pBuf[0] = NULL;
			//}
			return TRUE;
		}

		BOOL bRet = FALSE;
		if (0 < dwNewSize)
		{
			LPTSTR pszNewBuf = new TCHAR[dwNewSize];
			if (pszNewBuf)
			{
				if (m_pBuf)
				{
					StringCchCopy(pszNewBuf, dwNewSize - 1, m_pBuf);
					SAFE_DELETE_ARRAY(m_pBuf);
				}
				else
				{
					pszNewBuf[0] = NULL;
				}
				m_pBuf = pszNewBuf;
				m_dwTotalSpaceSize = dwNewSize;
				bRet = TRUE;
			}
		}
		else if(0 == dwNewSize)
		{
			SAFE_DELETE_ARRAY(m_pBuf);
			m_dwTotalSpaceSize = dwNewSize;
			bRet = TRUE;
		}
		return bRet;
    }

    HRESULT CFStringFormater::Format(LPCTSTR lpszFormat, ...)
    {
        CHECK_POINTER_RETURN_VALUE_IF_FAIL(lpszFormat,E_INVALIDARG);
        HRESULT hr = E_FAIL;

        va_list pArgs;
        va_start(pArgs, lpszFormat);
        COM_VERIFY(FormatV(lpszFormat, pArgs));
        va_end(pArgs);

        return hr;
    }

    HRESULT CFStringFormater::FormatV(LPCTSTR lpszFormat, va_list argList)
    {
        HRESULT hr = E_FAIL;

		DWORD dwLength = m_dwTotalSpaceSize ? m_dwTotalSpaceSize : m_dwInitAllocLength;
		LPTSTR pszDestEnd = NULL;
		size_t cchRemaining = 0;
		DWORD dwFlags = 0;
		//STRSAFE_FILL_ON_FAILURE | STRSAFE_FILL_BEHIND_NULL; //ʧ��ʱȫ�����NULL���ɹ�ʱ��������NULL

		do 
		{
			if (!m_pBuf)
			{
				m_pBuf = new TCHAR[dwLength];
				if (NULL == m_pBuf)
				{
					break;
				}
			}

			ZeroMemory(m_pBuf,sizeof(TCHAR) * dwLength);
			COM_VERIFY_EXCEPT1(StringCchVPrintfEx(m_pBuf,dwLength, &pszDestEnd,&cchRemaining,dwFlags,lpszFormat,argList)
				,STRSAFE_E_INSUFFICIENT_BUFFER);
			if (SUCCEEDED(hr))
			{
				m_dwTotalSpaceSize = dwLength;
				// ȷ�����Ľ���������ʵ�����Ѿ��� StringCchVPrintfEx ���
				//m_pBuf[dwLength - 1] = TEXT('\0');
			}
			else if (STRSAFE_E_INSUFFICIENT_BUFFER == hr)
			{
				SAFE_DELETE_ARRAY(m_pBuf);
				//����ڴ�ռ䲻����ÿ������2���ڴ泤�ȣ����³��ԣ�ֱ���ɹ����ڴ����ʧ��
				dwLength <<= 1; //dwLength *=2;
			}
		} while (hr == STRSAFE_E_INSUFFICIENT_BUFFER && dwLength <= m_dwMaxBufferTimes * m_dwInitAllocLength);
		
		if (NULL == m_pBuf)
		{
			m_dwTotalSpaceSize = 0;
			hr = HRESULT_FROM_WIN32(ERROR_OUTOFMEMORY);
		}

		return hr;
    }
    HRESULT CFStringFormater::AppendFormat(LPCTSTR lpszFormat, ...)
    {
        CHECK_POINTER_RETURN_VALUE_IF_FAIL(lpszFormat,E_INVALIDARG);
        HRESULT hr = E_FAIL;

        va_list pArgs;
        va_start(pArgs, lpszFormat);
        COM_VERIFY(AppendFormatV(lpszFormat, pArgs));
        va_end(pArgs);
        return hr;
    }

    HRESULT CFStringFormater::AppendFormatV(LPCTSTR lpszFormat, va_list argList)
    {
        HRESULT hr = E_FAIL;
        if (NULL == m_pBuf)
        {
            COM_VERIFY(FormatV(lpszFormat,argList));
            return hr;
        }

        
        DWORD dwOldStringLen = static_cast<DWORD>(_tcslen(m_pBuf));
		LPTSTR pszOldString = NULL;

        LPTSTR pszDestEnd = NULL;
        size_t cchRemaining = 0;
        DWORD dwFlags = 0;
        //STRSAFE_FILL_ON_FAILURE | STRSAFE_FILL_BEHIND_NULL; //ʧ��ʱȫ�����NULL���ɹ�ʱ��������NULL

        //ָ��ԭ���ַ����Ľ���λ��
        LPTSTR pszAppendPos = m_pBuf + dwOldStringLen;
        DWORD dwLength = m_dwTotalSpaceSize;
        COM_VERIFY_EXCEPT1(StringCchVPrintfEx(pszAppendPos,dwLength - dwOldStringLen, &pszDestEnd,&cchRemaining,dwFlags,lpszFormat,argList)
            ,STRSAFE_E_INSUFFICIENT_BUFFER);

		if (STRSAFE_E_INSUFFICIENT_BUFFER == hr)
		{
			//����ԭ�����ַ���
			pszOldString = new TCHAR[dwOldStringLen + 1];
			CopyMemory(pszOldString, m_pBuf, sizeof(TCHAR) * dwOldStringLen);
			pszOldString[dwOldStringLen] = NULL;
		}

        //����ڴ�ռ䲻����ÿ������2���ڴ泤�ȣ����³��ԣ�ֱ���ɹ����ڴ����ʧ��
        while (hr == STRSAFE_E_INSUFFICIENT_BUFFER && dwLength < 8 * m_dwInitAllocLength)
        {
            SAFE_DELETE_ARRAY(m_pBuf);
            dwLength *= 2;
            m_pBuf = new TCHAR[dwLength];
            if (NULL == m_pBuf)
            {
                break;
            }
            ZeroMemory(m_pBuf,sizeof(TCHAR) * dwLength);
            COM_VERIFY(::StringCchCopy(m_pBuf,dwOldStringLen + 1,pszOldString));
            pszAppendPos = m_pBuf + dwOldStringLen;
            COM_VERIFY_EXCEPT1(StringCchVPrintfEx(pszAppendPos,dwLength - dwOldStringLen, &pszDestEnd,&cchRemaining,
                dwFlags,lpszFormat,argList),STRSAFE_E_INSUFFICIENT_BUFFER);
        }
        if (NULL == m_pBuf)
        {
            //Not enough memory
            SAFE_DELETE_ARRAY(pszOldString);
            return HRESULT_FROM_WIN32(ERROR_NOT_ENOUGH_MEMORY);
        }

        m_dwTotalSpaceSize = dwLength;
        SAFE_DELETE_ARRAY(pszOldString);
        return hr;
    }

    LPCTSTR CFStringFormater::GetString() const
    {
        return m_pBuf;
    }
	LPTSTR CFStringFormater::GetString()
	{
		return m_pBuf;
	}
    LONG CFStringFormater::GetStringLength() const
    {
        if (m_pBuf)
        {
            return (lstrlen(m_pBuf));
        }
        return 0;
    }
	LONG CFStringFormater::GetSize() const
	{
		return LONG(m_dwTotalSpaceSize);
	}

    LPTSTR CFStringFormater::Detach()
    {
        LPTSTR pBuf = m_pBuf;
        m_pBuf = NULL;
        m_dwTotalSpaceSize = 0;
        return pBuf;
    }
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    CFFastTrace::CFTFileWriter::CFTFileWriter()
    {
        m_hFile = INVALID_HANDLE_VALUE ;
        m_lWrites = 0 ;
        m_dwTID = 0 ;
    }
    CFFastTrace::CFTFileWriter::~CFTFileWriter()
    {
        if (INVALID_HANDLE_VALUE != m_hFile)
        {
            Close();
        }
    }

    BOOL CFFastTrace::CFTFileWriter::Initialize( HANDLE hFile , DWORD dwTID )
    {
        FTLASSERT ( INVALID_HANDLE_VALUE != hFile ) ;
        if ( INVALID_HANDLE_VALUE == hFile )
        {
            return ( FALSE ) ;
        }
        BOOL bRet = FALSE;
        m_hFile = hFile ;
        m_dwTID = dwTID ;
        m_lWrites = 0 ;// Ensure the count starts at zero.
        API_VERIFY( WriteHeader() );
        if ( FALSE == bRet )
        {
            // Not much else I can do.
            API_VERIFY ( CloseHandle ( m_hFile ) ) ;
            m_hFile = INVALID_HANDLE_VALUE ;
        }
        return ( bRet ) ;
    }
    BOOL CFFastTrace::CFTFileWriter::WriteData( LPFTDATA pData )
    {
        FTLASSERT ( INVALID_HANDLE_VALUE != m_hFile ) ;
        if ( INVALID_HANDLE_VALUE == m_hFile )
        {
            return ( FALSE ) ;
        }
        FTLASSERT(pData->nTraceInfoLen == (LONG)(_tcslen(pData->pszTraceInfo) + 1));
        BOOL bRet = FALSE;
        DWORD dwWritten = 0 ;
        API_VERIFY(::WriteFile(m_hFile,pData,
            sizeof(FTDATA) - sizeof(LPCTSTR),  //��ȥ���� pszTraceInfoָ��
            &dwWritten,NULL));
        FTLASSERT(sizeof(FTDATA) - sizeof(LPCTSTR) == dwWritten);
        if (bRet)
        {
            //д�ַ�����Ϣ
            API_VERIFY(::WriteFile(m_hFile,pData->pszTraceInfo,sizeof(TCHAR) * pData->nTraceInfoLen,&dwWritten,NULL));
        }
        if(bRet)
        {
            // It's a good write.
            m_lWrites++ ;
        }
        else
        {
            // Let's attempt to write the header so it gets updated.
            WriteHeader ( ) ;
            bRet = FALSE ;
            // Not much else I can do.
            API_VERIFY ( CloseHandle ( m_hFile ) ) ;
            m_hFile = INVALID_HANDLE_VALUE ;
        }
        return ( bRet ) ;
    }
    BOOL CFFastTrace::CFTFileWriter::Flush()
    {
        BOOL bRet = FALSE;
        return bRet;
    }
    BOOL CFFastTrace::CFTFileWriter::SnapFile()
    {
        BOOL bRet = FALSE;
        return bRet;
    }
    BOOL CFFastTrace::CFTFileWriter::WriteHeader()
    {
        BOOL bRet = FALSE ;
        FTLASSERT ( INVALID_HANDLE_VALUE != m_hFile ) ;
        if ( INVALID_HANDLE_VALUE != m_hFile )
        {
            // Scoot back to the beginning of the file and update the header.
            DWORD dwMove = SetFilePointer ( m_hFile,0,NULL,FILE_BEGIN) ;
            FTLASSERT ( INVALID_SET_FILE_POINTER != dwMove ) ;
            if ( INVALID_SET_FILE_POINTER != dwMove )
            {
                FTFILEHEADER stFH = {0} ;
                stFH.dwSig = FTFILESIG ;
                stFH.bIsUnicode = (sizeof(TCHAR) == 2);
                stFH.lItemCount= m_lWrites ;
                stFH.lPID = (LONG)GetCurrentProcessId ( ) ;
                stFH.lTID = (LONG)m_dwTID ;
                DWORD dwWritten = 0 ;
                DWORD dwWrtieSize = sizeof(FTFILEHEADER);
                API_VERIFY(::WriteFile(m_hFile,&stFH,dwWrtieSize,&dwWritten,NULL)) ;
                FTLASSERT ( dwWrtieSize == dwWritten ) ;
                if ( ( FALSE == bRet) || ( dwWrtieSize != dwWritten )   )
                {
                    bRet = FALSE ;
                }
            }
            else
            {
                bRet = FALSE ;
            }
        }
        return ( bRet ) ;
    }
    BOOL CFFastTrace::CFTFileWriter::Close()
    {
        BOOL bRet = FALSE;
        FTLASSERT ( INVALID_HANDLE_VALUE != m_hFile ) ;
        if ( INVALID_HANDLE_VALUE != m_hFile )
        {
            // Scoot back to the beginning of the file and update the
            // header.
            API_VERIFY(WriteHeader()) ;
            // I'm done with the file.
            API_VERIFY(::CloseHandle( m_hFile ) ) ;
            m_hFile = INVALID_HANDLE_VALUE ;
            m_lWrites = 0 ;
        }
        return ( bRet ) ;
    }

	bool CFFastTrace::CFTFileWriter::operator < (const CFTFileWriter & other) const
	{
		COMPARE_MEM_LESS(m_hFile, other);
		return true;
	}
	
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    CFFastTrace& CFFastTrace::GetInstance()
    {
        CFLastErrorRecovery  lastErrorRecovery;
        static CFFastTrace fastTrace(TEXT("Software\\FTL"),NULL);
        return fastTrace;
    }

    CFFastTrace::CFFastTrace(LPCTSTR pszKey, HMODULE hModule)
    {
        //BOOL bRet = FALSE;
		//m_bAllocTls = FALSE;

        InitializeCriticalSection(&m_CsLock);
        //m_SequenceNumber = 0;
		
		DWORD& rTlsIndex = g_GlobalShareInfo.GetShareValue().dwTraceTlsIndex;
		FTLASSERT(rTlsIndex != TLS_OUT_OF_INDEXES);
        
        TCHAR szModuleName[MAX_PATH] = {0};
        if(GetModuleFileName(hModule,szModuleName,_countof(szModuleName)) > 0)
        {
            LPTSTR pszExtPos = StrRChr(szModuleName,NULL,TEXT('\\'));
            if (pszExtPos)
            {
                pszExtPos++;
                StringCchCopyN(szModuleName,_countof(szModuleName),pszExtPos,
                    _countof(szModuleName) - (pszExtPos - &szModuleName[0]));
            }
        }

        HKEY hKey = NULL;
        TCHAR szModuleKey[MAX_PATH] = {0};
        StringCchPrintf(szModuleKey,_countof(szModuleKey),TEXT("%s\\%s"),pszKey,szModuleName);
        LONG lRet = RegCreateKeyEx(HKEY_CURRENT_USER,szModuleKey,0,NULL,0,KEY_ALL_ACCESS,NULL,&hKey,NULL);
        if (ERROR_SUCCESS == lRet)
        {
            DWORD dwValue = 0;
            DWORD dwBufLen = sizeof(dwValue);
            DWORD dwType;
            lRet = RegQueryValueEx(hKey,TEXT("WriteThrough"),NULL,&dwType,reinterpret_cast<LPBYTE>(&dwValue),&dwBufLen);
            if (ERROR_SUCCESS == lRet && REG_DWORD == dwType)
            {
                m_Options.bWriteThrough = (BOOL)dwValue;
            }
            else
            {
                ::RegSetValueEx(hKey,TEXT("WriteThrough"),0,REG_DWORD,reinterpret_cast<LPBYTE>(&m_Options.bWriteThrough), 
                    sizeof(m_Options.bWriteThrough));
            }

            lRet = RegQueryValueEx(hKey,TEXT("WriteToFile"),NULL,&dwType,reinterpret_cast<LPBYTE>(&dwValue),&dwBufLen);
            if (ERROR_SUCCESS == lRet && REG_DWORD == dwType)
            {
                m_Options.bWriteToFile = (BOOL)dwValue;
            }
            else
            {
                ::RegSetValueEx(hKey,TEXT("WriteToFile"),0,REG_DWORD,reinterpret_cast<LPBYTE>(&m_Options.bWriteToFile),
                    sizeof(m_Options.bWriteToFile));
            }

            lRet = RegQueryValueEx(hKey,TEXT("WriteDetail"),NULL,&dwType,reinterpret_cast<LPBYTE>(&dwValue),&dwBufLen);
            if (ERROR_SUCCESS == lRet && REG_DWORD == dwType)
            {
                m_Options.bWriteDetail = (BOOL)dwValue;
            }
            else
            {
                ::RegSetValueEx(hKey,TEXT("WriteDetail"),0,REG_DWORD,reinterpret_cast<LPBYTE>(&m_Options.bWriteDetail),
                    sizeof(m_Options.bWriteDetail));
            }

            lRet = RegQueryValueEx(hKey,TEXT("traceThreshold"),NULL,&dwType,reinterpret_cast<LPBYTE>(&dwValue),&dwBufLen);
            if (ERROR_SUCCESS == lRet && REG_DWORD == dwType)
            {
                m_Options.traceThreshold = TraceLevel(dwValue);
            }
            else
            {
                ::RegSetValueEx(hKey,TEXT("traceThreshold"),0,REG_DWORD,reinterpret_cast<LPBYTE>(&m_Options.traceThreshold),
                    sizeof(m_Options.traceThreshold));
            }
            ::RegCloseKey(hKey);
        }
    }

    CFFastTrace::~CFFastTrace()
    {
        CloseAllFileWriters();
#pragma TODO(TlsSetValue NULL when thread quit)
        DeleteCriticalSection ( &m_CsLock ) ;
    }

    BOOL CFFastTrace::CloseAllFileWriters()
    {
        BOOL bRet = TRUE;
        EnterCriticalSection(&m_CsLock);
        CFFastTrace::CFTFileWriter* pFileWriter = NULL;
        for (AllFileWriterArrayIterator iter = m_AllFileWriters.begin(); iter != m_AllFileWriters.end();
            ++iter)
        {
            pFileWriter = (*iter);
            FTLASSERT(pFileWriter);
            API_VERIFY(pFileWriter->Close());
            delete pFileWriter;
        }
        m_AllFileWriters.clear();
        LeaveCriticalSection(&m_CsLock);
        return bRet;
    }

    BOOL CFFastTrace::SetTraceOptions(LPFAST_TRACE_OPTIONS pOptions)
    {
        EnterCriticalSection(&m_CsLock);
        m_Options.bWriteDetail = pOptions->bWriteDetail;
        m_Options.bWriteThrough = pOptions->bWriteThrough;
        m_Options.bWriteToFile = pOptions->bWriteToFile;
        m_Options.traceThreshold = pOptions->traceThreshold;
        m_Options.pDebugOut = pOptions->pDebugOut;
        LeaveCriticalSection(&m_CsLock);
        return TRUE;
    }

    BOOL CFFastTrace::GetTraceOptions(LPFAST_TRACE_OPTIONS pOptions)
    {
        EnterCriticalSection(&m_CsLock);
        pOptions->bWriteDetail = m_Options.bWriteDetail;
        pOptions->bWriteThrough = m_Options.bWriteThrough;
        pOptions->bWriteToFile = m_Options.bWriteToFile;
        pOptions->traceThreshold = m_Options.traceThreshold;
        pOptions->pDebugOut = m_Options.pDebugOut;
        LeaveCriticalSection(&m_CsLock);
        return TRUE;
    }


    BOOL CFFastTrace::CheckLevel(TraceLevel level)
    {
        BOOL bRet = (level >= m_Options.traceThreshold);
        return bRet;
    }

	LPCTSTR CFFastTrace::GetLevelName(TraceLevel level)
	{
		LPCTSTR pszLevelName = TEXT("Unknown");
		switch(level)
		{
		case tlDetail:
			pszLevelName = TEXT("Detail");
			break;
		case tlInfo:
			pszLevelName = TEXT("Info");
			break;
		case tlTrace:
			pszLevelName = TEXT("Trace");
			break;
		case tlWarning:
			pszLevelName = TEXT("Warn");
			break;
		case tlError:
			pszLevelName = TEXT("Error");
			break;
		default:
			FTLASSERT(FALSE);
			break;
		}
		return pszLevelName;
	}

    void CFFastTrace::InternalWriteLogData(TraceLevel level, LPCTSTR pStrInfo)
    {
        BOOL bRet = FALSE;
        {
            //�˴��Ƿ���Ҫ���Ᵽ��?
            //EnterCriticalSection(&m_CsLock);
            if (m_Options.pDebugOut)
            {
				if (m_Options.bWriteDetail)
				{
					CFStringFormater formater;
					SYSTEMTIME st = {0};
					GetLocalTime(&st);
					DWORD dwCurThreadId = GetCurrentThreadId();
					TCHAR chLevelNamePrefix = *GetLevelName(level);
					formater.Format(TEXT("[%02d:%02d %02d:%03d,%c,TID(%d)] %s"), 
						st.wHour, st.wMinute, st.wSecond, st.wMilliseconds,
						chLevelNamePrefix,
						dwCurThreadId,
						pStrInfo);
					m_Options.pDebugOut(formater.GetString());
				}
				else
				{
					m_Options.pDebugOut(pStrInfo);
				}
            }
            //LeaveCriticalSection(&m_CsLock);
        }

        if (m_Options.bWriteToFile)
        {       
            CFTFileWriter* pFileWriter = GetCurrentThreadFile();
            if ((CFTFileWriter*)INVALID_HANDLE_VALUE != pFileWriter)
            {
				FTDATA ftdata = {0} ;
                GetSystemTimeAsFileTime ( &ftdata.stTime ) ;// Get the current time stamp.
                ftdata.level = level;
                ftdata.nTraceInfoLen =  (LONG)(_tcslen(pStrInfo) + 1); //��������Ľ�����
                ftdata.pszTraceInfo = pStrInfo;

				LONG& rTraceSequenceNumber = g_GlobalShareInfo.GetShareValue().nTraceSequenceNumber;
                ftdata.lSeqNum = InterlockedIncrement (&rTraceSequenceNumber) ;

                API_VERIFY(pFileWriter->WriteData(&ftdata));
                if (!bRet)
                {   //д��Ϣʧ�ܣ����TLS
					DWORD& rTlsIndex = g_GlobalShareInfo.GetShareValue().dwTraceTlsIndex;
                    TlsSetValue ( rTlsIndex ,(LPVOID)INVALID_HANDLE_VALUE) ;
                    pFileWriter->Close() ;
                    RemoveFileFromArray ( pFileWriter ) ;
                    // Free the memory.
                    delete pFileWriter ;
                }
            }
        }
    }
    void CFFastTrace::WriteLogInfo(const LPCTSTR lpszFormat,...)
    {
        CFLastErrorRecovery  lastErrorRecovery;
        CFStringFormater formater(MAX_BUFFER_LENGTH, 1024);  //256K
		va_list argList;
        va_start(argList, lpszFormat);
		HRESULT hr = formater.FormatV(lpszFormat,argList);
		FTLASSERT(SUCCEEDED(hr));
        va_end(argList);
        InternalWriteLogData(tlTrace, formater.GetString());
    }

    void CFFastTrace::WriteLogInfoEx(TraceLevel level,const LPCTSTR lpszFormat,...)
    {
        CFLastErrorRecovery  lastErrorRecovery;
        if (CheckLevel(level))
        {
            CFStringFormater formater(MAX_BUFFER_LENGTH, 1024);	//256K
            va_list argList;
            va_start(argList, lpszFormat);
            HRESULT hr = formater.FormatV(lpszFormat,argList);
			FTLASSERT(SUCCEEDED(hr));
            va_end(argList);
            InternalWriteLogData(level, formater.GetString());
        }
    }

    CFFastTrace::CFTFileWriter* CFFastTrace::GetCurrentThreadFile()
    {
		DWORD& rTlsIndex = g_GlobalShareInfo.GetShareValue().dwTraceTlsIndex;
        FTLASSERT(TLS_OUT_OF_INDEXES != rTlsIndex);
        if (TLS_OUT_OF_INDEXES == rTlsIndex)
        {
            //��ʱ CFFastTrace �Ѿ��������������߳���д��־ -- ȫ�ֻ�̬�������� CFFastTrace �������ڳ�(�� CModulesHolder)
            return (CFFastTrace::CFTFileWriter*)(DWORD_PTR)(TLS_OUT_OF_INDEXES);
        }

        CFFastTrace::CFTFileWriter* pFileWriter = (CFFastTrace::CFTFileWriter*) TlsGetValue(rTlsIndex);
        
        //����NULL��ʾ��һ�η���  -- TODO: ��������� INVALID_HANDLE_VALUE, ��˵���ļ����ִ���,�Ѿ����ر�
        if (NULL == pFileWriter)  
        {
            BOOL bRet = FALSE ;
            DWORD dwTID = GetCurrentThreadId ( ) ;
            HANDLE hFile = CreateRawThreadFile ( dwTID ) ;
            if (INVALID_HANDLE_VALUE != hFile)
            {
                pFileWriter = new CFFastTrace::CFTFileWriter(); 
                API_VERIFY(pFileWriter->Initialize(hFile,dwTID));
                if (FALSE == bRet)
                {
                    pFileWriter->Close();
                    delete pFileWriter;
                    hFile = INVALID_HANDLE_VALUE ;
                    pFileWriter = (CFFastTrace::CFTFileWriter *)INVALID_HANDLE_VALUE ;
                }
            }
			//��pFileWriter���õ�TLS�У������´ο���ֱ�ӻ�ȡ
			API_VERIFY(TlsSetValue( rTlsIndex ,(LPVOID)pFileWriter)) ;
            if (INVALID_HANDLE_VALUE != pFileWriter)
            {
                AddFileToArray(pFileWriter);
            }
        }
        return pFileWriter;
    }

    HANDLE CFFastTrace::CreateRawThreadFile( DWORD dwTID )
    {
        HANDLE hRet = INVALID_HANDLE_VALUE ;
        TCHAR szFileName [ MAX_PATH + 1 ] = {0};
        BOOL bRet = FALSE;
        API_VERIFY( BuildThreadFileName ( szFileName ,MAX_PATH ,dwTID ));
        if ( TRUE == bRet )
        {
            DWORD dwFlags = FILE_ATTRIBUTE_ARCHIVE | FILE_FLAG_SEQUENTIAL_SCAN ;
            if ( TRUE == m_Options.bWriteThrough )
            {
                dwFlags |= FILE_FLAG_WRITE_THROUGH ;
            }

            // I've got the filename so I can now create the file.
            hRet = CreateFile ( szFileName ,
                GENERIC_WRITE      ,
                FILE_SHARE_READ    ,
                NULL               ,
                CREATE_ALWAYS      ,
                dwFlags            ,
                NULL                ) ;
            FTLASSERT( INVALID_HANDLE_VALUE != hRet ) ;
        }
        return ( hRet ) ;
    }

    BOOL CFFastTrace::BuildThreadFileName ( LPTSTR szFileNameBuff ,size_t iBuffLen,DWORD dwTID)
    {
        CHECK_POINTER_WRITABLE_DATA_RETURN_VALUE_IF_FAIL(szFileNameBuff,iBuffLen,FALSE);

		//TODO:�ŵ� SHGetFolderPath(CSIDL_LOCAL_APPDATA) �õ�����Ŀ¼�£������޷�д�����ļ�������

        BOOL bRet = FALSE ;
        // Get the process directory and name and peel off the .EXE extension.
        TCHAR szExeName[ MAX_PATH + 1 ] ;
        DWORD dwRet = GetModuleFileName ( NULL , szExeName , MAX_PATH ) ;
        API_VERIFY ( 0 != dwRet ) ;
        if ( 0 != dwRet )
        {
            TCHAR * pPeriod = _tcsrchr ( szExeName , _T ( '.' ) ) ;
            FTLASSERT ( NULL != pPeriod ) ;
            if ( NULL != pPeriod )
            {
                *pPeriod = _T ( '\0' ) ;

                DWORD dwPID = GetCurrentProcessId ( ) ;
                // Build the complete
                HRESULT hr = E_FAIL;
                //���ǵ��߳�ID�����ظ�����(Ƶ��������ɾ��ʱ)����˼���һ��ʱ������
                COM_VERIFY(StringCchPrintf ( szFileNameBuff, iBuffLen ,TEXT("%s_%04d_%04d_%04d.FTL") ,
                    szExeName ,dwPID ,dwTID,GetTickCount()));
                if ( SUCCEEDED ( hr ) )
                {
                    bRet = TRUE ;
                }
            }
        }
        return ( bRet ) ;
    }

    BOOL CFFastTrace::AddFileToArray(CFTFileWriter* pFileWriter)
    {
        BOOL bRet = FALSE;
        {
            EnterCriticalSection(&m_CsLock);
            AllFileWriterArrayPair pr;
            pr = m_AllFileWriters.insert(pFileWriter);
            bRet = pr.second;
            LeaveCriticalSection(&m_CsLock);
        }
        return bRet;
    }
    BOOL CFFastTrace::RemoveFileFromArray(CFTFileWriter* pFileWriter)
    {
        BOOL bRet = FALSE;
        {
            EnterCriticalSection(&m_CsLock);
            AllFileWriterArrayIterator iter = m_AllFileWriters.find(pFileWriter);
            if (iter != m_AllFileWriters.end())
            {
                bRet = TRUE;//�ҵ�
                m_AllFileWriters.erase(iter);
            }
            LeaveCriticalSection(&m_CsLock);
        }
        return bRet;
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    CFElapseCounter::CFElapseCounter()
    {
        BOOL bRet = FALSE;
        m_StopTime.QuadPart = 0;
        API_VERIFY(QueryPerformanceFrequency(&m_Frequency));
        API_VERIFY(QueryPerformanceCounter(&m_StartTime));
        if (0 == m_Frequency.QuadPart)
        {
            m_Frequency.QuadPart = 1; //��ֹ��0����ʲô����ϵͳ�ϻ����486��Win3.1��
        }
        m_PauseTime.QuadPart = m_StartTime.QuadPart;

        m_Status = rsRunning;
    }
    CFElapseCounter::~CFElapseCounter()
    {

    }
    RunningStatus CFElapseCounter::GetStatus() const
    {
        return m_Status;
    }

	BOOL CFElapseCounter::Reset()
	{        
		m_Status = rsStopped;
		m_StartTime.QuadPart = 0;
		m_PauseTime.QuadPart = 0;
		m_StopTime.QuadPart = 0;
		return TRUE;
	}

    BOOL CFElapseCounter::Start()
    {
        BOOL bRet = FALSE;
        API_VERIFY(QueryPerformanceCounter(&m_StartTime));
        m_PauseTime.QuadPart = m_StartTime.QuadPart;
        m_StopTime.QuadPart = 0;
        m_Status = rsRunning;
        return bRet;
    }
    BOOL CFElapseCounter::Pause()
    {
        FTLASSERT(rsRunning == m_Status||!TEXT("Must Start After Stop"));
        if (rsRunning != m_Status)
        {
            SetLastError(ERROR_BAD_COMMAND);
            return FALSE;
        }
        BOOL bRet = FALSE;
        API_VERIFY(QueryPerformanceCounter(&m_PauseTime));
        m_Status = rsPaused;
        return bRet;
    }
    BOOL CFElapseCounter::Resume()
    {
        FTLASSERT(rsPaused == m_Status);
        if (rsPaused != m_Status)
        {
            SetLastError(ERROR_BAD_COMMAND);
            return FALSE;
        }
        BOOL bRet = FALSE;
        LARGE_INTEGER   resumeTime;
        API_VERIFY(QueryPerformanceCounter(&resumeTime));
        m_StartTime.QuadPart += (resumeTime.QuadPart - m_PauseTime.QuadPart);
        m_PauseTime.QuadPart = m_StartTime.QuadPart;
        m_Status = rsRunning;
        return bRet;
    }
    BOOL CFElapseCounter::Stop()
    {
        BOOL bRet = TRUE;
        switch (m_Status)
        {
        case rsRunning://����״̬�£���ȡ�µĽ���ʱ��
            API_VERIFY(QueryPerformanceCounter(&m_StopTime));
            break;
        case rsPaused://��ͣ״̬�£���ȡ��ͣ��ʱ��
            m_StopTime.QuadPart = m_PauseTime.QuadPart;
            break;
        case rsStopped://ֹͣ״̬�£�ʲô������
        default:
            break;
        }
        m_Status = rsStopped;
        return bRet;
    }
    LONGLONG CFElapseCounter::GetElapseTime() //���ص�λ������(NS)
    {
        //in some machine,will overflow,so change to double count, and convert back

        BOOL bRet = FALSE;
        LONGLONG elapseNs;
        switch (m_Status)
        {
        case rsRunning: //(m_PauseTime.QuadPart == m_StartTime.QuadPart) //����״̬����ȡ��ǰʱ�䵽��ʼ��ʱ��
            {
                LARGE_INTEGER   nowTime;
                API_VERIFY(QueryPerformanceCounter(&nowTime));
                elapseNs = (LONGLONG)((double)(nowTime.QuadPart - m_StartTime.QuadPart) * ( NANOSECOND_PER_SECOND ) /m_Frequency.QuadPart); 
            }
            break;
        case rsPaused: ////��ͣ״̬����ȡ��ͣ����ʱ�䵽��ʼ��ʱ��
            elapseNs = (LONGLONG)((double)(m_PauseTime.QuadPart - m_StartTime.QuadPart) * ( NANOSECOND_PER_SECOND ) /m_Frequency.QuadPart);
            break;
        case rsStopped: //m_StopTime.QuadPart != 0  //�Ѿ�ֹͣ����ȡ��������ʼ��ʱ��
            elapseNs = (LONGLONG)((double)(m_StopTime.QuadPart - m_StartTime.QuadPart) * ( NANOSECOND_PER_SECOND ) /m_Frequency.QuadPart); 
            break;
        default:
            FTLASSERT(FALSE);
            elapseNs = -1;
            break;
        }

        FTLASSERT(elapseNs >= 0);
        return elapseNs;
    }

#ifdef FTL_DEBUG
    CFBlockElapse::CFBlockElapse(LPCTSTR pszFileName,DWORD line, 
        LPCTSTR pBlockName, TraceDetailType detailType, LPVOID pReturnAddr, DWORD MinElapse/* = 0*/)
        :m_pszFileName(pszFileName)
        ,m_Line(line)
        ,m_pszBlkName(pBlockName)
        ,m_traceDetailType(detailType)
        ,m_pReturnAdr(pReturnAddr)
        ,m_MinElapse(MinElapse)

    {
        FTLASSERT(pBlockName);
        FTLASSERT(pReturnAddr);
		LONG& rElapseId = g_GlobalShareInfo.GetShareValue().nBlockElapseId;
		m_nElapseId = InterlockedIncrement(&rElapseId);

		DWORD& rBlockElapseTlsIndex = g_GlobalShareInfo.GetShareValue().dwBlockElapseTlsIndex;
        ATLASSERT(rBlockElapseTlsIndex != TLS_OUT_OF_INDEXES);
        if (rBlockElapseTlsIndex != TLS_OUT_OF_INDEXES)
        {
            BlockElapseInfo* pInfo = (BlockElapseInfo*)TlsGetValue(rBlockElapseTlsIndex);
            if (NULL == pInfo)
            {
                FTLTRACEEX(FTL::tlWarning, TEXT("%s New Thread[%d] Begin Block Elapse Trace\n"), 
                    __FILE__LINE__, GetCurrentThreadId());
                pInfo = new BlockElapseInfo();
                ZeroMemory(pInfo, sizeof(BlockElapseInfo));
                TlsSetValue(rBlockElapseTlsIndex, pInfo);
            }
            pInfo->indent++;
            LONG curLevel = FTL_MIN(pInfo->indent, MAX_TRACE_INDICATE_LEVEL); //InterlockedIncrement(&s_Indent);
            for (LONG n = 0; n < curLevel; n++)
            {
                pInfo->bufIndicate[n] = TEXT('>');
            }
            pInfo->bufIndicate[curLevel] = 0;
            FAST_TRACE_EX(tlDetail, TEXT("%s(%d) :\t TID(0x%04x) %s (Enter \t%d): %s\n"),
                m_pszFileName,m_Line,GetCurrentThreadId(),pInfo->bufIndicate,pInfo->indent ,
                m_pszBlkName);
        }

        m_StartTime = GetTickCount();
    }

    CFBlockElapse::~CFBlockElapse()
    {
		//Save old last error to avoid effect it
        CFLastErrorRecovery lastErrorRecory;
        DWORD dwElapseTime = GetTickCount() - m_StartTime;

        DWORD& rBlockElapseTlsIndex = g_GlobalShareInfo.GetShareValue().dwBlockElapseTlsIndex;
        ATLASSERT(rBlockElapseTlsIndex != TLS_OUT_OF_INDEXES);
        if (rBlockElapseTlsIndex != TLS_OUT_OF_INDEXES)
        {
            BlockElapseInfo* pInfo = (BlockElapseInfo*)TlsGetValue(rBlockElapseTlsIndex);
            FTLASSERT(pInfo);
#pragma TODO(һ���̴߳�������������߳����ͷţ���ô������GraphEdt)
            if (m_MinElapse != 0 && dwElapseTime >= m_MinElapse)
            {
                if (pInfo)
                {
                    DWORD dwMaxSize = _countof(pInfo->szDetailName);
                    switch (m_traceDetailType)
                    {
                    case TraceDetailExeName:
                        GetModuleFileName(NULL, pInfo->szDetailName, dwMaxSize);
                        StringCchPrintf(pInfo->szDetailName, dwMaxSize, TEXT("%s in %s"), m_pszBlkName, 
                            PathFindFileName(pInfo->szDetailName));
                        break;
                    case TraceDetailModuleName:
                        {
                            MEMORY_BASIC_INFORMATION memInfo = {0};
                            VirtualQuery(m_pReturnAdr, &memInfo, sizeof(memInfo));
                            if (memInfo.BaseAddress)
                            {
                                GetModuleFileName((HMODULE)memInfo.AllocationBase, pInfo->szDetailName, dwMaxSize);
                                StringCchPrintf(pInfo->szDetailName, dwMaxSize, TEXT("%s in %s"), m_pszBlkName, 
                                    PathFindFileName(pInfo->szDetailName));
                            }
                        }
                        break;
                    case TraceDetailNone:
                    default:
                        StringCchPrintf(pInfo->szDetailName, dwMaxSize, TEXT("%s"), m_pszBlkName);
                        break;
                    }
                    FAST_TRACE_EX(tlWarning, TEXT("%s(%d):TID(0x%04x),ID(%ld) \"%s\"(0x%p) Elaspse Too Long (Want-%dms:Real-%dms)\n"),
                        m_pszFileName,m_Line,GetCurrentThreadId(), m_nElapseId, pInfo->szDetailName,m_pReturnAdr,m_MinElapse,dwElapseTime);
                }
                else
                {
                    FAST_TRACE_EX(tlWarning, TEXT("%s(%d):TID(0x%04x),ID(%ld) \"%s\"(0x%p) Elaspse Too Long (Want-%dms:Real-%dms)\n"),
                        m_pszFileName,m_Line,GetCurrentThreadId(), m_nElapseId, m_pszBlkName,m_pReturnAdr,m_MinElapse,dwElapseTime);
                }
            }
            if (pInfo)
            {
                LONG curLevel = FTL_MIN(pInfo->indent,MAX_TRACE_INDICATE_LEVEL);
                for (LONG n = 0; n < curLevel; n++)
                {
                    pInfo->bufIndicate[n] = TEXT('<');
                }
                pInfo->bufIndicate[curLevel] = 0;
                FAST_TRACE_EX(tlDetail,TEXT("%s(%d) :\t TID(0x%04x) %s (Leave \t%d): %s\n"),
                    m_pszFileName,m_Line,GetCurrentThreadId(), pInfo->bufIndicate,pInfo->indent,m_pszBlkName);
                pInfo->indent--;
                if (0 == pInfo->indent)
                {
                    FTLTRACEEX(FTL::tlWarning, TEXT("%s Thread[%d] End Block Elapse Trace\n"), 
                        __FILE__LINE__, GetCurrentThreadId());
                    delete pInfo;
                    TlsSetValue(rBlockElapseTlsIndex, NULL);
                }
            }
        }
    }

//#if defined(_M_IA64) || defined(_M_IX86) || defined (_M_AMD64)
//
////#pragma data_seg(".FTL")
//#pragma section(".FTL", shared, read,write)
//	//extern "C"
//	//{
//		//__declspec(allocate(".FTL")) __declspec(selectany)   DWORD CFBlockElapse::s_dwTLSIndex = TLS_OUT_OF_INDEXES;
//		//__declspec(allocate(".FTL")) __declspec(selectany)  LONG  CFBlockElapse::s_lElapseId = 0;
//	//}
////#pragma data_seg()
//
//#if !defined(_M_IA64)
//	 #pragma comment(linker,"/SECTION:.FTL,RWS") 
////#pragma comment(linker, "/merge:FTL=.data")
//#endif
//
//#else
//	extern "C"
//	{
//		__declspec(selectany) CFBlockElapse::s_dwTLSIndex = TLS_OUT_OF_INDEXES; 
//		__declspec(selectany) CFBlockElapse::s_lElapseId = 0;
//	}
//
//#endif  // defined(_M_IA64) || defined(_M_IX86)
//    //__declspec(selectany) LONG CFBlockElapse::s_Indent = 0;
//    //__declspec(selectany) TCHAR CFBlockElapse::s_bufIndicate[MAX_TRACE_INDICATE_LEVEL + 1];
//
#endif //FTL_DEBUG

#if 0
    CModulesHolder::CModulesHolder()
    {
    }

    CModulesHolder::~CModulesHolder()
    {
        ModeulesHolderIterator iter;
        for (iter = m_allModules.begin(); iter != m_allModules.end(); iter++)
        {
            //�˲��ֵĴ������ FastTrace ���������
//#ifdef FTL_DEBUG   
//            BOOL bRet = FALSE;
//            TCHAR bufPath[MAX_PATH] = {0};
//            API_VERIFY(GetModuleFileName(*iter,bufPath,_countof(bufPath)));
//            FTLTRACE(TEXT("CModulesHolder::~CModulesHolder FreeLibrary %s\n"),bufPath);
//#endif 
            ::FreeLibrary(*iter);
        }
        m_allModules.clear();
    }

    void CModulesHolder::Erase(HMODULE hModule)
    {
        ModeulesHolderIterator iter = m_allModules.find(hModule);
        if (iter != m_allModules.end())
        {
#ifdef FTL_DEBUG
            BOOL bRet = FALSE;
            TCHAR bufPath[MAX_PATH] = {0};
            API_VERIFY(GetModuleFileName(*iter,bufPath,_countof(bufPath)));
            FAST_TRACE_EX(tlTrace, TEXT("CModulesHolder Erase %s\n"),bufPath);
#endif 
            ::FreeLibrary(*iter);
            m_allModules.erase(iter);
        }
    }
    bool CModulesHolder::Find(HMODULE hModule)
    {
        ModeulesHolderIterator iter = m_allModules.find(hModule);
        bool bFinded = (iter != m_allModules.end());
        return bFinded;
    }
    bool CModulesHolder::Insert(HMODULE hModule)
    {
        ModeulesHolderPair pr;
        pr = m_allModules.insert(hModule);
        return pr.second;
    }
#endif

    int FormatMessageBox(HWND hWnd, LPCTSTR lpCaption, UINT uType, LPCTSTR lpszFormat, ...)
    {
        CHECK_POINTER_RETURN_VALUE_IF_FAIL(lpszFormat,FALSE);
        int nRet = 0; //Zero indicates that there is not enough memory to create the message box.
        HRESULT hr = E_FAIL;
        CFStringFormater formater;

        va_list pArgs;
        va_start(pArgs, lpszFormat);

        COM_VERIFY(formater.FormatV(lpszFormat,pArgs));
        va_end(pArgs);

        if (SUCCEEDED(hr) && NULL != formater.GetString())
        {
            nRet = ::MessageBox(hWnd,formater,lpCaption,uType);
        }
        return nRet;
    }
}

#endif //FTL_BASE_HPP