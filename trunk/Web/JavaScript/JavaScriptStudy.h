//JavaScript �����ŵ���ͨ -- P107(Navigator����Ĳ�νṹ)
//��ͨJavaScript xxxx .pdf -- P33,������,���������ô��롣(������ڿ������ŵ���ͨ�󣬴�ͷ�ٿ�һ��)

//������רҵJavaScript����ĸ�������֮һ�ǣ����١���̬���ṩ����������Ľӿڣ�ͬʱ��֤����Ŀ������


//<a href="javascript:self.close()"> �ر� </a>

//??TODO: ����ʽJavaScript �ƺ�����Ҫ������ʹ�����ڲ�ͬ���������ʹ��ʱ���������򽵼�����(����Ӧ��ͬ�������)
//DOM -- Document Object Model, domObj.onmouseout = function() { xxxx; };  //��������
//DHTML -- ��DOM���¼������Ļ����ϲ�����DHTML����ʵ����JavaScript��DOMԪ���ϵ�CSS����֮��Ľ���

//������������������ -- �ƺ����κεط�������ʹ�� ?
//����:
//  function sendMessage(msg, obj) { obj.handleMsg(msg) }
//����
//  sendMessage("how are you?", handleMsg : function( msg ) { alert("show Msg:" + msg); }); 
/***********************************************************************************************
* ���� -- Firefox �е� Firebug
* 
* JavaScript�����ķ�װ����
*   DOM��ѡ��Ͳ����������������jQuery����ʽAPI��
*   DOM�¼�����һ���ص��Ǽ��¼���delegation���������¼�ð�ݻ����ڸ�Ԫ������һ�������������������ڶ����Ԫ���ϵ��¼���
*   Ajax���򻯷�����XMLHttpRequest API�����Ҽ�ǿ�������ԡ�
*   ������ǿ����Ҫ�ṩһЩ������Ͷ�����в����ı���������jQuery����һЩ���������͵���Underscore��Lodash��
*
* ������MSDN -> Development Tools and Languages -> Visual Studio -> JScript
*
* �﷨��
*   0.JavaScript�����еĶ��������������(reference)������ָ���ֻ���Ǿ���Ķ��󣬶�������һ������ -- ������������
*   1.�������ִ�Сд��ʹ��ǰ��Ҫͨ�� var <������> [ = <��ʼֵ> ] �ķ�ʽ������Ĭ�ϸ�ֵΪ"null"��JavaScript ���������͵�Ҫ���ϸ�
*     null -- һ������Ŀ�ֵ��������δ���壬���߶���֮��û�ж�������κθ�ֵ����ʱ��ֵ��
*     Number.NaN(Not a Number) -- �������޷�������ȷ����ֵʱ�����ظ�ֵ(�� parseInt("��ĸ"))���κ���(����NaN)�����������
*     Infinity -- ����󣬶�Ӧ���� Number.NEGATIVE_INFINITY �� Number.POSITIVE_INFINITY
*   2.����(Array) -- ����ļ��ϣ���ߵĶ�������ǲ�ͬ���͵ģ�ͨ�� [�±�] ����, var myArray = new Array(��ѡ�ĳ�ʼ�б�);
*       JSֻ֧��һά���顣����ģ���ά����Ҫ���� new Array(new Array(), xxx) �ķ�ʽ������ʱͨ�� myArray[2][3] = xxx;
*       join(�ָ���) -- ʹ�÷ָ����������еĸ���Ԫ�ش�������������Ӧ���ַ�����ͨ�������ڵ������?
*       sort([������]) -- ��ָ�������������������簴�����������ֵ��������� function myGreater(a, b){return a-b;}
*   3.Date -- �ɴ洢 0001��9999������һ�����ڣ���ȷ�����룬�ڲ�ʵ����һ������(1970-01-01 00:00:00:000 ��ʼ�ĺ�����)��
*     �粻ָ��ʱ����Ĭ�ϲ���"UTC"���� get/set[UTC]XXXX �ķ���������ȡ�����ö�Ӧ��ʱ��ֵ��
*     getTimezoneOffset() -- �������ڶ�����õ�ʱ�����������ʱ������ķ��������й�(GMT+0800)���� -480��
*   4.�쳣����
*     try { xxxx } catch(e) { alert('some error'); }
*   5.һЩ���õ����⺯��
*     eval() -- �������ڵ��ַ���������׼������ʽ������
*     escape() -- ���������е��ַ����������������ַ������ñ���Ӧ����URL��Ҳ���ǰѿո�д��"%20"���ָ�ʽ
*     parseInt()/parseFloat() -- ���ذ������ڵ�����ת��������/������֮���ֵ�����������ָ�� radix �Է�ֹ���������ַ���(��0xxx);
*   6.JavaScript��������(scope)���ɺ������ֵģ��������ɿ�(block)���ֵ� -- ����ȫ��������ı�������window���������
*     �հ�(closure) -- �ڲ�ĺ����������ô����ڰ�Χ���ĺ����ڵı�������ʹ��㺯����ִ���Ѿ���ֹ,�����Էǳ�ǿ��͸��ӣ�
*       ͨ������ʹ�ô����������(��: ��ִ�е�������������ȫ�ֱ���; )
*       ע�⣺�հ�����ʹ�ø������еı������䷶Χ�ڵ�����ֵ��������ɴ���(��forѭ���ļ�����)
*       http://jibbering.com/faq/notes/closures/
��  7.������(context)���� -- ��this(���봦�ڸö�����)��ȫ�ֶ����this��window?
*      ����.call(someObj, ��������) -- �ں���ִ�����н������Ķ���(this) ����Ϊ someObj��Ȼ����ú�����;
*      ����.apply(somObj, ��������) -- ???
* JavaScript �� HTML5 �Լ������ִ�������е�Ĭ�Ͻű�����,���������ʱ���Բ����� <script> ��ǩ��ʹ�� type="text/javascript"
***********************************************************************************************/

/***********************************************************************************************
* ���󻯱��
*   ����ʱ��className.funName1();
*   var className = {
*     funName1 : function(param1, param2) {
*       this.initialized = true;
*       return [����ֵ];
*     },
*     funName2 : function(param..) {
*     }
*   };
*
*   ����
*   ����
*     �κζ����� toString() ���������ؿɶ����ַ�����ʽ
*   �¼�--��Ӧ�����ڶ����ϵ�����
*
*   
* �ĵ����ո����¼� <== document.captureEvent()
***********************************************************************************************/



//Jscript�ṩSendKeys�����������ڽ�һ�������������͵�����ڡ�

var CFUtil = {
    //��һ���µĴ��ڣ�����ʹ���Ѿ������Ĵ��ڻ�ý���
    openWindow: function(windowName, url, flags, params) {
        var windowsMediator = Components.classes["@mozilla.org/appshell/window-mediator;1"].getService(Components.interfaces.nsIWindowMediator);
        var aWindow = windowsMediator.getMostRecentWindow(windowName);
        if (aWindow) {
            aWindow.focus();
        }
        else {
            aWindow = window.openDialog(url, windowName, flags, params);
        }
        return aWindow;
    },
    
    
   openProgram : function () {
   	    var WshShell = WScript.CreateObject("Wscript.Shell");	//����WshShell����
	    WshShell.Run("notepad",3); //����WshShell�������������±�,���������
	    WshShell.AppActivate("���±�"); //������Ӧ�ó����������ȵ���һ��AppActivate������ȷ��Ӧ�ó���Ĵ��ڱ�����
	    WScript.Sleep(1000);//��ЩGUI�Ľ�����Ӧ�ٶȻ�Ƚ���������ڸ�����֮������ܲ���ʱ�仺��

	    WshShell.SendKeys("ABCD...");	//���Ͱ���������������Ӧ�ó���Ĺ���
	    WScript.Sleep(1000);

	    WshShell.SendKeys("%OF");   //������Ϲ��ܼ�(ALT+O+F���������±����������ô���)
   }
};

//CFUtil.openProgram();
