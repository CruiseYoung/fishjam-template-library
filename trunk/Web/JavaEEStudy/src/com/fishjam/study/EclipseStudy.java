package com.fishjam.study;

/**************************************************************************************************************************************
 * Eclipse -- ��ѿ�Դ��IDE��http://www.eclipse.org/����IBM����
 *   Eclipse �������֮�������ڳ��˵ײ�ĺ������⣬���ж���������ң��߱�ǿ��������
 *   perspective -- Eclipse �ṩ��ȺԤ��ѡ������ͼ���������ȶ���õķ�ʽ���С�
 *   Eclipse ��Ŀ�� Project Management Committee(PMC) ������Eclipse ��Ŀ�ֳ�3 ������Ŀ��
 *     1.ƽ̨--Platform
 *     2.����������--Java Development Toolkit(JDT)
 *     3.��ҿ�������--Plug-in Development Environment(PDE)
 *   ͨ�������ְ汾��
 *     1.Release Builds -- ��Ҫ�ȶ��汾,�����������ԡ�
 *     2.Stable builds -- ����Eclipse �����ŶӲ��ԣ��¹���ͨ�����ڴ˹��ɰ汾���֡�
 *     3.Integration builds -- ��������������Ѿ������ԣ�������֤���Ϻ���ȶ���
 *     4.Nightly builds -- ÿ��AutoBuild�����ġ�
 *   Eclipse ƽ̨�����������ɣ�
 *     ƽ̨����(platform kernel)--����ͼ��ظ��ֲ��
 *     ����̨(workbench)--ͼ�β����ӿڣ���Eclipse�Լ��ı�׼ͼ�ι�����(Standard Widget Toolkit-SWT)��JFace���������ġ�
 *        Eclipse ʹ�ò���ϵͳ��ԭ��ͼ��������,���Ҫ��SWT��ֲ����ƽ̨��Eclipse �������Ǹ�ƽִ̨�С�
 *     ��ͼ(View)--���ֹ���̨�е���Ϣ
 *     �༭��(Editor)--��ʾ�ļ��ı༭���������ļ����ͣ��в�ͬ�ı༭��
 *     ͸��ͼ(Perspective)--�Թ���̨��һ����ͼ�����úͲ��֣�������ض����͵�����(��Java������Debug)
 *     ������(workspace)--����ʹ���ߵ���Դ(����֯��һ��������Ŀ)�����Ա�����ĵ���ʷ��¼
 *     �Ŷ����(tram component)
 *     ˵�����(help)
 *   Elcipse Ϊ�˷���֧�ָ��ֲ���ϵͳ�����ļ�ϵͳ���ṩ��һ������㼶����Ŀ(?) > ���ݼ�(?)
 *    
 * MyEclipse = Eclipse + һ����ҵ���, �����ڿ��� Java/J2EE �ȣ�
 *   2014�棺����֧�� AJAX,CSS,Hibernate,HTML5,Javascript,Java Servlet,JDBC,JSF,JSP,JQuery,Spring,SQL,Struts
 *           ��ǶMySql, tomcat �ȷ���
 *  
 *
 * ��Ŀ����(���� WAR ��)
 *   1.��web��Ŀ���: Export -> JavaEE ��WAR file(myeclipse)
 *   2.��war�ŵ�tomcat��webappsĿ¼��, ���� server.xml �ļ�
 *
 * ֱ���������ݿ�
 *   Windows->Preferences->MyEclipse->Database Explorer->Database Drivers���������ݿ�����������
 *   1.Derby -- MyEclipse�Դ���һ����ϵ���ݿ�
***************************************************************************************************************************************/

/**************************************************************************************************************************************
 * ������:
 *   1.Web Project
 *   2.�Ҽ� -> Add struts Capabilities, Add Hibernate Capabilities(hibernate.cfg.xml) ��
 *   3.
 * 
**************************************************************************************************************************************/


/**************************************************************************************************************************************
 * ���ÿ�ݼ������� -- http://baike.baidu.com/link?url=GF0-5mxd1L_V5w-wo7K8w-3YOrNER4jjkI9yaE2tbPUr7IB4TwHk4EiuDUVt5wkZ
 *   Ctrl+O ������ʾ OutLine
 *   Ctrl+T ������ʾ��ǰ��ļ̳нṹ
 *   Ctrl+E ������ʾ��ǰEditer�������б�
 *   Ctrl+J ������������
 *   Ctrl+Q ��λ�����һ�α༭��
 *   Ctrl+I ��ʽ�������Ԫ��Format Active Elements��
 *   Ctrl+F6 �л�����һ��Editor(�༭��)
 *   Ctrl+F7 �л�����һ��Perspective
 *   Ctrl+F8 �л�����һ��View
 *   
 * ���ù���(���������Զ���Ŀ�ݼ�)
 *   Code Formatter(Window->Preferences->Java->Code Formatter)
 *   Override/Implement Methods -- ���ػ�����麯�������Ը���Ϊ Alt+Shift+O, When Ϊ "Editing Java Source"
 *    
 * �������ֵ��Զ���ʾ
 *   1.Code Assist -- Ĭ���� Alt+/(�� Window->Perferences->General->Keys ������)�����Ը���Ϊ Ctrl+Space;
 *   2.Word Completion -- 
 *   2.Window->Perferences->Java->Editor->Content Assist ������ "Auto activation triggers for java"��
 *     ��������Ϊ .��a-z,A-Z��������ĸ���������ú��޸ģ�Ȼ���ٵ��룬����ͻ�����ڰ汾ֻ������4���ַ������ƣ�;
 *     �������ã� File -> Export -> Perferences -> ������ �������滻 content_assist_autoactivation_triggers_java ��ֵ
 *
***************************************************************************************************************************************/ 

/***************************************************************************************************************************************
 * MyEclipse�������߸��£�
 *   1.Window--Preferences--General-Capabilities, ѡ�� "Classic Update", Ȼ�� Apply
 *   2.�˺���ܿ������²˵���Help > Software Updates > Find and Install...
 *
 * ���ò���ķ���
 *   �������������⣬���Գ��Խ�configurationĿ¼�³� config.ini �ļ���������ļ���ɾ��(δ���ԣ�����ȱ���)��
 *   1.��ѹ�� plugins �� dropins(�� org.vssplugin_1.6.2) Ŀ¼�£�������
 *   2.��EclipseĿ¼�´���һ��linksĿ¼������*.link ���ı��ļ�������ָ���������ط��Ĳ����
 *     �� path=d:\\myplugin\\eclipse\\plugins\\vssplugin_1.6.1
 *   3.���߰�װ
 *   
 *   
 * ���ò��
 *   ADT -- Android Development Toolkit
 *   CDT -- C Development Toolkit
 *   JDT -- Java Development Toolkit
 *   JET -- Java Emitter Templates ,һ����Դ��ģ�����棬������EMF���Զ����ɴ��롣
 *   Subversive -- Subversion (SVN) Team Provider ( http://www.eclipse.org/subversive/ )
 *   TPTP -- Test & Performance Tools Platform�����Խ��г���ִ��ʱ���ͳ�Ʒ������ڴ�ļ�ء�������õķ�����
 *   VssPlug -- ֧��VSS����ѹ�� dropins Ŀ¼�£����˵����һ�VSS����ͨ�� "Team->Share..."��������,��VSS����������������
 *     Workspace\.metadata\.plugins\org.vssplugin\dialog_settings.xml ��
 *   WTP -- Web �����õĲ�����������֣�
 * 
 * 
***************************************************************************************************************************************/

public class EclipseStudy {

}
