#pragma once

#include <cppunit/extensions/HelperMacros.h>

/**********************************************************************************************
* �ṩ��һ�����ʸ��������ļ���ʽ�Ľӿڣ�����֧�ֶ��ָ�ʽ
*   Java-style��property�ļ� -- PropertyFileConfiguration.h
*   windows����.ini�ļ� -- IniFileConfiguration.h
*   XML -- XMLConfiguration.h
*
* Command Line Options --  ������ƽ̨�޹ص������в���
*   options.addOption(
*      Option("������", "����", "������Ϣ").required(false).repeatable(false)
*         .argument("value").binding("�󶨵�����������"));
**********************************************************************************************/

class CPocoConfigurationTester : public CPPUNIT_NS::TestFixture
{
public:
    CPPUNIT_TEST_SUITE( CPocoConfigurationTester );
    CPPUNIT_TEST( test_container_holder );
    CPPUNIT_TEST_SUITE_END();

    DECLARE_DEFAULT_TEST_CLASS(CPocoConfigurationTester);
private:
    void test_container_holder();
};
