//�ռ����������Ϣ�����п���ʹ�� this.xxx ����������������
function BrowserInfo()
{
    var agent = navigator.userAgent.toLowerCase();
    
    this.major = parseInt(navigator.appVersion); //���汾��
    this.minor = parseFloat(navigator.appVersion);//ȫ�汾��
    
    //�Ƿ� Netscape
    this.IsNetscape = ((agent.indexOf('mozilla')!=-1) &&
    ((agent.indexOf('spoofer')==-1) && 
    (agent.indexOf('compatible') == -1)));
    
    this.IsIE = (agent.indexOf("msie") != -1); //�Ƿ� IE
}

var gBrowserInfo = new BrowserInfo();