package com.fishjam.android.study;

import com.fishjam.util.CallbackTesterActivity;

import android.app.Activity;
import android.content.Intent;
import android.test.ActivityUnitTestCase;
import android.test.AndroidTestCase;
import android.util.Log;
import android.view.LayoutInflater;


/***************************************************************************************************************************************
 * ��Ԫ���� --  http://tech.ddvip.com/2012-12/1356946984188084.html
 *   Android������ĿҲ��һ��AndroidӦ����Ŀ�� �����ʹ�÷����Ϳ���һ���AndroidӦ�÷ǳ����ơ�
 *   
 *   Android Testing Framework -- ����������Android�ĸ������棬��Ԫ���ԡ���ܲ��ԡ�UI���Եȡ�
 *   Android JUint ��չ -- �ṩ�˶�Android�ض��������Activity��Service���Ĳ���֧��
 *   
 * �Զ�����
 *   SDK�ṩ��һ��monkeyrunner(һ��pythonӦ��)������ģ���û������¼�������UI��
 *   ���Ա�дPython��������װӦ�û���԰������С�ģ�ⷢ�Ͱ�����Ϣ�������ȡ�
 *   ����
 *     MonkeyRunner -- �ṩӦ�õĸ��������Լ����������豸����ģ�����ķ��������ṩUI֧�ֵ�(ע��ʱ Python �ļ�)
 *     MonkeyDevice -- ����һ���豸����ģ�������ṩ��װ��ж��Ӧ�õķ���������һ��Activity�����Ͱ�������Touch �¼��ȡ�
 *     MonkeyImage -- ����һ������ͼ�񣬿��Խ�ȡ��ͬ��ʽ��ͼ�񣬱Ƚ�����MonkeyImageͼ�񣬱���ͼ���
 *        device = MonkeyRunner.waitForConnection() -- ���ӵ��豸
 *        device.press('KEYCODE_MENU','DOWN_AND_UP')  -- ģ�ⰴ��
 *        device.takeSnapshot() -- ��ͼ
 *        
 *   �������
 *      adb shell monkey -p com.fishjam.android.study  -v 500  -- ʹ��ȱʡ���ã���Ӧ�÷���500������¼�(����������touch�¼���ϵͳ�¼���)
 *      Monkeyѡ�
 *        1.�������� -- �����ó��Ե��¼�����
 *        2.����Լ�� -- ������ֻ�Ե�����һ�������в���
 *        3.�¼����ͺ�Ƶ��
 *           �Cthrottle <milliseconds> -- ���¼�֮�����̶��ӳ٣�ͨ����ѡ����Լ��� Monkey ��ִ���ٶ�
 *        4.����ѡ��
***************************************************************************************************************************************/

/****************************************************************************************************************************************
 * 1.�������̶�Ӧ��Test����( �� ApiDemos ��Ӧ�� ApiDemos.tests)
 * 2.Java Build Path-> Projects �����ö�Ӧ�Ĵ����Թ���(��Test��Ŀ�м����������е���)
 * 3.����һ�� AllTests.java �ļ���������Ϊ(�μ� ApiDemos\tests �е�Դ��) -- �ƺ��ǿ�ѡ�ģ���ʲô�ã�
 *   public class AllTests extends TestSuite {
 *      public static Test suite() {
 *        // ָ�����иð������Ӱ��ж����TestCase��Ϊ����TestSuite ��һ����
 *        return new TestSuiteBuilder(AllTests.class).includeAllPackagesUnderHere().build();
 *      }
 *   }
 *
 * 
 *   1.Menifest.xml �м���:
 *     a.<application>�м���: 
 *       <uses-library android:name="android.test.runner" />
 *     b.<application>�����:
 *       <uses-permission android:name="android.permission.RUN_INSTRUMENTATION" />
*        <instrumentation android:name="android.test.InstrumentationTestRunner" android:targetPackage="com.fishjam.android.study" android:label="Test for Android Study"/>
 *  2.��д��Ԫ���Դ��룬�� android.test.AndroidTestCase, ActivityUnitTestCase, ActivityInstrumentationTestCase2 �ȼ̳У�
 *     ������ setUp, tearDown �Ⱥ��������б�Ҫ�ĳ�ʼ������Դ�ͷ�
 *  3.�� public void testXxx() throws Throwable{ } ��ͨ�� junit.framework.Assert �Ƚ��ж����ж�
 *    JUnit ���� test ��ͷ�ķ�����Ϊһ��ʵ����Ҳ����ʹ�� annotation @Test ��ʾһ������Ϊ���Է�����
 *  4.����
 *    a. eclipse�� -- �Ҽ� Run as "Android JUnit Test"
 *    b. adb shell am instrument -w com.fishjam.android.study/android.test.InstrumentationTestRunner
 * 
 * Activity Testing -- ������Activity�Ĳ��ԣ�ʹ�� Instrumentation ������Activity��������֮��������Activity��Ȼ�����Activity���ض��Ĺ���
 * Content Provider Testing
 * Service Testing
 **************************************************************************************************************************************/

/***************************************************************************************************************************************
 * ������صİ�����
 *   android.test -- ���ڱ�дAndroid��������
 *   android.test.mock -- �����˷�������õĲ���"׮"��, �� MockApplication, MockContentProvider, MockContext, MockCursor �� 
 *   android.test.suitebuilder -- ���в���������Test Runner��
 * 
 *  Instrumentation -- �ṩ��һЩ"����"�������ӵ�Android����ϵͳ�У��ɶ�������Android���(Activity,Service��)����������,
 *    �����Կ���Android��ε���һ��Ӧ�á�ʹ��Instrumatation APIʱ�����ֱ�ӵ��� onCreate, onResume �������� ������
 *    Ҳ����֧��ǿ��ĳ��Ӧ�ú���һ���Ѿ���������Ӧ��������ͬһ��������(�ڲ��Դ����п���ֱ�ӵ��ñ�����Ӧ�õķ����ͷ������Ա)������ͨ����������ǲ�����ʵ�ֵġ�
 *  InstrumentationTestRunner 
 *   
 *  TestCase  -- JUnit �еĻ���Case���࣬�����ڱ�дһЩ��ƽ̨�޹صĲ�������
 *   +-InstrumentationTestCase -- �ṩ��Instrumentation�ӿڸ����࣬��ֱ�ӿ���Activity���������ڣ�������һЩMock�������������ԣ�����UI���Ͱ����ʹ����¼���ģ���û����潻����
 *                                               �� getInstrumentation().callActivityOnStart(xxx);
 *      +-ActivityTestCase
 *         +-ActivityUnitTestCase<MyActivity> -- ͨ���������Ե���Activity(ҵ���߼���)����������һ��Ӧ�����еĻ�����(û��UI����)Ҳ��������Activity����������
 *            �����������Ե�Activity֮ǰ�������Injectһ���ٵ�Context(setActivityContext)����Application(setApplication)
 *            getActivity() -- ���ø÷����󣬱����Ե�Activity�Ż�������
 *            ʹ�÷�ʽ��
 *               1.��ActivityUnitTestCase�̳У�ָ��Activity��ģ�����; �����޲ι��죬ʵ��Ϊ super(MyActivity.class);
 *               2. setUp() ��  mStartIntent = new Intent(Intent.ACTION_MAIN);
 *               3. testXxx() �����У�һ��ʼ��Ҫ���� startActivity(mStartIntent, null, null); Ȼ����ܵ��� getActivity() ���ʵ�������򽫻᷵�� null
 *               4. Ȼ�󼴿�ʹ�� getActivity().findViewById(xxx) �ȷ�����ȡ������Ԫ�ؽ��в���
 *         +-ActivityInstrumentationTestCase2 -- ͨ�����ڶ��Activity�Ĺ��ܲ��ԣ�ʹ��������ϵͳ���������Activity, 
 *               1.����ֱ�� getActivity ��ȡʵ��(���Զ�����)
 *               2.����ʹ�� a.sendKeys(xxxx); b.getActivity().runOnUiThread(new Runnable() {xxxx});  getInstrumentation().waitForIdleSync(); �ȷ�������UI���͵Ľ�����
 *            ʹ�÷�ʽ��
 *               1.�޲ι����� super("packagepath", MyActivity.class>);
 *               2.setUp �п���ֱ�� getActivity() ��ȡ��ʵ��
 *      +-SingleLaunchActivityTestCase -- ���ڲ��Ե���Activity����ֻ����setUp��tearDownһ�Σ����Ա�֤���ж������֮��fixture���ᱻ����
 *   +-AndroidTestCase -- ͨ�����ڱ�д Android ������ͨ�õĲ�������(����ֱ��ͨ�� getContext() ��ȡ���ǿ�ʵ��)���ɲ���permission������ʹ�� FocusFinder
 *      ���� LayoutInflater.from(getContext()).inflate(R.layout.xxx, null) �ķ�ʽ������Դ������
 *      +-ApplicationTestCase<MyApplication> -- ����Application���󡣲�����������createApplication()��Ż�ִ��Application��onCreate��������ͨ�� setContext ��ע���Զ����MockContext����
 *      +-ProviderTestCase2 -- ����Content Provider
 *      +-ServiceTestCase -- ����Service���������ṩ�� getService() ��ȡ�õ�ǰ�����ԵĶ���
 *      
 * MoreAsserts -- ֧�ָ���ıȽϷ������� RegEx(����)�Ƚϵ�
 * ViewAsserts -- ��������У�� UI View
 * 
 * ���Է���
 *   assertEquals -- ��ȶ���
***************************************************************************************************************************************/

/***************************************************************************************************************************************
 * ��ע -- ��ע���Է��������໮����Ҫ�Ǹ��ݲ��Է������ݵ�λ�ã��籾�أ����ݿ⣬SD�������磬֧�ֶ��߳� ��
 *   @SmallTest 
 *   @MediumTest
 *   @LargeTest 
***************************************************************************************************************************************/

//*
public class AndroidTester  extends ActivityUnitTestCase<CallbackTesterActivity>{
	private final static String TAG = AndroidTester.class.getSimpleName();
	private Intent mStartIntent;
	public AndroidTester() {
		super(CallbackTesterActivity.class);
	}

	
	@Override
	protected void setUp() throws Exception {
		super.setUp();
		mStartIntent = new Intent(Intent.ACTION_MAIN);
	}


	public void testActivityLife(){
		assertNull(getActivity()); 
		Log.i(TAG, "Before call startActivity");
		
		startActivity(mStartIntent, null, null);		//start ֮�� getActivity �Ų�Ϊnull
		Activity activity1 = getActivity();
		Activity activity2 = getActivity();
		
		assertNotNull(activity1 != null);
		assertEquals("��һ�β��Ժ����У���ε���getActivity() �᷵��ͬһʵ��", activity1,  activity2);
		
		Log.i(TAG, "Before callActivityOnStart");
        // At this point, onCreate() has been called, but nothing else Complete the startup of the activity
        getInstrumentation().callActivityOnStart(activity1);
        Log.i(TAG, "Before callActivityOnResume");
        getInstrumentation().callActivityOnResume(activity1);
        
        Log.i(TAG, "Before callActivityOnStop");
        getInstrumentation().callActivityOnStop(activity1);
        
        Log.i(TAG, "Before testActivityLife function End");			//ʵ�ⷢ��û�е��� onDestroy
        // ActivityUnitTestCase.tearDown(), which is always automatically called, will take care
        // of calling onDestroy().
	}
}

/*/
class AndroidTester extends AndroidTestCase{
	public void testGetContext(){
		assertNotNull("AndroidTestCase �п���ֱ�ӻ�ȡContext", getContext());
	}
}
//*/