package com.fishjam.android.study;
import java.util.Calendar;

import android.R.integer;

import junit.framework.TestCase;

/***************************************************************************************************************************************
 * os --
 *   Bundle -- ���Զԣ�ͨ������״ֵ̬����ͻָ���Ҳ���������ݴ��ݡ�
 *     new Bundle ��ͨ�� Intent �� putExtras �����������á����ն˵� Activity.getIntent (������savedInstanceState).getExtras ���л�ȡ��
 *   Handler
 *   Message
 *     content--
 *   Context--Context���Կ���ϵͳ����, ���ṩ��������Դ����, �������ݿ��
 *     getResources() -- �õ�Resources���󣬴Ӷ��������ʸ���������Դ���� r.getDimension(�ߴ���Դ)��r.getXML(�ļ���), getDrawable(ͼƬ��Դ)
 *       ������Դ��һ���ʽΪ: @[����:]��Դ����/��Դ��
 *     getString() -- TODO: ������ Context.getString ���� Resources.getString ?
 *
 * intent(���� + URI��ʽ������) -- Android��������µ����Ԫ�أ���ͬ���֮���໥������Ŧ������װ�˲�ͬ���֮�䵼�����ҵ�������
 *   Ӧ�ó������ͨ�����������󣬾����Ƿ��������źš�Ӧ�ó�����԰������ƻ򻥲��ķ�ʽ����ע�ᣬ��������������������Ȥִ�и��������intent��
 *   
***************************************************************************************************************************************/

/***************************************************************************************************************************************
 * ͨ�������(NotificationManager)--��������Ӧ�ó�����ʾ�ض��ľ�����Ϣ��״̬����
 * ���������(ActivityManager)--����Ӧ�ó���Ļ���ڣ������ṩͨ�õĵ���֧�֣�navigation backstack��
***************************************************************************************************************************************/

public class OsTester  extends TestCase {
	public OsTester(){
		
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
