package com.fishjam.android.study;
import java.io.ByteArrayInputStream;
import java.security.cert.CertificateFactory;
import java.security.cert.X509Certificate;

import android.content.ComponentName;
import android.content.pm.PackageManager;
import android.test.AndroidTestCase;

/**************************************************************************************************************************************
 * �����ܵ�Blogϵ��(�ܶ�ӽ��ܵ�����) -- http://www.eoeandroid.com/thread-314366-1-1.html
 * 
 * Google���ܷ�ʽ
 *    Google��Android APP�İ�ȫ�����Ǹ�APK�򵥼��ܡ�java��Դ��ӿǱ������Ժ���so�⡢��Դ�ļ��������ļ���
 *    �������ܰ�ȴû���κα������������ı���Ҳ���ѱ��ڿ͹��Ƶ���û�����κ�����ά������Android APP����û���κΰ�ȫ�ԡ�
 * ������ ��� -- ��Դ��ӿǱ�����so����Ĵ���ӿǱ�������Դ�ļ�ǩ��������APK�����δ�������ȷ�������APK����ȫ���汣��
 * �ڴ��޸��� -- �� ��������, ͨ���޸��������е���Ϸ���ڴ����ݣ��ɷ�����޸���Ϸ��HP��MP���������ȵȲ���.
 *                    �����������ݹ���������Ӧ�õ���Ҫ���ݲ����޸�
 * 
 * 
 * Apk������(https://code.google.com/p/innlab/downloads/list)
 *   1.�� .apk �ļ���׺��Ϊ .zip ����ѹ����ѹ���Ŀ¼�ṹ
 *     assets\ -- ���xml�����ļ���ͼƬ�ļ���һ�㾭�������޷�ֱ�Ӳ鿴
 *     META-INF\ -- ���ǩ�����ļ���
 *     res\
 *     classes.dex -- Դ��ļ����ļ�
 *     resources.arsc --
 *   2.ʹ�� dex2jar ( http://code.google.com/p/dex2jar/ ) �� classes.dex �ļ�תΪ .jar �ļ�
 *      dex2jar.bat classes.dex  ������� classes_dex2jar.jar
 *   3.�� jar ��ѹ ��ʹ�� jd-gui �鿴�ļ� 
 * 
 * ApkTool(https://code.google.com/p/android-apktool/) -- ���������ɳ����Դ�����ͼƬ��XML���á�������Դ
 *   �����룺 apktool.bat d -f <apk�ļ�> [����ļ���] -- 
 *                ����������ڲ��� .smali �ļ����� davlik Դ��? 
 *   ���´��:  apktool.bat  b <�ļ���>  -- ͨ���������޸ĺ����´����
 * 
 * APK��֮��(ApkIde) -- http://pan.baidu.com/share/link?shareid=164149&uk=3291471913#dir/path=%2Fmysoft%2Fapkide
 *   
**************************************************************************************************************************************/

/**************************************************************************************************************************************
 * ǩ��
 *   
 * ��APK�ļ�����ǩ��(Jarsigner.exe)
 *   �磺Jarsigner.exe -keystore [ǩ���ļ�·��, ��fishjam.keystore] -storepass [����] -verbose [��Ҫǩ���ļ�·��, �� MyApp.apk] [ǩ���ļ�����] 
 *   ����˵���� 
 *      -keystore <url> -- ��Կ��λ��
 *      -storepass <����> -- ������Կ�������ԵĿ���
 *      -storetype <����> -- ��Կ������
 *      -keypass <����> -- ר����Կ�Ŀ�������ͬ��
 *      -signedjar <�ļ�> --  ��ǩ���� JAR �ļ������� 
 *      -tsa <url> -- ʱ���������λ��
 *      -tsacert <����> -- ʱ��������Ĺ�����Կ֤��
 *      -altsigner <��> -- �����ǩ�����Ƶ�����
 *      -providerName <����> -- �ṩ������
 *      -providerClass <��> -- ���ܷ����ṩ�ߵ�����
 *      
 * ǩ������ -- ��������ʱ��ȡ��ǰӦ�õ�ǩ�������бȶ��жϣ��粻һ������ֹ���򣬷�ֹ���δ����
 *   APK��Ψһʶ��������������ǩ�����������ġ�
 *   
 * �������ֲ�����׹��(���� SDK)
 *   1.����SDK�� �� http://static.youmi.net/files/sdk/YoumiSdk_v4.04_2013-07-12.zip
 *   2.�� AndroidManifest.xml �м��� SYSTEM_ALERT_WINDOW �� GET_TASKS ��Ȩ��
 *   3.�� AndroidManifest.xml �м��� ��Ӧ�Ĵ���(�ο�SDK���� Activity, Service, Receiver �� )
 *   
 * Դ����� -- �������е����б�����������������Ƽ���Ϊ��̵�Ӣ����ĸ���ţ���APP���ƽ�������ƽ��߶Դ�����Ķ��Ѷ�
 * 
 * α����(Android 4.2.x ֮ǰ) -- ͨ��java�����APK(ѹ���ļ�)����α���ܣ����޸�ԭ�����޸�����4λ�ֽڱ��Ϊ"P K 01 02"�ĺ��5λ�ֽڣ�������ʾ������ż����ʾ���ܡ�
 *   α���ܺ��APK�������Է�ֹPC�˶����Ľ�ѹ�Ͳ鿴��Ҳͬ���ܷ�ֹ�����빤�߱��롣
 *   ����α���ܶ���APK���ܺ��г�Ҳ�޷�������а�ȫ��⣬�����г���ܾ�����APK�ϴ��г�(Android 4.2.x���ϵͳ�޷���װ)
 *   
**************************************************************************************************************************************/

public class SecutityTester extends AndroidTestCase {
	public void testRemoteHideOrShowAppIcon(){
		//Զ�̿���Ӧ��ͼ�����ʾ������
		
		/*
		private void setComponentEnabled(Class<?> clazz, boolean enabled) {
			final ComponentName c = new ComponentName(this, clazz.getName());
			getPackageManager().setComponentEnabledSetting(c, enabled? PackageManager.COMPONENT_ENABLED_STATE_ENABLED:PackageManager.COMPONENT_ENABLED_STATE_DISABLED,PackageManager.DONT_KILL_APP)
		}
		*/
	}
	
	public void testSignProtect(){
		//��ȡ��ǰ�����ǩ��
		/*
		PackageInfo packageInfo = getPackageManager().getPackageInfo(getPackageName(), PackageManager.GET_SIGNATURES);
		Signature[] signs = packageInfo.signatures;
		Signature sign = signs[0];
		CertificateFactory certFactory = CertificateFactory.getInstance("X.509");
		X509Certificate cert = (X509Certificate)certFactory.generateCertificate(new ByteArrayInputStream(sign.toByteArray()));
		byte[] buffer =cert.getEncoded();  //���APKǩ����MD5ֵ��ͨ���ȶԸ�ֵ����ȷǩ����MD5ֵ���жԱȣ�����ʶ���Ƿ񱻵���
		*/
		//��Ӧ�� smali��������(��ͨ�� ApkIde �������޸ĵ����߼��������ƽ��ǩ������ )��  
		//  Landroid/content/pm/PackageInfo;->signatures:[Landroid/content/pm/Signature��
		
	}
}