#include "StdAfx.h"
#include "FTLNlsTester.h"
#include <ftlConversion.h>
#include <atlenc.h>

#pragma comment(lib, "Rpcrt4.lib")


void CFTLNlsTester::test_ATLBase64()
{
	std::string strUtf8 = "Some UTF8 code string";
	int iDestLen = ATL::Base64EncodeGetRequiredLength(strUtf8.size());
	CHAR *pBase64 = new CHAR[iDestLen + 1]; 
#pragma TODO(�������ʱ�����һ�µ����費��Ҫ������� 1) //��������Ҫ��
	if (pBase64)
	{
		pBase64[0] = '\0';

		if (ATL::Base64Encode((LPBYTE)strUtf8.c_str(), strUtf8.size(), pBase64, &iDestLen))
		{
			pBase64[iDestLen] = '\0';
			CStringW wstrBase64 = FTL::CFConversion().UTF8_TO_UTF16(pBase64);
			//ע�⣺��URL��ʹ��Base64ʱ������Ҫ��Щת��

			//wstrBase64.Replace(_T("+"), _T("%2B"));
			//wstrBase64.Replace(_T("/"), _T("%2F"));
			wstrBase64.Replace(_T("="), _T("-"));
			CPPUNIT_ASSERT( wstrBase64 == TEXT("U29tZSBVVEY4IGNvZGUgc3RyaW5n"));
		}
		delete []pBase64;
	}

}

void CFTLNlsTester::test_ATLBase64_ForGuid()
{
    HRESULT hr = E_FAIL;
	BOOL bRet = FALSE;
	
	TCHAR szGuidString[40] = {0};
	CONST INT GUID_LENGTH = 39; //(128/4 + 4 + 2 + 1)) -- GUID��128λ��ÿ4λһ������,Ȼ������ĸ��ָ���(-),��ǰ��Ĵ�����({}),���ַ����Ľ�����(\0)
	//GUID_NULL == "{00000000-0000-0000-0000-000000000000}"

	int nGuidNullLength = StringFromGUID2(GUID_NULL, szGuidString, _countof(szGuidString));
    CPPUNIT_ASSERT(nGuidNullLength == GUID_LENGTH); 

    int nBase64Length = ATL::Base64EncodeGetRequiredLength(nGuidNullLength);
    FTL::CFMemAllocator<CHAR> buffBase64(nBase64Length + 1);
    CPPUNIT_ASSERT(nBase64Length == 54);  //GUID ���Ҫת����BASE64��Ҫ�Ŀռ�(�Ѿ����������Ľ������������� +1 )

    int nMaxBase64Length = 0;
    int nMinBase64Length = INT_MAX;
    for ( int i = 1; i <= 2; i++)
    {
        UUID newUuid = GUID_NULL;
        COM_VERIFY(UuidCreate(&newUuid));
        if (SUCCEEDED(hr))
        {
			ZeroMemory(szGuidString, sizeof(szGuidString));
            int nGuidLength = StringFromGUID2(newUuid, szGuidString, _countof(szGuidString));
            CPPUNIT_ASSERT(nGuidLength == nGuidNullLength);

			//szGuidString[GUID_LENGTH-2] = NULL;		//ȥ�������� "}"
			//LPCTSTR pszPureGuid = &szGuidString[1];		//������ǰ��� "{"
            CStringA strUTF8 = FTL::CFConversion().UTF16_TO_UTF8(szGuidString); //pszPureGuid);

            int nBase64ResultLength = nBase64Length;
            API_VERIFY(ATL::Base64Encode((const BYTE*)(LPCSTR)strUTF8, strUTF8.GetLength(), (LPSTR)buffBase64, &nBase64ResultLength));
			CPPUNIT_ASSERT(nBase64ResultLength == 52);

			FTLTRACE(TEXT("guid=%s, base64=%s\n"), szGuidString, FTL::CFConversion().UTF8_TO_TCHAR(buffBase64.GetMemory()));
            nMaxBase64Length = FTL_MAX(nMaxBase64Length, nBase64ResultLength);
            nMinBase64Length = FTL_MIN(nMinBase64Length, nBase64ResultLength);
        }
    }

    CPPUNIT_ASSERT(nMinBase64Length == nMaxBase64Length);
}

void CFTLNlsTester::test_CFConversion()
{
    CFConversion    cov;
    LPSTR pszUtf8 = cov.UTF16_TO_UTF8(L"�й�");
    //utf16 == 0x4e2d 56fd 0000
    //pszUtf8 == 0xe4 b8 ad e5 9b bd 00
}

void CFTLNlsTester::test_MultiByteToWideChar()
{
	//http://blog.sina.com.cn/s/blog_88cbbb730100vew6.html
	//TODO:
	CPPUNIT_ASSERT(TRUE);
}

void CFTLNlsTester::test_WideCharToMultiByte()
{
	
}