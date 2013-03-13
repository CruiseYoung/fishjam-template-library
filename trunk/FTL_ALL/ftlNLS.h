#ifndef FTL_NLS_H
#define FTL_NLS_H
#pragma once

#ifndef FTL_BASE_H
#  error ftlNLS.h requires ftlbase.h to be included first
#endif

/****************************************************************************************************************
* cmd��������
*   chcp [nnn] -- ��ʾ�����û����ҳ��ţ���������(936)
* 
* http://blog.csdn.net/w8u/article/details/5773449  -- ��ͬcodepage�ļ�ת������ʵ¼
* NLS -- National Language Support�����ʻ�֧��
* MUI --
* 
* �������� GetLocaleInfo ��ȡ�ܶ��û���صĶ�������Ϣ
*   �� GetLocaleInfo( LOCALE_USER_DEFAULT, LOCALE_SABBREVLANGNAME ...) ���Ի�ȡ��ǰ�û����Ե���д(CHS)
*   http://www.flounder.com/localeexplorer.htm ��վ���� localeexplorer.zip Դ�������ϸ�г��˶��ֹ��ʻ���صĴ���
*
* CheckList
*   1.ʹ�� UNICODE/_UNICODE
*   2.Դ�������� fixed text
*   3.Դ�������� graphic element(�����ֵ�ͼƬ?)
*   4.Դ��������UIԪ�ع̶�λ�úʹ�С
*   5.ͳһʹ�� _T �� TCHAR
*   6.����Դ��XML Encode ΪUTF-8
*   7.Unicode�ļ���Ҫ BOM(Byte Order Marker)
*   8.����XMLʱ�� UTF-8 ת��Ϊ Unicode
*   9.��ʾ date,time,currency,number,calendar ��ʱ��Ҫ���Ǳ��ػ�����
*     GetLocalInfo, GetUserDefaultLangID,
*     GetNumberFormat, GetDateFormat, GetCurrencyFormat, SetThreadLocale
*   10.��Ҫ������ʱ�޸ı��ػ� string(?)
*   11.���ַ�����ʾ�����㹻�Ŀռ�
*   12.do not put UI controls on string (?)
*   13.do not allow UI controls to overlay each other.
*   14.��ò�Ҫ��CRT�е� _tcsxxx �Ⱥ�������Ӧ��Win32 API�� lstrncpy �ȣ�������ʹ�� system locale of windows)
*   15.�ַ����Ƚ��� ?
*      CompareString(LOCALE_USER_DEFAULT,
*      LCID locale = MAKELCID(MAKELANGID(), SORT_DEFAULT); Ȼ���� LCMapString(...)
*   16.������ַ�����ʽ���� CString::FormatMessage("%1,%2,%3")
*   17.��һ����Դ(��res[codepage].dll)�У����Ա��������Ե���Դ��ͨ�� SetThreadLocale �л���
*      Ȼ��ͨ�� LoadString �Ⱥ�����ȡ��ͬ���Ե�ֵ
*
* _tsetlocale(LC_ALL, _T("korea"))  -- ���ú���Ļ����������Ժ�ûӴЧ����
*
* ͳһ����汾����Ϣ
*   ���� version.h �ļ���Ȼ���� .rc �ļ��а����������� CompanyName �ȵ�ֵΪ�������غ�
*     QQUOTE(__MAJOR_VERSION)"."QQUOTE(__MINOR_VERSION)"."QQUOTE("__BUILD_TYPE")"."QQUOTE(BUILD_NUMBER)
*   ���⣺�ƺ��༭���Ժ���Զ����ԭ����
*   
*
* ���뷨���
*   ԭ������û��İ�������Ԥ���û�������Ҫ���ַ������ṩһ�ű����ַ��б��Թ��û�����ѡ�������ַ�
*   ����
*     IME(Input Method Editor) -- ʹ�ñ�׼�������븴����ĸ����ţ��������ĵȣ�
*     IMM(Input Method Manager) -- ���뷨��������ͨ�� GetSystemMetrics(SM_IMMENABLED) �жϵ�ǰIMM�Ƿ񼤻�
*   Ӧ�ó������ͣ�
*     1.IME-unaware(Ĭ�ϴ���) -- ϵͳ(DefWindowProc)�Զ�����IME��ص���Ϣ������
*     2.IME-aware -- Ӧ�ó���ͨ�����ݡ���ȡ�ʹ�������Ĭ�ϴ������Ϣ�������ƴ���Ĳ�����λ���Լ���ۡ�
*   ����������(ImmGetContext/ImmReleaseContext) -- 
*     Ĭ��ʱ��ϵͳΪÿ���̴߳�����ָ��һ�����������ģ������ӵ����߳��ڵ�ÿ���½�����,���ɴ����͹��������ض��������ġ�
*     �����������봰�����Ӻ�(ITextHost::TxImmGetContext)��ϵͳ�Զ�ѡ���ʱ���屻����ͱ����뽹��������ģ�
*     1.ImmCreateContext/ImmDestroyContext -- ����/���� ����������
*     2.ImmAssociateContext -- ������ȡ������ָ�����������������
*   ��ش��壺
*     1.״̬����(StatusWindow) -- ָʾIME���򿪣������û��ṩ����ת��ģʽ�ķ���
*     2.¼�봰��(CompositionWindow) -- �û������ı�ʱ������ת��ģʽ��ʾ�����ı���ת������ı�
*       ¼���ַ���--¼�봰��ĵ�ǰ�ı�(��IMEת���������ַ����ı�)��Imm Get/Set CompositionString -- ��ȡ������¼���ַ���
*       ¼���ַ�����һ����Clauses(IME��ת���������ַ����ַ���С���)���
*     3.��ѡ����(CandidateWindow) -- �û������ı�ʱ��������ѡ�ı����ַ������봰���е��ַ����û����Թ��������б���ѡ��������ַ�
*       ��ѡ�б�(CANDIDATELIST) -- ��ָ���ַ���������ɻ��û����Դ���ѡ����ַ������
*         ImmGetCandidateListCount��ImmGetCandidateList
*   �����Ϣ -- ���Ӧ�ó��򲻴��������Ϣ��DefWindowProc �����Ĭ�ϴ����� WM_IME_CHAR ת��Ϊ һ������WM_CHAR�� 
*   
*  TF_FloatingLangBar_WndTitle(CiceroUIWndFrame)
****************************************************************************************************************/

//////////////////////////////////////////////////////////////////////////
#include <map>

namespace FTL
{
    //! ʹ�����ַ�ʽ����֧�ֶ�̬�������ԣ���������Ҫ�ͻ������ܶ๤�� -- ֱ��ʹ�� Win32 API
    //  ͨ����English��DLL�У�ֱ��ʹ��Main����Դ(Main��ԴΪӢ��)���������Ա�֤��Satellite������ʱ������ʹ��Ӣ����ʾ
    class CFSatelliteDllManager
    {
    public:
        CFSatelliteDllManager();
        ~CFSatelliteDllManager();
        BOOL AddSatelliteDll(LANGID language, LPCTSTR pszDllPath);
        BOOL RemoveSatelliteDll(LANGID language);
        BOOL SetDefaultModule(HMODULE hDefaultModule);
        BOOL ChangeLanguage(LANGID DesiredLanguage); //ʵ��ʱ��Ҫע��ɵ�Module���ͷ�
    public:
        //���ֻ��漰�� Satellite Dll �� WinUser ������װ -- ������ A/W��ֱ��ʹ�� T
        int LoadString(__in UINT uID, LPTSTR lpBuffer,int cchBufferMax);
        HACCEL LoadAccelerators(LPCTSTR lpTableName);
        HICON LoadIcon(LPCTSTR lpIconName);
        INT_PTR DialogBoxIndirectParam(LPCDLGTEMPLATE hDialogTemplate,HWND hWndParent,DLGPROC lpDialogFunc,LPARAM dwInitParam);
        HMENU LoadMenu(LPCTSTR lpMenuName);
        HCURSOR LoadCursor(LPCTSTR lpCursorName); //ע�⣺�����ϵͳ��Cursor(�� IDC_ARROW)������ʹ�ø�Satellite����
    private:
        typedef std::map<LANGID,LPCTSTR> SATELLITE_MAP;
        SATELLITE_MAP   m_AllSatellites;
        HMODULE         m_DefaultModule;    //�������õ�Satellite Dll�Ҳ��������
        HMODULE         m_CurrentSatelliteDll;
        LANGID          m_CurrentLanguage;
    };

    class CFNLS
    {
    public:
        //detects a correct initial UI language for all platforms (Win9x, ME, NT4, Windows 2000, Windows XP)
        //Ӣ��Ϊ1033������Ϊ1031������Ϊ1041 -- GetLocaleInfo(AvailableLangID,LOCALE_SNATIVELANGNAME,AvailableLangName,MAX_LANGNAME)
        FTLINLINE LANGID DetectUILanguage();
    protected:
        // The following functions contain code to detect the language in which the initial user interface should be displayed
        FTLINLINE static BOOL CALLBACK EnumLangProc(HANDLE, LPCTSTR, LPCTSTR,WORD wIDLanguage, LONG_PTR lParam);
        // Detects the language of ntdll.dll with some specific processing for the Hongkong SAR version
        FTLINLINE LANGID GetNTDLLNativeLangID();
        // Checks if NT4 system is Hongkong SAR version
        FTLINLINE BOOL IsHongKongVersion();
    };

    class CFConversion
    {
    public:
        FTLINLINE CFConversion(UINT CodePage = CP_ACP);
        FTLINLINE ~CFConversion();

        FTLINLINE LPWSTR UTF8_TO_UTF16( LPCSTR szUTF8 , BOOL bDetached = FALSE);
        FTLINLINE LPSTR  UTF8_TO_MBCS( LPCSTR szUTF8 , BOOL bDetached = FALSE);
        FTLINLINE LPWSTR MBCS_TO_UTF16( LPCSTR szMBCS , BOOL bDetached = FALSE);
        FTLINLINE LPSTR  MBCS_TO_UTF8( LPCSTR szMBCS , BOOL bDetached = FALSE);
        FTLINLINE LPSTR  UTF16_TO_MBCS( LPCWSTR szUTF16 , BOOL bDetached = FALSE);
        FTLINLINE LPSTR  WCS_TO_MBCS( const UINT CodePage, LPCWSTR szWcs , BOOL bDetached = FALSE);
        FTLINLINE PWSTR  MBCS_TO_WCS( const UINT CodePage, LPCSTR szMBCS , BOOL bDetached = FALSE);
        FTLINLINE LPSTR  UTF16_TO_UTF8( LPCWSTR szUTF16 , BOOL bDetached = FALSE);

#ifdef _UNICODE
#define UTF8_TO_TCHAR	UTF8_TO_UTF16
#define TCHAR_TO_UTF8	UTF16_TO_UTF8
        FTLINLINE LPCTSTR UTF16_TO_TCHAR( LPCTSTR lp , BOOL bDetached = FALSE);
        FTLINLINE LPCTSTR TCHAR_TO_UTF16( LPCTSTR lp , BOOL bDetached = FALSE);

#define MBCS_TO_TCHAR	MBCS_TO_UTF16
#define TCHAR_TO_MBCS	UTF16_TO_MBCS

#else
#define UTF8_TO_TCHAR	UTF8_TO_MBCS
#define UTF16_TO_TCHAR	UTF16_TO_MBCS
#define TCHAR_TO_UTF8	MBCS_TO_UTF8
#define TCHAR_TO_UTF16	MBCS_TO_UTF16

        FTLINLINE LPCTSTR MBCS_TO_TCHAR( LPCTSTR lp , BOOL bDetached = FALSE);
        FTLINLINE LPCTSTR TCHAR_TO_MBCS( LPCTSTR lp , BOOL bDetached = FALSE);
#endif
    private:
        UINT    m_codePage;
        CFMemAllocator<BYTE> m_Mem;
        CFMemAllocator<BYTE> m_Mem2;
        
    };

	class CFIMEUtil
	{
	public:

		//��ȡ WM_IME_NOTIFY ��Ϣ��Ӧ���ַ�����Ϣ
		FTLINLINE static LPCTSTR GetIMENotifyInfoString(CFStringFormater& formater, WPARAM wParam, LPARAM lParam);

		//IME Composition String Values
		FTLINLINE static LPCTSTR GetCompositionValueString(CFStringFormater& formater, UINT nGCSValue, LPCTSTR pszDivide = TEXT("|"));
	};
}

#endif //FTL_NLS_H

#ifndef USE_EXPORT
# include "ftlNLS.hpp"
#endif //USE_EXPORT