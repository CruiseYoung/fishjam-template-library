package com.fishjam.storeserver.study;

/**************************************************************************************************************************************
 * Android sqlite���ݿ����ͨ�ÿ��AHibernate(һ)-CRUDʾ����ʹ�ò���
 *   http://blog.csdn.net/lk_blog/article/details/7455992
 * 
 * Hibernate -- ��Դ�Ķ����ϵӳ�估�־û���ܣ���JDBC�����˷ǳ��������Ķ����װ������ʹ�ö�����˼ά���������ݿ⡣
 *   ����Ӧ�����κ�ʹ��JDBC�ĳ��ϡ�����������Ӧ��EJB��J2EE�ܹ���ȡ��CMP��������ݳ־û������Ρ�
 *   ʹ�� Java ���������ʵ��͸����
 *   ʹ����J2EE�ܹ��еģ�JDBC��JTA��JNDI �ȼ���
 * ��ܽṹ��Session Bean <-> DAO <-> Hibernate <-> DB
 * �ŵ㣺
 *   1.��JDBC�������ݿ�Ĵ������˷�װ�����������ݷ��ʲ㷱�����ظ��Դ���
 *   2.��һ�������ORMʵ�֡�������DAO��ı��빤�� ��ʹ����������
 *   3.��ֲ�Ժã�֧�ָ������ݿ⣬����������ݿ�ֻҪ�������ļ��б任���þͿ����ˣ����øı�hibernate����
 *   4.֧��͸���־û�����Ϊhibernate�������Ǵ���ģ�pojo��java�࣬û��ʵ���κνӿڣ�û��������
 *   5.���������κ�JDBC����ʹ�õĳ���(��ʹ�ֻ���?)
 *   6.֧�ָ��ֹ�ϵ���ݿ⣬��һ��һ����Զ�ĸ��ָ��ӹ�ϵ
 * ȱ�ݣ�
 *   1.��������ʹ�õĶ���ģ�͡�(���磬һ���־����಻��ӳ�䵽�����)
 * 
 * ORM(����-��ϵӳ��) -- ��ɶ������ݵ���ϵ����ӳ��Ļ���, �� Java�е�User�� <==> ���ݿ��е�Tbl_User��
 *
 * Hibernate���������ֻ����£�
 *  �ɹ����� -- ���ֻ����ɹ���������Դ������Դ�����ݿ����ӳأ����񡢰�ȫ����ȡ�һЩ���͵�J2EE��������JBoss��Weblogic��WebSphere���Ѿ�ʵ������Щ��
 *  ���ɹ����� -- ֻ���ṩ��һЩ�����Ĺ��ܣ�������Jetty��Tomcat������servlet����������
**************************************************************************************************************************************/

/**************************************************************************************************************************************
 * hibernate.cfg.xml 
 *   <mapping resource="com/xxx/User.hbm.xml"
 *
 * Dialect -- ���ԡ�hibernateΪ�˸��õ�������ֹ�ϵ���ݿ⣬���ÿ�����ݿⶼָ����һ������Dialect��������Java����ת���ɹ�ϵ���ݿ⡣
 *   ������һ�� �� org.hibernate.dialect.Dialect �̳е��࣬
 *
 *   �ٷ�Ŀǰ����֧��ʹ�� SQLite ���ݿ�, ��Դ������dialect(https://code.google.com/p/hibernate-sqlite/)
 *     ʹ�ã�<property name="dialect">com.applerao.hibernatesqlite.dialect.SQLiteDialect</property>
 *     Bug:1.��֧�ַ�ҳ��ѯ( where xxx limit xxx offset yyy )
 *  
***************************************************************************************************************************************/

/**************************************************************************************************************************************
 * ���ýӿ�
 *   Callback -- 
 *   Configuration -- ��ȡ���ô���SessionFactory����
 *     buildSessionFactory()
 *   Criteria -- ��������ִ���������ı�׼����ѯ
 *   Query -- ����ض����ݿ⼰�־ö�����в�ѯ�������ֱ�﷽ʽ��HQL���Ի򱾵����ݿ��SQL��䡣
 *            ���������󶨲�ѯ���������Ʋ�ѯ��¼������������ִ�в�ѯ������
 *   Session -- ���������࣬���������ٶ�����ռ�úܶ���Դ
 *     beginTransaction()
 *     getTransaction().commit()
 *   SessionFactory -- ����ģʽȡ��Session��ʵ����ͨ����ʵ��������Ӧ���й���
 *     openSession()
 *   Transaction
**************************************************************************************************************************************/

/**************************************************************************************************************************************
* �������ݿ�
* sqlite��cд�ģ�û�йٷ��Ĳ���sqlite��java api�����Ҫ��Java��ʹ��Sqlite�Ļ����м��ַ�ʽ��
*     1.SQLite JDBC Driver(https://bitbucket.org/xerial/sqlite-jdbc)����(�� sqlite-jdbc-3.7.2.jar)
*       ��Javaʵ�֣����㣬��Ч�ʽϵ�
*     2.SQLite Java Wrapper/JDBC Driver( http://www.ch-werner.de/javasqlite/)�� ��Ҫ���ؿ�
*   ֧���ڴ����ݿ�:  Connection connection = DriverManager.getConnection("jdbc:sqlite::memory:");
**************************************************************************************************************************************/

public class HibernateStudy {

	public testSqliteDemo(){
		/*
		try {
			String fileName = "c:/SqliteDemo.db";  // Linux: /home/myuser/SqliteDemo.db
			Class.forName("org.sqlite.JDBC");
			// Create Connection Object to SQLite Database, If you want to only create a database in memory, exclude the fileName
			Connection conn = DriverManager.getConnection("jdbc:sqlite:" + fileName);
			// Create a Statement object for the database connection, dunno what this stuff does though.
			Statement stmt = conn.createStatement();
			
			//stmt.executeUpdate("drop table if exists tbl_User");
			//statement.executeUpdate("create table person (id integer, name string)");
			
			// Create a result set object for the statement
			ResultSet rs = stmt.executeQuery("SELECT * FROM tbl_User");
			
			// Iterate the result set, printing each column if the column was an int, we could do rs.getInt(column name here) as well, etc.
			while (rs.next()) {
				int id = rs.getInt("Id");					// Column 1
				String username = rs.getString("UserName"); // Column 2
				String password = rs.getString("Password"); // Column 3
				System.out.println("ID: " + id + " username: " + username + " password: " + password);
			}
			System.out.println("End loop table");
			// Close the connection
			conn.close();
		} catch (Exception e) {
			// Print some generic debug info
			System.out.println(e.getMessage());
			System.out.println(e.toString());
		}
		*/
	}

}
