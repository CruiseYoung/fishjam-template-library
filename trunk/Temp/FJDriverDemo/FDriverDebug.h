#ifndef F_DRIVER_DEBUG_H
#define F_DRIVER_DEBUG_H

/******************************************************************************************************************
* ���Ե�Checked�汾���� define DBG 1 �ĺ궨�壬�����п�ͨ�� #if DBG  XXXX #else YYYY #endif  �ķ�ʽ�������԰�ͷ�����
* ��ӡ��־��
*   DbgPrint("x=%d\n", 100) -- 
*     %#x -- ��ַ�����Զ��� 0x ��ͷ
*     %s -- ANSI �ַ���
*     %S/%ws -- UnicodeString.Buffer
*     %Z -- ANSI_STRING �����ĵ�ַ
*     %wZ -- UNICODE_STRING �����ĵ�ַ
*   KdPrint(("x=%d\n", 100)) -- DbgPrint �ĺ꣬ע��Ϊ�����ø�ʽ�����ܣ�һ����Ҫʹ����������
*
* �������Դ�ע�������ȡ��Ӧ���������������ԡ�
******************************************************************************************************************/

// LiveKd  ?
/******************************************************************************************************************
* һ��Ĺ��˵ȷ�Ӳ��������װ��Ϻ󣬻���"�����豸"����ʽ�����豸�������� "Not-Plug and Play Drivers"(NTʽ��)��
*
* SrvInstW.exe(http://www.onlinedown.net/soft/36059.htm)
*   ��������Ϊ����ķ�ʽ��װ��Ȼ�����ͨ�� net start/stop ������/ֹͣ ��������
*    ж��������sc delete xxxx
* InstDrv.exe -- 
*
* WinObj(http://technet.microsoft.com/en-us/sysinternals/bb896657.aspx)
*   ������ʾ���е��ں˶���
*
* ARK(�μ� FDriverRootkit.h ) -- ���������鿴����������
*
* Win2K ʱ���Ĳ�Ʒ
*   DriverStudio���߰�
*     DriverWorks -- Win2Kʱ��֧��WDM�����ĸ�������(�Ƿ������°汾?)���ṩ�˷ǳ����Ƶ���
*     DriverMonitor -- ���԰�װж��������������ֱ����ʾ�������
*     EzDriverInstaller -- ���Ժܷ����ͨ�� inf ��װж��WDM�����Ĺ���
*
* Win7ʱ���Ĳ�Ʒ( http://www.osronline.com ��վ�ƺ��в��ٺö�����)
*   DeviceTree -- ��ʾ���е�Pnp�豸�������������豸�Ĺ�ϵ(�豸��ջ ��)
*   IrpTrace -- ���Է���ظ���IRP�ĸ��ֲ�������ԭ���ǽ���һ�����������������丽������Ҫ�۲����������֮�ϣ�����ÿ��IRP����ǲ����������һ��������̽��и��١�
*
* ͨ��DebugView�鿴��ӡ����־��Ϣ����Ҫѡ�� Capture Kernel ���ܿ����ں����
* BusHound -- ����USB��������
* EzDriverInstaller -- ����WDMʽ����
* DeviceTree -- �鿴�豸����
* Driver Verifier  -- ?
*
* ���������
*   VMWare
*     ����Ŀ¼��Options -> Shared Foloders -> ���ӹ���Ŀ¼, �ù���Ŀ¼�������������еġ���������������(�ϰ汾VMWare��Ҫͨ�� \\.host\ ����)
*     �ùܵ����⴮�ڣ�Hardware -> Add -> Serial Port -> Output to named pipe -> Named pipe �е�ֵ����Ϊ��
*		" \\.\pipe\com_1 " , " This end is the server " , " The other end is an application ", ѡ�� "Conect at power on"�� TODO:" Yield CPU on poll "
*       ���Ի�������windbg(�����ÿ�ݷ�ʽ������)��windbg.exe -b -k com:port=\\.\pipe\com_1,baud=115200,pipe �����ú÷����ļ���Դ��·����
*       ��������ʱ����������Windowsϵͳ�ᱻ�ж�(ò��ʱ��),��Ҫ��WinDbg��������ʾ����ִ�� "g" ����������������У�
*       ����Դ���е��԰汾ʱ ���� _asm int 3 ������ϵ�, x64 ʹ�� __debugbreak
*
*
******************************************************************************************************************/
/******************************************************************************************************************
* WinDbg
*   �����ں�ʱ������˫�����ԣ�ͨ���������������磿
*   ���ñ����Ի���Ϊ����ִ�з�ʽ��WinDbg����ں���Ϊһ��������е���
*     WinXP��boot.ini �ļ��п���������һ�������� /debug /debugport=com1 /baudrate=115200
*     Vista(����ԱȨ��) -- copyһ���µ����ú�ͨ��GUI���÷�ʽ����(msconfig -> Boot -> Advanced options)��
*       bcdedit /enum OSLOADER <== ��ö�ٳ����������е�"����ϵͳ������"
*       bcdedit /copy {current} /d "Windows Debug Model"  <== ������һ������ģʽ��������
*       TODO: ���øտ���������������Ϊ��ǰ������������ĵ���ǰ����������(���߿�����һ�� Original ?)
*         bcdedit /displayorder {Debug�������GUID} {current} -- ����һ���µ�ѡ������˵� ��
*       bcdedit /debug {Debug�������GUID} ON  <== �������
*       bcdedit /bootdebug {Debug�������GUID}   
*       bcdedit /dbgsettings <== �鿴�����õ�ǰ�ĵ������ã��粻�Ǵ��ڣ���Ҫ�����������
*         bcdedit /dbgsettings serial baudrate:115200 debugport:1
*       bcdedit /set loadoptions DDISABLE_INTEGRITY_CHECKS  <== �ر� Win7 ϵͳ�е�����ǩ��ǿ��Ҫ��
*          64ϵͳ�ƺ���Ч����Ҫͨ�� bcdedit /set nointegritychecks ON �� �� Driver Signature �����ƺ����ԣ�
*          ���¿���ǩ��Ҫ��bcdedit /set nointegritychecks off  
*         bcdedit /set testsigning ON <== ������ز���ǩ��ǩ������������(����Ҫǩ�������������÷�΢����֤��֤��)
*       ����ʱѡ�� F8�� Ȼ��ѡ�񡰽�ֹǿ������ǩ���� �����Բ���ǩ�����в���
*     
* 
*   ����WinDbg�ĳ�������μ� FtlDebug.h �ļ�
*
* ETHREAD
* PKTHREAD
* EPROCESS
******************************************************************************************************************/


/******************************************************************************************************************
* !analyze -v  -- ���� dmp �ļ��Ժ�ʹ�ø������Զ�������Ȼ�󼴿�ͨ�� kb ����� "CallStack ����(Alt+6)" �鿴��ջ
* 
* ������������(BSOD -- Blue Screen Of Death)�Ϳ��ܵ�ԭ��
*   IRQL_NOT_LESS_OR_EQUAL(0x0000000A) -- ����û��Ȩ�޵��ڴ� 
*   SYSTEM_THREAD_EXCEPTION_NOT_HANDLED(0x0000007E) -- 
*   DRIVER_IRQL_NOT_LESS_OR_EQUAL(0x000000D1) -- 
*   PAGE_FAULT_IN_NONPAGED_AREA(0x00000050) -- �Ƿ�ҳ�ڴ��з�����ȱҳ����
*     1.IRQL ���ܲ��������󣬿���ͨ�� KeGetCurrentIrql ��ȡ��ǰ��IRQL�� Ҳ����ͨ�� PAGED_CODE ���æ���;
*     2.����ж������ʱ IoDeleteSymbolicLink ������������Ϊ DriverEntry ������Ϊ #pragma code_seg("INIT") ���
*
* ��������
*   1.�û�̬ CreateFile �򿪷���ʱ���� 1(Incorrect function), �������Ϊ����û��ʵ�� IRP_MJ_CREATE/IRP_MJ_CLOSE �ȷַ�����
******************************************************************************************************************/

#endif //F_DRIVER_DEBUG_H