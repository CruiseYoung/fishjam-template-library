#ifndef FTL_CONVERSION_H
#define FTL_CONVERSION_H

#pragma once

#ifndef FTL_BASE_H
#  error ftlConversion.h requires ftlbase.h to be included first
#endif

/************************************************************************************************************
* CodePage -- �ַ���,�������� CP_ACP(��ǰϵͳANSI����ҳ)��CP_THREAD_ACP(��ǰ�̵߳�ANSI), CP_UTF8 ��
* ����ַ� -- ��һ�������ַ�(�� e)��һ���ǿ��ַ�����(�� e ͷ�ϵ��������)��ϳɶ�������(������)
* 
* �ַ�ӳ��ʱ�Ĳ���(dwFlags) 
*   MultiByteToWideChar ʱ�Ĳ��� -- MB_PRECOMPOSED �� MB_COMPOSITE ����
*     ��δȷ�ϣ���dwFlags Ϊ 0 ʱ�����ȳ���ת���� Ԥ���ַ������ʧ�ܣ�������ת��������ַ���
*     MB_PRECOMPOSED -- ʹ��Ԥ���ַ�������һ�������ַ���һ���ǿ��ַ���ɵ��ַ�ֻ��һ����һ���ַ�ֵ
*     MB_COMPOSITE() -- ʹ������ַ�, ��һ�������ַ���һ���ǿ��ַ���ɵ��ַ��ֱ��в�ͬ���ַ�ֵ
*     MB_ERR_INVALID_CHARS -- ��������Ч�������ַ���ʧ�ܣ�GetLastError Ϊ ERROR_NO_UNICODE_TRANSLATION��
*       TODO:��ʱ���صĽ��Ϊ0���ƺ�û�а취ֱ�Ӷ�λ��Ч�ַ���λ�ã�ֻ�ܳ����ö��ַ������ң�
*       ��û�����������ǣ�������Ч�ַ�ʱ(��DBCS���з�����ͷ�ֽڶ�û����Ч��β�ֽ�)����Ч�ַ���ת��Ϊȱʡ�ַ���
*     MB_USEGLYPHCHARS -- ʹ������������������ַ�
*
*   WideCharToMultiByteʱ�Ĳ���
*     WC_NO_BEST_FIT_CHARS -- �Ѳ���ֱ��ת������Ӧ���ֽ��ַ���Unicode�ַ�ת����lpDefaultCharָ����Ĭ���ַ���
*        ������ú󣬰�Unicodeת���ɶ��ֽ��ַ���Ȼ����ת������������һ���õ���ͬ��Unicode�ַ�(ʹ����Ĭ���ַ�)��
*        ����������û���Σ�
*     WC_COMPOSITECHECK -- �Ѻϳ��ַ�ת����Ԥ�Ƶ��ַ�
*     WC_ERR_INVALID_CHARS -- �������˵Ļ�������������Ч�ַ�ʱʧ�ܷ���(ERROR_NO_UNICODE_TRANSLATION)��
*        ���������Զ������Ƿ��ַ�����ѡ��ֻ������UTF8
*     WC_DISCARDNS -- ת��ʱ������ռ�ռ���ַ�����WC_COMPOSITECHECKһ��ʹ��
*     WC_SEPCHARS(Ĭ��) -- ת��ʱ�����������ַ�����WC_COMPOSITECHECKһ��ʹ��
*     WC_DEFAULTCHAR -- ת��ʱʹ��Ĭ���ַ�����������ַ�(������� '?')����WC_COMPOSITECHECKһ��ʹ��
************************************************************************************************************/

namespace FTL
{
	class CFConversion
	{
	public:
		FTLINLINE CFConversion(UINT CodePage = CP_ACP, DWORD dwFlags = 0);
		FTLINLINE ~CFConversion();
		FTLINLINE BOOL IsUsedDefaultChar() { return m_bUsedDefaultChar; }
		FTLINLINE VOID SetDefaultCharForWC2MB(LPSTR pszDefaultChar);

		FTLINLINE LPWSTR UTF8_TO_UTF16( LPCSTR szUTF8 , INT* pLength = NULL, BOOL bDetached = FALSE);
		FTLINLINE LPSTR  UTF8_TO_MBCS( LPCSTR szUTF8 , INT* pLength = NULL, BOOL bDetached = FALSE);
		FTLINLINE LPWSTR MBCS_TO_UTF16( LPCSTR szMBCS , INT* pLength = NULL, BOOL bDetached = FALSE);
		FTLINLINE LPSTR  MBCS_TO_UTF8( LPCSTR szMBCS , INT* pLength = NULL, BOOL bDetached = FALSE);
		FTLINLINE LPSTR  UTF16_TO_MBCS( LPCWSTR szUTF16 , INT* pLength = NULL, BOOL bDetached = FALSE);
		FTLINLINE LPSTR  WCS_TO_MBCS( const UINT CodePage, LPCWSTR szWcs , INT* pLength = NULL, BOOL bDetached = FALSE);
		FTLINLINE PWSTR  MBCS_TO_WCS( const UINT CodePage, LPCSTR szMBCS , INT* pLength = NULL, BOOL bDetached = FALSE);
		FTLINLINE LPSTR  UTF16_TO_UTF8( LPCWSTR szUTF16 , INT* pLength = NULL, BOOL bDetached = FALSE);

#ifdef _UNICODE
#define UTF8_TO_TCHAR	UTF8_TO_UTF16
#define TCHAR_TO_UTF8	UTF16_TO_UTF8
		FTLINLINE LPCTSTR UTF16_TO_TCHAR( LPCTSTR lp , INT* pLength = NULL, BOOL bDetached = FALSE);
		FTLINLINE LPCTSTR TCHAR_TO_UTF16( LPCTSTR lp , INT* pLength = NULL, BOOL bDetached = FALSE);

#define MBCS_TO_TCHAR	MBCS_TO_UTF16
#define TCHAR_TO_MBCS	UTF16_TO_MBCS

#else
#define UTF8_TO_TCHAR	UTF8_TO_MBCS
#define UTF16_TO_TCHAR	UTF16_TO_MBCS
#define TCHAR_TO_UTF8	MBCS_TO_UTF8
#define TCHAR_TO_UTF16	MBCS_TO_UTF16

		FTLINLINE LPCTSTR MBCS_TO_TCHAR( LPCTSTR lp , INT* pLength = NULL, BOOL bDetached = FALSE);
		FTLINLINE LPCTSTR TCHAR_TO_MBCS( LPCTSTR lp , INT* pLength = NULL, BOOL bDetached = FALSE);
#endif
	private:
		UINT    m_codePage;
		DWORD	m_dwFlags;

		//WideCharToMultiByte ʱʹ�õĲ��� -- ע�⣺CodePage Ϊ CP_UTF7 �� CP_UTF8 ʱ������������������NULL�������ʧ��
		LPSTR   m_pDefaultChar;		//����һ������ת���Ŀ��ַ����������ʹ��pDefaultChar����ָ����ַ�
		BOOL    m_bUsedDefaultChar;	//������һ���ַ�����ת��Ϊ����ֽ���ʽ�������ͻ�����������ΪTRUE
		CFMemAllocator<BYTE> m_Mem;
		CFMemAllocator<BYTE> m_Mem2;
	};


	class CFConvUtil
	{
	public:
		//ex. pb = {255,15} -> sz = {"FF0F"} -- TODO: AtlHexDecode/AtlHexEncode
		//FTLINLINE static LONG HexFromBinary(__in const BYTE* pBufSrc, __in LONG nSrcCount, 
		//	__out LPTSTR pBufDest, __inout long* pDestCount, 
		//	__in LPCTSTR pszDivision = NULL);

	};
}

#endif //FTL_CONVERSION_H

#ifndef USE_EXPORT
# include "ftlConversion.hpp"
#endif //USE_EXPORT