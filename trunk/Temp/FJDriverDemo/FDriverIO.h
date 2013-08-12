#ifndef F_DRIVER_IO_H
#define F_DRIVER_IO_H

/******************************************************************************************************************
* �����������豸һ�������ֶ�д��ʽ,�� IoCreateDevice ��ͨ�������豸����� Flags ��ָ����
*   1.DO_BUFFERED_IO(������)��ʽ -- OS �Զ��� R3(�������̶���)/R0(����OS�ĸ�������ʹ��ͳһ���ڴ�ռ�)֮�俽���ڴ����ݣ�
*     �ŵ㣺�Ƚϼ򵥵ؽ���˽��û���ַ��������������; ȱ�㣺��Ҫ���û�ģʽ���ں�ģʽ֮�临�����ݣ�Ӱ��Ч�ʡ�������ֻ�������ڴ����ʱ��
*     ReadFile/WriteFile -- ʹ�� IRP.AssociatedIrp.SystemBuffer 
*   2.DO_DIRECT_IO(ֱ�Ӷ�д)��ʽ -- OS ���û�ģʽ�µĻ�������ס����ʹ��MDL����λ��������ں�ģʽ��ַ��ӳ��һ�Σ�ʹ���û�ģʽ���ں�ģʽ�Ļ�����ָ��ͬһ����������ڴ档
*     ʹ�� IRP.MdlAddress �ṹ�õ���˵���Ļ������ڴ�, Ȼ��ͨ�� MmGetMdlVirtualAddress(Ӧ�ó����е������ַ) / MmGetSystemAddressForMdlSafe(�ں�ģʽ�µ�ӳ���ַ) �Ⱥ�ʹ��
*   3.������ʽ(�������� DO_BUFFERED_IO Ҳ������ DO_DIRECT_IO) -- ��ǲ����ֱ�Ӷ�дӦ�ó����ṩ�Ļ�������ַ -- ��
*     ʹ�� IRP.UserBuffer(�û�ģʽ��ַ) -- ��Σ�յ�����Ч�ʡ�Ӧ�ò�Ļ�������ֱַ�ӷ������У����ں˿����з��ʡ�ֻ��������Ӧ����������ͬ�߳�������ʱ����ʹ�����ַ�ʽ��
*     ��Ҫ����С�ģ���ΪReadFile�ȿ��ܰѿ�ָ���ַ��Ƿ���ַ���ݸ���������ʹ��ǰ��Ҫ��ʹ�� try + ProbeForWrite ̽������ڴ��Ƿ�ɶ�д��������ܻ�������
* 
*   ��������
*     IoStatus.Information -- ���ر�ʾ����ʵ�ʲ����˶����ֽ�
* 
*   ͨ�õĶ�д���ݷ�ʽ(TODO:û�б�־ָ��ʹ�õľ��建������):
*     if(irp->MdlAddress != NULL) buffer = (PBYTE)MmGetSystemAddressForMdlSafe(irp->MdlAddress, xxx);
*     else buffer = (PBYTE)irp->UserBuffer;
*     if(buffer == NULL) buffer = (PBYTE)irp->AssociatedIrp.SystemBuffer;
******************************************************************************************************************/
 
/******************************************************************************************************************
* OBJECT_ATTRIBUTES -- �����з����ļ�������������ʱ��Ҫͨ��������͵ı���ָ��Ŀ���ļ�������
*   
*   
* NtCreateFile -- �ϰ汾��ͷ�ļ����Ѿ����ٰ�������ʵ�ʴ��ڣ���Zw-�Ǽ򵥵Ķ�Ӧ��ϵ��
* ZwCreateFile -- ��ǰ���ڲ����ļ��ĺ���
*
* IoCreateFile -- �� Zw-���ײ��IO����
* IoCreateDevice -- ����һ���豸����
* IoCallDriver -- ��������
******************************************************************************************************************/

#if 0
//�����������Ĵ���(ͬ����ʽ)
UNICODE_STRING otherDeviceName;
OBJECT_ATTRIBUTES objAttributes;
HANDLE hOtherDevice;
IO_STATUS_BLOCK status_block;
RtlInitUnicodeString(&otherDeviceName, L"\\Device\\OtherDeviceName");
InitializeObjectAttributes(&objAttributes, &otherDeviceName, OBJ_CASE_INSENSITIVE, NULL, NULL);
status = ZwCreateFile(&hOtherDevice, FILE_READ_ATTRIBUTES | SYNCHRONIZE, &objAttributes, &status_block, 
	NULL, FILE_ATTRIBUTE_NORMAL, FILE_SHARE_READ, FILE_OPEN_IF, FILE_SYNCHRONOUS_IO_NONALERT, NULL, 0);
#endif 

/******************************************************************************************************************
* StartIO -- ���Ա�֤�������е�IRP���л���ע�������� DISPATCH_LEVEL ���𣬺�������ʱ��Ҫ���� #pragma LOCKEDCODE ���η�
*   KDEVICE_QUEUE -- ����ϵͳ�ṩ�Ŀ�ʵ��IRP���л��Ķ��У���ʵ��Ϊ DEVICE_OBJECT::DeviceQueue,
*   ���� DRIVER_OBJECT::DriverStartIo ����ָ���Ļص�������
*
* �Զ����StartIo(��������Ҫ������Ӻͳ��Ӳ���) -- ϵͳ�����StartIoֻ��ʹ��һ�����У��Ὣ���е�IRP(�����д)����һ����д��д���
*   KeInitializeDeviceQueue
*   KeInsertDeviceQueue -- ������У�������FALSE����IRPû�в��뵽���У���Ҫ��������
*   KeRemoveDeviceQueue -- �Ӷ�����ɾ��Ԫ��
******************************************************************************************************************/

/******************************************************************************************************************
* �жϷ������(ISR)��
*   ������ʹ��ISR����Ҫ�Ȼ���ж϶���(INTERRUPT)
*   
*   IoConnectInterrupt -- ���ж϶����ISR��ϵ���������ж��ź�����ʱ����ISR����
*   KeSynchronizeExecution -- 
*
* DPC(Delayed Procedure Call) -- �ӳٹ��̵��ã���һ�����У�ϵͳ���ε��ã����� DISPATCH_LEVEL ����
*   ISR���кܸߵ�IRQL�������������е��̣߳����һ����ISR�д���������룬��������Ҫ��������Ĵ������DPC�����С�
*   KeInitializeDpc -- 
*   IoRequestDpc -- ����DPC���̵Ķ���
******************************************************************************************************************/

#endif //F_DRIVER_IO_H