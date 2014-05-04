#pragma once

#include <cppunit/extensions/HelperMacros.h>

/**********************************************************************************************************
* ��������������������м���ѧ�����ģ�庯���������ӷ��ͳ˷��������ϵ�һЩ����
**********************************************************************************************************/
class CSTLNumericTester : public CPPUNIT_NS::TestFixture
{
public:

    CPPUNIT_TEST_SUITE( CSTLNumericTester );
    CPPUNIT_TEST( test_accumulate);

    CPPUNIT_TEST_SUITE_END();

    //����ͳ�ƣ�ͳ��ָ�����䣨�� �� ָ�������Ĳ��������������ֵ���ܺ� -- Ҳ����ʹ�� for_each �Լ�ͳ��
    //  ע�⣺����accumulate�ĺ����в����и�����
    //        ������ֵ��һ���Ǽ򵥵����ͣ�ͨ������operator+()��Ҳ�����������
    void test_accumulate();

    void test_adjacent_difference();
    void test_inner_product();
    void test_partial_sum();

    DECLARE_DEFAULT_TEST_CLASS(CSTLNumericTester);
};
