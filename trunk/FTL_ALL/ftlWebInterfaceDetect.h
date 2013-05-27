#ifndef FTL_WEB_INTERFACE_DETECT_H
#define FTL_WEB_INTERFACE_DETECT_H

#pragma once

#ifndef FTL_BASE_H
#  error ftlWebInterfaceDetect.h requires ftlbase.h to be included first
#endif

#ifndef FTL_COM_H
#  error ftlWebInterfaceDetect.h requires ftlCom.h to be included first
#endif


/*********************************************************************************************************************************
* �� WebBrowser(IWebBrowser) �����νű����� -- http://www.cnblogs.com/zhangqingping/archive/2009/06/16/1504260.html
*   ����1: SetSilent -- ������ʾ��Ϣ(�� alert)Ҳ������ʾ
*   ����2: ���� IOleCommandTarget::Exec������ nCmdID == OLECMDID_SHOWSCRIPTERROR �����;
*   ����3: �� OnNavigateComplete2 �� OnNavigateComplete ������ JavaScript �� window.onerror �¼�
*          ����ʱ���� OnNavigateComplete2 ʱ�޷�QI�� IHTMLDocument2
*   ����4: this.webBrowser1.ScriptErrorsSuppressed = true;
*********************************************************************************************************************************/

namespace FTL
{
	/*********************************************************************************************************************************
	* IWebBrowser
	*********************************************************************************************************************************/


	enum FindElementMethod
	{
		femById,
		femByTag,
		femByClass,
	};
	class CFWebBrowserUtil
	{
	public:
		FTLINLINE static HRESULT FindChildElement(IHTMLElement* pParent, 
			FindElementMethod method, LPCTSTR pszName, LONG nStartIndex, RecursiveWay recurWay,
			IHTMLElement** ppFoundElement);
	};

	class CFWebBrowserDumper : public CFInterfaceDumperBase<CFWebBrowserDumper>
	{
		DISABLE_COPY_AND_ASSIGNMENT(CFWebBrowserDumper);
	public:
		FTLINLINE explicit CFWebBrowserDumper(IUnknown* pObj, IInformationOutput* pInfoOutput, int nIndent, LONG_PTR param = INVLIAD_INTERFACE_DUMPER_PARAM)
			:CFInterfaceDumperBase<CFWebBrowserDumper>(pObj, pInfoOutput, nIndent, param){}
	public:
		FTLINLINE HRESULT GetObjInfo(IInformationOutput* pInfoOutput);
	};

	//IHTMLDocument6
	//IHTMLDocument7
	class CFHTMLDocumentDumper : public CFInterfaceDumperBase<CFHTMLDocumentDumper>
	{
		DISABLE_COPY_AND_ASSIGNMENT(CFHTMLDocumentDumper);
	public:
		FTLINLINE explicit CFHTMLDocumentDumper(IUnknown* pObj, IInformationOutput* pInfoOutput, int nIndent, LONG_PTR param = INVLIAD_INTERFACE_DUMPER_PARAM)
			:CFInterfaceDumperBase<CFHTMLDocumentDumper>(pObj, pInfoOutput, nIndent, param){}
	public:
		FTLINLINE HRESULT GetObjInfo(IInformationOutput* pInfoOutput);
	};

	//���Կ��� focus ��״̬
	class CFHTMLWindowDumper: public CFInterfaceDumperBase<CFHTMLWindowDumper>
	{
		DISABLE_COPY_AND_ASSIGNMENT(CFHTMLWindowDumper);
	public:
		FTLINLINE explicit CFHTMLWindowDumper(IUnknown* pObj, IInformationOutput* pInfoOutput, int nIndent, LONG_PTR param = INVLIAD_INTERFACE_DUMPER_PARAM)
			:CFInterfaceDumperBase<CFHTMLWindowDumper>(pObj, pInfoOutput, nIndent, param){}
	public:
		FTLINLINE HRESULT GetObjInfo(IInformationOutput* pInfoOutput);
	};

	//����ͨ�� IHTMLWindow2::get_event ��ȡ�� IHTMLEventObj��Ȼ��ͨ�� IHTMLEventObj::get_srcElement ��ȡ�����¼���Ԫ��(�����İ�ť��)
	class CFHTMLEventObjDumper : public CFInterfaceDumperBase<CFHTMLEventObjDumper>
	{
		DISABLE_COPY_AND_ASSIGNMENT(CFHTMLEventObjDumper);
	public:
		FTLINLINE explicit CFHTMLEventObjDumper(IUnknown* pObj, IInformationOutput* pInfoOutput, int nIndent, LONG_PTR param = INVLIAD_INTERFACE_DUMPER_PARAM)
			:CFInterfaceDumperBase<CFHTMLEventObjDumper>(pObj, pInfoOutput, nIndent, param){}
	public:
		FTLINLINE HRESULT GetObjInfo(IInformationOutput* pInfoOutput);
	};

	class CFHTMLStyleDumper : public CFInterfaceDumperBase<CFHTMLStyleDumper>
	{
		DISABLE_COPY_AND_ASSIGNMENT(CFHTMLStyleDumper);
	public:
		FTLINLINE explicit CFHTMLStyleDumper(IUnknown* pObj, IInformationOutput* pInfoOutput, int nIndent, LONG_PTR param = INVLIAD_INTERFACE_DUMPER_PARAM)
			:CFInterfaceDumperBase<CFHTMLStyleDumper>(pObj, pInfoOutput, nIndent, param){}
	public:
		FTLINLINE HRESULT GetObjInfo(IInformationOutput* pInfoOutput);
	};

	//param�����ǿ�������Collection�е�������ͨ���� CFHTMLElementCollectionDumper ����ʱ����
	class CFHTMLElementDumper : public CFInterfaceDumperBase<CFHTMLElementDumper>
	{
		DISABLE_COPY_AND_ASSIGNMENT(CFHTMLElementDumper);
	public:
		FTLINLINE explicit CFHTMLElementDumper(IUnknown* pObj, IInformationOutput* pInfoOutput, int nIndent, LONG_PTR param = INVLIAD_INTERFACE_DUMPER_PARAM)
			:CFInterfaceDumperBase<CFHTMLElementDumper>(pObj, pInfoOutput, nIndent, param){}
	public:
		FTLINLINE HRESULT GetObjInfo(IInformationOutput* pInfoOutput);
	};

	class CFHTMLElementCollectionDumper : public CFInterfaceDumperBase<CFHTMLElementCollectionDumper>
	{
		DISABLE_COPY_AND_ASSIGNMENT(CFHTMLElementCollectionDumper);
	public:
		FTLINLINE explicit CFHTMLElementCollectionDumper(IUnknown* pObj, IInformationOutput* pInfoOutput, int nIndent, LONG_PTR param = INVLIAD_INTERFACE_DUMPER_PARAM)
			:CFInterfaceDumperBase<CFHTMLElementCollectionDumper>(pObj, pInfoOutput, nIndent, param){}
	public:
		FTLINLINE HRESULT GetObjInfo(IInformationOutput* pInfoOutput);
	};

	class CFHTMLTableDumper : public CFInterfaceDumperBase<CFHTMLTableDumper>
	{
		DISABLE_COPY_AND_ASSIGNMENT(CFHTMLTableDumper);
	public:
		FTLINLINE explicit CFHTMLTableDumper(IUnknown* pObj, IInformationOutput* pInfoOutput, int nIndent, LONG_PTR param = INVLIAD_INTERFACE_DUMPER_PARAM)
			:CFInterfaceDumperBase<CFHTMLTableDumper>(pObj, pInfoOutput, nIndent, param){}
	public:
		FTLINLINE HRESULT GetObjInfo(IInformationOutput* pInfoOutput);
	};

	class CFHTMLTableRowDumper : public CFInterfaceDumperBase<CFHTMLTableRowDumper>
	{
		DISABLE_COPY_AND_ASSIGNMENT(CFHTMLTableRowDumper);
	public:
		FTLINLINE explicit CFHTMLTableRowDumper(IUnknown* pObj, IInformationOutput* pInfoOutput, int nIndent, LONG_PTR param = INVLIAD_INTERFACE_DUMPER_PARAM)
			:CFInterfaceDumperBase<CFHTMLTableRowDumper>(pObj, pInfoOutput, nIndent, param){}
	public:
		FTLINLINE HRESULT GetObjInfo(IInformationOutput* pInfoOutput);
	};
}

#endif //FTL_WEB_INTERFACE_DETECT_H



#ifndef USE_EXPORT
#  include "ftlWebInterfaceDetect.hpp"
#endif
