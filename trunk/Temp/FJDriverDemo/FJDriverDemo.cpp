#include "stdafx.h"
#include "FDriverMemory.h"
#include "FDriverDemoDefine.h"
#include "FDriverUtil.h"

/*
	FJDriverDemo - Main file
	This file contains a very simple implementation of a WDM driver. Note that it does not support all
	WDM functionality, or any functionality sufficient for practical use. The only thing this driver does
	perfectly, is loading and unloading.

	To install the driver, go to Control Panel -> Add Hardware Wizard, then select "Add a new hardware device".
	Select "manually select from list", choose device category, press "Have Disk" and enter the path to your
	INF file.
	Note that not all device types (specified as Class in INF file) can be installed that way.

	To start/stop this driver, use Windows Device Manager (enable/disable device command).

	If you want to speed up your driver development, it is recommended to see the BazisLib library, that
	contains convenient classes for standard device types, as well as a more powerful version of the driver
	wizard. To get information about BazisLib, see its website:
		http://bazislib.sysprogs.org/
*/

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
#pragma INITCODE
extern "C" NTSTATUS DriverEntry(IN PDRIVER_OBJECT DriverObject, IN PUNICODE_STRING  RegistryPath);
#endif

NTSTATUS SameDemo()
{
	return STATUS_NO_EAS_ON_FILE;
}

//�ṩһ��Unload����ֻ��Ϊ������������ܶ�̬ж�أ�������ԡ�����һ���ں�ģ��һ�����ؾͲ�����ж����
void FJDriverDemoUnload(IN PDRIVER_OBJECT DriverObject)
{
	//ExAllocatePool()
    NTSTATUS status;
    FNT_VERIFY(SameDemo());
	KdPrint(("Goodbye from FJDriverDemo, PID=%d\n", PsGetCurrentProcessId()));
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
    PAGED_CODE();
	PFJDriverDemo_DEVICE_EXTENSION deviceExtension = NULL;
	
	NTSTATUS status = STATUS_SUCCESS;
	//pIrp->IoStatus.Status = status;	//����IRP���״̬
	//pIrp->IoStatus.Information = 0;	//���ò������ֽ���Ϊ0
	//IoCompleteRequest( pIrp, IO_NO_INCREMENT);	//ָʾ��ɴ�IRP�����ټ�������

	//������ǰ���豸�����豸ջ�е��²��豸��������
	IoSkipCurrentIrpStackLocation(pIrp);
	deviceExtension = (PFJDriverDemo_DEVICE_EXTENSION) DeviceObject->DeviceExtension;
	return IoCallDriver(deviceExtension->TargetDeviceObject, pIrp);
}

NTSTATUS FJDriverDemoAddDevice(IN PDRIVER_OBJECT  pDriverObject, IN PDEVICE_OBJECT  PhysicalDeviceObject)
{
	PDEVICE_OBJECT pDeviceObject = NULL;
	PFJDriverDemo_DEVICE_EXTENSION pExtension = NULL;
	NTSTATUS status = STATUS_SUCCESS;
	
    //�����豸����
    UNICODE_STRING  devName;
    RtlInitUnicodeString(&devName, FDRIVER_DEMO_DEVICE_NAME);
    
    //�����豸
	status = IoCreateDevice(pDriverObject,
						    sizeof(FJDriverDemo_DEVICE_EXTENSION),
							&devName,
							FILE_DEVICE_UNKNOWN,
							0,
							TRUE, //Exclusive -- ��ռ�豸��ͬһʱ��ֻ�ܱ�һ��Ӧ�ó�����ʹ��
							&pDeviceObject);

	if (!NT_SUCCESS(status))
		return status;

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
	//pDriverObject->DriverExtension->AddDevice = xxxx; //WDM�����д����豸������Pnp���������õĻص�����

	KdPrint(("New Enter FJDriverDemo DriverEntry,PID=%d\n", PsGetCurrentProcessId()));

	//for (i = 0; i <= IRP_MJ_MAXIMUM_FUNCTION; i++)
	//	DriverObject->MajorFunction[i] = FJDriverDemoDefaultHandler;

	//pDriverObject->MajorFunction[IRP_MJ_CREATE] = FJDriverDemoCreateClose;
	//pDriverObject->MajorFunction[IRP_MJ_CLOSE] = FJDriverDemoCreateClose;
	//pDriverObject->MajorFunction[IRP_MJ_PNP] = FJDriverDemoPnP;

	pDriverObject->DriverUnload = FJDriverDemoUnload;
	//DriverObject->DriverStartIo = NULL;
	//DriverObject->DriverExtension->AddDevice = FJDriverDemoAddDevice;

    KdPrint(("Leave FJDriverDemo DriverEntry,PID=%d\n", PsGetCurrentProcessId()));

	return status;
}