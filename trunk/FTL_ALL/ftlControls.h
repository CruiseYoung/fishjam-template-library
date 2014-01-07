#ifndef FTL_CONTROLS_H
#define FTL_CONTROLS_H
#pragma once
//TODO: WTL �е� CHAIN_MSG_MAP_ALT(CRichEditCommands<CFirstRibbonView>, 1) 

#include "ftlDefine.h"

#ifndef __ATLCTRLS_H__
#  error ftlControls.h requires WTL( atlctrls.h )
#endif //__ATLCTRLS_H__
#include <atlbase.h>
#include <atlimage.h>
#include <atlgdi.h>

#include <ftlWindow.h>
#include <ftlcom.h>

#include <atlcrack.h>
#include <atlctrls.h>

//�ؼ��ڲ����� LButtonUp ʱ������ͨ�����´����򸸴��巢�͵����Ϣ
/*
//�μ� WTL.CBitmapButtonImpl.OnLButtonUp
bool bHover = IsHoverMode();
if(!bHover)
{
   lRet = DefWindowProc(uMsg, wParam, lParam);
}
if(::GetCapture() == m_hWnd)
{
	if(bHover && m_fPressed == 1)
	{
		::SendMessage(GetParent(), WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(), BN_CLICKED), (LPARAM)m_hWnd);
    }
	::ReleaseCapture();
}
*/

/***********************************************************************************************************
* http://www.functionx.com/visualc/applications/deskpad.htm
* http://blog.csdn.net/takikoyx/article/details/6311807
* ���� atlctrls �� CRichEditCtrlT Դ��֧�ָ߰汾�� MSFTEDIT_CLASS RICHEDIT_CLASS
* 
* riched32.dll -> riched20.dll/riched32.dll(ģ��) -> msftedit.dll/riched20.dll  -- 98�Ժ��ϵͳ�� riched20.dll �����ã�
* Ϊ��ʹ���°汾��RichEdit -- ͨ������ _RICHEDIT_VER �겢���� RichEdit.H �Ƿ���ԣ� afxwin ����Ϊ 0x0210
*   1.��Ҫ�ֹ�����RC�ļ��� �� RichEdit20w��RICHED20.DLL �е� 2.0 �ؼ��� -> RICHEDIT50W(Msftedit �е� 4.1 �ؼ�)
*     ��ֱ�ӷ�һ�� "Custom Control"��Ȼ��ClassName ��Ϊ RICHEDIT50W ? RichEdit20a(�ĸ��汾����
*   2.�� App �Ĺ����� LoadLibrary(_T("msftedit.dll")); ������ FreeLibrary,
*   3.�� InitInstance ����Ȼ��Ҫ���� AfxInitRichEdit2 ? -- ʵ���������Ҫ��������ʵ��Ҳ�Ǽ��� RICHED20.DLL �� RICHED32.DLL
* 
* Ĭ��ʱ�����û��л����̲���(keyboard layout -- ������Ӣ���ȣ�)�����Զ��л�����(auto font feature)����ɳ���������⣬Ϊ�˹رոù��ܣ�
*   LRESULT lres = SendMessage(hRE, EM_GETLANGOPTIONS, 0, 0);
*   lres &= ~( IMF_AUTOFONT | IMF_AUTOFONTSIZEADJUST) ;
*   SendMessage(hRE, EM_SETLANGOPTIONS, 0, lres);
*   ȱʡʱ�ǣ�IMF_AUTOFONT 
IMF_DUALFONT |   
*
* Rich Text Edit Control ( RichEdit.H, �ڱ�׼ EDIT �ؼ��Ļ�������չ )�� 
* Ϊ��ʹ��RichEdit����Ҫ�ȼ��ض�Ӧ��DLL��MFC�µ���AfxInitRichEdit ��
*                                        WTL�µ���(���ܲ���) LoadLibrary(CRichEditCtrl::GetLibraryName());
*   IID_ITextServices 
*     ϵͳ��textserv.hԤ����ģ�E_NOINTERFACE��                -- {957C25FF-0149-25FF-7895-4901FF25AC8A}
*     �Զ����ͨ��GetProcAddress(IID_ITextServices)�õ���  -- 8D33F740-CF58-11CE-A89D-00AA006CADC5
*     
+		IID_ITS         	{3B032DE9-017F-25FF-AC2A-7F01FF25B02A}	-- �᷵�� E_NOINTERFACE
+		IID_ITextServices	{357825FF-017F-25FF-AC2A-7F01FF25B02A}	_GUID


*     CreateTextServices 
*       riched20.dll!CTxtEdit::Init �л�ͨ��QI��ѯ����ӿڣ�{13E670F5-1A5A-11CF-ABEB-00AA00B65EA1}
*       MsftEdit.dll!CTxtEdit::Init                         {13E670F5-1A5A-11CF-ABEB-00AA00B65EA1}
*
* �ж���汾(���ܲ��ԣ���Ҫȷ��)
*   1.0 -- RichEd32.dll (Win95)
*   2.0 -- RICHED20.DLL (Win95/Win98), ����֧��һЩ���������԰汾��Rich Edit 1.0��֧�ֵ���Ϣ
*          (�� EM_CONVPOSITION��EM_SETIMECOLOR��EM_SETPUNCTUATION ��)
*   3.0 -- RichEd20.dll(WinXP)
*          Rich Edit 3.0֧��Hex To Unicode IME�������û�����һ�ֻ������ȼ���ʽ��ʮ�������ַ���Unicode�ַ��以����
*          Word ʹ�� Alt+x �ɽ����ǰ������ת���� UNICODE ��Ӧ��ֵ��
*   1.0��װ(Wrapper Dll for Richedit 1.0) -- riched32.dll(XP/Vista)
*  
*   4.1 -- MsftEdit.dll (WinXP SP1/Vista)
*   5.?? -- Win7(MsftEdit.dll)
*   6.1 -- Win7
*
*
*   CHARFORMAT2(EM_SETCHARFORMAT) -- �ַ���ʽ���� ���塢�ߴ硢��ɫ�Լ�����塢б��ͱ���(�����ڽ��ܵ�EN_PROTECTED��֪ͨ)��
*   PARAFORMAT2(EM_SETPARAFORMAT) -- �����ʽ���� ���롢���������ͱ��
*   ����Ƕ���COM����
*   ����:
*     ES_SELECTIONBAR -- 
*   ��Ϣ(EM_) -- 
*     EM_EXLIMITTEXT -- һ��Rich Edit�ؼ������ܰ�������32K�ֽڵ��ı���������ʹ��EM_EXLIMITTEXT��Ϣ��չ���������
*     EM_SETTEXTEX -- �����ı�����ͨ��RTFָ����ʽ����������
*     EM_SETOLEINTERFACE -- 
*   ֪ͨ��EN_) -- ע����Ҫ�� EM_SETEVENTMASK �����ܷ��͵�֪ͨ����, ��׼�ؼ��Ǹ����ڴ����޴����� ITextHost::TxNotify
*     ע�⣺���������͵�֪ͨ�� direct and delayed
*     EN_MSGFILTER -- �������еļ��̺�������룬��������Է�ֹ���̺������Ϣ������(��ITextHost�в�����,�μ�MSDN)
*     EN_SELCHANGE -- �����ڿɼ�⵱ǰѡ�����ݺ�ʱ���ı�
*     EN_REQUESTRESIZE(�޵�,bottomless) -- ���κ�ʱ���������ݴ�С�ı䣬�������丸���巢��EN_REQUESTRESIZE֪ͨ��
*        �ɵ����ؼ��Ĵ�С�Ա��������������ݴ�Сƥ�䣬��ͨ������ EM_REQUESTRESIZE ��Ϣǿ�Ʒ��͸�֪ͨ(�� WM_SIZE ʱ������)
*   ���ִ����� -- (�û���ͨ�� EM_SETWORDBREAKPROC �Զ���) ���ҵ��ʼ�ָ����Լ��жϺδ����Ի���
*   
*   �������ʽ(ע��������)
*     1.���ı���ʽ(CF_RTF)
*     2.RichEdit �ı������(CF_RETEXTOBJ)
*   ��(Stream) -- ��ؼ�������ߴ�������
*     EM_STREAMIN -- �����ݶ���ؼ�(���ݴ���)���ؼ����ظ�����Ӧ�ó���Ļص��������ú���ÿ�δ������ݵ�һ���ֵ���������
*     EM_STREAMOUT -- ����ؼ�����(���ݴ���)���ؼ����ظ�д�뻺����Ȼ�����Ӧ�ó���Ļص�������
*     
*     
* ITextHost -- �޴���
* 
* CToolTipCtrl -- 
***********************************************************************************************************/

/***********************************************************************************************************
* CListCtrl(TODO:�ϲ���MFCStudy.h��)
*   �ı��и� -- ? ON_WM_MEASUREITEM_REFLECT, �麯�� MeasureItem ������ lpMeasureItemStruct->itemHeight
*   
* ʵ�����ɫ(256ɫ)�Ĺ�����(MFC -- http://www.codeguru.com/cpp/controls/toolbar/article.php/c2537/FullFeatured-24bit-Color-Toolbar.htm)
*   1.ϵͳĬ�� -- ��16ɫ(nColorTableSize)������Ϊ IDR_MAINFRAME ��Toolbar ����Դ�����еİ�ť��bmp��ʽ���屣�棬��Դ�༭����֧�ִ���16ɫ��λͼ��
*   2.���ɫ -- ͨ��ͼ��������������Ƶ�ͼ����λͼ��ʽ���ص�ͼ���б���ͨ�� SetImageList ָ��Ϊ��������ͼ����Դ��
*     HBITMAP hBmp =  (HBITMAP)LoadImage(,MAKEINTRESOURCE(IDB_TOOLBAR), IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADMAP3DCOLORS);
*     CBitmap bmp; bmp.Attach(hBmp);
*     m_ilToolBar.Create(32,32, ILC_COLOR8, 4, 4);  //ILC_COLOR8 ������256ɫ����Ӧ�Ļ��� ILC_COLOR24��ILC_COLOR32 ��
*     m_ilToolBar.Add(&bmp,(CBitmap*)NULL);
*     m_wndToolBar.GetToolBarCtrl().SetImageList(&m_ilToolBar); //ָ��ͼ����Դ
*     ע�⣺
*       1.�´�����λͼ��Ҫ�ʹ����в����� IDR_MAINFRAME ��Դ����ͬ�Ĳ��֣��Ų�������߼�����
*       2.���ַ�ʽ�����Ĺ������ƺ����ܺܺõ�֧�� ��ֹ������  -- ��MFC�ṩ�� SetDisabledImageList/SetHotImageList ����
*       3.����ʱ������ɫΪ light gray(RGB (192, 192, 192))������ʱ�ᱻ�û�ѡ��ť����ɫ�滻 -- ��Ҫ���ó�͸���ģ�
*
* ���Ϳؼ�(CTreeViewCtrlExT)
*   ע��:
*   1.��Դ������Tree�� TVS_CHECKBOXES ���Ժ���OnInitDialog�е��� SetCheck ��Ч��
*     ԭ��Initʱ��ImageList���ǿյ�(State Icon)û�м��أ���ʼ��������Icon��Ϊ3(m_tree.GetImageList(TVSIL_STATE).GetImageCount())
*            http://blog.csdn.net/dongchongyang/article/details/9142559
*     �������:
*       ����a.ͨ��ModifyStyle���ò���(�����ͨ������ȡ��һ�Σ�����.rc���Ѿ����ó�һģһ����)
*       ����b.ָ���Զ����ͼ���б�: m_Tree.SetImageList(&m_ImageList, TVSIL_STATE);
***********************************************************************************************************/

/***********************************************************************************************************
* ͨ�� UICC.xsd ���Բ鿴 Ribbon.xml �ļ����﷨����, 
*   MSDN �İ�����http://msdn.microsoft.com/en-us/library/dd371191(VS.85).aspx
*   ����: http://www.codeproject.com/Articles/32785/Windows-7-Ribbon-The-Time-Has-Come-Your-Win32-Appl
*         
* Ribbon -- ���������������Ĺ��ܼ��ɵ������Ϸ���һϵ�б�ǩ��tabs���У����ڴ��洫ͳ�Ĳ˵����͹�����
*   CMFCRibbonCategory -- ��Tab(Core + Contextual) 
*     CMFCRibbonPanel -- ��Group
*       CMFCRibbonBaseElement -- ����Ԫ�أ���Button��CheckBox ��   
*   Gallery �ؼ����Զ������ִ��Ч������ֱ�۵�Ԥ�������Դ������û����Ը��ֲ����ҵ����ʸ�ʽ�Ĺ��̡�
*   QAT(Quick Access Toolbar) --
* 
* MFC ���ṩ�� CMFCRibbonButton��CMFCRibbonGallery ����
* WTL��û���ṩSlider?
* �� "propsys.dll;dwmapi.dll" ����Ϊ��Delay Loaded(�ӳټ���)����֤��WinXP�ϲ����� 
* 
* BEGIN_RIBBON_CONTROL_MAP -- RIBBON_CONTROL  -- END_RIBBON_CONTROL_MAP
***********************************************************************************************************/



//�������Ժ� DDX_CONTROL һ������ CWindowImpl ������
//  �磺DDX_CONTROL_IMPL(CListViewCtrl)������������һ����Ϊ CListViewCtrl_DDX ���࣬
//      ������ CListViewCtrl һ�������ǿ��Ա� DDX_CONTROL ���ܡ�

#define DDX_CONTROL_IMPL(x) \
    class x##_DDX : public CWindowImpl<x##_DDX, x> \
    { public: DECLARE_EMPTY_MSG_MAP() }; 


namespace FTL
{
	FTLEXPORT class CFControlUtil
	{
	public:
		FTLINLINE static LPCTSTR GetEditNotifyCodeString(CFStringFormater& formater, DWORD iNotify, void *pv);

		//tomeTrue �� tomFalse
		FTLINLINE static LPCTSTR GetRichEditTomBoolString(long nValue);

		//��ȡStory�����ͣ�MainText��Comments��Footnotes ��)
		FTLINLINE static LPCTSTR GetRichEditStoryTypeString(long nStoryType);
		FTLINLINE static LPCTSTR GetRichEditAnimationTypeString(long nAnimation);
		FTLINLINE static LPCTSTR GetRichEditUnderLineStyleString(long nUnderLine);
		FTLINLINE static LPCTSTR GetRichEditColorString(FTL::CFStringFormater& formater, long nColor);

		//��� RichEditCtrl �� CHARFORMAT ����Ϣ -- Ҳ������ Effect�����ֵ� CFE_ �� CFM_��
		FTLINLINE static LPCTSTR GetCharFormatEffectAndMaskString(FTL::CFStringFormater& formater, DWORD dwMasks, LPCTSTR pszDivide = TEXT("|"));

		//��� RichEditCtrl �� PARAFORMAT ����Ϣ -- Ҳ����Effect?
		FTLINLINE static LPCTSTR GetParaFormatEffectAndMaskString(FTL::CFStringFormater& formater, DWORD dwMasks, LPCTSTR pszDivide = TEXT("|"));

		//��� RichEditCtrl �е� PropertyBits ��Ϣ(TxGetPropertyBits)
		FTLINLINE static LPCTSTR GetRichEditPropertyBits(FTL::CFStringFormater& formater, DWORD dwBits, LPCTSTR pszDivide = TEXT("|"));

		//����ListBoxˮƽ������Χ
		FTLINLINE static BOOL UpdateListboxHorizontalExtent(HWND hwndListbox, INT nPadding = 0);

        //��CheckBox��TreeCtrl�������ڵ�ѡ��ʱ��ͬʱ������ȫ���ӽڵ�
        //�ݹ������ѡ�����������޸�
        FTLINLINE static BOOL CheckTreeSubItems(HWND hWndTree, HTREEITEM hParent, BOOL bCheck);
	};

	//RichEdit �е� ITextRange
	class CFTextRangeDumper : public CFInterfaceDumperBase<CFTextRangeDumper>
	{
		DISABLE_COPY_AND_ASSIGNMENT(CFTextRangeDumper);
	public:
		FTLINLINE explicit CFTextRangeDumper(IUnknown* pObj, IInformationOutput* pInfoOutput, int nIndent)
			:CFInterfaceDumperBase<CFTextRangeDumper>(pObj, pInfoOutput, nIndent){}
	public:
		FTLINLINE HRESULT GetObjInfo(IInformationOutput* pInfoOutput);
	};

	class CFTextFontDumper : public CFInterfaceDumperBase<CFTextFontDumper>
	{
		DISABLE_COPY_AND_ASSIGNMENT(CFTextFontDumper);
	public:
		FTLINLINE explicit CFTextFontDumper(IUnknown* pObj, IInformationOutput* pInfoOutput, int nIndent)
			:CFInterfaceDumperBase<CFTextFontDumper>(pObj, pInfoOutput, nIndent){}
	public:
		FTLINLINE HRESULT GetObjInfo(IInformationOutput* pInfoOutput);
	};

	//��ʾ�ļ���·���Ĵ��� -- ��ʾ�ɴ�ʡ�Ժŵģ������� Tooltip
	//δд��Ͳ���
	class CFileNameWnd : public CWindowImpl<CFileNameWnd>
	{
	public:
		BEGIN_MSG_MAP(CFileNameWnd)
			MESSAGE_HANDLER(WM_CREATE, OnCreate)
		END_MSG_MAP()
	protected:
		LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
		{
			m_tooltip.Create(m_hWnd);
			//CToolInfo ti(0, m_hWnd, m_nToolTipID, &rcClient, NULL);
			//DrawText �� DT_CALCRECT �ɼ���Rect�ķ���ֵ��ȷ���Ƿ�Ҫ Tooltip::UpdateTipText �� Active
			//���ڿ��� DT_PATH_ELLIPSIS ���ƣ�
			//�����崦�������Ϣ���� Tooltip ���� if(m_tooltip.IsWindow()) { m_tooltip.RelayEvent(...); }
		}
		
	private:
		CToolTipCtrl	m_tooltip;
	};

	//�Զ����ִ�ֱ�������� EDIT����Ҫ�Ƚ���������Ϊ�� ES_MULTILINE | ES_AUTOVSCROLL | ES_WANTRETURN
	class CFEditEx : public CWindowImpl<CFEditEx, CEdit> //<ATL::CWindowImpl>
	{
		enum  {	UWM_CHECKTEXT = WM_USER + 100 };
	public:
		FTLINLINE CFEditEx(void);
		FTLINLINE ~CFEditEx(void);
	public:
		BEGIN_MSG_MAP_EX(CFEditEx)
			//DUMP_WINDOWS_MSG(__FILE__LINE__, DEFAULT_DUMP_FILTER_MESSAGE | DUMP_FILTER_KEYDOWN, uMsg, wParam, lParam)
			MSG_WM_CHAR(OnChar)
			MESSAGE_HANDLER(UWM_CHECKTEXT, OnCheckText)
		END_MSG_MAP()
	private:
		FTLINLINE void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
		FTLINLINE LRESULT OnCheckText(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

		FTLINLINE void ShowHorizScrollBar(BOOL bShow = TRUE);
		FTLINLINE void ShowVertScrollBar(BOOL bShow = TRUE);
	private:
		BOOL m_bShowHoriz;
		BOOL m_bShowVert;
	};

    class CCheckedTreeViewCtrlEx : public CTreeViewCtrlEx
    {
    public:
        CCheckedTreeViewCtrlEx();
        ~CCheckedTreeViewCtrlEx();
    public:
    };

    struct FScrollSkinInfo
    {
        CImage* pBtnVUp;
        CImage* pBtnVDown;
    };

    class CFSkinComboBox : public CWindowImpl<CFSkinComboBox, CComboBox>
    {
    public:
        CFSkinComboBox()
        {
            m_hWndList = NULL;
            m_pSkinInfo = NULL;
        }
        virtual ~CFSkinComboBox()
        {

        }

        BEGIN_MSG_MAP_EX(CFSkinComboBox)
#ifdef FTL_DEBUG
            //DUMP_WINDOWS_MSG(__FILE__LINE__, DEFAULT_DUMP_FILTER_MESSAGE, uMsg, wParam, lParam)
#endif 
            REFLECTED_COMMAND_CODE_HANDLER_EX(CBN_DROPDOWN, OnCbnDropDown)
            REFLECTED_COMMAND_CODE_HANDLER_EX(CBN_CLOSEUP, OnCbnCloseup)
            MSG_WM_CTLCOLORLISTBOX(OnCtlColorListBox)
            //MSG_WM_CREATE(OnCreate)
        END_MSG_MAP()

        FTLINLINE LRESULT OnCbnDropDown(UINT uNotifyCode, int nID, CWindow wndCtl);
        FTLINLINE LRESULT OnCbnCloseup(UINT uNotifyCode, int nID, CWindow wndCtl);
        FTLINLINE HBRUSH OnCtlColorListBox(CDCHandle dc, CListBox listBox);
        FTLINLINE BOOL SkinScroll(const FScrollSkinInfo* pSkinInfo);
        FTLINLINE void UnSkinScroll();
    protected:
        HWND	m_hWndList;
        const FScrollSkinInfo*    m_pSkinInfo;
    };

    //template </*class T,*/ class TBase = WTL::CScrollBar, class TWinTraits = ATL::CControlWinTraits>
    class CFSkinScrollBar : public ATL::CWindowImpl<CFSkinScrollBar, WTL::CScrollBar>
    {
    public:
        BEGIN_MSG_MAP_EX(CFSkinScrollBar)
            MSG_WM_CREATE(OnCreate)
            //MSG_WM_ERASEBKGND(OnEraseBkgnd)
            //CHAIN_MSG_MAP(CZoomScrollWindowImpl<CScrollImageViewView>) // CScrollWindowImpl<CScrollImageViewView>)
        END_MSG_MAP()
    public:
        FTLINLINE CFSkinScrollBar();
        FTLINLINE ~CFSkinScrollBar();
        FTLINLINE int OnCreate(LPCREATESTRUCT lpCreateStruct);
    private:
        FScrollSkinInfo    m_SkinInfo;
    };

    class CFSkinScrollWindow : public ATL::CWindowImpl<CFSkinScrollWindow, ATL::CWindow>
    {
    public:
        FTLINLINE CFSkinScrollWindow();
        FTLINLINE virtual ~CFSkinScrollWindow();

        BEGIN_MSG_MAP_EX(CFSkinScrollWindow)
        END_MSG_MAP()
    public:
        FTLINLINE BOOL SkinScroll(HWND hWnd, const FScrollSkinInfo* pSkinInfo);
        FTLINLINE void UnSkinScroll();
    public:
        int                 m_nScrollWidth;
        CFSkinScrollBar     m_ScrollBarVert;
        CFSkinScrollBar     m_ScrollBarHorz;
        ATL::CWindow        m_wndMiddle;
        WNDPROC             m_funOldProc;
        FTLINLINE static LRESULT CALLBACK HookWndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
    };

    extern "C" FTLINLINE CFSkinScrollWindow* SkinWndScroll(HWND hWnd, const FScrollSkinInfo* pScrollSkinInfo);
    extern "C" FTLINLINE BOOL UnSkinWndScroll(HWND  hWnd);
}

#endif //FTL_CONTROLS_H

#ifndef USE_EXPORT
#  include "ftlControls.hpp"
#endif