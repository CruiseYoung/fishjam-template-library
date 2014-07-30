package com.java.tester;

import static org.junit.Assert.*;

import org.junit.Test;
import org.omg.CORBA.PRIVATE_MEMBER;


/***************************************************************************************************************
 * TODO
 *   1.容器中迭代 : SomeType [] values = xxx; for(SomeType v : values) { v.Xxx(); }
 * 
 ***************************************************************************************************************/
/****************************************************************************************
 * 子类 extends 父类  implements 接口1,接口2
 * 
 * Java的访问控制分为： public > protected(同一package中其他class也可以访问) >
 * package/friendly(同一个包内可以访问，其他包内同 private) > private
 * 
 * Java 是单根继承，所有类有共同的父类(Object)，有共同的接口 -- GC(垃圾回收机制)实现更容易；完全通过引用操作对象；都有RTTI，易于异常处理
 * 子类调用父类的方法：super.方法名(参数)；如果是构造函数中调用父类构造，则是 super(参数)
 * 
 * C++成员函数缺省是非virtual的(前期绑定)，而java的成员函数缺省是virtual的(后期绑定),子类覆盖实现时，最好加上 @Override，可以发现名字不匹配的问题。
 * abstract -- 定义抽象类(有部分实现，但不可实例化)，并指定其中的抽象成员函数 
 * interface -- 只有函数定义，不能有任何实现和成员变量，是接口和实现分离的完美境界
 * 
 * 对象相等的测试： == 和 != 判断对象引用是否相同，
 * equals 判断内容是否相同（Object缺省的实现是比较引用），可以override，标准库中很多类（如基本类型的Wrapper类、String等）都进行了override
 * 
 * 位运算符 -- 操作基本整数类型中个别的位(bits)，&(与)、|(或)、^(异或)、~(取反)，
 * 如~0x1234(4660)=0xFFFFEDCB(-4661) 位移运算符 -- <<、>> 只能用于基本整数，左移(补0) 、右移(符号位扩展) >>>、右移(无论原值正负，一律在较高处补0)
 * Java 不允许程序员实现自定义的运算符重载 -- Java库唯一实现了String的+重载？
 * Java没有sizeof运算符 -- 在所有的机器上，每种数据类型有相同的容量大小，可以直接计算出来，易于移植
 * 
 * 容器 -- 迭代器 -- 选择容器中的元素，将容器实现细节和对容器进行访问动作之程序代码分离开来，可轻易更改底层结构(如容器类型)，而不干扰应用程序代码.
 * 模版(哪个版本后实现？) -- 参数化类型，降低向下转型的需求及可能导致的错误（模版之前使用Object保存对象，需要转型）
 * 
 * Java 在程序语言中内置了线程，线程功能在对象层次上提供(C/C++是通过函数提供的)，一个线程由一个对象表示，也提供有限
 * 的资源锁定功能，可锁定任何对象所有的内存(synchronized) -- 通常做法是产生一个对象代表欲锁定的资源，所有线程在访问
 * 这份资源之前都必须加以检测。
 * 
 * 持久性 -- Java提供轻量级持久性(light weight persistence)，可轻松地将对象存储于磁盘，并于日后取出
 * 
 * 类定义中，成员变量可在定义时初始化或构造中初始化，如没有，则由Java设置初始值(0或null)
 * Java源文件中最多只能有一个public类，该类的类名必须和文件名相同
 * 基本类型(primitive type)用自动变量(automatic)方式解决效率问题；基本类型有所谓的包装类(Wrapper classes)，可用new创建
 * 高精度计算(如精确金融)使用 -- BigInteger 和 BigDecimal(任意精度的定点数)
 * 
 * Java的applet使用sandbox(砂盒)的方式，applet无法写、删除文件，如果数字签名了的话可以不受限。 
 *   可以使用SecurityManager--设定Java的安全策略 
 *   Java在服务器端，编写servlets及其衍生物JSP，可消除因浏览器能力不同造成的问题
 * 
 * 嵌套类可以直接访问嵌套它的类的成员，包括private成员，但是，嵌套类的成员不能被嵌套它的类直接访问。
 *   在内部类对象保存了一个对外部类对象的引用，当内部类的成员方法中访问某一变量时，如果在该方法和内部类
 *   中都没有定义过这个变量，内部类中对this的引用会被传递给那个外部类对象的引用 (即 this.变量 不存在时会传递 Outer类.this.变量)。
 *   如果内部类被定义为static，则不能访问外部类的变量(少用static内部类)
 * 
 * Java不具备条件编译的能力 -- 可以用package来模拟(参考 C#?) -- 编译不同的实现，并放在不同的package中。 
 * 可以使用 this在构造函数中调用另一个构造函数或private的初始化函数（以免编写重复代码，注意：由于Java默认是虚拟函数，
 *   因此在构造函数 中不能调用protected/public等可以被override的函数），C++不允许(调用一个共同的初始化函数)。
 * 
 * Java的数组在定义时不能指定大小，在new或初始化时才能指定，如果数组成员是对象，初始化前都是null。Java的多维数组的维数可以不同，是真正的数组的数组。
 * 
 * Java初始化顺序： 
 *   装载.class文件->产生Class对象->查找父类->初始化static变量->new出变量->在heap上分配存储空间->
 *   将其中的值全部初始化为0 ->按定义顺序执行数据定义处的初始化(或初始化block)->构造函数(如有this调用，则调用对应的重载构造)
 * 
 * 每个Class都可以有一个public的main -- 可以使每个Class的单元测试更容易？
 * 
 * final -- 用于不能被改变处(类似C++中的const)。可用于：
 *   data -- 编译器常量(static)；或执行期被构造函数初始化后不能再更改。如果作用于对象引用，表示指向的目标不变，但目标的内容可变；
 *   argument -- 参数中声明为常量，函数实体中不可更改 
 *   method -- 1.锁住函数，使子类无法override(即成为非虚函数)；
 *             2.效率，可被编译器转化为内联函数。
 *             Class中所有的private函数自动是final的 class -- 不能被继承，主要基于设计上的考虑（如不想变动）或为了安全、保密
 * 
 * 强制类型转换：((子类名)对象).方法
 * 
 * innerClass(内嵌类) -- 除有代码隐藏功能外，更可和外部类沟通（典型使用：Swing中的event机制） 
 *   private内嵌类 使设计者完全避免任何"与型别相依的程序代码"，并得以完全隐藏实现细节
 *   innerClass还可置于函数之内(实现某接口并返回匿名对象)或任意程序范畴内(语法为： return new 接口名() {方法定义和接口实现};)
 * 
 * 匿名类不能有构造函数(没有类名)，可用括号括起来进行实体初始化，通常会调用super。匿名内部类如想调用外部的对象(如参数、外部类变量)则该对象必须是final的。
 *   可访问外部对象的所有成员(即使是private的)，编译器会编译成 "外部类名.this.变量" 
 *   内部类在创建时，一定要关联至某个外部类对象（C++没有这个限制，因此内部类不能访问外部类的变量），
 *   外部创建时的语法为： 外部类名.内部类名 内部变量名 = 外部变量.new 内部类名。
 *   使用innerClass的理由：
 *     1.每个内嵌类都能够各自继承某一实现类或接口，不受限于外部类是否已继承自某一实现类。
 *     2.内部类机制才能有效而实际地允许"多实现继承" -- 在单一的外部类中，可以拥有多个内嵌类，每个都实现相同的interface或以不同的方式继承同一个class
 * 
 * Java泛型－－SE 1.5引入的新特性，有 泛型类、泛型接口、泛型方法 
 *   1.泛型的类型参数只能是类类型（包括自定义类），不能是简单类型，即默认 <T extends Object> 
 *   2.泛型的参数类型可以使用extends语句，例如<T extends superclass>。 习惯上称为"有界类型"
 *   3.为了解决类型被限制死了不能动态根据实例来确定的缺点，引入了"通配符泛型"。 
 *     例如 MyStack<? extends Collection> typeStack = new ... 
 *     3.1 如果只指定了<?>，而没有extends，则默认是允许Object及其下的任何Java类了。也就是任意类。
 *     3.2 通配符泛型不单可以向下限制,还可以向上限制，如<? super Double>，表示类型只能接受Double及其上层父类类型
 *   4.泛型方法--只需将泛型参数列表置于返回值前，如 public <T> void fun(T x)
 * 
 * 远程调用(RMI--Remote Method Invocation)，通过存活于远程机器上的对象，发送消息并执行。
 *   interface->本地Stub->网络 远端接口必须继承自rmi.Remote，而且远端接口的函数都必须声明可抛出RemoteException，
 *   RMISecurityManager rm = new RMISecurityManager(); //产生并安装支持RMI的安全管理器
 *   //产生一个或一个以上的远端对象实体 Naming.bind(name, obj); //向远端对象注册点注册远端对象
 *   注意：需要一个服务--rmiregistry
 * 
 * CORBA(Common Object Request Broker Architecture)--使用其他语言撰写执行于其它架构上的程序代码，
 *   是一种OMG定义的一套和语言无关的分布式"对象交换标准框架"，提供了Java对象和非Java对象的 远端程序调用(RPC)能力。
 * 
 * Java中检测密码(必须有数字、大、小写、特殊符号等，长度8-30)的正则表达式:
 *   "((?=.*\\d)(?=.*[a-z])(?=.*[A-Z]).{8,30})"
 * 
 * JNI调用库？： system.loadLibrary("MyLib);
 ****************************************************************************************/

public class JavaLanguageTester {

	private String myContacStrings(String... ss)
	{
		StringBuilder sBuilder = new StringBuilder("[");
		boolean bAppened = false;
		boolean bMoreThanOne = (ss.length > 1);
		for (String string : ss) {
			if (bAppened ) {
				if (bMoreThanOne) {
					sBuilder.append(", ");
				}
			}
			else {
				bAppened = true;
			}
			sBuilder.append(string);
		}
		sBuilder.append("]");
		return sBuilder.toString();
	}
	
	@Test
	public void testArrayParam() {
		String testString = myContacStrings("one", "two", "three");
		System.out.println(testString);
		
		assertEquals(testString, "[one, two, three]");
		//fail("Not yet implemented");
	}
	
	//测试字符串
	@Test
	public void testString(){
		String strChinese = "中华人民共和国";
		try {
			byte[] rawBytes = strChinese.getBytes("UTF-8");
			String strUTF8 = new String(rawBytes, "UTF-8");
			assertEquals(strChinese, strUTF8);
		} catch (Exception e) {
			e.printStackTrace();
		}
		
		{
			//String.split
			String splitString = "one, two, three";
			String strs[] = splitString.split(",");
			assertEquals(strs.length, 3);
			
			assertEquals(strs[0].trim(), "one");
			assertEquals(strs[1].trim(), "two");
			assertEquals(strs[2].trim(), "three");
		}
	}
	
	@Test
	public void testParse(){
		assertEquals(Integer.parseInt("123"), 123);
		assertEquals(Integer.parseInt("0123"),  123);
		assertEquals(Integer.parseInt("0123", 10), 123);
		assertEquals(Integer.parseInt("0123", 8), 83);

		boolean bThrowException = false;
		try {
			int a = Integer.parseInt("A0123B", 10);	//解析字符串时，会抛出异常
		} catch (NumberFormatException e) {
			bThrowException  = true;
			//e.printStackTrace();
		}
		assertTrue(bThrowException);
	}
	

	@Test
	public void testSingleInstance(){
		/*
		 * 单实例：处于线程安全考虑，getInstance() 使用了一种被称为 "initialization on demand holder" 的技术来创建单例类的实例
		class Stage{
			private Stage() {}
		}
		private static class StageSingletonHolder{
			static Stage instance = new Stage();
		}
		public static Stage getInstance() {
			return StageSingletonHolder.instance;
		}	
		*/
	}
}
