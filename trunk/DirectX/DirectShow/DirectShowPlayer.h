#ifndef DIRECT_SHOW_PLAYER_H
#define DIRECT_SHOW_PLAYER_H
#pragma once

/****************************************************************************************************
* CLSID_DvdGraphBuilder -- DVD���ź͵�����֧�� IDvdGraphBuilder �ӿڣ�Ҳ����ʹ�� MSWebDVD ActiveX �ؼ�
* CLSID_FilterGraph -- DirectShow �Ĳ�������⣬���š���ȡ��ת���ȶ���Ҫ������֧�����½ӿڣ�
*   IAMGraphStreams
*   IAMOpenProgress
*   IAMStats
*   IBasicAudio
*   IBasicVideo
*   IBasicVideo2
*   IFilterChain
*   IFilterGraph
*   IFilterGraph2
*   IFilterMapper
*   IFilterMapper2
*   IFilterMapper3
*   IGraphBuilder -- 
*   IGraphConfig -- 
*   IGraphVersion
*   IKsQualityForwarder{97EBAACB-95BD-11D0-A3EA-00A0C9223196}
*   IMediaControl -- ý�岥��ת�����ƽӿڣ������� Run/Pause/Stop/StopWhenReady �ȷ���
*   IMediaEvent
*   IMediaEventEx
*   IMediaEventSink
*   IMediaFilter
*   IMediaPosition
*   IMediaSeeking
*   IMarshal
*   IObjectWithSite
*   IPersist
*   IPersistStream
*   IQueueCommand
*   IRegisterServiceProvider
*   IResourceManager
*   IServiceProvider
*   IVideoFrameStep
*   IVideoWindow -- 
*   {877E4352-6FEA-11d0-B863-00AA00A216A1}
*   
* ý���ļ��Ĳ��� -- ����ʹ�� COINIT_APARTMENTTHREADED ��ʼ��COM��(���̣߳�)
*   #.��Ƶ����(IBasicAudio)
*     VOLUME_FULL=0L;VOLUME_SILENCE=-10000L; Ҫ������������Ҫʹ�ö����������� log10(vol)/pow(10.0,pos)
*   #.��Ƶ����(Windowed ModeʱǶ����APP UI�еķ�ʽ)
*     1.����ǰ�ĳ�ʼ��
*       1.1.ʹ�� IVideoWindow �� put_Owner((OAHWND)m_hWnd); //���ò��Ŵ���ĸ�����,
*       1.2.put_WindowStyle(WS_CHILD | WS_CLIPSIBLINGS);// | WS_CLIPCHILDREN); //���Ĳ��Ŵ���ķ��
*       1.3.m_StaticVideoWindow.ModifyStyle(0,WS_CLIPCHILDREN); ��ֹ������Ƶ����(�����˵��ȣ���ȫ��ʱ�����޷�������ʾ?��Ҫˢ��?)
*       1.4.�����Ի����WM_ERASEBKGND��Ϣ -- ������ܻᱻƵ����ˢ�³� Gray ����ɫ�� ֱ�ӷ��ؼ��ɣ�
*           CRect rect;
*           m_StaticVideoWindow.GetWindowRect(&rect);  //��ȡVideo���Ŵ���ķ�Χ
*           ScreenToClient(&rect);
*           pDC->ExcludeClipRect(&rect);    //�ų���Ƶ�Ĳ������� -- ��Ҫˢ����
*           return CDialog::OnEraseBkgnd(pDC);
*       1.5.��������� WM_MOVE ��Ϣ�������ݵ�VideoWindow��(���ʹ��Ӳ��overlayʱ,ʹ��Render����Overlayλ�ã��������?)
*            IVideoWindow::NotifyOwnerMessage((OAHWND)hWnd, msg, wParam, lParam);
*       1.6.�����Ի����WM_SIZE��Ϣ��������Ƶ���ڵĴ�С��λ�ã���ͨ�� SetWindowPosition ����(��ʼ��ʱ��Ҫֱ������һ��?)
*     2.ʹ�� IVideoFrameStep ����(���� CanStep �������м��)�����봦����ͣ״̬��(Paused)���ܹ���
*     3.ֹͣ���ź�(IMediaControl::Stop)���� IVideoWindow ���� put_Visible(OAFALSE); put_Owner(NULL); ���ͷŴ��壻
*     4.����Ϣѭ���н���Ϣ����Video�����Ա��䴦��һЩϵͳ��Ϣ(�� PlayWnd Sample)
*       //Pass this message to the video window for notification of system changes
*       IVideoWindow::NotifyOwnerMessage((LONG_PTR) hWnd, message, wParam, lParam); 
*     5.����λ�õ���
*       REFERENCE_TIME rtNew = (g_rtTotalTime * dwPosition) / 100;
*       hr = g_pMediaSeeking->SetPositions(&rtNew, AM_SEEKING_AbsolutePositioning, NULL,AM_SEEKING_NoPositioning);
*     6.ͼƬץȡ
*       6.1. ʹ�� CFDirectShowUtility::SnapShotBitmap ���������ʹ�õ��Ǵ�ͳ��VideoRenderer��ץͼ�ǲ��ɿ���(ʹ����
*            DirectDraw���٣�������Ϊ����)�����봦����ͣ״̬�������ʹ�õ���VMR����û�и����ơ�
*       6.2. ʹ��Sample�е�GrabbleFilter��ץ���е�IMediaSample -- ע����治Ҫ���ӱ�׼��VideoRenderer��Grabble��
*            TransInPlace����������Pinʹ����ͬ�����ݻ��棬����λ���Դ��ϣ����Դ��ȡ���ݱ�ֱ�Ӵ����������ܶࡣ
*       6.3. ��ץȥ������ֱʹ��GDI������ʾ����
*            VIDEOINFOHEADER* pVideoInfoHeader = ...;
*            SetDIBitsToDevice(hdc, 0,0,
*              pVideoInfoHeader->bmiHeader.biWidth,
*              pVideoInfoHeader->bmiHeader.biHeight,
*              0,0,0,
*              pVideoInfoHeader->bmiHeader.biHeight,
*              (BITMAPINFO*)&pVideoInfoHeader->bmiHeader,
*              DIB_RGB_COLORS);
*
* �¼�֪ͨ����(Event Notification) -- ��FilterGraphManager�����ת����Ӧ�ó�����ʹ��IMediaEvent(Ex)�ӿ�
*   FilterGraphManagerʹ��һ����Ϣ������֪ͨӦ�ó�������Filter Graph�е��¼�?
*   �¼��������̣�
*     1.Filter�ڷ��������¼�ʱ����FilterGraphManager����һ���¼� -- ʹ�� CBaseFilter::NotifyEvent
*     2.FGManager��һЩ������¼���Ĭ�ϵĴ�����(����ʹ��IMediaEvent::CancelDefaultHandlingȡ��Ĭ�ϴ���),
*       �����ŵ��¼������У��ȴ��ϲ�Ӧ�ó�����
*     3.Ӧ�ó����֪���¼�������(ͨ���Զ�����Ϣ���¼�ͬ������Event)��ʹ��GetEvent�ȷ�����ȡ�¼��������д���
*       if(m_pME) //����ʹ�ö��У����첽����ʽ���������˳�ʱ���յ���Ϣ����˴���ǰ��Ҫ���ж�ָ���Ƿ���Ч
*       {
*         while(SUCCEEDED(m_pME->GetEvent(&evCode,...)) //ѭ���������е���Ϣ
*         {
*           switch(evCode){
*             case EC_COMPLETE: Clip������ϵ��¼���������ֱ�ӿ���FilterGraph��״̬��ͨ���� IMediaControl::Stop
*                  �����ͨ�� PostMessage �ķ�ʽת����Ϣ������
*             case EC_ERRORABORT: ������(����̿ռ���)
*             case EC_DEVICE_LOST: �Ȳ���豸(��USB��1394��)����(lParam2Ϊ0)�����½���(lParam2Ϊ1)ϵͳ��
*                  ȫ���豸����ʱ��Ҫʹ�� WM_DEVICECHANGE ��Ϣ��Ӧ
*             case EC_REPAINT: ��Ƶ����Ҫ���ػ���ǰͼ��֡
*             case EC_USERABORT: 
*             case ...
*           }
*           m_pME->FreeEventParams(...); //�ͷŹ�������Դ
*         }
*       }
*   ʹ���Զ������Ϣ��ʽ
*     1.�Զ�����Ϣ����  #define WM_GRAPHNOTIFY  (WM_USER+100)
*     2.ʹ�� IMediaEventEx::SetNotifyWindow((OAHWND)m_hWnd, WM_GRAPHNOTIFY, 0); ע�ᴰ�����Ϣ
*     3.����Ϣѭ���� WM_GRAPHNOTIFY ��������У���ȡ��������Ϣ -- ON_MESSAGE(WM_GRAPHNOTIFY...)
*     4.�ͷŽӿ�ǰ����סʹ�� pME->SetNotifyWindow((OAHWND)NULL, 0, 0) ȡ�������ע��
*   ʹ���¼�ͬ������ -- FGManager�ڲ��������¼�ͬ�����󣬵��¼�����������δ������¼�ʱΪ��Ч״̬
*     1.ʹ�� IMediaEvent::GetEventHandle((OAEVENT*)&hEvent) �õ��¼�ͬ������ľ��;
*     2.ʹ�� WaitFor �����ؽ��еȴ�
*   ѭ���ȴ�
*     1.ʹ�� IMediaEvent::WaitForCompletion(�ȴ�ʱ��, ����ֵ) -- ��������᷵�� EC_COMPLETE
*
****************************************************************************************************/

// File filter for OpenFile dialog
#define FILE_FILTER_TEXT \
    TEXT("Video Files (*.avi; *.qt; *.mov; *.mpg; *.mpeg; *.m1v)\0*.avi; *.qt; *.mov; *.mpg; *.mpeg; *.m1v\0")\
    TEXT("Windows Media Files (*.asf,*.wmv,*.wma)\0*.*.asf,*.wmv,*.wma\0")\
    TEXT("Audio files (*.wav; *.mpa; *.mp2; *.mp3; *.au; *.aif; *.aiff; *.snd)\0*.wav; *.mpa; *.mp2; *.mp3; *.au; *.aif; *.aiff; *.snd\0")\
    TEXT("MIDI Files (*.mid, *.midi, *.rmi)\0*.mid; *.midi; *.rmi\0") \
    TEXT("Image Files (*.jpg, *.bmp, *.gif, *.tga)\0*.jpg; *.bmp; *.gif; *.tga\0") \
    TEXT("All Files (*.*)\0*.*;\0\0")

#endif//DIRECT_SHOW_PLAYER_H