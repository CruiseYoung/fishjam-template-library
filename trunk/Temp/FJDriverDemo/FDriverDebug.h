#ifndef F_DRIVER_DEBUG_H
#define F_DRIVER_DEBUG_H

/******************************************************************************************************************
* SrvInstW.exe(http://www.onlinedown.net/soft/36059.htm)
*   ��������Ϊ����ķ�ʽ��װ��Ȼ�����ͨ�� net start/stop ������/ֹͣ ��������
*
* WinObj(http://technet.microsoft.com/en-us/sysinternals/bb896657.aspx)
*   ������ʾ���е��ں˶���
*
* ͨ��DebugView�鿴DbgPrint��ӡ����־��Ϣ����Ҫѡ�� Capture Kernel ���ܿ����ں����
*
* ���������
*   VMWare
*     ����Ŀ¼��Options -> Shared Foloders -> ���ӹ���Ŀ¼, �ù���Ŀ¼�������������еġ���������������
*     �ùܵ����⴮�ڣ�Hardware -> Add -> Serial Port -> Output to named pipe -> �޸ĵ�����ѡ��Ϊ"The other end is an application"
*       ���Ի�������windbg��windbg.exe -b -k com:port=\\.\pipe\com_1,baud=115200,pipe
*       ��������ʱ����������Windowsϵͳ�ᱻ�ж�(ò��ʱ��),��Ҫ��WinDbg��������ʾ����ִ�� "g" �����������������
*
*
******************************************************************************************************************/
/******************************************************************************************************************
* WinDbg(http://msdn.microsoft.com/en-us/windows/hardware/gg463009)
*   �����ں�ʱ������˫�����ԣ�ͨ���������������磿
*   ���ñ����Ի���Ϊ����ִ�з�ʽ��WinDbg����ں���Ϊһ��������е���
*     WinXP��boot.ini �ļ��п���������һ�������� /debug /debugport=com1 /baudrate=115200
*     Vista��
*       bcdedit /enum OSLOADER <== ��ö�ٳ����������е�"����ϵͳ������"
*       bcdedit /copy {current} /d "Windows Debug Model"  <== ������һ������ģʽ��������
*       TODO: ���øտ���������������Ϊ��ǰ������������ĵ���ǰ����������(���߿�����һ�� Original ?)
*       bcdedit /debug ON  <== ����Ϊ����
*       bcdedit /bootdebug ON 
*       bcdedit /dbgsetting  <== �鿴��ǰ�ĵ�������
*
*   ��������(���ж�״̬�£�ͨ��F9����������/ȡ���ϵ�)
*     g -- �жϵĳ����������
******************************************************************************************************************/



#endif //F_DRIVER_DEBUG_H