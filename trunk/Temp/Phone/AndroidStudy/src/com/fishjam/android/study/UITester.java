package com.fishjam.android.study;
import junit.framework.TestCase;

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
 * view  -- ����UI�඼������View��ViewGroup������Ļ���֮�ϣ��������(Composite)���ģʽ
 *   View --�����ؼ�,  �������Ϊ Widget
 *     Button( setOnClickListener(new Button.OnClickListener(){public void onClick(View v) {...}}); )
 *     EditText
 *     TextView
 *     RelativeLayout
 *     lists,grids,text boxes,buttons,
 *   ViewGroup -- ���������������ΪLayout
 *   
 * �Ի�
 *   ��View������������ onDraw(Canvas canvas) ��������������ʹ�� Canvas �� Paint���л��ơ�
***************************************************************************************************************************************/

/***************************************************************************************************************************************
 * View
 *   BaseAdapter -- 
 *   Button
 *     text
 *   EditText
 *     textSize -- 18sp
 *     password -- true
 *     numeric -- decimal
 *   CheckBox
 *   Gallery(ͼ��)
 *   ImageButton
 *     src -- "@drawable/iconempty"
 *     setImageResource(R.drawable.iconempty);
 *   ImageView -- ��ʾͼƬ
 *     setImageDrawable(getResources().getDrawable(R.drawable.right));   //������ʾ��ͼƬ, ��ͨ�� src ��������
 *   ListView
 *   RadioGroup(����һ��RadioButton)
 *     checkedButton -- "@+id/sex1"
 *     orientation -- vertical,horizontal
 *   RadioButton
 *   TextView -- �ı���ͼ����ʾ�ַ���
 *     text -- @string/str_id
 *     textColor -- @drawable/darkgray
 *     gravity="center_vertical|center_horizontal", bottom
 *     autoLink -- all(������ʾ��ַ���� http://)
 *   ProgressDialog -- ������ʾ
 *   Spinner --
 *   
***************************************************************************************************************************************/

/***************************************************************************************************************************************
 * Layout -- Android ͨ�� LayoutInflater �ཫ XML ��ʽ�Ĳ����ļ��е��������Ϊ���ӻ�����ͼ�����
 *   �����ļ��е� <requestFocus/> �����ʲô��˼? 
 *   AbsoluteLayout -- ����λ�ö�λ�����ͼ����ԣ�ά����
 *   RelativeLayout -- ���������Զ���Ӧ���������ͼ����Ρ�
 *   LinearLayout -- ���Ͳ���
 *     orientation -- vertical,horizontal
 *     layout_width -- fill_parent,wrap_content(��������ָ���߿�),320px,80dip
 *     layout_height -- fill_parent,wrap_content
 *     layout_alignParentRight -- true
 *     layout_alignParentTop -- true
 *     layout_alignParentLeft -- true
 *     layout_marginRight -- 80dip, 
 *     layout_below -- "@+id/ExitButton", 
 *     layout_alignTop -- "@+id/sizebutton"
 *     layout_toLeftOf -- "@+id/sizebutton"
 *     layout_x -- 30px
 *     layout_y
 *     layout_centerHorizontal -- true
 *     id -- "@+id/ResultText"
   *   background -- "@drawable/testpic",
 *   TableLayout + TableRow + 
 *     stretchColumns -- "����"
 *   TableRow
 *     gravity -- right
 *
***************************************************************************************************************************************/
 
/***************************************************************************************************************************************
 * Activity  :  ��ʾ�û������е�һ����Ļ����ʾ�ɼ���Views�ؼ���ɵ��û��ӿڣ������¼�������Ӧ��������xml�ļ��ж�������Ҫ��Views���ڴ����ж��壬
 *     Context <- ContextWrapper <- ContextThemeWrapper <- Activity
 *   ÿ��Activity��״̬��������Activityջ�е�λ�������������е�ǰ�������е�Activity����ѭ����ȳ���ԭ��
 *   ��һ���µ�Activity��������ǰ��Activity��������ջ�Ķ���������û�ʹ��Back��ť������ǰ̨Activity���رգ���һ��Activity�����������������ջ�Ķ�����
 *   Activity״̬ -- ״̬���ɾ��в�ȷ���Բ�����OPhone Runtime��ȫ����
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
 *   ��������
 *     findViewById
 *     setContentView -- �ɽ�Activity��Views����һ����ʵ���û������Ĺ���
 *     startActivity -- ��һ����Ļ��������һ����Ļ����Activity����������װ��Intent��
 **************************************************************************************************************************************/

/**************************************************************************************************************************************
 * Dialog
 *   AlertDialog
 *     ʾ��: 
 *       AlertDialog.Builder dlgBuilder = new AlertDialog.Builder(this);  //����Builder��Ȼ������������ø��ֲ���
 *       dlgBuilder.setMessage(msg).setCancelable(false).setPositiveButton("ȷ��", new DialogInterface.OnClickListener() { ...onClick(...){finish();} }); //����builder
 *       AlertDialog alert = dlgBuilder.create();  //����
 *       alert.show(); //��ʾ
 * 
 * Toast -- ��ʾ����(û�н������ܣ�ֻ����ʾ)
 *    Toast.makeText(EX04_03.this,"��ʾ��Ϣ", Toast.LENGTH_LONG).show;
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
 *    requestWindowFeature(Window.FEATURE_NO_TITLE);   		//Hide title bar;
 *    getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);  //Hide status bar;
 *    //getWindow().setBackgroundDrawableResource(R.color.red_bg); //���ñ���ɫ
 * 2. ͨ����Դ�ж����id�ҵ���Ӧ�Ķ��� -- findViewById
 *    TextView ResultText = (TextView)findViewById(R.id.ResultText);
**************************************************************************************************************************************/

public class UITester extends TestCase {
	public UITester()	{
		
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
}

