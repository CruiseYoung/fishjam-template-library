package com.fishjam.study;

/**************************************************************************************************************************************
 * MyEclipse = Eclipse + һ����ҵ���, �����ڿ��� Java/J2EE �ȣ�
 *   2014�棺����֧�� AJAX,CSS,Hibernate,HTML5,Javascript,Java Servlet,JDBC,JSF,JSP,JQuery,Spring,SQL,Struts
 *           ��ǶMySql, tomcat �ȷ���
 *
 * 2014 ���ƽ����: http://filemarkets.com/fs/9luj7unx1udc2628/ �� ���صط�ע����û���/����: fishjam/fishjam
 *  1.��װ�� MyEclipse2014 ���Ȳ�Ҫ���г���
 *  2.1.�Թ���Ա��ݣ�ִ���ƽ��ļ����µ� run.bat
 *  2.2.ѡ�� Professional �汾 -> usercode ������� -> ͨ�� SystemId �������к�;
 *  2.3.Tools -> rebuildKey -> Active , ����һ������
 *  2.4.Tools -> replaceJarFiles, �ŵ� %APPDATA%\MyEclipse\Common\
 *  2.5.�� MyEclipse Ŀ¼�н������ ?
 *  
 * http://jingyan.baidu.com/article/e4d08ffdd684640fd2f60ddd.html
 * 
 * ��:
 *   1.Web Project
 *   2.�Ҽ� -> Add struts Capabilities, Add Hibernate Capabilities(hibernate.cfg.xml) ��
 *   3.
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
 * �ߴ�������
 *   1.JavaEEģ��
 *   2.WEB��������
 *   3.EJB��������
 *   4.Ӧ�ó����������������
 *   5.JavaEE��Ŀ�������
 *   6.���ݿ����
 *   7.MyEclipse���ϰ���
 *
 * ���ÿ�ݼ������� -- http://baike.baidu.com/link?url=GF0-5mxd1L_V5w-wo7K8w-3YOrNER4jjkI9yaE2tbPUr7IB4TwHk4EiuDUVt5wkZ
 *   Ctrl+O ������ʾ OutLine
 *   Ctrl+T ������ʾ��ǰ��ļ̳нṹ
 *   Ctrl+E ������ʾ��ǰEditer�������б�
 *   Ctrl+J ������������
 *   Ctrl+Q ��λ�����һ�α༭��
 *   Ctrl+I ��ʽ�������Ԫ��Format Active Elements��
 *   Ctrl+F6 �л�����һ��Editor
   * ��Ctrl+F7 �л�����һ��Perspective
 *   Ctrl+F8 �л�����һ��View
 *   
 *   �������ֵ��Զ���ʾ
 1.Code Assist -- Ĭ���� Alt+/(�� Window->Perferences->General->Keys ������)�����Ը���Ϊ Ctrl+Space;
 2.Word Completion -- 
 2.Window->Perferences->Java->Editor->Content Assist ������ "Auto activation triggers for java"��
   ��������Ϊ .��a-z,A-Z��������ĸ���������ú��޸ģ�Ȼ���ٵ��룬����ͻ�����ڰ汾ֻ������4���ַ������ƣ�;


Eclipse -- ��ѿ�Դ��IDE��http://www.eclipse.org/����IBM����
MyEclipse -- ����շѣ������е�plugins������Eclipse�У�
OSGIЭ�� -- 

��Դ�ƻ�(Open Source Project)
Eclipse �������֮�������ڳ��˵ײ�ĺ������⣬���ж���������ң��߱�ǿ��������
perspective -- Eclipse �ṩ��ȺԤ��ѡ������ͼ���������ȶ���õķ�ʽ���С�


Eclipse ��Ŀ��Project Management Committee(PMC)������Eclipse ��Ŀ�ֳ�3 ������Ŀ��
1.ƽ̨--Platform
2.����������--Java Development Toolkit(JDT)
3.��ҿ�������--Plug-in Development Environment(PDE)

ͨ�������ְ汾��
1.Release Builds -- ��Ҫ�ȶ��汾,�����������ԡ�
2.Stable builds -- ����Eclipse �����ŶӲ��ԣ��¹���ͨ�����ڴ˹��ɰ汾���֡�
3.Integration builds -- ��������������Ѿ������ԣ�������֤���Ϻ���ȶ���
4.Nightly builds -- ÿ��AutoBuild�����ġ�

Eclipse ƽ̨�����������ɣ�
  ƽ̨����(platform kernel)--����ͼ��ظ��ֲ��
  ����̨(workbench)--ͼ�β����ӿڣ���Eclipse�Լ��ı�׼ͼ�ι�����(Standard Widget Toolkit-SWT)��JFace���������ġ�
    Eclipse ʹ�ò���ϵͳ��ԭ��ͼ��������,���Ҫ��SWT��ֲ����ƽ̨��Eclipse �������Ǹ�ƽִ̨�С�
    ��ͼ(View)--���ֹ���̨�е���Ϣ
    �༭��(Editor)--��ʾ�ļ��ı༭���������ļ����ͣ��в�ͬ�ı༭��
    ͸��ͼ(Perspective)--�Թ���̨��һ����ͼ�����úͲ��֣�������ض����͵�����(��Java������Debug)
  ������(workspace)--����ʹ���ߵ���Դ(����֯��һ��������Ŀ)�����Ա�����ĵ���ʷ��¼
  �Ŷ����(tram component)
  ˵�����(help)

Elcipse Ϊ�˷���֧�ָ��ֲ���ϵͳ�����ļ�ϵͳ���ṩ��һ������㼶����Ŀ > ���ݼ�

���
  ADT -- Android Development Toolkit
  CDT -- C Development Toolkit
  JDT -- Java Development Toolkit
  TPTP -- Test & Performance Tools Platform�����Խ��г���ִ��ʱ���ͳ�Ʒ������ڴ�ļ�ء�������õķ�����
  VssPlug -- ֧��VSS����ѹ�� dropins Ŀ¼�£����˵����һ�VSS����ͨ�� "Team->Share..."��������,��VSS����������������
    Workspace\.metadata\.plugins\org.vssplugin\dialog_settings.xml ��
  WTP -- Web �����õĲ�����������֣�

���ò���ķ���
  1.��ѹ�� plugins �� dropins(�� org.vssplugin_1.6.2) Ŀ¼�£�������
  2.��EclipseĿ¼�´���һ��linksĿ¼������*.link ���ı��ļ�������ָ���������ط��Ĳ����
    �� path=d:\\myplugin\\eclipse\\plugins\\vssplugin_1.6.1
  ����������⣬���Գ��Խ�configurationĿ¼�³� config.ini �ļ���������ļ���ɾ��(δ���ԣ�����ȱ���)��

���ܲ���
  JBuilder���� Optimize Suite�����Լ�����еĲ����߳����ݡ��ڴ��ʹ�������

���ÿ�ݼ�
  Ctrl+F6 -- �л��༭��
  Code Formatter(Window->Preferences->Java->Code Formatter)
  
���
  JET(Java Emitter Templates) -- һ����Դ��ģ�����棬������EMF���Զ����ɴ��롣
     http://www.ibm.com/developerworks/cn/opensource/os-cn-ecl-jet/?ca=drs-tp4608

***************************************************************************************************************************************/

public class EclipseStudy {

}
