#ifndef F_DRIVER_API_H
#define F_DRIVER_API_H

//�������� KeDelayExecutionThread �ĺ�
#define DELAY_ONE_MICROSECOND	(-10)
#define DELAY_ONE_MILLISECOND	(DELAY_ONE_MICROSECOND * 1000)
#define DELAY_ONE_SECOND		(DELAY_ONE_MILLISECOND * 1000)

/******************************************************************************************************************
* �ں˶���
*   DRIVER_OBJECT(��������) -- ��Ҫ��дһ��ص���������Windows���ã����ģʽ��ÿ����������ֻ��һ����������
*     ����IO�ַ����� -- FAST_IO_DISPATCH
*     ��ͨ�ַ�����   -- DRIVER_DISPATCH,  MajorFunction?
*     DriverStart/DriverSize -- ������������������ں�ģ�����ں˿ռ��еĿ�ʼ��ַ�ʹ�С
*     DeviceObject -- �豸��(���е� NextDevice ָ��ͬһ�������е���һ���豸)
*     DriverUnload -- ж�غ�����ָ�룬����Ϊ�գ���ʾ���Զ�̬ж��
*     
*   DEVICE_OBJECT(�豸����) -- ���Խ�������(IRP)
*     DeviceExtension -- �豸��չ,�ɰ����κ��Զ�����Ϣ����IoCreateDeviceʱָ��
*   FILE_OBJECT(�ļ�����)
******************************************************************************************************************/

/******************************************************************************************************************
* IRP (�����ܺ� + �ι��ܺ� PIO_STACK_LOCATION::MinorFunction)
*   ���������ܵķַ������������ܺ������������� DriveObject::MajorFunction ������
*
*   IoCopyCurrentIrpStackLocationToNext -- �ѵ�ǰIRPջ�ռ俽������һ��ջ�ռ�,��Ҫ�� IoSetCompletionRoutine ���ã��������̹���(�·��������ڶ�����������)
*   IoGetCurrentIrpStackLocation -- ��õ�ǰջ�ռ�
*   IoMarkIrpPending -- 
*   IoSetCompletionRoutine -- ������ɻص�����
*   IoSkipCurrentIrpStackLocation -- ������ǰջ�ռ�
*   IoCompleteRequest(, IO_NO_INCREMENT); ??
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
*   RtlInitUnicodeString
******************************************************************************************************************/

/******************************************************************************************************************
* Driver
*   IoCallDriver -- ���豸��������?
*
* Device
*   IoAttachDevice/IoDetachDevice  -- ��/������豸���豸ջ�ϣ���������������(Filter)
*   IoCreateDevice/IoDeleteDevice
*   IoEnumerateDeviceObjectList -- ö��һ�������µ������豸����
*   IoGetAttachedDevice -- ���һ���豸���ڵ��豸ջ��˵��Ǹ��豸
*   IoGetDeviceObjectPointer -- �����豸���ֻ���豸�����ָ��,��Ҫʹ�� ObDereferenceObject ���ļ����������÷�ֹй¶
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



#endif //F_DRIVER_API_H