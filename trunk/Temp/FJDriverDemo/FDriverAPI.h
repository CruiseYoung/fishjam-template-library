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
* Ӧ�ó���һ��ͨ�� �ļ���ʽ( �� WriteFile / DeviceIoControl ) ���������� -- I/O�������������û�ģʽ������豸��������֮��Ľӿڡ�
* ���ж��豸�Ĳ���(����˿ڵĶ�д�����̷��ʡ������ļ��Ĳ�����)���ս�ת��ΪIRP����,��ЩIRP����ᱻ���͵���ǲ��������
*
* ���������е��豸����ֻ�����ں�״̬�ɼ�, Ӧ�ó�����ֱ�Ӹ����豸�����豸,Ӧ�ó���Ҫʶ���豸�����ַ�����
*   1.ͨ�� ��������(IoCreateSymbolicLink) �ҵ��豸����ͨ�� "\??\C:" �ҵ� "/Device/HarddiskVolume1"
*     �ں�ģʽ�£����������� "\??\" ���� "\DosDevices\" ��ͷ���� "\??\C:"
*     �û�ģʽ�£����������� "\\.\" ��ͷ�� �� "\\.\C:"
*   2.ͨ���豸�ӿ��ҵ��豸
*
*   
******************************************************************************************************************/

/******************************************************************************************************************
* �ں˶���
*   DRIVER_OBJECT(��������) -- ��Ҫ��дһ��ص���������Windows���ã����ģʽ��ÿ����������ֻ��һ����������
*     FastIoDispatch -- ����IO�ַ�����(�ļ�������ʹ��)
*     MajorFunction -- ��ͨ�ַ�����(DRIVER_DISPATCH)��ָ������,��Ӧ����  IRP_MJ_XXX 
*     DriverStart/DriverSize -- ������������������ں�ģ�����ں˿ռ��еĿ�ʼ��ַ�ʹ�С
*     DriverStartIo -- ��¼StartIO�����ĵ�ַ�����ڴ��л�����
*     DeviceObject -- �������е��豸��(DEVICE_OBJECT::NextDevice ָ��ͬһ�������е���һ���豸)
*     DriverName -- ������������֣�һ��Ϊ "\Driver\������������"
*     DriverUnload -- ж�غ�����ָ�룬����Ϊ�գ���ʾ���Զ�̬ж��
*     DriverExtension -- ������չ
*       AddDevice -- WDM���͵������У���Pnp���������ã����𴴽��豸������PhysicalDeviceObjectΪPnp������
*                    ����ĵײ������豸����
*
*   DEVICE_OBJECT(�豸����) -- �����豸������״̬��Ϣ��ϵͳ�ϵ�ÿһ�����⡢�߼���������豸����һ���豸���󣬿��Խ�������(IRP)��
*     AttachedDevice -- �и���һ����������ӵ��������ʱ����ָ����Ǹ��߲������(�� PDO ָ�� FDO)
*     CurrentIrp -- ʹ��StartIO����ʱ��ָ��ǰIRP�ṹ
*     DeviceExtension -- �豸��չ,�ɰ����κ��Զ�����Ϣ����IoCreateDeviceʱָ������IO�����������������ڷǷ�ҳ�ڴ��С�
*       ���ݲ�ͬ�����������Ҫ�����𲹳䶨���豸�������Ϣ��
*     DeviceType -- �豸�����ͣ��� FILE_DEVICE_UNKNOWN(�������������豸ʱʹ��) ��
*     DriverObject -- ָ�����������е���������ͬ����һ�������������������ָ�����ͬһ��������(���������е���Σ�)
*     NextDevice -- ָ����һ���豸�����γ��豸��
*     StackSize -- ��������Ĳ���( ����֮����γ����ƶ�ջ�Ľṹ,IRP�����δ���߲㴫�ݵ���ײ� )
*   FILE_OBJECT(�ļ�����)
******************************************************************************************************************/

/******************************************************************************************************************
* �����������Ҫ�������� Dispatch Function(�ַ�����/��ǲ����) �д��� I/O ����
* IRP(I/O Request Packages) -- �����ܺ� + �ι��ܺ� PIO_STACK_LOCATION::MinorFunction,
*     ���������ܵķַ������������ܺ��������������� DriveObject::MajorFunction ������(�ܸ���Ϊ IRP_MJ_MAXIMUM_FUNCTION - 0x1b)��
*     ����ϵͳ����IRPʱ���͵��ö�Ӧ�ĺ��������������ã�Ĭ���ǹ��� _IopInvalidDeviceRequest ����(��ͨ�� extern �������޷����� ?)
*     TODO: pDriverObject->MajorFunction[IRP_MJ_WRITE] = USE_WRITE_FUNCTION��   // ?
*   IRP�ǴӷǷ�ҳ�ڴ���з���Ŀɱ��С�Ľṹ������һ�� IO_STACK_LOCATION �ṹ����
* 
* IO_STACK_LOCATION -- IO��ջ����Ӧ�豸��ջ��ÿ���豸�����Ĳ����������豸��Ӧ��ֵ��ͨ�� IoGetCurrentIrpStackLocation ���
* 
*   IoCopyCurrentIrpStackLocationToNext -- �ѵ�ǰIRPջ�ռ俽������һ��ջ�ռ�,��Ҫ�� IoSetCompletionRoutine ���ã��������̹���(�·��������ڶ�����������)
*   IoGetCurrentIrpStackLocation -- ��õ�ǰջ�ռ���Ϣ
*   IoMarkIrpPending -- 
*   IoSetCompletionRoutine -- ������ɻص�����
*   IoSkipCurrentIrpStackLocation -- ������ǰջ�ռ�
* 
*   IoCompleteRequest(, IO_NO_INCREMENT) -- ����IRP���������(WriteFile)��ͬ���������Irpʱ�ỽ��֮ǰ��������ͬ�������߳�
*   IoCreateSymbolicLink -- �ڶ���������д���һ��"��������"
*   IoDeleteSymbolicLink --
*
* �����������豸һ�������ֶ�д��ʽ��1.��������ʽ(DO_BUFFERED_IO); 2.ֱ�ӷ�ʽ; 3.������ʽ(?)
*   ��������
*     MDLAddress -- ͨ�� MDL ��Ӧ�ò�ĵ�ַ�ռ�ӳ�䵽�ں˿ռ�
*     UserBuffer -- ��׷��Ч�ʵĽ��������Ӧ�ò�Ļ�������ֱַ�ӷ������У����ں˿����з��ʡ�ֻ�����ڵ�ǰ���̺ͷ����������һ�µ������
*     AssociatedIrp.SystemBuffer -- ���ڱȽϼ��Ҳ�׷��Ч������µĽ������(��R3���ڴ�ռ��еĻ������ݿ�����R0)
*   ͨ�õĶ�д���ݷ�ʽ(TODO:û�б�־ָ��ʹ�õľ��建������):
*     if(irp->MdlAddress != NULL) buffer = (PBYTE)MmGetSystemAddressForMdlSafe(irp->MdlAddress);
*     else buffer = (PBYTE)irp->UserBuffer;
*     if(buffer == NULL) buffer = (PBYTE)irp->AssociatedIrp.SystemBuffer;
* 
* IRP����
******************************************************************************************************************/

/******************************************************************************************************************
* CONTAINING_RECORD -- ͨ���Զ������ݽṹ��ĳһ��������ָ��õ�ָ������ݽṹ��ָ�롣
******************************************************************************************************************/

/******************************************************************************************************************
* DDK����������Աʹ��C���Ե��ַ�������Ҫ�����׳��ֻ������������������Աʹ�� STRING/ANSI_STRING/UNICODE_STRING �ȴ����ȵĽṹ
* ASCII�ַ���(ANSI_STRING) -- 
* ���ַ���(UNICODE_STRING) = RTL_CONSTANT_STRING(L"xxx")
*   �ַ�������ʱ��Rtl-����(�� RtlInitUnicodeString��)
*
* <ntstrsafe.h> -- �ַ��������� ��Ҫ����Win2K����Ҫ�ȶ��� NTSTRSAFE_LIB �ꣿ
*   RtlInitAnsiString -- ��ʼ�� ANSI_STRING �ṹ�壬���ַ��� Buffer ָ���ָ���ʼ�����ַ���(�������䣬�����������ڴ�)
*   RtlInitUnicodeString -- ��ʼ�� UNICODE_STRING �ṹ�壬ע�⣺�ýṹ�岻Ҫ��ʹ��NULL��Ϊ�ַ����Ľ���(ʹ��ͨ��NULL�жϽ������ַ����������������)
*   RtlStringCchPrintfW
*   RtlFreeAnsiString/RtlFreeUnicodeString -- ������ ExAllocatePool �ֹ������ڴ���ַ���(��ע��������RtlInit��ʼ�����ַ���)
*   RtlUnicodeStringToInteger/RtlIntegerToUnicodeString -- �ַ��� �� ��������ת��
*
* �ں�̬����ʱ����(RtlXxx)
*   RtlCopyMemory -- �����ص����ڴ�临��
*   RtlMoveMemory -- ���ص����ڴ�临��(�����ƶ�����ǰ���ڴ�ᱻ������)
*   RtlCompareMemory/RtlEqualMemory -- �ڴ�Ƚ�
*   
* ���ڴ����
*   ExAllocatePool/ExFreePool -- ָ������ ��ҳ�ڴ�/�Ƿ�ҳ�ڴ�, �ڴ��С����� 4/8 �ı���
*   ExAllocatePoolWithTag/ExFreePoolWithTag -- �������4�ֽڵı�ǩ������ʱ���ҳ��Ƿ��б��������ǩ���ڴ�û�б��ͷ�
*   ExAllocatePoolWithQuota	-- ��������
*   ExAllocatePoolWithQuotaTag
*
* Lookaside �����ڴ�ն�(�����ڣ�ÿ������̶���С���ڴ棻������ͷŵĲ���ʮ��Ƶ��)
*   ��ʶʱ��ϵͳ����һ��ϴ���ڴ棬�Ժ�ÿ������ʱ�����л��֣��������ܵر�������ڴ�ն�������Ч�ʸߡ�
*   Win2K���ϵͳLookaside��Ϊ Paged �� NPaged ���ຯ����Vista ���ϵͳͨ��һ�׺���(ExInitializeLookasideListEx)ͳһʵ��
*   ExInitializeNPagedLookasideList/ExInitializePagedLookasideList -- ��ʼ�� Lookaside
*   ExDeleteNPagedLookasideList/ExDeletePagedLookasideList -- �ͷ�Lookaside
*   ExAllocateFromNPagedLookasideList/ExAllocateFromPagedLookasideList -- ��Lookaside�����ڴ�
*   ExFreeToNPagedLookasideList/ExFreeToPagedLookasideList -- �����ڴ�
*
*   
*   
* ����(����)
*   ��������(Nextָ����һ��) -- 
*   ˫������(Blink/Flink) -- �� LIST_ENTRY ��Ϊ��һ��Ԫ�أ�������ÿ��Ԫ�ص��������ͣ���ͨ�� CONTAINING_RECORD ���ñ�����ַ
*     InitializeListHead -- ��ʼ������ͷ��Flink��Blink��ָ���Լ�����ʾΪ����
*     IsListEmpty -- �ж��Ƿ��ǿ���
*     InsertHeadList/InsertTailList -- ��ͷ����β������Ԫ��
*     RemoveEntryList -- ֱ���Ƴ�Ԫ��
*     RemoveHeadList/RemoveTailList -- ��ͷ����β��ɾ��Ԫ��
*
* �ļ�����( Zw ϵ�к���)���ļ��������Ƿ�������("\??\C:\Somefile.txt")���豸��("\Device\HarddiskVoluem1\Somefile.txt")
*   ZwCreateFile/ZwOpenFile  -> ZwClose
*   ZwSetInformationFile
*
* ע������
*   ͨ��Rtl����ʱ���������˷�װ�����˲���
*     RtlCreateRegistryKey/RtlWriteRegistryValue ��
*   ZwCreateKey/ZwOpenKey
*   ZwSetValueKey/ZwQueryValueKey
*   ZwQueryKey(ö������)/ZwEnumerateKey(ö���ӽ�)
******************************************************************************************************************/

/******************************************************************************************************************
* Driver
*   IoCallDriver -- ���豸��������?
*
* Device
*   IoAttachDevice/IoDetachDevice  -- ��/������豸���豸ջ�ϣ���������������(Filter)
*   IoAttachDeviceToDeviceStack -- WDM��IoCreateDevice����FDO����ҪAttach��PDO��
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