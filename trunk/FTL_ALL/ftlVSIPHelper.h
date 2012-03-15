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

********************************************************************************************/

namespace FTL
{
    class CFVSIPUtils
    {
    public:
		FTLINLINE static LPCTSTR GetVSHPropIdString(DWORD_PTR propId);

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

}//namespace FTL

#ifndef USE_EXPORT
#  include "ftlVSIPHelper.hpp"
#endif

#endif //FTL_VSIP_HELPER_H
