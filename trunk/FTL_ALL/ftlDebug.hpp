#ifndef FTL_DEBUG_HPP
#define FTL_DEBUG_HPP
#pragma once

#ifdef USE_EXPORT
#  include "ftlDebug.h"
#endif

namespace FTL
{
	CFRectDumpInfo::CFRectDumpInfo(const RECT& rect) 
		: CFConvertInfoT<CFRectDumpInfo, const RECT&, 64>(rect)
	{
	}

	LPCTSTR CFRectDumpInfo::ConvertInfo()
	{
		if (NULL == m_bufInfo[0])
		{
			StringCchPrintf(m_bufInfo,_countof(m_bufInfo),TEXT("(%d,%d)-(%d,%d), %dx%d"),
				m_Info.left, m_Info.top, m_Info.right, m_Info.bottom,
				m_Info.right - m_Info.left, m_Info.bottom - m_Info.top);
		}
		return m_bufInfo;
	}

	CFFILETIMEDumpInfo::CFFILETIMEDumpInfo(const FILETIME& fileTime) 
		: CFConvertInfoT<CFFILETIMEDumpInfo, const FILETIME&, 64>(fileTime)
	{
	}

	LPCTSTR CFFILETIMEDumpInfo::ConvertInfo()
	{
		if (NULL == m_bufInfo[0])
		{
			BOOL bRet = FALSE;
			SYSTEMTIME systemTime = {0};
			API_VERIFY(FileTimeToSystemTime(&m_Info, &systemTime));
			StringCchPrintf(m_bufInfo,_countof(m_bufInfo),TEXT("0x%x:0x%x, %4d-%02d-%02d %02d:%02d:%02d:%03d"),
				m_Info.dwHighDateTime, m_Info.dwLowDateTime,
				systemTime.wYear, systemTime.wMonth, systemTime.wDay,
				systemTime.wHour, systemTime.wMinute, systemTime.wSecond, systemTime.wMilliseconds);
		}
		return m_bufInfo;
	}
}//FTL

#endif //FTL_DEBUG_HPP