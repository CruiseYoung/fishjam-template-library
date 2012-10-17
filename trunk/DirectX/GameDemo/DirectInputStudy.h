#pragma once

/*********************************************************************************************************************
* DirectInput ��һ��������Ӳ������������ϵͳ������Ӳ��������������ͳһ�ӿ���Ӧ�õĴ�ͳ�ĺͷǴ�ͳ�������豸��
*   �����豸(��ꡢ���̡������ҡ���Ϸ����)�����豸(Device)
*
* �ӿ�(������ A �� W �����汾)
*   IDirectInput -- ���ӿڣ��������ȡ����õ��κ������豸��ͨ�� DirectInputCreate �ȷ�������
*   IDirectInputDevice(0,2,7,8) -- �豸��ͨ�Źܵ�
*
* ���ݲɼ�ģʽ
*   1.��ʱ����ģʽ(Ĭ��)
*   2.��������ģʽ
*
* ��ȡ���������ַ�ʽ
*   1.������ -- ͨ�� IDirectInputDevice2::Poll ��ѯ�豸(����������ʹ���жϣ����������µ�)
*     if(SUCCESSED(pDIJoyStick->Poll()){...}
*   2 ������� -- ͨ�� GetDeviceState ���豸��ȡ����
*
* ������ݵĻ�ȡ -- ͨ�� SetProperty ��������
*   1.����ģʽ -- �����������Ļ�����λ��
*   2.���ģʽ -- ���������ÿ��ʱ��(����һ�Σ�)�����ֵ
*
* �ϲ����������豸�����룬ͳһ������������ơ����̵��ҷ�����������������ƣ�
*   ���������豸(��ꡢ���̡�������)������ӳ�䵽ͳһ�Ľṹ��
*   typedef struct INPUT_EVENT_TYPE
*   {
*     int dx;         //X���ƶ���
*     int dy;         //Y���ƶ���
*     int fire;       //����
*     int special;
*     //...
*   };
*
* �������豸
*   IDirectInputDevice2::CreateEffect  
*********************************************************************************************************************/
 
#include <dinput.h>
#pragma comment(lib, "dinput.lib")
#include <ftlDirectx.h>

inline HRESULT DirectInputDemo(HINSTANCE hins, HWND hWnd)
{
    HRESULT hr = E_FAIL;
    IDirectInput*   pDInput = NULL;
    IDirectInputDevice* pDIKeyboard = NULL;

    //1.ͨ�� DirectInputCreate ���������� IDirectInput �ӿ�;
    DX_VERIFY(DirectInputCreate(hins, DIRECTINPUT_VERSION, &pDInput, NULL));

    //2.(��ѡ)ö�ٲ�ѯ���õ��豸GUIDS����Ҫ��Ϊ�˲��Ҳ����˵ȷǱ�׼�豸,�ص��������� DIENUM_CONTINUE ��ʾҪ����ö��
    //pDInput->EnumDevices(DIDEVTYPE_JOYSTICK, FUN_PTR, FUN_PARAM, DIEDFL_ATTACHEDONLY) //DI8DEVCLASS_ALL

    //3.ͨ����֪��GUID����ͨ�õ����(GUID_SysMouse)�ͼ���(GUID_SysKeyboard)
    //  ע�⣺�����ˡ��ֱ�������Ҫ QI IDirectInputDevice2
    DX_VERIFY(pDInput->CreateDevice(GUID_SysKeyboard, &pDIKeyboard, NULL));
    

    //4.�����豸��Э���ȼ� -- ע�⣺��ꡢ���̶��Ƕ�ռ�豸����ý���ĳ������ʹ�ã��������豸Ӧ�����ڶ�ռģʽ��
    //  DISCL_EXCLUSIVE -- ��ռ���ʣ�һ��Ҫ����豸������Ӧ�ó��򶼲��ܷ��ʣ�������Ӧ�ó����ܹ�Ҫ��Ƕ�ռ�ط��ʣ�ʲô��˼����
    //  DISCL_NONEXCLUSIVE -- �Ƕ�ռ����
    //  DISCL_BACKGROUND -- Ӧ�ó���ʹ�� �Ǽ����С�� ״̬����Ȼ�ܽ�������
    //  DISCL_FOREGROUND -- Ҫ��ǰ̨���ʣ�����ش����Ƶ���̨ʱ�����豸�Զ������(�ͷţ���
    DX_VERIFY(pDIKeyboard->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE))

    //5.�����豸�����ݰ���ʽ -- �������豸����ĵȼ��¾����������豸������������θ�ʽ��
    //  �����Լ����ã�Ҳ����ʹ��ϵͳԤ�ȶ���õ��豸���ݸ�ʽ
    //    DIDATAFORMAT df = {0};
    //    df.dwSize = sizeof(DIDATAFORMAT);
    //    df.dwFlags = ...
    DX_VERIFY(pDIKeyboard->SetDataFormat(&c_dfDIKeyboard)); //ͨ�ü���

    //6.�����豸�����ܣ����������ķ�Χ��ä����(�����ˡ��ֱ�ʱ��Ҫ), SetProperty
#if 0
    IDirectInputDevice2* pDIJoyStick = NULL;

    //�����ֱ������˵ķ�Χ�� [-1024 ~ 1024]
    DIPROPRANGE joy_axis_rang;
    joy_axis_rang.diph.dwSize = sizeof(joy_axis_rang);
    joy_axis_rang.diph.dwHeaderSize = sizeof(DIPROPHEADER);
    joy_axis_rang.diph.dwObj = DIJOFS_X;
    joy_axis_rang.diph.dwHow = DIPH_BYOFFSET;
    joy_axis_rang.lMin = -1024;
    joy_axis_rang.lMax = 1024;
    
    DX_VERIFY(pDIJoyStick->SetProperty(DIPROP_RANGE, &joy_axis_rang.diph));
    joy_axis_rang.diph.dwObj = DIJOFS_Y;
    DX_VERIFY(pDIJoyStick->SetProperty(DIPROP_RANGE, &joy_axis_rang.diph));

    //�����ֱ�X��Y��ä�� -- ��������ķ�Χ����������ʱ���������ݣ�ע�⣺ä����ֵ�� 0��10000 ֮��
    //��ˣ����Ҫ���� %10��ä����Ӧ������Ϊ 1000
    DIPROPDWORD dead_band;
    dead_band.diph.dwSize = sizeof(dead_band);
    dead_band.diph.dwHeaderSize = sizeof(DIPROPHEADER);
    dead_band.diph.dwObj = DIJOFS_X;
    dead_band.diph.dwHow = DIPH_BYOFFSET;
    //X���ä��Ϊ10%
    dead_band.dwData = 1000;
    DX_VERIFY(pDIJoyStick->SetProperty(DIPROP_DEADZONE, &dead_band.diph));

    //Y���ä��
    dead_band.diph.dwObj = DIJOFS_Y;
    DX_VERIFY(pDIJoyStick->SetProperty(DIPROP_DEADZONE, &dead_band.diph));
    
#endif

    //7.ͨ�� Acquire ����豸�������豸ͬӦ�ó�������������DirectInput������Ҫ����Щ�豸�л�ȡ���ݡ�
    DX_VERIFY(pDIKeyboard->Acquire());

    //8.��ȡ����--���ַ�ʽ
    //8.1 ������ʱͨ�� IDirectInputDevice2::Poll ��ѯ�豸(ȷ��û���ж��������򣬲����˾�����Ч����)����ȡ����״̬
    //    DX_VERIFY(pDIJoyStick->Poll());

    //8.2 �������ͨ�� GetDeviceState ���豸��ȡ����
    
    //��ȡ������Ϣ--ÿ��ѭ��֮ǰ�����û�ȡһ��
    //typedef UCHAR[256] _DIKEYSTATE ;
    UCHAR keyState[256] = {0};
    //����Ԥ��������ݸ�ʽ�� DIMOUSESTATE��DIJOYSTATE ��
    DX_VERIFY(pDIKeyboard->GetDeviceState(sizeof(keyState), (LPVOID)keyState));
DIJOYSTATE

    //���԰���
    if(DIKEYDOWN(keyState,DIK_ESCAPE))
    {
        //��������� Esc ��
    }
    
    //ʹ����Ϻ��ͷ�
    DX_VERIFY(pDIKeyboard->Unacquire());

    SAFE_RELEASE(pDIKeyboard);
    SAFE_RELEASE(pDInput);
}