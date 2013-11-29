#pragma once

#include <cppunit/extensions/HelperMacros.h>
#include "Poco/Types.h"     //�������ݽṹ��ͳһ��������ͬƽ̨int��long�����ݽṹ�Ķ���


/**********************************************************************************************
* �������Ŀ⣬�����ײ�ƽ̨�ķ�װ�㣬Ҳ��������ʹ�õĹ�����ͺ���
*   
* ��Ҫ����:
*   Core -- ������ƽ̨��Ļ���ͷ�ļ�����װ��ԭ�Ӽ����Ļ���(AtomicCounter), �����ռ�(AutoPtr��SharedPtr)
*   Cache -- �ڴ�Cache
*   Crypt -- ����ժҪ
*   Filesystem -- �ļ�ϵͳ����Ҫ�Ƕ��ļ�����Ĳ��������ƶ��������ļ���
*   Hashing -- Hash��
*   Logging -- ��־ϵͳ
*   Notifications -- ֪ͨ
*   Processes -- ����ͨѶ
*   RegularExpression -- ������ʽ��������PCRE��.(http://www.pcre.org)
*   SharedLibrary -- ����ʱ��̬����/ж�ع���⣬������ƽ̨�޹صķ�ʽ��дӦ�ó�������
*   Streams -- ��
*   Tasks -- ����
*   Text -- �ı�ת��
*   Threading -- ���߳�
*   URI -- 
*   UUID -- UUID���ɺͲ���
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
*
a.Timestamp ΢�뼶���ȣ���gettimeofday()��װ,�ο�Timestamp.h   
b.Stopwatch ��ʱ��,�ο�Stopwatch.h   
c.DateTime  ����ʱ��,�ο�DateTime.h    
d.����ʱ��ĸ�ʽת��DateTimeFormat,DateTimeFormatter,DateTimeParser��   
e.����ʱ��Timezone   
f.����ʱ��LocalDateTime

**********************************************************************************************/


/**********************************************************************************************
* ��������� -- TODO: ͬһ���ļ��е������ Plugin ������һ��Ŀ¼���ж���ļ������Ե����Լ���Plugin?
*   1.���� class AbstractPlugin { virtual SomeFunc() = 0; }
*   2.ʵ�ֶ������Ĳ���� class PluginA: public AbstractPlugin { ... }
*   3.ʹ��PocoԤ����ĺ���е���(����ʱʵ�ֶ�̬)
*     POCO_BEGIN_MANIFEST(AbstractPlugin)
*        POCO_EXPORT_CLASS(PluginA)
*     POCO_END_MANIFEST
*   4.ʵ�ֳ�ʼ�����������(��ѡ) -- ���̶ֹ�?
*     void pocoInitializeLibrary() { ... } �� void pocoUninitializeLibrary() { ... } 
**********************************************************************************************/


class CPocoFoundationTester : public CPPUNIT_NS::TestFixture
{
public:
    CPPUNIT_TEST_SUITE( CPocoFoundationTester );
    CPPUNIT_TEST( test_DateTime );
    CPPUNIT_TEST( test_PluginLoader );
    CPPUNIT_TEST( test_SharedLibrary );
    CPPUNIT_TEST_SUITE_END();

    DECLARE_DEFAULT_TEST_CLASS(CPocoFoundationTester);
private:
    void test_DateTime();
    void test_PluginLoader();
    void test_SharedLibrary();


    void test_DateTimeFormat();
    void test_Stopwatch();
};
