'Dim obj as Object;
Set obj = CreateObject("ATLCom.MyMath")	'��ProgID�������
MsgBox "hello's Upper is " + vbCrLf + obj.Upper("hello") 'ֻ�ܵ����Զ����ӿ�(IDispatch)�����ݣ�HELLO
Set obj = Nothing			            '�ͷ�