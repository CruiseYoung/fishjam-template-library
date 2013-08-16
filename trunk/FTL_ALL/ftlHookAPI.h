#ifndef FTL_HOOK_API_H
#define FTL_HOOK_API_H
#pragma once

#ifndef FTL_BASE_H
#  error ftlHookAPI.h requires ftlbase.h to be included first
#endif

#  define DETOURS_VERIFY(x) \
	nRet = (x);\
	if(nRet != NO_ERROR)\
	{\
		REPORT_ERROR_INFO(FTL::CFAPIErrorInfo, nRet, x);\
	}
//http://bbs.csdn.net/topics/300138796 -- atlstdthunk.h ������ jmp rax �Ĵ���

//����˹�� ��64BitWin7�ϵ�Hook
//3		NtUserGetKeyState				0xFFFFF96000131DCA	0xFFFFF96000131DD0	-	
//5		NtUserCallNoParam				0xFFFFF96000123E6A	0xFFFFF96000123E70	-	
//7		NtUserMessageCall				0xFFFFF960000FFBE6	0xFFFFF960000FFBEC	-	
//8		NtGdiBitBlt						0xFFFFF960001256BE	0xFFFFF960001256C4	-	
//15	NtUserPostMessage				0xFFFFF96000120756	0xFFFFF9600012075C	-	
//28	NtUserBuildHwndList				0xFFFFF960000C464E	0xFFFFF960000C4654	-	
//49	NtGdiStretchBlt					0xFFFFF9600024813A	0xFFFFF96000248140	-	
//68	NtUserGetAsyncKeyState			0xFFFFF9600012A5E6	0xFFFFF9600012A5EC	-	
//95	NtUserPostThreadMessage			0xFFFFF960000D428E	0xFFFFF960000D4294	-	
//105	NtGdiMaskBlt					0xFFFFF96000247222	0xFFFFF96000247228	-	
//110	NtUserFindWindowEx				0xFFFFF9600012FD12	0xFFFFF9600012FD18	-	
//119	NtUserSetParent					0xFFFFF9600012BF4E	0xFFFFF9600012BF54	-	
//120	NtUserGetKeyboardState			0xFFFFF96000131C4E	0xFFFFF96000131C54	-	
//130	NtUserSendInput					0xFFFFF9600012F83A	0xFFFFF9600012F840	-	
//140	NtUserSetWindowsHookEx			0xFFFFF960000CE096	0xFFFFF960000CE09C	-	
//236	NtUserAttachThreadInput			0xFFFFF96000129582	0xFFFFF96000129588	-	
//260	NtUserSetWinEventHook			0xFFFFF96000128152	0xFFFFF96000128158	-	
//586	NtGdiPlgBlt						0xFFFFF9600022F95E	0xFFFFF9600022F964	-	
//755	NtUserRegisterHotKey			0xFFFFF9600012B72A	0xFFFFF9600012B730	-	
//756	NtUserRegisterRawInputDevices	0xFFFFF9600013687A	0xFFFFF96000136880	-	
//811	NtUserUnregisterHotKey			0xFFFFF9600012CBE6	0xFFFFF9600012CBEC	-	


/******************************************************************************************************
* ��Щ����£�������ƽ��̵Ľ��ޣ�������һ�����̵ĵ�ַ�ռ䣬��Щ���������
* 1.������ҪΪ��һ�����̴����Ĵ��ڽ�������ʱ -- SetWindowLongPtr(hwnd,GWLP_WNDPROC,..) �ı䴰�ڵ��ڴ���еĴ��ڹ��̵�ַ��
* 2.������Ҫ���԰���ʱ�����磬������Ҫȷ����һ����������ʹ���ĸ�DLLʱ����
* 3.������Ҫ�ҽ���������ʱ��
*   ����Ҫ��������̵Ĵ������Ŀ�Ľ��̵ĵ�ַ�ռ䣭����DLL����Ŀ�Ľ��̵ĵ�ַ�ռ䡣
*
* ע�⣺
*   1.���ע���DLL�������κ��µ��̣߳���ȷ���ڳ����˳�ʱ���˳��̡߳�
*   2.Զ��ע�����Ҫͨ�����̼�ͨ�ŵķ�ʽ���� -- �� FileMapping��WM_COPYDATA��Clipboard��#pragma data_seg ��
*     #pragma data_seg (".shared")
*       UINT    WM_HOOKEX = 0;
*       HHOOK   g_hHook = 0;
*       XXXX    g_MyDATA = yyy; //��ʼ��
*     #pragma data_seg ()
*     #pragma comment(linker,"/SECTION:.shared,RWS")  //���ù����
*   3.ͨ������DLL�е� DllMain ����Ӧ DLL_PROCESS_ATTACH ������ 
*       ������Ҫ���� DisableThreadLibraryCalls ����ֹ��DLL �����߳�֪ͨ��Ϣ��
*   4.ͨ������ RegisterWindowMessage ע����Ϣ�����й����� Exe �� ע���DLL���Խ�����
*       DLL �� DllMain �У�if(WM_HOOKEX == 0) WM_HOOKEX = ::RegisterWindowMessage(TEXT("WM_HOOKEX_RK"));
*       Exe �У�
*
* ���÷���
*   a. ʹ��ע���������DLL
*      HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Windows �� AppInit_DLLs ������һ��DLL�ļ�������һ��DLL�ļ���
*      (�ÿո�򶺺Ÿ���)���г��ĵ�һ��DLL�ļ������԰���һ��·�� ���� ��ý����DLL���� Windows ��ϵͳĿ¼�У������Ͳ����趨·����
*      ϵͳ����ʱ���� User32.dll ���ε���DLL�е� DllMain(DLL_PROCESS_ATTACH)
*      ���㣺ֻ����ϵͳ�����󣬼��ص�����ʹ��User32.dll �� GUI �����У���֧��Win98
*
*   b.ʹ�� Windows �ҹ�������DLL������Ե����̵߳�Hook���������ϵͳ��Hook(�������ĸ���������ν����һ������� WH_GETMESSAGE/WH_CALLWNDPROC ?)
*       ϵͳHook�� HHOOK hHook = SetWindowsHookEx(WH_GETMESSAGE,GetMsgProc,hInstDll,0);
*       �߳�Hook�� g_hHook = SetWindowsHookEx( WH_CALLWNDPROC,HookProc,hInstDll, GetWindowThreadProcessId(hWndParam,NULL));
*     ��ϵͳ�������ӳ������ҹ�������������DLLʱ������DLL����ӳ�䣬����ֻ�ǹҹ�������������ӳ�䡣
*     ����ζ��DLL�а������κ�һ�����������к������ڶ����ڣ����ҿ��Դ�Ŀ�Ľ��̵Ļ��������е��߳��е��á�
*     Sample������̻�ȡ���� ListView �и�����Ŀ��λ�á�
*
*   c.ʹ��Զ���߳�������DLL -- ���и���������(DLL ��ִ�� DLL_PROCESS_ATTACH ֪ͨ�� DllMain ���� )
*     CreateRemoteThread/NtCreateThreadEx(����� pfnStartAddrָ���̺߳������ڴ��ַ,���˳̺����Ĵ��벻��λ���Լ����̵ĵ�ַ�ռ���)��
*     ��Ҫ���̵߳��� LoadLibrary �������������ǵ�DLL������һ�����̣߳�����ʹ�̺߳����ĵ�ַ��Ϊ LoadLibraryA ��LoadLibraryW�����ĵ�ַ��
*     TODO����ʹ�� VirtualQueryEx �����Ѵ��ָ���ҳ���Ȩ�޸���Ϊ�ɶ���д��ִ�У��ٸ�д�����ݣ��Ӷ��޸��������еĳ��� 
*     1).ʹ�� VirtualAllocEx ����������Զ�̽��̵ĵ�ַ�ռ��е��ڴ档
*     2).ʹ�� WriteProcessMemory ��������DLL��·������������һ���������Ѿ�������ڴ��С�
*     3).ʹ�� GetProcAddress ��������ȡ LoadLibraryA �� LoadLibraryW ������ʵ��ַ����Kernel32.dll�У�
*     4).ʹ�� CreateRemoteThread ��������Զ�̽����д���һ���̣߳���������ȷ�� LoadLibrary ������Ϊ�����ݵ�һ�������з�����ڴ�ĵ�ַ��
*        ��ʱ,DLL�Ѿ�������Զ�̽��̵ĵ�ַ�ռ��У�ͬʱDLL�� DllMain �������յ�һ�� DLL_PROCESS_ATTACH ֪ͨ�������ܹ�ִ����Ҫ�Ĵ��롣
*        �� DllMain ��������ʱ��Զ���̴߳����� LoadLibrary �ĵ��÷���,ʹԶ���߳���ֹ���С�
*        ����Զ�̽���ӵ�е�һ�������з�����ڴ�飬��DLL����Ȼ���������ĵ�ַ�ռ��С�
*     5).����GetExitCodeThread��ȡԶ���̵߳��˳��� -- ��LoadLibrary�����ķ���ֵ������ӳ���DLL�Ļ�ַ�����Ա����������Ժ��ͷ����������
*     6).ʹ�� VirtualFreeEx �������ͷŵ�һ�������з�����ڴ档
*     7).ʹ�� GetProcAddress ��������� FreeLibrary ������ʵ��ַ��
*     8).ʹ�� CreateRemoteThread ��������Զ�̽����д���һ���̣߳������� FreeLibrary ����������Զ��DLL��HINSTANCE��
*
*   d.ʹ��������DLL������DLL
*     ȡ����֪�����̽�Ҫ���ص�DLL(�Լ���DLLȡ��Ϊ��Ҫ�滻��DLL������Ҫ�滻��DLL���ɱ�����֣�ʹ�� "����ת����" ���ԭʼDLL�ķ���)
*     --���߱��汾��������
*     ��ı���Ҫ����DLL�������
*   
*   e.��DLL��Ϊ���Գ���������
*     ���Գ����ܹ��Ա����ԵĽ���ִ������Ĳ������������Խ��̼���ʱ���ڱ����Խ��̵ĵ�ַ�ռ��Ѿ�����׼����
*     ���Ǳ����Խ��̵����߳���δִ���κδ���֮ǰ��ϵͳ���Զ���������֪ͨ���Գ�����ʱ�����Գ������ǿ��
*     ��ĳЩ������뱻���Խ��̵ĵ�ַ�ռ���(����ʹ�� WriteProcessMemory ����������)��Ȼ��ʹ�����Խ��̵����߳�ִ�иô��롣
*     Ҫ��Ա������̵߳� CONTEXT �ṹ���в�������ζ�ű����д�ض�CPU�Ĵ���
*   
*   f. �� CreateProcess ������� -- ��Ҫ�Լ��Ľ�������������������½��̡�
*     1).ʹ��Ľ���������ͣ���е��ӽ���;
*     2).��.exe ģ���ͷ�ļ��м������̵߳���ʼ�ڴ��ַ��
*     3).������ָ����ڸ��ڴ��ַ�С�
*     4).��ĳЩӲ����Ļ���ָ��ǿ�Ʒ���õ�ַ�С���Щָ��Ӧ�õ��� LoadLibrary ����������DLL��
*     5).���������ӽ��̵����̣߳�ʹ�ô������ִ�С�
*     6).��ԭʼָ�����·�����ʼ��ַ��
*     7).�ý��̼�������ʼ��ַ��ʼִ�У�����û�з����κ�����һ��
*   
*   g. ʹ�� WriteProcessMemory ��Զ�̽�����д��ִ�д��룬��ʹ�� CreateRemoteThread(Vistaǰ) �� NtCreateThreadEx(Vista�����) ִ�С�
*     1).ʹ�� VirtualAllocEx ��Զ�̽����з��� ������� �� ����(INJDATA)���ڴ棻
*     2).ʹ�� WriteProcessMemory д��"�������"�͡��������Ŀ�����
*     3).ʹ�� CreateRemoteThread ��ʼִ�� ������룻
*     4).ʹ�� WaitForSingleObject �ȴ��߳̽�����
*     5).ʹ�� ReadProcessMemory �� GetExitCodeThread ��ȡԶ���̵߳�ִ�н����
*     6).ʾ�������Դ��VirtualFreeEx��CloseHandle����
*     �ŵ㣺
*       ����ֻ��һ��EXE(����ҪDLL�ļ�)��
*     ȱ�㣺
*       �ǳ����Ӳ������׳���
*       Զ�̷���ֻ��ִ�� kernel32.dll �� user32.dll �еķ�����
*       ����ʹ�� /GZ ����ѡ�Debug�е�ȱʡֵ�� -- ���ں�����ͷ���ӵ����õĴ��룬��Զ�̽����в����ڡ�
*       �̺߳��������� static ��(��ֱ�ӵ���)�����߽�ֹ incremental linking(ͨ�� JMP ���ã�Զ�̽����е�ַ��ͬ)
*       �̺߳����еı��ر���������һ�� Page ����ڣ�4K��--ESP����ֱ�ӱ仯�������ú���(Զ�̽����е�ַ��ͬ)���е��� 
*       switch �в���ʹ�ù��ࣨ����3������case����û��� if..else if -- ����3��ʱ���ڱ���ʱ����ʹ�þ��Ե�ַ����ת��ַ��
*         ͨ��ƫ������ת��������ܣ�������ת����Զ�̽����в����ڡ�
*         
*     ע�⣺ʹ�� VirtualAllocEx ʱ����ָ�� PAGE_EXECUTE_READWRITE ���� -- ����д�����ĵط�ִ��
*
*
******************************************************************************************************/

/******************************************************************************************************
* Hook API
*   ʵ�ַ�ʽ
*     1.�������ʽ(import/export) -- ����ʵ�֣�OpenProcess + WriteProcessMemory ��д������ַ
*     2.����ʽ(inline) -- ��������hook�ڲ����������ҵ����ߺ����ƹ�hook
*         ͨ����Ҫ�걸�ķ�������ͽ�����
*   
* ���еĿ�
*   1.EasyHook��http://easyhook.codeplex.com/
*               https://code.google.com/p/easyhook-continuing-detours
*       ʹ�õ�����:http://blog.csdn.net/baggiowangyu/article/details/7675098
*     ֧��������Hook��֧��X64ע�룬����ȫ��ѣ�LicenseΪ LGPL, ����ͨ�� IDA Pro 5.0����� Ildasm.exe ����Ŀ¼DriverShared\ASM����Ҫ���Ļ�����(Trampoline_ASM_x86/x64)
*     �ṩ������ģʽ��ע������йܴ��� �� ���йܴ���(����Ϊ UnmanagedHook �� TestDriver)��ͨ��Inline Hook ��ʽʵ��
*     ʾ��:
*       TRACED_HOOK_HANDLE hHookCreateFileW = { NULL };  
*       ULONG HookCreateFileW_ACLEntries[1] = {0}; 
*       LhInstallHook(OriginalCreateFileW, MyCreateFileW, NULL, &hHookCreateFileW);
*       LhSetExclusiveACL(HookCreateFileW_ACLEntries, 1, hHookCreateFileW);  
*       ���ú���
*       LhUninstallAllHooks();
*       LhUninstallHook(&hHookCreateFileW);  
*       delete hHookCreateFileW;  
*       LhWaitForPendingRemovals(); //�ȴ��ڴ��ͷ�
*     Դ�����:
*       LhInstallHook -> LhRelocateEntryPoint -> 
* 
*	  DetoursԴ�����(�μ� Detours.chm )��
*       ����:
*         Source Function -- �û����õĺ���
*         Target Function -- �����õĺ���
*         Detour Function -- �Լ�������������
*         Trampoline -- ��Detour Function ��ת��Target��һ��������ʵ���� Target��һ���ּ��˸���תָ�
*       ����:
*         Hookǰ: Source -> ����Target -> ����Source
*         Hook��: Source(Target��ֱ��Jmp��Detour) -> Detour [ -> Trampoline -> Target -> ����Detour ] -> ����Source
*       ��ؽṹ����:
*         _DETOUR_TRAMPOLINE -- ƽ̨���(�� Trampoline �����壿)�������˶�����Ϣ��
*            Target ���ص��Ĵ���(rbRestore��)������(cbCode)��Target���������ָ��ĵ�ַ(pbRemain)��Detour�����ĵ�ַ(pbDetour)
*         DetourOperation -- ����ṹ����¼��Target����ָ��ĵ�ַ(ppbPointer?)��
*       ʵ�ֺ���
*         DetourCodeFromPointer -- �ڲ����� detour_skip_jmp�õ�����������λ�á����ö�̬���ӿ�ʱͨ��ͨ�� "JMP ��ַ" ���ַ�ʽ�����У�
*         ���ͨ���ú����ҵ����������λ�á�
*         
*   2.Detours��http://research.microsoft.com/en-us/projects/detours
*     ΢��Ŀ�Դ�о��⣬��Ѱ汾����������ҵ����ҵ�汾��Ҫ 10K USD��֧��x64��IA64��64λƽ̨
*     ���룺nmake��TODO: Debug/Release�� ����ʱ������Ҫ���� samples\common.mak �ļ���ȥ�����е� /nologo 
*     ȱ�ݣ�x86�޷�hookС��5�ֽڵĺ�������Ϊû�������ķ���������޷��������Ĵ��� jmp ������ǰ����ֽڷ�Χ��(�պñ����) �Ĵ����߼�
*     ԭ���ڻ���ı�Ŀ��API���ں���ڵ�һЩ���ָ��
*       �����֣�
*         Target������Ҫ���صĺ�����ͨ��ΪWindows��API����Hook�����ʼ���ֵĻ�౻����Ϊ��jmp DetourFunction
*         Trampoline����(�Ĵ�,ԭʼ������Ĵ��� + JMP��Target)��Target�����Ĳ��ָ���Ʒ��
*           ��ΪDetours�����дTarget�����������Ȱ�Target������ǰ5���ֽڸ��Ʊ���ã�
*           һ������Ȼ����Target�����Ĺ��̵������壬��һ��������Ժ�Ļָ���Ȼ����һ��������ת��
*         Detour�������������Target�����Ľػ���
*       Դ�������
*         1.
*     Detoursͬ���ṩ��DLLע��ķ�ʽ�����ɱ༭�κ�DLL��EXE�����Ĺ��ܣ��ﵽ����ڵĶ����ƴ���������������ݽڱ��Ŀ��
*     ��Ч����(Payloads) -- ���Զ�Win32�������ļ���ӡ�ж�����ݽڱ�(.detours Section)�Լ��༭DLL�����
*     ʵ��ע�⣺
*       1.��Ҫ��DLL���Ƶ�Ŀ�����Ŀ¼ �� System32(SysWOW64 -- 64λ�ϵ�32λ����ʱ)
*       2.ͨ�� #pragma data_seg("MyShare") + #pragma comment(linker,"/SECTION:MyShare,RWS") ���ض��������빲�����ݶ�
*       3.DLL_PROCESS_ATTACH + DLL_THREAD_ATTACH ʱ����Hook�� 
*         DLL_PROCESS_DETACH + DLL_THREAD_DETACH ʱ����UnHook��
*       4.�Ƿ���Ҫ���ļ��� detoured.dll ��һ����־������΢��Ŀ�����Ա�͹����ж�ĳ�������Ƿ��Ѿ���detours����
*       5.32 <==> 64 �������ʱ�������ɸ�������(HelperProcess)��
*       6.���Ծ�̬��̬�ش���������ComicViewer�в���������Ƶ��BitBlt��
*       A1.һ��Ҫö���̲߳�����DetourUpdateThread������������ܳ��ֺܵͼ��ʵı������⣬����������ѱ���������
*       A2.������غ�����DLL�У���ô�����������²�����Unhook֮��ж�����DLL������ж�ش�����ɱ�����Σ�ա�
*     ʹ�÷�ʽ��
*       1.��ͨ��DetourTransactionBegin => DetourUpdateThread => DetourAttach/DetourDetach => DetourTransactionCommit
*       2.���Ա���� -- ����CXxxHook��(ע�⣺��δ���� -- http://wenku.baidu.com/view/7b441cb665ce05087632133b.html)
*         a.����static�ĺ���ָ�룺 static int (CXxxHook::* TrueFun)(xxxx) = &CTrueXxx::Fun;
*         b.���� FilterFun �ĳ�Ա���������п���ͨ�� (this->*TrueFun)(xxx) �ķ�ʽ����ԭ����ʵ��
*         c.DetourAttach(&(PVOID&)CXxxHook::TrueFun, (PVOID)(&(PVOID&)CXxxHook::FilterFun));
*     ��Ҫ��API
*       DetourCreateProcessWithDllEx -- ������ָ�����̵�ʱ��ע��DLL(��DLL����Ҫ��һ����������)
*       DetourIsHelperProcess -- �жϵ�ǰ�Ƿ��ڸ���������,ͨ����DllMain�У��ú�������TRUEʱ��ֱ�� return TRUE;
*       DetourRestoreAfterWith -- �ָ���ʼ״̬(�� DllMain::PROCESS_ATTACH ʱ���ã�)
*       DetourUpdateThread -- 
*       DetourFindFunction -- ��̬�ҵ�����ָ�����ʵ��ַ�� TrueXxx ��ֵ?
*     ����:
*       SetDll.exe -- ������Exe�ļ������Ӷ�ָ��DLL������(����.detours�ڣ�����Ŀ�ִ���ļ�),
*         ע��DLL��Ҫ�����Ϊ"1" �ĵ�������(Makefile ����: /export:DetourFinishHelperProcess,@1,NONAME )
*         TODO:def �ļ���������ã�
*       WithDll.exe -- ��ע��ָ��DLL������´���������ָ������(���ڴ��������ִ���ļ�����Ӱ��)
*     TODO:
*       �Ƿ���Ҫ���� DisableThreadLibraryCalls ?
*     ��ֲ�� x64 �� Kernel
*       1.VirtualProtect���ڴ� READ_WRITE_EXECUTE -> ���cr0 �� ʹ�� native API ���VirtualProtect�Ĺ���( NtProtectVirtualMemory )
*       2.x64�²����� jmp 64_address����ָ��

*   3.Deviare(��֧��C/C++?  http://www.nektra.com/products/deviare-api-hook-windows/)
*   4.MinHook -- �ձ���д�ģ��Ƚϼ򵥣�ֻ��Hook���ܡ�
*     ��ʹ���� Relay ������֧����С5�ֽڵĺ�����VirtualAlloc ����ת������������ռ䣬��ʹ��32λ��JMP��ת����λ�ã�Ȼ���ٽ���64λ����ת��
* 
* TODO(FUAPIHook.h ������) -- http://wenku.baidu.com/view/5ae307f04693daef5ef73d48.html
*   Win64�ģ�http://bbs3.driverdevelop.com/read.php?tid=99096
* http://blog.csdn.net/zzw315/article/details/4102488
******************************************************************************************************/

/******************************************************************************************************
* Ӧ�ü���ȫ�� Hook API��SetWindowsHookEx + Detours -- TODO: ������ ftlHookAPI(DUI �� HookAPI Դ�룬�Ƚ�)
*   ȱ�㣨������
*     1.��Ҫ����ԱȨ��
*     2.���ȶ�������������WindowsHook��ʵ��(Hooks ��һ���������ƣ�û���κα�֤��������ʵ�ֲ����� CallNextHookEx �Ͳ��ܽ��д���)��
*     3.���ܱ�������������ѣ�
*   ע�⣺��Ӧ���� ��������ȫ�� Hook API(�μ� FDriverHookAPI.h )
*   ���ӻ��� -- ���ȴ����ض���Ϣ���¼��Ĵ���Σ�ÿһ��Hook����һ������(��������Hook Chains)��
*     ÿһ���һ��Ӧ�ó�����Ĺ��Ӵ�������ÿһ����������������ͨ������ CallNextHookEx ��֤֮ǰ�Ĺ��˺��������ã�
*     �����װ�Ĺ��ӷ������Ŀ�ʼ�����Ȼ�ÿ���Ȩ��
*     
*   SetWindowsHookEx
*     ϵͳ����() -- ������DLL�У�ϵͳ�Զ�����DLLע����Ӱ��Ľ��̵�ַ�ռ�
*     �̹߳��� -- 
*    
*     IAT(���ڼӿǵ������Ч) ? 
*     JMP ��ʽ -- ���ԶԸ��ӿ����   
*  
*   Զ��ע�룺
*     1.Ring3: CreateRemoteThread + LoadLibrary
******************************************************************************************************/

/******************************************************************************************************
* ������������(Ĭ�� x86, ǰ���������Ǻŵ��� x64 �ϵ�)
*   0x26 -- jmp es:
*   0x2e -- jmp cs:
*   0x33c0 -- xor     eax,eax
*   0x36 -- jmp ss:
*   0x488d -- lea xxxx
*** 0x48b8 XXXXXXXX`XXXXXXXX -- mov rax, Xxxxxx; ��ָ����64λ���ݷ��� rax 
*   0x50 -- push eax / rax
*   0x58 -- pop eax / rax
*   0x64 -- jmp fs:
*   0x65 -- jmp gs:
*   0x74 XX -- je imm8
*   0x90 -- nop	, һ����������������
*   0xB8 XXXXXXXX -- mov eax, xxxx , ��ָ����32λ���ݷ��� eax 
*   0xc2 -- ret +imm8
*   0xc3 -- ret
*   0xcc -- int 3, ����ϵ㣬һ�����ڴ�����ಿ�ֵ����
*   0xe0 -- jmp eax
*   0xe3 -- jmp ds ���� jcxz imm8 ?
*   0xe8 XXXXXXXX -- call imm16/imm32
*   0xe9 XXXXXXXX -- jmp imm16/imm32 (jmp offset, offset����0��ǰ��,offsetС��0�����) ; x64 �¶�Ӧ ff15  [xxxxxxxx], xxxxxxxx ��32λ�ģ�[xxxxxxxx]ָ��һ��64λ��ַ
*   0xeb -- jmp imm8
*   0xff25 -- jmp [+imm32] -- ����ָ���ڴ��б���� Address Table ������ת? JMP QWORD NEAR��
*     ��תĿ����ָ��8�ֽ��ڴ��еĵ�ַ(64λϵͳ�Ͽ���ֱ����ת��ȫ��Ŀ�ĵ�)������ָ��8�ֽڵ�ַΪ0,�����ݵ�ǰָ���ƫ��λ��Ϊ0�ĵ�ַ������ת
*   0xffe0 -- jmp rax -- ���� rax �Ĵ�����ֵ������ת���� jmp imm64 ?
* 
*
* 64λϵͳ�£���Ҫ��ת��64λ�ľ��Ե�ַ����Ҫͨ���Ĵ�����ʵ��, ��(EasyHook��ʵ�ַ�ʽ)
*   48 B8 8877665544332211  MOV RAX,1122334455667788H 
*   ffe0                    jmp rax
*
* ��ǰָ���ַ�� x86���� EIP�� x64���� RIP
* JMP�кü�����ʽ(�����������λ����ת, �µĵ�ַ = EIP/RIP + OFFSET + sizeof(JMP OFFSET) )
*   ��ˣ���������Ҫ����� ƫ�� = Ŀ�ĵ�ַ - (From + sizeof(JMP OFFSET) )
*   0xe9 -- 32λ�ϸ���������ַ�ռ�
*
******************************************************************************************************/

namespace FTL
{
}
#endif //FTL_HOOK_API_H

#ifndef USE_EXPORT
#  include "ftlHookAPI.hpp"
#endif 