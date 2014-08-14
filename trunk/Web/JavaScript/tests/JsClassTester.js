"use strict";

//JavaScript�̳����ϵ�� -- http://www.cnblogs.com/sanshi/archive/2009/07/08/1519036.html

//JS �����������������ԣ����ǻ��ڶ��������,�������ﶼ�Ƕ��� -- ������������ĸ����C++�Ȳ�һ��
//JS ���ö��󻯱�̣�����������̣���ÿ�����������Լ������ԡ��������¼���������ȫ���������ʽ���ԣ�ֻ�ǻ��ڶ���(Object-Based)�ķ�ʽ
//JS û����ĸ�� -- ���á�ԭ�ͻ��̳�(prototypal inheritance)���ĸ��������������������¶��󣬶���Ҳ���Լ̳�����������
//   �κκ��������Ա�ʵ����Ϊһ������ 
//   ԭ��(prototype) -- ��prototype������һ��ģ�棬�´������Զ�����������ģ��(prototype)��һ������


//�������������ı�̷�ʽ��
//1. ���� MyMathClass ? ÿһ������֮���ö���"," �ָ�
//2. ����CShape��ԭ��prototyp��ʽ(���Ƽ���) -- ����ʵ�ּ̳У�ÿ���඼��һ�� __proto__ ��ԭ����������� Object: 
//   ���캯��: function ClassName(�����б�) { ʵ�� };  ClassName.prototype.������ = function(�����б�) { ����ʵ��; }, 
//     ע�⣺constructorʼ��ָ�򴴽���ǰ����Ĺ��캯��
//   ʹ�ã� var ������ = new ����(����);
//   1.����������prototype(ԭ��) ���԰����˿�����Ϊ�����¸����Ļ�����(base reference)
//   2.˽�з���(private method)���ں����ڲ�����ĺ���������ֻ���ں����ڲ����á����⣺���˶���ĺ����⣬�������������������ʹ�ã��ǻ���û��ʲô�ô��ˣ�
//   3.��Ȩ����(privileged method)���鿴�����������˽�б�����ͬʱ�����û��Թ����������ʵķ�����Ҳ�ں����ڶ��壬������ this. ǰ׺��
//     ˵����Ϣ:http://javascript.crockford.com/private.html
//     ��̬���ɴ���(��Ȩ�������¶���ʵ����ʱ�Ŵ���)�� prototype �ϰ�һ���򵥵ķ����������󣬵����ܸ�ǿ������(�μ� "CUser(properties)" )
//   4.��̬���� -- 
//   5.ʹ�� uber ������ parent
//?��������(ͨ�������Ű�������һЩ����ֵ����?) -- { ������: ����ֵ }

//ע��д��:
//MyClass.prototype.xxx = function(){ ... };    -- ����д�����޸�prototype, ok(myObj.constructor == MyClass);
//MyClass.prototype = { xxx : function(){...} } -- ����д���Ǹ���prototype���ȼ��� MyClass.prototype = new Object({...}); ok(myObj.constructor == Object)

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
    
    //�������Ȩ����(ע��ǰ��� this. ) -- �����ܱ��̳еķ���,ͨ�� log �ɷ����䷽������ __proto__ ����
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

CUser.prototype.whoAreYou = function(){
    return "I'm " + this.name + " and my birthday is " + this.birthday;
}

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

//function CShape(){
//    this.name = "unknown";
//}
function CShape(name){ //���캯���� ע�⣺JSû�����صĸ����������һ��Ĭ�Ϲ����ƺ�û���ô�?
    this.name = name;
}

//����CShape��ԭ��(prototype��ÿ���������е�����)��ԭ���е����Կ��Ա��Զ����������
CShape.prototype = {
    getName : function(){
        return this.name;
    },
    toString : function(){
        return this.name + "(in my toString)";
    }
}


//CRect -- ��CShape�̳�ԭ��prototype�е��������Ժͷ���
function CRect(width, height){  //���캯��
    CShape.call(this, "rect");
    this.width = width;
    this.height = height;
}
CRect.prototype = new CShape();  //��CRect��ԭ��ָ��CShape��һ��ʵ������˿��Ե���CShapeԭ���е���������
CRect.prototype.constructor = CShape;
CRect.prototype.getSize = function(){
    return this.width * this.height;
}


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


var someParamForCall = "in window param";

//����һ���� this �з��� someParamForCall �ĺ���
function returnParamForThis(){      //ע�⣺��Ϊ������strictģʽ��thisĬ��ֵ���� undefined(����window)
   return this.someParamForCall;    //����Ҳ�Ƕ���,Ҳ��this,������ʱ����this�ľ���ָ(���Ǻ�������ʱ)
}

//ȫ�ֺ��� apply �� call -- ���Ըı亯����this�ľ���ָ��
//apply��call����������������ͬ��Ψһ�����������������Ĳ������岻ͬ
//apply�÷�?: function xxx(){ if(...) { this.Yyy.apply(this, arguments) } } -- �����������ø�������������������ݲ�����ȥ
test("call and apply", function(){
    
    //���ֵȼ۵��÷�ʽ
    equal(window.returnParamForThis(), "in window param", "����ȫ���еı���ֵ"); //��Ϊ������strcitģʽ��this����Ĭ��ָ��window�����Դ˴�������� window. �ķ�ʽ����
    equal(returnParamForThis.apply(window), "in window param", "�ں�����ʹ��apply");
    
    var paramObj = { //�Զ���һ�����󣬲����˶�������� someParamForCall ָ��ȫ�ֺ��� someParamForCall
        someParamForCall : "in Object Param",
        returnParamForThis : window.returnParamForThis,
    };
    //���ֵȼ۵��÷�ʽ
    equal(paramObj.returnParamForThis(), "in Object Param", "����paramObj�����еı���ֵ");
    equal(returnParamForThis.apply(paramObj), "in Object Param", "�ں�����ʹ��apply�ı�this");
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
	equal(userObj1.constructor, CUser, "constructorʼ��ָ�򴴽���ǰ����Ĺ��캯��");
	equal(userObj1.constructor.constructor, Function, "���캯����constructor�Ǻ�������");
	
	console.log("prototype ��������CUser����: %o", userObj1);
	equal(userObj1.whoAreYou(), "I'm fishjam and my birthday is Mon Apr 30 1979 12:00:00 GMT+0800 (�й���׼ʱ��)", "���ù��з���");
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

test("�̳�", function(){
    equal(1, 1, "test");
    
    //new ʱ�ڲ���ʵ������:
    //  1.����һ���հ׶��� -- new Object()
    //  2.���� CShape.prototype �е�����(��ֵ��)������ն�����(�ڲ�ʵ��ʱ���ǿ�������һ�����ص�����)
    //  3.���������ͨ��this�ؼ��ִ��ݵ����캯���в�ִ�й��캯��
    //  4.���������ֵ������unkShape
    var unkShape = new CShape("shape");
    console.log("unkShape=%o, toString=%s, getName=%s", unkShape, unkShape.toString(), unkShape.getName());

    var rectShape = new CRect(10, 20);
    console.log("rectShape=%o, toString=%s, getName=%s", rectShape, rectShape.toString(), rectShape.getName());
    equal(rectShape.getSize(), 200, "���þ��εļ����������"); 
    
});

//��JavaScript��������Array��չһ����ȡ��Сֵ�ķ���
//����: ��Array��ԭ���������չ�����󣬵�ʹ��for-inѭ������ʱ�������չ����Ҳ�ᱻѭ������
//       ���������ʹ�� hasOwnProperty �ж�
Array.prototype.min = function() {
    var min = this[0];
    for (var i = 1; i < this.length; i++) {
        if (this[i] < min) {
            min = this[i];
        }
    }
    return min;
};

Array.prototype.sum = function(){
    var total = 0;
    for (var i = 0; i < this.length; i++) {
        total += this[i];
    }
    return total;
}

test("��չϵͳ�ṩ�Ĺ���" , function(){
    var arrNumber = [ 2, 4, 6, 8, 10, 1, 3, 5, 7, 9];
    equal(arrNumber.min(), 1, "��չϵͳ��Array, ����min����");
    equal(arrNumber.sum(), 55, "��չArray������sum����");
    
    for( var attr in arrNumber){
        if (!arrNumber.hasOwnProperty(attr)) {
            console.log("%o", attr); //���ӡ�� "min" �� "sum"�� TODO:Ϊʲôû�д�ӡ��10?
            ok(attr== "min" || attr =="sum", "�Զ������չ����for-inѭ��ʱ��ö�ٳ���");
        }
    }
});