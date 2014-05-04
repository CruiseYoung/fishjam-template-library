#pragma once

#include <cppunit/extensions/HelperMacros.h>

/************************************************************************
* list -- �ɽڵ���ɵ�˫������ÿ����������һ��Ԫ��, list�������ṩ˫��� iterator
* 
* ����Ƶ���ض������в����в����ɾ��ʱӦ����list(˫���б�), slist(�����б�)
* list�ĳ�Ա����
*   push_back()��һ������ŵ�һ��list�ĺ��棬
*   push_front()�Ѷ���ŵ�ǰ��  -- ���������������˳��
*   end() ����ָ�����������һ��λ�õ�iterator -- ����Чλ�õĺ����
*   sort() -- ����һ��list�������ַ�����ʹ��ͨ�õ� sort �㷨�� list �еĳ�Ա����sort -- �������
*     ͨ���㷨����Ϊ��Щ�ṩ�����ȡ����Ԫ�� ���������򣬶�����list����Ϊһ�����ӵ�����ʵ�ֵģ�
*     ����֧�ֶ��������Ԫ�������ȡ�����Ծ���Ҫһ������� sort()��Ա����������list
*   insert -- �Ѷ�����뵽list�е��κεط�,���Լ���һ������һ����������ɷݿ���������һ����Χ���ڵĶ���
*   pop_front()ɾ��list�еĵ�һ��Ԫ��
*   pop_back()ɾ�����һ��Ԫ��
*   erase()ɾ����һ��iteratorָ����Ԫ�ػ�ָ��һ����Χ��Ԫ�� -- �ɵ�����ָ��ɾ��
*   remove ɾ��Ԫ�� -- ����ָ����Ԫ������
*   splice ��Դlist��ɾ��Ԫ�أ����ŵ�Ŀ��list�� -- �������ԣ������ٵ����ƶ�Ԫ�صĿ�������(ֱ���ƹ�ȥ����֤��Ч��)
************************************************************************/

class CSTLListTester : public CPPUNIT_NS::TestFixture
{
public:

    CPPUNIT_TEST_SUITE( CSTLListTester );

    CPPUNIT_TEST( test_constructor);
    CPPUNIT_TEST( testMyTestDataList );
    CPPUNIT_TEST( test_sort );
    CPPUNIT_TEST( test_splice );
    CPPUNIT_TEST( test_unique );
    CPPUNIT_TEST( test_remove );
    CPPUNIT_TEST_SUITE_END();

    void test_constructor();
    void testMyTestDataList();
    void test_sort();

    //Ŀ��.splice( Ŀ�ĵĲ���λ��, Դ,[Դ����ʼ��Դ�Ľ���])
    void test_splice();

    //���ɾ���ڽ����ظ�ֵ -- �ı��С
    void test_unique();

    //���ɾ����remove��ֵ -- �ı��С
    void test_remove();

    DECLARE_DEFAULT_TEST_CLASS(CSTLListTester);
};
