#pragma once

#include <cppunit/extensions/HelperMacros.h>

/******************************************************************************************************************
* vector -- �����洢��Ԫ��(���ڴ����������������ʱ���ܻ�����ڴ濽�������⣬�����Ժ����黥����
* vector<bool> -- α����?
******************************************************************************************************************/

class CSTLVectorTester : public CPPUNIT_NS::TestFixture
{
public:

    CPPUNIT_TEST_SUITE( CSTLVectorTester );
    
    CPPUNIT_TEST( test_assign );
    CPPUNIT_TEST( test_reserve );
    
    CPPUNIT_TEST( test_vector_memory );
    CPPUNIT_TEST_SUITE_END();

    void test_assign();
    void test_reserve();

    //����Vector���ڴ����
    void test_vector_memory();

    DECLARE_DEFAULT_TEST_CLASS(CSTLVectorTester);
};
