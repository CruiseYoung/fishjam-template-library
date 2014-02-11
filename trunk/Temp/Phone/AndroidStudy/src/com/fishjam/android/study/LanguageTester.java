package com.fishjam.android.study;
import java.util.ArrayList;
import java.util.Date;

import android.R.string;
import android.test.AndroidTestCase;
import android.util.FloatMath;
import android.util.Log;
import junit.framework.Assert;
import java.util.Collection;

/***************************************************************************************************************************************
 * Android ����������
 *   Java ���Ա���û��������������Ĺ��ܣ�����Java���������Ż��Ͳ�������(final Boolean)�Ļ��ƣ������������ʽ����ԶΪfalse����䣬
 *   �������������������ǵĴ���������ֽ���(�����������ļ�) ���磺
 *      final boolean bDebug = false;
 *      if(bDebug) { xxxxx -- ����ʱ�ᱻ�Ż��� }
 *   ���Զ���һ�������ݵ������������ƣ��磺
 *      public class AppConfig { public static final boolean bDebug = true; .... }
 *      Ȼ���ڴ�����ֱ��ʹ����Щ final ���� -- if(AppConfig.bDebug) { ... }
 *
 *  TODO:ʹ�����Ʒ���������ʹ������API��ȡ�������������룿 
 *     ϵͳ������һ�� BuildConfig �࣬������ public final static boolean DEBUG = true;
 *     
 *   system.prop(AndroidԴ�����ʱʹ�ã�) -- ͨ�� Key=Value ��ֵ�� �ķ�ʽ����ϵͳ���ԣ�
 *   ͨ�� android.os.SystemProperties ���ȡ�������ݣ���ʵ�ָ��������ļ����������롣
 *   ע�⣺SystemProperties �ǷǱ�׼��SDK�ӿڣ���Ҫʹ�ã��� Android.mk �ļ��в��ܶ��� LOCAL_SDK_VERION ����(�ñ�����ʾӦ��ֻ��ʹ�ñ�׼��SDK�ӿ�)
 *     �磺./build/target/board/generic/system.prop �ļ�(����ʱ������ű�����ݸ��ļ�����  build.prop �ļ�)
 *     private static final boolean bSupportBluetooth = SystemProperties.getBoolean("ro.Gallery.bSupportBluetooth", false);
 *     if(bSupportBluetooth) { ... } 
***************************************************************************************************************************************/

/***************************************************************************************************************************************
 * Android�﷨
 *  1.Android�е������������ mXXXX �ķ�ʽ��û���»��ߣ�
 *  
 *  HashMap<String, String> -- ��ͬ�����ռ�ֵ��Ч�ʸ�
 *  HashTable -- ͬ�����ǿռ�ֵ��Ч���Ե�
 * 
 * ����
 *    private int[] mColors = new int[] { Color.BLACK, Color.RED, Color.BLUE,Color.GREEN, Color.MAGENTA, Color.YELLOW };
 *    ArrayAdapter -- ����������
 *      String[] strs = {"a", "b", "c"}; ArrayAdapter aa = new ArrayAdapter(this, android.R.layout.simple_spinner_item, strs);
 *      spinner.setAdapter(aa); 
 *    
 * super ���û����ͬ����������: super.onCreate(xxxx);
 *
 * annotation
 * 
 * Android ʹ��Java�е��¼�������ƣ�����( TODO ):
 *   �¼� -- (extends EventObject), ͨ�� FireEvent �����¼� 
 *   �¼�Դ
 *   �¼������� -- ��EventListener �̳�����������ӿڣ�����ļ����� implements �ýӿ�  
 *     Androidϵͳ�г������¼���������
 *       View �е� OnClickListener, OnFocusChangeListener, OnKeyListener, OnTouchListener, OnCheckedChangeListener �� 
 **************************************************************************************************************************************/

/**************************************************************************************************************************************
 *  Android ����һ��C/C++�⣬���Ա�Androidϵͳ�ĸ������ʹ�ã���Щ����ͨ��Android Ӧ�ó����ܿ��Ÿ������ߣ������г��˺��Ŀ⡣
 *    ϵͳC������ ��BSD��׼Cϵͳ��̳еģ�����Ƕ��ʽlinux�豸�����Ż��İ汾��
 *    ��ý��� -- ����PacketVideo��˾��OpenCORE�⣬�ÿ�֧��¼���طţ�¼�����е���������Ƶ��ʽ�ļ����Լ���̬ͼ���ļ�����Щ�ļ���ʽ����MPEG4,H.264,MP3,AAC,AMR,JPG,��PNG.
 *    �������(Surface Manager) -- �������ʾ��ϵͳ�ķ��ʣ������޷�ϳɲ�ͬ��Ӧ�ó����е�2D��3Dͼ���
 *    LibWebCord �C һ�����е���ҳ��������棬��ǿAndroid�������Ƕ��ʽ��ҳ���������
 *    SGL �C �ײ��2D ͼ������
 *    3D �� �C һ������OpenGL ES 1.0 APIs�İ汾�������ȿ���ʹ��Ӳ��3D���٣�Ӳ��֧�֣��������õģ��߶��Ż���3D�����
 *    ��������(FreeType) -- λͼ(bitmap)����ʸ��(vector)����
 *    SQLite �C һ��ǿ�����������ϵ���ݿ����棬�������е�Ӧ�ó���ʹ��
 *    ����(?): ����(Bluetooth), EDGE, 3G, WiFi(������Ӳ��)�������, GPS, ָ����, ���ٶȼ�
 *    
**************************************************************************************************************************************/

public class LanguageTester  extends AndroidTestCase{
	private static final String TAG = LanguageTester.class.getName();
	
	public void testParseDouble() throws Throwable 
	{
		double dValue = Double.parseDouble("1.05");
		assertEquals(dValue, 1.05, 0.001);
		//��Double������������
		//NumberFormat formatter = new DecimalFormat("0.00");
	    //String s=formatter.format(num);
		
		assertEquals(FloatMath.sqrt(100), 10.0);  //ƽ����
	}
	
	public void testTemplate(){
		Collection<String> colStrings = new ArrayList<String>();
		colStrings.add("fishjam");
		Log.i("Language", new Integer(colStrings.size()).toString());
		assertEquals(colStrings.size(), 1);
	}
	
	public void testDateTime(){
		Log.i(TAG, new Date().toString());
	}
}
