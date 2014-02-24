package com.fishjam.android.study;
import java.io.IOException;
import java.io.UnsupportedEncodingException;
import java.net.URL;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.apache.http.HttpResponse;
import org.apache.http.NameValuePair;
import org.apache.http.client.ClientProtocolException;
import org.apache.http.client.HttpClient;
import org.apache.http.client.entity.UrlEncodedFormEntity;
import org.apache.http.client.methods.HttpGet;
import org.apache.http.client.methods.HttpPost;
import org.apache.http.impl.client.DefaultHttpClient;
import org.apache.http.message.BasicNameValuePair;
import org.apache.http.util.EntityUtils;

import android.graphics.BitmapFactory;
import android.test.AndroidTestCase;
import android.text.Html.TagHandler;
import android.util.Log;

/***************************************************************************************************************************************
 * ע�⣺ 
 *   1.������صĲ����������߳��е���(���߳��е��õĻ������׳� xxx �쳣 )
 * 
 * Socket/ServerSocket -- TCP/IP�����ʹ��
 *   Socket client = server.accept(); //��ÿͻ����׽��֣�Ȼ�� client.getOutputStream() �������������ɽ��н�����
 *   InputStream in = socket.getInputStream();  byte[] buffer = new byte[in.available()];  in.read(buffer); String msg = new String(buffer);
 * DatagramSocket/DatagramPackage -- UDP
 * URL/URLConnection/HttpURLConnection 
 *   1.InputStream in = new URL(myUrl).openStream();  //��Url��Ӧ��������(�� �ļ��������?) 
 *   2.1. �������:
 *       URLConnection conn = url.openConnection(); InputStream in = conn.getInputStream();
 *       HttpURLConnection httpConn = (HttpURLConnection)conn; in = httpConn.getInputStream();
 *   2.2 ����ͼƬ����
 *       bitmap = BitmapFactory.decodeStream(in);
 *   3. in.close();  //ʹ����Ϻ�ע��ر�
 * URLEncoder 
 *   encode -- ?
 * Servlet 
 *   HttpServlet -- �̳����࣬�������е� doGet /doPost �ȷ���
 *   HttpServletRequest/HttpServletResponse -- 
 * Apache Http �ͻ���
 *   HttpGet/HttpPost��ʹ�� DefaultHttpClient ִ����������Ӧ
 *     HttpPost request = new HttpPost(urlStr); ... HttpResponse response = new DefaultHttpClient().execute(request);
 * WebService -- ʹ�õ������ص� KSOAP.jar ?
 * WebView -- ��ͼ���ֱ����ʾ��ҳ����ʹ���˿�Դ�� WebKit ����
 * 
 * 
***************************************************************************************************************************************/

/***************************************************************************************************************************************
 * Json
 *   JSONArray
 *   JSONObject
 *   JSONStringer
 *   JSONTokener
***************************************************************************************************************************************/


public class NetTester extends AndroidTestCase {
	private static final String TAG = NetTester.class.getName();
	

	public void testGetCityListFromWebService(){
		/****************************************************************************************************************************************
		//ͨ��WebService��ȡ����Ԥ���ĳ����б�
		List<String > lstCiteStrings = new ArrayList<String>();
		SoapObject request = new SoapObject("http://webXml.com.cn/", "getRegionProvince");
		//������л��� envelope
		SoapSerializationEnvelope envelope = new SoapSerializationEnvelope(SoapEnvelope.VER11);
		envelope.bodyOut = request;
		(new MarshalBase64()).register(envelope);
		AndroidHttpTransport ht = new AndroidHttpTransport("http://webservice.webxml.com.cn/WebServices/WeatherWS.asmx");
		ht.debug = true;
		ht.call("http://webXml.com.cn/getRegionProvince", envelope);  //����
		if (envelope.getResponse() != null) {
			String str = envelope.bodyIn.toString();
			if (str.length() > 0) {
				//�ַ���������������
				lstCiteStrings.add("xxxx");
			}
		}
    ****************************************************************************************************************************************/
	}
	
	public void testHttpClientGet() throws Exception{
		final String GET_URL = "http://www.baidu.com/";

		// ����HttpClient����
		HttpClient httpClient = new DefaultHttpClient();

		// ����HttpGet����
		HttpGet get = new HttpGet(GET_URL);
		// ����GET����
		HttpResponse httpResponse = httpClient.execute(get);
		if (httpResponse.getStatusLine().getStatusCode() == 200) {
			// ��ȡ��������Ӧ�ַ���
			String result = EntityUtils.toString(httpResponse.getEntity());
			Log.i(TAG, result);
		}
	}
	
	public void testHttpClientPost() throws Exception {
		final String POST_URL = "http://www.baidu.com/";
		Map<String ,String> rawParams = new HashMap<String, String>();  //�������
		rawParams.put("user", "fishjam");
		rawParams.put("pass", "password");
		
		HttpClient httpClient = new DefaultHttpClient();		// ����HttpClient����
		HttpPost post = new HttpPost(POST_URL);		// ����HttpPost����
		
		// ������ݲ��������Ƚ϶�Ļ����ԶԴ��ݵĲ������з�װ
		List<NameValuePair> params = new ArrayList<NameValuePair>();
		for(String key : rawParams.keySet())
		{
			params.add(new BasicNameValuePair(key, rawParams.get(key)));	//��װ�������
		}
		post.setEntity(new UrlEncodedFormEntity(params, "UTF-8"));		// �����������
		HttpResponse httpResponse = httpClient.execute(post);		// ����POST����
		
		// ����������ɹ��ط�����Ӧ
		if (httpResponse.getStatusLine().getStatusCode() == 200)
		{
			// ��ȡ��������Ӧ�ַ���
			String result = EntityUtils.toString(httpResponse.getEntity());
			Log.i(TAG, result);
		}
	}
}