#include "StdAfx.h"
#include "TCharTester.h"

#define MIN_BUFFER_SIZE     5
#define MAX_BUFFER_SIZE     15
#define TEST_STRING         TEXT("abcde12345")
#define TEST_STRING_LENGTH  10

#pragma warning(disable : 4996) //This function or variable may be unsafe.

void CTCharTester::test_tcscpy()
{

    TCHAR bufMin[MIN_BUFFER_SIZE] = {0};

    //ע�⣺��������ɳ������ -- �����޷����쳣���Ʋ���
    //_tcscpy_s(bufMin,TEST_STRING);


    //"abcd\0"
    _tcsncpy_s(bufMin,_countof(bufMin),TEST_STRING,_TRUNCATE);
    CPPUNIT_ASSERT(_tcscmp(bufMin,TEXT("abcd")) == 0);
    CPPUNIT_ASSERT(bufMin[MIN_BUFFER_SIZE - 1] == TEXT('\0'));

    //"abcd\0"
    ResetBuf(bufMin,_countof(bufMin));
    _tcsncpy_s(bufMin,_countof(bufMin),TEST_STRING,_TRUNCATE);
    CPPUNIT_ASSERT(_tcscmp(bufMin,TEXT("abcd")) == 0);
    CPPUNIT_ASSERT(bufMin[MIN_BUFFER_SIZE - 1] == TEXT('\0'));


    //"abcd\0"
    _tcsncpy(bufMin,TEST_STRING,_countof(bufMin) - 1);
    CPPUNIT_ASSERT(_tcscmp(bufMin,TEXT("abcd")) == 0);
    CPPUNIT_ASSERT(bufMin[MIN_BUFFER_SIZE - 1] == 0);

    //"abcd*"
    ResetBuf(bufMin,_countof(bufMin));
    _tcsncpy(bufMin,TEST_STRING,_countof(bufMin) -1);
    CPPUNIT_ASSERT(_tcsncmp(bufMin,TEXT("abcd"),MIN_BUFFER_SIZE - 1) == 0);
    CPPUNIT_ASSERT(bufMin[MIN_BUFFER_SIZE - 1] == TEXT('*'));  //û���Զ��������NULL
    bufMin[MIN_BUFFER_SIZE - 1]  = 0;  //��Ҫ�ֹ����ȷ������NULL
    CPPUNIT_ASSERT(bufMin[MIN_BUFFER_SIZE - 1] == TEXT('\0'));
}

#pragma warning(default : 4996)