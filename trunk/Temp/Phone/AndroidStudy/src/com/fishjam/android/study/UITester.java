package com.fishjam.android.study;
import java.security.PublicKey;

import android.R.integer;
import android.graphics.Bitmap;
import android.graphics.drawable.BitmapDrawable;
import android.test.AndroidTestCase;
import android.view.MotionEvent;
import android.view.View;
import android.widget.ImageView;

/***************************************************************************************************************************************
 * HVGA( Half-size VGA) -- VGA(640x480)��һ�룬�ֱ���Ϊ480x320, iPhone,��һ��gPhone���ֻ����ǡ�
 * WVGA() -- 800x480
 *
 * ���þ��뵥λ
 *    dip/dp(density-independent pixels) -- �ܶ��޹ص�����, ������Ļ�ܶȵĳ���λ�������������������Ԫ�ء�
 *      ����(px)=dip * (�ܶ� / 160) , �� �ܶ�Ϊ160dpi����Ļ�ϣ�1dip����1x; �ܶ�240dpi����Ļ��,1dip=1.5px
 *    in(Ӣ��) -- ������Ļ������ߴ�
 *    mm(����) -- 
 *    pt(��)-- 1/72Ӣ��
 *    px -- ���أ���Ӧ��Ļ�ϵ�һ����
 *    sp(scaled pixels) -- �������أ��;����޹ص����ء���Ҫ��������Ĵ�С�����Ը����û��������С��ѡ���������

 *
 * �ߴ磺��Ļ������ߴ磬ָ��Ļ�ĶԽ��߳��ȣ��� 2.8Ӣ�硢3.7Ӣ��
 * ��������Ļ�������Ⱥ������ȱȣ�16:9��16:10���ǿ�����4:3��Ϊխ��
 * �ֱ��ʣ���Ļ��ӵ�е����ص���������ʹ��"���x����"����ͨ��Ӧ�ó��򲢲�ֱ�Ӵ���ֱ��ʣ�
 * �ܶȣ�����Ļ�ֱ���Ϊ����������Ļ���������е����أ��ܶȽϵ͵���Ļ���ڳ�����ֻ�бȽ��ٵ����ء�
 * ����Ĥ(Retina)��ʾ����һ�ֳ������غ�Խ���ܶȵ�Һ�������������ǰ��iPhone��������4��������������
 *       ��960*640�ķֱ���ѹ������ǰ��iPhone��ͬ����ߴ�(3.5Ӣ��)����ʾ���ڡ���Ļ�ܶȴﵽ 326����/Ӣ��
 *  
 * ��ֱ��ʹ��Ľ������ -- ������Ҫ�������OS�汾(1.5��1.6������)��������Ļ�ֱ���
 *   ������Ļ��UI���ͣ�
 *     HTC Sense UI, SamSung TouchWiz UI, Motorola MOTO blur UI, Xiaomi MIUI, OMS(Ophone), LEOS(LePhone), Alibaba��OS ��
 *   A.1.5��ǰ��֧�ָ߷���������ͼƬ����ͬһ��drawableĿ¼��;
 *     1.6��ʼ֧�� ��(drawable-hdpi)����(drawable-mdpi)����(drawable-ldpi)�����ܶȡ�
 *     3.0�Ժ�֧�� ����(drawable-xhdpi)��
 *     Ϊ�˵���? ֧�� (drawable-xxhdpi)
 * 
 *  ������ƽ��飺
 *    1.�����ܶ�(mdpi)��������Ļ(HVGA, 320x480)Ϊ��ƻ�׼���н��沼��(��ʱ dip��pxΪ1:1���ܸ�������ռ䲢����ʹ��)��
 *    2.�������Ϊ���ֳߴ����Ļ�ṩ��ͬ����Դ(��Ҫ�ǲ���),Ҳ����Ϊ�����ܶȵ���Ļ�ṩ��ͬ����Դ(��Ҫ��λͼ);
 *    3.ʹ�����ܶ��޹ص�����(dip)������Ӧ�ó���Ľ��沼�֣�������px -- �����ɱ�֤Ӧ�ó���Ľ������ڸ��ֱַ��ʵ���Ļ�϶�����������ʾ��
 *    4.ִ��ʱ��ƽ̨�������Ļ����ĳߴ����ܶ����ԣ��Զ������Ӧ����Դ���������Ǵ����ܶ��޹ص�����ת������Ļ�ϵ���������
 *    5.�ڴ�����Ļ����仯ʱ��Ϊ���Ż������ǵ����ظı䷽��
 *    6.��ѡ���д����Ե����Խ��������Ԥ��Ч��
 *    7.����ť�ķŴ���Сʱ��������ʹ�þŹ���ͼƬ�������ǵ�����ͼ
***************************************************************************************************************************************/

/***************************************************************************************************************************************
 * ���(Style)
 *   �Զ���Style(ָ����ɫ����С��)
 *     1.��дstyle.xml�ļ�,����valuesĿ¼��
 *       <?xml version="1.0" encoding="utf-8"?>
 *       <resources>
 *         <style name="DavidStyleText1" [parent=����Style����]>
 *           <item name="android:textSize">18sp</item>
 *           <item name="android:textColor">#EC9237</item>
 *         </style>    ����ʹ�� <style></style> ������,
 *       </resources>
 *     2.main.xml �� <TextView style="@style/DavidStyleText1" ...>
 *     
 * ����(Theme)
 * 
 * view  -- ����UI�඼������View��ViewGroup������Ļ���֮�ϣ��������(Composite)���ģʽ����Ҫͨ�� Activity.setContentView ������ʾ
 *   View --�����ؼ�,  �������Ϊ Widget
 *     Button( setOnClickListener(new Button.OnClickListener(){public void onClick(View v) {...}}); )
 *     EditText
 *     TextView
 *     RelativeLayout
 *     lists,grids,text boxes,buttons,
 *   ViewGroup -- ���������ĳ����࣬�����ΪLayout(���ֹ�����)��������������ķֲ�������LayoutParams,MarginLayoutParams 
 *     �����ڲ��࣬���ṩ��һЩXML����(�� layout_heidth/layout_width)��ViewGroup������ͨ��ָ����Щ���������в��֣�
 *   
 * �Ի�
 *   ������: Canvas + Bitmap + Paint 
 *   ��View������������ onDraw(Canvas canvas) ��������������ʹ�� ���л��ơ�
 *   
 * ����ʶ�� -- �� ����ͼƬ����������ҳ���� android.gesture ���л��ṩ�˺ܶศ���࣬
 *   GestureDetector(v1)/GestureDetectorCompat(sv4���Ƽ�������ʹ�����Action���䵽API4)���ɼ򵥵Ľ� View.onTouchEvent �¼�ת����ȥ����
 *     OnDoubleTapListener -- 
 *   +-ScaleGestureDetector -- ���������ָ����Ļ�����ŵ����ƣ�
 *       http://www.cnblogs.com/over140/archive/2010/12/08/1899839.html
 *       OnScaleGestureListener -- �û��ṩ�Ļص��ӿڣ�һ���ڸ����ص������е��� Detector �ķ������д���
 *         onScaleBegin(����true������ʼ����) -> ѭ��onScale -> onScaleEnd
 *       getScaleFactor() -- �������ű�����ע�⣺onScale ���������true�����factor�Ļ���, ����false���Ծ�ֵ����factor.
 *   ʵ������ʶ��ķ�����
 *     1.ʵ�� OnGestureListener �ӿڣ�ʵ�����з��� 
 *       onFling(�����ƶ����ɿ�)
 *       onLongPress(������δ�ɿ�)
 *       onScroll(���� -- ��������?), 
 *       onShowPress(���µ���������֮ǰ)
 *       onSingleTapUp(��ָ�뿪������)
 *     2.�̳����е� SimpleOnGestureListener �ֻ࣬��������Ҫ�Ĳ��ַ�������
 *    
 * ������Ӧ
***************************************************************************************************************************************/

/***************************************************************************************************************************************
 * View�¼�
 *     onTouchEvent() -- �������������¼�ʱ�Ļص���
 *           int action = MotionEventCompat.getActionMasked(event);  Ȼ�� switch(action) {...}
 *       �� switch(event.getAction() & MotionEvent.ACTION_MASK) ���жϰ���״̬(down,move,up ��), 
 *     getX(������������Ͻǵ�����)/getRawX(�������Ļ���Ͻǵ�����) �ȷ�����ȡλ�á�
 *       ���㣺ACTION_DOWN -> ACTION_MOVE -> ACTION_UP
 *       ��㣺ACTION_DOWN -> ACTION_POINTER_DOWN(getPointerCount == 2) -> ACTION_MOVE -> ACTION_POINTER_UP -> ACTION_UP
 * View����
 *   tag -- Ϊ�������I���ַ������͵�tagֵ��Ȼ���ͨ�� getTag()��ȡ��ֵ ��ͨ�� findViewWithTag �ҵ������
 *   
 * View --
 * +-AbsListView --  �б���ͼ�ĳ�����࣬�� GridView��ListView ������
 *    choiceMode -- ѡ����Ϊ���� none(����ʾ�κ�ѡ����), singleChoice(��ѡ), multipleChoice(��ѡ)
 *    divider -- �����б���ķָ���(��������ɫ��Ҳ����Drawable)
 *    entries -- ָ��һ��������Դ�������ݸ�������Դ������ListView, ��ָ�� "@array/bools"
 *    fastScrollEnabled -- �����Ƿ�������ٹ�������Ϊtrue�������ʾ����ͼ�꣬�������û��϶��ù���ͼ����п��ٹ���
 *    scrollingCache -- ����Ϊtrue���ڹ���ʱ����ʹ�û��ƻ���
 *    textFilterEnabled -- �����Ƿ���б�����й��ˣ���Ҫ��Ӧ��Adapterʵ����Filter�ӿ�ʱ��������
 * +-AdapterView -- ������࣬���԰������"�б���"�������Ժ��ʵ���ʽ��ʾ����������ʾ���б����� Adapter �ṩ(ͨ�� setAdapter ��������).
 *     AdapterView �ĸ����ฺ����ú��ʵķ�ʽ��ʾAdapter�ṩ��ÿ��"�б���"�����
 *     setOnItemClickListener()/setOnItemSelectedListener() -- ��ӵ�����ѡ���¼�
 * +-AdapterViewFlipper -- һ�����ڻõƲ��š�һ����ʾһ��Adapter�ṩ��View���������ͨ�� showPrevious()/showNext() ������������ʾ��һ������һ����
 *    inAnimation/outAnimation -- ���� ��ʾ/���� ʱʹ�õĶ���
 *    flipInterval -- �����Զ����ŵ�ʱ����
 * +-AnalogClock -- ģ��ʱ�ӣ���ʾ Сʱ������
 *    dial -- ���ñ���ʹ�õ�ͼƬ
 *    hand_hour/hand_minute -- ����ʱ�롢����ʹ�õ�ͼƬ
 * +-AutoCompleteTextView -- �Զ�����ı���ͨ��������Ҫ��ʾ��Դ��������(setAdapter)��ʵ��
 *    completionHint/completionHintView -- �����˵��е� ��ʾ���� / ��ʾ������ͼ 
 *    completionThreshold -- �û��������뼸���ַ��Ż���ʾ��ʾ
 * +-BaseAdapter -- 
 * +-Button
 *     text
 * +-CheckBox -- ��ѡ��ť
 * +-Chronmeter -- ��ʱ������ʾ��ĳ����ʼʱ�俪ʼ��һ����ȥ�˶೤ʱ��
 *     format -- ָ����ʱ���ļ�ʱ��ʽ��
 *     setBase(long) -- ���ü�ʱ������ʼʱ�䣬��λΪ���롣�� SystemClock.elapsedRealtime()
 * +-DigitalClock -- ����ʱ�ӣ�������ʾ��ǰ������
 * +-EditText -- �༭��������
 *     hint -- û����������ʱ����ʾ��Ϣ
 *     inputType -- ָ���������������(���� HTML �� <input> ��type����)���� date(����), number(��ֵ), numberPassword(ֻ������������), ��
 *     numeric -- ���ù�������ֵ���뷨���� integer(����), signed(����������ŵ���ֵ)�� decimal(��������С�������ֵ)
 *     password -- true
 *     textSize -- 18sp
 * +-ExpandableListView -- ��չ�����б����(�������ؼ�?)�����б����Ϊ���飬ÿ����ɰ�������б���������� ExpandableListAdapter �ṩ��
 * +-ExtractEditText -- ��EditText����ĵײ�����࣬�����ṩȫ�����뷨֧��
 * +-Gallery(ͼ��) -- ��ˮƽ������ʾ�����ݣ������û������϶����л��б��һ��� ImageSwitcher һ���������ͼƬ����ѡ�е���λ���м䡣
 *      ͨ��ʹ�� Gallery ��ʾ����ͼ���ڵ��ʱͨ�� ImageSwitcher::setImageResource �л�����ͼ
 *      TODO: �Ѳ����Ƽ��ÿؼ����Ƽ�ʹ�� HorizontalScrollView + ViewPager ������Gallery��
 *      animationDuration -- ����ͼƬ�л�ʱ�Ķ�������ʱ��
 *      unselectedAlpha -- ����û��ѡ�е�ͼƬ��͸���ȣ��� "0.6"
 *      ImageSwitcher::setOutAnimation(AnimationUtils.loadAnimation(this, android.R.anim.fade_out)); // ���ö���Ч��
 * +-Gesture -- ����ʶ�� 
 * +-GridView -- �������еķ�ʽ��ʾ���ݣ�һ���ʺ���ʾͼƬ)
 *     horizontalSpacing/verticalSpacing -- ��Ԫ��֮��� ˮƽ/��ֱ ���
 *     numColumns -- ����������Ĭ��Ϊ1��ע�⣺�����Ǹ���Adapter��̬�ı��
 *     stretchMode -- ��������ģʽ���� NO_STRETCH(������)�� STRETCH_COLUMN_WIDTH(����Ԫ�ر��Ԫ�ر���)
 * +-ImageButton -- ͼƬ��ť(ImageView�����࣬����text��Ч)
 *     src -- "@drawable/iconempty"
 *     setImageResource(R.drawable.iconempty);
 * +-ImageView -- ��ʾͼƬ��Drawable����
 *     adjustViewBounds -- �����Ƿ�����Լ��ı߽�����������ʾ��ͼƬ�ĳ����
 *     cropToPadding -- �����Ƿ�ü�������padding
 *     scaleType -- ������ʾ��ͼƬ������Ż��ƶ�����ӦImageView�Ĵ�С��
 *       center(ͼƬ�����м䣬����������), centerCrop(�����ݺ�����ţ�ʹ��ͼƬ����ȫ����ImageView), centerInside(�����ݺ�����ţ�ʹ������ȫ��ʾͼƬ)
 *       fitCenter(�����ݺ�����ţ��������ź��ͼƬ��������) , fitXY(��������������ţ�ͼƬ��ȫ��Ӧ��ImageView)��
 *       matrix(ʹ��ͼ�����ʽ����),
 *     setOnTouchListener -- ����Touch�¼������� 
 *     ������ʾ������
 *       setImageDrawable(getResources().getDrawable(R.drawable.right));   //������ʾ��ͼƬ, ��ͨ�� src ��������
 *       setImageResource(xxx); setImageBitmap(); setImageURI()
 * +-ListView/ListActivity -- �б���ͼ���Դ�ֱ�б�ķ�ʽ�г���Ҫ��ʾ���б���(����ϵ��������ϵͳ�������)
 *     �ؼ��㣺����Adapter�� �� ListActivty ��Ҫʹ���Զ���Ľ��沼���ļ��������б�����һ��idΪ "@+id/android:list" ��ListView��
 * +-MapView -- ��ʾGoogle��ͼ
 * +-MultiAutoCompleteTextView -- ������������ʾ��(�����ʾ���Էָ����ָ�)���Զ���ʾ��
 *    setTokenizer -- ���÷ָ�������  myAuto.setTokenizer(new MultiAutoCompleteTextView.CommaTokenizer());
 * +-ProgressBar -- ���������кܶ���:
 *     1.ProgressDialog(�Ի��������)�� ����Activity��onCreateDialog�������������д��������������أ�����showDialog������ʾ;
 *     2.ProgressBarIndeterminate(������������)������ Activity.requestWindowFeature(Window.FEATURE_PROGRESS) �������ô����н�����������;
 *       ���� Activity.setProgressBarVisibility(������)/setProgressBarIndeterminateVisibility(��������) ��ʾ�������� Activity.setProgress ���ý���
 *     3.ProgressBar(��ͨ������) -- �����ļ�������ProgressBar����ȡ����������� incrementProgressBy �ȷ������ý���
 *       style -- ָ�����, �� : @android:style/Widget.ProgressBar.Inverse -- ��ͨ��С�Ļ��ν�����
 *       indeterminate/indeterminateDuration -- ����Ϊtrue����ʾ����ȷ��ʾ ���� /���ȵĳ���ʱ��
 *       progressDrawable -- ���ý������Ĺ����Ӧ��Drawable����(Ϊ LayerDrawable ����, ͨ�� <layer-list> Ԫ�ؽ�������)
 *       
 * +-QuickContactBadge -- ��ʾ�������ض���ϵ�˵�ͼƬ������ͼƬʱ�����Ӧ��ϵ�˵���ϵ��ʽ����
 *     assignContactFromEmail()/assignContactPhone()/assignContactUri() 
 * +-RadioButton(��ѡ��ť) + RadioGroup(����һ��RadioButton) 
 *     checkedButton -- "@+id/sex1"
 *     orientation -- vertical,horizontal
 * +-RatingBar -- 
 * +-SeekBar -- �϶���
 *     thumb -- ָ��һ��Drawable���󣬿��Զ��廬�����ۣ���Ӧ OnSeekBarChangeListener 
 * +-StackView -- �Զ�ջ��ʽ����ʾ����б���
 * +-Spinner -- �����б�
 *     entries -- ʹ��������Դ���ø������б����б���Ŀ
 * +-Switch -- ����
 *     switchTextAppearance -- �ÿ���ͼ���ϵ��ı���ʽ
 *     thumb -- ָ��ʹ���Զ���Drawable���Ƹÿ��صĿ��ذ�ť
 *     track -- ָ��ʹ���Զ���Drawable���Ƹÿ��صĿ��ع��
 * +-TabHost / TabActivity -- ѡ�
 * +-TextView -- �ı���ͼ����ʾ�ַ���
 *     autoLink -- all(������ʾ���ӣ��� http://)�� �� "email|phone" -- ���ʼ����绰��������
 *     editable -- �����Ƿ�����༭
 *     ellipsize -- ���õ���ʾ���ı���������ʱ��δ����ı����ݣ��� end(���ı���β���ض���ʾʡ�Ժ�), marquee(����������ʾ�ı�)
 *     text -- @string/str_id
 *     textColor -- @drawable/darkgray
 *    +-CheckedTextView -- ������checked״̬
 * +-ToggleButton -- ״̬���ذ�ť��ӵ�� checked ����
 *     textOn/textOff -- ����ť��״̬ ��/�ر� ʱ��ʾ���ı�
 * +-VideoView -- 
 * +-WebView -- ����������ؼ�����ֱ�Ӽ�����ҳ��Ϊ��Ӧ�����ӹ��ܣ����� setWebViewClient ���������Զ���� WebViewClient ����ʵ��
 *     getSettings().setJavaScriptEnabled(true) -- ��������
 *     loadUrl -- ����ָ����URL��ַ��ҳ
 * +-Window
 *     setFormat -- ���ô����������� PixelFormat.TRANSLUCENT(��͸��)�� ��Ҫ�� setContentView ֮ǰ����
 * +-ZoomButton -- �Ŵ�/��С ��ť��Androidϵͳ�ṩ�� @android:drawable/btn_minus,btn_plus ������Դ��ֻ��ָ���� src ���Լ���
 * +-ZoomControls -- ͬʱ����� �Ŵ���С ������ť�����ɷֱ�󶨲�ͬ���¼�������
***************************************************************************************************************************************/

/***************************************************************************************************************************************
 * Layout -- Android ͨ�� LayoutInflater/MenuInflater  ���ཫ XML ��ʽ�Ĳ����ļ��е��������Ϊ���ӻ�����ͼ��������������ƽ̨�������������λ�á���С
 *   �����ļ��е� <requestFocus/> �����ʲô��˼?
 * ViewGroup -- ���⸸��
 *   gravity -- ���Ʋ��ֹ�����������Ķ��뷽ʽ���� top, bottom, left, right, center_vertical �ȣ�������Կ�ͨ�� "|" ���
 *   layout_width/layout_height -- fill_parent(��ȥ���հ׺��ͬ�ڸ�����),match_parent(�ȼ���fill_parent?�Ƽ�) wrap_content(��������ָ���߿�),320px,80dip
 *   +-AbsoluteLayout(TODO:�ѷ��� ) -- ����λ�ö�λ�����ͼ����ԣ�ά���ɱ���
 *       layout_x, layout_y -- 30px�� ָ��������� X��Y ����
 *   +-FrameLayout -- ֡���֣��������Ļ�����Ͻǿ�ʼ���֣�������������򣬺�����������ǰ��������
 *                             Ϊÿ���������е��������һ���հ׵�����(��Ϊһ֡),ÿ�������ռ��һ֡����Щ֡�����gravity�����Զ����롣
 *                             ��ͨ�����ö�� layout_gravity="center" ���𽥼�С����Ԫ�شﵽ�޺�Ƶ�Ч��(��ͬ��Բ��)
 *         foreground -- ���ø�֡����������ǰ��ͼ��(Drawable����)
 *         foregroundGravity -- �������ǰ��ͼ���gravity����
 *   +-GridLayout -- 4.0���������񲼾�,����������Ϊ rowsXcolumns ������ÿ��������Է�ֹһ�����
 *       columnCount/rowCount -- ��������� ���� ������
 *       layout_column/layout_row -- ���ø��������GridLayout�ĵڼ��С��ڼ���
 *       layout_columnSpan/layout_rowSpan -- ���ø��������GridLayout ���������� �缸�С�����
 *   +-LinearLayout -- ���Ͳ��֣����մ�ֱ����ˮƽ���򲼾������ע�⣺ˮƽ����ʱ���ỻ����ʾ����������
 *       background -- "@drawable/testpic",
 *       divider -- ��ֱ����ʱ������ť֮��ķָ���(Drawable����)
 *       layout_gravity -- ���ø���Ԫ���ڸ������еĶ��뷽ʽ���� center_horizontal
 *       orientation -- ���з�ʽ, ��   vertical(��ֱ���У�Ĭ��ֵ),horizontal
 *       layout_weight -- Ȩ��
 *       layout_alignParentLeft/layout_alignParentTop/ -- true �� false
 *       layout_marginRight -- 80dip, 
 *       layout_below/layout_alignTop -- "@+id/ExitButton", �������λ�õ�Ԫ��ID
 *       layout_centerHorizontal -- true
 *       measureWithLargestChild -- ����Ϊtrueʱ�����д�Ȩ�ص���Ԫ�ض�����������Ԫ�ص���С�ߴ�
 *     +-TableLayout  -- ��񲼾֣��̳���LinearLayout���������з�ʽ�������( ���� > TableRow > Ԫ�� �ķ�ʽ)��
 *         ��ֱ�ӽ�Ԫ�ط���TableLayout�����Ԫ�ؽ�ռ��һ��
 *           collapseColumns -- ������Ҫ�����ص��е�����ţ���������֮���ö��Ÿ���
 *           shrinkColumns -- ���������������е�����ţ���������֮���ö��Ÿ���
 *           stretchColumns -- ��������������е�����ţ���������֮���ö��Ÿ������ɱ�֤�������ȫ����������ռ�
 *   +-RelativeLayout -- ��Բ��֣������������Ĳ��ַ�ʽ(�������������ҵ�)�����������Զ���Ӧ���������ͼ����Ρ�
 *        layout_below, layout_toLeftOf
 *        ignoreGravity -- ָ���ĸ��������gravity���Ե�Ӱ��
 *   +-ScrollView 
 *   +-TableRow
 *
 * LayoutInflater infater = (LayoutInflater)getSystemService(Context.LAYOUT_INFLATER_SERVICE); //��LayoutInflater.from(getApplicationContext())
 * convertView = infater.inflate(R.layout.baseadapter_provider,null);
***************************************************************************************************************************************/

/**************************************************************************************************************************************
 * �Ի���(Dialog)
 *   AlertDialog(����Ի���)
 *     ʾ��: 
 *       AlertDialog.Builder dlgBuilder = new AlertDialog.Builder(this);  //����Builder��Ȼ������������ø��ֲ���
 *       dlgBuilder.setMessage(msg).setCancelable(false).setPositiveButton("ȷ��", new DialogInterface.OnClickListener() { ...onClick(...){finish();} }); //����builder
 *       AlertDialog alert = dlgBuilder.create();  //�����Ի���ʵ��
 *       alert.show(); //��ʾ
 *  ColorPickerDialog -- ѡ����ɫ�Ի���
 *  DatePickerDialog/TimePickerDialog(����/ʱ�� ѡ��Ի���) --  
 *    1.���� Activity.onCreateDialog(int id) { switch(id) {...}  } ���������Ի���;
 *    2.�� OnDateSetListener �� OnTimeSetListener �Ķ�Ӧ�¼���������Ӧ���ں�ʱ�������;
 *    3.���� Activity.showDialog ��ʾ�Ի���
 *  
 * Toast -- ��ʾ��Ϣ(û�н������ܣ�ֻ����ʾ), ��̬��makeText������ʾ�ı���ʱ��
 *    Toast.makeText(getApplicationContext(), "��ʾ��Ϣ", Toast.LENGTH_LONG).show;
**************************************************************************************************************************************/

/***************************************************************************************************************************************
 * Activity  :  ��ʾ�û������е�һ����Ļ����ʾ�ɼ���Views�ؼ���ɵ��û��ӿڣ������¼�������Ӧ��������xml�ļ��ж�������Ҫ��Views���ڴ����ж��壬
 *     Context <- ContextWrapper <- ContextThemeWrapper <- Activity
 *     ע�⣺Activity�������嵥�ļ�����������ʹ��
 *   ÿ��Activity��״̬��������Activityջ�е�λ�������������е�ǰ�������е�Activity����ѭ����ȳ���ԭ��
 *   ��һ���µ�Activity��������ǰ��Activity��������ջ�Ķ���������û�ʹ��Back��ť������ǰ̨Activity���رգ���һ��Activity�����������������ջ�Ķ�����
 *   Activity״̬ -- ״̬���ɾ��в�ȷ���Բ�����Runtime��ȫ����
 *     Active״̬����ʱ��Activity����ջ�������ǿɼ��ģ��н���ģ��ܹ������û�����ǰ��Activity��
 *     Paused״̬��Activity�ǿɼ��ģ���û�н��㡣���磬�����һ��͸�����ȫ��Ļ�ϵ�Activity�����Activity���档
 *     Stopped״̬����ȫ����ס�����ɼ���ʱ���������������ڴ��б��ֵ�ǰ������״̬�ͳ�Ա��Ϣ�������Ǳ����ն������Ҫ��ѡ��
 *     Inactive״̬��Activity��ɱ���Ժ���߱�������ǰ���ѱ���Activity��ջ���Ƴ�����Ҫ���������ſ�����ʾ��ʹ�á�
 *   ���Activity��״̬�仯
 *     onCreate -- ��Activity�������ڿ�ʼʱ�����ã���ʼ���û����棬��������������������ݿؼ���������������߳� -- ͨ����ҪfindViewById����Ա�����Ϳؼ�����������
 *     onRestore -- onCreate��ɺ󱻵��ã������ظ�UI״̬
 *     onRestart -- ��activity��ֹͣ״̬��������ʱ����(onStop->onRestart->onStart)
 *     onStart -- ��activity���û������ɼ���ʱ����ã���ע������û�����Intent������
 *     onResume -- ��activity��Ҫ���û�����ʱ���ô˷�������ʱactivity��activityջ��ջ�����û������Ѿ����Դ��ݸ��� 
 *     onSave -- Activity�����Ƴ�ջ������UI״̬ʱ���ô˷���
 *     onPause -- ��ϵͳҪ����һ��������activityʱ���ã���������������ύ��Щ�־����ݵĸı䡢ֹͣ������������ռ�� CPU��Դ�Ķ�����
 *     onStop -- ������һ��activity�ָ����ڸ�ס��activity,��������û����ٿɼ�ʱ���á�һ����activity����������activity���л���ǰ������ǰactivity������ʱ���ᷢ�����ֳ�������ע�������û�����Intent������
 *     onDestroy -- ��activity������ǰ�����õ����һ���������п�����ĳЩ����£�һ��Activity����ֹʱ��������onDestroy������
 *   ����������ִ��˳��:
 *     ��������:    onCreate -> onStart -> onResume
 *     �����˳�:    onPause  -> onStop -> onDestroy
 *     ��̨��ǰ̨: onRestart ->onStart -> onResume
 *   ��������
 *     ConfigChanges -- ����ʹActivity��׽�豸״̬�仯, �� orientation(�豸��ת) ��
 *   ��������
 *     findViewById() -- ����ID���������ʵ��
 *     finish() -- ����Activity, ͨ���÷�Ϊ MyActivity.this.finish();
 *     getWindow() -- ���ظ�Activity�������Ĵ��ڣ��粻���� setContentView �����øô�����ʾ�����ݣ�����ʾΪһ���մ��� 
 *     setContentView -- ͨ�������ص� onCreate �е���������Ҫ��ʾ����ͼ���Ӷ�ʵ�����û������Ĺ���
 *     setThemem -- ���ô��ڷ��(�� ����ʾActionBar���ԶԻ�����ʽ��ʾ���ڵ�)
 *     startActivity -- ��������������һ��Activity����������װ��Intent��
 *       Intent intent = new Intent(FirstActivity.this, SecondActivity.class);  ͨ��Bundle���ò���;  startActivity(intent);  //��FirstActivity������SecondActivity
 *       �� SecondActivity::onCreate ��: Intent intent = getIntent(); Bundle bundle = intent.getExtras(); ...
 *     startActivityForResult -- �Եȴ����ؽ���ķ�ʽ��������������һ��Activity�� ��Ҫ���� onActivityResult �����ص� Intent �Ƚ����
 *                                       ��SecondActivity��ͨ�� SecondActivity.this.setResult(0, intent); ���÷���ֵ�� Ȼ�� finish() ����
 *   ��������
 *     ListActivity, MapActivity ��
 **************************************************************************************************************************************/

/**************************************************************************************************************************************
 * ��ť����
 *   1. implements OnClickListener
 *      Button ResolutionTest = (Button)findViewById(R.id.ResolutionTest); 
 *      ResolutionTest.setOnClickListener(this);  //������ onClick�бȽ� getId���ܴ�������ť�ĵ�����
 *      public void onClick(View arg0) { 
 *          if (arg0.getId() == R.id.ResolutionTest)  { startTest(); } 
 *      } 
 *   2. ʹ�ó�Ա����
 *       ResolutionTest.setOnClickListener(m_myButtonClick);
 *       Button.OnClickListener m_myButtonClick =  new Button.OnClickListener() {...}
 *   3. ʹ�������¼�
 *       ResolutionTest.setOnClickListener(new Button.OnClickListener() {
 *           //@Override -- �Ƿ���Ҫ����
 *           public void onClick(View v) {
 *              startTest();  //���ñȽ�getId,ÿ��Button���������Լ���OnClick
 *           }
 *       });
**************************************************************************************************************************************/

/**************************************************************************************************************************************
 * 1. ����TitleBar �� Status Bar -- TODO����Ҫ��setContentView(R.layout.start_test)֮ǰ���ã���������force close ����
 *    �ڵ�ǰActivity��JavaԴ�ļ�����android.view.Window��android.view.WindowManager������oncreate()�����е���:
 *    requestWindowFeature(Window.FEATURE_NO_TITLE);   //Hide title bar;
 *    getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);  //Hide status bar;
 *    //getWindow().setBackgroundDrawableResource(R.color.red_bg); //���ñ���ɫ
 * 2. ͨ����Դ�ж����id�ҵ���Ӧ�Ķ��� -- findViewById
 *    TextView ResultText = (TextView)findViewById(R.id.ResultText);
 * 3. ��ʾͼƬ
 *    
**************************************************************************************************************************************/

public class UITester extends AndroidTestCase {
    public UITester()    {
        
    }
    protected void setUp() throws Exception {
        super.setUp();
    }

    protected void tearDown() throws Exception {
        super.tearDown();
    }
    
    //�����л� -- ��ʲô����?��׼Ӧ����ʹ�õڶ��ַ�������Ȼ�鷳����������ͣ�
    //  1.�л�layout -- ���岻ͬ��layout xml�ļ�,ͨ�� setContentView���� �����л�; ����ͬһ��Activity�У���Ա�������䡣
    //  2.�л�Activity -- ���岻ͬ��layout xml��ʵ�ֲ�ͬ��Activity����(��ʾ��Ӧ��layout),ͨ�� Intent�� �����л���
    //  ע����Ҫ��AndroidManifest.xml�ж����µ�activity����������ô���ԣ������޷����롣
    public void TestSwitchUI() {
        /*************************************************************************************************
         * Intent intent = new Intent();// newһ��Intent����
         * intent.setClass(EX03_09.this, EX03_09_1.class); //ָ��Ҫ������class
         * startActivity(intent);// ����һ���µ�Activity -- ���Ʒ�ģ̬��
         * //startActivityForResult(intent,0); // ������ActivityҪ����ֵ������,
         * �����µ�Activity���ȴ�������� // ��ͨ������ onActivityResult �����жϷ���ֵ(resultCode ==
         * RESULT_OK), ����ͨ�� data.getExtras ��ȡ���ݣ� ���в�ι�ϵ // ��Activity��ͨ�����·�ʽ���ؽ��:
         * // EX03_09_1.this.setResult(RESULT_OK, intent); //
         * ����result����һ��activity // EX03_09_1.this.finish();// �������activity
         * EX03_09.this.finish(); // �ر�ԭ����Activity(���ݾ������)
         **************************************************************************************************/
    }
    
    public void TabHostTester(){
        /********************************************************************************************
         * ����1:
         *   1. �ڲ����ļ���ʹ�� FrameLayout �г�Tab�����Tab�е��������
         *   2. Activity Ҫ�̳� TabActivity;
         *   3.���� TabActivity::getTabHost ��� TabHost ����Ȼ�󴴽� Tab ѡ��ҳ
         * ����2:
         *   1.ʵ�ֽӿ� TabHost.TabContentFactory �� createTabContent ������ָ��
         *     public class MyActivity extends TabActivity implements TabHost.TabContentFactory { 
         *          onCreate ��:  getTabHost().addTab(getTabHost().newTabSpec("all").setContent(this);
         *          public View createTabContent(String tag) { ...; if(tag.equals("all")) {  ����allҳ���Ӧ������  }  } 
         ********************************************************************************************/
        
        /*******************************************************************************************
        //���벿��
            TabHost tabHost = getTabHost();
            LayoutInflater.from(this).infalte(R.layout.main, tabHost.getTabContentView(), true);
            tabHost.addTab(tabHost.newTabSpec("all").setIndicator("����ͨ����¼").setContent(R.id.TextViewAll));
            tabHost.addTab(tabHost.newTabSpec("ok").setIndicator("�ѽ�����").setContent(R.id.TextViewReceived));
            tabHost.addTab(tabHost.newTabSpec("cancel").setIndicator("δ������").setContent(R.id.TextViewUnreceived));

         //���ֲ���
           <FrameLayout ....>
                 <TabHost>
                   <TextView android:id="@+id/TextViewAll" android:layout_width="wrap_content" android.text="����ͨ����¼">
                   </TextView>
                 </TabHost>
           </FrameLayout>
         *******************************************************************************************/
    }
    
    public void testTouchImage(){
    	//setOnTouchListener -- ��ͼƬ��ͨ����������λ�á���С��(�������� ���ҿ��ܻ����������⣿)
    	/*
    	public boolean onTouchEvent(View view, MotionEvent event){
	    	ImageView image1 = new ImageView();
	    	BitmapDrawable bitmapDrawable = (BitmapDrawable)image1.getDrawable();
	    	Bitmap bitmap = bitmapDrawable.getBitmap();  //��ȡλͼ
	    	double scale  = bitmap.getWidth() / 320.0; // ������ű���
	    	//����Ҫ��ʾ��ͼƬ�Ŀ�ʼ�㣨ע�����ӷ�Χ��飩
	    	int x = (int)(event.getX() * scale);
	    	int y = (int)(event.getY() * scale);
	    	image2.setImageBitmap(Bitmap.createBitmap(bitmap, x, y, 120, 120));  //��ָ����ʼλ�ô���Ҫ��ʾ����λͼ������ʾ
	    	return false;
    	}
    	*/
    }
    
    public void GridViewDisplayImageTester(){
        //ʹ�� BaseAdapter �࣬��д���еķ���( �� getView ������ͼƬ��ʾ��ʽ )
        /********************************************************************************************
        //�Զ���� BaseAdapter �࣬������Ҫ���ز�ͼƬ��������Ϣ
            class MyAdapter extends BaseAdapter{
                private Integer [] imgs = {
                        R.drawable.ic_launcher
                };
                Context context;
                MyAdapter(Context context){
                    this.context = context;
                }
                @Override
                public View getView(int position, View convertView, ViewGroup parent) {
                    ImageView imageView;
                    if(convertView == null){
                        imageView = new ImageView(context);
                        imageView.setLayoutParams(new GridView.LayoutParams(45, 45));    //����ImageView���󲼾�
                        imageView.setAdjustViewBounds(false);         //���ñ߽����
                        imageView.setPadding(8,  8,  8,  8);                //���ü��
                    }
                    else{
                        imageView = (ImageView)convertView;
                    }
                    imageView.setImageResource(imgs[position]);    //Ϊ ImageView ����ͼƬ��Դ
                    return imageView;
                }
            }
        //Activity �����У�ʹ���Զ���� Adapter �� GridView ����ʾͼƬ
            GridView gridView = (GridView)findViewById(R.id.xxx);
            gridView.setNumColumns(4);     //��������
            gridView.setAdapter(new MyAdapter(this));
        *********************************************************************************************/
    }
}

