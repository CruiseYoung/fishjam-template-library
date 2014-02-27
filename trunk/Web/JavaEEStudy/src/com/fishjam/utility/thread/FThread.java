package com.fishjam.utility.thread;

/*******************************************************************************
 * ���кܶ๦��û��ʵ�ֺͲ��� -- ���磺�����������жϡ�Sleep��ʱ�������
 * 
 * Java��ʵ�ֶ��߳�������;�����̳�Thread�����ʵ��Runnable�ӿ�
 * ʹ�� synchronized �ؼ���ͬ������ؼ��Ƕ���̶߳�����ͬһ��������Դ
 *   1.����������Ϊ synchronized,ͬ����������̳�(�����ǣ����಻���Զ���)
 *   2.ʹ���ض���������ָ�������ͬ��(�ؼ���)
 * 
 * wait() ������synchronized �������ߴ��������,�����Ѿ����synchronized ��������
 *   ��������Ȩ��Thread��ʱ��Ϣ�������Զ��ͷ�synchronized��������ͬ������/block�ɱ�����
 *     
 * �����׳� IllegalMonitorStateException �쳣--ֻ�ܶ��Լ�ӵ�����Ķ������wait/notify 
 * 
 * daemon�߳� -- �ڱ����ṩͨ���Է�����̣߳�ʹ��setDaemon�л������߳̽��������е�
 *   daemon�̻߳��Զ�ǿ����ֹ���������򡣶�����з�daemon�̣߳�������ȴ��������
 * 
 * ThreadGroup -- �߳��飬���е��̶߳�������ĳ���߳��飬ȱʡΪϵͳ�߳���
 ******************************************************************************/

//Java�߳���صķ���ͨ���׳� InterruptedException �쳣���������̿���--��Ʋ

public abstract class FThread implements Runnable
{
    public enum ThreadWaitType
    {
        twtContinue, twtTimeOut, twtStop, twtError
    };

    public FThread()
    {
        this(new Object());
    }

    public FThread(Object syncObject)
    {
        m_syncObject = syncObject;
        m_stop = false;
        m_continue = true;
    }

    public void start()
    {
        m_stop = false;
        m_continue = true;
        m_thread = new Thread(this);
        m_thread.start();
    }

    public void pause()
    {
        //�Ƿ���Ҫͬ������Ϊ����Ҫ���ѣ����Բ���Ҫ��
        System.out.println("pause");
        m_continue = false;
    }

    public void resume()
    {
        synchronized (m_syncObject)
        {
            System.out.println("resume");
            m_continue = true;
            m_syncObject.notifyAll();
        }
    }

    public void stop()
    {
        synchronized (m_syncObject)
        {
            System.out.println("stop");
            m_stop = true;
            m_syncObject.notifyAll();
        }
    }

    public void join()
    {
        try
        {
            //Thread t = m_thread;
            //m_thread = null;
            //t.join();
            
            //û��ʹ��ͬ����ʹ����һ�ַ�ʽ���ܳ���
            m_thread.join();
            m_thread = null;
        } catch (InterruptedException e)
        {
            e.printStackTrace();
        }
    }
    
    public ThreadWaitType getWaitType()
    {
        ThreadWaitType waitType = ThreadWaitType.twtContinue;
        try
        {   
            //����Ƶ���� -- ��ϸ���� m_stop��
            if(/*!m_stop && */!m_continue)
            {
                synchronized(m_syncObject)
                {
                    while (!m_stop && !m_continue)
                    {
                        m_syncObject.wait();
                    }
                }
            }
            if (m_stop)
            {
                waitType = ThreadWaitType.twtStop;
            }
        } catch (InterruptedException e)
        {
            //ʲôʱ��ᷢ����
            waitType = ThreadWaitType.twtError;
            e.printStackTrace();
        }
        return waitType;
    }

    public ThreadWaitType sleepAndCheckStop(long timeout)
    {
        ThreadWaitType waitType = ThreadWaitType.twtContinue;
        synchronized (m_syncObject)
        {
            try
            {
                m_syncObject.wait(timeout);
            } catch (InterruptedException e)
            {
                e.printStackTrace();
            }
        }
        return waitType;
    }
    
    private volatile boolean m_stop;
    private volatile boolean m_continue;
    private Thread m_thread;
    private Object m_syncObject;
}
