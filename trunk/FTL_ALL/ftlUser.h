#ifndef FTL_USER_H
#define FTL_USER_H
#pragma once

#ifndef FTL_BASE_H
#  error ftlUser.h requires ftlbase.h to be included first
#endif

#pragma TODO(�� FUUser �ļ���ͬ��)
//����:
//http://blog.csdn.net/chenjinxian88/article/details/8933073
//http://blog.csdn.net/xbgprogrammer/article/details/16818973
//https://msdn.microsoft.com/zh-cn/library/2007.07.securitywatch.aspx
//http://pnig0s1992.blog.51cto.com/393390/908495/
//http://wenku.baidu.com/link?url=tiLVXTK_8BYakF985QIRX9GFNiWKiLtKuRBJne0bDNmw-3379rz8wAAAjFkgvCWw7dN_zlHZMvRJGRszH9D_T5N5cJBwauafSfUKm2F32g3
//http://mqc173.blog.163.com/blog/static/30899093200751110582422/

/*********************************************************************************************************************************
* ���ʵ��
* 
* ����
*   cacls|icacls(�Ƽ�) -- ϵͳ�Դ���������ACL������
*     ʹ�÷�ʽ�μ�: https://technet.microsoft.com/zh-cn/magazine/2007.07.securitywatch.aspx
*   
*
* TODO:
*   CreateWellKnownSid -- ?
*********************************************************************************************************************************/


//�û��ʺſ���(UAC) -- http://www.cnblogs.com/rainbowzc/archive/2012/02/28/2371526.html
/*********************************************************************************************************************************
* ��ȫ����
*   ACE(Access Control Entry) -- 
*   ACL(Access Control List) -- ��0����ACE�������б����, ���������͵�ACL:
*     DACL(Discretionary ACL) -- ������ʿ����б�, �������û�������б��Լ���Ӧ��Ȩ�ޣ������ܾ���
*        ÿһ���û�������������ʿ����б��ж��������Ȩ�ޡ����һ������û��DACL����ô����˵����������κ��˶�����ӵ����ȫ�ķ���Ȩ��.
*     SACL(System ACL) -- ϵͳ���ʿ����б�Ϊ��˷���ģ������˶��󱻷��ʵ�ʱ��
*   Security Context -- ��ȫ�����ģ�����ĳ������������ʲô����ɺ�Ȩ�޵ļ��ϣ�ͨ����¼�Ựȷ����ͨ����������ά����
*   SD(Security Descriptors) -- ��ȫ������,����Ȩ�޵�������Ϣ,��Ӧ SECURITY_DESCRIPTOR �ṹ,��Ҫ�� SID + DACL + SACL �����.
*   Securable Object -- ��ȫ����,��ӵ��SD��Windows�Ķ���(��Ҫ�Ǹ���Handle)
*   SID(Security Identifier) -- ��ȫ��ʶ��, ��ʶ�û�����ͼ�����ʻ���Ψһ�ĺ���,�ڲ���ȫ����ͨ��SID����(�����û���������),
*     ���ɾ���˻����ٴ���ͬ�����˻�,Ҳ�޷��̳�֮ǰ��Ȩ�ޡ�
*     ע��: Ghost �Ļ�������Ϊʹ����ͬ��SID��������صİ�ȫ���� -- �°汾��Ghost��ҵ����ڿ���̨���޸�? ΢��Ҳ�ṩ�� Riprep ?
*     SID��Ϊ���õ�ϵͳ�˺� �� �û��˺�(�ɼ������+ʱ�� �������������)���磺S-1-5-21-310440588-250036847-580389505-500����ϸ�ĸ�ʽ˵��:
*     S(������SID)-1(�汾��)-5(��־���İ䷢����)-21(�Ӱ䷢����)-310440588-250036847-580389505-(ID authority����ֵ֤����Ϣ)-500(��־���ڵ��ʻ�����?)
*     HKLM\SOFTWARE\Microsoft\Windows NT\CurrentVersion\ProfileList ���Բο�SID���û���(�μ� ProfileImagePath )�Ķ�Ӧ��ϵ? TODO: �Ƿ��к���?
*   Security Principal -- Windows���εİ�װ��ȫ����
*   LUID -- locally unique identifier, ����Ψһ��ʶ������ϵͳ��ÿһ�������ڼ䱣֤��Ψһ�������ֵͨ��û��ʵ�����塣ͨ����Ҫͨ�� LookupPrivilegeName ��ȡ��Ӧ��Ȩ��������Ϣ
*
*   DEP -- ����ִ�б���
*   NTLM -- NT LAN Manager(win2000���ڰ汾�е�һ����֤��ݷ�ʽ), ����һ��"����-��"���������пͻ�����֤
*   MIC(Mandatory Integrity Control) -- ǿ�������Կ��ơ�Vista�е����а�ȫ�Զ���ͽ��̶���һ�������Լ���
*     �����Լ���(Integrity Level -- IL)�͵Ľ��̲����޸�(ͨ�����Զ�ȡ?)����ߵ��ļ���ע�����
*     ע�⣺Win2K/XP �£���ȫ��ϵͳֻ�ѽ��̵ķ������ƺ���Դ�ķ��ʿ����б����ƥ��Ƚϣ���ȷ�ϸý����Ƿ���з��ʸ���Դ��Ȩ�ޡ�
*     0.AppContainer -- Win8 ����,�Ǹ�ɳ�л��ƣ��͵Ľ��̲��ܶ�д(ע��:�Ѿ����ܶ���)����ߵ��ļ���ע����
*             ����Metro-sytle��Ӧ�� ��������EPM�� IE Tab���� ������������,��Ҫ����Ȩ��?
*     1.Untrusted(�����μ���) -- �����ø����������ӻỰ, TODO: Chrome.exe �Ĳ��ֽ�����ʾΪ���
*     2.Low(��) -- ����ģʽ�µ�IE�����, �� Untrusted ��ϵͳȨ�����г���ֻ�ܴ�ȡ�ʹ�ȡ�����·��λ�ã��� "Temporary Internet Files\Low"��
*       ע�⣺δ���û�ͬ������ص���Ϊ�����ڵͼ���
*             �û��ֶ������ػ�����ִ�м���һ������ �������еȼ���
*             ��һ������Ȩ�޵��û���ͬ�������(���簲װһ��ActiveX�ؼ�)�������ڸ������Լ���
*     3.Medium(�е�) -- Ĭ������£��û�����Ĵ��룬��Windows Explorer�����������, ʹ��"User"Ȩ�ޣ��ܶ�д�û����ļ���ע�����
*     4.High(��) -- ������ administrator�û� ������Ȩ�޺�ĳ���
*     5.System(ϵͳ) -- �ں˼����Windows�ļ�
*     6.�������̼��� -- ������ߣ�ֻ����ϵͳ��Ҫ��ʱ��Żᱻʹ��
*
*   Ӧ���ļ���ע�����⻯(�����ض���) -- ��ָ��λ�ý��ж�д��ʱ��ᱻ�ض���ÿ���û������⻯������
*     ������λ�ý�����һ����"�û������ļ���"��ȫһ�µ��������Լ���Ϊ"Low"��Ŀ¼���:
*     %LocalAppData%\Microsoft\Windows\Temporary Internet Files\Virtualized\XXXX -- XXX���ּ���Ӧ�õ� C:\ProgramData\MyProgramData ��
*     HKCU/Software/Microsoft/InternetExplorer/InternetRegistry -- TODO:��δȷ��
* 
*     ����ע�����п��ƣ���HKLM\SYSTEM\CurrentControlSet\services\luafv

*   LSA(Local Security Authority) -- ���Ʊ���Ȩ�޵�API
*     Ȩ�޹���API���̣�
*       LsaOpenPolicy -> LookupAccountName/LsaLookupNames(get SID) -> LsaAddAccountRights/LsaRemoveAccountRights(���ӻ����Ȩ��) -> LsaClose
*         LsaEnumerateAccountRights(ö���û�Ȩ��), LsaEnumerateAccountsWithUserRight(ö����ָ��Ȩ�޵��û�)
* 
*   FUS(Fast User Switch) -- �����û��л�
*   �Ự����(Session Isolation) -- Vista������ϵͳ���������ڻỰ0����ǿϵͳ����İ�ȫ��
*   Restricted Token -- ���޷�������
*   SFP -- ϵͳ�ļ�����, Win2K ��ǰ���ļ���������
*   UAC(User Account Control) -- �û��ʻ����ƣ� ����Ա��¼ʱ��Ϊ�õ�¼�Ự������������ͬ�ķ������ƣ�Ĭ��ʹ�����޷�������(����ʱָ���˽���SID��ɾ����ĳЩȨ��)��
*     �Լ���Windows Vistaϵͳ���ܹ����棻��ҪȨ������ʱ����ʹ�÷����Ʒ����������ṩ�İ�ȫ������������Ӧ�ó���
*     UAC Elevation -- ʵ����һ��������ò�ͬ��ģ�����У���Virtualizationӳ�䵽����λ�õ�Data�ļ��������ڲ��������ļ��Լ�ע����ʱ��ʵ������һ��������ò�ͬ��ģ�����С�
*       CheckElevationEnabled -- Kernel32.dll �е�δ����������typedef DWORD (WINAPI* CheckElevationEnabledProc)(BOOL* pResult);
*   UIPI(User Interface Privilege Isolation) -- �û�������Ȩ���룬�����Լ����(lower integrity)�Ľ��̣������������Լ���ߵĽ��̷���Window��Ϣ��
*     Ĭ������£������� WM_USER ���ϵ���Ϣ���ᱻ���Σ�����ͨ�� ChangeWindowMessageFilter[EX](WM_xxx, MSGFLT_ADD) ����ӵ������Խ��̴�������Ϣ
*   WFP -- Windows�ļ�����, Win2Kʱ���룬ֻ�����ļ�
*   WRP -- Windows��Դ�������Թؼ��ļ����ļ��к�ע���ֵ���б���
*
* Ȩ�������ķ�ʽ(���������������û���LUA����Ӧ�ó��򼤻�һ������Ȩ�޵�COM���󣬼���Vistaϵͳ�ϻᵯ��UAC��ʾ�����û�����Ȩ�ޣ�UAC��״̬�£�)
*   1.manifest �е� requireAdministrator ?
*   2.COM Elevation Moniker -- �μ� ftlCom.h 
*
* ��ȫ������
*   
* ����
*   CreateRestrictedToken -- �������еķ������Ƶ�Լ������һ���µ����޷�������
*   ConvertStringSecurityDescriptorToSecurityDescriptor -- ����ȫ��������ʽ���ַ���ת������Ч�İ�ȫ�������ṹ
*********************************************************************************************************************************/


namespace FTL
{
    enum IntegrityLevel{
        ilUnknown = 0,
        ilAppContainer, //Win8 + IE10 + EPM �� Metro ʱ, 
        ilLowCompat,    //TODO: ��ҳҪ�� ActiveX control which is not EPM-compatible ʱ
        ilUntrusted,
        ilLow, 
        ilMedium,
        ilHigh,
        ilSystem,
        ilProtectedProcess
    };

	class CFUserUtil
	{
	public:
        FTLINLINE static IntegrityLevel GetIntegrityLevel(DWORD dwIntegrityLevel);
        FTLINLINE static LPCTSTR GetIntegrityLevelString(IntegrityLevel iLevel);
        FTLINLINE static LPCTSTR GetWellKnownSidTypeString(WELL_KNOWN_SID_TYPE sidType);
        FTLINLINE static LPCTSTR GetAclInfo(CFStringFormater& formater, PACL pAcl);
        FTLINLINE static LPCTSTR GetPrivilegeNameByLuid(CFStringFormater& formater, PLUID pLuid, LPCTSTR lpSystemName = NULL);
        FTLINLINE static LPCTSTR GetSidInfo(CFStringFormater& formater, PSID pSid, BOOL bGetSubAuthority);
        FTLINLINE static LPCTSTR GetSidAttributesString(CFStringFormater& formater, DWORD dwAttributes, LPCTSTR pszDivide /*= TEXT("|") */);
        FTLINLINE static LPCTSTR GetSidAndAttributesInfo(CFStringFormater& formater, PSID_AND_ATTRIBUTES pSidAndAttributes, BOOL bGetSubAuthority, LPCTSTR pszDivide);
        FTLINLINE static LPCTSTR GetPrivilegeAttributesString(CFStringFormater& formater, DWORD dwAttributes, LPCTSTR pszDivide /*= TEXT("|") */);
        FTLINLINE static LPCTSTR GetSecurityDescriptorinfo(CFStringFormater& formater, SECURITY_DESCRIPTOR* pSecurityDescriptor);

        //�жϵ�ǰ�û�(��ǰ���̵�Owner)�Ƿ��Ǳ��� Adminstrators ���еĳ�Ա(ע�⣺�������)
        FTLINLINE static BOOL IsProcessUserAdministrator();
        FTLINLINE static BOOL IsVistaUACEnabled();

        FTLINLINE static IntegrityLevel GetProcessIntegrityLevel(HANDLE hProcess);

        //TODO: ����������ȷ����Ҫ��д�� ����һ���κ��˶�����ʹ�õ� SECURITY_ATTRIBUTES  -- ATL�����к�����
        FTLINLINE static BOOL CreateEmptySecurityAttributes(SECURITY_ATTRIBUTES* pSecurityAttr);

        //����ָ��Token(��������)��Ȩ����Ϣ
        FTLINLINE static BOOL DumpTokenInformation(HANDLE hToken);
    private:
        static LPCTSTR WINAPI GetTokenReservedInfo(CFStringFormater& formater, LPVOID TokenInformation, DWORD TokenInformationLength);
        static LPCTSTR WINAPI GetTokenDwordInfo(CFStringFormater& formater, LPVOID TokenInformation, DWORD TokenInformationLength);
        static LPCTSTR WINAPI GetTokenUserInfo(CFStringFormater& formater, LPVOID TokenInformation, DWORD TokenInformationLength);
        static LPCTSTR WINAPI GetTokenGroupsInfo(CFStringFormater& formater, LPVOID TokenInformation, DWORD TokenInformationLength);
        static LPCTSTR WINAPI GetTokenPrivilegesInfo(CFStringFormater& formater, LPVOID TokenInformation, DWORD TokenInformationLength);
        static LPCTSTR WINAPI GetTokenOwnerInfo(CFStringFormater& formater, LPVOID TokenInformation, DWORD TokenInformationLength);

        static LPCTSTR WINAPI GetTokenPrimaryGroupInfo(CFStringFormater& formater, LPVOID TokenInformation, DWORD TokenInformationLength);
        static LPCTSTR WINAPI GetTokenDefaultDaclInfo(CFStringFormater& formater, LPVOID TokenInformation, DWORD TokenInformationLength);
        static LPCTSTR WINAPI GetTokenSourceInfo(CFStringFormater& formater, LPVOID TokenInformation, DWORD TokenInformationLength);
        static LPCTSTR WINAPI GetTokenTypeInfo(CFStringFormater& formater, LPVOID TokenInformation, DWORD TokenInformationLength);
        static LPCTSTR WINAPI GetSecurityImpersonationLevelInfo(CFStringFormater& formater, LPVOID TokenInformation, DWORD TokenInformationLength);
        static LPCTSTR WINAPI GetTokenStatisticsInfo(CFStringFormater& formater, LPVOID TokenInformation, DWORD TokenInformationLength);
        static LPCTSTR WINAPI GetTokenGroupsAndPrivilegesInfo(CFStringFormater& formater, LPVOID TokenInformation, DWORD TokenInformationLength);
        static LPCTSTR WINAPI GetTokenOriginInfo(CFStringFormater& formater, LPVOID TokenInformation, DWORD TokenInformationLength);
        static LPCTSTR WINAPI GetTokenElevationTypeInfo(CFStringFormater& formater, LPVOID TokenInformation, DWORD TokenInformationLength);
        static LPCTSTR WINAPI GetTokenLinkedTokenInfo(CFStringFormater& formater, LPVOID TokenInformation, DWORD TokenInformationLength);
        static LPCTSTR WINAPI GetTokenElevationInfo(CFStringFormater& formater, LPVOID TokenInformation, DWORD TokenInformationLength);
        static LPCTSTR WINAPI GetTokenAccessInformationInfo(CFStringFormater& formater, LPVOID TokenInformation, DWORD TokenInformationLength);
        static LPCTSTR WINAPI GetTokenMandatoryLabelInfo(CFStringFormater& formater, LPVOID TokenInformation, DWORD TokenInformationLength);
        static LPCTSTR WINAPI GetTokenMandatoryPolicyInfo(CFStringFormater& formater, LPVOID TokenInformation, DWORD TokenInformationLength);
        static LPCTSTR WINAPI GetTokenAppcontainerInformationInfo(CFStringFormater& formater, LPVOID TokenInformation, DWORD TokenInformationLength);
        static LPCTSTR WINAPI GetClaimSecurityAttributesInformationInfo(CFStringFormater& formater, LPVOID TokenInformation, DWORD TokenInformationLength);
	};

    //How To Manage User Privileges Programmatically in Windows NT
    //  support.microsoft.com/kb/132958

    //TODO: δд��
    class CFUserPrivileges
    {
    public:
        FTLINLINE CFUserPrivileges();
        FTLINLINE ~CFUserPrivileges();
        FTLINLINE BOOL Open();
        FTLINLINE VOID Close();
        //����Ȩ�ޣ��� L"SeServiceLogonRight", ���ڲ��ɵ��� LsaAddAccountRights/LsaRemoveAccountRights
        FTLINLINE BOOL SetPrivilegeOnAccount(LPWSTR PrivilegeName, BOOL bEnable ); 
    private:
        //FTLINLINE void _InitLsaString(PLSA_UNICODE_STRING LsaString,LPWSTR String);
    private:
        //LSA_HANDLE  m_hPolicy;
    };
}
#endif //FTL_USER_H

#ifndef USE_EXPORT
#  include "ftlUser.hpp"
#endif 