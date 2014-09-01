
#ifndef FTL_GDI_H
#define FTL_GDI_H
#pragma once

#ifndef FTL_BASE_H
#  error ftlGdi.h requires ftlbase.h to be included first
#endif
#include <atlimage.h>

//http://www.codeproject.com/KB/GDI/anieffect.aspx
//����ģ�� http://html.nhndesign.com/markup_tools/preview
//���Թ��ߣ�GdiWatch, http://www.189works.com/article-9611-1.html
//���幤��(FontCreator/FontForge) -- ���Դ�����༭ ttf �������ļ�
//ͼ��� -- http://findicons.com/pack

/*
.FON(��դ����)--����ض�����ʾ�ֱ����Բ�ͬ��С�洢��λͼ,��������ʸ��������,�Ŵ��Ժ����־��,����ʾ�ٶȷǳ���
.FON(ʸ������)--����ʸ������ѧģ�Ͷ���,һЩwindowsӦ�ó�����ڽϴ�ߴ����Ļ��ʾ���Զ�ʹ��ʸ�������������դ�������ʾ
.PFM(PostScript����)--����Adobe PostScriptʸ�����Ե�����,��Ҫ��װ"Adobe Type Manger"(ATM)�������ʹ��
.TTF(TrueType����)--�õ����,��һ����ѧģʽ�����ж���Ļ����������������壬�Ȼ���ʸ������������״�����֤����Ļ���ӡ�����һ���ԡ�
                    �����������š���ת�����ص��Ļ���־��

DOS�ȴ�ͳ�ַ������õ���ģ����ʾ����
GUI�е�����һ�����������������еĿ��Ƶ����ݾ�������ٻ��Ƴ���,
  Windows���û��ڶ��α��������ߵ�TrueType��������(*.ttf)

DPI-- dots per Inch(ÿӢ��ĵ���,��ӡ�ֱ���ʹ�õĵ�λ��LOGPIXELSY)��һ����˵�� 96(��С)/120(�е�)/150(�ϴ�)/600(�����ӡ��)
PPI -- Pixels Per Inch(ÿӢ�����������ͼ��ֱ�����ʹ�õĵ�λ)
  TODO: http://hi.baidu.com/kingcham/item/b3653ce0c69756216dabb8cd

1����Ӣ�� = 25.4mm; 1�� = 1/72 Ӣ�� = 25.4/72 mm
VGA��, 1�߼�Ӣ��=96����, 300dpi�Ĵ�ӡ����1Ӣ��300����

��ɫ�ռ�(Color Space) -- ΢���� CDrawingManager ���ṩ�˸�����ɫ�ռ�ת���ĺ���(��RGBtoHSL),��������MFC
  RGB -- Red, Green, Blue, ��ѧ�о�һ�㲻����RGB��ɫ�ռ�(���Խ������ֻ��ĵ���),����ͨ�õ�����Ӳ���Ĳ�ɫģ��
  HSL -- Hue(ɫ�� 0~239), Saturation(���Ͷ�,0~240), Lum(����,0~240)���ܸ��õ����ֻ�����
  CMY -- ��ҵӡˢ���õ���ɫ�ռ�

COLORREF -- 0x00bbggrr, ���ֽڣ�RGB ֻ������3���ֽڣ��Զ���һ����չ�� RGBA, ������Canvas���ֶ�����RGBA������
RGB(r,g,b)==#bbggrr

ȡ��ɫ�ĺ꣺RGB565_MASK_RED��RGB555_MASK_RED �ȣ� 
  R = (wPixel & RGB565_MASK_RED) >> 11;//ȡֵ��Χ0-31
  R = (wPixel & RGB555_MASK_RED) >> 10; // ȡֵ��Χ0-31


Ĭ�ϵĻ�ͼģʽΪ MM_TEXT(�߼���λΪ���أ�����ӡ���ʱ��Ļ��ͼ����ѻ�ͼģʽ -- ����ת��)
CScrollView����ͨ���ı��߼�����ԭ������λ�á��ı�ViewportExt�ȵĴ�С��ʵ���ƶ����Ŵ���С��

PtInRect��Rectangle -- �Ⱥ����ľ������򲻰������ε��ұ߽�͵ױ߽�,
����DrawClient��GetHandleRect�з��ص��� left-3, 7(��3��4����֤������Ч�ĸ߿�Ϊ3)

ǿ������ -- ���õ�ǰ����ɫ��䴰�ڣ����������ϵͳ�ػ洰�ڣ�����OnDraw��OnPaint������
  1.Invalidate(ʹ����������Ч) + UpdateWindow(���´���)
  2.RedrawWindow

ͼƬ��ܿؼ� -- Type����ΪFrame��ͼƬ�ؼ�(Picture Control), ��ֱ����Dialog��OnPaint��ֱ�ӻ���
�����ؼ� -- �����Ȱ�˳�����ε���CWnd���Invalidate��UpdateWindow�������ٿ�ʼ��DC��ͼ
            CWnd* pWnd = GetDlgItem(IDC_COLOR);  
            pWnd->Invalidate(); // ʹ�ؼ�������Ч
            pWnd->UpdateWindow(); // ���¿ؼ����� 
            pDC->FillRect(&rect, &brush); // ����������

�ƶ�ͼ��(��TrackBox) -- ��Ҫ�ڲ��ƻ�ԭ�б���ͼ���Ļ������ƶ���Щͼ�Σ����������ͼ������
  �ƶ�ͼ�εĹ���Ϊ��SetROP2(R2_XORPEN); ���ͼ����ԭλ�������ͼ��������������λ�����ͼ

���� -- �豸�޹ص�,��Ҫ��TrueType���͵���������
  CreatePointXXX -- ��Windows�������������ѡ��һ���������ò�����ƥ�������
  AddFontResourceEx -- ��ttf�������ļ��м������嵽ϵͳ�����ؼӵ�ϵͳ�е��������
  GetStockObject(DEVICE_DEFAULT_FONT/DEFAULT_GUI_FONT/SYSTEM_FONT) -- ��ȡϵͳ�е�Ĭ������
  */

/*****************************************************************************************************
* GDI -- 1985���Ƴ�
* GDI+ -- 2001����WinXP�Ƴ�
* WPF(Windows Presentation Foundation) -- 2006����Vista��.Net3�Ƴ�,ͳһ�������������ȿͻ���Ӧ�ó����ͼ�ν���
* 
* ��ͼǰ��ÿͻ����Ĵ�С(GetClientRect),�����л��ƣ�����Ҫoffsetviewport? ����Scroll��������
* 
* ��ĻDC -- CDC dc; dc.Attach(::GetDC(NULL)); xxx, ::ReleaseDC(NULL,dc.Detach()); //GetWindowDC Ҳ��һ����
*
* CDC 
*   ��Ա����
*     PatBlt -- ��DC�е�ˢ��λͼ��(��Դλͼ��)��դ��������λͼͼ��
*     MaskBlt -- ��ָ������Ĥ�͹�դ�������Դλͼ��Ŀ��λͼ
*     PlgBlt(Parallelogram Block transfer, ƽ���ı��ο鴫��)--��Դλͼ������Ŀ��λͼ��ƽ���ı��ν�����ɫ���ݵ�λ�鴫��
*     FloodFill -- �õ�ǰˢ(����λͼˢ)��ָ���㿪ʼ�����ָ����ɫΪ�߽������
*   ����
*     CClientDC -- GetDC
*     CWindowDC -- ::GetWindowDC
*     CPaintDC -- ::BeginPaint
*     CMetaFileDC --
*     CMemoryDC(WTL��) -- 
*   
* CGDIObject -- �ֲ������˳�ʱ���Զ�DeleteObject����ͨ��SelectStockObjectѡ��֮ǰ����ľɶ����Ԥ�������֤����ɾ��ʱδ��ʹ��
*   CPen
*   CBrush
*     CreatePatternBrush -- λͼ��ˢ
*     SetBrushOrg -- λͼ��ˢ�����ʱĬ���Կͻ��������Ͻ�Ϊԭ���(�豸����?)�������ڿͻ�����ͬλ�û��ƶ������ʱ��
*       ��ˢ�������ƫ��(�Զ�����)��������ǹ������壬�������� SetBrushOrg ʹ���豸�������û�ˢԭ��(ʹ��ʾ����GuiTest) 
*       ��ֵ�����ַ�ʽ��ȡ(���Ժ���OK)
*       1.CPoint pt(0,0); pDC->LPtoDP(&pt); dcMem.SetBrushOrg(pt.x,pt.y);
*       2.�� WTL.CZoomScrollImpl �� dcMem.SetBrushOrg( -m_ptOffset.x , -m_ptOffset.y);
*       ע�⣺��Ϊ��ӳ���������ڻ���ʱ��Ҫ�ŵ����Ʒ�Χ(�ƺ��Ŵ������߼�����ֵ����)--������rcClient,�����rcClip?
*         CRect rcClient;GetClientRect(rcClient);dc.DPtoLP(rcClient);rcClient.InflateRect(2,2);

*   CFont -- �豸�޹ص�����(TrueType���͵���������),�䴴����������"����"һ��ȫ�µ����壬
*     ���Ǵ�Windows�������������ѡ��һ���������ò�����ƥ�������.
*     ClearType -- Vista/Win7ʹ�õļ���������ʾ��RGB����������Ҳ�Ź⣬��ɫ������΢�����ɴﵽʵ�ʷֱ������ϵ�������ʾЧ����
*       ʹ���������ƽ��(WinXPĬ��δ����:��ʾ->���->Ч��->ʹ�����з�ʽʹ��Ļ����ı�Եƽ��)
*     MacType(ǰ��ΪGDI++) -- ���Խӹ�Windowsϵͳ��������Ⱦ���ܣ������ֿ���������ƽ����������
*     �����С��λ( WTL::CDialogBaseUnits::InitDialogBaseUnits )�� ��Ļ���ʱ����px(����ҳ)����ӡ���ʱ����pt(��Word)
*       1.point/pt(��,1/72�߼�Ӣ��) -- 96DPIʱһ������Ϊ 72/96=0.75��(9pt=12px)�� �� ITextFont::SetSize ��, ͨ����Ϊ"���Գ���"��������ϵͳ�����С���ö��ı�(�� 96DPI��120DPI)
*         nPointSize -- ͨ����ʮ��֮һ���� �� CreatePointFont ʱ�ĵ�λ, CFontDialog::GetSize ��1pt = 10 nPointSize
*       2.pixel/px() -- ���أ�����Ļ�ֱ����йأ�ͨ����Ϊ����Գ��ȡ�
*       3.twips(1/1440 �߼�Ӣ��� 1/20��) -- �� CHARFORMAT::yHeight
*       4.�ֺ� -- �� ���š�С�� �ȣ��ŵ���ʹ�ü򵥷��㣨������������ʵ�ʳߴ磩��ȱ��������Ĵ�С���ֺŵ����ƣ�
*                 ̫С��̫����ֺŵȼ���������޷����ֺű�ʾ
*       5.�߼���λ���� LOGONT::lfHeight�� ע�⣺ʹ�����ĸ߶ȵõ��������ʹ�ø�ֵ�õ�������С
*           <0 ʱ,�߶ȱ�ת��Ϊ�豸��λ����С�����������ַ��߶� -- ? Windows ���ݾ���ֵ����
*                 ���ҵ�����߶� = lfHeight - InternalLeading ???
*           =0 ʱ,ʹ�ú����Ĭ�ϸ߶�
*           >0 ʱ,�߶ȱ�ת��Ϊ�豸��λ,��С��������������߶�, �� nPointSize Ϊ8, LOGPIXELSY Ϊ96���������Ϊ -10.666?
*                 ���ҵ�����߶� = -lfHeight ???
*       ת��(MM_TEXT)��
*          �߼����� => ��: nPointSize = LOGFONT::lfHeight * 72 / dpiY;
*          �� => �߼�����: LOGFONT::lfHeight = -MulDiv(nPointSize, pDC->GetDeviceCaps(LOGPIXELSY), 72) = nPointSize*dpiY/72
*          �� => twips:    nTwips = nPointSize * 20;
* 
*       6.Em -- ������߼�����߶�, һ��em��ʾһ����������Ĵ�д��ĸM�ĸ߶ȣ�Ҳ����Զ�����λ��
*           GdiPlus::Font ����ʱʹ��, LOGFONT::lfHeight Ϊ��ֵʱ(������Ի�����ѡ�� 8 ��)��
*           1em=16px
*
*       //�Ի����ϵĵ�λ�� http://blog.csdn.net/dclchj/article/details/5938497
*       nPointSize(CreatePointFont) -- �����С,��0.1�㣨����/ī��/������Ϊ��λ��
*         5�������ֵĸ�ֵ��105,����Ϊ 10.5
*     lpszFaceName -- ���������ַ�����ָ�룬���õ���"����","Times New Roman"��
*   CRgn
*   CPallete
* ע�⣺ÿ��GetDC��õĶ���һ��ȫ�µ���ʱĬ�����õ�DC��ԭ��ѡ��ĸ���GDI����ȫ������
*   -- ���Լ�CreateComptableDC�����ĳ�Ա��������һֱ���棻��HDC�ڴ��ڵĴ����ڼ�һֱ��Ч
* 
* 
* ����(Region) -- �����ú�ͨ�� SelectObject ѡ�� HDC ���ǲü�����?
*  CombineRgn(�ϲ�����)
*    RGN_AND --��������Ľ���
*    RGN_COPY--��������ĵ�һ��������Ϊ�µ����� 
*    RGN_DIFF--��һ������Ĳ��ּ�ȥ��������Ľ���
*    RGN_OR  --��������Ĳ���
*    RGN_XOR --��������Ĳ�����ȥ��������Ľ���
*  InvertRgn -- ʹ�������ɫ��ɫ���ڲ�ɫ��Ļ�ϣ������ת�����ڲ�����Ļ��ɫ�ļ�������
*  FrameRgn -- Ϊ������Χ��һ���߽磬��ָ���߽�Ŀ�Ⱥ�ˢ�ӵ�ģʽ
*  PathToRegion -- ��·��ת��Ϊ���򣬵��ڴ˹����У�·�������ƻ�
*
* �ü�����(ClipRegion) -- ͨ�����òü����򣬼�ʹ��ȫ���߼���������ͼ��Ҳ�ܴ����߻�ͼЧ�ʡ�
*   BeginPaint��ȡ��DC�����ü�����(���ܸ�)��GetDC��CreateDC��ȡ��DCĬ�ϲ������ü�����(���ܵ�)
*   
*   IntersectClipRect -- �ϲ���ǰ�ü�����Ϳͻ����ľ�������
*   ExcludeClipRect -- �ӵ�ǰ�ü�����ȥ��ĳ����
*   ExtSelectClipRgn -- ͨ��ָ���ķ�ʽ(�� RGN_AND/RGN_OR ��)��һ���ض��������뵱ǰ�ļ�������ϲ���һ��
*
* ͼ��ռ�任 -- ʹ�� ����9�������ľֲ�������(��Ϊģ��)������ɣ�ͨ���Ƕ� ���ص㼰��Χ��9�������ɫֵ���γ���ģ���ֵ����ӡ�ƽ��
*   �ữ(Smooth)--ʹͼ������һ���������������ص���ɫֵ�Ĵ�С������ֶ��䣬�����ڳ�ȥͼ���е�״�������ή��ͼ��ĶԱȶȡ�
*           ����ÿ�����غ��ܱ�(��������)��9�����صļ�Ȩƽ��ֵȡ��������ģ��Ϊ ��ά��˹(Gauss)��
*           { 1,2,1,  2,4,2,  1,2,1 } x 1/16
*   ��(Sharp)--��ǿͼ���ԵЧ����ͨ����ǿ��Ƶ��������ͼ���е�ģ����Ҳ��Ϊ��ͨ�˲�������ģ��Ϊ������˹(Laplacian) ģ�档
*           { -1,-1,-1,  -1,9,-1,  -1,-1,-1 } x 1
*****************************************************************************************************/

/*****************************************************************************************************
* ��ͼ����(������˸ -- flick)
*   1.ֻ����һ��
*   2.WM_ERASEBKGND -- ��������˸���Ϣ����1������ϵͳ�����Ĭ�ϴ�����ơ����ƺ����۷���TRUE��FALSE����������˸��(ֻҪ������ϵͳĬ�ϵĴ�����)
*   3.��Ҫ���Ʋ���Ҫ��(Clip) -- Windows���Զ����е�����ģ�����Invalidate����Ƶ����棿��ô���Եģ�����
*     GetClipBox -- ʼ�պ� PAINTSTRUCT::rcPaint һ��
*     GetUpdateRgn --
*     GetUpdateRect(BeginPaint��Ϊ��)
*     PAINTSTRUCT::rcPaint
*   4.CS_VREDRAW,CS_HREDRAW -- ��������,��������ѡ��ڴ�����СʱҲ���ػ棬��������Сʱ���ػ�,�Ŵ�ʱ��������Ч������ȫClient
*   5.WS_CLIPCHILDREN, WS_CLIPSIBLINGS -- �ų��Ӵ��塢�ֵܴ���(ע��Zorder)��λ��
*   6.ExcludeClipRect -- �ų��ض�����(���ƺ���Ч -- ������˸,�������ص�����)
*   7.Double Buffering -- ������ʹ�ã����ÿ���ػ�ʱ����MemoryDC�����(1024x768x32/8=3M),�ڴ��ܼ���
*     CWindowDC dcDesktop(NULL/m_hWnd); CDC dcMem; dcMem.CreateCompatibleDC(dcDesktop);
*     bitmap.CreateCompatibleBitmap(dcDesktop,cx,cy); //ע�⣬���������� dcMem������ֻ���Ƕ�ɫͼ
*   LockWindowUpdate
*
* �������
*   0.if(!IntersectRect(xx)){return;} -- �жϱ��ؼ���������κ���Ҫ���Ƶľ����Ƿ��н��������û��ֱ�ӷ��أ��������
*   1.OnDraw ��,pDC->GetClipBox �����Ҫ���Ƶ��߼�����
*   2.����rcClip�ĸ���rect��ת���豸���꣬����ߡ�����λͼ
*   3.offsetViewportOrg( - rect.left, -rect.right); -- Ϊ�˱�֤�������������ҲOK��
*     ѡ���bitmap��������ȷ��ƫ��
*   4.selectBitmap ��ʹ���߼������ͼ, 
*   5.IntersectClipRect( rcClient )
*   6.��memoryDC��pDC��viewPortOrg��WinOrg�ȶ�����Ϊ(0,0)��ӳ��ģʽΪMM_TEXT
*   7.Bitblt(rect.left, rect.right, .... ,&memDC, 0,0, SRCCOPY)
*
* ����ͨ�����ı䴰���Сʱ�Ƿ������ػ棺Display  -> ��� -> Effects -> Show window contents while dragging 
*****************************************************************************************************/

/*****************************************************************************************************
* ����ϵͳ  -- ������ת��?��������ϵ�ռ� =(SetWorldTransform)=>ҳ��ռ�(�߼�����?)=(SetMapMode)=>�豸�ռ�(�豸����)
*   ��������(world space)
*   �߼�����(page space ,logical coordinates) -- ����ϵ��ΪWindow��ʹ���߼���λ(�� 0.1Ӣ��)
*     ���ñ���Ķ���λ�ã�GDI����(�� GetClipBox)��ʹ���߼����������������õĻ���
*     GDIʹ���û�ֻ�����߼������ϻ�ͼ����( �߼������ͼ ==(MapModeӳ��)=> �������� )
*   �豸����(device coordinates) -- ����ϵ��ΪViewPort�����������豸�����豸�ܱ�ʾ����С����(������)Ϊ��λ
*     CWindow��Ա����ʹ��(�� InvalidateRect), Mouse, λ�ò���(�� PtInRect��)
*     �����豸����ϵ���ͻ���(CClientDC, MouseMove)��������(CWindowDC)����Ļ����(�� GetCursorPos��SetBrushOrg��)
*       ת����ScreenToClient��ClientToScreen
*     ע�⣺Viewport(���ɼ�)����������(�ɼ�)�ǲ�ͬ�ģ���ͬ���Ƕ�������Ϊ��λ��ֻ��Ҫ����λ������
*     
* ����ʱ������ʹ���߼����꣬Ȼ��Windowsͨ��ӳ��ģʽ��ֵ,�Զ�ת��Ϊ�豸����,����ʾ���豸(����ʾ������ӡ��)��
* 
* WindowOrg ���Ǳ�ӳ��Ϊ ViewporOrg -- ע�⣺��Ȼ��Ϊԭ�㣬����ֵ��һ��Ϊ0
*
* Win9xʹ��ʵ��Ӣ��?WinNT�Ժ�ʹ�û����߼�Ӣ���ӳ�䷽ʽ����ʵ�ʵ�Ӣ��Ҫ��1�߼�Ӣ��=100�߼���λ ?�� 96 - 100��
*   ������ͨ��10���ı�(Լ3.53����)�ǳ�С��ֻӰ����ʾ����Ӱ���ӡ��
* 
*   SetWorldTransform -- ���� ��Сһ��(eM11=0.5,eM22=0.5)����ת��X�ᷴ��(eM11=1,eM22=-1)��Normal(eM11=1,eM22=1)��
*     ��Ҫͨ�� SetGraphicsMode(GM_ADVANCED) ������������
*     page space�еĵ�����(x'��y')�� world space�еĵ�����(x,y)
*     x' = x * eM11 + y * eM21 + eDx, 
*     y' = x * eM12 + y * eM22 + eDy, 
*     
*   SetWindowOrgEx(x,y) -- �Ѵ��ڵ�(x,y)��ӳ�䵽�ӿڵ�ԭ��.
*     ���window�ǿ�������,���Լ�������ôһ��window, �ṩGDI��ͼ�������߼���������ͼ,
*     �������ϵҲ�� page space ����ϵ,���Զ���ԭ���뵥λ����( �߼���λ,��0.1mm,�Ӷ��������豸�޹� )
*     ͨ�����ڻ���ʱ�������Ͻǻ��ƣ�����л���ʱ����Ҫ SetWindowOrgEx([-?]m_rcDrawTarget.left, [-?]m_rcDrawTarget.top);
*   SetViewportOrgEx(x,y) -- �Ѵ��ڵ�ԭ��(���ĵ�)ӳ�䵽�ӿڵ�(x,y)��.
*     �����豸�������������Ͻ�Ϊԭ��,������Ϊ��λ,X����,Y���� 
*     viewport��֮��������ԭ�������SetViewportOrgEx�ı�(����Ч��?),�Լ���С������SetViewportExtEx�ı�(�Ŵ���С?),
*     �������ϵҲ��device space����ϵ,viewportҲ�ǿ�������,����ܿ�����ֻ����������ϵ(��������Ϊ��λ��ֻ����λ��)
*     ͨ�������й�����ʱ����ƫ�ƣ��� WTL::CZoomScrollImpl::SetViewportOrg(-m_ptOffset.x, -m_ptOffset.y);
*   Ĭ��ӳ���£�SetViewportOrg(x,y) = SetWindowOrg(-x,-y)
*     
*   SetWindowExtEx    //�����߼�����ϵͳ�ж�Ӧ���߼���λ�������� 100 ��
*   SetViewportExtEx  //�����豸����ϵͳ�ж�Ӧ�����ظ���, �� GetDeviceCaps(hdc, LOGPIXELSX)��ʾ1�߼�Ӣ���Ӧ��96������
*     �� GetClientRect ȡ�õĿͻ������ظ���.����ͨ�� ViewExt/WinExt �ı�ֵ���1�߼���λ��Ӧ���豸���굥λ��
*     ������ı�ֵ��ʾ100���߼���λ��Ӧ96������(1�߼�Ӣ��)��
*     ��ˣ�ͨ���������У����ߵľ���ֵ����Ҫ���ؼ��Ǳ�ֵ��
* 
*  ʹ��CDC���� SetViewPortOrg �� SetWindowOrg ���Ըı�ԭ��
*  ���ܶԴ��ں��ӿ�ԭ����ʲô�ı䣬�豸��(0,0)ʼ���ǿͻ��������Ͻ�
*  
* ӳ��ģʽ -- ȷ��GDI����ʱWindowsϵͳ��δ��߼��Զ�ӳ�䵽�豸(����Ļ���ӡ�������ص�)
*     �߼���λ��ʵ�ʴ�С�����������豸��λ�ͷ�����ѡ���豸(��ʾ������ӡ��)ʱ�Ѿ�ȷ����
*   ģʽ                   �߼����굥λ                 
*   MM_TEXT               ����(ͬ�豸����)              x��������,y��������( x+,y+ )
*   MM_LOENGLISH           0.01����Ӣ��(0.254mm)        x��������,y���¼���( x+,y- )
*   MM_HIENGLISH           0.001����Ӣ��                x��������,y���¼���( x+,y- )
*   MM_LOMETRIC            0.1�������                  x��������,y���¼���( x+,y- )
*   MM_HIMETRIC            0.01�������                 x��������,y���¼���( x+,y- )
*   MM_TWIPS         1/1440����Ӣ��(1/20��,��0.176mm)   x��������,y���¼���( x+,y- )
*   MM_ISOTROPIC        ���Ըı�������Ӻ�ԭ��          �ݺ��1��1
*   MM_ANISOTROPIC      ���Ըı�������Ӻ�ԭ��          �������ӿ��Էֱ�ı�
* 
* ע�⣺
*   1.Ĭ�������(��û����SetWindowExt/SetViewportExt�趨һ���߼���λ��Ӧ���ٸ����ص�ʱ��
*     MM_ANISOTROPIC��MM_ISOTROPICʹ��MM_LOMETRICӳ��ģʽ)
*   2.ʹ�� SetLogicalMapMode ���������߼��������߼���λ(���� 0.01�߼�Ӣ��)
*   3.AtlWin.h ���Ѿ��ṩ�� HIMETRIC_PER_INCH��MAP_PIX_TO_LOGHIM �Ⱥ����ת����
*     ��? hmWidth = MAP_PIX_TO_LOGHIM(pxWidth, GetDeviceCaps(hdc, LOGPIXELSX));
* 
* ���Ĺ�ʽ(��ֵ��ͬ,Ҳ��˵��һ�߼���λ��Ӧ�����ظ���):
*   (xViewPort - xViewportOrg)/(xWindow-xWindowOrg) = xViewExt/xWindowExt
*   ע�⣺�߼�ԭ��WindowOrg����ӳ����豸ԭ��(ViewportOrg)������ֵ��һ��Ϊ0
*   MyLPToDP: xViewPort = (xWindow-xWindowOrg)*(xViewExt/xWindowExt)+xViewportOrg
*             yViewPort = (yWindow-yWindowOrg)*(yViewExt/yWindowExt)+yViewportOrg
*   MyDPToLP: xWindow = (xViewport-xViewportOrg)*(xWindowExt/xViewExt)+xWindowOrg
*             yWindow = (yViewport-yViewportOrg)*(yWindowExt/yViewExt)+yWindowOrg
*
* ӳ�����(�߼���λ���豸��λ�����������)
*   ���ܵĺ�����ͻ����Ĳ���δˢ�£��������ּ�ࣻ��������Ļ�Ĳ�ͬλ�û���С��Ŵ�һ�����صȡ�
*   ���ⷽʽ��1.��ֻ����Ļ��ͼ(����ӡ?)��ʹ�� MM_TEXT��������ӳ�����?
*             2.����ѡ�񴰿ڷ�Χ���ӿڷ�Χ�ȿ�������ӳ��ģʽ�����߼�Ӣ���MM_TWIPS��1440/96,�򻯺�Ϊ15/1(���豸���߼�ʱ�����)
*   ��Ϊ���ܴ���ӳ����Ϊ�˱�������ʱ���û�����Ӱ��(��Ҫ�ǷŴ�ʱ����ʱһ���߼���λ��ӦN���豸��λ����50��ʱһ���߼���λ��Ӧ50���豸��λ),
*     ����漰�߼�������豸����ת����ƫ��ʱ,���ת��Ϊ��С��λ�����м��㣬�Ӷ��������(��Ŵ�ʱ���豸�����¼���ƫ�Ƶ�)
*   GDI+�и������꣬�Ƿ񻹴����������?
*
* ����ת��(����ӳ��):
* 
*   Windows�����Ϣ����������������豸����,CDC,CRect�ȵĳ�Ա������Ҫʹ���߼�����ſ�������������
*   Windows GDI �����߼����굽�豸����Ļ��㡣�ٶ��������豸����ӳ��ģʽ���йصĲ���,
*   CDC���� LPtoDP �� DPtoLP ����������ϵͳ֮����л��㡣
*   ��Ҫ�������£�
*     1.�ٶ�CDC��Ա���������߼��������
*     2.�ٶ�CWnd��Ա���������豸�������
*     3.���豸�����н������еļ������Բ��������豸�����ж�������
*       ��CRect::PtInRect�����ĺ���,ֻ����ʹ���豸����ʱ,���ܱ�֤�������ȷ�ԡ�
*     4.���߼������д洢����ʹ�õ�ֵ��������豸�����д洢һ���������,ֻҪ�û���һ�������й���,
*       �õ������Ͳ�����Ч�ˡ�
*   
* ��������:����ӳ��ģʽʱ�洢�û�������ֵ,��ӳ��ģʽ�޹�?
*
*****************************************************************************************************/

//GDI���� -- GDI����ľ������Windows�ڲ�ʹ�õĶ��󣬲���ʹ��CHECK_POINTER_XXX(IsBadReadPtr)һ��ĺ��������ж�
/*****************************************************************************************************
* ���֣�
* GetTextMetrics -- ��ȡ���������Ϣ��ƽ���ַ���ȡ�Family ������
* GetTextExtentPoint32 -- �����ȡָ���ַ����ĸ߶ȺͿ��
* SetTextJustification -- �����ڻ��Ƶ��ַ������Զ����ӿո�( �����ÿ�еĿո��� )��
*   specifies the amount of space should add to the break characters in a string of text
*   (���� nBreakExtra=������-�ַ����ܿ��)��
*   ÿ�����������ʹ��SetTextJustification(hdc, 0, 0);�ָ�Ĭ�ϲ���
*   DrawText 
*     DT_CALCRECT -- ���㲢�������εĿ����
*     DT_NOCLIP -- �����ã��������ʾ�ٶ�(�ử��������?)
*     DT_SINGLELINE -- ������ʾ�ı������Իس��ͻ��з�������ָ����ֵ����ʹ�� DT_VCENTER?
*     DT_TABSTOP -- ����Tab���ַ�λ��(Ĭ��Ϊ8)����������nFormat�ĸ�λ�ֽ��У�ͬʱ�� DT_EXPANDTABS ��չ
*     DT_WORDBREAK -- �ı��г�����������Ŀ�ʱ����
*   DrawTextEx -- ������ LPDRAWTEXTPARAMS(����Tab��С�����ұ߽��) ѡ��,
*   ExtTextOut -- ����ָ����������ļ�� �����磺
*     int space[]   = {20, 25, 30, 35, 40, 45, 50, 55, 60};
*     ExtTextOut(hdc, 20, 80, ETO_CLIPPED, &rect, "ExtTextOut", 10, space); // ���������
*   TabbedTextOut -- ����ָ��λ����ʼTab��λ����ַ�
*     int tabList[] = {120, 280, 380};
*     TabbedTextOut(hdc, 20, 170, TEXT("Tabbed\t\tText\tOut"), 16, 3, tabList, 1);
*   PolyTextOut -- 
*   ����: GDI��û���ܿ����м����ı������ʽ?
*
* RTF -- Rich Text Format,��΢��˾�����Ŀ�ƽ̨�ĵ���ʽ,��һ���ܺõ��ļ���ʽת�����ߣ������ڲ�ͬӦ�ó���֮����и�ʽ���ı��ĵ��Ĵ���
*   ʹ�� wordpad.exe ���Ժܷ��������RTF�ĵ�����׼��RTF�ļ�Ӧ�ý�����7λASCII���ַ��� 
*   ���ָ�ʽ
*     a.δ��ʽ���ı�(unformatted text)
*     b.������(control words) -- ��Ǵ�ӡ�����Ʒ��ĸ�ʽ������, "\[a-z��Сд��ĸ�ַ�], ���ļ���ʼ���� "\rtf1" ��
*     c.���Ʒ���(control symbols)
*     d.��(group)
*   �ļ���ɣ�������)
*     a.��־: \rtf1 �� \urtf1
*     b.�ļ�ͷ -- ������ļ�����ɫ��ȣ�ÿ������һ�Դ�����������
*     c.����
*
* λͼ -- 
*   ע�⣺��������λͼʱ������DC�����ǲŴ����õ��ڴ�DC -- ����λͼֻ�кڰ���ɫ
*   CBitmap -- ���ܽ�����ֻ����ʾ������Դ�е�ͼ�ꡢλͼ����꼰ͼԪ�ļ�
*     LoadBitmap -- ����Դװ��λͼ,ע�⣺��ɫ�ᱻ�ü�Ϊ��Ļ����ɫλ����Ҳ���ֱ�ӷ�������(�� DIBSection )
*     LoadOEMBitmap -- װ��Ԥ����λͼ(�� OBM_CLOSE )
*     LoadMappedBitmap -- ����Դװ��λͼ��������ɫӳ�䵽��ǰϵͳ��ɫ
*     CreateBitmap -- ��������ָ���߿��ͼ���� DDB �ڴ�λͼ
*     CreateDiscardableBitmap -- ������ָ��DC���ݵĿɷ���λͼ(ʲô�ǿɷ���λͼ?)
*     SetBitmapBits(SetDIBits) -- ��ָ���Ļ�������λͼ���ݸ��Ƶ�CBitmap��(�ú����ѹ�ʱ,Ӧ��SetDIBits)
*     GetBitmapBits(GetDIBits) -- ��CBitmap�е�λͼ���ݸ��Ƶ�ָ���Ļ�����(�ú����ѹ�ʱ,Ӧ��GetDIBits)
*     SetBitmapDimensionEx -- ����λͼ�ĸߺͿ�(��0.1����Ϊ��λ��Ϊʲô��������?�� CreateXXX �еĿ����ʲô��ϵ?)
*   CImage(atlimage.h) -- MFC/ATL���������(����ʹ�õ��� Gdiplus::Bitmap)��ͬʱ֧�� DDB/DIB,
*       �ܴ��ⲿ�����м��ز���ʾ JPEG/GIF/BMP/PNG�ȸ�ʽ��ͼ���ļ���֧�� Exit ��Ϣ?
*       ��Draw�����Զ�֧��͸��ɫ��Alphaͨ��(�ۺ���StretchBlt��TransparentBlt��AlphaBlend�Ⱥ���)
*       ע�⣺�����DLL��ʹ�� CImage�����ܻ�������� --��̬�� CImage::CInitGDIPlus ���������û���� DllMain ����ǰ����
*             CImage::ReleaseGDIPlus �����ͷŵĻ�������Ϊ�����ñ���ʱ�� DLLMain �е��� GdiplusShutdown ������
*             �μ�:http://support.microsoft.com/kb/322909
*     AlphaBlend -- ֧�����ؼ�����ɫ���(Դλͼ��Ŀ��λͼʹ��Alpha��Ϲ���)��ʵ��͸���Ͱ�͸����Ч��
*       AlphaFormatΪ0 -- ��������ʹ��ͬ���ĳ���Alpha
*                  ΪAC_SRC_ALPHA -- ÿһ�����ر������Լ���alphaͨ��������32-bpp��DC
*     Draw -- �������ط�ʽ���ۺ��� StretchBlt��TransparentBlt��AlphaBlend�Ⱥ����Ĺ��ܣ��Զ�֧��͸��ɫ��Alphaͨ��
*     GetBits -- ��ȡͼƬ�е�λͼ����
*     Load/LoadFromResource -- �� �ļ�/��Դ ����ͼ��
*     IsIndexed -- �ж��Ƿ�������ɫ(�е�ɫ���α��ɫ)
*     PlgBlt -- ʹһ�����������λͼӳ�䵽һ��ƽ���ı��������У��ҿ���ʹ��λ���β�����
*     TransparentBlt -- ��Ŀ�������в���͸��ͼ��
*     SetTransparentColor -- ����ĳ����ɫ��͸��ɫ
*     MaskBlt -- ��Ŀ�������в���Դλͼ������λͼ�ϳɺ��Ч��
*     GetExporterFilterString -- ��ȡ֧�ֵ�ͼ���ļ��Ĺ����ַ���,�����ڴ򿪡�����Ի���
*     Save -- ���ָ�� GUID_NULL���ʽ������չ��ȷ����������ָ�� GUID_BMPFILE/GUID_PNGFILE/GUID_JPEGFILE ��
*     ע�⣺��GetDC/ReleaseDC����������(��GDI����ʧЧ)���GDI�������ʱ�ᱨ����ԣ��������ٷ�������
*   GUI����ʧЧ��� GDI����� ��Ч��
*     HBITMAP ����Windows�ڲ�����(the internal index of system objects)��
*     ��ֵ���ǵ�ַ(ֱ�Ӳ鿴��Ӧֵ�ĵ�ַʧ��,�ҵ��� IsBadReadPtr �Ⱥ���ʱ����� Access Violation����)
*     ��ɾ��(DeleteObject)���HBITMAP(Ұָ��?)����GetObject�᷵��0(ʧ��),��GetLastErrorΪERROR_INSUFFICIENT_BUFFER��
*     ��SelectʱҲ�᷵��NULL������ʱGetLastErrorΪ0��
*     HBITMAP ͬһʱ��ֻ��ѡ��һ��DC�У��ڶ���Selectʱ�᷵��NULL����GetLastErrorΪ0
* 
*  CreateDIBSection -- ������ֱ�ӷ��ʵ�DIB����ֱ�ӷ���λͼ��λ��Ϣ������DIBSECTION
*    LoadImage(, LR_DEFAULTSIZE | LR_CREATEDIBSECTION) -- ����ͼ����Դ�����Ҵ�����ֱ�ӷ������ص� DIBSection
*  CreateDIBitmap -- ��DIB����DDB������BITMAP
* 
* ����
*   ��������:CreatePen/CreatePenIndirect/ExtCreatePen
*   // ����Geometric����( LOGBRUSH lb )
*     lb.lbStyle = BS_HATCHED ; 
*     lb.lbHatch = HS_DIAGCROSS;  
*     lb.lbColor = RGB(255, 0, 0);
*     ExtCreatePen(PS_GEOMETRIC | PS_DASH | PS_ENDCAP_SQUARE | PS_JOIN_ROUND,10, &lb, 0, NULL);
*
* ��ˢ
*   ��������:GetStockObject(LTGRAY_BRUSH)/CreateSolidBrush/CreateHatchBrush/CreatePatternBrush/CreateBrushIndirect
*
* ��ͼ 
* SetArcDirection --  ������Բ�����Ʒ��� 
* RoundRect -- Բ�Ǿ���
* Ellipse -- ��Բ
* Pie -- ��ͼ
* Arc -- ������Բ��
* PolyBezier -- ����Bezier����
* PolyDraw -- ����һֱ�ߣ����������һBezier����
* PolyPolyline -- һ�λ�����������ֱ�����ͼ��
* POINT pplPoint[] = 
* { 
*   200,300,280,300,280,380,200,380, 200, 300,
*   200,300,140,270,70,220,150,220,190,270,280,300,
*   200,380,140,320,70,300,70,220,
*   140,320,140,270,190,270
*   };
* DWORD value[] = {5, 6, 4, 3};
* PolyPolyline(hdc, pplPoint, value, 4);
*
* ExtFloodFill��
*   FLOODFILLBORDER  -- ��䵽��ɫ�ı߽磬��������ڲ���ɫ��ͬ���߽���ɫ��ͬ������(Ч��ͬFloodFill)
*   FLOODFILLSURFACE -- ���������ɫΪcrColor�ĵ㣬ֱ��������crColor�ĵ㣬
*     ��������ڲ���ɫ��ͬ���߽���ɫ���Բ�ͬ������
*****************************************************************************************************/

/*****************************************************************************************************
* ͸������
*   AlphaBlend -- ��ʾ��͸���ȵ�ͼ�� Alpha ָ����������ɫ�뱳����ɫ�Ļ�ϳ̶�
*     ��ʾ��ɫ = Դ������ɫ*Alpha/255 + ������ɫ*(255-Alpha)/255( Alpha Ϊ0ʱ��ȫ͸����255ʱ��ȫ��͸�� ) 
* 
* ��͸����ʾ(snagitץͼʱ��ʾ��Ч��)
*   1.Bitblt һ��������ԭʼλͼ�� dcTran
*   2.����һ���ڴ�DC(dcAlpha), ����ȫ�� -- dcAlpha.FillSolidRect(RGB(0,0,0))
*   3.ʹ�� dcTran.AlphaBlend( xxx, dcAlpha, 0,0, (AC_SRC_OVER, 0, 50, 0)) ��ȫ�ڵ�ͼ����� 50 ��Alphaͨ�����ǵ�ԭʼλͼ��
*     BLENDFUNCTION �ĵ��ĸ���������� AC_SRC_ALPHA ��ʾɶ��˼? �ƺ����� UpdateLayeredWindow ������ʹ�õ�
*   4.ѡ�����ʱ�����Ȼ��� dcTran��Ȼ���ڸ���λ�ô�ճ��ָ����С��bg
*
* �����ͼAPI��Vista�Ժ�֧�� ( atltheme ���� CBufferedPaintBase��CBufferedPaint ������)
*   BufferedPaintInit/BufferedPaintUnInit -- ��ʼ������ֹ����ÿ���̶߳���
*   BeginBufferedPaint -- ��ʼ�����ͼ��ָ�����յĻ��潫Ҫ���Ƶ�λ��
*     BufferedPaintSetAlpha
* 
* ��ȡ����DC
*   ::GetDC(NULL) �� ::GetWindowDC(NULL) -- ��ǰ���Գ�������û�в��ע����� ::ReleaseDC
*****************************************************************************************************/

namespace FTL
{
    //ARGB1555 �� RGBA8888 ?  RGBA �Ǵ���� ARGB ��С����?

    //��Ӧ���ڴ�ṹ�� 0xAABBGGRR -- 
    //  ���⣺gdi+���� Color::MakeARGB,�ڴ�ṹ�� 0xAARRGGBB(��) �� 0xBBGGRRAA���������ֶ�

#define MAKE_RGBA(r,g,b,a)  ((COLORREF)((((BYTE)(r)|((WORD)((BYTE)(g))<<8))|(((DWORD)(BYTE)(b))<<16))|(((DWORD)(BYTE)(a))<<24)))
#define GetAValue(rgba)     (LOBYTE((rgba)>>24))

    #ifdef FTL_DEBUG
        //�ж�λͼ�Ƿ��ѡ��MemoryDC:1.Ҫ��Ч; 2.û�б�ѡ����DC��
    #define CHECK_BITMAP_SELECTABLE(h)   \
        {\
        FTL::CFGdiObjectChecker checker;\
        FTLASSERT(checker.IsValidate(h));\
        FTLASSERT(!checker.IsSelected(h));\
    }

    #define CHECK_GDIOBJECT_VALIDATE(h) \
        {\
        FTL::CFGdiObjectChecker checker;\
        FTLASSERT(checker.IsValidate(h));\
    }
    #else
    #  define CHECK_BITMAP_SELECTABLE(h)      __noop
    #  define CHECK_GDIOBJECT_VALIDATE(h)     __noop
    #endif 


    class CFGdiObjectInfoDump
    {
    public:
        FTLINLINE CFGdiObjectInfoDump();
        FTLINLINE ~CFGdiObjectInfoDump();
        FTLINLINE BOOL GetGdiObjectInfo(HGDIOBJ hgdiObj);
        FTLINLINE LPCTSTR GetGdiObjectInfoString();
    private:
        LPCTSTR m_pszGdiObjectInfo;
    };

    class CFGdiObjectChecker
    {
    public:
        FTLINLINE CFGdiObjectChecker(HDC hDCTarget = NULL);
        FTLINLINE ~CFGdiObjectChecker();
        FTLINLINE BOOL IsValidate(HGDIOBJ hGdiObj);
        FTLINLINE BOOL IsSelected(HBITMAP hGdiObj);
    private:
        HDC     m_hDCCompatible;
    };

    //��Ȼ���ȡ����Ϣ
#define HDC_PROPERTY_GET_MAPMODE        0x00000000
#define HDC_PROPERTY_GET_WINDOW_INFO    0x00000001
#define HDC_PROPERTY_GET_VIEWPORT_INFO  0x00000002
#define HDC_PROPERTY_GET_MODE           0x00000004
#define HDC_PROPERTY_GET_DEVICE_CAPS    0x00000008

#define HDC_PROPERTY_GET_BITMAP         0x00000010

#define HDC_PROPERTY_GET_ALL            0xFFFFFFFF
#define HDC_PROPERTY_GET_DEFAULT        0x00000010

    struct HDCProperty
    {
    public:
        FTLINLINE HDCProperty();
        FTLINLINE ~HDCProperty();
    public:
        FTLINLINE LPCTSTR GetPropertyString(DWORD dwPropertyGet = HDC_PROPERTY_GET_DEFAULT);
		HWND	m_hWnd;
        HDC     m_hSafeHdc;
        int     m_nGraphicsMode;
        int     m_nMapMode;
        int     m_nArcDirection;
        
        //������ͼ���������ƻ�ͼ��ˢ����㣺��Ĭ��ֵΪ�ͻ������Ͻǵ�����ԭ�㣨0, 0����
        POINT   m_ptBrushOrg;

        POINT   m_ptWindowOrg;
        SIZE    m_szWindowExt;                  //�߼�����(�� 2000,-2000)
        POINT   m_ptViewportOrg;
        SIZE    m_szViewportExt;                //�豸����(�� WM_SIZE �л�ȡ���Ĵ�С)

        int     m_nDeviceCapsDriverVersion;     //(DRIVERVERSION),�� 0x4000
        int     m_nDeviceCapsTechnology;        //(TECHNOLOGY), �� 1
        int     m_nDeviceCapsHorzsize;          //(HORZSIZE), �� 337
        int     m_nDeviceCapsVertsize;          //(VERTSIZE), �� 270
        int     m_nDeviceCapsHorzres;           //��Ļ��(HORZSIZE),�� 1024
        int     m_nDeviceCapsVertres;           //��Ļ��(VERTSIZE),�� 768
        int     m_nDeviceCapsLogPixelsX;        //ˮƽ��ÿ�߼�Ӣ���������, �� 96 �� 120
        int     m_nDeviceCapsLogPixelsY;        //��ֱ��ÿ�߼�Ӣ���������, �� 96 �� 120
        int     m_nDeviceCapsBitsPixel;         //��ɫ���(BITSPIXEL),�� 32
        int     m_nDeviceCapsPlanes;            //(PLANES), �� 1
        int     m_nDeviceCapsNumBrushes;        //(NUMBRUSHES), �� -1
        int     m_nDeviceCapsNumPens;           //(NUMPENS), �� -1
        int     m_nDeviceCapsNumFonts;          //(NUMFONTS), �� 0
        int     m_nDeviceCapsNumColors;         //(NUMCOLORS), �� -1
        int     m_nDeviceCapsAspectX;           //(ASPECTX), �� 36
        int     m_nDeviceCapsAspectY;           //(ASPECTY), �� 36
        int     m_nDeviceCapsAspectXY;          //(ASPECTXY), �� 51
        int     m_nDeviceCapsPDeviceSizeReserved;   //(PDEVICESIZE), �� 0
        int     m_nDeviceCapsClipCaps;          //(CLIPCAPS), �� 1
        int     m_nDeviceCapsSizePalette;       //(SIZEPALETTE), �� 0
        int     m_nDeviceCapsNumReserved;       //(NUMRESERVED), �� 20
        int     m_nDeviceCapsColorRes;          //(COLORRES), �� 24
        int     m_nDeviceCapsPhysicalWidth;     //(PHYSICALWIDTH), �� 0
        int     m_nDeviceCapsPhysicalHeight;    //(PHYSICALHEIGHT), �� 0
        int     m_nDeviceCapsPhysicalOffsetX;   //(PHYSICALOFFSETX), �� 0
        int     m_nDeviceCapsPhysicalOffsetY;   //(PHYSICALOFFSETY), �� 0
        int     m_nDeviceCapsVRefresh;          //(��ֱˢ����), �� 60
        int     m_nDeviceCapsScalingFactorX;    //(SCALINGFACTORX), �� 0
        int     m_nDeviceCapsScalingFactorY;    //(SCALINGFACTORY), �� 0
        int     m_nDeviceCapsBltAlignment;      //(BLTALIGNMENT), �� 0
        int     m_nDeviceCapsShadeBlendCaps;    //(SHADEBLENDCAPS), �� 3
        int     m_nDeviceCapsRasterCaps;        //(RASTERCAPS), �� 0x7E99
        int     m_nDeviceCapsCurveCaps;         //(CURVECAPS), �� 0x1FF
        int     m_nDeviceCapsLineCaps;          //(LINECAPS), �� 0xFE
        int     m_nDeviceCapsPolygonalCaps;     //(POLYGONALCAPS), �� 0xFF
        int     m_nDeviceCapsTextCaps;          //(TEXTCAPS), �� 0x7807(TC_VA_ABLE|TC_RA_ABLE...)
        int     m_nDeviceCapsColorMgmtCaps;     //(COLORMGMTCAPS), �� 2
        COLORREF m_BkColor;
        int     m_BkMode;
        int     m_nROP2;
        int     m_nStretchBltMode;
        int     m_nPolyFillMode;

        //--��������˸ú�����ӳ�����������ϵ�ռ俪ʼ������ӳ����ҳ��ռ��н��У�
        //MSDN ������
        XFORM   m_xForm;    

        //�ı��Ķ��뷽ʽ -- TODO,��ȡ��Ϣ
        int     m_TextAlign;
        RECT    m_rcClipBox;
        POINT   m_ptCurrentPosition;

        //HBITMAP m_hCurBitmap;
        BITMAP  m_curBitmapInfo;
        //LOGPEN  m_curPenInfo;      //GetCurrentFont
    private:
        LPCTSTR m_pszProperty;
        DWORD   m_dwOldGetProperty;
    };
    
    struct BitmapProperty
    {
    public:
        FTLINLINE BitmapProperty(const BITMAP& bitmap);
        FTLINLINE ~BitmapProperty();
        FTLINLINE LPCTSTR GetPropertyString();

        BITMAP    m_bitmap;
    private:
        LPCTSTR     m_pszProperty;
    };

	//X�������ң�Y�������� -- ������ھ��ε�����λ��
	enum PointPosQuadrant
	{
		//posTop,
		posTopRight = 0,
		posRightTop,
		//posRight,
		posRightBottom,
		posBottomRight,
		//posBottom,
		posBottomLeft,
		posLeftBottom,
		//posLeft,
		posLeftTop,
		posTopLeft,
	};

    FTLEXPORT class CFGdiUtil
    {
    public:
		//����һ�����Ӧ�ض����ε���Թ�ϵ
		FTLINLINE static PointPosQuadrant CalcPointPosQuadrant(const RECT& rect, const POINT& point);

        //���㲢��ָ���ļ��(�м��)�����ı�
		FTLINLINE static int DrawInterValText(HDC hdc, LPCTSTR lpchText, int cchText,	LPRECT lprc, UINT format, POINT ptInterval);

		FTLINLINE static BOOL LoadPNGFromResource(CImage& image, HMODULE hModule, UINT nIDResource, LPCTSTR pszType = TEXT("PNG"));
        FTLINLINE static BOOL LoadPNGFromResource(Gdiplus::Image *& pImage, HINSTANCE hInstance, UINT nIDResource, LPCTSTR pszType = TEXT("PNG"));

        FTLINLINE static LPCTSTR GetGraphicsModeString(int nGraphicsMode);

        //����ģʽ -- Ӱ���п�϶��ͼԪ�еĿ�϶��ʲô�취���
        FTLINLINE static LPCTSTR GetBkModeString(int nBkMode);
        
        FTLINLINE static LPCTSTR GetMapModeString(int nMapMode);
        FTLINLINE static LPCTSTR GetMapModeDescString(int nMapMode);

        //��ͼģʽ(Raster OPeration����դ����) -- ǰ��ɫ����ԭ��ɫ���Ļ�Ϸ�ʽ
        //  ������ǰ��ͼ�ıʺ�ˢ����ɫ��pbCol�������ԭ��ͼ����ɫ��scCol�����϶��õ��������ɫ��pixel��
        FTLINLINE static LPCTSTR GetROP2String(int fnDrawMode);

        //����ģʽ,Ӱ�� StretchBlt �����Ĳ���Ч��
        FTLINLINE static LPCTSTR GetStretchBltModeString(int nStretchMode);

        //��ȡBitBlt�Ⱥ���ʹ�õ�ģʽ
        FTLINLINE static LPCTSTR GetBitBltRopString( DWORD dwRop );

        //�������䷽ʽ -- SetPolyFillMode
        FTLINLINE static LPCTSTR GetPolyFillModeString(int nPolyFillMode);

        //�������� -- ȷ��Arc��Chord��Pie�Ⱥ����Ļ�������
        FTLINLINE static LPCTSTR GetArcDirectionString(int nArcDirection);

        //��ȡRegion��Type
        FTLINLINE static LPCTSTR GetRegionType(int nType);

        FTLINLINE static LPCTSTR GetTextAlignString(int nFlags);
        FTLINLINE static BOOL    GetHDCProperty(HDC hdc, HDCProperty* pProperty);

		FTLINLINE static LPCTSTR GetTextMetricsInfo(HDC hdc, CFStringFormater& strFormater , HFONT hFont = NULL);

		//��ȡLogFont�ṹ��Ӧ����Ϣ
		FTLINLINE static LPCTSTR GetLogFontInfo(CFStringFormater& strFormater, LOGFONT* pLogFont);

		//���ݴ���� 10*���� �õ���Ӧ���ֺ�(�� ���š�С����)
		FTLINLINE static LPCTSTR GetFontNumberByPointSize(int nPointSize);
		
        //��HDC�ϻ�������ϵͳ����Ϣ(ӳ��ģʽ��9����׼�������)���Ӷ���������
        FTLINLINE static BOOL   DrawCoordinate(HDC hdc, const RECT& rcClient, BOOL bDrawText = TRUE,
            BOOL bDrawClipbox = FALSE);
		
		//ͨ���Ź���ͼƬ������Ч�����л���
		//FTLINLINE static BOOL DrawNineBlockImage(HDC hdc, )

        //���û��� һ�߼���λ��Ӧ���߼���С ����ӳ��
        enum LogicalMapMode
        {
            lmmLoEnglish,   //0.01  �߼�Ӣ�� -- SetWindowExtEx(100)
            lmmHiEnglish,   //0.001 �߼�Ӣ�� -- SetWindowExtEx(1000)
            lmmLoMetric,    //0.1   �߼����� -- SetWindowExtEx(254)
            lmmHiMetric,    //0.01  �߼����� -- SetWindowExtEx(2540)
            lmmTwips        // 1/1440�߼�Ӣ��(�߼��) ? -- SetWindowExtEx(1440)
        };
        //ע��ʵ���е� xWinExt/yWinExt �Ȳ�������ȡ����������ת�� X/Y ��ķ���
        //Ȼ���ͨ�� OffsetWindowOrg(-x,y)�ƶ�ԭ�㣬ʹ����Ļƽ����ʾ�ĸ����޵�(�μ� MFC �е� DrawClient )
        FTLINLINE static BOOL   SetLogicalMapMode(HDC hdc, LogicalMapMode logicalMapmode);

		FTLINLINE static BOOL SaveBitmapToFile(HBITMAP hBmp, LPCTSTR pszFilePath);
		FTLINLINE static BOOL SaveDCImageToFile(HDC hdc, LPCTSTR pszFilePath);

		//��λͼ��ָ����ɫ�ĵ�ȫ��ת��Ϊ�ض�����ɫ(��͸��ɫ?) -- �罫 light gray(RGB (192, 192, 192)) �滻�� ::GetSysColor (COLOR_BTNFACE)
		//  ReplaceBitmapColor(hBmp, RGB(192,192,192), ::GetSysColor (COLOR_BTNFACE));
		//http://www.codeguru.com/cpp/controls/toolbar/article.php/c2537/FullFeatured-24bit-Color-Toolbar.htm
		FTLINLINE static BOOL ReplaceBitmapColor(HBITMAP hBmp, COLORREF clrFrom, COLORREF clrTo);

        //�Ƚ�λͼ��ͼ������, ����ֵ >= 0 ��ʾ��ͬ��ɫ����Ŀ, < 0 ��ʾʧ��
        FTLINLINE static int ComapreBitmapData(int nWidth, int nHeight, int bpp, void* pBmp1, byte* pBmp2, byte* pOutResult, int nResultSize);
    };

	//����ָ���ķ�ʽ������һ��RECT��(����)����һ��SIZEʱ��RECTλ�ã�ͨ�����ڴ����ھ�����ʾͼƬ����Ƶ
	FTLEXPORT class CFCalcRect
	{
	public:
		enum ZoomMode
		{
			modeAutoFitIfBigSize,		
			modeAutoFit,					//���ݸ߿�����Զ���������
			//modeWidthFit,
			//modeHeightFit,
			//modeOriginal,
			//E_ZOOM_AUTO_FIT_IF_BIGSIZE,
			//E_ZOOM_AUTO_FIT,
			//E_ZOOM_WIDTH_FIT,
			//E_ZOOM_HEIGHT_FIT,
			//E_ZOOM_ORIGINAL,
			//E_ZOOM_NORMAL
		};
		FTLINLINE static 	CFCalcRect*	GetCalcRectObject( ZoomMode nZoomMode );
	public:
		//ֻ������ж���ʱ��ֵ���Ǿ���(�����ϽǶ���)�ķ�ʽ�ܼ�
		virtual CRect GetFitRect( const CRect& rcMargin, const CSize& szContent ) = 0; 
	};

	FTLEXPORT class CFAutoFitIfBigSizeCalcRect : public CFCalcRect
	{
	public:
		FTLINLINE virtual CRect GetFitRect( const CRect& rcMargin, const CSize& szContent );
	};

	FTLEXPORT class CFAutoFitCalcRect : public CFCalcRect
	{
	public:
		FTLINLINE virtual CRect GetFitRect( const CRect& rcMargin, const CSize& szContent );
	};

    FTLEXPORT class CFCanvas
    {
    public:
        FTLINLINE CFCanvas();
        FTLINLINE ~CFCanvas();
        FTLINLINE BOOL Create(HWND hWnd, int width, int heigth, int bpp = 32);//bpp -- bits-per-pixel
        FTLINLINE VOID Release();
        FTLINLINE int GetWidth() const { return m_width; }
        FTLINLINE int GetHeight() const { return m_height; }
        FTLINLINE int GetBpp() const { return m_bpp; }
        FTLINLINE int GetPitch() const { return m_width * m_bpp >> 3; }
        FTLINLINE BOOL  IsCanvasChanged( const RECT& rc , int bpp = 32 );
        FTLINLINE BYTE* GetBuffer() { return m_pBuffer; }
        FTLINLINE HDC   GetCanvasDC() const { return m_hCanvasDC; }
        FTLINLINE  operator HDC() const { return m_hCanvasDC; }
        FTLINLINE HBITMAP GetMemoryBitmap() const { return m_hMemBitmap; }
        FTLINLINE HANDLE CopyToHandle();
        FTLINLINE DWORD GetSize();
    private:
        HDC     m_hCanvasDC;
        HBITMAP m_hMemBitmap;
        HBITMAP m_hOldBitmap;
        union {
            BITMAPINFO  m_bmpInfo;
            BYTE m_ReserveSpace[sizeof(BITMAPINFO) + 0xFF * sizeof(RGBQUAD)];
        };
        
        BYTE*   m_pBuffer;
        int     m_width;
        int     m_height;
        int     m_bpp;
    };

    template <typename T>
    //������� ��ģ���� -- �������ʵ��
    class CClipDrawer
    {
    public:
        void OnPaint(HDC hDC);
    };

    template <typename T>
    FTLEXPORT class CFGdiResourceGuardT
    {
    public:
        FTLINLINE CFGdiResourceGuardT(HDC hdc, T hNewObject);
        FTLINLINE ~CFGdiResourceGuardT();
        BOOL      IsValide();
    private:
        HDC m_hDC;
        T   m_hOldObject;
    };

    template <typename T>
    CFGdiResourceGuardT<T>::CFGdiResourceGuardT(HDC hdc, T hNewObject)
        :m_hDC(hdc)
    {
        m_hOldObject = (T)::SelectObject(hdc, (T)hNewObject);
    }

    template <typename T>
    CFGdiResourceGuardT<T>::~CFGdiResourceGuardT()
    {
        ::SelectObject(m_hDC, m_hOldObject);
    }

    template <typename T>
    BOOL CFGdiResourceGuardT<T>::IsValide()
    {
        return (NULL != m_hOldObject && HGDI_ERROR != m_hOldObject);
    }

    //�� region ���ػ�
    //CFGdiResourceGuardT<HRGN>::IsValide()
    //{
    //}

#ifdef __ATLGDI_H__
    //ATL �ṩ�ı�׼ CMemoryDC ��֧�� Zoom �� Scroll(����λͼ������ʽû��ת��Ϊ�豸���������--λͼ�Ŀ�ߵ�λ��������)
	FTLEXPORT class CFScrollZoomMemoryDC : public CDC
    {
    public:
        // Data members
        HDC m_hDCOriginal;
        RECT m_rcPaint;
        HBITMAP m_hBmp;
        HBITMAP m_hBmpOld;
    public:
        //pRectPaint is logical units -- default is GetClipBox
        FTLINLINE CFScrollZoomMemoryDC(HDC hDC, RECT* pRectPaint);
        FTLINLINE ~CFScrollZoomMemoryDC();
        //operator CFScrollZoomMemoryDC*()
        //CFScrollZoomMemoryDC* operator->()
    };
#endif //__ATLGDI_H__

	class CFMMTextDCGuard
	{
	public:
		explicit CFMMTextDCGuard(HDC hdc)
			:m_hDC(hdc)
		{
			BOOL bRet = FALSE;
			ATLASSERT(m_hDC);
			API_VERIFY(::SetWindowOrgEx(m_hDC, 0,0, &m_ptOldWindowOrg));
			API_VERIFY(::SetViewportOrgEx(m_hDC, 0,0, &m_ptOldViewPortOrg));
			API_VERIFY(::SetViewportExtEx(m_hDC, 1,1, &m_szOldViewPortExt));
			API_VERIFY(::SetWindowExtEx(m_hDC, 1,1,&m_szOldWindowExt));
			m_nOldMapMode = ::SetMapMode(m_hDC, MM_TEXT);

			//FTLTRACE(TEXT("CFMMTextDCGuard, MapMode=%d,")
			//	TEXT("szOldWindowExt={%d,%d}, szOldViewPortExt={%d,%d},")
			//	TEXT("ptOldWindowOrg=[%d,%d], ptOldViewPortOrg=[%d,%d]\"),
			//	m_nOldMapMode,
			//	m_szOldWindowExt.cx, m_szOldWindowExt.cy, m_szOldViewPortExt.cx, m_szOldViewPortExt.cy,
			//	m_ptOldWindowOrg.x, m_ptOldWindowOrg.y,m_ptOldViewPortOrg.x, m_ptOldViewPortOrg.y);

		}
		~CFMMTextDCGuard()
		{
			BOOL bRet = FALSE;
			::SetMapMode(m_hDC, m_nOldMapMode);
			API_VERIFY(::SetWindowExtEx(m_hDC, m_szOldWindowExt.cx,m_szOldWindowExt.cy,NULL));
			API_VERIFY(::SetViewportExtEx(m_hDC, m_szOldViewPortExt.cx,m_szOldViewPortExt.cy, NULL));
			API_VERIFY(::SetViewportOrgEx(m_hDC, m_ptOldViewPortOrg.x, m_ptOldViewPortOrg.y, NULL));
			API_VERIFY(::SetWindowOrgEx(m_hDC, m_ptOldWindowOrg.x,m_ptOldWindowOrg.y, NULL));
		}
	private:
		HDC		m_hDC;
		int		m_nOldMapMode;
		SIZE	m_szOldWindowExt;
		SIZE    m_szOldViewPortExt;
		POINT   m_ptOldViewPortOrg;
		POINT   m_ptOldWindowOrg;
	};
}

#endif //FTL_GDI_H

#ifndef USE_EXPORT
#  include "ftlGdi.hpp"
#endif