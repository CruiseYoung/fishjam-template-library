/*************************************************************************************************************************
* ���ֺ�ִ̨�з�ʽ�ķ�װ(ͳһ�ӿڣ��������л���ѡ��ʵ��) -- https://code.google.com/p/jsworker/
*
* ��ִ̨�У�������� JavaScript���� �ǵ��߳�ִ�еģ�һ����˵ʵ�ַ�ʽ�����δ��ڲ���������¼�������ȡ���¼���ִ�С�
*   timer = setTimout(func, time [,args]) -- ������ָ���ļ��ʱ��֮��ִ��ĳ�� JavaScript ���������ʱʹ�� clearTimeout(timer)
*     �����÷�����һ����Ҫ�ϳ�ִ��ʱ�������ֳ����ɸ�С������ЩС����֮���� setTimeout()��������
*   id = setInterval(func, time [,args]) -- ���ø���ָ���ļ���ظ�ִ��ĳ�� JavaScript ���������ʱʹ�� clearInterval(id)
* 
*   Google Gears -- Google������������������(�ѱ�Web Worker����)��������ǿ����������Ĺ���(Ӧ�ó��򱾵ػ��桢�������ݿ�� JavaScript �����̳߳ص�)
*     google.gears.factory.create('beta.workerpool') -- ����һ�������̳߳أ�ͨ����Ϣ���ݻ��ƽ��н���(onmessage)
*     createWorker(scriptText) -- ����ִ�нű��Ĺ����߳����񣬷��ر�ʶ��
*     createWorkerFromUrl(scriptUrl) -- ��������URL�������ز�ִ�еĹ����߳����񣬷��ر�ʶ��
*     sendMessage(message, destWorkerId) --  �߳�֮�������Ϣ����
*   
*   Web Worker -- HTML5 �н��Goolge Gears���γɵ�ʹ�ù����̵߳ı�׼����
*     ר�������߳�(Worker) -- ֻ��Ϊһ��ҳ����ʹ�ã�ͨ��ָ����Ҫִ�е�JavaScript�ļ���URL�� Worker ���췽������
*     �������߳�(SharedWorker) -- ���Ա����ҳ��������ʹ�ã�ÿ��ҳ�涼�����ӵ��ù������̵߳�ĳ���˿ڣ�port�����档ҳ��ͨ���˶˿��빲�����߳̽���ͨ�š�
*       
*************************************************************************************************************************/

module("ThreadTester", {
	setup: function() {
		//dump("in MathTester::setup\n");
	},
	teardown: function() {
		//dump("in MathTester::teardown\n");
	}
});

test("setTimeoutTest", function() {
    ok(1 == 1, "TODO:setTimeOut");
});

//google Gears ������ṩ��JavaScript�̳߳ع���
test("googleGears", function() {
/*
	var workerPool = google.gears.factory.create('beta.workerpool');

	workerPool.onmessage = function(a, b, message) {
		var result = message.body;
		alert("������ɣ���������Ϊ��" + result);
	};

	function calculate() {
		var limit = parseInt(document.getElementById("limit").value) || 100000;
		var childWorkerId = workerPool.createWorkerFromUrl('prime.js');
		workerPool.sendMessage(limit, childWorkerId);
	}
	
	
	//prime.js �ļ��� �����߳���ִ�е� JavaScript �����е�ͨ�Ų���
	var wp = google.gears.workerPool; 
	wp.onmessage = function(a, b, message) { 
		var limit = message.body; 
		var count = calculateNormal(limit); 
		wp.sendMessage(count, message.sender);  //ͨ�� message.sender���Ի�ȡ��������Ϣ�Ĺ����̳߳صı�ʶ�������Ѽ�����ͨ�� sendMessage()���ͻ�ȥ
	}
*/
	ok(1 == 1, "TODO: Google Gears");
});

test("WebWorkerTester", function() {
    ok(1 == 1, "TODO: WebWorkerTester");

    //��ҳ�洴����һ��ר�������̣߳������ں�ִ̨�� JavaScript �ļ� prime_worker.js
	/*
	var worker = new Worker("prime_worker.js"); 

	 worker.onmessage = function(event) { 
		var result = event.data; 
		alert("������ɣ���������Ϊ��" + result); 
	 }; 

	 function calculate() { 
		var limit = parseInt(document.getElementById("limit").value) || 100000; 
		worker.postMessage(limit); 
	 }
    */
 
	/*
	//ר�������߳���ִ�е� JavaScript ���� -- �� onmessage��ֱ�Ӷ�����յ���Ϣʱ�Ĵ�����
	onmessage = function(event) { 
		var limit = event.data; 
		var count = calculateNormal(limit); 
		postMessage(count); 
	}
    */
});

test("WebSharedWorkerTester", function() {
    ok(1 == 1, "TODO: WebSharedWorkerTester");

    /*
	var worker = new SharedWorker("prime_worker.js"); 

	worker.port.onmessage = function(event) { 
		var result = event.data; 
		alert("������ɣ���������Ϊ��" + result); 
	}; 

	function calculate() { 
		var limit = parseInt(document.getElementById("limit").value) || 100000; 
		worker.port.postMessage(limit); 
	}
	*/
	
	/*
	//�������߳���ִ�е� JavaScript ���� -- ��Ҫʹ�� onconnect��������յ�����ʱ�Ĵ����߼�
	onconnect = function(event) { 
		var port = event.ports[0]; 
		port.onmessage = function(event) { 
			var limit = event.data; 
			var count = calculateNormal(limit); 
			port.postMessage(count); 
		}; 
	}
     */
});