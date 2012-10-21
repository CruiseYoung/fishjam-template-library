//------------------------------------------------------------------------------
// File: TextType.h
//
// Desc: DirectShow sample code - definition of CTextMediaType class.  This
//       is a class that can return itself as text.
//
// Copyright (c) Microsoft Corporation.  All rights reserved.
//------------------------------------------------------------------------------


class CTextMediaType : public CMediaType {

public:

    CTextMediaType(AM_MEDIA_TYPE mt):CMediaType(mt) {}

    HRESULT AsText(LPTSTR szType,unsigned int iLen);
	HRESULT AsText(FTL::CFStringFormater& strFormater);
	static HRESULT CLSID2String(LPTSTR szBuffer, UINT iLength, const GUID* pGuid);
    
    //���ع���ָ��CLSID��COM�����λ��
    //�����������ڴ��ݵ�GUID��Filter��ʱ����ȷ�� -- �� SysEnum �п�����������δȷ��������
    static HRESULT GetFilenameByCLSIDString(const WCHAR *szGUID, WCHAR *szFile, size_t cch);
    static HRESULT GetFilenameByCLSID(const GUID *pGUID, WCHAR *szFile, size_t cch);
private:

    // Provide a string description for this format block

    HRESULT Format2String(LPTSTR szBuffer,
                       UINT iLength,
                       const GUID* pGuid,
                       BYTE* pFormat,
                       ULONG lFormatLength);

    // Convert this CLSID into a meaningful string


	HRESULT _GetVideoInfoHeaderInfo(LPTSTR szBuffer, UINT iLength, 
		LPCTSTR pszInfo, BYTE* pFormat, ULONG lFormatLength);

	HRESULT _GetWaveFormatInfo(LPTSTR szBuffer, UINT iLength, 
		LPCTSTR pszInfo, BYTE* pFormat, ULONG lFormatLength);

};

