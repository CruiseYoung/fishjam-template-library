#ifndef DRIVER_STUDY_H
#define DRIVER_STUDY_H

//��������-Windows�ں˰�ȫ��� -- P
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
* �ں˿ռ� -- �ܵ�Ӳ��������x66�ܹ��� Ring 0 ��Ĵ���ſ��Է���
* �û��ռ� -- ��ͨӦ�ó��������� Ring 3 �㣬ͨ������ϵͳ�ṩ��һ�����(������е���sysenter)������R0��Ĺ���
*
******************************************************************************************************************/

/******************************************************************************************************************

******************************************************************************************************************/



/******************************************************************************************************************
* CDO -- �����豸(Control Device Object)���޸������������ڲ����ã�һ��Driverֻ��Ӧһ��CDO��
* DCT -- Display Compatibility Kit
* DDK -- Driver Development Kit��WinNT��WinXP��ƽ̨�Ŀ�����
* DSF -- Device Simulation Framework
* FDE -- Full Disk Encryption,ȫ���̼���
* FDO -- Function Device Object �������豸����
* HCT -- Hardware Compatibility Test(Ӳ�������Բ��Թ���)
* IFS -- Installable File Systems 
* IRP -- I/O Request Package����������������������������У�����IRP������ͨ�������ܺ�(Major)�ʹι��ܺű�ʶһ��IRP�Ĺ��ܡ�
* KMDF (Kernel Mode Driver Framework): A framework for developing kernel mode drivers. 
* MDL -- �ڴ���������
* PDO -- Physical Device Object�������豸����ͨ�����豸ջ��������Ǹ��豸����
* UMDF (User Mode Driver Framework): A framework for developing user mode drivers. 
* VPB -- Volume parameter block
* WDF  -- Windows Driver Foundation
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

#endif //DRIVER_STUDY_H