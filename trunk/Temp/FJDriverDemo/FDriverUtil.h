#ifndef F_DRIVER_UTIL_H
#define F_DRIVER_UTIL_H

/******************************************************************************************************************
* NT_SUCCESS  -- �жϷ���ֵ�Ƿ�ɹ�
* 
* ���ַ���(UNICODE_STRING) = RTL_CONSTANT_STRING(L"xxx")
*   �ַ�������ʱ��Rtl-����(�� RtlInitUnicodeString��)
* 
*
* DbgPrint
*   %wZ -- UNICODE_STRING �����ĵ�ַ
*
*
* PMDL -- �ڴ�����������ָ�룿ͨ������һ��������
*
* IRP -- ����(�������ܺ�Ϊ IRP_MJ_CREATE ��IRP)
*
* �ַ�����Hook���� -- 
******************************************************************************************************************/
//NT_ERROR�겻��ͬ��!NT_SUCCESS�����ʹ��!NT_SUCCESS����Ϊ���˴����⣬�����ػ񾯸���Ϣ
//if(!NT_SUCCESS(status))return status;

#ifdef __cplusplus
extern "C" {
#endif

#include "ntddk.h"


#if DBG
# define REPORT_ERROR_INFO(s, x)   \
    DbgPrint("ERROR:%d, %s", s, #x);

# define FNT_VERIFY(x)   \
    status = (x);\
    if(STATUS_SUCCESS != status)\
    {\
        REPORT_ERROR_INFO(status, x);\
    }

#else

# define FNT_VERIFY(x)   \
    status = (x);\

#endif 

LPWSTR GetNtStatusString(NTSTATUS status);


#ifdef __cplusplus
}
#endif


#endif //F_DRIVER_UTIL_H