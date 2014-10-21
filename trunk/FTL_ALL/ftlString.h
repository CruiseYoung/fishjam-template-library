#ifndef FTL_STRING_H
#define FTL_STRING_H
#pragma once

#include "ftlDefine.h"

namespace FTL
{
    //���Դ�Сд�ַ����Ƚϵĺ���
    FTLINLINE int ciStringCompare(const std::string& s1, const std::string& s2);

    //���ں��Դ�Сд�ַ����Ƚϵķº����� -- �μ� Effective STL �е�����19
    struct CIStringCompare : public std::binary_function<std::string, std::string, bool>
    {
        FTLINLINE bool operator()(const std::string& lhs, const std::string& rhs) const
        {
            return ciStringCompare(lhs, rhs);
        }

    };

    class CFStringUtil
    {
    public:
        FTLINLINE static BOOL IsMatchMask(LPCTSTR Name, LPCTSTR Mask, BOOL bCaseSensitive = TRUE);
    };
}
#endif //FTL_STRING_H

#ifndef USE_EXPORT
#  include "ftlString.hpp"
#endif