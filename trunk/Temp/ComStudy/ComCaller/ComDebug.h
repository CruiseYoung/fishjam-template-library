#pragma once

/************************************************************************
* ʹ�� IMallocSpy ���Լ����ڴ��ʹ��
* 
* �����
*   _ATL_DEBUG_QI <== ��DEBUG�������QueryInterface()�������
*   _ATL_DEBUG_INTERFACES <== ��DEBUG��������ӿ�ʹ������������ӿ�����������ע�⣺MSʵ���ƺ���Bug
* 
* ���ߣ�
*   DCOMCNFG.exe
*   RpcSs.exe -- Զ�̴������ʱʹ�ã�
************************************************************************/
class CComDebug
{
public:
    CComDebug(void);
    ~CComDebug(void);

    //����ע��ͷ�ע��ָ����COM���
    HRESULT RegisterCOMObject(LPCTSTR pszObjectPath, BOOL bRegister);
};
