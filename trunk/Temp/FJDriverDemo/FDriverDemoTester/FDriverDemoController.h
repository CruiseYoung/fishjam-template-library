#pragma once

#include <WinSvc.h>

//NT ʽ����������ͨ������ʽ����
//WDM ʽ��������Ҫͨ�� INF �ļ��ķ�ʽ������(Pnp���ҵ�Ӳ���豸�� VenderID �� ProductID,����ϳ�ע����е� HardwareID)

class CFDriverDemoController
{
public:
    CFDriverDemoController(void);
    ~CFDriverDemoController(void);

    BOOL InstallService(LPCTSTR pszDriverFilePath, LPCTSTR pszServiceName);
    BOOL UnInstallService();

	BOOL OpenDemoDriver(LPCTSTR pszDosDriverName);
	BOOL CloseDemoDriver();

	BOOL IoControl(DWORD dwIoControlCode, LPVOID lpInBuffer, DWORD nInBufferSize,
		LPVOID lpOutBuffer, DWORD nOutBufferSize);
    //BOOL StartService();
    //BOOL StopService();

private:
    SC_HANDLE m_hSCManager;
    SC_HANDLE m_hDriverDemoService;

	HANDLE	  m_hDemoDriverFile;
};
