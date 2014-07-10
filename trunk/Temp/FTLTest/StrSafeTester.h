#pragma once

#include <cppunit/extensions/HelperMacros.h>
#include <strsafe.h>

/***************************************************************************************************
* Strsafe.h������ȫ��C�����ַ���������(C++����ʹ�� CString��CComBSTR��stl::string ����)
*  ��׼��C��������ʱ����Ҫô�ڷ���ֵ�Ͳ�����ȱ��һ���ԣ�Ҫô��������ν�ġ��ض���(truncation errors) 
*  �������ײ������ڴ�����������⡣
* ע�⣺
*  1.������ strsafe ϵ�к���ʱ��ԭ�е� C �����ַ����������������Զ����� #undef ����
*  2.��صĺ�(��Ҫ�� #include <strsafe.h> ֮ǰ)
*    #define STRSAFE_LIB -- �� library form ��ʽʹ��(���Զ�ʹ�� #pragma comment(lib, "strsafe.lib") )
*    #define STRSAFE_NO_DEPRECATE -- ��ֹ���ɵĺ���������Ϊ deprecated ��(������뾯��)
*    #define STRSAFE_NO_CB_FUNCTIONS -- allow only character count functions(ֻ�����ַ�������������Byte������)
*    STRSAFE_NO_CCH_FUNCTIONS -- allow only byte count functions(ֻ����Byte������)
*    STRSAFE_MAX_CCH -- ֧�ֵ������ַ�������(2,147,483,647)
*
* ������Ĵ��룺û�м�鷵��ֵ��strncat�д���ʹ��cchPath(Ӧ��ʣ��ռ䳤�ȣ��������ܳ���)
* void UnsafeFunc(LPTSTR szPath,DWORD cchPath) 
* {
*   TCHAR szCWD[MAX_PATH];
*   GetCurrentDirectory(ARRAYSIZE(szCWD), szCWD);
*   strncpy(szPath, szCWD, cchPath);
*   strncat(szPath, TEXT("\\"), cchPath);
*   strncat(szPath, TEXT("desktop.ini"),cchPath);
* }
* 
* �õĴ��룺StringCchCat��ʹ�õ� cchPath ����Ŀ�껺�������ܳ��ȡ�
* bool SaferFunc(LPTSTR szPath,DWORD cchPath) 
* {
*   TCHAR szCWD[MAX_PATH];
*   if (GetCurrentDirectory(ARRAYSIZE(szCWD), szCWD) &&
*     SUCCEEDED(StringCchCopy(szPath, cchPath, szCWD)) &&
*     SUCCEEDED(StringCchCat(szPath, cchPath, TEXT("\\"))) &&
*     SUCCEEDED(StringCchCat(szPath, cchPath, TEXT("desktop.ini")))) {
*       return true;
*   }
*   return false;
* }	
*
* strsafe ϵ�к�����Ex�汾����ʵ�ָ��Ӹ߼��Ĺ��ܣ�
*   ��ȡĿ�껺�����ĵ�ǰָ�롣
*   ��ȡĿ�껺������ʣ��ռ䳤�ȡ�
*   ��ĳ���ض��ַ������л�������
*   һ���ַ���������ʧ�ܣ��Ͱ����ض�ֵ����ַ�����
*   һ���ַ���������ʧ�ܣ��Ͱ�Ŀ�껺������� NULL ��
***************************************************************************************************/

class CStrSafeTester : public CPPUNIT_NS::TestFixture
{
public:

    CPPUNIT_TEST_SUITE( CStrSafeTester );

    CPPUNIT_TEST(test_StringCchCat );
    //CPPUNIT_TEST(test_StringCchCatEx );
    //CPPUNIT_TEST(test_StringCchCatN );
    //CPPUNIT_TEST(test_StringCchCatNEx );
    CPPUNIT_TEST(test_StringCchCopy );
    //CPPUNIT_TEST(test_StringCchCopyEx );
    //CPPUNIT_TEST(test_StringCchCopyN );
    //CPPUNIT_TEST(test_StringCchCopyNEx );
    //CPPUNIT_TEST(test_StringCchGets );
    //CPPUNIT_TEST(test_StringCchGetsEx );
    //CPPUNIT_TEST(test_StringCchLength );
    CPPUNIT_TEST(test_StringCchPrintf );
    CPPUNIT_TEST(test_StringCchPrintfEx );
    //CPPUNIT_TEST(test_StringCchVPrintf );
    //CPPUNIT_TEST(test_StringCchVPrintfEx );

    CPPUNIT_TEST_SUITE_END();

    void test_StringCchCat();
    //void test_StringCchCatEx();
    //void test_StringCchCatN();
    //void test_StringCchCatNEx();
    void test_StringCchCopy();      //��Χ��Buffer�Ĵ�С�����Զ��������NULL
    //void test_StringCchCopyEx();
    //void test_StringCchCopyN();
    //void test_StringCchCopyNEx();
    //void test_StringCchGets();
    //void test_StringCchGetsEx();
    //void test_StringCchLength();
    void test_StringCchPrintf();    //��Χ��Buffer�Ĵ�С�����Զ��������NULL
    void test_StringCchPrintfEx();
    //void test_StringCchVPrintf();
    //void test_StringCchVPrintfEx();
    DECLARE_DEFAULT_TEST_CLASS(CStrSafeTester);
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
