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

#  define METHOD_BUFFERED 0
#  define FILE_WRITE_ACCESS         ( 0x0002 )    // file & pipe


//��  IRP_MJ_DEVICE_CONTROL �ķַ������� switch(irpStack->Parameters.DeviceIoControl.IoControlCode)

//ͨ�ŷ��棬�� METHOD_NEITHER ����(���ã�������)�����õ�Ӧ���� BUFFERED ģʽ

//Notice: SDK �� <winioctl.h> �� CTL_CODE �Ķ���

#define FDRIVER_DEV_DRV 0x00002A80
#define IOCTL_FDRIVER_INSTALL_HOOK		(ULONG) CTL_CODE(FDRIVER_DEV_DRV, 0x01, METHOD_BUFFERED, FILE_WRITE_ACCESS)
#define IOCTL_FDRIVER_UNINSTALL_HOOK	(ULONG) CTL_CODE(FDRIVER_DEV_DRV, 0x02, METHOD_BUFFERED, FILE_WRITE_ACCESS)

#define IOCTL_FDRIVER_FILTER_DESKTOP	(ULONG) CTL_CODE(FDRIVER_DEV_DRV, 0x03, METHOD_BUFFERED, FILE_WRITE_ACCESS)
#define IOCTL_FDRIVER_UNFILTER_DESKTOP	(ULONG) CTL_CODE(FDRIVER_DEV_DRV, 0x04, METHOD_BUFFERED, FILE_WRITE_ACCESS)

typedef struct _SCROLL_HOOK_TARGET
{
	HWND hWndDeskTop;
	HWND hTargetWindow;
	HANDLE hSelfProcess;
	HANDLE hTargetProcess;
} SCROLL_HOOK_TARGET, *PSCROLL_HOOK_TARGET;


//#define NT4_DRIVERNAME "\\\\.\\NTProcDrv"
//#define OTHER_DRIVERNAME "\\\\.\\Global\\NTProcDrv"
//#define PROCDRVPROCESSEVENT "NTProcDrvProcessEvent"


#endif //F_DRIVER_DEMO_DEFINE_H