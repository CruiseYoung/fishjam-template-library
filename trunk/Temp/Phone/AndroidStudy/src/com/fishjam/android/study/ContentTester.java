package com.fishjam.android.study;
import android.app.ListActivity;
import android.content.ContentResolver;
import android.database.Cursor;
import android.provider.Contacts.People;
import android.widget.Adapter;
import android.widget.BaseAdapter;
import android.widget.ListAdapter;
import android.widget.SimpleCursorAdapter;
import junit.framework.TestCase;

/***************************************************************************************************************************************
 * �����ṩ��(ContentProvider) -- ����һ��Ӧ�ó����������Ӧ�ó��򣨱���ͨ��¼�������ݣ����߹������Լ�������
 *   ContentResolver -- �����ṩ��? ����Cursor���� ? 
 *   
 * Adapter -- �������ӿ�
 *   BaseAdapter
 *   ArrayAdapter
 *   SimpleCursorAdapter 
 *
 **************************************************************************************************************************************/

public class ContentTester  extends TestCase{
	public void ListActivityCursorTester(){
		/*********************************************************************************************************
		//class MyActivity extends ListActivity
		
		Cursor cursor = getContentResolver().query(People.CONTENT_URI, null, null, null, null);
		startManagingCursor(c);
		ListAdapter adapter = new SimpleCursorAdapter(this,  android.R.layout.simple_list_item_1,  c,  
				new String[] { People.NAME },  new int[] {android.R.id.text1} );
		setListAdapter(adapter);
		*********************************************************************************************************/
	}
}
