#include "StdAfx.h"
#include "STLSetTester.h"
#include "STLTesterUtil.h"

#include <streambuf>
#include <sstream>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

void CSTLSetTester::test_set_order()
{
    std::set<char>   int_set;

    //���� 1��10֮�������
    for (int i = 1; i < 10; i+= 2)
    {
        int_set.insert(i + '0');
    }

    //���� 2��10֮���ż��
    for(int i = 2; i < 10; i+= 2)
    {
        int_set.insert(i + '0');
    }
    std::string strBuf;
    //����˳��set�е�ֵ�����string�У���Ȼ��˳���
    std::copy(int_set.begin(),int_set.end(),std::back_inserter<std::string>(strBuf));
    CPPUNIT_ASSERT(strBuf == "123456789");
}

void CSTLSetTester::test_set_find()
{
    DECLARE_MYTEST_DATA_COUNT_CHECKER(myDataCountChecker,ctDataCount, 0, __FILE__, __LINE__);
    {
        std::set<CMyTestData*,less_CMyTestData_Pointer> myDataSet;
        {
            //���ڱ������ָ�룬��˲���������ʱ����
            DECLARE_MYTEST_DATA_COUNT_CHECKER(constructCountChecker,ctConstructCount, 5, __FILE__, __LINE__);
            DECLARE_MYTEST_DATA_COUNT_CHECKER(copyConstructCountChecker,ctCopyConstructCount, 0, __FILE__, __LINE__);
            DECLARE_MYTEST_DATA_COUNT_CHECKER(destructCountChecker,ctDestructCount, 0, __FILE__, __LINE__);
            for (int i = 5; i > 0; i--)
            {
                myDataSet.insert(new CMyTestData(i));
            }
        }

        //�ܹ�5�������� CurrentIndex ��С�����˳������(1~5)
        CPPUNIT_ASSERT(myDataSet.size() == 5);
        CPPUNIT_ASSERT(((CMyTestData*)(*myDataSet.begin()))->GetCurrentIndex() == 1 );
        CPPUNIT_ASSERT(((CMyTestData*)(*myDataSet.rbegin()))->GetCurrentIndex() == 5 );

        //���ҵ��м��3���������ҵ�����set�е� 8;
        CMyTestData tempData3(3);
        CPPUNIT_ASSERT(myDataSet.find(&tempData3) != myDataSet.end());

        CMyTestData tempData8(8);
        CPPUNIT_ASSERT(myDataSet.find(&tempData8) == myDataSet.end());

        std::for_each(myDataSet.begin(),myDataSet.end(),ObjecteDeleter<CMyTestData*>());
        myDataSet.clear();
    }

    {
#ifdef NEED_OPERATOR_FUNC
        std::set<CMyTestData> myDataSet;
        for( int i = 0; i < 100; i++)
        {
            myDataSet.insert(CMyTestData(i, FALSE));
        }
        {
            //ʹ�ó�Ա��������
            DECLARE_MYTEST_DATA_COUNT_CHECKER(compareLessCountChecker,ctCompareEqualCount, 0, __FILE__, __LINE__);
            LONG lOldCompareLessCount = CMyTestData::GetCompareLessCount();
            std::set<CMyTestData>::iterator iter = myDataSet.end();
            {
                //�ڲ���ʱ�����������м����ʱ�����ˣ��˴���1�� CMyTestData(80, FALSE)
                DECLARE_MYTEST_DATA_COUNT_CHECKER(destrucyCountChecker,ctDestructCount, 1, __FILE__, __LINE__);
                iter = myDataSet.find(CMyTestData(80, FALSE));
            }
            CPPUNIT_ASSERT(iter != myDataSet.end() );
            CPPUNIT_ASSERT(iter->GetCurrentIndex() == 80 );

            LONG lUseCompareLessCount = CMyTestData::GetCompareLessCount() - lOldCompareLessCount;
            TRACE(" Find CMyTestData(80) in [0,100) use Compare Less %d Count\n", lUseCompareLessCount);
            //���Ҵ���ԶС�ڱ�����ʽ���ҵ�80 -- ���Խ��Ϊ 15
            CPPUNIT_ASSERT( lUseCompareLessCount < 80);
        }

        {
            //ʹ��ȫ�ֵ�find�������� -- ��ʹ�����Եķ�ʽ��ʹ�� operator== ���β���
            DECLARE_MYTEST_DATA_COUNT_CHECKER(compareEqualCountChecker,ctCompareEqualCount, 81, __FILE__, __LINE__);
            DECLARE_MYTEST_DATA_COUNT_CHECKER(compareLessCountChecker,ctCompareLessCount, 0, __FILE__, __LINE__);

            std::set<CMyTestData>::iterator iter = std::find(myDataSet.begin(), myDataSet.end(), CMyTestData(80, FALSE));
            CPPUNIT_ASSERT(iter != myDataSet.end());
            CPPUNIT_ASSERT(iter->GetCurrentIndex() == 80);
        }
#endif
    }
}

void CSTLSetTester::test_set_insert()
{
#ifdef NEED_OPERATOR_FUNC
    DECLARE_MYTEST_DATA_COUNT_CHECKER(myDataCountChecker,ctDataCount, 0, __FILE__, __LINE__);
    {
        const int MY_DATA_COUNT = 5;
        //�ᰴ��Ĭ�ϵ� < ������ ���������� 0~4
        typedef std::set<CMyTestData> MyDataSet;
        typedef std::pair<MyDataSet::iterator, bool> MyDataSetInsertResult;

        MyDataSet myDataSet;
        {
            DECLARE_MYTEST_DATA_COUNT_CHECKER(constructCountChecker, ctConstructCount, MY_DATA_COUNT, __FILE__, __LINE__); //�����LONG�����Ĺ���
            
            //���ڱ������Ԫ�أ���˻�ͨ�������������� MY_DATA_COUNT ����ʱ������Ȼ�����ͷ�
            DECLARE_MYTEST_DATA_COUNT_CHECKER(copyConstructCountChecker,ctCopyConstructCount, MY_DATA_COUNT, __FILE__, __LINE__);
            DECLARE_MYTEST_DATA_COUNT_CHECKER(destructCountChecker, ctDestructCount, MY_DATA_COUNT, __FILE__, __LINE__);
            DECLARE_MYTEST_DATA_COUNT_CHECKER(compareLessCountChecker, ctCompareLessCount, 16, __FILE__, __LINE__); // 3 + 4 + 4 + 5

            //set ʹ�� < �� !< ���бȽ�
            DECLARE_MYTEST_DATA_COUNT_CHECKER(compareEqualCountChecker,ctCompareEqualCount, 0, __FILE__, __LINE__);
            for (int i = 0; i < MY_DATA_COUNT; i++)
            {
                myDataSet.insert(CMyTestData(i));
            }
        }

        CMyTestData& oldOneData = *myDataSet.begin();
        CPPUNIT_ASSERT(oldOneData.GetCurrentIndex() == 0);
        LONG lOldOtherInfo = oldOneData.GetOtherInfo();

        MyDataSetInsertResult insertResult = myDataSet.insert(CMyTestData(0));
        //�����Ѿ�����ͬ��Ԫ���ˣ���˲���ʧ��,���᷵�ز��ҵ���ͬԪ�صĵ�����
        CPPUNIT_ASSERT(insertResult.first == myDataSet.find(CMyTestData(0)));
        CPPUNIT_ASSERT(insertResult.second == false);

        //ԭ����Ԫ�ز�������仯
        CPPUNIT_ASSERT(lOldOtherInfo == (*myDataSet.begin()).GetOtherInfo());
    }
#endif
}