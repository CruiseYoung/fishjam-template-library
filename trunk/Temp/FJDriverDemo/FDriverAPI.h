#ifndef F_DRIVER_API_H
#define F_DRIVER_API_H

//�������� KeDelayExecutionThread �ĺ�
#define DELAY_ONE_MICROSECOND	(-10)
#define DELAY_ONE_MILLISECOND	(DELAY_ONE_MICROSECOND * 1000)
#define DELAY_ONE_SECOND		(DELAY_ONE_MILLISECOND * 1000)

/******************************************************************************************************************
* DriverEntry -- �����������ں���(extern "C")�����ں��е�IO������������ã���Ҫ�����ǳ�ʼ�� DriverObject, ָ��Dispatch����
*   IN PDRIVER_OBJECT DriverObject    -- IO���������ݽ�������������
*   IN PUNICODE_STRING  RegistryPath  -- ����������ע����е�·��
******************************************************************************************************************/

/******************************************************************************************************************
* �ں˶���
*   DRIVER_OBJECT(��������) -- ��Ҫ��дһ��ص���������Windows���ã����ģʽ��ÿ����������ֻ��һ����������
*     ����IO�ַ����� -- FAST_IO_DISPATCH
*     MajorFunction -- ��ͨ�ַ�����(DRIVER_DISPATCH),��Ӧ����  IRP_MJ_XXX 
*     DriverStart/DriverSize -- ������������������ں�ģ�����ں˿ռ��еĿ�ʼ��ַ�ʹ�С
*     DeviceObject -- �豸��(DEVICE_OBJECT::NextDevice ָ��ͬһ�������е���һ���豸)
*     DriverUnload -- ж�غ�����ָ�룬����Ϊ�գ���ʾ���Զ�̬ж��
*     DriverExtension -- �豸��չ�������� AddDevice �Ȼص�����ָ��
* 
*   DEVICE_OBJECT(�豸����) -- �����豸������״̬��Ϣ��ϵͳ�ϵ�ÿһ�����⡢�߼���������豸����һ���豸���󣬿��Խ�������(IRP)��
*     DeviceExtension -- �豸��չ,�ɰ����κ��Զ�����Ϣ����IoCreateDeviceʱָ�������ݲ�ͬ�����������Ҫ�����𲹳䶨���豸�������Ϣ��
*   FILE_OBJECT(�ļ�����)
******************************************************************************************************************/

/******************************************************************************************************************
* Ӧ�ó���һ��ͨ�� DeviceIoControl ����������
* ���ж��豸�Ĳ������ս�ת��ΪIRP����,��ЩIRP����ᱻ���͵���ǲ��������
*
* IRP (�����ܺ� + �ι��ܺ� PIO_STACK_LOCATION::MinorFunction),���������ܵķַ������������ܺ�������
*     ������ DriveObject::MajorFunction ������(�ܸ���Ϊ IRP_MJ_MAXIMUM_FUNCTION - 0x1b)������ϵͳ����IRPʱ���͵��ö�Ӧ�ĺ�������
*     TODO: pDriverObject->MajorFunction[IRP_MJ_WRITE] = USE_WRITE_FUNCTION��   // ?
*   IRP�ǴӷǷ�ҳ�ڴ���з���Ŀɱ��С�Ľṹ������һ�� IO_STACK_LOCATION �ṹ����
*
*   IoCopyCurrentIrpStackLocationToNext -- �ѵ�ǰIRPջ�ռ俽������һ��ջ�ռ�,��Ҫ�� IoSetCompletionRoutine ���ã��������̹���(�·��������ڶ�����������)
*   IoGetCurrentIrpStackLocation -- ��õ�ǰջ�ռ���Ϣ
*   IoMarkIrpPending -- 
*   IoSetCompletionRoutine -- ������ɻص�����
*   IoSkipCurrentIrpStackLocation -- ������ǰջ�ռ�
* 
*   IoCompleteRequest(, IO_NO_INCREMENT); ??
*   IoCreateSymbolicLink -- �ڶ���������д���һ��"��������"(�� Z: => \Device\NetworkRedirector )
*   IoDeleteSymbolicLink --
*
*   ��������
*     MDLAddress -- ͨ�� MDL ��Ӧ�ò�ĵ�ַ�ռ�ӳ�䵽�ں˿ռ�
*     UserBuffer -- ��׷��Ч�ʵĽ��������Ӧ�ò�Ļ�������ֱַ�ӷ������У����ں˿����з��ʡ�ֻ�����ڵ�ǰ���̺ͷ����������һ�µ������
*     AssociatedIrp.SystemBuffer -- ���ڱȽϼ��Ҳ�׷��Ч������µĽ������(��R3���ڴ�ռ��еĻ������ݿ�����R0)
*   ͨ�õĶ�д���ݷ�ʽ(TODO:û�б�־ָ��ʹ�õľ��建������):
*     if(irp->MdlAddress != NULL) buffer = (PBYTE)MmGetSystemAddressForMdlSafe(irp->MdlAddress);
*     else buffer = (PBYTE)irp->UserBuffer;
*     if(buffer == NULL) buffer = (PBYTE)irp->AssociatedIrp.SystemBuffer;
* 
******************************************************************************************************************/

/******************************************************************************************************************
*   
******************************************************************************************************************/

/******************************************************************************************************************
* <ntstrsafe.h> -- �ַ��������� ��Ҫ����Win2K����Ҫ�ȶ��� NTSTRSAFE_LIB ��
*   RtlStringCchPrintfW
*   RtlInitUnicodeString -- ��ʼ�� UNICODE_STRING �ṹ�壬�ýṹ�岻��Ҫʹ��NULL��Ϊ�ַ����Ľ���
******************************************************************************************************************/

/******************************************************************************************************************
* Driver
*   IoCallDriver -- ���豸��������?
*
* Device
*   IoAttachDevice/IoDetachDevice  -- ��/������豸���豸ջ�ϣ���������������(Filter)
*   IoAttachDeviceToDeviceStack
*   IoCreateDevice/IoDeleteDevice
*   IoEnumerateDeviceObjectList -- ö��һ�������µ������豸����
*   IoGetAttachedDevice -- ���һ���豸���ڵ��豸ջ��˵��Ǹ��豸
*   IoGetDeviceObjectPointer -- �����豸���ֻ���豸�����ָ��,��Ҫʹ�� ObDereferenceObject ���ļ����������÷�ֹй¶
*   IoSetDeviceInterfaceState
* 
* Other
*   ObDereferenceObject
*   ObReferenceObjectByName(ͷ�ļ���û������?) -- ͨ��һ�����ֻ��һ�������ָ��
*
* ��Դ(Power)���?
*   PoStartNextPowerIrp -- 
*   PoCallDriver -- ??
******************************************************************************************************************/

/******************************************************************************************************************
* KeDelayExecutionThread(KernelMode, FALSE, &interval) -- ��ʱָ��ʱ��(ԭ��? �����̻߳����ִ��?)
* KsGetCurrentThread --
*
******************************************************************************************************************/

/******************************************************************************************************************
* WDF
*   WdfDriverCreate -- 
******************************************************************************************************************/



#endif //F_DRIVER_API_H