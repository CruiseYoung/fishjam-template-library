module("CookieTester", {
    setup: function() {
        //dump("in MathTester::setup\n");
    },
    teardown: function() {
        //dump("in MathTester::teardown\n");
    }
});

test("��дCookie", function() {
    var oldCookie = document.cookie;

    with (window.document) {
        //ע�⣺���ش����е�ҳ�棬chrome�Ŀ���̨���޷���JavaScript��д���� cookie ��
        //һ����վ���Դ������cookie(ͨ�� �� �� ·��?)
        
        //cookie ������(typeof) ���ַ���(string), ��˿������ַ����ķ�ʽ������(�� indexOf, substring ��)
        //ÿ��Cookie����ŵ����ݲ��ܳ���4K����ʽΪ <cookie��>=<ֵ>;
        //·��(routing) -- Ĭ��ʱ��ֻ���봴�� cookie ��ҳ����ͬһ��Ŀ¼����Ŀ¼�µ���ҳ�ſ��Է��ʣ�ͨ������ path=xxx ���Լ��ɸ���
        //��(domain) -- ��ͬ����(��www.qq.com��sports.qq.com)֮�䲻��ֱ�ӷ���cookie��ͨ������ domain=qq.com ������path=x ������ͬ����ܷ��ʡ�


        //ÿ�� Cookie ����ʧЧ���ڣ�һ�����Ե�ʱ�ӹ���ʧЧ���ڣ���� Cookie�ͻᱻɾ����Ĭ������ʱ���ʵģ�������رռ�ɾ����
        //����ֱ��ɾ��һ�� Cookie�����ǿ������趨ʧЧ������������ʱ�̵ķ��������ɾ����(������ͨ�� cookie="" ��������е�cookie )
        //  �趨 Cookie ��ʧЧ���ڶ����� GMT ��ʽ��ʱ��ģ�ͨ�� expires=date ֵ���趨
        var expires = new Date();
        expires.setTime(expires.getTime() + 3 * 30 * 24 * 60 * 60 * 1000); //�趨��Լ�����º����
        //expires.setDate(expires.getDate() + 30);  //��ʾ����30���Ժ�

        //���ܴ���(���� secure ���ԣ�û������ֵ ) -- ͨ�� cookie ��Ϣ����ʹ��HTTP���Ӵ������ݣ�����ȫ
        //  ע�⣺ֻ�����紫��ʱ���ܣ����ر���δ����(��Ҫ�Լ�����)

        var strCookie = "name=fishjam,email=fishjam@163.com;secure;path=/;domain=jstest.com;expires=" + expires.toGMTString();
        cookie = escape(strCookie); //����ʱ���ʹ�� escape -- cookie��Ϣʱ���ܰ����ո񣬷ֺţ����ŵ��������
        equal(unescape(cookie), strCookie, "document.cookie�� TODO: ���cookie��ʲô�ָ�����ͨ�Ƕ���, ʧЧʱ��ǰ�Ƿֺ�?");
        equal(0, 0, "TODO: ��ô�жϡ�����ʧЧʱ��? expires ?, Ĭ�ϵ�����������������ر�ʱ");
    }
    
    //�ָ�Cookieԭ����ֵ
    document.cookie = oldCookie;
    
    //ok(0144 == 100, "�˽���");

});