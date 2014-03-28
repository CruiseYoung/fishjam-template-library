package com.fishjam.android.study;

import android.test.ActivityTestCase;

/***************************************************************************************************************************************
 * NDK(Native Development Kit)--http://developer.android.com/tools/sdk/ndk/index.html
 *   �ٷ��ṩ�Ĺ��ߣ����Կ��� C/C++�Ķ�̬�⣬�ṩ .mk �ļ�����ƽ̨��CPU��API�Ȳ���(ָ�������ļ�����������)�����Զ���so��javaӦ��һ������apk��
 *   NDK���ṩ��API��C��׼��(libc), ��׼��ѧ��(libm),ѹ����(libz),log��(liblog)��ý���(libjnigraphics), OpenGL ES, JNI�ӿ�ͷ�ļ� ��
 *   Ӧ�ó�����������Ӧ���߼�����Ҫ���ܵ�Ӧ���߼�
 *   ȱ�㣺���������(App��Ҫͨ��JNI����)�����������Ա�֤���޷�����Framework API��Debug�Ѷȴ��
 * 
 * JNI -- 
****************************************************************************************************************************************/
/****************************************************************************************************************************************
* �����(Windows ��) -- TODO:�ƺ����� cygwin Ҳ����?
*   1.���ض�Ӧƽ̨��SDK��NDK����STL debug info����
*   2.��װcygwin(1.7 ����)��http://www.cygwin.com, ��Ҫ GNU Make, AWK, 
*     ����վ�㣺http://mirrors.kernel.org �ȱȽϿ�
*     ѡ��װ����autoconf2.1,automake1.10,binutils,gcc-core,gcc,g++,gcc4-core,gcc4-g++,gdb,pcre,pcre-devel,gawk,make ��12����
*     ���ԣ�a.����Cygwin������ cygcheck -c cygwin ������ӡ����ǰcygwin�İ汾������״̬;
*           b.gcc -v; g++ --version; make --version; gdb --version �Ȳ���
*   3.����NDK��������:<cygwin>\<user>\.bash_profile ������ NDK=/cygdrive/<ndk_path>; export NDK
*     ����: cd $NDK<CR> ȷ���Ƿ񵽶�Ӧ��Ŀ¼
*     TODO: �� ndk_build ��������Ŀ¼���� $PATH ����? export PATH=${PATH}:$NDK
*   4.���� Eclipse:
*     a.���� CDT ���;
*     b.������Ŀ��, Properties->Builders-> Newһ����������ѡ��Program -> 
*       ����ΪC_Builder,LocationΪ<cygwin>/bin/bash.exe,ArgumentsΪ --login -c "cd <projectpath> && $NDK/ndk-build";
*       �л���"Refresh"ѡ�����ѡ"Refresh resources upon completion";�л���"Build Options"��ѡ�������(����?);
*       "Specify Resources"->��ѡ��ĿĿ¼��Ϊ��ԴĿ¼��
*
* ����
*   0.Mac/Linux��������Ҫִ�� build/tools/make-standalone-toolchain.sh --platform=android-8 --install-dir=$HOME/my-android-toolchain
*   1.��ԭ��������� <project>/jni/ Ŀ¼��
*   2.���� <project>/jni/Android.mk �����������Ϣ,��ϸ���òμ� <ndk>/docs/ANDROID-MK.html
*   3.[��ѡ]���� <project>/jni/Application.mk����ϸ���òμ� <ndk>/docs/ANDROID-MK.html
*   4.cd <project>, ʹ�� <ndk>/ndk-build �ű�����, ����ɹ�������� libs/Xxx.so 
*   5.����ʹ��.so��App,�����ַ�ʽ��
*     a.ʹ�� JNI ���ʣ��÷�ʽ�������� Android framework ������;
*     b.�� NativeActivity �̳�(Android 2.3 �Ժ�֧��)
*     ע��:App��Manifest����Ҫͨ�� <uses-sdk android:minSdkVersion="xx" /> ����ʹ����native library
*   5.������App�����Զ�����ӦĿ¼�µ� .so ����� .apk �ļ���
****************************************************************************************************************************************/
public class NdkTester extends ActivityTestCase {

}
