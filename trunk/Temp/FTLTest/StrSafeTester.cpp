#include "StdAfx.h"
#include "StrSafeTester.h"

#define MIN_BUFFER_SIZE     5
#define MAX_BUFFER_SIZE     15
#define TEST_STRING         TEXT("abcde12345")
#define TEST_STRING_LENGTH  10


void CStrSafeTester::test_StringCchCat()
{
    HRESULT hr = E_FAIL;
    TCHAR bufMax[MAX_BUFFER_SIZE] = {0};

    hr = StringCchCat(bufMax,_countof(bufMax),TEST_STRING);
    CPPUNIT_ASSERT(hr == S_OK);
    CPPUNIT_ASSERT(_tcscmp(bufMax,TEXT("abcde12345")) == 0);
    CPPUNIT_ASSERT(_tcslen(bufMax) == TEST_STRING_LENGTH);

    hr = StringCchCat(bufMax,_countof(bufMax),TEST_STRING);
    CPPUNIT_ASSERT(hr == STRSAFE_E_INSUFFICIENT_BUFFER);
    CPPUNIT_ASSERT(_tcscmp(bufMax,TEXT("abcde12345abcd")) == 0);
    CPPUNIT_ASSERT(_tcslen(bufMax) == MAX_BUFFER_SIZE - 1);
    CPPUNIT_ASSERT(bufMax[MAX_BUFFER_SIZE - 1] == 0);
}

void CStrSafeTester::test_StringCchCopy()
{
    HRESULT hr = E_FAIL;
    TCHAR bufMin[MIN_BUFFER_SIZE] = {0};
    ResetBuf(bufMin,_countof(bufMin));

    hr = StringCchCopy(bufMin,_countof(bufMin) - 1,TEST_STRING);//ע�⣬���Ǵ���д���������ڲ��ῼ������ NULL �ռ�
    CPPUNIT_ASSERT(hr == STRSAFE_E_INSUFFICIENT_BUFFER);
    CPPUNIT_ASSERT(_tcscmp(bufMin,TEXT("abc")) == 0);
    CPPUNIT_ASSERT(bufMin[MIN_BUFFER_SIZE - 2] == 0);
    CPPUNIT_ASSERT(bufMin[MIN_BUFFER_SIZE - 1] == TEXT('*'));   //�����һ���ַ��ռ�û��ʹ��

    hr = StringCchCopy(bufMin,_countof(bufMin),TEST_STRING);
    CPPUNIT_ASSERT(hr == STRSAFE_E_INSUFFICIENT_BUFFER);
    CPPUNIT_ASSERT(_tcscmp(bufMin,TEXT("abcd")) == 0);
    CPPUNIT_ASSERT(bufMin[MIN_BUFFER_SIZE - 1] == 0);

}

void CStrSafeTester::test_StringCchPrintf()
{
    HRESULT hr = E_FAIL;
    TCHAR bufMin[MIN_BUFFER_SIZE] = {0};
    ResetBuf(bufMin,_countof(bufMin));

    hr = StringCchPrintf(bufMin,_countof(bufMin),TEXT("%s"),TEST_STRING);
    CPPUNIT_ASSERT(hr == STRSAFE_E_INSUFFICIENT_BUFFER);
    CPPUNIT_ASSERT(_tcscmp(bufMin,TEXT("abcd")) == 0);
    CPPUNIT_ASSERT(bufMin[MIN_BUFFER_SIZE - 1] == 0);
}

void CStrSafeTester::test_StringCchPrintfEx()
{
    HRESULT hr = E_FAIL;
    LPTSTR pszDestEnd = NULL;
    size_t cchRemaining = 0;    //�������������ڣ�δʹ�õ��ַ���
    DWORD dwFlags;              //���ָ����Ĳ���

    /////////////////////////////                  ���Ի���������ʱ�����           /////////////////////////////////
    TCHAR bufMin[MIN_BUFFER_SIZE] = {0};

    dwFlags = 0;                            //"abcd\0"  -- ʧ��ʱ�� StringCchPrintf һ��
    ResetBuf(bufMin,_countof(bufMin));
    hr = StringCchPrintfEx(bufMin,_countof(bufMin),&pszDestEnd,&cchRemaining,dwFlags,TEXT("%s"),TEST_STRING);
    CPPUNIT_ASSERT(hr == STRSAFE_E_INSUFFICIENT_BUFFER);
    CPPUNIT_ASSERT(_tcscmp(bufMin,TEXT("abcd")) == 0);
    CPPUNIT_ASSERT(bufMin[MIN_BUFFER_SIZE - 1] == 0);
    CPPUNIT_ASSERT(pszDestEnd == &bufMin[MIN_BUFFER_SIZE - 1]);
    CPPUNIT_ASSERT(cchRemaining == 1); 

    dwFlags = STRSAFE_FILL_BEHIND_NULL;     //"abcd\0"  -- ʧ��ʱ�� StringCchPrintf һ��
    ResetBuf(bufMin,_countof(bufMin));
    hr = StringCchPrintfEx(bufMin,_countof(bufMin),&pszDestEnd,&cchRemaining,dwFlags,TEXT("%s"),TEST_STRING);
    CPPUNIT_ASSERT(hr == STRSAFE_E_INSUFFICIENT_BUFFER);
    CPPUNIT_ASSERT(_tcscmp(bufMin,TEXT("abcd")) == 0);
    CPPUNIT_ASSERT(bufMin[MIN_BUFFER_SIZE - 1] == 0);
    CPPUNIT_ASSERT(pszDestEnd == &bufMin[MIN_BUFFER_SIZE - 1]);
    CPPUNIT_ASSERT(cchRemaining == 1); 

    dwFlags = STRSAFE_IGNORE_NULLS;         //"abcd\0"  -- ʧ��ʱ�� StringCchPrintf һ��
    ResetBuf(bufMin,_countof(bufMin));
    hr = StringCchPrintfEx(bufMin,_countof(bufMin),&pszDestEnd,&cchRemaining,dwFlags,TEXT("%s"),TEST_STRING);
    CPPUNIT_ASSERT(hr == STRSAFE_E_INSUFFICIENT_BUFFER);
    CPPUNIT_ASSERT(_tcscmp(bufMin,TEXT("abcd")) == 0);
    CPPUNIT_ASSERT(bufMin[MIN_BUFFER_SIZE - 1] == 0);
    CPPUNIT_ASSERT(pszDestEnd == &bufMin[MIN_BUFFER_SIZE - 1]);
    CPPUNIT_ASSERT(cchRemaining == 1); 

    dwFlags = STRSAFE_FILL_ON_FAILURE;      //"\0\0\0\0\0",ʧ��ʱȫ�����Ϊ NULL
    ResetBuf(bufMin,_countof(bufMin));
    hr = StringCchPrintfEx(bufMin,_countof(bufMin),&pszDestEnd,&cchRemaining,dwFlags,TEXT("%s"),TEST_STRING);
    CPPUNIT_ASSERT(hr == STRSAFE_E_INSUFFICIENT_BUFFER);
    for (INT i = 0; i < _countof(bufMin); i++)
    {
        CPPUNIT_ASSERT(bufMin[i] == 0);
    }
    CPPUNIT_ASSERT(pszDestEnd == &bufMin[0]);
    CPPUNIT_ASSERT(cchRemaining == 5);

    dwFlags = STRSAFE_NULL_ON_FAILURE;      //"\0bcd\0",ʧ��ʱ�� StringCchPrintf һ�����󣬽�����ĸ��ΪNULL
    ResetBuf(bufMin,_countof(bufMin));
    hr = StringCchPrintfEx(bufMin,_countof(bufMin),&pszDestEnd,&cchRemaining,dwFlags,TEXT("%s"),TEST_STRING);
    CPPUNIT_ASSERT(hr == STRSAFE_E_INSUFFICIENT_BUFFER);
    CPPUNIT_ASSERT(bufMin[0] == 0);
    CPPUNIT_ASSERT(_tcslen(bufMin) == 0);
    CPPUNIT_ASSERT(_tcscmp(&bufMin[1],TEXT("bcd")) == 0);
    CPPUNIT_ASSERT(pszDestEnd == &bufMin[0]);
    CPPUNIT_ASSERT(cchRemaining == 5);

    dwFlags = STRSAFE_NO_TRUNCATION;        //"\0bcd\0",ʧ��ʱ�� StringCchPrintf һ�����󣬽�����ĸ��ΪNULL
    ResetBuf(bufMin,_countof(bufMin));
    hr = StringCchPrintfEx(bufMin,_countof(bufMin),&pszDestEnd,&cchRemaining,dwFlags,TEXT("%s"),TEST_STRING);
    CPPUNIT_ASSERT(hr == STRSAFE_E_INSUFFICIENT_BUFFER);
    CPPUNIT_ASSERT(_tcslen(bufMin) == 0);
    CPPUNIT_ASSERT(_tcscmp(&bufMin[1],TEXT("bcd")) == 0);
    CPPUNIT_ASSERT(bufMin[MIN_BUFFER_SIZE - 1] == 0);
    CPPUNIT_ASSERT(pszDestEnd == &bufMin[0]);
    CPPUNIT_ASSERT(cchRemaining == 5); 


    dwFlags = STRSAFE_FAILURE_BYTE('?');         //Unicode ʱ��0x3F3F 0x3F3F 0x3F3F 0x3F3F '\0',ʧ��ʱǰ�����Ϊ 0x3F,���ΪNULL
    ResetBuf(bufMin,_countof(bufMin));
    hr = StringCchPrintfEx(bufMin,_countof(bufMin),&pszDestEnd,&cchRemaining,dwFlags,TEXT("%s"),TEST_STRING);
    CPPUNIT_ASSERT(hr == STRSAFE_E_INSUFFICIENT_BUFFER);
    CPPUNIT_ASSERT(bufMin[MIN_BUFFER_SIZE-1] == 0);

    for (INT i = 0; i < _countof(bufMin) - 1; i++)
    {
#ifdef UNICODE
        CPPUNIT_ASSERT(bufMin[i] == 0x3F3F);
#else
        CPPUNIT_ASSERT(bufMin[i] == 0x3F);
#endif
    }
    CPPUNIT_ASSERT(pszDestEnd == &bufMin[MIN_BUFFER_SIZE-1]);
    CPPUNIT_ASSERT(cchRemaining == 1);




    /////////////////////////////                  ���Ի������㹻�������           /////////////////////////////////
    TCHAR bufMax[MAX_BUFFER_SIZE] = {0};

    dwFlags = 0;                            //"abcde12345\0****"
    ResetBuf(bufMax,_countof(bufMax));
    hr = StringCchPrintfEx(bufMax,_countof(bufMax),&pszDestEnd,&cchRemaining,dwFlags,TEXT("%s"),TEST_STRING);
    CPPUNIT_ASSERT(hr == S_OK);
    CPPUNIT_ASSERT(_tcscmp(bufMax,TEXT("abcde12345")) == 0);
    CPPUNIT_ASSERT(_tcslen(bufMax) == TEST_STRING_LENGTH);
    CPPUNIT_ASSERT(bufMax[TEST_STRING_LENGTH] == 0);
#ifndef UNICODE
    //for (INT i = TEST_STRING_LENGTH + 1; i < MAX_BUFFER_SIZE; i++)
    //{
    //    CPPUNIT_ASSERT(bufMax[i] == TEXT('*'));
    //}
#endif
    CPPUNIT_ASSERT(pszDestEnd == &bufMax[TEST_STRING_LENGTH]);
    CPPUNIT_ASSERT(cchRemaining == MAX_BUFFER_SIZE - TEST_STRING_LENGTH); 

    dwFlags = STRSAFE_FILL_BEHIND_NULL;     //"abcde12345\0\0\0\0\0"
    ResetBuf(bufMax,_countof(bufMax));
    hr = StringCchPrintfEx(bufMax,_countof(bufMax),&pszDestEnd,&cchRemaining,dwFlags,TEXT("%s"),TEST_STRING);
    CPPUNIT_ASSERT(hr == S_OK);
    CPPUNIT_ASSERT(_tcscmp(bufMax,TEXT("abcde12345")) == 0);
    CPPUNIT_ASSERT(_tcslen(bufMax) == TEST_STRING_LENGTH);
    CPPUNIT_ASSERT(bufMax[TEST_STRING_LENGTH] == 0);
    for (INT i = TEST_STRING_LENGTH + 1; i < MAX_BUFFER_SIZE; i++)
    {
        CPPUNIT_ASSERT(bufMax[i] == 0);
    }
    CPPUNIT_ASSERT(pszDestEnd == &bufMax[TEST_STRING_LENGTH]);
    CPPUNIT_ASSERT(cchRemaining == MAX_BUFFER_SIZE - TEST_STRING_LENGTH); 


    dwFlags = STRSAFE_IGNORE_NULLS;         //"abcde12345\0****"
    ResetBuf(bufMax,_countof(bufMax));
    hr = StringCchPrintfEx(bufMax,_countof(bufMax),&pszDestEnd,&cchRemaining,dwFlags,TEXT("%s"),TEST_STRING);
    CPPUNIT_ASSERT(hr == S_OK);
    CPPUNIT_ASSERT(_tcscmp(bufMax,TEXT("abcde12345")) == 0);
    CPPUNIT_ASSERT(_tcslen(bufMax) == TEST_STRING_LENGTH);
    CPPUNIT_ASSERT(bufMax[TEST_STRING_LENGTH] == 0);
#ifndef UNICODE
    //for (INT i = TEST_STRING_LENGTH + 1; i < MAX_BUFFER_SIZE; i++)
    //{
    //    CPPUNIT_ASSERT(bufMax[i] == TEXT('*'));
    //}
#endif
    CPPUNIT_ASSERT(pszDestEnd == &bufMax[TEST_STRING_LENGTH]);
    CPPUNIT_ASSERT(cchRemaining == MAX_BUFFER_SIZE - TEST_STRING_LENGTH); 

    dwFlags = STRSAFE_FILL_ON_FAILURE;      //"abcde12345\0****"
    ResetBuf(bufMax,_countof(bufMax));
    hr = StringCchPrintfEx(bufMax,_countof(bufMax),&pszDestEnd,&cchRemaining,dwFlags,TEXT("%s"),TEST_STRING);
    CPPUNIT_ASSERT(hr == S_OK);
    CPPUNIT_ASSERT(_tcscmp(bufMax,TEXT("abcde12345")) == 0);
    CPPUNIT_ASSERT(_tcslen(bufMax) == TEST_STRING_LENGTH);
    CPPUNIT_ASSERT(bufMax[TEST_STRING_LENGTH] == 0);
#ifndef UNICODE
    //for (INT i = TEST_STRING_LENGTH + 1; i < MAX_BUFFER_SIZE; i++)
    //{
    //    CPPUNIT_ASSERT(bufMax[i] == TEXT('*'));
    //}
#endif
    CPPUNIT_ASSERT(pszDestEnd == &bufMax[TEST_STRING_LENGTH]);
    CPPUNIT_ASSERT(cchRemaining == MAX_BUFFER_SIZE - TEST_STRING_LENGTH); 

    dwFlags = STRSAFE_NULL_ON_FAILURE;      //"abcde12345\0****"
    ResetBuf(bufMax,_countof(bufMax));
    hr = StringCchPrintfEx(bufMax,_countof(bufMax),&pszDestEnd,&cchRemaining,dwFlags,TEXT("%s"),TEST_STRING);
    CPPUNIT_ASSERT(hr == S_OK);
    CPPUNIT_ASSERT(_tcscmp(bufMax,TEXT("abcde12345")) == 0);
    CPPUNIT_ASSERT(_tcslen(bufMax) == TEST_STRING_LENGTH);
    CPPUNIT_ASSERT(bufMax[TEST_STRING_LENGTH] == 0);
#ifndef UNICODE
    //for (INT i = TEST_STRING_LENGTH + 1; i < MAX_BUFFER_SIZE; i++)
    //{
    //    CPPUNIT_ASSERT(bufMax[i] == TEXT('*'));
    //}
#endif
    CPPUNIT_ASSERT(pszDestEnd == &bufMax[TEST_STRING_LENGTH]);
    CPPUNIT_ASSERT(cchRemaining == MAX_BUFFER_SIZE - TEST_STRING_LENGTH); 

    dwFlags = STRSAFE_NO_TRUNCATION;        //"abcde12345\0****"
    ResetBuf(bufMax,_countof(bufMax));
    hr = StringCchPrintfEx(bufMax,_countof(bufMax),&pszDestEnd,&cchRemaining,dwFlags,TEXT("%s"),TEST_STRING);
    CPPUNIT_ASSERT(hr == S_OK);
    CPPUNIT_ASSERT(_tcscmp(bufMax,TEXT("abcde12345")) == 0);
    CPPUNIT_ASSERT(_tcslen(bufMax) == TEST_STRING_LENGTH);
    CPPUNIT_ASSERT(bufMax[TEST_STRING_LENGTH] == 0);
#ifndef UNICODE
    //for (INT i = TEST_STRING_LENGTH + 1; i < MAX_BUFFER_SIZE; i++)
    //{
    //    CPPUNIT_ASSERT(bufMax[i] == TEXT('*'));
    //}
#endif
    CPPUNIT_ASSERT(pszDestEnd == &bufMax[TEST_STRING_LENGTH]);
    CPPUNIT_ASSERT(cchRemaining == MAX_BUFFER_SIZE - TEST_STRING_LENGTH); 


    //ʹ������ַ� -- '?' ��Ӧ 0x3F, Ĭ�ϵ�����ַ�Ϊ NULL
    dwFlags = STRSAFE_FILL_BYTE('?');       //"abcde12345\0\" 0x3F3F 0x3F3F 0x3F3F 0x3F3F
    ResetBuf(bufMax,_countof(bufMax));
    hr = StringCchPrintfEx(bufMax,_countof(bufMax),&pszDestEnd,&cchRemaining,dwFlags,TEXT("%s"),TEST_STRING);
    CPPUNIT_ASSERT(hr == S_OK);
    CPPUNIT_ASSERT(_tcscmp(bufMax,TEXT("abcde12345")) == 0);
    CPPUNIT_ASSERT(_tcslen(bufMax) == TEST_STRING_LENGTH);
    CPPUNIT_ASSERT(bufMax[TEST_STRING_LENGTH] == 0);
    for (INT i = TEST_STRING_LENGTH + 1; i < MAX_BUFFER_SIZE; i++)
    {
#ifdef UNICODE
        CPPUNIT_ASSERT(bufMax[i] == 0x3F3F);
#else
        CPPUNIT_ASSERT(bufMax[i] == 0x3F);
#endif
    }
    CPPUNIT_ASSERT(pszDestEnd == &bufMax[TEST_STRING_LENGTH]);
    CPPUNIT_ASSERT(cchRemaining == MAX_BUFFER_SIZE - TEST_STRING_LENGTH); 

}