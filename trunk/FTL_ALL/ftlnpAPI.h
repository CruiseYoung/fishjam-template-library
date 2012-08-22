#ifndef FTL_NP_API_H
#define FTL_NP_API_H
#pragma once

#ifndef FTL_BASE_H
#  error ftlnpAPI.h requires ftlbase.h to be included first
#endif


/*************************************************************************************************************************
*   
*************************************************************************************************************************/
#include "npapi.h"
#include "npruntime.h"

namespace FTL
{
	#ifdef FTL_DEBUG
	# define NPAPI_VERIFY(x)   \
		err = (x);\
		if(NPERR_NO_ERROR != err)\
		{\
			REPORT_ERROR_INFO(FTL::CFnpAPIErrorInfo, err, x);\
		}
	#else
	# define NPAPI_VERIFY(x)   \
		err = (x);
	#endif 

	FTLEXPORT class CFnpAPIErrorInfo : public CFConvertInfoT<CFnpAPIErrorInfo, NPError>
	{
		DISABLE_COPY_AND_ASSIGNMENT(CFnpAPIErrorInfo);
	public:
		FTLINLINE explicit CFnpAPIErrorInfo(NPError err);
		FTLINLINE virtual LPCTSTR ConvertInfo();
	};

	FTLEXPORT class CFNPVariantInfo : public CFConvertInfoT<CFNPVariantInfo, const NPVariant&>
	{
		DISABLE_COPY_AND_ASSIGNMENT(CFNPVariantInfo);
	public:
		FTLINLINE explicit CFNPVariantInfo(const NPVariant& info);
		FTLINLINE virtual LPCTSTR ConvertInfo();
	private:
		FTLINLINE VOID GetTypeInfo(CFStringFormater& formaterType);
		FTLINLINE VOID GetValueInfo(CFStringFormater& formaterValue);
	};

	FTLEXPORT class CFnpAPIUtil
	{
	public:
		//��ȡ���Ӧ��ʵ���������
		FTLINLINE static LPCTSTR GetNPPVariableString(NPPVariable variable);

		//��ȡ�����Ӧ��ʵ���������
		FTLINLINE static LPCTSTR GetNPNVariableString(NPPVariable variable);
	};

}
#endif //FTL_NP_API_H

#ifndef USE_EXPORT
#  include "ftlnpAPI.hpp"
#endif 