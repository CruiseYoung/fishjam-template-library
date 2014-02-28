#pragma once

#include <cppunit/extensions/HelperMacros.h>

/**********************************************************************************************
* Poco �����쳣�ķ�ʽ������ҵ���߼���
*   Poco::Exception -- ���࣬�̳���std::exception
*   +-LogicException -- ���༰�����ฺ����������(һ�㶼�Ǵ����߼�����)������AssertionViolation��NullPointer��
*   +-ApplicationException -- ������Ӧ�ó�����صĴ���
*   +-PosoRuntimeException -- �������������ʱ�Ĵ���
*   ��ط���:
*     displayText() -- ͬʱ�����쳣���ֺ���Ϣ�ַ������м�ʹ��": "�ָ�
*     nested() -- �������Ƕ���쳣�Ļ�������ָ��Ƕ���쳣��ָ�룬���򷵻�NULL
*
* �Զ����쳣
*   POCO_DECLARE_EXCEPTION -- �����쳣�ĺ�
*   POCO_IMPLEMENT_EXCEPTION -- �����쳣���ִ����
*   �磺
*      .h --   POCO_DECLARE_EXCEPTION(MyLib_API, MyException, Poco::Exception) 
*      .cpp -- POCO_IMPLEMENT_EXCEPTION(MyException, Poco::Exception, "Something really bad happened") 
**********************************************************************************************/

/**********************************************************************************************
* ���� -- POCO�����ṩ��һЩ���Եĺ�����������ʱ��飬��Щ�����ܹ��ṩ���������кź��ļ���Ϣ��
*   ��������debug����ģʽ��(������Visual C++)��ʱ���ᴥ��һ��breakpoint
* 
*   Debugger
*     _assert(cond) -- ��� !cond ʱ���׳�һ��AssertionViolationException�쳣��
*     enter("��ʾ��Ϣ") -- ���԰汾�½������(��VC ʹ�� DebugBreak API ����)
*   Bugcheck -- ���԰���
*     ȫ�ָ�������
*       poco_bugcheck_msg(string) -- �׳�BugcheckException�쳣, �ȼ��� enter ?
*       poco_assert_dbg(cond) -- ͬpoco_assert���ƣ�����ֻ��debugģʽ��������
*       poco_check_ptr(ptr) -- ���ptrΪ�գ����׳�NullPointerException�쳣
*     
**********************************************************************************************/

/**********************************************************************************************
* NDC(Nested Diagnostic Context) -- Ϊ�˶��߳���϶���Ƶ�(ʹ��TLSΪÿ���̲߳������Ե���־)
*   Poco::NestedDiagnosticContext -- ά��һ��NDC�������а����������ĵ�ջ��Ϣ���к�����������Դ�ļ������ļ������к�.
*   �� poco_ndc(func)|poco_ndc_dbg(func) -- ������һ�� NDCScope ����������������ĵ���ջ����ջ������
*
*   1.DiagnosticLogger -- ������־�ͳ�������ģ��
*   2.TransactionalBuckets -- ����Ͱ��Ϊ���񵥶�������־
*   3.TypedDiagnostics -- ���ͻ���ϣ�Ϊ���е������Ϣ�ṩͳһ��չ��
**********************************************************************************************/

class CPocoDebugTester : public CPPUNIT_NS::TestFixture
{
public:
    CPPUNIT_TEST_SUITE( CPocoDebugTester );
    CPPUNIT_TEST( test_NDC );
    CPPUNIT_TEST_SUITE_END();

    DECLARE_DEFAULT_TEST_CLASS(CPocoDebugTester);
private:
    void test_NDC();
};
