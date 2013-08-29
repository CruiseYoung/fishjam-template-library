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


#if defined(_M_IX86)
#  define FDRIVER_DEMO_FILENAME           TEXT("FJDriverDemo32.sys")
#else
#  define FDRIVER_DEMO_FILENAME           TEXT("FJDriverDemo64.sys")
#endif 

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

//�Զ���� DEVICE_TYPE ? ϵͳ�Ѿ�Ԥ���ܶ�����( FILE_DEVICE_BEEP ��), ���û���������ʹ�õķ�Χ�� 0x8000(32768)~0xFFFF(65535)
#define FDRIVER_DEV_DRV 0xA812

//��������� IOCTL �� FSCTL �ı��룬���û���������ʹ�õ�function codes��Χ�� 0x800(2048)-0xFFF(4095)
//CTL_CODE( �豸����, ����, I/O�����ڴ�ʹ�÷�ʽ, �������� ) -- �����а��� <ntddk.h>�� Ӧ�ó����а��� <winioctl.h>

#define IOCTL_FDRIVER_INSTALL_HOOK		    (ULONG) CTL_CODE(FDRIVER_DEV_DRV, 0x800, METHOD_BUFFERED, FILE_WRITE_ACCESS)
#define IOCTL_FDRIVER_UNINSTALL_HOOK	    (ULONG) CTL_CODE(FDRIVER_DEV_DRV, 0x801, METHOD_BUFFERED, FILE_WRITE_ACCESS)

#define IOCTL_FDRIVER_UPDATE_PROTECT_WND    (ULONG) CTL_CODE(FDRIVER_DEV_DRV, 0x802, METHOD_BUFFERED, FILE_WRITE_ACCESS)

typedef struct _INSTALL_COPY_PROTECT_HOOK
{
    HANDLE  hTargetProcess;
}INSTALL_COPY_PROTECT_HOOK, *PINSTALL_COPY_PROTECT_HOOK;

typedef struct _PROTECT_WND_INFO
{
#if 0
	HWND hProtectWindow;
	HANDLE hSelfProcess;
	HDC    hDCDesktop;
#endif 
	HWND    hWndDeskTop;
    HDC	    hDCWndProtect;
	RECT    rcProtectWindow;
} PROTECT_WND_INFO, *PPROTECT_WND_INFO;

#if defined(_M_AMD64)

typedef struct _INSTALL_COPY_PROTECT_HOOK_32
{
    VOID*POINTER_32	    hTargetProcess;
}INSTALL_COPY_PROTECT_HOOK_32, *PINSTALL_COPY_PROTECT_HOOK_32;

typedef struct _PPROTECT_WND_INFO_32 {
    VOID*POINTER_32     hWndDeskTop;        //UlongToPtr
    VOID*POINTER_32	    hDCWndProtect;
    RECT                rcProtectWindow;
} PROTECT_WND_INFO_32, *PPROTECT_WND_INFO_32;
#endif 

//#define NT4_DRIVERNAME "\\\\.\\NTProcDrv"
//#define OTHER_DRIVERNAME "\\\\.\\Global\\NTProcDrv"
//#define PROCDRVPROCESSEVENT "NTProcDrvProcessEvent"


#endif //F_DRIVER_DEMO_DEFINE_H