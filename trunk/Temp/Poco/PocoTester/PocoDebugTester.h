#pragma once

#include <cppunit/extensions/HelperMacros.h>

/**********************************************************************************************
* Poco �����쳣�ķ�ʽ������ҵ���߼�
**********************************************************************************************/
#include "Poco/Bugcheck.h"      //���԰���

class CPocoDebugTester : public CPPUNIT_NS::TestFixture
{
public:
    CPPUNIT_TEST_SUITE( CPocoDebugTester );
    CPPUNIT_TEST( test_container_holder );
    CPPUNIT_TEST_SUITE_END();

    DECLARE_DEFAULT_TEST_CLASS(CPocoDebugTester);
private:
    void test_container_holder();
};
