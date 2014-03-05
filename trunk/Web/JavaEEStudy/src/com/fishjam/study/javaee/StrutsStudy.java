package com.fishjam.study.javaee;

/**************************************************************************************************************************************
 * TODO:
 *   1.��Ҫʹ�� applicationContext.xml �ļ�������spring���İ���
 *   2.Struts1�е�Action������thread��safe��ʽ��������һ��ʵ��ȥ�������е��������õ������е���Դ������ͳһͬ����
 *   
 *   Convention(Լ��)֧�� -- ?
**************************************************************************************************************************************/

/**************************************************************************************************************************************
 * MVC�������Ҫ����:
 *   1.��Webҳ���е�����Ԫ�ط�װΪһ��(����)���ݶ��� -- һ���Ǵ�����Ӧ��ʵ���࣬Ȼ��ͨ����Ӧ���Ե�setter����
 *   2.(Dispatcher)��������Ĳ�ͬ��������Ӧ���߼�����Ԫ������(����)���ݶ�����Ϊ��������;
 *   3.�߼�����Ԫ��һ����Action������?���������󣬷���һ��������ݶ���;
 *   4.��Resolver����������ݶ����е�������Ԥ����Ƶı��ֲ����ںϲ�չ�ָ��û���
 * 
 * ������MVC���
 *   JSF -- Sun �Ƽ��� Java EE �淶��Apache ҲΪ JSF �ṩ�� MyFaces ʵ�֣���������ϱ�Struts 2 ������( ��ͳ RAD )�����г�ռ���ʲ���
 *   Tapestry -- ��ȫ�����˴�ͳServlet API����һ�ִ���ġ����ʽ��MVC��ܣ�ͬʱ�ṩ�˿�������ҳ��ģ��Ľ��������
 *      ����ʹ�� JSP ���������ֲ㼼��
**************************************************************************************************************************************/


/**************************************************************************************************************************************
 * Struts1.x�Ķ���һ�㶼��.do��β��
 * Struts2����.action��β������ <struts>��ǩ�»ָ�Ϊ.do��ʽ, <constant name="struts.action.extension" value="do"></constant>
 *
 * Struts2 -- �� Webwork �����Ϲ���������MVC��ܣ�������MVC����� Spring MVC, JSF ��
 *   struts.apache.org ���ذ�װ�������� struts2-core, xwork, commons-logging, freemarker, ognl)
 *   MVCģʽ�� ����������
 *    Model -- Action/ActionForm/JavaBean/EJB��ʵ�ֺ�����ҵ�߼������ѿ���Ȩ����������JSP�ļ�������ͼ
 *    View -- Struts taglib/JSP��Ҳ��ʹ�� XSLT ���������ֲ��Ʒ��taglib�а����� Html,Bean,Logic,Template ��
 *    Control -- ActionServlet(1)/ActionSupport(2)���ṩ�˴������з��͵�Struts��HTTP�������ڵ㣬��ȡ�ͷַ���Щ������Ӧ�Ķ���(Action)��
 *    �����ļ���struts-config.xml(1)/struts.xml(2) -- ����ģ�͡���ͼ����������Ӧ��ϵ��ת����ͼ(View)��������װ��Ӧ����ģ�ͣ�Model��
 * 
 * Struts2��ʹ�ò��裺
 *   1.�� web.xml ������ Filter(������)
 *     <filter>
 *       <filter-name>struts-prepare</filter-name>
 *       <filter-class>org.apache.struts2.dispatcher.FilterDispatcher</filter-class>
 *     </filter>
 *     <filter-mapping>
 *       <filter-name>struts2</filter-name>
 *       <url-pattern>/*</url-pattern>
 *     </filter-mapping>
 *   2.�� com.opensymphony.xwork2.ActionSupport �̳�ʵ���Զ���� Action �࣬��д execute ���������ر�ʾִ�н���ı�־
 *     public class MyAction extends ActionSupport{
 *       private int operand1;  //ע�⣺�ж�Ӧ�� get/set ����
 *       private int operand2;
 *       public String execute() throws Exception{
 *         if ( getSum() >= 0){  // ������ǷǸ�����������positive.jspҳ��, getSum() ��ʵ��Ϊ return (operand1 + operand2);
 *           return "positive";
 *         }else // ������Ǹ�����������negative.jspҳ��
 *         {
 *           return "negative";
 *         }
 *       }        
 *   4.�� struts.xml ������ Action(������)�Ĵ���. struts(1) > package(n) > action(n)
 *     <package name="struts2" namespace="/mystruts" extends="struts-default"> 
 *       <action name="sum" class="action.MyAction">
 *         <result name="positive">/positive.jsp</result>    //name���Ա�ʾ������(execute���ص��ַ���)��class��ʾ��������(��ת��ҳ��)
 *         <result name="negative">/negative.jsp</result>
 *       </action>
 *     </package>
 *     ע�⣺֧��ͨ����������Զ�ƥ�䣺<action name="*" class = "com.fishjam.{1}Action" method="show"></action>
 *   5.��дJSP(�� sum.jsp)ҳ�棬ͨ�����е�form�������ύ�� Action, ����ʹ�� Struts2����tag(�� ��ǩ�� "/struts-tags" ��)
 *     ��<s:form>����ö�ʹ��Struts2��ǩ��������Ҫ��HTML����ͨ�ı�.
 *     <%@ page language="java" import="java.util.*" pageEncoding="GBK"%>
 *     <%@ taglib prefix="s" uri="/struts-tags"%>
 *     <html>... 
 *       <s:form action="mystructs/sum.action>
 *         <s:textfield name="operand1" label="������1"/>
 *         <s:textfield name="operand2" label="������2"/>
 *         <s:submit value="������" />
 *       </s:form>
 *     </html>
 *     ���ҳ�棺 positive.jsp��������Ҫ��  <s:property value="sum" />   -- <s:property>��ʾAction���е�sum����ֵ
**************************************************************************************************************************************/

/**************************************************************************************************************************************
 * struts.xml -- ͨ���������ļ����ɰ�������ϵͳ������֮�����ϵ(ҳ�浼��ʹϵͳ�������������)
 *   
**************************************************************************************************************************************/

/**************************************************************************************************************************************
 * ActionProxy -- 
 * FilterDispatcher -- ���Ŀ�����? ����ActionMapperȷ�������Ǹ�Action
 * HttpServletRequest
 * HttpServletResponse 
 * ServletDispatcher
***************************************************************************************************************************************/
/***************************************************************************************************************************************
* JSTL -- JSP Standard Tag Library,JSP��׼��ǩ��
***************************************************************************************************************************************/ 	 
public class StrutsStudy {

}
