#pragma once

#include <WinSvc.h>

//NT ʽ����������ͨ������ʽ����
//WDM ʽ��������Ҫͨ�� INF �ļ��ķ�ʽ������(Pnp���ҵ�Ӳ���豸�� VenderID �� ProductID,����ϳ�ע����е� HardwareID)

class CFDriverDemoController
{
public:
    CFDriverDemoController(void);
    ~CFDriverDemoController(void);

    BOOL InstallService();
    BOOL UnInstallService();

    //BOOL StartService();
    //BOOL StopService();

private:
    SC_HANDLE m_hSCManager;
    SC_HANDLE m_hDriverDemoService;

};
