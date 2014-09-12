#include "StdAfx.h"
#include "STLListTester.h"
#include "MyTestData.h"
#include "STLTesterUtil.h"

#include <list>
#include <string>
#include <algorithm>

#include <ftlString.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

void CSTLListTester::test_constructor()
{
    // define a list of 10 elements and initialise them all to 0 
    std::list<int> Fred(10, 0); //����һ��list����ͬʱ��ʼ������Ԫ��
    // list now contains 0,0,0,0,0,0,0,0,0,0 
    CPPUNIT_ASSERT(Fred.size() == 10);

    //����һ��list������һ��STL������һ����Χ����ʼ���������STL������һ����һ��list�� ������Ҫ��Ԫ��������ͬ�ĵ������Ϳ���

    int IntArray[2] = {1,2};
    std::list<int> Harry(&IntArray[0], &IntArray[2]);  //ע�⣬����ʹ�õ��ǲ����ڵ��±�2

    CPPUNIT_ASSERT(Harry.size() == 2);
    CPPUNIT_ASSERT( 1 == (*Harry.begin()));
    CPPUNIT_ASSERT( 2 == (*Harry.rbegin()));
}

void CSTLListTester::testMyTestDataList()
{
    DECLARE_MYTEST_DATA_COUNT_CHECKER(dataCountChecker,ctDataCount, 0, __FILE__, __LINE__);
    {
        //����List �б��������ʵ������ָ�룩������� push_back �� pus_front ��ʱ�����ÿ������죨�������⣩

        std::list<CMyTestData> myList;

        //��empty��������size()�Ƿ�Ϊ0
        //�������еı�׼������empty��һ������ʱ��Ĳ�����������һЩlistʵ�֣�size��������ʱ��--��Ҫ����(�� splice ��ʵ�ֳ���ʱ���ͻ)
        CPPUNIT_ASSERT( myList.empty() );
#if 0
        CPPUNIT_ASSERT( 0 == myList.size() ); //���ã�
#endif

        myList.push_back(CMyTestData(2));
        myList.push_back(CMyTestData(3));
        myList.push_front(CMyTestData(1));

        //��ָ��λ�õ�ǰ����루�� 0 ����ǰ�棩
        myList.insert(myList.begin(),CMyTestData(0));
        myList.insert(myList.end(),CMyTestData(4));
        
#if 0
        //Ҳ���Բ���һ����Χ�ڵ����ݣ�����ʹ�����������������
        //int IntArray[2] = {11,12}; 
        //list1.insert(list1.end(), &IntArray[0], &IntArray[2]); 
#endif

        CPPUNIT_ASSERT(myList.size() == 5);
        CPPUNIT_ASSERT(!myList.empty());
        CPPUNIT_ASSERT( (*myList.begin()).GetCurrentIndex() == 0);//0 ����ǰ��
        CPPUNIT_ASSERT( (*myList.rbegin()).GetCurrentIndex() == 4);//4 �������

        std::list<CMyTestData>::iterator iter = myList.begin();
        while (iter != myList.end())
        {
            //��ʱû���������ü����������� operator = �� ��������
            TRACE(_T("myList Index is %d \n"),iter->GetCurrentIndex());
            ++iter;
        }
        myList.pop_back();  //4
        myList.pop_front(); //0

        CPPUNIT_ASSERT(3 == myList.size());
        CPPUNIT_ASSERT(1 == (*myList.begin()).GetCurrentIndex());

        myList.erase(myList.begin()); //1
        CPPUNIT_ASSERT(2 == myList.size());
#if NEED_OPERATOR_FUNC
        //����� operator == ���бȽ� -- ע�� �ͱ���ָ��ʱ������
        myList.remove(CMyTestData(3));
#endif
        TRACE(_T("Before Call List::clear\n"));
        myList.clear();
        //�� clear ��ʱ�򣬻�����ڲ��� CMyTestData ����
        //-- ����Ŀǰ������Ƕ�����˻��Զ����������������������ָ��Ļ�����Ҫ�ֹ�delete

        //clear �ȼ����������
        //myList.erase(myList.begin(),myList.endif());
        TRACE(_T("After Call List::clear\n"));

        CPPUNIT_ASSERT(myList.empty());
    }
}

void CSTLListTester::test_sort()
{
    std::list<std::string> Staff; 
    std::list<std::string>::iterator PeopleIterator; 

    Staff.push_back("John"); 
    Staff.push_back("Bill"); 
    Staff.push_back("Tony"); 
    Staff.push_back("Fidel"); 
    Staff.push_back("Nelson"); 

    //δ�����ʱ��
    CPPUNIT_ASSERT( (*Staff.begin()) == "John"); 

    Staff.sort();
    //�������
    CPPUNIT_ASSERT( (*Staff.begin()) == "Bill");  //��һ��λ��
    CPPUNIT_ASSERT( (*Staff.rbegin()) == "Tony"); //���һ��λ��


    
    std::list<unsigned char> numbers;  //4, 6, 
    int i = 0;
    for (i = 4; i >= 0; i--)
    {
        numbers.push_back('0' + i);
        numbers.push_back('0' + (9 - i));
    }

    std::string strBuf;
    std::copy(numbers.begin(),numbers.end(),std::back_inserter<std::string>(strBuf));
    CPPUNIT_ASSERT(strBuf == "4536271809");

    numbers.sort();
    strBuf = "";
    std::copy(numbers.begin(),numbers.end(),std::back_inserter<std::string>(strBuf));
    CPPUNIT_ASSERT(strBuf == "0123456789");

    //ʹ��ָ���� greater �������з�������
    numbers.sort(std::greater<unsigned char>());
    strBuf = "";
    std::copy(numbers.begin(),numbers.end(),std::back_inserter<std::string>(strBuf));
    CPPUNIT_ASSERT(strBuf == "9876543210");

}


void CSTLListTester::test_remove()
{

}

void CSTLListTester::test_splice()
{
    std::list<CMyTestData> numbers;
    int i = 0;
    for (i = 1; i < 10; i++)
    {
        numbers.push_back(CMyTestData(i));
    }

    i = 1;
    std::list<CMyTestData>::iterator iter = numbers.begin();
    while (i <= 5 && iter != numbers.end())
    {
        i++;
        iter++;
    }

    std::list<CMyTestData> numberFirst5;
    std::list<CMyTestData> numberLast;

    CPPUNIT_ASSERT( numbers.size() == 9);
    CPPUNIT_ASSERT(numberFirst5.size() == 0);

    numberFirst5.splice(numberFirst5.begin(),numbers,numbers.begin(),iter);
    TRACE(_T("list splice, first 5: \n"));

    CPPUNIT_ASSERT(numbers.size() == 4);// splice��5������ʣ�� 4��
    CPPUNIT_ASSERT(numberFirst5.size() == 5);// splice��5������ʣ�� 4��

    for_each(numberFirst5.begin(),numberFirst5.end(),PrintMyTestData);

    //Flags.splice(Flags.begin(), CmdLineParameters, CmdLineParameters.begin(), StartOfFiles);
    numberLast.splice(numberLast.begin(),numbers);
    CPPUNIT_ASSERT(numberLast.size() == 4);//�� number ��ʣ�µģ�4����ȫ�ŵ� numberLast ��
    CPPUNIT_ASSERT(numbers.size() == 0);// ԭ�е�number�Ѿ��������ˡ�

    TRACE(_T("\n list splice, the last: \n"));
    for_each(numberLast.begin(),numberLast.end(),PrintMyTestData);
}


void CSTLListTester::test_unique()
{

}