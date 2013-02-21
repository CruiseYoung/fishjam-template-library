//��JavaScript���ִ�Сд��
//Netscape����������Ϊ�� for ѭ�����ĵ��ظ�д HTML ����ᵼ�����������

module("JSLanguageTester", {
    setup: function() {
    //dump("in JavaScriptLanguage::setup\n");
    },
    teardown : function () {
    //dump("in JavaScriptLanguage::teardown\n");
    }
});


//����: ֻ������ĸ�����ֺ�/���»��ߣ�Ҫ����ĸ��ͷ��������Ҫ���� û�������ı�������ʹ��(undefined)
//      JS���������͵�Ҫ���ϸ��������Ϳ����ø����ʼֵ�ķ�����������֮����Ը����������������͵�ֵ
//      ͨ�� typeof(������) �� typeof ������ �ķ�ʽ���Բ�ѯ���������ͣ������������� string, number, array ��
test("����", function() {
    var varNull;
    equal(varNull, null, "����δ�������֮��δ��ֵ����ֵΪ\"null\"");

    ok(0144 == 100, "�˽���");
    ok(100 == 100, "ʮ����");
    ok(0x64 == 100, "ʮ������");


    var varInfinity = 1 / 0;
    equal(varInfinity, Infinity, "Infinity");

    //NaN -- Not a Number
    var varNaN = parseInt("abc");
    equal(varNaN.toString(), NaN.toString(), "NaN �ַ����Ƚ�");
    notEqual(varNaN, NaN, "NaNֱ�ӱȽ�");  //ע�⣺NaN ֵ�ǳ����⣬��Ϊ�����������֡��������κ�������������ȣ����� NaN ����Ҳ������ NaN

    var iNumber = 100;
    ok(iNumber == 100, "��ֵ");
    ok(typeof (iNumber) == "number", "typeof");
});

test("�����﷨", function() {
    var iNumber = 2;
    var strNumber = "";
    switch (iNumber) {
        case 0:
            strNumber = "Zero";
            break;
        case 1:
            strNumber = "One";
            break;
        case 2:
            strNumber = "Two";
            break;
        default:
            strNumber = "Unknown";
            break;
    }
    equal(strNumber, "Two", "����Switch");

    with (navigator) {
        var strAppName = appName;
        var strAppCodeName = appCodeName;

        ok(strAppName != "" && strAppCodeName != "", "���� with() �﷨����ȡ����������ݣ�������");
        //alert(strAppName);
    }

    equal(1, 1, "TODO: undefined ���� JavaScript ����, ����ֱ��ʹ�� ?");
});

test("�ַ���", function() {
    var strInfo = "";
    for (var i = 1; i < 10; i++) {
        strInfo += i;
    }
    equal(strInfo, "123456789", "�ַ������������(+)�ᰴ�ַ������ӵķ�ʽ");

    var strHelloworld = "hello world";
    ok(strHelloworld.length == 11, "String Length");

    ok(strHelloworld.charAt(0) == "h", "CharAt");       //���ظ��ַ���λ�ڵ�<λ��>λ�ĵ����ַ�(Unicode)
    ok(strHelloworld.charAt(strHelloworld.length - 1) == "d", "CharAt(Length-1)");


    //charCodeAt -- ���ظ��ַ���λ�ڵ�<λ��>λ�ĵ����ַ��� ASCII �롣
    //var strChina = String.fromCharCode();   //����һ���ַ��������ַ���ÿ���ַ��� ASCII ���� a, b, c... ����ȷ����

    var strHelloArray = strHelloworld.split(" "); //split����һ�����飬�������Ǵ�<�ַ�������>�з��뿪����
    ok(strHelloArray.length == 2, "split length");
    ok(strHelloArray[0] == "hello", "Split Pos[0]");
    ok(strHelloArray[1] == "world", "Split Pos[1]");
});

test("�����", function() {
    equal(100 == "100", true, "���ֺ��ַ����Ƚ�(==) -- ���");
    equal(100 === "100", false, "���ֺ��ַ����Ƚ�(===) -- �����");

    equal(0xF1 & 0xE2, 0xE0, "�����");
    equal(0xF1 | 0xE2, 0xF3, "�����");
    equal(0xF1 ^ 0xE2, 0x13, "λ������");
    
});

test("����", function() {
    var myArray = new Array();
    myArray[0] = "abc";
    myArray[1] = 123;

    var strArrayJoin = myArray.join(" "); //join�����������еĸ���Ԫ����<�ָ���>�������γ�һ���ַ�������Ӱ����������
    ok(strArrayJoin == "abc 123");

    //ע�⣺JavaScript ֻ��һά����, ���Ҫģ���ά���飬��Ҫ�������Ԫ������Ϊ����
    myArray[2] = new Array();
    myArray[2][0] = "1000";

    ok(myArray.length == 3, "array length");
    ok(parseInt(myArray[2][0]) == 1000, "array element");

    myArray.push("push info");
    equal(myArray.length, 4, "array push");
});

function sortLesser(a, b) {
    return (b - a);
}

test("��������", function() {
    var myArray = new Array();
    for (var i = 1; i < 10; i++) {
        myArray[i] = i;
    }
    equal(myArray.join("") ,"123456789");
    myArray.sort(sortLesser);       //���Ӵ�С����
    equal(myArray.join("") , "987654321");

});

test("Math", function() {
    //alert("PI=" + Math.PI);
    equal(Math.ceil(1.5), 2, "ceil ���ش��ڵ��� x ����С����");
    equal(Math.floor(1.5), 1, "floor ����С�ڵ��� x ���������");
    equal(Math.round(1.5), 2, "round ���� ����������ֵ");

    var myRandomNumber = Math.random() * 40 + 60;
    ok(myRandomNumber >= 60 && myRandomNumber <= 100, "random ���ش��� 0 С�� 1 ��һ�������");
});

test("Number", function() {
    ok(Number.MAX_VALUE < Number.POSITIVE_INFINITY);    //���ֵ < �������
    ok(Number.NEGATIVE_INFINITY < Number.MIN_VALUE); //������� < ��Сֵ
});

test("Date", function() {
    //Date���ڶ������������Դ�������һ�����ڣ��� 0001 �굽 9999 �꣬���ҿ��Ծ�ȷ���������� 1/1000 �룩
    //��������ʱ�䣬�����ָ��ʱ���������á�UTC������ʱʱ��
    var d = new Date(2012, 11, 21, 1, 2, 3, 400);
    equal(d.getTimezoneOffset(), -480, "�й��Ƕ����������������ʱ������ķ�����Ϊ -480(����Ϊ��)");
    equal(d.getMonth(), 11, "ע���·��Ǵ� 0 ��ʼ��");
    equal(d.toGMTString(), "Thu, 20 Dec 2012 17:02:03 UTC", "��ȡGMT(��������ʱ��)��ʽ���������ַ���");
    ok(d.getFullYear() == 2012);

    var dParser = Date.parse(d);
    equal(dParser, 1356022923000, "Date.parse ���ظ����ڶ�����ڲ���﷽ʽ(��1970��1��1����ʱ����ʼ���㵽���ڶ�����ָ�����ڵĺ�����");
    //equal(d.toString(), dParser.toString(), "ֱ�Ӷ���Ͳ���parse ���ַ�ʽ��� -- TODO: Ϊʲô����ֱ���õȵķ�ʽ��");
});

test("TODO: ȫ�ֺ���", function() {
    equal(1, 1, "TODO: eval");  //eval�������ڵ��ַ���������׼������ʽ������

    var strSrcURL = "<>[]{}+-=,.;;'\"";
    var strTargetURL = "%3C%3E%5B%5D%7B%7D+-%3D%2C.%3B%3B%27%22";
    equal(escape(strSrcURL), strTargetURL, "escape ���ַ�����URL���뷽��������(��ո��Ϊ %20 ),��Ҫ��'+'���룬��Ҫ�Ӳ���(1)");
    equal(unescape(strTargetURL), strSrcURL, "unescape �����������ַ�����Ϊһ���ַ���");
    equal(isFinite(parseInt("abc")), false, "isFinite -- ��������ڵ������ǡ����ޡ���(MIN_VALUE��MAX_VALUE֮��)�ͷ���true");

    equal(parseInt("123"), 123, "parseInt -- �������ڵ�����ת��������֮���ֵ");
    equal(parseInt("0123"), 83, "ǰ����0ʱĬ�ϻᰴ�հ˽��Ʒ�����������ɴ���");
    equal(parseInt("0123", 10), 123, "parseInt(xxx, 10) -- ǿ�ư���10���Ʒ�ʽ���������Ƽ���");

    equal(isNaN(parseInt("abc")), true, "parseInt -- �������ĸ��ͷ���򷵻ء�NaN")

    equal(parseFloat("123.456"), 123.456, "parseFloat���ذ������ڵ��ַ���ת���ɸ�����֮���ֵ");

    equal("123".toString(), "123", "toString�Ѷ���ת�����ַ���");
    
    equal("123".toString(16), "123", "0x7B -- TODO(Ϊʲô���У�):toString(N)����ת�����ض�����");
});

/*
function SomeFun(num1, num2, num3) {
    for (var i = 0; i < arguments.length; i++) {
        alert(arguments[i]);
    }
}
*/

test("�Զ��庯��", function() {
    equal(1, 1, "TODO:�����ڲ������� arguments ��������������ⲿ������ú���ʱָ���Ĳ�����Ϣ");
    //���������ں����л�ȡ���������� -- �����Կ�����˵���ɶ��Խ��ͣ�ֻ�ڵ���ʱʹ��?
    
    //SomeFun(100, "abc", new Date(), new String("abcde"));

});


//ָ���¼��������ķ���
//1.ֱ���� HTML �����ָ��, onload="xxx" onunload="xxx"
//2.��д�ض������ض��¼��� JavaScript��ͨ�������á�
//  �磺<script language="JavaScript" for="����" event="�¼�">�¼����������� </script>
//3.�� JavaScript ��˵��, <����>.<�¼�> = <�¼��������>; �� window.onerror = globalError; 
test("TODO: �¼��������", function() {

    ok(1, "TODO:�¼��������ķ���ֵ(����beHandled)��false������Ҫϵͳ��������true�����Ѿ��������");
});


//window.onerror = globalError; //ע��û����������"()"
//  ����ֵ��
//    true --
//    false -- 
test("TODO: ���������", function() {

    ok(1, "TODO");
});
