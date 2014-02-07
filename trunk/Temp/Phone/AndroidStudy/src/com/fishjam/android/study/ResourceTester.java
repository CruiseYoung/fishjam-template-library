package com.fishjam.android.study;
import android.test.AndroidTestCase;

/**************************************************************************************************************************************
 * ע�⣺
 *   1. id="@+id/MyTextView01" -- ǰ��ļӺ�"+"��ʾֱ�ӷ����ʶ��(û��ר�ŵ���Դ�ļ�)��ֻ���� id�������ط�����ʱʹ�� "@id/xxx" �ķ�ʽ
 *
 * ��Դ������(ResourceManager)--�ṩ���ʷǴ�����Դ���籾�ػ��ַ�����ͼ�񣬲����ļ���������
 *   ʹ����Դ(resouces -- ������ attr/drawable/id/layout/string/style �ȶ���)
 *     Resources resources = getBaseContext().getResources();
 *     Drawable HippoDrawable = resources.getDrawable(R.drawable.white);
 *     mTextView01.setBackgroundDrawable(HippoDrawable); //���ñ���ɫ
 *   ʹ����Դʱ��������������ʽ��
 *     android:xxx/yyy,
 *     android:@xxxx/yyy  -- �� @string/hello, @drawable/white
 *     android:?xxx
 *       @+id/myPassword
 *   
**************************************************************************************************************************************/
/**************************************************************************************************************************************
 * Android�ṩ�˻���XML�Ľ��湹����ʽ���������Ӧ�ó����߼�����.xml�ļ����� res(������зǴ�����Դ -- ͼƬ�����ػ��ַ�����XML�����ļ���)Ŀ¼�¡�
 *   ʹ����Ϊ"R.java"���ļ�������������Դ���������Զ����ɣ���Ҫ���ġ�
 *   ���з�Ϊ attr(����, declare-styleable), drawable(ͼƬ��Դ)��layout(ҳ�沼��)��string�ȶ����̬�ڲ���, public final class R { xxxx }��
 *   ������������Դʱ�� R.��Դ����.��Դ����
 *   
 *   ? layout ʱ��˳�򣺣�  setFrame -> layout [ onLayout ]-> 
 *   
 * ϵͳ�ṩ����Դ�� android.R.��Դ����.��Դ����, �� android.R.anim.fade_in(ϵͳԤ���嶯��); android.R.string.cancel(ϵͳԤ������ַ��� -- ���Զ�֧�ֶ�����?)
 *   Context.getString()/
 *   ��Դ�ļ�������ʱ�� "@��Դ����.��Դ����"
 *   
 * ����Դ�м����ַ���
 *   CharSequence str_2 = getString(R.string.str_2);  �� String str = getString(R.string.str_2).toString();
 *   �ַ�����Դ����ͨ�� array/ item �ķ�ʽ�����ַ������飬Ȼ���� AlertDialog��ͨ�� setItems(R.array.items_irdc_dialog, ...) ��ʹ��
 *   ����ͨ�� getResources().getStringArray(R.array.items_irdc_dialog) ���� String[] ��
 *   ʾ���� <array name="items_irdc_dialog"><item>�в�</item><item>��ʽ���</item><item>�����տ�</item></array>
 *   
 * �ַ�������
 *   <resources> <string-array name="books"> <item>xxxx</item></string-array></resources>
 *   
 * ��ɫ( #Alpha-Red-Green-Blue ��ʽ), �磺 #RGB, #ARGB, #RRGGBB, #AARRGGBB�� �����л����ɫ������Resources.getColor
 *   ������Ϊcolor.xml���ļ�,����valuesĿ¼��,Ȼ�����main.xml��ʹ��:�� android:background="@drawable/white" 
 *      <resources>  //�� graphics.Color �ж�����Ĭ�ϵ���ɫ����
 *        <drawable name="darkgray">#808080FF</drawable>
 *        <drawable name="white">#FFFFFFFF</drawable>
 *      </resources>
 *
 * ���� -- ArrayAdapter<String> ���� ? 
 *   ArrayAdapter<String> adapter = new ArrayAdapter<String>(this, android.R.layout.simple_spinner_item, countriesStr);
 *   adapter.setDropDownViewResource(R.layout.myspinner_dropdown);  //myspinner_dropdownΪ�Զ��������˵�ģʽ������res/layoutĿ¼��
 *   mySpinner.setAdapter(adapter);		//��ArrayAdapter���Spinner������
 * 
 * ʹ������
 *   1.��assetsĿ¼�д��� fonts ��Ŀ¼,���� .ttf �ļ�;
 *   2.���� android.graphics.Typeface;
 *   3.ͨ�� creatFromAsset ����Typeface����
 *      m_Text.setTypeface(Typeface.createFromAsset(getAssets(),"fonts/HandmadeTypewriter.ttf"));
 *  
 * �˵� -- ѡ��˵�(OptionMenu)�������Ĳ˵�(ContextMenu -- ��ʱ�䰴������ʱ)���Ӳ˵�(SubMenu)
 *   MenuInflater --
 *   ϵͳ�˵� -- ���� Activity �� �ض�����
 *      onCreate { mi = new MenuInflater(this); �� getMenuInflater(); } 
 *      onCreateOptionsMenu(Menu menu){ mi.inflate(R.menu.test_menu, menu); return true; } 
 *        ��ͨ�� menu.add/addSubMenu �ȷ�����̬���
 *      onOptionsItemSelected(MenuItem item){ switch(item.getItemId()) { case R.id.about:  xxxx; break; } return true; }
 *    �����Ĳ˵� -- ���� onCreateContextMenu/onContextItemSelected,  Ȼ����� registerForContextMenu(xxx) Ϊָ����ͼע��
 *    
 *      
 * ʹ�óߴ� -- ��λ(dimens)
**************************************************************************************************************************************/

/**************************************************************************************************************************************
 * XML -- ͨ�� getResources().getXml() ��� XmlResourceParser ����ʵ����ʾXMLԭʼ�ļ�
 *   import org.xmlpull.v1.XmlPullParser;
 *   XmlResourceParser xrp = getResources().getXml(R.xml.testXmlFile);
 *   while(xrp.getEventType() != XmlResourceParser.END_DOCUMENT) { 	//δ���ļ�β�����ѭ��
 *     if(xrp.getEventType() == XmlResourceParser.START_TAG) { 	//����ǿ�ʼ��ǩ  
 *       String name = xrp.getName(); 	//��ȡ��ǩ��
 *       xrp.getAttributeValue(nIndex); 	//��ȡָ������λ�õ�����ֵ,���õķ����Ǹ�����������ȡֵ
 *     }
 *     xrp.next();  //��ȡ��һ����¼
 *   }
**************************************************************************************************************************************/
public class ResourceTester  extends AndroidTestCase {

}
