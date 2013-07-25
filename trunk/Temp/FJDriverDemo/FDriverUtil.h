#ifndef F_DRIVER_UTIL_H
#define F_DRIVER_UTIL_H

/******************************************************************************************************************
* NT_SUCCESS  -- �жϷ���ֵ�Ƿ�ɹ������� FNT_VERIFY �����жϷ���ֵ�Ƿ���� STATUS_SUCCESS(0)
* 
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
#include "wdm.h"

#if DBG

	#pragma PAGEDCODE 
	void DumpDeviceObjectInfo(IN PDEVICE_OBJECT pDeviceObject);
	#pragma PAGEDCODE 
	void DumpDriverObjectInfo(IN PDRIVER_OBJECT pDriverObject);
	#pragma PAGEDCODE 
	void DumpDeviceFlags(USHORT nFlags);

	#pragma PAGEDCODE 
	PCHAR GetNtStatusString(NTSTATUS status);
	
	#pragma PAGEDCODE 
	PCHAR GetIrpMajorCodeString(UCHAR MajorFunction);

# define REPORT_ERROR_INFO(s, x)   \
    DbgPrint( "%s(%d) : ERROR: 0x%x(%s) for call %s\n", __FILE__, __LINE__, s, GetNtStatusString(s), #x);

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


#ifdef __cplusplus
}
#endif

#endif //F_DRIVER_UTIL_H