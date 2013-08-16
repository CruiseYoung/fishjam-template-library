#include "stdafx.h"
#include "FDriverUtil.h"
#include "KernelHookAPI.h"
#include "KernelHelper.h"

#define _M_AMD64

#if defined(_M_AMD64)


PSYSTEM_SERVICE_TABLE	g_pSystemServiceTable= NULL;

KIRQL WPOFFx64()
{
	KIRQL irql = KeRaiseIrqlToDpcLevel();  //KeGetCurrentIrql();
	UINT64 cr0;
	_disable();
	cr0 = __readcr0();
	cr0 &= ~0x10000; //0xfffffffffffeffff;
	__writecr0(cr0);
	//_disable(); //��Ҫע��?
	return irql;
}

void WPONx64(KIRQL irql)
{
	UINT64 cr0=__readcr0();
	cr0 |= 0x10000;
	//_enable();
	__writecr0(cr0);
	_enable();
	KeLowerIrql(irql);
}


SYSTEM_SERVICE_TABLE* GetKeServiceDescriptorTableShadowAddress()  //GetKeServiceDescriptorTableShadow64()
{
	PUCHAR StartSearchAddress = (PUCHAR)__readmsr(0xC0000082);
	PUCHAR EndSearchAddress = StartSearchAddress + 0x500;
	PUCHAR i = NULL;
	UCHAR b1=0,b2=0,b3=0;
	ULONG templong=0;
	ULONGLONG addr=0;

	if (g_pSystemServiceTable)
	{
		return g_pSystemServiceTable;
	}

	for(i=StartSearchAddress;i<EndSearchAddress;i++)
	{
		if( MmIsAddressValid(i) && MmIsAddressValid(i+1) && MmIsAddressValid(i+2) )
		{
			b1=*i;
			b2=*(i+1);
			b3=*(i+2);
			if( b1==0x4c && b2==0x8d && b3==0x1d ) //4c8d1d
			{
				memcpy(&templong,i+3,4);
				addr = (ULONGLONG)templong + (ULONGLONG)i + 7;
				break;
			}
		}
	}
	g_pSystemServiceTable = addr;

	if (g_pSystemServiceTable)
	{
		KdPrint(("g_pSystemServiceTable : %#x, SSDT EntryCount=%d, Shadow EntryCount=%d\n", 
			g_pSystemServiceTable, g_pSystemServiceTable[0].NumberOfServices, 
			g_pSystemServiceTable[1].NumberOfServices));
	}
	else
	{
		KdPrint(("!!! Find Shadow SSDT Failed\n"));
	}

	return g_pSystemServiceTable; 
}

PVOID GetSSDTFuncAddr(LONG nServiceIndex)
{
	//ʹ�� 7 ���ֽڱ�������� g_pSystemServiceTable[1].ServiceTableBase �ĵ�ַƫ��

	PBYTE				W32pServiceTable=0, qwTemp=0;
	LONG 					dwTemp=0;
	PSYSTEM_SERVICE_TABLE pServiceTable = nServiceIndex < 0x1000 ? &g_pSystemServiceTable[0] : &g_pSystemServiceTable[1];

	LONG nIndex = nServiceIndex;
	if (nIndex > 0x1000)
	{
		nIndex -= 0x1000;
	}

	W32pServiceTable=(PBYTE)pServiceTable->ServiceTableBase;

	qwTemp = W32pServiceTable + 4 * nIndex;	//�����ǻ��ƫ�Ƶ�ַ��λ�ã�ҪHOOK�Ļ��޸����Ｔ��
	dwTemp = *(PLONG)qwTemp;
	dwTemp = dwTemp >> 4;
	qwTemp = W32pServiceTable + (LONG64)dwTemp;
	return qwTemp;
}

//ULONG GetSSDTOffsetAddress(ULONGLONG FuncAddr)
//{
//	ULONG dwtmp=0;
//	PULONG ServiceTableBase=NULL;
//	ServiceTableBase=(PULONG)KeServiceDescriptorTable->ServiceTableBase;
//	dwtmp=(ULONG)(FuncAddr-(ULONGLONG)ServiceTableBase);
//	return dwtmp<<4;
//}


//���룺��HOOK������ַ����������ַ������ԭʼ������ַ��ָ�룬���ղ������ȵ�ָ�룻���أ�ԭ��ͷN�ֽڵ�����
#if 0
PVOID HookKernelApi(IN PVOID ApiAddress, IN PVOID Proxy_ApiAddress, OUT PVOID *Original_ApiAddress, OUT ULONG *PatchSize)
{
	KIRQL irql;
	UINT64 tmpv;
	PVOID head_n_byte,ori_func;
	UCHAR jmp_code[]="\xFF\x25\x00\x00\x00\x00\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF";
	UCHAR jmp_code_orifunc[]="\xFF\x25\x00\x00\x00\x00\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF";
	//How many bytes shoule be patch
	*PatchSize= 14; //GetPatchSize((PUCHAR)ApiAddress);
	//step 1: Read current data
	head_n_byte=kmalloc(*PatchSize);
	irql=WPOFFx64();
	memcpy(head_n_byte,ApiAddress,*PatchSize);
	WPONx64(irql);
	//step 2: Create ori function
	ori_func=kmalloc(*PatchSize+14);	//ԭʼ������+��ת������
	RtlFillMemory(ori_func,*PatchSize+14,0x90);
	tmpv=(ULONG64)ApiAddress+*PatchSize;	//��ת��û���򲹶����Ǹ��ֽ�
	memcpy(jmp_code_orifunc+6,&tmpv,8);
	memcpy((PUCHAR)ori_func,head_n_byte,*PatchSize);
	memcpy((PUCHAR)ori_func+*PatchSize,jmp_code_orifunc,14);
	*Original_ApiAddress=ori_func;
	//step 3: fill jmp code
	tmpv=(UINT64)Proxy_ApiAddress;
	memcpy(jmp_code+6,&tmpv,8);
	//step 4: Fill NOP and hook
	irql=WPOFFx64();
	RtlFillMemory(ApiAddress,*PatchSize,0x90);
	memcpy(ApiAddress,jmp_code,14);
	WPONx64(irql);
	//return ori code
	return head_n_byte;
}
#endif

/*
 -- �������trampoline��ʱ��ͨ��VirtualAlloc�޶���������ڴ��old_func��ͬһ��4G���ɽ����ƽ���Ϊ 5���ֽ�(2+4)
1���ҵ���Ҫhook�ĺ�����ַ
2�������Ӻ�����ʼ��ַ��ʼ������6+8=14���ֽڵĴ��롣���벻�ܶϿ�������2�����̺�detourx86һ������ͬ���ǣ�detoursx86֮��Ҫe9 xxxxxxxx��Ҳ����˵ֻ��Ҫ5���ֽڣ������Ǳ�����ff15 [xxxxxxxx]�����������С��14���ֽڣ�����ζ�Ÿú����޷�detours��
����������С��14�ֽڶ������Ϊ����ִ����һ��call����jmp����ô�����ô��룬�Ѻ�����ʼ��ַ����Ϊjmp֮��ĵ�ַ�����½���2���̡�
3������14����15��16...���ֽڿ�����Ԥ�ȷ����һ���ڴ��У����ǽ���trampoline��
4����ǰ6���ֽڸ�Ϊff15 [0]��Ҳ��ff15 00000000
5��������8���ֽ��б���new_function����ʼ��ַ
6������trampoline�е�14�ֽڵĴ��룬���������jmp��call����ת��䣬�޸�ƫ��������ʱ��ͨ������Ҫ��4G����ת����ô��������ķ����޸�֮��trampoline���ֽ������ܻ����ӡ�
7����trampoline�Ĵ���֮�󣬲���ff15 [0]������������8���ֽ������old_function+14��
*/

NTSTATUS HookSSDTFunc(PHOOK_API_INFO pHookApiInfo)
{
	
	NTSTATUS status = STATUS_SUCCESS;
	KIRQL irql = 0;

	pHookApiInfo->pTargetAddress = GetSSDTFuncAddr(pHookApiInfo->nIndexInSSDT);
	NT_ASSERT(pHookApiInfo->pTargetAddress);

	FNT_VERIFY(CreateInlineHook(pHookApiInfo->pTargetAddress, pHookApiInfo->pNewApiAddress, &pHookApiInfo->pOrigApiAddress,
		&pHookApiInfo->pInlineHookInfo));

	KdPrint(("Inline Hook SSDT func %ws at [0x%x], TargetAddress=%p, newAddress=%p, OrigiApiAddress=%p, nParamCount=%d\n", 
		pHookApiInfo->pwzApiName, pHookApiInfo->nIndexInSSDT, 
		pHookApiInfo->pTargetAddress, pHookApiInfo->pNewApiAddress, 
		pHookApiInfo->pOrigApiAddress, pHookApiInfo->nParamCount));

	return status;
}	

NTSTATUS RestoreSSDTFunc(PHOOK_API_INFO pHookApiInfo)
{
	pHookApiInfo->pOrigApiAddress = NULL;
	RestoreInlineHook(pHookApiInfo->pInlineHookInfo);
	pHookApiInfo->pInlineHookInfo = NULL;
	return STATUS_SUCCESS;
}

#if 0
NTSTATUS HookSSDTFunc(PSYSTEM_SERVICE_TABLE pServiceTable, int nIndex, PVOID newAddress)
{
	NTSTATUS status = STATUS_SUCCESS;
	KIRQL irql;
	//eb93 -- jmp
	UCHAR jmp_code[]="\xFF\x25\x00\x00\x00\x00\x90\x90\x90\x90\x90\x90\x90\x90";	//��Ҫ14�ֽ�+4�ֽڣ�xor rax,rax + ret��
	
	UCHAR fuckcode[]="\x48\x33\xC0\xC3\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90";

	ULONG64 OldAddress = NULL;
	//��������ַ
	ULONG64 myfun = (ULONG64)newAddress;
	ULONGLONG				W32pServiceTable=0, qwTemp=0;
	LONG 					dwTemp=0;
	PSYSTEM_SERVICE_TABLE	pWin32k = &pServiceTable[1];

	OldAddress = GetSSDTFuncAddr(pServiceTable, nIndex);

	//���shellcode
	memcpy(jmp_code+6,&myfun,8);
	//д��shellcode
	
	irql=WPOFFx64();

	memcpy((PVOID)OldAddress,fuckcode,23);	//����23���ֽڡ�����ָ�������ԡ�
	memcpy((PVOID)(OldAddress+4),jmp_code,14);
	//�޸ļ�¼ԭʼ��ַ�ĵط���[AddressNtUserWindowFromPhysicalPoint+4]��ʼ��һ��long jmp����ת��ProxyNtUserPostMessage
	//ModifySSSDT(IndexOfNtUserPostMessage, AddressNtUserWindowFromPhysicalPoint+4);

	//pWin32k = (PSYSTEM_SERVICE_TABLE)((ULONG64)pServiceTable + sizeof(SYSTEM_SERVICE_TABLE));	//4*8
	W32pServiceTable=(ULONGLONG)(pWin32k->ServiceTableBase);
	qwTemp = W32pServiceTable + 4 * nIndex;
	dwTemp = (LONG)((ULONG64)(OldAddress + 4) - W32pServiceTable);
	dwTemp = dwTemp << 4;	
	DbgPrint("*(PLONG)qwTemp: %x, dwTemp: %x",*(PLONG)qwTemp,dwTemp);

	*(PLONG)qwTemp = dwTemp;

	WPONx64(irql);

	return status;
}

#endif //0

#endif //_M_AMD64