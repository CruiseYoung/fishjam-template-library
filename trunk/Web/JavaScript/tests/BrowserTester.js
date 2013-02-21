module("BrowserTester", {
    setup: function() {
        //dump("in BrowserTester::setup\n");
    },
    teardown: function() {
        //dump("in BrowserTester::teardown\n");
    }
});

test("TODO: Browser", function() {

    //һ���ж�������ķ�ʽ -- �ƺ���ȫ��
    if (document.all) {
        //MSIE
        equal(navigator.appName, "Microsoft Internet Explorer", "MSIE");
    }
    else {
        equal(navigator.appName, "Netscape", "Navigator");
    }
    
    equal(1, 1, "TODO: ��������󣬰���������ʹ�õ� Navigator �İ汾��Ϣ����ӳ�˵�ǰʹ�õ������������");
    //JavaScript �ͻ�������ʱ�������Զ����� navigator����


    equal(navigator.appCodeName, "Mozilla", "���������������--IE��FF��Chromeȫ������ Mozilla");
    equal(navigator.appName, "Netscape", "�����������, FF/Chrome--Netscape, IE -- Microsoft Internet Explorer");

    equal(navigator.appVersion, "5.0 (Windows)");       //������汾
    //Chrome -- 5.0 (Windows NT 5.1) AppleWebKit/536.5 (KHTML, like Gecko) Chrome/19.0.1084.52 Safari/536.5
    //FF --
    //IE -- 4.0 (compatible; MSIE 8.0; Windows NT 5.1; Trident/4.0;  <== ע�⣺��������������һ����Ϣ

    equal(navigator.language, "zh-CN", "����")
    equal(navigator.platform, "Win32", "ƽ̨");
    equal(navigator.userAgent, "Mozilla/5.0 (Windows NT 5.1; rv:14.0) Gecko/20100101 Firefox/14.0.1", "��������������ȫ����Ϣ�����Բ����Ƿ����ָ���ַ������ж���������ͣ�")
    //equal(navigator.plugins, "a");    //�������ʾ�Ѱ�װ����ҳ���

    //ok(true, "tmp");
});

test("TODO: Screen", function() {
    with (screen) { //ȫ����Ļ���� ��ӳ�˵�ǰ�û�����Ļ����
        equal(width, 1280, "��Ļ���");
        equal(height, 1024, "��Ļ�߶�"); //960
        equal(availWidth, 1280, "��Ļ�Ŀ��ÿ��");
        equal(availHeight, 994, "��Ļ�Ŀ��ø߶ȣ���ȥ��һЩ���Զ����ص������������Ķ�����ռ�õĸ߶�"); //960
        equal(colorDepth, 24, "��ǰ��ɫ�������õ�λ��");
    }
});

test("TODO: Window", function() {
    //JavaScript �����Ķ�������������һ����������ڣ�һ�����ҳ��Ҳ��һ������
    //��һ��ҳ���ڶ��������ȫ�ֱ�����ȫ�ֺ���������Ϊ window ����ĳ�Ա -- ��ΪʲôFF��jstrace�����ã�

    equal(window.name, "", "���ڵ�����");
    equal(window.opener, null, "������ڲ������������ڴ򿪵ģ�Netscape�з���null;IE�з���undefined"); //
    equal(window.self, window, "self ָ���ڱ��������صĶ���� window ������һģһ����,��õ���\"self.close()\"");

    equal(window.parent, parent, "parent ���ش��������Ŀ��ҳ����");
    equal(window.top, top, "top ����ռ��������������ڵ���˵Ŀ��ҳ����");
    equal(window.parent, window.top, "�˴� parent �� top ��ͬһ��");

    ok(window.parent != null, "window.parent != null");
    ok(window.parent.top == window.self, "�����ܵ� parent/top ����ʾ�ÿ�ܱ���if(self==top){�Ƕ�����}");

    //ÿ����ܶ���Ӧһ��HTML ҳ��
    equal(window.frames.length == 0, true, "TODO:frames -- ��ҳ�������п�ܵļ��ϣ���ͨ��frames[\"����\"] �� frames[index] �ķ�ʽ����");

    equal(window.history, history, "history -- ��ʷ����");
    equal(window.location, location, "location -- ��ַ����");
    equal(window.document, document, "document -- �ĵ�����");

    //open �򿪴��� <== open(<URL�ַ���>, <��������window.name,����ʹ�� _top/_blank ���ڽ�����>, <�����ַ���>);
    //     ����(��������ö��Ÿ���)��
    //      top,left,width,height <== ���ڵ�λ�á���С��Ϣ
    //      menubar     <== ������û�в˵���ȡֵyes ��no
    //      toolbar     <== ������û�й�������ȡֵyes �� no
    //      location    <== ������û�е�ַ����ȡֵyes ��no
    //      directories <== ������û����������ȡֵyes ��no
    //      scrollbars  <== ������û�й�������ȡֵyes ��no
    //      status      <== ������û��״̬����ȡֵyes ��no
    //      resizable   <== ���ڸ�����������С��ȡֵyes ��no
    //var winBaidu = window.open("http://www.baidu.com", "_blank", 
    //  "top=100,left=200,width=400,height=100,menubar=no,toolbar=no,location=no,directories=no,status=no, scrollbars=yes,resizable=yes");
    //if (winBaidu) {
    //    winBaidu.close();
    //}
    ok(window.location.href.indexOf("QUnitTestFramework.html") > 0, "location");
    equal(window.location.protocol, "file:", "location.protocol");
    equal(window.location.hostname, "", "location.hostname");

    with (window.document) {
        equal(characterSet, "GB2312", "�ַ���");
        equal(referrer, "", "referrer -- �����ǰ�ĵ���ͨ��������Ӵ򿪵�,����ԭ����URL");

        equal(anchors.length, 0, "�ĵ�������ê���(����name���Ե�<a>���)������");
        equal(links.length > 0, true, "�ĵ����������ӱ��(����href���Ե�<a>��Ǻ�<map>��Ƕ����<area>��ǵ�����");
        equal(applets.length, 0, "�ĵ������е�Applet����(Java С����)������");
        equal(embeds.length, 0, "�ĵ������еĲ��<embed>��ǵ�����");
        equal(forms.length, 0, "�ĵ������б�<form>������");
    }
});

test("TODO: Firefox�е�ѧϰ����", function() {
    ok(1, "TODO:Info");
    //window.onload -- �������ĵ�ȫ��(HTML��ͼƬ���ؼ���С�����)������ϵ�ʱ��onload/onunload ��Ȼ��window�¼���������Ҫд��<body>��
    //History -- ��ʷ����������û�������� URL ����Ϣ,���ڰ�ȫ�Ե���Ҫ���ö����յ��ܶ����ƣ�Ŀǰֻ���������Ժͷ�����
    //   length, back(), forward(), go(pos);
    //location -- ��ַ����, ��������ĳһ�����ڶ������򿪵�����URL,�ڲ��� protocol��pathname��href �ȶ������
    //  ���ڲ�ͬЭ���ͬ������������ַ֮�䲻�ܻ������öԷ��� location����(��ȫ�Ե���Ҫ)
    //  URL��ַ��Э��//����:�˿�/·������#��ϣ��ʶ?��������
    //    ��ϣ��ʶ ������URL�е�ê���ƣ�������ϣ����(#)��������ֻӦ���� HTTP �� URL
    //    �������� ������URL�е��κβ�ѯ��Ϣ�������ʺţ��� ����=ֵ �Ķԣ������֮���� & ����
    //Document -- ��ʾ�ڴ��ڻ����ڵ�һ���ĵ���������ǰ���ڻ�ָ�����ڶ�����ĵ������������ĵ���<head>��</body>������
    //  close() �ر��ĵ���ֹͣд�����ݡ�������� write[ln]() �� clear()��������һ��Ҫ�� close() ��������֤�����ĸ����ܹ���ʾ������
    //Form -- ��
    // action -- ����/�趨�����ύ��ַ,��<form action="...">����
    // method -- ����/�趨�����ύ����,��<form method="...">����
    // target -- ����/�趨���ύ�󷵻صĴ���,��<form target="...">����
    // encoding -- ����/�趨���ύ���ݵı��뷽ʽ,��<form enctype="...">
    // onreset() -- ���ñ�ʱ����
    // onsubmit() -- �ύ��ʱ����
});

//һ��HTML ҳ�������һ�������ӿ�ܣ���Щ�ӿ����<iframe>����ǣ�������ʾһ��������HTML ҳ��
test("TODO:iframe", function() {
    equal(1, 1, "iFrame");
});

test("BrowserInfo", function() {
    equal(gBrowserInfo.IsNetscape, true, "��Netscape");
});

