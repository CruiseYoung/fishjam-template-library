package com.fishjam.utility.io;

import java.io.Serializable;

/************************************************************************************************************
* IOʹ��Stream(��)�ĳ�������ʵ��I/O�豸�д������ݵ�ϸ�ڶ�������������ͨ�����϶��
* �������ʽ�����Ҫ�Ĺ��ܡ�ͨ��ʹ����Ϻ���Ҫʹ��close�ر��ļ�
* 
* Reader��Writer�ṩ��"������Unicode�������ַ���I/O"����Ҫ��Ϊ�˹��ʻ�(�ɵ�I/OStream
*   �̳���ϵ��֧��8Bit Byte Stream)��ͨ��InputStreamReader �� OutputStream ����ת����
* 
* �߱����幦�ܵ������ļ���BufferedReader(FileInputReader) -- readLine���ļ�βʱ����null;
*   BufferedReader(InputStreamReader(System.in)) -- ����(ת��(��׼����))
* ������ļ���PrintWriter(BufferedWriter(FileWriter)) -- ��ʽ��(����(�ļ�))
* 
* Java����ͨ��static�����Ա�׼���롢��׼�������׼���������I/O Streams�����ض���
*   System.setIn(InputStream)
*   System.setOut(PrintStream)
*   System.setErr(PrintStream)
*   
* Java�ṩ��ѹ��/��ѹ�� Streams ����
*   ѹ����DeflaterOutputStream <= ZipOutputStream/GZIPOutputStream
*   ��ѹ��InflaterInputStream  <= ZipInputStream/GZIPInputStream/ZipFile
*   CheckSum:Adler32(�Ͽ�)��CRC32(�������ȽϾ�ȷ)
*     ZipOutputStream(new BufferedOutputStream(new CheckedOutputStream(
*       new FileOutputStream("test.zip"),new Adler32())));
*       
* JAR--��ĳ��applet����������ļ����ڵ�һJAR�ļ��ڣ������ֻ�������������һ�ξͺã����Ҵ����ٶȿ졣
*   �����ڲ��ļ���һ���嵥(manifest)
*   
* �������л�(Serialization)��ʵ�ֳ� Serializable �ӿڵĶ����ȫ�������Զ�ת��Ϊ����bytes���ݣ�
*   ��Щ���ݿ�ͨ�����紫��򱣴浽���̣�֮��ɱ���ԭΪԭ�ȵĶ���״̬��ע�⣺ͨ�����л�
*   ���ɶ���ʵ��ʱ����������κ�(����ȱʡ)���캯�������������״̬ȫ��ͨ��InputStream
*   ����λ���ݽ��лظ�--��MFC��ͬ������ʹ�ùؼ���transient�ر�ָ����Ա������Ҫ���л�
*   ObjectOutputStream(new FileOutputStream).writeObject��
*   ���Ҫ�Կ����л��Ĳ��ֽ��п��ƣ��簲ȫ���⣩��Ӧ��ʵ��Externalizable�ӿڣ�
*   ��override���е�writeExternal��readExternal������ע�⣺�����ȱʡ���죬Ȼ��ͨ��
*   readExternal�ظ�������ͨ����Ҫ���û����ͬ������--��MFC��ͬ
*   ֻҪ�����ж���д�뵥һstream����ɻظ�ԭ�ȵĶ������磬���Ҳ�����⸴���κζ���
* 
************************************************************************************************************/

public class FIOStream
{
    
}
