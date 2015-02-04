#ifndef COM_THREADING_MODEL_H_
#define COM_THREADING_MODEL_H_

/***********************************************************************************************
* COM���߳�ͬ��
*   ʵ�����ݵ�ͬ��:���󼶵���(���㰲ȫ�������Ե�) �� ϸ���ȵ�������(����߲�������ҪС��ʹ�÷�ֹ����)
*     CComCriticalSection -- ��Ҫ�ֶ� Init/Term �� CRITICAL_SECTION, Init->Lock->Unlock->Term
*       ������Ҫһ��ȫ�ֻ��߾�̬���ٽ���������C���п�(CRT)�����Զ���ɹ������������ʱ��Ϊʲô������?��ʹ��CRT?��
*     CComAutoCriticalSection -- �������Զ���ʼ�����������Զ���ֹ�� Lock->Unlock
*     CComAutoDeleteCriticalSection -- �� CComCriticalSection �̳У����Է�ֹ��ε��� Init/Term ?
*     CComFakeCriticalSection -- ͨ�����ڵ��߳�ģ��ʱ�����������Ŀ���
*   �Զ��ķ�Χ����CComObjectLockT<>���� CComObjectRootEx ��typedef Ϊ ObjectLock��ʹ�÷�����ObjectLock lock(this);
*
* CComXXXThreadModel -- �����˾�̬�� Increment/Decrement�����ڳ��������һ��������������ڼ������ڵ��̺߳Ͷ��̷߳�ʽ�µĲ���
*   CComSingleThreadModel
*   CComMultiThreadModel
*   CComMultiThreadModelNoCS -- ���ü���ʱ�����̰߳�ȫ�ķ�ʽ������ʹ�ö���������û�г����ü�������Ҫ�����ĳ�Ա����
*      ��ʹ��ϸ���ȵ�������
*   
* �������ġ�ȱʡ���߳�ģ��(û�п�����Ӧ�ĺ�),ATLʹ����صĺ���typedef CComObjectThreadModel/CComGlobalsThreadModel��
*   ATL���ڲ�ʹ�� CComObjectThreadModel ����ʵ�����ݣ� CComGlobalsThreadModel ������ȫ�ֺ;�̬���ݡ�
*   Ϊ�˱�֤����������ȷ�����·�������ֻ��һ�����߳��࣬Ҳ����������ɵ¶� _ATL_APARTMENT_THREADED ��Ϊ _ATL_FREE_THREADED
*     _ATL_SINGLE_THREADED
*     _ATL_APARTMENT_THREADED  -- ftlConfigDetect.h ���ж�����ж�
*     _ATL_FREE_THREADED
*
* һ������ֻ����һ��MTA����һ��Ҳû�С�����ʹ��MTA MODULE��COM OBJECT��ͬһ��MTA��ߣ��������ʱ��ͨ��stubs��proxies ��
* 
* ʹ�� IComThreadingInfo �ӿڿ��Ի�õ�ǰ��COM�߳�ģ�� -- ���⣺CLSID_???
* COM���߳�ģʽָ�����������棺
*   �ͻ�������߳�ģʽ -- STA��MTA
*   �����֧�ֵ��߳�ģʽ -- Single(���߳�)��Apartment(STA)��Free(MTA)��Both(STA+MTA)��Neutral
* ����ͻ�����������֧��ͬ�����߳�ģʽ����ôCOM������ͻ�����ֱ�ӵ��ö�������������������ܡ�
*
* Apartment(�׼�/��Ԫ) -- ���ڰ�������Ŀ�������ʵ�ֶ��߳��µ������ʱ���ͬ������
*   �ṩ�������߳�ģ�ͣ������ֲ�ͬ���׼�ֱ��Ӧ��
* COM �ṩ�����߳�ģ��
*   0.STA����ǰ������������ʶ�ͨ��Ӧ�ó�������߳�(��Ϣѭ��)���У�������� -- ��STA
*   1.Single-Threaded Apartment��STA ���߳��׼䣩-- ÿһ��COM�߳����Լ���STA��
*     �ʺ���ӵ�н�����������ActiveX�ؼ���OLE�ĵ��������ȡ�
*     �׼��1���̡߳�0~n�����������ͨ������һ������OleMainThreadWndClass����������ش��ڣ������߳�ͨ������
*     ��Ϣ�ķ�ʽ���е���(COM���Զ�����ͬ��)��STA�еĶ�������Ҫ���У������Ӱ�졣�ȫ�������ܺܲ
*   2.Multithread Apartment��MTA ���߳��׼䣩-- ����ͬ�̵߳�CLIENTͬʱ����COM OBJECT������������Ҫ��ϸߵ�ҵ���߼������ɺ�̨��������,
*     �׼�Ͷ���߳�,0~n�����������ֻ�и��׼��е��߳��ܷ��ʶ��������̶߳��׼��ڶ���ĵ��ý�
*     ʹ��RPC������̳߳� ����Զ�̵��ã����Ҹö���Ĵ������Լ��ķ�����ʵ�����̱߳�����
*     ����ϵͳ�ķ���ʱ�̰߳�ȫ��������ȫ�ֱ�������̬������Ҫ�����߳�ͬ���������еȡ�
*     ��Ҫ�Լ�ͬ��������ʴ��룬�� Release ʱ�ĳ�ͻ����
*       DWORD temp = InterlockedDecreament( &m_RefCount ); if(!temp) delete this;   //�ж�ʱ����ʹ��m_RefCount
*     ��дMTAʱ���ܱ����κμ�¼��TLS�ڴ��ָ��򴰿ھ����û���κ��̷߳�Χ�ĳ�Ա������
*       -- ����MFC�ײ����ʹ��TLS�����MFC���ܱ�д��MTA�����е������
*   3.Neutral Apartment��NTA �����߳��׼䣬��COM+�ṩ��ֻ�����ڽ����ڷ�������
*     �׼䲻�����̣߳�ֻ���� 0~n �������κ��̶߳�����ֱ�ӷ�������Ķ��� -- ������ѣ�����Ҫ�û�ͬ��,
*     COM+�е�ÿ��������һ������������󶨣�������¼�˱�Ҫ����Ϣ���������Զ����ÿһ�ε��ã�
*       �Ա�֤��������Ľӿ�ָ���Ա�������д��ݻ�ص�ʱ���������ȷ��.
*   3.1.Both -- �����������NA���ĸ��׼䶼����ֱ�ӷ���������ֻ�ǿ��ܣ���NA����ǿ��ԡ�
*       ֧��һ�ֳ��� �����̵߳�������FTM����Free Threaded Marshaler���ļ��������Ի��������ܡ�
*       ��COM�ṩ��ϵͳ�����CoCreateFreeThreadedMarshaler��
*       FTMʵ���˽ӿ� IMarshal�������Ƚӿ�ָ��ĺ���ʱ���ڲ�����IMarshal�ڵ���غ��������ж�������ȷ���
*       ��һ�������ڵĹ�Ԣ֮����ֱ�ӷ��ؽӿ�ָ�룻������ȷ����ڽ���֮�����Զ�̼�����䣬����ñ�׼�ĵ�������
*       ������ָ���������ָ�롣���Կɼ�ʹ��FTM�����Կ����Ԣ��ĵ��ȶ�ֱ�ӵ��ýӿڷ��� -- ������ܡ�
*
* ����ѡ��(����������������������߳�ģ��)
*   Apartment   -- ���ֻ����STAģʽ����,��Ԫģʽ�������߳��е������������ʱ��ϵͳͨ�����ص�
*                  ������Ϣ���Ŷ���Щ���ã����ͨ�����ÿ���ͬ�������⡣
*                  �����ǵ��߳�ģ�ͣ�ȫ���Ƕ��߳�ģ�͡�
*   Free        -- MTA�������ȫ�ֶ��Ƕ��߳�ģ��
*   Both        -- STA+MTA���� �����̵߳�����(FreeThreaded Marshaler)���
*     1.���� CComPtr<IUnknown> m_pUnkMarshaler �ĳ�Ա����
*     2.�� FinalConstruct �� return CoCreateFreeThreadedMarshaler(GetControllingUnknown,&m_pUnkMarshaler.p);
*        
*   Single(None)-- ֻ����һ����һ���߳���ִ��(��STA),�����ȫ���߳�ģ�Ͷ��ǵ��߳�
*    
* �м���Marshaling����ɢ����Unmarshaling�� 
*     -- ���̼��Զ��Marshal����Ϊ���ڲ�ͬ�ĵ�ַ�ռ䲢��Ϊ�˽�������������ؽ���Marshal��Ϊ���߳�ͬ����
*   �м� -- ����Ϣ��ĳ�ָ�ʽ��Ϊ����IStream*����ʽ�Ĳ�����
*   ɢ�� -- �м��ķ�����������Ϣ������ʽ�з���������
*     �м�����ͨ������CoMarshalInterface���������,������CoMarshalInterface��һ��Ϊ��������м�ʱ��Ϊ����
*     ��һ�����������,���Ժ��ͬһ������������ӿ��м�ʱ,�Ͳ����ٽ�һ�������������
*     CoUnmarshalInterface���Ǹ���ʲô��ȷ���Ƿ�Ҫ���������������????
*
* һ�������ִ�������ʵ�ַ�ʽ -- ����׼�м����Խӿ�Ϊ�������Զ����м����Զ���Ϊ��������
*   �Զ���Marshal(custom marshaling) -- �����ʵ��IMarshal�ӿں�һ���������������ȫʵ������������нӿڵ�һ��������
*     ʵ���˻㼯�����������߳�ģ�͵Ĺ���[��ӿ�ָ����CoMarshalInterface/CoUnmarshalInterface]��
*     Ҳ����ʵ��IMarshal�ӿڣ������������������ڿͻ�����ע�ᣬ�Ա�֤����������ȷ���ɡ� 
*   ��׼Marshal(standard marshaling) -- �����ҪΪ�Լ�����һ������(Proxy)/���(Stub)���(��ͨ��MIDL��IDL�ļ��Զ�����),
*     ��ͨ���ۺϷ�ʽ����COM�ṩ�ı�׼��������ʵ�ֵ��á�
*
*  ������û���: Client [ -> ���ش���Proxy -> ͨ�� -> Զ�̴��Stub ] -> Server������ �������Զ�������
*   �Լ����̻߳����д���ӿ�ָ��(Ϊ��ͬ������)ʱ ʹ�� ����/������������������ע�������DLL���������ӿڶ��޷�������
*   COM+�д�����DLL��Ҫע�ᵽ������Ҫ�м�(Marshal)��ɢ��(Unmarshal)�Զ���ӿڵ�COM�ͻ��ͷ��������ڵĻ����ϡ�
* ����ʹ�ñ�׼���Զ���������(Standard Automation Marshaler�����Ϳ������)ʵ�ֵ��ȣ�����ÿ���������Ҫ
*   ������proxy/stub DLL����ʹ��OLEView�鿴 proxy/stub ���Ϊ oleaut32.dll��Ҫ��
*     1.�ӿڷ���������ʹ�����Զ������ݵ����Ϳ�;
*     2.��IDL�м��� oleautomation ���ԣ���ʹ�� RegisterTypeLibrary ����ע�����Ϳ⣩
* Marshaling ������Marshal���������ʱʹ��LPC���ֲ�ʽ������ʹ��RPC(���ʵ���Զ�����ȣ���Ҫʵ�� IMarshal �ӿڣ�
*   �磺����ʵ��Ϊʹ�� TCP/IP ʵ��DCOM )
*  
* ȫ�ֽӿڱ�(GIT)
*   ���԰ѽӿ�ָ�������У�Ȼ���ڱ�� Apartment �ڰ���ȡ����GIT�Զ�ִ�й�Ԣ��ĵ��ȣ����Ժܷ��㡣
*   ͨ��IGlobalInterfaceTable���ʵġ�ͨ������CLSIDΪ CLSID_StdGlobalInterfaceTable �Ķ���ɵ�������
*
* ע��
*   1.��MTA��STA��STA��MTA�����ᷢ���߳��л������Ĵ���ʱ��(�ں�/�û�ģʽ�л�)����˲�����NTA��
*   2.�׼�Ĺ�����ͨ���Դ����������������������Խ�ȡ���������ĵ����ɴ��������ʵ�ֵġ�
*   3.���׼����ʱ�������õĶ���ָ����ָ��һ��������󣬲���������������ֱ�Ӵ��ݽӿ�ָ�뵽������߳��е��ã�
*     ���ܱ�֤���ý����ȷ(�޷�ʹ��COM���߳�ģ��)����Ҫͨ�� CoMarshalInterface/CoUnmarshalInterface ��ʹ�ô��͵�
*     �ӿ�ָ��ָ��������ͬһ�����ڵ��߳̿���ʹ��CoMarshalInterThreadInterfaceInStream/CoGetInterfaceAndReleaseStream ���м򻯣���
*     ���⣺��ô��֤�ڽ����������ֱ�Ӵ��ݽӿ�ָ�����ִ��󣿣���
*           ��ô֪���߳̾������ڵ����ĸ��׼�(STA/MTA ��)������
*
* CoInitialize -- ����COM����һ���µ�STA�׼䣬������ǰ�ĵ����̺߳�����׼��������
* CoInitializeEx:
*   COINIT_APARTMENTTHREADED -- ͬ CoInitialize�����߳�ִ��ʱ�������á����л���������������̡߳�
*   COINIT_MULTITHREADED(Ҳ�������߳�) -- ����COM����Ƿ��Ѿ�����һ��MTA�׼䣬û��������һ��MTA�׼䣬Ȼ���Ǹ��׼�͵����߳��������
*     ���߳�ִ��ʱ�����������ĵ��á���ܡ��������COM�����ThreadingModel��ͬʱ��������ҪCOM��������ͬ����
*
* �����ڵ���STA�׼� -- �����е�һ������ COINIT_APARTMENTTHREADED ���߳����������׼䣨�������еĵ�һ��STA�׼䣩
* 
* ���������ʱ������ע��� InprocServer32\ThreadingModel �� �̵߳Ĳ�ͬ��������������׼���
*   �����̹߳������׼�����          ThreadingModel��ֵ              ���������������׼�  
*            STA                        Apartment                      �����̵߳��׼�(A)  
*            STA                           Free                        �����ڵ�MTA�׼�(B) 
*            STA                           Both                        �����̵߳��׼�(A)
*            STA                        ""��Single                    �����ڵ���STA�׼�(C)  
*            STA                          Neutral                       �����ڵ�NTA�׼�(D)
*            MTA                         Apartment                     �½���һ��STA�׼�(E)
*            MTA                            Free                        �����ڵ�MTA�׼�(B)
*            MTA                            Both                        �����ڵ�MTA�׼�(B)
*            MTA                        ""��Single                     �����ڵ���STA�׼�(C)
*            MTA                          Neutral                       �����ڵ�NTA�׼�(D)
*   �����̵߳��׼�(A)   -- 
*   �����ڵ�MTA�׼�(B)  --
*   �����ڵ���STA�׼�(C)--
*   �����ڵ�NTA�׼�(D)  --
*   �½���һ��STA�׼�(E)--
*
* ���ʵ����
*   1.����ActiveX��UI����⣿������ͬʱ֧�� STA/MTA �� Both ��� -- ������ã��������ߣ�����Ҫע��
*     a.����ѡ�� Free Threaded Marshaler ֧�֣��������� -- Ϊʲô��
*       ���ƣ�ʹ��FTM�Ķ�����ֱ��ӵ��û��ʵ��FTM�Ķ���Ľӿ�ָ�룻ʹ��FTM�Ķ�����ӵ��������Ԣ�����������á�
*     b.ע���̷߳��ʵĻ��ⰲȫ��ʹ�� Lock/Unlock �������б��������õ����� ObjectLock lock(this) �Զ�������
*   2.�̼߳䴫�ݽӿڱ������Marshal(CoMarshalInterThreadInterfaceInStream) -- �������Ҫ(��MTA��)��COM��Ҳ����ȷ����
*   3.ѡ��DLL �� Proxy/Stub Dll �Ĵ��룬��ֹ��ҪMarshal(�Ƿ������Ҫ������)
***********************************************************************************************/

#endif //COM_THREADING_MODEL_H_