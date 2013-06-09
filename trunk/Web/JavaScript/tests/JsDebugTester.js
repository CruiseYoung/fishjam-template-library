/*************************************************************************************************************************
* ���� -- Firefox �е� Firebug
* TODO��дһ���Լ��ķ�װ�࣬���ṩ����������ж�����ʹ�õ���־���ܣ����ܺܺõ��л�����ʵ��(���Ƿ����á��Ƿ����緢�͵�?)
*
* Console -- ������ʾ JS�� DOM ������Ϣ�ĵ������ڣ�js����ȫ�ֵ� console ���󣬿��������Ϣ��Console����
*   ����������5�еȼ�(debug < info < log < warn < error), ���ֵ���ɫ��һ��
*     log(object[,object,...]) -- ����﷨��֧�ָ�ʽ��(%s,%d,%o ��)
*     assert(expression[, object, ...])  -- ����������� expression ���ʽΪ false ��ʱ��ʹ�� error ��ʽ���
*     clear(); -- ��� console �е�������Ϣ
*     count([title]) -- ͳ�Ƶ�ǰ���뱻ִ�й����ٴΣ�title ���������ڴ���ǰ���������ı����԰����Ķ�
*     dirxml(obj); -- �� html Ԫ�ص�html �����ӡ����(ʵ�ʲ������� %o)
*     group|groupCollapsed|groupEnd (object[, object, ...]) -- �� log ���������Ϣ���з��飬�����Ķ��鿴 
*     trace() -- ��ӡ���ö�ջ
*     time|timeEnd (name) -- ����js������ִ��ʱ��,time(name)����name����1���µļ�ʱ����timeEnd(name)ֹͣ����name�ļ�ʱ��������ʾʱ�䡣
*     table(data) -- (ֻ��FireBug1.6+�Ժ��֧��) ��data �����ñ��ķ�ʽ��ʾ������������ʾ�������JSON �����ʱ��ǳ�����
*     profile|profileEnd (name) -- ��������ķ����������ڷ������麯��֮��� js ִ�����
* 
*   �������֧�֣�
*     ֧�֣�Chrome��FireFox(FireBug) 
*     ����֧�֣�IE8�Ŀ�������(�����ܱȽϵ���,��ʾ����ʱ����), �����汾��IE���԰�װ Developer Toolbar
*     Firebuglite(https://getfirebug.com/) -- Firebug �Ƴ��Ĺ��ߣ������������������֧��Console����? 
*************************************************************************************************************************/


module("JsDebugTester", {
    setup: function() {
        //dump("in DebugTester::setup\n");
    },
    teardown: function() {
        //dump("in DebugTester::teardown\n");
    }
});

test("ConsoleTester", function() {
    ok(1 == 1, "TODO: Console Tester");

    console.groupCollapsed("ConsoleTester");  //groupCollapsed Ĭ�����۵��ģ� group Ĭ����չ����
    {
        console.time("consoleTime");        //��ʼ��ʱ
        //console.profile("consoleProfile");  //��ʼ����

        console.log("simple formatted text log, s-�ַ���:%s, d-����:%d, f-������:%f",
        "SomeString", 100, 3.1415926);

        console.log("o-����: %o", this);

        console.log("�Զ������������%o", { AA: "OK", BB: "No" }); //���õ������� Json �ķ�ʽ
        //console.table({ AA: "OK", BB: "No" });  //Chrome ��֧�֣���δ����FireBug

        console.debug("some debug string");
        console.info("some info string");
        console.log("some log string");
        console.warn("some warn string");
        console.error("some error string");   //Chrome�л��������ö�ջ(trace)

        console.assert(1 == 1, "��������������(1==1)");
        console.assert(1 == 0, "������������(1==0)");

        console.trace();    //��ӡ���ö�ջ

        //console.profileEnd("consoleProfile");   //����������ÿһ�ζ�������һ�� profile �ļ��������Tester����ر�

        console.count("consoleCount");          //ִ�д���
        console.timeEnd("consoleTime");         //������ʱ����ʾ����ʱ��
    }
    console.groupEnd();

    console.log("some other info out of group");    //��ӡ���ö�ջ
});
