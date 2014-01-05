package com.fishjam.android.study;
import android.test.AndroidTestCase;

/***************************************************************************************************************************************
 * Socket/ServerSocket -- TCP/IP�����ʹ��
 *   Socket client = server.accept(); //��ÿͻ����׽��֣�Ȼ�� client.getOutputStream() �������������ɽ��н�����
 *   InputStream in = socket.getInputStream();  byte[] buffer = new byte[in.available()];  in.read(buffer); String msg = new String(buffer);
 * DatagramSocket/DatagramPackage -- UDP
 * URL/URLConnection/HttpURLConnection 
 *   1.InputStream in = url.openStream();
 *   2.URLConnection conn = url.openConnection(); InputStream in = conn.getInputStream();
 *   3.HttpURLConnection httpConn = (HttpURLConnection)conn; in = httpConn.getInputStream();
 * Servlet 
 *   HttpServlet -- �̳����࣬�������е� doGet /doPost �ȷ���
 *   HttpServletRequest/HttpServletResponse -- 
 * Apache Http �ͻ���
 *   HttpGet/HttpPost��ʹ�� DefaultHttpClient ִ����������Ӧ
 *     HttpPost request = new HttpPost(urlStr); ... HttpResponse response = new DefaultHttpClient().execute(request);
 * WebService -- ʹ�õ������ص� KSOAP.jar ?
 * WebView -- ��ͼ���ֱ����ʾ��ҳ����ʹ���˿�Դ�� WebKit ����
***************************************************************************************************************************************/
public class NetTester extends AndroidTestCase {
	public void TestGetCityListFromWebService(){
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
}