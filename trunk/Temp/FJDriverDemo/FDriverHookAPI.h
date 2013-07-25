#ifndef F_DRIVER_HOOK_API_H
#define F_DRIVER_HOOK_API_H
#pragma once

//#include "WindowsTypes.h"

//TODO: Depends �鿴 ntkrnlpa.exe + win32k.sys ���Ƚ�����ֵ
//SoftICE for Windows NT/2000��ntcall������Խ���ЩSystem Service��ʾ�� -- http://dev.21tx.com/2005/03/14/12553.html
//Hook NtGdiBitBlt  -- http://www.osronline.com/showthread.cfm?link=187950

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
* NtGdiBitBlt
* 
******************************************************************************************************************/

/******************************************************************************************************************
* SSDT(System Service Descriptor Table) -- ϵͳ�����������Ǹ���ϵͳ����ŵ� ���� => ��Ӧϵͳ�������ĵ�ַ��
*   ��ring3��Win32 API��ring0���ں�API��ϵ������
*   ͨ���޸Ĵ˱�ĺ�����ַ���ԶԳ���windows������API����hook���Ӷ�ʵ�ֶ�һЩ���ĵ�ϵͳ�������й��ˡ���ص�Ŀ��
*   TODO: ��ô�õ�һ��������SSDT������(dumpbin �� depends ?)
*         http://www.cnblogs.com/nlsoft/archive/2013/04/02/2994628.html
*
*   ϵͳ�ڲ��и���Ϊ KeServiceDescriptorTable �ı����������������Ϣ����ͨ�� extern �����
*     �� WinDbg ��: dd KeServiceDescriptorTable
******************************************************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

#define   CR0_WP_MASK     0x0FFFEFFFF

#define SSDT_API_CALL_ENTER(x) (InterlockedIncrement(&x))
#define SSDT_API_CALL_LEAVE(x) (InterlockedDecrement(&x))

typedef struct _SYS_SERVICE_TABLE { 
	void **ServiceTable; 
	unsigned long CounterTable; 
	unsigned long ServiceLimit; 
	PUCHAR ArgumentsTable; 
} SYS_SERVICE_TABLE, *PSYS_SERVICE_TABLE;

typedef void *HWND;
typedef void *HDC;
typedef void *HRGN;

typedef unsigned int UINT;
//typedef short unsigned int WORD;
typedef unsigned char BYTE, *PBYTE;

#pragma push(1)
typedef struct _RECT { 
	LONG left; 
	LONG top; 
	LONG right; 
	LONG bottom; 
} RECT, *PRECT;
typedef RECT *LPRECT;

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
//int GetScrollDCIndex(SYS_SERVICE_TABLE *p);

void InstallCopyProtectHook(HANDLE hProcess);
void UnInstallScrollHook(void);

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

