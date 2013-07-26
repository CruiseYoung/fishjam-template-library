#ifndef F_DRIVER_HOOK_API_H
#define F_DRIVER_HOOK_API_H
#pragma once

//#include "WindowsTypes.h"

//TODO: Depends �鿴 ntkrnlpa.exe + win32k.sys ���Ƚ�����ֵ
//SoftICE for Windows NT/2000��ntcall������Խ���ЩSystem Service��ʾ�� -- http://dev.21tx.com/2005/03/14/12553.html
//Hook NtGdiBitBlt  -- http://www.osronline.com/showthread.cfm?link=187950
//http://bbs.pediy.com/showthread.php?t=56955

/******************************************************************************************************************
* ��Windows����ϵͳ�У���Ϣ���ӣ�ͨ��SetWindowsHookEx���ã�ֻ�ᵱǰ�������ϵĴ�����Ч��
* ���Կ��Խ���һ����ȫ���棬����������Ҫ���ܱ����Ľ��̣������Ǳ����������еĳ����޷�ͨ����Ϣ���ӵķ�ʽ��
* ��ȡ��Ҫ�������̴��ڵ���Ϣ���ﵽ�˱���Ŀ����̴��ڵ�Ŀ��(360������ͽ�ɽ�ܱ� �� ��ȫ���湦�� )
******************************************************************************************************************/

/******************************************************************************************************************
* �������� HOOK API( ͨ������ SSDP �еĺ�����ַ�� Hook) -- 
*   ע���ȿ������ߵ��������ء�http://download.csdn.net/detail/jyw1111/5275135
*   ȱ�㣺���ܱ������������Ϊ rootkit ����
*
* ���жϣ�
*   Win2K -- int 2eh
*   WinXp -- sysenter
*
* Native API��
*   ntdll.dll -- Native API �Ľӿڣ�ͨ�����жϷ�ʽ�����ô����ں�������ϵͳ���񣬵���������Ring 3��
*     ������һ���Ƕ�ӦWin32APIǰ�� Nt ������ĸ��
*     1.�Ѷ�Ӧ�Ĺ��ܱ�Ŵ���eax�Ĵ���,
*     2.ʹ�� SYSENTER ָ��(����ʹ�ô��� Int 2e �жϵķ�ʽ)����"����"
*     3.KiSystemService �ں˴����������
*     4.��SDT���ҵ����ܱ�ŵĶ�Ӧ����λ�ã�������
*       SDT��ϵͳ��һ����4��()��
*       0.SSDT -- λ�� ntoskrnl.exe �� ntkrnlpa.exe �У���¼��ϵͳ�ؼ����ܵ�λ��,�漰�ļ�����,ע������,���̲����ȡ�TODO��Ϊʲô������������
*       1.Shadow SSDT -- λ�� win32k.sys �У� ��¼�˺��û���ͼ�ν����йصĹ��ܡ�
*       2.3 -- Ԥ����SDT�������˹�����SDT�Ŀ��ܣ�����������û������
*     5.Object Hook -- ���ײ�Ľӿڣ������Ѿ����ѿ�����
*
* ntkrnlpa.exe (SSDT?) -- Kernel32.Dll ���ں�ʵ��
* ntoskrnl.exe -- Windowsִ�������
* win32k.sys (Shadow SSDT?) -- User32.dll + Gdi32.DLL ���ں�ʵ��
*   ��GUI��ʵ�ַ����ں�ģʽ��������ϵͳ���ȶ��ļ��ʡ����������ͼ�δ��������Ч�ʡ�
*
* KeUserModeCallBack -- ��R0����λ��Ring3�ĺ���(ϵͳ���е���Ϣ���ӻص��������øú�����ɵ�)
* 
* NtGdiBitBlt
NtGdiOpenDCW 
NtGdiDeleteObjectApp
NtGdiBitBlt
NtGdiStretchBlt
NtUserPrintWindow
NtGdiDdLock
NtGdiPlgBlt
NtGdiMaskBlt
NtGdiTransparentBlt
NtGdiCreateDIBSection
NtGdiAlphaBlend
NtGdiGetPixel
NtUserSendInput
NtOpenSection
NtGdiBitBlt��NtGdiMaskBlt��NtGdiPlgBlt��NtGdiStretchBlt��NtUserBuildHwndList��NtUserFindWindowEx
******************************************************************************************************************/

/******************************************************************************************************************
*      System Services Descriptor Table
* SSDT(System Service Descriptor Table) -- ϵͳ�����������Ǹ���ϵͳ����ŵ� ���� => ��Ӧϵͳ�������ĵ�ַ��
*   ��Ring3��Win32 API��Ring0���ں�API��ϵ������
*   ͨ���޸Ĵ˱�ĺ�����ַ���ԶԳ���windows������API����hook���Ӷ�ʵ�ֶ�һЩ���ĵ�ϵͳ�������й��ˡ���ص�Ŀ��.
*   TODO: ��ô�õ�һ��������SSDT������(dumpbin �� depends ?)
*         http://www.cnblogs.com/nlsoft/archive/2013/04/02/2994628.html
* Shadow SSDT(Shadow System Services Descriptor Table) -- ��ͼ�Ρ��û���صĺ���(gdi32.dll,user32.dll)
*
*   ϵͳ�ڲ��и���Ϊ KeServiceDescriptorTable �ı����������������Ϣ����ͨ�� extern �����
*     �� WinDbg ��: dd KeServiceDescriptorTable
*
* ��SSDT����Hook�Ĺ���
*   1.�ر�CR0д����(�ı�CR0�Ĵ�����WPλ)��
*   2.1.Hook Shadow SSDT ֮ǰ�����뽫�����������л���CSRSS����
*   2.2.���µĺ�����ַ�滻ԭ��SSDT�еĺ�����ַ
*   3.�ָ�CR0д����
*
* ���ڱ��� NtUserFindWindowEx��NtUserGetForegroundWindow��NtUserBuildHwndList��NtUserQueryWindow��
*          NtUserWindowFromPoint��NtUserSetParent��NtUserPostMessage��NtUserMessageCall��
*          NtUserSetWindowLong��NtUserShowWindow��NtUserDestroyWindow��NtUserCallHwndParamLock
* ģ�ⰴ�� NtUserSendInput
* ��ȡ���̰���״̬ NtUserGetAsyncKeyState
* �򿪰�ȫ���� NtUserOpenDesktop
* ����ٰ�����ԭ����ʵ�İ��� NtUserTranslateMessage
* �������� NtGdiBitBlt��NtGdiStretchBlt
******************************************************************************************************************/

/******************************************************************************************************************
* ϵͳ���̷���
*   csrss.exe -- �ᱣ�����н��̵ľ����
*     Console����� ScrollDC ��API��ҪHook������̲��У�
*     win32!RawInputThread ͨ�� GUID_CLASS_KEYBOARD ��ü����豸ջ��PDO�ķ���������
*   System -- ϵͳ���̣���ϵͳ����ʱ������
*     ���������ص�ʱ��ϵͳ���������µ��̣߳����ö��������������һ����������(DRIVER_OBJECT)��Ȼ������� DriverEntry 
******************************************************************************************************************/

/******************************************************************************************************************
*             Ring3(kernel32)                    |   Native API(ntdll!)     |       Ring0(nt!)          |
* OpenProcess                                    | NtOpenProcess            | NtOpenProcess             |
* NtDuplicateObject                              |                          |                           |
* CreateThread/CreateRemoteThread                | NtCreateThread           | CreateThread              |
* OpenThread                                     |                          | NtOpenThread              |
* WriteProcessMemory                             | NtWriteVirtualMemory     | NtWriteVirtualMemory      |

* ------------------------------------------------ Shadow SSDT -----------------------------------------|
* FindWindow                                     |                          | NtUserFindWindowEx        |
* GetForegroundWindow                            |                          | NtUserGetForegroundWindow |
* EnumWindows                                    |                          | NtUserBuildHwndList       |
* GetWindowThreadProcessId                       |                          | NtUserQueryWindow         |
* WindowFromPoint                                |                          | NtUserWindowFromPoint     |
* SetParent                                      |                          | NtUserSetParent           |
* PostMessage                                    |                          | NtUserPostMessage         |
* SendMessage                                    |                          | NtUserMessageCall         |
* SetWindowLong                                  |                          | NtUserSetWindowLong       |
* ShowWindow                                     |                          | NtUserShowWindow          |
* DestroyWindow                                  |                          | NtUserDestroyWindow       |
* EnableWindow                                   |                          | NtUserCallHwndParamLock   |
* BitBlt                                         |                          | NtGdiBitBlt               |
* StretchBlt                                     |                          | NtGdiStretchBlt           | 
******************************************************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

#define SSDT_API_CALL_ENTER(x) (InterlockedIncrement(&x))
#define SSDT_API_CALL_LEAVE(x) (InterlockedDecrement(&x))

//SSDT��ṹ
typedef struct _SYS_SERVICE_TABLE { 
	void **ServiceTable;					//SSDT���ڴ��еĻ���ַ(��ÿһ����Ԫ�ؾ��Ǹ���Rin0�ĺ�����)
	unsigned long CounterTable; 
											//������ SSDT ��ÿ�����񱻵��ô����ļ����������������һ����sysenter ����
	unsigned long uNumberOfServices;		//SSDT��ĸ������� CounterTable �����ķ������Ŀ
	PUCHAR ArgumentsTable;					//TODO:ʵ�����ǲ�������? ����ÿ��ϵͳ��������ֽ�����Ļ���ַ-ϵͳ���������
} SYS_SERVICE_TABLE, *PSYS_SERVICE_TABLE;

typedef struct _SCROLL_HOOK_TARGET
{
	HWND hTargetWindow;
	HANDLE hSelfProcess;
	HANDLE hTargetProcess;
} SCROLL_HOOK_TARGET, *PSCROLL_HOOK_TARGET;

typedef struct _SCROLL_DATA
{
	int dy;
	RECT rcScroll;
} SCROLL_DATA, *PSCROLL_DATA;
#pragma push(0)

SYS_SERVICE_TABLE *GetServiceDescriptorShadowTableAddress();
//int GetNtGdiBitBltIndex(SYS_SERVICE_TABLE *p);

void InstallCopyProtectHook(HANDLE hProcess);
void UnInstallScrollHook(void);

typedef struct _SYSTEM_HANDLE_INFORMATION{
	ULONG ProcessID;                //���̵ı�ʶID
	UCHAR ObjectTypeNumber;         //��������
	UCHAR Flags;                    //0x01 = PROTECT_FROM_CLOSE,0x02 = INHERIT
	USHORT Handle;                  //����������ֵ
	PVOID  Object;                  //��������ָ���ں˶����ַ
	ACCESS_MASK GrantedAccess;      //�������ʱ��׼��Ķ���ķ���Ȩ
}SYSTEM_HANDLE_INFORMATION, * PSYSTEM_HANDLE_INFORMATION;

typedef struct _SYSTEM_HANDLE_INFORMATION_EX
{
	ULONG NumberOfHandles;
	SYSTEM_HANDLE_INFORMATION Information[1];
}SYSTEM_HANDLE_INFORMATION_EX, *PSYSTEM_HANDLE_INFORMATION_EX;

//typedef enum _OBJECT_INFORMATION_CLASS{
//	ObjectBasicInformation,
//	ObjectNameInformation,
//	ObjectTypeInformation,
//	ObjectAllInformation,
//	ObjectDataInformation
//}OBJECT_INFORMATION_CLASS, *POBJECT_INFORMATION_CLASS;

#ifdef __cplusplus
}
#endif


#if 0
//SSDT��ṹ
typedef struct _ServiceDescriptorTable {
	PULONG ServiceTableBase;			//System Service Dispatch Table �Ļ���ַ  
	PULONG ServiceCounterTableBase; 
	//������ SSDT ��ÿ�����񱻵��ô����ļ����������������һ����sysenter ����
	ULONG NumberOfServices;				//�� ServiceTableBase �����ķ������Ŀ
	PBYTE ParamTableBase;				//TODO:ʵ�����ǲ�������? ����ÿ��ϵͳ��������ֽ�����Ļ���ַ-ϵͳ���������  
}*PServiceDescriptorTable;
//��SSDT�����Ż�ȡ��ǰ������ַ
//NtOpenProcess  [[KeServiceDescriptorTable]+0x7A*4]
extern "C" PServiceDescriptorTable KeServiceDescriptorTable;


����(�ƺ�û�п��� _SERVICE_DESCRIPTOR_TABLE �Ķ���)
typedef struct _SERVICE_DESCRIPTOR_TABLE *PSERVICE_DESCRIPTOR_TABLE;
xtern PSERVICE_DESCRIPTOR_TABLE KeServiceDescriptorTable;
#endif

#endif //F_DRIVER_HOOK_API_H

