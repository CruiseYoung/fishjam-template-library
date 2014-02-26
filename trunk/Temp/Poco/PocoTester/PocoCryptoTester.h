#pragma once

#include <cppunit/extensions/HelperMacros.h>

/**********************************************************************************************
* Crypto
*   initializeCrypto()/uninitializeCrypto() -- ʹ��ǰ��ĳ�ʼ������ֹ��
**********************************************************************************************/
#include "Poco/Crypto/Crypto.h"

class CPocoCryptoTester : public CPPUNIT_NS::TestFixture
{
public:
    CPPUNIT_TEST_SUITE( CPocoCryptoTester );
    CPPUNIT_TEST( test_container_holder );
    CPPUNIT_TEST_SUITE_END();

    DECLARE_DEFAULT_TEST_CLASS(CPocoCryptoTester);
private:
    void test_container_holder();
};
