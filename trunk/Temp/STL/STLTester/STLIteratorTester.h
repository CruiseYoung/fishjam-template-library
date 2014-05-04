#pragma once

#include <cppunit/extensions/HelperMacros.h>

/************************************************************************
* ������������ -- ���Խ���������Ϊ�����εļ̳й�ϵ������ļ̳�������
*   ���������(Input Iterator) <== ֻ��
*   ���������(Output Iterator) <== ֻд��ͨ�����ڽ����ݴ�һ��λ�ÿ�������һ��λ��
*   ǰ�������(Forward Iterator) <== ++ (��ǰ�ƽ�������),��д����
*   ˫�������(Bidirectional Interator) <== ++��--(����ǰ��������)
*   �����ȡ������() <== +n��-n��[]
*   
* ʹ�ø������͵ĵ����� -- <iterator>( ע�⣺Ψ�����壬��һ������)����
* //Sample ���룺 ���������������֣���������������
*   typedef vector<int> int_vector;
*   typedef istream_iterator<int> istream_itr;
*   typedef ostream_iterator<int> ostream_itr;
*   typedef back_insert_iterator< int_vector > back_ins_itr;
*  
*   // �ӱ�׼�����豸����������ֱ��������Ƿ���������Ϊֹ
*   // istream_itr��������(adaptor)���� cin ���а�װ����������������ʱ���� -- �㷨ֻ�ϵ�����
*   // ע�⣺����д�� copy(istream_iterator<int>(cin), istream_iterator<int>(), num.begin()); 
*   //   ԭ���ǣ�����Խ�� -- ��ʱ copy �����Զ�������������Ҫʹ�������� back_insert_iterator
*   copy(istream_itr(cin), istream_itr(), back_ins_itr(num));  //���� int_vector num;
*   
*   // ����
*   sort(num.begin(), num.end());
*   
*   // ���������������׼����豸
*   // ostream_iterator(cout, "\n") -- ����һ����������������ĵ�����������λ��ָ������������ʼ����������"\n"��Ϊ�ָ��
*   copy(num.begin(), num.end(), ostream_itr(cout, "\n"));
*    
* ������ʧЧ
*   ��ʹ��һ��������insert����erase����ͨ�������������ɾ��Ԫ�ء����ܡ��ᵼ�µ�����ʧЧ(win�¶��ԣ�linux�²�ȷ��)
*   һ����Ҫ��ȡinsert����erase���صĵ��������Ա������»�ȡ�µ���Ч�ĵ�����������ȷ�Ĳ���
************************************************************************/

class CSTLIteratorTester : public CPPUNIT_NS::TestFixture
{
public:
    CPPUNIT_TEST_SUITE( CSTLIteratorTester );
    CPPUNIT_TEST( test_deque_loopdelete );
    CPPUNIT_TEST( test_vector_loopdelete );
    CPPUNIT_TEST( test_list_loopdelete );
    CPPUNIT_TEST( test_set_loopdelete );
    CPPUNIT_TEST( test_map_loopdelete );

    CPPUNIT_TEST( test_bad_loopdelete );
    CPPUNIT_TEST( test_better_loopdelete );

    CPPUNIT_TEST( test_ostream_iterator );
    CPPUNIT_TEST( test_front_inserter );
    CPPUNIT_TEST_SUITE_END();

    //insert/erase��������ĸ����м��ַ�ʽ��
    //  1.˳��ʽ������ʹ�� insert/erase �ķ���ֵ���µ����� -- iter = intCon.erase(iter)
    //    Win�����е�����������ʹ�ø÷�ʽ����Linux��vector/deque/list�ſ���
    //  2.�������������õ��������� -- intCon.erase(iter++);
    //    set/map  ���� ʹ��
    //
    //ע�⣺һ�㲻ʹ��forѭ�� -- ÿ�ζ������++����ɾ������ֽ���++�Ļ�������һ��Ԫ��
    //for (IntContainer::iterator iter = intCon.begin();
    //    iter != intCon.end(); ++iter)

    void test_deque_loopdelete();
    void test_vector_loopdelete();
    void test_list_loopdelete();
    void test_set_loopdelete();
    void test_map_loopdelete();

    void test_bad_loopdelete();
    //���õ�ɾ������
    void test_better_loopdelete();

    void test_ostream_iterator();

    // front_inserter -- ��������뵽���ݼ���ǰ��
    // back_inserter -- ��������뵽���ϵ�β��
    // inserter -- ���뵽�����κζ����ǰ�棬������ remove_copy_if
    // ע�⣺ʹ�ò�����������ܵ��������е����������ƶ�λ�ã����ʹ���ִ�ĵ������Ƿ� -- ���뵽vector�н����¶�̬��չ
    //      һ����˵�����뵽����(list)���͵Ľṹ�и�Ϊ��Ч
    void test_front_inserter();
    DECLARE_DEFAULT_TEST_CLASS(CSTLIteratorTester);

    typedef int TestType;
};
