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
*
* ��Ϊ Buffered IO �� Direct IO
*
* NtCreateFile -- �ϰ汾��ͷ�ļ����Ѿ����ٰ�������ʵ�ʴ��ڣ���Zw-�Ǽ򵥵Ķ�Ӧ��ϵ��
* ZwCreateFile -- ��ǰ���ڲ����ļ��ĺ���
*
* IoCreateFile -- �� Zw-���ײ��IO����
* IoCreateDevice -- ����һ���豸����
* IoCallDriver -- ��������
******************************************************************************************************************/

#endif //F_DRIVER_IO_H