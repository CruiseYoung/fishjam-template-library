package com.fishjam.android.study;
import junit.framework.TestCase;

/***************************************************************************************************************************************
 * ѹ��ͼƬ������ͼ(��MediaStore�в���ͼƬ����)
 *   Uri uri = getContentResolver().insert(Media.EXTERNAL_CONTENT_URI, values); 
 *   Bitmap sourceBmp = BitmapFactory.decodeFile("�ļ�ȫ·��.jpg");             //���Bitmapʵ��
 *   OutputStream outStream = getContentResolver().openOutputStream(uri);  //��������
 *   sourceBmp.compress(Bitmap.CompressFormat.JPEG, 50, outStream);      //ѹ������� 
 *   outStream.close();
***************************************************************************************************************************************/

public class MultiMediaTester extends TestCase {

}
