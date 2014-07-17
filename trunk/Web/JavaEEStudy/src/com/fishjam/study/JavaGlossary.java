package com.fishjam.study;

/************************************************************************************************************
 * AOP(Aspect-Oriented Programming) -- 面向切面编程
 * BO(Business Object) -- 业务对象，可以理解为
 * CORBA(Common Object Request Broker Architecture) -- 一种和语言无关的分布式"对象交换标准框架"，
 * CRUD -- Create + Retrieve + Update + Delete
 * CVS(Concurrent Versions System) -- 版本控制工具CVS，已被SVN替代
 * DAO(Data Access Object) -- 数据访问对象
 * DBCP -- 数据源？
 * DI(Dependency Injection) -- 依赖注入
 * DMI(Dynamic Method Invocation) -- 动态方法调用， struts2中指定action不同的请求为 "ActionName!methodName" 
 * DTO(Data Transfer Object) -- 数据传输对象，JavaEE中，中间层组件将应用底层的状态信息封装成名为DTO的JavaBean，并传送到JSP页面进行显示。
 * EJB(Enterprise Java Bean) -- (将事务逻辑和连接的问题隔离),已被 JPA 替代？
 * GC(Garbage Collector) -- 垃圾回收
 * JAAS(Java Authentication Authorization Service) -- Java验证和授权服务，用于控制对 Java Web 应用的授权访问，
 *   在 web.xml 中的 <security-constraint>， <login-config> 等处体现 
 * JDK(Java Development Kit) -- 按Java的应用环境划分
 *   J2EE(Java 2 Platform Enterprise Edition) -- 企业Web应用开发
 *   J2SE(Java 2 Platform Stand Edition) -- 普通应用程序
 *   J2ME(Java 2 Platform Micro Edition) -- 手机等消费电子产品
 * JDO(Java Data Object) -- Java数据对象，是Java EE标准中出JPA外，另一个支持管理持久化数据的规范。 
 * JINI(Java Intelligent Network Infrastructure) -- 能轻松动态地在代表本机系统的网络中加入或移除硬件设备
 * JIT(Just-In-Time) -- JIT编译器，在运行时按需编译，可以减少编译时间和运行时的程序大小，但初次运行时较慢?
 * JMS -- (发送和接收异步消息的?)
 * JMX -- (管理应用程序对象? )
 * JNDI(Java Naming Directory Interface) -- Java命名和目录接口，为某个Java对象起一个名字，然后可通过该名字来访问。
 *    访问方式(JSP页面脚本)： Context ctx = new InitialContext(); DataSource ds = (DataSource)ctx.lookup("java:comp/env" + "自定义名字全路径");
 * JNI(Java Native Interface) -- Java原生接口，用以和C/C++程序接轨，获得高性能
 * JPA(Java Persistence API) -- 是sun公司官方提供的Java持久化的解决方案
 * JSF() -
 * JSP(Java Server Page) -- 最终会编译成Servlet才能运行
 * JSR(Java Spec. Request) -- Java 规格需求
 * JSTL(JSP Standard Tag Library) -- Sun提供的一套标签库，用于IO、流程控制、XML文件解析、数据库插叙及文字格式标准化等
 * OGNL() -- 
 * ORM(Object Relation Mapping) -- 对象-关系映射，完成对象数据到关系数据映射的机制, 如 Java中的User类 <==> 数据库中的Tbl_User表。
 *   最早的ORM是 Entity EJB，但非常麻烦。目前流行的ORM框架有Hibernate、iBatis、TopLink等
 * OSGi(Open Service Gateway Initiative) --
 * PO(Persistent Object) --  持久对象，可以理解为 数据库模型通过映射转化成的持久化类，通常和数据库的表对应 
 * POJO(Plain Old Java Object) -- 普通的传统的Java对象，其中有一些属性及其getter setter方法的类,没有业务逻辑。
 *   不允许有业务方法,也不能携带有connection之类的方法。实际就是普通JavaBeans，是为了避免和EJB混淆所创造的简称 。
 * RAD() -- 快速应用开发
 * REST(Representational State Transfer) -- 表述性状态转移, 针对网络应用的设计和开发方式的一种软件架构风格
 * RMI(Remote Method Invocation) -- 远程调用，通过存活于远程机器上的对象，发送消息并执行。 
 * RTTI(Run-time Type Information) -- 运行时类型信息
 * SOAP -- 
 * SSH(Struts + Spring + Hibernate) -- SSH组合是一种轻量级的JavaEE平台，可运行在普通Web容器中，无需EJB容器的支持
 * TLD(Tag Library Definition) -- 标签库定义，用于JSP中，文件扩展名也是 tld，一个标签库中可包含多个标签
 * VO(Value Object) -- 值对象, 可以理解为了便于使用而在PO的基础上进行又一次加工而产生的对象, 通常用于业务层之间的数据传递，可以不和数据库的表对应 
 * WAP(Wireless Application Protocol) -- 无线应用协议
*************************************************************************************************************/
public class JavaGlossary {

}
