#pragma once

#include <cppunit/extensions/HelperMacros.h>
#include <tchar.h>

/***************************************************************************************************
* VS2005 ���ṩ���ַ�����ȫ����(_s) -- ���õ��԰汾��ʹ�ö��Դ���Release�汾��ֱ�ӱ����ķ�ʽ������ֹ��
*    ����������������޷����쳣������(C++�쳣 �� �ṹ���쳣)��
*    ���ǰ�ȫ�����汾�Ƿ�����������ڻ������Ƿ񸲸Ƿ��ص�ַ��
*
* _tcsncpy_s ���Ļ����С ������  ����NULL����   �Զ��������NULL
* _tcsncpy   ���Ļ����С ������  ����NULL������ �Զ��������NULL����Ҫ�ֹ����ȷ����NULL����
***************************************************************************************************/

class CTCharTester : public CPPUNIT_NS::TestFixture
{

    CPPUNIT_TEST_SUITE( CTCharTester );

    CPPUNIT_TEST( test_tcscpy );

    CPPUNIT_TEST_SUITE_END();


    void test_tcscpy();

    DECLARE_DEFAULT_TEST_CLASS(CTCharTester);

private:
    void ResetBuf(LPTSTR pbuf,DWORD cCount)
    {
#ifdef UNICODE
        wmemset(pbuf,TEXT('*'),cCount);
#else
        memset(pbuf,TEXT('*'),cCount);
#endif
    }

};