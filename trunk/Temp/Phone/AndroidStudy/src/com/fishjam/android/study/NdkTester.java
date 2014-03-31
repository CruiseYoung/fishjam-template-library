package com.fishjam.android.study;

import android.test.ActivityTestCase;

/***************************************************************************************************************************************
* ����С��:
*   1.NDK��ʹ��STL -- �� Application.mk ������ APP_STL := stlport_static �� gnustl_shared ��(��ʲô����?)
***************************************************************************************************************************************/


/***************************************************************************************************************************************
 * NDK(Native Development Kit)--http://developer.android.com/tools/sdk/ndk/index.html
 *   �ٷ��ṩ�Ĺ��ߣ����Կ��� C/C++�Ķ�̬�⣬�ṩ .mk �ļ�����ƽ̨��CPU��API�Ȳ���(ָ�������ļ�����������)�����Զ���so��javaӦ��һ������apk��
 *   NDK���ṩ��API��C��׼��(libc), ��׼��ѧ��(libm),ѹ����(libz),log��(liblog)��ý���(libjnigraphics), OpenGL ES, JNI�ӿ�ͷ�ļ� ��
 *   Ӧ�ó�����������Ӧ���߼�����Ҫ���ܵ�Ӧ���߼�
 *   ȱ�㣺���������(App��Ҫͨ��JNI����)�����������Ա�֤���޷�����Framework API��Debug�Ѷȴ��
 * 
 * JNI(Java Native Interface) -- <jni.h>
 *   ������ -- ֱ�ӹ�JNI���õĺ���������Ҫ��ѭ Java_����_����_������ �ķ�ʽ��Java��ֻ��Ҫimport+ֱ��ʹ�ú����� ����
 *   ���� -- jlong, jobject,jstring ��
 *   JNIEnv* env-- ������,��JNI�ӿڵĵ�һ��������
 *   ע�⣺
 *     1.��ΪJava�л��ջ��ƣ����ܰ�ȫ�Ļ�úͲ���Java�����ָ��(��String��Ӧ���ַ�������)
 *     2.JNIEnv* evn �ĵ��÷�ʽ�� C/C++ �в�һ��
 *       C�еĵ��÷���Ϊ (*env)->NewStringUTF(env, "xxx"); C++ �еĵ��÷���Ϊ env->NewStringUTF("xxx");
 *     3.JNI �ӿڶ�����C��ʽ����ģ����C++�ļ��е���C��������Ҫ�� #ifdef __cplusplus extern "C" { #endif �ķ�ʽ
 ****************************************************************************************************************************************/
 
/****************************************************************************************************************************************
 * Ԥ����ı����
 *   ���л���(ARM CPU)-- �μ� hello-jni.c
 *     __arm__          -- ��Ӧͨ�õ� armeabi(ARMv5TE)
 *     __ARM_ARCH_7A__  -- ��Ӧ armeabi-v7a(ARMv7)
 *     __ARM_NEON__     -- ��Ӧ armeabi-v7a/NEON , ��Ҫ���� LOCAL_ARM_NEON:=true
 *     __i386__         -- ��Ӧ x86 ��ͨ����ģ����(i686)?
 *     __mips__         -- ��Ӧ mips(mips32)
****************************************************************************************************************************************/

/****************************************************************************************************************************************
* �����(Windows ��) -- TODO:�ƺ����� cygwin Ҳ����?
*   1.���ض�Ӧƽ̨��SDK��NDK����STL debug info����
*   2.��װcygwin(1.7 ����)��http://www.cygwin.com, ��Ҫ GNU Make, AWK, 
*     ����վ�㣺http://mirrors.kernel.org �ȱȽϿ�
*     ѡ��װ����autoconf2.1,automake1.10,binutils,gcc-core,gcc,g++,gcc4-core,gcc4-g++,gdb,pcre,pcre-devel,gawk,make ��12����
*     ���ԣ�a.����Cygwin������ cygcheck -c cygwin ������ӡ����ǰcygwin�İ汾������״̬;
*           b.gcc -v; g++ -v; make -v; gdb -v �Ȳ���
*   3.����NDK��������:<cygwin>\<user>\.bash_profile ������ NDK=/cygdrive/<ndk_path>; export NDK
*     �� ndk_build ��������Ŀ¼���� $PATH ����:export PATH=${PATH}:$NDK
*     ����:  ִ�� ndk-build ȷ��
*   4.���� Eclipse:
*     a.���� CDT ���;
*     b.������Ŀ��, Properties->Builders-> Newһ����������ѡ��Program -> 
*       ����ΪC_Builder,LocationΪ<cygwin>/bin/bash.exe,ArgumentsΪ --login -c "cd <projectpath> && $NDK/ndk-build";
*       �л���"Refresh"ѡ�����ѡ"Refresh resources upon completion";�л���"Build Options"��ѡ�������(����?);
*       "Specify Resources"->��ѡ��ĿĿ¼��Ϊ��ԴĿ¼��
*
* ����
*   0.Mac/Linux��������Ҫִ�� build/tools/make-standalone-toolchain.sh --platform=android-8 --install-dir=$HOME/my-android-toolchain
*   1.��ԭ��������� <project>/jni/ Ŀ¼��(��Ӧ��Java����һ���� src Ŀ¼��),
*     ��Java���õĺ�����������ѭ Java_����_����_������(JNIEnv* env, xxxx) �ĸ�ʽ
*   2.���� <project>/jni/Android.mk �����������Ϣ(����Ҫ�����C/C++Դ�ļ�),��ϸ���òμ� <ndk>/docs/ANDROID-MK.html
*     ע�⣺���ļ����ܻᱻ�������Σ���˲��ܼ������ֵ��ǰû�ж������
*     ģ���������� -- ֱ�Ӷ��壬ͨ�� $(������) �ķ�ʽ����
*       LOCAL_PATH:=$(call my-dir) -- ������ǵ�һ�С������Ҫ�����C/C++Դ�����λ�ã�"my-dir"�꺯����NDK�ṩ����ʾ��ǰĿ¼
*       LOCAL_MODULE:=MyModule1 -- ����ģ����,NDK����ʱ���Զ��� libǰ׺ �� .so��׺
*       LOCAL_SRC_FILES:=jniSrc.cpp -- ������Ҫ����� C/C++ �ļ��б����Զ�����������ϵ
*       LOCAL_CPP_EXTENSION := .cxx .cpp .cc -- ����ָ��CPP�ļ�����չ����Ĭ���� .cpp
*       LOCAL_CPP_FEATURES := rtti exceptions -- ָ��C++����
*       LOCAL_C_INCLUDES:= $(LOCAL_PATH)/Poco -- ָ��ͷ�ļ�������·��
*       LOCAL_CFLAGS|LOCAL_CPPFLAGS -- ָ������ѡ��
*       LOCAL_STATIC_LIBRARIES|LOCAL_SHARED_LIBRARIES -- �����ľ�̬��̬��
*       LOCAL_WHOLE_STATIC_LIBRARIES -- �μ�GNU linker �� --whole-archive��ͨ�����ڼ�����̬���ѭ������ʱ
*       LOCAL_LDLIBS -- ��������ӿ⣬�� -L����·��, -lz ��ʾ��Ҫ���ӵ� libz.so , -lstdc++ ��ʾ���ӵ���׼C++֧�ֿ�
*       LOCAL_LDFLAGS -- ��������ӱ�־���� -fuse-ld=gold ��ʾָ��ʹ�� ld.gold ������(Ĭ��)
*       LOCAL_ALLOW_UNDEFINED_SYMBOLS:=true -- ����ʱ����"undefined symbol"������Ȼ������ͨ����������������ʱ����
*       LOCAL_ARM_MODE:=thumb -- ָ��ARMĿ���ļ���ģʽ����ѡ thumb(ȱʡ,ָ��Ϊ16λģʽ),arm(,32λָ��ģʽ)
*       LOCAL_DISABLE_FORMAT_STRING_CHECKS:=true -- ��ֹNDK��������printfһ��ĺ����ж�"ʹ�÷ǳ����ĸ�ʽ���ַ���"����
*       LOCAL_EXPORT_CFLAGS:=-DEXPORT_XXX=1 -- ���嵼��ʱ(���������ʹ�ñ���)�����ӵ�ʹ�÷��� LOCAL_CFLAG �еı����־��
*         ���Ƶ��� LOCAL_EXPORT_CPPFLAGS|LOCAL_EXPORT_C_INCLUDES|LOCAL_EXPORT_LDFLAGS|LOCAL_EXPORT_LDLIBS ��
*       LOCAL_SHORT_COMMANDS:=true -- (������Windows,��Ϊ������ֻ�ܽ���8191���ַ�)��Ҫ��ϵͳ����ʱ�ڲ�ʹ�ý϶̵��������
*       MY_XXX:=xxxx -- �����Զ�������������� MY_ ��ͷ(Ҳ�����ù�˾��)��NDK������ LOCAL_, PRIVATE_, NKD_, APP_, Сд �ȵı�����
*     NDK�ṩ�ı���(ͨ�� include $(xxxx) �ķ�ʽ����)
*       $(CLEAR_VARS) -- ��ճ�LOCAL_PATH��������"LOCAL_"��ʼ�ı���(�� LOCAL_MODULE/LOCAL_SRC_FILES ��)����ֹ��η���ʱ����
*       $(BUILD_SHARED_LIBRARY) -- ��������ɹ���ⷽʽ�����Ƶ��� $(BUILD_STATIC_LIBRARY)��̬��, 
*         $(PREBUILT_SHARED_LIBRARY)Ԥ���빲���, $(PREBUILT_STATIC_LIBRARY)Ԥ���뾲̬��
*       $(TARGET_ARCH) -- CPU��ϵ�ܹ����� arm
*       $(TARGET_ARCH_ABI) -- Ŀ¼�� CPU+ABI,�� armeabi,armeabi-v7a,x86 ��
*     NDK�ṩ�ĺ����� -- ͨ�� $(call xxx) �ķ�ʽ����
*       $(call my-dir) -- ���ص�ǰ Android.mk �ļ���·��,ͨ�����ڶ��� LOCAL_PATH 
*       $(call all-subdir-makefiles) -- ���ص�ǰĿ¼�µ����� Android.mk �ļ���ÿ���ļ����Զ���һ��module����Ϣ
*       $(call this-makefile) -- ���ص�ǰMakefile��·�������Ƶ��� parent-makefile, grand-parent-makefile
*       $(call import-module, <name>) -- ����ģ�������Ҷ�Ӧ�� Android.mk 
*     ���ɣ�
*       a.��������(�μ� Makefile ): ifneq ($(MY_CONFIG_BAR),) MY_SOURCES += bar.c endif
*   3.���� <project>/jni/Application.mk��������Ӧ�ó�������Ҫ��ԭ��ģ�����Ϣ(�羲̬�����),
*     ��ϸ���òμ� <ndk>/docs/APPLICATION-MK.html
*     APP_ABI:=all -- ָ�������ABIĿ��(��CPU��ϵ�ܹ�)����������[armeabi] armeabi-v7a x86
*     APP_CFLAGS:= -- ����C����ʱ���ݵı�������ʶ
*     APP_CXXFLAGS|APP_CPPFLAGS:= -- ����C/C++����ʱ���ݵı�������ʶ
*     APP_MODULES:=MyModule1 MyModule2 -- ��ʾAndroid������Ҫ���õķ��й�ģ�飬��Ӧ�Ķ�̬���ļ�Ϊ libMyModule1.so
*     APP_OPTIM:=release|debug -- ������԰汾���Ƿ����汾��ȱʡ��release, �����App��android:debuggableΪtrue��
*     APP_PROJECT_PATH:=$(call my-dir)/project -- ָ��Android�������ڵ�Ŀ¼�������APK���Զ������ļ�������<app>\libs\armeabiĿ¼��
*   4.cd <project>, ʹ�� <ndk>/ndk-build �ű�����, ����ɹ�������� libs/libMyModule1.so ���ļ�
*   5.����ʹ��.so��App,�����ַ�ʽ��
*     a.ʹ�� JNI ����(ʹ�� native ��������ԭ��)���÷�ʽ�������� Android framework ������;
*       System.loadLibrary("MyModule1"); -- �ú������� static ��ʼ�������е��ü���ԭ��ģ��
*     b.�� NativeActivity �̳�(Android 2.3 �Ժ�֧��)
*     ע��:App��Manifest����Ҫͨ�� <uses-sdk android:minSdkVersion="xx" /> ����ʹ����native library
*   5.������App�����Զ�����ӦĿ¼�µ�.so�����.apk�ļ���,��װ�� /data/data/com.xxx/lib/ Ŀ¼��
*
****************************************************************************************************************************************/

/****************************************************************************************************************************************
* Prebuilt -- Ԥ����⣬������ʹ�ÿ��Ԥ����汾
*   ����:
*     1.�������NDK������Ա������Ĺ����������ṩԴ��
*     2.ʹ����ǰ�����Ԥ����������ٱ������
*   ʹ�÷�ʽ:
*     1.include $(PREBUILT_SHARED_LIBRARY) �� $(PREBUILT_STATIC_LIBRARY)
*     2.LOCAL_SRC_FILES:=libfoo.so -- ָ��Ϊ��Ҫ�ṩ�Ĺ�����·��
****************************************************************************************************************************************/

/****************************************************************************************************************************************
* ndk-build ����
*   ע�⣺windows�±������cygwin���ٱ����clean����ʧ��("���Ŀ��ƥ��"), ��ʱ��Ҫ�ֶ�ɾ�� libs/obj �ȱ�����Ŀ¼���ٴα���
*   clean -- ����
*   NDK_DEBUG=1 -- ���ɴ�������Ϣ�ı��룬�ڴ����л�����"xxx"��
*   -B V=1 -- rebuild ������ʾ��ϸ�ı�������
****************************************************************************************************************************************/

/****************************************************************************************************************************************
* NDKʹ�õ�JNI�﷨
*   ��־
*   #define LOGI(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__
****************************************************************************************************************************************/

/****************************************************************************************************************************************
* Զ��gdb����(ndk-gdb),2.2�Ժ��֧��
*   1.AndroidManifest.xml ������ android:debuggable Ϊ"true";
*   2.���벢��ģ���������г����Ժ��� Ӧ�ó���Ŀ¼���� ndk-gdb ;
*   3.ʹ�� gdb ������е���
****************************************************************************************************************************************/
public class NdkTester extends ActivityTestCase {

}
