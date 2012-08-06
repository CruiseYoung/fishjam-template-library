
#ifndef FTL_WIN_INTERFACE_DETECT_H
#define FTL_WIN_INTERFACE_DETECT_H

#pragma once

#ifndef FTL_BASE_H
#  error ftlWinIntDetect.h requires ftlbase.h to be included first
#endif

#ifndef FTL_COM_H
#  error ftlWinIntDetect.h requires ftlCom.h to be included first
#endif


/*********************************************************************************************************************************

*********************************************************************************************************************************/

namespace FTL
{

	class CFWinInterfaceDetectUtil
	{
	public:
		FTLINLINE static LPCTSTR GetSystemStateFlags(CFStringFormater& formater, DWORD nFlags, LPCTSTR pszDivide = TEXT("|"));
	};

	class CFEnumVARIANTDumper: public CFInterfaceDumperBase<CFEnumVARIANTDumper>
	{
		DISABLE_COPY_AND_ASSIGNMENT(CFEnumVARIANTDumper);
	public:
		FTLINLINE explicit CFEnumVARIANTDumper(IUnknown* pObj, IInformationOutput* pInfoOutput, int nIndent)
			:CFInterfaceDumperBase<CFEnumVARIANTDumper>(pObj, pInfoOutput, nIndent){}
	public:
		FTLINLINE HRESULT GetObjInfo(IInformationOutput* pInfoOutput);
	};

	/*********************************************************************************************************************************
	* IAccessible -- MSAA(Microsoft Active Accessibility), ��Ŀɷ��ʼ�����Ӧ�á���ͨ��΢��� Inspect.exe ���߲鿴
	*   UI����¶��һ��Interface��������һ�����������п��ƣ�������Ϊ�˷���м��ˣ������ڶ���ץͼ��UI�Զ������Եȡ�
	*   AccessibleObjectFromWindow(hwnd, OBJID_WINDOW, IID_IAccessible, (void**)&paccMainWindow)��
	*     ��Ӧ�������յ� WM_GETOBJECT ��Ϣ������ʵ����IAccessible���ڲ���(MS��.NET WinForm Framework ���ṩ��IAccessible��Ĭ��ʵ��,
	*     �������� Local oleacc proxy?)��Ȼ��ͨ�� LresultFromObject ���ظýӿ�
	*********************************************************************************************************************************/

	class CFAccessibleDumper : public CFInterfaceDumperBase<CFAccessibleDumper>
	{
		DISABLE_COPY_AND_ASSIGNMENT(CFAccessibleDumper);
	public:
		FTLINLINE explicit CFAccessibleDumper(IUnknown* pObj, IInformationOutput* pInfoOutput, int nIndent)
			:CFInterfaceDumperBase<CFAccessibleDumper>(pObj, pInfoOutput, nIndent){}
	public:
		FTLINLINE HRESULT GetObjInfo(IInformationOutput* pInfoOutput);
	};

}

#endif //FTL_WIN_INTERFACE_DETECT_H



#ifndef USE_EXPORT
#  include "ftlWinIntDetect.hpp"
#endif
