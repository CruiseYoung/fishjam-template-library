package com.fishjam.android.study;
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
}
