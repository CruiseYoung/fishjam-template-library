package com.fishjam.android.study;
import junit.framework.TestCase;

/***************************************************************************************************************************************
* GoogleMap
*   ʹ�ò���:
*     1.������Ŀʱ Build Target Ҫѡ��"GoogleAPIs", ����� map.jar
*     2.ѡ��"AVD"ʱҪʹ"Target"Ϊ"GoogleAPIs", 
*     3.AndroidManifest.xml ��
*       ��� <uses-library android:name="com.google.android.maps" />
*       ����Ȩ��: <uses-permission android:name="android.permission.INTERNET" />
*          �Ƿ���Ҫ���? <uses-permission android:name="android.permission.ACCESS_MOCK_LOCATION" />
*     4.��� "Google Map API key"��ʹ�� JDK��keytool����MD5 key����google��վ���� ApiKey
*       http://code.google.com/intl/zh-cn/android/maps-api-signup.html
*     5.�����ļ� <com.google.android.maps.MapView android:apiKey="xxxxxxx" />
*     5.�̳� MapActivity, ����ʹ�� MapView ������ʾ�Ϳ���
*      
 **************************************************************************************************************************************/

public class GoogleAPITester  extends TestCase{

}
