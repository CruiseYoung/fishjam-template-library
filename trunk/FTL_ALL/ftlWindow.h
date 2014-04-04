#ifndef FTL_WINDOW_H
#define FTL_WINDOW_H
#pragma once

#ifndef FTL_BASE_H
#  error ftlwindow.h requires ftlbase.h to be included first
#endif

//#include <Wtsapi32.h>
//#pragma comment(lib, "Wtsapi32.lib")

/*************************************************************************************************************************
* �Ѷ�����
*   winui\fulldrag -- (KBQ121541), ������ƱȽϻ���ʱ�䣬���� WM_ENTERSIZEMOVE -> WM_EXITSIZEMOVE ʱ�򻺳壬
*     ��ֹƵ����������СʱƵ�����ƶ���˸��������������ɴ�����ʱ���ֱ���ɫ�Ŀհ׵�Bug(���Կ���ͨ�� CFCanvas ���������Ļ�ͼ��� ?)
*   winui\shell\appplatform\DragDropVisuals -- ???, ֻ����Vista���ƽ̨����
*
* ΢��δ�����ĺ����б� http://undoc.airesoft.co.uk/index.php
*************************************************************************************************************************/

//�����б����Զ������ݣ�����ͨ��  _SetWindowLongPtr(hDlg, DWLP_USER, (this)); �ķ�����thisָ�봫��

//CWndClassInfo & MagnifierLayer::GetWndClassInfo()
//{
//	//
//	// ATL Internals pp 419
//	//
//	static CWndClassInfo wc =
//	{
//		{
//			sizeof( WNDCLASSEX ),
//				0,
//				StartWindowProc,
//				0,
//				0,
//				NULL,
//				NULL,
//				NULL,
//				(HBRUSH)GetStockObject(NULL_BRUSH),
//				NULL,
//				_T( "TestClassInfo"),
//				NULL
//		},
//		NULL,
//		NULL,
//		IDC_ARROW,
//		TRUE,
//		0,
//		_T( "TestClassInfo")
//	};
//
//	return wc;
//}

/******************************************************************************************************
* Monitor -- MonitorFromPoint ��ϵͳ��һ�� multimon.h �ļ�
* 
* �������� -- Child���и�����,����Onwer���ڡ�Overlapped/Popup������Onwer����,һ����Parent����
*     Combobox�������б��listbox��Parent��NULL(����ʾ��Dialog��)����Owner��Combobox�ĵ�һ�����Ӵ���Owner(��Dialog�������ڹ���)
*   Child   -- WS_CHILD�� ���Ӵ��ڿ�����һ���ص�����,��������һ���������ڣ�����ע�⣺Child����û��ά����Owner����(���Զ���Ϊ��Child��ParentΪ��Owner)
*              �Ӵ����ڸ���������ǰ������,�ڸ���������ǰ������,�ڸ�������ʾ����ʾ
*   Owner   -- �����ߣ����������ڿ���(����)��Owner������Overlapped��Popup(WS_CHILD������Owner����)����ӵ�е���ʾ��Owner��ǰ�棬
*              Onwer��С��ʱ����ӵ�еĴ��ڶ������أ�������Ownerʱ����Ӱ��ӵ���ߵĿɼ�״̬(�� A > B > C, �� A ��С��ʱB�����أ���C�Կɼ�)
*                ���� ��Owner��������ʱ��������Owned���ڲ������ء�����Owner��С��������Owned���ڻᱻ���ء�
*              ����ʱֻ��( GetWindow(GW_OWNER) )����Child���ڵ��øú����᷵��NULL(�����ڼ�Owner?)��MFC����Ȼ��CWnd::SetOwner����ֻ���������Ա���� m_hWndOwner
*              CreateWindow�����е�hWndParent����Child����Ҳ���Ϊ��Owner������ݹ������ң�GetWindowLong(GWL_HWNDPARENT) �õ������hWndParent����
*   Parent  -- �����ڣ�������ʾ(left/top ������ϵͳ)��Parent�ƶ�ʱ�Ӵ���Ҳ���ƶ���Parent����ʱ�����Ӵ���Ҳ�����أ�
*              ��������С��ʱ���Ӵ���Ҳ����С��������WS_VISIBLE���Բ��䣻
*              ��ͨ�� GetParent/SetParent ��ȡ��̬�ı䣬������ΪNULL(���棬����Top-Level���ڵ�Parent����NULL)��HWND_MESSAGE(ֻ������Ϣ�Ĵ���message-only)
*              ͨ�����֪ͨ��Ϣ���͸�Parent(��Toolbar�Ƿ��͸�Owner)��
*              MSDN����Ȼ˵����SetParentֻ����ͬһ�����н��У���ʵ���Ͽ��Զ���̵�HWND�ϵ��ã���IE�����ģʽʱ�������ͨ��IEBarHost�����һ�𣻵�Chrome����������ʾUI�������ӽ���ֻ���̣߳�
*      ��Child����ΪTop-Level: SetParent(NULL); ModifyStyle(WS_CHILD, WS_POPUP, 0); ModifyStyleEx(0, WS_EX_TOPMOST | WS_EX_TOOLWINDOW, 0);
*      ��Top-Level����ΪChild: ModifyStyle(WS_POPUP, WS_CHILD); SetParent(hWndNewParent);
*   Sibling -- 
*   
* ��������
*   Desktop <== Ψһ�����洰��
*    +- top-level <== ���з�child����������NULL(desktop)�Ĵ��ڣ���ӵ������top-level������top-level��ӵ��
*      +- Overlapped <== �ص����ڣ��б������ͱ߿�һ����������������ڣ����ص����ڿ�����Child���ڣ���
*        +- Child <== 
*      +- Popup <== ��WS_POPUP�Ĵ��ڣ�����ʵ����Ļ�κεط���һ��û�и����ڣ�����û�б�����
*
******************************************************************************************************/

/******************************************************************************************************
* CHAIN_MSG_MAP(__super)
* 
* Vista/Win7
*   Aero��(CPropertySheetImpl<>, CPropertyPageImpl<>)
*     ������ҳ��Ϊ������ʽ����:  m_psh.dwFlags |= PSH_WIZARD97; Areo �� PSH_AEROWIZARD
*     ��Ϣ PSM_SHOWWIZBUTTONS -- ��ʾ���������еı�׼��ť�� �� PropSheet_ShowWizButtons ������
*          PSM_ENABLEWIZBUTTONS -- ���û����ĳ����׼��ť���� PropSheet_EnableWizButtons ������
*          PSM_SETBUTTONTEXT -- �޸İ�ť�ϵ����֣��� PropSheet_SetButtonText  ������
* ����Ի���(TaskDialog/TaskDialogIndirect) -- 
*   TASKDIALOGCONFIG::pfCallback �ص�������������Ӧ����Ի������������¼�
*     ֪ͨ˳��: TDN_DIALOG_CONSTRUCTED -> TDN_CREATED 
*   ��Ϣ��TDM_SET_ELEMENT_TEXT -- ��������Ի����Ͽؼ����ı�
*         TDM_SET_BUTTON_ELEVATION_REQUIRED_STATE -- ����������ʾ��UAC����ͼ��
*   Flags
*     TDF_USE_COMMAND_LINKS -- ���Զ��尴ť��ʾΪ��������(���ܿ��Ʊ�׼��ťTDCBF_OK_BUTTON��)
*     TDF_SHOW_PROGRESS_BAR -- ��ʾ������
*     ��TDF_SHOW_MARQUEE_PROGRESS_BAR -- ��ʾ�������ʽ(��ͣ�Ĵ�����)�Ľ�����
*   ������()
*     TDM_SET_PROGRESS_BAR_RANGE -- ָ����������ָʾ��Χ����Ϣ
*     TDM_SET_PROGRESS_BAR_POS -- ָ����������ָʾ��Χ�е�λ��
*     TDM_SET_PROGRESS_BAR_STATE -- �ı��������״̬
*     
* 
* UAC(User Account Control)
*   Button_SetElevationRequiredState --������Ч?
*
* DWM(Desktop Window Manager,���ڹ�����) -- ������������ϵĸ�������, ������������ĳ���������������������/�ص�ʱ����ʾЧ����
*   ��������ʵ�֡���͸������(Glass)����Ч��������ƴ��巶Χ�ڲ��������͸����)
*     ��������(Window Region) -- ָ����ϵͳ�����������н��л��Ƶ����򣬳����л���Basic���⣬����Vista�Ѳ���ʹ��
*     ����ϳ�(Desktop Composition) -- DWM���ṩ��һ�����ܣ�����ʵ�����粣����3D���ڱ任���Ӿ�Ч����
*       ����ʱ��DWMĬ�Ͻ��Ѵ���ķǿͻ������Բ���Ч�����֣����ͻ�����Ĭ��Ϊ��͸����
*       DwmIsCompositionEnabled -- �ж��Ƿ������˺ϳ�Ч��
*       DwmEnableComposition -- ��ʱ����/��������ϳɹ��ܣ�����Ҫ����ԱȨ�ޣ������˳�ʱ�Զ��ָ�
*       DwmGetColorizationColor -- ��⵽�ϳ�Ч���ǰ�͸���Ļ��ǲ�͸���ģ��Լ��ϳ���ɫ
*       DwmEnableBlurBehindWindow -- �ÿͻ�������ȫ��ĳ����ʵ�ֲ���Ч��
*       DwmExtendFrameIntoClientArea -- ���ÿ��(Window Frame)��ͻ�����չ
*       DwmSetWindowAttribute  -- ����DWM���������,�� ���� Flip3D����С��ʱ�Ķ���Ч��
*         MARGINS margins={-1}; -- �������չΪ�����ͻ��������ɽ������ͻ�����ͷǿͻ�������Ϊһ���޷�����������ʾ(�粣��Ч��)
*     ����Ч��(aurora effect) -- 
*     Flip3D(Win+Tab) -- 
*     ����������ͼ�Զ�ͬ�� -- DwmRegisterThumbnail��DwmUpdateThumbnailProperties
* 
* RGB --  0x00BBGGRR
* Gdiplus::ARGB -- 0xAARRGGBB  <== ע�⣺��ɫ˳���RGB���෴
* 
******************************************************************************************************/

/******************************************************************************************************
* WinProc ����ֵ��
*   0 -- �û��Ѿ��������Ϣ���� WM_CLOSE �з���0�򲻻�رգ����� DefaultWindowProc �Źر�(WM_DESTROY)
*
* �������ڵ�˳��PreCreateWindow -> PreSubclassWindow -> OnGetMinMaxInfo -> OnNcCreate -> OnNcCalcSize -> OnCreate
*   -> OnSize -> OnMove -> OnChildNotify
* �رմ��ڵ�˳��OnClose -> OnDestory -> OnNcDestroy -> PostNcDestory
*
* ����ʵ�����ĸ���ϵ���ڵľ��:
*   1.�����ڵ� Z-Order ����Ӵ��ھ�� <== GetTopWindow
*   2.�����ڵ���һ�ֵܴ��ھ�� <== GetNextWindow
*   3.�����ڵĸ����ڵľ��   <== GetParent
*   4.�����ڵ������ߴ��ھ�� <== GetWindow(GW_OWNER)
*
* SendMessage ��ִ������UI�߳�(��̨�̻߳�ȴ���ֱ��UI�߳�ִ�����)
*   ����յ�Send��Ϣʱ��UI�̻߳�����Ϣ��������,��ﲻ������ǿ������ִ�С�,��Ҫ�ȴ�ִ���������
*   ��ͨ��GetMessage�����ȡ����ִ�С�
*   WTL�����ӳ������:
*     Sendʱ:_GetMessage -> _DispatchClientMessage -> _InternalCallWinProc
*     Postʱ:_DispatchMessage -> DispatchMessageWorker -> _InternalCallWinProc
* 
* DefaultWindowProc -- Ĭ����Ϣ������
*
* ע�⣺�ı䴰���Сʱ������������ WM_SIZE + WM_PAINT ��Ϣ���������������������˸
* 
* �Ի�ؼ���OWNERDRAW���ڴ���ʱ���� WM_MEASUREITEM ��Ϣ���ı����ʱ���� WM_DRAWITEM ��Ϣ
* 
* ����ȫ������( WS_POPUP����, 0,0 ~ CxScreen, CyScreen )��
*   hWnd = CreateWindow(szWindowClass, szTitle, WS_POPUP | WS_VISIBLE, 0, 0, 
*     GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), NULL, NULL, hInstance, NULL);
*
* ��ȡ����Ҽ�������λ�ã�����ʾ�����˵�(����󷵻�)
*   TrackPopupMenu(hPopMenu, TPM_RIGHTBUTTON | TPM_TOPALIGN | TPM_LEFTALIGN, LOWORD(lParam), HIWORD(lParam) , 0, hwnd, NULL);
*
* �� FormView �����ý�����»��ƣ�
*   1.���� <afxpriv.h>��
*   2.ӳ�䴦����Ϣ WM_IDLEUPDATECMDUI (�Ի�������Ҫ���� WM_KICKIDLE )
*   3.��ʵ���е��� UpdateDialogControls
*
* AdjustWindowRectEx -- ���ݿͻ����Ĵ�С�ʹ�����ʽ�����㲢�������ڵ������ߴ�,Ȼ��MoveWindow���е�������
*   RECT rcClient = { 0,0,800,600 };
*   AdjustWindowRectEx( &rcClient, GetWindowStyle(hWnd), GetMenu(hWnd) != NULL, GetWindowExStyle(hWnd));
* MapWindowPoints -- �������һ�����ڵ�����ռ��һ���ӳ����������һ���ڵ�����ռ��һ���
*
* �������� -- ��������MapMode��ViewportExt/WindowExt, ViewportOrg/WindowOrg ��ʵ��
*   CScrollView ���� m_totalDev, m_pageDev �Ȳ���
*   CScrollView �� SetScrollSize ���߼�����
*
* �ͻ����϶�
*   1.OnLButtonDown �У�SendMessage ( m_hWnd, WM_NCLBUTTONDOWN, HTCAPTION, 0 ); return 0;
*
* ShowWindow 
*   SW_HIDE -- ���أ�ʧ��
*   SW_MINIMIZE -- ��С����ʧ��
*   SW_RESTORE -- ����ָ�ԭ��ʾ
*   SW_SHOW -- �����ʾ
*   SW_SHOWMAXIMIZED -- ������
*   SW_SHOWMINIMIZED -- ���ͼ�껯
*   SW_SHOWMINNOACTIVE -- ͼ�껯���ǲ�����
*   SW_SHOWNA -- ��ʾ������ԭ����״̬
*   SW_SHOWNOACTIVATE -- ������Ĵ�С��λ����ʾ������ԭ����״̬
*   SW_SHOWNORMAL -- �����ʾ����Ҫʱ�������С���лָ�
*
* ShowOwnedPopups -- ���û���ɾ����ǰ������ӵ�е����д��ڵ�WS_VISIBLE���ԣ�Ȼ���� WM_SHOWWINDOW ��Ϣ���´�����ʾ
*   �ô�����������Owned���ڵ���������С����Ownerʱ -- ��Ϊʲô��ֱ����С����ShowWindow(SW_HIDE) ?
*
* ������Ϣ�� ( ��Ϣ + WM_REFLECT_BASE , ��Ӧ����Ϣӳ����� ON_WM_XXXX_REFLECT )
*  WM_COMMAND -- ��Ϣ�Ĳ����м��з�����Ϣ�Ŀؼ��� ID�����пؼ��� HWND������֪ͨ����
*  WM_NOTIFY  -- ��Ϣ�� WM_COMMAND �Ĳ���֮�⻹��һ��ָ�� NMHDR ���ݽṹ��ָ��
*  WM_MEASUREITEM -- 
******************************************************************************************************/

/******************************************************************************************************
* ListCtrl -- �б�ؼ�
*   LVS_OWNERDATA -- ���б���  //http://download.csdn.net/source/236507
*     ���� LVN_GETDISPINFO ֪ͨ��Ϣ�������Ҫ��ʾ������
*     ���� NM_CUSTOMDRAW ֪ͨ (�������д��� ?),�� pResult ��ֵ
*   LVIS_OVERLAYMASK -- ��̬��ȡ�б����Overlayͼ��(����TortoiseSVN��Shell�еĸ���״̬ͼ��?),INDEXTOOVERLAYMASK ���� IShellIconOverlay ��ȡ������?
* RichEdit -- CWindowImple<CEditView, CRichEditCtrl>
*   BitBlt �� HDC �У���ΪͼƬ
*   StreamOutRtf -- ��Buffer�г־û�
*
* SliderCtrl -- ����ؼ�,�����Դ���ʱ��ָ���������ƶ�
*   �������ԡ�����:
*     Buddy -- ��������
*     LineSize -- ��Ӧ���̵ķ����
*     PageSize -- ��ӦPageUp��PageDown��
*     Selection-- ѡ��Χ��������ʾ
*     ChannelRect -- �ڿ̶�֮�ϣ���Thumb�ƶ�������
*     ThumbRect -- ��Ĵָ��(ͨ������϶����Ǹ�С����)�ķ�Χ
*     Tic/TicFreq -- ��ȡ��������ʾ�̶ȵ�λ��/���
*   ���:
*     TBS_AUTOTICKS -- ���������п̶�(?)
*     TBS_NOTICKS -- �����������п̶�(?)
*     TBS_ENABLESELRANGE -- ����ѡ��Χ
*   Ҫ���������仯���¼�����Ҫ�����崦�� WM_HSCROLL/WM_VSCROLL ��Ϣ(��Ϊ��ˮƽ����������ͬһ��OnXScroll����,
*     ���Բ����е�ָ�����������ΪCScrollBar*���ͣ���Ҫת��ΪCSliderCtrl*)
*     TRBN_THUMBPOSCHANGING Notify �¼�������
*     
******************************************************************************************************/

/******************************************************************************************************
* ��Դ�ļ��� RC -> RES�����巽ʽͨ������  ID ���� �ļ�������Դ��    ��ע�⣺ֻ����һ��STRINGTABLE��û�����֣�
*   CURSOR -- ���
*   DIALOGEX --
*   ICON --
*   MENU -- DISCARDABLEָʾʲô������������
*   STRINGTABLE -- ÿ��RC�ļ���ֻ����һ���ַ�����ÿ���ַ������ܳ���255���ַ�
*   WAVE -- ������ʹ�� PlaySound ���ţ�SND_PURGE(ֹͣ����)
******************************************************************************************************/

/******************************************************************************************************
* �ֲ㴰��(WS_EX_LAYERED) -- ������ƴ����͸���ȡ��ֲ㴰���ṩ�����ֽ�Ȼ��ͬ�ı��ģ��
*   WS_EX_LAYERED ��չ���ڷ�� -- ���彫�߱�������״��������Alpha��ϵȷ�����Ӿ���Ч
*     ע�⣺��������Child���壬Ҳ���������� CS_OWNDC or CS_CLASSDC ���ԵĴ��塣
*     ͸�������Ϊ255�����Ҫ����͸���� X%, bAlpha = X * 255 / 100;
*   SetLayeredWindowAttributes(��) -- ��������һ��RGB��ɫ(ͨ���Ǵ����в�����ֵ���ɫ)��Ȼ�������Ը���ɫ��������ض�������Ϊ͸��
*     SetLayeredWindowAttributes( 0, 150, LWA_ALPHA);  //����͸����Ϊ150(��������͸��,�ӿؼ�Ҳ͸��)
*     SetLayeredWindowAttributes( RGB(240,240,240), 0, LWA_COLORKEY); //��������͸��,�ӿؼ���͸�� 
*       ����ָ����ɫ�Ĳ���͸��(Dialog������ɫ),��Ҫ����������ɫ����Ҫ�� OnCtlColor �� WM_ERASEBKGND ��ָ����ɫ
*     TODO: ���ʹ�� LWA_ALPHA �� LWA_COLORKEY ?
*       ::SetClassLongPtr(m_hWnd, GCL_HBRBACKGROUND, (LONG_PTR)GetStockObject(BLACK_BRUSH));
*       SetLayeredWindowAttributes(m_hWnd, RGB(0xFF, 0, 0), 100, LWA_ALPHA | LWA_COLORKEY); //����͸����Ϊ 100
*   UpdateLayeredWindow(����) -- �ṩһ�����豸�޹ص�λͼ������������Ļ�ϴ����������ʽ���Ὣָ����λͼ�����ر���Alphaͨ����Ϣ��������������
*     ::UpdateLayeredWindow( m_hWnd, NULL, &ptDst, &WndSize, dcMem.m_hDC, &ptSrc, 0, &blendPixelFunction, ULW_ALPHA );
*     ���ִ��岻֧���ӿؼ�����֧��OnPaint()��������ͨ��PNGͼƬ�е�Alphaֵ�� ��ȫ������Ļ�ϴ����͸�����
*
*   �ֲ㴰������ʵ����������Ȼ��ͬ�ĸ���ֲ���ض���
*     1. ���� WS_EX_LAYERED ����;
*          class ScreenRgnSelector : public CWindowImpl<ScreenRgnSelector, CWindow, 
*              CWinTraits<WS_VISIBLE | WS_POPUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, WS_EX_TOPMOST | WS_EX_LAYERED | WS_EX_TOOLWINDOW> >
*     2. ͨ�� UpdateLayeredWindow ���������·ֲ㴰�� -- ��Ҫ��λͼ�л��Ƴ��������򣬲�������ؼ�ɫ��
*        Alpha��ϲ�����һ���ṩ�� UpdateLayeredWindow ����,��ʱӦ�ó��򲢲���Ҫ��ӦWM_PAINT������������Ϣ.
*     ��: ʹ�ô�ͳ��Win32���ƻ��� -- ���� SetLayeredWindowAttributes ��ɶԹؼ�ɫ(COLORREF crKey)�򰢶���(BYTE bAlpha)��ϲ���ֵ���趨,
*         ֮��ϵͳ����ʼΪ�ֲ㴰���ض������еĻ��Ʋ��Զ�Ӧ��ָ������Ч
*   ��͸������:(#define _WIN32_WINNT 0x0501)
*     SetWindowLong(m_hWnd,GWL_EXSTYLE,GetWindowLong(m_hWnd,GWL_EXSTYLE) | WS_EX_LAYERED );
*     //���Ի���Ĵ���(��ɫΪCOLOR_BTNFACE�ĵط�)��Ϊ͸�����Ҳ��ٽ��е�����,������͸��
*     SetLayeredWindowAttributes(GetSysColor(COLOR_BTNFACE),127,LWA_COLORKEY|LWA_ALPHA); 
*     //RedrawWindow(NULL, NULL, RDW_ERASE | RDW_INVALIDATE | RDW_FRAME | RDW_ALLCHILDREN);
*   ���δ��� -- �༭�������ض�KeyColor��ͼƬ,ʹ�� SetLayeredWindowAttributes ����KeyColor
*     ͨ�����������غ��ҷֱ�ʹ�� SetLayeredXXX(,LWA_COLORKEY) �� UpdateXXX �ķ������ṩ���δ���
* 
*   ��ӰЧ�� -- http://www.codeproject.com/Articles/16362/Bring-your-frame-window-a-shadow
*   
*   ����Բ�δ��壺��Windows 9x�µ���ȷ������ͨ�� SetWindowRgn ����ָ����Ҫ�Ĵ�����״���������ִ�����Ƶ�����Ĵ�����״
*     ��������Ļ����קʱ����ȱ�ݴ��ڣ�ǰ̨���彫Ҫ��λ�����µĴ����ػ����������⽫���������Ϣ�ͼ�������
*     ����ʹ�� SetWindowRgn ֻ��ʵ�ִ����ȫ͸�����޷�ʵ�ְ�͸��Ч����
*  
* ����Ϣ����(Message-Only Window)
*   �����շ���Ϣ�������ɼ�(���ܽ��ܼ��̺������Ϣ)��û��Z�򣬲��ܱ�ö�٣����ܽ��չ㲥(broadcast)��Ϣ��
*   ������������ CreateWindowEx �����У�ָ�� hWndParent ����Ϊ HWND_MESSAGE ����
*             �� SetParent �����У�ָ�� hWndParent ����Ϊ HWND_MESSAGE ���� �����ڵĴ���ת��Ϊ����Ϣ����
*   �Ҵ��壺FindWindowEx(HWND_MESSAGE,xxxx);
*
* MoveWindow
*  WM_WINDOWPOSCHANGING => WM_WINDOWPOSCHANGED => WM_MOVE=> WM_SIZE => WM_NCCALCSIZE
* SetWindowPos -- �ı�һ�����ڵĳߴ磬λ�ú�Z��
*   WM_SYNCPAINT(!SWP_DEFERERASE)
*   WM_NCCALCSIZE(SWP_FRAMECHANGED)
*   WM_WINDOWPOSCHANGING(!SWP_NOSENDCHANGING)
*   WM_WINDOWPOSCHANGED 
* BeginDeferWindowPos/DeferWindowPos/EndDeferWindowPos -- һ�����ƶ��������(�μ� GraphChat.sln �е� ResizeMainWindow)
* SetForegroundWindow
*
* Z��
*   ����(Topmost) => ����(��ͨ) => �Ӵ���(�󴴽�������)
*   BringWindowToTop -- ͬ��Ķ���
******************************************************************************************************/

/******************************************************************************************************
* ģ�ⰴ�� Paste ( Ctrl + V )
*   keybd_event(VK_CONTROL, MapVirtualKey(VK_CONTROL, 0), 0, 0);
*   keybd_event('V', MapVirtualKey('V', 0), 0, 0);
*   keybd_event('V', MapVirtualKey('V', 0), KEYEVENTF_KEYUP, 0);
*   keybd_event(VK_CONTROL, MapVirtualKey(VK_CONTROL, 0), KEYEVENTF_KEYUP, 0);
* ���ģ�� mouse_event
*
* ���λ�ü��(HitTest), ϵͳ�Ѿ������� HTOBJECT��HTCLIENT �Ⱥ�
******************************************************************************************************/

/******************************************************************************************************
* ͨ���Ϸ�(Drag & Drop)֧��(��ԭʼ��ͨ�õ���ʹ�� IDropTarget �ӿ�) http://blog.csdn.net/vcbear/article/details/5990
*   DoDragDrop -- ϵͳAPI�������Ϸŵ��������̣�һ�㲻��ֱ�ӵ���(DUI�п��Գ���ֱ�ӵ���)�� ��ĳ����Դ�����ݱ��϶���ʱ��ͱ����á�
*   RegisterDragDrop(HWND, IDropTarget*) -- ����ϵͳHWnd���ڿ��Խ����Ϸţ�ע�⣺����ǰ��Ҫͨ�� CoLockObjectExternal ������
*     ->
*   RevokeDragDrop()  
*
*   FORMATETC -- OLE���ݽ����Ĺؼ��ṹ�����豸�����ݣ������ý�����˸�ʽ�ϵ�������
*     �磺 FORMATETC cFmt = {(CLIPFORMAT) CF_TEXT, NULL, DVASPECT_CONTENT, -1, TYMED_HGLOBAL};
*                        �� {CF_BITMAP(�������ʽ��λͼ), NULL(Ŀ���豸����Ļ), DVASPECT_CONTENT(Level of detail is Full content), -1, TYMED_GDI(Storage medium��λͼ���) };
*   STGMEDIUM -- ���� tymed ��Ա����ȷ����һϵ�о�������ݶ���ӿڵ�����
*     �磺STGMEDIUM stgm = { TYMED_GDI, hBitmap , NULL(��ʾʹ�����ͨ�� ReleaseStgMedium �ͷ�) };
*   
*   IDropSource -- D&Dʱ������Դ��?�����Ϸ�ʱ�� IDataObject ?
*     QueryContinueDrag -- 
*     GiveFeedback -- 
*   IDropTarget -- D&Dʱ�Ľ��շ��ӿڣ�ϵͳ���ں��ʵ�ʱ�����ε��ö�Ӧ����������û�����Щ�����ﷵ�صı�־(DROPEFFECT_COPY ��)�����������۱��ֺ��ϷŽ��
*     DragEnter -- �϶�������ν��봰��ʱ���ã��ж��Ƿ�֧��ĳ�������͵��Ϸţ������� grfKeyState(���/����) ���� pdwEffect
*     DragOver -- ���ڷ�Χ���ƶ�ʱ����
*     DragLeave -- �϶������뿪��������ʱ���ã�ͨ������������Դ
*     Drop -- �ڴ������ͷ��϶�����ʵ�֡��š����߼�
*   IDropSourceNotify -- 
*     DragEnterTarget
*     DragLeaveTarget
*   IDataObject -- ��������, RichEdit ��Ҳ���Բ���ýӿڷ�װ��ͼƬ��(�ɴ� IDataObjectImpl �̳����� CImageDataObject)
*     GetData/QueryGetData/SetData
*     GetDataHere
*     GetCanonicalFormatEtc
*     EnumFormatEtc -- ö�ٰ�����������������(IEnumFORMATETC)����Ϊ GET �� SET ����
*     DAdvise/DUnadvise/EnumDAdvise
* 
* ע��(δ����ȷ��)��
*   1.Ҫ����OleInitialize������CoInitialize��CoInitializeEx��COM���г�ʼ������RegisterDragDrop������ɹ������صĴ�����E_OUTOFMEMORY
*   2.����ReleaseStgMedium�ͷ�STGMEDIUM������ݣ�������ֱ�Ӷ���hGlobal��Ա����CloseHandle
*   3.�ϷŲ�����ϵ���������̵����ݽ������Ὣ�������̶�������ֱ���Ϸ����Ϊֹ�����ԣ��ڽӹ��ϷŵĽӿڷ����У���Ҫ���й��ں�ʱ������
*   4.���ֻ��ͨ�� Shell �Ϸ��ļ������򵥵ķ�������Ӧ WM_DROPFILES ��Ϣ
******************************************************************************************************/

#ifndef WM_SYSTIMER
#  define WM_SYSTIMER 0x0118		//UnDocument Message(caret blink) , afxtrace.cpp!_AfxTraceMsg()���Կ�������
#endif 

const UINT DEAFULT_DUMP_FILTER_MESSAGES[] = {
	WM_MOUSEMOVE,
	WM_SETCURSOR,

	WM_NCMOUSEMOVE,
	WM_NCHITTEST,
	WM_NCACTIVATE,

	WM_PAINT,
	WM_ERASEBKGND,
    WM_NCPAINT,

	WM_TIMER,
	WM_SYSTIMER,

	WM_ENTERIDLE,
#ifndef WM_KICKIDLE
#  define WM_KICKIDLE	0x036A
#endif 
	WM_KICKIDLE,
	WM_CTLCOLORMSGBOX,
	WM_CTLCOLOREDIT,
	WM_CTLCOLORLISTBOX,
	WM_CTLCOLORBTN,
	WM_CTLCOLORDLG,
	WM_CTLCOLORSCROLLBAR,
	WM_CTLCOLORSTATIC,
	WM_PRINTCLIENT,

};

namespace FTL
{
	//ע��:
	//1.WTSRegisterSessionNotification ����ܽ��� WM_WTSSESSION_CHANGE
	//2.RegisterShellHookWindow ����Խ��� WM_SHELLHOOKMESSAGE

	//��Output��Dump����ǰ���ܵ�����Ϣ��ͨ����һ�������� __FILE__LINE__
#ifdef FTL_DEBUG
#  define DUMP_WINDOWS_MSG(pszName, pFilters, nCount, uMsg, wParam, lParam) \
    {\
        BOOL bFilterd = FALSE;\
        if(pFilters) \
        {\
		    for(int i = 0; i < nCount; i++){\
			    if(uMsg == *((UINT*)(pFilters + i))){\
				    bFilterd = TRUE;\
				    break;\
			    }\
		    }\
        }\
        if(!bFilterd)\
        {\
            FTLTRACE(TEXT("%s[%d](%d)%s, wParam=0x%x, lParam=0x%x, Tick=%d\n"),\
                pszName,GetCurrentThreadId(),uMsg, FTL::CFMessageInfo(uMsg, wParam, lParam).GetConvertedInfo(),\
		        wParam, lParam, GetTickCount() );\
        }\
    }
#else
#  define DUMP_WINDOWS_MSG(pszName, filters, nCount, uMsg, wParam, lParam)	__noop;
#endif 

	//ͨ�� RegisterWindowMessage ע�����Ϣ
	//���̰߳�ȫ
	class CFRegistedMessageInfo
	{
	public:
		FTLINLINE BOOL Init();

		FTLINLINE explicit CFRegistedMessageInfo();
		FTLINLINE virtual LPCTSTR GetMessageInfo(UINT msg, WPARAM wParam, LPARAM lParam);
	private:
		BOOL m_bInited;
		TCHAR m_bufInfo[128];

		UINT RWM_ATL_CREATE_OBJECT;		//ATL.CAtlAutoThreadModuleT.CreateInstance ����COM����ʱʹ��(�ڲ�ʹ�õ��׼��߳�ͬ��?)
		UINT RWM_ATLGETCONTROL;			//AtlAxGetControl ��ȡControl��IUnknown�ӿ�(����ֵ)
		UINT RWM_ATLGETHOST;			//AtlAxGetHost ��ȡHost��IUnknown�ӿ�(����ֵ��
		UINT RWM_COLOROKSTRING;         //_T("commdlg_ColorOK")
		UINT RWM_COMMDLG_FIND;
		UINT RWM_FILEOKSTRING;
		UINT RWM_FINDMSGSTRING;         //_T("commdlg_FindReplace")
		UINT RWM_LBSELCHSTRING;

		//zmouse.h �д��� MSWheel ��������Ϣ(��һ�� MouseZ ����)
		UINT RWM_MSH_MOUSEWHEEL;		//MouseWheel
		UINT RWM_MSH_WHEELSUPPORT;		//3DSupport
		UINT RWM_MSH_SCROLL_LINES;		//ScrollLines

		UINT RWM_HELPMSGSTRING;
		UINT RWM_HTML_GETOBJECT;		//��IE�����л�ȡ��Ӧ��IHTMLDocument2�ӿ�
		UINT RWM_SETRGBSTRING;          //_T("commdlg_ColorOK")
		UINT RWM_SHAREVISTRING;
		UINT WM_SHELLHOOKMESSAGE;		//RegisterShellHookWindow �����
		UINT RWM_TASKBARBUTTONCREATED;	//���������´���?(û��ȷ�ϣ��������ڳ�ʼ��Vista�� ITaskbarList3 �ӿ�?)
		UINT RWM_TASKBARCREATED; 

        UINT RWM_DRAGLISTMSGSTRING;
        
        //WTL
        UINT RWM_WTL_CMDBAR_INTERNALAUTOPOPUPMSG;   // _T("WTL_CmdBar_InternalAutoPopupMsg")
        UINT RWM_WTL_CMDBAR_INTERNALGETBARMSG;      // _T("WTL_CmdBar_InternalGetBarMsg")
        UINT RWM_WTL_GETEXTERIORPAGETITLEFONT;      // _T("GetExteriorPageTitleFont_531AF056-B8BE-4c4c-B786-AC608DF0DF12")
        UINT RWM_WTL_GETBULLETFONT;                 // _T("GetBulletFont_AD347D08-8F65-45ef-982E-6352E8218AD5")

        //WM_DEVICECHANGE; //�����Ϣ��Ҫͨ�� RegisterDeviceNotification ע����ܻ��?
	};

    //! ����Ϣ( WM_XXX )ת��Ϊ�׶��ĸ�ʽ�������� ",wm"
    FTLEXPORT class CFMessageInfo : public CFConvertInfoT<CFMessageInfo,UINT>
    {
    public:
        FTLINLINE explicit CFMessageInfo(UINT msg, WPARAM wParam, LPARAM lParam);
        FTLINLINE virtual LPCTSTR ConvertInfo();
    public:
        WPARAM m_wParam;
        LPARAM m_lParam;

	private:
		static CFRegistedMessageInfo	s_RegistedMessageInfo;
    };

    #define BEGIN_WINDOW_RESIZE_MAP(thisClass) \
        static const _WindowResizeMap* GetWindowResizeMap() \
        { \
            static const _WindowResizeMap theMap[] = \
            {

    #define END_WINDOW_RESIZE_MAP() \
                { (DWORD)(-1), 0 }, \
            }; \
            return theMap; \
        }

    #define WINDOW_RESIZE_CONTROL(id, flags) \
                { id, flags },

    //��ʼ����һ���飬֮�����Control��λ�ý���Groupǰ��һ��Control��λ��Ϊ��׼
    #define BEGIN_WINDOW_RESIZE_GROUP() \
                { -1, _WINSZ_BEGIN_GROUP },

    #define END_WINDOW_RESIZE_GROUP() \
                { -1, _WINSZ_END_GROUP },


    FTLEXPORT template <typename T>
    class CFMFCDlgAutoSizeTraits
    {
    public:
        static HWND GetWinHwndProxy(T* pThis)
        {
            return pThis->GetSafeHwnd();
        }
        static DWORD GetStatusBarCtrlID()
        {
            return AFX_IDW_STATUS_BAR;
        }
        //static LRESULT OnSizeProxy(T* pThis,UINT nType, int cx, int cy);
    };

    FTLEXPORT template <typename T>
    class CFWTLDlgAutoSizeTraits
    {
    public:
        static HWND GetWinHwndProxy(T* pThis)
        {
            return pThis->m_hWnd;
        }
        static DWORD GetStatusBarCtrlID()
        {
            return ATL_IDW_STATUS_BAR;
        }
        //static void OnSizeProxy(UINT nType, CSize size);
    };

    //! ��WTL�� CDialogResize ������������Զ���������ؼ�λ�á���С���֧࣬��MFC��WTL��Dialog
    //! TODO:֧����ͨ��Windows���� View�����Ӵ���λ�ñ��桢�ָ��ȹ��ܣ���չWTL��
    //! Bug:�Ƿ��� TWindowAutoSizeTraits �ı�Ҫ��
    //! ʹ�÷������̳��б������� public FTL::CFWindowAutoSize<CMyDlg,FTL::CFWTLDlgAutoSizeTraits<CMyDlg> >
    //!   Ȼ���� OnInitDialog �е��� InitAutoSizeInfo���� WM_SIZE �������е��� AutoResizeUpdateLayout������ WM_GETMINMAX...
    //! ����������� WS_CLIPCHILDREN ���ԣ����Է�ֹ������˸����ʵ�ʲ���ʱ����Childʱ�޷���ȷ����
    FTLEXPORT template <typename T, typename TWindowAutoSizeTraits >
    class CFWindowAutoSize
    {
    public:
        // Data declarations and members
        enum
        {
            WINSZ_SIZE_X        = 0x00000001,   //�Զ�������չ����������֤���ұ߽粻��
            WINSZ_SIZE_Y        = 0x00000002,   //�Զ�������չ����������֤�ϡ��±߽粻��
            WINSZ_MOVE_X        = 0x00000004,   //�Զ������ƶ�����֤�ұ߽粻��
            WINSZ_MOVE_Y        = 0x00000008,   //�Զ������ƶ�����֤�±߽粻��
            WINSZ_CENTER_X      = 0x00000010,   //�Զ������ƶ�����֤���ұ߽粻��
            WINSZ_CENTER_Y      = 0x00000020,   //�Զ������ƶ�����֤�ϡ��±߽粻��
            WINSZ_REPAINT       = 0x00000040,   //Resize��ǿ���ػ棬ͨ��û�б�Ҫ 

            // internal use only
            _WINSZ_BEGIN_GROUP  = 0x00001000,
            _WINSZ_END_GROUP    = 0x00002000,
            _WINSZ_GRIPPER      = 0x00004000    //�ڲ�ʹ�ã���������ק��Gripper��ֻ����һ��(�ڲ�����)
        };
        struct _WindowResizeMap
        {
            DWORD   m_nCtlID;
            DWORD   m_dwResizeFlags;
        };
        struct _WindowResizeData
        {
            int     m_nCtlID;
            //! 0x   7   6   |    5   4   |    3   2   1   0    |
            //!      δ��(0) |   Group��  |      WINSZ ��־     |
            DWORD   m_dwResizeFlags; 
            RECT    m_rect;

            int GetGroupCount() const
            {
                return (int)LOBYTE(HIWORD(m_dwResizeFlags));
            }

            void SetGroupCount(int nCount)
            {
                FTLASSERT(nCount > 0 && nCount < 256);
                FTLASSERT(( m_dwResizeFlags & 0x00FFFFFF ) == m_dwResizeFlags); //Ŀǰ��ߵ�һ���ֽ�û������
                DWORD dwCount = (DWORD)MAKELONG(0, MAKEWORD(nCount, 0));
                m_dwResizeFlags &= 0xFF00FFFF;
                m_dwResizeFlags |= dwCount;
            }

            bool operator ==(const _WindowResizeData& r) const
            { 
                return (m_nCtlID == r.m_nCtlID && m_dwResizeFlags == r.m_dwResizeFlags); 
            }
        };
        typedef std::vector<_WindowResizeData> WindowResizeDataArray;
        WindowResizeDataArray   m_allResizeData;
        SIZE    m_sizeWindow;
        POINT   m_ptMinTrackSize;
        BOOL    m_bGripper;
    public:
        FTLINLINE CFWindowAutoSize();
        FTLINLINE BOOL InitAutoSizeInfo(BOOL bAddGripper = TRUE, BOOL bUseMinTrackSize = TRUE);
        FTLINLINE BOOL AutoResizeUpdateLayout(int cxWidth, int cyHeight);
    protected:
        FTLINLINE BOOL AutoPositionControl(int cxWidth, int cyHeight, RECT& rectGroup, _WindowResizeData& data, bool bGroup, 
            _WindowResizeData* pDataPrev = NULL);
    };

    //������ʾ������Ϣ(��˫��)
	//  ʹ�����µ�API(multimon.h����Ҫ���� COMPILE_MULTIMON_STUBS ��?)�� MonitorFromWindow/MonitorFromPoint/EnumDisplayMonitors ��
    class CFMonitorManager
    {
    public:
		
    private:
        //static BOOL CALLBACK MonitorEnumProc(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData);
    };

	enum ColorType
	{
		ctCtrlColor,
		ctSysColor,
	};
	
	typedef BOOL (CALLBACK* TranslateWndClassProc)(LPCTSTR pszOriClassName, LPTSTR pszNewClass, UINT nLength);
	__declspec(selectany) TranslateWndClassProc g_pTranslateWndClassProc = NULL;

    FTLEXPORT class CFWinUtil
    {
    public:
		//��Caption�Ĵ���(��DUI::ListDemo)Ҫʵ�ִ�С�ı䡢�������϶�ʱ��Ҫ��Ӧ WM_NCHITTEST ��Ϣ�����ض�Ӧ��λ��ֵ
		//pPtClient -- ScreenToClient(����ڿͻ����е�����)
		//prcClient -- GetClientRect(��Ϊ����Caption���壬��󻯡���С�����رյȰ�ť�����ڿͻ������Ի��);
		//prcCaption -- �Ի��Cpation�ķ�Χ(TODO:��ô�ų� �رյȰ�ť������ ��Region?)
		//bZoomed -- IsZoomed
		FTLINLINE static LRESULT CalcNcHitTestPostion(LPPOINT pPtClient, LPCRECT prcClient, LPCRECT prcCaption, BOOL bZoomed);

		//����ָ��ProcessId��������ID
		FTLINLINE static HWND GetProcessMainWindow(DWORD dwProcessId);

        //�����ָ�����������ǰ�� -- �����״̬����˸������ǰ̨��Bug(win98/2000?)
        //�����ڶ�������ʱ��������ʾ��ǰ̨
		//ע�⣺Windows 98/2000 doesn't want to foreground a window when some other window has keyboard focus
        FTLINLINE static BOOL ActiveAndForegroundWindow(HWND hWnd);

        //����������
        FTLINLINE static BOOL HideTaskBar(BOOL bHide);

        //ȫ�����ڣ�WS_POPUP|WS_VISIBLE ����(������ WS_OVERLAPPEDWINDOW ), 0,0 ~ CxScreen, CyScreen
        FTLINLINE static BOOL SetWindowFullScreen(HWND hWnd,BOOL isFullScreen, BOOL &oldZoomedState);
        FTLINLINE static LPCDLGTEMPLATE LoadDialog(HMODULE hModuleRes, LPCTSTR szResource, HINSTANCE * phInstFoundIn);

		//��ȡ GetSysColor ʱ��ɫ������Ӧ���ַ���
		FTLINLINE static LPCTSTR GetColorString(ColorType clrType, int nColorIndex);

        //WM_HSCROLL �� WM_VSCROLL ��֪ͨ��
        FTLINLINE static LPCTSTR GetScrollBarCodeString(UINT  nSBCode);

		//��ȡ������Ӧ�� VK_
		FTLINLINE static LPCTSTR GetVirtualKeyString(int nVirtKey);

        //��ȡ WM_NOTIFY ��Ϣ Code ��Ӧ���ַ�����Ϣ
        FTLINLINE static LPCTSTR GetNotifyCodeString(HWND hWnd, UINT nCode, LPTSTR pszCommandNotify, int nLength, 
			TranslateWndClassProc pTransProc = g_pTranslateWndClassProc);

        //��ȡ WM_COMMAND ��Ϣ�� notifyCode
        FTLINLINE static LPCTSTR GetCommandNotifyString(HWND hWnd, UINT nCode, LPTSTR pszCommandNotify, int nLength, 
			TranslateWndClassProc pTransProc = g_pTranslateWndClassProc);

        FTLINLINE static LPCTSTR GetSysCommandString(UINT nCode);

		//��ȡ��������͡����֡�λ�á���С�����������Ϣ
		FTLINLINE static LPCTSTR GetWindowDescriptionInfo(FTL::CFStringFormater& formater, HWND hWnd);
		//��ȡ Windows �������Զ�Ӧ���ַ�����Ϣ 
        FTLINLINE static LPCTSTR GetWindowClassString(FTL::CFStringFormater& formater, HWND hWnd, LPCTSTR pszDivide = TEXT("|"));
        FTLINLINE static LPCTSTR GetWindowStyleString(FTL::CFStringFormater& formater, HWND hWnd, LPCTSTR pszDivide = TEXT("|"));
        FTLINLINE static LPCTSTR GetWindowExStyleString(FTL::CFStringFormater& formater, HWND hWnd, LPCTSTR pszDivide = TEXT("|"));
		FTLINLINE static LPCTSTR GetWindowPosFlagsString(FTL::CFStringFormater& formater, UINT flags, LPCTSTR pszDivide = TEXT("|"));

		FTLINLINE static LPCTSTR GetOwnerDrawState(FTL::CFStringFormater& formater, UINT itemState, LPCTSTR pszDivide = TEXT("|"));
        FTLINLINE static LPCTSTR GetOwnerDrawAction(FTL::CFStringFormater& formater, UINT itemAction, LPCTSTR pszDivide = TEXT("|"));
#if 0
        //������з���(ATL��Դ�룿)
        FTLINLINE static BOOL CenterWindow(HWND hWndCenter , HWND hWndParent, BOOL bCurrMonitor);
#endif
    };//CFWinUtil

	//SetWindowHook ʱ��һЩ��������
	FTLEXPORT class CFWinHookUtil
	{
	public:
		FTLINLINE static LPCTSTR GetCBTCodeInfo(CFStringFormater& formater, int nCode, WPARAM wParam, LPARAM lParam);
	};

    //DI��Ŀ��ʹ��������Notification�Ļ���
    template <typename T>
    class CFUIAdapterForWin
    {
    public:
        //typedef T     parent_class;

        //virtual bool PostNotification(XfNotificationPtr pNotify) = 0;
        virtual void UpdateUI() = 0;

        //static bool PostNotificationProxy(XfNotificationPtr pNotify, void* data)
        //{
        //    T* pThis = static_cast<T*>(data);
        //    return pThis->PostNotification(pNotify);
        //}
        static void UpdateUIProxy(void* data)
        {
            T* pThis = static_cast<T*>(data);
            pThis->UpdateUI();
        }
    };

    //��MessageBox����Hook�����Ը��İ�ť���ı���Ŀǰû�и��İ�ť�Ĵ�С
    //! �÷���CFMessageBoxHook(m_hWnd, TEXT("MyClose"));
    //        ::MessageBox(...);
    #define PREV_WND_PROC_NAME  TEXT("PrevWndProc")
    class CFMessageBoxHook
    {
    public:
         //���Ĵ��̵߳����ĶԻ����ϵ�OK��ť����
        FTLINLINE CFMessageBoxHook(DWORD dwThreadId, LPCTSTR pszOKString);
        FTLINLINE ~CFMessageBoxHook(void);
    private:
        FTLINLINE static LRESULT CALLBACK CBTProc(int nCode, WPARAM wParam, LPARAM lParam);
        FTLINLINE static LRESULT WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    private:
        static HHOOK    s_hHook;
        static HWND     s_ProphWnd;
        static LPCTSTR  s_pszOKString;
    };

    class CFLockUpdate     
    { 
    public: 
        CFLockUpdate(HWND hwnd, BOOL bHighPrivilege = FALSE)
            :m_bHighPrivilege(bHighPrivilege)
            ,m_hwnd(hwnd) 
          { 
              if(m_hwnd==NULL)return; 
              if(m_bHighPrivilege) 
                  LockWindowUpdate(m_hwnd); 
              else 
                  SendMessage(m_hwnd,WM_SETREDRAW,FALSE,0); 
          } 
          ~CFLockUpdate() 
          { 
              if(m_hwnd==NULL)
              {
                  return; 
              }
              if(m_bHighPrivilege) 
                  LockWindowUpdate(NULL); 
              else 
                  SendMessage(m_hwnd,WM_SETREDRAW,TRUE,0); 
          } 
    private: 
        BOOL   m_bHighPrivilege; 
        HWND   m_hwnd; 
    };

#if 0
    template <typename ControlT , typename ConverterFun>
    class CControlPropertyHandleT
    {
    public:
        CControlPropertyHandleT(ControlT& control);//, ConverterFun& fun);
        FTLINLINE INT AddProperty(INT value);
    private:
        ControlT&       m_control;
        //ConverterFun&   m_fun;
    };
#endif

}//namespace FTL

#endif //FTL_WINDOW_H

#ifndef USE_EXPORT
#  include "ftlwindow.hpp"
#endif 