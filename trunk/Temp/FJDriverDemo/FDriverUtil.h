#ifndef DRIVER_UTIL_H
#define DRIVER_UTIL_H

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


//#if DBG
//# define REPORT_ERROR_INFO(s, x)   \
//    DbgPrint("ERROR:%d, %s", s, #x);
//
//# define NT_VERIFY(x)   \
//    status = (x);\
//    if(STATUS_SUCCESS != status)\
//    {\
//        REPORT_ERROR_INFO(status, x);\
//    }
//
//#else
//
//# define NT_VERIFY(x)   \
//    nRet = (x);\
//
//#endif 


#endif //DRIVER_UTIL_H