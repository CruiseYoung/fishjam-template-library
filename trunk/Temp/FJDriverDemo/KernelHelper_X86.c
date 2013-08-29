#include "stdafx.h"
#ifdef FDRIVER_DEMO_PROJECT
#  include "FDriverUtil.h"
#endif 
#include "KernelHookAPI.h"
#include "KernelHelper.h"
#include "HookApiUtil.h"

#if defined(_M_IX86)

PSYSTEM_SERVICE_TABLE	g_pSystemServiceTable= NULL;

typedef struct _SERVICE_DESCRIPTOR_TABLE *PSERVICE_DESCRIPTOR_TABLE;

extern PSERVICE_DESCRIPTOR_TABLE KeServiceDescriptorTable;
extern __declspec(dllimport) KeAddSystemServiceTable(ULONG, ULONG, ULONG, ULONG, ULONG); 

//������ͬ�汾����SSDT��ַ���ڵ�ƫ�ƻ�ȡ��SSDT SHADOW�ĵ�ַ
// WinDbg �� ?KeServiceDescriptorTable-
// ���ӡ����Evaluate expression: 64 = 00000040(XP), -0xE0(2K)
//����˹���и����Ƶĺ�����FindSystemServiceDescriptionTableShadow

//KeServiceDescriptorTable -- ������ SSDT,(�� Shadow SSDT ������NULL)
//KeServiceDescriptorTableShadow -- ������ SSDT + Shadow SSDT�����GUI�̲߳���ת��Ҳ�ܵ��� SSDT �еĺ���
SYSTEM_SERVICE_TABLE *GetKeServiceDescriptorTableShadowAddress ()
{ 
    //32λϵͳ�е�����KeServiceDescriptorTable������
    //����������δ������ KeServiceDescriptorTableShadow, ��ǰ���ֵ����ݺ� KeServiceDescriptorTable ����һ��
    //ͨ���� KeAddSystemServiceTable ����������ʼ���������Ƚ��ڴ��е����ݣ��Ӷ��ҵ� KeServiceDescriptorTableShadow��
    //ͨ������ ����SSDT�ĺ���ʵ���е���Ч�ڴ��ַ�İ취 ������ Shadow SSDT
    //TODO: ��ȡ�� KeServiceDescriptorTable ���������ĵ�ַ������ KeServiceDescriptorTable[1] ������ KeServiceDescriptorTableShadow �ĵ�ַ�ˣ�

    // First, obtain a pointer to KeAddSystemServiceTable
    unsigned char *check = (unsigned char*)KeAddSystemServiceTable; 
    int i;
    //Initialize an instance of System Service Table, will be used to
    //obtain an address from KeAddSystemServiceTable
    SYSTEM_SERVICE_TABLE *rc=0; 

    if (g_pSystemServiceTable)
    {
        return g_pSystemServiceTable;
    }

    // Make 100 attempts to match a valid address with that of KeServiceDescriptorTable 
    for (i=0; i<4096; i++) {  //PAGE_SIZE
        __try { 
            // try to obtain an address from  KeAddSystemServiceTable 
            rc = *(SYSTEM_SERVICE_TABLE **)check; 
            // if this address is NOT valid OR it itself is the address of 
            //KeServiceDescriptorTable OR its first entry is NOT equal 
            //to the first entry of KeServiceDescriptorTable 
            if (!MmIsAddressValid (rc) 
                || (rc == (SYSTEM_SERVICE_TABLE *)KeServiceDescriptorTable) 
                || (memcmp (rc, KeServiceDescriptorTable, sizeof (*rc)) != 0)) { 
                    // Proceed with the next address 
                    check++; 
                    // don't forget to reset the old address 
                    rc = 0; 
            } 
        } __except (EXCEPTION_EXECUTE_HANDLER) 
        {
            rc = 0; 
        } 
        // when the loop is completed, check if it produced a valid address 
        if (rc) 
        {
            // because if it didn't, we failed to find the address of KeServiceDescriptorTableShadow 
            break; 
        }
    } 
    // otherwise, there is a valid address! So return it! 

    g_pSystemServiceTable = rc;

    if (g_pSystemServiceTable)
    {
        KdPrint(("g_pSystemServiceTable : %#x, SSDT EntryCount=%d, Shadow EntryCount=%d\n", 
            g_pSystemServiceTable, g_pSystemServiceTable[0].NumberOfServices, 
            g_pSystemServiceTable[1].NumberOfServices));
    }
    else
    {
        KdPrint(("!!! Find Shadow SSDT Failed from KeAddSystemServiceTable=%p\n", KeAddSystemServiceTable));
    }

    return g_pSystemServiceTable; 
}


PVOID GetSSDTFuncAddr(LONG nServiceIndex)
{
    PSYSTEM_SERVICE_TABLE pServiceTable = nServiceIndex < 0x1000 ? &g_pSystemServiceTable[0] : &g_pSystemServiceTable[1];
    LONG nIndex = nServiceIndex;
    if (nIndex > 0x1000)
    {
        nIndex -= 0x1000;
    }

    return pServiceTable->ServiceTableBase[nIndex];
}

NTSTATUS HookSSDTFunc(PHOOK_API_INFO pHookApiInfo)
{
    NTSTATUS status = STATUS_UNSUCCESSFUL;
    ULONG oldProtect = 0;
    
    PSYSTEM_SERVICE_TABLE pServiceTable = pHookApiInfo->nIndexInSSDT < 0x1000 ? &g_pSystemServiceTable[0] : &g_pSystemServiceTable[1];
    LONG nIndex = pHookApiInfo->nIndexInSSDT;
    if (nIndex > 0x1000)
    {
        nIndex -= 0x1000;
    }

    NT_ASSERT(NULL == pHookApiInfo->pOrigApiAddress);
    NT_ASSERT((LONG)pServiceTable->ArgumentsTable[nIndex] == pHookApiInfo->nParamCount * sizeof(PVOID));	//�жϲ����ĸ���

    pHookApiInfo->pOrigApiAddress = pServiceTable->ServiceTableBase[nIndex];

    KdPrint(("Hook SSDT func %ws at [0x%x], oldAddress=%p, newAddress=%p, nParamCount=%d\n", 
        pHookApiInfo->pwzApiName, pHookApiInfo->nIndexInSSDT, 
        pHookApiInfo->pOrigApiAddress, pHookApiInfo->pNewApiAddress, pHookApiInfo->nParamCount));

#ifdef USE_INLINE_HOOK
    pHookApiInfo->pTargetAddress = GetSSDTFuncAddr(pHookApiInfo->nIndexInSSDT);
    NT_ASSERT(pHookApiInfo->pTargetAddress);
    if(CreateInlineHook(&(PVOID&)pHookApiInfo->pTargetAddress, pHookApiInfo->pNewApiAddress, &pHookApiInfo->pOrigApiAddress,
        &pHookApiInfo->pInlineHookInfo))
    {
        status = STATUS_SUCCESS;
    }
    KdPrint(("Inline Hook SSDT func %ws at [0x%x], TargetAddress=%p, newAddress=%p, OrigiApiAddress=%p, nParamCount=%d\n", 
        pHookApiInfo->pwzApiName, pHookApiInfo->nIndexInSSDT, 
        pHookApiInfo->pTargetAddress, pHookApiInfo->pNewApiAddress, 
        pHookApiInfo->pOrigApiAddress, pHookApiInfo->nParamCount));
#else
    oldProtect = ClearWriteProtect(pServiceTable->ServiceTableBase[nIndex], sizeof(PVOID));
    InterlockedExchangePointer(&(pServiceTable->ServiceTableBase[nIndex]), pHookApiInfo->pNewApiAddress);
    RestoreWriteProtect(pServiceTable->ServiceTableBase[nIndex], sizeof(PVOID), oldProtect);
    status = STATUS_SUCCESS;
#endif 

    return status;
}

NTSTATUS RestoreSSDTFunc(PHOOK_API_INFO pHookApiInfo)
{
    NTSTATUS status = STATUS_UNSUCCESSFUL;
    ULONG oldProtect = 0;

    PSYSTEM_SERVICE_TABLE pServiceTable = pHookApiInfo->nIndexInSSDT < 0x1000 ? &g_pSystemServiceTable[0] : &g_pSystemServiceTable[1];
    LONG nIndex = pHookApiInfo->nIndexInSSDT;
    if (nIndex > 0x1000)
    {
        nIndex -= 0x1000;
    }

    KdPrint(("Restore SSDT func %ws at [0x%x], origAddress=%p\n", 
        pHookApiInfo->pwzApiName, pHookApiInfo->nIndexInSSDT, pHookApiInfo->pOrigApiAddress));

    NT_ASSERT(NULL != pHookApiInfo->pOrigApiAddress);

    //KeAcquireSpinLock(&pHookApiInfo->spinLock, &irql1);

#ifdef USE_INLINE_HOOK
    KdPrint(("Restore Inline SSDT func %ws at [0x%x], TargetAddress=%p, newAddress=%p, OrigiApiAddress=%p\n", 
        pHookApiInfo->pwzApiName, pHookApiInfo->nIndexInSSDT, pHookApiInfo->pTargetAddress, pHookApiInfo->pNewApiAddress, 
        pHookApiInfo->pOrigApiAddress));
    if(RestoreInlineHook(pHookApiInfo->pInlineHookInfo))
    {
        status = STATUS_SUCCESS;
    }
    pHookApiInfo->pInlineHookInfo = NULL;
#else
    oldProtect = ClearWriteProtect(pServiceTable->ServiceTableBase[nIndex], sizeof(PVOID));
    InterlockedExchangePointer(&(pServiceTable->ServiceTableBase[nIndex]), pHookApiInfo->pOrigApiAddress);
    RestoreWriteProtect(pServiceTable->ServiceTableBase[nIndex], sizeof(PVOID), oldProtect);
    status = STATUS_SUCCESS;
#endif 

    pHookApiInfo->pOrigApiAddress = NULL;

    return status;
}

#endif //_M_IX86