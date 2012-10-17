#ifndef DXUT_STUDY_H
#define DXUT_STUDY_H
#pragma once

/*****************************************************************************************************
* DXUT -- һ����SDK�ṩ��D3DӦ�ó����ܣ�ͨ���ص����Ƶ������û�ʵ�ֵĻص�����
*   ͨ�� DXUT ����D3D�����ܣ�DirectX Sample Browser -> ѡ��"EmptyProject"��"Installl Project"
*     ʵ���Ͼ��ǽ� Sample �е� EmptyProject �ĸ����ֿ������µ�·����(������ncb�����⣬���ɿ��ʱ��Ҫ�򿪸ù���)
*
*   DXUT���֧��Direct3D�豸�ʹ���һһ��Ӧ��һ���豸ֻ�ܶ�Ӧһ�����ڣ�����֧��ͬʱʹ�ö���豸
*     ����ʾ���Direct3D���ڵĸ߼�Ӧ�ó���
*
* ����ʱ���� DXUTSetCallbackXXXXX �Ⱥ������ûص�������ָ��
*   ������
*     IsD3D9DeviceAcceptable -- ����Ⱦ�豸���������м�飬�鿴�豸�Ƿ�֧����Ҫ�Ĺ���
*     ModifyDeviceSettings   -- �޸���Ⱦ�豸������
*     OnD3D9CreateDevice()   -- DXUT��ܴ�����Direct3D�豸֮����ã��ɴ��������ڴ������Ϊ D3DPOOL_MANAGED ��
*       D3DPOOL_SYSTEMMEM����Դ����Щ��Դ����Ҫ����Ա���й���
*     OnD3D9ResetDevice      -- ���������ڴ������Ϊ D3DPOOL_DEFAULT ����Դ�����豸�Ӷ�ʧ״̬�ָ�ʱҲ�����
*  
*   ���У�
*     MsgProc -- ���������Ϣ�����ڿ���ʱ�������� OnFrameMove �� OnFrameRender �����������г�����Ⱦ
*       OnFrameMove   -- ͨ����дΪ�û��������Ӧ
*       OnFrameRender -- ������Ⱦ�Ĵ���
*       OnD3D9LostDevice  -- ��Ӧ�ó��������ڼ䣬��Direct3D�豸��Ϊ��ʧ״̬ʱ(��Alt+Tab���뿪һ��ȫ��ģʽ��Ӧ�ó���)���ã�
*         �ͷ� OnD3D9ResetDevice �д������豸��Դ
*       OnD3D9ResetDevice -- ���豸�Ӷ�ʧ״̬�ָ�ʱ����
* 
*   �˳���
*     OnD3D9LostDevice 
*     OnD3D9DestroyDevice  -- �ͷ��ɺ��� OnD3D9CreateDevice ��������Դ
*
* ȫ�ֺ���
*   DXUTInit         -- ���������У����ҽ��г�ʼ��
*   DXUTCreateWindow -- �������ڣ���ͨ�� DXUTSetCallbackMsgProc ���ô�����Ϣ�ص������������Ĵ��ھ������DXUTGetHWND������ȡ
*   DXUTCreateDevice -- ����D3D�豸������ IDirect3D9::CreateDevice �Ĵ󲿷ֲ�������չ DXUTCreateDeviceFromSettings��
*                       ��ͨ�� pCallbackIsDeviceAcceptable �ص������ܾ��κ�Ӳ����֧�ֻ���Ҫ�����
*   DXUTGetD3DObject -- ��ȡ IDirect3D9 ָ�����
*   DXUTSetWindow    -- ����Ӧ�ó����д����Ĵ��ڣ�ע����Ҫͨ�� DXUTStaticWndProc ������������Ϣ���ݸ���ܡ�
*   DXUTMainLoop     -- ������Ϣѭ��ֱ�������˳�����������Ϣ�����º���Ⱦ�����������豸�¼�
*
*   DXUTSetCursorSettings -- Ϊȫ��ģʽ�¹����÷�����ѡ��
*   DXUTSetMultimonSettings -- Ϊ�������� ����ʾ��(multiple monitors) �����й�������ѡ��
*   DXUTToggleFullscreen -- ʹӦ�ó����ڴ���ģʽ��ȫ��ģʽ���л�
*   DXUTToggleREF -- ʹӦ�ó�����Ӳ�������Ͳο��豸���л�
*
*   DXUTFindValidDeviceSettings -- ʹ�ø߶����ķ�����ö�ټ�����ѡ����õ��豸
*     1.D3DDEVTYPE_HAL(ʹ��Ӳ������) 
*     2.ȫ��ģʽʱѡ�������ʽ����Ӧ����������ʽ(���ں�ȫ���л�ʱ��)
*     3.��������ʽ����Ӧ�ĺ�̨��������ʽ       
* 
*   DXUTGetFPS -- ��ȡ��ǰÿ���ύ��֡��
*   DXUTGetFrameStats -- ��ȡһ��ָ���ַ�����ָ�룬���ַ�������ÿ��֡�����ֱ��ʡ���̨��������ʽ����Ȼ�������ʽ
*   DXUTGetDeviceStats -- ��ȡһ��ָ���ַ�����ָ�룬���ַ���������ǰ�豸���͡�����������Ϊ���豸��
*   DXUTSetConstantFrameTime -- ���û���ù̶�֡ʱ��(Ĭ����ÿ��30֡��fTimePerFrameΪ0.0333f)
*   DXUTSetTimer -- ���һ���µļ�ʱ��
*   DXUTPause -- ����ܵ��ڲ��������ͣ�����Ⱦ������Ϊ��ͣ״̬
*   DXUTIsKeyDown -- ��鵱�ú�������ʱ��������ָ����ĳ����( VK_F1, VK_LSHIFT, VK_RCONTROL, VK_RMENU ��)�Ƿ���
*   DXUTIsMouseButtonDown -- ��鵱�ú�������ʱ��ָ��������(VK_LBUTTON, VK_RBUTTON, VK_MBUTTON, VK_XBUTTON1 ��)�Ƿ���
*
* 
*   �߷ֱ��ʼ�ʱ��
*   Ϊ�Զ������ṩ������֧��
*****************************************************************************************************/

/*****************************************************************************************************
* �豸�¼�
*    �ı�(DXUTSetCallbackDeviceChanging) -- ���ûص��������޸�Direct3D�豸�Ĵ������ã�Ӧ�ó�����Է���FALSE���ܾ��ı���豸��
*    ����(DXUTSetCallbackDeviceCreated) -- ��Ӧ�ó����ʼ�������´����豸ʱ����Direct3D�豸����֮����������
*    ����(DXUTSetCallbackDeviceReset) -- ��Direct3D�豸��ʧ�ֱ��������ú���������
*    ��ʧ(DXUTSetCallbackDeviceLost) -- �� Alt+Tab���뿪һ��ȫ��ģʽ��Ӧ�ó��򣬻��߰���Ctrl+Alt+Del����
*      ������������һ��ȫ��3DӦ�ó���Ӧ�ó���Ӧ �ͷ����в������豸��ʧʱ���ڵ�Direct3D��Դ����
*      ���豸����ʱ��Ӧ�ó��������ú��� IDirect3DDevice9::Reset()�������´������в�����Reset()�����д��ڵĶ���
*    ����(DXUTSetCallbackDeviceDestroyed) -- ��Ӧ�ó�����ֹ�����´����豸ʱ��Direct3D�豸�����ٺ���������
* 
* ֡�¼�(��Ⱦ�����е�ÿһ֡������)
*   DXUTSetCallbackFrameMove -- ��ÿһ֡��ʼʱ����һ�Σ�ͨ�����ڴ���������(����任)
*   DXUTSetCallbackFrameRender -- ��ÿһ֡�����򴰿���Ҫ�ػ�ʱ���ã���ɶԳ�������Ⱦ���ú������غ�
*     ��ܽ����� Present ����ʾ����������һ������������
*
* ��Ϣ�¼�
*   DXUTSetCallbackKeyboard -- ��������¼�
*   DXUTSetCallbackMouse -- ��������¼�
*   DXUTSetCallbackMsgProc -- ��������Ϣ
*****************************************************************************************************/

/*****************************************************************************************************
* �ı����� -- �ṩ���� CDXUTTextHelper �����ı���ʾ
*   ��ʼ�� ID3DXSprite �� ID3DXFont ���� -- ������ OnCreateDevice ���� D3DXCreateFont ����
*   ��ʾ�ı�: DrawTextLine ��
*   �ͷ� ID3DXSprite �� ID3DXFont ����
*
* �ؼ� -- ͨ�����ڴ����м��ضԻ���Ȼ���ڶԻ����������Ӧ�Ŀؼ����ɶԻ���������ؼ�
*   CDXUTDialogResourceManager -- �Ի�����Դ�������࣬һ����Ҫ�ȴ���ȫ����Ϣ�Ը���GUI��Ȼ���������Ի������Ϣ������
*   CDXUTDialog -- 
*     AddButton
*     AddComboBox
*     AddCheckBox
*     AddEditBox
*     AddRadioButton
*     AddSlider
*   CD3DSettingsDlg -- DXUT��װ�õ�һ��ר������Direct3D��Ⱦ�豸���õĶԻ�����
*****************************************************************************************************/

#endif //DXUT_STUDY_H
