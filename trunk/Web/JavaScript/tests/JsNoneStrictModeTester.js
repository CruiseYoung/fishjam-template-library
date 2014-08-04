//���Է� Strict Mode��һЩ����
//"use strict";

/******************************************************************************************************************
* �Ͻ�ģʽ(Strict Mode) -- ͨ�� "use strict;" ����
* Ŀ�꣺�ܸ��������ĵ��ԣ��ϸ�ģʽ���׳������������ע�Ⲣ�޸��ܶ���ǰ���ѱ����ֵ�Ǳ������
*   1.ȥ����with��� -- ��ʹ�û��׳��쳣
*   2.�ֲ������ڸ�ֵǰ�����Ƚ������� -- δ����ʱΪһ��δ�����ľֲ���������ʱ���Զ�����һ��ͬ��ȫ�ֱ�����
*   3.�����е�this����Ĭ��ָ��ȫ��window���� -- this�ڱ���ֵ֮ǰ��һֱ����Ϊundefined����˵��ù���ʱ����ʹ�� new
* ע�⣺
*   1.��Ҫ��ȫ�ֻ����������ϸ�ģʽ -- ���ں����ڲ�������ִ�к������ʽ(IIFE)�У�����������ȵĴ���������
*     ʾ��: (function() { "use strict"; function doSomeFun(){}} ())
******************************************************************************************************************/


module("NoneStrictModeTester", {
    setup: function() {
        //dump("in NoneStrictModeTester::setup\n");
    },
    teardown: function() {
        //dump("in NoneStrictModeTester::teardown\n");
    }
});

test("with", function() {
    with (window.document) {
        equal(links.length > 0, true, "���ϸ�ģʽ�¿���ͨ��with�򻯴��룬�����ܳ���");
    }
});

(function() {
    //"use strict";
    console.log("����Strictģʽ��ʱ��ʹ����with�Ĵ���ᱨ�� 'Strict mode code may not include a with statement'");
    test("with", function() {
        with (window.document) {
            equal(links.length > 0, true, "Chrome��Console�л����");
        }
    });
}());
 
