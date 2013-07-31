#ifndef FTL_WOW_H
#define FTL_WOW_H

#pragma once

#ifndef FTL_BASE_H
#  error ftlWow.h requires ftlbase.h to be included first
#endif

namespace FTL
{
	/*****************************************************************************************************
	* ��ʼ����64λ Windowsϵͳ���֮ǰ��Ҫ�˽����Ϣ -- http://www.microsoft.com/china/MSDN/library/Windev/64bit/issuesx64.mspx?mfr=true
    * WOW/WOW64 -- Windows On Windows /64-bit 
    *   64λ���� -- ��Ϊ AMD64, Intel��EM64T, IA64�� ��ʲô����?
    *   ��Ҫ������Ѹ�ٶ�����ؽ����е� 32 λӦ�ó�����ֲ�� 64 λ������ȥ����ʱ�����Դ�����У�
    *   ָ�뱻��Ϊ32λֵ������򵥵����±�дӦ�ó��򣬾ͻ����ָ�뱻�ضϵĴ���Ͳ���ȷ���ڴ���ʡ�
    *   Ĭ������£���ϵͳȷ������ 0x000000007FFFFFFF ���ϵ��ڴ��ַ���з��䣨��Ҫʹ6 4λӦ�ó����ܹ�����
    *   ����ȫ�� 4TB �û���ʽ��������Ӧ�ó������ʹ�� /LARGEADDRESSAWARE ���ӿ�������������
    *   ��ϵͳ�������̵� 64 λ��ַ�ռ�ʱ��Ҫ���һ����ִ���ļ��� LARGEADDRESSAWARE ��־���������DLL����ôϵͳ�����Ըñ�־��
    * 
    * �������ʱ��ʹ�� UINT_PTR��DWORD_PTR ���Զ�֧�� 32/64 �ı���
	*
	* 64λ����ʹ��ע���ʱ����Ҫʹ�� KEY_WOW64_32KEY ����?
	*   CRegKey reg;
	*   #if _WIN64
	*   if ( ERROR_SUCCESS == reg.Open(HKEY_LOCAL_MACHINE, _T("SOFTWARE\Microsoft\VisualStudio"),	KEY_READ | KEY_WOW64_32KEY) )
	*   #else
	*   if ( ERROR_SUCCESS == reg.Open(HKEY_LOCAL_MACHINE, _T("SOFTWARE\Microsoft\VisualStudio"),	KEY_READ) )
	*   #endif
	*
	* 64λOS��ע��DLLʱ(�ǲ��Ƿ���?)��
	*   32-bit DLL -- syswow64\regsvr32.exe
	*   64-bit DLL -- system32\regsvr32.exe
    *
    * �ļ�  ��64λϵͳ��32λ����C:Windows\SysWow64��32λ������ͼ��System32�м��ؿ�ʱ�ᱻWOW64�ض���
    * ע���WoW64�ض���HKLM\Software��HKLM\Software\Wow6432Node
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