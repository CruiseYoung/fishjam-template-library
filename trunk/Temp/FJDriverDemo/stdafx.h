#ifndef _WIN32_WINNT		// Allow use of features specific to Windows XP or later.                   
#define _WIN32_WINNT 0x0501	// Change this to the appropriate value to target other versions of Windows.
#endif						

#ifdef __cplusplus
extern "C" 
{
#endif

#include "VisualDDKHelpers.h"

//#include <ntddk.h>                      //NTʽ����(��֧��PnP)
//#include <ntddstor.h>
//#include <mountdev.h>
//#include <ntddvol.h>
#include <ntifs.h>

//#include <wdm.h>                        //WDMʽ����(֧��PnP)

#ifdef __cplusplus
}
#endif

#ifndef _countof
#  define _countof(p)       (sizeof(p)/sizeof((p)[0]))
#endif 