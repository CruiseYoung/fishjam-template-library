#pragma once

#ifdef __cplusplus
extern "C" 
{
#endif
	//#include <ntddk.h>                      //NTʽ����(��֧��PnP)
	//#include <ntddstor.h>
	//#include <mountdev.h>
	//#include <ntddvol.h>
	#include <ntifs.h>
	#include <windef.h>
	#include <wingdi.h>

	//#include <intrin.h>

	//#include <winddi.h>
	//#include <wdm.h>                        //WDMʽ����(֧��PnP)


#ifndef FNT_VERIFY
#  define FNT_VERIFY(x)     status=(x);
#endif 

#ifdef __cplusplus
}
#endif