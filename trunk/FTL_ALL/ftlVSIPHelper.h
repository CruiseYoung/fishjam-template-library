#ifndef FTL_VSIP_HELPER_H
#define FTL_VSIP_HELPER_H

#pragma once

#ifndef FTL_BASE_H
#  error ftlVSIPHelper.h requires ftlbase.h to be included first
#endif

#ifndef FTL_COM_H
#  error ftlDTEHelper requires ftlCom.h to be included first
#endif

/********************************************************************************************

********************************************************************************************/

namespace FTL
{
    class CFVSIPUtils
    {
    public:

        //��ȡ cmdidcmd.h �ж���� CmdID ��Ӧ���ַ������� IOleCommandTarget::QueryStatus ���ж� cmds[n].cmdID
        FTLINLINE static LPCTSTR GetStdIdCommandtring(ULONG cmdID);
    };

}//namespace FTL

#ifndef USE_EXPORT
#  include "ftlVSIPHelper.hpp"
#endif

#endif //FTL_VSIP_HELPER_H
