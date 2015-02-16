#ifndef FTL_WOW_H
#define FTL_WOW_H

#pragma once

#ifndef FTL_BASE_H
#  error ftlWow.h requires ftlbase.h to be included first
#endif

namespace FTL
{
    /*****************************************************************************************************
    * __int64 i;   // �������ڴ���1<<31��ʱ���ǰ���32λ����������
    * ����д��
    *   i = 1 << 31;            // i == 0xffffffff80000000
    *   i = 1 << 32;            // i == 0x0000000000000000
    * ��ȷд��
    *   i = __int64(1) << 31;   // i == 0x0000000080000000
    *   i = __int64(1) << 32;   // i == 0x0000000100000000
    *
    *****************************************************************************************************/

    /*****************************************************************************************************
    * ����X86ָ���64( _M_X64 )
    *   EM64T(Extended Memory 64 Technology) -- Intel��
    *   AMD64(Advanced Micro Devices 64),Ŀǰʵ����ֻ����64λ��48λ����ʾ��ַ�ռ�(��128T��UserMode + ��128T��KernelModel)
    * IA64(_M_IA64) -- ������X86,������64λCPU��ָ��кܴ�仯,΢����ר�ŵ�OS��Ӧ�汾(������)
    *   �������ģ�ⷽʽ����IA32��X86ָ�Ч�ʲ���(����Ҳ������32λ����)
    *   
    *****************************************************************************************************/

    /*****************************************************************************************************
    * ����ѡ��
    *   1.C++ -> General -> Detect 64-bit Portability Issues(/Wp64) -- ���԰������ִ������64λ����
    *   2.������� IMAGE_FILE_LARGE_ADDRESS_AWARE��־ (/LARGEADDRESSAWARE)����32λӦ�ó�����Wow64��
    *     ����ʹ��ȫ����4G��ַ�ռ�(����Ĭ��ֻ��ʹ��2G -- ��2G��ϵͳʹ��)
    *****************************************************************************************************/

	/*****************************************************************************************************
	* ��ʼ����64λ Windowsϵͳ���֮ǰ��Ҫ�˽����Ϣ -- http://www.microsoft.com/china/MSDN/library/Windev/64bit/issuesx64.mspx?mfr=true
    * WOW/WOW64 -- Windows On Windows /64-bit 
    *   ��Ҫ������Ѹ�ٶ�����ؽ����е� 32 λӦ�ó�����ֲ�� 64 λ������ȥ����ʱ�����Դ�����У�
    *   ָ�뱻��Ϊ32λֵ������򵥵����±�дӦ�ó��򣬾ͻ����ָ�뱻�ضϵĴ���Ͳ���ȷ���ڴ���ʡ�
    *   Ĭ������£���ϵͳȷ������ 0x000000007FFFFFFF ���ϵ��ڴ��ַ���з��䣨��Ҫʹ6 4λӦ�ó����ܹ�����
    *   ����ȫ�� 4TB �û���ʽ��������Ӧ�ó������ʹ�� /LARGEADDRESSAWARE ���ӿ�������������
    *   ��ϵͳ�������̵� 64 λ��ַ�ռ�ʱ��Ҫ���һ����ִ���ļ��� LARGEADDRESSAWARE ��־���������DLL����ôϵͳ�����Ըñ�־��
    * 
    * �������ʱ��ʹ�� UINT_PTR��DWORD_PTR ���Զ�֧�� 32/64 �ı���(WPARAM,LPARAM,LRESULT,����HANDLE���ʶ���ָ��)
	*
	* 64λ����ʹ��ע���ʱ������ʹ�� KEY_WOW64_32KEY ����ָ������32λ��ע���
	*   CRegKey reg;
	*   #if _WIN64
	*   if ( ERROR_SUCCESS == reg.Open(HKEY_LOCAL_MACHINE, _T("SOFTWARE\Microsoft\VisualStudio"),	KEY_READ | KEY_WOW64_32KEY) )
	*   #else
	*   if ( ERROR_SUCCESS == reg.Open(HKEY_LOCAL_MACHINE, _T("SOFTWARE\Microsoft\VisualStudio"),	KEY_READ) )
	*   #endif
	*
	* 64λOS��ע��DLLʱ(TODO:���� .js ��ʱ��Ҳ��Ҫʹ�� %systemroot%\SysWoW64\wscript.exe ��)��
	*   32-bit DLL -- %systemroot%\SysWoW64\regsvr32.exe
	*   64-bit DLL -- %systemroot%\System32\regsvr32.exe
    *
    * ע���༭��(regedit)
    *   32-bit�汾 -- %windir%\SysWOW64\regedit.exe
    *   64-bit�汾 -- %windir%\regedit.exe
    *
    * 64λTask Explorer�鿴32λ���򣬿��Լ��������DLL
    *   wow64cpu -- ����wow��ÿ���������е��̵߳�32λCPU�����ģ�
    *               ���ҿ���CPU��32λģʽ��64λģʽ֮����л�(wow64cpu!X86SwitchTo64BitMode)
    *   wow64.dll -- ������̺��̵߳Ĵ������ػ��쳣�ַ�����(Exception Dispatching)��ϵͳ����(ntolkrnl.exe����¶������ϵͳ����).
    *                ʵ�����ļ�ϵͳ�ض���ע����ض��򣬺�ע����䡣
    *   wow64win -- �ػ�GUIϵͳ���ã���Щ������Win32k.sys��¶������
    *   ntdll.dll(32λ) -- 32λ���̵��õĽӿڣ����ڲ�ͨ�� wow64cpu.dll+wow64.dll+wow64win.dll ����64λ��ntdll.dll
    *   ntdll.dll(64λ) -- 64λϵͳ�ĵ��ýӿ�
	*****************************************************************************************************/

    /*****************************************************************************************************
    * �ض���
    *   �ļ�ϵͳ�ض���(File System Redirector)
    *     64λϵͳ��32λ����C:Windows\SysWow64��32λ������ͼ��System32�м��ؿ�ʱ�ᱻWOW64�ض���
    *     ���⣺%windir%\system32\ �µ� catroot, drivers\etc, logfiles, spool ��
    *   ע����ض���(Registry Redirector)
    *     WoW64�ض��򼸸�ע���·�����磺HKLM\Software �� HKLM\Software\Wow6432Node��
    *     RegXxxEx �ĺ����п���ʹ�� KEY_WOW64_64KEY(����64λע���) /KEY_WOW64_32KEY(����32λע���) /
    *       KEY_WOW64_RES(�Ӷ���������ͬʱ 32KEY �� 64KEY����������?) ����
    *     ���⣺HKLM\Software �µ� Classes��Microsoft\EventSystem��Microsoft\Ole �� HKEY_USERS\*\Software\Classes ��
    *            HKLM\Software\Classes\CLSID -- �����е�����(ע���32λCOM��local Server�����ӳ�䵽64������In-Proc��ӳ��)
    *   ע�����(Registry Reflection)
    *     ���ڴ���ע����ض���ʱ���������(������� HKLM\Software\Wow6432Node\Classes ���½�ֵ�����̸��Ƶ� HKLM\Software\Classes  �� )
    * 
    * Wow64DisableWow64FsRedirection-- ��ֹ�ļ�ϵͳ�ض���
    * Wow64EnableWow64FsRedirection -- �����ļ�ϵͳ�ض���
    * Wow64RevertWow64FsRedirection -- �ָ��ļ�ϵͳ�ض���
    * RegEnableReflectionKey  -- ��ע����书��
    * RegDisableReflectionKey -- ����ע����书��
    *****************************************************************************************************/
    
    class CFWow
    {
    public:
        FTLINLINE CFWow(void);
        FTLINLINE virtual ~CFWow(void);
    };

}

#endif //FTL_WOW_H

#ifndef USE_EXPORT
#  include "ftlWow.hpp"
#endif