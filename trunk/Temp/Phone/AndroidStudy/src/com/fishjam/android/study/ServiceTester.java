package com.fishjam.android.study;
import java.io.FileDescriptor;

import android.app.Service;
import android.content.Intent;
import android.os.Binder;
import android.os.IBinder;
import android.os.IInterface;
import android.os.Parcel;
import android.os.RemoteException;
import android.test.AndroidTestCase;
import android.test.ServiceTestCase;
import android.util.Log;

/**************************************************************************************************************************************
 * AIDL(Android Interface Definition Language) -- Android�ӿ� �������ԡ�
 *   �ͻ��˺ͱ�����ʵ��֮����ͨ������ģʽ��������ʵ�֣�������ͱ������඼ʵ�� IBinder �ӿ�
 *   Stub -- ����࣬ͨ���� asInterface ����Ŀ��ӿ�(ͨ������ģʽ����Java�ӿڵ�ʵ��)
 *   Proxy -- ������
 * 
 * ʹ��RPC������
 *   1.ʹ��AIDL����ӿ�(�� IMath)�� ��չ��Ϊ .aidl;
 *   2.ʹ�� aidl.exe  ���߲���Java�ӿ�ʵ�ִ���(ADT ���Զ���genĿ¼������)
 *   3.����ʵ����, IMathImpl extends IMath.Stub { ʵ�־���ķ��� }
 *   4.���ӿڱ�¶���ͻ��� -- Service::onBind �з��ظýӿڵ�ʵ��
 *   5.�ͻ��� �� ServiceConnection::onServiceConnected �� IMath iMath = IMath.Stub.asInterface(ibinder);     
 *   AIDL������:   
 *     interface IMath{   //������ public interface IPerson extends android.os.IInterface { ... } �Ľӿ�ʵ�ֻ���
 *       int Add(int  a, int b);
 *     }
 *
 * 
 *   ע�⣺
 *     1.���뵼����ڽ���������κ��������͡�
 *     2.Զ�̵���ʱ�����׳� RemoteException �쳣����Ҫ����
**************************************************************************************************************************************/
/**************************************************************************************************************************************
 * 
 * ��дService -- �̳� Service �࣬���������������еķ��������� AndroidManifest.xml ��ͨ�� <service> Ԫ��������
 *   Service
 *     onBind -- ����ʵ�֣�����һ���󶨵Ľӿڸ�Service
 *     onCreate -- ��һ�δ���ʱ��ϵͳ����
 *     onStart -- ��ͨ�� startService ��������ʱ���÷���������
 *     onDestroy -- �����˳�ʱ��ϵͳ����
 *     stopSelf -- �����Լ������˳�?
 * ʹ��Service (ʹ�� Context ��ķ���)
 *   startService/stopService -- ����/ֹͣ Service
 *   bindService/unbindService -- ��/��� Service��һ������Զ��Service����
 *   ��ͨ�� RPC ������ʵ�ֲ�ͬ���̼� Service �ĵ���
 *   
 *   ServiceConnection -- ����Զ��Service����ʱ�Ļص��ӿڣ��� onServiceConnected/onServiceDisconnected ���ж����ӳɹ���Ͽ���
 *   IBinder -- 
***************************************************************************************************************************************/

class MyService extends Service{
	private final static String TAG = MyService.class.getSimpleName();
	
	 private final IBinder mBinder = new Binder(){
		 
	 };
	 
	@Override
	public IBinder onBind(Intent intent) {
		return mBinder;
	}

	@Override
	public void onCreate() {
		Log.i(TAG, "onCreate");
		super.onCreate();
	}

	@Override
	public void onDestroy() {
		Log.i(TAG, "onDestroy");
		super.onDestroy();
	}

	@Override
	public boolean onUnbind(Intent intent) {
		Log.i(TAG, "onUnbind");
		return super.onUnbind(intent);
	}

	@Override
	public void onRebind(Intent intent) {
		Log.i(TAG, "onRebind");
		super.onRebind(intent);
	}
}


public class ServiceTester  extends ServiceTestCase<MyService> {

	public ServiceTester() {
		super(MyService.class);
	}

	public void testMyService(){
		  Intent startIntent = new Intent();
	      startIntent.setClass(getContext(), MyService.class);
	      startService(startIntent);
	      
	      MyService myService = getService();
	      assertNotNull(myService);
	      
	      IBinder serviceBinder = bindService(startIntent); 
	      assertNotNull(serviceBinder);
	}
}