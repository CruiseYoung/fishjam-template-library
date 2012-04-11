#ifndef FTL_VSIP_HELPER_H
#define FTL_VSIP_HELPER_H

#pragma once

#ifndef FTL_BASE_H
#  error ftlVSIPHelper.h requires ftlbase.h to be included first
#endif

#ifndef FTL_COM_H
#  error ftlVSIPHelper.h requires ftlCom.h to be included first
#endif

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
*   IOleCommandTargetImpl -- ʵ�� IOleCommandTarget �ӿڣ��ܴ���˵��򹤾����¼�
*   IServiceProviderImpl -- �ṩService��ʹ�� BEGIN_SERVICE_MAP��SERVICE_ENTRY �Ⱥ��������ṩ��Service
*   ISupportErrorInfoImpl -- ģ�����һ���� InterfaceSupportsErrorInfoList< IXXXX, ErrorInfoList�ĵݹ� >
*   IVsInstalledProductImpl -- ��Splash��������ʾ�����Ϣ(�Ͳ�����Ҫ .rgs �е���Ϣ?)��ģ�����Ϊ��ص���ԴID����.rgs�ļ��л���Ҫ InstalledProducts ��
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
*   3.ע�����Դӳ��( �ṩ.rgs��ͨ�� %token% ʹ�õı���ֵ ? )
*     VSL_BEGIN_REGISTRY_MAP(.rgs�ļ���Ӧ����ԴID)
*       VSL_REGISTRY_MAP_GUID_ENTRY( �ṩ .rgs �ļ���ʹ�õ� CLSID ֵ )
*       VSL_REGISTRY_MAP_NUMBER_ENTRY( ���ṩ PLK -- �� IDS_XXXX_LOAD_KEY ) 
*       VSL_REGISTRY_RESOURCE_STRING_ENTRY( �ṩ�ַ�����Դ��PackageName )
*     VSL_END_REGISTRY_MAP
*   4.PLK ӳ��(�ƺ��Ѿ��� VSL_REGISTRY_MAP_NUMBER_ENTRY ��� ?)
*     VSL_DECLARE_REGISTRY_RESOURCEID(IDS_XXXX_LOAD_KEY)
*        ...
*     VSL_REGISTRY_RESOURCEID_ENTRY(IDS_XXXX_LOAD_KEY)
*     
* 
* ���õ�ϵͳ����
*   CLSID_VSUIHIERARCHYWINDOW -- VS �ṩ�� hierarchy tool window������ ToolWindowBase�����GetLocalRegistryCLSIDViewObject
*     �з��ر�tool window ������?
*
* ��������(Service)
*   SID_SLocalRegistry(ILocalRegistry4) -- 
*
* Hierarchy(��νṹ)
*   IVsHierarchy / IVsUIHierarchy
*   �Զ��� Hierarchy �ṹʱ���Դ� IVsUIHierarchyItem/IVsHierarchyItem �̳�?
*
* ������
*   VSL_CREATE_ERROR_HRESULT -- �� throw exception ?
*   VSL_STDMETHODTRY {...} VSL_STDMETHODCATCH()<CR> return VSL_GET_STDMETHOD_HRESULT(); -- �ڷ��� HRESULT �ĺ����е� try..catch �ṹ
*  
********************************************************************************************/

/********************************************************************************************
* RGS������Ϣ (%REGROOTBEGIN% -- %REGROOTEND%)
*   Menus -- 
*   Packages -- 
*     CLSID_XXX = s '����'
*   Services
*     IID_YYY = s '%CLSID_XXX%'
********************************************************************************************/

/********************************************************************************************
* VS��ϵͳFrame��GUID,��ͨ�� VsUIShellUtilities<>::FindToolWindow(FTW_fForceCreate, ...) ��ȡ
* ͨ�� CFVsUIShellDumper::GetToolWindowEnum ��ȡ�� IVsWindowFrame �е� VSFPROPID_GuidPersistenceSlot ����)
* Ҳ��ͨ��ע����в���( VisualStudio\�汾\Configuration\ToolWindows )
*   Macro Explorer -- {07CD18B4-3BA1-11D2-890A-0060083196C6}
*   Object Browser -- {269A02DC-6AF8-11D3-BDC4-00C04F688E50}
*   Output -- {34E76E81-EE4A-11D0-AE2E-00A0C90FFFC3}
*   Properties -- {EEFA5220-E298-11D0-8F78-00A0C9110057}
*   Property Manager -- {DE1FC918-F32E-4DD7-A915-1792A051F26B}
*   Solution Explorer -- {3AE79031-E1BC-11D0-8F78-00A0C9110057}
********************************************************************************************/
namespace FTL
{
    class CFVSIPUtils
    {
    public:
		//Hierarchy property Id -- VSHPROPID_
		FTLINLINE static LPCTSTR GetVSHPropIdString(DWORD_PTR propId);
		
		//Frame(Window/Document/ToolWidndow) property Id -- VSFPROPID_
		FTLINLINE static LPCTSTR GetVSFPropIdIdString(DWORD_PTR propId);

        //��ȡ cmdidcmd.h �ж���� CmdID ��Ӧ���ַ������� IOleCommandTarget::QueryStatus ���ж� cmds[n].cmdID
        FTLINLINE static LPCTSTR GetStdIdCommandtring(ULONG cmdID);

		FTLINLINE static BOOL IsVsHierarchyHasChildren(IVsHierarchy* pParent, VSITEMID ItemId);
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
		HRESULT DumpAllPropertiesInfo(IVsHierarchy* pParent, VSITEMID ItemId, IInformationOutput* pInfoOutput);
		HRESULT EnumAllChildRen(IVsHierarchy* pParent,VSITEMID startItemId, IInformationOutput* pInfoOutput);
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

	class CFVsTextViewDumper : public CFInterfaceDumperBase<CFVsTextViewDumper>
	{
		DISABLE_COPY_AND_ASSIGNMENT(CFVsTextViewDumper);
	public:
		FTLINLINE explicit CFVsTextViewDumper(IUnknown* pObj, IInformationOutput* pInfoOutput, int nIndent)
			:CFInterfaceDumperBase<CFVsTextViewDumper>(pObj, pInfoOutput, nIndent){}
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
