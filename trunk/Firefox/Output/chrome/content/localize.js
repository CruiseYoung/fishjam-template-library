//properties �ļ���������FF������ʾ����


//����д�������ȫ�ֱ����ķ�ʽ���أ�������޸� properties �ļ���������� FF ������Ч

//*
//ע�⣺ÿһ��(��������)֮��ͨ������(,)�ָ��������Ƿֺţ���β���÷ֺ�
//  ����д��ʵ���϶�����һ��ȫ�ֵı��������һ���js�ļ�����ʱ��ִ��
var localize =
{
    _bundle: Cc["@mozilla.org/intl/stringbundle;1"]
    .getService(Ci.nsIStringBundleService)
    .createBundle("chrome://fishjamTest/locale/res.properties"),

    getLocalizedMessage: function(strKey) {
        var strValue = this._bundle.GetStringFromName(strKey);
        return strValue;
    }
};

/*/

//����д���ƺ�Ҳ����ˢ�� properties �ļ�
function Localize() {}
    Localize.prototype = {
    getLocalizedMessage: function(strKey) {
        var bundleService = Cc["@mozilla.org/intl/stringbundle;1"].getService(Ci.nsIStringBundleService);
        var bundle = bundleService.createBundle("chrome://fishjamTest/locale/res.properties");
        var strValue = bundle.GetStringFromName(strKey);
        return strValue;
    },
    anotherFun : function() {
    }
};
var localize = new Localize();

//*/
