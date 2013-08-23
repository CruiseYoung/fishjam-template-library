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
*
* ע�⣺
*   1.�ں����кܶຯ����δ�ĵ�����ͷ�ļ���û�ж����(����ʵ�ʵ��ļ��д���)��ͨ����˵ֻҪ������Ϳ���ʹ�á�
*     
******************************************************************************************************************/

/******************************************************************************************************************
* Ӧ�ó���һ��ͨ�� �ļ���ʽ( �� WriteFile / DeviceIoControl ) ���������� -- I/O�������������û�ģʽ������豸��������֮��Ľӿڡ�
* ���ж��豸�Ĳ���(����˿ڵĶ�д�����̷��ʡ������ļ��Ĳ�����)���ս�ת��ΪIRP����,��ЩIRP����ᱻ���͵���ǲ��������
* 
* Ϊ��ͬʱ֧��32/64����������������ʹ��IOCTL���ݰ�����ָ��Ľṹ������봫��(�� HANDLE ��)��Ҫ�ر���
*   1.ͨ�� IoIs32bitProcess �ж��ϲ�����ߵĳ������ͣ����ǵĻ�������ͨ�� VOID* POINTER_32 ת��(�μ� PROTECT_WND_INFO_32 )
*   3.��64λ�����в����µ�IOCTL����
*
* ���������е��豸����ֻ�����ں�״̬�ɼ�, Ӧ�ó�����ֱ�Ӹ����豸�����豸,Ӧ�ó���Ҫʶ���豸�����ַ�����
*   1.ͨ�� ��������(IoCreateSymbolicLink) �ҵ��豸����ͨ�� "\??\C:" �ҵ� "/Device/HarddiskVolume1"
*     �ں�ģʽ�£����������� "\??\" ���� "\DosDevices\" ��ͷ���� "\??\C:"
*     �û�ģʽ�£����������� "\\.\" ��ͷ�� �� "\\.\C:"
*   2.ͨ���豸�ӿ��ҵ��豸
*
* ZwOpenSymbolicLinkObject -- ���ں�ģʽ�ķ������ӣ��� L"\\??\\C:"
* ZwQuerySymbolicLinkObject -- ��ȡ�������Ӷ�Ӧ���豸��Ϣ
* 
* �����е�����������  
*   1.ʹ���ļ������ʽ(����App��������) -- ZwCreateFile(SYNCHRONIZE ͬ��) -> ZwReadFile ��
*   2.IoGetDeviceObjectPointer -- ����豸ָ��
******************************************************************************************************************/

/******************************************************************************************************************
* �ں˶���
* 
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
* ͬ��IRP -- ��ǲ������ֱ�Ӵ������� IoCompleteRequest ��������IRP����( ���ڲ����� IRP.UserEvent )
* �첽IRP -- ��ǲ�����б�����Ϣ(��ͨ�� IoStartPacket ����StartIo����)������ IoMarkIrpPending ������֪����ϵͳ��IRP���ڹ���״̬�������� STATUS_PENDING��
*            ���������Ч�ʣ���ͨ����Ҫ����ͬ������(�� StartIO/�жϷ������̵�)����ֹ�����߼��ϵĴ���
* 
* IO_STACK_LOCATION -- IO��ջ����Ӧ�豸��ջ��ÿ���豸�����Ĳ����������豸��Ӧ��ֵ��ͨ�� IoGetCurrentIrpStackLocation ���
*   Parameters�����Ǻܶ�ṹ���union����Ҫ���ݾ���� ���ܺ� ѡ���Ӧ�ĳ�Ա�������д���   
*
*   IoAllocateIrp -- �ֹ������������͵�IRP(�����ֹ�������ʽ�ĵײ�ʵ��)��������Ϻ���Ҫ�ֶ����IRP�������˳�ʱ��Ҫͨ�� IoFreeIrp ɾ��
*   IoBuildSynchronousFsdRequest/IoBuildAsynchronousFsdRequest -- �ֹ����� ͬ��/�첽 IRP
*   IoBuildDeviceIoControlRequest -- �ֹ�����ͬ�����͵� IRP_MJ_INTERNAL_DEVICE_CONTROL �� IRP_MJ_DEVICE_CONTROL
*   IoCallDriver -- ���豸��������ͨ�������豸��ջ���²��豸��������
*   IoCancelIrp -- ȡ��ָ����IRP�������ڲ���ʹ�ý��� cancel ������������ͬ������Ҫ�� IoSetCancelRoutine �趨��
*     ȡ���ص������е��� IoReleaseCancelSpinLock(Irp->CancelIrql) �ͷŸ���������
*     ��ͨ��DPC��ʱ�����ó�ʱ�����øú���ȡ��IRP����ֹ����Ӧ��
*   IoCompleteRequest(, IO_NO_INCREMENT) -- ����IRP���������(WriteFile)��ͬ���������Irpʱ�ỽ��֮ǰ��������ͬ�������߳�
*   IoCopyCurrentIrpStackLocationToNext -- �ѵ�ǰIRPջ�ռ俽������һ��ջ�ռ�,��Ҫ�� IoSetCompletionRoutine ���ã��������̹���(�·��������ڶ�����������)
*   IoGetCurrentIrpStackLocation -- ��õ�ǰջ�ռ���Ϣ
*   IoMarkIrpPending -- ��֪����ϵͳ��IRP���ڹ���״̬
*   IoSetCancelRoutine -- ����ȡ��IRP����Ļص�����������IRP��ȡ��ʱ������ϵͳ����ø�ȡ������
*   IoSetCompletionRoutine -- ������ɻص�����
*   IoSkipCurrentIrpStackLocation -- ������ǰջ�ռ�(�����ǽ���ǰIO��ջ�����ƶ�һ��?)�������ڵ�ǰ�豸����Ҫ��ӦI/O��ջʱ
*   IoStartNextPacket -- �Ӷ�����ȡ����һ��IRP������StartIo
*   IoStartPacket -- ����StartIo������IRP����ȴ�����
*   IoCreateSymbolicLink -- �ڶ���������д���һ��"��������"
*   IoDeleteSymbolicLink --
*   KeRemoveEntryDeviceQueue -- 
* 
* IRP����
*
* �ֶ�����IRP
*   1.�õ��豸ָ�� -- IoGetDeviceObjectPointer �� ObReferenceObjectByPointer ��
*   2.�ֹ�����IRP  -- IoBuildSynchronousFsdRequest / IoAllocateIrp ��
*   2.����IRP��I/O��ջ��ÿ��IO��ջ��Ӧһ���豸����, IoGetNextIrpStackLocation ?
*   3.ͨ�� IoCallDriver ������Ӧ������
******************************************************************************************************************/

/******************************************************************************************************************
* CONTAINING_RECORD -- ͨ���Զ������ݽṹ��ĳһ��������ָ��õ�ָ������ݽṹ��ָ�롣
******************************************************************************************************************/

/******************************************************************************************************************
* DDK����������Աʹ��C���Ե��ַ�������Ҫ�����׳��ֻ������������������Աʹ�� STRING/ANSI_STRING/UNICODE_STRING �ȴ����ȵĽṹ
*
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
* ����(����) -- ��ͨ�� CONTAINING_RECORD ��� LIST_ENTRY ��������û�����ṹ������ĵ�ַ
*   ��������(Nextָ����һ��) -- 
*   ˫������(Blink/Flink) -- �� LIST_ENTRY ��Ϊ��һ��Ԫ�أ�������ÿ��Ԫ�ص��������ͣ�
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
*
* ϵͳ����
*   MmGetSystemRoutineAddress -- ���Եõ����Ĳ�ĺ���ָ��?
*   KeUserModeCallback -- Ring0����Ring3�ĺ����������ķ�ʽ���� APC ��
*
* �ں˶�������ָ�� -- ÿ���ں��еľ�������һ���ں˶����ָ����ϵ�������ɻ���ת��
*   IoGetDeviceObjectPointer -- ͨ���豸������豸ջ�����豸���ļ�����
*
* ��������
*   PtrToUlong/PtrToLong -- �ض�ָ�룬 ���Ҫ��ָ���ֵ����� INT ��ֵ����Ҫʹ�� INT_PTR ��
******************************************************************************************************************/

/******************************************************************************************************************
* Driver
*
* Device
*   IoAttachDevice/IoDetachDevice  -- ��/������豸���豸ջ�ϣ���������������(Filter)
*   IoAttachDeviceToDeviceStack/IoAttachDeviceToDeviceStackSafe -- �����ڷֲ��������أ��磺WDM��IoCreateDevice����FDO����ҪAttach��PDO��
*   IoCreateDevice/IoDeleteDevice
*   IoEnumerateDeviceObjectList -- ö��һ�������µ������豸����
*   IoGetAttachedDevice -- ���һ���豸���ڵ��豸ջ��˵��Ǹ��豸
*   IoGetDeviceObjectPointer -- �����豸���ֻ���豸�����ָ��,��Ҫʹ�� ObDereferenceObject ���ļ����������÷�ֹй¶
*   IoSetDeviceInterfaceState
* 
* Other
*   ObDereferenceObject
*   ObReferenceObjectByName(ͷ�ļ���û������?) -- ͨ��һ�����ֻ��һ�������ָ�룬ʹ����Ϻ���Ҫͨ�� ObDereferenceObject �����ü�����1
*
* ʱ����صĺ���
*   ʱ�������
*     С��0��ʱ��ֵ����ӵ�ǰ��ʼ�����ʱ��(100ns�ı���)
*     ����0��ʱ��ֵ����� 1601-01-01 ��ʼ�ľ��Ե�ʱ��(100ns�ı���)
*   ��ʱ�� -- ��Ϊ IO��ʱ�� �� DPC��ʱ��
*     IoInitializeTimer/IoStartTimer/IoStopTimer -- IO��ʱ��,ϵͳÿ1s����һ�ζ�ʱ������(������ DISPATCH_LEVEL),
*       �����������̣߳���˲���ֱ��ʹ��App�е��ڴ��ַ
*     KeInitializeTimer/KeSetTimer/KeCancelTimer -- DPC��ʱ����ͨ��DPC����ִ�У��ɶ�������ʱ����ж�ʱ��
*       ÿһ�� KeSetTimer ֻ�ᴥ��һ��,��Ҫ�����Դ���DPC���̣���Ҫ�ڴ������������Ե��� KeSetTimer.
* 
*   KeDelayExecutionThread -- ǿ�Ƶ�ǰ�߳̽���˯��״̬������ָ����˯��ʱ����ָ̻߳����С�
*   KeStallExecutionProcessor -- ��CPU����æ��״̬������ָ��ʱ��󣬼������߳�����(���ڲ������˯�ߣ���˲��ᷢ���̵߳��л�����ʱ�ȽϾ�ȷ)
*   KeQuerySystemTime -- ��ȡ��ǰϵͳʱ��(��  1601-01-01 ��ʼ�ĸ�������ʱ��)
*   ExSystemTimeToLocalTime -- ��ϵͳʱ��ת��Ϊ��ǰʱ����Ӧ��ʱ��
*   RtlTimeToTimeFields -- ��ϵͳʱ��õ�����������յ���Ϣ
*
* ��Դ(Power)���?
*   PoStartNextPowerIrp -- 
*   PoCallDriver -- ??
******************************************************************************************************************/
/******************************************************************************************************************
* KeDelayExecutionThread(KernelMode, FALSE, &interval) -- ��ʱָ��ʱ��(ԭ��? �����̻߳����ִ��?)
* KeInitializeSpinLock / KeQueryInterruptTime
* KsGetCurrentThread --
* 
******************************************************************************************************************/

/******************************************************************************************************************
* WDF
*   WdfDriverCreate -- 
******************************************************************************************************************/

#ifdef __cplusplus
extern "C" 
{
#endif


//����δ�ĵ������ҳ��õ�һЩAPIԭ��
NTSTATUS ZwQuerySystemInformation( 
								  IN ULONG SystemInformationClass, 
								  IN PVOID SystemInformation, 
								  IN ULONG SystemInformationLength, 
								  OUT PULONG ReturnLength);

NTKERNELAPI 
NTSTATUS ObReferenceObjectByName(
                                 IN PUNICODE_STRING ObjectName,                 //���豸���豸������ L"\\??\\HelloDDK"
                                 IN ULONG Attributes,                           //һ������Ϊ OBJ_CASE_INSENSITIVE
                                 IN PACCESS_STATE PassedAccessState OPTIONAL,   //�����ã�һ����Ϊ NULL
                                 IN ACCESS_MASK DesiredAccess OPTIONAL,         //Ȩ�ޣ�һ��Ϊ FILE_ALL_ACCESS
                                 IN POBJECT_TYPE ObjectType,                    //ָ���������ͣ�IoDeviceObjectType ��ʾ��Ҫ����豸�����ָ��
                                 IN KPROCESSOR_MODE AccessMode,                 //���ں��з��ʸ�ָ�룬��Ϊ KernelMode
                                 IN OUT PVOID ParseContext OPTIONAL,            //һ����Ϊ NULL
                                 OUT PVOID *Object                              //���ص��ں˶���ָ��
                                 );

extern POBJECT_TYPE IoDeviceObjectType;

//extern "C" NTSTATUS ZwDuplicateObject(
//						   IN HANDLE                 SourceProcessHandle,
//						   IN PHANDLE                 SourceHandle,
//						   IN HANDLE                 TargetProcessHandle,
//						   OUT PHANDLE               TargetHandle,
//						   IN ACCESS_MASK             DesiredAccess OPTIONAL,
//						   IN BOOLEAN                 InheritHandle,
//						   IN ULONG                   Options );


#ifdef __cplusplus
}
#endif


//ntifs.h ��

#endif //F_DRIVER_API_H