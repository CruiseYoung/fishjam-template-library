#pragma once
#include <cppunit/extensions/HelperMacros.h>

/**********************************************************************************************
* ����Cpp������ʹ��ʱ��һЩע������
*   C����Ϊ֧��printf�ȱ�κ�������Ҫ����������ջ�����ɵ����߳�ջ�������Ҫ���� add esp N ������ esp -- ���÷���֪�����˶��ٲ���
*      �ɱ䳤����(...)��ȱ���ǣ�ȱ�����Ͱ�ȫ�ԣ���������δ֪�ȡ��ɿ���ʹ�� boost::format �滻
*   C++(Pascal)���б����ú�����ջ(ret)����ʡ�� add esp N �ĵ��ã�ʱ�䡢�ռ䶼��ʡ�ˣ������޷�ʵ�ֿɱ�κ����ĵ���
*   
* ˽�м̳���ζ����XX��ʵ�֣��������д��ڵĴ��룩��Ӧ�����ܻ�����ϣ�����Ҫ��д�麯�������ʱ�����Ա�ȣ�
* Ҫ���ö�̬�ԣ���Ҫͨ��ָ������õķ�ʽ����ֵ��ʽ�޷���ö�̬ -- �и����⡣
*   ͨ��������Ŀ������캯����Ϊ������������һ���������clone����(virtual MyClass* clone() const = 0 {return new MyClass(*this);}
* ���ǰ���Ա����������˳���д��Ա��ʼ����䣬������Ҫ��һ����Ա�ĳ�ʼ��������������Ա���á�
* �����ڹ��졢������ֱ�ӡ���ӵ������⺯�� -- ʹ�ú��죺1˵��(�� Init);2����һ��Ա�������жϲ�����;3�ù�������
*
* POD(Plain Old Data) -- ��׼C++�����У�����һЩ�ض�����Ľṹ�彫����������C�Ķ��󲼾֣����԰���C�ķ�ʽ���������㡢�����ȣ���
*
* �������ò���
*   SetParameter(var_list argptr)
*   {
*      INT myVar = va_arg(argptr, INT); //ȡ�����β���;
*      BOOL bVar = va_arg(argptr, BOOL); //
*   }
*  
* XXX* operator[](int nIndex) -- ���ж�������±귵��ָ��Ԫ�صķ���
**********************************************************************************************/
#pragma TODO(va_arg test)

class CCppLanguageTester : public CPPUNIT_NS::TestFixture
{
public:
    CCppLanguageTester();
    ~CCppLanguageTester();

    CPPUNIT_TEST_SUITE( CCppLanguageTester );
    CPPUNIT_TEST( test_bad_struct_pointer_assign );
    CPPUNIT_TEST( test_construct_init_list );
    CPPUNIT_TEST( test_exception_lifecycle );
    CPPUNIT_TEST( test_good_struct_pointer_assign );
    CPPUNIT_TEST( test_good_struct_object_assign );
    CPPUNIT_TEST( test_cpp_alloc_fail );
    CPPUNIT_TEST( test_cpp_alloc_physical_memory );
    CPPUNIT_TEST( test_anonymous_temp_objcet_Life );
    CPPUNIT_TEST( test_static_var_life );
    CPPUNIT_TEST( test_enum );
    CPPUNIT_TEST_SUITE_END();

    void test_bad_struct_pointer_assign();
    
    //���Թ��캯����ʹ�ó�ʼ���б��ֱ�Ӹ�ֵ������
    void test_construct_init_list();

    //�����쳣���������� -- ���飺ͨ��ֵ�׳��쳣��ͨ���ǳ��������ò����쳣�������ֵ���񣬽��ڲ��񴦵����и����⣩
    //  δ������쳣����ʱ�������ε��� terminate->abort����ͨ�� set_unexpected �� set_terminate ���д���
    void test_exception_lifecycle();

    void test_good_struct_pointer_assign();

    void test_good_struct_object_assign();

    //VC2008 �²���ʧ�ܣ�����
    //C++ newʧ�ܺ�Ĭ����������׳� std::bad_alloc �쳣������ͨ�� new(std::nothrow)��ʹ�䷵��NULL
    //  gcc �е� -fno-exceptions ����ͳһ����ɲ��׳��쳣��ע�⣺������ɲ����׳������쳣��
    //  ע�⣺΢���VC�� new ʧ�ܲ����׳� std::bad_alloc �쳣��http://support.microsoft.com/default.aspx?scid=kb;EN-US;q167733
    void test_cpp_alloc_fail();

    // ����new�Ժ������ڴ�
    void test_cpp_alloc_physical_memory();

    //������ʽ��ʱ������������ڣ�
    //  VC������ɾ�����Ͷ���ֲ�������ͬ��������ɷ�Χ����Bug��
    //  FORTEC++���˳�������Χʱɾ�����Ͷ���ֲ�������ͬ��
    void test_anonymous_temp_objcet_Life();

    //���Ծ�̬������������
    void test_static_var_life();
    //DECLARE_DEFAULT_TEST_CLASS(CCppLanguageTester);

    void test_enum();
private:
    BOOL m_bCallGetMyTestDataInstance;
};