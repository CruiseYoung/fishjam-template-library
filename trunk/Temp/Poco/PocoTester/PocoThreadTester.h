#pragma once

#include <cppunit/extensions/HelperMacros.h>

/**********************************************************************************************
* ���̷߳�װ
*   Thread -- ���������߳�����
*     ʹ�þ�̬���� Thread::current() ���Ի�ȡ��ÿ���̶߳���Thread��ָ��(ʵ�ַ�ʽΪTLS)
*   ThreadPool(�̳߳�) -- ���ܲ�ǿ
*     1.�޷�������������
*     2.��ThreadPool����������,��û�п����߳�ʱ�����׳� NoThreadAvailableException �쳣�������Ƿ�����еȴ�
*  
*   thread-local �洢 -- Poco::ThreadLocal + ThreadLocalStorage + TLSSlot
*   ͬ��ԭ��(Poco::Mutex, Poco::ScopedLock, Poco::Event,  Poco::Semaphore, Poco::RWLock), 
*     Mutex��Semaphore��Event��Scopelock��ReadWriteLock
*   Active Object(��������) -- ʹ���Լ��߳������Լ��ĳ�Ա����,���Թ����̣߳�Poco֧������������Ա������
*      Activity -- ����ҵ����Ҫ����ֵ���޲����ĳ�Ա����ʱ
*      ActiveMethod + ActiveResult(�첽���ؽ��) -- ����ҵ����Ҫ����ֵ�Ͳ����ĳ�Ա����ʱ
*   Timer(��ʱ��) -- 
**********************************************************************************************/

class CPocoThreadTester : public CPPUNIT_NS::TestFixture
{
public:
    CPPUNIT_TEST_SUITE( CPocoThreadTester );
    CPPUNIT_TEST( test_Locker );
    CPPUNIT_TEST( test_Thread );
    CPPUNIT_TEST( test_ThreadPool );
    CPPUNIT_TEST( test_Timer );
    CPPUNIT_TEST( test_Activity );
    CPPUNIT_TEST( test_ActiveMethod );
    CPPUNIT_TEST_SUITE_END();

    DECLARE_DEFAULT_TEST_CLASS(CPocoThreadTester);
private:
    void test_Locker();
    void test_Thread();
    void test_ThreadPool();
    void test_Timer();
    void test_Activity();
    void test_ActiveMethod();
};
