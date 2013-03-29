//����Ƿ�֧��HTML5����Ƶ����
function checkHtml5Video() {
	if (!!document.createElement('video').canPlayType) {
		var vidTest = document.createElement("video");
		oggTest = vidTest.canPlayType('video/ogg; codecs="theora, vorbis"');
		if (!oggTest) {
			h264Test = vidTest.canPlayType('video/mp4; codecs="avc1.42E01E, mp4a.40.2"');
			if (!h264Test) {
				document.getElementById("checkVideoResult").innerHTML = "Sorry. No video support."
			}
			else {
				if (h264Test == "probably") {
					document.getElementById("checkVideoResult").innerHTML = "Yes! Full support!";
				}
				else {
					document.getElementById("checkVideoResult").innerHTML = "Well. Some support.";
				}
			}
		}
		else {
			if (oggTest == "probably") {
				document.getElementById("checkVideoResult").innerHTML = "Yes! Full support!";
			}
			else {
				document.getElementById("checkVideoResult").innerHTML = "Well. Some support.";
			}
		}
	}
	else {
		document.getElementById("checkVideoResult").innerHTML = "Sorry. No video support."
	}
}

//�ж��Ƿ�Ϊ����
function isPrime(n) {
	if (n == 0 || n == 1) {
		return false;
	}
	var bound = Math.floor(Math.sqrt(n));
	for (var i = 2; i <= bound; i++) {
		if (n % i == 0) {
			return false;
		}
	}
	return true;
}

//ֱ�Ӽ���С�ڵ��� maxNumber �����������ĸ��� -- ���ܺķѺܳ���ʱ��
function calculateNormal(maxNumber) 
{   
	var count = 0;
	for (var i = 2; i <= maxNumber; i++) { 
		if (isPrime(i)) {      
			count++;     
		}   
	}
	return count;
}

//ͨ�� setTimeout()�Ѽ�������ֳ����ɸ�С����ķ�ʽ������С�ڵ��� maxNumber �����������ĸ���
//TODO: ��ô����?
function calculateUsingTimeout(maxNumber) {
	var jobs = 10, numberPerJob = Math.ceil(maxNumber / jobs);
	var count = 0;

	function calculate(start, end) {
		for (var i = start; i <= end; i++) {
			if (isPrime(i)) {
				count++;
			}
		}
	}

	var start, end, timeout, finished = 0;
	function manage() {
		if (finished == jobs) {
			window.clearTimeout(timeout);
			alert("������ɣ���������Ϊ��" + count);
		}
		else {
			start = finished * numberPerJob + 1,
                end = Math.min((finished + 1) * numberPerJob, MAX);
			timeout = window.setTimeout(function() {
				calculate(start, end);
				finished++;
				manage();
			}, 100);
		}
	}
	manage();
}

/*///////////////////////////////////////////////////////////////////////////////
//һ���Զ����Form������Ԫ��(input)�����Ƿ���Ч�ķ���
//onchange ������ʱ������ʱ������ oninput ��ÿ���ַ��ı�󴥷�
window.onload = function() {
	var inputs = document.getElementsByTagName("input");
	for (var i = 0; i < inputs.length; i++) {
		if (!inputs[i].willValidate) {
			continue;
		}
		inputs[i].onchange = function() {  //�� onchange �¼�����������������HTML5�ṩ�� checkValidity ����������Ч�Լ��
			if (!this.checkValidity()) {
				this.style.border = 'solid 2px red';
				this.style.background = '';
			} else {
				this.style.border = '';
				this.style.background = 'lightgreen';
			}
		}
	}
}
//*/

///////////////////////////////////////////////////////////////////////////////