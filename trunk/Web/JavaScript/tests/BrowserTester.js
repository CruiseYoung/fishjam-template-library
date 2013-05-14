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
    equal(navigator.appName, "Netscape", "navigator.appName �����������, FF/Chrome--Netscape, IE -- Microsoft Internet Explorer");

    equal(navigator.appVersion, // "5.0 (Windows)");       //������汾
     "5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.31 (KHTML, like Gecko) Chrome/26.0.1410.64 Safari/537.31");  //Chrome -- 5.0 
    //FF --
    //IE -- 4.0 (compatible; MSIE 8.0; Windows NT 5.1; Trident/4.0;  <== ע�⣺��������������һ����Ϣ

    equal(navigator.language, "zh-CN", "����")
    equal(navigator.platform, "Win32", "ƽ̨");
    equal(navigator.userAgent,
		"Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.31 (KHTML, like Gecko) Chrome/26.0.1410.64 Safari/537.31", 
		"��������������ȫ����Ϣ�����Բ����Ƿ����ָ���ַ������ж���������ͣ�");
    //equal(navigator.plugins, "a");    //�������ʾ�Ѱ�װ����ҳ���

    //ok(true, "tmp");
});

test("TODO: Screen", function() {
    with (screen) { //ȫ����Ļ���� ��ӳ�˵�ǰ�û�����Ļ����
        //TODO:��˫����ʾ��ʱ��ʹ�� equal �е�ʱ���ʧ�ܣ�F5ˢ�º��ֱ�ã�Ϊʲô��
        ok(width >= 1280, "��Ļ���");
        ok(height >= 1024, "��Ļ�߶�"); //960
        ok(availWidth >= 1280, "��Ļ�Ŀ��ÿ��");
        ok(availHeight >= 1024, "��Ļ�Ŀ��ø߶ȣ���ȥ��һЩ���Զ����ص������������Ķ�����ռ�õĸ߶�"); //960
        equal(colorDepth, 32, "��ǰ��ɫ�������õ�λ��");
    }
});

test("TODO: Window", function() {
    //JavaScript �����Ķ�������������һ����������ڣ�һ�����ҳ��Ҳ��һ������
    //��һ��ҳ���ڶ��������ȫ�ֱ�����ȫ�ֺ���������Ϊ window ����ĳ�Ա -- ��ΪʲôFF��jstrace�����ã�

    //equal(window.name, "test", "���ڵ�����");

    equal(window.closed, false, "��ǰ������δ�ر�");
    equal(window.opener, null, "������ڲ������������ڴ򿪵ģ�Netscape�з���null;IE�з���undefined"); //
    equal(window.self, window, "self ָ���ڱ��������صĶ���� window ������һģһ����,��õ���\"self.close()\"");

    equal(window.parent, parent, "parent ���ش��������Ŀ��ҳ������һ��frame���һ�ȡframeset������frame -- parent.frames[n]");
    equal(window.top, top, "top ����ռ��������������ڵ���˵Ŀ��ҳ����");
    equal(window.parent, window.top, "�˴� parent �� top ��ͬһ��");

    ok(window.parent != null, "window.parent != null");
    ok(window.parent.top == window.self, "�����ܵ� parent/top ����ʾ�ÿ�ܱ���if(self==top){�Ƕ�����}");

    //ÿ����ܶ���Ӧһ��HTML ҳ��
    equal(window.frames.length == 0, true, "TODO:frames -- ��ҳ�������п�ܵļ��ϣ���ͨ��frames[\"����\"] �� frames[index] �ķ�ʽ����");

    equal(window.history, history, "history -- ��ʷ����");
    equal(window.location, location, "location -- ��ַ������ reload() �� repalce() ����");
    equal(window.document, document, "document -- �ĵ�����");
    //equal(window.Link, Link, "�����ӻ�ͼ�ε�λ�õ�"); //��Chrome��ʵ��û���������
    //equal(window.elements, elements, "���ʴ����е�����Ԫ��"); //��Chrome��ʵ��û���������

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
    ok(window.location.href.indexOf("QUnitTestFramework.html") > 0, "location.href ��������URL");
    equal(window.location.protocol, "file:", "location.protocol");
    equal(window.location.hostname, "", "location.hostname");

    //window.scroll(x, y); //�����ڹ���������ָ����λ��
    //window.moveTo(x, y); //�������ƶ���ָ����λ��
    //window.resizeTo(cx, cy);  //�����ڵ�����ָ���Ĵ�С

    window.defaultStatus = "ȱʡ״̬����Ϣ";
    window.status = "ָ���û�������Ƶ�ĳ�����ڵȲ���ʱ�����ֵ�˲ʱ״̬��Ϣ"

    with (window.document) {
        //document.open([MIMEType]) -- ��ָ����MimeType(ȱʡ��text/html)���ĵ���Ȼ���ͨ�� write/writeln �ȷ���д�룬д��ͨ��close�ر�

        //����� form ��ָ���� name="xxx", �����ͨ�� window.document.xxx �ķ�ʽ����?

        equal(characterSet, "GBK", "�ַ���");
        equal(referrer, "", "referrer -- �����ǰ�ĵ���ͨ��������Ӵ򿪵�,����ԭ����URL");

        equal(anchors.length, 0, "�ĵ�������ê���(����name���Ե�<a>���)������");
        equal(links.length > 0, true, "�ĵ����������ӱ��(����href���Ե�<a>��Ǻ�<map>��Ƕ����<area>��ǵ�����");
        equal(applets.length, 0, "�ĵ������е�Applet����(Java С����)������");
        equal(embeds.length, 0, "�ĵ������еĲ��<embed>��ǵ�����");
        equal(forms.length, 0, "�ĵ������б�<form>������");
        equal(cookie.length, 0, "�ĵ��е�Cookie");
        
        equal(images.length, 0, "�ĵ��е�����ͼ��");
        /*
        //�� QUnitTestFramework.html �м��� <img src="..\HTML\img\merglobe.gif"/> �������Щ
        equal(images.length, 1, "�ĵ��е�����ͼ��");
        ok(images[0].src.indexOf("merglobe.gif") > 0, "img src");
        equal(images[0].height, "100", "img src");
        equal(images[0].width, "100", "img src");
        //*/
    }

    //form ���� -- windows.document.forms[n]
    //���ṩ�� submit()/reset() �ȷ���
    //         action/elements(��ϸ��Ϊ button/radio/text ��)/encoding/target ������
    //         �ύʱ(form.OnSubmit��): document.submitForm.result.value = submitValue; document.submitForm.submit();
});

test("Windows��ʾ��ʽ", function() {
    equal(1, 1, "TODO");
    //alert(��ʾ��Ϣ) -- ��ʾ��Ϣ��ֻ��һ��OK��ť
    
    //confirm -- ϵͳ�ṩ��ȷ�϶Ի���(�� ȷ��/ȡ�� ������ť)���û�ȷ��ʱ���� true
    //if (confirm("��ʾ��Ϣ")) {
    //    alert("�û�ѡ����\"ȷ��\"");
    //}

    //prompt -- ��ʾҪ���û�����ĶԻ������û�ȡ������ null�����򷵻��û����������
    //var s = prompt("��ʾ��Ϣ", "ȱʡ����");
    
});

test("TODO: Firefox�е�ѧϰ����", function() {
    ok(1, "TODO:Info");
    //window.onload -- �������ĵ�ȫ��(HTML��ͼƬ���ؼ���С�����)������ϵ�ʱ��onload/onunload ��Ȼ��window�¼���������Ҫд��<body>��
    //History -- ��ʷ����������û�������� URL ����Ϣ,���ڰ�ȫ�Ե���Ҫ���ö����յ��ܶ����ƣ�Ŀǰֻ���������Ժͷ�����
    //   length, back(), forward(), go(pos);
    //location -- ��ַ����, ��������ĳһ�����ڶ������򿪵�����URL,�ڲ��� protocol��pathname��href �ȶ������
    //  ���ڲ�ͬЭ���ͬ������������ַ֮�䲻�ܻ������öԷ��� location����(��ȫ�Ե���Ҫ)
    //  URL��ַ��Э��//����:�˿�/·������#��ϣ��ʶ?��������
    //    Э�� -- �� http, ftp, file, javascript(��JS���ʽ��ֵ���� JavaScript:void(0) ), about �ȣ�
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

test("ͼ��ӳ��Area", function() {
    //hash -- HREF���Ե�ƫ��������
    //host -- HREF���Ե���������
    //href -- HREF����ȫ��
    //target -- TARGET����
    equal(1, 1, "TODO:Area");
    //var myarea = new Area();
    //myarea.href = "javascript:void(0)"; //�����û�����

});

test("BrowserInfo", function() {
    equal(gBrowserInfo.IsNetscape, true, "��Netscape");
});

test("Event", function() {
    //JavaScript ���¼������ܿɸı��������Ӧ�û������Ĵ���ʽ
    //  ����ֱ�ӰѴ���д�� onxxx ���棻Ҳ����д�� .js �ļ���ֱ�����ã�Ҳ����ʹ���������� onxxx=function(){ callFun }
    //
    //�¼��������ķ���ֵ
    //  false -- �ж�ϵͳĬ�ϴ���(���� <a onclick="return false;" xxx/> �Ͳ�����ת
         
    //�����¼�(TODO: �¼���û�в��� ? ���� onerror ��ô֪������Ĵ���ԭ�� )��
    //  onblur -- ʧȥ���뽹��ʱ
    //  onabort -- �û��ж�(��ͼ��װ��)����ʱ
    //  onclick -- �������ʱ
    //  onerror -- ���ִ���
    //  onfocus -- ������뽹��ʱ
    //  onload -- װ��ͼ�Ρ��ĵ���frames(֡��)ʱ
    //  onselect -- �ı������ı��ֶε��ı���ѡ��ʱ
    //  onsubmit -- �ύ����ʱ 
    equal(1, 1, "TODO:�¼�");
});

