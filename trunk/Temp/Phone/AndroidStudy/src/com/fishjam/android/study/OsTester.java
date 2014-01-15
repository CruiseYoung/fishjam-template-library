package com.fishjam.android.study;
import java.util.Calendar;
import android.util.Log;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.test.AndroidTestCase;

/***************************************************************************************************************************************
 * os --
 *   Bundle -- ���Զԣ�ͨ������״ֵ̬����ͻָ���Ҳ���������ݴ��ݡ�
 *     new Bundle ��ͨ�� Intent �� putExtras �����������á����ն˵� Activity.getIntent (������savedInstanceState).getExtras ���л�ȡ��
 *   Context--Context���Է���AndroidӦ�û�����ϵͳ����, ���ṩ��������Դ����, �������ݿ�ȣ�Activity��Service�ȶ��������ࡣ
 *     getResources() -- �õ�Resources���󣬴Ӷ��������ʸ���������Դ���� r.getDimension(�ߴ���Դ)��r.getXML(�ļ���), getDrawable(ͼƬ��Դ)
 *       ������Դ��һ���ʽΪ: @[����:]��Դ����/��Դ��
 *     getString() -- TODO: ������ Context.getString ���� Resources.getString ?
 *   Environment
 *     getExternalStorageState -- ��ȡSD����״̬���� MEDIA_MOUNTED 
 *   Handler + Message -- (�첽������ƣ�) ������ Handler::handleMessage �������������� switch(Message.whtat) { } ���д���
 *     content--
 *
 * Intent(���� + URI��ʽ������) -- Android��������µ����Ԫ�أ���ͬ���֮���໥������Ŧ������װ�˲�ͬ���֮�䵼�����ҵ�������
 *   Ӧ�ó������ͨ�����������󣬾����Ƿ��������źš�Ӧ�ó�����԰������ƻ򻥲��ķ�ʽ����ע�ᣬ��������������������Ȥִ�и��������intent��
 *   ��Ҫ���֣� 
 *     ����(Action) -- Ҫ��ɵĶ�����Intent ����Ԥ�����˴�����Action������ �� ACTION_CALL��ACTION_EDIT, ACTION_BATTERY_LOW ��
 *       ����ϵͳAction: �����ϵͳ�绰�������ҵ绰�����ֱ��ʹ�� intent.setAction(Intent.ACTION_GET_CONTENT); intent.setType("vnd.android.cursor.item/phone"); 
 *       �Զ���Action: 1.�����Ӧ�� static final �ַ���; 2.��Ŀ�������AndroidManifest.xml��Ӧ�� <intent-filter> ��ָ�� action ����
 *     ����(Data) -- ִ�ж�����URI��MIME���͵���Ϣ����ͬ��Action�в�ͬ��Data����ָ��
 *     ����(Category) -- ִ��Action�ĸ�����Ϣ���� CATEGORY_LAUNCHER ��ʾ���س���ʱ��Activity�����������档
 *     ����(Type),
 *     ���(Component) -- ����ͨ���������ֱ��ָ��Ŀ��
 *     ��չ��Ϣ(Extra) -- ����ĸ�����Ϣ����ͨ��Activity�����ʼ�ʱ����ͨ��Extras���������subject��body��
 *     setClass(Դ, Ŀ��) -- ������ת��Դ(�� MainActivity.this)��Ŀ��class(ResultActivity.class)
 *   ͨ��Intent���ԣ�
 *       ����Activity( startActivity, startActivityForResult)
 *       ����Service( startService, bindService  )
 *       ����Broadcasts( sendBroadcast, sendOrderedBroadcast, sendStickyBroadcast )
 *   Ѱ��Ŀ������ķ�����
 *     1.ͨ���������ֱ��ָ��(  intent.setComponent(new ComponetName(MyActivity.this, "com.fishjam.targetApp.MainActivity")) ) ;
 *     2.ͨ�� AndroidManifest.xml �ļ��е� <intent-filter> ָ���ܴ����Intent���ԣ�ϵͳ������ӳ�䵽��Ӧ�� Activity��IntentReceiver��Service �ȡ�
 *       ��ϵͳ��ע���˶���ܴ����ӦIntent�� <intent-filter>����ϵͳ����ʾ�û�ѡ��һ����������С�
 *       ʹ��ʱ��Ҫ�����������ԣ�
 *          <action> -- �� Intent ָ���ˣ���IntentFilter�еı���ƥ��; �� <action android:name="android.intent.action.VIEW" />
 *          <data> -- ָ��Ҫ�������ݵ�URI(��ʽΪ: scheme://host:port/path )��MIME���ͣ��� <data android:scheme="http" />
 *          <category> -- ������֣� �� <category android:name="android.intent.category.BROWSABLE" />
 *        createChooser
 *   ��������
 * PendingIntent -- ���� Alarm��Notification ���е��ӳ� Intent ?
 *   PendingIntent pi = PendingIntent.getBroadcast(MainActivity.this, 0, intent, 0);     
***************************************************************************************************************************************/

/***************************************************************************************************************************************
 * BroadcastReceiver -- �㲥��������Ӧ�ó����м̳и�����ʵ�ּ�������Ӧ��Щ�㲥��Intent�� ��Ҫ��ϵͳע��(�����ļ��� Context.registerReceiver )��
 *   onReceive -- ���ظ÷�������Ӧ�¼�
 * ? �ͻ���ͨ�� Context::sendBroadcast  ����һ��ϵͳ������¼��㲥��������Ϣ
 * ϵͳԤ�����ϵͳ�㲥�¼�:
 *   ACTION_BOOT_COMPLETED -- ϵͳ�������
 *   ACTION_BATTERY_LOW -- ������
 *   ACTION_MEDIA_BUTTON -- ����ý�尴ť
 *   ACTION_MEDIA_EJECT -- �����γ��ⲿý��
 *   ACTION_PACKAGE_ADDED -- ��Ӱ�
 *   ACTION_TIME_CHANGED -- ʱ��ı�
***************************************************************************************************************************************/

/***************************************************************************************************************************************
 * AlarmManager -- ϵͳ������ʾ����ͨ�� Context.getSystemService(Context.ALARM_SERVICE) ���
 *   setRepeating(AlarmManager.RTC_WAKEUP, time, 8*1000, pi);
 *   
 * ͨ�������(NotificationManager)--��������Ӧ�ó�����ʾ�ض��ľ�����Ϣ��״̬����
 *   ������ʾ�㲥��Ϣ�����ݡ�ͼ�꼰�񶯵���Ϣ
 *   ʹ�ò��裺
 *     1.���ϵͳ���� NotificationManager -- (NotificationManager)Context.getSystemService(NOTIFICATION_SERVICE);
 *     2.ʵ���� Notification ����������
 *        notice = new Notification(); notice.tickerText = "xxx";  notice.when = System.currentTimeMillis();
 *        Intent intent = new Intent(MainActivity.this, MainActivity.class);
 *        PendingIntent pi = PendingIntent.getActivity(MainActivity.this, 0, intent, 0); 
 *        notice.setLatestEventInfo(this, "MyTitile", "MyContent", pi); // �����¼���Ϣ
 *     3.��ʾ��֪ͨ��ID������֪ͨ
 *        nm.notify(nID, n);
 *     
 * ���������(ActivityManager)--����Ӧ�ó���Ļ���ڣ������ṩͨ�õĵ���֧�֣�navigation backstack��
 * 
 * TODO: ���䲿��?
 * ��������(PackageManager)--����Ӧ�ó������ͨ�� getPackageManager()�������
 *   ���Ի�ȡӦ�ó������Ϣ(������,CacheSize��DataSize��),�����Ǵ����а��� AndroidManifest.xml �н�������?
 *   PackageItemInfo -- AndroidManifest.xml�ļ������нڵ�Ļ��࣬�� ActivityInfo��ApplicationInfo��ServiceInfo �ȶ�������
 *   ʾ����queryIntentActivities �ɲ�ѯ���о߱�ACTION_MAIN��CATEGORY_LAUNCHER��Intent��Ӧ�ó���Ȼ������û����ʱ����Ӧ��(����Home�����Launcher)
***************************************************************************************************************************************/

class MyReceiver extends BroadcastReceiver{
	@Override
	public void onReceive(Context context, Intent intent) {
		Log.i("Receiver", "Receive BOOT_COMPLETED broadcast information");
	}
}

public class OsTester  extends AndroidTestCase {
	public OsTester(){
		//Intent intent = new Intent();
	}
	
	public void CheckPhoneConfig() {
		//DisplayMetrics dm = new DisplayMetrics();
		//getWindowManager().getDefaultDisplay().getMetrics(dm);
		//String strOpt = "�ֻ���Ļ�ֱ���Ϊ��" +  dm.widthPixels + " �� " + dm.heightPixels;  //320 x 480
	}
	public void IntentTester(){
		//���´�����ԴActivity������(MainActivity) ��ִ�У��������� ResultActivity ���ݲ���
		/*
		Bundle bundle = new Bundle();
		bundle.putString("username", "userNameString");
		bundle.putString("passwd", "passwdString");
		Intent intent = new Intent(MainActivity.this, ResultActivity.class);
		intent.putExtra("data", bundle);
		startActivity(intent);
		*/
		
		//���´�����Ŀ��Activity������(ResultActivity) ��ִ�У����մ���Ĳ���
		/*
		Intent intent = this.getIntent();
		Bundle bundle = intent.getBundleExtra("data");
		List list = new ArrayList();
		list.add(bundle.getString("username"));
		ArrayAdapter adapter = new ArrayAdapter(this, android.R.layout.simple_list_item_checked, list);
		listView.setAdapter(adapter);
		*/
	}
	
	public void CalendarTester()
	{
		 //Calendar -- ����(TODO: �ǲ���UI�ؼ� ?)
		Calendar calendar = Calendar.getInstance();
		if (calendar != null) {
			int  year 	= calendar.get(Calendar.YEAR);
			int month = calendar.get(Calendar.MONTH);
			int day 		= calendar.get(Calendar.DAY_OF_MONTH) ;
		}
		
	}
}
