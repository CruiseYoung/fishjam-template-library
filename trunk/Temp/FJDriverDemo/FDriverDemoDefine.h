#ifndef F_DRIVER_DEMO_DEFINE_H
#define F_DRIVER_DEMO_DEFINE_H

//ͨ�� CreateFile(FDRIVER_DEMO_DOS_NAME,..., OPEN_EXISTING ) ��ʽ������
#define FDRIVER_DEMO_DOS_NAME           TEXT("\\\\.\\Global\\FJDriverDemo")

#define FDRIVER_DEMO_FILENAME           TEXT("FJDriverDemo.sys")

#define FDRIVER_DEMO_SERVICE_NAME       TEXT("FJDriverDemo")

//�豸�������� \\Device\\�豸�� ����ʽ�� �� ���̷�����C�̣� \\Device\\HarddiskVolume1
//  �粻֧���豸��������ʱ I/O ���������Զ�����һ��������Ϊ�豸��
#define FDRIVER_DEMO_DEVICE_NAME        L"\\Device\\FJDriverDemoDevice"

//#define NT4_DRIVERNAME "\\\\.\\NTProcDrv"
//#define OTHER_DRIVERNAME "\\\\.\\Global\\NTProcDrv"
//#define PROCDRVPROCESSEVENT "NTProcDrvProcessEvent"


#endif //F_DRIVER_DEMO_DEFINE_H