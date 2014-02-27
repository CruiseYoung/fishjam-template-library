package com.fishjam.study;

import java.io.IOException;

import com.fishjam.utility.thread.FThread;

/************************************************************************************************************
 * JSR(Java Spec. Request)--Java �������
 * 
 * ��װ��JDK�Ժ�Ҫ���� JAVA_HOME �� CLSS_PATH ��������
 *   CLASS_PATH -- ��һ������Ŀ¼��ÿ��Ŀ¼����Ϊ.class �ļ��Ĳ�����㣬�ڴ���ͨ��չ��package���Ƶ�·������class��
 *     ��װ������package����Ҫ����CLASS_PATH��jar�ļ�����Ϊ��Ŀ¼��Ҳ���Է��ڸñ����С�
 * 
 * һ�α���,�������� -- java�ֽ���(Code-Byte).class����JVM�����������
 * ClassPath ������������ java �������class��·��
 * 
 * 
 * JVM�����й��̣�������(ClassLoader)->�ֽ���У��(ByteCodeVerifier)->������->����ʱ����
 * JDK(Java Development Kit)��Java��Ӧ�û�������
 *   J2EE(Java 2 Platform Enterprise Edition) -- ��ҵWebӦ�ÿ���
 *   J2SE(Java 2 Platform Stand Edition) -- ��ͨӦ�ó���
 *   J2ME(Java 2 Platform Micro Edition) -- �ֻ������ѵ��Ӳ�Ʒ
 *
 * java�е�charռ�������ֽ�(unicode����)
 * Obfuscator(���������)--���ֽ���ת��Ϊһ���߼��ϵĶԵ�����õ��У�JODE��JavaGuard ��
 * JAD/FrontEnd--Java�����빤�ߣ����Եõ�Դ���롣
 *
 * Java�����Ż���
 *   1.����������ʱ�����ʹ��
 *   2.������ʱ�����ʽ��ΪNull
 *   3.����ʹ��StringBuffer,������String���ۼ��ַ���
 *   4.���û���������Int,Long,�Ͳ���Integer,Long����
 *   5.�������þ�̬�������--��̬��������ȫ�ֱ���,���ᱻGC����,���ǻ�һֱռ���ڴ�
 *   6.��ɢ���󴴽���ɾ����ʱ��
 * 
 * Java�м������(���������֡���Сд��������ŵȣ�����8-30)��������ʽ:
 *   "((?=.*\\d)(?=.*[a-z])(?=.*[A-Z]).{8,30})"
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
