#ifndef F_DRIVER_HOOK_API_H
#define F_DRIVER_HOOK_API_H

/******************************************************************************************************************
* �������� HOOK API( ͨ�� SSDP ��Hook) -- ע���ȿ������ߵ��������ء�http://download.csdn.net/detail/jyw1111/5275135
*   ȱ�㣺���ܱ������������Ϊ rootkit ����
*
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

