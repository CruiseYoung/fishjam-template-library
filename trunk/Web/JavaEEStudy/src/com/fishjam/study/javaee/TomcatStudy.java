package com.fishjam.study.javaee;

/**************************************************************************************************************************************
 * TODO:
 *     Apache -- ΪHTMLҳ�����
 *     Tomcat -- ����JSPҳ���Servlet����һ��Servlet��JSP���������Դ���HTMLҳ�棬����������Apache
 *     
 * ����Web������
 *     JBoss -- ��Դ��רҵ Jave EE ������
 *     Jetty -- �ص㣺����Ϊһ��Ƕ��ʽ������(�� ����Ӧ���м����� JAR �ļ��� Ӧ�ÿ��ڴ����ж����ṩWeb���� )
 *     Resin -- Ŀǰ���� JSP��Servlet ����ƽ̨��֧��EJB������ѧϰ��ѣ��������շ�
 *     WebLogic |  WebSphere  -- רҵ������ Jave EE ������
 *     
**************************************************************************************************************************************/

/**************************************************************************************************************************************
 * Tomcat -- ��Դ����ѵ�jsp������������С��ϵͳ�Ͳ��������û����Ǻܶ�ĳ����±��ձ�ʹ�ã���Apache����������չ
 * 
 * ʹ�÷�ʽ���Ƽ�ʹ�ý�ѹ���ķ�ʽ( ���Կ�������������ʱ�Ŀ���̨��������ڿ�������)
 *   ��������:
 *     1. ���� TOMCAT_HOME ����������
 *     2. �� lib Ŀ¼�µ� jsp-api.jar �� servlet-api.jar ��ӵ� CLASSPATH ����������
 *     3. ��WebӦ�� (����Ŀ¼ �� WAR�ļ�) ���Ƶ� webapps Ŀ¼�£������Զ����������У�
 *   ��������( Ĭ��Ϊ http://localhost:8080 )
 *     1.��ѹ��ʽ : startup.bat 
 *     2.��װ : ��ʼ ->Apache Tomcat -> Configure Tomcat -> Start( bin\TomcatX.exe ?)
 * 
 * ����
 *   1. bin\catalina.bat ������Java��������ڴ������Xms��PermSizeֻ��������Ӱ�죬Xmx�������������ʹ�õ��ڴ棬
 *     set JAVA_OPTS=-Xms1024m -Xmx1024m -Xmn512m -XX:PermSize=128m -XX:MaxPermSize=256m
 *   2. ����DBCP����Դ -- ������Ϊ ȫ������Դ(server.xml) �� �ֲ�����Դ(����WebӦ�õ������ļ�)��
 *      �μ� myProject.xml �ļ� 
 *     
 *   
 * ����ͨ������Ŀ¼����(��������Tomcat�б߲��Ա߿���)
 *   ����1.�� server.xml �� <Host> �У�����:
 *     <Context path="/myProject" docBase="D:\\workspaces\\myProject" debug="0" reloadable="false" privileged="true"></Context>
 *     path Ϊ����Ŀ¼��docBase Ϊʵ��Ŀ¼������������Ŀ¼Ϊ "/", �����Ϳ���ֱ�ӷ���
 *   ����2.�� conf\Catalina\localhost Ŀ¼���½� myProject.xml �ļ�( �ļ�������ΪWebӦ�õ�����·�� )��������Ϊ��
 *     <Context docBase=D:\\workspaces\\myProject" debug="1" privileged="true">
 *       <Resource />  --  �����ĸ��ֲ������� ����DBCP����Դ
 *     </Context> 
 *   
 * ����
 *   ����ƽ�� -- 
 *   �ʼ����� -- 
 *   
**************************************************************************************************************************************/

/**************************************************************************************************************************************
 * tomcat-users.xml -- �û�Ȩ�޹���TomcatĬ�ϲ����ļ���ȫ�򣬿��޸ĳ�������ʽ�����壿�������û���������
 *  ��������ͨ����ҳ��ʽ����( http://localhost/manager/status��Ĭ��������κ��˶����ܷ��� ) -- �����û� admin, ������ admin-gui �Ƚ�ɫ
 *    <role rolename="admin-gui"/>
 *    <role rolename="manager-gui"/>
 *    <role rolename="manager"/>
 *    <role rolename="tomcat"/>
 *    <role rolename="admin"/>
 *    <user username="admin" password="admin" roles="admin,manager,admin-gui,manager-gui"/>
 *
 * web.xml -- ���������ļ�
 *   <error-page> -- �����ض��쳣�������ʾҳ��
 *   �г�WebӦ��·��������ҳ�棬����ֱ��ѡȡ�ļ����� -- /web-app/servlet/init-para[param-name='listings'] -- ����Ӧ�� param-value ��Ϊ 'true'
 *     
 *   </servlet>
 *   
 *   
 *   
 * server.xml --
 *   ����ļ����˿ںͳ�ʱ�� -- /Server/Service/Connector[@port] -- Ĭ�϶˿�Ϊ"8080", ��ʱΪ "20000"(�� 20s)
 *   ͬʱ�ṩ������� -- ���Ʋ��޸� /Server/Service �ڵ�
**************************************************************************************************************************************/

public class TomcatStudy {

}
