#pragma once

#include <cppunit/extensions/HelperMacros.h>

/**********************************************************************************************
* Poco
*   ::Net -- 
*     ::NetCore -- Ϊ��������Ӧ���ṩ���Ĺ������,���� network address,DNS ��
*     ::NetSSL -- NetSSLΪNet���е��������ṩSSL֧��
*     ::Socket 
*     ::Reactor 
*
* ����socket ���ṩ������ܴ���TCP������:
*   1.���̷߳�����(ÿ������һ���߳�, ���̳߳ػ��)
*   2.����Acceptor-Reactorģʽ�ķ�����
**********************************************************************************************/

class CPocoNetTester : public CPPUNIT_NS::TestFixture
{
public:
    CPPUNIT_TEST_SUITE( CPocoNetTester );
    CPPUNIT_TEST( test_container_holder );
    CPPUNIT_TEST_SUITE_END();

    DECLARE_DEFAULT_TEST_CLASS(CPocoNetTester);
private:
    void test_container_holder();
};
