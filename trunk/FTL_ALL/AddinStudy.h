#ifndef ADD_IN_STUDY_H
#define ADD_IN_STUDY_H
#pragma once

//�����ĵ�
//Visual Studio 2008 ����չ�Կ��� -- http://www.cnblogs.com/anderslly/archive/2009/02/23/visualstudio-overview.html
//LearnVSXNow -- http://www.cnblogs.com/default/archive/2010/02/26/1674582.html

//CodeBlocks -- ��Դ�Ŀ�ƽ̨ C++ IDE������ѧϰ��ص�֪ʶ
//  ������ ICodeCompletionItemsProvider �ӿ�?

//Visual Assist(us$249) -- http://www.wholetomato.com
//VSAid(Freeware) -- http://www.brocksoft.co.uk/vsaid.php 
//PhaStudio(OpenSource,C#) -- http://phatstudio.sourceforge.net
//Visual Studio Flat Solution Explorer(OpenSource, C++) http://vsflatsolutionexp.codeplex.com/
//DPack(Freeware) -- http://www.usysware.com/dpack

//DTE�� ProjectItem -> FileCodeModel -> CodeElement->CodeElementChildren ?
//CAtlDllModuleT ������ DECLARE_LIBID(idl��library��UUID) -- ���� ?
//   ��Ҫ��ͨ�� DECLSPEC_UUID ������Ӧ��UUID
//idl �� library(1) -> coclass(n),��ͨ�� CoCreateInstance ���� -> interface(n)
//��򵥵�COM�����
//  1.��Ҫ�� CComObjectRootEx<�߳�ģ��>, CComCoClass<����,CLSID_XXX> �̳�
//  2.DECLARE_REGISTRY_RESOURCEID(.rgs�ļ���Ӧ��ResourceID)
//  3.BEGIN_COM_MAP/END_COM_MAP ���ýӿ�ӳ��
//  4.OBJECT_ENTRY_AUTO(CLSID_XXX, ����) ӳ��

//  COM_INTERFACE_ENTRY2 -- ���ʹ��(���IDispatch? ) COM_INTERFACE_ENTRY2(IDispatch, IDTExtensibility2)
//
//SetScrollPos(m_handles.vert, SB_CTL, line, TRUE); -- ʹ�� SB_CTL ��ʲô��˼? 
/*********************************************************************************************************
* ����
*   1.C# ����һ�� CommandBar.resx �ļ��� C++/ATL ��Ӧ����ʲô? ʹ�� CultureInfo ����������ԣ�C++/ATL���? 
*   2.����Ŀ��(IDTCommandTarget)��ִ��(Exec)��״̬��ȡ��ͨ���Ƚ��ַ����ķ�ʽ��ִ�е�,��û�и��õķ�ʽ��
*
*
* ��������Ҫ import �� libid
*   AC0714F2-3D04-11D1-AE7D-00A0C90F26F4(1.0) -- AddInDesignerObjects(MSADDNDR.tlh)
*     ��Ҫ���� _IDTExtensibility2(DTE Addin �Ļ������ӿ�) �Ƚӿ�
*     public IDispatchImpl<_IDTExtensibility2, &IID__IDTExtensibility2, &LIBID_AddInDesignerObjects, 1, 0>,
*   80CC9F66-E7D8-4DDD-85B6-D9E6CD0E93E2(8.0) -- EnvDTE(Environment Development Tools Extensibility)
*     ��������������չ, ��ӦVS2005֮ǰ��DTE�汾(�� DTE �ӿ�), EnvDTE[x0]�������Զ�������ģ��(AOM--Automation Object Model)�е�����
*       Solution(�����������Ŀ)
*       Command(����)��������ṹ
*         �����ߣ��˻��ӿڣ��� �˵�/������/��ݼ� ��
*           => ����(Command), ���״̬(���Ƿ�ɼ������õ�)����ֱ��ִ�л�ת������Ŀ��
*             => ����Ŀ��(IDTCommandTarget),ʵ�־���Ķ��������ı��༭���кϽ�������ϵ� Edit.Copy ����ִ�����ݲ���ͬ
*         DTE -> get_CommandBars(��ȡ���˵��ӿ�) -> get_Item("MenuBar") -> get_Controls -> get_Item("Tools")��ȡCommandBarPopup
*         VS���õĲ˵���ղ˵���Ƕ�׹�ϵ������������ "Edit.Copy" ��ʾEdit�µ�Copy��(�пո�Ļ���Ҫȥ��)��
*           �Զ���Addin�Ĳ˵�����Ϊ��ProjectName.ClassName.CommandName(�� FtlLogAddin.Connect.DoLogAddin)
*         VS ��������˵���MenuBar�� 
*         Doc ���Ҽ��˵���Easy MDI Document Window(����Ϊ EzMdiTabs)
*         ͨ�� DTE::ExecuteCommand(����",xxx) ����ִ��VS���еı�׼����(�� Window.CloseAllDocuments )
*       �¼�
*       �ĵ�
*       ������
*   1A31287A-4D7D-413E-8E32-3B374931BD89(8.0) -- EnvDTE80, ��Ӧ��VS2005��DTE(ֻ�����仯�˵İ汾���� DTE2 �ӿ�)����Ӧ�Ļ��� EnvDTE90 ��
*   1CBA492E-7263-47BB-87FE-639000619B15(8.0) -- Microsoft_VisualStudio_CommandBars
*   xxxxx -- Extensibility (C#�в���?) -- ������IDTExtensibility2ʹ�õ�����
*
* ContextUIGUIDs -- VS������һЩGUIDֵ����ʶ��״̬�ĸı䡣ͨ�������еĸò�������ָ���Ƿ���ָ����״̬��(vsContextGuidDebugging)���õ�
*********************************************************************************************************/

/*********************************************************************************************************
* ��д DTEʱ������ error C2872: 'ULONG_PTR' : ambiguous symbol ����
*   �����������DTE �����Ϳ�(libid:80cc9f66-e7d8-4ddd-85b6-d9e6cd0e93e2)ʱ����ULONG_PTR������
*   �������� rename("ULONG_PTR","ULONG_PTRDTE") ��
*   Ȼ����Ҫע��ͷ�ļ��İ���˳�򣬲�Ҫ�� Connect.h �� using namespace XXX ���ٰ�����Щ���ͻ���ļ�(������ǰ��)
* 
* ����
*   C#�� Addin��Ŀ����һ�� ${ProjectName}-For Testing.AddIn �ļ�������ļ���ŵ�λ����
*     ${My Documents}\Visual Studio 2008\Addins����F5����ʱ��VSʹ�ø��ļ����м��ء�
*     ������Ϻ󣬽� Testing.AddIn ɾ���������ɵĳ��򼯺ͽ���ļ��ŵ� {$My Documents} ��ӦĿ¼�¼������򵥵Ĳ���?
*
* Add-in �еĶ��߳�
*   1��ѭCOM���̴߳��ݽӿڹ�����Ҫ��DTE�Ľӿ� Marshal �����߳��У�Ȼ�󼴿�����ʹ��
*   2.���߳�ͬʱ����ͬһ���ӿ�ʱ��ͬ�����⣿�Զ�ͬ����
*********************************************************************************************************/


/*********************************************************************************************************
* http://www.cnblogs.com/sifang2004/archive/2006/06/26/436076.html
*
* Visual Studio �ṩ�˶��ֲ�ͬ�������չ��
*   Macro(��)--��VS�е�һЩ�ظ������Զ�������¼�ƺ��ֹ���д�ꡣ
*     ע�⣺��ǰֻ����VB.NET(VBA?)�������꣬�� Add-In ������ͬ��API��ͨ��¼�ƺ겢�����ɰ���Add-In����
*   Add-In(���)--����IDE�ĵײ�API�����Խ�һЩ������������Զ������Ӷ���չIDE��
*     ���Բ��������������Ŀ���ļ����༭����VS�еĸ������ڵȡ�
*   VsPackage--ͨ��VS SDK���������ԴﵽIDE�ͱ������֮������εļ���
*   Visualizer--
*   VS Shell--
*   Code Snippet--
*   Project/Item Template
*   MSBuild--
*   Visual Studio��ҵ������� (VSIP) �ƻ���
*   ��ӳ���
*
* Developer Studio Objects 
*
* IDE�е���Ҫ����
*   Solution Explorer(���������Դ������)--�鿴�͹����������ڵĸ�����Ŀ����Ŀ���������ļ�s
*   Property Window(���Դ���)--�鿴��ǰѡ���������
*   Toolbox(������)--���������ɿؼ����б�
*   Server Explorer(������������)--�鿴�͹����ػ�Զ�̵ķ�����
*   Class View(�������)--��ʾ�˽�������ڵ����п�������,�������ǰ������ռ����
*   Error List(�����б�)--��ʾ���󡢾������Ϣ��
*   Output Window(�������)--
*   Task List(�����б�)--�������������, һ�� "//TODO:xxx"; ��һ�����û��Զ�������
*   Object Browser(���������)--�鿴������ص��������������������Ϣ
* 
* ��ӳ���
*   ����һ���������:Other Project Types => Extensibility => Visual Studio Add-in => "ѡ��Ӧ�ó�������"��
*   �� COM ��������ʵ�� IDTExtensibility2 �ӿڣ���ͨ�� EnvDTE �� EnvDTE80 �����а����ĺ����Զ�������ģ���� IDE ͨ�š�
*   ������ģ�Ͱ����� Microsoft.VisualStudio.CommandBars �С�ʹ�õ�����Ҫ�������DTE���������������ܱ�̿�����
*   Visual Studio�е��κζ������ӹ�������װж���ߴ��ڣ������༭�ļ��ͳ�ʼ�����롣
*
*   �ӿڶ����ļ� 
*     AddIn.idl
*
*   �����ļ�(C#����XML��ʽ�� ${ProjectName}.AddIn ),Add-Inͨ������ļ���VS����ע��, <Version> �ڵ���"*"��ʾ֧�����а汾
*     AddIn.rgs -- ���е� LoadBehavior ��������Ϣ�����뵽��ѡ��� �������������ط�ʽ��
*       LoadBehavior -- 0(���Զ�����),1(VS����ʱ����),4(ͨ�������м���)
*       CommandPreload -- ָ��Add-In�Ƿ�Ӧ��Ԥ�ȼ���
*       CommandLineSafe -- ָ��Add-In�Ƿ��������а�ȫ���Լ��Ƿ���ʾ�û�����
*   
*
**********************************************************************************************************/
 
/**********************************************************************************************************
* C++/ATL ��ʽ��Addin������
*   Connect.h -- Ĭ��ʵ���� IDTCommandTarget �� IDTExtensibility2 �����ӿ�
* 
* ���ýӿ�
*   IDTExtensibility2 -- ������Add-In��IDE֮�����ͨ�ţ�������е� custom ���ǿ����� ? 
*     OnConnection : �ڼ���Add-Inʱ����,����� Application ��AOM�ĸ�����
*       ע�⣬��ʱ������Щ��Ҫ��VS�����δ�������
*     OnStartupComplete : ����������������ɵ�ʱ�򣬱��뽫AddIn����Ϊ����ʱ���زŻᴥ�����¼�
*     OnAddInsUpdate : ���κ�Add-In�ļ���״̬�����ı�(��AddIn Manager�иı���һ���������)��ʱ��
*     OnBeginShutdown : ���������������رյ�ʱ��
*     OnDisconnection : ��ж��Add-Inʱ����
*   IDTCommandTarget -- ���ѡ�������е�UIѡ�����Ҫʵ����
*     QueryStatus
*     Exec
*   
**********************************************************************************************************/

/**********************************************************************************************************
* MPF(Managed Package Framework) -- ΢����Visual Studio��COM interoperability����֮�ϴ����Ŀ�ܣ�
*   ���԰��������á������������йܴ���������VSPackage
*   
* VSIP(Visual Studio Integration Package����Ҫ���� vsguids.lib ) -- ��չVisual studio��ǿ��Ĺ��ߣ�
*   ����visual studio�Ĺ��ܾ��ǽ�������Visual studio��ǣ�shell��Ϊ���ĵ���չ���ϡ�
*   Package������Visual Studio��������������ڹ��ڶԻ�������ʾ���Լ�����Ϣ
*   ���е�Language���༭������������Project System�Լ������ܶ���������Package����չ��
*     ����Service�� -- �ṩ���ܹ������߻�������package���ã��� C#���Է���
*     ����Ԫ�� -- ����˵��������������ڵ�
*     �༭�� -- VisualStudio�� ���ı༭�� + ��չ���Զ���༭��
*     ����� -- ͨ���ǿ��ӻ����ߣ��� WinForm�����
*     ��Ŀ -- ��֯Դ�ļ�����Դ�����롢���Ժͷ�����Դ�ļ������Ĳ�Ʒ
*   Package���ص�
*     ������� -- ֻ�з��񱻵��õ�Package�Ż����
*     Site Package -- Package ��Ҫ SetSite������ͨ�������ʵ������Visual Studio����
*     Global Service -- �����Ѿ���site��IVsPackage���󣬼���ͨ��GetService����ȥ����ȫ�ַ���,
*       C#���о�̬�� Package.GetGlobalService ������ʹ�÷���ǰ��Ҫע�����(C#����ProvideServiceAttribute)
* 
*   ��Ҫ��װ Visual Studio 200X �� SDK(VSX SDK) -- http://msdn.com/vsx 
* 
* ����Service(IServiceProvider::QueryService) -- Visual Studio�ṩ��Add-in��Package�Ĺ��ܣ�PackageҲ���ṩ(Proffer)�Լ���Service
*   ע�⣺��Ϊһ��Service�����ṩ����ӿڣ�������õ�ϰ���ǵ�������һ�� Sxxx(�� SVsResourceManager) ��Ϊ Service ������
*   _DTE(SID_SDTE)
*   Help
*   ICQryAutoFactory
*   IHxRegisterSession
*   IHxSession
*   IInternetSecurityManager
*   ILocalRegistry/ILocalRegistry4(SID_SLocalRegistry) -- 
*   IOleComponentUIManager(SID_SOleComponentUIManager) --
*   OleMenuCommandService(IID_IMenuCommandService) -- δ����
*   IProfferService(SID_SProfferService) -- ��VS�ṩ�������Զ���� IVsTextMarkerTypeProvider
*     C# �ķ�����Ҫʵ�� Microsoft.VisualStudio.OLE.Interop.dll �е�IServiceProvider(ע�⣺���� System �е�),�� Package �̳м��ɡ�
*        ͨ�� ProvideService �����ṩȫ�ַ���[ProvideService(typeof(SMyService))]
*   IUIHostLocale
*   IVsAddProjectItemDlg
*   IVsAddWebReferenceDlg
*   IVsAppCommandLine
*   IVsBuildEventCommandLineDialogService
*   IVsCmdNameMapping
*   IVsCodeShareHandler
*   IVsCommandWindow
*   IVsComponentSelectorDlg
*   IVsdDeployment
*   IVsdDeployment
*   IVsDiscoveryService
*   IVsdPackageManager
*   IVsEnumHierarchyItemsFactory
*   IVsExternalFilesManager
*   IVsFileChange(SVsFileChange)
*   IVsFileChangeEx
*   IVsFilterKeys
*   IVsFontAndColorStorage
*   IVsHTMLConverter
*   IVsIntelliMouseHandler(SID_SVsIntelliMouseHandler) -- 
*   IVsInvisibleEditorManager
*   IVsLibraryReferenceManager
*   IVsLog
*   IVsMacroRecorder(SID_SVsMacroRecorder) -- ��¼����
*   IVsMenuEditorFactory
*   IVSMDCodeDomProvider(SID_SVSMDCodeDomProvider)
*   IVsMonitorSelection
*   IVsMonitorUserContext
*   IVsObjectManager[0-2](SID_SVsObjectManager)
*   IVsOutputWindow
*   IVsOutputWindowPane(SID_SVsGeneralOutputWindowPane) -- ��ȡ����Output�����е� General��������λ�ȡ? QI?
*   IVsParseCommandLine
*   IVsPropertyPageFrame
*   IVsQueryEditQuerySave2(SID_SVsQueryEditQuerySave)
*   IVsRegisterEditors(SID_SVsRegisterEditors) -- 
*   IVsRegisterFindScope
*   IVsRegisterProjectTypes
*   IVsRegisterScciProvider
*   IVsResourceManager(SID_SVsResourceManager) -- 
*   IVsRunningDocumentTable(SID_SVsRunningDocumentTable)
*   IVsShell(SID_SVsShell) --
*   IVsSolution(SID_SVsSolution)
*   IVsSolutionBuildManager
*   IVsSolutionPersistence
*   IVsStatusbar(SID_SVsStatusbar)
*   IVsStructuredFileIO
*   IVsTaskList
*   IVsTextImageUtilities
*   IVsTextManager(SID_SVsTextManager) -- ֧�� IVsTextManagerEvents �����ӵ�
*   IVsToolbox/IVsToolboxClipboardCycler(SID_SVsToolbox)
*   IVsUIHierWinClipboardHelper
*   IVsUIShell(SID_SVsUIShell) -- �ṩ�����ɷ���ȥʵ��������йصĹ���
*   IVsUIShellOpenDocument(SID_SVsUIShellOpenDocument) -- ��ͨ���� IsDocumentOpen(NULL, 0, bstrDocumentPath, LOGVIEWID_TextView,
*      0, &spVsUIHierarchy, &itemId, &spVsWindowFrame, &isOpen) �ķ�ʽ����ȡ�Ѿ��򿪵��ĵ��Ŀ��ƽӿڡ� bstrDocumentPath ��ͨ�� Document �Ȼ�ȡ
*   IVsWindowFrame(SID_SVsWindowFrame)
*   IVsXMLMemberIndexService
*   ObjectExtenders
*   OleMenuCommandService() -- �� AddCommand �ȷ���
*   SourceControl
*   STrackSelection
*   SVsHelpOnlineF1Service
*   SVsHelpPage
*   SVsLinkedUndoTransactionManager
*   SVsOnlineHelpData
*   SVsProfilesManagerUI
*   SVsSearchService
*   SVsStartPageDownload
*   SVsWebProxy
*
* �����ӿ�
*   IVsPackage -- ������ PackageRegistration �����ԣ� VSX������Ϊ Package �Ļ���ʵ���� IVsPackageImpl��
*     CLR��Package����ǿ��������ҪKey�ļ���Ϊ�����ܣ�Packageֻ������Ҫ�书�ܵ�ʱ��Ż���ء�
*     IVsPackage �� Site ���� IDE��Package����ͨ�� IServiceProvider �ӿڻ�ȡ������Services
*   CommandSet? -- ���
*  
* ��������
*   IVsPackageImpl -- 
*     �����صķ���(��Ҫֱ������ SetSite ����): 
*       void PostSited(IVsPackageEnums::SetSiteResult result)
*       void PreClosing()
* 
* ���ù���
*    $(RegitPath)\Regit.exe/RegPkg.exe -- Package��װ�Լ�ע�Ṥ�ߣ���ȡ Package �����еĸ���
*      Attribute(�� DefaultRegistryRoot/PackageRegistration) ��ע��
*
* ע��/ȡ��ע��
*   VSͨ��PLK��Package Load Key -- 128���ַ�������ɢ���룬��΢��վ�� http://msdn.microsoft.com/vstudio/extend/ ��ã�
*   ����鷢���汾����չ���Ƿ�Ϸ�(����ʱʹ��VSIP�ĵ������֤)��
*   C# ͨ�� ProvideLoadKey(��С�汾,�汾��,����,����������,PLK����ԴID) �����ṩ
*   PLK��Ҫ������������ɣ�
*     1. .rgs �ļ��е� CompanyName/ProductName/ProductVersion/MinEdition/ID(�� val ID  = d '%IDS_BASICPACKAGE_LOAD_KEY%') ��
*     2. VSL_REGISTRY_MAP_NUMBER_ENTRY(IDS_BASICPACKAGE_LOAD_KEY)
*     3. ������Դ�� IDS_BASICPACKAGE_LOAD_KEY(Hash���ַ���)��IDS_BASICPACKAGE_REGISTRY_NAME ��
* 
*   ȡ��ע��ʱʹ�� unregister �������ã������ VSDllUnregisterServer
*     0.����GUID�� guids.h �ļ��в�Ҫʹ�� #pragma once, �� DEFINE_GUID ����� CLSID_XXX ����Ҫ�� #include <initguid.h> ǰ�������һ��
*     �� Package ��ʵ���ļ��У�
*     1.#define DEFAULT_REGISTRY_ROOT XXXX �꣬ �� LREGKEY_VISUALSTUDIOROOT L"Exp"
*     2.#include <VSLPackageDllEntryPoints.cpp>
* 
*
*  Microsoft Visual Studio 2008 Experimental hive -- VS2008ʵ���ң�devenv.exe /rootSuffix Exp /RANU
*     ��ʹ�ö�����ע�����Ϣ���е��ԣ���ֹӰ��VS�Ŀ������� -- ͨ������ʱ�� regpkg ������?
*     /RANU -- run as normal user( ע�ᵽ HKEY_CURRENT_USER ?)
*     /setup -- ��VS������ʱ�Զ��������� ��װ��ж�ء�������
*     /NoVsip -- ģ��û�а�װ VS SDK �Ļ�������Ϊ����Ҫ������֤ PLK ?
*     /rootSuffix Exp -- ����Visual Studioʹ�� experimental registry hive
*     /root:Software\Microsoft\VisualStudio\9.0Exp --ָ��ע���ע����λ��
*     /Splash -- ��ʱ����ʾSplash���壬�Խ���ȷ��
*  vsct -- Visual Studio Command Table, VS2008�ж����û������XML������ɶ�������Դ����1000��Ϊ��ԴID��ӵ�VSPackage.resx��Դ�ļ��У�
*     regpkg.exeע��ʱ�Ὣ�������ע�ᵽע�����(λ��?)��VS����ʱֻ���ȡע����ɸ��½��棨�������DLL����
*    CommandTable -> Commands -> Groups  -> Group  ->
*                             -> Buttons -> Button -> Strings 
*                             -> Bitmaps -> Bitmap
*                 -> Symbols  -> GuidSymbol(����Guid�ķ�����)
* ctc -- Command Table Configuration, VS2005ʱ����UI��Ϣ���ı��ļ�
* 
* ���ɲ�����Ŀ
* ��Ԫ������Ŀ
*
* ToolWindow -- IDE�ͽ���֮�����ϵ��ͨ����дWindow����ʵ�ֵģ�Window������һ��ʵ����IWin32Window�ӿڵĶ��������صľ���MyControl��ʵ��
*   ��ȫ�ԣ�using System.Security.Permissions;
*           UIPermission(SecurityAction.LinkDemand, Window = UIPermissionWindow.AllWindows)]
*
*
* ProcessMnemonic -- ����"&"(���Ƿ���)?
*
* Customizing the Text Editor
*   File extension --> Language Service
*   Features (IVsLanguageInfo)
*     Text colorization
*     Statement completion
*     Method tips
*     Special markers
*     Red squigglies
*     Custom commands (& context menus)
*     Hidden Text (concealed & expand/collapse)
*     CodeWindow dropdown bar
*
* 
* ��Դӳ���
*   VSL_BEGIN_REGISTRY_MAP(IDR_REGISTRYSCRIPT)
*     VSL_REGISTRY_MAP_GUID_ENTRY -- ��������GUID���� ToolWindow��TaskList ��
*     VSL_REGISTRY_MAP_GUID_ENTRY_EX
*     VSL_REGISTRY_MAP_NUMBER_ENTRY -- ����ָ�� PLK �� Resouce ID
*     VSL_REGISTRY_RESOURCEID_ENTRY -- 
*     VSL_REGISTRY_MAP_TEMPLATE_PATH_ENTRY
*   VSL_END_REGISTRY_MAP()
*
* ����ӳ���
*   VSL_BEGIN_COMMAND_MAP()
*     VSL_COMMAND_MAP_ENTRY(cmdSet��Guid, cmdId, ��ѯ״̬����, ������) --
*       ϵͳ�ṩ�� cmdSet �� CMDSETID_StandardCommandSet97��CMDSETID_StandardCommandSet2K ��
*   VSL_END_VSCOMMAND_MAP()
*
* ��Ϣӳ�� -- ��ATL::BEGIN_MSG_MAP �Ļ����������� TRY��������˵Ҳ������ WTL
*   VSL_BEGIN_MSG_MAP(className)
*     CHAIN_MSG_MAP(����)
*   VSL_END_MSG_MAP()
*
* IVsPackage::CreateTool ���� ToolWindow ��ӳ���
*   VSL_BEGIN_TOOL_MAP()
*     VSL_TOOL_ENTRY(ToolWindow��Guid, ��������)
*   VSL_END_TOOL_MAP()
*
**********************************************************************************************************/

#endif //ADD_IN_STUDY_H