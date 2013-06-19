/**************************************************************************************************************************
* Ajax(Asynchronous JavaScript and XML) -- �첽JavaScript��XML���ӿ��û���Ӧ(������ʱ���Զ���ȫ?GoogleMap��)
*   �� Ajax�����У��������������ؿ��ʵ�ָ��ּ��� -- ����ˢ���ϴ��ļ�
*   ������� XMLHttpRequest -- ����Զ��HTTP����
*   ��һ��������־�ǿ��Ծֲ�ˢ��Webҳ�棬������ҳ����������ֲ����κ�Ӱ��
* ȱ�㣺
*   ������ -- ����ͨ�� ASP.NET Ajax �ؼ������ͣ�������Զ����������JS������ʵ�������AjaxЧ��
*   �����֧�� -- ��Ͱ汾����������JavaScript���ܵ��û�
*   �����ƻ���������˰�ť��������Ϊ
* TODO:
*   1.��Ϊʹ��ajxa���ݲ�����ʱ����Ҫhead֧�֣��������WebServer(Apache/IIS)�²���
*
*
***************************************************************************************************************************/

/***************************************************************************************************************************
* XMLHttpRequest
*   ����
*     readyState -- ����,
*     response -- �ַ���
*     responseText -- ���صĽ���ַ���
*     responseType
*     responseXML
*     status -- 
*     statusText -- ? ����״̬�ַ����� �� success��error
*   �ص�
*     onabort
*     onerror
*     onreadystatechange -- �� readyState ֵ�ı�Ļص�����, ͨ����Ҫ�������ж� readyState(4) �� status(200) 
*     onprogress
*   ����
*     open(METHOD, url, isAsync) -- ������Ҫ�򿪵� URL �� METHOD(GET/POST)���Ƿ��첽 ��
*     send(?) -- ��������
***************************************************************************************************************************/

module("AjaxTester", {
    setup: function() {
        //dump("in AjaxTester::setup\n");
    },
    teardown: function() {
        //dump("in AjaxTester::teardown\n");
    }
});


test("TODO:Ajax", function() {
    var bCreateByActiveX = false;
    var xmlHttpReq = null;          //����һ���ն�������װ��XMLHttpRequest
    if (window.ActiveXObject) {     //IE5 IE6����ActiveXObject�ķ�ʽ����XMLHttpRequest��
        xmlHttpReq = new ActiveXObject("Microsoft.XMLHTTP");
        bCreateByActiveX = true;
    }
    else if (window.XMLHttpRequest) {//��IE5 IE6 ����������XMLHttpRequest��window���Ӷ���
        xmlHttpReq = new XMLHttpRequest(); //ʵ����һ��XMLHttpRequest
        bCreateByActiveX = false;
    }
    
    //���߼�д�����·�ʽ
    //  xmlHttpReq = window.ActiveXObject ? new ActiveXObject("Microsoft.XMLHTTP") : new XMLHttpRequest();
    
    equal(bCreateByActiveX, false, "���ڵ��������XMLHttpRequest��window���Ӷ���");
    ok(xmlHttpReq != null, "���� XMLHttpRequest ����");


//*
    if (xmlHttpReq != null) {//�������ʵ�����ɹ�
        //xmlHttpReq.open("GET", "http://www.baidu.com", false);      //����open()����������ͬ����ʽ
        xmlHttpReq.onreadystatechange = RequestCallBack;    //���ûص�����
        //xmlHttpReq.send(null);                              //��Ϊʹ��get��ʽ�ύ�����Կ���ʹ��null�ε���
    }

    console.log("%o", xmlHttpReq);

    function RequestCallBack() {
        if (xmlHttpReq.readyState == 4) {
            if (xmlHttpReq.status == 200) {
                //��xmlHttpReq.responseText��ֵ����IDΪ resText ��Ԫ��
                alert(xmlHttpReq.responseText);
                //document.getElementById("resText").innerHTML = xmlHttpReq.responseText;
            }
        }
    }
//*/

});
