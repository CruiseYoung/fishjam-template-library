#pragma once

#include <cppunit/extensions/HelperMacros.h>

/*******************************************************************************************************
* set -- �ɽڵ���ɵĺ������ÿ���ڵ㶼������һ��Ԫ�أ��ڵ�֮����ĳ��������Ԫ�ضԵ�ν�����У�
*        û��������ͬ��Ԫ���ܹ�ӵ����ͬ�Ĵ���
*        set����Key��������洢 -- ��ö��ʱĬ�ϻ��� less<key> �ķ�ʽ����������Զ������������(������ 
*          operator < ���� -- �����е��ڣ������� find �Ȳ���ʱ���׳��쳣��
*          ����� test_set_find �� less_CMyTestData_Pointer ��ʵ�ָ�Ϊ <= ���ɷ����쳣)
* multiset -- �����������������ȵ�Ԫ�صļ���
*******************************************************************************************************/
class CSTLSetTester : public CPPUNIT_NS::TestFixture
{
public:

    CPPUNIT_TEST_SUITE( CSTLSetTester );
    CPPUNIT_TEST( test_set_order );
    CPPUNIT_TEST( test_set_find );
    CPPUNIT_TEST( test_set_insert );
    CPPUNIT_TEST_SUITE_END();

    //����set����˳��洢
    void test_set_order();
    
    //�����Զ�����������ݺͲ���
    void test_set_find();

    //���Բ�����ͬ������
    void test_set_insert();
    DECLARE_DEFAULT_TEST_CLASS(CSTLSetTester);
};
