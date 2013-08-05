#pragma once

#if defined(_M_IX86)
#  define PROTECT_DRIVER_FILE_NAME           TEXT("ProtDrv32.sys")
#elif defined(_M_AMD64)
#  define PROTECT_DRIVER_FILE_NAME           TEXT("ProtDrv64.sys")
#elif defined(_M_IA64)
#  error NOT SUPPORT IA64 now
#endif 

#define PROTECT_SERVICE_NAME        TEXT("ProtectService")
#define PROTECT_DEVICE_NAME         L"\\Device\\ProtDrvDevice"

//�ͻ���ʹ�õ�����������������ͨ�� CreateFile(FDRIVER_DEMO_DOS_DEVICE_NAME,..., OPEN_EXISTING ) ��ʽ������
//#define PROTECT_NT_DEVICE_NAME		L"\\\\.\\Global\\ProtDev"
#define PROTECT_DOS_DEVICE_NAME			L"\\DosDevices\\ProtDev"
#define PROTECT_NT_CONTROL_FILE_NAME	L"\\\\.\\Global\\ProtDev"



//#  define METHOD_BUFFERED 0
//#  define FILE_WRITE_ACCESS         ( 0x0002 )    // file & pipe


//�Զ���� DEVICE_TYPE , ���û���������ʹ�õķ�Χ�� 0x8000(32768)~0xFFFF(65535)
#define PROTDRV_DEVICE_TYPE 0xA934

//��������� IOCTL �� FSCTL �ı��룬���û���������ʹ�õ�function codes��Χ�� 0x800(2048)-0xFFF(4095)
//CTL_CODE( �豸����, ����, I/O�����ڴ�ʹ�÷�ʽ, �������� )

#define IOCTL_PROTDRV_INSTALL_HOOK		(ULONG) CTL_CODE(PROTDRV_DEVICE_TYPE, 0x800, METHOD_BUFFERED, FILE_WRITE_ACCESS)
#define IOCTL_PROTDRV_UNINSTALL_HOOK	(ULONG) CTL_CODE(PROTDRV_DEVICE_TYPE, 0x801, METHOD_BUFFERED, FILE_WRITE_ACCESS)

#define IOCTL_PROTDRV_SET_INFO      	(ULONG) CTL_CODE(PROTDRV_DEVICE_TYPE, 0x802, METHOD_BUFFERED, FILE_WRITE_ACCESS)
#define IOCTL_PROTDRV_CLEAR_INFO      	(ULONG) CTL_CODE(PROTDRV_DEVICE_TYPE, 0x803, METHOD_BUFFERED, FILE_WRITE_ACCESS)


typedef struct _PROTECT_WND_INFO
{
    HANDLE  hProcess;
    HWND    hWndDesktop;
    HWND    hTargetWindow;
    HDC     hFillDC;
    HBITMAP hFillBitmap;
    RECT    rcWindow;
} PROTECT_WND_INFO, *PPROTECT_WND_INFO;

