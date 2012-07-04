#ifndef FTL_CONTROLS_H
#define FTL_CONTROLS_H
#pragma once

#include "ftlDefine.h"

#ifndef __ATLCTRLS_H__
#  error ftlControls.h requires WTL( atlctrls.h )
#endif //__ATLCTRLS_H__
#include <atlbase.h>
#include <atlimage.h>

#include <ftlWindow.h>
#include <ftlcom.h>

#include <atlcrack.h>

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
* Rich Edit Control ( RichEdit.H, �ڱ�׼ EDIT �ؼ��Ļ�������չ )�� 
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
*       msftedit.dll!CTxtEdit::Init                         {13E670F5-1A5A-11CF-ABEB-00AA00B65EA1}
*
* �ж���汾(���ܲ��ԣ���Ҫȷ��)
*   1.0 -- RichEd32.dll (Win95)
*   2.0 -- RICHED20.DLL (Win95/Win98), ����֧��һЩ���������԰汾��Rich Edit 1.0��֧�ֵ���Ϣ
*          (�� EM_CONVPOSITION��EM_SETIMECOLOR��EM_SETPUNCTUATION ��)
*   3.0 -- 
*          Rich Edit 3.0֧��Hex To Unicode IME�������û�����һ�ֻ������ȼ���ʽ��ʮ�������ַ���Unicode�ַ��以����
*          Word ʹ�� Alt+x �ɽ����ǰ������ת���� UNICODE ��Ӧ��ֵ��
*   4.1 -- Msftedit.DLL (WinXP SP1)
*   6.1 -- Win7 
*
*
*   CHARFORMAT2(EM_SETCHARFORMAT) -- �ַ���ʽ���� ���塢�ߴ硢��ɫ�Լ�����塢б��ͱ���(�����ڽ��ܵ�EN_PROTECTED��֪ͨ)��
*   PARAFORMAT2(EM_SETPARAFORMAT) -- �����ʽ���� ���롢���������ͱ��
*   ����Ƕ���COM����
*   ��Ϣ(EM_) -- 
*     EM_EXLIMITTEXT -- һ��Rich Edit�ؼ������ܰ�������32K�ֽڵ��ı���������ʹ��EM_EXLIMITTEXT��Ϣ��չ���������
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
		CFEditEx(void);
		~CFEditEx(void);
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