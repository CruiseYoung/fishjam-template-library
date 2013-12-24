package com.fishjam.android.study;
import junit.framework.TestCase;

/***************************************************************************************************************************************
 * ѧϰ�鼮
 *     AndroidӦ�ÿ������.pdf -- P100, 
 *     ��ӭ����Android���� http://www.docin.com/p-31448231.html
 *     Android�п��ӻ�GUI����������Դ���߳���  http://code.google.com/p/droiddraw , ����ͨ�� http://www.droiddraw.org/ ����ʹ��
 *     Google Android SDK����������ȫ  -- http://book.csdn.net/bookfiles/1060/100106031790.shtml
 * 
 * Android��Google�����Ļ���Linuxƽ̨�Ŀ�Դ�ֻ�����ϵͳ��
 *   Android Market(http://www.android.com/market/) -- �û���������ػ���Ӧ�������
 *   Google�Ļ���Linuxƽ̨�Ŀ�Դ�ֻ�����ϵͳ,��Google�뿪���ֻ�����(�����й��ƶ���Ħ��������)�������������Ĳ������:
 *     1.Ӧ�����(Java)
 *     2.Ӧ�ó����� -- �û�����(Views),���ݹ�����(Content Providers), ��Դ������(Resource Manager),֪ͨ������(Notification Manager),�������(Activity Manager).
 *        Ӧ�ó�����֧��������������滻������滻ϵͳ�еĴ�绰Ӧ�ó����ļ���������
 *     3.�м��(��Щ? ָAndroid�����? -- ) C/C++�⼯��ͨ��Ӧ�ó�����Ϊ�������ṩ����
 *     4.����ϵͳ(Linux) -- ����ϵͳ����������Linux�ں�(2.6)���簲ȫ�ԡ��ڴ�������̹�������Э��ջ������ģ�͵�
 *   ����WebKit��������棬�߱����������߼�ͼ����ʾ���������ܣ���iPhone�������ֻ���ǿ���������ܣ������ǿ�󣬿���˵��һ������ȫ��WebӦ�õĵ�һƽ̨��
 *   �����ֻ�����OHA (Open Handset Alliance)
 *   ����Androidƽ̨��SDK(Software Development Kit) -- m3-rc20a
 *   Android��Ŀ���Ƿ�չ��Ϊ�ƶ������������ն��豸�����ƽ̨���ͺñ�Windows��PC������ĵ�λ��
 * 
 * TODO: 
 *   1.SDK �汾 �� Google API �汾��������ʲô����
 * 
 **************************************************************************************************************************************/

/**************************************************************************************************************************************
 * ����� ����Ҫ Apache Ant 1.7 ���� ��) -- JDK + Eclipse + Android SDK + ADT
 *   TODO: ����ֱ������ adt-bundle-windows-x86-20131030 �ȵ�ѹ������ѹ����? 
 *      http://code.google.com/android/ �� http://developer.android.com/
 *   1.���ذ�װJDK -- jdk-6u16-windows-i586.exe, ����Java��������(%JAVA_HOME%, %CLASSPATH%ֵΪ".", ����path)
 *   2.��ѹAnt, ��Ant·������path��(���û�������?)
 *   3.���ؽ�ѹEclipse -- http://www.eclipse.org/downloads/
 *   4.��ѹAndroid SDK(91M) -- http://developer.android.com/sdk/index.html��1.6�Ժ�ֻ�ṩ�����߰�װ
 *      ������Android SDK�Ļ�������(�� ANDROID_SDK ������ tools ��Ŀ¼����path)��Ŀ¼����ò�Ҫ���ո�
 *   5.���ػ�ͨ�����簲װ Android for Eclipse �Ĳ��(ADT -- Android Development Toolkit)��http://dl-ssl.google.com/android/eclipse/ (��ʹ�� https:// ?).
 *      ���� Android Development Tools �� Android Editors.
 *      ADT����: ������Զ����ɡ����ԡ����롢�������קʽ�������ɵ�
 *   6.����Eclipse��Window > Preferences > Android > SDK  -- ָ��SDK�ĸ�Ŀ¼��
 *   7.File -> New -> Project... -> Android -> Android Project �������̡�
 *                  
 *   1.Eclipse 
 *     �������壺 Window-> Prefrences -> General -> Appearance -> Colors and Fonts -> Text Font, 
 *                     Ȼ��"Use System Font"�����Զ�ʹ�� ΢���ź� 
**************************************************************************************************************************************/

/**************************************************************************************************************************************
 * ���̵�Ŀ¼�ṹ
 *   assets -- ԭ�����ļ�(��MP3��)��Android������ֱ�ӷ��ʣ�����ͨ�� AssetManager���Զ����Ʒ�ʽ����ȡ
 *   src -- ����Դ��
 *   res -- ��ͨ�� R.java �е���Դ��ֱ�ӷ��ʣ������뵽Ӧ�ó�����
 *     |-anim : ���涯��
 *     |-drawable : ����ͼƬ����Ҫ��Ϊ��BitmapFile, ColorDrawable(��ɫ), Nine-Patch Image(��ƬͼƬ)
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
**************************************************************************************************************************************/

public class AndroidStudy  extends TestCase{

}
