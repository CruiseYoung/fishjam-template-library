"use strict";

//JS ���ö��󻯱�̣�����������̣���ÿ�����������Լ������ԡ��������¼���������ȫ���������ʽ���ԣ�ֻ�ǻ��ڶ���(Object-Based)�ķ�ʽ
//JS û����ĸ�� -- ���á�ԭ�ͻ��̳�(prototypal inheritance)���ĸ��������������������¶��󣬶���Ҳ���Լ̳�����������
//   �κκ��������Ա�ʵ����Ϊһ������ 

//�������������ı�̷�ʽ��
//1. ���� MyMathClass ? ÿһ������֮���ö���"," �ָ�
//2. ����CUser��ԭ�ͷ�ʽ(���Ƽ���): 
//   ���캯��: function ClassName(�����б�) { ʵ�� };  ClassName.prototype.������ = function(�����б�) { ����ʵ��; }, 
//   ʹ�ã� var ������ = new ����(����);
//   1.����������prototype(ԭ��) ���԰����˿�����Ϊ�����¸����Ļ�����(base reference)
//   2.˽�з���(private method)���ں����ڲ�����ĺ���������ֻ���ں����ڲ����á����⣺���˶���ĺ����⣬�������������������ʹ�ã��ǻ���û��ʲô�ô��ˣ�
//   3.��Ȩ����(privileged method)���鿴�����������˽�б�����ͬʱ�����û��Թ����������ʵķ�����Ҳ�ں����ڶ��壬������ this. ǰ׺��
//     ˵����Ϣ:http://javascript.crockford.com/private.html
//     ��̬���ɴ���(��Ȩ�������¶���ʵ����ʱ�Ŵ���)�� prototype �ϰ�һ���򵥵ķ����������󣬵����ܸ�ǿ������(�μ� "CUser(properties)" )
//   4.��̬���� -- 
// 
//?��������(ͨ�������Ű�������һЩ����ֵ����?) -- { ������: ����ֵ }

var MyMathClass = { //TODO: ������׶�������໹�Ƕ���
    //���캯���У�this.<������> = <��ʼֵ>;
	 someValue: 10, 
	 init : function(value) {
		 this.m_value = value;
	 },
     add : function(num1, num2) {
       return num1 + num2;
     },
	//��������?	 
};

//ע�⣺�����ֲ�ͬ�Ĺ��췽ʽ -- ���ƺ���֧������ ?
//*
//ͨ��ԭ��(prototype)������ -- JS����ʹ�� prototype �������Ѿ�����õĿ�ʵ������������(������ù��캯��)�м������Ժͷ���.
function CUser(name, birthday){      //���캯��
    this.name = name;
    this.birthday = birthday;

    //����ֻ�����ڲ�ʹ�õ�˽�з���
    function dumpInfo() {
        //TODO:������ this.name/this.age �����Ϊʲô?
        //console.log("CUser Init: name=%s, age=%s", this.name, this.age); 
        console.log("CUser Init: name=%s, birthday=%s", name, birthday); 
    }
    dumpInfo();
    
    //�������Ȩ����(ע��ǰ��� this. )
    this.privilegedName = function() {
        return this.birthday;
    };
    
    //JavaScript�����ŵ���ͨP124���ҽ��ܵľɷ��� -- δ�����Ƿ���Ч
    //1.����ȫ�ֵ� CUser_xxx ������ʵ����ʹ�� this.name �ȷ�ʽ�������ڲ�����
    //2.�ڹ����н�������ֵ�����ɶ�Ӧ�ĺ���, �� Xxx = CUser_xxx; Ȼ����缴���� user.Xxx() ����������
}

CUser.prototype.getName = function(){  //����Ĺ��з��� -- �����ǽ�����(getName)��ӵ��˶���� prototype ������(��Ϊ������)
    //dumpInfo();  //��������Щ������ʹ��, ˽�з����ƺ�û��ʲô�ô���
    return this.name;
};
CUser.prototype.getBirthday = function() {
    return this.birthday;
};
CUser.prototype.getAge = function() {
    var age = (new Date()).getFullYear() - this.birthday.getFullYear();
    return age;
};

//��̬����
CUser.clone = function( user ) {
    return new CUser(user.getName(), user.getBirthday());
};
/*/
function CUser(properties) {  //����һ���µ��û����󣬽���һ����������ԵĶ�����Ϊ����
    for (var i in properties) { (function(which) {  //�����ö������������
          var p = i; 
          which["get" + p] = function() {       //���������Ե�һ���µĶ�ȡ��(getter)
            return properties[p];
          };
          which["set" + p] = function(val) {    //���������Ե�һ���µ�������(setter)
            properties[p] = val;
          };
      })(this);
   }
};
//*/


//����һ�����������Ե��������������ʹ��
function User( name ){
	this.name = name;
}

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
	var obj1 = new Object(); //����ϵͳ�ṩ��Object����
	obj1.val = 5;			//����һ����ֵ��䶼������������
	equal( obj1.val, 5, "��ֵʱ�Զ����ӻ���������");
	//obj.val = "change string";
	//equal( typeof obj.val, "string");
	obj1.click = function() { alert("hello"); };
	
	//�������ȼ��� -- ��ͬ���﷨
	var obj2 = {
		val : 5,
		click : function() { alert("hello"); }
	};


    var someObj = new SomeConstructor(1);
    
    var addResult = MyMathClass.add(1 , 2);
    ok(addResult == 3, "Call Class Function");
});

test("funObj", function() {
	//equal(window.name, "test", "ԭ����window.name");
	var me = new User("fishjam");	//������ʵ����Ϊһ������

	equal(me.name , "fishjam", "�򵥵ĺ�����Ϊ����ķ�ʽ"); 
	equal(me.constructor , User, "me.constructor ��User�����һ��ʵ��");  //
	
	var you = new me.constructor(); //
	equal(typeof(me), typeof(you), "ʹ�ö���� construcotr ���Դ�����ͬ���͵Ķ������ƿ������� ?");

    //δ�����ϸ�ģʽʱ���������Ⱦ��bug	
	//User("setUserName");					//���������ĺ�����ʹ�� -- ע�⣺�ϸ�ģʽ�»��׳��쳣������ͨ��new��ʹ�ù���
	// equal(window.name, "setUserName");		//����Ϊ���� window ������õģ���������name���ԣ����������Ⱦ,���׳���Bug?
	new User("setUserName");
	equal(window.name, "");  //�����ϸ�ģʽ�󣬲������������Ⱦ
	   
	
	//����������Ϊ���������������� Function() ��������������������������﷨Ϊ "new Function([arg1,..,argn], functionBody);
	var triple = new Function("n", "return 3*n");  //��������ֵ����3�Ժ�ĺ���
	equal( triple(100), 300, "��������");
});


test("prototype", function(){
//*
	//ʹ�� prototype �������, �����Զ��ڶ����У����к��ʵ�������
	var userObj1 = new CUser("fishjam", new Date(1979, 3, 30, 12, 0, 0, 0)); //1979-03-30 12:00:00:000
	equal(userObj1.getName(), "fishjam", "userObj1.getName() == fishjam");
	equal(userObj1.getAge(), 35, "userObj1.getAge() == 35, ���ˡ�������");
	
	var userCloneObj = CUser.clone(userObj1);
	console.log("%s:%o", userCloneObj.toString(), userCloneObj);
	console.log("%s:%o", userObj1.toString(), userObj1);
	//ok(userCloneObj == userObj1, "��̬����");   //TODO: ��������ıȽ�( == ������������)
/*/
	//����һ���µ��û�����ʵ�������Ѿ����������Ե�һ����������г�ʼ�� -- ���Զ����ɶ�Ӧ�� getter/setter
	var userObj2 = new CUser({ 
	    Name:"fishjam", 
	    Birthday: new Date(1979, 3, 30, 12, 0, 0, 0) 
	});

	ok(userObj2.Name == null);   //ע�⣺��������ΪName������--�������Զ����˽�б���

	equal(userObj2.getName(), "fishjam");
	ok(userObj2.getBirthday().toString() == new Date(1979, 3, 30, 12, 0, 0, 0).toString());
	
	userObj2.setName("fujie");
	equal(userObj2.getName(), "fujie");
//*/

    equal(1, 1, "prototype");

});