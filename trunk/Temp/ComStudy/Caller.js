main();

function main()
{
  //var obj = new ActiveXObject("ATLCom.MyMath"); //��ProgID�������
  var obj = WScript.CreateObject("ATLCom.MyMath");
  WScript.Echo(obj.Upper("hello"));
}