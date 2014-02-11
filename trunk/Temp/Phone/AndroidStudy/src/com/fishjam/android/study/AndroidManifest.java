package com.fishjam.android.study;
import android.Manifest;
import android.test.AndroidTestCase;
import junit.framework.Assert;

/**************************************************************************************************************************************
 * AndroidManifest.xml -- ϵͳ�嵥�ļ�������Ӧ�õ����ơ�ͼ�ꡢ����Ȩ�޵����ԣ�Ҳ������������������øó��������Ȩ�������� 
 *   Ҳ�������AndroidӦ�ó��������ЩActivity��Service��Provider��Receiver �ȣ���������������������������ʹ�á�
 *   
 *   versionCode="1" -- 
 *   versionName="1.0" -- 
 * <application>
 *   <activity android:name=".MainActivity" android:label="@string/app_name> -- ����Activity
 *   <provider android:name="EmployeeProvider" android:authorities="com.fishjam.android.study.Employees" /> -- ���� ContentProvider
 *   <receiver android:name="MyReceiver"> -- ���� BroadcastReceiver
 *   <service android:name="MyService"> -- ����Service
 *     <intent-filter> -- ָ������������actionָ���������?; category ָ�� LAUNCHER(���س���ʱ����),  ��
 *       <action android:name="android.intent.action.MAIN" /> -- MAINָ����Activity�ǳ������ڣ�
 *       <category android:name="android.intent.category.LAUNCHER" /> -- LAUNCHERָ�����ظ�Ӧ��ʱ���и�Activity(����Activity)
 *       <data>
 *     </intent-filter>
 *     <uses-permission xxx/> -- �������ø�Ӧ�������Ȩ�ޣ�д��Ӧ��(�� Activity��Service��)���ڲ�
 * </application>
 * <uses-sdk> -- ָ��SDk�İ汾��Ϣ
 *   minSdkVersion -- ����֧�ֵ���Ͱ汾�ȼ�
 *   targetSdkVersion -- ? ָ���ð汾������ƽ̨���ò���Ҫ�ļ����Դ��������ʹ�µĹ��������ھɰ汾�ĳ����� 
 * <uses-permission android:name= "android.permission.xxxx"/> -- ��������Ҫ��Ȩ��, �� READ_CONTACTS(��ȡ��ϵ��)
 * 
 *  
**************************************************************************************************************************************/

/**************************************************************************************************************************************
 * Ȩ���б�( uses-permission android:name="android.permission.xxxx" )
 *   ACCESS_CHECKIN_PROPERTIES
 *   ACCESS_COARSE_LOCATION
 *   ACCESS_FINE_LOCATION
 *   ACCESS_LOCATION_EXTRA_COMMANDS
 *   ACCESS_MOCK_LOCATION
 *   ACCESS_NETWORK_STATE -- ��ȡ����״̬��Ϣ
 *   ACCESS_SURFACE_FLINGER
 *   ACCESS_WIFI_STATE -- ��ȡWi-Fi����״̬��Ϣ
 *   ACCOUNT_MANAGER
 *   AUTHENTICATE_ACCOUNTS
 *   BATTERY_STATS	-- ��ȡ���״̬��Ϣ
 *   BIND_APPWIDGET
 *   BIND_INPUT_METHOD
 *   BLUETOOTH		-- ����ƥ��������豸
 *   BLUETOOTH_ADMIN	-- ����(���֡�ɾ��?)ƥ��������豸
 *   BRICK
 *   BROADCAST_PACKAGE_REMOVED
 *   BROADCAST_SMS	-- �㲥�յ���������
 *   BROADCAST_STICKY
 *   BROADCAST_WAP_PUSH
 *   CALL_PHONE		-- ����绰
 *   CALL_PRIVILEGED
 *   CAMERA				-- ʹ�������
 *   CHANGE_COMPONENT_ENABLED_STATE
 *   CHANGE_CONFIGURATION
 *   CHANGE_NETWORK_STATE	-- �ı���������״̬
 *   CHANGE_WIFI_MULTICAST_STATE	-- �ı�Wi-Fi��������״̬
 *   CHANGE_WIFI_STATE
 *   CLEAR_APP_CACHE
 *   CLEAR_APP_USER_DATA
 *   CONTROL_LOCATION_UPDATES
 *   DELETE_CACHE_FILES		-- ɾ�������ļ�
 *   DELETE_PACKAGES			-- ɾ����װ��
 *   DEVICE_POWER
 *   DIAGNOSTIC
 *   DISABLE_KEYGUARD
 *   DUMP
 *   EXPAND_STATUS_BAR
 *   FACTORY_TEST
 *   FLASHLIGHT			-- ���������
 *   FORCE_BACK
 *   GET_ACCOUNTS
 *   GET_PACKAGE_SIZE
 *   GET_TASKS
 *   GLOBAL_SEARCH
 *   HARDWARE_TEST
 *   INJECT_EVENTS
 *   INSTALL_LOCATION_PROVIDER
 *   INSTALL_PACKAGES
 *   INTERNAL_SYSTEM_WINDOW
 *   MANAGE_ACCOUNTS
 *   INTERNET		-- ������Socket(����������?)
 *   MANAGE_APP_TOKENS
 *   MASTER_CLEAR
 *   MODIFY_AUDIO_SETTINGS	-- �޸�ȫ����������
 *   MODIFY_PHONE_STATE
 *   MOUNT_FORMAT_FILESYSTEMS
 *   MOUNT_UNMOUNT_FILESYSTEMS
 *   PERSISTENT_ACTIVITY
 *   PROCESS_OUTGOING_CALLS	-- ���������ơ�ȡ�������绰
 *   READ_CALENDAR
 *   READ_CONTACTS	-- ��ȡ�û�����ϵ������
 *   READ_FRAME_BUFFER
 *   READ_HISTORY_BOOKMARKS	-- ��ȡ��ʷ��ǩ
 *   READ_INPUT_STATE
 *   READ_LOGS
 *   READ_OWNER_DATA	-- ��ȡ�û�����(����ָ��Щ?)
 *   READ_PHONE_STATE	-- ��ȡ�绰״̬
 *   READ_PHONE_SMS(�� 4.4����û���ҵ����Ѿ�û����?) -- ��ȡ����
 *   READ_SYNC_SETTINGS
 *   READ_SYNC_STATS
 *   REBOOT	-- ����ϵͳ
 *   RECEIVE_BOOT_COMPLETED
 *   RECEIVE_MMS	-- ���ա���ء��������
 *   RECEIVE_SMS	-- ���ա���ء��������
 *   RECEIVE_WAP_PUSH
 *   RECORD_AUDIO	-- ¼��
 *   REORDER_TASKS
 *   RESTART_PACKAGES
 *   SEND_SMS	-- ���Ͷ���
 *   SET_ACTIVITY_WATCHER
 *   SET_ALWAYS_FINISH
 *   SET_ANIMATION_SCALE
 *   SET_DEBUG_APP
 *   SET_ORIENTATION	-- ��ת��Ļ
 *   SET_PREFERRED_APPLICATIONS
 *   SET_PROCESS_LIMIT
 *   SET_TIME -- ����ʱ��
 *   SET_TIME_ZONE	-- ����ʱ��
 *   SET_WALLPAPER	-- ���������ֽ 
 *   SET_WALLPAPER_HINTS
 *   SIGNAL_PERSISTENT_PROCESSES
 *   STATUS_BAR
 *   SUBSCRIBED_FEEDS_READ
 *   SUBSCRIBED_FEEDS_WRITE
 *   SYSTEM_ALERT_WINDOW
 *   UPDATE_DEVICE_STATS
 *   USE_CREDENTIALS
 *   VIBRATE	-- ��������, (Vibrator) context.getSystemService(Service.VIBRATOR_SERVICE); 
 *   WAKE_LOCK
 *   WRITE_APN_SETTINGS
 *   WRITE_CALENDAR
 *   WRITE_CONTACTS	-- д���û���ϵ��
 *   WRITE_EXTERNAL_STORAGE -- дSD��
 *   WRITE_GSERVICES
 *   WRITE_HISTORY_BOOKMARKS	-- д��ʷ��ǩ
 *   WRITE_OWNER_DATA	-- д�û�����
 *   WRITE_SECURE_SETTINGS
 *   WRITE_SETTINGS
 *   WRITE_SMS	-- д����
 *   WRITE_SYNC_SETTINGS
 *   
**************************************************************************************************************************************/

public class AndroidManifest  extends AndroidTestCase{
	public void testSave() throws Throwable
	{
		int i=4+8;
		Assert.assertEquals(12,i);
	}
}

