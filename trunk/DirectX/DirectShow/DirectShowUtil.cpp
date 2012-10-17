#include "stdafx.h"
#include "DirectShowUtil.h"

CDirectShowUtil::CDirectShowUtil(void)
{
} 

CDirectShowUtil::~CDirectShowUtil(void)
{ 
}

typedef struct tagFILTER_HEADER
{
    DWORD dwMerit;
}FILTER_HEADER;

BOOL CDirectShowUtil::SetFilterMerit(LPCTSTR inClsid, DWORD inMerit)
{
    BOOL bRet = FALSE;
    _ASSERT(FALSE);//TODOû��д��

    //P24, ��ȡ FilterData ת���� FILTER_HEADER* ָ�룬Ȼ���޸� dwMerit����

    //CLSID_LegacyAmFilterCategory
    LPCTSTR pszRegistryEntry = TEXT("CLSID\\{083863F1-70DE-11D0-BD40-00A0C911CE86}\\Instance\\");
    const LONG cMaxLength = 1024 * 16;
    BYTE filterData[cMaxLength];
    DWORD actualLength = 0;
    TCHAR szEntry[1000] = {0};
    HKEY  hKey = NULL;

    ::RegOpenKeyEx(HKEY_CLASSES_ROOT,szEntry,0,KEY_ALL_ACCESS,&hKey);

    ::RegQueryValueEx(hKey,TEXT("FilterData"),NULL,NULL,filterData,&actualLength);
    FILTER_HEADER* filterHeader = (FILTER_HEADER*)filterData;
    filterHeader->dwMerit = inMerit;
    ::RegSetValueEx(hKey,TEXT("FilterData"),NULL,REG_BINARY,filterData,actualLength);

    if (hKey)
    {
        ::RegCloseKey(hKey);
    }
    return FALSE;
}