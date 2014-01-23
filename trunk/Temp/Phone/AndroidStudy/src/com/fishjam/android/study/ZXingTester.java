package com.fishjam.android.study;
import android.test.AndroidTestCase;

/**************************************************************************************************************************************
 * 
 * ZXing(http://code.google.com/p/zxing/) -- ��Դ�Ķ�ά�롢�����봦��⣬�����˶������ԵĽӿڡ���΢���е�ɨһɨ��
 *    ��Ҫ��Ϊ��
 *      core -- ���ı�����
 *      android -- Android�ͻ��ˣ�ȱ�㣺Ҫ���û����밲װBarcodeScanner?�û����鲻�á�
 *    ȱ�㣺
 *       ʵ��̫�����Ӷ��ණ��̫��(ָ 2.1 ��ǰ�İ汾?)��
 *  
 * ���룺
 *   ZXing2.1 ��ǰ�汾(ʹ��ant����, ʹ�� build.xml)
 *     1.B.���� Ant -- http://ant.apache.org/, ��ѹ������ ANT_HOME ��������������bin����%PATH%
 *     2.���� ProGuard -- http://proguard.sourceforge.net/ -- ѹ�����Ż��ͻ���Java�ֽ����ļ�����ѹ��ߣ�����ɾ�����õ��ࡢ�ֶΡ����������Ե�
 *   ZXing2.1 �Ժ�汾(ʹ��maven����, ʹ�� pom.xml ?)
 *     1.���� maven -- https://maven.apache.org/download.cgi�� ��ѹ������ MAVEN_HOME ��������������bin����%PATH%
 *     2.���� core Ŀ¼��ִ��  mvn -DskipTests -Dgpg.skip=true install ���б��룬��һ��ʹ��ʱ�����غܶ������⡣������� target �ҵ����ɵ�jar��
 *     3.���±���Android��������(���� local.properties �ļ�����SDK��·��): android.bat update project --path <����Ŀ¼���� android>
 *     3.����Դ���"Build path" -> "Add External Archives" ������ core.jar
 *   
 *  Դ�����
 *    CaptureActivity -- ����Activity(ɨ������������?)
 *    CaptureActivityHandler -- ���봦���࣬�������������߳̽��н���
 *    DecodeThread -- ������߳�
 *    ViewfinderView -- �Զ����View��������ʱ�м�Ŀ��
 * ������
 *    IntentIntegrator + IntentResult -- ͨ�� Intents ��ʽʹ�ö�ά��(��Ҫ���û���װBarcodeScanner)
 *    MatrixToImageWriter -- ���ɶ�ά��
 *    MultiFormatReader -- ��ȡ�ͷ�����ά��ͼƬ
**************************************************************************************************************************************/

public class ZXingTester extends AndroidTestCase {

	public void testGenerateCode(){
		//���ɶ�ά���ļ�
		/*
		 BitMatrix bitMatrix = new MultiFormatWriter().encode(text, BarcodeFormat.QR_CODE, width, height,hints);  //���ɶ�ά�� 
		 File outputFile = new File("d://new.png"); //����ͼƬ�洢��λ�ã�D����Ŀ¼�����ļ���(new.png)
		 MatrixToImageWriter.writeToFile(bitMatrix, format, outputFile);  //����ά��д��ͼƬ�ļ�
		 */ 		
	}
}
