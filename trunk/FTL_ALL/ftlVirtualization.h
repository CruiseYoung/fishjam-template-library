#ifndef FTL_VIRTUALIZATION_H
#define FTL_VIRTUALIZATION_H

#pragma once

#ifndef FTL_BASE_H
#  error ftlVirtualization.h requires ftlbase.h to be included first
#endif

namespace FTL
{
	/*****************************************************************************************************
	* ���⻯����
	*   ��������
	*     VirtualBox -- ��Դ���(GPLЭ��)�����С��֧��Win/Linux,����Ҫ��VMvareһ����װ����������
	*     VMware Workstation -- ��ͬ��OS֮�仹�ܻ�������(�� �ļ������Լ�����ճ�� ��)
    *       ����������Ŀ¼: 
	*     Virtual PC -- 
	*
	* VirtualBox���������Ĵ��� -- http://www.cnblogs.com/cxun/archive/2013/03/12/2956175.html
	*****************************************************************************************************/
    class CFVirtualization
    {
    public:
        FTLINLINE CFVirtualization(void);
        FTLINLINE virtual ~CFVirtualization(void);
    };

}

#endif //FTL_VIRTUALIZATION_H

#ifndef USE_EXPORT
#  include "ftlVirtualization.hpp"
#endif