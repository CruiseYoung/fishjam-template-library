#include "StdAfx.h"
#include "ShlWAPITester.h"
#include <Shlwapi.h>

void CShlWAPITester::test_StrFormatByteSizeXX()
{
    DWORD dwNum;
    LONGLONG llNum;
    TCHAR buf[100] = {0};

    llNum = dwNum = 1000;
    ::StrFormatByteSize(dwNum,buf,_countof(buf));
    CPPUNIT_ASSERT( _tcscmp(buf,TEXT("1000 �ֽ�")) == 0);
    ::StrFormatByteSize64(llNum,buf,_countof(buf));
    CPPUNIT_ASSERT( _tcscmp(buf,TEXT("1000 �ֽ�")) == 0);

    llNum = dwNum = 1024;
    ::StrFormatByteSize(dwNum,buf,_countof(buf));
    CPPUNIT_ASSERT( _tcscmp(buf,TEXT("1.00 KB")) == 0);
    ::StrFormatByteSize64(llNum,buf,_countof(buf));
    CPPUNIT_ASSERT( _tcscmp(buf,TEXT("1.00 KB")) == 0);

    llNum = dwNum = 1000000;
    ::StrFormatByteSize(dwNum,buf,_countof(buf));
    CPPUNIT_ASSERT( _tcscmp(buf,TEXT("976 KB")) == 0);
    ::StrFormatByteSize64(llNum,buf,_countof(buf));
    CPPUNIT_ASSERT( _tcscmp(buf,TEXT("976 KB")) == 0);

    llNum = dwNum = 1024 * 1024;
    ::StrFormatByteSize(dwNum,buf,_countof(buf));
    CPPUNIT_ASSERT( _tcscmp(buf,TEXT("1.00 MB")) == 0);
    ::StrFormatByteSize64(llNum,buf,_countof(buf));
    CPPUNIT_ASSERT( _tcscmp(buf,TEXT("1.00 MB")) == 0);

    llNum = dwNum = 1000000000;
    ::StrFormatByteSize(dwNum,buf,_countof(buf));
    CPPUNIT_ASSERT( _tcscmp(buf,TEXT("953 MB")) == 0);
    ::StrFormatByteSize64(llNum,buf,_countof(buf));
    CPPUNIT_ASSERT( _tcscmp(buf,TEXT("953 MB")) == 0);

    llNum = dwNum = 1024 * 1024 * 1024;
    ::StrFormatByteSize(dwNum,buf,_countof(buf));
    CPPUNIT_ASSERT( _tcscmp(buf,TEXT("1.00 GB")) == 0);
    ::StrFormatByteSize64(llNum,buf,_countof(buf));
    CPPUNIT_ASSERT( _tcscmp(buf,TEXT("1.00 GB")) == 0);

    llNum = dwNum = MAXDWORD;
    ::StrFormatByteSize(dwNum,buf,_countof(buf));
    CPPUNIT_ASSERT( _tcscmp(buf,TEXT("3.99 GB")) == 0);
    ::StrFormatByteSize64(llNum,buf,_countof(buf));
    CPPUNIT_ASSERT( _tcscmp(buf,TEXT("3.99 GB")) == 0);

#pragma warning(disable : 4307)
    llNum = 1024 * 1024 * 1024 * 1024;      //C4307 �ı��뾯��
#pragma warning(default: 4307)
    //ע�⣺���������׺���á�L����׺ֱ�ӳˣ��ᱻ��Ϊ��LONG�ĳ˷�����overflow,����1024��0x10000�����Ķ���0����������ʣ�µ�ֵ��0
    CPPUNIT_ASSERT( llNum == 0);            

    llNum = 1024LL * 1024LL * 1024LL * 1024LL;
    ::StrFormatByteSize64(llNum,buf,_countof(buf));
    CPPUNIT_ASSERT( _tcscmp(buf,TEXT("1.00 TB")) == 0);


    llNum = 1024LL * 1024LL * 1024LL * 1024LL * 1024LL;
    ::StrFormatByteSize64(llNum,buf,_countof(buf));
    CPPUNIT_ASSERT( _tcscmp(buf,TEXT("1.00 PB")) == 0);

    llNum = 1024LL * 1024LL * 1024LL * 1024LL * 1024LL * 1024LL;
    ::StrFormatByteSize64(llNum,buf,_countof(buf));
    CPPUNIT_ASSERT( _tcscmp(buf,TEXT("1.00 EB")) == 0);

    llNum = MAXLONGLONG;
    ::StrFormatByteSize64(llNum,buf,_countof(buf));
    CPPUNIT_ASSERT( _tcscmp(buf,TEXT("7.99 EB")) == 0);
}

void CShlWAPITester::test_StrFromTimeInterval()
{
    TCHAR buf[100] = {0};
    DWORD dwTimeMS = 0;
    
     //ע��ת�������ַ���ǰ����пո�

    dwTimeMS = 1000 * 34;
    StrFromTimeInterval(buf,_countof(buf),dwTimeMS,3);
    CPPUNIT_ASSERT( _tcscmp(buf, TEXT(" 34 ��")) == 0);

    StrFromTimeInterval(buf,_countof(buf),dwTimeMS,2);
    CPPUNIT_ASSERT( _tcscmp(buf, TEXT(" 34 ��")) == 0);

    StrFromTimeInterval(buf,_countof(buf),dwTimeMS,1);  //ע����Чλ������
    CPPUNIT_ASSERT( _tcscmp(buf, TEXT(" 30 ��")) == 0);

    dwTimeMS = 1000 * 60;
    StrFromTimeInterval(buf,_countof(buf),dwTimeMS,3);
    CPPUNIT_ASSERT( _tcscmp(buf, TEXT(" 1 ���� 0 ��")) == 0);

    dwTimeMS = 1000 * ( 60 + 1);
    StrFromTimeInterval(buf,_countof(buf),dwTimeMS,3);
    CPPUNIT_ASSERT( _tcscmp(buf, TEXT(" 1 ���� 1 ��")) == 0);

    dwTimeMS = 1000 * 60 * 60;
    StrFromTimeInterval(buf,_countof(buf),dwTimeMS,3);
    CPPUNIT_ASSERT( _tcscmp(buf, TEXT(" 1 Сʱ 0 ��")) == 0 );

    dwTimeMS = 1000 * ( 60 * 60 * 24 + 1 );
    StrFromTimeInterval(buf,_countof(buf),dwTimeMS,2);        
    CPPUNIT_ASSERT( _tcscmp(buf, TEXT(" 24 Сʱ")) == 0 );

    StrFromTimeInterval(buf,_countof(buf),dwTimeMS,3);
    CPPUNIT_ASSERT( _tcscmp(buf, TEXT(" 24 Сʱ 1 ��")) == 0 );

    StrFromTimeInterval(buf,_countof(buf),dwTimeMS,10);
    CPPUNIT_ASSERT( _tcscmp(buf, TEXT(" 24 Сʱ 1 ��")) == 0 );

}