package com.fishjam.android.study;
import junit.framework.TestCase;

/**************************************************************************************************************************************
 * AndroidManifest.xml -- �������AndroidӦ�ó��������ЩActivity��Service����Receiver��Ҳ��Ȩ�����á���������������������������ʹ�á�
 * <application>
 *   <activity android:name=".MainActivity" android:label="@string/app_name> -- ����Activity
 *   <provider android:name="EmployeeProvider" android:authorities="com.fishjam.android.study.Employees" /> -- ���� ContentProvider
 *   <receiver android:name="MyReceiver"> -- ���� BroadcastReceiver
 *   <service android:name="MyService"> -- ����Service
 *     <intent-filter> -- ָ������������actionָ���������?; category ָ�� LAUNCHER(���س���ʱ����),  ��
 *       <action>
 *       <category>
 *       <data>
 * <uses-sdk> -- ָ��SDk�İ汾��Ϣ���� minSdkVersion  
 * 
 * 
 *  
**************************************************************************************************************************************/

public class AndroidManifest  extends TestCase{

}
