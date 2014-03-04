package com.fishjam.study.javaee;

//������Java_EE��ҵӦ��ʵս_Struts_2+Spring_3+Hibernate���Ͽ��� -- P

/**************************************************************************************************************************************
 * TODO:
 *   1.��Ŀ����ְ���ʱ�򣬿����ȷ�Ϊ MVC ��Ӧ�ļ������
 *     model -- dao/dao.hibernate/entity/service/service.spring
 *     view -- struts.action/struts.actionform/taglib
 *     controller -- servlet/utils
 * 
 *
 * �������ߣ�   MyEclipse 
 * Web��������TomCat
 * J2EE
 * Struts -- MVC���
 * Spring
 * Hibernate

**************************************************************************************************************************************/

/**************************************************************************************************************************************
 * JavaEE�������·�Ϊ���ַ�ʽ��
 *   1.��SpringΪ���ĵ�������JavaEE��ҵ����ƽ̨
 *   2.��EJB3 + JPA Ϊ���ĵľ���JavaEE����ƽ̨
 *   
 * � -- http://jingyan.baidu.com/article/6181c3e054a42d152ff1535c.html
 * 
 * SSH���(WebProject) -- Struts + Spring + Hibernate
 *   Ŀǰ�����е�һ��WebӦ�ó���Դ���ɿ�ܣ����ڹ�����������չ�Ķ��WebӦ�ó���
 *   Struts -- ���� Servlet/JSP ������ϵͳ����������ܹ�������MVC�ķ��룬����ҵ����ת
 *   Spring -- �������Ŀ��Ʒ�תIoC����(����������)����������(AOP)��������ܣ�������ҡ���λ�������͹�����󼰶���֮���������ϵ
 *   Hibernate -- �Գ־ò��ṩ֧��
 *   
 * ��ְ���Ϸ�Ϊ�Ĳ㣺
 *   ��ʾ��(UI Layer)--Struts Action + ActionForm + JSP + struts.xml 
 *   ҵ���߼���(Business Layer)--Spring + Transactions + Hibernate Session Management + Business Service Classes
 *   ���ݳ־ò�(Persistence Layer)--Hibernate + DataSource/Connection Pool + SQL + Hibernate Services
 *   ��ģ���(ʵ���)-- Domain Model Business Objects 
 *   
 * ҵ�����̣�
 *   UI  ��: JSP�û�����  -> Request/Respone -> Structs/Structs2���� struts-config.xml(1)/struts.xml(2) ��ActionServlet/ActionSupport���յ���Requestί�ɸ���Ӧ��Action
 *   ҵ���: Spring IoC����������Action�ṩҵ��ģ��(Model)��� �� ��Ӧ��DAO��� ���ҵ���߼������ṩ����������ص��������������ϵͳ���ܺͱ�֤���ݵ�������
 *   �־ò㣺������Hibernate�Ķ���ӳ������ݿ⽻��������DAO�����������ݣ������ش�����
**************************************************************************************************************************************/

/**************************************************************************************************************************************
* ���ֳ��ù��ܶ�Ӧ�İ�
*   Mail(�շ��ʼ�) -- activation + mail
*   Oracle(���ݿ�) -- classes12
*   Json -- �ü���ʵ�֣�http://www.json.org/��
*     org.json.jar; 
*     json-lib -- http://sourceforge.net/projects/json-lib/
*     google-gson -- 
* 
**************************************************************************************************************************************/


public class JavaEEStudy {

}
