#ifndef FTL_HARDWARE_H
#define FTL_HARDWARE_H

#pragma once

#ifndef FTL_BASE_H
#  error FtlHardware.h requires ftlbase.h to be included first
#endif

namespace FTL
{
//Class
    class CFDeviceNotification;  //ע��Device֪ͨ��Ϣ�󣬿���ͨ��WM_DEVICECHANGE�����豸��Ϣ

//Function

}//namespace FTL


namespace FTL
{
    /*****************************************************************************************************
    * �洢����Ҫ�ɣ��Ĵ��������ٻ���(Cache)���ڴ桢��� �Ȳ�����ɡ�
    * CPU������֮�佻�����ݵķ�ʽ:
    *   1.ֱ�ӳ�����ƣ������ַ�Ϊ ���������ͷ�ʽ �� �����ѯ��ʽ(���ܵͣ��޷�ʵʱ);
    *   2.�жϷ�ʽ������(�����ȼ�)�ж�ʱ�������ֳ����жϴ����ָ��ֳ����޷�������ٵ��������ݴ���Ҫ��
    *   3.DMA��ͨ��DMA������ʵ��������I/O�豸���ֱ�����ݴ��ͣ�����ҪCPU�ĸ�Ԥ��������Ĺ����ĳЩ���Ʋ�������CPU�е���
    *   4.ͨ�����Ʒ�ʽ��ר�ÿ�������ִ��ͨ���������I/O�����Ĺ���Ϊ������I/O�豸�ṩ���ݴ���ͨ���������� �����ͻ���
    *
    * Ѱַ��ʽ��
    *   1.����Ѱַ   -- �������Ͱ�����ָ���У��� ADD AX,100 �е� 100
    *   2.ֱ��Ѱַ   -- ������������ڴ浥Ԫ�У�ָ����ֱ�Ӹ������������ڴ洢��Ԫ�ĵ�ַ���� ADD AX,(100) �е� (100)
    *   3.�Ĵ���Ѱַ -- �����������ĳһ�Ĵ����У�ָ���и����Ĵ��������� ADD AX, 100 �е� AX
    *   4.�Ĵ������Ѱַ -- ������������ڴ浥Ԫ�У����������ڴ洢��Ԫ�ĵ�ַ���ڼĴ����С��� ADD AX,(BX) �е� (BX)
    *   5.���Ѱַ   -- ָ���и����˲������ĵ�ַ�ĵ�ַ����Ҫ�������η����ڴ�Ĳ������� ADD AX ((100))
    *   6.���Ѱַ   -- ָ���ַ�벿�ָ�������һ��ƫ����(�����ɸ�)����������ַ���ڱ���ָ��ĵ�ַ���ϸ�ƫ������
    *       �� ADD AX, *100���洢��Ԫ�ĵ�ַ = ����ָ�����ڴ洢��Ԫ��ַ + 100
    *   7.��ַѰַ   -- ��������ַ���ڱ�ַ�Ĵ��������ݼ�ƫ�������� ADD AX, (DI+100)��(DI+100)���ڵĴ洢��Ԫ�ǲ�����
    *
    * 90�����Intel�ȹ�˾��ʼ��X86ָ�����ɶ�С�̶�����RISCָ�ʹ�����ڵ�X86CPU��ʼ�����Ƚ���RISC���ġ��Ĵ���
    *   ������������ִ�еȼ�����������RISC��һЩ�ص㡣
    *****************************************************************************************************/

    /*****************************************************************************************************
    * ���弴�ã������豸���豸���Զ�Ѱ�Ҳ���װ���ʵ����������û�л���ʾ�û���װ��
    * �Ǽ��弴�ã������豸�󣬱����ֶ���װ�����������Զ���װ����ʾ��
    * 
    * //����HID�豸(Home Information Device)���͵�����
    * 1.ע�᣺
    * UINT nDevices;
    * //ȡ�����ӵ�ϵͳ��raw input�豸��Ϣ
    * ::GetRawInputDeviceList(NULL, &nDevices, sizeof(RAWINPUTDEVICELIST));
    * PRAWINPUTDEVICELIST pRawInputDeviceList = new RAWINPUTDEVICELIST[nDevices];
    * ::GetRawInputDeviceList(pRawInputDeviceList, &nDevices,sizeof(RAWINPUTDEVICELIST);
    * //���ζ��豸����ע��
    *  for(UINT i = 0;i < nDevices;i++)
    *  {
    *    RID_DEVICE_INFO stDeviceInfo = {sizeof(RID_DEVICE_INFO)};
    *    UINT nSize = sizeof(RID_DEVICE_INFO);
    *    ::GetRawInputDeviceInfo((pRawInputDeviceList + i)->hDevice,RIDI_DEVICEINFO,&stDeviceInfo,&nSize);
    *    switch(stDeviceInfo.dwType)
    *    {
    *      case RIM_TYPEHID:
    *        RAWINPUTDEVICE    stRawInputDevice;
    *        stRawInputDevice.usUsagePage = stDeviceInfo.hid.usUsagePage;
    *        stRawInputDevice.usUsage = stDeviceInfo.hid.usUsage;
    *        stRawInputDevice.dwFlags = RIDEV_INPUTSINK;
    *        stRawInputDevice.hwndTarget = m_hWnd;
    *        ::RegisterRawInputDevices(&stRawInputDevice,1,sizeof(RAWINPUTDEVICE));
    *         break;
    *      default:
    *        break;
    *    }
    *  }
    *  //�ͷ�RAWINPUTDEVICELIST�ڴ�
    *  delete []pRawInputDeviceList;
    * 2.���� WM_INPUT ��Ϣ -- ON_MESSAGE(WM_INPUT, OnWmInput)
    *   LRESULT CVcDlg::OnWmInput(WPARAM wParam, LPARAM lParam)
    *   {
    *     UINT uSize;
    *     //ȡ����������
    *     GetRawInputData((HRAWINPUT)lParam,RID_INPUT,NULL,&uSize,sizeof(RAWINPUTHEADER));
    *     char *szData = new char[uSize];
    *     GetRawInputData((HRAWINPUT)lParam,RID_INPUT,szData,&uSize,sizeof(RAWINPUTHEADER));
    *     PRAWINPUT pRawInput = (PRAWINPUT)szData;
    *     //���������ͽ����ж�
    *     if(pRawInput->header.dwType == RIM_TYPEHID){ ... }
    *     //�ͷ������ڴ�
    *     delete []szData;
    *     //����ϵͳ����Ĭ�ϴ���
    *     DefWindowProc(WM_INPUT,wParam,lParam);
    *     return 0L;
    *   }
    *****************************************************************************************************/
    class CFDeviceNotification
    {
    public:
        CFDeviceNotification();
        ~CFDeviceNotification();
        BOOL Register(HANDLE hRecipient,LPVOID NotificationFilter,DWORD Flags);
        BOOL UnRegister();
    private:
        HDEVNOTIFY m_hDevNotify;
    };
}

#endif //FTL_HARDWARE_H

#ifndef USE_EXPORT
#  include "ftlHardware.hpp"
#endif