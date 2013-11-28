#pragma once

#include <cppunit/extensions/HelperMacros.h>

/**********************************************************************************************
* ���̷߳�װ
*   Poco::Thread
*   ͬ��ԭ��(Poco::Mutex, Poco::ScopedLock, Poco::Event,  Poco::Semaphore, Poco::RWLock), 
*   Poco::ThreadPool �༰֧��thread-local �洢, 
*   �߼���Ļ����(active object) -- ӵ�������������߳��еķ�������
*      ActiveMethod + ActiveResult
**********************************************************************************************/

class CPocoThreadTester : public CPPUNIT_NS::TestFixture
{
public:
    CPPUNIT_TEST_SUITE( CPocoThreadTester );
    CPPUNIT_TEST( test_container_holder );
    CPPUNIT_TEST_SUITE_END();

    DECLARE_DEFAULT_TEST_CLASS(CPocoThreadTester);
private:
    void test_container_holder();
};
