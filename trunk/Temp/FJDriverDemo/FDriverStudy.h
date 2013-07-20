#ifndef F_DRIVER_STUDY_H
#define F_DRIVER_STUDY_H

//��������-Windows�ں˰�ȫ��� -- P14X
//Windows��������������� -- P   (���ȿ����Լ���?)

//���û�ģʽͨ��(DeviceIoControl?)
//  ��������ͨ�� HAL ����Ӳ������㣬Ȼ��ź�Ӳ������
/******************************************************************************************************************
* �ں�ģ��ĳ�����;��������������������ʵʱ���(�ļ�Filter)������ǽ��
*   ��ʹ���������ܻᵼ����Ӳ��������(����QQ��¼ʱ���������������� ����̭)
*
* 
* �����Ӧ�ó��� + ������ں˿ռ�
*   �ں�ģ��λ���ں˿ռ�(�����н��̹���)���������ִ��һ��λ��ĳ�����̿ռ���(��ͨ�� PsGetCurrentProcessId ��ȡ��ǰ���̵Ľ��̺� )
* 
* �ں�ģʽ(Kernel mode) -- 
* �û�ģʽ(User mode) -- 
*
* �ں˿ռ� -- �ܵ�Ӳ��������x66�ܹ��� Ring 0 ��Ĵ���ſ��Է��ʡ���Ϊ paged pool �� non-paged pool���������������Ҫ��������
* �û��ռ� -- ��ͨӦ�ó��������� Ring 3 �㣬ͨ������ϵͳ�ṩ��һ�����(������е���sysenter)������R0��Ĺ���
*
* ÿ������������һ�����Ʒ������ʽ���ڣ���ϵͳע���� HKLM\SYSTEM\CurrentControlSet\Services �º���������ͬ������
* 
******************************************************************************************************************/

/******************************************************************************************************************
* CDO -- �����豸(Control Device Object)���޸������������ڲ����ã�һ��Driverֻ��Ӧһ��CDO��
* DCT -- Display Compatibility Kit
* DDK -- Driver Development Kit��WinNT��WinXP��ƽ̨�Ŀ�����
* DMA -- 
* DSF -- Device Simulation Framework
* DPC -- Delayed Procedure Call���ӳٹ��̵��ã����һ��IO����Ϳ�ʼ��һ���ж������Ĵ���
* FDE -- Full Disk Encryption,ȫ���̼���
* FDO -- Function Device Object �������豸����һ������Լ�������ͨ�� IoCreateDevice �������豸����?
* FSD -- File System Driver, �ļ�ϵͳ����
* HAL -- 
* HCT -- Hardware Compatibility Test(Ӳ�������Բ��Թ���)
* IFS -- Installable File Systems 
* IRP -- I/O Request Package����������������������������У�����IRP������ͨ�������ܺ�(Major)�ʹι��ܺű�ʶһ��IRP�Ĺ��ܡ�
* ISR -- �жϷ������
* KMDF (Kernel Mode Driver Framework): A framework for developing kernel mode drivers. 
* MDL -- �ڴ���������
* PDO -- Physical Device Object�������豸����ͨ�����豸ջ��������Ǹ��豸����
* UMDF (User Mode Driver Framework): A framework for developing user mode drivers. 
* VPB -- Volume parameter block
* WDF  -- Wi����ndows Driver Foundation
* WDM -- Windows Driver Model
* WDK -- Windows Driver Kit��Vista��Ŀ�����������WinXP
* WDTF -- Windows Device Testing Framework
* 
* WDK = DDK (Driver Development Kit) 
*   + HCT Kit (Hardware Compatibility Test) 
*   + WDF (Windows Driver Foundation) 
*   + DTM (Driver Test Manager) 
*   + WDF (Driver Verification Tools)
*   + IFS Kit (Installable File Systems Kit) 
*   + Free ISO image download 
*   - Visual Studio 2005 out of the box integration
* 
* 
* ��ͬ���͵���������
*   �ļ�ϵͳ����(File System Driver)
*   �洢�豸����(Storage Driver)
*   ��������(Network Driver)
*   
*   ������ -- ͳ��һ���豸������������ ������URB���̻���PS/2���̾���������
******************************************************************************************************************/

/******************************************************************************************************************
* ϵͳ���̷���
*   csrss.exe -- 
*     win32!RawInputThread ͨ�� GUID_CLASS_KEYBOARD ��ü����豸ջ��PDO�ķ���������
*        (Ӧ�ó�����ֱ�Ӹ����豸�����豸��һ�㶼ͨ����������������)
******************************************************************************************************************/

#endif //F_DRIVER_STUDY_H