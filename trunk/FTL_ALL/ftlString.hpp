#ifndef FTL_STRING_HPP
#define FTL_STRING_HPP
#pragma once

#ifdef USE_EXPORT
#  include "ftlString.h"
#endif

namespace FTL
{
    int ciStringCompare(const std::string& s1, const std::string& s2)
    {
        //Ч�ʱ�ʹ�� mismatch �� lexicographical_compare ��
        //return std::lexicographical_compare(s1.begin(), s1.end(), s2.begin(), s2.end()).
        return _stricmp(s1.c_str(), s2.c_str());
    }

    BOOL CFStringUtil::IsMatchMask(LPCTSTR pszName, LPCTSTR pszMask, BOOL bCaseSensitive /* = TRUE */)
    {
        if (!pszMask)
        {
            return TRUE;
        }
        //���ñȽϺ�����ָ��(�Ƿ����ִ�Сд)
        typedef int (WINAPI *StrCompareFun)(LPCTSTR, LPCTSTR);
        StrCompareFun pCompareFun = bCaseSensitive ? lstrcmp : lstrcmpi;

        if (!pszName)
        {
            if (pszMask && pszMask[0] && (*pCompareFun)(pszMask, _T("*")))
            {
                return FALSE;
            }
            else
            {
                return TRUE;
            }
        }

        while(*pszName && *pszMask)
        {
            switch(*pszMask) 
            {
            case _T('?'):
                {
                    pszName++;
                    pszMask++;
                }
                break;
            case _T('*'):
                {
                    pszMask++;
                    if (!*pszMask)
                    {
                        return TRUE;
                    }
                    do 
                    {
                        if (IsMatchMask(pszName, pszMask, bCaseSensitive))
                        {
                            return TRUE;
                        }
                        pszName++;
                    } while(*pszName);
                    return FALSE;
                }
                break;
            default:
                {
                    if (bCaseSensitive)
                    {
                        //���ִ�Сд�Ƚ�
                        if (*pszName != *pszMask)
                        {
                            return FALSE;
                        }
                    }
                    else
                    {
                        //�����ִ�Сд(_totupper Ҫ��Ĳ����� int, ����ʹ����ʱ����?)
                        TCHAR tmpName = *pszName;
                        TCHAR tmpMask = *pszMask;
                        if (_totupper(tmpName) != _totupper(tmpMask))
                        {
                            return FALSE;
                        }
                    }
                    pszName++;
                    pszMask++;
                }
            }
        }
        return ((!*pszName && !*pszMask) || (_T('*') == *pszMask));
    }
}

#endif //FTL_STRING_HPP
