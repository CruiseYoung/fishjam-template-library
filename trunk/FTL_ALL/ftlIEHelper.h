#ifndef FTL_IE_HELPER_H
#define FTL_IE_HELPER_H

#pragma once

#ifndef FTL_BASE_H
#  error ftlIEHelper.h requires ftlbase.h to be included first
#endif

#ifndef FTL_COM_H
#  error ftlIEHelper.h requires ftlCom.h to be included first
#endif


/********************************************************************************************
* Win8 -- ���ֲ�ͬ���û��ӿڣ���Ҫ�ֱ����в�ͬ���͵�Ӧ�ó�����IE�ж�Ӧ�� Metro style �� Desktop style)
*   1.���� -- ����Ӧ�ó���
*   2.Metro -- ר�ŵ� Metro ����� Metro-style enabled desktop browsers (MEDB).Metroģʽ�µ�IEֻ��Flash��������ܰ�װ�������������κβ��?
*     WinRT -- ARMƽ̨�ϵ�Window8��ֻ�����д����Metro���򣬲���ʹ��Win32API����Ҫʹ��ר�ŵ� WinRT API?
*
* <iepmapi.h> -- IE Protected Mode API, ������ IEIsProtectedModeProcess �Ⱥ����Ķ���(�����ǵ� WinXP �ȵ����У���ö�̬����)
* EPM(Enhanced Protected Mode) -- ���ú�ܶ������޷���ȷ���С�Metro���͵�ʼ��������EPM, Desktop���͵Ŀ�ѡ(Internet Options->Advanced),
*   Web��վ�Ŀ������� Http Header ��ͨ��"X-UA-Compatible:requiresActiveX=true" ѡ��Ҫ��plugin(��Ҫ�󾭵�ģʽ?)

* IE 7 �ı���ģʽ(Protected Mode) -- Ҳ��IE��Ȩ��(Low Rights)��ͨ�� Process Explorer �鿴���̵�"��ȫ"���ԣ��ɿ�����
*   "Mandatory Label\Low Mandatory Level"��SID(��Ӧ�ı�־λ��"Integrity")�������IE���̵������Լ�����"Low"��
*   "Virtualized" Ϊ Yes
*   ����ģʽ�·��ʵ��ļ�Ŀ¼(�����Լ�����"Low")��
*     1.IE��ʱ�ļ�: %UserProfile%\AppData\Local\Microsoft\Windows\Temporary Internet Files\Low
*     2.ϵͳ��ʱĿ¼: %UserProfile%\AppData\Local\Temp\Low
*     3.Cookies: %UserProfile%\AppData\Roaming\Microsoft\Windows\Cookies\Low
*     4.��ʷ: %UserProfile%\AppData\Local\Microsoft\Windows\History\Low
*     5.�ղؼ�: �ղؼ�Ĭ������ͼ����IE���̷��ʣ���˲���Ҫ������Ŀ¼��
*   ������������⣺
*     1.ͨ������NTFS������(Soft Link)�ķ��������IE����ģʽ�ļ��������⣺linkd ԴĿ¼ ��Ȩ�޵�Ŀ��Ŀ¼
*       ʵ�ʣ���NTFS�ļ�����һ���ؽ�����(REPARSE)������
*       Linkd ��ִ�г����� Windows Server 2003 Resource Kit Tools���߰� ��
*     2.SHGetSpecialFolderPath(NULL, szSavePath, CSIDL_LOCAL_APPDATA , TRUE); //ȡ�� %UserProfile%\AppData\Local
*       CString strFilePath(szSavePath); 
*       strFilePath += TEXT("\\Temp\\Low\\nCapture\\IEToobar\\");  -- ֮��ķ��ʶ��ھ��� Low �����Եȼ���Ŀ¼��
* 
* �������(��Ȩ����˼��) -- �Ѵ�ͳ��IE����һ��Ϊ�����ֱ���ͬ��ȫ�ȼ�������.
*   IE���̱��� -- �����õġ�������Ȩ����ҳ�������
*   IeUser.exe(�û����������) -- ����е���Ȩ������(���籣��ͼƬ��)
*   IeInstal.exe(�����������) -- ��ɸ���Ȩ������(���簲װ�������ҪдINF�ļ�?)��
* 
* IE�еĶ���̿���
*   HKLM��HKCR \SOFTWARE\Microsoft\Internet Explorer\MAIN\TabProcGrowth  -- REG_SZ, Ϊ 0 ��ʾ�� Ϊ Medium ��ʾ
*
********************************************************************************************/

/********************************************************************************************
* IWebBrowser2::Navigate �� PostData �Ǹ� VT_ARRAY �� Variant������ MakeNavigatePosData��δȷ�ϣ�
*  
********************************************************************************************/

/********************************************************************************************
* �����ҳʱ���¼�˳��(DISPID_XXX)
*   DISPID_ONVISIBLE(OnVisible)
*   DISPID_BEFORENAVIGATE2(BeforeNavigate2)
*   DISPID_DOWNLOADBEGIN(DownloadBegin)
*   DISPID_COMMANDSTATECHANGE(CommandStateChange) -- ָʾ Toolbar/Forward/Back �Ȱ�ť��״̬�ı�
*   DISPID_STATUSTEXTCHANGE(StatusTextChange) -- ״̬���ı��ı�,��"������������Ϣ"��
*   DISPID_PROGRESSCHANGE(ProgressChange) -- ���ȸı�
*   DISPID_FILEDOWNLOAD(FileDownload) -- ָʾ��ʼ�����ļ�������ͨ�� bCancel ȡ��
*   DISPID_DOWNLOADCOMPLETE(DownloadComplete) -- ������ʱ�������ʧ��ʱ) -- ������������Σ��� NavigateComplete2 �Ĺ�ϵ��˳��
*   DISPID_SETSECURELOCKICON(SetSecureLockIcon) -- ���ܵȼ��仯
*   DISPID_TITLECHANGE(TitleChange) -- ����ı�
*   DISPID_NAVIGATECOMPLETE2(NavigateComplete2) -- ��������(Ϊʲôû�з��� NavigateComplete?)
*   DISPID_PROGRESSCHANGE(ProgressChange) -- ���ȸı�(ProgressMax/ProgressMax ��Ȼ�� -1/ProgressMax ��ʾ��ȫ���� )
*   DISPID_DOCUMENTCOMPLETE(DocumentComplete) -- �ĵ���ȫ����(һ��������¼��н��к�������), 
*     Url �ǹ淶�����URL��Ҳ���ܾ����������ض�λ��ģ���û��frame��ҳ���У�������ɺ�ֻ����һ�Σ��ж��frameʱ��ÿ��frame��������
*     highest frame �������յ��¼�����ʱ pDisp ָ�� highest frame��
********************************************************************************************/

namespace FTL
{
    class CFIEUtils
    {
    public:
		//��Window��������ȡIE�ڵ�Docuemnt�ӿ�
		FTLINLINE static HRESULT GetIEDocumentFromHWnd(HWND hWnd, IHTMLDocument** ppDocument);

		FTLINLINE static BOOL IsProtectedModeProcess();

		FTLINLINE static BOOL MakeNavigatePosData(LPCTSTR pszPostInfo)
		{
			if (!pszPostInfo)
			{
				return FALSE;
			}
			int nSrcLength = lstrlen(pszPostInfo);
			VARIANT vtPost = {0};
			::VariantInit(&vtPost);

			LPSAFEARRAY psa = SafeArrayCreateVector ( VT_UI1, 0, nSrcLength );
			if (psa)
			{
				LPSTR pszPostData = NULL;
				SafeArrayAccessData ( psa, ( void** ) &pszPostData );
#ifdef _UNICODE
				USES_CONVERSION;
				int nLength = WideCharToMultiByte ( CP_ACP, 0, pszPostInfo, 
					nSrcLength, NULL, 0, NULL, NULL );
				CopyMemory ( pszPostData, CT2A ( pszPostInfo ), nLength );
#else
				CopyMemory ( pszPostData, pszPostInfo, nSrcLength );
#endif
				SafeArrayUnaccessData ( psa );
				V_VT ( &vtPost ) = VT_ARRAY | VT_UI1;
				V_ARRAY ( &vtPost ) = psa;

				//CFVariantInfo varInfo(vtPost);
				//FTLTRACE(TEXT("vtPost=%s\n"), varInfo.GetConvertedInfo());

				//ʹ�� vtPost
				 //pWebBrowser->Navigate ( bsURL, &varFlags, &varTarget, &vtPost, &varHeader );

				VariantClear ( &vtPost );
				SafeArrayDestroyData ( psa );
			}

		}
    };

}//namespace FTL

#ifndef USE_EXPORT
#  include "ftlIEHelper.hpp"
#endif

#endif //FTL_IE_HELPER_H
