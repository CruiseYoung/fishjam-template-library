package com.fishjam.android.study;
import android.test.AndroidTestCase;


/***************************************************************************************************************************************
 * ѧϰ�鼮
 *     ���Android���� -- P44 (AndroidManifest.xml)
 *     AndroidӦ�ÿ������.pdf -- 311, 
 *     ��ӭ����Android���� http://www.docin.com/p-31448231.html
 *     Android�п��ӻ�GUI����������Դ���߳���  http://code.google.com/p/droiddraw , ����ͨ�� http://www.droiddraw.org/ ����ʹ��
 *     Google Android SDK����������ȫ  -- http://book.csdn.net/bookfiles/1060/100106031790.shtml
 * 
 * Android��Google�����Ļ���Linuxƽ̨�Ŀ�Դ�ֻ�����ϵͳ��
 *   Android Market(http://www.android.com/market/) -- �û���������ػ���Ӧ�������֧�ּ��Ϸ�����ϵͳ�Զ������û��ֻ�ѡ����ʵ�apk��
 *   Google�Ļ���Linuxƽ̨�Ŀ�Դ�ֻ�����ϵͳ,��Google�뿪���ֻ�����(�����й��ƶ���Ħ��������)�������������Ĳ������:
 *     1.Ӧ�����(Java) -- ����һЩϵͳ�ĺ���Ӧ�ó����� SMS������������ͼ����ϵ�˵ȡ�
 *     2.Ӧ�ó����� -- �û�����(Views),���ݹ�����(Content Providers), ��Դ������(Resource Manager),֪ͨ������(Notification Manager),�������(Activity Manager).
 *        Ӧ�ó�����֧��������������滻������滻ϵͳ�еĴ�绰Ӧ�ó����ļ���������
 *     3.�м������������? -- C/C++�⼯(��BSDϵͳ��������libcϵͳ��)��ͨ��Ӧ�ó�����Ϊ�������ṩ����
 *     4.����ϵͳ(Linux) -- ����ϵͳ����������Linux�ں�(2.6)���簲ȫ�ԡ��ڴ�������̹�������Э��ջ������ģ�͵�
 *   ����WebKit��������棬�߱����������߼�ͼ����ʾ���������ܣ���iPhone�������ֻ���ǿ���������ܣ������ǿ�󣬿���˵��һ������ȫ��WebӦ�õĵ�һƽ̨��
 *   �����ֻ�����OHA (Open Handset Alliance)
 *   ����Androidƽ̨��SDK(Software Development Kit) -- m3-rc20a
 *   Android��Ŀ���Ƿ�չ��Ϊ�ƶ������������ն��豸�����ƽ̨���ͺñ�Windows��PC������ĵ�λ��
 * 
 * ÿ��AndroidӦ�ó��������ڵ�����Dalvik������ڣ����Է����ʵ�ֶ�Ӧ�ó���ĸ��롣
 *   ע�⣺Dalvik��δ��ȫ����JVM�淶������Ҳ�����ݡ�
 *        JVM���е���Java�ֽ��룬JVMֱ�Ӵ�.class�ļ���JAR���м����ֽ���Ȼ�����У�
 *        Dalvik���е�����ר�е�dex(Dalvik Executable)�ļ�����Ҫͨ��DX���߽�Ӧ�ó��������.class�ļ������.dex�ļ�������(���ܸ��ߡ��ļ���С�����ڼĴ���ʵ��)
 * 
 * TODO: 
 *   1.SDK �汾 �� Google API �汾��������ʲô����
 *   2.ƽ̨�汾 �� API Level �� ���API Level �Ĵ��룺 Integer apiLevel = Integer.parseInt(VERSION.SDK); 
 *      ƽ̨�汾				API Level				����
 *     Android 4.4.2            19
 *     Android ???				18
 *     Android 4.2				17
 *     Android 4.1				16
 *     Android 4.0.3            15
 *     Android 4.0				14
 *     Android 3.2				13
 *     Android 3.1				12
 *     Android 3.0				11
 *     Android 2.3.3            10
 *     Android 2.3				9
 *     Android 2.2				8
 *     Android 2.1				7
 *     Android 2.0.1            6
 *     Android 2.0              5
 *     Android 1.6              4
 *     Android 1.5				3
 *     Android 1.1				2
 *     Android 1.0				1
 **************************************************************************************************************************************/

/**************************************************************************************************************************************
 * TODO: 
 *   1.����ֱ������ adt-bundle-windows-x86-20131030 �ȵ�ѹ������ѹ����(��װ��ADT����� Eclipse + SDK)?
 *     Ȼ�� Android SDK Ŀ¼�µ�tools��platform-tools ����Ŀ¼����PATH��������(�򴴽����û���������������) 
 *      http://code.google.com/android/ �� http://developer.android.com/
 *   2.�������� ANDROID_SDK_HOME ��������������SDK���� AVD ��������Ϣ�ĵط�������Ĭ�ϻᱣ����  %HOMEPATH%\.android ·����?
 *   
 * ����� ����Ҫ Apache Ant 1.7 ���� ��) -- JDK + Eclipse + Android SDK + ADT
 *   1.���ذ�װJDK -- jdk-6u16-windows-i586.exe/jdk-7u45-windows-i586.exe, 
 *     ����Java��������(%JAVA_HOME%, %CLASSPATH%ֵΪ  "%JAVA_HOME%\lib\tools.jar;%JAVA_HOME%\lib\dt.jar;.", ����path)
 *   2.��ѹAnt, ���� ANT_HOME ������������ %ANT_HOME%\bin ·������ PATH��(���û�������?)
 *   3.���ؽ�ѹEclipse -- http://www.eclipse.org/downloads/, һ��������� Eclipse for Java EE Developers
 *   4.��ѹAndroid SDK(91M) -- http://developer.android.com/sdk/index.html��1.6�Ժ�ֻ�ṩ�����߰�װ
 *      ������Android SDK�Ļ�������(�� ANDROID_SDK ������ tools ��Ŀ¼����path)��Ŀ¼����ò�Ҫ���ո�
 *   5.���ػ�ͨ�����簲װ Android for Eclipse �Ĳ��(ADT -- Android Development Toolkit)��
 *     http://dl-ssl.google.com/android/eclipse/ (��ʹ�� https:// ?).
 *      ���� Android Development Tools �� Android Editors.
 *      ADT����: ������Զ����ɡ����ԡ����롢�������קʽ�������ɵ�
 *   6.����Eclipse��Window > Preferences > Android > SDK  -- ָ��SDK�ĸ�Ŀ¼��
 *   7.File -> New -> Project... -> Android -> Android Project �������̡�
 *   
 *   TODO:
 *     1.��Ҫ��װ Tomcat(ʹ�ý�ѹ���İ�װ��ʽ),��װ��Ϻ� jsp-api.jar,servlet-api.jar �����ļ���ӵ� %CLASSPATH% ������;
 *       WebӦ�ø��Ƶ� %TOMCAT_HOME%\webapps Ŀ¼�£�Ȼ����� build.xml ����·����ִ�� ant compile ������Ӧ��
 *     
 *   1.Eclipse 
 *     �������壺 Window-> Prefrences -> General -> Appearance -> Colors and Fonts -> Text Font, 
 *                     Ȼ��"Use System Font"�����Զ�ʹ�� ΢���ź� 
 * 
 * �����Ѵ��ڵĹ���
 *   ����1(��ĿĿ¼����workspaceĿ¼��): Import -> Existing Projects into Workspace
 *   ����2. �½�Android���� -> Create project form existing source
 *
 * ���� -- Android ��Ŀ�԰�����ΪΨһ��ʶ����ͬһ�ֻ��ϰ�װ����������ͬ��Ӧ�ã���װ�Ŀ��Ը���ǰ�氲װ�ġ�
 *   1.ͨ��DX���߶�*.class�ļ�����ת���ʹ�����õ�.dex�ļ�;
 *   2.ͨ��AAPT���ߴ��������Դ�ļ����õ� .ap_ �ļ�
 *   3.ͨ�� apkbuilder ���߰� .dex �� .ap_ �ļ������ APK��
 *   4.ǩ��(ȷ����������ݺ�ȷ��Ӧ�õ�������) 
 *      ���������Խ׶� -- ADT������Զ����ɵ���֤���AndroidӦ��ǩ��(������ϻ���ô����)
 *      ��ʵ����ʱ -- ����ʹ�ú��ʵ�����֤������Ӧ�ó���ǩ����
 *      ������
 *        a.�Ҽ���Ŀ -> Android Tools -> Export Signed Application Package
 *        b.ʹ�� keytool + jarsigner.exe + zipalign.exe ���ߣ����õ����� ǩ�� + �Ż� ��APK��װ�������ɶ��ⷢ���� 
**************************************************************************************************************************************/

/**************************************************************************************************************************************
 * SDK��Ŀ¼�ṹ
 *   add-ons -- ��������˾ΪAndroidƽ̨�����ĸ��ӹ���ϵͳ����ʼʱΪ�ա��ɼ��� ???
 *   extras -- ���Google�ṩ��USB������Intel�ṩ��Ӳ�����ٵȸ��ӹ��߰� 
 *   platforms -- ��Ų�ͬ�汾��Androidϵͳ,ͨ�� Android SDK Manager �������ذ�װ(�� android-7, android-19 ��)
 *   platform-tools -- �����Androidƽ̨��ع���
 *   
 * ���̵�Ŀ¼�ṹ
 *   assets -- ԭ�����ļ�(��MP3��)��Android������ֱ�ӷ��ʣ�����ͨ�� AssetManager���Զ����Ʒ�ʽ����ȡ
 *   src -- ����Դ��
 *   res -- ��ͨ�� R.java �е���Դ��ֱ�ӷ��ʣ������뵽Ӧ�ó����еĸ�����Դ��aapt�����Զ�ɨ��������Դ�嵥�ļ�(R.java)
 *     |-anim : ���涯��
 *     |-drawable : ����ͼƬ����Ҫ��Ϊ��BitmapFile, ColorDrawable(��ɫ), Nine-Patch Image(��ƬͼƬ)
 *     |---drawable-ldpi,drawable-mdpi,drawable-hdpi,drawable-xhdpi �ֱ��ŵͷֱ��ʡ��зֱ��ʡ��߷ֱ��ʡ����߷ֱ��ʵ�4��ͼƬ�ļ�
 *     |-layout :���沼���ļ�(xml��ʽ)
 *     |-menu   :xml�˵��ļ�, <menu> -> <group> -> <item> ��<group>��ʾһ���˵��飬����һ������������(��visible,checkableBehavior ��)
 *     |-raw    :ֱ�Ӹ��Ƶ��豸�е�ԭ���ļ�
 *     |-values :����һЩ����ֵ���� strings.xml(�ַ�������), arrays.xml(����), colors.xml(��ɫ), dimens.xml(�ߴ�, �����ֳߴ硢�߿��), styles.xml(��ʽ)
 *     |-xml    :xml�ļ�
**************************************************************************************************************************************/

/**************************************************************************************************************************************
 * AndroidӦ�ó��������¼������ͣ�
 *   1.ǰ��Activity��Foreground Activities����
 *   2.��̨����Background Services�� -- ϵͳ����System Service��
 *      Service -- ���������ڣ�û���û�����ĳ��������ڴӲ����б��в��Ÿ����ĺ�̨ý�岥������ 
 *      Context.startService ����һ��Service; Context.bindService ���ӵ�һ��Service��(�绹û������������)
 *      ϵͳBroadcast���㲥��Ϣ����Receiver���㲥��Ϣ�������� -- ������͵Ĺ㲥�ͽ�����
 *   3.���ִ��Activity��Intermittent Activities��-- ����Notification Manager�ȵ�
 *    
**************************************************************************************************************************************/

/**************************************************************************************************************************************
 * ��������
 *    1.��ֱ����豸����������������
 *       ������Ӧ�ֱ��ʵ���Դ�ļ��к�ҳ�沼���ļ��У��ڱ�������ʱ����ݵ�ǰ�豸�ֱ��ʣ��Զ�ѡ����Ӧ����Դ�ļ���ҳ�沼���ļ���
 *       
 * NDK���� ?
 *   ��ר�õ� NDK Plugins,��Ҫ���� C/C++�������
**************************************************************************************************************************************/

public class AndroidStudy  extends AndroidTestCase{
	public void testAndriod()
	{
	}
}
