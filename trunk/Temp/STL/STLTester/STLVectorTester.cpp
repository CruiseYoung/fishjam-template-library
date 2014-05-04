#include "StdAfx.h"
#include "STLVectorTester.h"
#include "MyTestData.h"

#include <vector>
using std::vector;
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

void CSTLVectorTester::test_assign()
{
    //�������׼��������ʱ��Ӧ�õ�Ԫ�س�Ա���������ͬ��Ŀ�ĵ������Ա������Ҫ������ڴ���䣬��Ƶ���ؿ������󣬶���/������ɶ������
    //inserter,front_inserter
    //copy(v2.begin() + v2.size() / 2, v2.end(), back_inserter(v1)); //Ҳ������ copy ������

#ifdef NEED_OPERATOR_FUNC
    //ע�⣺vector�б���Ԫ��ʱ����Ҫ���� operator=����list����Ҫ
    vector<CMyTestData> myTestVectorSrc;
    vector<CMyTestData> myTestVectorTarget;

    for (int i = 0; i< 5; i++)
    {
        myTestVectorSrc.push_back(CMyTestData(i));
    }

    CPPUNIT_ASSERT(myTestVectorSrc.size() == 5);
    CPPUNIT_ASSERT(myTestVectorTarget.empty());

    myTestVectorTarget.assign(myTestVectorSrc.begin() + myTestVectorSrc.size() /2 , myTestVectorSrc.end());
    CPPUNIT_ASSERT(myTestVectorTarget.size() == 3);
    CPPUNIT_ASSERT(myTestVectorSrc.size() == 5);
#else
    vector<int> myTestVectorSrc;
    vector<int> myTestVectorTarget;
    for (int i = 0; i< 5; i++)
    {
        myTestVectorSrc.push_back(i);
    }

    CPPUNIT_ASSERT(myTestVectorSrc.size() == 5);
    CPPUNIT_ASSERT(myTestVectorTarget.empty());

    myTestVectorTarget.assign(myTestVectorSrc.begin() + myTestVectorSrc.size() /2 , myTestVectorSrc.end());
    CPPUNIT_ASSERT(myTestVectorTarget.size() == 3); 
    CPPUNIT_ASSERT(myTestVectorSrc.size() == 5); //����ԭ�еĲ��䣿
#endif
}

void CSTLVectorTester::test_reserve()
{
    vector<int> intVector;
    CPPUNIT_ASSERT( 0 == intVector.capacity());  //�ճ�ʼ��ʱʹ��Ĭ��ֵ -- VC��Ĭ��ֵ�� 0
    CPPUNIT_ASSERT( 0 == intVector.size());

    intVector.reserve(10);
    CPPUNIT_ASSERT( 10 == intVector.capacity());
    
    CPPUNIT_ASSERT( intVector.empty() );
    CPPUNIT_ASSERT( 0 == intVector.size() );

    for (int i = 0; i< 5; i++)
    {
        intVector.push_back(i);
    }

    CPPUNIT_ASSERT( 10 == intVector.capacity());
    CPPUNIT_ASSERT( 5 == intVector.size());

    for (int i = 5; i< 20; i++)
    {
        intVector.push_back(i);
    }
    CPPUNIT_ASSERT( intVector.capacity() >= 20);  //�����ڴ��VCʵ����Ĭ�Ϸ���(���Գ��� 22)
    CPPUNIT_ASSERT( 20 == intVector.size());

}

struct BigMemoryStruct
{
    DWORD buf[1000];
};
void CSTLVectorTester::test_vector_memory()
{
    std::vector<BigMemoryStruct*>   vectAddress;    //����ÿһ��Ԫ�صĵ�ַ
    std::vector<BigMemoryStruct>    vectBigMemorys;

    //����4���Ŀռ�
    vectBigMemorys.reserve(4);
    vectBigMemorys.push_back(BigMemoryStruct());
    BigMemoryStruct* pFirstBigMemoryStruct = &vectBigMemorys[0];
    vectAddress.push_back(pFirstBigMemoryStruct);

    DWORD dwCount = 1;
    while (&vectBigMemorys[0] == pFirstBigMemoryStruct && dwCount < 10)
    {
        vectBigMemorys.push_back(BigMemoryStruct());

        //��Ȼ�������ռ�
        CPPUNIT_ASSERT(((BYTE*)(&vectBigMemorys[dwCount]) - (BYTE*)(&vectBigMemorys[dwCount-1])) == sizeof(BigMemoryStruct));

        vectAddress.push_back(&vectBigMemorys[dwCount]);
        dwCount++;
    }
    CPPUNIT_ASSERT(dwCount <= 5);   //ǰ�汣����4�����ڵ�5����ʱ��push_backʱ�ͻ����·����ڴ棬�����п���
    for (DWORD dwIndex = 0; dwIndex < dwCount - 1; dwIndex++)
    {
        //vectBigMemorys�е�Ԫ���ƶ���λ�ã��Ѿ�����ԭ������
        CPPUNIT_ASSERT(&vectBigMemorys[dwIndex] != vectAddress[dwIndex]);
    }
    CPPUNIT_ASSERT(&vectBigMemorys[dwCount-1] == vectAddress[dwCount-1]);   //���ڵ�5�������ڿ����ڴ�֮����˱�������µ�ַ

}