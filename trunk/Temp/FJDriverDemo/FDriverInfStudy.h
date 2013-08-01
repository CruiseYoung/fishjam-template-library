#ifndef F_DRIVER_INF_STUDY_H
#define F_DRIVER_INF_STUDY_H
/******************************************************************************************************************
* INF -- Device INFormation File, ����Ӳ���������ļ���������Ϣ���ļ�
* 
* ����
*   1.������÷������������Ľ���ɣ������֮��û���Ⱥ�˳�������
*   2.ÿһ����������������Ŀ��ɵ�,ÿһ����Ŀ���������� signature="$CHICAGO$" �ļ�ֵ�ԣ����ж�ֵ���ö���","�ֿ�
*   3.�ֺ�(";")���������Ϊע��
*   
* �ṹ
*   [Defaultinstall]
*   [DestinationDirs] -- Ŀ�ĵ��ļ����ڵ�Ŀ��·��
*   [Manufacturer] -- ����INF�ļ�����ʶ�������Ӳ���豸,���а������豸����������,�Ա��豸����ȷ��װ
*   [SourceDisksNames.x86] -- ָ����װ�ļ����ڵĽ���, ������ϸ��Ϊ .x86, .amd64 ��
*   [SourceDisksFiles.amd64] -- ���������ļ��б�����װ��λ�ã��� SourceDisksNames ����(ͨ���䶨�������)����֪�������λ��
*   [String] -- �����˻�Ƶ����INF��ʹ�õ��ַ�������
*   [Version] -- ����֧�ֵ��豸���ͺ����õĲ���ϵͳ
*     signature => ʹ�õĲ���ϵͳ: "$CHICAGO$"(Win98���ϵͳ), "$Windows NT$"(WinK/Xp/2003��)
*     CLASS => �豸������: Display(��ʾ�豸), Media(��ý���豸), Net(�����豸), Modem(���ƽ����), Printer(��ӡ), 
*              Image(ͼ�񲶻�������ͷ)
*     ClassGuid =>
*
* WDM��ص�ע���
*   HKLM\SYSTEM\CurrentControlSet\Enum -- ����ʵ����Ϣ��
*   HKLM\SYSTEM\CurrentControlSet\Control\Class -- ���ӽ���ÿ���豸���������һ���豸��(����ϵͳԤ���ģ�Ҳ�����Զ����)
*   HKLM\SYSTEM\CurrentControlSet\services -- �����ӽ���������ǰ��NTʽ��������
*
* Ŀ¼����(���� DestinationDirs �ȵط�, ����μ� Using Dirids )
*   10 -- %SystemRoot%, �� C:\Windows
*   11 -- %SystemRoot%\system32 , �� C:\Windows\System32
*   12 -- %SystemRoot%\system32\drivers, ����Ŀ¼
*   17 -- INF file directory
*   30 -- C�̸�Ŀ¼
*   31 -- Windowsϵͳ�����̵ĸ�Ŀ¼
* 
* Pnpʽ����������(WDM ��)��Ҫ����INF�ļ����ӿ�������Ӳ���򵼰�װ(��ͨ�� EzDriverInstaller ��װ��) 
*   ������WDM ��������Ĳ���Ӳ���豸����Ϣ�����������һЩ��Ϣ
*
******************************************************************************************************************/




#endif //F_DRIVER_INF_STUDY_H