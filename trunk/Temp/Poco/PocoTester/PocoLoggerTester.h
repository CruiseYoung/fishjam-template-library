#pragma once

#include <cppunit/extensions/HelperMacros.h>

/**********************************************************************************************
* Poco
*   ::Logger -- ��־���,���Ҹ�����һ�����ڲ��� Message������Channel������������ͷ��ͨ�� setChannel ���ú�����������
*      m_logger(Poco::Logger::get("default"))
*   ��־�����������:
*     Channel -- ����(FileChannel,ConsoleChannel,StreamChannel, AsyncChannel,NullChannel,FormattingChannel ��)
*       setProperty("path", path); 
*       setChannel(Channel* pChannel) -- ���ú�������Channel(������ʽģʽ?)
*     Formater -- ����(PatternFormatter)
*       setProperty("times", "local");
*       setProperty("pattern", "%Y-%m-%d %H:%M:%S [%p] %T %t %U(%u)");
*     Message --
*   ���̣��̳���Channel������ʵ��,ͨ��Formaterʵ����Message���и�ʽ����Ȼ�󴫵ݵ�Ŀ�ĵء�
*   
**********************************************************************************************/

class CPocoLoggerTester : public CPPUNIT_NS::TestFixture
{
public:
    CPPUNIT_TEST_SUITE( CPocoLoggerTester );
    CPPUNIT_TEST( test_container_holder );
    CPPUNIT_TEST_SUITE_END();

    DECLARE_DEFAULT_TEST_CLASS(CPocoLoggerTester);
private:
    void test_container_holder();
};
