package com.fishjam.study.javaee;

/**************************************************************************************************************************************
 * TODO:
 *     Apache -- ΪHTMLҳ�����
 *     Tomcat -- ����JSPҳ���Servlet����һ��Servlet��JSP���������Դ���HTMLҳ�棬����������Apache
**************************************************************************************************************************************/

/**************************************************************************************************************************************
 * Tomcat -- ��Դ����ѵ�jsp������������С��ϵͳ�Ͳ��������û����Ǻܶ�ĳ����±��ձ�ʹ�ã���Apache����������չ
 * 
 * ʹ�÷�ʽ���Ƽ�ʹ�ý�ѹ���ķ�ʽ�����Կ�����־
 *   1.��ѹ : bin\TomcatX.exe 
 *   2.��װ : ��ʼ ->Apache Tomcat -> Configure Tomcat -> Start
 * 
 * ����
 *   1.bin\catalina.bat ������Java��������ڴ������Xms��PermSizeֻ��������Ӱ�죬Xmx�������������ʹ�õ��ڴ棬
 *     set JAVA_OPTS=-Xms1024m -Xmx1024m -Xmn512m -XX:PermSize=128m -XX:MaxPermSize=256m
 *   
 * ����ͨ������Ŀ¼����(��������Tomcat�б߲��Ա߿���)
 *   ����1.�� server.xml �� <Host> �У�����:
 *     <Context path="/myProject" docBase="D:\\workspaces\\myProject" reloadable="false" privileged="true"></Context>
 *     path Ϊ����Ŀ¼��docBase Ϊʵ��Ŀ¼
 *   ����2.�� conf\Catalina\localhost Ŀ¼���½� myProject.xml �ļ�(�ļ�������ΪWebӦ�õ�����·��)��������Ϊ��
 *     <Context docBase=D:\\workspaces\\myProject" debug="1" privileged="true"></Context> 
 *   
 * ����
 *   ����ƽ�� -- 
 *   �ʼ����� -- 
 *   
**************************************************************************************************************************************/

/**************************************************************************************************************************************
 * tomcat-users.xml -- �û�Ȩ�޹�����������ͨ����ҳ��ʽ����( http://localhost/manager/status )
 *  <role rolename="admin-gui"/>
 *  <role rolename="manager-gui"/>
 *  <role rolename="manager"/>
 *  <role rolename="tomcat"/>
 *  <role rolename="admin"/>
 *  <user username="admin" password="admin" roles="admin,manager,admin-gui,manager-gui"/>
 *
 * web.xml -- ���������ļ�
 *   <error-page> -- �����ض��쳣�������ʾҳ��
 *
 * server.xml --
 *  <Connector port=8080 ... > -- ָ���������ļ����˿ڡ�Э���
**************************************************************************************************************************************/

public class TomcatStudy {

}
