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
}

#endif //FTL_DEBUG_H

#ifndef USE_EXPORT
#  include "ftlDebug.hpp"
#endif
