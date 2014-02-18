package com.fishjam.android.study;
import java.io.DataOutputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStream;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.SharedPreferences.Editor;
import android.graphics.Bitmap;
import android.os.Environment;
import android.os.StatFs;
import android.test.ActivityTestCase;
import android.test.ActivityUnitTestCase;
import android.text.StaticLayout;
import android.util.Log;
import android.widget.ArrayAdapter;
import android.widget.AutoCompleteTextView;
import android.widget.SimpleAdapter;


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
 *   SharedPreferences -- ���ݽ��ٵ�������Ϣ�ȣ� ʹ�ü�ֵ�Եķ�ʽ������ XML �ļ��� ( /Android/data/<package>/shared_prefs/ Ŀ¼�� ),
 *     ʹ���ϱȽϷ��㣬�����ܲ�ǿ�����ʺϱ���Ƚϴ�����ݣ�������ļ�ֻ����ͬһ����ʹ�ã������ڲ�ͬ��֮��ʹ��(ͬһ������Ĳ�ͬpackageҲ�����ã�)
 *        ʹ�ã�Activity.getSharedPreferences(���ʵ��) -> 
 *     ����������
 *         1.���� Preference ���͵�xml���������� res/xml �£����ж������Preference��Ϣ��(��������ʾ)������������(key/title/summary)
 *            PreferenceScreen -- PreferenceActivity�ĸ�Ԫ�أ�ֻ��һ��
 *              PreferenceCategory -- ������(�ɶ��)����Ϊ���ڵ㣬���µ��ӽڵ������ṹչʾ
 *                CheckBoxPreference -- 
 *                EditTextPreference -- �༭��
 *                ListPreference -- �б����ͨ�� entries/entryValues ������ string-array ���õ�������Դ 
 *                Preference -- ֻ�����ı���ʾ��һ�����ڵ����ض������ý��棬��"��������"��ͨ����ͨ�� dependency ����������������(��"��������")���Ʊ������Ƿ����
 *                RingtonePreference -- ����
 *         2.�� PreferenceActivity �̳У��� onCreate �� addPreferencesFromResource(R.xml.mypreference); ����Ҫ��ʾ����������ҳ��
 *         3.�� onPreferenceTreeClick ��ʹ�� SharedPreferences ͨ�� key���ж�д
 *      
 *   File (Ĭ�ϱ����� /data/<package>/files ����)
 *     Context.openFileInput   -- ��ñ�׼��Java�ļ�������(FileInputStream)��Ȼ��ѭ����ȡ 
 *     Context.openFileOutput -- ��ñ�׼��Java�ļ������(FileOutputStream)��fos.write(content.getBytes()); fos.close();
 *     Resources.openRawResource -- ���� InputStream
 *     ����ʹ�� DataOutputStream/DataInputStream ��װFile��������֧�ָ���Ķ�д����( �� writeUTF(String) ).
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
 *   BaseAdapter -- �Զ����������Ļ��࣬һ��Ӹ���̳У���ΪModule��View֮����������Ӹ���̳п���ȡ�ö�Adapter���Ŀ���Ȩ��
 *     ʵ�� getCount, getItem{return null}, getView{ return myCustomView; } �ȷ��������������� Windows Virtual ListView �ĸ�������ʾ
 *   ArrayAdapter -- �����ڽ������List���ϵĶ��ֵ��װ�ɶ���б���������ޣ����б���ֻ����TextView��һ��ֻ���� AutoCompleteTextView ��ֻ��ʾ�ı��ĵط�
 *   SimpleAdapter -- �����ڽ�List���ϵĶ�������װ�ɶ���б�����ܺ�ǿ��

 *   SimpleCursorAdapter -- ��װCursor�ṩ������
 *     
 * Intent/IntentFilter -- ��ͬ���֮��ͨ�ŵ�����
 *   ��ʽIntent -- ��ȷָ����Ҫ�����򴥷������������
 *   ��ʽIntent -- ֻ��ָ����Ҫ�����򴥷������Ӧ����������������Androidϵͳ����������������ϵͳ�в�����֮ƥ���Ŀ����������ҵ��������򴥷���
 *   
 *   �������ͺͷ�ʽ
 *     ����Activity -- Context.startActivity/startActivityForResult
 *     ����Service -- Context.startService/bindService
 *     ���͹㲥 -- Context.sendBroadcast, sendStickyBroadcast, sendOrderedBroadcast

 **************************************************************************************************************************************/

public class ContentTester  extends ActivityUnitTestCase<Activity>{
	private final static String TAG = ContentTester.class.getSimpleName();
	 private Intent mStartIntent;
	 
	public ContentTester() {
		super(Activity.class);
		// TODO Auto-generated constructor stub
	}
	
	@Override
	protected void setUp() throws Exception {
		super.setUp();
		
		mStartIntent = new Intent(Intent.ACTION_MAIN);
	}

	public void testArrayAdapter(){
		 startActivity(mStartIntent, null, null);
		 assertNotNull(getActivity());
		
		String [] booksStrings = {"book1" , "book2", "book3"};
		ArrayAdapter<String> adapter1 = new ArrayAdapter<String>(getActivity() , android.R.layout.simple_list_item_1, booksStrings);
		AutoCompleteTextView autoCompleteTextView = (AutoCompleteTextView)getActivity().findViewById(R.id.textView1);
		if (autoCompleteTextView != null) {
			autoCompleteTextView.setAdapter(adapter1);
		}
	}
	
	public void testFile()
	{
		final String pathString =Environment.getExternalStorageDirectory() + File.separator  + "androidStudy";
		final String fileNameString = "testFile.txt";
		File dir = new File(pathString);
		if (!dir.exists()) {
			dir.mkdirs();
		}
		File file = new File(pathString + File.separator + fileNameString);
		Log.i(TAG, "FilePath=" + file.getPath());
		
		try {
			file.createNewFile();
			OutputStream outStream = new FileOutputStream(file);
			
			DataOutputStream dataOutputStream = new DataOutputStream(outStream);
			dataOutputStream.writeUTF("some unicode string");
			//bitmap.compress(Bitmap.CompressFormat.PNG, quantity, outStream);
			outStream.flush();
			outStream.close();
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
	public void testFileIoStream() {
		startActivity(mStartIntent, null, null);
		
		FileOutputStream fileOutputStream = null;
		DataOutputStream dataOutputStream = null;
		try {
			
			fileOutputStream = getActivity().openFileOutput("dstFile.txt", Context.MODE_PRIVATE);
			dataOutputStream = new DataOutputStream(fileOutputStream);
			dataOutputStream.writeUTF("some unicode string");
			
		} catch (IOException e) {
			e.printStackTrace();
		} finally {
			try {
				if (dataOutputStream != null) {
					fileOutputStream.close();
				}
				if (fileOutputStream != null) {
					fileOutputStream.close();
				}
			} catch (IOException e) {
				e.printStackTrace();
			}
		}
	}
	
	public void testSimpleAdapter(){
		startActivity(mStartIntent, null, null);
		
		List<Map<String, Object>> listItems = new ArrayList<Map<String, Object>>();
		for (int i = 0; i < 10; i++) {
			Map<String, Object> listItem = new HashMap<String, Object>();
			listItem.put("keyName", "myValue");
			listItems.add(listItem);
		}
		SimpleAdapter simpleAdapter = new SimpleAdapter(
				this.getActivity(), 
				listItems,	//List<? extends Map<String, ?>> ���͵ļ��϶������е�ÿ�� Map<String, ?> ��������һ���б��� 
				android.R.layout.simple_list_item_checked, 	//һ�����沼��ID��ÿ���б������������в���
				new String[]{"keyName"}, 	//String[] �����飬������ȡMap<String,?>��������Щkey��Ӧ��value�������б���
				new int[]{android.R.id.text1}	//int[] ���͵����飬������Ӧ�� Key ��ʾ�ڽ��沼���е��ĸ������
		);
		//ListView listView = (ListView)getActivity().findViewById(R.id.mylist);
		//listView.setAdapter(simpleAdapter);
	}
	public void testListActivityCursor(){
		
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
	public void testQueryUserInfoByName(){
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
	
	public void testDialTel(){
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
	
	public void testSharedPreferences(){
		startActivity(mStartIntent, null, null);
		//��ȡʵ��������ģʽ�У� 
		//   MODE_PRIVATE(�����ݸ���ԭ����)
		//   MODE_APPEND(������׷�ӵ�ԭ���ݺ�), 
		//   MODE_WORLD_READABLE(��������Ӧ�ó����ȡ)
		//   MODE_WORLD_WRITEABLE(��������Ӧ�ó���д�룬�Ḳ��ԭ����)
		SharedPreferences sharedPreferences = getActivity().getSharedPreferences("FileName",  Context.MODE_PRIVATE);
		
		//ͨ��Editor�ӿڽ���༭ģʽ
		Editor editor = sharedPreferences.edit();   
		editor.putString("key","Value");  
		editor.commit();  //�ύ��Ч
		 
		 //��ȡģʽ����ȡ֮ǰ���õ�ֵ
		String sKeyValue = sharedPreferences.getString("key", "defaultValue");
		assertEquals(sKeyValue, "Value");
	}

	final static int MB = 1024 * 1024;
	public void testSDCardInfo(){
		Log.i(TAG, Environment.getExternalStorageState()); //Environment.MEDIA_MOUNTED -- ��ʾ mounted 
		
		StatFs stat = new StatFs(Environment.getExternalStorageDirectory().getPath());
		double sdFreeMB = ((double) stat.getAvailableBlocks()  * (double) stat.getBlockSize()) / MB;
		Log.i(TAG, "SDCard Info: FreeMB=" + sdFreeMB );
	}
}
