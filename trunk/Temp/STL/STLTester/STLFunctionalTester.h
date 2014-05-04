#pragma once

#include <cppunit/extensions/HelperMacros.h>

/*************************************************************************************************************
* ��׼�º���������less<T>���ͷº���������������not1��bind2nd��
*   �º���(��������)����һ��������"()"�����������struct��class���������չ���㷨�Ĺ��ܡ�
*   �º���(ͨ��)�ĵ��÷�ʽ�����㷨������һ�������Ķ��� ( �� less<int>() )
* 
* Ϊ������޶ȵ�ʹ�÷º����������(�º����ܹ��������º���������һ��ʵ���µĹ���)��
*    ��ô� unary_functon �� binary_function �̳У��μ���Effective STL��40��
*    std::unary_function(��һ��ʵ��)�� ͨ�� typedef ������ argument_type �� result_type
*    std::binary_function(������ʵ��)��ͨ�� typedef ������ first_argument_type��second_argument_type��result_type
*       .
*   һ����˵��ͨ�ú���Ҳ������Ϊ�º����������ݸ��㷨�������������ڡ�ͨ�ú��������п������ԡ���
*
* STL���һ���ձ�ϰ�ߣ������ͺ���������ʹ�����ڷǳ�Ա�������﷨��ʽ���ã���ȫ�ֺ�����̬��Ա�����������ܱ��룩��
* STL�ṩ�˰� ͨ�ú��������Ա���� ת��Ϊ�º����Ĺ��ܣ���ת����Ա����ʱ�������ú�ָ�������汾��
*   mem_fun -- ͨ������ָ����ó�Ա�����������ڶ�̬���麯����ֻ�����ɲ��������������ǽ���һ�������ĺ�������
*     boost��������չ���ܶ��������������ĳ�Ա��������functor(boost::bind��boost::mem_fn)
*     for_each(lpw.begin(), lpw.end(),mem_fun(&Shape::draw));  //list<Shape*> lpw;draw Ϊ�޲εģ�����вεģ�
*   mem_fun_ref -- ͨ��������ó�Ա���� 
*     for_each(lw.begin(), lw.end(),mem_fun_ref(&Shape::test)); //list<Shape> lw; 
*   ptr_fun -- ͨ�����������ͨ�������Ӳ��Ӳ�Ӱ�����ܺ��Ƿ�ɱ��룩
*     pointer_to_binary_function/pointer_to_unary_function -- Ϊͨ�ú����ṩ���书�ܣ�������ֱ��ʹ��(Ӧ���� ptr_fun )��
*     �ڷº���������ʱ��ͨ�ú�����Ϊ�������롣
*   
* ���������������������������󣩣� not1��not2��bind1st��bind2nd ������Ҫ����ĳЩtypedef �����û��ʱ��ʹ�� ptr_fun
*   һ������ ��һ�����������һ����������Ϊ��һ���������󣬲����ö�����Ϊ�㷨�Ĳ������е���(�㷨Ҫ��ֻ��һ������)
*   bind1st��bind2nd�������ڽ�һ����Ԫ���ӣ�binary functor��bf��ת����һԪ���ӣ������ṩ����Ĳ���
*   bind1st ��ʾ�����ǵ�һ����������bind2nd ��ʾ�����ǵڶ���������
*     v.erase(remove_if( v.begin(), v.end(),bind1st(less<int>(), 100)),v.end()); //�Ƴ����д���100��Ԫ�� 100 < v.value
*     v.erase(remove_if( v.begin(), v.end(),bind2nd(less<int>(), 100)),v.end()); //�Ƴ�����С��100��Ԫ�� v.value < 100
*   not1�񶨵������ĺ�������not2��˫�����ĺ�������
*   not2(ptr_fun(ciCharCompare))); //int ciCharCompare(char c1, char c2)	
*
*
*************************************************************************************************************/
class CSTLFunctionalTester : public CPPUNIT_NS::TestFixture
{
public:

    CPPUNIT_TEST_SUITE( CSTLFunctionalTester );
    CPPUNIT_TEST ( test_bind1st_this );
    CPPUNIT_TEST ( test_another_bind1st_this );
    CPPUNIT_TEST ( test_bind2nd );
    CPPUNIT_TEST ( test_compose1 );
    CPPUNIT_TEST ( test_equal_to ); //equal_to,notequal_to,greater,greater_equal,less_equal,
    CPPUNIT_TEST ( test_not );      
    CPPUNIT_TEST ( test_logical_not );
    CPPUNIT_TEST ( test_negate );
    CPPUNIT_TEST ( test_plus );  //���Ƶ��� minus(��),multiples(��),divieds(��),modulus(ģ?)
    CPPUNIT_TEST ( test_ptr_fun );
    CPPUNIT_TEST ( test_pointer_to_unary_function );
    CPPUNIT_TEST ( test_my_triple_funcion );
    CPPUNIT_TEST_SUITE_END();

    //ͨ��for_eachʹ�ó�Ա����������һ�����������Ҹ���this�еı���
    void test_bind1st_this();
    void test_another_bind1st_this(); //����һ�ַ���

    void test_bind2nd();

    void test_compose1();

    //! ʹ�� operator == �ж�����Ԫ���Ƿ����, ��Ӧ�Ļ��� notequal_to
    void test_equal_to();

    //�񶨺������� not1 �� not2
    void test_not();

    //�Բ������߼�ȡ��
    void test_logical_not();   // ��Ӧ�Ļ��� logical_or, test_logical_and

    //�Բ�����ֵȡ��
    void test_negate();

    void test_plus();
    void test_ptr_fun();

    //��˫�κ���ʹ�õ�adapter
    void test_pointer_to_unary_function();

    //�����Զ����һ����Ԫ�������������
    void test_my_triple_funcion();

    DECLARE_DEFAULT_TEST_CLASS(CSTLFunctionalTester);
private:
    std::vector<std::string>    m_UpperVec;
    
    template <typename Fn>
    bool HandleRecordByEvent(std::string eventName, Fn f);
    void UpperAndInsert2This(std::string str);
};
