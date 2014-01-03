package com.fishjam.android.study;
import junit.framework.TestCase;

/***************************************************************************************************************************************
* GoogleMap
*   ʹ�ò���:
*     1.������Ŀʱ Build Target Ҫѡ��"GoogleAPIs", ����� maps.jar
*     2.ѡ��"AVD"ʱҪʹ"Target"Ϊ"GoogleAPIs", 
*     3.AndroidManifest.xml ��
*       ��� <uses-library android:name="com.google.android.maps" />
*       ����Ȩ��: <uses-permission android:name="android.permission.INTERNET" />
*          �Ƿ���Ҫ���? <uses-permission android:name="android.permission.ACCESS_MOCK_LOCATION" />
*     4.��� "Google Map API key"��ʹ�� JDK��keytool����MD5 key����google��վ���� ApiKey
*       http://code.google.com/intl/zh-cn/android/maps-api-signup.html
*     5.�����ļ� <com.google.android.maps.MapView android:apiKey="xxxxxxx" />
*     6.�̳� MapActivity, ����ʹ�� MapView ������ʾ�Ϳ���
*   
*   Ҫ��ʾ��ͼʱ��Ҳ��ʹ�� ACTION_VIEW + Uri="geo������,ά��" ��Intent��ʽ
*
*   GPS(Global Position System) -- 24�����������1.2W����ĸ߿գ���12Сʱ�����ڻ��Ƶ������С�
*     ����ʱ�̣��ڵ����ϵ�����һ�㶼���ٿ���ͬʱ�۲⵽4�����ϵ����ǡ��������ǵ����ջ��ľ������۲���λ�á�
*     LocationManager -- �ṩ��ϵͳλ�÷��ʵķ������綨λ������(requestLocationUpdates)�������澯(addProximityAlert)��.
*       locationManager = (LocationManager)Context.getSystemService(Context.LOCATION_SERVICE);
*       getLastKnownLocation() -- ����Provider���λ����Ϣ
*     LocationProvider -- ������λ�÷�����ṩ����(�� GPS�豸�������ṩ ),����������ԣ���ͨ�� Criterial ������������
*       
*     LocationListener -- λ�ñ仯�ص��ӿڣ����Է��� onLocationChanged��onProviderDisabled��onStatusChanged �Ȼص�����
*     Location -- λ����Ϣ
*       getLatitue -- ��þ���
*       getLongitude -- ���ά��
*   
*   �ʼ�
*     1.���� Gmail �����շ��ʼ�
*     2.ʹ�� SMTP ���շ�
 **************************************************************************************************************************************/

public class GoogleAPITester  extends TestCase{

}
