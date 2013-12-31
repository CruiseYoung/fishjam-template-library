package com.fishjam.android.study;
import android.app.ListActivity;
import android.content.ContentResolver;
import android.content.Intent;
import android.database.Cursor;
import android.net.Uri;
import android.provider.Contacts.People;
import android.widget.Adapter;
import android.widget.BaseAdapter;
import android.widget.ListAdapter;
import android.widget.SimpleCursorAdapter;
import junit.framework.TestCase;

/***************************************************************************************************************************************
* Uri
*      Uri uri = Uri.parse("content://contacts/people/1");  
* UriMatcher 
*     UriMatcher sUriMatcher = new UriMatcher(UriMatcher.NO_MATCH);
*     sUriMatcher.addURI(Employees.AUTHORITY, "employee", EMPLOYEE);
*     sUriMatcher.addURI(Employees.AUTHORITY, "employee/#", EMPLOYEE_ID);
*     switch(sUriMatcher.match(uri)){ case EMPLOYEE_ID:  String noteId= uri.getPathSegments().get(1);  };
*     
* ϵͳ�����˴�����URI
*   Contacts.People.CONTENT_URI --
***************************************************************************************************************************************/

/***************************************************************************************************************************************
 * Android�ṩ�˶������ݴ�ȡ��ʽ��
 *   Preference -- ���ݽ��ٵ�������Ϣ�ȣ� ʹ�ü�ֵ�Եķ�ʽ������ XML �ļ��� ( /Android/data/<package>/shared_prefs/ Ŀ¼�� )
 *      SharedPreferences --  Context.getSharedPreferences(�ļ�����ģʽ).edit().putString("key","Value");  �޸ĺ���Ҫ commit();
 *   File (Ĭ�ϱ����� /data/<package>/files ����)
 *     Context.openFileInput   -- ��ñ�׼��Java�ļ�������(FileInputStream)��Ȼ��ѭ����ȡ 
 *     Context.openFileOutput -- ��ñ�׼��Java�ļ������(FileOutputStream)��fos.write(content.getBytes()); fos.close();
 *     Resources.openRawResource -- ���� InputStream
 *   SQLite (�����ļ�һ�㱣���� /data/<package>/databases/ �£��������ʱ��Ҫʹ��ȫ·�� )
 *     SQLiteDatabase -- ���ݿ�����࣬���� ��������ɾ�� ��
 *     SQLiteOpenHelper -- �����࣬�������ݿⴴ���Ͱ汾���¡��̳к����� onCreate/onUpgrade �ȷ�������ͨ�� getWritableDatabase/getReadableDatabase ��ȡ��Ӧ�����ݿ�
 *     Cursor(�α�ӿڣ��ṩ������ѯ����ķ���)
 *   ����
***************************************************************************************************************************************/

/**************************************************************************************************************************************
 * �����ṩ��(ContentProvider) -- ��Ӧ�ó��������ݵ�Ψһ������ ��Ӧ��URI������ "content://" ��ͷ�ġ�
 *   Android�и���Ӧ�ó������֮�����໥�����ģ��˴˵����ݲ��ܹ���(�ܷ������������ DB/File ��)�� 
 *     ContentProvider -- �����ͱ������ݣ�����һ��Ӧ�ó����������Ӧ�ó��򣨱���ͨ��¼�������ݣ����߹������Լ������ݣ����а�������ɾ�ġ���ѯ�Ȳ���
 *       ���Զ���ʱ����Ҫ���صĳ��󷽷�: query/insert/update/delete/getType �ȣ�Ȼ���������ļ���������
 *       ϵͳ�Ѿ�Ϊ���õ���������(��Ƶ����Ƶ��ͼƬ����ϵ��ʽ��)�ṩ�˴�����ContentProvider
 *       -- MediaStoreContentProvider -- ���Ӷ�ý���ļ��Ĳ����� ContentProvider
 *     ContentResolver -- �ͻ��˷���ʱʹ�õ���(Context.getContentResolver() )�����ڲ�������Ӧ�� ContentProvider �������ݽ��в����� 
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
				new String[] { People.NAME },  new int[] {android.R.id.textName } );
		setListAdapter(adapter);
		*********************************************************************************************************/
		
	}
	
	//�����û�������ϵ�˹�������в�ѯ
	public void QueryUserInfoByName(){
	/********************************************************************************************************
	//TODO: 1.ʹ�õķ�ʽ�Ѿ�����̭��Ӧ���и����ʵķ�ʽ; 2.��Ҫ���� READ_CONTACTS ��Ȩ��
	 
		String name = "userName";
		//������ѯ����
		String [] projectionStrings = new String[] { People._ID, People.NAME, People.NUMBER };
		Uri contactsUri = People.CONTENT_URI;  //��ѯUri����
		String[] args = { name };	//��ѯ����
		//����������ѯ��ϵ��
		Cursor managedCursor = managedQuery(contactsUri, projectionStrings, "name=?", args, People.NAME, " ASC" );
		if(managedCursor.moveToFirst()){
			String name1 = managedCursor.getString(1); 	//�������
			String number = managedCursor.getString(2); //��õ绰����
		}
	*******************************************************************************************************/
	}
	
	public void DialTelTester(){
	/*******************************************************************************************************
	    //��ʾ����绰����ʹ�� Intent.ACTION_CALL ��ֱ�Ӳ���绰
		String data = "tel:15184464231";
		Uri uri = Uri.parse(data);
		Intent intent = new Intent();
		intent.setAction(Intent.ACTION_DIAL);
		intent.setData(uri);
		startActivity(intent);
	*******************************************************************************************************/
	}
}
