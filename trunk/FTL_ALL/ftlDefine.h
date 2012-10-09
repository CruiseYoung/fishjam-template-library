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
    //使用模版的export关键字，将声明和实现分开 -- 但目前VS2003~VS2008都不支持 export 关键字
    //考虑：是否可以扩展该宏实现静态FTL库？参考 ATL_DLL_ 宏的用法？
    #ifdef USE_EXPORT
    #  define FTLEXPORT  export
    #  define FTLINLINE
    #else
    #  define FTLEXPORT
    #  define FTLINLINE inline
    #endif

        //调试版本下自动定义 FTL_DEBUG，RELEASE 版本下可以手工定义 FTL_DEBUG
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
    * 宏展开是按字符顺序扫描的
    * JOIN_ONE(var_, __LINE__) 中，首先扫描到JOIN_ONE，因此结果直接生成 var___LINE__, 展开结束
    * JOIN_TWO(var_, __LINE__) 中，首先扫描到JOIN_TWO，展开成中间结果
    *   JOIN_ONE(var_, __LINE__)， 在此次展开中，再看到__LINE__，替换为 JOIN_ONE(var_，11)，
    *   然后再进行第二次展开，生成 var_11  -- 可以根据当前的行号，生成“临时的唯一”变量
    /**********************************************************************************************/
    #define JOIN_ONE(x, y) x ## y
    #define JOIN_TWO(x, y) JOIN_ONE(x, y)

    //用于在编译器的输出窗口(Output)中输出“TODO”信息，可以双击直接进行定位
    //使用方式：pragma TODO(Fix this later)
    #define TODO(desc) message(__FILE__ "(" QQUOTE(__LINE__) ") : TODO: " #desc)

    //处理虚拟按键(如 VK_ESCAPE )， 或者在 WM_KEYDOWN 中 (GetAsyncKeyState(vk_code) & 0x01) ?
    #define KEY_DOWN(vk_code)   ( (GetAsyncKeyState(vk_code) & 0x8000) ? TRUE  : FALSE ) 
    #define KEY_UP(vk_code)     ( (GetAsyncKeyState(vk_code) & 0x8000) ? FALSE : TRUE  )

	#define FTL_MAX_CLASS_NAME_LENGTH	256

    //换行符(10,A)
    #define LF  '\n'
    //回车符(13,D)
    #define CR  '\r'

    //注意，需要区分Dos和Unix
    #define LINE_SEP    CR LF

    //定义来查找一个数组中的 past-the-end 位置，使用方式:
    //  std::find(intArray, ARRAY_PAST_THE_END(intArray), 10);
    #define ARRAY_PAST_THE_END(c) ( c + _countof( c ) )

    //用于<重载时比较两个成员的对象变量
    #define COMPARE_OBJ_LESS(f,l,r) \
        if( l.f < r.f ) { return true; } \
        else if( l.f > r.f ) { return false; }
        
    //定义比较成员变量的宏，常用于重载类、结构的 operator < 时（注意：最后一个变量时），参数分别为 field , other&
    #define COMPARE_MEM_LESS(f, o) \
        if( f < o.f ) { return true; }\
        else if( f > o.f ) { return false; }

	#ifndef MAKELONGLONG
		#define MAKELONGLONG(a, b)	((LONGLONG)(((((LONGLONG)(a)) & 0xFFFFFFFF)) | (((((LONGLONG)(b)) & 0xFFFFFFFF)) << 32)))
		#define HILONG(a) (LONG)	((((LONGLONG)(a)) & 0xFFFFFFFF00000000) >> 32)
		#define LOLONG(a) (LONG)	(((LONGLONG)(a)) & 0xFFFFFFFF)
	#endif 
}


#endif //FTL_DEFINE_H