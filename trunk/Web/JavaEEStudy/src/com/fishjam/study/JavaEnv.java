package com.fishjam.study;

import com.fishjam.utility.thread.FThread;

/************************************************************************************************************
 * ��װ��JDK�Ժ�Ҫ���� JAVA_HOME �� CLSS_PATH ��������
 *   CLASS_PATH -- ��һ������Ŀ¼��ÿ��Ŀ¼����Ϊ.class �ļ��Ĳ�����㣬�ڴ���ͨ��չ��package���Ƶ�·������class��
 *     ��װ������package����Ҫ����CLASS_PATH��jar�ļ�����Ϊ��Ŀ¼��Ҳ���Է��ڸñ����С�
 * 
 * һ�α���,�������� -- java�ֽ���(Code-Byte).class����JVM�����������
 * ClassPath ������������ java �������class��·��
 * 
 * 
 * JVM�����й��̣�������(ClassLoader)->�ֽ���У��(ByteCodeVerifier)->������->����ʱ����
 *
 * java�е�charռ�������ֽ�(unicode����)
 * Obfuscator(���������)--���ֽ���ת��Ϊһ���߼��ϵĶԵ�����õ��У�JODE��JavaGuard ��
 * JAD/FrontEnd--Java�����빤�ߣ����Եõ�Դ���롣
 *
 *
 * JavadocתΪCHM(΢��˾������һ�ְ����ĵ���ʽ,��Ҫ��װ HTML Help Workshop --
 *   http://msdn.microsoft.com/en-us/library/ms669985.aspx)
 *   1.Java�����ʽ���ͳһ����ʹ��GBK��UTF-8��������Javadocʱ����Ҫ��VM options���б�������
 *     -locale en_US -encoding UTF-8 -charset UTF-8
 *   2.ʹ��jd2chm���ߣ��ܹ�����hhc��hhk��hhp�ļ�����������ʹ��HHW���б༭������ָ��HTML��Ŀ¼����������һ��About.html��
 *     http://download.csdn.net/source/1024968 
 *   3.ʹ�� javadoc2chm-1.1.0.7.msi��ָ��  index.html ��·�����ɣ����Զ�����Ŀ¼������
 *
 ***********************************************************************************************************/
class MyStack<T>
{
	private T ob;
	public MyStack(T ob)
	{
		this.ob = ob;
	}
	@Override
	public String toString() 
	{
		return ob.getClass().getName() + ":" + ob.toString();
	}
}

class MyThread extends FThread
{
    @Override
    public void run()
    {
        int count = 10;
        while(count >0 && this.getWaitType()==ThreadWaitType.twtContinue)
        {
            System.out.println("Count=" + count);
            count--;
            this.sleepAndCheckStop(1000);
        }
    }
    
}
public class JavaEnv {
	
	public void testEnv() throws InterruptedException
	{
		MyStack<Integer> iStack = new MyStack<Integer>(10);
		System.out.println(iStack.toString());
		
		MyStack<String> sStack = new MyStack<String>("hello world");
		System.out.println(sStack.toString());
		
		//MyStack<Object> oStack = new MyStack<String>("����ʹ��ͨ�������");
		MyStack<?> oStack = new MyStack<String>("����ʹ��ͨ�������");
		System.out.println(oStack.toString());
		
		MyThread myThread = new MyThread();
		myThread.start();
		
		Thread.sleep(1000);
		myThread.pause();
		Thread.sleep(1000);
		myThread.resume();
		
		Thread.sleep(2000);
		myThread.stop();
		
		myThread.join();
		System.out.println("after thread end");
	}
}
