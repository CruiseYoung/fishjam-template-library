//JS ���ö��󻯱�̣�����������̣���ÿ�����������Լ������ԡ��������¼�
var MyMathClass = {
    //���캯���У�this.<������> = <��ʼֵ>;
    
     add : function(num1, num2) {
       return num1 + num2;
     },
};

function SomeConstructor(intParam1)
{
    this._value = intParam1;
};


module("JsClassTester", {
    setup: function() {
        //dump("in JsClassTester::setup\n");
    },
    teardown: function() {
        //dump("in JsClassTester::teardown\n");
    }
});

test("Class", function() {
    var someObj = new SomeConstructor(1);
    
    var addResult = MyMathClass.add(1 , 2);
    ok(addResult == 3, "Call Class Function");
    
    ok(1, "with Ϊһ����һ�����ָ��Ĭ�϶���, �μ� window.document ����");
    ok(1, "this ����\"��ǰ\"�����ڲ�ͬ�ĵط���this ����ͬ�Ķ���, ȫ�ִ��� window����");
});