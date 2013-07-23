"use strict";

module("MathTester", {
    setup: function() {
        //dump("in MathTester::setup\n");
    },
    teardown: function() {
        //dump("in MathTester::teardown\n");
    }
});

test("MathTester", function() {
	//�� ʱ��(125S) ת��Ϊ MM:SS ��Ӧ���ַ���
	var mm = Math.floor(125 / 60.0);
	var ss = parseInt(125 % 60);
	ss = (ss < 10) ? '0' + ss : ss;
	var result = mm + ':' + ss;
	equal(result, "2:05", "ʱ��ת��");
});
