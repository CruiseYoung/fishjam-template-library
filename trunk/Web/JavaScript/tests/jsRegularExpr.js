"use strict";

/*************************************************************************************************************************
JavaScript�е�������ʽ
  ? / xxxx / �м������ ������Լ� g(��ȫ��) ?
  RegExp �����ʾ������ʽ, �����﷨Ϊ new RegExp(pattern, attributes);
*************************************************************************************************************************/


module("RegularExprTester", {
    setup: function() {
        //dump("in MathTester::setup\n");
    },
    teardown: function() {
        //dump("in MathTester::teardown\n");
    }
});

test("������ʽ", function() {
    //�ж��ʼ���ַ��
    var bEmail = /.+@.+\.[a-zA-Z]{2,4}$/.test("fishjam@163.com");
    equal(bEmail, true, "�Ϸ����ʼ���ַ");

    bEmail = /.+@.+\.[a-zA-Z]{2,4}$/.test("fishjam");
    equal(bEmail, false, "�Ƿ��ʼ���ַ");
});

test("RegExp, TODO:", function() {
    var regExp = new RegExp();
    equal(1, 1, "TODO:")
});

