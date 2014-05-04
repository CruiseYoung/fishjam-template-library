#pragma once

#include <cppunit/extensions/HelperMacros.h>

/************************************************************************
* STL ��û�а�����׼��ɢ��(Hash)������������STL��ɢ�й����������ԴӶ����Դ���
*   hash_set��hash_multiset��hash_map��hash_multimap -- �� stdext �����ƿռ���
* ע�⣺��ͬ��ʵ���в�ͬ�Ľӿڡ����������ݽṹ��Ч�ʡ�
*
* �Զ��� hash ����(�� string )
* struct str_hash: public unary_function<string,bool> {
*    size_t operator()(const string& str) const
*    {return ...}  //MSԤ����� hash ������ <xhash>
*  }
************************************************************************/
class CSTLHashContainerTester : public CPPUNIT_NS::TestFixture
{
public:

    CPPUNIT_TEST_SUITE( CSTLHashContainerTester );
    CPPUNIT_TEST( test_hash_map );
    CPPUNIT_TEST( test_hash_multimap );
    CPPUNIT_TEST( test_has_multiset );
    CPPUNIT_TEST( test_hash_set );
    CPPUNIT_TEST_SUITE_END();

    
    /***********************************************
     * @brief 
     *   hash_map����hash table����ϣ���������ݵĴ洢�Ͳ������ĵ�ʱ���󽵵ͣ��������Կ����ǳ���ʱ�䣻
     *   �����۽��������ıȽ϶���ڴ� -- �ռ任ʱ��
     * ����ԭ��ʹ��һ���±귶Χ�Ƚϴ���������洢Ԫ�ء��������һ����������ϣ������Ҳ����ɢ�к�������
     *   ʹ��ÿ��Ԫ�صĹؼ��ֶ���һ������ֵ���������±꣬hashֵ�����Ӧ��������������鵥Ԫ���洢���Ԫ�� -- "ֱ�Ӷ�ַ"��
     * ���ܹ���֤ÿ��Ԫ�صĹؼ����뺯��ֵ��һһ��Ӧ�ģ���ͬ��Ԫ�ؿ��ܼ������ͬ�ĺ���ֵ -- "�����ͻ"��
     * Ҫʵ�ֹ�ϣ��, ���û���ص��ǣ�hash���� �� �ȽϺ��� -- ��ʹ��hash_mapʱָ���Ĳ���
     ***********************************************/
    
    
    //
    void test_hash_map();

    void test_hash_multimap();

    void test_has_multiset();

    void test_hash_set();

    
    DECLARE_DEFAULT_TEST_CLASS(CSTLHashContainerTester);
};
