package com.fishjam.utility.net;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;
import java.net.ServerSocket;
import java.net.Socket;
import java.net.UnknownHostException;

/*******************************************************************************
* Java��socket��װ��stream����ʹ��I/Oָ��������ݽ���
* Ĭ������£�socket��read/write������������ĳɷ�������ʽ���ſ���select��
* 
* Http��һ����session���Ե�ͨ��Э�飬�������޷����ֻ�õ����������Ƿ�����ͬһʹ���ߣ�
*   Ŀǰ���кܶ෽ʽ׷��session���� Cookie�ࡢURL��д����
*   Session��������ͨ�����ӵķ���ˣ�Ŀ����ϣ�����ͻ����������������������л���ʱ��
*   �ܲ�׽�ͻ���������ݣ�������ֻ�����ڸ�session�����繺���б�
*   
* JSP������servlet�ϵ�һ��Java��׼������ƣ��ɽ���ҳ��HTML���ֺͶ��Java�������ϲ�
*   �ڵ�һ�ĵ��У���JSP�����״�װ��JSPʱ�����Զ����������������JSP��servlet������룬
*   ��������servlet������
*******************************************************************************/
public class FNet
{
    public void Server() throws IOException
    {
        ServerSocket ss = new ServerSocket(1000);
        Socket cs = ss.accept();
        BufferedReader in = new BufferedReader(
                new InputStreamReader(cs.getInputStream()));
        PrintWriter out = new PrintWriter(
                new BufferedWriter(
                        new OutputStreamWriter(
                                cs.getOutputStream())),
                        true); //println���Զ������������������Ҫ�Ȼ���������ע�����ݽ�����Ч�ʵ�ƽ��
        
        in.close();
        out.close();
        cs.close();
        ss.close();
    }
    
    public void Client() throws UnknownHostException, IOException
    {
        Socket s = new Socket("127.0.0.1",1000);
        //s.connect(endpoint);
        BufferedReader in = new BufferedReader(
                new InputStreamReader(s.getInputStream()));
        PrintWriter out = new PrintWriter(
                new BufferedWriter(
                        new OutputStreamWriter(
                                s.getOutputStream())),
                        true);
        
        in.close();
        out.close();
        s.close();
    }
}
