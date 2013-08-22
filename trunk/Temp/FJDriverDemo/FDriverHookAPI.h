#ifndef F_DRIVER_HOOK_API_H
#define F_DRIVER_HOOK_API_H
#pragma once

#ifdef __cplusplus
extern "C" {
#endif



#ifdef __cplusplus
}
#endif

//[ԭ����Դ]��WIN7 X64��Hook Shadow SSDT  --  http://www.m5home.com/bbs/thread-6963-1-1.html
//ʵ�� Win64 �ϵ��ں˼�Inline Hook���� --  http://www.jybase.net/biancheng/20120429881.html

/******************************************************************************************************************
����˳��
  KiFastCallEntry  -> SSDT[ServiceIndex] -> 
    ServiceIndex���� 0x1000 ���õڶ��ű�(Shadow SSDT)


ϵͳ�еĺ���
  nt!KeAddSystemServiceTable -- ϵͳ�ĵ�������
ϵͳ�еı���, 
  nt!KeServiceDescriptorTable			-- fffff800`02ac5840
  nt!KeServiceDescriptorTableShadow	-- fffff800`02ac5880
  nt!KiServiceTable					-- fffff800`0288fb00, �� nt!KeServiceDescriptorTable->ServiceTableBase(SSDT)
  win32k!W32pServiceTable				-- fffff960`001c1c00, �� nt!KeServiceDescriptorTableShadow->ServiceTableBase[1](Shadow SSDT)

1: kd> dp nt!KeServiceDescriptorTableShadow
fffff800`02ac5880  fffff800`0288fb00 00000000`00000000
fffff800`02ac5890  00000000`00000191 fffff800`0289078c
fffff800`02ac58a0  fffff960`001c1c00 00000000`00000000
fffff800`02ac58b0  00000000`0000033b fffff960`001c391c

1: kd> ?? pServiceTable   -- (��ͨ��������ʾ nt!KeServiceDescriptorTableShadow::ntoskrnl )
struct _SYSTEM_SERVICE_TABLE * 0xfffff800`02ac5880
+0x000 ServiceTableBase : 0xfffff800`0288fb00  -> 0x02f6f000`04106900 Void
+0x008 CounterTable     : (null) 
+0x010 NumberOfServices : 0x191
+0x018 ArgumentsTable   : 0xfffff800`0289078c  ""

1: kd> ?? &pServiceTable[1] -- (��ͨ��������ʾ nt!KeServiceDescriptorTableShadow::win32k )
struct _SYSTEM_SERVICE_TABLE * 0xfffff800`02ac58a0
+0x000 ServiceTableBase : 0xfffff960`001c1c00  -> 0xfff0a301`fff39800 Void
+0x008 CounterTable     : (null) 
+0x010 NumberOfServices : 0x33b
+0x018 ArgumentsTable   : 0xfffff960`001c391c  ""

0: kd> dv
pServiceTable = 0xfffff800`02ac5880
nIndex = 0n8
newAddress = 0xfffff880`02ab6980
status = 0n0
W32pServiceTable = 0xfffff960`001c1c00
i = 0n0
qwTemp = 0
pWin32k = 0xfffff800`02ac58a0
dwTemp = 0n0

Shadow SSDT �еĵ�ַ��Win64������Ե�ַ(3.5 ���ֽڣ��ɱ�ʾ�ķ�Χ����)����˲����ý�����ַ�ķ�ʽֱ���滻
TODO�����һλ�п��ܲ���0��������ô���ʲô��

0: kd> dd fffff960`001c1c00	-- W32pServiceTable
fffff960`001c1c00  fff39800 fff0a301 000206c0 00101d00		1~4
fffff960`001c1c10  00095ac0 00022700 fff99a00 ffddfec3		5~8
fffff960`001c1c20  0003ac47 00fbb500 ffed1d80 ffe4f980		9~12
fffff960`001c1c30  000c5380 000af0c0 000e8700 fffeb5c0
fffff960`001c1c40  ffb1d500 0004e780 ffa561c0 000b9000
fffff960`001c1c50  000b1fc0 000fbd40 00037b00 000b3900
fffff960`001c1c60  ffb212c0 000b5380 000a0f00 ffb2a580
fffff960`001c1c70  ffa2a543 ffa83800 0012aac0 00093c00

0xfffff960`001c1c00 + ���ε�����>;4
Address of 1 is fffff960`001b2630 -- win32k!NtUserPeekMessage    	-- 001c1c00 + Ffff0a30   -- 1
Address of 2 is fffff960`001c3c6c -- win32k!NtUserCallOneParam 		-- 001c1c00 + 0000206c   -- 2
Address of 3 is fffff960`001d1dd0 -- win32k!NtUserGetKeyState		-- 001c1c00 + 000101d0   -- 3
Address of 4 is fffff960`001cb1ac -- win32k!NtUserInvalidateRect	-- 001c1c00 + 000095ac   -- 4
Address of 5 is fffff960`001c3e70 -- win32k!NtUserCallNoParam		-- 001c1c00 + 00002270	 -- 5
Address of 6 is fffff960`001bb5a0 -- win32k!NtUserGetMessage		-- 001c1c00 + Ffff99a0	 -- 6
Address of 7 is fffff960`0019fbec -- win32k!NtUserMessageCall		-- 001c1c00 + Fffddfec	 -- 7
Address of 8 is fffff960`001c56c4 -- win32k!NtGdiBitBlt			-- 001c1c00 + 00003ac4	 -- 8
Address of 9 is fffff960`002bd750 -- win32k!NtGdiGetCharSet		-- 001c1c00 + 000fbb50	 -- 9
******************************************************************************************************************/

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

//WinDbg ��ѭ������SSDT�ĺ�����Ӧ�� -- ��Ҫ����Hook���֣��� load ��ص�pdb�ļ����ܲ���
// 32 Bit(WinXP)
//   SSDT -- .for (r eax=0, edx=5; @eax <= @edx; reax=eax+1){? eax; ln (dwo(nt!KiServiceTable + 4 * eax)) }
//   Shadow SSDT -- .for (r eax=0, edx=5; @eax <= @edx; reax=eax+1){? eax; ln (dwo(win32k!W32pServiceTable + 4 * eax)) }
// 64 Bit(Win7)
//   SSDT -- 
//   Shadow SSDT -- .for (r eax=0, edx=5; @eax <= @edx; reax=eax+1){? eax; r ebx=dwo(win32k!W32pServiceTable+4*eax); .if(ebx & 0x80000000) { r ebx=((ebx>>>4)+0xF0000000); r rcx=0x100000000; } .else { r ebx=ebx>>>4; r rcx=0x0; }; ln win32k!W32pServiceTable+ebx-rcx }

//���⣺�������ı�ʾ�����
//  x64����windbg�鿴SSDT/Shadow SSDT -- http://hi.baidu.com/ithurricane/item/4cabc91964d1460de75c3634
//     �鿴SSDT�����ķ���(Win7 x64���� OK) -- ln (dwo(nt!KiServiceTable + 4*index)>>4) + nt!KiServiceTable
//     �鿴Shadow SSDT�ķ���(Win7 x64����NG -- 3~5������ ) -- ln win32k!W32pServiceTable+((poi(win32k!W32pServiceTable+4*index)&0x00000000`ffffffff)>>4)-10000000
//       .for (r eax=0, edx=5; @eax <= @edx; reax=eax+1){? eax; ln (dwo(win32k!W32pServiceTable + 4*eax)>>4) + win32k!W32pServiceTable }


//  ��ȡ������ַ�Ĺ�ʽ�ǣ�dwo(nt!KiServiceTable+n)+nt!KiServiceTable��n=0,1,2������
//  http://bbs.dbgtech.net/forum.php?mod=viewthread&tid=360


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
* ntkrnlpa.exe 
* ntkrnlmp.exe (SSDT?) -- Kernel32.Dll ���ں�ʵ��
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

//http://drmemory.googlecode.com/svn/trunk/drmemory/syscall_usercallx.h
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

#endif //F_DRIVER_HOOK_API_H

