#ifndef F_DRIVER_STUDY_H
#define F_DRIVER_STUDY_H

// http://www.ltr-data.se/opencode.html -- ��Դ��һЩ����(�� imdisk)
// http://bazislib.sysprogs.org/ -- BazisLib library�������кܶ��򵼺ͱ�׼�����ĸ���������

//��������-Windows�ں˰�ȫ��� -- P14X
//Windows��������������� -- P330(����IRP)  (���ȿ����Լ���?)

//���û�ģʽͨ��( ReadFile/DeviceIoControl/CancelIo ��)
//  ��������ͨ�� HAL ����Ӳ������㣬Ȼ��ź�Ӳ��������
//  �����������У�Ӧ��������ʹ�����ƽ̨�Ļ��ָ��(�� IN, OUT)����Ӧ��ʹ����ƽ̨�޹ص�HAL������꣨�� READ_PORT_BUFFER_UCHAR ��
/******************************************************************************************************************
* �ں�ģ��ĳ�����;��������������������ʵʱ���(�ļ�Filter)������ǽ��
*   ��ʹ���������ܻᵼ����Ӳ��������(����QQ��¼ʱ���������������� ����̭)
*
* 
* �����Ӧ�ó��� + ������ں˿ռ�
*   �ں�ģ��λ���ں˿ռ�(�����н��̹���)���������ִ��һ��λ��ĳ�����̿ռ���(��ͨ�� PsGetCurrentProcessId ��ȡ��ǰ���̵Ľ��̺� )
* 
* CPUһ����ж����Ȩ��(Intel��386��4����Ȩ��, Ring0~Ring3)
*   �ں�ģʽ(Kernel mode) -- ������Ring0�㣬��ִ���������(��ֱ�ӷ��������ڴ桢����˿ڵ�)
*   �û�ģʽ(User mode) -- ������Ring3�ɣ�ֻ��ִ�����ȵĴ���
*
* �ں˿ռ� -- �ܵ�Ӳ��������x66�ܹ��� Ring 0 ��Ĵ���ſ��Է��ʡ���Ϊ paged pool �� non-paged pool���������������Ҫ��������
* �û��ռ� -- ��ͨӦ�ó��������� Ring 3 �㣬ͨ������ϵͳ�ṩ��һ�����(������е���sysenter)������R0��Ĺ���
*
* ��һ�ں�(Linux) -- ϵͳ����Ҫ�������ȫ�����ں�ʵ�֣�����������Դ󣬲�����ģ�黯��ơ����ٶȿ졣
* ΢�ں�(Windows) -- ϵͳ����Ҫ������������ڶ����Ľ����У����Լ�˽�еĵ�ַ�ռ䣬ʹ�÷���ԭ��ͨ�����̼�ͨ�Ž�����
*
* ÿ������������һ�����Ʒ������ʽ���ڣ���ϵͳע���� HKLM\SYSTEM\CurrentControlSet\Services �º���������ͬ������
* 
******************************************************************************************************************/

/******************************************************************************************************************
* APC -- Asynchronous Procedure Call, �첽���̵���(�̴߳��� Alert ״̬ʱ�ص��ſ��ܱ�����)
* APIC -- Advanced Programmable Interrupt Controller, �߼���̿�����
* CDO -- �����豸(Control Device Object)���޸������������ڲ����ã�һ��Driverֻ��Ӧһ��CDO��
* DCT -- Display Compatibility Kit
* DDK -- Driver Development Kit��WinNT��WinXP��ƽ̨�Ŀ�����
* DIRQL -- 
* DMA -- 
* DSF -- Device Simulation Framework
* DPC -- Delayed Procedure Call���ӳٹ��̵��ã����һ��IO����Ϳ�ʼ��һ���ж������Ĵ��䣬һ���ISR���ʹ�÷�ֹ����������е��߳�
* FDE -- Full Disk Encryption,ȫ���̼���
* FDO -- Function Device Object �������豸����һ������Լ�������ͨ�� IoCreateDevice �������豸����?
* FSD -- File System Driver, �ļ�ϵͳ����
* GDT -- Global Descriptor Table, ȫ����������
* GPF -- General Protection Fault, һ�㱣���Դ���
* HAL -- Ӳ�������
* HCT -- Hardware Compatibility Test(Ӳ�������Բ��Թ���)
* IDT -- Interrupt Descriptor Table, �ж���������
* IFS -- Installable File Systems 
* IRP -- I/O Request Package����������������������������У�����IRP������ͨ�������ܺ�(Major)�ʹι��ܺű�ʶһ��IRP�Ĺ��ܡ�
* ISR -- �жϷ������
* KMDF (Kernel Mode Driver Framework): A framework for developing kernel mode drivers. 
* MDL -- Memory Descriptor List(�ڴ���������)
* NDIS -- Network Driver Interface Specification
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
* Windows �����Ŀ���ʵ�ַ�ʽ(���˼���ǽ��ں���Ƶľ�����С��������"�ͻ���"-"������"�Ľṹ�����������ģ��ͨ����Ϣ����ͨ��)
*   1.����ֲ�� -- ����ֲ㣬ֻ���������HAL(Ӳ�������)��Ӳ�����ܹ���
*   2.������ -- һ�µ�API�ӿڣ�������ϵͳ(����Ե�Ӧ�ó����ṩ��Ӧ��API֧��)֧�ּ���ģʽ
*   3.��׳�ԺͿɿ��� -- �û�ģʽ���ں�ģʽ�Ļ��֣������κ��漰����Ӳ���Ĳ�����Ӧ�ó��򶼱���ͨ�����ں�ģʽ�е�ϵͳ��������ɡ�
*   4.����չ�� -- �ں˴�ִ��������з��������ֻ��������̵߳ĵ��ȹ���������ִ�г������(�ڴ�������̹���)��Ϊ�������ں�
*       �����������ģ�黯�ķ�����ƣ�����Ҫ�Ľ���ʱ���������������ִ�г��������
*   5.���� -- �첽IO�������/���̲߳��У�
*
* Windows����Ƴ�Ϊ����ֲ����ϵ�ṹ:
*   Ӧ�ó���
*   ��ϵͳ -- SubSystem: Win32, native, OS/2, POSIX, WOW(Windows On Windows), 
*             VDW((Virtual DOS Machine) ��
*   Native API																				�û�ģʽ
*   ----------------------------------------------------------------------------------------------------
*   ϵͳ������																			�ں�ģʽ
*   ִ�������(IO������,���������,���̹�����,�����ڴ������,���ù�����,���������) + ��������
*   �ں�(��Ҫ������̡��̵߳ĵ��ȣ���CPUͬ��֧�֣��ṩ�жϴ����)
*   HAL -- Ӳ�������         
*   Ӳ���� -- 
* 
* ������ϵͳ -- ����Ե�Ӧ�ó����ṩ��Ӧ��API֧�֣����� WinXp ������Dos�ȳ���
* Native API �������û�ģʽ���ں�ģʽ�Ľ��棬�ﵽ���ں�ģʽ
* ϵͳ������ͨ��IO����������Ϣ���ݸ���������
*
* �ֲ�����(WDM������������ڸû����Ϸ�չ����)
*   ��һ����������ֽ�����ɸ�С��������ÿ�����һ���򵥵����������ڽ���������ֽ�ɶ���򵥲���
******************************************************************************************************************/

#endif //F_DRIVER_STUDY_H