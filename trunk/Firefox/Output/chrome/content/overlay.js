var fishjam = {
    onLoad: function() {
        this.initialized = true;
    },

    onMenuItemCommand: function() {
        //1.��һ���µĶԻ���(infoDialog.xul)
        //window.open("chrome://fishjamTest/content/infoDialog.xul", "", "chrome");
        //��һ��û��Firefox ����GUI ��Firefox ����
        window.openDialog("chrome://fishjamTest/content/infoDialog.xul", "_blank", "chrome, all, dialog=no");
        //window.openDialog("chrome://fishjamTest/content/infoDialog.xul", "_blank", "chrome, centerscreen,modal");
        
        //2.ʹ��ϵͳ�ṩ����� ����ǰ�ĵ��ı��⿽����������
        /*
        var myObj = Components.classes["@mozilla.org/widget/clipboardhelper;1"].createInstance(Components.interfaces.nsIClipboardHelper);
        dump("title=" + content.document.title);
        if (myObj) {
        myObj.copyString(content.document.title);
        }
        */

        //3.ʹ���Զ���� C++ XPCOM
        /*
        dump("before create myHello Object\n");
        var myHelloObj = Components.classes["@fishjam.com/hello;1"].createInstance();
        dump("after create myHello Object\n");
        
        if (myHelloObj) {
        myHelloObj.QueryInterface(Components.interfaces.nsIHelloFishjam);
        dump("before call myHelloObj Hello\n");
        myHelloObj.Hello("abcdef");
        }
        else {
        dump("create fishjam hello fail\n");
        }
        //*/
    }
};

//window.addEventListener("load", function(e) { HelloWorld.onLoad(e); }, false);

//window.addEventListener("load", naverToolbar.Load, false);
//window.addEventListener("unload", naverToolbar.Unload, false);
//window.addEventListener("resize", naverToolbar.Resize, false);
//window.addEventListener("AppCommand", ntbTrack.HandleAppCommand, true);
