#include "stdafx.h"
#include "FDriverMemory.h"
#include "FDriverDemoDefine.h"
#include "FDriverUtil.h"
#include "FDriverHookAPI.h"
#include "KernelHookAPI.h"

#if defined(_M_AMD64)
#  include "LDE64x64.h"
#endif 

#ifdef __cplusplus
//#pragma INITCODE
extern "C" NTSTATUS DriverEntry(IN PDRIVER_OBJECT DriverObject, IN PUNICODE_STRING  RegistryPath);
#endif


//�ṩһ��Unload����ֻ��Ϊ������������ܶ�̬ж�أ�������ԡ�����һ���ں�ģ��һ�����ؾͲ�����ж����
void FJDriverDemoUnload(IN PDRIVER_OBJECT DriverObject)
{
	KdPrint(("Enter FJDriverDemoUnload, PID=%d\n", PsGetCurrentProcessId()));
	
    NTSTATUS status = STATUS_SUCCESS;
	FNT_VERIFY(UnInstallCopyProtectHook());

	UNICODE_STRING Win32NameString;

	RtlInitUnicodeString (&Win32NameString , FDRIVER_DEMO_DOS_DEVICE_NAME);	
	FNT_VERIFY(IoDeleteSymbolicLink (&Win32NameString));
	NT_ASSERT(NULL != DriverObject->DeviceObject);

	IoDeleteDevice(DriverObject->DeviceObject);

	KdPrint(("Leave FJDriverDemoUnload, PID=%d\n", PsGetCurrentProcessId()));
}

#pragma PAGEDCODE
NTSTATUS FJDriverDemoDefaultHandler(IN PDEVICE_OBJECT DeviceObject, IN PIRP pIrp)
{
    //PAGED_CODE();

	PIO_STACK_LOCATION pIoStackLoc = IoGetCurrentIrpStackLocation(pIrp);
	KdPrint(("DefaultHandler %s\n", GetIrpMajorCodeString(pIoStackLoc->MajorFunction)));

	NTSTATUS status = STATUS_SUCCESS;
	pIrp->IoStatus.Status = status;	//����IRP���״̬
	pIrp->IoStatus.Information = 0;	//���ò������ֽ���Ϊ0
	IoCompleteRequest( pIrp, IO_NO_INCREMENT);	//ָʾ��ɴ�IRP�����ټ�������

	return status;
	//������ǰ���豸�����豸ջ�е��²��豸��������
	//IoSkipCurrentIrpStackLocation(pIrp);
	//deviceExtension = (PFJDriverDemo_DEVICE_EXTENSION) DeviceObject->DeviceExtension;
	//return IoCallDriver(deviceExtension->TargetDeviceObject, pIrp);
}

#pragma PAGEDCODE
NTSTATUS FJDriverDemoDeviceControl(IN PDEVICE_OBJECT DeviceObject, IN PIRP pIrp)
{
    NTSTATUS status	= STATUS_INVALID_DEVICE_REQUEST;

	PIO_STACK_LOCATION pIoStackLoc = IoGetCurrentIrpStackLocation(pIrp);
	NT_ASSERT(pIoStackLoc->MajorFunction == IRP_MJ_DEVICE_CONTROL);
	PVOID inputBuffer = pIrp->AssociatedIrp.SystemBuffer;
	PVOID outputBuffer = pIrp->AssociatedIrp.SystemBuffer;

	ULONG inputBufferLength = pIoStackLoc->Parameters.DeviceIoControl.InputBufferLength;
	ULONG outputBufferLength = pIoStackLoc->Parameters.DeviceIoControl.OutputBufferLength;
	ULONG ioControlCode = pIoStackLoc->Parameters.DeviceIoControl.IoControlCode;

	switch (ioControlCode)
	{
	case IOCTL_FDRIVER_INSTALL_HOOK:
		{
			//SYS_SERVICE_TABLE* pServiceTable = GetServiceDescriptorShadowTableAddress();
			KdPrint(("Enter IOCTL_FDRIVER_INSTALL_HOOK, inputLen=%d\n", inputBufferLength));
			NT_ASSERT(inputBufferLength == sizeof(PROTECT_WND_INFO));

			if (inputBufferLength == sizeof(PROTECT_WND_INFO))
			{
				FNT_VERIFY(InstallCopyProtectHook((PPROTECT_WND_INFO)inputBuffer));
			}
            status = STATUS_SUCCESS;
		}
		break;
	case IOCTL_FDRIVER_UNINSTALL_HOOK:
		{
			UnInstallCopyProtectHook();
			KdPrint(("Enter IOCTL_FDRIVER_INSTALL_HOOK\n"));
            status = STATUS_SUCCESS;
		}
		break;
    default:
        break;
	}
	return FJDriverDemoDefaultHandler(DeviceObject, pIrp);
}	

//ÿ���ں�ģ�����ں������ڼ��ظ�ģ��ʱ��ϵͳ����System����һ��
NTSTATUS DriverEntry(IN PDRIVER_OBJECT pDriverObject, IN PUNICODE_STRING  RegistryPath)
{
	NTSTATUS status = STATUS_SUCCESS;

	KdPrint(("Enter FJDriverDemo DriverEntry,PID=%d\n", PsGetCurrentProcessId()));
	
#if DBG
	//���ָ��൱���ֹ�����һ���ϵ㣬�������Ե���
	//_asm int 3
#endif 
	PDEVICE_OBJECT deviceObject = NULL;
	UNICODE_STRING ntName;
	UNICODE_STRING win32Name;

	//UNICODE_STRING					SymbolName;
	//PVOID pAddress = NULL;
	//RtlInitUnicodeString(&SymbolName, L"KeCancelTimer");
	//pAddress = MmGetSystemRoutineAddress(&SymbolName);

	//KdPrint(("%wZ pAddress=0x%p\n"), &SymbolName, pAddress);


	RtlInitUnicodeString(&ntName, FDRIVER_DEMO_NT_DEVICE_NAME);
	FNT_VERIFY(IoCreateDevice(pDriverObject, 
		0, 
		&ntName, 
		FDRIVER_DEV_DRV,	//FILE_DEVICE_UNKNOWN
		0,	//FILE_DEVICE_SECURE_OPEN
		TRUE, 
		&deviceObject));
	if (FALSE == NT_SUCCESS(status))
	{
		KdPrint( ("%s >> IoCreateDevice fail", __FUNCTION__) );
		return status;
	}

	RtlInitUnicodeString (&win32Name, FDRIVER_DEMO_DOS_DEVICE_NAME);	
	FNT_VERIFY(IoCreateSymbolicLink ( 
		&win32Name, 
		&ntName));
	if (FALSE == NT_SUCCESS(status))
	{
		KdPrint( ("%s >> IoCreateSymbolicLink() failed", __FUNCTION__) );
		IoDeleteDevice( deviceObject );
		return status;
	}

	NT_ASSERT(pDriverObject->DeviceObject == NULL || pDriverObject->DeviceObject == deviceObject);
	//if (pDriverObject->DeviceObject == NULL)
	//{
	//  �Ƿ���Ҫ���ù�ȥ��û�п����ĸ���������Ĵ��������ù���ϵͳ���Զ����ù�ȥ��Unload��ֱ�� Delete ��
	//	//WinXP
	//	pDriverObject->DeviceObject = deviceObject;
	//}

	for (unsigned int i = 0; i <= IRP_MJ_MAXIMUM_FUNCTION; i++)
	{
		pDriverObject->MajorFunction[i] = FJDriverDemoDefaultHandler;
	}

	//pDriverObject->MajorFunction[IRP_MJ_CREATE] = FJDriverDemoCreateClose;
	//pDriverObject->MajorFunction[IRP_MJ_CLOSE] = FJDriverDemoCreateClose;
	//pDriverObject->MajorFunction[IRP_MJ_PNP] = FJDriverDemoPnP;
	
	pDriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = FJDriverDemoDeviceControl;
	pDriverObject->DriverUnload = FJDriverDemoUnload;

	//��ʼ�����������
#if defined(_M_AMD64)
	LDE_init();
#endif //defined

	KdPrint(("Leave FJDriverDemo DriverEntry,PID=%d\n", PsGetCurrentProcessId()));

	return status;
}