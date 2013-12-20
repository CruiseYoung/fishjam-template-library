#pragma once

#include <cppunit/extensions/HelperMacros.h>

/**********************************************************************************************
��ƽ̨�޹ص��ļ���Ŀ¼�Ĳ����ࡣ  
a. File�࣬��ȡ�ļ��������Ϣ������Ȩ�ޣ���С�����ڣ��ļ���ɾ���������� 
b. Path �࣬ �ļ�Ŀ¼�Ľ�����Ŀ¼������  
c. DirectoryIterator�࣬����ָ��Ŀ¼�µ��ļ��б�  
d. TemporaryFile �࣬��ʱ�ļ�������ɾ����
e. 
**********************************************************************************************/

class CPocoFileTester : public CPPUNIT_NS::TestFixture
{
public:
    CPPUNIT_TEST_SUITE( CPocoFileTester );
    CPPUNIT_TEST( test_container_holder );
    CPPUNIT_TEST_SUITE_END();

    DECLARE_DEFAULT_TEST_CLASS(CPocoFileTester);
private:
    void test_container_holder();
};
