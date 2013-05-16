#ifndef FTL_DEBUG_H
#define FTL_DEBUG_H

#pragma once

namespace FTL
{
	/*********************************************************************************************************
	* Windbg
	* ����
	*   dds ��ַ -- ����ʾָ��λ�õĺ�����?
	*   lmm -- list module match
	*   u ��ַ -- �����
	*********************************************************************************************************/

    /*********************************************************************************************************
    * ��Release�汾�����öϵ�: _asm int 3
	*   
	* ʹ�� TRACE/ATLTRACE ��ӡ�������պ�������ʱ�����ܻᱨ" _CrtDbgReport: String too long or IO Error "�Ĵ��󣬶��޷������־��
	*   ԭ��: wprintf_s ������ȷ������պ��� UNICODE ����(�ƺ�VS2010���޸������Bug��)
	*   ��������(��δ��ϸ�о�)��http://www.cnblogs.com/hnrainll/archive/2011/05/07/2039700.html
	*   ���������
	*     1.ʹ��FTLTRACE(�ڲ�ʹ�� StringCchVPrintfEx + OutputDebugString )
	*     2.#include <locale.h>
	*       char* old_locale = _strdup( setlocale(LC_ALL,NULL) );
	*       setlocale( LC_ALL, "chs" );    �� "", "korean"
	*       TRACE(XXXX);
	*       setlocale( LC_ALL, old_locale); 
	*       free(old_locale); 
    *********************************************************************************************************/

    //printf �� Ctrl Code -- printf("\033[30m" "�������Ʒ�" "\033[0m" , �����б�)
    #define CTRL_COLOR_RESET        TEXT("\033[0m")

    #define CTRL_COLOR_BOLD         TEXT("\033[1m")
    #define CTRL_COLOR_GREY         TEXT("\033[5m")
    #define CTRL_COLOR_UNDERLINE    TEXT("\033[4m")
    #define CTRL_COLOR_BLINK        TEXT("\033[5m")
    #define CTRL_COLOR_BLACKBLOCK   TEXT("\033[7m")
    #define CTRL_COLOR_DELETELINE   TEXT("\033[9m")

    #define CTRL_COLOR_BLACK        TEXT("\033[30m")
    #define CTRL_COLOR_RED          TEXT("\033[31m")
    #define CTRL_COLOR_GREEN        TEXT("\033[32m")
    #define CTRL_COLOR_YELLOW       TEXT("\033[33m")
    #define CTRL_COLOR_BLUE         TEXT("\033[34m")
    #define CTRL_COLOR_MAGENTA      TEXT("\033[35m")
    #define CTRL_COLOR_CYAN         TEXT("\033[36m")


	FTLEXPORT class CFRectDumpInfo : public CFConvertInfoT<CFRectDumpInfo, const RECT&, 64>
	{
		DISABLE_COPY_AND_ASSIGNMENT(CFRectDumpInfo);
	public:
		FTLINLINE explicit CFRectDumpInfo(const RECT& rect);
		FTLINLINE virtual LPCTSTR ConvertInfo();
	};

	FTLEXPORT class CFFileTimeDumpInfo : public CFConvertInfoT<CFFileTimeDumpInfo, const FILETIME&, 64>
	{
		DISABLE_COPY_AND_ASSIGNMENT(CFFileTimeDumpInfo);
	public:
		FTLINLINE explicit CFFileTimeDumpInfo(const FILETIME& fileTime);
		FTLINLINE virtual LPCTSTR ConvertInfo();
	};

	//˫��ʾ��ʱ: dwFlags = 01(MONITORINFOF_PRIMARY) ������
	//  rcMonitor ��ʾ������ʾ����
	//  rcWork    �ų�������������
	//��������ߵ�����(�·���������)��cbSize= 40,dwFlags=0x1, rcMonitor=(0,0)-(1440,900), 1440x900, rcWork=(0,0)-(1440,869), 1440x869
	//�������ұߵĴ��ԣ�              cbSize= 40,dwFlags=0x0, rcMonitor=(1440,-124)-(2720,900), 1280x1024, rcWork=(1440,-124)-(2720,900), 1280x1024

	FTLEXPORT class CFMonitorInfoDumpInfo: public CFConvertInfoT<CFMonitorInfoDumpInfo, const MONITORINFO &>
	{
		DISABLE_COPY_AND_ASSIGNMENT(CFMonitorInfoDumpInfo);
	public:
		FTLINLINE explicit CFMonitorInfoDumpInfo(const MONITORINFO& monitorInfo);
		FTLINLINE virtual LPCTSTR ConvertInfo();
	};
}

#endif //FTL_DEBUG_H

#ifndef USE_EXPORT
#  include "ftlDebug.hpp"
#endif
