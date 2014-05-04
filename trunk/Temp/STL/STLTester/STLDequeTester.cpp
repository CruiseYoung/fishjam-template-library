#include "StdAfx.h"
#include "STLDequeTester.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

struct BigMemoryStruct
{
    DWORD buf[1000];
};

void CSTLDequeTester::test_deque_memory()
{
    std::deque<BigMemoryStruct*>   dequeAddress;    //����ÿһ��Ԫ�صĵ�ַ
    std::deque<BigMemoryStruct>    dequeBigMemorys;

    dequeBigMemorys.push_back(BigMemoryStruct());
    BigMemoryStruct* pFirstBigMemoryStruct = &dequeBigMemorys[0];
    dequeAddress.push_back(pFirstBigMemoryStruct);

    DWORD dwCount = 1;
    
    while (&dequeBigMemorys[0] == pFirstBigMemoryStruct && dwCount < 10)
    {
        dequeBigMemorys.push_back(BigMemoryStruct());

        DWORD dwOffset = ((BYTE*)(&dequeBigMemorys[dwCount]) - (BYTE*)(&dequeBigMemorys[dwCount-1]));
        TRACE(TEXT("test_deque_memory,dwCount = %d, dwOffset = %d, &dequeBigMemorys[%d] = %p\n"),
            dwCount,dwOffset, dwCount,&dequeBigMemorys[dwCount]);
        dequeAddress.push_back(&dequeBigMemorys[dwCount]);
        dwCount++;
    }
    CPPUNIT_ASSERT(dwCount == 10);//deque ����������ṹ����������ڴ濽�������⡣
    for (DWORD dwIndex = 0; dwIndex < dwCount - 1; dwIndex++)
    {
        //��û��ɾ��������£�ÿһ��Ԫ�ض��ڳ�ʼ����ʱ�ĵ�ַ
        CPPUNIT_ASSERT(&dequeBigMemorys[dwIndex] == dequeAddress[dwIndex]);
    }

    //ɾ����β��Ԫ�أ�����Ԫ�ص�λ�ò�����е���
    dequeBigMemorys.erase(dequeBigMemorys.begin());     //ɾ����һ��
    dequeBigMemorys.erase(dequeBigMemorys.end() - 1);   //ɾ�����һ��
    
    for (DWORD dwIndex = 0; dwIndex < 10 -2 ; dwIndex++)
    {
        CPPUNIT_ASSERT(&dequeBigMemorys[dwIndex] == dequeAddress[dwIndex + 1]);
    }


    //���ɾ���м��Ԫ�أ��Ὣ��ͷ��ɾ��λ��ǰһ����Ԫ����󿽱������ͷ���Ԫ�ص��ڴ�
    dequeBigMemorys.erase(dequeBigMemorys.begin() + 3);     //ɾ��������
    for (DWORD dwIndex = 0; dwIndex < 10 - 3; dwIndex++)
    {
        CPPUNIT_ASSERT(&dequeBigMemorys[dwIndex] == dequeAddress[dwIndex + 2]);
    }

}