#ifndef F_DRIVER_HOOK_API_H
#define F_DRIVER_HOOK_API_H

/******************************************************************************************************************
* �������� HOOK API( ͨ�� SSDP ��Hook) -- ע���ȿ������ߵ��������ء�http://download.csdn.net/detail/jyw1111/5275135
*   ȱ�㣺���ܱ������������Ϊ rootkit ����
*
*
* Native API��
*   ntdll.dll -- Native API �Ľӿڣ������ô����ںˣ�����Ring 3
*     1.�Ѷ�Ӧ�Ĺ��ܱ�Ŵ���eax�Ĵ���,
*     2.ʹ�� SYSENTER ָ��(����ʹ�ô��� Int 2e �жϵķ�ʽ)����"����"
*     3.KiSystemService �ں˴����������
*     4.��SDT���ҵ����ܱ�ŵĶ�Ӧ����λ�ã�������
*       SDT��ϵͳ��һ����4��()��
*       a.SSDT -- λ�� ntoskrnl.exe �� ntkrnlpa.exe �У���¼��ϵͳ�ؼ����ܵ�λ��,�漰�ļ�����,ע������,���̲����ȡ�TODO��Ϊʲô������������
*       b.Shadow SSDT -- λ�� win32k.sys �У� ��¼�˺��û���ͼ�ν����йصĹ��ܡ�
*       c.d -- Ԥ����SDT�������˹�����SDT�Ŀ��ܣ�����������û������
*     5.Object Hook -- ���ײ�Ľӿڣ������Ѿ����ѿ�����
*
* ntkrnlpa.exe (SSDT?) -- 
* win32k.sys (Shadow SSDT?) -- 
*
* NtGdiBitBlt
******************************************************************************************************************/

/******************************************************************************************************************
* SSDT(System Services Descriptor Table) -- ϵͳ��������������ring3��Win32 API��ring0���ں�API��ϵ������
*   ͨ���޸Ĵ˱�ĺ�����ַ���ԶԳ���windows������API����hook���Ӷ�ʵ�ֶ�һЩ���ĵ�ϵͳ�������й��ˡ���ص�Ŀ��
*   TODO: ��ô�õ�һ��������SSDT������
*   ϵͳ�ڲ��и���Ϊ KeServiceDescriptorTable �ı����������������Ϣ����ͨ�� extern �����
*     �� WinDbg ��: dd KeServiceDescriptorTable
******************************************************************************************************************/

#if 0
//SSDT��ṹ
typedef struct _ServiceDescriptorTable {
	PULONG ServiceTableBase;			//System Service Dispatch Table �Ļ���ַ  
	PULONG ServiceCounterTableBase; 
	//������ SSDT ��ÿ�����񱻵��ô����ļ����������������һ����sysenter ����
	ULONG NumberOfServices;				//�� ServiceTableBase �����ķ������Ŀ
	PUCHAR ParamTableBase;				//����ÿ��ϵͳ��������ֽ�����Ļ���ַ-ϵͳ���������  
}*PServiceDescriptorTable;
//��SSDT�����Ż�ȡ��ǰ������ַ
//NtOpenProcess  [[KeServiceDescriptorTable]+0x7A*4]
extern "C" PServiceDescriptorTable KeServiceDescriptorTable;

#endif //F_DRIVER_HOOK_API_H

