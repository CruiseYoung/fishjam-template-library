#ifndef COM_OFFICE_H
#define COM_OFFICE_H

/***************************************************************************************************
* ���⣺
*   1.ע���û���װ����汾��Officeʱ -- �����汾�ĺ����������������Ͳ�ͬ
*     �� Office 2000 ʱ�� Office\MSO9.dll
*        Office XP   ʱ�� Office11\MSO.DLL
*        Office 2007 ʱ�� Office12\MSO.DLL
*
* Microsoft �� Office ��Ʒ�У����ṩ��OLE Automation �Զ�������Ľӿڡ�
* ���� -- ���ں����Ĳ���ͨ������VARIANT���͵�ָ�룬����ͨ�� Office ���Դ��ġ��ꡱ����ȷ������Ĳ�������,
*   ��ʹ�� CComVariant �����Ӧ�ı�����
*   ��¼�Ƶĺ꣺
*     ActiveDocument.SaveAs FileName:="Hello.doc", FileFormat:=wdFormatDocument, _
*       LockComments:=False, Password:="", AddToRecentFiles:=True, _
*       WritePassword:="", ReadOnlyRecommended:=False, EmbedTrueTypeFonts:=False, _
*       SaveNativePictureFormat:=False, SaveFormsData:=False, SaveAsAOCELetter:= False
*   
* 
* Microsoft ��Ӧ��(Application)���߼����ܻ���Ϊ���µ����νṹ
*   Application(WORD Ϊ����ֻ�г�һ���֣�
*   +- Documents�����е��ĵ���
*      +- Document��һ���ĵ���
*   +- Templates������ģ�壩
*      +- Template��һ��ģ�壩
*   +- Windows�����д��ڣ�
*      +- Window
*      +- Selection
*      +- View
*   +- Selection���༭����
*      +- Font
*      +- Style
*      +- Range
*
* ʹ��WORD
*   1.#import MSOX.dll �ӿڿ�(ע�������Ҫ rename, raw_interfaces_only ��)
*   2.CComPtr<Word::_Application> m_pApp; 
*     COM_VERIFY(m_pApp.CoCreateInstance(__uuidof(Word::Application), NULL, CLSCTX_SERVER));
***************************************************************************************************/

#endif //COM_OFFICE_H