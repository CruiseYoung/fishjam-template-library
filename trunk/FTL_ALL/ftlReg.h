#ifndef FTL_REG_H
#define FTL_REG_H
#pragma once

#ifndef FTL_BASE_H
#  error ftlReg.h requires ftlbase.h to be included first
#endif

#include "ftlFile.h"
/*************************************************************************************************************************
*************************************************************************************************************************/


namespace FTL
{
    enum RegExportFlags
    {
        WRITE_PATH_NONE     = 0x0000,
        WRITE_PATH_AUTO     = 0x0001,       //���� pszValueName Ϊ *ʱ
        
        WRITE_SUB_KEY       = 0x0010
    };

    class CFRegUtil
    {
    public:
        FTLINLINE static LPCTSTR ConvertRegRootKeyToString(HKEY hKeyRoot);
        FTLINLINE static HKEY ConvertStringToRegRootKey(LPCTSTR pszKeyRoot);

        //RegNotifyChangeKeyValue -- ���ע���ı仯�����Կ�����Ӧ
        //RegCreateKeyEx -- ����ע����������ڵĻ����
        //RegQueryValueEx -- ��ȡ �ַ���(REG_SZ)��DWORD(REG_DWORD) ע���� ���� ��ֵ
        //RegSetValueEx -- ����ֵ�����lpSubKeyΪNULL��""��������Ĭ��ֵ
        //RegOpenKeyEx -- ��ע���
        //RegEnumKeyEx -- ö���Ӽ���������ɾ���������Ӽ��ȣ�û�и���ʱ���� ERROR_NO_MORE_ITEMS
        //RegDeleteKey -- ɾ��û���¼��Ӽ����Ӽ�,���Ҫ�ݹ�ɾ��������ʹ�� CRegKey::RecurseDeleteKey 
        //RegSaveKey/RegSaveKeyEx -- �Զ����Ʒ�ʽ����ע�������
        FTLINLINE static LONG GetRegValueExportString(HKEY hKey, LPCTSTR pszValueName, CAtlString& strResult, DWORD* pRegType);

        //�� .reg ���ı���ʽ ����ע��������
        //  ע�⣺���ܲ��� CRegKey �����෽ʽʵ��(����m_hKey) -- ��û�б���ע���·��
        FTLINLINE static LONG ExportRegValueToFile(LPCTSTR pszFullKey, 
            LPCTSTR pszValueName, 
            CFUnicodeFile* pRegFile, 
            DWORD flags = WRITE_PATH_AUTO | WRITE_SUB_KEY,
            REGSAM samDesired = KEY_READ | KEY_ENUMERATE_SUB_KEYS | KEY_QUERY_VALUE);

        //FTLINLINE static LONG EnumRegKeyHandle(HKEY hKey, )
    };

}
#endif //FTL_REG_H

#ifndef USE_EXPORT
#  include "ftlReg.hpp"
#endif 