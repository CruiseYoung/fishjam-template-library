#include "StdAfx.h"
#include "STLMapTester.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


void CSTLMapTester::testAddData()
{
#ifdef NEED_OPERATOR_FUNC
    typedef std::map<int, CMyTestData > IntTestDataMap;
    IntTestDataMap dataMap;
    
    //���֮ǰû�� insert ��ͨ�� [] �������ã���ʹ��Ĭ�Ϲ���������ֵ 
    CMyTestData& defaultData = dataMap[3];
    CPPUNIT_ASSERT(defaultData.GetCurrentIndex() == -1);
    CPPUNIT_ASSERT(dataMap.size() == 1);

    dataMap[1] = 100; //�տ�ʼʱû��1��Ӧ�����ݣ�����
    CPPUNIT_ASSERT(dataMap.size() == 2);

    //��Ҫ���� operator = �� Ĭ�Ϲ��캯��
    #if 0
    //���ϴ���ȼ���������� -- ��Ҫ�������� CMyTestData ����Ч�ʵ�
       //�ü�1������ӳ����ں�һ��Ĭ�Ϲ����ֵ����
        pair<IntTestDataMap::iterator, bool> result = dataMap.insert(IntTestDataMap::value_type(1, CMyTestData()));
        result.first->second = 100;  //��ֵ���¹����ֵ����
    //��ʱӦ����ֱ���˵���insert�������滻
        dataMap.insert(IntTestDataMap::value_type(1, 100)); 
    #endif 

    dataMap[1] = 200; //��ʱ1�Ѿ���Ӧ�����ݣ����£���ʱЧ�ʸ�
    CPPUNIT_ASSERT(dataMap.size() == 2);
    //�ȼ����������
    #if 0
        dataMap.insert(IntTestDataMap::value_type(1, 200)).first->second = 200;
    #endif

    dataMap[2] = 300; //û�� 2 ��Ӧ�����ݣ�����
    CPPUNIT_ASSERT(dataMap.size() == 3);
    
    IntTestDataMap::iterator iter = efficientAddOrUpdate(dataMap,4,500);  //��Ч�Ľ��в���
    CPPUNIT_ASSERT(dataMap.size() == 4);

    efficientAddOrUpdate(dataMap,4,1000);  //��Ч�Ľ��и���
    CPPUNIT_ASSERT(dataMap.size() == 4);

#endif
}

void CSTLMapTester::test_map_sort()
{
    std::map<char,char>   int_map;

    //���� 1��10֮�������
    for (int i = 1; i < 10; i+= 2)
    {
        int_map.insert(std::map<char,char>::value_type(i + '0', i+'0'));
    }

    //���� 2��10֮���ż��
    for(int i = 2; i < 10; i+= 2)
    {
        int_map.insert(std::map<char,char>::value_type(i + '0',i+'0'));
    }
    std::string strBuf;
    //std::copy(int_map.begin(),int_map.end(),std::back_inserter<std::string>(strBuf));
    //CPPUNIT_ASSERT(strBuf == "123456789");

}
