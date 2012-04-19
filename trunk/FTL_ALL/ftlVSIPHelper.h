#ifndef FTL_VSIP_HELPER_H
#define FTL_VSIP_HELPER_H

#pragma once

#ifndef FTL_BASE_H
#  error ftlVSIPHelper.h requires ftlbase.h to be included first
#endif

#ifndef FTL_COM_H
#  error ftlVSIPHelper.h requires ftlCom.h to be included first
#endif

//IVsTextViewEx ? IVsSmartTagTipWindow
//IntelliSense Hosting :  ms-help://MS.VSCC.v90/MS.VSIPCC.v90/ms.vssdk.v90/dv_vsintegration/html/20c61f8a-d32d-47e2-9c67-bf721e2cbead.htm

/********************************************************************************************
* #define NOMINMAX // Windows Platform min and max macros cause problems for the Standard C++ Library
*
* __if_exists(Derived_T::PreCreate)
* ʵ�� XXXImpl �ӿڸ�����򵥵ķ������μ� ISupportErrorInfoImpl
* OBJECT_ENTRY_AUTO(COM�����CLSID, COM�����Ӧ����) -- ͨ�� DllGetClassObject ����COM�����ʵ�ַ���,
* ģ��ʵ���У����������·�ʽ���������Ƿ�ʵ�����ƶ��ķ�������������
*   Derived_T& rDerived = *(static_cast<Derived_T*>(this));   //��ȡģ��ʵ�������ʵ��
*   __if_exists(Derived_T::XXXX) { rDerived.XXXX(); }         //�ж�ģ��ʵ�������Ƿ����˶�Ӧ�ķ���������������
* 
* IDL �ļ���
*   idl��ʽ�� [����] interface �ӿ��� { [��������] ������([��������] ����); } 
*   importlib(".tlb/.olb")--�� library �е���ָ�������Ϳ���Ϣ?��import������?
*   midl_pragma warning -- ָ������ʱ������Ϣ���� (disable : 2015) / (default : 2015)
*   nonextensible -- 
*   ���ԣ�
*     propget
*     propput
*     bindable
*     requestedit
*     id
*     retval -- ��˵���Ƿ���ֵ���� BSTR* pName, ���⣺��Count([out, retval] short* piCountOfItems)
********************************************************************************************/

/********************************************************************************************
*   
* �ӿ�ʵ����(VSL::)
*   ATL::IDispatchImpl -- �ṩ IDispatch �ӿڵ�ʵ�֣�ʹ����������(ֱ��ʹ�� DTE ����Addinʱ���μ� FlatSolutionExplorer )
*     CMyAddin : public IDispatchImpl<IDTExtensibility2, &__uuidof(IDTExtensibility2), &__uuidof(__AddInDesignerObjects), 1, 0>
*   ATL::IServiceProviderImpl -- �ṩService��ʹ�� BEGIN_SERVICE_MAP��SERVICE_ENTRY �Ⱥ��������ṩ��Service
*   IOleCommandTargetImpl -- ʵ�� IOleCommandTarget �ӿڣ��ܴ���˵��򹤾����¼���Ȼ��ͨ�� VSL_BEGIN_COMMAND_MAP ӳ�������
*   ISupportErrorInfoImpl -- ģ�����һ���� InterfaceSupportsErrorInfoList< IXXXX, ErrorInfoList�ĵݹ� >
*   IVsInstalledProductImpl -- ��Splash��������ʾ�����Ϣ(�Ͳ�����Ҫ .rgs �е���Ϣ?)��ģ�����Ϊ��ص���ԴID����.rgs�ļ��л���Ҫ InstalledProducts ��
*     ����Ҫ���� /setup һ��? C#�п����� InstalledProductRegistration �������ṩ(useInterfaceֵΪtrue��ʾ�� IVsInstalledProduct �ӿ�
*   IVsPackageImpl -- ʵ�� IVsPackage �ӿ�, COM�����Ϊ VS Package
*   IVsUIHierarchyImpl -- ��״�ṹ(��Solution Explorer��ʵ����)��ʵ���� IVsHierarchy �� IVsUIHierarchy �����ӿ�
* 
* VSL�е���
*   VsSiteCache(�����Cache,�ṩ QueryService ) -- VsSiteCacheGlobal��VsSiteCacheLocal
*     ��QueryServiceʱһ��ʹ�� xxxPackageVsSiteCache.GetCachedService<IVsXXXX, SID_SVsXXX>() �ķ�ʽ���ã�SID_ ������C#�ж�Ӧ���ַ�
*     ToolWindowBase ���� GetVsSiteCache/GetPackageVsSiteCache�� ������ʲô���𣿴�ȫ����η��ʣ�
*   StaticallySizedIconImageListWin32Control -- ͼ���б�? ��Ҫ����һ�� enum{xxx1,xxx2, CountOfImages};
*     ��һ�� static const WORD m_sResourceIDs[CountOfImages] = { IDI_XXX, } ������
*   ToolWindowBase -- ���� ToolWindow ʱ�Ļ��࣬ʵ����
*     һ����Ҫʵ�� PostCreate ��������������� VS Frame(��û���Զ����������� ExtensibilityExplorer �����ֶ�ʵ�ֵ�)
*   LoadUILibrary::ExtendedErrorInfo -- ��Package��̬�� GetLoadUILibraryErrorInfo ���ṩ������Ϣ
*   VSL::Pointer -- 
*   NonCocreateableComObject -- ���� CreateInstance ����COM���󣬺� CComObject ������?
* 
* ��ֱ��ʹ�õ�ȫ�ֹ����࣬һ���Ƕ�Ӧ�ӿڵ�ȫ��ʵ���ķ�װ���п�ֱ�ӵ��õľ�̬����
*   VsIServiceProviderUtilities<> -- �� IServiceProvider �ķ�װ����ֱ�ӵ����侲̬�� GetCachedService<IVsShell, SID_SVsShell>() ����
*   VsUIShellUtilities<> -- �� IVsUIShell �ķ�װ
*   OleComponentUIManagerUtilities<> -- �� IOleComponentUIManager �ķ�װ
*
*
* ��
*   1.�����ӳ��(IOleCommandTargetImpl)�� ���� GUID/DWORD �Եķ�ʽ����ӳ�䴦��
*     VSL_BEGIN_COMMAND_MAP
*       VSL_COMMAND_MAP_ENTRY(vsct��ʹ�õ�GUID, ����ID, ��ѯ״̬�ص�, ִ�лص� )
*       VSL_COMMAND_MAP_CLASS_ENTRY( ʹ�ô� CommandHandler �̳е����ද̬ӳ�� )
*     VSL_END_VSCOMMAND_MAP
*     ϵͳ�Դ��� GUID/����ID�� �� GUID_VsUIHierarchyWindowCmds/UIHWCMDID_RightClick ��
*   2.����ӳ��(ToolMap), ʵ�� IVsPackage::CreateTool ��������VS��Ҫʱ������Ӧ�� Tool, Tool ��Ҫ��ע���� ToolWindows ��ע��
*     VSL_BEGIN_TOOL_MAP -- VSL_TOOL_ENTRY(��Ӧ��GUID, �����ص� )
*   3.ע�����Դӳ��( �ṩ.rgs��ͨ�� %token% ʹ�õı���ֵ ?  ATL�е�  DECLARE_REGISTRY_RESOURCEID -- ?)
*     VSL_BEGIN_REGISTRY_MAP(.rgs�ļ���Ӧ����ԴID)
*       VSL_REGISTRY_MAP_GUID_ENTRY( �ṩ .rgs �ļ���ʹ�õ� CLSID ֵ�����ߵ�����һ�� )
*       VSL_REGISTRY_MAP_GUID_ENTRY_EX(�ṩ .rgs �ļ���ʹ�õ� CLSID ֵ, ���ߵ����ֿ��Բ�һ��������.rgs�ļ���һ����Pakcage��CLSIDͳһΪCLSID_Package)
*       VSL_REGISTRY_MAP_NUMBER_ENTRY( ���ṩ PLK -- �� IDS_XXXX_LOAD_KEY ) , C#��ʹ�� ProvideLoadKey ����
*       VSL_REGISTRY_RESOURCE_STRING_ENTRY( �ṩ�ַ�����Դ��PackageName )
*     VSL_END_REGISTRY_MAP
*   4.PLK ӳ��(�ƺ��Ѿ��� VSL_REGISTRY_MAP_NUMBER_ENTRY ��� ?)
*     VSL_DECLARE_REGISTRY_RESOURCEID(IDS_XXXX_LOAD_KEY)
*        ...
*     VSL_REGISTRY_RESOURCEID_ENTRY(IDS_XXXX_LOAD_KEY)
*   5.����ӳ��
*     BEGIN_SERVICE_MAP
*        SERVICE_ENTRY(xxxx)
*        SERVICE_ENTRY_FWD(SID_XXX, COM�ӿ�)
*     END_SERVICE_MAP
* 
* ���õ�ϵͳ����
*   CLSID_VSUIHIERARCHYWINDOW -- VS �ṩ�� hierarchy tool window������ ToolWindowBase�����GetLocalRegistryCLSIDViewObject
*     �з��ر�tool window ������?
*
*
* Hierarchy(��νṹ)
*   IVsHierarchy / IVsUIHierarchy
*   �Զ��� Hierarchy �ṹʱ���Դ� IVsUIHierarchyItem/IVsHierarchyItem �̳�?
*
* ������
*   VSL_CREATE_ERROR_HRESULT -- �� throw exception ?
*   VSL_STDMETHODTRY {...} VSL_STDMETHODCATCH()<CR> return VSL_GET_STDMETHOD_HRESULT(); -- �ڷ��� HRESULT �ĺ����е� try..catch �ṹ
*   C#��ͨ���׳��쳣���д��� Microsoft.VisualStudio.ErrorHandler.ThrowOnFailure(hr);
* 
* RDT(Running Document Table) -- ��ͨ�� VSFPROPID_RDTDocData �����صĽӿ�
********************************************************************************************/

/********************************************************************************************
* ÿһ��Marker����ͨ�� IVsTextManager::GetRegisteredMarkerTypeID ��ȡ��һ��Ψһ��MarkerType(����0?)��֮���ͨ����ֵ���е���
*   �� IVsTextLines::CreateLineMarker(MarkerType, xxx)�ȷ�������ָ���� LineMarker
* ʵ���Զ���������ʽ��( TextMarkerType )
*   1. ʵ�� IVsTextMarkerTypeProvider �ṩ�Զ���� TextMarker ����
*        IVsTextMarkerTypeProvider::GetTextMarkerType �з��� GUID_MyMakerService ��Ӧ�� (IVsPackageDefinedTextMarkerType*)this
*   2. ʵ�� IVsPackageDefinedTextMarkerType
*             GetVisualStyle -- ���ض�Ӧ��������ϣ�MV_GLYPH(����õ� DrawGlyphWithColors?)�� MV_COLOR_ALWAYS(ֻ����ɫ?) ��
*             DrawGlyphWithColors �в�Ҫ�����κ��ı���Ϊʲô?
*           IVsMergeableUIItem(Environment->Fonts and Colors����ʾ GetDisplayName)
*             GetMergingPriority -- �ҵ�������ͬcanonical name��Markerʱʹ��(�����Ȼ���С����? �Զ������Ӧ�� 0 ~ 0x1000)
*      ���ṩ����� TextMarker
*   3. [��ѡ]ʵ�� IVsTextMarkerClient ������VSShell���TextMaker��֪ͨ�ʹ���(�� tooltips )
*            CComObject<CMyTextMarkerClient>* pTMClient;
*            CComObject<CMyTextMarkerClient>::CreateInstance(&pTMClient);
*   4. �ڳ�ʼ��(�� PostSited)ʱͨ�� IProfferService::ProfferService(GUID_MyMakerService, (IServiceProvider*)this, xxx) �ṩ����
*   5. �� IServiceProvider::QueryService �з��ط��� ��GUID_MyMakerService, IVsTextMarkerTypeProvider, this) 
*   6. ����ע�����Ϣ( .rgs �� .pkgdef �ļ� ), �� Services �� Text Editor\External Markers ������ָ����
*        Services �� ForceRemove '%GUID_MyMakerService%' = s '%CLSID_Package%' { val Name = s 'MyMakerServiceName' }
*        Text Editor\External Markers �� ForceRemove '%GUID_MyMakerService%' = s 'MyMakerServiceName'
*           { val DisplayName='xxx'; var Package='%CLSID_Package%'; var Service='%GUID_MyMakerService%' }
*
********************************************************************************************/

/********************************************************************************************
* .RGS������Ϣ (%REGROOTBEGIN% -- %REGROOTEND%) , ���еĸ��� %������% ������ VSL_BEGIN_REGISTRY_MAP/VSL_END_REGISTRY_MAP �ж����
*   Menus -- 
*   Packages -- 
*     CLSID_XXX = s '����'
*   Services
*     IID_YYY = s '%CLSID_XXX%'
*   ����
*     %MODULE% -- ����Ľ���ļ�ȫ·��(�� D:\\XXX\\xxx.dll)
*     %RESOURCE_DLL% -- SatelliteDll ���ļ����� �� xxxUI.dll
*     %RESOURCE_PATH% -- SatelliteDll ��·��
* 
* .pkgdef �ļ�������Ϣ(define the registration of a package.����.reg��ʽ��ע����Ϣ�ļ��� ���� .rgs �ļ���ͻ? ��ֻ���� DEBUG ��汾��?)
*   �ɶ��� isolated shell application, ϵͳ�Դ��� .pkgdef�ļ� -- BaseConfig.pkgdef, 
*   �����ĵ���http://msdn.microsoft.com/en-us/library/cc138553.aspx
*   ����(ʹ�÷�ʽΪ $������$ )
*     $BaseInstallDir$ -- Visual Studio ��װ��ȫ·������ $ShellFolder$ һ����
*     $PackageFolder$ -- ����������package assembly��ȫ·��
*     $RootFolder$ -- ���ó����и�Ŀ¼��ȫ·�����ȼ��� %RESOURCE_PATH%
*     $RootKey$ -- ע���ĸ�(��ͨ�������е��ض���?)
*     $System$ -- Windows\system32 Ŀ¼
*   #resourceID/@resourceID -- ʹ��ָ��ID�ĵ��ַ�����Դ(localizable Resource Identifier)
*
********************************************************************************************/

/********************************************************************************************
* VS��ϵͳFrame��GUID,��ͨ�� VsUIShellUtilities<>::FindToolWindow(FTW_fForceCreate, ...) ��ȡ
* ͨ�� CFVsUIShellDumper::GetToolWindowEnum ��ȡ�� IVsWindowFrame �е� VSFPROPID_GuidPersistenceSlot ����)
* Ҳ��ͨ��ע����в���( VisualStudio\�汾\Configuration\ToolWindows )
*   ���������� dte80.h��dteinternal.h ͷ�ļ��е� vsContextGuidMacroExplorer ��
*   Macro Explorer -- {07CD18B4-3BA1-11D2-890A-0060083196C6}
*   Object Browser -- {269A02DC-6AF8-11D3-BDC4-00C04F688E50}
*   Output -- {34E76E81-EE4A-11D0-AE2E-00A0C90FFFC3}
*   Properties -- {EEFA5220-E298-11D0-8F78-00A0C9110057}
*   Property Manager -- {DE1FC918-F32E-4DD7-A915-1792A051F26B}
*   Solution Explorer -- {3AE79031-E1BC-11D0-8F78-00A0C9110057}
*
* vsshell80.idl �ļ����� FontsAndColorsCategory::TextEditor �ȵ�GUID����
*   
********************************************************************************************/

/********************************************************************************************
* C# ʱ -- Microsoft.VisualStudio.Shell, ������[using XXX = ]Ϊ MsVsShell
*   �� Package �̳�, ���� DefaultRegistryRoot��PackageRegistration ����(Attribute)
********************************************************************************************/
//#include <containedlanguage.h>
namespace FTL
{
    class CFVSIPUtils
    {
    public:
		FTLINLINE static BOOL IsVsHierarchyHasChildren(IVsHierarchy* pParent, VSITEMID ItemId);

		//Hierarchy property Id -- VSHPROPID_
		FTLINLINE static LPCTSTR GetVSHPropIdString(DWORD_PTR propId);
		
		//Frame(Window/Document/ToolWidndow) property Id -- VSFPROPID_
		FTLINLINE static LPCTSTR GetVSFPropIdIdString(DWORD_PTR propId);

        //��ȡ cmdidcmd.h �ж���� CmdID ��Ӧ���ַ������� IOleCommandTarget::QueryStatus ���ж� cmds[n].cmdID
        FTLINLINE static LPCTSTR GetStdIdCommandtring(ULONG cmdID);


		FTLINLINE static LPCTSTR GetTextMakerTypeString(LONG nMarkerType);

		FTLINLINE static LPCTSTR GetMarkerBehaviorFlagsString(FTL::CFStringFormater& strFormater, DWORD dwBehaviorFlags);

		FTLINLINE static LPCTSTR GetMarkerVisualFlagsString(FTL::CFStringFormater& strFormater, DWORD dwVisualFlags);

    };

	class CFVsShellDumper : public CFInterfaceDumperBase<CFVsShellDumper>
	{
		DISABLE_COPY_AND_ASSIGNMENT(CFVsShellDumper);
	public:
		FTLINLINE explicit CFVsShellDumper(IUnknown* pObj, IInformationOutput* pInfoOutput, int nIndent)
			:CFInterfaceDumperBase<CFVsShellDumper>(pObj, pInfoOutput, nIndent){}
	public:
		//override
		FTLINLINE HRESULT GetObjInfo(IInformationOutput* pInfoOutput);
	};

	class CFEnumPackagesDumper : public CFInterfaceDumperBase<CFEnumPackagesDumper>
	{
		DISABLE_COPY_AND_ASSIGNMENT(CFEnumPackagesDumper);
	public:
		FTLINLINE explicit CFEnumPackagesDumper(IUnknown* pObj, IInformationOutput* pInfoOutput, int nIndent)
			:CFInterfaceDumperBase<CFEnumPackagesDumper>(pObj, pInfoOutput, nIndent){}
	public:
		//override
		FTLINLINE HRESULT GetObjInfo(IInformationOutput* pInfoOutput);
	};

	class CFVsPackageDumper : public CFInterfaceDumperBase<CFVsPackageDumper>
	{
		DISABLE_COPY_AND_ASSIGNMENT(CFVsPackageDumper);
	public:
		FTLINLINE explicit CFVsPackageDumper(IUnknown* pObj, IInformationOutput* pInfoOutput, int nIndent)
			:CFInterfaceDumperBase<CFVsPackageDumper>(pObj, pInfoOutput, nIndent){}
	public:
		//override
		FTLINLINE HRESULT GetObjInfo(IInformationOutput* pInfoOutput);
	};

	class CFVsInstalledProductDumper : public CFInterfaceDumperBase<CFVsInstalledProductDumper>
	{
		DISABLE_COPY_AND_ASSIGNMENT(CFVsInstalledProductDumper);
	public:
		FTLINLINE explicit CFVsInstalledProductDumper(IUnknown* pObj, IInformationOutput* pInfoOutput, int nIndent)
			:CFInterfaceDumperBase<CFVsInstalledProductDumper>(pObj, pInfoOutput, nIndent){}
	public:
		//override
		FTLINLINE HRESULT GetObjInfo(IInformationOutput* pInfoOutput);
	};

	class CFVsUIShellDumper : public CFInterfaceDumperBase<CFVsUIShellDumper>
	{
		DISABLE_COPY_AND_ASSIGNMENT(CFVsUIShellDumper);
	public:
		FTLINLINE explicit CFVsUIShellDumper(IUnknown* pObj, IInformationOutput* pInfoOutput, int nIndent)
			:CFInterfaceDumperBase<CFVsUIShellDumper>(pObj, pInfoOutput, nIndent){}
	public:
		//override
		FTLINLINE HRESULT GetObjInfo(IInformationOutput* pInfoOutput);
	};

	class CFEnumWindowFramesDumper : public CFInterfaceDumperBase<CFEnumWindowFramesDumper>
	{
		DISABLE_COPY_AND_ASSIGNMENT(CFEnumWindowFramesDumper);
	public:
		FTLINLINE explicit CFEnumWindowFramesDumper(IUnknown* pObj, IInformationOutput* pInfoOutput, int nIndent)
			:CFInterfaceDumperBase<CFEnumWindowFramesDumper>(pObj, pInfoOutput, nIndent){}
	public:
		//override
		FTLINLINE HRESULT GetObjInfo(IInformationOutput* pInfoOutput);
	};
	

	class CFVsTextManagerDumper : public CFInterfaceDumperBase<CFVsTextManagerDumper>
	{
		DISABLE_COPY_AND_ASSIGNMENT(CFVsTextManagerDumper);
	public:
		FTLINLINE explicit CFVsTextManagerDumper(IUnknown* pObj, IInformationOutput* pInfoOutput, int nIndent)
			:CFInterfaceDumperBase<CFVsTextManagerDumper>(pObj, pInfoOutput, nIndent){}
	public:
		//override
		FTLINLINE HRESULT GetObjInfo(IInformationOutput* pInfoOutput);
	};

	//������Ϣ��Ҫ�� HKEY_CURRENT_USER\Software\Microsoft\VisualStudio\9.0\FontAndColors ע�� ?
	class CFVsTextMarkerTypeDumper : public CFInterfaceDumperBase<CFVsTextMarkerTypeDumper>
	{
		DISABLE_COPY_AND_ASSIGNMENT(CFVsTextMarkerTypeDumper);
	public:
		FTLINLINE explicit CFVsTextMarkerTypeDumper(IUnknown* pObj, IInformationOutput* pInfoOutput, int nIndent)
			:CFInterfaceDumperBase<CFVsTextMarkerTypeDumper>(pObj, pInfoOutput, nIndent){}
	public:
		//override
		FTLINLINE HRESULT GetObjInfo(IInformationOutput* pInfoOutput);
	};

	//Solution and Projects
	class CFVsSolutionDumper : public CFInterfaceDumperBase<CFVsSolutionDumper>
	{
		DISABLE_COPY_AND_ASSIGNMENT(CFVsSolutionDumper);
	public:
		FTLINLINE explicit CFVsSolutionDumper(IUnknown* pObj, IInformationOutput* pInfoOutput, int nIndent)
			:CFInterfaceDumperBase<CFVsSolutionDumper>(pObj, pInfoOutput, nIndent){}
		//override
		FTLINLINE HRESULT GetObjInfo(IInformationOutput* pInfoOutput);
	};

	//WindowFrame -> IVsWindowPane/CodeWindow/TextView -> TextBuffer(Colorizer) 
    //  Type: 
    //    ToolWindow -- ͨ����ʵ��,�� IOleCommandTarget/IVsWindowPane �ȣ�Ҫ�� ToolWindows/Menus ע�����ע��
    //    DocumentWindow(EditorWindow) -- ͨ����ʵ��������Document
    class CVsWindowFrameDumper : public CFInterfaceDumperBase<CVsWindowFrameDumper>
    {
        DISABLE_COPY_AND_ASSIGNMENT(CVsWindowFrameDumper);
    public:
        FTLINLINE explicit CVsWindowFrameDumper(IUnknown* pObj, IInformationOutput* pInfoOutput, int nIndent)
            :CFInterfaceDumperBase<CVsWindowFrameDumper>(pObj, pInfoOutput, nIndent){}
    public:
        //override
        FTLINLINE HRESULT GetObjInfo(IInformationOutput* pInfoOutput);
    };

	class CFVsHierarchyDumper : public CFInterfaceDumperBase<CFVsHierarchyDumper>
	{
		DISABLE_COPY_AND_ASSIGNMENT(CFVsHierarchyDumper);
	public:
		FTLINLINE explicit CFVsHierarchyDumper(IUnknown* pObj, IInformationOutput* pInfoOutput, int nIndent)
			:CFInterfaceDumperBase<CFVsHierarchyDumper>(pObj, pInfoOutput, nIndent){}
		//override
		FTLINLINE HRESULT GetObjInfo(IInformationOutput* pInfoOutput);
	private:
		//BOOL    HasChildren(IVsHierarchy* pParent, VSITEMID ItemId);
		FTLINLINE HRESULT DumpAllPropertiesInfo(IVsHierarchy* pParent, VSITEMID ItemId, IInformationOutput* pInfoOutput);
		FTLINLINE HRESULT EnumAllChildRen(IVsHierarchy* pParent,VSITEMID startItemId, IInformationOutput* pInfoOutput);
	};

	//Window
	class CFVsWindowFrameDumper : public CFInterfaceDumperBase<CFVsWindowFrameDumper>
	{
		DISABLE_COPY_AND_ASSIGNMENT(CFVsWindowFrameDumper);
	public:
		FTLINLINE explicit CFVsWindowFrameDumper(IUnknown* pObj, IInformationOutput* pInfoOutput, int nIndent)
			:CFInterfaceDumperBase<CFVsWindowFrameDumper>(pObj, pInfoOutput, nIndent){}
		//override
		FTLINLINE HRESULT GetObjInfo(IInformationOutput* pInfoOutput);
	};

	class CFVsCodeWindowDumper : public CFInterfaceDumperBase<CFVsCodeWindowDumper>
	{
		DISABLE_COPY_AND_ASSIGNMENT(CFVsCodeWindowDumper);
	public:
		FTLINLINE explicit CFVsCodeWindowDumper(IUnknown* pObj, IInformationOutput* pInfoOutput, int nIndent)
			:CFInterfaceDumperBase<CFVsCodeWindowDumper>(pObj, pInfoOutput, nIndent){}
		//override
		FTLINLINE HRESULT GetObjInfo(IInformationOutput* pInfoOutput);
	};

	class CFVsEnumTextViewsDumper : public CFInterfaceDumperBase<CFVsEnumTextViewsDumper>
	{
		DISABLE_COPY_AND_ASSIGNMENT(CFVsEnumTextViewsDumper);
	public:
		FTLINLINE explicit CFVsEnumTextViewsDumper(IUnknown* pObj, IInformationOutput* pInfoOutput, int nIndent)
			:CFInterfaceDumperBase<CFVsEnumTextViewsDumper>(pObj, pInfoOutput, nIndent){}
		//override
		FTLINLINE HRESULT GetObjInfo(IInformationOutput* pInfoOutput);
	};

	//IVsTextView -- ���Ե��� AddCommandFilter ������������Ϣ(�� ECMD_DOUBLECLICK��ECMD_CANCEL �� )���� Filter ����
	class CFVsTextViewDumper : public CFInterfaceDumperBase<CFVsTextViewDumper>
	{
		DISABLE_COPY_AND_ASSIGNMENT(CFVsTextViewDumper);
	public:
		FTLINLINE explicit CFVsTextViewDumper(IUnknown* pObj, IInformationOutput* pInfoOutput, int nIndent)
			:CFInterfaceDumperBase<CFVsTextViewDumper>(pObj, pInfoOutput, nIndent){}
		//override
		FTLINLINE HRESULT GetObjInfo(IInformationOutput* pInfoOutput);
	};

	class CFVsEnumTextBuffersDumper : public CFInterfaceDumperBase<CFVsEnumTextBuffersDumper>
	{
		DISABLE_COPY_AND_ASSIGNMENT(CFVsEnumTextBuffersDumper);
	public:
		FTLINLINE explicit CFVsEnumTextBuffersDumper(IUnknown* pObj, IInformationOutput* pInfoOutput, int nIndent)
			:CFInterfaceDumperBase<CFVsEnumTextBuffersDumper>(pObj, pInfoOutput, nIndent){}
		//override
		FTLINLINE HRESULT GetObjInfo(IInformationOutput* pInfoOutput);
	};

	class CFVsTextBufferDumper : public CFInterfaceDumperBase<CFVsTextBufferDumper>
	{
		DISABLE_COPY_AND_ASSIGNMENT(CFVsTextBufferDumper);
	public:
		FTLINLINE explicit CFVsTextBufferDumper(IUnknown* pObj, IInformationOutput* pInfoOutput, int nIndent)
			:CFInterfaceDumperBase<CFVsTextBufferDumper>(pObj, pInfoOutput, nIndent){}
		//override
		FTLINLINE HRESULT GetObjInfo(IInformationOutput* pInfoOutput);
	};

	class CFVsTextLinesDumper : public CFInterfaceDumperBase<CFVsTextLinesDumper>
	{
		DISABLE_COPY_AND_ASSIGNMENT(CFVsTextLinesDumper);
	public:
		FTLINLINE explicit CFVsTextLinesDumper(IUnknown* pObj, IInformationOutput* pInfoOutput, int nIndent)
			:CFInterfaceDumperBase<CFVsTextLinesDumper>(pObj, pInfoOutput, nIndent){}
		//override
		FTLINLINE HRESULT GetObjInfo(IInformationOutput* pInfoOutput);
	};
	
	class CFVsEnumLineMarkersDumper : public CFInterfaceDumperBase<CFVsEnumLineMarkersDumper>
	{
		DISABLE_COPY_AND_ASSIGNMENT(CFVsEnumLineMarkersDumper);
	public:
		FTLINLINE explicit CFVsEnumLineMarkersDumper(IUnknown* pObj, IInformationOutput* pInfoOutput, int nIndent)
			:CFInterfaceDumperBase<CFVsEnumLineMarkersDumper>(pObj, pInfoOutput, nIndent){}
		//override
		FTLINLINE HRESULT GetObjInfo(IInformationOutput* pInfoOutput);
	};

	class CFVsTextMarkerDumper : public CFInterfaceDumperBase<CFVsTextMarkerDumper>
	{
		DISABLE_COPY_AND_ASSIGNMENT(CFVsTextMarkerDumper);
	public:
		FTLINLINE explicit CFVsTextMarkerDumper(IUnknown* pObj, IInformationOutput* pInfoOutput, int nIndent)
			:CFInterfaceDumperBase<CFVsTextMarkerDumper>(pObj, pInfoOutput, nIndent){}
		//override
		FTLINLINE HRESULT GetObjInfo(IInformationOutput* pInfoOutput);
	};

	class CFVsTextLineMarkerDumper : public CFInterfaceDumperBase<CFVsTextLineMarkerDumper>
	{
		DISABLE_COPY_AND_ASSIGNMENT(CFVsTextLineMarkerDumper);
	public:
		FTLINLINE explicit CFVsTextLineMarkerDumper(IUnknown* pObj, IInformationOutput* pInfoOutput, int nIndent)
			:CFInterfaceDumperBase<CFVsTextLineMarkerDumper>(pObj, pInfoOutput, nIndent){}
		//override
		FTLINLINE HRESULT GetObjInfo(IInformationOutput* pInfoOutput);
	};
	
	class CFVsMergeableUIItemDumper : public CFInterfaceDumperBase<CFVsMergeableUIItemDumper>
	{
		DISABLE_COPY_AND_ASSIGNMENT(CFVsMergeableUIItemDumper);
	public:
		FTLINLINE explicit CFVsMergeableUIItemDumper(IUnknown* pObj, IInformationOutput* pInfoOutput, int nIndent)
			:CFInterfaceDumperBase<CFVsMergeableUIItemDumper>(pObj, pInfoOutput, nIndent){}
		//override
		FTLINLINE HRESULT GetObjInfo(IInformationOutput* pInfoOutput);
	};

	class CFVsUIHierarchyDumper : public CFInterfaceDumperBase<CFVsUIHierarchyDumper>
	{
		DISABLE_COPY_AND_ASSIGNMENT(CFVsUIHierarchyDumper);
	public:
		FTLINLINE explicit CFVsUIHierarchyDumper(IUnknown* pObj, IInformationOutput* pInfoOutput, int nIndent)
			:CFInterfaceDumperBase<CFVsUIHierarchyDumper>(pObj, pInfoOutput, nIndent){}
		//override
		FTLINLINE HRESULT GetObjInfo(IInformationOutput* pInfoOutput);
	};

	//Intellisense
	class CFVsIntellisenseHostDumper : public CFInterfaceDumperBase<CFVsIntellisenseHostDumper>
	{
		DISABLE_COPY_AND_ASSIGNMENT(CFVsIntellisenseHostDumper);
	public:
		FTLINLINE explicit CFVsIntellisenseHostDumper(IUnknown* pObj, IInformationOutput* pInfoOutput, int nIndent)
			:CFInterfaceDumperBase<CFVsIntellisenseHostDumper>(pObj, pInfoOutput, nIndent){}
		//override
		FTLINLINE HRESULT GetObjInfo(IInformationOutput* pInfoOutput);
	};

	class CFVsTextViewIntellisenseHostDumper : public CFInterfaceDumperBase<CFVsTextViewIntellisenseHostDumper>
	{
		DISABLE_COPY_AND_ASSIGNMENT(CFVsTextViewIntellisenseHostDumper);
	public:
		FTLINLINE explicit CFVsTextViewIntellisenseHostDumper(IUnknown* pObj, IInformationOutput* pInfoOutput, int nIndent)
			:CFInterfaceDumperBase<CFVsTextViewIntellisenseHostDumper>(pObj, pInfoOutput, nIndent){}
		//override
		FTLINLINE HRESULT GetObjInfo(IInformationOutput* pInfoOutput);
	};

	class CFVsResourceManagerDumper : public CFInterfaceDumperBase<CFVsResourceManagerDumper>
	{
		DISABLE_COPY_AND_ASSIGNMENT(CFVsResourceManagerDumper);
	public:
		FTLINLINE explicit CFVsResourceManagerDumper(IUnknown* pObj, IInformationOutput* pInfoOutput, int nIndent)
			:CFInterfaceDumperBase<CFVsResourceManagerDumper>(pObj, pInfoOutput, nIndent){}
		//override
		FTLINLINE HRESULT GetObjInfo(IInformationOutput* pInfoOutput);
	};

	//Registry
	class CFLocalRegistryDumper : public CFInterfaceDumperBase<CFLocalRegistryDumper>
	{
		DISABLE_COPY_AND_ASSIGNMENT(CFLocalRegistryDumper);
	public:
		FTLINLINE explicit CFLocalRegistryDumper(IUnknown* pObj, IInformationOutput* pInfoOutput, int nIndent)
			:CFInterfaceDumperBase<CFLocalRegistryDumper>(pObj, pInfoOutput, nIndent){}
		//override
		FTLINLINE HRESULT GetObjInfo(IInformationOutput* pInfoOutput);
	};
}//namespace FTL

#ifndef USE_EXPORT
#  include "ftlVSIPHelper.hpp"
#endif

#endif //FTL_VSIP_HELPER_H
