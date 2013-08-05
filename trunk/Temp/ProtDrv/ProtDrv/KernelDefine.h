#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <ntddk.h>
#include <windef.h>

#define SSDT_API_CALL_ENTER(x) (InterlockedIncrement(&x))
#define SSDT_API_CALL_LEAVE(x) (InterlockedDecrement(&x))


#pragma push(0)

//SSDT��ṹ
typedef struct _SYSTEM_SERVICE_TABLE { 
    // PNTPROC ServiceTable;
    PVOID* ServiceTableBase;					//SSDT���ڴ��еĻ���ַ(���飬��ÿһ����Ԫ�ؾ��Ǹ���Rin0�ĺ���)
    PULONG CounterTable;					// array of usage counters, ������ SSDT ��ÿ�����񱻵��ô��������������顣���������һ����sysenter ����
    //ULONG NumberOfServices;				    //SSDT��ĸ���
    SIZE_T NumberOfServices;				    //SSDT��ĸ���
    PBYTE ArgumentsTable;					//TODO:ʵ�����ǲ�������(ParamTableBase)? ����ÿ��ϵͳ��������ֽ�����Ļ���ַ-ϵͳ���������
} SYSTEM_SERVICE_TABLE, *PSYSTEM_SERVICE_TABLE;

//ϵͳ�е�4��SSDT��
typedef struct _SERVICE_DESCRIPTOR_TABLE
{
    SYSTEM_SERVICE_TABLE ntoskrnl;  // ntoskrnl.exe(native api���� SSDT, KeServiceDescriptorTable )
    SYSTEM_SERVICE_TABLE win32k;    // win32k.sys (gdi/user support, shadow SSDT)
    SYSTEM_SERVICE_TABLE Table3;    // not used
    SYSTEM_SERVICE_TABLE Table4;    // not used
}SYSTEM_DESCRIPTOR_TABLE, *PSYSTEM_DESCRIPTOR_TABLE;


#ifdef __cplusplus
}
#endif
