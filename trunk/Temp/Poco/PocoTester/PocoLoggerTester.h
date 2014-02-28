#pragma once

#include <cppunit/extensions/HelperMacros.h>
#include "Poco/Logger.h"

/**********************************************************************************************
* ::Logger -- ��־���,���Ҹ�����һ�����ڲ��� Message������Channel������������ͷ��ͨ�� setChannel ���ú�����������
*    get() -- POCO�����ڲ�������һ��ȫ�ֵ���־map,�û���ͨ��get��ȡ����ָ�����ֹ�����logger��������
*      m_logger(Poco::Logger::get("default")) -- ����һ����ϢԴ����Ϊ "default" ��Logger
*    setLevel -- ? �������ù�����Ϣ�����ȼ���ֻ����Ϣ�����ȼ������õĸ߲Żᱻ����
* 
* ��־�����������: 
*   �������ֿ���ͨ�� setProperty() �������ü�ֵ�Ը�ʽ�����Կ��ƶ��������������õ����Կ��Բμ�ͷ�ļ�
*   Channel -- ͨ�����󣬸��𴫵���Ϣ�������հ���Ϣ�͵�Ŀ�ĵأ�ͨ�����ü���������������
*     setChannel(Channel* pChannel) -- ���ú�������Channel(������ʽģʽ?)
*     +-AsyncChannel -- ����������һ��������߳���ȥ��¼ͨ������־��ͨ��һ���Ƚ��ȳ�����Ϣ���н�����ת��
*     +-ConsoleChannel -- ���������Ŀ���̨���,ֻ�Ǽ򵥵İ���Ϣ����д���˱�׼�����(std::clog)
*     +-EventLogChannel -- �¼���־ͨ����д��Windows�¼���־�С�ʹ�úͲ鿴ʱ������ PocoFoundation.dll �ļ���
*     +-FileChannel -- �ṩ����������־֧�֣�֧�ְ��ļ���С��ʱ��������־����ѭ�����ǣ�֧���Զ��鵵(ʹ�ò�ͬ���ļ���������),
*         ֧��ѹ��(GZIP)�����(�����ѹ鵵�ļ������ڻ�����)�鵵��־�ļ���
*     +-FormattingChannel -- ��������ܵ���ÿһ����Ϣͨ�� Formatter ���ݸ���һ�������ͨ��
*     +-NullChannel -- �������з���������Ϣ�����Һ�������setProperty()�������õ�����
*     +-RemoteSyslogChannel -- ����ͨ������UDP��ϵͳ��־Э��(Syslog protoco)����־�����Զ�̵�Linux������־�ػ�������
*     +-SimpleFileChannel -- ����־�ļ�����ļ򵥹��ܡ�����ÿһ����Ϣ�������ݶ��ᱻ��ӵ��ļ��У���ʹ��һ�����������
*         ֧������(secondaryPath)��־�ļ���ѭ��(rotation="10M")
*     +-SplitterChannel -- ���԰���Ϣ���͸�һ�����߶��(ͨ�� addChannel ����)������ͨ�����������־�ڶ��Ŀ����
*     +-StreamChannel
*     +-SyslogChannel -- ����־�����Linuxϵͳ�ı���ϵͳ��־�ػ�����
*     +-WindowsConsoleChannel -- ��windows����̨���,֧��UTF-8����
*   Formater -- ��Ϣ���ʱ����Ϣ���и�ʽ��
*     +-PatternFormatter -- ���Ը��ݴ�ӡ��ʽ����ʽ����Ϣ
*   Message -- ���е���Ϣ�����洢��ͨ����Poco::Message����,���а��� ���ȼ�����ϢԴ�����ݡ�ʱ������̱߳�ǡ���ֵ�Ը�ʽ����Ϣ������
* ���̣��̳���Channel������ʵ��,ͨ��Formaterʵ����Message���и�ʽ����Ȼ�󴫵ݵ�Ŀ�ĵء�
* 
* LogStream -- �ṩ��һ����־��������ӿڡ���������־���У���ʽ�������־��¼��Ϣ����־��Ϣ������std::endl(��CR��LF�ַ�)��β
**********************************************************************************************/

/**********************************************************************************************
* ��־Ч��
*   1.��Ϣͨ���������ͨ�����õķ�ʽ���ݸ���һ��ͨ�������������ǣ�FormattingChannel��AsyncChannel�ࡣ���ǻ�������Ϣ��һ������
*   2.�����ܵı���Ƶ���ĵ���Logger::get()���������õķ�������ͨ�����������־(logger)�������ú󣬱��沢ʹ��
*   3.��¼�������־��Ч��ȡ������־�����ͨ����ͨ����Ч�ʷǳ������ڲ���ϵͳ��ʵ�֡�
**********************************************************************************************/

class CPocoLoggerTester : public CPPUNIT_NS::TestFixture
{
public:
    CPocoLoggerTester();
    virtual ~CPocoLoggerTester();
    virtual void setUp();
    virtual void tearDown();

public:
    CPPUNIT_TEST_SUITE( CPocoLoggerTester );
    CPPUNIT_TEST( test_AsyncLogger );
    CPPUNIT_TEST( test_Logger );
    CPPUNIT_TEST( test_SplitterLogger );
    CPPUNIT_TEST( test_LogStream );
    CPPUNIT_TEST_SUITE_END();

    //DECLARE_DEFAULT_TEST_CLASS(CPocoLoggerTester);
    DISABLE_COPY_AND_ASSIGNMENT(CPocoLoggerTester);
private:
    void test_AsyncLogger();
    void test_Logger();
    void test_SplitterLogger();
    void test_LogStream();
    Poco::Logger& m_logger;
};
