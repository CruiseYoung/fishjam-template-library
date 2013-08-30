#ifndef FTL_SERVICE_H
#define FTL_SERVICE_H
#pragma once

#ifndef FTL_BASE_H
#  error ftlService.h requires ftlbase.h to be included first
#endif

/*************************************************************************************************************************
* SCM(Service Control Manager)--������ƹ������
* 
* Windows����ֳ��������ͣ�interactive service (����ʽ����)��non-interactive service���ǽ���ʽ���񣩡�
* 1.���һ��������"LocalSystem Account"���У�����������"SERVICE_INTERACTIVE_PROCESS"���ԡ�
*   ��ô���ͱ���Ϊ��һ��interactive service������ʽ�����ڷ�Vistaϵͳ�£�XP,NT����
*   ʹ��"interactive window station"(WinSta0)����ʾUI�����û����룬���û����н�����
*   ����"SERVICE_INTERACTIVE_PROCESS"���ԣ�
*     ��������ҳ->["log on"����ҳ]->[local system account] ->[Allow service to interact with desktop]
* 2.���һ��������"LocalSystem Account"���У�����û������"SERVICE_INTERACTIVE_PROCESS"����,��ʹ����Ϊ
*   "Service-0x0-3e7$\default" ��"window station",�ǽ�����,XP/2003/Vista ����ͬ;
* 3.���������"User Account"���У�����ʹ�õ�"window station"�����ֻ����û���SID:
*   Service-0xZ1-Z2$, Z1, Z2�ֱ����SID�ĸ�λ�͵�λ -- �ǽ�����.
*
* Interactive service���ڵİ�ȫ����( WinXP�� �������͵�һ����¼�û���Ӧ�ó��� ��ͬһ��Session 0������)��
*   ��Vista�£������е�ϵͳ����ͷ�����򶼸��뵽һ��Session 0�Ŀռ���,����Ӧ�ó�����ͨ�������Session���С�
*   �����û�����ͷ��������������ȫ�����Session��,����ʹ�ò�ͬ��window station������,���ֿռ�ķ��룬
*   ʹ���û�����û�л�����ʵ��������Ĵ��ھ������������Ч�ı����˲����û�����Ĺ�����
*   ע�⣺��ͨ��ProcessExplorer �� Security ����ҳ�鿴��������Session
*
* Session���������Ӱ�� -- 
*  �ڷ���ǰ����������������û�����ͨ�ţ�ֻ��Ҫ��Խ���̱߽磬
*  ������󣬲������ǿ�Խ���̱߽磬����Ҫ��ԽSession�߽���ܽ��С�
*  1>.��ͬSession�ڵĽ����޷�������ʸ��ԵĴ��ھ��,����ʹ�� PostMessage/SendMessage �Ⱥ���;
*  2>.�����ں˶�������û����̺ͷ�����̵�ͬ��ʱ���ڷ���Sessionģʽ�£���Ҫָ���ں˶�������ֿռ�(�� "Global\\Event1")
* 
* Services��UI��ʾ���û�������:
*  1> WTSSendMessage(WTS_CURRENT_SERVER_HANDLE, WTSGetActiveConsoleSessionId(), ) -- ��ʾ�򵥵�Message
*  2> ���ӵ�UI -- ʹ�� CreateProcessAsUser()���û���Session�ڴ���һ�����̣��������������ʾUI.
*     WTSQueryUserToken(WTSGetActiveConsoleSessionId(), &hToken) -> DuplicateTokenEx -> CreateEnvironmentBlock -> CreateProcessAsUser
*  3>.�κ�֧�ֿ�Խ������ͨ�ŷ�ʽ��֧�ֿ�ԽSession����ͨ��,�����õķ�ʽ��RPC(Զ�̹��̵���)Named Pipes(�����ܵ�)��WCF �ȡ�
*
* ����İ�ȫ����
*   1.������ֹͣ����(�� Fasoo) -- AcceptStop �� AcceptPause ���ó� FALSE�� ��ͨ�����õķ�ʽ��
*     ��ͨ�������в鿴����ȷ�ϣ�wmic service where name="������" get /value
*     ���⣺����Ƕ������̵ķ��������ֹ���̣������������svchost���������û���ˣ���Ϊ��ǿ��ͣ����Ҳ���ͬһ�����µ���������Ҳ�˳���
*   2.��ֹ ���� -- 
*   3.DesktopInteract = FALSE
*************************************************************************************************************************/

/*************************************************************************************************************************
* ATL �������Service(Exe)
*   ��װ����(�����ע��, Register as Service����SCM�в鿴)��XXX /Service, 
*   TODO: /RegServer �Ǳ��ط�����ע��(ע��COM��)
*   ж�ط���XXX /UnRegServer
*   ����|ֹͣ ���� net start|stop ��������
* 
* ����
*   �������ƣ�IDS_SERVICENAME
*   �������ú������ȣ����� CAtlServiceModuleT::RegisterAppId(bool bService = false){ ChangeServiceConfig + ChangeServiceConfig2(SERVICE_CONFIG_DESCRIPTION) }
*   �޸İ�ȫ����(����ͻ��˵���ʱ���ܻ� E_ACCESSDENIED)��InitializeSecurity ��
*    CoInitializeSecurity(NULL, -1, NULL, NULL, RPC_C_AUTHN_LEVEL_NONE, RPC_C_IMP_LEVEL_IDENTIFY, NULL, EOAC_NONE, NULL);
*
* ��չʵ��
*   1.ͨ������ ATL Simple Object �ķ�ʽ���ӹ��ܣ�
*     �ӿ�����ʹ�õĲ���������OLE���ݵģ�������QueryInterfaceʱ���ܻᱨ 0x80020008(DISP_E_BADVARTYPE) �Ĵ���
*   2.��Ҫ�ڶ�Ӧ�� rgs �ļ������� val AppID = s '%APPID%'������ͻ��˴��������ʱ��ᳬʱ��������0x80080005(CO_E_SERVER_EXEC_FAILURE)
*   3.��Win7�»� XP���û�����ʱ��Ҫ ע�� xxxPS �������ɵ�DLL(��Session����)
*
* ��������(WinXP��ǰ)
*   GetProcessWindowStation() -- ��ȡ��ǰ���̵� Window Station
*   OpenDesktop("default", xxx)
*   OpenWindowStation("winsta0", xxx)  
* 
*
*
*************************************************************************************************************************/

/*************************************************************************************************************************
* C#��������
*   �� ServiceBase �̳У��п����ص� OnStart/OnStop �ȷ�����
*   ��������ͨ��Installutil ������ص�ϵͳ������
*************************************************************************************************************************/

#pragma comment(lib, "wtsapi32.lib")
#pragma comment(lib, "Userenv.lib")

namespace FTL
{
    class CFService
    {
    public:
        //�жϷ����Ƿ�װ
        FTLINLINE static BOOL IsServiceInstalled(LPCTSTR lpServiceName);
        FTLINLINE static BOOL InstallService(LPCTSTR lpBinaryPathName,LPCTSTR lpServiceName, LPCTSTR lpDependencies);
        FTLINLINE static BOOL UninstallService(LPCTSTR lpServiceName);

        //ģ��Service ������ʾUI�Ĳ���
        //http://blog.csdn.net/sonsie007/article/details/8835830
        FTLINLINE static BOOL CreateServiceUIProcess(LPCTSTR pszProcessPath, BOOL bAsSystem, ULONG SessionId = WTSGetActiveConsoleSessionId());
    };

}
#endif //FTL_SERVICE_H

#ifndef USE_EXPORT
#  include "ftlService.hpp"
#endif 