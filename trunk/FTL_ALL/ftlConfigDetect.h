#ifndef FTL_CONFIG_DETECT_H
#define FTL_CONFIG_DETECT_H

#pragma once

#ifndef FTL_BASE_H
#  error ftlConfigDetect.h requires ftlbase.h to be included first
#endif
/*************************************************************************************************************************
* �Ѷ�����
*   begin\winsdkver_button\winsdkver_button -- �� WinSDKVer.h ͷ�ļ���ȡ�õ�ǰSDK֧�ֵ����汾����(_WIN32_MAXVER)������Ӧ����
*************************************************************************************************************************/


//#include <atlassem.h>
/************************************************************************
* �����жϵ�ǰ��������ѡ�ͨ������ stdafx.h �ļ������·�
* ����ʹ�����·�����������Link -- _MFC_FILENAME_VER ��һ���ַ����ĺ�(VS2008 ���� "90")
*   #pragma comment(lib, "mfc" _MFC_FILENAME_VER "d.lib")
*
************************************************************************/


//#if defined(_MSC_VER) && (_MSC_VER >= 1020)

#  pragma message(__FILE__ "(" QQUOTE(__LINE__) ") : Begin IDE Config Detect.................................." )

#if defined(_INC_WINSDKVER)
//<WinSDKVer.h> ͷ�ļ��ж����˵�ǰSDK֧�ֵ����汾�������� _WIN32_MAXVER��_WIN32_IE_MAXVER ��
#    pragma message( "_WIN32_MAXVER = "			QQUOTE(_WIN32_MAXVER))
#    pragma message( "_WIN32_WINDOWS_MAXVER = " QQUOTE(_WIN32_WINDOWS_MAXVER))
#    pragma message( "NTDDI_MAXVER = "			QQUOTE(NTDDI_MAXVER))
#    pragma message( "_WIN32_IE_MAXVER = "		QQUOTE(_WIN32_IE_MAXVER))
#    pragma message( "_WIN32_WINNT_MAXVER = "	QQUOTE(_WIN32_WINNT_MAXVER))
#    pragma message( "WINVER_MAXVER = "			QQUOTE(WINVER_MAXVER))

////ͨ�������� targetver.h �а������´��룬ʹ�û�ȡ��SDK֧�ֵ�����ܣ�����Ӧ����
//#ifndef WINVER
//#  define WINVER _WIN32_MAXVER
//#endif
//#ifndef _WIN32_WINNT
//#  define _WIN32_WINNT _WIN32_WINNT_MAXVER
//#endif
//#ifndef _WIN32_WINDOWS
//#  define _WIN32_WINDOWS _WIN32_WINDOWS_MAXVER
//#endif
//#ifndef _WIN32_IE
//#  define _WIN32_IE _WIN32_IE_MAXVER
//#endif

#endif //_INC_WINSDKVER

//�Ƿ����ANSI��׼
#  if defined(__STDC__)
#    pragma message( "  defined __STDC__" )
#  else
#    pragma message( "  not defined __STDC__" )
#  endif

//�жϵ�ǰ���뻷���µ� Manifest ��Ϣ
#  pragma message( "  Manifest Info:")

#  if defined(_CRT_ASSEMBLY_VERSION)
#    if defined(_DEBUG)
#      pragma message( "    CRT Manifest = " __LIBRARIES_ASSEMBLY_NAME_PREFIX ".DebugCRT_" _VC_ASSEMBLY_PUBLICKEYTOKEN "_" _CRT_ASSEMBLY_VERSION)
#    else
#      pragma message( "    CRT Manifest = " __LIBRARIES_ASSEMBLY_NAME_PREFIX ".CRT_" _VC_ASSEMBLY_PUBLICKEYTOKEN "_" _CRT_ASSEMBLY_VERSION)
#    endif 
#  endif 

// ��Ϊ ATL ��ģ�棬����û�� Debug �� Release ������?
#  if defined(_ATL_ASSEMBLY_VERSION)
#    pragma message( "    ATL Manifest = " __LIBRARIES_ASSEMBLY_NAME_PREFIX ".ATL_" _VC_ASSEMBLY_PUBLICKEYTOKEN "_" _ATL_ASSEMBLY_VERSION)
#  endif 

#  if defined(_MFC_ASSEMBLY_VERSION)
#    if defined(_DEBUG)
#      pragma message( "    MFC Manifest = " __LIBRARIES_ASSEMBLY_NAME_PREFIX ".DebugMFC_" _VC_ASSEMBLY_PUBLICKEYTOKEN "_" _MFC_ASSEMBLY_VERSION)
#    else
#      pragma message( "    MFC Manifest = " __LIBRARIES_ASSEMBLY_NAME_PREFIX ".MFC_" _VC_ASSEMBLY_PUBLICKEYTOKEN "_" _MFC_ASSEMBLY_VERSION)
#    endif
#  endif 

#  if defined(__OPENMP_ASSEMBLY_VERSION)
#    pragma message( "    OpenMP Manifest = " __LIBRARIES_ASSEMBLY_NAME_PREFIX ".ATL_" _VC_ASSEMBLY_PUBLICKEYTOKEN "_" __OPENMP_ASSEMBLY_VERSION)
#  endif 


//�жϱ���������
#if defined(__cplusplus)
#  pragma message( "  C++ Compiler" )
#else
#  pragma message( "  C Compiler" )
#endif

//Ŀ��ƽ̨(Ŀǰ����ȫ)
#if defined(_WIN32) || defined(WIN32) || defined(_WIN64)         // Windows
#  pragma message( "  Target Platform is Windows" )
#elif defined(__APPLE__) || defined(_MAC)   //Mac OS X
#  pragma message( "  Target Platform is Mac OS" )

//ע�⣺���µĶ��嶼��M2000�еģ���һ���ڱ�ĵط�����
#elif defined(SUN) || defined(_SUN_)
#  pragma message( "  Target Platform is SUN Solaris" )
#elif defined(SUSE_IA64)
#  pragma message( "  Target Platform is SUSE on HP" )
#elif defined(SUSE_X86)
#  pragma message( "  Target Platform is SUSE on ATAE" )
#elif defined(WIN32) || defined(_WIN64)
#  pragma message( "  Target Platform is Windows" )

#else
#  pragma message( "  Unknown Target Platform" )
#endif 

#if defined(ORACLE)
#  pragma message( "  Database is ORACLE" )
#elif defined(SYBASE)
#  pragma message( "  Database is SYBASE" )
#elif defined(DB2)
#  pragma message( "  Database is DB2" )
#else
#  pragma message( "  Unknown Database" )
#endif

#if defined(_M_IX86)
#  pragma message( "  defined _M_IX86" )			//32bit������
#elif defined(_M_IX64)
#  pragma message( "  defined _M_X64")				//64bit Itanium������
#elif defined(_M_X64)
#  pragma message( "  defined _M_X64")				//64bit AMD(AMD64) and Intel(EM64T)������(VC2008���Ժ�), ����X86
#elif defined(_M_IA64)
#  pragma message( "  defined _M_IA64")             //��ȫ��64λ��������x86
#elif defined(_M_AMD64)
#  pragma message( "  defined _M_AMD64" )			//64bit AMD������ (VC2008��ǰ),����X86
#else
#  pragma message( "Unknown CPU Type")
#endif

//�ж��Ƿ���64λ���� -- 
#if defined(_WIN64) || defined(WIN64)
#  pragma message( "  WIN64 Version" )		//_WIN64 ֻ��64�϶���
#elif defined (WIN32)
#  pragma message( "  WIN32 Version" )		//_WIN32 ��32��64�϶��ᶨ��
#else 
#  pragma message( "  Unknown WinXX Version" )
#endif

//�ж�Ŀ����������
#if defined(_WINDLL)
#  pragma message( "  Configuration Type is 'Dynamic Library (.dll)' " )
#elif defined(_LIB)
#  pragma message( "  Configuration Type is 'Static Library (.lib)' " )
#else
#  pragma message( "  Configuration Type is 'Application (.exe)' or 'Static Library (.lib)' " )
#endif 

//���������İ汾
#if defined _MSC_VER  
#  pragma message( "  _MSC_VER = " QQUOTE(_MSC_VER) )
#  if _MSC_VER == 1200
#    pragma message( "     IDE is VC6")  //ʵ��������ftlbase.h ʹ���� strsafe.h ���ļ��ͺ���������ǲ�֧�ֵ�
#  elif _MSC_VER == 1300
#    pragma message( "     IDE is VC70")
#  elif _MSC_VER == 1310
#    pragma message( "     IDE is VC71")
#  elif _MSC_VER == 1400
#    pragma message( "     IDE is VS2005")
#  elif _MSC_VER == 1500
#    pragma message( "     IDE is VS2008")
#  elif _MSC_VER == 1600
#    pragma message( "     IDE is VS2010")
#  elif _MSC_VER == 1700
#    pragma message( "     IDE is VS2012")
#  else
#    pragma message( "     IDE is Unknow Type")
#  endif
#endif

//�ж��Ƿ���Unicode�汾
//  UNICODE is used by Windows headers
//  _UNICODE is used by C/C++ runtime/MFC headers
#if defined (UNICODE) || defined (_UNICODE)
#  pragma message( "  Use Unicode Character Set" )			//wchar_t
#elif defined (_MBCS)
#  pragma message( "  Use Multi-Byte Character Set" )		//unsigned char ���� char ?
#else
#  pragma message( "  Not Set Character Set" )				//char
#endif //UNICODE


//�ж���Debug�汾����Release�汾
#if defined (DEBUG) || defined(_DEBUG)
#  pragma message( "  DEBUG Version, defined ( DEBUG or _DEBUG )")
#else
#  pragma message( "  Release Version, not define ( DEBUG )")
#endif //DEBUG

//�ж��Ƿ�֧�ֶ��߳̿�
#if defined _MT
#  pragma message( "  Multi Thread Version" )
#else
#  pragma message( "  Single Thread Version" )
#endif //_MT


//�ж��Ƿ�֧��CLR
#ifdef _MANAGED
#  pragma message( "  Managed C++ -- /CLR" )
#else
#  pragma message( "  not support Managed C++" )
#endif 

#if defined ( AFX_NO_CLR_COINIT_STA )
#  pragma message( "  defined AFX_NO_CLR_COINIT_STA" )
#else
#  pragma message( "  not defined AFX_NO_CLR_COINIT_STA" )
#endif


#if defined(_WIN32_WCE)
#  pragma message( "  CE Specific _WIN32_WCE = " QQUOTE(_WIN32_WCE))
#endif 

#if defined(_DLL)
#  pragma message( "  CRT Dll version(/MD[d]) -- _DLL" )
#else
#  pragma message( "  CRT Static Lib Version(/MT[d])" )
#endif 

#if defined(_USRDLL)
#  pragma message( "  defined _USRDLL" )
#else
#  pragma message( "  not defined _USRDLL" )
#endif


//�ж�Ҫ����С��ϵͳ��IE�汾
#ifdef WINVER
#  pragma message( "  WINVER = " QQUOTE(WINVER) )
#  if WINVER >= 0x0601
#    pragma message( "     Minimum system required: Windows 7")
#  elif WINVER >= 0x0600
#    pragma message( "     Minimum system required: Windows Vista")
#  elif WINVER >= 0x0502
#    pragma message( "     Minimum system required: Windows Server 2003")
#  elif WINVER >= 0x0501
#    pragma message( "     Minimum system required: Windows XP | Windows Me")
#  elif WINVER >= 0x0500
#    pragma message( "     Minimum system required: Windows 2000")
#  elif WINVER >= 0x0410
#    pragma message( "     Minimum system required: Windows 98")
#  elif WINVER >= 0x0400
#    pragma message( "     Minimum system required: Windows 95 | Windows NT 4.0")
#  else
#    pragma message( "     Minimum system required: Not Known")
#  endif 
#endif //WINVER

#ifdef _WIN32_WINNT
#  pragma message( "  _WIN32_WINNT = " QQUOTE(_WIN32_WINNT) )
#endif //_WIN32_WINNT

# ifdef _WINDOWS
#  pragma message( "  defined _WINDOWS" )
# else
#  pragma message( "  not defined _WINDOWS" )
# endif 

# ifdef STRICT
#  pragma message( "  defined STRICT" )
# else
#  pragma message( "  not defined STRICT" )
# endif 

# ifdef INITGUID
#  pragma message( "  defined INITGUID" )
# else
#  pragma message( "  not defined INITGUID" )
# endif 

#ifdef _WIN32_WINDOWS
#  pragma message( "  _WIN32_WINDOWS = " QQUOTE(_WIN32_WINDOWS) )
#endif //_WIN32_WINDOWS

#ifdef _WIN32_IE
#  pragma message( "  _WIN32_IE = " QQUOTE(_WIN32_IE) )
#  if _WIN32_IE >= 0x0800
#    pragma message( "     Minimum Internet Explorer required: 8.0")
#  elif _WIN32_IE >= 0x0700
#    pragma message( "     Minimum Internet Explorer required: 7.0")
#  elif _WIN32_IE >= 0x0603
#    pragma message( "     Minimum Internet Explorer required: 6.0 SP2")
#  elif _WIN32_IE >= 0x0601
#    pragma message( "     Minimum Internet Explorer required: 6.0 SP1")
#  elif _WIN32_IE >= 0x0600
#    pragma message( "     Minimum Internet Explorer required: 6.0")
#  elif _WIN32_IE >= 0x0560
#    pragma message( "     Minimum Internet Explorer required: 5.6")
#  elif _WIN32_IE >= 0x0501
#    pragma message( "     Minimum Internet Explorer required: 5.01, 5.5")
#  elif _WIN32_IE >= 0x0500
#    pragma message( "     Minimum Internet Explorer required: 5.0, 5.0a, 5.0b")
#  elif _WIN32_IE >= 0x0401
#    pragma message( "     Minimum Internet Explorer required: 4.01")
#  elif _WIN32_IE >= 0x0400
#    pragma message( "     Minimum Internet Explorer required: 4.0")
#  elif _WIN32_IE >= 0x0300
#    pragma message( "     Minimum Internet Explorer required: 3.0, 3.01, 3.02")
#  else
#    pragma message( "     Minimum Internet Explorer required: Not Known")
#  endif 
#endif //_WIN32_IE

//NTDDI_WINXP��NTDDI_LONGHORN��
#ifdef NTDDI_VERSION
#  pragma message( "  NTDDI_VERSION = " QQUOTE(NTDDI_VERSION) )
#  if (NTDDI_VERSION >= 0x06010000)
#    pragma message( "     Minimum system required: Windows 7")
#  elif defined(NTDDI_LONGHORN) && (NTDDI_VERSION >= NTDDI_LONGHORN)
#    pragma message( "     Minimum system required: Windows Vista")
#  elif defined(NTDDI_WS03SP1) && (NTDDI_VERSION >= NTDDI_WS03SP1)
#    pragma message( "     Minimum system required: Windows Server 2003 SP1")
#  elif defined(NTDDI_WS03) && NTDDI_VERSION >= NTDDI_WS03
#    pragma message( "     Minimum system required: Windows Server 2003")
#  elif defined (NTDDI_WINXPSP2) && NTDDI_VERSION >= NTDDI_WINXPSP2
#    pragma message( "     Minimum system required: Windows XP SP2")
#  elif defined (NTDDI_WINXPSP1) && NTDDI_VERSION >= NTDDI_WINXPSP1
#    pragma message( "     Minimum system required: Windows XP SP1")
#  elif defined (NTDDI_WINXP) && NTDDI_VERSION >= NTDDI_WINXP
#    pragma message( "     Minimum system required: Windows XP")
#  elif defined (NTDDI_WIN2KSP4) && NTDDI_VERSION >= NTDDI_WIN2KSP4
#    pragma message( "     Minimum system required: Windows 2000 SP4")
#  elif defined (NTDDI_WIN2KSP3) && NTDDI_VERSION >= NTDDI_WIN2KSP3
#    pragma message( "     Minimum system required: Windows 2000 SP3")
#  elif defined (NTDDI_WIN2KSP2) && NTDDI_VERSION >= NTDDI_WIN2KSP2
#    pragma message( "     Minimum system required: Windows 2000 SP2")
#  elif defined (NTDDI_WIN2KSP1) && NTDDI_VERSION >= NTDDI_WIN2KSP1
#    pragma message( "     Minimum system required: Windows 2000 SP1")
#  elif defined (NTDDI_WIN2K) && NTDDI_VERSION >= NTDDI_WIN2K
#    pragma message( "     Minimum system required: Windows 2000")
#  else
#    pragma message( "     Minimum system required: Not Known")
#  endif
#endif //NTDDI_VERSION

//���ֿ�İ汾
#ifdef _RICHEDIT_VER
#  pragma message( "  _RICHEDIT_VER = " QQUOTE(_RICHEDIT_VER) )
#endif 

//GdiPlus �İ汾
#ifdef GDIPVER
#  pragma message( "  GDIPVER = " QQUOTE(GDIPVER) )
#endif 

//�ж�ʹ�õĸ��ֿ⣺��MFC��ATL��WTL��
#ifdef __AFX_H__
#  pragma message( "  Use MFC, _MFC_VER = " QQUOTE(_MFC_VER) )

#  if defined (_AFXDLL)
#    pragma message( "    Use MFC in a Shared DLL")
#  else
#    pragma message( "    Use MFC in a Static Library")
#  endif

#  if defined (_NATIVE_WCHAR_T_DEFINED) //Treat wchar_t as a native(Built-in) type(MFC use this way)
#    pragma message( "    Treat wchar_t as Built-in Type")
#  else
#    pragma message( "    Treat wchar_t as unsigned short")
#  endif

#  if defined (_AFX_MINREBUILD)
#    pragma message( "    defined _AFX_MINREBUILD")
#  else
#    pragma message( "    not defined _AFX_MINREBUILD")
#  endif

#  if defined (_AFX_FULLTYPEINFO)
#    pragma message( "    defined _AFX_FULLTYPEINFO = " QQUOTE(_AFX_FULLTYPEINFO) )
#  endif

#  if defined( _AFX_NOFORCE_LIBS )
#    pragma message( "    defined _AFX_NOFORCE_LIBS")
#  else
#    pragma message( "    not defined _AFX_NOFORCE_LIBS")
#  endif 

#  if defined(_AFX_NO_DEBUG_CRT)
#    pragma message( "    defined _AFX_NO_DEBUG_CRT")
#  else 
#    pragma message( "    not defined _AFX_NO_DEBUG_CRT")
#  endif

#  if defined( _AFX_NOFORCE_MANIFEST )
#    pragma message( "    defined _AFX_NOFORCE_MANIFEST")
#  else
#    pragma message( "    not defined _AFX_NOFORCE_MANIFEST")
#  endif 

#  if defined(_AFX_PACKING)
#    pragma message( "    _AFX_PACKING = " QQUOTE(_AFX_PACKING))
#  endif
#endif //__AFX_H__

//ATL���п��� MIDL_DEFINE_GUID �Ƿ����ɶ���
#ifdef _MIDL_USE_GUIDDEF_
#  pragma message( "    defined _MIDL_USE_GUIDDEF_")
#else
#  pragma message( "    not defined _MIDL_USE_GUIDDEF_")
#endif //_MIDL_USE_GUIDDEF_

#ifdef __ATLBASE_H__
#  pragma message( "  Use ATL, _ATL_VER = " QQUOTE(_ATL_VER))

#  if defined(_ATL_DLL)
#    pragma message( "    defined _ATL_DLL")
#  endif 

#  if defined(_ATL_STATIC_REGISTRY)
#    pragma message( "    defined _ATL_STATIC_REGISTRY")
#  endif 

#  if defined(_ATL_DLL)		//���� atl.lib, ����ʱ���� ATL.dll
#    pragma message( "    Dynamic Link to ATL")
#  elif defined(_ATL_STATIC_REGISTRY)
#    pragma message( "    Static Link to ATL")		//Ϊʲôѡ����ѡ�����"Not Using ATL"����ȴ��鵽������ _ATL_STATIC_REGISTRY ��
#  else
#    pragma message( "    Not Using ATL")
#  endif 

#  ifdef _ATL_APARTMENT_THREADED
#    pragma message( "    defined _ATL_APARTMENT_THREADED")
#else
#    pragma message( "    not defined _ATL_APARTMENT_THREADED")
#endif

#  ifdef _ATL_DLL_IMPL
#    pragma message( "    defined _ATL_DLL_IMPL")
#  else
#    pragma message( "    not defined _ATL_DLL_IMPL")
#  endif 

// ATL �кϲ� Proxy/Stub 
#  ifdef _MERGE_PROXYSTUB
#    pragma message( "    defined _MERGE_PROXYSTUB")
#  else
#    pragma message( "    not defined _MERGE_PROXYSTUB")
#  endif

#  ifdef _ATL_MIN_CRT
//   ע�⣺���ѡ���Ѿ�����֧���ˣ���ò�Ҫʹ��(�μ� atlbase.h )
#    pragma message( "    defined _ATL_MIN_CRT")
#    pragma TODO(Using no longer supported _ATL_MIN_CRT)
#  else
#    pragma message( "    not defined _ATL_MIN_CRT")
#  endif

//mixing compilation units that are built as native code with those that are built /clr
#  ifdef _ATL_MIXED
#    pragma message( "    defined _ATL_MIXED")
#  else
#    pragma message( "    not defined _ATL_MIXED")
#  endif 

#  ifdef _ATL_NO_AUTOMATIC_NAMESPACE
#    pragma message( "    defined _ATL_NO_AUTOMATIC_NAMESPACE")
#else
#    pragma message( "    not defined _ATL_NO_AUTOMATIC_NAMESPACE")
#endif

#  ifdef _ATL_NO_COM_SUPPORT
#    pragma message( "    defined _ATL_NO_COM_SUPPORT")
#  else
#    pragma message( "    not defined _ATL_NO_COM_SUPPORT")
#  endif

#  ifdef _ATL_NO_COMMODULE
#    pragma message( "    defined _ATL_NO_COMMODULE")
#  else
#    pragma message( "    not defined _ATL_NO_COMMODULE")
#  endif

//����ȱʡ��������?
#  ifdef _ATL_NO_DEFAULT_LIBS
#    pragma message( "    defined _ATL_NO_DEFAULT_LIBS")
#  else
#    pragma message( "    not defined _ATL_NO_DEFAULT_LIBS")
#  endif 

#endif //__ATLBASE_H__

#ifdef __ATLAPP_H__
#  pragma message( "  Use WTL, _WTL_VER = " QQUOTE(_WTL_VER))
#endif //__ATLAPP_H__

#ifdef RC_INVOKED		//��Դ����������������꣬ʹ�����ܹ���ѡ��ر�����Դͷ�ļ��Ĳ�ͬ����
//���������ûʲô��
#    pragma message( "    defined RC_INVOKED")
#  else
#    pragma message( "    not defined RC_INVOKED")
#endif

#ifdef BOOST_MSVC
#  pragma message( "  Use Boost , BOOST_MSVC = " QQUOTE(BOOST_MSVC))
#  if defined(BOOST_NO_MEMBER_TEMPLATE_FRIENDS)
#    pragma message( "    defined BOOST_NO_MEMBER_TEMPLATE_FRIENDS")
#  endif
#  ifdef BOOST_MULTI_INDEX_HPP
#    pragma message( "    Use Boost MultiIndex")
#    if defined(BOOST_MULTI_INDEX_DISABLE_SERIALIZATION)
#      pragma message( "      defined BOOST_MULTI_INDEX_DISABLE_SERIALIZATION")
#    endif
#    if defined(BOOST_MULTI_INDEX_ENABLE_INVARIANT_CHECKING)    //ͨ�� DEBUG ʱ��
#      pragma message( "      defined BOOST_MULTI_INDEX_ENABLE_INVARIANT_CHECKING")
#    endif
#    if defined(BOOST_MULTI_INDEX_ENABLE_SAFE_MODE)             //ͨ�� DEBUG ʱ��
#      pragma message( "      defined BOOST_MULTI_INDEX_ENABLE_SAFE_MODE")
#    endif
#    if defined(BOOST_NO_POINTER_TO_MEMBER_TEMPLATE_PARAMETERS) 
#      pragma message( "      defined BOOST_NO_POINTER_TO_MEMBER_TEMPLATE_PARAMETERS")
#    endif
#  endif //BOOST_MULTI_INDEX_HPP
#endif //BOOST_MSVC

//�ж��Ƿ������� OPENMP ֧��
#  if defined(_OPENMP)
#  pragma message( "  Support OpenMP" )
#  else
#  pragma message( "  NOT Support OpenMP" )
#  endif

#  pragma message(__FILE__ "(" QQUOTE(__LINE__) ") : End IDE Config Detect.................................." )

#endif //FTL_CONFIG_DETECT_H
