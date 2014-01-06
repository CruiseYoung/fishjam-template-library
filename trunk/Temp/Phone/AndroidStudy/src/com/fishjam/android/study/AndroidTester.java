package com.fishjam.android.study;
import android.test.ActivityUnitTestCase;


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
 *  2.��д��Ԫ���Դ��룬�� android.test.AndroidTestCase �̳У�
 *     ������ setUp, tearDown �Ⱥ���
 *     TODO: �� ActivityUnitTestCase �� ActivityInstrumentationTestCase2 �ȼ̳м���ʹ�� Activity �ķ�����
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
 *         +-ActivityUnitTestCase -- ͨ���������Ե���Activity����������һ��Ӧ�����еĻ�����Ҳ��������Activity����������
 *                                              �����������Ե�Activity֮ǰ�������Injectһ���ٵ�Context����Application��
 *            getActivity() -- ���ø÷����󣬱����Ե�Activity�Ż������� 
 *         +-ActivityInstrumentationTestCase2 -- ͨ�����ڶ��Activity�Ĺ��ܲ��ԣ�ʹ��������ϵͳ���������Activity
 *      +-SingleLaunchActivityTestCase -- ���ڲ��Ե���Activity����ֻ����setUp��tearDownһ�Σ����Ա�֤���ж������֮��fixture���ᱻ����
 *   +-AndroidTestCase -- ͨ�����ڱ�д Android ������ͨ�õĲ����������ɲ���permission
 *      +-ApplicationTestCase<MyApplication> -- ����Application���󡣲�����������createApplication()��Ż�ִ��Application��onCreate��������ͨ�� setContext ��ע���Զ����MockContext����
 *      +-ProviderTestCase2 -- ����Content Provider
 *      +-ServiceTestCase -- ����Service���������ṩ�� getService() ��ȡ�õ�ǰ�����ԵĶ���
 *      
 * MoreAsserts -- ֧�ָ���ıȽϷ������� RegEx(����)�Ƚϵ�
 * ViewAsserts -- ��������У�� UI View
***************************************************************************************************************************************/

/***************************************************************************************************************************************
 * ��ע -- ��ע���Է��������໮����Ҫ�Ǹ��ݲ��Է������ݵ�λ�ã��籾�أ����ݿ⣬SD�������磬֧�ֶ��߳� ��
 *   @SmallTest 
 *   @MediumTest
 *   @LargeTest 
***************************************************************************************************************************************/

public class AndroidTester  extends ActivityUnitTestCase{
	public AndroidTester(Class activityClass) {
		super(activityClass);
		// TODO Auto-generated constructor stub
		 
	}

	public void testActivityLable(){
	
	}
}

