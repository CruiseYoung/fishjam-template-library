#include "StdAfx.h"
#include "STLHashContainerTester.h"

#include <hash_set>
#include <hash_map>
#include <string>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//using namespace stdext;

void CSTLHashContainerTester::test_hash_map()
{
    typedef stdext::hash_map<int,CMyTestData> CMYTestDataHashMap;
    
    //ʹ�����ַ�ʽ����� һ�ι��죬���ο�������
    
    CMYTestDataHashMap myHashMap;
    myHashMap.insert(CMYTestDataHashMap::value_type(1,CMyTestData(1)));

    stdext::hash_map<CMyTestData, std::string>   TestData2NameString;
}

void CSTLHashContainerTester::test_hash_multimap()
{

}

void CSTLHashContainerTester::test_has_multiset()
{

}

void CSTLHashContainerTester::test_hash_set()
{
    stdext::hash_set<int> intSet;
    for (int i = 0; i< 10; i++)
    {
        intSet.insert(i);
    }
    
}