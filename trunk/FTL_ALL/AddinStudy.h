#ifndef ADD_IN_STUDY_H
#define ADD_IN_STUDY_H
#pragma once

//CodeBlocks -- ��Դ�Ŀ�ƽ̨ C++ IDE������ѧϰ��ص�֪ʶ
//  ������ ICodeCompletionItemsProvider �ӿ�?

//DTE�� ProjectItem -> FileCodeModel -> CodeElement->CodeElementChildren ?

/*********************************************************************************************************
* ����
*   1.C# ����һ�� CommandBar.resx �ļ��� C++/ATL ��Ӧ����ʲô? ʹ�� CultureInfo ����������ԣ�C++/ATL���? 
*   2.����Ŀ��(IDTCommandTarget)��ִ��(Exec)��״̬��ȡ��ͨ���Ƚ��ַ����ķ�ʽ��ִ�е�,��û�и��õķ�ʽ��
*
*
* ���õ������ռ� ��ע�⣺EnvDTE[x0]�������Զ�������ģ��(AOM--Automation Object Model)�е�����
*     AddInDesignerObjects;
*     EnvDTE(Environment Development Tools Extensibility)--��������������չ, ��ӦVS2005֮ǰ��DTE�汾(�� DTE �ӿ�)
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
*     EnvDTE80 -- ��Ӧ��VS2005��DTE(ֻ�����仯�˵İ汾���� DTE2 �ӿ�)����Ӧ�Ļ��� EnvDTE90 ��
*     Microsoft_VisualStudio_CommandBars;
*     Extensibility (C#�в���?) -- ������IDTExtensibility2ʹ�õ�����
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
* http://tortoisesvn.net/docs/nightly/TortoiseSVN_zh_CN/index.html
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
*   LearnVSXNow -- http://www.cnblogs.com/default/archive/2010/02/26/1674582.html
*   ��Ҫ��װ Visual Studio 200X �� SDK(VSX SDK) -- http://msdn.com/vsx 
* 
* ����Service -- Visual Studio�ṩ��Add-in��Package�Ĺ��ܣ�PackageҲ���ṩ(Proffer)�Լ���Service
*   1 : IVsObjectManager
*   2 : _DTE
*   3 : IVsCommandWindow
*   4 : IVsRegisterFindScope
*   5 : IVsAddProjectItemDlg
*   6 : IVsStructuredFileIO
*   7 : IVsInvisibleEditorManager
*   8 : ICQryAutoFactory
*   9 : IVsHTMLConverter
*   10 : IVsLog
*   11 : IVsFileChange(SVsFileChange)
*   12 : IUIHostLocale
*   13 : IVsEnumHierarchyItemsFactory
*   14 : IHxSession
*   15 : IHxRegisterSession
*   16 : IVsUIShellOpenDocument
*   17 : IVsIntelliMouseHandler
*   18 : IVsParseCommandLine
*   19 : SVsProfilesManagerUI
*   20 : IVsFontAndColorStorage
*   21 : SVsOnlineHelpData
*   22 : SVsSearchService
*   23 : Help
*   24 : SVsHelpPage
*   25 : IVsOutputWindow
*   26 : IVsRegisterScciProvider
*   27 : IVsMonitorSelection
*   28 : SVsWebProxy
*   29 : IVsComponentSelectorDlg
*   30 : SVsHelpOnlineF1Service
*   31 : IVsSolutionPersistence
*   32 : IVsLibraryReferenceManager
*   33 : IVsCmdNameMapping
*   34 : IVsToolbox(SVsToolBox)
*   35 : IVsRegisterEditors(SVsRegisterEditors) -- 
*   36 : IInternetSecurityManager
*   37 : IVsSolution
*   38 : IVsdDeployment
*   39 : ObjectExtenders
*   40 : IVsSolutionBuildManager
*   41 : IVsFileChangeEx
*   42 : IVsMonitorUserContext
*   43 : IVsTaskList
*   44 : IVsBuildEventCommandLineDialogService
*   45 : SVsStartPageDownload
*   46 : IVsRunningDocumentTable
*   47 : IVsdPackageManager
*   48 : IVsUIShell(SVsUIShell) -- �ṩ�����ɷ���ȥʵ��������йصĹ���
*   49 : IVsDiscoveryService
*   50 : IVsPropertyPageFrame
*   51 : IVsAddWebReferenceDlg
*   52 : IVsFilterKeys
*   53 : IVsTextImageUtilities
*   54 : IVsAppCommandLine
*   55 : IProfferService
*   56 : IVsXMLMemberIndexService
*   57 : IVsStatusbar
*   58 : IVsUIHierWinClipboardHelper
*   59 : IVsExternalFilesManager
*   60 : IVsMenuEditorFactory
*   61 : IVsdDeployment
*   62 : IVsRegisterProjectTypes
*   63 : SourceControl
*   64 : IVsCodeShareHandler
*   65 : SVsLinkedUndoTransactionManager
*   66 : IVsShell(SVsShell) --
*   67 : OleMenuCommandService(IMenuCommandService) -- �� AddCommand �ȷ��� 
*   68 : STrackSelection
*   69 : SVsWindowFrame
*
* �����ӿ�
*   IVsPackage -- ������ PackageRegistration �����ԣ� VSX������Ϊ Package �Ļ���ʵ�֣�
*     CLR��Package����ǿ��������ҪKey�ļ���Ϊ�����ܣ�Packageֻ������Ҫ�书�ܵ�ʱ��Ż���ء�
*     IVsPackage �� Site ���� IDE��Package����ͨ�� IServiceProvider �ӿڻ�ȡ������Services
*   CommandSet? -- ���
*  
* ���ù���
*    $(RegitPath)\Regit.exe/regpkg.exe -- Package��װ�Լ�ע�Ṥ�ߣ���ȡ Package �����еĸ��� Attribute ��ע��
*
* ע��/ȡ��ע��
*   VSͨ��PLK��Package Load Key -- 128���ַ�������ɢ���룬��΢��վ�� http://msdn.microsoft.com/vstudio/extend/ ��ã�
*   ����鷢���汾����չ���Ƿ�Ϸ�(����ʱʹ��VSIP�ĵ������֤)��
*   ͨ�� ProvideLoadKey(��С�汾,�汾��,����,����������,PLK����ԴID) �����ṩ
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
*     /RANU 
*     /setup -- ������������splash screen ��� logo and product name
*     /NoVsip -- ģ��û�а�װ VS SDK �Ļ�������Ϊ
*     /root:Software\Microsoft\VisualStudio\9.0Exp --ָ��ע���ע����λ��
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
**********************************************************************************************************/

#endif //ADD_IN_STUDY_H