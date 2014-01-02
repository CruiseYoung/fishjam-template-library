package com.fishjam.android.study;
import android.view.SurfaceHolder;
import junit.framework.TestCase;

/***************************************************************************************************************************************
 * Android �Գ���ý���ʽ�ṩ��֧�֣�
 *   ͼƬ��JPEG, GIF, PNG, BMP ��
 *   ��Ƶ��3GP, MP3, WAV ��
 *   ��Ƶ��3GP, MP4 ��
 *   
 * ��������Ƶ
 *   MediaPlayer -- ��̬�� create ��������ʵ����Ȼ���ͨ�� start/pause/stop �ȷ�������
 *     setDataSrouce -- ������Ҫ�����ļ���·��
 *     prepare -- ��startǰ����
 *     OnCompletionListener -- ��ý�岥�����ʱ��ص��Ľӿڣ����� onCompletion ������ͨ�� setOnCompletionListener ����
 *   JetPlayer
 * VideoView + MediaController -- �ɺ����׵�ʵ����Ƶ������
 *   video.setMediaController �� ctrl.setMediaPlayer �໥������ Ȼ�� video.requestFocus ��ý���
 *   
 * ¼������Ƶ
 *   MediaRecorder 
 *     setAudioSource -- ������ƵԴ���� AudioSource.MIC
 *     setOutputFormat -- ���������ʽ���� OutputFormat.THREEE_GPP
 *     setAudioEncoder -- ������Ƶ���������� AudioEncoder.AMR_NB
 *     setOutputFile -- ��������ļ�·��
 *     
 * �ֻ�����
 *   Camera -- ����࣬ʵ�����չ���
 *     PictureCallback -- �ڲ��ӿڣ�������Ƭ׼���ú�Ļص�
 *       onPictureTaken() -- ��Ƭ�ص�����ͨ�� AsnycTask ��������첽����
 *     open() -- �����
 *     release() -- �ͷŷ��й���Դ, ������ SurfaceHolder.Callback.surfaceDestroyed �е��ã�ע���� stopPreview
 *     setPreviewDisplay(SurfaceHolder) -- ����Ԥ���������� SurfaceHolder.Callback.surfaceCreated �е���
 *     setParameters() -> startPreview() -- ���ò��������Ԥ���������� SurfaceHolder.Callback.surfaceChanged �е���
 *     takePicture() -- ��������
 *   SurfaceView -- ʵ����ƬԤ������ͼ���
 *   SurfaceHolder -- ������ƽӿڣ�һ��ͨ�� SurfaceView.getHolder ��ã�������ƽ���ĳߴ硢��ʽ�����أ�����ؽ���ߴ�ĸı��
 *     Callback -- �ڲ��ص��ӿڣ��ɻ�ý���ı���Ϣ(surfaceCreated, surfaceChanged, surfaceDestroyed )��ͨ�� addCallback ����
 *     setType() -- �������ͣ��� SurfaceHolder.SURFACE_TYPE_PUSH_BUFFERS
 *     
 * 2Dͼ�δ���  -- android.graphics, android.view.animation �Ȱ���
 *   Drawable --
 *   Bitmap / BitmapFactory 
 *   Tween / Frame -- ����
 *   Canvas
 *   Color
 *   Paint
 *   Path
 *   Matrix / Shader -- ͼ����Ч
 * 3Dͼ�δ���(OpenGL ES) -- λ�� javax.microedition.khronos.opengles, android.opengl �Ȱ���
 *   
 ***************************************************************************************************************************************/

/***************************************************************************************************************************************
 * ѹ��ͼƬ������ͼ(��MediaStore�в���ͼƬ����)
 *   Uri uri = getContentResolver().insert(Media.EXTERNAL_CONTENT_URI, values); 
 *   Bitmap sourceBmp = BitmapFactory.decodeFile("�ļ�ȫ·��.jpg");             //���Bitmapʵ��
 *   OutputStream outStream = getContentResolver().openOutputStream(uri);  //��������
 *   sourceBmp.compress(Bitmap.CompressFormat.JPEG, 50, outStream);      //ѹ������� 
 *   outStream.close();
***************************************************************************************************************************************/

public class MultiMediaTester extends TestCase {
	SurfaceHolder holder;
}
