#ifndef F_DRIVER_HOOK_API_H
#define F_DRIVER_HOOK_API_H
#pragma once

//[ԭ����Դ]��WIN7 X64��Hook Shadow SSDT  --  http://www.m5home.com/bbs/thread-6963-1-1.html


//#include "WindowsTypes.h"

//ע�⣺2��driver�Ⱥ�hook ssdt��������Ⱥ�unloadͨ��������֮Դ -- ������ֵ�ַ����ԭ����λ��(win32k.sys?)�Ļ���������hook��

// nt!_KTHREAD -- X64�������ȫ�ֱ���(ʲô�������߳�? )����������ҵ�ServiceTable ��


//Tommy ��ר�� -- http://blog.csdn.net/whf727/article/details/4986575
//KeServiceDescriptorTable �ṹ���޸��ڴ汣��д�ķ��� --   http://www.3600safe.com/?post=94

//bf84d8de  win32k!FastWindowFromDC 
//bf8c30bc  win32k!_WindowFromDC 

//��SSDT Shadow���еĺ�������Ͳ���˵�� -- WRK(2003��Դ��)
//  WRK(Windows Research Kernel) -- ΢��Ϊ��У����ϵͳ�γ��ṩ�Ŀ��޸ĺ͸��ٵĲ���ϵͳ��ѧƽ̨��
//  ������Windows����ɹ�����ҵ����ϵͳ���ں˴󲿷ִ��룬���Զ�������޸ġ����룬���ҿ���������ں�����Windows����ϵͳ��
//  ��鵼�����ŵ�С���� -- ��̬����PE�ļ��ĵ���������Ե�����еķ��ţ�ȥ�����ļ��ĵ�������ȥ����(�ο�WRK)

//���⣺�������ı�ʾ�����
//  x64����windbg�鿴SSDT/Shadow SSDT -- http://hi.baidu.com/ithurricane/item/4cabc91964d1460de75c3634
//     �鿴SSDT�����ķ���(Win7 x64�����ƺ�����) -- ln (dwo(nt!KiServiceTable + 4*index)>>4) + nt!KiServiceTable
//     �鿴Shadow SSDT�ķ���(Win7 x64���Բ���) -- ln win32k!W32pServiceTable+((poi(win32k!W32pServiceTable+4*index)&0x00000000`ffffffff)>>4)-10000000
//  ��ȡ������ַ�Ĺ�ʽ�ǣ�dwo(nt!KiServiceTable+n)+nt!KiServiceTable��n=0,1,2������
//  http://bbs.dbgtech.net/forum.php?mod=viewthread&tid=360

//Windows WIN32K.SYS System Call Table (NT/2000/XP/2003/Vista/2008/7)
//  http://j00ru.vexillium.org/win32k_syscalls/
//  http://j00ru.vexillium.org/win32k_x64/


//Win7 64λ����δ����֤ǩ��������������������ư�װ -- ����ʱF8�����ѡ�񡰽�����������ǩ��ǿ�ơ���ͨ��bcdedit����
// on x86,we use Hook shadowSSDT; on x64,we use inline hook in user-mode;

//TODO: Depends �鿴 ntkrnlpa.exe + win32k.sys ���Ƚ�����ֵ
//SoftICE for Windows NT/2000��ntcall������Խ���ЩSystem Service��ʾ�� -- http://dev.21tx.com/2005/03/14/12553.html
//��ȡӲ���ϵ�ntoskrnl.exe�ļ�������export table��λԭʼ��SSDT�����������ڴ��������ڴ�ӳ��İ취ת�����õ�ϵͳ��SSDT��λ��

/******************************************************************************************************************
* ��Windows����ϵͳ�У���Ϣ���ӣ�ͨ��SetWindowsHookEx���ã�ֻ�ᵱǰ�������ϵĴ�����Ч��
* ���Կ��Խ���һ����ȫ���棬����������Ҫ���ܱ����Ľ��̣������Ǳ����������еĳ����޷�ͨ����Ϣ���ӵķ�ʽ��
* ��ȡ��Ҫ�������̴��ڵ���Ϣ���ﵽ�˱���Ŀ����̴��ڵ�Ŀ��(360������ͽ�ɽ�ܱ� �� ��ȫ���湦�� )
******************************************************************************************************************/

/******************************************************************************************************************
* PatchGuard -- 64λϵͳ�е��ں˲�����������, ��Ӧ�û����������޸ĺ���������ͻ���� CRITICAL_STRUCTURE_CORRUPTION(0x109)��������
*   �����������
*     ϵͳģ�� (ntoskrnl.exe��ndis.sys��hal.dll)
*     ϵͳ������ɱ� (SSDT, System Service Dispatch Table -- KeServiceDescriptorTable)
*     ȫ���������� (GDT -- Global Descriptor Table)
*     �ж��������� (IDT -- Interrupt Descriptor Table)
*     Processor MSRs (syscall)
*     ʹ�ò������ں˷�����ں˶�ջ(Using kernel stacks that are not allocated by the kernel)
*     �޲��ں˵��κβ���	 -- Patching any part of the kernel (detected only on AMD64-based systems) 
*     TODO������� inline hook �� Shadow SSDT ?
* 
*  ����PatchGuard -- ����˫���������ڱ����Ի�����ʱ�͹������ں˵�������PatchGuard�Ͳ��ᱻ���á�
*
*   PatchGuard���������в���ϵͳ�е�һ����֤�ں˾����ڴ漰 MSR�����ϵͳ�Ƿ��ܵ��𺦵������
*     Ϊ��ֹ�������ں�ģ������ں˲������������ʼ���Ͳ���ģʽ�Ѹ߶�ģ���������������ͼ���� PatchGuard����ͽ���޹��ġ�
*   PatchGuard ԭ�� -- 
*     1.��ʼ��(ʹ���˺ܶ�Ĵ��������Ի�Դ˺������ʿ)
*     	if (KiDivide6432(KiTestDividend, 0xCB5FA3) != 0x5EE0B7E5){ KeBugCheck(UNSUPPORTED_PROCESSOR); }
*     2.ʵ������ʱ�������������������쳣���� KiDivideErrorFault �������� ����PatchGuard�ĳ�ʼ�����̡�
*     3.ѡ��������ĳر�ǩ����λ��С�������ڴ棬Ȼ��ʹ�������Կ�� PatchGuard ���м��ܲ����Ƶ����ڴ�
******************************************************************************************************************/

//  http://www.microsoft.com/whdc/driver/kernel/64bitPatching.mspx
//  http://uninformed.org/index.cgi?v=8&a=5
//  http://www.mcafee.com/cn/resources/reports/rp-defeating-patchguard.pdf
//  DEP(����Ӳ��������ִ�б���) -- 

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
* ntkrnlmp.exe ?
* ntoskrnl.exe -- Windowsִ�������
* win32k.sys (Shadow SSDT?) -- User32.dll + Gdi32.DLL ���ں�ʵ��
*   ��GUI��ʵ�ַ����ں�ģʽ��������ϵͳ���ȶ��ļ��ʡ����������ͼ�δ��������Ч�ʡ�
*
* KeUserModeCallBack -- ��R0����λ��Ring3�ĺ���(ϵͳ���е���Ϣ���ӻص��������øú�����ɵ�)
* 
* http://bbs.pediy.com/showthread.php?t=149861&highlight=NtGdiBitBlt
* NtGdiBitBlt / NtGdiAlphaBlend / NtGdiCancelDC / NtGdiColorCorrectPalette / NtGdiConsoleTextOut / NtGdiCreateColorSpace /NtGdiCreateColorTransform
* NtGdiGetDCPoint / NtGdiGetDCObject
*
 

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
*   ע�⣺
*   1.win32k.sys���ǳ����ڴ�ģ��������GUI�̣߳�Shadow SSDT ��ַ��Ч -- ��GUI�߳���DeviceIoControl����;
*   2.ͨ����Ҫ���ӵĽ����� csrss.exe ? GetCsrPid()
*   3.ʹ��MDLӳ��һ�鲻��ҳ�ڴ棬���óɿ���д�룬��פ�������ڴ�(�μ� RegmonMapServiceTable)
*     TODO:ͨ�� #pragma LOCKEDCODE �����������ɣ�
*   4.GDI�ĺܶຯ������� _pGdiSharedHandleTable ��?
*
* KeServiceDescriptorTable -- ϵͳԤ����ı���SSDT��Ϣ�ı�������ͨ��extern����ʣ��� WinDbg ��: dd KeServiceDescriptorTable
* KeServiceDescriptorTableShadow -- Ϊ���� Shadow SSDT Hook ������ģ�ϵͳ�в����ڣ�����ͨ�� KeServiceDescriptorTable ����λ��
* 

* ��SSDT����Hook�Ĺ���
*   1.�ر�CR0д����(�ı�CR0�Ĵ�����WPλ)��
*   2.1.Hook Shadow SSDT ֮ǰ�����뽫�����������л���CSRSS����
*   2.2.���µĺ�����ַ�滻ԭ��SSDT�еĺ�����ַ
*   3.�ָ�CR0д����
* 
* Shadow SSDT �к�������λ��
*   1.���Խ���pdb�õ������Ƚ��鷳(���� ��ȡshadow��ַ�ͺ������ƵĹ��� )
*     SymInitialize -> SymSetSearchPath -> SymLoadModule -> SymGetSymFromName
*   2.ͬһ���汾��ϵͳ���ú�һ�����������Ժ����ֱ��д��
*     
* Hook ���(����ͱ��˵�Hook��ͻ��ȫ���)
*   1.�鿴 OldFunc �ĺ�����ַ�Ƿ���win32k��ģ����
* 
* ���ڱ��� NtUserFindWindowEx��NtUserGetForegroundWindow��NtUserBuildHwndList��NtUserQueryWindow��
*          NtUserWindowFromPoint��NtUserSetParent��NtUserPostMessage��NtUserMessageCall��
*          NtUserSetWindowLong��NtUserShowWindow��NtUserDestroyWindow��NtUserCallHwndParamLock
* ģ�ⰴ�� NtUserSendInput
* ��ȡ���̰���״̬ NtUserGetAsyncKeyState
* �򿪰�ȫ���� NtUserOpenDesktop
* ����ٰ�����ԭ����ʵ�İ��� NtUserTranslateMessage
* �������� NtGdiStretchBlt
******************************************************************************************************************/

/******************************************************************************************************************
* ϵͳ���̷���
*   csrss.exe -- Windows��ϵͳ����������, �ᱣ�����н��̵ľ����
*     Console����� ScrollDC ��API��ҪHook������̲��У�
*     win32!RawInputThread ͨ�� GUID_CLASS_KEYBOARD ��ü����豸ջ��PDO�ķ���������
*   System -- ϵͳ���̣���ϵͳ����ʱ������
*     ���������ص�ʱ��ϵͳ���������µ��̣߳����ö��������������һ����������(DRIVER_OBJECT)��Ȼ������� DriverEntry 
******************************************************************************************************************/

/******************************************************************************************************************
*             Ring3(kernel32)                    |   Native API(ntdll!)     |       Ring0(nt!)          |
* ------------------------------------------- SSDT (WinXP 284) -----------------------------------------|
* OpenProcess                                    | NtOpenProcess            | NtOpenProcess             |
* NtDuplicateObject                              |                          |                           |
* CreateThread/CreateRemoteThread                | NtCreateThread           | CreateThread              |
* OpenThread                                     |                          | NtOpenThread              |
* WriteProcessMemory                             | NtWriteVirtualMemory     | NtWriteVirtualMemory      |

* ------------------------------------- Shadow SSDT (WinXP 667)-----------------------------------------|
* BitBlt                                         |                          | NtGdiBitBlt               |
* CreateDC										 |                          | NtGdiOpenDCW              |
* DeleteDC                                       |                          | NtGdiDeleteObjectApp      |
* DestroyWindow                                  |                          | NtUserDestroyWindow       |
* EnableWindow                                   |                          | NtUserCallHwndParamLock   |
* EnumWindows                                    |                          | NtUserBuildHwndList       |
* FindWindow                                     |                          | NtUserFindWindowEx        |
* GetForegroundWindow                            |                          | NtUserGetForegroundWindow |
* GetWindowThreadProcessId                       |                          | NtUserQueryWindow         |
* PostMessage                                    |                          | NtUserPostMessage         |
* SendMessage                                    |                          | NtUserMessageCall         |
* SetParent                                      |                          | NtUserSetParent           |
* SetWindowLong                                  |                          | NtUserSetWindowLong       |
* ShowWindow                                     |                          | NtUserShowWindow          |
* StretchBlt                                     |                          | NtGdiStretchBlt           | 
* WindowFromDC                                   |                          | NtUserCallOneParam        |
* WindowFromPoint                                |                          | NtUserWindowFromPoint     |
*                                                |                          | NtUserCallTwoParam        |
******************************************************************************************************************/
//#define TWOPARAM_ROUTINE_SETDCBRUSHCOLOR    0xfffd0046
//#define NtUserSetDCBrushColor(hbr, crColor) (COLORREF)NtUserCallTwoParam((DWORD)(hbr), (DWORD)crColor, TWOPARAM_ROUTINE_SETDCBRUSHCOLOR)

#ifdef __cplusplus
extern "C" {
#endif

#define SSDT_API_CALL_ENTER(x) (InterlockedIncrement(&x))
#define SSDT_API_CALL_LEAVE(x) (InterlockedDecrement(&x))

//SSDT��ṹ -- 32/64 ͨ�ã� func = (PBYTE)Base+Base[index];
typedef struct _SYSTEM_SERVICE_TABLE { 
	// PNTPROC ServiceTable;
	PVOID *ServiceTable;					//SSDT���ڴ��еĻ���ַ(���飬��ÿһ����Ԫ�ؾ��Ǹ���Rin0�ĺ���)
	PULONG CounterTable;					// array of usage counters, ������ SSDT ��ÿ�����񱻵��ô��������������顣���������һ����sysenter ����
	//ULONG NumberOfServices;				    //SSDT��ĸ���
	SIZE_T NumberOfServices;				    //SSDT��ĸ���
	PBYTE ArgumentsTable;					//TODO:ʵ�����ǲ�������(ParamTableBase)? ����ÿ��ϵͳ��������ֽ�����Ļ���ַ-ϵͳ���������
} SYSTEM_SERVICE_TABLE, *PSYSTEM_SERVICE_TABLE;

//ϵͳ�е�4��SSDT��
typedef struct _SERVICE_DESCRIPTOR_TABLE
{
	SYSTEM_SERVICE_TABLE ntoskrnl;  // ntoskrnl.exe(native api���� SSDT, KeServiceDescriptorTable )
	SYSTEM_SERVICE_TABLE win32k;    // win32k.sys (gdi/user support, shadow SSDT)
	SYSTEM_SERVICE_TABLE Table3;    // not used
	SYSTEM_SERVICE_TABLE Table4;    // not used
}SYSTEM_DESCRIPTOR_TABLE, PSYSTEM_DESCRIPTOR_TABLE;

typedef struct _SCROLL_DATA
{
	int dy;
	RECT rcScroll;
} SCROLL_DATA, *PSCROLL_DATA;
#pragma push(0)

SYSTEM_SERVICE_TABLE *GetServiceDescriptorShadowTableAddress();
//int GetNtGdiBitBltIndex(SYSTEM_SERVICE_TABLE *p);

void InstallCopyProtectHook(HANDLE hProcess, HWND hWndDesktop);
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

