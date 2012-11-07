#pragma once

// Insert your headers here
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// �� Windows ��ͷ���ų�����ʹ�õ�����
#endif

// ���������ʹ��������ָ����ƽ̨֮ǰ��ƽ̨�����޸�����Ķ��塣
// �йز�ͬƽ̨����Ӧֵ��������Ϣ����ο� MSDN��

#ifndef WINVER
#define WINVER 0x0501
#endif

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0501
#endif						

#ifndef _WIN32_WINDOWS
#define _WIN32_WINDOWS 0x0501 
#endif

#ifndef _WIN32_IE
#define _WIN32_IE 0x0600
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// ĳЩ CString ���캯��������ʽ��

#  pragma warning(disable: 4995)  //'%s': name was marked as #pragma deprecated
#  pragma warning(disable: 4996)  //'%s': This function or variable may be unsafe.

#include <Windows.h>
#include <ShellAPI.h>

//ʹ��ATL
#include <atlbase.h>
#include <atlstr.h>
#include <atltypes.h>

//ʹ��WTL
#include <atlapp.h>
#include <atldlgs.h>
#include <atlcrack.h>
#include <atlddx.h>
#include <atlgdi.h>
#include <atlctrls.h>
#include <atlctrlw.h>
#include <atlctrlx.h>

#include <streams.h>

//ʹ��FTL
//#define INCLUDE_DETECT_DDRAW	1
#define INCLUDE_DETECT_INTEL_MEDIA_SDK	1
#define INCLUDE_DETECT_MEDIAOBJ	1
#define INCLUDE_DETECT_MEDIA_FOUNDATION	1
//#define INCLUDE_DETECT_KSPROXY	1
//#define	INCLUDE_DETECT_QEDIT	1
//#define INCLUDE_DETECT_SHLOBJ	1
#define INCLUDE_DETECT_STRMIF	1
#define	INCLUDE_DETECT_WM_CODEC_DSP	1
#define INCLUDE_DETECT_WMF		1

#include <ftlbase.h>
#include <ftlcom.h>
#include <ftlComDetect.h>
#include <ftlDirectx.h>
#include <ftlControls.h>

//#include <ftlhardware.h>
using namespace FTL;

