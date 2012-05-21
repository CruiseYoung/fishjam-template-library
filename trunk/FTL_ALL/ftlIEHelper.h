#ifndef FTL_IE_HELPER_H
#define FTL_IE_HELPER_H

#pragma once

#ifndef FTL_BASE_H
#  error ftlIEHelper.h requires ftlbase.h to be included first
#endif

#ifndef FTL_COM_H
#  error ftlIEHelper.h requires ftlCom.h to be included first
#endif


/********************************************************************************************
* ��ȫ����
*   Security Principal -- Windows���εİ�װ��ȫ����
* 
*   DEP -- ����ִ�б���
*   MIC(Mandatory Integrity Control) -- ǿ�������Կ��ơ�Vista�е����а�ȫ�Զ���ͽ��̶���һ�������Լ���
*     �����Լ���(Integrity Level -- IL)�͵Ľ��̲����޸�(���Զ�ȡ?)����ߵ��ļ���ע�����
*     ע�⣺Win2K/XP �£���ȫ��ϵͳֻ�ѽ��̵ķ������ƺ���Դ�ķ��ʿ����б����ƥ��Ƚϣ���ȷ�ϸý����Ƿ���з��ʸ���Դ��Ȩ�ޡ�
*     1.�����μ��� -- �����ø����������ӻỰ
*     2.Low(��) -- ����ģʽ�µ�IE�����, �� Untrusted ��ϵͳȨ�����г���ֻ�ܴ�ȡ�ʹ�ȡ�����·��λ�ã��� "Temporary Internet Files\Low"��
*       ע�⣺δ���û�ͬ������ص���Ϊ�����ڵͼ���
*             �û��ֶ������ػ�����ִ�м���һ������ �������еȼ���
*             ��һ������Ȩ�޵��û���ͬ�������(���簲װһ��ActiveX�ؼ�)�������ڸ������Լ���
*     3.Medium(�е�) -- Ĭ������£��û�����Ĵ��룬��Windows Explorer�����������, ʹ��"User"Ȩ�ޣ��ܶ�д�û����ļ���ע�����
* ����4.High(��) -- ������ administrator�û� ������Ȩ�޺�ĳ���
* ����5.System(ϵͳ) -- �ں˼����Windows�ļ�
*     6.�������̼��� -- ������ߣ�ֻ����ϵͳ��Ҫ��ʱ��Żᱻʹ��
*     
*   Ӧ���ļ���ע�����⻯(�����ض���) -- ��ָ��λ�ý��ж�д��ʱ��ᱻ�ض���ÿ���û������⻯������
*     ������λ�ý�����һ����"�û������ļ���"��ȫһ�µ��������Լ���Ϊ"Low"��Ŀ¼���:
*     %UserProfile%\AppData\Local\Microsoft\Windows\Temporary Internet Files\Virtualized\XXXX -- XXX���ּ���Ӧ�õ� C:\ProgramData\MyProgramData ��
*
*     ����ע�����п��ƣ���HKLM\SYSTEM\CurrentControlSet\services\luafv
*   SFP -- ϵͳ�ļ�����, Win2K ��ǰ���ļ���������
*   UAC(User Account Control) -- �û��ʻ����ƣ� �ù���Ա�ʻ��Զ����һ����׼�û��ķ������ƣ��Լ���Windows Vistaϵͳ���ܹ�����
*   UIPI(User Interface Privilege Isolation) -- �û�������Ȩ���룬�����Լ���͵Ľ��̣������������Լ���ߵĽ��̷���Window��Ϣ��
*   WFP -- Windows�ļ�����, Win2Kʱ���룬ֻ�����ļ�
*   WRP -- Windows��Դ�������Թؼ��ļ����ļ��к�ע���ֵ���б���
*
* IE 7 �ı���ģʽ(Protected Mode) -- Ҳ��IE��Ȩ��(Low Rights)��ͨ�� Process Explorer �鿴���̵�"��ȫ"���ԣ��ɿ�����
*   "Mandatory Label\Low Mandatory Level"��SID(��Ӧ�ı�־λ��"Integrity")�������IE���̵������Լ�����"Low"��
*   "Virtualized" Ϊ Yes
*   ����ģʽ�·��ʵ��ļ�Ŀ¼(�����Լ�����"Low")��
*     1.IE��ʱ�ļ�: %UserProfile%\AppData\Local\Microsoft\Windows\Temporary Internet Files\Low
*     2.ϵͳ��ʱĿ¼: %UserProfile%\AppData\Local\Temp\Low
*     3.Cookies: %UserProfile%\AppData\Roaming\Microsoft\Windows\Cookies\Low
*     4.��ʷ: %UserProfile%\AppData\Local\Microsoft\Windows\History\Low
*     5.�ղؼ�: �ղؼ�Ĭ������ͼ����IE���̷��ʣ���˲���Ҫ������Ŀ¼��
*   ������������⣺
*     1.ͨ������NTFS������(Soft Link)�ķ��������IE����ģʽ�ļ��������⣺linkd ԴĿ¼ ��Ȩ�޵�Ŀ��Ŀ¼
*       ʵ�ʣ���NTFS�ļ�����һ���ؽ�����(REPARSE)������
*       Linkd ��ִ�г����� Windows Server 2003 Resource Kit Tools���߰� ��
*     2.SHGetSpecialFolderPath(NULL, szSavePath, CSIDL_LOCAL_APPDATA , TRUE); //ȡ�� %UserProfile%\AppData\Local
*       CString strFilePath(szSavePath); 
*       strFilePath += TEXT("\\Temp\\Low\\nCapture\\IEToobar\\");  -- ֮��ķ��ʶ��ھ��� Low �����Եȼ���Ŀ¼��
*   
* �������(��Ȩ����˼��) -- �Ѵ�ͳ��IE����һ��Ϊ�����ֱ���ͬ��ȫ�ȼ�������.
*   IE���̱��� -- �����õġ�������Ȩ����ҳ�������
*   IeUser.exe(�û����������) -- ����е���Ȩ������(���籣��ͼƬ��)
*   IeInstal.exe(�����������) -- ��ɸ���Ȩ������(���簲װ������)��
* 
* IE�еĶ���̿���
*   HKLM��HKCR \SOFTWARE\Microsoft\Internet Explorer\MAIN\TabProcGrowth  -- REG_SZ, Ϊ 0 ��ʾ�� Ϊ Medium ��ʾ
*
* Ȩ�޼�鹤�ߣ�AccessChk -- ��֪��ָ���˺Ŷ��ض�Ŀ¼��Ȩ�޺������Լ����
*   AccessChk -i Ŀ¼ -- �鿴Ŀ¼���ļ��������Լ���
*   AccessChk -d -q %AppData% 
********************************************************************************************/

namespace FTL
{
    class CFIEUtils
    {
    public:
		FTLINLINE static BOOL IsProtectedModeProcess();
    };

}//namespace FTL

#ifndef USE_EXPORT
#  include "ftlIEHelper.hpp"
#endif

#endif //FTL_IE_HELPER_H
