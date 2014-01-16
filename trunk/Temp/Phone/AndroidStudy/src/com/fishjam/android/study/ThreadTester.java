package com.fishjam.android.study;
import java.util.Date;
import java.util.Timer;
import java.util.TimerTask;
import java.util.concurrent.Executor;

import android.os.AsyncTask;
import android.os.Handler;
import android.os.Message;
import android.test.AndroidTestCase;
import android.text.StaticLayout;
import android.util.Log;

/**************************************************************************************************************************************
 * Android ��View��UI��������̰߳�ȫ�ģ�������ֱ�����߳��з���UI��һ��ͨ�� Handler ������Ϣ�����
 * 
 * Thread -- ʵ�� Runnable �ӿ�֧�ֶ��̣߳�  new Thread(Runable).start();
 *   while(!Thread.currentThread().isInterrupted()) { ... } 
 *   
 *   AsyncTask<byte[], String, String> -- �첽�����ࣿ����Ϊģ����ʽ? ������������첽����ͼƬ ?
 *     doInBackground(byte[]...params) -- �̳�������Ҫ���صľ��幤������ 
 *     execute(����) -- ���ÿ�ʼ�첽ִ��
 *
 *   Handler + Message -- (�첽������ƣ�) ������ Handler::handleMessage �������������� switch(Message.what) { } ���д���
 *     content--
 *     sendEmptyMessage -- ������Ϣ
 *     TODO: HandlerLeak -- ? ͬһ���߳��µ�handler����һ��looper������Ϣ�б����˶�handler�����ã�ֻҪ����Ϣ�ڶ����У���ôhandler���޷�������
 *     
 *   Timer -- ��ʱ��
 *     schedule(new TimerTask() { run }, 0, 1000 ); -- ������ִ������
 *     
 * �������
 *   Executor��� -- �����̳߳أ�Executor��Executors��ExecutorService��CompletionService��Future��Callable��
 *   Executor -- �ӿ�
**************************************************************************************************************************************/

public class ThreadTester extends AndroidTestCase{
	private final static String TAG = ThreadTester.class.getName();
	
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
