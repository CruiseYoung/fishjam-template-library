#pragma once

#include <cppunit/extensions/HelperMacros.h>

/**********************************************************************************************
* �������Ŀ⣬�����ײ�ƽ̨�ķ�װ�㣬Ҳ��������ʹ�õĹ�����ͺ���
*
**********************************************************************************************/

/**********************************************************************************************
* ����ָ��(smart pointer)�͹���ָ��(shared pointer) -- AutoPtr.h,SharedPtr.h 
* ���ü��������������ջ��� AtomicCounter.h,refCountedObject.h
*
* BinaryReader/BinaryWriter -- ���������Ķ�д���Զ���͸���ش����ֽ�˳������
* 
* Logging -- ����չ����־��ܣ��ÿ��֧�ֹ��ˡ�·�ɵ���ͬ��ͨ���Լ���ʽ����־��Ϣ�ȡ�
*   ������������Ŀ�ĵ�(console,log files,Unix Syslog,Windows Event Log,remote logging)
*
* NotificationCenter/NotificationQueue -- �첽֪ͨ?
*   source.theEvent += Delegate<Target, int>(&target,&Target::onEvent);
*
* SharedLibrary -- ����ʱװ�ؼ�ж�صĹ���⣬������ƽ̨�޹صķ�ʽ��дӦ�ó�������
*
a.Timestamp ΢�뼶���ȣ���gettimeofday()��װ,�ο�Timestamp.h   
b.Stopwatch ��ʱ��,�ο�Stopwatch.h   
c.DateTime  ����ʱ��,�ο�DateTime.h    
d.����ʱ��ĸ�ʽת��DateTimeFormat,DateTimeFormatter,DateTimeParser��   
e.����ʱ��Timezone   
f.����ʱ��LocalDateTime



**********************************************************************************************/

class CPocoFoundationTester : public CPPUNIT_NS::TestFixture
{
public:
    CPPUNIT_TEST_SUITE( CPocoFoundationTester );
    CPPUNIT_TEST( test_DateTime );
    CPPUNIT_TEST_SUITE_END();

    DECLARE_DEFAULT_TEST_CLASS(CPocoFoundationTester);
private:
    void test_DateTime();
    void test_DateTimeFormat();
    void test_Stopwatch();
};
