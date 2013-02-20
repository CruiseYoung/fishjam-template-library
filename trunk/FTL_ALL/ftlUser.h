#ifndef FTL_USER_H
#define FTL_USER_H
#pragma once

#ifndef FTL_BASE_H
#  error ftlUser.h requires ftlbase.h to be included first
#endif

#pragma TODO(�� FUUser �ļ���ͬ��)
//�û��ʺſ���(UAC) -- http://www.cnblogs.com/rainbowzc/archive/2012/02/28/2371526.html

/*********************************************************************************************************************************
* ��ȫ����
*   Security Context -- ��ȫ�����ģ�����ĳ������������ʲô����ɺ�Ȩ�޵ļ��ϣ�ͨ����¼�Ựȷ����ͨ����������ά����
*   Security Principal -- Windows���εİ�װ��ȫ����
*   LUID -- locally unique identifier
*
*   DEP -- ����ִ�б���
*   NTLM -- NT LAN Manager(win2000���ڰ汾�е�һ����֤��ݷ�ʽ), ����һ��"����-��"���������пͻ�����֤
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
* 
*   Restricted Token -- ���޷�������
*   SFP -- ϵͳ�ļ�����, Win2K ��ǰ���ļ���������
*   UAC(User Account Control) -- �û��ʻ����ƣ� ����Ա��¼ʱ��Ϊ�õ�¼�Ự������������ͬ�ķ������ƣ�Ĭ��ʹ�����޷�������(����ʱָ���˽���SID��ɾ����ĳЩȨ��)��
*     �Լ���Windows Vistaϵͳ���ܹ����棻��ҪȨ������ʱ����ʹ�÷����Ʒ����������ṩ�İ�ȫ������������Ӧ�ó���
*     UAC Elevation -- ʵ����һ��������ò�ͬ��ģ�����У���Virtualizationӳ�䵽����λ�õ�Data�ļ��������ڲ��������ļ��Լ�ע����ʱ��ʵ������һ��������ò�ͬ��ģ�����С�
*   UIPI(User Interface Privilege Isolation) -- �û�������Ȩ���룬�����Լ����(lower integrity)�Ľ��̣������������Լ���ߵĽ��̷���Window��Ϣ��
*     Ĭ������£������� WM_USER ���ϵ���Ϣ���ᱻ���Σ�����ͨ�� ChangeWindowMessageFilter[EX](WM_xxx, MSGFLT_ADD) ����ӵ������Խ��̴�������Ϣ
*   WFP -- Windows�ļ�����, Win2Kʱ���룬ֻ�����ļ�
*   WRP -- Windows��Դ�������Թؼ��ļ����ļ��к�ע���ֵ���б���
*
* Ȩ�������ķ�ʽ(���������������û���LUA����Ӧ�ó��򼤻�һ������Ȩ�޵�COM���󣬼���Vistaϵͳ�ϻᵯ��UAC��ʾ�����û�����Ȩ�ޣ�UAC��״̬�£�)
*   1.manifest �е� requireAdministrator ?
*   2.COM Elevation Moniker -- COM�������ע�������Ȩ������֧�ֱ�ʶ( AppID/DllSurrogate, Elevation/Enabled=1, LocalizedString="xxx", )
*   
* ����
*   CreateRestrictedToken -- �������еķ������Ƶ�Լ������һ���µ����޷�������
*********************************************************************************************************************************/


namespace FTL
{
	class CFUser
	{
	public:
	};
}
#endif //FTL_USER_H

#ifndef USE_EXPORT
#  include "ftlUser.hpp"
#endif 