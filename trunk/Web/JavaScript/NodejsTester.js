"use strict";
/************************************************************************************************************
* NodeJs(http://nodejs.org/) -- ʹ��Google��V8�����(Chrome�����)�������ͺ�ִ��JavaScript����
*   npm(node packagemanager) -- node������������������packageȫ��ͨ��npmȥ��װ��
*     npm��װuglify-js <== npm �Cg install uglify-js
************************************************************************************************************/


module("NodejsTester", {
    setup: function() {
        //dump("in NodejsTester::setup\n");
    },
    teardown: function() {
        //dump("in NodejsTester::teardown\n");
    }
});

test("NodeTester", function() {
    equal(1, 1, "NodeJs");
});
