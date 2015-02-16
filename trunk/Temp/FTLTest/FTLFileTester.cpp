#include "StdAfx.h"
#include "FTLFileTester.h"


void CFTLFileTester::test_CreateFile()
{
	BOOL bRet = FALSE;
	TCHAR szTempFilePath[MAX_PATH] = {0};
	API_VERIFY(0 != ::GetTempPath(_countof(szTempFilePath), szTempFilePath));
	API_VERIFY(::PathAppend(szTempFilePath, TEXT("FtlTestFile.tmp")));
	
	//�Կɶ�д��ʽ�򿪣���Share��ָ��Ϊ0����ʾ���������
	HANDLE hOpenWriteFile = ::CreateFile(szTempFilePath, GENERIC_WRITE | GENERIC_READ, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL , NULL);
	CPPUNIT_ASSERT(INVALID_HANDLE_VALUE != hOpenWriteFile);
	
	DWORD dwWriteCount = 0;
	CString strValue = TEXT("Some Value");
	API_VERIFY(WriteFile(hOpenWriteFile, strValue, strValue.GetLength() * sizeof(TCHAR), &dwWriteCount, NULL));
	API_VERIFY(FlushFileBuffers(hOpenWriteFile));

	//��ʹ��ֻ����ʽ�򿪣�Ҳ��ʧ��
	HANDLE hOpenReadFile = ::CreateFile(szTempFilePath, GENERIC_READ , FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	CPPUNIT_ASSERT(INVALID_HANDLE_VALUE == hOpenReadFile);

	//dwDesiredAccess ָ��Ϊ0������Դ򿪲�����Share���ļ��������ܻ�ȡ���ļ���С�����������ȡ���ݵ�
	HANDLE hOpenQueryFile = ::CreateFile(szTempFilePath, 0,            FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	CPPUNIT_ASSERT(INVALID_HANDLE_VALUE != hOpenQueryFile);
	DWORD dwFileSize = GetFileSize(hOpenQueryFile, NULL);
	CPPUNIT_ASSERT(dwFileSize == strValue.GetLength() * sizeof(TCHAR));
	//���ܶ�ȡ�ļ�����
	BYTE pReadBuffer[100] = {0};
	bRet = ReadFile(hOpenQueryFile, pReadBuffer, _countof(pReadBuffer), NULL, NULL);
	DWORD dwError = GetLastError();
	CPPUNIT_ASSERT(!bRet);
	CPPUNIT_ASSERT(ERROR_ACCESS_DENIED == dwError);

	SAFE_CLOSE_HANDLE(hOpenQueryFile, INVALID_HANDLE_VALUE);
	SAFE_CLOSE_HANDLE(hOpenReadFile, INVALID_HANDLE_VALUE);
	SAFE_CLOSE_HANDLE(hOpenWriteFile, INVALID_HANDLE_VALUE);

	DeleteFile(szTempFilePath);
}

void CFTLFileTester::test_DirectoryCopier()
{
    BOOL bRet = FALSE;
    //API_VERIFY(m_DirCopier.SetCallback(this));
    //API_VERIFY(m_DirCopier.Start(_T("E:\\Test\\testMFT_Old"), _T("E:\\Test\\testCopyTarget")));

    Sleep(10000);

}

void CFTLFileTester::test_DumpAllAPISystemFolderPath()
{
    BOOL bRet = FALSE;
    API_VERIFY(FTL::CFFileUtil::DumpAllAPISystemFolderPath(SHGFP_TYPE_CURRENT));
    //API_VERIFY(FTL::CFFileUtil::DumpAllAPISystemFolderPath(SHGFP_TYPE_DEFAULT));
}