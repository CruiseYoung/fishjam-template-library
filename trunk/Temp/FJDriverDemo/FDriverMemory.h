#ifndef F_DRIVER_MEMORY_H
#define F_DRIVER_MEMORY_H

/******************************************************************************************************************
* Windows���������У����г���ĺ����ͱ�����Ҫָ�������ص� ��ҳ�ڴ� �л��� �Ƿ�ҳ�ڴ� ��
*   ��ҳ�ڴ�
*   �Ƿ�ҳ�ڴ�
*   INIT��־���ڴ� -- DriverEntry����ʹ�ã������ú���ֻ�ڼ��ص�ʱ����Ҫ�����ڴ棬����������ɹ����غ󣬸ú������Դ��ڴ���ж�ص�
*
******************************************************************************************************************/

#define PAGEDCODE   code_seg("PAGE")
#define LOCKEDCODE  code_seg()

//ָ���˺����Ǽ��ص�INIT�ڴ������У����ɹ����غ󣬿����˳��ڴ�(DriverEntry)
#define INITCODE    code_seg("INIT")            

#define PAGEDDATA   data_seg("PAGE")
#define LOCKEDDATA  data_seg()
#define INITDATA    data_seg("INIT")


#endif //F_DRIVER_MEMORY_H