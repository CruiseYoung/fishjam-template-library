#ifndef FTL_HOOK_API_H
#define FTL_HOOK_API_H
#pragma once

#ifndef FTL_BASE_H
#  error ftlHookAPI.h requires ftlbase.h to be included first
#endif

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
*   b.ʹ�� Windows �ҹ�������DLL������Ե����̵߳�Hook���������ϵͳ��Hook
*       ϵͳHook�� HHOOK hHook = SetWindowsHookEx(WH_GETMESSAGE,GetMsgProc,hInstDll,0);
*       �߳�Hook�� g_hHook = SetWindowsHookEx( WH_CALLWNDPROC,HookProc,hInstDll, GetWindowThreadProcessId(hWndParam,NULL));
*     ��ϵͳ�������ӳ������ҹ�������������DLLʱ������DLL����ӳ�䣬����ֻ�ǹҹ�������������ӳ�䡣
*     ����ζ��DLL�а������κ�һ�����������к������ڶ����ڣ����ҿ��Դ�Ŀ�Ľ��̵Ļ��������е��߳��е��á�
*     Sample������̻�ȡ���� ListView �и�����Ŀ��λ�á�
*
*   c.ʹ��Զ���߳�������DLL -- ���и���������(DLL ��ִ�� DLL_PROCESS_ATTACH ֪ͨ�� DllMain ���� )
*     CreateRemoteThread(����� pfnStartAddrָ���̺߳������ڴ��ַ,���˳̺����Ĵ��벻��λ���Լ����̵ĵ�ַ�ռ���)��
*     ��Ҫ���̵߳��� LoadLibrary �������������ǵ�DLL������һ�����̣߳�����ʹ�̺߳����ĵ�ַ��Ϊ LoadLibraryA ��LoadLibraryW�����ĵ�ַ��
*     1).ʹ�� VirtualAllocEx ����������Զ�̽��̵ĵ�ַ�ռ��е��ڴ档
*     2).ʹ�� WriteProcessMemory ��������DLL��·������������һ���������Ѿ�������ڴ��С�
*     3).ʹ�� GetProcAddress ��������ȡ LoadLibraryA �� LoadLibraryW ������ʵ��ַ����Kernel32.dll�У�
*     4).ʹ�� CreateRemoteThread ��������Զ�̽����д���һ���̣߳���������ȷ�� LoadLibrary ������Ϊ�����ݵ�һ�������з�����ڴ�ĵ�ַ��
*        ��ʱ,DLL�Ѿ�������Զ�̽��̵ĵ�ַ�ռ��У�ͬʱDLL�� DllMain �������յ�һ�� DLL_PROCESS_ATTACH ֪ͨ�������ܹ�ִ����Ҫ�Ĵ��롣
*        �� DllMain ��������ʱ��Զ���̴߳����� LoadLibrary �ĵ��÷���,ʹԶ���߳���ֹ���С�
*        ����Զ�̽���ӵ�е�һ�������з�����ڴ�飬��DLL����Ȼ���������ĵ�ַ�ռ��С�
*     5).����GetExitCodeThread��ȡԶ���̵߳��˳��� -- ��LoadLibrary�����ķ���ֵ������ӳ���DLL�Ļ�ַ�����Ա����������Ժ�ʾ��
*     ���������
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
*   g. ʹ�� WriteProcessMemory ��Զ�̽�����д��ִ�д��룬��ʹ�� CreateRemoteThread ִ�С�
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
*     1.�������ʽ -- ����ʵ�֣�OpenProcess + WriteProcessMemory ��д������ַ
*     2.����ʽ
*   
*   ���еĿ�
*     1.Detours��΢��Ŀ�Դ�о��⣬ ��ҵ�汾(��Լ6000RMB?֧��X64
*       http://nokyo.blogbus.com/logs/35243687.html
*       ���룺nmake��TODO: Debug/Release�� ������Ҫ���� samples\common.mak �ļ���ȥ�����е� /nologo 
*       ԭ����Ŀ������С���׮�����������Ŀ�꺯����
*   
* TODO(FUAPIHook.h ������) -- http://wenku.baidu.com/view/5ae307f04693daef5ef73d48.html
*   Win64�ģ�http://blog.csdn.net/zzw315/article/details/4102488
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

namespace FTL
{
}
#endif //FTL_HOOK_API_H

#ifndef USE_EXPORT
#  include "ftlHookAPI.hpp"
#endif 