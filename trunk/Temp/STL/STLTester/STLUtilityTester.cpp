#include "StdAfx.h"
#include "STLUtilityTester.h"
#include "STLTesterUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

void CSTLUtilityTester::test_advance()
{
    typedef std::deque<int> IntDeque;
    typedef IntDeque::iterator Iter;
    typedef IntDeque::const_iterator ConstIter;

    IntDeque  d;
    for(int i = 0; i< 10; i++)
    {
        d.push_back(i);
    }
    ConstIter ci = d.end();         // ��ciָ��d

    //Ҫ�õ���const_iteratorָ��ͬһλ�õ�iterator�����Ƚ�iteratorָ����������ʼλ�ã�
    //Ȼ�������ǰ�Ƶ���const_iterator����������ʼλ�õ�ƫ����һ����λ�ü���
    Iter i(d.begin());				// ��ʼ��iΪd.begin()
    //distance��������ָ��ͬһ��������iterator֮��ľ��룬advance�����ڽ�һ��iterator�ƶ�ָ���ľ���
    //ͨ�� ConstIter��ʽ��ָ��distance���õ�ģ��������ͣ��Ӷ�����������Լ��ó����ǵ�����(�����޷�����)
    advance(i, std::distance<ConstIter>(i, ci));	// ��i�Ƶ�ָ��ciλ��

    //advance(p, 2);  // same as p = p + 2;

    //TODO: �����ĵ�Ԫ�����Ƿ���˵������
    CPPUNIT_ASSERT(i == ci);        // �Ѿ���const_iteratorת����iterator
}

void CSTLUtilityTester::test_distance()
{
    std::vector<int> intVector;
    for (int i = 1; i <= 9; i++)
    {
        intVector.push_back(i);
    }

    std::vector<int>::iterator iter = std::find(intVector.begin(), intVector.end(), 7);
    //�ҵ�Ԫ��7�ĵ�����
    CPPUNIT_ASSERT(7 == *iter);

    //��Ԫ��7�ĵ���������β����Ҫ���� 3 �Σ�����Ϊ 8,9,end()
    std::vector<int>::difference_type diff = std::distance(iter, intVector.end());
    CPPUNIT_ASSERT(3 == diff );

    //����3���Ժ󣬾�ָ���ǽ�����
    std::advance(iter, diff);
    CPPUNIT_ASSERT(intVector.end() == iter);
}

void CSTLUtilityTester::test_delete()
{
    DECLARE_MYTEST_DATA_COUNT_CHECKER(dataCountChecker,ctDataCount, 0, __FILE__, __LINE__);

    std::vector<CMyTestData*> myTestArray;
    for (int i = 0; i< 10; i++)
    {
        myTestArray.push_back(new CMyTestData(i));
    }

    //dosomething

    {
        DECLARE_MYTEST_DATA_COUNT_CHECKER(destructCountChecker,ctDestructCount, 10, __FILE__, __LINE__);
        std::for_each(myTestArray.begin(),myTestArray.end(), ObjecteDeleter<CMyTestData*>());
    }
}

void CSTLUtilityTester::test_boost_delete()
{
    DECLARE_MYTEST_DATA_COUNT_CHECKER(dataCountChecker,ctDataCount, 0, __FILE__, __LINE__);
    {
#ifdef BOOST
        //ע�⣺����ʹ�ñ�׼�� auto_ptr ְ��ָ�� -- �μ� Effective STL ����8
        //  �����ܣ� vector<auto_ptr<CMyTestData> > MyTestDatas;	

        typedef boost::shared_ptr<CMyTestData> SPD;	//SPD = "shared_ptr to Widget"
        vector< SPD > vwp;
        for (int i = 0; i < 10; ++i)
        {
            vwp.push_back(SPD(new CMyTestData));	// ��һ��Widget����SPD,Ȼ�����һ��push_back
        }
#endif
    }
    //�����������ڣ��Զ��ͷ�(û���ڴ�й©������������������׳��쳣)
}

void CSTLUtilityTester::test_StringPtrSet()
{
    {
        StringPtrSet strPtrSet;
        //set<string*> strPtrSet;
        strPtrSet.insert(new std::string("Banana"));
        strPtrSet.insert(new std::string("Orange"));
        strPtrSet.insert(new std::string("Apple"));

#if 0
        // ͨ�������á�ת����ssp�е�ÿ��Ԫ�أ��ѽ��д��cout
        transform(strPtrSet.begin(), strPtrSet.end(),ostream_iterator<string>(cout, "\n"),Dereference());
#endif

        //������ĸ˳��Բ����stringָ������
        CPPUNIT_ASSERT((**strPtrSet.begin()) == "Apple"); 
        CPPUNIT_ASSERT((**strPtrSet.rbegin()) == "Orange");

        for_each(strPtrSet.begin(),strPtrSet.end(),ObjecteDeleter<std::string*>());

    }

    //ʹ��ģ��
    {
        std::set<std::string*,DereferenceLess> strPtrSet2;
        strPtrSet2.insert(new std::string("Banana"));
        strPtrSet2.insert(new std::string("Orange"));
        strPtrSet2.insert(new std::string("Apple"));

        //������ĸ˳��Բ����stringָ������
        CPPUNIT_ASSERT((**strPtrSet2.begin()) == "Apple"); 
        CPPUNIT_ASSERT((**strPtrSet2.rbegin()) == "Orange");

        std::for_each(strPtrSet2.begin(),strPtrSet2.end(),ObjecteDeleter<std::string*>());
    }
}


void CSTLUtilityTester::test_swap()
{
    std::vector<int> intVector;
    CPPUNIT_ASSERT(intVector.capacity() == 0);

    for (int i = 0; i< 100; i++)
    {
        intVector.push_back(i);
    }

    CPPUNIT_ASSERT(intVector.capacity() >= 100);

    intVector.erase(intVector.begin() + 20,intVector.end());

    CPPUNIT_ASSERT(intVector.capacity() >= 100);

    //
    std::vector<int>(intVector).swap(intVector);

    CPPUNIT_ASSERT(intVector.capacity() < 100 );
    CPPUNIT_ASSERT(intVector.capacity() >= 20 ); //��ʣ20��
}

template<typename T>
class CMyBPFCImpl1 : public CBPFCImpl<T>
{
public:
    virtual void operator()(const T& val) const
    {
        TRACE("CMyBPFCImpl1 CurrentIndex = %d\n",m_MyData.GetCurrentIndex());
    }
};

void CSTLUtilityTester::test_BPFC()
{
    std::list<CMyTestData> myTestDataList;
    for (int i = 0; i< 5; i++)
    {
        myTestDataList.push_back(CMyTestData(i));
    }
    //CPPUNIT_ASSERT(FALSE); 
    //��ô���롢���ԣ� ͷ�ļ�Ҳ����ȷ��
    //for_each(myTestDataList.begin(),myTestDataList.end(),CBPFC<CMyTestData>(CBPFCImpl<CMyTestData>()));
}