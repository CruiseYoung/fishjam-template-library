#pragma once

/******************************************************************************************************
* DOM(Document Object Mode) -- �ĵ�����ģ�ͣ������Զ�����ƽ̨�����Եķ�ʽ���ʺ��޸�һ���ĵ������ݺͽṹ��
*   �Զ���ķ�ʽ����XML�ĵ����ݡ�Javascriptͨ���ĵ�����ģ��(DOM)������XUL�е�Ԫ�ء�
*
* 
* tabbrowser -- ���������?
* 
* ����ҳ���¼�
*   autocompletepopup -- 
*   onnewtab --
*
* ����
*   document -- ȫ�ֱ��� ?
*     write()
* 
* ҳ���¼���
*   window.addEventListener("load", xxx);
*   appcontent.addEventListener("DOMContentLoaded", this.onPageLoad, true); //var appcontent = document.getElementById("appcontent");
*   window.addEventListener("pagehide", OnPageUnload, false);
* 
* ����gecko����ʵ����2 ��DOM���¼�����������˿��Զ��嶯̬���¼���������
*   var item = document.getElementById('menu-item-custom');
*   function handleCommandEvent(aEvent) {
*     alert('OK');
*     item.removeEventListener('command', handleCommandEvent, false);
*     item.parentNode.removeChild(item);
*   }
*   item.addEventListener('command', handleCommandEvent, false);
* 
******************************************************************************************************/
#if 0
//Sample 
var bar = document.getElementById('toolbar');
bar.removeChild(bar.childNodes[1]);
bar.appendChild(document.createElement('button'));
bar.lastChild.setAttribute('label', 'Hello!');

#endif
