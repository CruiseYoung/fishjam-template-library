/*************************************************************************************************************************
* HTML5 �������˺ܶ��µ�Ԫ��
*  
*************************************************************************************************************************/


module("HTML5Tester", {
    setup: function() {
        //dump("in MathTester::setup\n");
    },
    teardown: function() {
        //dump("in MathTester::teardown\n");
    }
});

test("CanvasTester", function() {
    var c = document.createElement("canvas");
    var ctx = c.getContext("2d");
    var imgData = ctx.createImageData(100, 100);
    equal(imgData.data.length, 100 * 100 * 4, "ÿһ�����ض��� RGBA");
    
    
});
