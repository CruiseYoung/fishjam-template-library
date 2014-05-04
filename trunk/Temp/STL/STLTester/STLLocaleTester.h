#pragma once

#include <cppunit/extensions/HelperMacros.h>

/************************************************************************
* C++�е�����������һ��std::locale���͵Ķ��󣬿��Խ��������������������������κζ���һ��
*   std::locale L = std::locale::classic(); //����һ����ʾͨ��������������ö���
* C++����������÷ֳɶ�����棨facet����ÿ�����洦��һ�����ʻ��Ĳ�ͬ���򣬺���std::use_facet���������ö�������ȡһ���ض��ķ���
*    std::use_facet<numpunct<char_type> >(loc:decimal_point);  -- �﷨��ȫ�������д�
*    local::global(local(" ", local::ctype)); -- ���ַ�������...
*    
* collate ����Ĵ������÷�װ�������ϸ�� 
* tolower�Ĳ����ͷ���ֵ������int�����������int��EOF������ֵ�����ܱ���Ϊһ��unsigned char��
* static_cast<unsigned char>(c1));// char c1;
* 
************************************************************************/
class CSTLLocaleTester : public CPPUNIT_NS::TestFixture
{
public:

    CPPUNIT_TEST_SUITE( CSTLLocaleTester );
    //CPPUNIT_TEST( testAddData );
    CPPUNIT_TEST_SUITE_END();

    //void testAddData();


    DECLARE_DEFAULT_TEST_CLASS(CSTLLocaleTester);
};
