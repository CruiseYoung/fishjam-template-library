#ifndef DRIVER_FILTER_H
#define DRIVER_FILTER_H

/******************************************************************************************************************
* ����(Filter) -- ���豸ջ�п������Ƚ��д���(��ʵ���ļ����ˡ�ʵʱ�����ȹ���)
*   1.����һ��������豸����
*     IoCreateDevice -- 
*   2.��Attach����ʵ���豸��
*     UNICODE_STRING com_device_name = RTL_CONSTANT_STRING(L"\\Device\\Serial0");
*     IoAttachDevice(myFilterDevice, &com_device_name, &attached_device);	//�󶨾������ֵ��豸
*     //IoAttachDeviceToDeviceStackSafe -- �����豸�����ָ���
* 
*
******************************************************************************************************************/


#endif //DRIVER_FILTER_H