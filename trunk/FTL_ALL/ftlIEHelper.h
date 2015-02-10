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
//����ͨ��Fiddle����Metro������Ĺ���: http://blogs.msdn.com/b/fiddler/archive/2011/12/10/fiddler-windows-8-apps-enable-loopback-network-isolation-exemption.aspx

//Cross-Site-Request-Forgery (CSRF)

/****************************************************************************************************************************
* AppContainers (ɳ��) -- ��������Ӧ��ʹ�õ�����(TODO:��ô����? ��չDLL�ܶ�����?)
* App capability declarations -- https://msdn.microsoft.com/en-us/library/windows/apps/hh464936.aspx
*   internetClient
*   location
*   sharedUserCertificates
*   
*   internetClientServer -- ������Ϊ������,�ܽ�����������
*   privateNetworkClientServer -- ���Է���Intranet��Դ, (EPMʱû������������,��˿ɱ��� CSRF(Cross-Site-Request-Forgery) �� Intranet Port Scanning) 
*   enterpriseAuthentication
*   musicLibrary
*   picturesLibrary
*   videosLibrary
*   documentsLibrary
*   
* 
****************************************************************************************************************************/

/****************************************************************************************************************************
* TODO: 
*   1.Protected Mode �� Enhanced Protected Mode  ������?
*     PM -- ��������������͵�Sites�ֱ����û����
*     EPM -- �������IE�������������
*   2.MotleyFoolĿǰ������, ��ͬʱ���� PM + EPM ���޷���ʾ��Ϊʲô?ǩ��?
*   3.ͨ�� Delete Browser History ����ɾ��EPM��ص�������Ϣ, ������ʾ?
* 
* ����ģʽ(Protected Mode): Internetѡ��=>��ȫ=>ѡ�������"���ñ���ģʽ"
*   �����Ƶ�վ�� -- �������
*   Internet -- ���ƽ϶�, ����ͨ�� 127.0.0.1 ����ʱ
*   ����Intranet(Local Intranet) -- ���ƽ���(Medium), ����ͨ�� localhost ����ʱ
*   �����ε�վ��(Trusted Zones) -- ��������
* ��ǿ����ģʽ(Enhanced Protected Mode):Internetѡ��=>�߼�=>��ȫ=>������ǿ����ģʽ
*   ���е�Content Process Ϊ64Bit, �������ڱ���ģʽ, Win8�Ժ�������AppContainer��.
*   ���ܽ����������󣬲������ӱ��ط���(loopback) -- ����ͨ��Fiddler����
* AppContainer(windows_ie_ac_001) -- Win8�Ժ�, ����EPMʱ Content Process��һ��ɳ��(sandboxed)������ʽ?
*   ����AppContainer�ȵ���Դ���� -- �� Cookies/Cache/Web Storage/IndexedDB ��, �������ͬһ����վ�ж�� cookies ʵ��(Low/Medium/AppContainer)
*   ������ internetClient, location, sharedUserCertificates ������?
*   AppContainer �����������ͺܶ෽�涼�й�ϵ: OS(32/64); Win�汾; IE�汾������(Desktop|Metro); PM|EPM ������; ���ʵ���ַ����(Internet|Intranet|Trusted); 
*      ��������(��ͥ|����|����); ����ǽ ��
* LowIL
****************************************************************************************************************************/


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
* EPM -- (�Ѷ���)http://blogs.msdn.com/b/ieinternals/archive/2012/03/23/understanding-ie10-enhanced-protected-mode-network-security-addons-cookies-metro-desktop.aspx 
*        �ظ����� tommcg1 1 Mar 2013 11:54 AM 
*        https://msdn.microsoft.com/en-us/library/bb250462(v=vs.85).aspx
*        https://msdn.microsoft.com/en-us/library/ie/dn519894(v=vs.85).aspx
* 
* Win8 -- ���ֲ�ͬ���û��ӿڣ���Ҫ�ֱ����в�ͬ���͵�Ӧ�ó�����IE�ж�Ӧ�� Metro style �� Desktop style)
*   1.���� -- ����Ӧ�ó���, 64Bit�� 64Bit Manager + 32Bit Content Process
*   2.Metro -- ר�ŵ� Metro ����� Metro-style enabled desktop browsers (MEDB), ������AppContainer�С�ͨ�� HTML+JavaScript ����.
*              Metroģʽ�µ�IEֻ��Flash,Silverlight,Java ���?
*              ���ܰ�װ�������������κβ��, Toolbar, BHO, non-browser-platform COM objects(MIME Handler, URLMon Protocol Handler,ActiveX Controls) ��
*              64Bit��ֻ��64Bit Content Process, 
*              ע�⣺������"View on the Desktop" ������ѡ��,��push ��ǰ��session cookies(����push persistent cookies)��������ʽ��IEʵ����Ȼ�������ͬ��վ
*     WinRT -- ARMƽ̨�ϵ�Window8��ֻ�����д����Metro���򣬲���ʹ��Win32API����Ҫʹ��ר�ŵ� WinRT API?
*
* <iepmapi.h> -- IE Protected Mode API, ������ IEIsProtectedModeProcess �Ⱥ����Ķ���(�����ǵ� WinXP �ȵ����У���ö�̬����)
* EPM(Enhanced Protected Mode) -- IE10����, ���ú�ܶ������޷���ȷ���С�Metro���͵�ʼ��������EPM, Desktop���͵Ŀ�ѡ(Internet Options->Advanced),
*   Web��վ�Ŀ������� Http Header ��ͨ��"X-UA-Compatible:requiresActiveX=true" ѡ��Ҫ��plugin,�û���õ���ʾ�������������� "Low IL Compat" ������
*   Win8.0Ĭ��Disabled, Win8.1Ĭ��Enabled, IE11�����ָ�ΪĬ�Ͻ���
*   Win7�ϵ�IE10,�������EPM,ֻ�� ��64λContent Process(û�� AppContainer ��ɳ��)
*   TODO:EPM ����ʱ����ʹ�� AdminAPI ?
*   BHO|Toolbar��EPM������ʱ, ����ʾ " 'Xxx' isn't compatible with Enhanced Protected Mode and has been disabled.
*
*   ����ʱ: ��IE������ AppContainer �� Integrity Level, ��Ϊ Metro Style, �ܶ��Ȩ�޵Ĺ��ܻᱻDeny(Create global event �Ⱥ������� Access denied)
*   �ر�ʱ: ��IE������ Low �� Integrity Level, �� 
*   TODO: ��IE������ Medium �� Integrity Level
*   ԭ��:64λ���̵ĸ߼��ڴ汣��(��crackǰcrash + ���ٶ�ջ����Ŀ�����?) + �����Լ��� + AppContainers(ɳ��)
*        ASLR -- ��ַ�ռ䲼�������, ͨ�� /DYNAMICBASE ����
*   �������(TODO:��δʵ�ʲ���):
*     0.��װ 32 λ�� 64 λ�������ļ� + �����ע��Ϊ CATID_AppContainerCompatible ��� -- ������EPM����ʱ�������?
*       ������Դ���ʴ���Ľ��: ����ͨ��������Դ�İ�ȫ���ʿ����б� (SACL) ������ IE AppContainer �İ�ȫ ID (SID) �Խ��������.
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
*     6.ע���: HKEY_CURRENT_USER\Software\AppDataLow\Software ��
*     TODO: %userprofile%/AppData/LocalLow , ��ͨ�� SHGetKnownFolderPath(FOLDERID_LocalAppDataLow) ��ȡ,�ƺ���ͬ��Tab�᷵��ͬ�Ľ��? 
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
* IE�еĶ���̿���: 
*   Manager|Frame process -- Medium(64Bit),���������κε�����������
*   Content|Tab Process -- Low(Protected Mode) or Medium(32Bit, Not Protected Mode or Intranet) -- 
*     ���е� HTML, ActiveX, Toolbar(�������ƺ���Manager process) �ȶ��������� Content Process �е�
*     TODO:��������ʵ�վ���йصĻ������ǽ��̵�Ȩ�޻���ʱ��?
*
*   HKCR\SOFTWARE\Microsoft\Internet Explorer\Main\TabProcGrowth  -- REG_SZ, Ϊ 0 ��ʾ������? Ϊ Medium ��ʾ�����?
*     TODO:��Ҫ����������Ч?
* 
* ��غ���
*   IEIsProtectedModeURL -- 
*   IEIsProtectedModeProcess  -- �ж��Ƿ������ڱ���ģʽ��
*   IEGetWriteableFolderPath -- ��ȡָ�����͵�Ŀ¼��ַ��ע��: ��Ϊ EPM �����͹ر�ʱ��õ�ַ��һ��������һ�㲻��ͨ������д�ļ���
*   IELaunchURL -- ʹ�ú��ʵ�����������IE����URL(��Ҫʹ�� CreateProcess ����IE?)
*   IESaveFile -- �����ļ���IEShowSaveFileDialog���ص�·����? TODO: ��Ҫ����  %userprofile%/AppData/LocalLow �д���һ����ʱ�ļ�? Ȼ�󱣴�ʱָ�����ļ�?
*   IEShowSaveFileDialog -- ʾ�û������ļ�����·��(����������������?)
*   TODO: �� AppContainer �ĺ���?
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
    enum IETabIntegrityLevel{
        ilAppContainer,
        ilLowCompat,  //��ҳҪ�� ActiveX control which is not EPM-compatible ʱ
        ilLow, 
    };

    class CFIEUtils
    {
    public:
		//��Window��������ȡIE�ڵ�Docuemnt�ӿ�
		FTLINLINE static HRESULT GetIEDocumentFromHWnd(HWND hWnd, IHTMLDocument** ppDocument);

		FTLINLINE static BOOL IsProtectedModeProcess();
        
        FTLINLINE static IETabIntegrityLevel GetIETabProcessIntegrityLevel(HANDLE hProcess);

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
