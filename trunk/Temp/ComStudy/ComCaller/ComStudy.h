#ifndef COM_STUDY_H
#define COM_STUDY_H
#pragma once

/******************************************************************************************************
* ���������(��CMyMath)�� Class View ��ͼ�У�ͨ���Ҽ��˵����Զ����� ��Ҫʵ�ֵĽӿں����ӵ�
* 
* COM ��������
*   1.error LNK2001: unresolved external symbol _CLSID_DirectDraw
*     ��������ͬ�汾�� DEFINE_GUID �꣬һ��ֻ�������ⲿ���õķ������ƣ���һ������ʵ�ʵķ��Ŷ���
*     ���ʹ��Ԥ����ͷ�Ļ���#include <InitGuid.h> ������ DEFINE_GUID ��
*     �����ʹ��Ԥ����ͷ�Ļ����ڰ��� <objbase.h> ǰ���� DEFINE_GUID ��
*     ����Ҫȷ��ֻInitguid.hֻ����һ�Σ�������ܳ���"redefinition; multiple initialization"�Ĵ���
*   2.ATL����Զ��IDispatchʵ��֧�ֲ�̫�ã��޷������׵Ľ����IDispatchʵ�ֺϲ���һ����
*     ��̳��Զ��IDispatchImplʱ����ʹ�� COM_INTERFACE_ENTRY2(IDispatch, XXXX), 
*     �Ƿ��������������Ҫע��ĵط�?
*
* ATL�еĺ�(TODO)
*   DECLSPEC_UUID -- class DECLSPEC_UUID("74740E11-DDD5-4B62-A739-32A56DD60933") FtlLogAddinLib;
*   DECLARE_LIBID(LIBID_XXXX) -- CXXXModule �ж�������COM����� ATL::CAtlModule::m_libid ����
*   DECLARE_REGISTRY_APPID_RESOURCEID -- CXXXModule ��
*   DECLARE_PROTECT_FINAL_CONSTRUCT -- ��ʲô��? TODO:������ FinalConstruct �оۺ϶�������ü���? һ���Ǳؼӵ�?
*   DECLARE_CLASSFACTORY_SINGLETON(COM��) -- ����COM����Ϊ��ʵ�������еĿͻ��˻����ӵ�ͬһ��ʵ��
*   OBJECT_ENTRY_AUTO(CLSID_XXX, ʵ����) -- ���ඨ�����ⲿ���������Զ�ʵ����ص��๤����
*     �ɵ�ӳ�䷽ʽΪ BEGIN_OBJECT_MAP =>OBJECT_ENTRY=> END_OBJECT_MAP
*
* ATL�Ļ���(û�п�����׼�ĵ����Լ����������ģ��Ƿ���ȷ?)
*   _ATL_MODULE
*     CAtlModule
*       CAtlModuleT
*         CAtlDllModuleT -- DLL ���͵�COM������࣬�ṩ��DllMain��ʵ���� DllCanUnloadNow��DllRegisterServer �Ⱥ���
*         CAtlExeModuleT -- EXE ���͵�COM������࣬�ṩ��WinMain��
******************************************************************************************************/


/******************************************************************************************************
* COM(Component Object Model) -- �������ģ��
*   COM������(TypeLib=DLL/Exe) 1 > n COM���/����(coclass,CLSID_XXX) 1 > n �ӿ�(IID_YYY)
*   COM�ӿ�ʵ���Ͼ���һ��ָ�� VTable �ṹ��ָ��
* COM++ -- �����״����ֲ�ʽ������
* DCOM -- �ֲ�ʽCOM����ҪDCOM��������
* ProgID -- Programmatic identifier���Ͱ汾�޹صĳ����ʶ�����汾��ص�ProgID�ɰ汾�޹ص�ProgID�Ӻ�׺(�� ".1" ���)
*
* COM �����ע�᣺
*   Exe -- RegServer��UnregServer��Automation
*   DLL -- Regsvr32.exe [/u] XXX.dll
* �ļ�ʵ���а���<initguid.h>ͷ�ļ�����ʹ�� DEFINE_GUID ��Ϊ����ͳ�ʼ��GUID����
* 
* �๤��(IClassFacory)--�������޹صķ�ʽ��ʵ����һ���������Ψһ���ܾ��Ǽ�����COM����Ĵ���������
*   CoGetClassObject->IClassFactory::CreateInstance
* 
* COM�Ĺ���
*   1.����ʵ��Iunknown -- �ӿڱ���ֱ�ӻ��ӵش�IUnknown�̳�
*   2.COM����ͬһ�� -- �Ƚ�����COMָ���Ƿ�Ϊͬһ�����󣬱��� QI �� IUnknown ��ָ����бȽϡ�
*     ʹ�� AreComObjectsEqual ���ߺ����Ƚ�
*   3.���������� -- ���ڿɴ���Ľӿ����ã������з�����(QI(A)->A)���Գ���(QI(QI(A)->B)->A)�͹����ԡ�
*   4.�ڸı�һ�����нӿ�ʱ����ʵ��һ���µĽӿ�(�ͻ��ڱ���ʱ��ͬԭ�нӿڵ�VTable����һ����)
* 
* �ӿڶ���Ŀ���
*   1.�ӿں����Ĺ��ܱ���򵥡���ȷ
*   2.Ҫ��ֿ���Marshalling�����ܵ�Ӱ�죬�ڽ������������ݴ���������������
*   3.Ҫ��ֿ��ǵ��ڴ�Ĺ�������
*   4.Ҫ��ֿ���COM��Ӧ�û�������IUnknown��IDispatch֮������ѡ��
*
* ���ü��������ԭ��
*   ����AddRef����
*     1.����һ���ǿ�ָ��д���ֲ�������ʱ��
*     2.�������÷���һ���ǿսӿ�ָ��Щ������������[out]��[in,out]������ʱ��
*     3.�������÷�����һ���ǿսӿ�ָ����Ϊ������ʵ�ʽ��ʱ��
*     4.����һ���ǿսӿ�ָ����Ϊ�����һ�����ݳ�Աʱ��
*   ����Release�Ĺ���
*     1.�ڸ�дһ���ǿվֲ������������ݳ�Աǰ��
*     2.���뿪�ǿվֲ�������������֮ǰ��
*     3.�ڱ����÷�Ҫ��д����������[in,out]����ʱ�����Ҳ����ĳ�ʼֵ�ǿա�
*     4.�ڸ�дһ������ķǿ����ݳ�Աǰ��
*     5.���뿪һ���������������ǰ�����Ҵ�ʱ����һ���ǿյĽӿ�ָ����Ϊ���ݳ�Ա��
*   ������AddRef��Release�Ĺ���
*     1.�����÷���һ���ǿսӿ�ָ��ͨ��[in]��������һ�������򷽷�ʱ��
*     2.ͬһ������������ӿڣ��ӿ�һ������������ӿڶ���������ʱ�����ڵڶ����ӿڲ�����á�
*
* COM ����������ڷֲ�ʽ�����е� -- INPROC_SERVER��LOCAL_SERVER��REMOTE_SERVER
* 
* ����ǣ�����ڴ������ͷŵ����⣬COM ��Ҫ��ȷ��ע���������� [in,out]�ȣ� -- ��Ҫʹ��COM�ķ�ʽ(CoTaskMemAlloc/CoTaskMemFree)
*   [in] -- ��ʾ�ͻ�������(������)���������ͷ���Щ��������Ҫ���ڴ棻
*   [out] -- ��ʾ������(��������)���������Щ����������ڴ棬�ͻ�������(������)�����ͷ�
*   [in,out] -- ��ʾ�ͻ�������(������)���䲢�ͷ��ڴ棬�������Ҫ�Ļ����������ѡ�����·����ڴ档
*   [out,retval]��ʾ���������������ͬʱ������Ϊ�����������ķ���ֵ��[retval]ֻ����һ��������Ҫ��[out]��Ϻ������һ��λ�á�
*     VBS��VC(Import)�����Եİ�װ����Ϊ�����ķ���ֵ����������˴���ͨ���쳣�����д���
*
* Visual Studio�Ĺ������У�ATL��
*   Allow merging of proxy/stub code -- �ѡ�����/���������ϲ������������(��Լ����ע��һ��DLL)��������Ҫ�������룬����ע�����������
*     ѡ�к�VC7�в������ɵ����� XXXCP ���̡�Ϊ�������ĺϲ�����/������룬��Ҫ���� _MERGE_PROXYSTUB �꣬������ dlldata.c ���ļ�
*   Support MTS(Microsoft Transaction Server) -- ֧�������������Ҳ����֧�� COM+ ���ܡ�����ʱ���Զ����� mtsrereg.exe ?
*   Threading Model -- ���֧�ֵ��߳�ģ��(���������ڷ�������ע�������е�ThreadingModelֵ)
*     Single    -- CComSingleThreadModel,û�� ThreadingModel ����
*     Apartment -- CComSingleThreadModel,ThreadingModel = s 'Apartment'
*     Both      -- CComMultiThreadModel ,ThreadingModel = s 'Both' 
*     Free      -- CComMultiThreadModel ,ThreadingModel = s 'Free'
*     Neutral   -- CComMultiThreadModel ,ThreadingModel = s 'Neutral'�����ԣ�ֻ������Win2K���Ժ� 
*   Interface   -- �ӿ�����
*     Dual      -- ˫�ӿڣ���һ���ӿ��У�ͬʱ֧���Զ���ӿں� IDispatch �ӿڣ����Զ���ӿ� �� IDispatch �̳У�
*                  �ͻ��˵���ʱ������ѡ��ʹ�� ǰ�� �� ��� -- ������ű����Ե�ʹ�÷����ԣ���������������Ե�ʹ�ø�Ч��
*                  ���������������� "dual",�ӿ����� id ���,CMyMath �� IDispatchImpl<IMyMath...> �̳У�
*                  BEGIN_COM_MAP ӳ������ COM_INTERFACE_ENTRY(IDispatch)��
*       ���������ȷ��Ҫ֧�ֽű��ĵ��ã�����ò�Ҫʹ��˫�ӿڣ�
*         1.������к���������һ��˫�ӿ��У���ô��Ρ��ṹ�����಻�壬
*         2.���ʹ�ö��˫�ӿڣ���������������
*         3.˫�ӿڡ�IDispatch�ӿ�ֻ֧���Զ������ݵĲ������ͣ�ʹ���ܵ����ƣ�ĳЩ����ºܲ����� -- �򡰱���ʹ��VARIVANT����
*         4.˫�ӿ������û��ӿڣ�һ�������Ͳ����ٸı䡣
*     Custom    -- �Զ���ӿ�, IMyMath �ӿ� �� IUnknown �̳У�CMyMath �� IMyMath �̳�
*   Aggregation -- �����Ƿ��ܹ���Ϊ�ܿص��ڲ��������ۺϣ�����Ӱ����Ķ����Ƿ�����Ϊ�ⲿ���ƶ���ʹ�þۺ�
*   Free Threaded Marshaler(FTM) -- ����һ�������߳��м�����������Ч����ͬһ�����̵Ĳ�ͬ�߳�֮���м��ӿ�ָ��(����������˵̫�����)
* 
* ISupportErrorInfo -- �̳��б������� ISupportErrorInfo�������� CXXX::InterfaceSupportsErrorInfo ������ʵ��
* Connection Points -- �̳��б������� IConnectionPointContainerImpl<XXX>������ CProxy_IXXXEvents �࣬
*                      IDL������ dispinterface _IXXXEvents�ӿ����� �� [default, source] dispinterface _IXXXEvents;
* IObjectWithSite -- IE����֧��
* 
* Visual Studio�Ĺ������У�MFC�� -- ֧���Զ���
*   1.���� MFC DLL����Ҫ֧�� Automation;
*   2.New Class -> ����Ҫѡ�� CCmdTarget -> Automation ��ѡ�� "Createable by type ID"(��������ʵ������COM�������)
* 
* �Ѿ������Ľӿڲ����ٸ��ģ���֤��������ġ�³�����ԣ���ֻ���ṩ�µĽӿڣ����ڽӿ��а����ɽӿ�
*   (ֻ��Ҫ����������ʵ�ֻ��Զ�ʹ�þɵ�)���� IMath(���û�) -> IMath2(���û�)
* ���Ҫ��һ���������ӵڶ����ӿڣ������ֹ����� IDL ��������������Ϣ��������ʹ���򵼣����ɵ���һ��DLL�еĵڶ����������
*   1.IDL �еĽӿ�������interface IMyMath2 ...
*   2.IDL �� library -> coclass �еĽӿ��б�
*   3.MyMath.h ͷ�ļ��еļ̳��б�
*   4.BEGIN_COM_MAP ���Ľӿ�ӳ����ڱ� -- ���ܱ� QueryInterface �ҵ�
*   
* �����Ӷ���ӿ�ʱ��Ӧ���ǡ�Custom���ӿڣ�������IDispatch�̳е�Dual�ӿڣ���������ܻ�������� IDispatch �ļ̳л��ң�
* 
* ����
*   1.ѡ����С����������̬���ӵ�ATL����Ŀ���ļ��ߴ���Դ󣬵�����ʱ����Ҫ ATL.DLL �ļ�
*   2.����� ATL ��������е����� CRT ������ʱ�̿⺯�������翪ƽ�� sqrt() ����ô�����ʱ����ܻᱨ��
*     ��error LNK2001: unresolved external symbol _main�� -- ��Ҫɾ��Ԥ����ꡰ_ATL_MIN_CRT����VC6����
*      ѡ���� ATL ����Сʹ�� CRT����VC7��
*   3.IDL(�ӿڶ�����������) -- ����Ľӿ����������κ����Ի����ж��ܹ���ʶ��IDL �����������ɶ����Ƶĵȼ����Ϳ�
*     �ļ� TLB �ṩ����������������(�ű���VB��)��ʹ��
*   ע�⣺
*     a.��������Ϳ��� TLB �ļ���ʽ�������ڣ�ͬʱҲ������Ŀ���ļ�����Դ�С���ˣ��� #import �������Ϳ��ʱ��
*       �ȿ���ָ�� TLB �ļ���Ҳ����ָ��Ŀ���ļ���
*     b.IDL ������ṩ�� C ������ʽ�Ľӿ��ļ���XXX.h�� �� CLSID �Ķ����ļ�( XXX_i.c )����Ҫ����ͻ��˹��̣�
*     c.IDL ��������ɴ���/���Դ�����У�dlldata.c��xxx_p.c��xxxps.def��xxxps.mak�������� NMAKE.EXE 
*       �ٴα��������������Ĵ���/���DLLĿ���ļ�
*    
* �����������ķ�����ǰ����Ч����ߣ���
*   1.#include IDL ��������ɵ� XXX.h �� XXX_i.c �ļ���ֱ��ʹ�ýӿ�(�ӿڱ仯���������) -- ǰ��
*   2.#import��(ʹ��LoadTypeLib API)�������Ϳ�(tbl)���ִ�г���(dll �� exe)����VC������װ��(tli �� tih�ļ�)��
*     ��������(������һ���ڶ����꣬����ͨ�� "\" ���л��У������������֮���ÿո�򶺺ŷֿ�):
*       exclude("OLECMDID", "OLECMDF") -- �ų�ָ���Ĳ���? �� #import <shdocvw.dll> ʱ
*       named_guids -- ����IID��CLSID�����֣�����Ҫ��__uuidof(...)������
*       no_namespace -- ���������ƿռ�(Ĭ����idl�ļ���library���������)
*       no_implementation --
*       no_smart_pointers -- 
*       raw_interfaces_only -- ��ʹ��ԭʼ�ӿ�(����HRESULT���ҿ�����XXXEvents�ķ���)��ȱʡʱimport���Զ����ɷ���automation�Ľӿ�(ͨ���쳣ָʾ����)��
*         �� Foo([out, retval] long * pVal) ����
*           ȱʡʱ��������ָ�� IXxxPtr(��ǰ�ķ�������Ϊ raw_XXX)�����÷�ʽΪ long val = obj->Foo();
*             ��Ҫ�� try...catch(const _com_error &e) �������,���з�װ��hr�����й�����IErrorInfo����
*           ʹ�ø����Ժ󣬵��÷�ʽΪ hr = obj->Foo(&val);
*         ����������������������ɵ� tlh/tli �ļ����ܿ��� Invoke �ķ�������dispid
*       raw_native_types
*       rename("����","����") -- ��ָ����������������������ڽ�� "error C2872 ambiguous symbol" �����Ƴ�ͻ����
*   3.ʹ�� IDispatch �ӿ�ֱ�ӵ��ã����� CComDispatchDriver ������ָ����ã�
*   3.�������Ϳ��װ��(����ṩ�� IDispatch �ӿ�) -- ����� -> MFC -> ���Ϳ��е�MFC��
*   4.����ActiveX��װ��
*   5.�ڽű��Ƚ��������У�ʹ���Զ����ӿ�(IDispatch) -- COM�������ʵ����IDispatch�ӿ�
* 
* COM �ĵ��������ְ󶨷�ʽ��Ч���ɸߵ��ͣ�
*   ǰ�� -- Custom�ӿ�(����ʱͨ��VTableȷ��������ַ)��
*   ID�� -- ͨ�����Ϳ�(import/class->from TypeLib)���ڱ���ʱȷ��DispID����ͨ��DispID����Invoke��
*     ����ܵĻ���ֱ��ͨ��VTable��
*   ��� -- IDispatch������ʱͨ�� GetIDsOfNames ��������DispID����ͨ�� ID ���� Invoke
*
* ����ָ��(ģ���װ��) -- T* operator->() const throw(){ return m_p;}  //ʹ�������﷨����"->"��������ؽӿڵ�ָ��
*   CComPtr -- �����������ָ��
*     Release -- ���ý�ָ���Ա������ֵ��һ����ʱ�����У����ѳ�Ա��������ΪNULL�󣬲�ʹ����ʱ�����ͷŽӿ�ָ�룬
*       �������������ֻ��ָ�뱣��Ľӿڿ��ܱ��ͷ����ε����⣨��������ָ�뻥�������ã�Release���ͷ�ʱ��ɶԷ�������
*       ������ñ�ָ��������������л����Ա�����Ƿ�ΪNULL�����NULL������ظ��ͷţ�
*     operator->() -- ���� _NoAddRefReleaseOnCComPtr ģ���࣬��ֹ(�������)ֱ��ʹ��ָ���Ա������ AddRef/Release
*     SetSite -- ���ڲ�ָ�����õĶ�����һ��site����(����punkParent)��ϵ������ֻ��ָ�����ָ��һ��ʵ���� IObjectWithSite �ӿڵĶ���
*     Advise(Sink) -- ��һ�����ӵ���������������ܽӿ�ָ�����õĶ���(�¼�Դ����)��ϵ����
*   CComQIPtr -- CComPtr ����չ�������Զ�����QueryInterface����,Ψһ��ȱ����ǲ��ܶ��� IUnknown * ָ��
*     CComQIPtr < IMyMath, &IID_IMyMath > spMyMath;
*   CComDispatchDriver -- ��IDispatch�ӿڵ�����ָ��(typedef CComQIPtr<IDispatch>)���ɻ�ȡ�����ö��������
*     CComDispatchDriver pDisp = XXX; pDisp->GetProperty(DISPID_COUNT, &var);
*     GetIDOfName -- ���Ը����������ֵ��ַ���������Ե�DISPID
*     GetProperty/PutProperty -- ��ȡ���������Ե�ֵ
*     GetPropertyByName/PutPropertyByName -- ����������������ȡ����������ֵ
*     InvokeN -- �ṩ�˶�����صķ����������Invoke�ĵ���
*   _com_ptr_t -- ʹ�� _COM_SMARTPTR_TYPEDEF �궨�������ָ��(IXxxPtr)��ʹ���쳣��������(_com_error)
*
* �����ڹ���
*   �����������ڣ����������������
*   ��������ڣ��ӿ����ü���
* �ڴ����
*   IMalloc
*   CoGetMalloc -- MEMCTX_TASK(��ҵ������)��MEMCTX_SHARED(��������� )
* 
* ������(CATID) -- ��������Ĺ�������(ͨ���������������������ö��ϵͳ��֧�ֵ����-�����VC�в���ActiveX�ؼ���
*   �����Զ������ʱ�����õķ����Ƿ���PluginĿ¼�£�����ʵ���ض��ӿڵ�DLL��)�������� OLEView �鿴��
* ������������(Component Categories Manager -- CCM)��һ���򵥵�COM���
*  ��CLSID_StdComponentCategoriesMgr����ʵ�����������Ľӿڣ�
*   ICatRegister
*     RegisterCategories <== ������ע���Զ�������ͨ����DllRegisterServer�����н���
*     UnRegisterCategories <== ע���Զ��������� DllUnRegisterServer �����н���
*     RegisterClassImplCategories <== ע�� CLSID Ϊָ�������
*     RegisterClassReqCategories  <== ?
*   ICatInformation 
*     EnumCategories
*   ע���Զ������
*     CATEGORYINFO catinfo;
*     catinfo.catid = CATID_MyComPlugin;
*     catinfo.lcid = 0x0409;  // locale=english
*     USES_CONVERSION;        // uses A2W
*     wcscpy(catinfo.szDescription, A2W("My Com Plugin."));
*     pcr->RegisterCategories(1, &catinfo);
*
* ������Ϣ֧��
*   ���COM���Ҫ��Ӧ��Ϣ(��CVzCsManagerͨ����Ϣ����)�����Դ� CWindowImpl �̳У�����DECLARE_WND_CLASS��(ע�ᴰ���ࣿ)��
*   ʹ�� BEGIN_MSG_MAP ������Ϣӳ�䡣�� FinalConstruct �е���Create�������ڣ���FinalRelease�е���DestroyWindow���ٴ��ڡ�
*    
*
* ATL���ڲ�������ʽ�Լ�����ʹ�õļ���
*   1.û���κγ�Ա��������СΪ 1(ռλ)���г�Ա�󣬴�СΪ��Ա(���ݡ�����)�Ĵ�С֮�͡�
*     ��ͨ�����������Ӵ�С�������麯��ʱ������һ���麯����ָ��VTPR(��СΪ4�����ڴ�ṹ�еĵ�һ��λ��)
*     �麯�����б������������麯���ĵ�ַ�����һ�����ΪNULL��
*     ���ؼ̳�ʱ���������ӵ�����л�����麯����ָ�룬��sizeof(obj) = 4 * ���麯���Ļ������ ����
*     ATL ʹ�ú� offsetofclass(base, derived) ���������������ģ���л����麯����ָ���ƫ���� -- ����ָ�������е��麯��
*
* ATL �������Ͽؼ�(���У�Controls->Composite Control) -- һ���ؼ����ж���ӿؼ�(�����ͬʱ�����б�ؼ��ͽ������ؼ�)
*   Ȼ�������IDE��"ToolBox->General"��ѡ��"Add/Remove Items" �����������COM�����������Dialog��ʹ�á�
*   ����ؼ��¼����Ҽ� -> Add Event Handler...
*
* ӳ�䣺
*   1.COM�������ӳ��(����COM�����C++ʵ�����ӳ�䣬�Ǿɵķ�ʽ���µķ�ʽ�Ѿ�����OBJECT_ENTRY_AUTO)
*       BEGIN_OBJECT_MAP(ObjectMap) -> OBJECT_ENTRY -> END_OBJECT_MAP
*       OBJECT_ENTRY(CLSID_AtlComp, CAtlComp) <== �� CLSID_AtlComp ӳ�䵽 CAtlCom �࣬�����๤����
*       �� CoCreateInstace ʱ֪��Ӧ��new�ĸ�
*       ʵ�֣�����һ����̬ȫ�� _ATL_OBJMAP_ENTRY ������ ObjectMap[]������ _Module.Init(ObjectMap...) �г�ʼ��
*   2.�ӿ�ӳ�䣺BEGIN_COM_MAP -> COM_INTERFACE_ENTRY -> END_COM_MAP
*       http://www.cnblogs.com/huqingyu/articles/46505.aspx
*       COM_INTERFACE_ENTRY(IMyMath)  <== ���� QueryInterface �� IMyMath �ӿ�
*       COM_INTERFACE_ENTRY2(��ѯ�ӿ�, ָ���ļ̳з�֧)  -- ��ѯ���μ̳еĸ��ӿ�ʱ��ͨ��ָ���ļ̳з�֧���أ�
*         COM�̳���������ж����֧ʵ����ͬһ���ӿ�(��IDispatch)����Ҫ֪����ѯ����ӿ�ʱ���ĸ���֧���ظ���
*         ���л����� pEntries->dw ��ֵΪ (DWORD)((x*)(x2*)((_ComMapClass*)8))-8��Ȼ��ͨ�� 
*         IUnknown* pUnk = (IUnknown*)((int)pThis+pEntries->dw) �ҵ���Ӧ��֧���麯����ָ��
*           <== ���μ̳�ʱ������麯����ָ����ڴ�鶥��֮���ƫ����
*       COM_INTERFACE_ENTRY_IID(��Ҫ��IID, ʵ�ʻ᷵�صĽӿ�����) -- һ�������и��ӹ�ϵ�Ľӿ�? �������ھۺ�?
*       COM_INTERFACE_ENTRY_TEAR_OFF(IID_IMyTearOff, CMyInnerTearOff) -- �Ѻ����õ���IID_IMyTearOff�ӿ�
*         ����һ�����������(CMyInnerTearOff)��ʵ�֣�������ѯ������ӿ�ʱ���Ŵ�����������
*         ���ҵ��������ü�����Ϊ0ʱ�ͻᱻ�ͷŵ���ע�⣺�ڲ��� CMyInnerTearOff ��Ҫ�� CComTearOffObjectBase �̳�
*       COM_INTERFACE_ENTRY_CACHED_TEAR_OFF -- ����ѯ�ָ�����������ӿ�ʱ���������½��µĶ���
*       COM_INTERFACE_ENTRY_AGGREGATE -- ���ھۼ�����
*       COM_INTERFACE_ENTRY_NOINTERFACE -- ��ѯʱ�᷵��E_NOINTERFACE(�����ñ��˲鵽����ӿ�)
*   3.����ӳ��(Ҳ֧�ֳ־û�)��BEGIN_PROP_MAP -> PROP_ENTRY/PROP_DATA_ENTRY -> END_PROP_MAP
*       PROP_DATA_ENTRY("_cx", m_sizeExtent.cx, VT_UI4) <== 
*       PROP_ENTRY("ForeColor", DISPID_FORECOLOR, CLSID_StockColorPage)     <== ӳ��ϵͳ��ForeColor����
*       PROP_PAGE(CLSID_XXXX) <== ָ��һ��ATL���е�����ҳ����������ؼ������Ե�ʱ���԰�������������ҳ
*   4.���ӵ�ӳ�䣺BEGIN_CONNECTION_POINT_MAP -> CONNECTION_POINT_ENTRY -> END_CONNECTION_POINT_MAP
*       CONNECTION_POINT_ENTRY(__uuidof(_IMyTimerEvents))  <== ��ǰ���ӵ��������� GUID Ϊ __uuidof(_IMyTimerEvents) �����ӵ�
*   5.��Ϣӳ��(WTLʹ�� _EX �汾���Խ�����Ϣcrack)��BEGIN_MSG_MAP -> CHAIN_MSG_MAP/MESSAGE_HANDLER -> END_MSG_MAP
*       MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)  <== �� OnInitDialog �������� WM_INITDIALOG ��Ϣ
*       CHAIN_MSG_MAP/CHAIN_MSG_MAP_MEMBER <== δ�������Ϣ���Խ���������Ա�����Ƚ��д���
*       DEFAULT_REFLECTION_HANDLER <== ���з���?
*   6.�¼�����ӳ�䣬�ṩDISPID�ͺ���ʵ�ֵĶ�Ӧ��ϵ���������ӵ�ͻ���ʵ�֣���dispid��Ӧ����������
*       BEGIN_SINK_MAP ( MyClassName )
*         SINK_ENTRY_INFO(��ʶID, DIID_DWebBrowserEvents2, DISPID_DOCUMENTCOMPLETE, OnDocumentComplete, &DocumentComplete2_Info)
*         SINK_ENTRY_EX ( ��ʶID, DIID_DWebBrowserEvents2, DISPID_DOCUMENTCOMPLETE, OnDocumentComplete )
*         SINK_ENTRY(��ʶID, DISPID_DOCUMENTCOMPLETE, OnDocumentComplete)
*       END_SINK_MAP()
*       ˵����
*         a.SINK_ENTRY_INFO -- ��Ҫ���ݶ�Ӧ��������_ATL_FUNC_INFO���͵ı������� 
*           __declspec(selectany) _ATL_FUNC_INFO CBrowserView::DocumentComplete2_Info = { CC_STDCALL, VT_EMPTY, 2, { VT_DISPATCH, VT_BYREF | VT_VARIANT } };
*         b.���ͨ�� GetFuncInfoFromId �ṩ���÷�ʽ(Ĭ��ͨ�� AtlGetFuncInfoFromId ʵ��?)��ʾ�����£�
*           virtual HRESULT GetFuncInfoFromId ( const IID&, DISPID, LCID, _ATL_FUNC_INFO& ) { check(iid); switch(dispid){info.xxx=???;}}
*         b.OnDocumentComplete ������ǩ����ǩ������ͨ��MSDN�� OleView �鿴��Ӧ�� tlb ���ļ���
*           void __stdcall OnDocumentComplete ( IDispatch* pDispatch, VARIANT* pvUrl); 

*   7.ʵ��������ע��(��IE������������)��BEGIN_CATEGORY_MAP -> IMPLEMENTED_CATEGORY -> END_CATEGORY_MAP
*       �������������ע����Ϣ������ "Implemented Categories" ����Ͷ�Ӧ�� CLSID
*       IMPLEMENTED_CATEGORY(CATID_SafeForScripting)  <== ʹ�ýű�ʱ����ӽű���ȫ������ͣ�����ȥ��������ȫ����������ʾ
*       IMPLEMENTED_CATEGORY(CATID_SafeForInitializing)
*       IMPLEMENTED_CATEGORY(CATID_InfoBand) <== IE�еĴ�ֱ�����?
*       IMPLEMENTED_CATEGORY(CATID_CommBand) <== IE�е�ˮƽ�����
*       IMPLEMENTED_CATEGORY(CATID_DeskBand) <== ���湤����
*       IMPLEMENTED_CATEGORY(CATID_SimpleFrameControl) <== 
*       IMPLEMENTED_CATEGORY(CATID_PropertyNotifyControl) <== 
*       IMPLEMENTED_CATEGORY(CATID_VBDataBound) <== 
*       IMPLEMENTED_CATEGORY(CATID_WindowlessObject) <== 
*       IMPLEMENTED_CATEGORY(CATID_PersistsToMoniker) <== 
*       IMPLEMENTED_CATEGORY(CATID_PersistsToStreamInit) <== 
*       IMPLEMENTED_CATEGORY(CATID_PersisitsToStream) <== 
*       IMPLEMENTED_CATEGORY(CATID_PersistsToStorage) <== 
*       IMPLEMENTED_CATEGORY(CATID_PersistsToMemory) <== 
*       IMPLEMENTED_CATEGORY(CATID_PersistsToFile) <== 
*       IMPLEMENTED_CATEGORY(CATID_PersistsToPropertyBag) <== 
*       IMPLEMENTED_CATEGORY(CATID_RequiresDataPathHost) <== 
*       IMPLEMENTED_CATEGORY(CATID_AppContainerCompatible) <== IE��������֧��EPM, //{59FB2056-D625-48D0-A944-1A85B5AB2640}
*   8.����ע�᣿��BEGIN_SERVICE_MAP -> SERVICE_ENTRY -> END_SERVICE_MAP
*
*   A.MFC�� BEGIN_EVENTSINK_MAP -> ON_EVENT -> END_EVENTSINK_MAP
*       ON_EVENT(CTestAtlDlg, IDC_ATLCOMP1,     1    ,  OnAtlEvent, VTS_I4  VTS_BSTR)��
*       �����ֱ��ǣ��������,  �ؼ�ID,      Invoke �� ID, �����������������б�������� OnAtlEvent(LONG state,BSTR info)
*
* ���ַ��� -- ?
*
* ���ýӿڼ�����ʵ��(ATLͨ�������� IxxxImpl �ӿ�ʵ��ģ���� �ṩ�ӿڵĻ���ʵ�ֺ���չ)��
*   IConnectionPointContainerImpl -- ���ӵ������Ļ���ʵ�֣�֧�ֲ��Һ�ö�����ӵ�
*   IDispatchImpl< disp����ӿ�, &IID_��Ӧ�Ľӿ�, &LIBID_�����ӿڵ��������, ���汾��, �ΰ汾�� >
*     -- �ṩ�Զ���/˫��ӿڵ�ʵ��֧��(IDispatch�ӿ�ʵ��)
*   IDispEventSimpleImpl -- �¼�����Ļ���ʵ�֣�����Ҫ�Լ��ֹ�(���� GetFuncInfoFromId �� ʹ�� SINK_ENTRY_INFO)�ṩ�����ĵ���ԭ��(SINK_INFO)
*     <== IDispEventImpl<��ʶID, ������, &__uuidof(��Ҫ������¼��ӿ�), &__uuidof(XXX::library), ���汾��, �ΰ汾��>
*           ATL �����ӵ�ͻ��˵�ʵ��(�� Invoke�д��� DISPID_XXX )�������ڶ�����ʱָ����Ӧ�����Ϳ�(LibId�Ͱ汾)��
*           �Զ����ض�Ӧ��Lib��ú����ĵ���ԭ�ͣ�����ֱ��ʹ�� SINK_ENTRY_EX/SINK_ENTRY��
*           �汾���� AtlGetObjectSourceInterface ���(��û�й���?)��Ȼ���Ӧ����
*   
*   IObjectWithSiteImpl -- Site��λ�ڰ������뱻���ݶ�����������֮���һ������,ͨ�������������ܶԱ�����
*       �����ϸ�ڽ��п��ƣ��������ݶ�������ִ���Լ��Ĺ��ܣ�IObjectWithSite������ͨ���ߵ�Site����ӿڡ�
*   IOleWindow          -- 
*   IPropertyPageImpl   -- ����ҳ�������� m_ppUnk[i] �Ľӿ�����
*     
* API ����
*   CoRegisterClassObject -- �����������������ע���๤��(˵�����๤����ʹ��)
*     CoRegisterClassObject(CLSID_CHello, pCF, CLSCTX_LOCAL_SERVER,REGCLS_MULTIPLEUSE, &dwRegistration);
*   DllGetClassObject   -- �����ڷ����������๤��
*   CoCreateInstanceEx  -- ������Զ�̻����ϴ���COM��������ҿ���һ�β�ѯ����ӿ�
*
* QueryInterface�ĵ���˳��
*   1.CComObject::QueryInterface
*   2.YourClass::_InternalQueryInterface
*   3.CComObjectRootBase::InternalQueryInterface
*   4.AtlInternalQueryInterface 
*
* COM��
*   CComClassFactory    -- �ṩ�˶Ի������๤����֧�֣��е�ʵ����չ CComClassFactorySingleton 
*   CComCoClass         -- �ṩ����֧��(ͨ��Error�������ͳ�����Ϣ�����˷��ͳ�����Ϣ�Ĺ���)�����ܻ�ȡ�����CLSID��������Ϣ
*                          ��Ҫ�ṩ ISupportErrorInfo ֧��
*   CComControl         -- ActiveX�ؼ��Ļ���
*   CComCreator(2)      -- �ṩCreateInstance������ͨ������ָ�봴��ʵ���� ÿ��creator����һ����̬CreateInstance������
*   CComModule          -- ��װ�����������(DLL �� EXE)֧�֣������� CATLComModule������ CWinApp
*   CComObject          -- ������COM������Ǵ�����(IUnknown)����Ҫ�� CMyMath ��Ϊģ�� CComObject �ĸ��࣬CComObject����л��CComObjectRootEx�Ĺ���
*                          �ṩ IUnknown �Ľӿڣ��� CComObject(�Ǿۺ϶���)��CComAggObject(�ۺ϶���)��
*                          CComPolyObject(����)��CComObjectCached(�����๤��)�ȼ��ֲ�ͬ�����͡�
*                          �����Ҫ�ֹ����� COM �������Ҫʹ���������µķ�����
*                          CComObject<CMyMath> *pMyMathObj;  //���ʹ������ָ�룬���ü�����������⣬Ϊʲô��
*                          CComObject<CMyMath>::CreateInstance(&pMyMathObj);  //�õ����Ƕ����ָ��pMyMathObj�������ǽӿ�ָ��
*                          pMyMathObj->AddRef(); //�������һ���������ü����������Ժ���� -1���޷�ɾ��
*                          ʹ����Ϻ󣬱���ͨ�� pMyMathObj->Release(); �����ͷ�
*   CComObjectRootBase  -- ������FinalConstruct �� FinalRelease �������ڳ�ʼ��������
*   CComObjectRootEx    -- ģ����࣬�ṩǡ���㹻�̰߳�ȫ(just-safe-thread-enough)�Ķ����������ڹ���������������
*                          ��IUnknown�Ĳ���ʵ�֣���������ͨ�� BEGIN_COM_MAP ʵ��
*   ObjectLock          -- �� CComObjectLockT ���� typedef ���Զ���: ObjectLock lock(this);
******************************************************************************************************/

#if 0

//�������� WORD �е��¼�(MSDN Sample�е� ATLEventHandling)
// #import doesn't generate a LIBID, so we have to do it manually
namespace Word
{
	struct __declspec(uuid("00020905-0000-0000-C000-000000000046"))
		/* library */ Library;
};

class CMyWordEventHandle : xxxxx
	, public IDispEventImpl</*nID*/ 1, CNotSoSimple, &__uuidof(Word::ApplicationEvents2), 
	  &__uuidof(Word::Library), /*wMajor*/ 8, /*wMinor*/ 1>

#endif

#endif //COM_STUDY_H