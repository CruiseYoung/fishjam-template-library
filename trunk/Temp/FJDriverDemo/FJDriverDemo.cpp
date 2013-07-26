#include "stdafx.h"
#include "FDriverMemory.h"
#include "FDriverDemoDefine.h"
#include "FDriverUtil.h"
#include "FDriverHookAPI.h"

extern SYS_SERVICE_TABLE *g_pShadowTable;

extern SCROLL_DATA g_ScrollData;
extern SCROLL_HOOK_TARGET g_ScrollHookTarget;

extern ULONG g_IsScrolled;

//void FJDriverDemoUnload(IN PDRIVER_OBJECT DriverObject);
//NTSTATUS FJDriverDemoCreateClose(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp);
//NTSTATUS FJDriverDemoDefaultHandler(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp);
//NTSTATUS FJDriverDemoAddDevice(IN PDRIVER_OBJECT  pDriverObject, IN PDEVICE_OBJECT  PhysicalDeviceObject);
//NTSTATUS FJDriverDemoPnP(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp);

typedef struct _deviceExtension
{
	PDEVICE_OBJECT DeviceObject;
	PDEVICE_OBJECT TargetDeviceObject;
	PDEVICE_OBJECT PhysicalDeviceObject;
	UNICODE_STRING DeviceInterface;
} FJDriverDemo_DEVICE_EXTENSION, *PFJDriverDemo_DEVICE_EXTENSION;

// {220e7e68-c6b4-4e63-aeb9-c9a1dc0cb2df}
static const GUID GUID_FJDriverDemoInterface = {0x220E7E68, 0xc6b4, 0x4e63, {0xae, 0xb9, 0xc9, 0xa1, 0xdc, 0xc, 0xb2, 0xdf } };

#ifdef __cplusplus
//#pragma INITCODE
extern "C" NTSTATUS DriverEntry(IN PDRIVER_OBJECT DriverObject, IN PUNICODE_STRING  RegistryPath);
#endif

NTSTATUS SameDemo()
{
	return STATUS_NO_EAS_ON_FILE;
}

//�ṩһ��Unload����ֻ��Ϊ������������ܶ�̬ж�أ�������ԡ�����һ���ں�ģ��һ�����ؾͲ�����ж����
void FJDriverDemoUnload(IN PDRIVER_OBJECT DriverObject)
{
	KdPrint(("Enter FJDriverDemoUnload\n"));

	UnInstallScrollHook();

	//ExAllocatePool()
    NTSTATUS status;
    //FNT_VERIFY(SameDemo());

	UNICODE_STRING Win32NameString;

	//UnInstallScrollHook();

	RtlInitUnicodeString (&Win32NameString , FDRIVER_DEMO_DOS_DEVICE_NAME);	
	FNT_VERIFY(IoDeleteSymbolicLink (&Win32NameString));	
	IoDeleteDevice (DriverObject->DeviceObject);

	KdPrint(("Goodbye xxxxxx from FJDriverDemo, PID=%d\n", PsGetCurrentProcessId()));
}

NTSTATUS FJDriverDemoCreateClose(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp)
{
	Irp->IoStatus.Status = STATUS_SUCCESS;
	Irp->IoStatus.Information = 0;
	IoCompleteRequest(Irp, IO_NO_INCREMENT);
	return STATUS_SUCCESS;
}

#pragma PAGEDCODE
NTSTATUS FJDriverDemoDefaultHandler(IN PDEVICE_OBJECT DeviceObject, IN PIRP pIrp)
{
    //PAGED_CODE();
	PFJDriverDemo_DEVICE_EXTENSION deviceExtension = NULL;

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
	NTSTATUS status; 
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
			//KdPrint(("%s, pServiceTable=0x%p\n", "IOCTL_FDRIVER_INSTALL_HOOK", pServiceTable));
	
			if (g_pShadowTable == NULL)
			{
				if (inputBufferLength == sizeof(SCROLL_HOOK_TARGET))
				{
					RtlCopyMemory(&g_ScrollHookTarget, inputBuffer, sizeof(SCROLL_HOOK_TARGET));
					InstallCopyProtectHook(g_ScrollHookTarget.hTargetProcess);
				}
			}
			else
			{
				KdPrint(("Already InstallCopyProtectHook()"));
			}

#if DBG
			//_asm int 3
#endif 
		}
		break;
	case IOCTL_FDRIVER_UNINSTALL_HOOK:
		{
			//UnInstallScrollHook();
			KdPrint(("%s\n", "IOCTL_FDRIVER_INSTALL_HOOK"));
		}
		break;
	}
	return FJDriverDemoDefaultHandler(DeviceObject, pIrp);
}	


NTSTATUS FJDriverDemoAddDevice(IN PDRIVER_OBJECT  pDriverObject, IN PDEVICE_OBJECT  PhysicalDeviceObject)
{
	PDEVICE_OBJECT pDeviceObject = NULL;
	PFJDriverDemo_DEVICE_EXTENSION pExtension = NULL;
	NTSTATUS status = STATUS_SUCCESS;
	
    //�����豸����
    UNICODE_STRING  ntName;
    RtlInitUnicodeString(&ntName, FDRIVER_DEMO_NT_DEVICE_NAME);
    
    //�����豸
	status = IoCreateDevice(pDriverObject,
						    sizeof(FJDriverDemo_DEVICE_EXTENSION),
							&ntName,
							FILE_DEVICE_UNKNOWN,
							0,
							TRUE, //Exclusive -- ��ռ�豸��ͬһʱ��ֻ�ܱ�һ��Ӧ�ó�����ʹ��
							&pDeviceObject);

	if (!NT_SUCCESS(status))
		return status;

	UNICODE_STRING win32Name;
	RtlInitUnicodeString(&win32Name, FDRIVER_DEMO_DOS_DEVICE_NAME);
	status = IoCreateSymbolicLink ( 
		&win32Name, 
		&ntName);
	if (FALSE == NT_SUCCESS(status))
	{
		KdPrint( ("%s >> IoCreateSymbolicLink() failed", __FUNCTION__) );
		IoDeleteDevice( pDeviceObject );
		return status;
	}

	pExtension = (PFJDriverDemo_DEVICE_EXTENSION)pDeviceObject->DeviceExtension;

	pExtension->DeviceObject = pDeviceObject;
	pExtension->PhysicalDeviceObject = PhysicalDeviceObject;
	pExtension->TargetDeviceObject = IoAttachDeviceToDeviceStack(pDeviceObject, PhysicalDeviceObject);

	status = IoRegisterDeviceInterface(PhysicalDeviceObject, &GUID_FJDriverDemoInterface, NULL, &pExtension->DeviceInterface);
	ASSERT(NT_SUCCESS(status));

    //pDeviceObject->Flags |= DO_BUFFERED_IO | DO_POWER_PAGABLE;
	pDeviceObject->Flags &= ~DO_DEVICE_INITIALIZING;
	return STATUS_SUCCESS;
}


NTSTATUS FJDriverDemoIrpCompletion(
					  IN PDEVICE_OBJECT DeviceObject,
					  IN PIRP Irp,
					  IN PVOID Context
					  )
{
	PKEVENT Event = (PKEVENT) Context;

	UNREFERENCED_PARAMETER(DeviceObject);
	UNREFERENCED_PARAMETER(Irp);

	KeSetEvent(Event, IO_NO_INCREMENT, FALSE);

	return(STATUS_MORE_PROCESSING_REQUIRED);
}

NTSTATUS FJDriverDemoForwardIrpSynchronous(
							  IN PDEVICE_OBJECT DeviceObject,
							  IN PIRP Irp
							  )
{
	PFJDriverDemo_DEVICE_EXTENSION   deviceExtension;
	KEVENT event;
	NTSTATUS status;

	KeInitializeEvent(&event, NotificationEvent, FALSE);
	deviceExtension = (PFJDriverDemo_DEVICE_EXTENSION) DeviceObject->DeviceExtension;

	IoCopyCurrentIrpStackLocationToNext(Irp);

	IoSetCompletionRoutine(Irp, FJDriverDemoIrpCompletion, &event, TRUE, TRUE, TRUE);

	status = IoCallDriver(deviceExtension->TargetDeviceObject, Irp);

	if (status == STATUS_PENDING) {
		KeWaitForSingleObject(&event, Executive, KernelMode, FALSE, NULL);
		status = Irp->IoStatus.Status;
	}
	return status;
}

NTSTATUS FJDriverDemoPnP(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp)
{
	PIO_STACK_LOCATION irpSp = IoGetCurrentIrpStackLocation(Irp);
	PFJDriverDemo_DEVICE_EXTENSION pExt = ((PFJDriverDemo_DEVICE_EXTENSION)DeviceObject->DeviceExtension);
	NTSTATUS status;

	ASSERT(pExt);

	switch (irpSp->MinorFunction)
	{
	case IRP_MN_START_DEVICE:
		IoSetDeviceInterfaceState(&pExt->DeviceInterface, TRUE);
		Irp->IoStatus.Status = STATUS_SUCCESS;
		IoCompleteRequest(Irp, IO_NO_INCREMENT);
		return STATUS_SUCCESS;

	case IRP_MN_QUERY_REMOVE_DEVICE:
		Irp->IoStatus.Status = STATUS_SUCCESS;
		IoCompleteRequest(Irp, IO_NO_INCREMENT);
		return STATUS_SUCCESS;

	case IRP_MN_REMOVE_DEVICE:      //Pnp��ж���豸
		IoSetDeviceInterfaceState(&pExt->DeviceInterface, FALSE);
		status = FJDriverDemoForwardIrpSynchronous(DeviceObject, Irp);
		IoDetachDevice(pExt->TargetDeviceObject);
		IoDeleteDevice(pExt->DeviceObject);
		RtlFreeUnicodeString(&pExt->DeviceInterface);
		Irp->IoStatus.Status = STATUS_SUCCESS;
		IoCompleteRequest(Irp, IO_NO_INCREMENT);
		return STATUS_SUCCESS;

	case IRP_MN_QUERY_PNP_DEVICE_STATE:
		status = FJDriverDemoForwardIrpSynchronous(DeviceObject, Irp);
		Irp->IoStatus.Information = 0;
		IoCompleteRequest(Irp, IO_NO_INCREMENT);
		return status;
	}
	return FJDriverDemoDefaultHandler(DeviceObject, Irp);
}

//ÿ���ں�ģ�����ں������ڼ��ظ�ģ��ʱ��ϵͳ����System����һ��
NTSTATUS DriverEntry(IN PDRIVER_OBJECT pDriverObject, IN PUNICODE_STRING  RegistryPath)
{
	unsigned i;
	NTSTATUS status = STATUS_SUCCESS;
	//FNT_VERIFY(SameDemo());
#if DBG
	//���ָ��൱���ֹ�����һ���ϵ㣬�������Ե���
	//_asm int 3
#endif 
	PDEVICE_OBJECT deviceObject = NULL;
	UNICODE_STRING ntName;
	UNICODE_STRING win32Name;

	RtlInitUnicodeString(&ntName, FDRIVER_DEMO_NT_DEVICE_NAME);
	FNT_VERIFY(IoCreateDevice(pDriverObject, 
		0, 
		&ntName, 
		FDRIVER_DEV_DRV, 
		0, 
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
	NT_ASSERT(pDriverObject->DeviceObject == NULL);
	pDriverObject->DeviceObject = deviceObject;

	//pDriverObject->DriverExtension->AddDevice = xxxx; //WDM�����д����豸������Pnp���������õĻص�����

	KdPrint(("New Enter FJDriverDemo DriverEntry,PID=%d\n", PsGetCurrentProcessId()));

	for (i = 0; i <= IRP_MJ_MAXIMUM_FUNCTION; i++)
	{
		pDriverObject->MajorFunction[i] = FJDriverDemoDefaultHandler;
	}

	//pDriverObject->MajorFunction[IRP_MJ_CREATE] = FJDriverDemoCreateClose;
	//pDriverObject->MajorFunction[IRP_MJ_CLOSE] = FJDriverDemoCreateClose;
	//pDriverObject->MajorFunction[IRP_MJ_PNP] = FJDriverDemoPnP;
	
	pDriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = FJDriverDemoDeviceControl;
	pDriverObject->DriverUnload = FJDriverDemoUnload;
	//DriverObject->DriverStartIo = NULL;
	//DriverObject->DriverExtension->AddDevice = FJDriverDemoAddDevice;

    KdPrint(("Leave FJDriverDemo DriverEntry,PID=%d\n", PsGetCurrentProcessId()));

	return status;
}