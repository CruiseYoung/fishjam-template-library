//��JavaScript���ִ�Сд��
//Netscape����������Ϊ�� for ѭ�����ĵ��ظ�д HTML ����ᵼ�����������
//�ַ�������û�� StringBuffer ������?

module("JSLanguageTester", {
    setup: function() {
    //dump("in JavaScriptLanguage::setup\n");
    },
    teardown : function () {
    //dump("in JavaScriptLanguage::teardown\n");
    }
});


//����: ֻ������ĸ�����ֺ�/���»��ߣ����ִ�Сд��Ҫ����ĸ��ͷ��������Ҫ���� û�������ı�������ʹ��(undefined)
//      JS���������͵�Ҫ���ϸ�(��̬����)���������Ϳ����ø����ʼֵ�ķ�����������֮����Ը����������������͵�ֵ
//      1.(�Ƽ���ʽ)ͨ�� typeof(������) �� typeof ������ �ķ�ʽ���Բ�ѯ���������ͣ������������� string, number, array ��
//      2.��һ�ֽ��ж������ͼ��ķ������������ж����е���Ϊ constructor ������
test("����", function() {
    var varNull;
    equal(varNull, null, "����δ�������֮��δ��ֵ����ֵΪ\"null\"");

    ok(0144 == 100, "�˽���");
    ok(100 == 100, "ʮ����");
    ok(0x64 == 100, "ʮ������");

    //��ֵ������ �� �����͵� typeof ����number
    //����
    var iNumber = 100;
    ok(iNumber == 100, "��ֵ");
    ok(typeof (iNumber) == "number", "��������");

    //������ -- ������ָ�����㷨��ʾ
    var varFloat = 3.1415926;
    ok(typeof (varFloat) == "number", "����������");
    equal(-4.1e-8, -0.000000041, "ָ����ʾ����ʾ�ĸ�����");

    var varBool = true; //false
    ok(typeof (varBool) == "boolean", "�������߼�ֵ");
    equal(true, 1, "����ֵ�������ֱ��ʽʱ�Զ���Ϊ1��0");
    equal(false, 0);

    var varInfinity = 1 / 0;
    equal(varInfinity, Infinity, "Infinity");

    //NaN -- Not a Number
    var varNaN = parseInt("abc");
    equal(varNaN.toString(), NaN.toString(), "NaN �ַ����Ƚ�");
    notEqual(varNaN, NaN, "NaNֱ�ӱȽ�");  //ע�⣺NaN ֵ�ǳ����⣬��Ϊ�����������֡��������κ�������������ȣ����� NaN ����Ҳ������ NaN

    var errResult = 123 * "test";  //���� ���� �ַ��� ���޷���ȷת����
    equal(errResult.toString(), NaN.toString(), "�޷���ȷ��������ʱ���� NaN");


    //ʹ�ù��캯���������ж϶�������� -- ���ص��Ƕ���
    ok(iNumber.constructor == Number, "constructor for Number");
    ok("stringInfo".constructor == String, "constructor for String");
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

    switch (strNumber) {
        case "two":
            iNumber += 1;
            break;
        case "Two":
            iNumber *= 2;
            break;
        default:
            iNumber = 0;
            break;
    }
    equal(iNumber, 4, "switch Ҳ�����ж��ַ���(���ִ�Сд)");


    with (navigator) {  //with Ϊһ����һ�����ָ��Ĭ�϶���
        var strAppName = appName;
        var strAppCodeName = appCodeName;

        ok(strAppName != "" && strAppCodeName != "", "���� with() �﷨����ȡ����������ݣ�������");
        //alert(strAppName);
    }

    ok(1, "this ����\"��ǰ\"�����ڲ�ͬ�ĵط���this ����ͬ�Ķ���, ȫ�ִ��� window����");

    equal(1, 1, "TODO: undefined ���� JavaScript ����, ����ֱ��ʹ�� ?");

    //label -- ���Ǯ�ɹ��Ա�����(label)������ break �� continue ���ȷ��ִ�г����ת�Ƶ�,
    //�� label : statement  ָ��������ת��λ�ú� break statement; �뿪��ű�ʾ�ıպ����?
    //TODO: ��ô���� label ?
    var arrayTimes17 = new Array();  //�����ܱ�17��������������
    for (var i = 1; i <= 100; i++) {
        if (i % 5 == 0) {
            continue;       //����5�ı����������Ļ��������оͲ������ 85(5 * 17)
        }
        if (i % 17 == 0) {
            arrayTimes17.push(i);
        }
    }
    equal(arrayTimes17.length, 4, "����continue");  //

    //for...in ѭ����������ж�������� -- ���Ի�ȡ�����ȫ������
    var iCountForIn = 0;
    for (var porp in window) {
        iCountForIn += 1;
        //console.log("%o", porp);
    }
    ok(iCountForIn > 50, "���� (for in) ��ȡ�������������");
});

//�ַ��� -- ˫���Ż������е��ַ����У�ת���ַ�Ϊ "\"
//����ֱ��ͨ�� ��ֵ�ַ������������ɱ�����Ҳ����ͨ�� new String �ķ�ʽ�����ɱ���
test("�ַ���String", function() {
    var strItem = "origial";
    var strItemRef = strItem;

    strItem += " some info";
    ok(strItemRef != strItem, "�޸��ַ����ᴴ���µı���");

    //equal(String.big(strItem), "", "");

    var strAppend = "";
    for (var i = 1; i < 10; i++) {
        strAppend += i;
    }
    equal(strAppend, "123456789", "�ַ������������(+)�ᰴ�ַ������ӵķ�ʽ");

    //����String����ķ���
    var strObject = new String("�ٶ�");
    equal(strObject.anchor('www.baidu.com'), "<a name=\"www.baidu.com\">�ٶ�</a>",
        "anchor���ض�Ӧ�ĳ����ӷ�ʽ��Ȼ����� document.write д���ĵ�");
    equal(strObject.link("www.baidu.com"),   "<a href=\"www.baidu.com\">�ٶ�</a>", "link �� anchor һ��");
    
    equal(strObject.charAt(0), "��", "����ָ��λ�õ��ַ�")
    equal(strObject.indexOf("��", 0), 1, "��0��ʼ����ָ���ַ���");
    equal(strObject.indexOf("��", 0), -1, "û���ҵ���Ӧ���ַ����᷵�� -1");
    equal(strObject.lastIndexOf("��", 0), 0, "lastIndexOf");

    var strHelloworld = "hello world";
    ok(strHelloworld.length == 11, "length���� �����ַ����ĳ���");
    ok(strHelloworld.charAt(0) == "h", "CharAt");       //���ظ��ַ���λ�ڵ�<λ��>λ�ĵ����ַ�(Unicode)
    ok(strHelloworld.charAt(strHelloworld.length - 1) == "d", "CharAt(Length-1)");
    equal(strHelloworld.substring(6, strHelloworld.length), "world", "substring��ȡ�Ӵ�");
    equal(strHelloworld.toUpperCase(), "HELLO WORLD", "toUpperCase���ض�Ӧ�Ĵ�д�ַ���")

    //charCodeAt -- ���ظ��ַ���λ�ڵ�<λ��>λ�ĵ����ַ��� ASCII �롣
    //var strChina = String.fromCharCode();   //����һ���ַ��������ַ���ÿ���ַ��� ASCII ���� a, b, c... ����ȷ����

    var strHelloArray = strHelloworld.split(" "); //split����һ�����飬�������Ǵ�<�ַ�������>�з��뿪����
    ok(strHelloArray.length == 2, "split length");
    ok(strHelloArray[0] == "hello", "Split Pos[0]");
    ok(strHelloArray[1] == "world", "Split Pos[1]");

    //ͨ�� fromCharCode ��ö�Ӧ��Unicode����(�˴������ҵĺ�ɫ��ͷ��һ�����ڲ��Ű�ť)
    ok(String.fromCharCode('0x25B6') == String.fromCharCode('0x25B6'), "String get Unicode");

    /*
    //ϵͳû���ṩ StringBuffer �����?
    var strBuf = new StringBuffer();
    strBuf.append("<?xml version='1.0' encoding='utf-8'?>");
    strBuf.append("<tree id='0'>");
    console.log("%o", strBuf);
    */

});

test("�����", function() {
    //ע�����������˳��Ҳ���������ŵ���

    //���������: +,-,*,/(����),%(ȡģ), ++, --
    equal(17 / 3, 5.666666666666667, "����");
    equal(Math.floor(17 / 3), 5, "�����Ժ�ȡ��");
    equal(17 % 3, 2, "ȡģ");

    //�߼������: &&, ||, !
    equal(!true, false, "�߼���");

    //�Ƚ������: ==, ===, !=, <, <=, >, >=
    equal(100 == "100", true, "���ֺ��ַ����Ƚ�(==) -- ���"); //JavaScript1.2�Ժ�Ż�����(��ת���ɹ������ͺ�Ƚ�)
    equal(100 === "100", false, "���ֺ��ַ����Ƚ�(===) -- �����");

    //�ַ��������: + 

    //λ���������: &, | , ^, <<, >>(����������), >>>(��0����)
    equal(0xF1 & 0xE2, 0xE0, "�����");
    equal(0xF1 | 0xE2, 0xF3, "�����");
    equal(0xF1 ^ 0xE2, 0x13, "λ������");
    equal(0xF1 << 2, 0x3C4, "���Ʋ���");

    //TODO: �ø��������� ?
    equal(0xF1 >> 2, 0x3C, "���������Ʋ���");
    equal(0xF1 >>> 2, 0x3C, "��0���Ʋ���");

    //���������ƵĻ�������һ����ֵ��������Ҳû������
    equal(-2 >> 1, -1, "���������Ʋ���");
    equal(-2 >> 10, -1, "���������Ʋ���");

    equal(-2 >>> 1, 0x7FFFFFFF, "��0���Ʋ���");
    equal(-2 >>> 2, 0x3FFFFFFF, "��0���Ʋ���");

    //��ֵ�����: =, +=, -=, *=, /=, %=, >>=, >>>=, &=, |=, ^= ��

    //���������: ?:
    equal(5 < 7 ? true : false, true, "���������");
});

//�����е����ݴ������� ���� Ϊ������λ����
test("����Array", function() {
    var arrayWithLength = new Array(10);
    equal(arrayWithLength.length, 10, "�����ȵ����鹹�캯��");
    equal(arrayWithLength[9], null, "��ʱ����Ԫ�صĳ�ʼֵ�� null");

    var myArray = new Array("abc", 123);  //����ʼֵ�Ĺ��캯��(�� "��������(dense array)"  )
    //�ȼ��� new ֮�������¸�ֵ��
    //  myArray[0] = "abc";
    //  myArray[1] = 123;

    var strArrayJoin = myArray.join(" "); //join�����������еĸ���Ԫ����<�ָ���>�������γ�һ���ַ�������Ӱ����������
    equal(strArrayJoin ,"abc 123");

    myArray.reverse();  //��ת����Ԫ��
    strArrayJoin = myArray.join(",");
    equal(strArrayJoin, "123,abc");

    //ע�⣺JavaScript ֻ��һά����, ���Ҫģ���ά���飬��Ҫ�������Ԫ������Ϊ����
    myArray[2] = new Array();
    myArray[2][0] = "1000";

    ok(myArray.length == 3, "array length");
    ok(parseInt(myArray[2][0]) == 1000, "array element");

    myArray.push("push info");
    equal(myArray.length, 4, "array push");

    myArray[10] = "new info";
    //console.log("myArray, %o", myArray);
    equal(myArray.length, 11, "ֱ�������±� 10 ��Ԫ�أ�����Զ��������鳤�ȣ����� 0~10 ��11��Ԫ�صĿռ�");

    var sitesArray = ["www.baidu.com", "www.google.com", "www.bing.com"];
    equal(sitesArray.length, 3, "ֱ�Ӷ��岢��ʼ������");
    sitesArray.push("www.soso.com");
    equal(sitesArray.length, 4, "��̬����Ԥ���������Ԫ��");

    //��������ʱֱ�ӳ�ʼ�� -- Schedule ���Զ�����࣬�乹�캯������ Lecture ������Ϊ����
    //var mySchedule = new Schedule([
    //	new Lecture("Math", "Mrs.Jonese"),
    //	new Lecture("English", "TBD"),
    //	]);

    //ע�⣺����д�����Ǹ����鸳ֵ�����Ǹ����������������������(one/two)
    var arrayProperty = new Array();
    arrayProperty["one"] = 1;
    arrayProperty["two"] = 2;
    equal(arrayProperty.length, 0);
    equal(arrayProperty["one"] + arrayProperty["two"], 3);  //ͨ��ȡ���Եķ�ʽ��������
    //equal(arrayMap["one"], 1);
    //console.log("arrayProperty, %o", arrayProperty);

});

//�ȽϺ���������������Array.sort �����䷵��ֵ����
function sortLesser(a, b) {
    return (b - a);
}

test("��������", function() {
    var myArray = new Array();
    for (var i = 1; i < 10; i++) {
        myArray[i] = i;
    }
    equal(myArray.join("") ,"123456789");
    myArray.sort(sortLesser);       //���Ӵ�С����,ȱʡ���򷽷�ʱ�ᰴ���ֵ�˳������
    equal(myArray.join("") , "987654321");
});

//ע�⣺Math���ڲ���������Ƕ�������(�����ܴ���Mathʵ��) -- 
//var math = new Math();  ���׳�"object is not a function"���쳣
test("Math", function() {
    equal(Math.E, "2.718281828459045", "ŷ������");
    equal(Math.LN2, "0.6931471805599453", "2����Ȼ����");
    equal(Math.LN10, "2.302585092994046", "10����Ȼ����");
    equal(Math.LOG2E, "1.4426950408889634", "��2Ϊ��e�Ķ���");
    equal(Math.LOG10E, "0.4342944819032518", "��10Ϊ��e�Ķ���");
    equal(Math.PI, "3.141592653589793", "Բ����PI����");
    equal(Math.SQRT1_2, "0.7071067811865476", "1/2��ƽ����");
    equal(Math.SQRT2, "1.4142135623730951", "2��ƽ����");
    
    equal(Math.abs(-99), 99, "abs���ؾ���ֵ")
    equal(Math.ceil(1.234), 2, "ceil ���ش��ڵ��� x ����С����");
    equal(Math.exp(2), Math.pow(Math.E, 2), "exp ����e��X�˷�")
    equal(Math.floor(1.5), 1, "floor ����С�ڵ��� x ���������");
    equal(Math.log(20), 2.995732273553991, "����x����Ȼ����")
    equal(Math.pow(2,3), 8, "����x��y�η�")
    equal(Math.round(1.234), 1, "round ���� ����������ֵ");

    var myRandomNumber = Math.random() * 40 + 60;
    ok(myRandomNumber >= 60 && myRandomNumber <= 100, "random ���ش��� 0 С�� 1 ��һ�������");
});

test("ͼ��image", function() {
    var img = new Image(); //����ʱ����ָ�� �߶ȡ����
    img.src = "../HTML/img/merglobe.gif";  //����ָ��ͼ�����ַ�ʽ���ص�ͼ�β���ͨ�� window.document.images ����
    equal(img.name, "", "ͼ�ε�name����");
    equal(img.lowsrc, "", "���ٴ���ʱ�ȼ��ص�ͼƬ");
    equal(img.complete, true, "��ʾͼ���Ƿ��Ѿ�װ�����");
    equal(img.width, 100, "���ص�ͼ��Ŀ��");
    equal(img.height, 100, "���ص�ͼ��ĸ߶�");
    //equal(img.border, 0, "�߿��ϸ����λΪ����");
    equal(img.hspace, 0, "�߽���");
    equal(img.vspace, 0, "�߽�߶�");
});

test("Number", function() {
    ok(Number.MAX_VALUE < Number.POSITIVE_INFINITY);    //���ֵ < �������
    ok(Number.NEGATIVE_INFINITY < Number.MIN_VALUE); //������� < ��Сֵ
});

test("Date", function() {
    //Date���ڶ������������Դ�������һ�����ڣ��� 0001 �굽 9999 �꣬���ҿ��Ծ�ȷ���������� 1/1000 �룩
    //��������ʱ�䣬�����ָ��ʱ���������á�UTC������ʱʱ��
    var d = new Date(2012, 11, 21, 1, 2, 3, 400);
    equal(d.getDate(), 21, "����Date�����ʱ��")
    equal(d.getTimezoneOffset(), -480, "�й��Ƕ����������������ʱ������ķ�����Ϊ -480(����Ϊ��)");
    equal(d.getMonth(), 11, "ע���·��Ǵ� 0 ��ʼ��");
    equal(d.toGMTString(), "Thu, 20 Dec 2012 17:02:03 GMT", "��ȡGMT(��������ʱ��)��ʽ���������ַ���");
    equal(d.toUTCString(), "Thu, 20 Dec 2012 17:02:03 GMT", "��ȡUTCʱ���ʽ���ַ�����ͬGMT?");
    equal(d.toLocaleString(), "2012��12��21�� ����1:02:03", "ת��Ϊ���ظ�ʽ�����û����ڵ������õĸ�ʽ");
    ok(d.getFullYear() == 2012);

    var dParser = Date.parse(d);
    equal(dParser, 1356022923000, "Date.parse ���ظ����ڶ�����ڲ���﷽ʽ(��1970��1��1����ʱ����ʼ���㵽���ڶ�����ָ�����ڵĺ�����");
    //equal(d.toString(), dParser.toString(), "ֱ�Ӷ���Ͳ���parse ���ַ�ʽ��� -- TODO: Ϊʲô����ֱ���õȵķ�ʽ��");

    var now = new Date(); //������������Ĺ��죬��ʹ�õ�ǰ���ں�ʱ�䴴��Dateʵ��
    //equal(now.toLocaleString(), 2012, "");
    ok(now.getFullYear() >= 2013, "��ǰ��>=2013");

});

test("TODO:��ɫ", function() {
//JavaScript�����������ɫ�����ͷ���
  equal(1, 1, "TODO:��ɫ")
 
});

test("TODO: ȫ�ֺ���", function() {
    equal(4321, eval("432.1 * 10"), "eval�������ڵ��ַ���������׼������ʽ������, ���еı��ʽ������ ��ѧ���� ?");
    //    try{
    //        eval("hello + world"));
    //        ok(false, "���׳��쳣�����ᵽ����");  //���ַ�ʽû�в���
    //    }
    //    catch(e)
    //    {
    //        ok(true);
    //    }

    var strSrcURL = "<>[]{}+-=,.;;'\"";
    var strTargetURL = "%3C%3E%5B%5D%7B%7D+-%3D%2C.%3B%3B%27%22";
    equal(escape(strSrcURL), strTargetURL, "escape ���ַ�����URL���뷽��������(��ո��Ϊ %20 ),��Ҫ��'+'���룬��Ҫ�Ӳ���(1)");
    equal(unescape(strTargetURL), strSrcURL, "unescape �����������ַ�����Ϊһ���ַ���");
    equal(isFinite(parseInt("abc")), false, "isFinite -- ��������ڵ������ǡ����ޡ���(MIN_VALUE��MAX_VALUE֮��)�ͷ���true");

    equal(parseInt("123"), 123, "parseInt -- �������ڵ�����ת��������֮���ֵ,�����һ��֧��������ֵ���򷵻� NaN");
    //equal(parseInt("0123"), 83, "ǰ����0ʱĬ�ϻᰴ�հ˽��Ʒ�����������ɴ��󣬵�Chrome��ʵ�⻹�ǽ����� 123");
    equal(parseInt("0123", 10), 123, "parseInt(xxx, 10) -- ǿ�ư���10���Ʒ�ʽ���������Ƽ���");
    equal(parseInt("abc").toString(), NaN.toString());
    equal(isNaN(parseInt("abc")), true, "parseInt -- �������ĸ��ͷ���򷵻ء�NaN")

    equal(parseFloat("123.456"), 123.456, "parseFloat���ذ������ڵ��ַ���ת���ɸ�����֮���ֵ");

    equal("123".toString(), "123", "toString�Ѷ���ת�����ַ���");
    equal("123".toString(16), "123", "0x7B -- TODO(Ϊʲô���У�):toString(N)����ת�����ض�����");
});


test("�Զ��庯��", function() {
    //1.���������ȶ����ʹ�ã�
    //2.�����п������Զ����ɵĲ��� arguments ���鴦����Ĳ���,ʹ��ʱ���Լ� ������ǰ׺.
    //3.��������ʱ����ǰ���� "var" ������Ǿֲ��������������ȫ�ֱ�������ʹ���ں����ڲ���

    function SomeFun() { //�˴����д�ϲ����б� -- ���㺯��ʹ����֪�����÷�ʽ
        equal(arguments.length, 4, "�����е� arguments �������");
        equal(SomeFun.arguments.length, 4, "������ǰ׺.arguments");

        strictParams([Number, String, Date, String], arguments);  //ʹ�� Utils �ж���ĸ���������֤����Ĳ���
        for (var i = 0; i < arguments.length; i++) {
            //alert(arguments[i]);
        }

        //�����ڲ���������ʱ��ǰ��û�м� "var"�� �����ȫ�ֱ���
        //tmpGlobalVariable = 100;  //���������䣬�� Qunit ��ѡ��"Check for Globals", �����ʾ����ȫ�ֱ���("Introduced global variable")
    }
    SomeFun(100, "abc", new Date(), new String("abcde"));


    //һ������������������������ת��Ϊ������Զ��庯��
    function makeArray() {
        //������ʱʹ�õ�����
        var arr = [];
        //���������ÿ�����������������飬Ȼ�󷵻�
        for (var i = 0; i < arguments.length; i++) {
            arr.push(arguments[i]);
        }
        return arr;
    }

    //������������һ�����غ����ĺ���
    //  ���ñհ������ԣ�����һ���򵥵����������ֺ͵ĺ���
    function addGenerator(num) {
        return function(toAdd) { return num + toAdd; }
    }

    //�Զ�ִ�е���������
    (function() {
        //�������ֻ�� IE �����Զ�ִ��
        var msg = "�Զ�ִ�е����������������������ʹ��ԭ������д��ȫ�ֵı��������ƺ�ֻ��IE�����Զ�ִ��";
        window.onunload = function() {
            //�󶨺�����ȫ�ֶ��󣬲�ʹ�á����ء���msg����
            alert(msg);
        };
    })();


    //TODO:����Ĳ�����ʲô?
    equal(arguments.length, 1, "TODO:�����ڲ������� arguments ��������������ⲿ������ú���ʱָ���Ĳ�����Ϣ");
    //���������ں����л�ȡ���������� -- �����Կ�����˵���ɶ��Խ��ͣ�ֻ�ڵ���ʱʹ��?
    //ע�⣺1.arguments���������޸�; 2.������β�û���ṩʵ�Σ�Ҳ�ܱ�������� typedef ������ == "undefined" 


    equal(makeArray("a", 1, 10.2).length, 3, "ʹ�� arguments �����ĺ���");

    var addFive = addGenerator(5);
    equal(addFive(4), 9, "����������");
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

var g_myGlobalVariable = "some value";
test("������", function() {
	//alert(this);
    equal(window.g_myGlobalVariable, "some value", "ȫ��������ı�������window���������");
	equal(1, 1);
});

/*
function delayCheckString(msg, time) {
	setTimeout(function() {
		//alert(msg == "some info");   //���alert����һ���Ժ󵯳� "true"
		equal(msg, "some info", "ͨ���հ����ԣ������ܷ����Ѿ������ĺ����еı���");
	}, time);
}
test("�հ�", function() {
	delayCheckString("some info", 1000);
	equal(1,1,"�հ�����");
});
//*/
