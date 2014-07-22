#include "StdAfx.h"
#include "FTLStringTester.h"
#include <ftlString.h>

void CFTLStringTester::test_IsMachMask()
{
    


    LPCTSTR pszName = TEXT("This is a same string");
    CPPUNIT_ASSERT(FTL::CFStringUtil::IsMatchMask(pszName, TEXT("*")));
    CPPUNIT_ASSERT(FTL::CFStringUtil::IsMatchMask(pszName, TEXT("This is * string")));
    CPPUNIT_ASSERT(FTL::CFStringUtil::IsMatchMask(pszName, TEXT("This is ? same s??ing")));
    CPPUNIT_ASSERT(FTL::CFStringUtil::IsMatchMask(pszName, TEXT("*string")));
    CPPUNIT_ASSERT(FTL::CFStringUtil::IsMatchMask(pszName, TEXT("* is * string")));
    CPPUNIT_ASSERT(!FTL::CFStringUtil::IsMatchMask(pszName, TEXT("* String")));
}

void CFTLStringTester::test_UpperLower()
{
    //ע�⣺�ַ�����������ʱ�����ʹ��Unicode�汾��������ʻ�ʱ���ܳ�������

    TCHAR buf[] = _T("some �л����񹲺͹� INFO");

    //ʹ��CRT����
    LPTSTR pUpperResult = _tcsupr(buf);
    CPPUNIT_ASSERT(StrCmp(pUpperResult, _T("SOME �л����񹲺͹� INFO")) == 0);

    //ʹ��΢��API
    CharLower(buf);
    CPPUNIT_ASSERT(StrCmp(buf, _T("some �л����񹲺͹� info")) == 0);

    CharUpperBuff(buf, _countof(buf) - 3); //���ⲻת������ "fo\0" ��3���ַ�������(��������NULL������)
    CPPUNIT_ASSERT(StrCmp(buf, _T("SOME �л����񹲺͹� INfo")) == 0);

    //ʹ��MFC�е�CString �� ATL �е� CAtlString
    CString strMFC = buf;
    strMFC.MakeLower();
    CPPUNIT_ASSERT(StrCmp(strMFC, _T("some �л����񹲺͹� info")) == 0);

    CAtlString strAtl = buf;
    strAtl.MakeUpper();
    CPPUNIT_ASSERT(StrCmp(strAtl, _T("SOME �л����񹲺͹� INFO")) == 0);


}