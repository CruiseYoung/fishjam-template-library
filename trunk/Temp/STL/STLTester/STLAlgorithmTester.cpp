#include "StdAfx.h"
#include "STLAlgorithmTester.h"
#include "MyTestData.h"
#include "STLTesterUtil.h"
#include <functional>
#include <algorithm>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ���صڶ���Ԫ���Ƿ��ǵ�һ��Ԫ�ص�����
bool twice (int elem1, int elem2 )
{
    return elem1 * 2 == elem2;
}

CSTLAlgorithmTester::CSTLAlgorithmTester()
{

}

CSTLAlgorithmTester::~CSTLAlgorithmTester()
{

}

void CSTLAlgorithmTester::setUp()
{
    m_string2MyTestDataMap.insert(String2MyTestDataPair("one", new CMyTestData(1)));
    m_string2MyTestDataMap.insert(String2MyTestDataPair("two", new CMyTestData(2)));
    m_string2MyTestDataMap.insert(String2MyTestDataPair("three", new CMyTestData(3)));

    m_string2MyTestDataMap.insert(String2MyTestDataPair("ONE", new CMyTestData(1)));
    m_string2MyTestDataMap.insert(String2MyTestDataPair("TWO", new CMyTestData(2)));
    m_string2MyTestDataMap.insert(String2MyTestDataPair("THREE", new CMyTestData(3)));


    m_MyTestDataList.push_back(new CMyTestData(8));
    m_MyTestDataList.push_back(new CMyTestData(1));
    m_MyTestDataList.push_back(new CMyTestData(3));
    m_MyTestDataList.push_back(new CMyTestData(2));
    m_MyTestDataList.push_back(new CMyTestData(4));

    m_MyTestDataList.push_back(new CMyTestData(-1));
    m_MyTestDataList.push_back(new CMyTestData(-3));
    m_MyTestDataList.push_back(new CMyTestData(-2));
    m_MyTestDataList.push_back(new CMyTestData(-4));

}

void CSTLAlgorithmTester::tearDown()
{
    for_each(m_string2MyTestDataMap.begin(), m_string2MyTestDataMap.end(),
        PairDeleter<std::string, CMyTestData*>());

    m_string2MyTestDataMap.clear();

    for_each(m_MyTestDataList.begin(), m_MyTestDataList.end(),ObjecteDeleter<CMyTestData*>() );
    m_MyTestDataList.clear();
}

void CSTLAlgorithmTester::test_adjacent_find()
{
    //std::adjacent_find(m_MyTestDataList.begin(), m_MyTestDataList.end() , )
    m_MyTestDataList.sort(less_CMyTestData_Pointer());

    std::list <int> L;
    std::list <int>::iterator Iter;
    std::list <int>::iterator result1, result2;
    L.push_back( 50 );
    L.push_back( 10 );
    L.push_back( 20 );
    L.push_back( 40 );
    L.push_back( 20 );
    
    L.sort();

    result1 = std::adjacent_find( L.begin( ), L.end( ) );
    //������������ֵͬ�������ٽ���20
    CPPUNIT_ASSERT(*result1 == 20);
    
    result2 = std::adjacent_find( L.begin( ), L.end( ), twice );
    //��������һ�����ǵ�һ����2�����ٽ�Ԫ��(�ֱ��� 10 & 20)
    CPPUNIT_ASSERT(*result2 == 10); 
    CPPUNIT_ASSERT(*(++result2) == 20);
}

void CSTLAlgorithmTester::test_adjacent_difference()
{
    //std::adjacent_difference()
}

void CSTLAlgorithmTester::test_binary_search()
{
    std::list <int> L;
    std::list <int>::iterator Iter;
    bool b1;

    L.push_back( 50 );
    L.push_back( 10 );
    L.push_back( 30 );
    L.push_back( 20 );
    L.push_back( 25 );
    L.push_back( 5 );
    L.push_back( -10 );
    L.push_back( -15 );
    L.sort( );  //����Ĭ���Ǵ�С���� -- less<int>��
    
    b1 = binary_search( L.begin( ), L.end( ), 10 );
    CPPUNIT_ASSERT(true == b1); //�ҵ���10

    b1 = binary_search( L.begin( ), L.end( ), 15 );
    CPPUNIT_ASSERT(false == b1); //û���ҵ�15

    L.sort(std::greater<int>()); //���մӴ�С��˳������

    b1 = binary_search( L.begin(), L.end(), 10, std::greater<int>());
    CPPUNIT_ASSERT( true == b1);

    //b1 = std::binary_search(L.begin(), L.end(), 10, std::binder2nd(std::ptr_fun(abs)),-15);
    //RIter = find_if( v1.begin( ), v1.end( ),
    //    not1 ( bind2nd (ptr_fun ( strcmp ), "pearly" ) ) );

}

void CSTLAlgorithmTester::test_copy_if()
{
    //���ļ����ж�ȡ���ݵ��ڴ�����У�
    //std::copy(std::istream_iterator<Pointer2D>(infile), std::istream_iterator<Pointer2D>(),
    //    std::back_inserter(points));

    const int BIGGER_NUMBER = 8;
    std::vector<int> intVector1,intVector2;

    intVector1.reserve(10);
    for (int i = 0; i<= 10; i++)
    {
        intVector1.push_back(i);
    }

    CPPUNIT_ASSERT(intVector2.size() == 0);  //��ʼʱû������
    FTL::copy_if(intVector1.begin(),intVector1.end(),back_inserter(intVector2) ,
        bind2nd(std::greater<int>(), BIGGER_NUMBER));
    CPPUNIT_ASSERT(intVector2.size() == 2);  //��������8��2������(9��10)
    
    CPPUNIT_ASSERT(*intVector2.begin() > BIGGER_NUMBER); 
}

void CSTLAlgorithmTester::test_for_each()
{
    std::list<std::string> FruitAndVegetables;
    FruitAndVegetables.push_back("carrot");
    FruitAndVegetables.push_back("pumpkin");
    FruitAndVegetables.push_back("potato");
    FruitAndVegetables.push_front("apple");
    FruitAndVegetables.push_front("pineapple");
    
    g_forEachCount = 0;

    for_each(FruitAndVegetables.begin(), FruitAndVegetables.end(), PrintStringItem);
    
    CPPUNIT_ASSERT(g_forEachCount == FruitAndVegetables.size());
}


void CSTLAlgorithmTester::test_generate()
{
    int checkArray[] = {1,2,3,4,5,6,7,8,9,10};
    
    std::vector<int>  intVect(10);
    //ʹ�� generate ����֪��С�� vector ������ [1,10]
    std::generate(intVect.begin(), intVect.end(), sequence_generator<int>(1,1));
    STLTESTER_ASSERT_CONTAINER_ELEMENT_EQUAL(checkArray, ARRAY_PAST_THE_END(checkArray), intVect.begin(), intVect.end());

    std::list<int> intList;
    //ʹ�� generate_n ��list������ָ�������� [1,10]
    std::generate_n(back_inserter(intList), _countof(checkArray), sequence_generator<int>(1,1));
    STLTESTER_ASSERT_CONTAINER_ELEMENT_EQUAL(checkArray, ARRAY_PAST_THE_END(checkArray), intList.begin(), intList.end());

    {
#ifdef NEED_OPERATOR_FUNC
        //���� set �е� CMyTestData -- ע����Ϊ �������⣬��Ч������
        DECLARE_MYTEST_DATA_COUNT_CHECKER(countChecker,ctDataCount, 0, __FILE__, __LINE__);
        DECLARE_MYTEST_DATA_COUNT_CHECKER(constructChecker,ctConstructCount, 5, __FILE__, __LINE__);

         //sequence_generator_ex �ķ��� + �ŵ� set ��
        DECLARE_MYTEST_DATA_COUNT_CHECKER(copyChecker,ctCopyConstructCount, 5 * 2, __FILE__, __LINE__);
        DECLARE_MYTEST_DATA_COUNT_CHECKER(assignmentChecker,ctAssignmentCount, 0, __FILE__, __LINE__);
        DECLARE_MYTEST_DATA_COUNT_CHECKER(destrucChecker,ctDestructCount, 5 * 3, __FILE__, __LINE__);

        std::set<CMyTestData>   dataSet;
        std::generate_n(
            inserter(dataSet, dataSet.begin()), 
            5, sequence_generator_ex<int,CMyTestData, CMyTestData>(1,1));
        CPPUNIT_ASSERT(dataSet.size() == 5);
#endif
    }

    {
        std::set<CMyTestData*>   dataPointerSet;

    }

    {
        typedef std::map<int, CMyTestData*> Int2DataPointerMap;
        typedef Int2DataPointerMap::value_type Int2DataPointerMapValue;
        Int2DataPointerMap int2DataPtrMap;
        //std::generate_n(
        //    inserter(int2DataPtrMap, int2DataPtrMap.begin()), 
        //    5, sequence_generator_ex<int,Int2DataPointerMapValue, Int2DataPointerMapValue(1,1));
    }
}

void CSTLAlgorithmTester::test_includes()
{

}

void CSTLAlgorithmTester::test_count()
{
    std::list<int> studentScroe;
    studentScroe.push_back(100); 
    studentScroe.push_back(80);
    studentScroe.push_back(45);
    studentScroe.push_back(75);
    studentScroe.push_back(99); 
    studentScroe.push_back(100);
    studentScroe.push_back(59);
    studentScroe.push_back(60);

    //ͳ������ָ��ֵ�ĸ���
    std::list<int>::iterator::difference_type NumberOf100Scores = count(studentScroe.begin(), studentScroe.end(),100);
    CPPUNIT_ASSERT(2 == NumberOf100Scores);


    //ʹ�þ�̬����ָ�� -- ͳ�Ʋ�����(����60�ĸ���)
    std::list<int>::iterator::difference_type NumberFailure = count_if(
        studentScroe.begin(),studentScroe.end(),
        std::bind2nd(std::less<int>(), 60));   // value < 60
        //CountFailure);
    CPPUNIT_ASSERT(2 == NumberFailure);

    //ʹ�ú�������(�����Ĺ��캯�� "IsFailure()" ��ʱ�����һ����������),count_if()���Ը�������ÿ������������������
    //ע�⣺�����ú�������Ŀ�������� -- ����ʹ�ô��ܶ��Ա��������ͨ��
    NumberFailure = std::count_if(studentScroe.begin(),studentScroe.end(),IsFailure());
    CPPUNIT_ASSERT(2 == NumberFailure);


    //ʹ�ô��ݲ����ĺ������� -- ע������ һ�ι��캯��(��������ʱ)�� ���ο������캯�� (���ݸ� count_if ʱ �� �� count_if �ڲ�����ʱ)
    TRACE(_T("\nNow Count Class A:\n"));
    {
        IsClassX classA(_T('A'),80,101); //Notice: not 100
        std::list<int>::iterator::difference_type NumberOfClassA = count_if(studentScroe.begin(),studentScroe.end(),classA);
        CPPUNIT_ASSERT(4 == NumberOfClassA );
    }

    TRACE(_T("\nNow Count Class B:\n"));
    {
        IsClassX classB(_T('B'),60,80);
        std::list<int>::iterator::difference_type NumberOfClassB = count_if(studentScroe.begin(),studentScroe.end(),classB);
        CPPUNIT_ASSERT(2 == NumberOfClassB );
    }
}

void CSTLAlgorithmTester::test_equal()
{

}

void CSTLAlgorithmTester::test_fill()
{

}

void CSTLAlgorithmTester::test_find()
{
    std::list<std::string> FruitAndVegetables;
    FruitAndVegetables.push_back("carrot");
    FruitAndVegetables.push_back("pumpkin");
    FruitAndVegetables.push_back("potato");
    FruitAndVegetables.push_front("apple");
    FruitAndVegetables.push_front("pineapple");

    std::list<std::string>::iterator iter = std::find(FruitAndVegetables.begin(),FruitAndVegetables.end(),"apple");
    CPPUNIT_ASSERT(iter != FruitAndVegetables.end());
    CPPUNIT_ASSERT((*iter) == "apple");

    iter = std::find(FruitAndVegetables.begin(),FruitAndVegetables.end(),"orange");
    CPPUNIT_ASSERT(FruitAndVegetables.end() == iter);


    //��������ʹ��ָ����в���
    int intArray[10] = {0};
    for (int i = 0; i < _countof(intArray); i++)
    {
        intArray[i] = i;
    }
    //���� end()
    int *pPastTheEnd = intArray + _countof(intArray);

    int *pFound = std::find(intArray,pPastTheEnd , 5);
    CPPUNIT_ASSERT( pPastTheEnd != pFound);
    CPPUNIT_ASSERT( 5 == *pFound);

    //û���ҵ����������� end() -- ע�⣺���ܺ� NULL �Ƚ�
    int *pNotFound = std::find(intArray, pPastTheEnd, 100);
    CPPUNIT_ASSERT( pPastTheEnd == pNotFound);
}

void CSTLAlgorithmTester::test_find_if()
{
    std::list<int> studentScroe;
    studentScroe.push_back(100); 
    studentScroe.push_back(80);
    studentScroe.push_back(45);
    studentScroe.push_back(75);
    studentScroe.push_back(99); 
    studentScroe.push_back(100);
    studentScroe.push_back(59);
    studentScroe.push_back(60);

    int count = 0;
    std::list<int>::iterator iter = std::find_if(studentScroe.begin(),studentScroe.end(),IsClassX('A',80,101));
    CPPUNIT_ASSERT(studentScroe.end() != iter);
    CPPUNIT_ASSERT(*iter = 100);  //�����ҵ��ĵ�һ�����������ĵ�����λ��

    //ͨ�����Ĳ��ҵ���ʼλ�ã���������������������
    while (iter != studentScroe.end())
    {
        count++;
        iter = std::find_if( ++iter, studentScroe.end(), IsClassX('A', 80, 101));
    }

    //��4���ȼ�ΪA[80,100) ��
    CPPUNIT_ASSERT(4 == count );

    //û�з����� 40 �����µ�
    iter = find_if(studentScroe.begin(),studentScroe.end(),IsClassX('E',0,40));
    CPPUNIT_ASSERT(studentScroe.end() == iter) ;
}

void CSTLAlgorithmTester::test_find_end()
{

}

void CSTLAlgorithmTester::test_find_first_of()
{

}

void CSTLAlgorithmTester::test_search()
{
    std::vector <int> sourceVector; //Ҫ�ҵ�����
    std::vector <int> targetVector; //�����ҵ�������
    std::vector <int> nonTargetVector; //�Ҳ���������

    //sourceVecotor -- { 0 5 10 15 20 25 0 5 10 15 20 25 }
    int i;
    for ( i = 0 ; i <= 5 ; i++ )
    {
        sourceVector.push_back( 5 * i );
    }
    for ( i = 0 ; i <= 5 ; i++ )
    {
        sourceVector.push_back( 5 * i );
    }

    //targetVector -- { 20 25 }
    int ii;
    for ( ii = 4 ; ii <= 5 ; ii++ )
    {
        targetVector.push_back( 5 * ii );
    }

    //nonTargetVector -- { 20 30 40 }
    int iii;
    for ( iii = 2 ; iii <= 4 ; iii++ )
    {
        nonTargetVector.push_back( 10 * iii );
    }

    std::vector<int>::iterator iter;
    iter = std::search(sourceVector.begin(),sourceVector.end(),targetVector.begin(),targetVector.end());
    CPPUNIT_ASSERT(4 == iter - sourceVector.begin() ); //��һ���ҵ���λ���� 4

    iter = std::search(sourceVector.begin(),sourceVector.end(),nonTargetVector.begin(),nonTargetVector.end());
    CPPUNIT_ASSERT(sourceVector.end() == iter);


    //˵��ʹ�� operator == ���бȽ�
#if NEED_OPERATOR_FUNC
    std::vector<CMyTestData> sourceTestDataVector1;
    std::vector<CMyTestData> targetTestDataVector1;
    //���� CMyTestData �� operator == Ϊ˽�к�������˱�����޷�����ͨ��
    search(sourceTestDataVector1.begin(),sourceTestDataVector1.end(),targetTestDataVector1.begin(),targetTestDataVector1.end());
#endif

    std::vector<CMyTestData*> sourceTestDataVector2;
    std::vector<CMyTestData*> targetTestDataVector2;
    
    sourceTestDataVector2.push_back(new CMyTestData(1));
    sourceTestDataVector2.push_back(new CMyTestData(2));
    sourceTestDataVector2.push_back(new CMyTestData(3));
    
    targetTestDataVector2.push_back(new CMyTestData(2));
    targetTestDataVector2.push_back(new CMyTestData(3));

    //����vector�б�����ǵ�ַ��search��Ƚϵ�ַ�����Ա���ͨ�������޷���ȷ�ҵ� CMyTestData ��ֵ��ͬ������λ��
    std::vector<CMyTestData*>::iterator iterMyTestData = std::search(sourceTestDataVector2.begin(),sourceTestDataVector2.end(),targetTestDataVector2.begin(),targetTestDataVector2.end());
    //CPPUNIT_ASSERT(1 == iterMyTestData - sourceTestDataVector2.begin()); //������ʧ�ܣ��Ҳ���
    CPPUNIT_ASSERT(sourceTestDataVector2.end() == iterMyTestData); //��Ȼ������ͬ�������ڵ�ַ��ͬ������Ҳ���

    
    //����ڴ�
    for (iterMyTestData = sourceTestDataVector2.begin();iterMyTestData!= sourceTestDataVector2.end();
        ++iterMyTestData)
    {
        delete *iterMyTestData;
    }
    sourceTestDataVector2.clear();

    for (iterMyTestData = targetTestDataVector2.begin();iterMyTestData!= targetTestDataVector2.end();
        ++iterMyTestData)
    {
        delete *iterMyTestData;
    }
    targetTestDataVector2.clear();
}

void CSTLAlgorithmTester::test_set_difference()
{

}

void CSTLAlgorithmTester::test_set_intersection()
{

}

void CSTLAlgorithmTester::test_random_shuffle()
{
    std::vector <int> intVector;
    for ( int i = 1 ; i <= 9 ; i++ )
    {
        intVector.push_back( i );
    }

    std::random_shuffle(intVector.begin(), intVector.end());
    std::ostringstream os;
    std::copy(intVector.begin(), intVector.end(), std::ostream_iterator<int>(os, ","));
    const std::string& strInfo = os.str();
    TRACE("after random_shuffle: %s\n",strInfo.c_str());

    for (std::vector<int>::iterator iter = intVector.begin();
        iter != intVector.end();
        ++iter)
    {
        int iNumber = *iter;
        CPPUNIT_ASSERT(iNumber >= 1 && iNumber <= 9);
    }
    
    //����������ԭ���� 1-9 ˳��(���ֲ���)
    std::sort(intVector.begin(), intVector.end());
    for (int i = 1; i <= 9; i++)
    {
        CPPUNIT_ASSERT(i == intVector[i-1]);
    }
}

void CSTLAlgorithmTester::test_remove()
{
    std::vector<int> NumberArray;

    for (int count = 0; count < 2; count++)
    {
        for (int i = 0; i< 5; i++)
        {
            NumberArray.push_back(i);
        }
    }
    std::vector<int>::size_type oldSize = NumberArray.size();
    std::vector<int>::iterator iterRemoveLast;

    TRACE(_T("Just Insert the date\n"));
    //{ 0,1,2,3,4,0,1,2,3,4 }
    for_each(NumberArray.begin(),NumberArray.end(),PrintIntItem);

    CPPUNIT_ASSERT(4 == (*NumberArray.rbegin()));   //�������һ���� 4

    iterRemoveLast = std::remove(NumberArray.begin(),NumberArray.end(),4); //ɾ���� NumberArray �е����е� 4
    std::vector<int>::size_type removeCount = NumberArray.end() - iterRemoveLast;

    TRACE(_T("\nremove all 4\n"));

    //{ 0,1,2,3,0,1,2,3,3,4 } //�������� 3, 4 ��ӦΪ�Ƴ���2��4�Ժ󣬱��������ġ�
    for_each(NumberArray.begin(),NumberArray.end(),PrintIntItem);

    std::vector<int>::size_type newSize = NumberArray.size();
    
    CPPUNIT_ASSERT( oldSize == newSize );   //remove ���������������Ĵ�С
    CPPUNIT_ASSERT( 2 == removeCount );     //remove ��2��4
    CPPUNIT_ASSERT(3 == (*iterRemoveLast));   //remove���ص�λ�����Ƴ����ض����ݺ�������ݽ�β -- �˴�ָ�����һ�� 3
    CPPUNIT_ASSERT(4 == (*NumberArray.rbegin()));   //�������һ������4 (ͨ�õ� remove �㷨��ɾ�����Ƴ���)

    NumberArray.erase(iterRemoveLast,NumberArray.end());
    
    TRACE(_T("\nNow after NumberArray.erase\n"));
    for_each(NumberArray.begin(),NumberArray.end(),PrintIntItem);
    TRACE(_T("\n"));

    CPPUNIT_ASSERT(NumberArray.size() == oldSize - 2); //ɾ���� 2 ��

    //ע�� list::remove ����ɾ������ͬʱ�ı��С
    std::list<int> NumberList;
    for (int count = 0; count < 2; count++)
    {
        for (int i = 0; i< 5; i++)
        {
            NumberList.push_back(i);
        }
    }
    CPPUNIT_ASSERT(NumberList.size() == 10);
    NumberList.remove(4);       //����ֵΪ void, �����㷨 remove �����µ��߼��յ�
    CPPUNIT_ASSERT(NumberList.size() == 10 - 2); //list::removeʱͬʱ�����˴�С

    //remove�����б������ָ��ʱ����� -- Effective STL 33
#if 0
    void delAndNullifyUncertified(Widget*& pWidget)
    {
        if (!pWidget->isCertified()) {                  //���*pWidget��һ��δͨ������Widget��
            delete pWidget;				                // ɾ��ָ�벢��������Ϊ��
            pWidget = 0;
        }
    }
    for_each(v.begin(), v.end(),delAndNullifyUncertified);//������ָ��δͨ������Widget��ָ��ɾ����������Ϊ��
    // ��v�г�ȥ��ָ�룬0����ӳ�䵽һ��ָ�룬��C++������ȷ���Ƴ�remove�ĵ���������������
    v.erase(remove(v.begin(), v.end(),static_cast<Widget*>(0)),v.end());
#endif
}

void CSTLAlgorithmTester::test_remove_copy()
{

}

void CSTLAlgorithmTester::test_remove_copy_if()
{
    //��ʱ���������ڱ��治�ᱻɾ����

}

void CSTLAlgorithmTester::test_set_union()
{
    std::set<int>  intSet1, intSet2, intSetResult;
    std::generate_n( std::inserter(intSet1, intSet1.begin()), 5, sequence_generator<int>(1,3));  // 1,4,7,10,13

    std::generate_n( std::inserter(intSet2, intSet2.begin()), 5, sequence_generator<int>(2, 2));  // 2,4,6,8,10

    std::set_union(intSet1.begin(), intSet1.end(), intSet2.begin(), intSet2.end(), inserter(intSetResult, intSetResult.begin()));
    
    //�����ź���� set �����ݵļ���
    int checkResultArray[] = {1,2,4,6,7,8,10,13};
    STLTESTER_ASSERT_CONTAINER_ELEMENT_EQUAL(checkResultArray, ARRAY_PAST_THE_END(checkResultArray), 
        intSetResult.begin(), intSetResult.end());

}

void CSTLAlgorithmTester::test_stable_sort()
{
    
}

void CSTLAlgorithmTester::test_swap()
{

}

void CSTLAlgorithmTester::test_swap_range()
{

}

void CSTLAlgorithmTester::test_stable_partition()
{
    std::list<int> numbers;
    int i;
    for ( i = 0 ; i <= 10 ; i++ )
    {
        numbers.push_back( i );
        numbers.push_back( 10 - i);
    }
    TRACE(_T("\nԭʼ���ݣ�\n"));
    //{ 0,10,1,9,2,8,3,7,4,6,5,5,6,4,7,3,8,2,9,1,10,0 }
    for_each(numbers.begin(),numbers.end(),PrintIntItem);
    
    //TODO: ע��˴�ʹ���� binder1st �� less
    std::list<int>::iterator iter = std::stable_partition(numbers.begin(),numbers.end(),
        std::binder1st<std::less<int> >(std::less<int>(), 5));
    
    CPPUNIT_ASSERT(*iter <= 5);

    TRACE(_T("\nNow is the first part: greater than 5\n"));
    for_each(numbers.begin(),iter,PrintIntItem);

    TRACE(_T("\nnow is the second parts(less or equal than 5)\n"));
    for_each(iter,numbers.end(),PrintIntItem);

    TRACE(_T("\n"));
}

void CSTLAlgorithmTester::test_unique()
{
    std::list<int> myList;
    for (int i = 1; i< 10; i++)
    {
        myList.push_back(i);
    }
    myList.push_back(5);
    CPPUNIT_ASSERT(myList.size() == 10);

    myList.sort();
    myList.erase(std::unique(myList.begin(),myList.end()), myList.end());
    CPPUNIT_ASSERT(myList.size() == 9); //not ok
}

void CSTLAlgorithmTester::test_permutation()
{

}

void CSTLAlgorithmTester::test_inner_product()
{

}

void CSTLAlgorithmTester::test_inner_merge()
{

}

void CSTLAlgorithmTester::test_iter_swap()
{

}

void CSTLAlgorithmTester::test_lexicographical_compare()
{

}

void CSTLAlgorithmTester::test_nth_element()
{
    //nth_element(widgets.begin(),			// ����õ�20��Ԫ��
    //    widgets.begin() + 19,		// ����widgetsǰ�ˣ�
    //    widgets.end(),			// �����õ���
    //    qualityCompare);			// ���ǵ�˳��

}
void CSTLAlgorithmTester::test_partition()
{
    //vector<Widget>::iterator goodEnd =			// ����������hasAcceptableQuality
    //    partition(widgets.begin(),			// ��widgets�ƶ���widgetsǰ�ˣ�
    //    widgets.end(),		// ���ҷ���һ��ָ���һ��
    //    hasAcceptableQuality);	// �������widget�ĵ�����


    //vector<Widget>::size_type goalOffset =		// ָ����Ȥ��Widget
    //    0.25 * widgets.size();			// �뿪ʼ�ж�Զ
    //nth_element(begin, begin + goalOffset, end,		// �ҵ�����ֵΪ
    //    qualityCompare);		// 75%��Widget
    //...						// begin + goalOffset����ָ��
    //    // �����ȼ�Ϊ75%��Widget


}

void CSTLAlgorithmTester::test_partial_sort()
{
    //partial_sort(widgets.begin(),				// ����õ�20��Ԫ��
    //    widgets.begin() + 20,		// ����˳�򣩷���widgets��ǰ��
    //    widgets.end(),
    //    qualityCompare);

}

void CSTLAlgorithmTester::test_partial_sum()
{

}

void CSTLAlgorithmTester::test_sort()
{
    std::vector<int> intVector;
    for (int i = 1; i< 10; i+= 2)
    {
        intVector.push_back(i);
    }
    for (int i = 0; i< 10; i+=2)
    {
        intVector.push_back(i);
    }
    for_each(intVector.begin(),intVector.end(),PrintIntItem);
    CPPUNIT_ASSERT(*intVector.begin() == 1);  //����ĵ�һ����1
    CPPUNIT_ASSERT(*intVector.rbegin() == 8); //��������һ����8

    sort(intVector.begin(),intVector.end());  //ʹ��vector��Ĭ�ϵ� less ��������(����)
    CPPUNIT_ASSERT(*intVector.begin() == 0);  //��һ����0
    CPPUNIT_ASSERT(*intVector.rbegin() == 9); //���һ����9

    sort(intVector.begin(),intVector.end(),std::greater<int>());//ʹ�� greater �������򣨽���
    CPPUNIT_ASSERT(*intVector.begin() == 9);  //��һ����9
    CPPUNIT_ASSERT(*intVector.rbegin() == 0); //���һ����0

    sort(intVector.rbegin(),intVector.rend(),std::greater<int>());// ʹ�� greater �����ʼ���� (����)
    CPPUNIT_ASSERT(*intVector.begin() == 0);  //��һ����0
    CPPUNIT_ASSERT(*intVector.rbegin() == 9); //���һ����9

#if 0
    std::set<int> intSet;
    //������� -- set �� map �ǲ��ú�������Զ��ź����
    //sort(intSet.begin(), intSet.end(), std::less<int>());

    std::map<int,std::string> mapSet;
    sort(mapSet.begin(), mapSet.end(), std::less<int>());
#endif
}

void CSTLAlgorithmTester::test_unique_copy()
{

}

void CSTLAlgorithmTester::test_min_max_element()
{
    std::vector<int> intVector(10);
    //1 ~ 10
    std::generate(intVector.begin(), intVector.end(), sequence_generator<int>(1,1));
    
    //�������˳��
    std::random_shuffle(intVector.begin(), intVector.end());

    std::vector<int>::iterator iterMin = std::min_element(intVector.begin(), intVector.end());
    std::vector<int>::iterator iterMax = std::max_element(intVector.begin(), intVector.end());

    CPPUNIT_ASSERT(1 == *iterMin);
    CPPUNIT_ASSERT(10 == *iterMax);
}

void CSTLAlgorithmTester::test_mismatch()
{

}

void CSTLAlgorithmTester::test_transform()
{
    //�� V1 + V2 => V3
    //transform(V1.begin(), V1.end(), V2.begin(), V3.begin(),plus<double>());

    // ������Ԫ��������41����data������d��ǰ��
    //double data[numDoubles];
    //deque<double> d;	
    //transform(data, data + numDoubles,inserter(d, d.begin()),bind2nd(plus<double>(), 41));
    //transform(V1.begin(), V1.end(), V2.begin(), V3.begin(),plus<double>());

    DECLARE_MYTEST_DATA_COUNT_CHECKER(dataChecker,ctDataCount, 0, __FILE__, __LINE__);

    MyTestDataList  tmpTestDataList;
    
    std::transform(m_MyTestDataList.begin(), m_MyTestDataList.end(), 
        std::back_insert_iterator<MyTestDataList>(tmpTestDataList), duplicate_abs_CMytestData());

    //ת���Ժ������ͬ����ת����Ľ���� index ��Ϊ��
    CPPUNIT_ASSERT_EQUAL(m_MyTestDataList.size(), tmpTestDataList.size());

    std::for_each(tmpTestDataList.begin(), tmpTestDataList.end(), ObjecteDeleter<CMyTestData*>());
    tmpTestDataList.clear();

}

void CSTLAlgorithmTester::test_replace_if()
{
    std::vector<int> intVector1;

    intVector1.reserve(10);
    for (int i = 0; i< 10; i++)
    {
        intVector1.push_back(i);
    }
    CPPUNIT_ASSERT(intVector1[8] == 8);
    CPPUNIT_ASSERT(intVector1[9] == 9);

    //�滻����8������(9)Ϊ0
    replace_if(intVector1.begin(),intVector1.end(), bind2nd(std::greater<int>(), 8), 0);
    
    CPPUNIT_ASSERT(intVector1.size() == 10); //��������10
    for (std::vector<int>::iterator iter = intVector1.begin();iter != intVector1.end();++iter)
    {
        CPPUNIT_ASSERT( *iter <= 8 );   //Ŀǰû�д���8��Ԫ����
    }
    CPPUNIT_ASSERT(intVector1[8] == 8);
    CPPUNIT_ASSERT(intVector1[9] == 0);
}

void CSTLAlgorithmTester::test_reverse()
{
    //����10�������
    std::vector<int>    intVector(10);
    std::generate(intVector.begin(), intVector.end(), rand );

    //����ԭ��������
    std::vector<int>    intBackupVector(intVector.begin(), intVector.end());
    
    //���������е���
    std::reverse(intVector.begin(), intVector.end());

    std::vector<int>::iterator iterIntVector = intVector.begin();
    std::vector<int>::reverse_iterator iterBackVector = intBackupVector.rbegin();
    for (; iterIntVector != intVector.end() 
        && iterBackVector != intBackupVector.rend();
        ++iterIntVector, ++iterBackVector)
    {
        CPPUNIT_ASSERT(*iterIntVector == *iterBackVector);
    }
}

void CSTLAlgorithmTester::test_rotate()
{

}

void CSTLAlgorithmTester::test_equal_range()
{

}

void CSTLAlgorithmTester::test_lower_bound()
{

}

void CSTLAlgorithmTester::test_upper_bound()
{
    //ʹ�� upper_bound ������ list �б���˳������µ�����

    //list<CMyTestData> lp;
    //CMyTestData newData;
    //// ��lp������newPerson֮ǰ���ߵȼ۵����һ������������newPerson
    //lp.insert(upper_bound(lp.begin(),lp.end(),newData),newData);

}

void CSTLAlgorithmTester::test_merge()
{

}

void CSTLAlgorithmTester::test_heap()
{

}