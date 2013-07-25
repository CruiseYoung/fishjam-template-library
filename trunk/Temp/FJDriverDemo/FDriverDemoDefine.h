#ifndef F_DRIVER_DEMO_DEFINE_H
#define F_DRIVER_DEMO_DEFINE_H

//#ifdef __cplusplus
//extern "C" 
//{
//#endif
//
//#include <ntddk.h>
//
//#ifdef __cplusplus
//}
//#endif


#define FDRIVER_DEMO_FILENAME           TEXT("FJDriverDemo.sys")

#define FDRIVER_DEMO_SERVICE_NAME       TEXT("FJDriverDemo")

//�豸�������� \\Device\\�豸�� ����ʽ�� �� ���̷�����C�̣� \\Device\\HarddiskVolume1
//  �粻֧���豸��������ʱ I/O ���������Զ�����һ��������Ϊ�豸��
#define FDRIVER_DEMO_NT_DEVICE_NAME        L"\\Device\\FJDriverDemoDevice"

//�ͻ���ʹ�õ�����������������ͨ�� CreateFile(FDRIVER_DEMO_DOS_DEVICE_NAME,..., OPEN_EXISTING ) ��ʽ������
#define FDRIVER_DEMO_DOS_DEVICE_NAME       L"\\DosDevices\\FJDriverDemo"			//L"\\\\.\\Global\\FJDriverDemo"
#define FDRIVER_DEMO_WIN2K_DEVICE_NAME		L"\\\\.\\Global\\FJDriverDemo"

//#ifndef CTL_CODE
//#  define DEFINE_CTL_CODE	1
#  define METHOD_BUFFERED 0
#  define FILE_WRITE_ACCESS         ( 0x0002 )    // file & pipe

#  define CTL_CODE( DeviceType, Function, Method, Access ) (                 \
	((DeviceType) << 16) | ((Access) << 14) | ((Function) << 2) | (Method) \
	)
//#else
//#  define DEFINE_CTL_CODE 0
//#endif 

//��  IRP_MJ_DEVICE_CONTROL �ķַ������� switch(irpStack->Parameters.DeviceIoControl.IoControlCode)
#define FDRIVER_DEV_DRV 0x00002A80
#define IOCTL_FDRIVER_INSTALL_HOOK		(ULONG) CTL_CODE(FDRIVER_DEV_DRV, 0x01, METHOD_BUFFERED, FILE_WRITE_ACCESS)
#define IOCTL_FDRIVER_UNINSTALL_HOOK	(ULONG) CTL_CODE(FDRIVER_DEV_DRV, 0x02, METHOD_BUFFERED, FILE_WRITE_ACCESS)

//#if DEFINE_CTL_CODE
//#  undef CTL_CODE
//#  undef DEFINE_CTL_CODE
////#  undef METHOD_BUFFERED
////#  undef FILE_WRITE_ACCESS
//#endif 

//#define NT4_DRIVERNAME "\\\\.\\NTProcDrv"
//#define OTHER_DRIVERNAME "\\\\.\\Global\\NTProcDrv"
//#define PROCDRVPROCESSEVENT "NTProcDrvProcessEvent"


#endif //F_DRIVER_DEMO_DEFINE_H