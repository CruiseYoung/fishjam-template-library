#pragma once

#include <cppunit/extensions/HelperMacros.h>

/**********************************************************************************************
* Xml�������Ƕ� Expat(http://www.libexpat.org) ������İ�װ, ֧�ֹ�ҵ��׼SAX��DOM�ӿ�
* 
*   SAX -- �����˻����¼���XML���ӿڣ�����Ҫ������XML�ĵ�װ�ص��ڴ���
*   DOM -- ��Ӧ�ó����������XML�ĵ�, ʹ�����εĶ���㼶
**********************************************************************************************/

class CPocoXMLTester : public CPPUNIT_NS::TestFixture
{
public:
    CPPUNIT_TEST_SUITE( CPocoXMLTester );
    CPPUNIT_TEST( test_container_holder );
    CPPUNIT_TEST_SUITE_END();

    DECLARE_DEFAULT_TEST_CLASS(CPocoXMLTester);
private:
    void test_container_holder();
};
