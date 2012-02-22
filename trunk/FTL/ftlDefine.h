#ifndef FTL_DEFINE_H
#define FTL_DEFINE_H
#pragma once

namespace FTL
{
    //ʹ��ģ���export�ؼ��֣���������ʵ�ַֿ� -- ��ĿǰVS2003~VS2008����֧�� export �ؼ���
    #ifdef USE_EXPORT
    #  define FTLEXPORT  export
    #  define FTLINLINE
    #else
    #  define FTLEXPORT
    #  define FTLINLINE inline
    #endif

    //���԰汾���Զ����� FTL_DEBUG��RELEASE �汾�¿����ֹ����� FTL_DEBUG
    #if defined(DEBUG) || defined(_DEBUG)
    #  ifndef FTL_DEBUG
    #    define FTL_DEBUG
    #  endif
    #endif

    #ifndef QQUOTE
    #  define    QUOTE(x)        #x
    #  define    QQUOTE(y)       QUOTE(y)
    #  define     __FILE__LINE__  TEXT(__FILE__) TEXT("(") TEXT(QQUOTE(__LINE__)) TEXT(") :")
    #endif //QQUOTE

    #define JOIN_ONE(x, y) x ## y
    #define JOIN_TWO(x, y) JOIN_ONE(x, y)

    //�����ڱ��������������(Output)�������TODO����Ϣ������˫��ֱ�ӽ��ж�λ
    //ʹ�÷�ʽ��pragma TODO(Fix this later)
    #define TODO(desc) message(__FILE__ "(" QQUOTE(__LINE__) ") : TODO: " #desc)

    //����������һ�������е� past-the-end λ�ã�ʹ�÷�ʽ:
    //  std::find(intArray, ARRAY_PAST_THE_END(intArray), 10);
    #define ARRAY_PAST_THE_END(c) ( c + _countof( c ) )

    //����<����ʱ�Ƚ�������Ա�Ķ������
    #define COMPARE_OBJ_LESS(f,l,r) \
        if( l.f < r.f ) { return true; } \
        else if( l.f > r.f ) { return false; }
        
    //����Ƚϳ�Ա�����ĺ꣬�����������ࡢ�ṹ�� operator < ʱ��ע�⣺���һ������ʱ���������ֱ�Ϊ field , other&
    #define COMPARE_MEM_LESS(f, o) \
        if( f < o.f ) { return true; }\
        else if( f > o.f ) { return false; }

}


#endif //FTL_DEFINE_H