#pragma once

#ifdef FDRIVER_DEMO_PROJECT
#  include "FDriverDemoDefine.h"
#else
#  include "nComicDrvDefine.h"
#endif

#include "InlineHook.h"

#define SSDT_API_CALL_ENTER(x) (InterlockedIncrement(&x))
#define SSDT_API_CALL_LEAVE(x) (InterlockedDecrement(&x))


//SSDT��ṹ -- 32/64 ͨ�ã� func = (PBYTE)Base+Base[index];
typedef struct _SYSTEM_SERVICE_TABLE { 
	// PNTPROC ServiceTable;
	PVOID* ServiceTableBase;				//SSDT���ڴ��еĻ���ַ(���飬��ÿһ����Ԫ�ؾ��Ǹ���Rin0�ĺ���)
	PULONG CounterTable;					// array of usage counters, ������ SSDT ��ÿ�����񱻵��ô��������������顣���������һ����sysenter ����
	SIZE_T NumberOfServices;			    //SSDT��ĸ�����������32(ֱ�ӱ����ַ)����64(ǰ 3.5 ���ֽڱ����ַ��ƫ��)Ϊϵͳ��ÿ���4���ֽڣ������ * 4 ����������ַ��Ĵ�С
	PBYTE ArgumentsTable;					//TODO:ʵ�����ǲ�������(ParamTableBase)? ����ÿ��ϵͳ��������ֽ�����Ļ���ַ-ϵͳ���������
} SYSTEM_SERVICE_TABLE, *PSYSTEM_SERVICE_TABLE;

//ϵͳ�е�4��SSDT��
typedef struct _SERVICE_DESCRIPTOR_TABLE
{
	SYSTEM_SERVICE_TABLE ntoskrnl;  // ntoskrnl.exe(native api���� SSDT, KeServiceDescriptorTable )
	SYSTEM_SERVICE_TABLE win32k;    // win32k.sys (gdi/user support, shadow SSDT) -- GUI �����в��У�CUI ������û�У�
	SYSTEM_SERVICE_TABLE Table3;    // not used
	SYSTEM_SERVICE_TABLE Table4;    // not used
}SYSTEM_DESCRIPTOR_TABLE, *PSYSTEM_DESCRIPTOR_TABLE;

//#pragma pack(push, 8)
typedef struct _HOOK_API_INFO
{
	//��Hook����ִ�й�����pOrig���ܱ��滻,��Ҫ�Ƚ� pOrigApiAddress �������ʱ�������жϲ�Ϊ�պ�ʹ����ʱ����
	LPWSTR  pwzApiName;
	PVOID   pTargetAddress;			
	PVOID   pNewApiAddress;
	PVOID	pOrigApiAddress;		//x86��Shadow SSDT�еĺ�����amd64��ԭʼ����
	LONG	nIndexInSSDT;
	LONG    nParamCount;
	BOOL	bEnableHook;

	//KSPIN_LOCK	spinLock;		//LONG	nAPICallCount;
	//LONG    nPatchSize;
	PINLINE_HOOK_INFO	pInlineHookInfo;
}HOOK_API_INFO, *PHOOK_API_INFO;
//#pragma pack(pop)


#ifdef __cplusplus
extern "C" {
#endif

NTSTATUS InstallCopyProtectHook(HANDLE hTargetProcess);
NTSTATUS UpdateCopyProtectWndInfo(PPROTECT_WND_INFO pProtectWndInfo);
NTSTATUS UnInstallCopyProtectHook(void);

#ifdef __cplusplus
}
#endif
