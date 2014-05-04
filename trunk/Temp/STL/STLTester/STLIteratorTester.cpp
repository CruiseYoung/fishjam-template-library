#include "StdAfx.h"
#include "STLIteratorTester.h"

#include <iterator>
#include "STLTesterUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//���bUseBackPlusIterΪtrue�Ļ�����ʹ�� ���õ��������� �ķ�ʽ������ʹ�� erase �ķ���ֵ���µ�����


template<typename ContainerT>
void testSeriesContainer(ContainerT container, bool bUseBackPlusIter)
{
    for (int i = 10; i < 20; i++)
    {
        container.push_back(ContainerT::value_type(i));
    }
    CPPUNIT_ASSERT(container.size() == 10);

    int iCount = 0;
    typename ContainerT::iterator iter = container.begin();
    while (iter != container.end())
    {
        if (iCount % 2 == 0)
        {
            if (bUseBackPlusIter)
            {
                //ʹ�ú��õ����ķ�ʽ��
                container.erase(iter++);
            }
            else
            {
                //ɾ���󷵻���һ��λ�ã�������++
                iter = container.erase(iter);
            }
        }
        else
        {
            ++iter;
        }
        iCount++;
    }

    CPPUNIT_ASSERT(container.size() == 5);
    for (ContainerT::iterator iter = container.begin();
        iter != container.end(); ++iter)
    {
        CPPUNIT_ASSERT(*iter % 2 == 1);
    }
}

template<typename ContainerT>
void testSetContainer(ContainerT container, bool bUseBackPlusIter)
{
    for (int i = 10; i < 20; i++)
    {
        container.insert(ContainerT::value_type(i));
    }
    CPPUNIT_ASSERT(container.size() == 10);

    int iCount = 0;
    typename ContainerT::iterator iter = container.begin();
    while (iter != container.end())
    {
        if (iCount % 2 == 0)
        {
            if (bUseBackPlusIter)
            {
                //ʹ�ú��õ����ķ�ʽ��
                container.erase(iter++);
            }
            else
            {
                //ɾ���󷵻���һ��λ�ã�������++
                iter = container.erase(iter);
            }
        }
        else
        {
            ++iter;
        }
        iCount++;
    }

    CPPUNIT_ASSERT(container.size() == 5);
    for (ContainerT::iterator iter = container.begin();
        iter != container.end(); ++iter)
    {
        CPPUNIT_ASSERT(*iter % 2 == 1);
    }
}

template<typename ContainerT>
void testMapContainer(ContainerT container, bool bUseBackPlusIter)
{

    for (int i = 10; i < 20; i++)
    {
        container[ContainerT::key_type(i)] = ContainerT::mapped_type(i);
    }
    CPPUNIT_ASSERT(container.size() == 10);

    int iCount = 0;
    typename ContainerT::iterator iter = container.begin();
    while (iter != container.end())
    {
        if (iCount % 2 == 0)
        {
            if (bUseBackPlusIter)
            {
                //ʹ�ú��õ����ķ�ʽ��
                container.erase(iter++);
            }
            else
            {
                //ɾ���󷵻���һ��λ�ã�������++
                iter = container.erase(iter);
            }
        }
        else
        {
            ++iter;
        }
        iCount++;
    }

    CPPUNIT_ASSERT(container.size() == 5);
    for (ContainerT::iterator iter = container.begin();
        iter != container.end(); ++iter)
    {
        CPPUNIT_ASSERT((*iter).first % 2 == 1);
    }
}

void CSTLIteratorTester::test_deque_loopdelete()
{
    std::deque<int> intVector;
    CPPUNIT_ASSERT(intVector.empty());
    testSeriesContainer(intVector, false);
    CPPUNIT_ASSERT(intVector.empty());

#if 0
    //deque����ʹ�� ���õ���������
    testSeriesContainer(intVector, true);
#endif
}

void CSTLIteratorTester::test_vector_loopdelete()
{
    std::vector<int> intVector;
    CPPUNIT_ASSERT(intVector.empty());
    testSeriesContainer(intVector, false);
    CPPUNIT_ASSERT(intVector.empty());
#if 0
    //deque����ʹ�� ���õ���������
    testSeriesContainer(intVector, true);
#endif
}


void CSTLIteratorTester::test_list_loopdelete()
{
    std::list<int> intVector;
    CPPUNIT_ASSERT(intVector.empty());
    testSeriesContainer(intVector, false);
    CPPUNIT_ASSERT(intVector.empty());
    testSeriesContainer(intVector, true);
}

void CSTLIteratorTester::test_set_loopdelete()
{
    std::set<int> intVector;
    CPPUNIT_ASSERT(intVector.empty());
    testSetContainer(intVector, false);
    CPPUNIT_ASSERT(intVector.empty());
    testSetContainer(intVector, true);
}

void CSTLIteratorTester::test_map_loopdelete()
{
    std::map<TestType,TestType> intVector;
    CPPUNIT_ASSERT(intVector.empty());
    testMapContainer(intVector, false);
    CPPUNIT_ASSERT(intVector.empty());
    testMapContainer(intVector, true);
}

void CSTLIteratorTester::test_bad_loopdelete()
{
    typedef std::list<TestType> TypeContainer;
    TypeContainer container;
    for (int i = 10; i < 20; i++)
    {
        container.push_back(i);
    }
    CPPUNIT_ASSERT(container.size() == 10);

#if 0
    for (TypeContainer::iterator iter = container.begin();
        iter != container.end(); ++iter)
    {
        if (*iter % 2 == 0)
        {
            //����ʹ�����ַ�ʽ���������������ִ���
            container.erase(iter);
        }
    }
    CPPUNIT_ASSERT(container.size() == 5);
#endif
}

void CSTLIteratorTester::test_better_loopdelete()
{
    typedef std::vector<TestType> VecContainer;
    VecContainer container;
    for (int i = 0; i < 10; i++)
    {
        container.push_back(TestType(i));
    }
    CPPUNIT_ASSERT( container.size()==10 );

    //ע�⣺��Ϊ remove_if �Ժ󣬲�Ҫ��Ԫ�ر��Ƶ��������˻���Ҫ erase,
    //ͨ�� ʹ�� erase(remove_if(xxx), end() ) �ķ�ʽһ��ִ����ȫ���߼�

    //ɾ�� 2 < x ��Ԫ�أ�ʣ�µ��� 0~2
    VecContainer::iterator iterReturn = std::remove_if(container.begin(),container.end(), bind1st(std::less<int>(),2));
    CPPUNIT_ASSERT(container.size() == 10);

    container.erase(iterReturn,container.end());

    CPPUNIT_ASSERT( container.size() == 3 );

	{
		DECLARE_MYTEST_DATA_COUNT_CHECKER(countChecker,ctDataCount, 0, __FILE__, __LINE__);
		DECLARE_MYTEST_DATA_COUNT_CHECKER(constructChecker,ctConstructCount, 10, __FILE__, __LINE__);
		DECLARE_MYTEST_DATA_COUNT_CHECKER(destructChecker,ctDestructCount, 10, __FILE__, __LINE__);

		//ɾ��map�е�Ԫ��
		typedef std::map<int , CMyTestData*> MapDataContainer;
		MapDataContainer mapDataContainer;
		for (LONG index = 0; index < 10; index++)
		{
			mapDataContainer[index] = new CMyTestData(index);
		}

		//������Ҫɾ���Ĳ��� -- 
		//MapDataContainer tmpMapContainer;
		//std::remove_copy_if(mapDataContainer.begin(), mapDataContainer.end(), 
		//	inserter(tmpMapContainer, tmpMapContainer.end()),
		//	std::bind1st(std::less<LONG>(), 3));
		
		for_each(mapDataContainer.begin(), mapDataContainer.end(),
			PairDeleter<LONG, CMyTestData*>());

	}

    //std::remove_copy_if(mapContainer.begin(), mapContainer.end(),
    // inserter(tmpMapContainer.begin(), tmpMapContainer.end()),
    // equal_to())
}

void CSTLIteratorTester::test_ostream_iterator()
{
    std::ostringstream os;
    std::vector<int>    intVector;
    for (int i = 1; i < 10; i++)
    {
        intVector.push_back(i);
    }

    std::copy(intVector.begin(), intVector.end(), std::ostream_iterator<int>(os, ","));
    const std::string& strInfo = os.str();

    //��ʲô�������Բ������һ�� "," �����
    CPPUNIT_ASSERT( strInfo == "1,2,3,4,5,6,7,8,9,");
}

void CSTLIteratorTester::test_front_inserter()
{
    std::vector<int>    intVector(10);
    std::list<int>      intList;

    std::generate(intVector.begin(), intVector.end(), rand );
    CPPUNIT_ASSERT(intVector.size() == 10);

    //ͨ��ʹ��front_inserter���������󣬽�vector�е�Ԫ�����β��뵽����ǰ�� -- �൱�� vector Ԫ�صĵ���
    std::copy(intVector.begin(), intVector.end(), std::front_inserter(intList));
    CPPUNIT_ASSERT(intList.size() == intVector.size());

    STLTESTER_ASSERT_CONTAINER_ELEMENT_EQUAL(intVector.begin(), intVector.end(), intList.rbegin(), intList.rend());
}

