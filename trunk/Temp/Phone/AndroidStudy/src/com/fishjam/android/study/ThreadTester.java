package com.fishjam.android.study;
import java.util.Date;
import java.util.Timer;
import java.util.TimerTask;

import android.os.Handler;
import android.os.Message;
import android.test.AndroidTestCase;
import android.util.Log;

/**************************************************************************************************************************************
 * Android ��View��UI��������̰߳�ȫ�ģ�������ֱ�����߳��з���UI��һ��ͨ�� Handler ������Ϣ�����
 * 
 * Thread -- ʵ�� Runnable �ӿ�֧�ֶ��̣߳�  new Thread(Runable).start();
 *   while(!Thread.currentThread().isInterrupted()) { ... } 
 *   
 *   AsyncTask<Params, Progress, Result> -- ��̨���첽�����࣬��������ģ�棬�粻��Ҫ������Ϊvoid��
 *     http://blog.csdn.net/hitlion2008/article/details/7560878 -- ��AsyncTask������������ͼƬ��ͬʱ��ʾ���ȣ�������ͼƬ����UI
 *     ִ�к�̨��������������͸�ǰ̨�̣߳������ڶ�ʱ������UI�н����Ĳ�����
 *     ע�⣺һ�����������е� AsyncTask ��ͨ���̳߳�˳��ִ�У������߳�ͨ��Handler����
 *     doInBackground(xxx...params) -- �̳�������Ҫ���صľ��幤�����������û����� execute ���Զ�ִ�и÷���
 *     onProgressUpdate(xxx) -- ���ȸ��µĻص�����ʱ����ֱ�Ӹ���UI���� doInBackground ��ͨ������ publishProgress ����ͳ�� 
 *     execute(����) -- ���ÿ�ʼ�첽ִ�У� 2.3��ǰͬʱ���5���߳�(THREAD_POOL_EXECUTOR), 3.0�Ժ����1���߳�(SERIAL_EXECUTOR)
 *     executeOnExecutor -- 3.0���������������ṩ�Զ�����̳߳������к͵���
 *       executeOnExecutor(AsyncTask.THREAD_POOL_EXECUTOR) �ȼ���2.3��ǰ�� execute()
 *
 *   Handler + Message -- �첽������ơ�Android�н�ֹ�߳���ֱ�Ӹ���UI�������Ҫͨ������ Message �����߳��У���Handle��ȡ��Message����¡� 
 *     ʹ��ʱֻ�� new Handler ������handleMessage ������ �������� switch(Message.what) { } ������( ����Ҫ set ��Activity�� )
 *     content--
 *     sendEmptyMessage -- ������Ϣ
 *     TODO: HandlerLeak -- ? ͬһ���߳��µ�handler����һ��looper������Ϣ�б����˶�handler�����ã�ֻҪ����Ϣ�ڶ����У���ôhandler���޷�������
 *     
 *   Timer -- ��ʱ��
 *     schedule(new TimerTask() { run }, 0, 1000 ); -- ������ִ������
 *     
 * �������(java.util.concurrent)
 *   Executor��� -- �����̳߳أ�Executor��Executors��CompletionService��Future��Callable��
 *   Executor -- �ӿ�
 *   Executors -- ��Ҫ�ṩһЩ��̬�����Ĺ�����
 *     newCachedThreadPool() -- ����һ�����贴���߳�(��������)���̳߳�
 *     newFixedThreadPool(n) -- ָ���̸߳�����[�̶�?]�̳߳�
 *     newScheduledThreadPool -- 
 *     newSingleThreadExecutor() -- ����һ��ֻ��һ����̨�߳���ִ�����ύ������̳߳�(3.0�Ժ�ȱʡ��)
 *   ExecutorService -- ͨ�� Executors ���ϵͳ�ṩ�ĸ����̳߳�
 *   ThreadPoolExecutor -- �̳߳أ��������� corePoolSize/maximumPoolSize ��
 *   FutureTask
**************************************************************************************************************************************/

public class ThreadTester extends AndroidTestCase{
	private final static String TAG = ThreadTester.class.getSimpleName();
	
	public void testHandleMessage(){
		Handler handler = new Handler(){
			@Override
			public void handleMessage(Message msg) {
				switch (msg.what) {
				case 100:
					Log.i(TAG,  "receive 100 message");  //ͨ�����Ը���UI���� view.setBackgroundResource(xxx)
					break;
				default:
					break;
				}
				super.handleMessage(msg);
			}
		};
		handler.sendEmptyMessage(100);
	}
	
	public void testTimer(){
		new Timer().schedule(new TimerTask() {
			@Override
			public void run() {
				Log.i(TAG,  "In Timer shcedule " + new Date().toString() );
			}
		}, 0, 1000);
	}
	public void testExecutor(){

		//Executor.execute(Runnalbe) ��Executor��ִ��ʱʹ���ڲ����̳߳���ɲ�����
	}
	public void ThreadDemo(){
		//�̺߳�ִ̨�У�ǰ̨��ʾ���ȶԻ���
		/**************************************************************************************************************************************
		 m_myDialog = ProgressDialog.show(EX03_18.this,strDialogTitle,strDialogBody, true);
		 new Thread()
		  {
		    public void run()
		    {
		      try
		      {
		        sleep(3000); //��̨���еĳ���Ƭ�Σ��˴�����ͣ3����Ϊʾ��
		      }
		      catch(Exception e)
		      {
		        e.printStackTrace();
		      }
		      finally
		      {
		         myDialog.dismiss(); //ж����������m_myDialog����
		      }
		    }
		  }.start();//��ʼ�����߳�
		//**************************************************************************************************************************************/
	}
}
