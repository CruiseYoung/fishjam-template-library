#pragma once

#include <cppunit/extensions/HelperMacros.h>

/**********************************************************************************************
* Data���ṩ���ʲ�ͬSQL���ݿ��һ���Խӿ�
**********************************************************************************************/

class CPocoDataTester : public CPPUNIT_NS::TestFixture
{
public:
    CPPUNIT_TEST_SUITE( CPocoDataTester );
    CPPUNIT_TEST( test_container_holder );
    CPPUNIT_TEST_SUITE_END();

    DECLARE_DEFAULT_TEST_CLASS(CPocoDataTester);
private:
    void test_container_holder();
};
