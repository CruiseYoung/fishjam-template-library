#include "StdAfx.h"
#include "PerformanceTester.h"
#include <atlcoll.h>

void CPerformanceTester::test_container_holder()
{
    {
#ifdef NEED_OPERATOR_FUNC
        //��������ֱ�ӱ������,�ڼ������/���Ƶ�ʱ ʵ�����ǽ����˿��������
        DECLARE_MYTEST_DATA_COUNT_CHECKER(dataCountChecker,ctDataCount, 0, __FILE__, __LINE__);
        DECLARE_MYTEST_DATA_COUNT_CHECKER(constructCountChecker,ctConstructCount, 5, __FILE__, __LINE__);

        //��Ϊvectorû�����ó�ʼ����,push_back ʱ������չ,ʵ���ϵ� �������캯���ﵽ15�� -- ��������˴�ʹ����,Ӧ��ֻ��5��
        DECLARE_MYTEST_DATA_COUNT_CHECKER(copyConstructCounterChecker,ctCopyConstructCount, 15, __FILE__, __LINE__); 
        DECLARE_MYTEST_DATA_COUNT_CHECKER(destructCountChecker,ctDestructCount, 20, __FILE__, __LINE__);

        typedef std::vector<CMyTestData>  MyDataVector;
        MyDataVector dataVector;

        for (int i = 0; i< 5; i++)
        {
            dataVector.push_back(CMyTestData(i, FALSE));
        }
#endif
    }

    {
#ifdef NEED_OPERATOR_FUNC
        //��������ֱ�ӱ������,�ڼ������/���Ƶ�ʱ ʵ�����ǽ����˿��������
        DECLARE_MYTEST_DATA_COUNT_CHECKER(dataCountChecker,ctDataCount, 0, __FILE__, __LINE__);
        DECLARE_MYTEST_DATA_COUNT_CHECKER(constructCountChecker,ctConstructCount, 5, __FILE__, __LINE__);

        DECLARE_MYTEST_DATA_COUNT_CHECKER(copyConstructCounterChecker,ctCopyConstructCount, 5, __FILE__, __LINE__); 
        DECLARE_MYTEST_DATA_COUNT_CHECKER(destructCountChecker,ctDestructCount, 10, __FILE__, __LINE__);

        typedef std::vector<CMyTestData>  MyDataVector;
        MyDataVector dataVector;
        dataVector.reserve(10);    //���ó�ʼ����,�����ƶ�

        for (int i = 0; i< 5; i++)
        {
            dataVector.push_back(CMyTestData(i, FALSE));
        }
#endif
    }

    {
        //����ָ��ķ�ʽ,��ֻ�����ʵ�����ɵı���,����ʱ����ָ�봦��,��Լ�ռ�/ʱ��
        //����Ҫ�ֹ��ͷ�,���������ڴ�й© -- ����ʹ�� shareptr 
        typedef std::vector<CMyTestData*>   MyDataPointVector;
        MyDataPointVector   dataPointVector;

        DECLARE_MYTEST_DATA_COUNT_CHECKER(dataCountChecker,ctDataCount, 0, __FILE__, __LINE__);
        DECLARE_MYTEST_DATA_COUNT_CHECKER(constructCountChecker,ctConstructCount, 5, __FILE__, __LINE__);
        DECLARE_MYTEST_DATA_COUNT_CHECKER(copyConstructCounterChecker,ctCopyConstructCount, 0, __FILE__, __LINE__);
        DECLARE_MYTEST_DATA_COUNT_CHECKER(destructCountChecker,ctDestructCount, 5, __FILE__, __LINE__);

        for (int i = 0; i < 5; i++)
        {
            dataPointVector.push_back(new CMyTestData(i));
        }

        for_each(dataPointVector.begin(), dataPointVector.end(), FTL::ObjecteDeleter<CMyTestData*>() );
        dataPointVector.clear();
    }

    {
#ifdef NEED_OPERATOR_FUNC
        const int DATA_COUNT = 10;

        //��������ֱ�ӱ������,�ڼ������/���Ƶ�ʱ ʵ�����ǽ����˿��������
        DECLARE_MYTEST_DATA_COUNT_CHECKER(dataCountChecker,ctDataCount, 0, __FILE__, __LINE__);
        DECLARE_MYTEST_DATA_COUNT_CHECKER(constructCountChecker,ctConstructCount, DATA_COUNT, __FILE__, __LINE__);

        DECLARE_MYTEST_DATA_COUNT_CHECKER(copyConstructCounterChecker,ctCopyConstructCount, DATA_COUNT, __FILE__, __LINE__);
        DECLARE_MYTEST_DATA_COUNT_CHECKER(destructCountChecker,ctDestructCount, 2*DATA_COUNT, __FILE__, __LINE__);

        //ע��:AtlArray ����չ����ʱ,���´��ԭ�������� -- ���ܱ� vector Ҫ��
        typedef CAtlArray<CMyTestData>  MyDataArray;
        MyDataArray dataArray;
        
        for (int i = 0; i< DATA_COUNT; i++)
        {
            dataArray.Add(CMyTestData(i));
        }
#endif

    }
}
