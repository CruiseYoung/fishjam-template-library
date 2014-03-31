#ifndef _POCO_STUDY_H
#define _POCO_STUDY_H

#pragma once

//http://blog.csdn.net/arau_sh/article/details/8620810

//���ٵĿ�ƽ̨���� -- STL + boost + Poco + Qt + Mysql ��һ����������

#include "Poco/Config.h"        //���붨��꣬����ָ������汾��һЩ����
#include "Poco/Foundation.h"    //����汾���������,������Dll���������
#include "Poco/Platform.h"      //֧��ƽ̨���壬����ϵͳ��Ӳ���ṹ��ϵ����
//#include "Poco/Platform_WIN32.h"   //�ض�ƽ̨����ƽ̨���Զ���


/****************************************************************************************************
* Poco(POrtable COmponents), http://pocoproject.org/ -- ��Դ�������ڻ�����Ӧ�ã�
*   ʹ��Boost Software License��������ȫ���
*   �ṩ��ƽ̨(Windows/WinCE/Linux/Mac OS X/Solaris/Android ��)�Ļ�����⣬����Ϊ��һ��С�͵�ACE?
*   ������Foundation��Crypto��XML��Util��Net��Zip��NetSSL��Data��֧�� SQLite��ODBC��MySQL����
*          Tools & Utilities��Application ��
* 
* ��ƽ̨�������
*   1.VC�±������û��Ԥ����ͷ�ļ�����ʽ, û��stdafx.h ��stdafx.cpp
*   2.���е�Dll����û����Դ��û��dllmain���ڵĹ���
*
* ����
*   TODO: ����ʱ����־����ͨ���ض��򱣴浽�ı�����
*   0.Poco��ֳɺܶ����� -- (Debug/Release) * (Share/Static) * (MultiThread/MultiThreadDLL), ���У���̬����Ҫ���� POCO_STATIC ��
*   1.���� buildwin.cmd �������������е� OPENSSL_DIR,OPENSSL_LIB
*   1.5. -- TODO: MySQL ��ôŪ����Ҫ�ӵ�VS������·���У������ buildwin.cmd ?
*     set MYSQL_DIR=D:\MaNGOS\MaNGOSfour\dep
*     set MYSQL_INCLUDE=%MYSQL_DIR%\include\mysql
*     set MYSQL_LIB=%MYSQ_DIR%\lib\win32_debug;%MYSQ_DIR%\lib\win32_release;
*     set INCLUDE=%INCLUDE%;%OPENSSL_INCLUDE%;%MYSQL_INCLUDE%
*     set LIB=%LIB%;%OPENSSL_LIB%;%MYSQL_LIB%
*   2.ִ�ж�Ӧ build_vs90.cmd ��������
* 
* ����
*   1.Poco�ж�����һЩ�꣬����ɱ�׼API(�� OutputDebugString) C2065(undeclared identifier), C3861(identifier not found)�Ĵ���
*     �����a.����ͷ�ļ��İ���˳��?
*            b.ʹ�ñ����ջ���ƺ꣬�� verify
*              #pragma push_macro("verify")
*              #undef verify
*              include����Pocoͷ�ļ�
*              #pragma pop_macro("verify")

* ����� -- ���� Poco/Config.h �ļ�? ���Կ����߳�������������Ƿ�֧��UTF8(Ĭ��֧��)��
****************************************************************************************************/


/****************************************************************************************************
* ���������������� Poco/Types.h �У������˿�ƽ̨�����Ͷ���(�� Poco::Int8, Poco::Int16, Poco::IntPtr ��)
*   
* ����
*   ��Ҫ���� $(POCO_BASE) �������� ?
*
* ��ƽ̨����( $(POCO_BASE)/build )
*   Linux/Unix -- �� Makefile ����Ҫ include $(POCO_BASE)/build/rules/global ?
*   Android/MacOS -- �ͻ���ʹ��ʱ���붨�� POCO_ANDROID �꣬���������ļ�Ϊ <POCO>/build/config/Android
*     �ο��ĵ�: http://www.appinf.com/docs/poco/99300-AndroidPlatformNotes.html
*                <POCO>/doc/99300-AndroidPlatformNotes.page
*     ���ƣ�Android�²�֧�����µ���: NamedEvent/NamedMutex/SharedMemory/RWLock ��
*     0.��ִ�нű�ʱ���ܻᱨ "$'\r': command not found" �Ĵ��� ����Ҫʹ�� dos2unix �����Ƚ��ű�ת����unix��ʽ
*       ��Ҫ���ĵ��ļ�Ϊ /<poco> Ŀ¼�µ� build/* �� rules/* ��
*     1.�����Զ���� Standalone Toolchain
*       $NDK/build/tools/make-standalone-toolchain.sh --platform=android-8 --install-dir=$HOME/my-android-toolchain
*       ע�⣺������Ҫ���� "--system=windows-x86_64" ������ʲô��˼��
*     2.���Զ���toolchain��binĿ¼����PATH��������(.bash_profile): export PATH=$PATH:$HOME/my-android-toolchain/bin
*     3.����: ./configure --config=Android --no-samples --no-tests 
*              make -- ����ͨ�� ANDROID_ABI=armeabi-v7a �ı�ȱʡ�� platform ABI
*       TODO:Ҳ����ͨ������ Android.mk ��ʹ��NDK�����룬���?
*     4.Android ��ʹ��Poco
*       Android.mk ��:
*         LOCAL_CPP_FEATURES := rtti exceptions
*       Application.mk ��:
*         APP_STL := gnustl_shared
****************************************************************************************************/

/***************************************************************************************************
�����б�:

DynamicAny��Any��
Cache���
����OpenSSL������ϵͳ
���ڣ�ʱ�����
�¼���֪ͨ���
FTP�ͻ���
��ƽ̨���ļ�ϵͳ���
HTML�����
HTTP�ͻ��˺ͷ���ˣ�֧��SSL����C++ ������ҳ�������
��־���
���߳̿�ܣ��̳߳أ�����󣬹������еȣ�
POP3�ͻ������
��ƽ̨��һ�α�д����ƽ̨���������
���̹���ͽ��̼�ͨ�����
������
����PCRE��������ʽ
SMTP�ͻ������
���ݿ������⣨SQLite,MySQL, ODBC)
֧��SSL/TLS������OpenSSL
��̬������
�Ƚ����ڴ��ָ�����Buffer, Pool��
Socket���
������������⣬֧��Base64,HexBinary����룬ѹ���ȵ�
�ַ�����ʽ���������ַ����������
TCP��������ܣ����̣߳�
�ı������ת��
Tuples
URI֧��
֧��UTF8��Unicode����
UUID������
XML���ɺͽ�����
Zip�ļ��������
****************************************************************************************************/

#endif _POCO_STUDY_H