#ifndef F_DRIVER_FILTER_H
#define F_DRIVER_FILTER_H

/******************************************************************************************************************
* ���ɸ��豸(�������ڲ�ͬ������)���ΰ��γ�һ���豸ջ��������˵��豸�Ƚ��յ�����(IRP)��
*   �ϲ���˲�����(FiDO)
*   ������������(FDO)
*   �²���˲�����(FiDO)
*   ������������(PDO)
* 
* ����(Filter) -- ���豸ջ�п������Ƚ��д���(��ʵ���ļ����ˡ�ʵʱ�����ȹ���)
*   1.����һ��������豸����
*     IoCreateDevice -- 
*    
*   2.��Attach����ʵ���豸��
*     UNICODE_STRING com_device_name = RTL_CONSTANT_STRING(L"\\Device\\Serial0");
*     IoAttachDevice(myFilterDevice, &com_device_name, &attached_device);	//�󶨾������ֵ��豸
*     ����ϱ������ DO_DEVICE_INITIALIZING ��־����������Ҫ�ı�־
*     
*   3.���������ܺŶ�IRP���й��ˣ��õ����ݲ�����
*     for (i < IRP_MJ_MAXIMUM_FUNCTION; i++) { driver->MajorFunction[i] = myDispatch; } 
*   n.IoDetachDevice + IoDeleteDevice
*     
*
******************************************************************************************************************/

/******************************************************************************************************************
* ����(������:\\Driver\\Kbdclass, USB����:\\Driver\\Kbdhid, PS/2����: \\Driver\\i8042prt ): 
*   �жϺ�Ϊ 0x93, �˿ں�Ϊ 0x60
*   ������µ�ɨ����Ϊ X����ͬһ���������ɨ����Ϊ X+0x80
*   �������Ǵ���"��һ��������û�����"��״̬(���������û���������ϻ������ȡ��һ�����룬��û��������ȴ�)
*   KEYBOARD_INPUT_DATA �� MakeCode ����ɨ���룬 if( ɨ���� & 0x80) == 0 ){ �������� }
*
* һ����˵���ڿͲ���ȥ����һ���ܱ���ȫ������׼�ص��Ĺ����豸��������Ȼ��ͨ���޸�һ���Ѿ����ڵ���������ķַ�����
*   ��ָ����ʵ�ֹ������������Ŀ�ġ�
******************************************************************************************************************/

//�����ĵķַ�����ʵ�ַ�ʽ, ��Ҫ��ͨ���Զ���� PC2P_DEV_EXT �豸��չ�ṹ�屣����һ�㼶���豸����
#if 0
NTSTATUS myDispatchGeneral(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp)
{
	IoSkipCurrentIrpStackLocation(Irp);
	return IoCallDriver(((PC2P_DEV_EXT)DeviceObject->DeviceExtension)->LowerDeviceObject, Irp);
}
#endif //if 0

#endif //F_DRIVER_FILTER_H