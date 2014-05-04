#include "StdAfx.h"
#include "STLNumericTester.h"

#include <numeric>
#include <list>
#include <set>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//ͳ��ʱʹ�õĺ���������Ŀǰ�ĺ����µ��ַ��������ҷ��ظ��µĺ�
std::string::size_type stringLengthSum(std::string::size_type sumSoFar, const std::string& s)
{
    return sumSoFar + s.size();
}

void CSTLNumericTester::test_accumulate()
{
    //ͳ�� list �� double ���ĺ�
    std::list<double> ld;
    for (int i = 1; i< 10; i++)
    {
        ld.push_back(i + 0.1*i);
    }
    //{ 1.1 + 2.2 + 3.3 + 4.4 + 5.5 + 6.6 + 7.7 + 8.8 + 9.9 }
    double sumDouble = std::accumulate(ld.begin(), ld.end(), 0.0);  //��ʼֵ�� 0.0 (double��)
    CPPUNIT_ASSERT_DOUBLES_EQUAL(sumDouble,49.5,0.1);


    //������ֵ����Ļ�, 1.1 * 2.2 * 3.3 * 4.4 * 5.5 * 6.6 * 7.7 * 8.8 * 9.9
    double multiDouble = std::accumulate(ld.begin(),ld.end(),1.0,
        std::multiplies<double>()); //ʹ�ñ�׼�� multiplies �º�����
    CPPUNIT_ASSERT_DOUBLES_EQUAL(multiDouble,855652.06 , 0.01); //855652.05811008
    
#if 0
    //accumulateֻ��Ҫ�������������������������ʹ��istream_iterator��istreambuf_iterator
    // ��ӡcin����Щint�ĺ�
    cout << "The sum of the ints on the standard input is"		
        << accumulate(istream_iterator<int>(cin),istream_iterator<int>(),0);
#endif

    //ʹ��accumulate�����������е��ַ����ĳ��Ⱥ�
    std::set<std::string> strList;
    strList.insert("fishjam ");
    strList.insert("study ");
    strList.insert("STL");
    
    std::string::size_type sumString = accumulate(strList.begin(),strList.end(),0,stringLengthSum);
    CPPUNIT_ASSERT(sumString == 17);// �ܳ���Ӧ����17


}