/******************************************************************************************************************************************
* TODO:
*   1.���ѡ�У����Ƴ�selected�࣬����ͼ���selected��(����ʲô�﷨?) -- $(this)[hasSelected ? "removeClass" : "addClass"]('selected')
*     �ȼ���: if(hasSelected){ $(this).removeClass('selected');} else { $(this).addClass('selected'); }
*   2.jQuery������Ч��ʱҪ���ڱ�׼ģʽ�£�����������𶯻�����(��  -//W3C//DTD XHTML 1.0 Transitional )
*   3.�е�ʱ��ѡ����ΪʲôҪ���� ",this" ? ���� ��������jQuery�С�P152,5.1.4������Ӧ���У� $var options=$("option:selected",this); //��ȡѡ�е�ѡ��
*
* �������߲��Ը������ݵĵ�ַ(��Ƭ������վ) -- TODO: �Ա�������ת��?
*   http://api.flickr.com/services/feeds/photos_public.gne?tags=car&tagmode=any&format=json&jsoncallback=?
*   TODO: jsoncallback ��ô���� ?
*   
******************************************************************************************************************************************/

/******************************************************************************************************************************************
* 08�ڰ���\�ڰ��½�����
*
* ����IDE(�����Զ���ʾ)
*   1.Dreamweaver + jQuery_API.mxp ���(Dreamweaver cs6 �Ѿ�����)
*   2.Aptana -- ����Eclipse��רע��JavaScript��Ajax����IDE��(Windows -> Preferences -> Aptana ->Editors -> Javascript -> Code Assist)
*   3.Eclipse + jQueryWTP �� Spket ���
*   4.Visual Studio 2008 
*     + KB958502 ����(http://archive.msdn.microsoft.com/KB958502)
*     + jquery-vsdoc.js(http://ajax.aspnetcdn.com/ajax/jQuery/jquery-1.9.1-vsdoc.js, ���ļ��汾��jQueryһ�£����� "-" ǰ��Ĳ��ֱ����jQuery�ļ���һ����������ͬĿ¼)
*     ʵ���ƺ�����?
* ��������:
*   1.Chrome����һ��jQuery����չ����  
* 
* JQuery(jquery.com) ��ѡ���Դ(ʹ��MIT���Э��)����һ����������JavaScript��(�����ֻ��20��K)�����ݸ��������,����ؼ��� JavaScript ���
*   �ܷ���ش���HTMLԪ�ء��¼��ȣ�ʵ�ֶ���Ч����Ϊ��վ�ṩAJAX������ʹ��HTMLҳ���ִ����ҳ�����(���ò���js���е��ã�ֻ�趨��id����)
*   
* ʹ�ã�head�а��� <script type="text/javascript" src="jquery.js"></script>
*       jquery.js ���Ա��浽���ػ��߷�����ֱ�����ã�Ҳ����ֱ��ʹ��CDN�ṩ��(�ŵ�:�û��Ѿ����ʹ�ʱ�Ỻ��;ȱ��:��һ������?)
*       Google CDN: http://ajax.googleapis.com/ajax/libs/jquery/1.8.0/jquery.min.js  -- ע��1.8.0 ����ʡ�Գ� 1.8 �� 1 ��(�Զ�ѡ������Ҫ�����߰汾)
*   �����汾��Production version -- �ѱ������ѹ��������ʵ�ʵ���վ
*             Development version -- ������ѹ�������ڲ��ԡ�ѧϰ�Ϳ���
*
* jQuery���� �� DOM ����Ա�
*   jQuery���� -- ͨ��jQuery��װDOM���������Ķ���(��һ������)��ֻ��ʹ��jQuery�ķ���������ʱͨ���ڱ�����ǰ����� "$"
*   DOM ���� -- ͨ�� document.getXxx ���صĶ���
*      $("#foo").html() <== �ȼ��� ==> document.getElementById("foo").innerHTML
*      jQuery����ת��ΪDOM: var domObj = $jObj[index] �� $jObj.get(index), �����Ǵ�������ȡԪ��
*      DOM����ת��ΪjQuery: var $jObj = $(domObj), ������ͨ�� $(objName) ������������, �� $(document), $(this) ��
*
* �����﷨�� $���� jQuery��һ����д��ʽ���� $("#foo") �� jQuery("#foo") �ȼ�; $.ajax �� jQuery.ajax �ȼ�
*   $(selector).action() -- ��Ԫ���Ŷ��� jQuery; ѡ���(selector)��ѯ�Ͳ���HTML Ԫ��; action() ִ�ж�Ԫ�صĲ���
*   $(document).ready(function(){ xxx }); -- �ȴ�domԪ�ؼ������ִ��ָ����䣬���Լ�дΪ $(function(){ xxxx });
*  
* �¼�: -- ���¼��������� return false ���ͬʱ ֹͣ�¼�ð�� �� ��ֹĬ����Ϊ
*   �¼�����(event) -- �¼��������в��ܷ��ʣ��¼�������ִ����Ϻ󣬸ö���ͱ�����
*     metaKey/ctrlKey -- �����¼��л�ȡ <ctrl> ���� -- ������ʲô����(jQueryͨ�� metaKey ͳһ�˸���������<ctrl>��?)
*     originalEvent -- ָ��ԭʼ���¼�����
*     pageX/pageY -- ��ȡ����������ҳ��� x ����� y ����(��װ��������������� -- ���ֺ͹������� )
*     relatedTarget -- �¼����Ԫ��(�� mouseover �¼��� IE::fromElement, mouseout �¼��� IE::toElement )
*     target -- ��ȡ�������¼���Ԫ�أ� �� button, a �ȣ�����object�����Խ�һ����ȡ���������ֵ
*     type -- ��ȡ���¼����͵����ԣ��� "click" ��
*     which -- ����굥���¼��л�ȡ������ ��(1)����(2)����(3)�����ڼ����¼��л�ȡ���̵İ���(�����ִ�Сд���� a/A ���� 65)
*     stopPropagation() -- ֹͣ�¼�ð�ݺ���
*     preventDefault() -- ��ֹĬ����Ϊ����(�� ���������ӵ���ת�������ύ��ť���ύ�� ��)
*   �¼�ð�� -- Ƕ��Ԫ�ص��¼����δ������⼤��, ���� button.onclick() -> body.onclick �� ( �û��ƿ��ܻ�����Ԥ��֮���Ч�� ?) 
*   �¼����� -- �������Ԫ�ؿ�ʼ�����ε������Ԫ��(ע�⣺��Ϊ���������������֧���¼�������jQuery�޷��޸������jQuery��֧���¼�����)
******************************************************************************************************************************************/

/******************************************************************************************************************************************
* ѡ����(Selectors) -- (�����﷨ͬCSSѡ����),��������ʹ�ô�CSS1��CSS3�������е�ѡ������jQuery�����ĸ߼������ӵ�ѡ������ͨ�����������֧��XPathѡ�������������Ա�д�����Լ���ѡ����
*   ���û������������Ԫ��(��û��ָ��id��Ԫ��)������jQuery����Ҳ���ᱨ��(��Ϊѡ�����ķ�����һ�����飬ֻ�Ǵ�ʱ�������Ϊ 0)
*   ѡ���������������ַ�(�� ".", "#","(",")","[","]" ��)�ģ���Ҫ�������ַ�ǰʹ��˫б��(\\)����ת�壬�� id="myId.index" �����ѡ����: $("myId\\.index")�� ����ò�Ҫʹ�������ַ�
*   querySelectorAll() -- W3C��SelectorsAPI�ݰ����ᵽ�ı�׼������Ϊ�����(IE8�Ժ�)ԭ����jQuery���ܻ�ʹ�ø÷������ع�ѡ����
* 
*   ����ѡ����
*     tagType -- ѡ������ָ�����͵�Ԫ�أ� ������ <p> �� $("p"), $("input[type=button]")ѡ������button��ť(�ȼ��� :button ?)
*     #IdObj -- ѡ��  id="IdObj" ��Ԫ�أ�����ѡȡ�κ�Ԫ��
*       ����� <link type="text/css" id="cssfile" href="css/skin_0.css"/>, ͨ�� $("#cssfile").attr("href","css/"+this.id+".css") ���л�ʹ�õ�css�ļ�������̬��������
*     .className -- ѡ������ class="className" ��Ԫ��
*     this -- ѡ��ǰԪ��,ͨ�������¼���������У���ʾ�������¼��Ķ���
*     sel1,sel2 -- �����ѡ����ƥ���Ԫ�غϲ���һ�𷵻�
*     * -- ƥ������Ԫ�� 
*   ���ѡ����
*     ancestor descendant -- ѡȡancestorԪ���������descendantԪ��(�������ж���), �� $("div span")ѡȡ<div>�������<span>��ע��ѡ����� <span>
*     parent > child -- ѡȡ parent Ԫ���µ� child ��Ԫ��(ֻ��������),�� $("div > span")ѡȡ<div>Ԫ����Ԫ������<span>����Ԫ��
*     prevSel + nextSel -- ѡȡ������ prevSel Ԫ�غ�� nextSel Ԫ�أ����������÷��� $("prevSel").next("nextSel")
*     prevSel ~ siblings -- ѡȡ prevSel Ԫ��֮������� siblings Ԫ�أ����������÷��� $("prevSel").nextAll("siblings)
*  ����ѡ���� -- ͨ���ض��Ĺ��˹�����ɸѡ
*     :first -- ѡȡ��һ��Ԫ�أ��� $("div:first")ѡȡ��һ��<div>Ԫ��
*     :last -- ѡȡ���һ��Ԫ��
*     :not(selector) -- ȥ�����������ѡ����ƥ���Ԫ�أ��� $("input:not(.myClass)") ѡȡclass����myClass��<input>Ԫ��
*     :even -- ѡȡ������ż��������Ԫ�أ�������0��ʼ�������ڱ��Ľ�����ɫ
*     :odd -- ѡȡ����������������Ԫ�أ�������0��ʼ�������ڱ��Ľ�����ɫ
*     :eq(index) -- ѡȡѡȡ������index�ĵ���Ԫ�أ�������0��ʼ������ֻƥ��һ��Ԫ��
*     :gt(index) -- ѡȡ��������index������Ԫ��(���� index ��������index)
*     :lt(index) -- ѡȡ����С��index������Ԫ��(С�� index ��������index)
*     :header -- ѡȡ���еı���Ԫ��(h1~h6)
*     :animated -- ѡȡ��ǰ����ִ�ж���������Ԫ��
*   ���ݹ���ѡ���� -- ���ݰ�������Ԫ�ػ��ı����ݽ���ѡ��
*     :contains(text) -- ѡȡ�������Ԫ���к����ı�����Ϊ"text"��Ԫ�أ� �� $("div:contains('��')") ѡȡ�����ı�"��"��<div>Ԫ��
*     :empty -- ѡȡ��������Ԫ�ػ��ı��Ŀ�Ԫ��, �� :parent �෴��
*     :has(selector) -- ѡȡ����ѡ������ƥ���Ԫ�ص�Ԫ�أ��� $("div:has(p)") ѡȡ���� <p>Ԫ�ص�<div>Ԫ�أ�ע��ѡ����� <div>
*     :parent -- ѡȡ������Ԫ�ػ����ı���Ԫ��(�������Ǹ��ؼ�)���� $("div:parent") ѡȡӵ����Ԫ��(�����ı�Ԫ��)��<div>Ԫ��
*   �ɼ��Թ���ѡ����
*     :hidden -- ѡȡ���в��ɼ���Ԫ�أ����� <input type="hidden"/>, style="display:none", style="visibility:hidden;" �ȵ�Ԫ��
*     :visible -- ѡȡ���пɼ���Ԫ��
*   ���Թ���ѡ����
*     [attribute] -- ѡȡӵ�д����Ե�Ԫ�أ� �� $("div[id]") ѡȡӵ��id���Ե�<div>Ԫ��
*     [attribute=value] -- ѡȡ����ֵΪ value ��Ԫ�أ��� $("div[title=test]") ѡȡ����titleΪ"test"��<div>Ԫ��
*        ��Ӧ���� !=value(������), ^=value(����ֵ��value��ʼ), $=value(����ֵ��value����), *=value(����ֵ����value)
*     [selector1][selector2] -- �����԰����롱�ķ�ʽ�ϲ���ͬʱ�������������� $("div[id][title*='test']") ѡȡ������id����������title�к���"test"ֵ��<div>Ԫ��
*   ��Ԫ�ع���ѡ����(ע��͹���ѡ����������1.��Ԫ�ع���ѡ������������1��ʼ; 2.��Ԫ�ع���ѡ�������ÿ����Ԫ�ض�����)
*     :nth-child(index/even/odd/equation) -- ѡȡÿ����Ԫ���µĵ�index����Ԫ��/��/ż/?? Ԫ��(index��1��ʼ��)���� $("div.one :nth-child(3n)")ѡȡclassΪone��div��Ԫ��������ֵ��3�ı�����Ԫ��
*        ע�⣺Ϊÿһ����Ԫ��ƥ����Ԫ�أ�����index�Ǵ�1��ʼ��
*     :first-child -- ѡȡÿ����Ԫ�صĵ�һ����Ԫ�أ��� $("ul li:first-child") ѡȡÿ��<ul>�е�һ��<li>Ԫ��
*     :last-child -- ѡȡÿ����Ԫ�ص����һ����Ԫ��
*     :only-child -- ���ĳ��Ԫ��������Ԫ����Ψһ����Ԫ�أ�����ƥ��(����Ԫ����ֻ��һ����Ԫ��)���� $("ul li:only-child") ��<ul>��ѡȡ��Ψһ��Ԫ�ص�<li>Ԫ��
*   ���������Թ����� -- ����ѡ��ı�Ԫ�ؽ��й���
*     :enabled/:disabled -- ѡȡ���� ����/������ Ԫ��
*     :checked -- ѡȡ���б�ѡ�е�Ԫ��(��ѡ��/��ѡ��)
*     :selected -- ѡȡ���б�ѡ�е�ѡ��Ԫ��(�����б�), �� $('#mySelect option:selected')
*   ��ѡ���� -- ѡȡ��(form)���ƶ����͵�Ԫ��,ʹ����ǰͨ��ָ��form��������������ѡ��Χ, �� $("form1 [name=items]:checkbox") ѡ��form1��name����Ϊ"items"������checkbox
*     :button -- ѡȡ���еİ�ť
*     :checkbox -- ��ѡ��ť
*     :file -- 
*     :hidden -- ���еĲ��ɼ�Ԫ�أ�<input type="hidden" />��<div style="display:none">test</div>������ƥ��.
*     :input -- ѡȡform�����е�������Ԫ��(input/textarea/select/button��)��ע��� $("input")������--":input"��"input"Ҫ��
*     :image -- 
*     :password 
*     :radio -- ��ѡ��ť
*     :reset -- ���ð�ť
*     :submit -- �ύ��ť
*     :text -- ѡȡ���еĵ����ı���
*     textarea -- ǰ��û�� ð��(:),���õ��� tagName ��ʽ
*  ѡ�������ӣ�
*    1.��϶��ѡ������ѡ��: <input type="checkbox" name="items" xxx> ��ѡ���� -- $('[name=items]:checkbox:checked')
******************************************************************************************************************************************/

/******************************************************************************************************************************************
*
* ���(ģ�黯��ʹ�÷�ʽ)��
* DOM������װ -- ��װ�˴������õ�DOM����
* �ɿ����¼�������� -- 
* ���Ƶ�Ajax -- �����е�Ajax������װ��һ������ $.ajax() ������ߴ���Ajaxʱ��ר�Ĵ���ҵ���߼�
* ����Ⱦ�������� -- ֻ����һ����ΪjQuery�Ķ��������еĺ������������������֮�£������ $ Ҳ������ʱ��������Ȩ( jQuery.noConflict();Ȼ����� jQuery(xxx) �ķ�ʽʹ�� )��
*   ��������Ⱦ�����Ķ���(�ɺ������⹲����޳�ͻ)
* ��ɫ����������� -- 
* ��ʽ������ʽ -- �Է�����ͬһ��jQuery�����ϵ�һ�鶯��������ֱ����д�������ظ���ȡ����(��������)
* ��ʽ���� -- ����������Ƴ��Զ��������󼯺ϣ������ǵ����Ķ��󣬲���Ҫ��д������ѭ���ṹ����
* ��Ϊ����ṹ��ķ��� -- ����ʹ��jQueryѡ����ѡ��Ԫ�أ�Ȼ��ֱ�Ӹ�Ԫ������¼������ַ�������������jQuery������Ա��ҳ�濪����Ա��˾��ְ
******************************************************************************************************************************************/

/******************************************************************************************************************************************
* $(html) -- ��������, 
*    ������������е�dom��������ת����jQuery����[ �� $(document) ]; 
*    �������html����ַ���(ע����Ҫ�Ǳպϵ�XHTML��ʽ)����DOM���󣬲���װ��jQuery���󷵻�[ �� $parent.append($("<li>���б���</li>")) ]; 
*    ����� ������ֱ��д "xxx" �ȼ��� $("xxxx")?
*   
* ����(TODO:�������д) -- ע�⣺jQuery�кܶ๦��ʹ����ͬ������ʵ�� getter/setter
*   ����Ԫ�� -- ע�⣺���������Ԫ�������еģ���ͨ�����ƶ��������� $parent.append("$obj") ʱ$obj�����е�Ԫ�أ�����ԭ����λ���ƶ��� $parent��
*     .after($siblingObj)/.before($siblingObj) -- ��ÿ��ƥ���Ԫ��֮ ����/ǰ��  �������ݣ������ֵ�Ԫ��
*     .append($obj) -- ׷����Ԫ�ص���Ԫ�ص������?(�� innerHTML ����������?)��������Ԫ��
*     .appendTo($parent) -- ��ƥ��Ԫ��׷�ӵ�ָ����Ԫ�ص������
*     .clone([bool]) -- ����ƥ��ڵ㲢���أ��������Ϊtrue�����丱������ͬ���İ��¼�
*     .empty() -- ��մ�Ԫ���������(�������ݺ����к���ڵ�)����Ԫ�ػ����ڣ��� $("ul li:first").empty() ��������ı������ڵ�(<li>)����
*     .insertAfter($siblingObj)/.insertBefore($siblingObj)  -- ���뵽ָ�����ֵܽڵ� ����/ǰ�� ��
*     .prepend($obj) -- ���뵽��Ԫ�ص���ǰ��
*     .prependTo($parent) -- ��ƥ��Ԫ�ز��뵽ָ����Ԫ�ص���ǰ��
*     .remove() -- ��DOM��ɾ������ƥ��Ԫ�ؼ�����ڵ㣬���ر�ɾ��Ԫ�ص�����
*     .remove(Selector) -- �ӱ��������Ƴ�ָ���������Ӷ���
*     .replaceWith($obj) -- ʹ��ָ��Ԫ���滻ƥ��Ԫ��
*     .replaceAll(Selector) --ʹ��ƥ��Ԫ���滻����ָ��Ԫ��?
*   �����ڵ� (ע�⣺��������������ʹ��jQuery���ʽ��Ϊ������ɸѡԪ��)
*     .closest() -- ȡ�������ƥ��Ԫ�أ����ȼ�鵱ǰԪ���Ƿ�ƥ�䣬��ƥ��ֱ�ӷ���Ԫ�ر����粻ƥ���������ϲ��Ҹ�Ԫ�أ�ֱ���ҵ�ƥ��Ļ��jQuery����
*       �� �������Ŀ������� li Ԫ�������ɫ: $(document).bind("click", function(e){ $(e.target).closest("li").css("color", "red"); })
*     .children() -- ȡ��ƥ��Ԫ�ص���Ԫ�ؼ���(�����Ǻ��Ԫ��)
*     .filter() -- ����ָ���������ˣ��� input.keyup �и����û���������й�����ʾ .filter(":contains('"+( $(this).val() )+"')").show();
*     .find() -- ��ƥ��Ԫ���в���ָ��(�ӣ�)Ԫ��,�� .find("tr")
*     .next()/.prev() -- ȡ��ƥ��Ԫ�� ����/ǰ�� ���ڵ�ͬ��Ԫ��
*     .parent() -- ? �õ�ֱ�Ӹ�Ԫ��
*     .parents() -- ? �ݹ��ڸ�����Ԫ���в���ָ��Ԫ��
*     .siblings() -- ȡ��ƥ��Ԫ��ǰ����������������ͬ��Ԫ��(����������)
*     ������: .nextAll, .prevAll,
*   ������ʽ����
*     .attr("����" [,"��ֵ"]) -- ���ػ�����ָ��������ֵ, �� class/style/title/checked ��, ��Ҫһ�������ö�����ԣ�����ʹ�����¸�ʽ��
*        .attr({ "title" : "your title", "name" : "your name" });
*     .addClass("className") -- ����ָ����class����(ͨ�����ڹ���CSS)��ע�⣺����׷�ӷ�ʽ���ж��ʱ���Ϊ "class1 class2" �ȶ��ֵ�ĺϲ�(������ͬ���ֵ����ԣ���ӵĸ����ȼӵ�)
*     .css("����" [,"�µ�ֵ"]) -- ��д��Ӧ��css����ֵ, �� .css("font-color", "red") 
*        ע�⣺���۶�Ӧ���������ⲿCSS���롢������������̬���õģ����ܻ�ȡ�����յĽ��
*        ��������(���ֺͱ�׼CSS��һ��): backgroundColor, color, fontSize/font-size(���ִ�С), opacity(͸����0~1)
*     .hasClass("className") -- �ж��Ƿ���ָ����class����,��ǿ����ɶ��Զ�������,�ȼ��� .is(".className")
*     .height([xx])/width([x]) -- ��д��pxΪ��λ��ʵ�ʸ߶ȺͿ��(����� .css("height") �õ��Ŀ�����"auto"��)
*     .offset() -- ��д�ڵ�ǰ�Ӵ������ƫ�ƣ����صĶ������ top �� left ��������
*     .position() -- ��ȡԪ������������һ��position��ʽ��������Ϊ relative ���� absolute ���游�ڵ�����ƫ�ƣ����صĶ������ top �� left ��������
*     .removeAttr(["����"]) -- �Ƴ��ض������ԣ��� removeAttr("style")��ʾ��������е�CSS����, �� "����"Ϊ�գ���ȥ�����е�����?
*     .removeClass(["className1 className2"]) -- ȥ��ָ����class���ԣ���� className Ϊ�գ���ȥ�����е�class����
*     .scrollTop([value])/.scrollLeft([value]) -- ��дԪ�صĹ������ඥ�˺���˵ľ��롣��ͬ������?( $comment.animate({ scrollTop:"-=50" } , 400);)
*     .toggleClass("className") -- �ظ��л�ָ������(������ɾ��,�����������)
*     .wrap("<a href='http://www.baidu.com' target='_blank'></a>") -- ʹ��ָ��Ԫ�ذ�ƥ��Ԫ�ذ�������
*     .wrapAll("<xxx></xxx>") -- ��ƥ�������Ԫ�ط���һ�� <xxx> ��, �� .wrap ���ÿ��ƥ���Ԫ�ض�����һ�� <xxx>
*     .wrapInner("<xxx></xxx>") -- ��ÿһ��ƥ���Ԫ�ص�������(�����ı��ڵ�)�������ṹ���ı�ǰ����������� �������ڲ�
*   �ı�����
*     .html(["�µ�ֵ"]) -- ��ȡ�������� HTML ֵ���ȼ���domObj.innerHTML, ע����������XML�ĵ�
*     .text(["�µ�ֵ"]) -- ��дԪ���е��ı�����,�ȼ��� domObj.innerText, �� <p>�е��ı���select��ÿһ��option���ı�
*     .val(["�µ�ֵ"]) -- ��дԪ�ص�ֵ,����domObj.value(���� button,select,text�ȵ�������ʾ)
*        �����Ҫ���ö�ѡֵ(�� multiple �� select)ʱ����Ҫ�������������Ҷ��ŷָ�����(��: $("#multiple").val(["check2","check3"]); )
*        ��Ӧ��ֵ�� <option>�е�text����, checkbox,radio �� value��text����
*   �������� -- Ϊ�˱��⶯���ѻ��������Ӷ�������ǰ��Ҫ���ж��Ƿ��ڶ����� if(!$Obj.is(":animated")){ �����ڶ������������� }
*     .animate( {������1:"����ֵ1"[, ������2:"����ֵ2"] } [, ʱ��] [, ����ʱ�Ļص�����] ) -- �Զ���Ķ�����������ָ��ʱ����ڸ�������ֵ�ﵽ����Ч����
*        �� $(this).animate( {left:"500px", height:"+=200px" }, 3000);
*        ������ǿ��Ĺ��ܣ�����ģ��������������� .show(600) == .animate( {height:"show", width:"show", opacity:"show" } , 600 );
*     .fadeIn/.fadeOut -- ͨ������ "͸����" �� "����/����" Ԫ��
*     .fadeTo(ʱ��, ͸����, �ص�) -- ����͸����
*     .show([ʱ��])/.hide([ʱ��]) -- ͬʱ���� "��/��/͸����" �� "��ʾ/����"ƥ��Ԫ��, hideǰ���ȼ�סԭ�ȵ� display ����ֵ������showʱ�ָ�
*     .slideDown/.slideUp -- ͨ������ "�߶�" �� "��ʾ/����" Ԫ��
*     .slideToggle( ʱ��, �ص�) -- ͨ�����ĸ߶��������л��ɼ�״̬
*     .stop([clearQueue=false] [,gotoEnd=false]) -- ֹͣƥ��Ԫ�صĶ�����clearQueueΪtrue��ʾ���δִ����Ķ������У� gotoEndΪtrue��ʾֱ�ӽ�����ִ�еĶ�����ת��ĩ״̬��
*     ע�⣺
*       1.ʱ������� "slow"(600ms)/"normal"(400ms)/"fast"(200ms) �� ������ ��ָ��
*       2.�����������������ʽ�����γɶ������У�����ִ��
*       3.stopʱֻ����������ִ�еĶ�������״̬����û���ṩֱ�ӵ���δִ�ж�����������״̬�ķ���
*       4.TODO:ʹ�ö��������������������ܲ��� -- HTML5��ʹ��Canvas���Ƹ���?
*   ��������
*     ?.children(selector) -- ͨ��ָ����ѡ����ѡ������������������Ԫ��?
*     .each(function (){ xxx }) -- ��ѡ�������ÿһ��Ԫ��ִ��ָ���¼�
*     .end() -- ���¶�λ���ϴβ�����Ԫ��?
*     .expr() -- ?
*     ? $.extend( {��ֵ��}, ���� ) -- ��չ������ֵ�������ڱ�д���ʱ����ȱʡֵ��
*     .filter(Selector) -- ����ָ���������˳������������Ԫ�ؼ���, �� $obj.filter(":contains('����'),:contains('�῵')")
*     .get(index) �� [index] -- ѡȡjQuery����������ָ����Ԫ�أ�ע�⣺����ֵ��DOM����
*     .index(domObj) -- ��ȡָ�� dom ������ƥ��Ԫ���е�����ֵ�������� var index= $objs.index(this) -- ��ȡ��ǰ�����¼���Ԫ����ȫ��ƥ��Ԫ���е�����
*     .is(xxx) -- �ж�???
*       .is(".className") -- �ж��Ƿ���ĳ��ʽ����
*       .is(":����") -- �� :checked(�Ƿ�ѡ��), :visible(�Ƿ�ɼ�), :animated(�Ƿ��ڶ���)
*       .is("#id") -- �ж��Ƿ���ָ��Ԫ��
*     .next("xx)
*     .nextAll("xxx") -- ѡȡƥ��Ԫ�� ����� ͬ��"xxx"Ԫ��
*     .siblings("xxx") -- ѡȡƥ��Ԫ��ͬ����"xxx"Ԫ��
*     .serialize() -- ���������л���ͨ�������� ��form �� inputԪ�� �ϣ����Խ��û��������л��� GET �Ĳ����ַ���(���Զ����� encodeURIComponent()���� )
*        ��: $.get("get1.asp", $("#form1").serialize(), function ... );
*     .serializeArray() -- ���������л��ɶ������鲢���أ�Ȼ�����ͨ�� $.each ���б�������
*     .toggle([ʱ��] [,�ص�]) -- �л�Ԫ�صĿɼ�״̬��ע��� toggle�ϳ��¼� ������
*     .trigger("eventName", [ �������� ] ) -- ģ�⼤��ָ���¼����� $("#isreset").trigger('click')���� onclick�¼�, Ҳ���� .click()���棬������ִ���������ȱʡ����
*        ע�⣺���Լ����Զ�����¼�(�Զ����¼�Ҳֻ������������)
*     .triggerHandler("eventName") -- ģ�⼤��ָ���¼�(��������ִ���������ȱʡ�������缤�� $inputObj.triggerHandler("focus") �ἤ���¼���
*        �����Ὣ�������õ� $inputObj ��)������ �� .blur ������֤����ʱ��������.focus��.keyup ��triggerHandler("blur")������ʵʱ��֤
*
*   Ajax��װ����(�� $. ��ʼ����ȫ�ֺ����� �� $obj. ��ʼ��ָ�������ϵĺ��� )
*     $.ajax( { ���������ļ�ֵ�� } ) -- �ײ�AJAXʵ�ֵ�ȫ�ֺ���������  $.ajax({ type:"GET", url:"test.js", dataType:"script" }); �ȼ��� .getScript("test.js")
*        cache -- �Ƿ񻺴�(true/false)����POSTʱĬ�ϲ�����
*        data -- ���ݵĲ������磿 $("#form1").serialize() �� a=1&b=2&c=3
*        dataType -- json/jsonp( ����ʲô? 6\asp\6\demo6-ajax\demo3.html ��ʹ�� )/script/xml ��
*        error -- ����ʱ�Ļص�����, function(value){xxx}��
*        success -- ��ȷִ�к�Ļص�����, function(value){xxx}��ע�⣺�ص������Ĳ������ܸ��� dataType ��ͬ����ͬ(�� .getJSON �� .getScript �Ļص�����)
*        timeout -- ��ʱ���� 1000 ��ʾ1��
*        type -- GET/POST ��
*        url  -- Ŀ��URL,�� "test.asp" ��,
*     $.each(����, function(index, ������) { �����߼� }) -- ����������д���
*     $.get("URL", { ������ֵ�� }, �ص����� ,�������� ) -- ȫ�ֵĻ�ȡ����
*        ������ֵ�� -- ��ֵ����Ҫʹ�����ţ��� username : $("#username").val()������� ?name1=value1&name2=value2 �ķ�ʽ�ύ��������
*        �ص�������ʽ -- function (data, statusText) { xxx }, 
*        �������� -- ��ѡ���� "xml","json" �ȣ��粻д�����ַ������ص��з��ص�data����ݸ����ͷֱ��� XML�����ַ�����JSON���������ַ�����
*     $.getJSON(".json��ַ" [, function(data){ ������ϵĻص����� }]) -- data�����Ѿ���������JSON����
*     $.getScript(".js��ַ" [, function(){ ������ϵĻص����� }]) -- ȫ�ֺ�������̬���ز�ִ�� JavsScript �ļ�?
*     $.param("��ֵ�Բ���") -- ����ֵ�Բ���ת�����ַ�����ʽ�� �� $.param({a:1,b:2,c:3}) ���� "a=1&b=2&c=3"
*     $.post("URL",{ ������ֵ�� }, �ص�����, �������� ) -- ȫ�ֵ��ύ����
*        �ص�������ʽ -- function (data, statusText) { xxx } 
*     $obj.load("URL ����" [,{������ֵ��}] [,�ص�����] ) -- ��ƥ��Ԫ���ϼ���ָ��URL��Ӧ�����ݣ�
*        GET��ʽ -- ֱ��ʹ�� xxx.asp?username=xxx&content=yyy �ȵ�URL��ʽ
*        POST��ʽ -- ͨ�� ������ֵ���ṩ
*        URL��������ô���£����� $("#resText").load("test.html .para"); �˴���ʾ����� para ����? 
*        �ص������ĸ�ʽ -- function (responseText, textStatus, XMLHttpRequest){ xxx }
*  
* �¼�( .xxx(function([event]) { xxx });
*     .ajaxStart()/.ajaxStop() -- AJAX��ʼ�ͽ����¼���TODO: ��Ȼ��ȫ�ֹ��õģ�������д�ڶ������ϣ�������дΪ $.ajaxStart ��������д������ $("#loading").ajaxStart
*     .bind("�¼�1 [�¼�2]" [,data] , fun) -- ͨ�õذ�ָ���¼�, data�ǿ�ѡ��������Ϊ event.data ����ֵ���ݸ��¼�����Ķ������ݶ���
*     .blur -- ʧȥ����ʱ
*     .change() -- ����change�¼����� $("select").change(function(){ xxx; })
*     .click -- ����click�¼�
*     .error
*     .focus -- ��ý���ʱ
*     .keyXxx -- �����¼�: .keydown/.keypress/.keyup
*     .load -- �ȼ��� onload
*     .mouseXxxx -- ����¼�: .mousedown/.mouseup/.mousemove(�ڲ��ƶ�ʱ)
*                             .mouseover/.mouseout -- �������ָ�봩����ѡԪ�ػ�����Ԫ�أ����ᴥ�� mouseover �¼�
*                             .mouseenter/.mouseleave -- ֻ�������ָ�봩����ѡԪ��ʱ���Żᴥ�� mouseenter �¼�(��Ԫ�ز�����)
*     .one("�¼�" [,data] , fun) -- ͨ�õذ�һ���¼�(ֻ����һ��)
*     .ready() -- �� $(document).ready(function(){ xxx }); 
*        ��ҳ������DOM��ȫ������ִ�У�����ʱDOMԪ�ع����Ķ�������û�м�����(����ͼƬ)����� window.onload �ܼ��������Ӧ�ٶ�(onload������Ԫ�ؼ������ִ��),
*          ����ͬʱ��д���(��window.onloadֻ����һ��)
*        ȱ�㣺ready�п����޷�׼ȷ��ȡͼƬ����Ϣ(��߿��)����Ӧ���� load �¼���
*      .resize --
*      .scroll, .unload, .dblclick, .select, .submit
*      .unbind(["�¼�" [,fun] ] ) -- ȡ���¼��İ󶨣�����ָ�� ȡ���ĸ��¼����¼��ϵľ��庯��
*   �Զ����¼� -- $obj.bind("myEvent", function(event, param1, ...paramN){ xxxx }
*     ����(������ trigger����ͨ���������ʽ���ݲ���)��$obj.trigger("myEvent",["����1","����2"]);
*   �¼������ƿռ� --  ���¼�����ָ�����ƿռ�(�� click.fishjam ), Ȼ����Ը������ƿռ�������(�� unbind(".fishjam") ��ȡ�� ".fishjam" ���ƿռ��µ������¼�,
*     ����ʱ���� trigger ���¼����ֺ�Ӹ�̾��("!") ��ƥ�����в������������ռ��еķ������� "click" �ἤ�����пռ��µ�, "click!"ֻ����Ĭ�Ͽռ��µ�
*   �ϳ��¼�(jQuery���Զ���ķ���)
*     .hover( enter, leave ) -- ģ������ͣ�¼����ֱ��Ӧ mouseenter �� mouseleave �¼�
*     .toggle( fun1,fun2,...funN ) -- ģ��������������¼���������ÿ�ε���ʱ�л���������Ĵ����߼�
******************************************************************************************************************************************/

/******************************************************************************************************************************************
* �ḻ�Ĳ��֧��(http://plugins.jquery.com/) -- 
*   Basic XPath -- �������û�ʹ�û�����XPath
*   Color -- jquery.color.js
*   Cookie -- jquery.cookie.js������cookie������ $.cookie("����" ,��ֵ ,{ �������Եļ�ֵ�ԣ��� path: '/', expires: 10 });
*     ɾ�� -- $.cookie("����", null, { path: '/' });
*     expires -- 10�� -- 10; 
*                ����� -- var date = new Date(); date.getTime() + (3 * 24 * 60 * 60 * 1000); { xxxx, expires: date }
*   Form -- jquery.form.js�� ����ʹ�� .ajaxForm/.ajaxSubmit/.formSerialize �ȷ���
*   jQueryUI -- ui.core.js/ui.sortable.js �ȣ�UI��չ�⣬���Ժ����׵�֧�� ��ק(.sortable({delay:1}))��
*   LiveQuery -- jquery.livequery.js������ʹ��ͳһ���﷨�� ǰ�ھ������ɵ�Ԫ�� �� ����ͨ���ű����ɵ�Ԫ�أ� ��  $('a').livequery('click', function(){ xxx });
*   MoreSelectors for jQuery -- ���Ӹ����ѡ�������� .color ƥ����ɫ, :colIndex ƥ�����е���, :focus ƥ���ȡ�����Ԫ�ص�
*   Validate -- jquery.validate.js������֤���(����֤ ��������ʼ���URL�����ȵ�)�������Զ�����ʾ��Ϣ��֧�ֶ����Եȡ�
*     ����.js�ļ��󣬿��� .ready ��ͨ�� $("#myForm").validate( { JSON �﷨�Ĺ�����ʾ��Ϣ���ص����� �� } ) ��ʼ����Ҫ��֤����Ϣ��
*     ��ͨ�� $.validator.addMethod( "��֤������", function(value, element, param) { ��֤���� }, "����ʱ��ʾ��Ϣ");
* 
* ��д��� -- (function($){ XXX })(jQuery);
*   $.fn.extend( { "������" : function(value){ʵ����; return xxx; } } );  -- ����ĺ�����ͨ�� $obj.������ ����
*     -- if (value==undefined){ ��ȡ���� }else{ ���õ��� }, ע��ͨ����Ҫ���ض���(�� this) ��֧����ʽ����
*     -- value = $.extend({ ��ʼֵ�ļ�ֵ�� }, value);
*   $.extend( { "������1" : function1{}, "������2" : function2{} } ); -- ȫ�ֺ�����ͨ�� $.������ ����
******************************************************************************************************************************************/

/******************************************************************************************************************************************
* ����XML -- $(xmlString) ����XML���������� ajax ����XML����ʱ $(responseText), ��������ѡ��������?
*   .find("selector") -- ����ָ���Ľڵ㣬ʹ�õ��﷨���� ѡ������ �� find("comment content").text() �ɲ��� <comment><content>someValue</content></comment>
*
* ����JSON -- ajax �� .get()��JSON����ʱ���� JSON �������� ֱ��ʹ�� data.varName ���ɷ��ʣ�
*   
* 
******************************************************************************************************************************************/

$(document).ready(function() {
    $("button").click(function() {
        $("p").hide();
    });
});

module("JQueryTester", {
    setup: function() {
        //dump("in JQueryTester::setup\n");
    },
    teardown: function() {
        //dump("in JQueryTester::teardown\n");
    }
});

test("JQuery", function() {
    var comments =
    [
        { "username": "����", "content": "ɳ��.", "score": 80 },
        { "username": "����", "content": "���.", "score": 70 },
        { "username": "����", "content": "�ذ�.", "score": 90 }
    ];
    var totalScore = 0;
    $.each(comments, function(index, comment) {
        totalScore += comment.score;
    });
    equal( totalScore, 240, ".each ����");
});

