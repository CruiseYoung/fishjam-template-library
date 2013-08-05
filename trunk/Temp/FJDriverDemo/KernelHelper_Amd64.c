#include "stdafx.h"
#include "KernelHelper_Amd64.h"

KIRQL ClearWriteProtect()
{
	KIRQL irql= KeRaiseIrqlToDpcLevel();  //KeGetCurrentIrql();
	UINT64 cr0=__readcr0();
	cr0 &= ~0x10000; //0xfffffffffffeffff;
	__writecr0(cr0);
	_disable();
	return irql;
}

void SetWriteProtect(KIRQL irql)
{
	UINT64 cr0=__readcr0();
	cr0 |= 0x10000;
	_enable();
	__writecr0(cr0);
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
				return (SYSTEM_SERVICE_TABLE*)addr;
			}
		}
	}
	return NULL;
}

PVOID GetShadowSSDTFuncAddr(PSYSTEM_SERVICE_TABLE pServiceTable, int nIndex)
{
	PBYTE				W32pServiceTable=0, qwTemp=0;
	LONG 					dwTemp=0;
	PSYSTEM_SERVICE_TABLE	pWin32k = &pServiceTable[1];
	W32pServiceTable=pWin32k->ServiceTableBase;
	//ul64W32pServiceTable = W32pServiceTable;
	qwTemp = W32pServiceTable + 4 * nIndex;	//�����ǻ��ƫ�Ƶ�ַ��λ�ã�ҪHOOK�Ļ��޸����Ｔ��
	dwTemp = *(PLONG)qwTemp;
	dwTemp = dwTemp >> 4;
	qwTemp = W32pServiceTable + (LONG64)dwTemp;
	return qwTemp;
}

NTSTATUS ModifyShadowSSDTFunc(PSYSTEM_SERVICE_TABLE pServiceTable, int nIndex, PVOID newAddress)
{
	NTSTATUS status = STATUS_SUCCESS;
	KIRQL irql;
	UCHAR jmp_code[]="\xFF\x25\x00\x00\x00\x00\x90\x90\x90\x90\x90\x90\x90\x90";	//��Ҫ14�ֽ�+4�ֽڣ�xor rax,rax + ret��
	UCHAR fuckcode[]="\x48\x33\xC0\xC3\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90";
	ULONG64 pOldAddress = NULL;
	ULONG64 myfun = 0;
	ULONGLONG				W32pServiceTable=0, qwTemp=0;
	LONG 					dwTemp=0;
	PSYSTEM_SERVICE_TABLE	pWin32k = &pServiceTable[1];

	pOldAddress = GetShadowSSDTFuncAddr(pServiceTable, nIndex);

	//��������ַ
	myfun =(ULONG64)newAddress;
	//���shellcode
	memcpy(jmp_code+6,&myfun,8);
	//д��shellcode
	
	irql=ClearWriteProtect();

	memcpy((PVOID)pOldAddress,fuckcode,23);	//����23���ֽڡ�����ָ�������ԡ�
	memcpy((PVOID)(pOldAddress+4),jmp_code,14);
	//�޸ļ�¼ԭʼ��ַ�ĵط���[AddressNtUserWindowFromPhysicalPoint+4]��ʼ��һ��long jmp����ת��ProxyNtUserPostMessage
	//ModifySSSDT(IndexOfNtUserPostMessage, AddressNtUserWindowFromPhysicalPoint+4);

	pWin32k = (PSYSTEM_SERVICE_TABLE)((ULONG64)pServiceTable + sizeof(SYSTEM_SERVICE_TABLE));	//4*8
	W32pServiceTable=(ULONGLONG)(pWin32k->ServiceTableBase);
	qwTemp = W32pServiceTable + 4 * nIndex;
	dwTemp = (LONG)((ULONG64)(pOldAddress + 4) - W32pServiceTable);
	dwTemp = dwTemp << 4;	//DbgPrint("*(PLONG)qwTemp: %x, dwTemp: %x",*(PLONG)qwTemp,dwTemp);

	*(PLONG)qwTemp = dwTemp;

	SetWriteProtect(irql);

	return status;
}
