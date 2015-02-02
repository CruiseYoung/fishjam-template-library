#ifndef FTL_IE_HELPER_H
#define FTL_IE_HELPER_H

#pragma once

#ifndef FTL_BASE_H
#  error ftlIEHelper.h requires ftlbase.h to be included first
#endif

#ifndef FTL_COM_H
#  error ftlIEHelper.h requires ftlCom.h to be included first
#endif

//����ڱ���ģʽ��Internet Explorer�Ĺ�����ʽ --  http://blog.csdn.net/xt_xiaotian/article/details/5336809
//  ԭ��:https://msdn.microsoft.com/zh-cn/library/bb250462(en-us,VS.85).aspx

//IEBlog �������� http://blogs.msdn.com/b/ie_cn/

/****************************************************************************************************************************
* TODO:
* 1.IELowutil.exe -- Internet Low-Mic Utility Tool, �ƺ�����ͨ��COM����?
* 2.Ӧ�ó�������Թ��߰�(ACT) -- �ƺ�ֻ���XP?
*
* Mandatory Label
*   AppContainer
*   mandatory --
*   Medium
*   
****************************************************************************************************************************/

/****************************************************************************************************************************
* http://blogs.msdn.com/b/ieinternals/archive/2012/03/23/understanding-ie10-enhanced-protected-mode-network-security-addons-cookies-metro-desktop.aspx 
* http://blogs.msdn.com/b/ieinternals/archive/2012/03/23/understanding-ie10-enhanced-protected-mode-network-security-addons-cookies-metro-desktop.aspx
*
* Win8 -- ���ֲ�ͬ���û��ӿڣ���Ҫ�ֱ����в�ͬ���͵�Ӧ�ó�����IE�ж�Ӧ�� Metro style �� Desktop style)
*   1.���� -- ����Ӧ�ó���
*   2.Metro -- ר�ŵ� Metro ����� Metro-style enabled desktop browsers (MEDB).Metroģʽ�µ�IEֻ��Flash��������ܰ�װ�������������κβ��?
*     WinRT -- ARMƽ̨�ϵ�Window8��ֻ�����д����Metro���򣬲���ʹ��Win32API����Ҫʹ��ר�ŵ� WinRT API?
*
* <iepmapi.h> -- IE Protected Mode API, ������ IEIsProtectedModeProcess �Ⱥ����Ķ���(�����ǵ� WinXP �ȵ����У���ö�̬����)
* EPM(Enhanced Protected Mode) -- ���ú�ܶ������޷���ȷ���С�Metro���͵�ʼ��������EPM, Desktop���͵Ŀ�ѡ(Internet Options->Advanced),
*   Web��վ�Ŀ������� Http Header ��ͨ��"X-UA-Compatible:requiresActiveX=true" ѡ��Ҫ��plugin(��Ҫ�󾭵�ģʽ?)
*   Win8.0Ĭ��Disabled, Win8.1Ĭ��Enabled, IE11�����ָ�ΪĬ�Ͻ���
* 
*   ����ʱ: ��IE������ AppContainer �� Integrity Level, ��Ϊ Metro Style, �ܶ��Ȩ�޵Ĺ��ܻᱻDeny(Create global event �Ⱥ������� Access denied)
*   �ر�ʱ: ��IE������ Low �� Integrity Level, �� 
*   TODO: ��IE������ Medium �� Integrity Level
*   �������: 
*     1.����Local COM Server ���͵�Broker, BHO(AppContainer) <=> Broker(Medium) <=> ��Ȩ��Process(?)
*       TODO: ? Broker����ص�dll��Ҫ����ALL_APPLICATION_PACKAGESȨ��? ��ͨ�� ConvertStringSidToSid("S-1-15-2-1") + AddAceToObjectsSecurityDescriptor ������?
*     2.��Ӧ��BHO,Broker ����Ҫǩ��;
*     3.Broker.exe ��Ҫע�ᵽ HKLM\SOFTWARE\Microsoft\Internet Explorer\Low Rights\ElevationPolicy ���������̵��е�������
*       Policy: 3(�е������Ծ�Ĭ����), 2(��ʾ�û������е�����������),1(�������Ծ�Ĭ����), 0(��ֹ����)
*       ��������ǿ�ִ���ļ�: AppName(��ִ�е������ļ����ļ���) + AppPath(��װ·��)
*       ���������COM����: CLSID
*     ע��: 8.1�Ļ�����Ҫ��CLSIDע��AppContainer��category�����������ݣ����⻹Ҫע��64λ��ActiveX
*   �������ҳ��ק��ָ������(������ʾ):
*     HKEY_LOCAL_MACHINE/SOFTWARE/Microsoft/Internet Explorer/Low Rights/DragDrop
*
* IE 7 �ı���ģʽ(Protected Mode) -- Ҳ��IE��Ȩ��(Low Rights) -- TODO: Win7��ʵ�ʲ���IE10, �����۲�ͬ
*   ͨ�� Process Explorer �鿴���̵�"��ȫ"���ԣ��ɿ�����
*   "Mandatory Label\Low Mandatory Level"��SID(��Ӧ�ı�־λ��"Integrity" -- ����)�������IE���̵������Լ�����"Low", "Virtualized" Ϊ Yes
*   ��ͨ������:"Mandatory Label\Medium Mandatory Level", "Virtualized" Ϊ No
*   ����ģʽ�·��ʵ��ļ�Ŀ¼(�����Լ�����"Low")��
*     1.IE��ʱ�ļ�: %UserProfile%\AppData\Local\Microsoft\Windows\Temporary Internet Files\Low
*     2.ϵͳ��ʱĿ¼: %UserProfile%\AppData\Local\Temp\Low
*     3.Cookies: %UserProfile%\AppData\Roaming\Microsoft\Windows\Cookies\Low
*     4.��ʷ: %UserProfile%\AppData\Local\Microsoft\Windows\History\Low
*     5.�ղؼ�: �ղؼ�Ĭ������ͼ����IE���̷��ʣ���˲���Ҫ������Ŀ¼��
*     TODO: %userprofile%/AppData/LocalLow , ��ͨ�� SHGetKnownFolderPath(FOLDERID_LocalAppDataLow) ��ȡ 
*   ������������⣺
*     1.ͨ������NTFS������(Soft Link)�ķ��������IE����ģʽ�ļ��������⣺linkd ԴĿ¼ ��Ȩ�޵�Ŀ��Ŀ¼
*       ʵ�ʣ���NTFS�ļ�����һ���ؽ�����(FILE_ATTRIBUTE_REPARSE_POINT)������
*       Linkd ��ִ�г����� Windows Server 2003 Resource Kit Tools���߰� ��
*     2.SHGetSpecialFolderPath(NULL, szSavePath, CSIDL_LOCAL_APPDATA , TRUE); //ȡ�� %UserProfile%\AppData\Local
*       CString strFilePath(szSavePath); 
*       strFilePath += TEXT("\\Temp\\Low\\nCapture\\IEToobar\\");  -- ֮��ķ��ʶ��ھ��� Low �����Եȼ���Ŀ¼��
*   TODO:
*     1.PhotoViewer ���� HKLM\SOFTWARE\Microsoft\Internet Explorer\Low Rights\ElevationPolicy ��д��Ϣ(Policy = 3 ��)���ƺ���Ϊ�˷���Ϣ?
* 
* �������(��Ȩ����˼��) -- �Ѵ�ͳ��IE����һ��Ϊ�����ֱ���ͬ��ȫ�ȼ�������.
*   IE���̱��� -- �����õġ�������Ȩ����ҳ�������
*   IeUser.exe(�û����������, User Broker) -- ����е���Ȩ������(���籣��ͼƬ��)
*   IeInstal.exe(����������� - Admin Broker) -- ��ɸ���Ȩ������(���簲װ�������ҪдINF�ļ�?)��
* 
* IE��������ͼ(CV)�б� -- ά��ϵͳ�µ���վ��IE���ݵ���Ϣ
*   %LocalAppData%\Microsoft\Internet Explorer\IECompatData\
*     ie9CompatViewList.xml IE9)
*     iecompatdata.xml (IE10)
*   ����ģʽ
*   Windows UI ģʽ�µ�IE
*   
* 
* IE�еĶ���̿���
*   HKLM��HKCR \SOFTWARE\Microsoft\Internet Explorer\MAIN\TabProcGrowth  -- REG_SZ, Ϊ 0 ��ʾ������? Ϊ Medium ��ʾ�����?
*
* ��غ���
*   IEIsProtectedModeURL -- 
*   IEIsProtectedModeProcess  -- �ж��Ƿ������ڱ���ģʽ��
*   IEGetWriteableFolderPath -- ��ȡָ�����͵�Ŀ¼��ַ��ע��: ��Ϊ EPM �����͹ر�ʱ��õ�ַ��һ��������һ�㲻��ͨ������д�ļ���
*   IELaunchURL -- ʹ�ú��ʵ�����������IE����URL(��Ҫʹ�� CreateProcess ����IE?)
*   IESaveFile -- �����ļ���IEShowSaveFileDialog���ص�·����? TODO: ��Ҫ����  %userprofile%/AppData/LocalLow �д���һ����ʱ�ļ�? Ȼ�󱣴�ʱָ�����ļ�?
*   IEShowSaveFileDialog -- ʾ�û������ļ�����·��(����������������?)
****************************************************************************************************************************/

/****************************************************************************************************************************
* IWebBrowser2::Navigate �� PostData �Ǹ� VT_ARRAY �� Variant������ MakeNavigatePosData��δȷ�ϣ�
* IDownloadManager -- ΢���ṩ�Ľӿڱ�׼���û�����ʵ�ָýӿڷ���(ͨ�� QueryService �ṩ�����ʹ��)��
*   �Ӷ�ʵ�ֶ��ƻ������ع�����(����ʾ��Ƶ?)
*  
****************************************************************************************************************************/

/****************************************************************************************************************************
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
****************************************************************************************************************************/

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
