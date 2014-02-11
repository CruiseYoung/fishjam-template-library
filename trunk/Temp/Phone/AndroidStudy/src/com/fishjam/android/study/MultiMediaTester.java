package com.fishjam.android.study;

import java.io.InputStream;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.test.ActivityTestCase;

/***************************************************************************************************************************************
 * ���ʵ��
 *   1.����ͼƬ
 *     BitmapFactory.decodeStream ����ͼƬ(��ʹ��Java���createBitmap����Լ�ڴ�)
 *     TODO: �÷���������ݻ����ĸ��ֱַ������Զ���Ӧ ��
 *   2.BitmapFactory.Options
 *     ��ȡͼƬʱ����ͼƬ��Config���������Ը���Ч���ټ��ص��ڴ�
 *   3.��ʾͼƬ -- setImageBitmap ?
***************************************************************************************************************************************/

/***************************************************************************************************************************************
 * �ֻ��ϵĵ��������İ�ť�����������
 *   1.�����ֻ��������������
 *   2.������Ϸ����������ֲ��ŵ������� ��Ҫ��ͨ�� Activity.setVolumeControlStream(AudioManager.STREAM_MUSIC) ����һ�£�
 *      ����ֻ����Ϸ���������ڲ��ŵ�ʱ����ܵ�����Ϸ��������
 * 
 * AudioManager -- (AudioManager)getSystemService(Context.AUDIO_SERVICE); //ͨ�������ȡ��Ƶ���������ɿ���������С��
 *   getStreamMaxVolume(AudioManager.STREAM_MUSIC); -- ��ȡ�������ֵ(TODO: ע���� 15��������100)
 *   setStreamVolume(AudioManager.STREAM_MUSIC, currentVol, AudioManager.FLAG_PLAY_SOUND); --  ���õ�ǰ��������С
 *   
 * Android �Գ���ý���ʽ�ṩ��֧�֣�
 *   ͼƬ��JPEG, GIF, PNG, BMP ��
 *   ��Ƶ��3GP, MP3, WAV ��
 *   ��Ƶ��3GP, MP4 ��
 *   
 * ��������Ƶ
 *   MediaPlayer -- ��̬�� create ��������ʵ����Ȼ���ͨ�� start/pause/stop �ȷ�������
 *       ȱ�㣺��Դռ�����ϸߡ��ӳ�ʱ��ϳ�����֧�ֶ����Ƶͬʱ����
 *       �ŵ㣺 
 *     setDataSrouce -- ������Ҫ�����ļ���·��
 *     setVolue -- �������������Ĵ�С
 *     prepare -- ��startǰ����
 *     OnCompletionListener -- ��ý�岥�����ʱ��ص��Ľӿڣ����� onCompletion ������ͨ�� setOnCompletionListener ����
 * JetPlayer -- ���Բ��űȽϳ�����Ϸ��������
 * VideoView + MediaController -- �ɺ����׵�ʵ����Ƶ������
 *   video.setMediaController �� ctrl.setMediaPlayer �໥������ Ȼ�� video.requestFocus ��ý���
 * SoundPool -- ֱ��newʵ���� new SoundPool(4, AudioManager.STREAM_MUSIC, 100);  //����4��������ͬʱ���ţ�����Ʒ��Ϊ100
 *   �ŵ㣺��������Ϸ��ͬʱ���Ŷ����Ч�� 
 *   ȱ�㣺ֻ�ܲ��Ž϶̵�����Ƭ��(���ֻ������1M���ڴ�ռ�)����Ƶ��ʽ����ʹ��OGG��Ŀǰֻ��16bit��WAV�ļ��нϺõ�֧��
 *   load -- ����������Դ������ID
 *   play() -- ����ID�����Ѿ����ص�������Դ
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
 *   SurfaceView -- ʵ���Ի����ͼ��������Կ��Ʊ���ĸ�ʽ���ײ��ṩ��GPU���٣����кܺõ����ܣ�
 *      �������� GLSurfaceView��VideoView ��. ͨ�����������������µĻ���(�� ������Ϸ��)
 *      ע�⣺SurfaceView������һ�����߳���ֱ�ӻ��ƣ���View������UI�����߳��л��ơ�
 *   SurfaceHolder -- ������ƽӿڣ�һ��ͨ�� SurfaceView.getHolder ��ã�������ƽ���ĳߴ硢��ʽ�����أ�����ؽ���ߴ�ĸı��
 *     Callback -- �ڲ��ص��ӿڣ��ɻ�ý���ı���Ϣ(surfaceCreated, surfaceChanged, surfaceDestroyed )��ͨ�� addCallback ����
 *     lockCanvas/unlockCanvasAndPost --����surface(���������ַ�Χ)�����ؿɻ�ͼ��Canvas��������ɺ����canvas���ȴ����̸߳���
 *       ע�⣺ unlockCanvasAndPost ��ֻ���ύ�˸����������߳̿�����δ���£���˻��Ƶ����ݻᱻ���ǡ�
 *                Ϊ��ʹ�ύ������Ч������ lockCanvas(new Rect(0, 0, 0, 0)) + unlockCanvasAndPost() һ�Ρ�
 *     setType() -- �������ͣ��� SurfaceHolder.SURFACE_TYPE_PUSH_BUFFERS
 *   Surface -- �����˵�ǰ���ڵ��������ݣ���һ�㲻ֱ�Ӻ�Surface�򽻵�������ͨ����Canvas�ϻ�ͼ���ı����е����� 
 *     
 * 2Dͼ�δ���  -- android.graphics, android.view.animation �Ȱ���
 *   ͼƬ: Drawable / Bitmap / BitmapFactory
 *     Bitmap
 *       createBitmap -- ���ݲ�������λͼ
 *       extractAlpha -- ����λͼ��Ϣ����ֻ��͸����û����ɫ(��Alphaͨ��)��λͼ
 *       isRecycle-- �ж�ͼƬ�ڴ��Ƿ��ѱ�����
 *       recycle -- ����ͼƬ��ռ���ڴ�
 *     Drawable -- ������࣬������Ա����Ƴ�����ĳ�����������ڶ���UI�����ж������: BitmapDrawable/ColorDrawable/ShapeDrawable/StateListDrawable ��
 *       ʹ�÷�ʽ:
 *         1.�����ڹ����е�ͼƬ�ļ�, �� ImageView.setImageResource(R.drawable.myImage); 
 *         2.XML�ļ�����Drawable����, �� <ImageView android:id=��@id/ImageView01" android:src="@drawable/myimage" ... />
 *           ע�⣺����ͨ�� XML ���� drawable ����, ʹ�÷�ʽ:  android:background="@drawable/bg_border" 
 *              bg_border.xml(͸������ɫ + ��ɫ�߿�) : <shape xmlns:android="http://xxx"><solid android:color="#0000"/><stroke android:width="4px" android:color="#f00" /></shape>
 *              button_selector.xml(��ť���¡�����ʱ�Ľ���) : <selector> <item state_pressed="true" drawable="xxx"/> <item state_pressed="false" drawable="xxx"/> </selector>
 *         3.ֱ�ӹ���ʵ����ȡ(SD) 
 *           a.Bitmap bmp = BitmapFactory.decodeFile("/sdcard/test.jpg"); 
 *             ���Ƶ��� decodeResource(getResources(), R.drawable.xxx);
 *                          decodeStream(context.getResources().openRawResource(xxx));
 *           b.Bitmap bmp2 = Bitmap.createBitmap(bmp, ...);  //����ԭʼλͼ��������ͼ  
 *     BitmapFactory -- Bitmap�Ĺ����࣬���� decodeFile/decodeResource/decodeStream �ȶ��ַ�ʽ������Bitmap
 *     
 *   9PitchͼƬ -- �����PNGͼƬ(��չ��Ϊ .9.png��ʹ�� draw9patch.bat �༭)��
 *     ��ԭʼͼƬ���ܸ����һ�����Ϊ1���ص���������4���߾�����ͼƬ�����š���ʾ����
 *     ������ʹ�ã�
 *       Bitmap bmp_9path = BitmapFactory.decodeResource(getResources(), R.drawable.9path);  
 *       NinePatch np = new NinePatch(bmp_9path, bmp_9path.getNinePatchChunk(), null); //�ڶ�����������.9.pngͼƬ�ļ��б���Ĵ������췽ʽ����Ϣ����
 *       np.draw(canvas, xxxx); 
 *     
 *   ���� -- Animation(���������࣬�ж������) 
 *       AnimationSet -- ���嶯�����Լ�����
 *       AnimationUtils -- ����������
 *       ViewPropertyAnimator -- 
 *     1.Tween -- ����ʹ��ͼ����ƶ����Ŵ���С����ת����͸���ȵȱ仯��
 *       ScaleAnimation/AlphaAnimation/RotateAnimation/TranslateAnimation
 *       ʹ�÷�ʽ:
 *         1.Ӳ���� -- ����Animation����ʵ�������úò�����ͨ�� ImageView.startAnimation �Ⱥ�������ָ������
 *         2.�����ļ�(�Ƽ�) -- res\anim\ Ŀ¼�����ö����ļ���Ȼ��ͨ�� AnimationUtils.loadAnimation ������ö���ʵ������ͨ������� startAnimation ��ʼ��
 *           <set> <alpha android:fromAlpha="0.1" toAlpha="1.0" duration="3000" /></set>�� ���Ƶ��� <scale>, <translate>, <rotate> ��
 *     2.Frame -- ֡������ͨ��˳�򲥷����кõ�ͼƬ��ʵ�֣����Ƶ�Ӱ��
 *       AnimationDrawable -- һ��ͨ��XML�����ļ����ã�<animation-list> <item/></animation-list>��Ȼ����Activity��xml�����п���ͨ�� android:background ָ����
 *     
 * �Ի�
 *   1.�� View/SurfaceView ����̳У�Ȼ��ѡ�����������·�����
 *      onDraw -- ʹ��Canvas���л��ƣ�ͨ�� invalidate �� postInvalidate �������
 *      onFinishInflate -- ��Ӧ�ô�XML�����ļ��������������������������֮�����
 *      onMeasure -- ���View�����������������������Ĵ�С����Ԫ��Ҫ���ø�Viewʱ���á�
 *        ����ʱ��ͨ�� MeasureSpec ��ľ�̬����getMode(AT_MOST ��ʾ���ɻ�õĿռ䣬��Ӧwrap_content; EXACTLY ��ʾ��ȷ�ĳߴ磬��Ӧfill_parent )
 *        ��getSize����������ı߽������Ȼ��ͨ�� setMeasuredDimension ָ����������
 *      onLayout -- �������Ҫ�������������λ�á���Сʱ�ص�
 *      onTrackballEvent -- �������켣���¼�ʱ����
 *      onTouchEvent -- �������������¼�ʱ������ TODO:��ô��������(Gesture?)
 *      onAttachedToWindow/onDetachedFromWindow -- ������� ���봰��/�Ӵ��ڷ��� ʱ����
 *   2.���Դ� GraphicsActivity �̳�
 *   
 *   Canvas -- ����
 *     save/restore -- �����ȡ��������״̬���ɶԳ��֡�ͨ�����ڶ�Canvas���� ��ת/����/ƽ�ƵȲ������������еĲ���Ԫ�ؽ���������ƣ�������Ϻ�ָ�Canvs
 *   Color/Paint(����)/Path/ShapeDrawable�������࣬�� OvalShape(��Բ)��RectShape(����)
 *   
 * ͼ����Ч
 *   Matrix -- 3x3������任����ͨ������ͼƬ����Ч����ȡ�
 *     ����ʵ��ͼ�ε� ƽ��(Translate)�� ��ת(Rotate)������(Scale)������(Skew, x��y���ֲ��䣬����Ӧ��y��x������ƽ��: �� ����=>���Σ����ڵ�����任 )�ȣ� (TODO: Persp ͸�ӱ任?)��
 *       ÿ�ֱ任�����ֲ�����ʽ�� set(ֱ������), pre(�ȳ�, ��������ҳ�)��post(���, ����������) -- ����ĳ˷������㽻���ɣ��ȳ˺�˱����ϸ����֡�
 *       ��ת/����/���� ����Χ��һ�����ĵ���У��粻ָ������Χ��(0,0)���б任��
 *       ˮƽ�Գƣ�float matrix_values[] = {1f, 0f, 0f, 0f, -1f, 0f, 0f, 0f, 1f}; matrix.setValues(matrix_values); 
 *       ��ֱ�Գƣ�float matrix_values[] = {-1f, 0f, 0f, 0f, 1f, 0f, 0f, 0f, 1f}; matrix.setValues(matrix_values);
 *       x=y��Գƣ� float matrix_values[] = {0f, -1f, 0f, -1f, 0f, 0f, 0f, 0f, 1f};
 *     invert() -- 
 *     setTranslate() -- 
 *   ColorMatrix -- ��ɫ����(RGBA)��������ͼƬ��Ч������ ��ͼƬ�����ڰ׵ģ���������Ƭ��Ч�� ��
 *   Shader -- ��ȾЧ������ͨ�� Paint.setShader(); �ȷ�ʽ��ʹ�á�
 *      ���кܶ����࣬�� BitmapShader(λͼ��Ⱦ)��ComposeShader(�����Ⱦ)��LinearGradient(������Ⱦ)��RadialGradient(������Ⱦ), SweepGradient(�ݶ���Ⱦ) �� 
 *   Xfermode -- ����ͼ����ʱ�ĸ���ģʽ��һ���÷�Ϊ paint.setXfermode(new PorterDuffXfermode(Mode.SRC_IN ��ģʽö��ֵ ));
 *   EdgeEffect/EdgeEffectCompat -- ��ԵЧ��
 *   
 * 3Dͼ�δ���(OpenGL ES) -- λ�� javax.microedition.khronos.opengles, android.opengl �Ȱ���
 *   
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

public class MultiMediaTester extends ActivityTestCase {
    public void testLoadBitmap(){
    	
    	InputStream is = getActivity(). getResources().openRawResource(R.drawable.ic_launcher);
        BitmapFactory.Options options=new BitmapFactory.Options();
        options.inJustDecodeBounds = false;
        
        //���ŵı����������Ǻ��Ѱ�׼���ı����������ŵģ���ֵ�������ŵı�����SDK�н�����ֵ��2��ָ��ֵ,ֵԽ��ᵼ��ͼƬ������
        options.inSampleSize = 4;   //width��hight��Ϊԭ�����ķ�һ��������ͼ��ԭ���� 16 ��֮һ
        
      //����ʡ�ڴ�ķ�ʽ��ȡ������Դ��ͼƬ
        options.inInputShareable = true;  
        options.inPurgeable = true;  
        options.inPreferredConfig = Bitmap.Config.RGB_565;
        
        Bitmap btp =BitmapFactory.decodeStream(is,null,options);
    }
}


