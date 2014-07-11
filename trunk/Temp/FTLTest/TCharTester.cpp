#include "StdAfx.h"
#include "TCharTester.h"

#define MIN_BUFFER_SIZE     5
#define MAX_BUFFER_SIZE     15
#define TEST_STRING         TEXT("abcde12345")
#define SHORT_TEST_STRING   TEXT("abc")
#define TEST_STRING_LENGTH  10

#pragma warning(disable : 4996) //This function or variable may be unsafe.

void CTCharTester::test_tcscpy()
{
    TCHAR bufMin[MIN_BUFFER_SIZE] = {0};
    errno_t eRet = 0;
    TCHAR* pChatRet = NULL;

    //ע�⣺��������ɳ������ -- �����޷����쳣���Ʋ���
    //  ���԰汾�����Ի��� -- "Buffer is too small"
    //ResetBuf(bufMin,_countof(bufMin));
    // _tcscpy_s(bufMin, _countof(bufMin) , TEST_STRING);

    //"abc\0" -- Buf�㹻�����������ַ��������ҷ���0��ʾ��ȷ
    ResetBuf(bufMin,_countof(bufMin));
    eRet = _tcsncpy_s(bufMin, _countof(bufMin), SHORT_TEST_STRING, _TRUNCATE);
    CPPUNIT_ASSERT(0 == eRet);
    CPPUNIT_ASSERT(_tcscmp(bufMin,TEXT("abc")) == 0);
    CPPUNIT_ASSERT(bufMin[MIN_BUFFER_SIZE - 1] == TEXT('*'));  //������Χ���ַ������

    
    //"abcd\0" -- _tcsncpy_s �Զ�������NULL
    ResetBuf(bufMin,_countof(bufMin));
    eRet = _tcsncpy_s(bufMin,_countof(bufMin),TEST_STRING,_TRUNCATE);
    CPPUNIT_ASSERT(STRUNCATE == eRet);
    CPPUNIT_ASSERT(_tcscmp(bufMin,TEXT("abcd")) == 0);
    CPPUNIT_ASSERT(bufMin[MIN_BUFFER_SIZE - 1] == TEXT('\0'));


    //"abcd*" -- _tcsncpy �����Զ�������NULL
    ResetBuf(bufMin,_countof(bufMin));
    pChatRet = _tcsncpy(bufMin,TEST_STRING,_countof(bufMin) - 1);
    CPPUNIT_ASSERT(pChatRet == bufMin);     //����Ŀ���ַ����ĵ�ַ
    CPPUNIT_ASSERT(_tcsncmp(bufMin,TEXT("abcd"), MIN_BUFFER_SIZE - 1) == 0);
    CPPUNIT_ASSERT(bufMin[MIN_BUFFER_SIZE - 1] == TEXT('*'));  //û���Զ��������NULL
    bufMin[MIN_BUFFER_SIZE - 1]  = 0;  //��Ҫ�ֹ����ȷ������NULL
    CPPUNIT_ASSERT(bufMin[MIN_BUFFER_SIZE - 1] == TEXT('\0'));
}

#pragma warning(default : 4996)