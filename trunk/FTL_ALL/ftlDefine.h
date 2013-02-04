///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file   ftlDefine.h
/// @brief  Functional Template Library Define Header File.
/// @author fujie
/// @version 0.6 
/// @date 03/30/2008
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef FTL_DEFINE_H
#define FTL_DEFINE_H
#pragma once

namespace FTL
{
    //ʹ��ģ���export�ؼ��֣���������ʵ�ַֿ� -- ��ĿǰVS2003~VS2008����֧�� export �ؼ���
    //���ǣ��Ƿ������չ�ú�ʵ�־�̬FTL�⣿�ο� ATL_DLL_ ����÷���
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
    #endif //QQUOTE

	#define     __FILE__LINE__  TEXT(__FILE__) TEXT("(") TEXT(QQUOTE(__LINE__)) TEXT(") :")

    /**********************************************************************************************
    * ��չ���ǰ��ַ�˳��ɨ���
    * JOIN_ONE(var_, __LINE__) �У�����ɨ�赽JOIN_ONE����˽��ֱ������ var___LINE__, չ������
    * JOIN_TWO(var_, __LINE__) �У�����ɨ�赽JOIN_TWO��չ�����м���
    *   JOIN_ONE(var_, __LINE__)�� �ڴ˴�չ���У��ٿ���__LINE__���滻Ϊ JOIN_ONE(var_��11)��
    *   Ȼ���ٽ��еڶ���չ�������� var_11  -- ���Ը��ݵ�ǰ���кţ����ɡ���ʱ��Ψһ������
    /**********************************************************************************************/
    #define JOIN_ONE(x, y) x ## y
    #define JOIN_TWO(x, y) JOIN_ONE(x, y)

    //�����ڱ��������������(Output)�������TODO����Ϣ������˫��ֱ�ӽ��ж�λ
    //ʹ�÷�ʽ��pragma TODO(Fix this later)
    #define TODO(desc) message(__FILE__ "(" QQUOTE(__LINE__) ") : TODO: " #desc)

    //�������ⰴ��(�� VK_ESCAPE )�� ������ WM_KEYDOWN �� (GetAsyncKeyState(vk_code) & 0x01) ?
    #define KEY_DOWN(vk_code)   ( (GetAsyncKeyState(vk_code) & 0x8000) ? TRUE  : FALSE ) 
    #define KEY_UP(vk_code)     ( (GetAsyncKeyState(vk_code) & 0x8000) ? FALSE : TRUE  )

	#define FTL_MAX_CLASS_NAME_LENGTH	256

    //���з�(10,A)
    #define LF  '\n'
    //�س���(13,D)
    #define CR  '\r'

    //ע�⣬��Ҫ����Dos��Unix
    #define LINE_SEP    CR LF

    //����������һ�������е� past-the-end λ�ã�ʹ�÷�ʽ:
    //  std::find(intArray, ARRAY_PAST_THE_END(intArray), 10);
    #define ARRAY_PAST_THE_END(c) ( c + _countof( c ) )

    //����<����ʱ�Ƚ�������Ա�Ķ������
    #define COMPARE_OBJ_LESS(f,l,r) \
        if( l.f < r.f ) { return true; } \
        else if( l.f > r.f ) { return false; }
        
    //����Ƚϳ�Ա�����ĺ꣬�����������ࡢ�ṹ�� operator < �� > ʱ��ע�⣺�ڱȽ���������Ҫ�ı�������Ҫ����true���������ֱ�Ϊ field , other&
    #define COMPARE_MEM_LESS(f, o) \
        if( f < o.f ) { return true; }\
        else if( f > o.f ) { return false; }

	#define COMPARE_MEM_BIG(f, o) \
	if( f > o.f ) { return true; }\
		else if( f < o.f ) { return false; }

	#ifndef MAKELONGLONG
		#define MAKELONGLONG(a, b)	((LONGLONG)(((((LONGLONG)(a)) & 0xFFFFFFFF)) | (((((LONGLONG)(b)) & 0xFFFFFFFF)) << 32)))
		#define HILONG(a) (LONG)	((((LONGLONG)(a)) & 0xFFFFFFFF00000000) >> 32)
		#define LOLONG(a) (LONG)	(((LONGLONG)(a)) & 0xFFFFFFFF)
	#endif 
}


#endif //FTL_DEFINE_H