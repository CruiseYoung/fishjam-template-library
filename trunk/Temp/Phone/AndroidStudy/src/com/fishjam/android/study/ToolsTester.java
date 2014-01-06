package com.fishjam.android.study;
import android.test.AndroidTestCase;

/***************************************************************************************************************************************
 * aapt.exe(Android Asset Packaging Tool) -- ��Դ(����Ƶ��ͼƬ��xml���õ�)�������? (��: ���Ի�ȡlog����װӦ�ó��򣬸����ļ��ȵ�)
 *    aapt p -A assets -S res -M AndroidManifest.xml -F \bin\resources.ap_ -- �������µ���Դ���Ϊ \bin\resources.ap_ ѹ���ļ�
 * activityCreator.py -- һ��Python�ű�������Ϊ��Ŀ�������е�Դ�����Ŀ¼�ı��뻷��(������������ant�����build.xml)
 *    activityCreator.py --out HelloAndroid com.google.android.hello.HelloAndroid  -- ���� build.xml
 * adb.exe(Android Debug Bridge) -- �ṩһ���򵥵�shell����,���Ե�¼���ֻ�/ģ�����Ͻ��и��������в�����������һ̨Linux������һ�������԰�װһ��ARM���busybox,ʹ���������ӷ���
 *    adb logcat --  ���Բ鿴ϵͳlog�����ڵ��ԡ�
 *    adb install myapp.apk -- ��CMD�����½�Ӧ�ó���װ��ģ���� /data/app Ŀ¼�£���װǰ��Ҫ�ȸ��Ƶ��豸�ϡ������ڲ��˳�ģ�����������ͨ����װ��ɾ���ķ�ʽ���е��ԡ�
 *    adb shell -- ����shell����ģʽ������ʹ�� ls��cd��rm��dmsg �ȼ����������ʹ�� sqlite3 ����������ݿ�
 *    adb forward tcp:5555 tcp:8000 -- ���÷����˿ڣ���Ϊ������ģ�������豸������˿�
 *    adb push test.txt /sdcard/test.txt -- ����һ���ļ���Ŀ¼���豸��ģ������
 *    adb pull /android/lib/libwebcore.os .\ -- ���豸��ģ�����ϸ���һ���ļ���Ŀ¼����ǰĿ¼
 *    adb devices -- ȡ�õ�ǰ���е�ģ�������豸��ʵ���б�ÿ��ʵ����״̬
 *    adb bugreport -- �鿴Bug����
 * ant -- ʹ��Ant���룬����binĿ¼������ HelloAndroid.apk ���ļ���Ȼ������� adb ��װ��ģ������
 *
 * apkbuilder.bat -- ��dx����������.dex �� appt������������Դ�ļ� ����� .apk �ļ�
 * AVD(Android Virtual Device) -- Android�����豸����ģ���������ã����û����Ը��õ�ģ����ʵ�豸��
 *    ����:Ӳ������(���Ƿ�����������������͡��ڴ��С��)���汾ѡ���豸����Ļ�ߴ硢SD����С�ȡ�
 *    �����������ļ�������%ANDROID_SDK_HOME%����������(��������%HOMEPATH%? ��)�� .android\avd Ŀ¼��
 * Dalvik VM -- Android�е���������ƣ���Java VM���ƣ��������ݡ�ר��Ϊ�ƶ��豸�����Ż�(���ڼĴ�����)�����Java������ٶȿ�ܶ�,ִ��.dex��Dalvik��ִ���ļ�
 * ddms.bat(Dalvik Debug Monitor Service) -- Dalvik ���Լ�ط�����Ҫ��ϵͳ���к�̨��־��ϵͳ�̡߳������״̬�ȵļ��? ����ģ�ⷢ�Ͷ��š�����绰������GPSλ����Ϣ�ȡ�
 *     (��: �ֻ�/ģ��������Ļ��ͼ��log)
 * dx.bat -- ��Java���������ļ�(.class�ֽ����ļ�)ת����Dalvik�������ִ�е�.dex(Dalvik Executable Format)�ļ�
 *    ��: dx --dex --dump-to=D:\MyAndroid\testProject.dex --core-library D:\MyAndroid\Bin
 * emulator.exe ģ��������������ṩ�˴��������Ӳ���豸�����ԣ������� �Ӵ�绰������ ��
 *    -wipe-data  -- ��ģ���������ûָ�����ʼ״̬
 *    -sdcard SD.file -- ģ�����sd�����龰
 * logcat  -- Debug ���ߣ���ʾ android.util.Log ����־�����
 *   abd shell ���뽻����ͨ�� logcat ����ִ�С����ò�����
 *     -s -- ����Ĭ�ϵĹ��˼��𼰹�����Ϣ���� -s "Module1:i"  -- ��ʾTagΪ "Module1" �д��ڵ���I(nfo)����Ϣ
 *     -b -- �򿪲�ͬ��log buffer(λ�� /dev/log/ Ŀ¼�£���������main��radio��events)
 *     -c -- ������е�log��Ϣ���磺#logcat -cb events #logcat -b events
 *     -d -- ����Ļ����ʾlog��Ϣ��������Ϣ�������˳���
 *     -f -- ��log��Ϣ��Ϊ�ļ��������������Խ�log��Ϣ��Ϊ���õı������ֻ��豸�С�
 *       # mount -o remount,rw rootfs /      -- ���ļ�ϵͳ��Ϊ�ɶ�д
 *       # logcat -b events -f /tmp/events   --  ��events buffer�е�log��Ϣ���� tmp�ļ����µ�events�ļ���
 *     -g -- �鿴����log buffer �Ĵ�С
 *     -n -- ������־�������Ŀ<count>��Ĭ����4���� -f ��������ʹ��
 *     -r  -- ÿ <kbytes>ʱ�����־��Ĭ��ֵΪ16���� -f ��������ʹ��
 *     -v -- ����log�������ʽ��brief(Ĭ��)��process��tag��thread��raw��time��long
 *            TODO: log���� V(erbose) ��D(ebug)��I(nfo)��W(arning)��E(rror)��F(atal)��S(ilent) 
 * mksdcard.exe -- ����sd��Ӱ���ļ�
 *   mksdcard.exe -l SDCard 512M "E:\Android_SDK\sdcard.img"
 * ������� �� adb shell monkey -p com.fishjam.android.study  -v 500  -- ʹ��ȱʡ���ã���Ӧ�÷���500������¼�(����������touch�¼���ϵͳ�¼���)
 **************************************************************************************************************************************/

public class ToolsTester  extends AndroidTestCase {

}
