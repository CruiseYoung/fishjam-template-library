#ifndef FTL_WINDOW_HPP
#define FTL_WINDOW_HPP
#pragma once

#ifdef USE_EXPORT
#  include "ftlwindow.h"
#endif

#include <prsht.h>
#include <CommCtrl.h>
#include <zmouse.h>
#include <ftlConversion.h>
#include <ftlNLS.h>

namespace FTL
{

#define GET_MESSAGE_INFO_ENTRY(msg, classMsgInfo ) \
	case (msg): { StringCchCopy(m_bufInfo, _countof(m_bufInfo), classMsgInfo().GetMsgInfo(msg, TEXT(#msg), m_wParam, m_lParam)); break; }

    class CFDefaultMsgInfo
    {
    public:
		virtual LPCTSTR GetMsgInfo(UINT /*uMsg*/, LPCTSTR pszMsgName, WPARAM /*wParam*/, LPARAM /*lParam*/)
		{
			return pszMsgName;
		}
	protected:
		CFStringFormater	m_strFormater;
    };

	class CFMoveMsgInfo : public CFDefaultMsgInfo
	{
	public:
		virtual LPCTSTR GetMsgInfo(UINT uMsg, LPCTSTR pszMsgName, WPARAM wParam, LPARAM lParam)
		{
			FTLASSERT(WM_MOVE == uMsg);
			UNREFERENCED_PARAMETER(uMsg);
			
			HRESULT hr = E_FAIL;
			int xPos = (int)(short) LOWORD(lParam);   // horizontal position 
			int yPos = (int)(short) HIWORD(lParam);   // vertical position
			COM_VERIFY(m_strFormater.Format(TEXT("%s{xPos=%d, yPos=%d }"), 
				pszMsgName, xPos, yPos));
			return m_strFormater;
		}
	};
	class CFWindowPosMsgInfo : public CFDefaultMsgInfo
	{
	public:
		virtual LPCTSTR GetMsgInfo(UINT uMsg, LPCTSTR pszMsgName, WPARAM wParam, LPARAM lParam)
		{
			FTLASSERT(WM_WINDOWPOSCHANGING == uMsg || WM_WINDOWPOSCHANGED == uMsg);
			UNREFERENCED_PARAMETER(uMsg);
	  
			HRESULT hr = E_FAIL;
			WINDOWPOS* pWindowPos = (WINDOWPOS*)lParam;
			FTLASSERT(pWindowPos);
			if (pWindowPos)
			{
				CFStringFormater formaterFlags;
				COM_VERIFY(m_strFormater.Format(TEXT("%s{hWnd=0x%x, hwndInsertAfter=0x%x,")
					TEXT("Pos=(%d,%d)-(%d,%d), %dx%d, flags=0x%x(%s) }"), 
					pszMsgName, pWindowPos->hwnd, pWindowPos->hwndInsertAfter,
					pWindowPos->x, pWindowPos->y, pWindowPos->x + pWindowPos->cx, pWindowPos->y + pWindowPos->cy,
					pWindowPos->cx, pWindowPos->cy,
 					pWindowPos->flags, CFWinUtil::GetWindowPosFlagsString(formaterFlags, pWindowPos->flags)));
			}
			return m_strFormater;
		}
	};

	class CFScrollMsgInfo : public CFDefaultMsgInfo
	{
	public:
		virtual LPCTSTR GetMsgInfo(UINT uMsg, LPCTSTR pszMsgName, WPARAM wParam, LPARAM lParam)
		{
            FTLASSERT(WM_VSCROLL == uMsg || WM_HSCROLL == uMsg);
            UNREFERENCED_PARAMETER(uMsg);

            HRESULT hr = E_FAIL;
			UINT nSBCode = LOWORD(wParam);
			UINT nPos = HIWORD(wParam);
			//nSBCode Ϊ SB_THUMBPOSITION �� SB_THUMBTRACK ʱ, nPos �������壬����ʱ��(�� SB_ENDSCROLL)��ֵ������ 
			COM_VERIFY(m_strFormater.Format(TEXT("%s{nSBCode=%s, nPos=%d, lParam(HWND)=0x%x }"), 
				pszMsgName, CFWinUtil::GetScrollBarCodeString(nSBCode), nPos, lParam));
			return m_strFormater;
		}
	};

	class CFCommandMsgInfo: public CFDefaultMsgInfo
	{
	public:
		virtual LPCTSTR GetMsgInfo(UINT uMsg, LPCTSTR pszMsgName, WPARAM wParam, LPARAM lParam)
		{
			FTLASSERT(WM_COMMAND == uMsg);
			UNREFERENCED_PARAMETER(uMsg);

			BOOL bRet = FALSE;
			WORD wNotifyCode = HIWORD(wParam);
			WORD wID = LOWORD(wParam);
			HWND hWndCtrl = HWND(lParam);

			TCHAR szCommandMsgInfo[256] = {0};
			if (NULL == hWndCtrl) 
			{
				//Menu or Accelerator
				m_strFormater.Format(TEXT("%s{ Menu or Accelerator, wID=%d(0x%x)"), pszMsgName, wID, wID);
			}
			else
			{
				//Control
				TCHAR szClassName[FTL_MAX_CLASS_NAME_LENGTH] = {0};
				API_VERIFY(0 != GetClassName(hWndCtrl, szClassName, _countof(szClassName)));

				CFWinUtil::GetCommandNotifyString(hWndCtrl, wNotifyCode, szCommandMsgInfo, _countof(szCommandMsgInfo));
				m_strFormater.Format(TEXT("%s{ Control(\"%s\") wID=%d(0x%x), NotifyCode=%s(%d, 0x%x), hWndCtrl=0x%x }"),
					pszMsgName, szClassName, wID, wID, szCommandMsgInfo, wNotifyCode, wNotifyCode, hWndCtrl);
			}
			return m_strFormater;
		}
	};

	class CFNotifyMsgInfo: public CFDefaultMsgInfo
	{
	public:
		virtual LPCTSTR GetMsgInfo(UINT uMsg, LPCTSTR pszMsgName, WPARAM wParam, LPARAM lParam)
		{
			FTLASSERT(WM_NOTIFY == uMsg);
			BOOL bRet = FALSE;

			int nIdCtrl = (int)wParam;
			LPNMHDR pNmHdr = (LPNMHDR)lParam;

			TCHAR szClassName[FTL_MAX_CLASS_NAME_LENGTH] = {0};
			API_VERIFY(0 != GetClassName(pNmHdr->hwndFrom, szClassName, _countof(szClassName)));

			TCHAR szNotifyCodeString[MAX_PATH] = {0};

			m_strFormater.Format(TEXT("%s{ Control(\"%s\"), idCtrl=%d(0x%x), idFrom=%d(0x%x), code=%s(%d, 0x%x), hwndFrom=0x%x }"),
				pszMsgName, szClassName, nIdCtrl, nIdCtrl, pNmHdr->idFrom, pNmHdr->idFrom,
				CFWinUtil::GetNotifyCodeString(pNmHdr->hwndFrom, pNmHdr->code, szNotifyCodeString, _countof(szNotifyCodeString), NULL),
				pNmHdr->code, pNmHdr->code, pNmHdr->hwndFrom);

			return m_strFormater;
		}
	};

#ifdef _RICHEDIT_
	//RichEdit �в���ʹ�� WM_USER ����Ϣ
	class CFRichEditCtrlMsgInfo : public CFDefaultMsgInfo
	{
		virtual LPCTSTR GetMsgInfo(UINT uMsg, LPCTSTR /*pszMsgName*/, WPARAM /*wParam*/, LPARAM /*lParam*/)
		{
			switch (uMsg)
			{
				//TODO:��ȫ������û��ʹ�������
				HANDLE_CASE_TO_STRING_FORMATER(m_strFormater, EM_CANPASTE);//�����ؼ��Ƿ�ʶ��ĳ��ָ���ĸ�ʽ

				//ͨ������ʹ��EM_FORMATRANGE��EM_DISPLAYBAND��Ϣ����ӡRich Text�ؼ����ݵ�Ӧ�ó������ʵ������������
				//������������ָ���ǽ�����ָ�Ϊ��С�������ڴ�ӡĿ�ĵĲ�������
				HANDLE_CASE_TO_STRING_FORMATER(m_strFormater, EM_DISPLAYBAND);//������豸�ı���ʽ����ɺ󣬽�����������豸

				//ѡ������ָ����ѡ���ַ��ķ�Χ�����߱�ʾû���ַ�ѡ��ʱ�Ĳ����λ��
				HANDLE_CASE_TO_STRING_FORMATER(m_strFormater, EM_EXGETSEL);//ȷ����ǰѡ������
				HANDLE_CASE_TO_STRING_FORMATER(m_strFormater, EM_EXLIMITTEXT);//
				HANDLE_CASE_TO_STRING_FORMATER(m_strFormater, EM_EXLINEFROMCHAR);//�жϸ����ַ�������һ��
				HANDLE_CASE_TO_STRING_FORMATER(m_strFormater, EM_EXSETSEL);//���õ�ǰѡ������
				HANDLE_CASE_TO_STRING_FORMATER(m_strFormater, EM_FINDTEXT);//
				HANDLE_CASE_TO_STRING_FORMATER(m_strFormater, EM_FORMATRANGE);//���������豸���ԣ�Ҫ��ʽ��Rich Edit�ؼ��в�������
				HANDLE_CASE_TO_STRING_FORMATER(m_strFormater, EM_GETCHARFORMAT);//
				HANDLE_CASE_TO_STRING_FORMATER(m_strFormater, EM_GETEVENTMASK);//
				HANDLE_CASE_TO_STRING_FORMATER(m_strFormater, EM_GETOLEINTERFACE);//�ӿؼ���ȡһ��IRichEditOle�ӿڣ��ýӿ�����������OLE����
				HANDLE_CASE_TO_STRING_FORMATER(m_strFormater, EM_GETPARAFORMAT);//
				HANDLE_CASE_TO_STRING_FORMATER(m_strFormater, EM_GETSELTEXT);//��ȡRich Edit�ؼ��е�ѡ���ı�(�������ַ�������)
				HANDLE_CASE_TO_STRING_FORMATER(m_strFormater, EM_HIDESELECTION);//
				HANDLE_CASE_TO_STRING_FORMATER(m_strFormater, EM_PASTESPECIAL);//ճ��ָ���ļ������ʽ
				HANDLE_CASE_TO_STRING_FORMATER(m_strFormater, EM_REQUESTRESIZE);//
				HANDLE_CASE_TO_STRING_FORMATER(m_strFormater, EM_SELECTIONTYPE);//��ȡ��ǰѡ�����ݵ������Ϣ
				HANDLE_CASE_TO_STRING_FORMATER(m_strFormater, EM_SETBKGNDCOLOR);//
				HANDLE_CASE_TO_STRING_FORMATER(m_strFormater, EM_SETCHARFORMAT);//
				HANDLE_CASE_TO_STRING_FORMATER(m_strFormater, EM_SETEVENTMASK);//����EventMask(�� EN_PROTECTED)
				HANDLE_CASE_TO_STRING_FORMATER(m_strFormater, EM_SETOLECALLBACK);//
				HANDLE_CASE_TO_STRING_FORMATER(m_strFormater, EM_SETPARAFORMAT);//

				//ָ��һ�������ı���ʽ����Ŀ���豸, ����Ϣ����WYSIWYG�����������ã�ģʽ�ǳ����ã�
				//  �ڸ�ģʽ��Ӧ�ó������Ĭ�ϴ�ӡ�������������Ļ����������λ�ı�
				//  �����ڿ��� RichEdit ���Զ����й���(�Զ�����ʱ Line width Ϊ 0, TODO:�Ƿ�������������?)��
				//    http://my.oschina.net/wangjijian/blog/9416?from=rss
				HANDLE_CASE_TO_STRING_FORMATER(m_strFormater, EM_SETTARGETDEVICE);

				HANDLE_CASE_TO_STRING_FORMATER(m_strFormater, EM_STREAMIN);//
				HANDLE_CASE_TO_STRING_FORMATER(m_strFormater, EM_STREAMOUT);//
				HANDLE_CASE_TO_STRING_FORMATER(m_strFormater, EM_GETTEXTRANGE);//
				HANDLE_CASE_TO_STRING_FORMATER(m_strFormater, EM_FINDWORDBREAK);//���Ҷ��ַ�����ȷ��һ���ַ���Ͷ��ֱ�־λ
				HANDLE_CASE_TO_STRING_FORMATER(m_strFormater, EM_SETOPTIONS);//
				HANDLE_CASE_TO_STRING_FORMATER(m_strFormater, EM_GETOPTIONS);//
				HANDLE_CASE_TO_STRING_FORMATER(m_strFormater, EM_FINDTEXTEX);//�����ַ���
				HANDLE_CASE_TO_STRING_FORMATER(m_strFormater, EM_GETWORDBREAKPROCEX);//
				HANDLE_CASE_TO_STRING_FORMATER(m_strFormater, EM_SETWORDBREAKPROCEX);//�Զ���"���ִ�����"

				// RichEdit 2.0 messages 
				HANDLE_CASE_TO_STRING_FORMATER(m_strFormater, EM_SETUNDOLIMIT);//����Undo���е��������Ŀ
				HANDLE_CASE_TO_STRING_FORMATER(m_strFormater, EM_REDO);//����Redo�����е���һ����
				HANDLE_CASE_TO_STRING_FORMATER(m_strFormater, EM_CANREDO);//�ж��Ƿ���Redo��������һЩ����
				HANDLE_CASE_TO_STRING_FORMATER(m_strFormater, EM_GETUNDONAME);//��ȡUndo�����е���һ��������������
				HANDLE_CASE_TO_STRING_FORMATER(m_strFormater, EM_GETREDONAME);//��ȡRedo�����е���һ��������������
				HANDLE_CASE_TO_STRING_FORMATER(m_strFormater, EM_STOPGROUPTYPING);//��ֹ��ǰUndo�������������붯�������
				HANDLE_CASE_TO_STRING_FORMATER(m_strFormater, EM_SETTEXTMODE);//�����ı�ģʽ����Undo����
				HANDLE_CASE_TO_STRING_FORMATER(m_strFormater, EM_GETTEXTMODE);//��ȡ�ı�ģʽ����Undo����
				HANDLE_CASE_TO_STRING_FORMATER(m_strFormater, EM_AUTOURLDETECT);//�Ƿ���/�ر��Զ�URL���


				HANDLE_CASE_TO_STRING_FORMATER(m_strFormater, EM_SETLANGOPTIONS);//����IME��Զ������֧��ѡ��
				HANDLE_CASE_TO_STRING_FORMATER(m_strFormater, EM_GETLANGOPTIONS);//��ȡIME��Զ������֧��ѡ��
				HANDLE_CASE_TO_STRING_FORMATER(m_strFormater, EM_GETIMECOMPMODE);//��ȡ��ǰ���뷽ʽ�༭(IME)ģʽ
				break;
			}
			//m_strFormater.Format(TEXT("%s{nFlag=%d(0x%x), Pos=[%d,%d]}"), pszMsgName, nFlags, nFlags, xPos, yPos );
			return m_strFormater;
		}
	};
#endif //_RICHEDIT_

	class CFMouseMsgInfo : public CFDefaultMsgInfo
	{
	public:
		virtual LPCTSTR GetMsgInfo(UINT /*uMsg*/, LPCTSTR pszMsgName, WPARAM wParam, LPARAM lParam)
		{
			UINT nFlags = (UINT)wParam;
			WORD xPos = LOWORD(lParam);
			WORD yPos = HIWORD(lParam);
			
			m_strFormater.Format(TEXT("%s{nFlag=%d(0x%x), Pos=[%d,%d]}"), pszMsgName, nFlags, nFlags, xPos, yPos );
			return m_strFormater;
		}
	};

	//������ PreTranslateMessage ���ж� WM_KEYDOWN ��Ϣ��ת������
	class CFKeyMsgInfo : public CFDefaultMsgInfo
	{
	public:
		virtual LPCTSTR GetMsgInfo(UINT /*uMsg*/, LPCTSTR pszMsgName, WPARAM wParam, LPARAM lParam)
		{
			TCHAR nChar = (TCHAR)wParam;

			UINT nRepCnt = LOWORD(lParam);
			UINT nFlags = HIWORD(lParam);
			
			m_strFormater.Format(TEXT("%s{nChar=0x%x(%s), nRepCnt=%d, nFlags=0x%x}"), pszMsgName, 
				nChar, CFWinUtil::GetVirtualKeyString(nChar) , nRepCnt, nFlags);
			return m_strFormater;
		}
	};

	class CFPowerBroadcastMsgInfo : public  CFDefaultMsgInfo
	{
	public:
		virtual LPCTSTR GetMsgInfo(UINT /*uMsg*/, LPCTSTR pszMsgName, WPARAM wParam, LPARAM lParam)
		{
			UNREFERENCED_PARAMETER(lParam);

			TCHAR szInfo[40] = {0};
			UINT nEvent = (UINT)wParam;
			switch (nEvent)
			{
				HANDLE_CASE_TO_STRING(szInfo, _countof(szInfo), PBT_APMQUERYSUSPEND);
				HANDLE_CASE_TO_STRING(szInfo, _countof(szInfo), PBT_APMQUERYSTANDBY);
				HANDLE_CASE_TO_STRING(szInfo, _countof(szInfo), PBT_APMQUERYSUSPENDFAILED);
				HANDLE_CASE_TO_STRING(szInfo, _countof(szInfo), PBT_APMQUERYSTANDBYFAILED);
				HANDLE_CASE_TO_STRING(szInfo, _countof(szInfo), PBT_APMSUSPEND);

				HANDLE_CASE_TO_STRING(szInfo, _countof(szInfo), PBT_APMSTANDBY);
				HANDLE_CASE_TO_STRING(szInfo, _countof(szInfo), PBT_APMRESUMECRITICAL);
				HANDLE_CASE_TO_STRING(szInfo, _countof(szInfo), PBT_APMRESUMESUSPEND);
				HANDLE_CASE_TO_STRING(szInfo, _countof(szInfo), PBT_APMRESUMESTANDBY);
				HANDLE_CASE_TO_STRING(szInfo, _countof(szInfo), PBT_APMBATTERYLOW);
				HANDLE_CASE_TO_STRING(szInfo, _countof(szInfo), PBT_APMPOWERSTATUSCHANGE);
				HANDLE_CASE_TO_STRING(szInfo, _countof(szInfo), PBT_APMOEMEVENT);
				HANDLE_CASE_TO_STRING(szInfo, _countof(szInfo), PBT_APMRESUMEAUTOMATIC);

#if (_WIN32_WINNT >= 0x0502)
				//case PBT_POWERSETTINGCHANGE:
				{
					//TODO:POWERBROADCAST_SETTING 
					HANDLE_CASE_TO_STRING(szInfo, _countof(szInfo), PBT_POWERSETTINGCHANGE);
					break;
				}
#endif // (_WIN32_WINNT >= 0x0502)
			default:
				FTLASSERT(FALSE);
				StringCchPrintf(szInfo, _countof(szInfo), TEXT("Unknown-%d"), nEvent);
				break;
			}
			m_strFormater.Format(TEXT("%s{%s}"), pszMsgName, szInfo);
			return m_strFormater;
		}
	};

	class CFIMENotifyMsgInfo : public CFDefaultMsgInfo
	{
	public:
		virtual LPCTSTR GetMsgInfo(UINT /*uMsg*/, LPCTSTR pszMsgName, WPARAM wParam, LPARAM lParam)
		{
			//UINT nCommand = (UINT)wParam;
			CFStringFormater formater;
			
			m_strFormater.Format(TEXT("%s{NotifyInfo=%s}"),pszMsgName, CFIMEUtil::GetIMENotifyInfoString(formater, wParam, lParam));
			return m_strFormater;
		}
	};

	class CFIMECompositionMsgInfo : public CFDefaultMsgInfo
	{
	public:
		virtual LPCTSTR GetMsgInfo(UINT /*uMsg*/, LPCTSTR pszMsgName, WPARAM wParam, LPARAM lParam)
		{
			//DBCS
			LPCSTR pszLastChange = (LPCSTR)wParam;
			CFStringFormater strLastChange;
			if (pszLastChange)
			{
				 CFConversion cov;
				 strLastChange.Format(TEXT("%s"), cov.MBCS_TO_TCHAR(pszLastChange));
			}
			CFStringFormater changeTypeFormater;
			m_strFormater.Format(
				TEXT("%s{pszLastChange=%s, changeType=%s}"),
				pszMsgName, 
				strLastChange, CFIMEUtil::GetCompositionValueString(changeTypeFormater, (UINT)lParam, TEXT("|"))
			);
			return m_strFormater;
		}
	};

	class CFIMECharMsgInfo : public CFDefaultMsgInfo
	{
	public:
		virtual LPCTSTR GetMsgInfo(UINT /*uMsg*/, LPCTSTR pszMsgName, WPARAM wParam, LPARAM lParam)
		{
			TCHAR nChar = (TCHAR)wParam;
			TCHAR nPrintChar = nChar;
			if(!IsCharAlphaNumeric(nPrintChar))
			{
				nPrintChar = TEXT('.');
			}

			UINT nRepCnt = LOWORD(lParam);
			UINT nFlags = HIWORD(lParam);

			m_strFormater.Format(TEXT("%s{nChar=0x%x(%c), nRepCnt=%d, nFlags=0x%x}"),pszMsgName, nChar, nPrintChar, nRepCnt, nFlags);
			return m_strFormater;
		}
	};

#if(_WIN32_WINNT >= 0x0501)
	class CFWtsSessionChangeMsgInfo : public CFDefaultMsgInfo
	{
	public:
		virtual LPCTSTR GetMsgInfo(UINT /*uMsg*/, LPCTSTR pszMsgName, WPARAM wParam, LPARAM lParam)
		{
			UNREFERENCED_PARAMETER(lParam);

			TCHAR szInfo[40] = {0};
			INT nReason = (INT)wParam;
			switch (nReason)
			{
				HANDLE_CASE_TO_STRING(szInfo, _countof(szInfo), WTS_CONSOLE_CONNECT);
				HANDLE_CASE_TO_STRING(szInfo, _countof(szInfo), WTS_CONSOLE_DISCONNECT);
				HANDLE_CASE_TO_STRING(szInfo, _countof(szInfo), WTS_REMOTE_CONNECT);
				HANDLE_CASE_TO_STRING(szInfo, _countof(szInfo), WTS_REMOTE_DISCONNECT);
				HANDLE_CASE_TO_STRING(szInfo, _countof(szInfo), WTS_SESSION_LOGON);
				HANDLE_CASE_TO_STRING(szInfo, _countof(szInfo), WTS_SESSION_LOGOFF);
				HANDLE_CASE_TO_STRING(szInfo, _countof(szInfo), WTS_SESSION_LOCK);
				HANDLE_CASE_TO_STRING(szInfo, _countof(szInfo), WTS_SESSION_UNLOCK);
				HANDLE_CASE_TO_STRING(szInfo, _countof(szInfo), WTS_SESSION_REMOTE_CONTROL);
			default:
				FTLASSERT(FALSE);
				StringCchPrintf(szInfo, _countof(szInfo), TEXT("Unknown-%d"), nReason);
				break;
			}
			m_strFormater.Format(TEXT("%s{%s}"), pszMsgName, szInfo);
			return m_strFormater;
		}
	};
#endif /* _WIN32_WINNT >= 0x0501 */

	///////////////////////////////////////////////////////////////////////////////////////////////////
	CFRegistedMessageInfo::CFRegistedMessageInfo()
	{
		m_bInited = FALSE;

		//ZeroMemory(&m_bInited, sizeof(CFRegistedMessageMgr) - offsetofclass())
	}
	BOOL CFRegistedMessageInfo::Init()
	{
		if (!m_bInited)
		{
			m_bInited = TRUE;

			RWM_ATL_CREATE_OBJECT	= RegisterWindowMessage(TEXT("ATL_CREATE_OBJECT"));
			RWM_ATLGETCONTROL		= RegisterWindowMessage(TEXT("WM_ATLGETCONTROL"));
			RWM_ATLGETHOST			= RegisterWindowMessage(TEXT("WM_ATLGETHOST"));
			RWM_COLOROKSTRING		= RegisterWindowMessage(COLOROKSTRING);
			RWM_COMMDLG_FIND		= RegisterWindowMessage(TEXT("COMMDLG_FIND"));
			RWM_FILEOKSTRING		= RegisterWindowMessage(FILEOKSTRING);
			RWM_FINDMSGSTRING		= RegisterWindowMessage(FINDMSGSTRING);
			RWM_LBSELCHSTRING		= RegisterWindowMessage(LBSELCHSTRING);
			RWM_MSH_MOUSEWHEEL		= RegisterWindowMessage(MSH_MOUSEWHEEL);
			RWM_MSH_WHEELSUPPORT	= RegisterWindowMessage(MSH_WHEELSUPPORT);
			RWM_MSH_SCROLL_LINES	= RegisterWindowMessage(MSH_SCROLL_LINES);
			RWM_HELPMSGSTRING		= RegisterWindowMessage(HELPMSGSTRING);
			RWM_HTML_GETOBJECT		= RegisterWindowMessage(TEXT("WM_HTML_GETOBJECT"));
			RWM_SETRGBSTRING		= RegisterWindowMessage(SETRGBSTRING);
			RWM_SHAREVISTRING		= RegisterWindowMessage(SHAREVISTRING);
			RWM_TASKBARBUTTONCREATED	= RegisterWindowMessage(TEXT("TaskbarButtonCreated"));
			RWM_TASKBARCREATED		= RegisterWindowMessage(TEXT("TaskbarCreated")); //ϵͳ���̱����ȫָ�� -- http://www.vckbase.com/index.php/wv/310
		}
		return m_bInited;
	}

#define HANDLE_IF_TO_STRING(buf,len,v,c) \
	if(v == c) { StringCchCopy(buf,len,TEXT(#c)); }
	

	LPCTSTR CFRegistedMessageInfo::GetMessageInfo(UINT msg, WPARAM wParam, LPARAM lParam)
	{
		//switch .. case ��Ҫ���� -- �� RWM_ ���Ǳ���
		HANDLE_IF_TO_STRING(m_bufInfo, _countof(m_bufInfo), msg, RWM_ATL_CREATE_OBJECT);
		HANDLE_IF_TO_STRING(m_bufInfo, _countof(m_bufInfo), msg, RWM_ATLGETCONTROL);
		HANDLE_IF_TO_STRING(m_bufInfo, _countof(m_bufInfo), msg, RWM_ATLGETHOST);
		HANDLE_IF_TO_STRING(m_bufInfo, _countof(m_bufInfo), msg, RWM_COLOROKSTRING);
		HANDLE_IF_TO_STRING(m_bufInfo, _countof(m_bufInfo), msg, RWM_COMMDLG_FIND);
		HANDLE_IF_TO_STRING(m_bufInfo, _countof(m_bufInfo), msg, RWM_FILEOKSTRING);
		HANDLE_IF_TO_STRING(m_bufInfo, _countof(m_bufInfo), msg, RWM_FINDMSGSTRING);
		HANDLE_IF_TO_STRING(m_bufInfo, _countof(m_bufInfo), msg, RWM_LBSELCHSTRING);
		HANDLE_IF_TO_STRING(m_bufInfo, _countof(m_bufInfo), msg, RWM_MSH_MOUSEWHEEL);
		HANDLE_IF_TO_STRING(m_bufInfo, _countof(m_bufInfo), msg, RWM_MSH_WHEELSUPPORT);
		HANDLE_IF_TO_STRING(m_bufInfo, _countof(m_bufInfo), msg, RWM_MSH_SCROLL_LINES);
		HANDLE_IF_TO_STRING(m_bufInfo, _countof(m_bufInfo), msg, RWM_HELPMSGSTRING);
		HANDLE_IF_TO_STRING(m_bufInfo, _countof(m_bufInfo), msg, RWM_HTML_GETOBJECT);
		HANDLE_IF_TO_STRING(m_bufInfo, _countof(m_bufInfo), msg, RWM_SETRGBSTRING);
		HANDLE_IF_TO_STRING(m_bufInfo, _countof(m_bufInfo), msg, RWM_SHAREVISTRING);
		HANDLE_IF_TO_STRING(m_bufInfo, _countof(m_bufInfo), msg, RWM_TASKBARBUTTONCREATED);

		//HANDLE_IF_TO_STRING(m_bufInfo, _countof(m_bufInfo), msg, XXXXXXXX);

		if(NULL == m_bufInfo[0])
		{
			StringCchPrintf(m_bufInfo, _countof(m_bufInfo), 
				TEXT("Unknown RegisterWindowMessage %d(0x%08x), wParam=%d, lParam=%d"), msg, msg, wParam, lParam);
		}
		return m_bufInfo;
	}

	__declspec(selectany) CFRegistedMessageInfo   CFMessageInfo::s_RegistedMessageInfo;

    CFMessageInfo::CFMessageInfo(UINT msg, WPARAM wParam, LPARAM lParam) 
        : CFConvertInfoT<CFMessageInfo,UINT>(msg)
        , m_wParam(wParam)
        , m_lParam(lParam)
    {
    }

    LPCTSTR CFMessageInfo::ConvertInfo()
    {
		s_RegistedMessageInfo.Init();
        if (NULL == m_bufInfo[0])
        {
            switch(m_Info)
            {
                GET_MESSAGE_INFO_ENTRY(WM_CREATE, CFDefaultMsgInfo);
				//GET_MESSAGE_INFO_ENTRY(WM_CREATE, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_DESTROY, CFDefaultMsgInfo);  //�رմ���ʱ,�����������,����PostQuitMessage,������̻�������(ֻ��û�д���)
                GET_MESSAGE_INFO_ENTRY(WM_MOVE, CFMoveMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_SIZE, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_ACTIVATE, CFDefaultMsgInfo); //���ڱ������ʧȥ����״̬, �����ڲ鿴�����Ƿ���С������ǰһ����
                GET_MESSAGE_INFO_ENTRY(WM_SETFOCUS, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_KILLFOCUS, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_ENABLE, CFDefaultMsgInfo); //����ֹ�򼤻�ʱ
                GET_MESSAGE_INFO_ENTRY(WM_SETREDRAW, CFDefaultMsgInfo); //���ô����Ƿ����ػ� 
                GET_MESSAGE_INFO_ENTRY(WM_SETTEXT, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_GETTEXT, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_GETTEXTLENGTH, CFDefaultMsgInfo); //�õ���һ�������йص��ı��ĳ��ȣ����������ַ���
                GET_MESSAGE_INFO_ENTRY(WM_PAINT, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_CLOSE, CFDefaultMsgInfo);
#ifndef _WIN32_WCE
                GET_MESSAGE_INFO_ENTRY(WM_QUERYENDSESSION, CFDefaultMsgInfo); //���û�ѡ������Ի��������Լ�����ExitWindows����
                GET_MESSAGE_INFO_ENTRY(WM_QUERYOPEN, CFDefaultMsgInfo);     //���û����ڻָ���ǰ�Ĵ�Сλ��ʱ���Ѵ���Ϣ���͸�ĳ��ͼ��
                //��ϵͳ���̷���WM_QUERYENDSESSION��Ϣ�󣬴���Ϣ���͸�Ӧ�ó���֪ͨ���Ի��Ƿ����,
                //һ����˵�������� WM_CLOSE ��Ϣ�ĳ�����Ҫ���������Ϣ��������ܵ���ϵͳ�޷��ر�
                // if(wParam || (lParam & ENDSESSION_LOGOFF)) { OnClose(); } 
                GET_MESSAGE_INFO_ENTRY(WM_ENDSESSION, CFDefaultMsgInfo);
#endif
                GET_MESSAGE_INFO_ENTRY(WM_QUIT, CFDefaultMsgInfo);
				//�����ڱ������뱻����ʱ�����ڴ��ڸı��Сʱ),���ʵ���в����˱������򷵻�TRUE���緵��FALSE���� PAINTSTRUCT::fErase ��ΪTRUE
				//ȱʡʵ����ʹ�ô�����ṹ�е�Brush����
                GET_MESSAGE_INFO_ENTRY(WM_ERASEBKGND, CFDefaultMsgInfo); 
                GET_MESSAGE_INFO_ENTRY(WM_SYSCOLORCHANGE, CFDefaultMsgInfo);    //��ϵͳ��ɫ�ı�ʱ�����ʹ���Ϣ�����ж�������
                GET_MESSAGE_INFO_ENTRY(WM_SHOWWINDOW, CFDefaultMsgInfo);

#ifndef WM_CTLCOLOR
#  define WM_CTLCOLOR                             0x0019
#endif
                //�Ӵ������ڻ��ƿͻ���֮ǰ�����䷢�͸������ڣ�ί�и�����Ϊ�Լ�׼��һ����ˢ��ͬʱ�����ڣ���Ի���Ҳ���Լ����͸���Ϣ(Ϊʲô��)
                GET_MESSAGE_INFO_ENTRY(WM_CTLCOLOR, CFDefaultMsgInfo);  

                GET_MESSAGE_INFO_ENTRY(WM_WININICHANGE, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_DEVMODECHANGE, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_ACTIVATEAPP, CFDefaultMsgInfo);//������һApp�Ĵ��彫����ʱ
                GET_MESSAGE_INFO_ENTRY(WM_FONTCHANGE, CFDefaultMsgInfo);    //��ϵͳ��������Դ��仯ʱ���ʹ���Ϣ�����ж�������
                GET_MESSAGE_INFO_ENTRY(WM_TIMECHANGE, CFDefaultMsgInfo);    //��ϵͳ��ʱ��仯ʱ���ʹ���Ϣ�����ж�������
                GET_MESSAGE_INFO_ENTRY(WM_CANCELMODE, CFDefaultMsgInfo);    //ȡ��ϵͳģʽʱ
                GET_MESSAGE_INFO_ENTRY(WM_SETCURSOR, CFDefaultMsgInfo);     //��������������ĳ���������ƶ����������û�б�����ʱ���ͷ���Ϣ��ĳ������
                GET_MESSAGE_INFO_ENTRY(WM_MOUSEACTIVATE, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_CHILDACTIVATE, CFDefaultMsgInfo); //�ʹ���Ϣ��MDI�Ӵ��ڵ��û�����˴��ڵı��������򵱴��ڱ�����ƶ����ı��С
                GET_MESSAGE_INFO_ENTRY(WM_QUEUESYNC, CFDefaultMsgInfo);     //����Ϣ�ɻ��ڼ������"ѵ��?"�����ͣ�ͨ�� WH_JOURNALPALYBACK ��hook���������û�������Ϣ
                GET_MESSAGE_INFO_ENTRY(WM_GETMINMAXINFO, CFDefaultMsgInfo); //�������Ϣ���Եõ�һ���ı�������С�Ĵ���ȱʡֵ�Ļ���

                GET_MESSAGE_INFO_ENTRY(WM_PAINTICON, CFDefaultMsgInfo);     //���͸���С�����ڵ���ͼ�꽫Ҫ���ػ�
                GET_MESSAGE_INFO_ENTRY(WM_ICONERASEBKGND, CFDefaultMsgInfo);//����Ϣ���͸�ĳ����С�����ڣ��������ڻ�ͼ��ǰ���ı������뱻�ػ�
                GET_MESSAGE_INFO_ENTRY(WM_NEXTDLGCTL, CFDefaultMsgInfo);    //�л�����һ���ؼ�( ģ��Tab �� ����һ��Focus�Ŀؼ�ʱ ��Ҫ�ֹ��������?)
                GET_MESSAGE_INFO_ENTRY(WM_SPOOLERSTATUS, CFDefaultMsgInfo); //ÿ����ӡ�����ж����ӻ����һ����ҵʱ��������Ϣ
                GET_MESSAGE_INFO_ENTRY(WM_DRAWITEM, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_MEASUREITEM, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_DELETEITEM, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_VKEYTOITEM, CFDefaultMsgInfo);    //��һ��LBS_WANTKEYBOARDINPUT���ķ�������������������ӦWM_KEYDOWN��Ϣ
                GET_MESSAGE_INFO_ENTRY(WM_CHARTOITEM, CFDefaultMsgInfo);    //��һ��LBS_WANTKEYBOARDINPUT�����б���͸���������������ӦWM_CHAR��Ϣ
                GET_MESSAGE_INFO_ENTRY(WM_SETFONT, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_GETFONT, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_SETHOTKEY, CFDefaultMsgInfo);     //Ӧ�ó����ʹ���Ϣ��һ��������һ���ȼ������
                GET_MESSAGE_INFO_ENTRY(WM_GETHOTKEY, CFDefaultMsgInfo);     //Ӧ�ó����ʹ���Ϣ���ж��ȼ���ĳ�������Ƿ��й���
                GET_MESSAGE_INFO_ENTRY(WM_QUERYDRAGICON, CFDefaultMsgInfo); //�͸���С�����ڣ����˴��ڽ�Ҫ���ϷŶ���������û�ж���ͼ�꣬Ӧ�ó����ܷ���һ��ͼ�����ľ����
                //���û��Ϸ�ͼ��ʱϵͳ��ʾ���ͼ�����
                GET_MESSAGE_INFO_ENTRY(WM_COMPAREITEM, CFDefaultMsgInfo);   //���ʹ���Ϣ���ж�combobox��listbox�����ӵ�������λ��
#if(WINVER >= 0x0500)
                GET_MESSAGE_INFO_ENTRY(WM_GETOBJECT, CFDefaultMsgInfo);
#endif
                GET_MESSAGE_INFO_ENTRY(WM_COMPACTING, CFDefaultMsgInfo);    //��ʾ�ڴ��Ѿ�������
                GET_MESSAGE_INFO_ENTRY(WM_COMMNOTIFY, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_WINDOWPOSCHANGING, CFWindowPosMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_WINDOWPOSCHANGED, CFWindowPosMsgInfo);	//�������λ�á���С��λ�ñ仯�Ժ�
                GET_MESSAGE_INFO_ENTRY(WM_POWER, CFDefaultMsgInfo);         //��ϵͳ��Ҫ������ͣ״̬ʱ���ʹ���Ϣ
                GET_MESSAGE_INFO_ENTRY(WM_COPYDATA, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_CANCELJOURNAL, CFDefaultMsgInfo); //��ĳ���û�ȡ��������־����״̬���ύ����Ϣ������
#if(WINVER >= 0x0400)
				GET_MESSAGE_INFO_ENTRY(WM_NOTIFY, CFNotifyMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_INPUTLANGCHANGEREQUEST, CFDefaultMsgInfo);    //���û�ѡ��ĳ���������ԣ����������Ե��ȼ��ı�
                GET_MESSAGE_INFO_ENTRY(WM_INPUTLANGCHANGE, CFDefaultMsgInfo);   //��ƽ̨�ֳ��Ѿ����ı���ʹ���Ϣ����Ӱ����������
                GET_MESSAGE_INFO_ENTRY(WM_TCARD, CFDefaultMsgInfo);         //�������Ѿ���ʼ��windows��������ʱ���ʹ���Ϣ��Ӧ�ó���
                GET_MESSAGE_INFO_ENTRY(WM_HELP, CFDefaultMsgInfo);          //����Ϣ��ʾ�û�������F1�����ĳ���˵��Ǽ���ģ��ͷ��ʹ���Ϣ���˴��ڹ����Ĳ˵���
                //  ����ͷ��͸��н���Ĵ��ڣ������ǰ��û�н��㣬�ͰѴ���Ϣ���͸���ǰ����Ĵ���
                GET_MESSAGE_INFO_ENTRY(WM_USERCHANGED, CFDefaultMsgInfo);   //���û��Ѿ�������˳����ʹ���Ϣ�����еĴ��ڣ����û�������˳�ʱϵͳ�����û��ľ���
                //  ������Ϣ�����û���������ʱϵͳ���Ϸ��ʹ���Ϣ
                GET_MESSAGE_INFO_ENTRY(WM_NOTIFYFORMAT, CFDefaultMsgInfo);  //ͨ������Ϣ���жϿؼ���ʹ��ANSI����UNICODE�ṹ
                GET_MESSAGE_INFO_ENTRY(WM_CONTEXTMENU, CFDefaultMsgInfo);   //���û�ĳ�������е����һ���Ҽ��ͷ��ʹ���Ϣ���������
                GET_MESSAGE_INFO_ENTRY(WM_STYLECHANGING, CFDefaultMsgInfo); //������SETWINDOWLONG���� ��Ҫ�ı� һ������ ���ڵķ��ʱ���ʹ���Ϣ���Ǹ�����
                GET_MESSAGE_INFO_ENTRY(WM_STYLECHANGED, CFDefaultMsgInfo);  //������SETWINDOWLONG���� �ı� һ���������ڵķ����ʹ���Ϣ���Ǹ�����
                GET_MESSAGE_INFO_ENTRY(WM_DISPLAYCHANGE, CFDefaultMsgInfo); //����ʾ���ķֱ��ʸı���ʹ���Ϣ�����еĴ���
                GET_MESSAGE_INFO_ENTRY(WM_GETICON, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_SETICON, CFDefaultMsgInfo);
#endif
                GET_MESSAGE_INFO_ENTRY(WM_NCCREATE, CFDefaultMsgInfo);      //��ĳ�����ڵ�һ�α�����ʱ������Ϣ��WM_CREATE��Ϣ����ǰ����
                GET_MESSAGE_INFO_ENTRY(WM_NCDESTROY, CFDefaultMsgInfo);     //����Ϣ֪ͨĳ�����ڣ��ǿͻ�����������
                
                //��Ҫ���㴰�ڿͻ����Ĵ�С��λ��ʱ���ͣ�ͨ���������Ϣ�������ڴ��ڴ�С��λ�øı�ʱ���ƿͻ���������     
                GET_MESSAGE_INFO_ENTRY(WM_NCCALCSIZE, CFDefaultMsgInfo);    
                GET_MESSAGE_INFO_ENTRY(WM_NCHITTEST, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_NCPAINT, CFDefaultMsgInfo);       //�����ʹ���Ϣ��ĳ�����ڵ��������ڣ��Ŀ�ܱ��뱻����ʱ
                GET_MESSAGE_INFO_ENTRY(WM_NCACTIVATE, CFDefaultMsgInfo);    //����Ϣ���͸�ĳ������ �������ķǿͻ�����Ҫ���ı�����ʾ�Ǽ���ǷǼ���״̬
                GET_MESSAGE_INFO_ENTRY(WM_GETDLGCODE, CFDefaultMsgInfo);    //���ʹ���Ϣ��ĳ����Ի����������Ŀؼ���widdows���Ʒ�λ����TAB��ʹ�������˿ؼ�
                //  ͨ����ӦWM_GETDLGCODE��Ϣ��Ӧ�ó�����԰�������һ�����������ؼ����ܴ�����
#ifndef _WIN32_WCE
                GET_MESSAGE_INFO_ENTRY(WM_SYNCPAINT, CFDefaultMsgInfo);
#endif
                GET_MESSAGE_INFO_ENTRY(WM_NCMOUSEMOVE, CFDefaultMsgInfo);   //�ǿͻ������ƶ�ʱ���ʹ���Ϣ���������
                GET_MESSAGE_INFO_ENTRY(WM_NCLBUTTONDOWN, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_NCLBUTTONUP, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_NCLBUTTONDBLCLK, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_NCRBUTTONDOWN, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_NCRBUTTONUP, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_NCRBUTTONDBLCLK, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_NCMBUTTONDOWN, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_NCMBUTTONUP, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_NCMBUTTONDBLCLK, CFDefaultMsgInfo);

#if(_WIN32_WINNT >= 0x0500)
                GET_MESSAGE_INFO_ENTRY(WM_NCXBUTTONDOWN, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_NCXBUTTONUP, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_NCXBUTTONDBLCLK, CFDefaultMsgInfo);
#endif

#if(_WIN32_WINNT >= 0x0501)
                GET_MESSAGE_INFO_ENTRY(WM_INPUT, CFDefaultMsgInfo);
#endif
                //GET_MESSAGE_INFO_ENTRY(WM_KEYFIRST, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_KEYDOWN, CFKeyMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_KEYUP, CFKeyMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_CHAR, CFKeyMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_DEADCHAR, CFKeyMsgInfo); //���� translatemessage ��������WM_KEYUP��Ϣʱ���ʹ���Ϣ��ӵ�н���Ĵ���
                GET_MESSAGE_INFO_ENTRY(WM_SYSKEYDOWN, CFKeyMsgInfo);    //���û���סALT��ͬʱ����������ʱ�ύ����Ϣ��ӵ�н���Ĵ���
                GET_MESSAGE_INFO_ENTRY(WM_SYSKEYUP, CFKeyMsgInfo);      //���û��ͷ�һ����ͬʱALT ��������ʱ�ύ����Ϣ��ӵ�н���Ĵ���
                GET_MESSAGE_INFO_ENTRY(WM_SYSCHAR, CFKeyMsgInfo);       //��WM_SYSKEYDOWN��Ϣ��TRANSLATEMESSAGE����������ύ����Ϣ��ӵ�н���Ĵ�
                GET_MESSAGE_INFO_ENTRY(WM_SYSDEADCHAR, CFKeyMsgInfo);   //
#if(_WIN32_WINNT >= 0x0501)
                GET_MESSAGE_INFO_ENTRY(WM_UNICHAR, CFDefaultMsgInfo);
                //GET_MESSAGE_INFO_ENTRY(WM_KEYLAST, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(UNICODE_NOCHAR, CFDefaultMsgInfo);
#else
                GET_MESSAGE_INFO_ENTRY(WM_KEYLAST, CFDefaultMsgInfo);
#endif

#if(WINVER >= 0x0400)
#endif
                GET_MESSAGE_INFO_ENTRY(WM_INITDIALOG, CFDefaultMsgInfo); //��һ���Ի��������ʾǰ���ʹ���Ϣ������ͨ���ô���Ϣ��ʼ���ؼ���ִ����������
                GET_MESSAGE_INFO_ENTRY(WM_COMMAND, CFCommandMsgInfo);

                //������������Ļ��������ʾ���ڵ�ģʽ(SC_SCREENSAVE/SC_MONITORPOWER),����0
                GET_MESSAGE_INFO_ENTRY(WM_SYSCOMMAND, CFDefaultMsgInfo);    //���û�ѡ�񴰿ڲ˵���һ��������û�ѡ����󻯻���С��ʱ�Ǹ����ڻ��յ�����Ϣ

                GET_MESSAGE_INFO_ENTRY(WM_TIMER, CFDefaultMsgInfo);     //�����˶�ʱ���¼�
                GET_MESSAGE_INFO_ENTRY(WM_HSCROLL, CFScrollMsgInfo);   //ˮƽ����������һ�������¼�
                GET_MESSAGE_INFO_ENTRY(WM_VSCROLL, CFScrollMsgInfo);   //��ֱ����������һ�������¼�
                GET_MESSAGE_INFO_ENTRY(WM_INITMENU, CFDefaultMsgInfo);  //��һ���˵���Ҫ������ʱ���ʹ���Ϣ�����������û��˵����е�ĳ�����ĳ���˵�����
                //  �������������ʾǰ���Ĳ˵�
                GET_MESSAGE_INFO_ENTRY(WM_INITMENUPOPUP, CFDefaultMsgInfo);

                GET_MESSAGE_INFO_ENTRY(WM_SYSTIMER, CFDefaultMsgInfo);  //UnDocument Message

                GET_MESSAGE_INFO_ENTRY(WM_MENUSELECT, CFDefaultMsgInfo); //���û�ѡ��һ���˵���ʱ���ʹ���Ϣ���˵��������ߣ�һ���Ǵ��ڣ�
                GET_MESSAGE_INFO_ENTRY(WM_MENUCHAR, CFDefaultMsgInfo);  //���˵��ѱ������û�������ĳ��������ͬ�ڼ��ټ��������ʹ���Ϣ���˵���������
                GET_MESSAGE_INFO_ENTRY(WM_ENTERIDLE, CFDefaultMsgInfo); //��һ��ģ̬�Ի����˵��������״̬ʱ(������һ��������ǰ����Ϣ��û����Ϣ�����ж��еȴ�)���ʹ���Ϣ�����������ߣ�
#if(WINVER >= 0x0500)
                GET_MESSAGE_INFO_ENTRY(WM_MENURBUTTONUP, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_MENUDRAG, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_MENUGETOBJECT, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_UNINITMENUPOPUP, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_MENUCOMMAND, CFDefaultMsgInfo);
#  if(_WIN32_WINNT >= 0x0500)
                GET_MESSAGE_INFO_ENTRY(WM_CHANGEUISTATE, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_UPDATEUISTATE, CFDefaultMsgInfo); //TODO: ���ܵ���û�� WM_PAINT ���ػ�? �μ� WTL.CBitmapButtonImpl.OnUpdateUiState 
                GET_MESSAGE_INFO_ENTRY(WM_QUERYUISTATE, CFDefaultMsgInfo);
#  endif //_WIN32_WINNT >= 0x0500

#endif //WINVER >= 0x0500

				//�ؼ���Ҫ������ǰ���ʹ���Ϣ�����ĸ����� -- �������ÿؼ����ı�������(���صĻ�ˢ���)����ɫ
                GET_MESSAGE_INFO_ENTRY(WM_CTLCOLORMSGBOX, CFDefaultMsgInfo);    //��windows������Ϣ��ǰ���ʹ���Ϣ����Ϣ��������ߴ��ڣ�ͨ����Ӧ������Ϣ�������ߴ��ڿ���
                //  ͨ��ʹ�ø����������ʾ�豸�ľ����������Ϣ����ı��ͱ�����ɫ
                GET_MESSAGE_INFO_ENTRY(WM_CTLCOLOREDIT, CFDefaultMsgInfo);      //�������ñ༭����ı��ͱ�����ɫ
                GET_MESSAGE_INFO_ENTRY(WM_CTLCOLORLISTBOX, CFDefaultMsgInfo);   //���������б����ı��ͱ�����ɫ
                GET_MESSAGE_INFO_ENTRY(WM_CTLCOLORBTN, CFDefaultMsgInfo);       //�������ð�Ŧ���ı��ͱ�����ɫ
                GET_MESSAGE_INFO_ENTRY(WM_CTLCOLORDLG, CFDefaultMsgInfo);       //�������öԻ�����ı�������ɫ(return CreateSolidBrush(xxx);
                GET_MESSAGE_INFO_ENTRY(WM_CTLCOLORSCROLLBAR, CFDefaultMsgInfo); //�������ù������ı�����ɫ
                GET_MESSAGE_INFO_ENTRY(WM_CTLCOLORSTATIC, CFDefaultMsgInfo);    //�������þ�̬�ؼ����ı��ͱ�����ɫ

                GET_MESSAGE_INFO_ENTRY(MN_GETHMENU, CFDefaultMsgInfo);

                //GET_MESSAGE_INFO_ENTRY(WM_MOUSEFIRST, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_MOUSEMOVE, CFMouseMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_LBUTTONDOWN, CFMouseMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_LBUTTONUP, CFMouseMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_LBUTTONDBLCLK, CFMouseMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_RBUTTONDOWN, CFMouseMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_RBUTTONUP, CFMouseMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_RBUTTONDBLCLK, CFMouseMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_MBUTTONDOWN, CFMouseMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_MBUTTONUP, CFMouseMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_MBUTTONDBLCLK, CFMouseMsgInfo);

#if (_WIN32_WINNT >= 0x0400) || (_WIN32_WINDOWS > 0x0400)
                GET_MESSAGE_INFO_ENTRY(WM_MOUSEWHEEL, CFDefaultMsgInfo);        //���������ת��ʱ���ʹ���Ϣ����ǰ�н���Ŀؼ�
#endif

#if (_WIN32_WINNT >= 0x0500)                
                GET_MESSAGE_INFO_ENTRY(WM_XBUTTONDOWN, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_XBUTTONUP, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_XBUTTONDBLCLK, CFDefaultMsgInfo);
#endif

#if (_WIN32_WINNT >= 0x0600)
                GET_MESSAGE_INFO_ENTRY(WM_MOUSEHWHEEL, CFDefaultMsgInfo);
#endif
                //GET_MESSAGE_INFO_ENTRY(WM_MOUSELAST, CFDefaultMsgInfo);

                GET_MESSAGE_INFO_ENTRY(WM_PARENTNOTIFY, CFDefaultMsgInfo); //��MDI�Ӵ��ڱ����������٣����û�����һ��������������Ӵ�����ʱ���ʹ���Ϣ�����ĸ�����
                GET_MESSAGE_INFO_ENTRY(WM_ENTERMENULOOP, CFDefaultMsgInfo); //���ʹ���Ϣ֪ͨӦ�ó���������� �Ѿ������˲˵�ѭ��ģʽ -- ?
                GET_MESSAGE_INFO_ENTRY(WM_EXITMENULOOP, CFDefaultMsgInfo);  //���ʹ���Ϣ֪ͨӦ�ó���������� �Ѿ��˳��˲˵�ѭ��ģʽ -- ?

#if(WINVER >= 0x0400)
                GET_MESSAGE_INFO_ENTRY(WM_NEXTMENU, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_SIZING, CFDefaultMsgInfo);        //���û����ڵ������ڴ�Сʱ���ʹ���Ϣ�����ڣ�ͨ������ϢӦ�ó�����Լ��Ӻ��޸Ĵ��ڴ�С��λ��
                GET_MESSAGE_INFO_ENTRY(WM_CAPTURECHANGED, CFDefaultMsgInfo);//����ʧȥ��������ʱ
                GET_MESSAGE_INFO_ENTRY(WM_MOVING, CFDefaultMsgInfo);        //���û����ƶ�����ʱ���ʹ���Ϣ
#endif

#if(WINVER >= 0x0400)
                GET_MESSAGE_INFO_ENTRY(WM_POWERBROADCAST, CFPowerBroadcastMsgInfo);//�͸�Ӧ�ó�����֪ͨ���йص�Դ�����¼�
                GET_MESSAGE_INFO_ENTRY(WM_DEVICECHANGE, CFDefaultMsgInfo);  //���豸��Ӳ�����øı�ʱ���ʹ���Ϣ��Ӧ�ó�����豸��������
#endif

                GET_MESSAGE_INFO_ENTRY(WM_MDICREATE, CFDefaultMsgInfo);     //Ҫ�󴴽�һ��MDI �Ӵ���
                GET_MESSAGE_INFO_ENTRY(WM_MDIDESTROY, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_MDIACTIVATE, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_MDIRESTORE, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_MDINEXT, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_MDIMAXIMIZE, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_MDITILE, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_MDICASCADE, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_MDIICONARRANGE, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_MDIGETACTIVE, CFDefaultMsgInfo);


                GET_MESSAGE_INFO_ENTRY(WM_MDISETMENU, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_ENTERSIZEMOVE, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_EXITSIZEMOVE, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_DROPFILES, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_MDIREFRESHMENU, CFDefaultMsgInfo);

				GET_MESSAGE_INFO_ENTRY(WM_IME_STARTCOMPOSITION, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(WM_IME_ENDCOMPOSITION, CFDefaultMsgInfo);
				//֪ͨ����¼���ִ��ĸı䣬ͨ���� if(lParam & GCS_RESULTSTR) ʱ���� ImmGetCompositionString ����ָʾ��������
				GET_MESSAGE_INFO_ENTRY(WM_IME_COMPOSITION, CFIMECompositionMsgInfo);	
#if(WINVER >= 0x0400)
                GET_MESSAGE_INFO_ENTRY(WM_IME_SETCONTEXT, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_IME_NOTIFY, CFIMENotifyMsgInfo);		//֪ͨ����IME�Ӵ�״̬��ͨ�ñ仯
                GET_MESSAGE_INFO_ENTRY(WM_IME_CONTROL, CFDefaultMsgInfo);		//ָʾIME����ִ������
                GET_MESSAGE_INFO_ENTRY(WM_IME_COMPOSITIONFULL, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_IME_SELECT, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_IME_CHAR, CFIMECharMsgInfo);			//IME��ȡ��ת����Ľ��
#endif

#if(WINVER >= 0x0500)
                GET_MESSAGE_INFO_ENTRY(WM_IME_REQUEST, CFDefaultMsgInfo);
#endif

#if(WINVER >= 0x0400)
                GET_MESSAGE_INFO_ENTRY(WM_IME_KEYDOWN, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_IME_KEYUP, CFDefaultMsgInfo);
#endif

#if((_WIN32_WINNT >= 0x0400) || (WINVER >= 0x0500))
                GET_MESSAGE_INFO_ENTRY(WM_MOUSEHOVER, CFDefaultMsgInfo); //

				//Ĭ������£�����ƿ���Ϣ�ǲ��ᷢ�͵ģ���Ҫͨ�� TrackMouseEvent( {TME_LEAVE|TME_CANCEL } ע��
                GET_MESSAGE_INFO_ENTRY(WM_MOUSELEAVE, CFDefaultMsgInfo); //����ƿ�ʱ����Ҫ�� ON_MESSAGE ��Ӧ
#endif

#if(WINVER >= 0x0500)
                GET_MESSAGE_INFO_ENTRY(WM_NCMOUSEHOVER, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_NCMOUSELEAVE, CFDefaultMsgInfo);
#endif


#if(_WIN32_WINNT >= 0x0501)
                GET_MESSAGE_INFO_ENTRY(WM_WTSSESSION_CHANGE, CFWtsSessionChangeMsgInfo); //Session�ı�(�����������������˳���¼��)
                GET_MESSAGE_INFO_ENTRY(WM_TABLET_FIRST, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_TABLET_LAST, CFDefaultMsgInfo);
#endif

                GET_MESSAGE_INFO_ENTRY(WM_CUT, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_COPY, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_PASTE, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_CLEAR, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_UNDO, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_RENDERFORMAT, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_RENDERALLFORMATS, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_DESTROYCLIPBOARD, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_DRAWCLIPBOARD, CFDefaultMsgInfo); //������������ݱ仯ʱ���ʹ���Ϣ��������۲����ĵ�һ�����ڣ��������ü�����۲촰������ʾ�������������
                GET_MESSAGE_INFO_ENTRY(WM_PAINTCLIPBOARD, CFDefaultMsgInfo);//�����������CF_OWNERDIPLAY��ʽ�����ݲ��Ҽ�����۲촰�ڵĿͻ�����Ҫ�ػ�
                GET_MESSAGE_INFO_ENTRY(WM_VSCROLLCLIPBOARD, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_SIZECLIPBOARD, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_ASKCBFORMATNAME, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_CHANGECBCHAIN, CFDefaultMsgInfo); //��һ�����ڴӼ�����۲�������ȥʱ���ʹ���Ϣ��������۲����ĵ�һ������
                GET_MESSAGE_INFO_ENTRY(WM_HSCROLLCLIPBOARD, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_QUERYNEWPALETTE, CFDefaultMsgInfo);   //����Ϣ��ʹ�������յ�����ʱͬʱ�л���ʵ�������߼���ɫ��
                GET_MESSAGE_INFO_ENTRY(WM_PALETTEISCHANGING, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_PALETTECHANGED, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_HOTKEY, CFDefaultMsgInfo);        //���û�������REGISTERHOTKEY����ע����ȼ�ʱ�ύ����Ϣ

#if(WINVER >= 0x0400)
                GET_MESSAGE_INFO_ENTRY(WM_PRINT, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_PRINTCLIENT, CFDefaultMsgInfo);
#endif

#if(_WIN32_WINNT >= 0x0500)
                GET_MESSAGE_INFO_ENTRY(WM_APPCOMMAND, CFDefaultMsgInfo);
#endif

#if(_WIN32_WINNT >= 0x0501)
                GET_MESSAGE_INFO_ENTRY(WM_THEMECHANGED, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_CLIPBOARDUPDATE, CFDefaultMsgInfo); // 0x031D          
#endif /* _WIN32_WINNT >= 0x0501 */

#if(_WIN32_WINNT >= 0x0600)
                GET_MESSAGE_INFO_ENTRY(WM_DWMCOMPOSITIONCHANGED, CFDefaultMsgInfo);          //0x031E
                GET_MESSAGE_INFO_ENTRY(WM_DWMNCRENDERINGCHANGED, CFDefaultMsgInfo);          //0x031F
                GET_MESSAGE_INFO_ENTRY(WM_DWMCOLORIZATIONCOLORCHANGED, CFDefaultMsgInfo);    //0x0320
                GET_MESSAGE_INFO_ENTRY(WM_DWMWINDOWMAXIMIZEDCHANGE, CFDefaultMsgInfo);       //0x0321
#endif /* _WIN32_WINNT >= 0x0600 */

#if(WINVER >= 0x0600)
                GET_MESSAGE_INFO_ENTRY(WM_GETTITLEBARINFOEX, CFDefaultMsgInfo);              //0x033F
#endif /* WINVER >= 0x0600 */

#if(WINVER >= 0x0400)
                GET_MESSAGE_INFO_ENTRY(WM_HANDHELDFIRST, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_HANDHELDLAST, CFDefaultMsgInfo);
#ifdef __AFXPRIV_H__
                //#  ifdef USING_MFC
                GET_MESSAGE_INFO_ENTRY(WM_QUERYAFXWNDPROC, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_SIZEPARENT, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_SETMESSAGESTRING, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_IDLEUPDATECMDUI, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_INITIALUPDATE, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_COMMANDHELP, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_HELPHITTEST, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_EXITHELPMODE, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_RECALCPARENT, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_SIZECHILD, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_KICKIDLE, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_QUERYCENTERWND, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_DISABLEMODAL, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_FLOATSTATUS, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_ACTIVATETOPLEVEL, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_RESERVED_036F, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_RESERVED_0370, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_RESERVED_0371, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_RESERVED_0372, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_SOCKET_NOTIFY, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_SOCKET_DEAD, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_POPMESSAGESTRING, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_HELPPROMPTADDR, CFDefaultMsgInfo);
                //GET_MESSAGE_INFO_ENTRY(WM_OCC_LOADFROMSTREAM, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_OCC_LOADFROMSTORAGE, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_OCC_INITNEW, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_OCC_LOADFROMSTREAM_EX, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_OCC_LOADFROMSTORAGE_EX, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_QUEUE_SENTINEL, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_RESERVED_037C, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_RESERVED_037D, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_RESERVED_037E, CFDefaultMsgInfo);
                //����Ϣת�������������Ƚ��д��� -- �����÷���PreTranslateMessage �� ����ꡢ������Ϣ���Ӵ������ȴ���
                //BOOL bRet = FALSE; 
                //if(pMsg->hwnd == m_hWnd || IsChild(pMsg->hwnd))
                //  {	bRet = (BOOL)SendMessage(WM_FORWARDMSG, 0, (LPARAM)pMsg);} return bRet;
                GET_MESSAGE_INFO_ENTRY(WM_FORWARDMSG, CFDefaultMsgInfo);
#  else
                GET_MESSAGE_INFO_ENTRY(WM_AFXFIRST, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_AFXLAST, CFDefaultMsgInfo);

#  endif //end __AFXPRIV_H__

#endif

                GET_MESSAGE_INFO_ENTRY(WM_PENWINFIRST, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_PENWINLAST, CFDefaultMsgInfo);
#  ifdef _DDEHEADER_INCLUDED_
                GET_MESSAGE_INFO_ENTRY(WM_DDE_INITIATE, CFDefaultMsgInfo);  //һ��DDE�ͻ������ύ����Ϣ��ʼһ�������������ĻỰ����Ӧ�Ǹ�ָ���ĳ����������
                GET_MESSAGE_INFO_ENTRY(WM_DDE_TERMINATE, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_DDE_ADVISE, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_DDE_UNADVISE, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_DDE_ACK, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_DDE_DATA, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_DDE_REQUEST, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_DDE_POKE, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_DDE_EXECUTE, CFDefaultMsgInfo);
                //GET_MESSAGE_INFO_ENTRY(WM_DDE_LAST, CFDefaultMsgInfo);
#  endif
#if(WINVER >= 0x0400)
                GET_MESSAGE_INFO_ENTRY(WM_APP, CFDefaultMsgInfo);
#endif
                GET_MESSAGE_INFO_ENTRY(DM_GETDEFID, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(DM_SETDEFID, CFDefaultMsgInfo);
#if(WINVER >= 0x0400)
                GET_MESSAGE_INFO_ENTRY(DM_REPOSITION, CFDefaultMsgInfo);
#endif

				//Edit Control Messages
				GET_MESSAGE_INFO_ENTRY(EM_GETSEL, CFDefaultMsgInfo); //����ȡ��32KB(16Bit)��ѡ���ı�(RichEdit��ʹ�� EM_EXGETSEL ��ͻ���������)
				GET_MESSAGE_INFO_ENTRY(EM_SETSEL, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(EM_GETRECT, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(EM_SETRECT, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(EM_SETRECTNP, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(EM_SCROLL, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(EM_LINESCROLL, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(EM_SCROLLCARET, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(EM_GETMODIFY, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(EM_SETMODIFY, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(EM_GETLINECOUNT, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(EM_LINEINDEX, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(EM_SETHANDLE, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(EM_GETHANDLE, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(EM_GETTHUMB, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(EM_LINELENGTH, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(EM_REPLACESEL, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(EM_GETLINE, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(EM_LIMITTEXT, CFDefaultMsgInfo); //EM_SETLIMITTEXT
				GET_MESSAGE_INFO_ENTRY(EM_CANUNDO, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(EM_UNDO, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(EM_FMTLINES, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(EM_LINEFROMCHAR, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(EM_SETTABSTOPS, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(EM_SETPASSWORDCHAR, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(EM_EMPTYUNDOBUFFER, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(EM_GETFIRSTVISIBLELINE, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(EM_SETREADONLY, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(EM_SETWORDBREAKPROC, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(EM_GETWORDBREAKPROC, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(EM_GETPASSWORDCHAR, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(EM_SETMARGINS, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(EM_GETMARGINS, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(EM_GETLIMITTEXT, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(EM_POSFROMCHAR, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(EM_CHARFROMPOS, CFDefaultMsgInfo);
#if(WINVER >= 0x0500)
				GET_MESSAGE_INFO_ENTRY(EM_SETIMESTATUS, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(EM_GETIMESTATUS, CFDefaultMsgInfo);
#endif /* WINVER >= 0x0500 */
				
#if (_WIN32_WINNT >= 0x0501)
				GET_MESSAGE_INFO_ENTRY(EM_SETCUEBANNER, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(EM_GETCUEBANNER, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(EM_SHOWBALLOONTIP, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(EM_HIDEBALLOONTIP, CFDefaultMsgInfo);
#if _WIN32_WINNT >= 0x0600
				GET_MESSAGE_INFO_ENTRY(EM_SETHILITE, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(EM_GETHILITE, CFDefaultMsgInfo);
#endif //_WIN32_WINNT >= 0x0600
#endif //_WIN32_WINNT >= 0x0501

				//Button Control Messages
				GET_MESSAGE_INFO_ENTRY(BM_GETCHECK, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(BM_SETCHECK, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(BM_GETSTATE, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(BM_SETSTATE, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(BM_SETSTYLE, CFDefaultMsgInfo);
#if(WINVER >= 0x0400)
				GET_MESSAGE_INFO_ENTRY(BM_CLICK, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(BM_GETIMAGE, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(BM_SETIMAGE, CFDefaultMsgInfo);
#endif /* WINVER >= 0x0400 */

#if(WINVER >= 0x0600)
				GET_MESSAGE_INFO_ENTRY(BM_SETDONTCLICK, CFDefaultMsgInfo);
#endif /* WINVER >= 0x0600 */
				
#if (_WIN32_WINNT >= 0x0501)
				//Button control messages
				GET_MESSAGE_INFO_ENTRY(BCM_GETIDEALSIZE, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(BCM_SETIMAGELIST, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(BCM_GETIMAGELIST, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(BCM_SETTEXTMARGIN, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(BCM_GETTEXTMARGIN, CFDefaultMsgInfo);
#endif // _WIN32_WINNT >= 0x0501

#if _WIN32_WINNT >= 0x0600
				GET_MESSAGE_INFO_ENTRY(BCM_SETDROPDOWNSTATE, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(BCM_SETSPLITINFO, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(BCM_GETSPLITINFO, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(BCM_SETNOTE, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(BCM_GETNOTE, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(BCM_GETNOTELENGTH, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(BCM_SETSHIELD, CFDefaultMsgInfo);
#endif // _WIN32_WINNT >= 0x0600

				//Static Control Mesages
				GET_MESSAGE_INFO_ENTRY(STM_SETICON, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(STM_GETICON, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(STM_SETIMAGE, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(STM_GETIMAGE, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(STM_MSGMAX, CFDefaultMsgInfo);

				//Listbox messages
				GET_MESSAGE_INFO_ENTRY(LB_ADDSTRING, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LB_INSERTSTRING, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LB_DELETESTRING, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LB_SELITEMRANGEEX, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LB_RESETCONTENT, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LB_SETSEL, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LB_SETCURSEL, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LB_GETSEL, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LB_GETCURSEL, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LB_GETTEXT, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LB_GETTEXTLEN, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LB_GETCOUNT, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LB_SELECTSTRING, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LB_DIR, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LB_GETTOPINDEX, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LB_FINDSTRING, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LB_GETSELCOUNT, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LB_GETSELITEMS, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LB_SETTABSTOPS, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LB_GETHORIZONTALEXTENT, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LB_SETHORIZONTALEXTENT, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LB_SETCOLUMNWIDTH, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LB_ADDFILE, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LB_SETTOPINDEX, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LB_GETITEMRECT, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LB_GETITEMDATA, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LB_SETITEMDATA, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LB_SELITEMRANGE, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LB_SETANCHORINDEX, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LB_GETANCHORINDEX, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LB_SETCARETINDEX, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LB_GETCARETINDEX, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LB_SETITEMHEIGHT, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LB_GETITEMHEIGHT, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LB_FINDSTRINGEXACT, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LB_SETLOCALE, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LB_GETLOCALE, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LB_SETCOUNT, CFDefaultMsgInfo);
#if(WINVER >= 0x0400)
				GET_MESSAGE_INFO_ENTRY(LB_INITSTORAGE, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LB_ITEMFROMPOINT, CFDefaultMsgInfo);
#endif /* WINVER >= 0x0400 */

#if(_WIN32_WCE >= 0x0400)
				GET_MESSAGE_INFO_ENTRY(LB_MULTIPLEADDSTRING, CFDefaultMsgInfo);
#endif //_WIN32_WCE >= 0x0400

#if(_WIN32_WINNT >= 0x0501)
				GET_MESSAGE_INFO_ENTRY(LB_GETLISTBOXINFO, CFDefaultMsgInfo);
#endif /* _WIN32_WINNT >= 0x0501 */

//#if(_WIN32_WINNT >= 0x0501)
				GET_MESSAGE_INFO_ENTRY(LB_MSGMAX, CFDefaultMsgInfo);
//#endif //_WIN32_WINNT

				//ListView messages
				GET_MESSAGE_INFO_ENTRY(LVM_GETBKCOLOR, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_SETBKCOLOR, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_GETIMAGELIST, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_SETIMAGELIST, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_GETITEMCOUNT, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_GETITEMA, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_GETITEMW, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_SETITEMA, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_SETITEMW, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_INSERTITEMA, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_INSERTITEMW, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_DELETEITEM, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_DELETEALLITEMS, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_GETCALLBACKMASK, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_SETCALLBACKMASK, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_GETNEXTITEM, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_FINDITEMA, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_FINDITEMW, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_GETITEMRECT, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_SETITEMPOSITION, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_GETITEMPOSITION, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_GETSTRINGWIDTHA, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_GETSTRINGWIDTHW, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_HITTEST, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_ENSUREVISIBLE, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_SCROLL, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_REDRAWITEMS, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_ARRANGE, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_EDITLABELA, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_EDITLABELW, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_GETEDITCONTROL, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_GETCOLUMNA, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_GETCOLUMNW, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_SETCOLUMNA, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_SETCOLUMNW, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_INSERTCOLUMNA, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_INSERTCOLUMNW, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_DELETECOLUMN, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_GETCOLUMNWIDTH, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_SETCOLUMNWIDTH, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_GETHEADER, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_CREATEDRAGIMAGE, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_GETVIEWRECT, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_GETTEXTCOLOR, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_SETTEXTCOLOR, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_GETTEXTBKCOLOR, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_SETTEXTBKCOLOR, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_GETTOPINDEX, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_GETCOUNTPERPAGE, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_GETORIGIN, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_UPDATE, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_SETITEMSTATE, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_GETITEMSTATE, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_GETITEMTEXTA, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_GETITEMTEXTW, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_SETITEMTEXTA, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_SETITEMTEXTW, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_SETITEMCOUNT, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_SORTITEMS, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_SETITEMPOSITION32, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_GETSELECTEDCOUNT, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_GETITEMSPACING, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_GETISEARCHSTRINGA, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_GETISEARCHSTRINGW, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_SETICONSPACING, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_SETEXTENDEDLISTVIEWSTYLE, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_GETEXTENDEDLISTVIEWSTYLE, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_GETSUBITEMRECT, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_SUBITEMHITTEST, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_SETCOLUMNORDERARRAY, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_GETCOLUMNORDERARRAY, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_SETHOTITEM, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_GETHOTITEM, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_SETHOTCURSOR, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_GETHOTCURSOR, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_APPROXIMATEVIEWRECT, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_SETWORKAREAS, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_GETWORKAREAS, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_GETNUMBEROFWORKAREAS, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_GETSELECTIONMARK, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_SETSELECTIONMARK, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_SETHOVERTIME, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_GETHOVERTIME, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_SETTOOLTIPS, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_GETTOOLTIPS, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_SORTITEMSEX, CFDefaultMsgInfo);
				//GET_MESSAGE_INFO_ENTRY(XXXXXXXXXXXX, CFDefaultMsgInfo);

				//Combo Box messages
				GET_MESSAGE_INFO_ENTRY(CB_GETEDITSEL, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(CB_LIMITTEXT, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(CB_SETEDITSEL, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(CB_ADDSTRING, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(CB_DELETESTRING, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(CB_DIR, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(CB_GETCOUNT, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(CB_GETCURSEL, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(CB_GETLBTEXT, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(CB_GETLBTEXTLEN, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(CB_INSERTSTRING, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(CB_RESETCONTENT, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(CB_FINDSTRING, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(CB_SELECTSTRING, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(CB_SETCURSEL, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(CB_SHOWDROPDOWN, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(CB_GETITEMDATA, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(CB_SETITEMDATA, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(CB_GETDROPPEDCONTROLRECT, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(CB_SETITEMHEIGHT, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(CB_GETITEMHEIGHT, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(CB_SETEXTENDEDUI, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(CB_GETEXTENDEDUI, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(CB_GETDROPPEDSTATE, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(CB_FINDSTRINGEXACT, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(CB_SETLOCALE, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(CB_GETLOCALE, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(CB_GETTOPINDEX, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(CB_SETTOPINDEX, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(CB_GETHORIZONTALEXTENT, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(CB_SETHORIZONTALEXTENT, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(CB_GETDROPPEDWIDTH, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(CB_SETDROPPEDWIDTH, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(CB_INITSTORAGE, CFDefaultMsgInfo);
#if(_WIN32_WCE >= 0x0400)
				GET_MESSAGE_INFO_ENTRY(CB_MULTIPLEADDSTRING, CFDefaultMsgInfo);
#endif //_WIN32_WCE >= 0x0400
#if(_WIN32_WINNT >= 0x0501)
				GET_MESSAGE_INFO_ENTRY(CB_GETCOMBOBOXINFO, CFDefaultMsgInfo);
#endif //_WIN32_WINNT >= 0x0501
				GET_MESSAGE_INFO_ENTRY(CB_MSGMAX, CFDefaultMsgInfo);

				//Scroll bar messages
				GET_MESSAGE_INFO_ENTRY(SBM_SETPOS, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(SBM_GETPOS, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(SBM_SETRANGE, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(SBM_SETRANGEREDRAW, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(SBM_GETRANGE, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(SBM_ENABLE_ARROWS, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(SBM_SETSCROLLINFO, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(SBM_GETSCROLLINFO, CFDefaultMsgInfo);

#if(_WIN32_WINNT >= 0x0501)
				GET_MESSAGE_INFO_ENTRY(SBM_GETSCROLLBARINFO, CFDefaultMsgInfo);
#endif /* _WIN32_WINNT >= 0x0501 */

                //Reflected Window Message IDs
                GET_MESSAGE_INFO_ENTRY(OCM_COMMAND, CFDefaultMsgInfo);

#ifdef _WIN32
                GET_MESSAGE_INFO_ENTRY(OCM_CTLCOLORBTN, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(OCM_CTLCOLOREDIT, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(OCM_CTLCOLORDLG, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(OCM_CTLCOLORLISTBOX, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(OCM_CTLCOLORMSGBOX, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(OCM_CTLCOLORSCROLLBAR, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(OCM_CTLCOLORSTATIC, CFDefaultMsgInfo);
#else 
                GET_MESSAGE_INFO_ENTRY(OCM_CTLCOLOR, CFDefaultMsgInfo);
#endif //ifndef _WIN32

                GET_MESSAGE_INFO_ENTRY(OCM_DRAWITEM, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(OCM_MEASUREITEM, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(OCM_DELETEITEM, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(OCM_VKEYTOITEM, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(OCM_CHARTOITEM, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(OCM_COMPAREITEM, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(OCM_HSCROLL, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(OCM_VSCROLL, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(OCM_PARENTNOTIFY, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(OCM_NOTIFY, CFDefaultMsgInfo);
				//GET_MESSAGE_INFO_ENTRY(XXXXXXXXXXXXXXXXX, CFDefaultMsgInfo);
            default:
				if (m_Info >= WM_USER && m_Info <= 0x7FFF)
				{
					//private window classes
					UINT userMsg = m_Info - WM_USER;
					StringCchPrintf(m_bufInfo,_countof(m_bufInfo),
						TEXT("0x%08x = (WM_USER+%d[0x%x])"),m_Info, userMsg, userMsg );
				}
				else if(m_Info >= WM_APP && m_Info <= 0xBFFF)
				{
					//Messages available for use by applications
					UINT appMsg = m_Info - WM_APP;
					StringCchPrintf(m_bufInfo,_countof(m_bufInfo),
						TEXT("0x%08x = (WM_APP+%d[0x%x])"),m_Info, appMsg, appMsg );

				}
				else if (m_Info >= 0xC000 && m_Info <= 0xFFFF)
				{
					//ͨ�� RegisterWindowMessage ע�����Ϣ
					StringCchPrintf(m_bufInfo,_countof(m_bufInfo),
						s_RegistedMessageInfo.GetMessageInfo(m_Info, m_wParam, m_lParam));
				}
				else
				{
					StringCchPrintf(m_bufInfo,_countof(m_bufInfo),
						TEXT("Unknown Message %d(0x%08x)"),m_Info,m_Info);
				}
            }
        }
        return m_bufInfo;
    }//CFMessageInfo


    ///////////////////////////////////////////////////////////////////////////////////////////////////

    template <typename T, typename TWindowAutoSizeTraits >
    CFWindowAutoSize<T,TWindowAutoSizeTraits>::CFWindowAutoSize()
    {
        m_sizeWindow.cx = 0;
        m_sizeWindow.cy = 0; 
        m_ptMinTrackSize.x = -1;
        m_ptMinTrackSize.y = -1;
        m_bGripper = FALSE;
    }

    template <typename T, typename TWindowAutoSizeTraits >
    BOOL CFWindowAutoSize<T,TWindowAutoSizeTraits>::InitAutoSizeInfo(BOOL bAddGripper /* = TRUE */, BOOL bUseMinTrackSize /* = TRUE */)
    {
        BOOL bRet = FALSE;
        T* pT = static_cast<T*>(this);
        HWND hWnd = TWindowAutoSizeTraits::GetWinHwndProxy(pT);
        FTLASSERT(::IsWindow(hWnd));
        DWORD dwStyle = (DWORD)::GetWindowLong(hWnd, GWL_STYLE);
#ifdef FTL_DEBUG
        // Debug only: Check if top level dialogs have a resizeable border.
        if(((dwStyle & WS_CHILD) == 0) && ((dwStyle & WS_THICKFRAME) == 0))
        {
            FTLTRACEEX(tlWarning,TEXT("top level dialog without the WS_THICKFRAME style - user cannot resize it\n"));
        }
#endif // _DEBUG

        {
            // Cleanup in case of multiple initialization
            // block: first check for the gripper control, destroy it if needed
            HWND HwndGripper = ::GetDlgItem(hWnd, TWindowAutoSizeTraits::GetStatusBarCtrlID());
            if( ::IsWindow(HwndGripper) && m_allResizeData.size() > 0 && (m_allResizeData[0].m_dwResizeFlags & _WINSZ_GRIPPER) != 0)
            {
                API_VERIFY(::DestroyWindow(HwndGripper));
            }
        }
        // clear out everything else
        m_allResizeData.clear();
        m_sizeWindow.cx = 0;
        m_sizeWindow.cy = 0;
        m_ptMinTrackSize.x = -1;
        m_ptMinTrackSize.y = -1;

        // Get initial dialog client size
        RECT rectDlg = { 0 };
        API_VERIFY(::GetClientRect(hWnd,&rectDlg));
        m_sizeWindow.cx = rectDlg.right;
        m_sizeWindow.cy = rectDlg.bottom;

       // Create gripper if requested
        m_bGripper = FALSE;
        if(bAddGripper)
        {
            // shouldn't exist already
            FTLASSERT(!::IsWindow(::GetDlgItem(hWnd, TWindowAutoSizeTraits::GetStatusBarCtrlID())));
            if(!::IsWindow(::GetDlgItem(hWnd, TWindowAutoSizeTraits::GetStatusBarCtrlID())))
            {
                HWND hWndGripper = ::CreateWindowEx(0,_T("SCROLLBAR"),NULL, 
                    WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | SBS_SIZEBOX | SBS_SIZEGRIP | SBS_SIZEBOXBOTTOMRIGHTALIGN,
                    rectDlg.left,
                    rectDlg.top,
                    rectDlg.right - rectDlg.left,
                    rectDlg.bottom - rectDlg.top,
                    hWnd,
                    (HMENU)(TWindowAutoSizeTraits::GetStatusBarCtrlID()), 
                    NULL,
                    NULL);
                FTLASSERT(::IsWindow(hWndGripper));
                if(::IsWindow(hWndGripper))
                {
                    m_bGripper = TRUE;
                    RECT rectCtl = { 0 };
                    API_VERIFY(::GetWindowRect(hWndGripper,&rectCtl));
                    API_VERIFY(::MapWindowPoints(NULL, hWnd, (LPPOINT)&rectCtl, 2) != 0);
                    _WindowResizeData data = 
                    {
                        TWindowAutoSizeTraits::GetStatusBarCtrlID(), 
                        WINSZ_MOVE_X | WINSZ_MOVE_Y | WINSZ_REPAINT | _WINSZ_GRIPPER, 
                        { 
                            rectCtl.left, 
                            rectCtl.top, 
                            rectCtl.right, 
                            rectCtl.bottom 
                        }
                    };
                    m_allResizeData.push_back(data);
                }
            }
        }

        // Get min track position if requested
        if(bUseMinTrackSize)
        {
            if((dwStyle & WS_CHILD) != 0)
            {
                RECT rect = { 0 };
                API_VERIFY(::GetClientRect(hWnd,&rect));
                m_ptMinTrackSize.x = rect.right - rect.left;
                m_ptMinTrackSize.y = rect.bottom - rect.top;
            }
            else
            {
                RECT rect = { 0 };
                API_VERIFY(::GetWindowRect(hWnd, &rect));
                m_ptMinTrackSize.x = rect.right - rect.left;
                m_ptMinTrackSize.y = rect.bottom - rect.top;
            }
        }

        // Walk the map and initialize data
        const _WindowResizeMap* pMap = pT->GetWindowResizeMap();
        FTLASSERT(pMap != NULL);
        int nGroupStart = -1;
        for(int nCount = 1; !(pMap->m_nCtlID == -1 && pMap->m_dwResizeFlags == 0); nCount++, pMap++)
        {
            if(pMap->m_nCtlID == -1)    //��ʼ�����һ����
            {
                switch(pMap->m_dwResizeFlags)
                {
                case _WINSZ_BEGIN_GROUP:
                    FTLASSERT(nGroupStart == -1);
                    nGroupStart = static_cast<int>(m_allResizeData.size());
                    break;
                case _WINSZ_END_GROUP:
                    {
                        FTLASSERT(nGroupStart != -1);
                        int nGroupCount = static_cast<int>(m_allResizeData.size()) - nGroupStart;
                        m_allResizeData[nGroupStart].SetGroupCount(nGroupCount);
                        nGroupStart = -1;
                    }
                    break;
                default:
                    FTLASSERT(FALSE && _T("Invalid WINDOWRESIZE Map Entry"));
                    break;
                }
            }
            else
            {
                // this ID conflicts with the default gripper one
                FTLASSERT(m_bGripper ? (pMap->m_nCtlID != TWindowAutoSizeTraits::GetStatusBarCtrlID()) : TRUE);
                HWND hWndCtrl = ::GetDlgItem(hWnd,pMap->m_nCtlID);
                FTLASSERT(::IsWindow(hWndCtrl));
                RECT rectCtl = { 0 };
                API_VERIFY(::GetWindowRect(hWndCtrl,&rectCtl));
                API_VERIFY(::MapWindowPoints(NULL, hWnd, (LPPOINT)&rectCtl, 2)!= 0);

                DWORD dwGroupFlag = (nGroupStart != -1 && static_cast<int>(m_allResizeData.size()) == nGroupStart) ? _WINSZ_BEGIN_GROUP : 0;
                _WindowResizeData data = 
                {
                    pMap->m_nCtlID, 
                    pMap->m_dwResizeFlags | dwGroupFlag, 
                    { 
                        rectCtl.left, 
                        rectCtl.top, 
                        rectCtl.right, 
                        rectCtl.bottom 
                    } 
                };
                m_allResizeData.push_back(data);
            }
        }
        FTLASSERT((nGroupStart == -1) && _T("No End Group Entry in the WINDOWRESIZE Map"));
        return bRet;
    }

    template <typename T, typename TWindowAutoSizeTraits >
    BOOL CFWindowAutoSize<T,TWindowAutoSizeTraits>::AutoResizeUpdateLayout(int cxWidth, int cyHeight)
    {
        BOOL bRet = FALSE;
        T* pT = static_cast<T*>(this);
        HWND hWnd = TWindowAutoSizeTraits::GetWinHwndProxy(pT);

        FTLASSERT(::IsWindow(hWnd));

        // Restrict minimum size if requested
        if((((DWORD)::GetWindowLong(hWnd, GWL_STYLE) & WS_CHILD) != 0) && m_ptMinTrackSize.x != -1 && m_ptMinTrackSize.y != -1)
        {
            if(cxWidth < m_ptMinTrackSize.x)
            {
                cxWidth = m_ptMinTrackSize.x;
            }
            if(cyHeight < m_ptMinTrackSize.y)
            {
                cyHeight = m_ptMinTrackSize.y;
            }
        }

        BOOL bVisible = ::IsWindowVisible(hWnd);
        if(bVisible)
        {
            ::SendMessage(hWnd, WM_SETREDRAW, (WPARAM)FALSE, 0);
            //pT->SetRedraw(FALSE);
        }

        for(size_t i = 0; i < m_allResizeData.size(); i++)
        {
            if((m_allResizeData[i].m_dwResizeFlags & _WINSZ_BEGIN_GROUP) != 0)   // start of a group
            {
                int nGroupCount = m_allResizeData[i].GetGroupCount();
                FTLASSERT(nGroupCount > 0 && i + nGroupCount - 1 < m_allResizeData.size());
                RECT rectGroup = m_allResizeData[i].m_rect;

                int j = 1;
                for(j = 1; j < nGroupCount; j++)
                {
                    rectGroup.left = min(rectGroup.left, m_allResizeData[i + j].m_rect.left);
                    rectGroup.top = min(rectGroup.top, m_allResizeData[i + j].m_rect.top);
                    rectGroup.right = max(rectGroup.right, m_allResizeData[i + j].m_rect.right);
                    rectGroup.bottom = max(rectGroup.bottom, m_allResizeData[i + j].m_rect.bottom);
                }

                for(j = 0; j < nGroupCount; j++)
                {
                    _WindowResizeData* pDataPrev = NULL;
                    if(j > 0)
                        pDataPrev = &(m_allResizeData[i + j - 1]);
                    pT->AutoPositionControl(cxWidth, cyHeight, rectGroup, m_allResizeData[i + j], true, pDataPrev);
                }

                i += nGroupCount - 1;   // increment to skip all group controls
            }
            else // one control entry
            {
                RECT rectGroup = { 0, 0, 0, 0 };
                pT->AutoPositionControl(cxWidth, cyHeight, rectGroup, m_allResizeData[i], false);
            }
        }

        if(bVisible)
        {
            ::SendMessage(hWnd, WM_SETREDRAW, (WPARAM)TRUE, 0);
            //pT->SetRedraw(TRUE);
        }
        ::RedrawWindow(hWnd,NULL, NULL, RDW_ERASE | RDW_INVALIDATE | RDW_UPDATENOW | RDW_ALLCHILDREN);
        return bRet;
    }

    template <typename T, typename TWindowAutoSizeTraits >
    BOOL CFWindowAutoSize<T,TWindowAutoSizeTraits>::AutoPositionControl(int cxWidth, int cyHeight, 
        RECT& rectGroup, _WindowResizeData& data, 
        bool bGroup, _WindowResizeData* pDataPrev /* = NULL */)
    {
        //BOOL bRet = FALSE;
        T* pT = static_cast<T*>(this);
        HWND hWnd = TWindowAutoSizeTraits::GetWinHwndProxy(pT);
        FTLASSERT(::IsWindow(hWnd));

        HWND hWndCtrl = NULL;
        RECT rectCtl = { 0 };

        hWndCtrl = ::GetDlgItem(hWnd, data.m_nCtlID);
        FTLASSERT(::IsWindow(hWndCtrl));
        ::GetWindowRect(hWndCtrl, &rectCtl);
        ::MapWindowPoints(NULL, hWnd, (LPPOINT)&rectCtl, 2);

        if(bGroup)
        {
            if((data.m_dwResizeFlags & WINSZ_CENTER_X) != 0)
            {
                int cxRight = rectGroup.right + cxWidth - m_sizeWindow.cx;
                int cxCtl = data.m_rect.right - data.m_rect.left;
                rectCtl.left = rectGroup.left + (cxRight - rectGroup.left - cxCtl) / 2;
                rectCtl.right = rectCtl.left + cxCtl;
            }
            else if((data.m_dwResizeFlags & (WINSZ_SIZE_X | WINSZ_MOVE_X)) != 0)
            {
                rectCtl.left = rectGroup.left + ::MulDiv(data.m_rect.left - rectGroup.left, rectGroup.right - rectGroup.left + (cxWidth - m_sizeWindow.cx), rectGroup.right - rectGroup.left);

                if((data.m_dwResizeFlags & WINSZ_SIZE_X) != 0)
                {
                    rectCtl.right = rectGroup.left + ::MulDiv(data.m_rect.right - rectGroup.left, rectGroup.right - rectGroup.left + (cxWidth - m_sizeWindow.cx), rectGroup.right - rectGroup.left);

                    if(pDataPrev != NULL)
                    {
                        HWND hWndCtlPrev = ::GetDlgItem(hWnd,pDataPrev->m_nCtlID);
                        FTLASSERT(::IsWindow(hWndCtlPrev));
                        RECT rcPrev = { 0 };
                        ::GetWindowRect(hWndCtlPrev, &rcPrev);
                        ::MapWindowPoints(NULL, hWnd, (LPPOINT)&rcPrev, 2);
                        int dxAdjust = (rectCtl.left - rcPrev.right) - (data.m_rect.left - pDataPrev->m_rect.right);
                        rcPrev.right += dxAdjust;
                        ::SetWindowPos(hWndCtlPrev, NULL, rcPrev.left,rcPrev.top,rcPrev.right - rcPrev.left,
                            rcPrev.bottom-rcPrev.top,SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOMOVE);
                    }
                }
                else
                {
                    rectCtl.right = rectCtl.left + (data.m_rect.right - data.m_rect.left);
                }
            }

            if((data.m_dwResizeFlags & WINSZ_CENTER_Y) != 0)
            {
                int cyBottom = rectGroup.bottom + cyHeight - m_sizeWindow.cy;
                int cyCtl = data.m_rect.bottom - data.m_rect.top;
                rectCtl.top = rectGroup.top + (cyBottom - rectGroup.top - cyCtl) / 2;
                rectCtl.bottom = rectCtl.top + cyCtl;
            }
            else if((data.m_dwResizeFlags & (WINSZ_SIZE_Y | WINSZ_MOVE_Y)) != 0)
            {
                rectCtl.top = rectGroup.top + ::MulDiv(data.m_rect.top - rectGroup.top, rectGroup.bottom - rectGroup.top + (cyHeight - m_sizeWindow.cy), rectGroup.bottom - rectGroup.top);

                if((data.m_dwResizeFlags & WINSZ_SIZE_Y) != 0)
                {
                    rectCtl.bottom = rectGroup.top + ::MulDiv(data.m_rect.bottom - rectGroup.top, rectGroup.bottom - rectGroup.top + (cyHeight - m_sizeWindow.cy), rectGroup.bottom - rectGroup.top);

                    if(pDataPrev != NULL)
                    {
                        HWND hWndCtlPrev = ::GetDlgItem(hWnd, pDataPrev->m_nCtlID);
                        FTLASSERT(::IsWindow(hWndCtlPrev));
                        RECT rcPrev = { 0 };
                        ::GetWindowRect(hWndCtlPrev, &rcPrev);
                        ::MapWindowPoints(NULL, hWnd, (LPPOINT)&rcPrev, 2);
                        int dxAdjust = (rectCtl.top - rcPrev.bottom) - (data.m_rect.top - pDataPrev->m_rect.bottom);
                        rcPrev.bottom += dxAdjust;
                        ::SetWindowPos(hWndCtlPrev, NULL, rcPrev.left,rcPrev.top,rcPrev.right - rcPrev.left,
                            rcPrev.bottom-rcPrev.top,SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOMOVE);
                    }
                }
                else
                {
                    rectCtl.bottom = rectCtl.top + (data.m_rect.bottom - data.m_rect.top);
                }
            }
        }
        else // no group
        {
            if((data.m_dwResizeFlags & WINSZ_CENTER_X) != 0)
            {
                int cxCtl = data.m_rect.right - data.m_rect.left;
                rectCtl.left = (cxWidth - cxCtl) / 2;
                rectCtl.right = rectCtl.left + cxCtl;
            }
            else if((data.m_dwResizeFlags & (WINSZ_SIZE_X | WINSZ_MOVE_X)) != 0)
            {
                rectCtl.right = data.m_rect.right + (cxWidth - m_sizeWindow.cx);

                if((data.m_dwResizeFlags & WINSZ_MOVE_X) != 0)
                    rectCtl.left = rectCtl.right - (data.m_rect.right - data.m_rect.left);
            }

            if((data.m_dwResizeFlags & WINSZ_CENTER_Y) != 0)
            {
                int cyCtl = data.m_rect.bottom - data.m_rect.top;
                rectCtl.top = (cyHeight - cyCtl) / 2;
                rectCtl.bottom = rectCtl.top + cyCtl;
            }
            else if((data.m_dwResizeFlags & (WINSZ_SIZE_Y | WINSZ_MOVE_Y)) != 0)
            {
                rectCtl.bottom = data.m_rect.bottom + (cyHeight - m_sizeWindow.cy);

                if((data.m_dwResizeFlags & WINSZ_MOVE_Y) != 0)
                    rectCtl.top = rectCtl.bottom - (data.m_rect.bottom - data.m_rect.top);
            }
        }

        if((data.m_dwResizeFlags & WINSZ_REPAINT) != 0)
        {
            ::InvalidateRect(hWndCtrl, NULL, TRUE);
        }
        if((data.m_dwResizeFlags & (WINSZ_SIZE_X | WINSZ_SIZE_Y | WINSZ_MOVE_X | WINSZ_MOVE_Y | WINSZ_REPAINT | WINSZ_CENTER_X | WINSZ_CENTER_Y)) != 0)
        {
            ::SetWindowPos(hWndCtrl, NULL, rectCtl.left,rectCtl.top,rectCtl.right - rectCtl.left,
                rectCtl.bottom-rectCtl.top,SWP_NOZORDER | SWP_NOACTIVATE);

        }
        return TRUE;
    }

    BOOL CFWinUtil::SetWindowFullScreen(HWND hWnd,BOOL isFullScreen, BOOL &oldZoomedState)
    {
        BOOL bRet = FALSE;
        //��ȡ������ SM_CXFRAME, SM_CYFRAME, SM_CXScreen �Ĵ�С
        int cyCaption = ::GetSystemMetrics(SM_CYCAPTION);
        int cxFrame = ::GetSystemMetrics(SM_CXFRAME);
        int cyFrame = ::GetSystemMetrics(SM_CYFRAME);
        int cyMenu = ::GetSystemMetrics(SM_CYMENU);
        int cxScreen = ::GetSystemMetrics(SM_CXSCREEN);
        int cyScreen = ::GetSystemMetrics(SM_CYSCREEN);
        //int cxBorder = ::GetSystemMetrics(SM_CXBORDER);
        //int cyBorder = ::GetSystemMetrics(SM_CYBORDER);

        if (isFullScreen)
        {
            //oldZoomedState = ::IsZoomed(hWnd); //���浱ǰ�Ƿ������״̬
            //if(oldZoomedState) //��ǰ�����
            {
                BringWindowToTop(hWnd);
                bRet = ::SetWindowPos(hWnd,HWND_TOPMOST,-cxFrame,-(cyFrame + cyCaption + cyMenu),
                    cxScreen + 2 * cxFrame, cyScreen + 2 * cyFrame + cyCaption + cyMenu,
                    0);//SWP_NOOWNERZORDER
                //SetForegroundWindow

            }
            //else //��ǰ����ͨ״̬�������
            //{
            //    bRet = ::ShowWindow(hWnd,SW_SHOWMAXIMIZED);
            //}
        }
        else //�ָ�ԭ���ڴ�С
        {
            if(oldZoomedState)
            {
                //�Ƿ���ˣ�����
                bRet = SetWindowPos(hWnd,NULL,-cxFrame,-cyFrame,cxScreen + 2*cxFrame, cyScreen + 2*cyFrame, SWP_NOZORDER);
            }
            else
            {
                bRet = ShowWindow(hWnd,SW_RESTORE);
            }
        }
        return bRet;
    }

#if 0
    BOOL CFWinUtil::CenterWindow(HWND hWndCenter , BOOL bCurrMonitor)
    {
        FTLASSERT ( ::IsWindow ( m_hWnd ) ) ;
        // determine owner window to center against
        DWORD dwStyle = (DWORD)GetWindowLongPtr ( GWL_STYLE ) ;
        if ( NULL == hWndCenter )
        {
            if( dwStyle & WS_CHILD )
            {
                hWndCenter = ::GetParent ( m_hWnd ) ;
            }
            else
            {
                hWndCenter = ::GetWindow ( m_hWnd , GW_OWNER ) ;
            }
        }

        // Get coordinates of the window relative to its parent
        RECT rcDlg ;
        GetWindowRect ( &rcDlg ) ;
        RECT rcArea ;
        RECT rcCenter ;
        HWND hWndParent ;
        if ( !( dwStyle & WS_CHILD ) )
        {
            // Don't center against invisible or minimized windows
            if( NULL != hWndCenter )
            {
                DWORD dwStyle = ::GetWindowLongPtr ( hWndCenter ,
                    GWL_STYLE   ) ;
                if( !( dwStyle & WS_VISIBLE  ) ||
                    ( dwStyle & WS_MINIMIZE )   )
                {
                    hWndCenter = NULL ;
                }
            }

            if ( FALSE == bCurrMonitor )
            {
                // Center within screen coordinates
                ::SystemParametersInfo ( SPI_GETWORKAREA ,
                    NULL            ,
                    &rcArea         ,
                    NULL             ) ;
            }
            else
            {
                // Center based on the monitor containing the majority of
                // the window.
                HMONITOR hMon = MonitorFromWindow ( m_hWnd  ,
                    MONITOR_DEFAULTTONEAREST); //MONITOR_DEFAULTTOPRIMARY

                MONITORINFO stMI ;
                ZeroMemory ( &stMI , sizeof ( MONITORINFO ) ) ;
                stMI.cbSize = sizeof ( MONITORINFO ) ;

                GetMonitorInfo ( hMon , &stMI ) ;

                rcArea = stMI.rcMonitor ;
            }

            if ( NULL == hWndCenter )
            {
                rcCenter = rcArea;
            }
            else
            {
                ::GetWindowRect ( hWndCenter , &rcCenter ) ;
            }
        }
        else
        {
            // center within parent client coordinates
            hWndParent = GetParent ( ) ;

            ::GetClientRect ( hWndParent , &rcArea ) ;

            ::GetClientRect ( hWndCenter , &rcCenter ) ;
            ::MapWindowPoints ( hWndCenter        ,
                hWndParent        ,
                (POINT*)&rcCenter ,
                2                  ) ;
        }

        int DlgWidth = rcDlg.right - rcDlg.left ;
        int DlgHeight = rcDlg.bottom - rcDlg.top ;

        // Find dialog's upper left based on rcCenter
        int xLeft = (rcCenter.left + rcCenter.right) / 2 - DlgWidth / 2 ;
        int yTop = (rcCenter.top + rcCenter.bottom) / 2 - DlgHeight / 2 ;

        // If the dialog is outside the screen, move it inside
        if ( xLeft < rcArea.left )
        {
            xLeft = rcArea.left ;
        }
        else if ( xLeft + DlgWidth > rcArea.right )
        {
            xLeft = rcArea.right - DlgWidth ;
        }

        if ( yTop < rcArea.top )
        {
            yTop = rcArea.top ;
        }
        else if ( yTop + DlgHeight > rcArea.bottom )
        {
            yTop = rcArea.bottom - DlgHeight ;
        }

        // Map screen coordinates to child coordinates
        return ( ::SetWindowPos ( m_hWnd ,
            NULL   ,
            xLeft  ,
            yTop   ,
            -1     ,
            -1     ,
            SWP_NOSIZE |
            SWP_NOZORDER |
            SWP_NOACTIVATE  ) ) ;

    }

#endif 

	LRESULT CFWinUtil::CalcNcHitTestPostion(LPPOINT pPtClient, LPCRECT prcClient, LPCRECT prcCaption, BOOL bZoomed)
	{
		 //���ʱ�����������ֵ(XP:4, Win7:8)
		int nCxFrame = GetSystemMetrics(SM_CXFRAME);
		int nCyFrame = GetSystemMetrics(SM_CYFRAME);
		RECT rcSizeBox = {nCxFrame, nCyFrame, nCxFrame, nCyFrame}; 

		POINT pt = *pPtClient;
		RECT rcClient = *prcClient;

		FTLASSERT(pt.x < 0xFFFF);
		FTLASSERT(pt.y < 0xFFFF);
		//ʲôʱ���������������һ����ק��һ�߰� Win+D �ȼ�?
		if (pt.x >= 0xffff) { pt.x -= 0xffff; }
		if (pt.y >= 0xffff) { pt.y -= 0xffff; }

		if( !bZoomed ) {
			if( pt.y < rcClient.top + rcSizeBox.top ) {
				if( pt.x < rcClient.left + rcSizeBox.left ) return HTTOPLEFT;
				if( pt.x > rcClient.right - rcSizeBox.right ) return HTTOPRIGHT;
				return HTTOP;
			}
			else if( pt.y > rcClient.bottom - rcSizeBox.bottom ) {
				if( pt.x < rcClient.left + rcSizeBox.left ) return HTBOTTOMLEFT;
				if( pt.x > rcClient.right - rcSizeBox.right ) return HTBOTTOMRIGHT;
				return HTBOTTOM;
			}
			if( pt.x < rcClient.left + rcSizeBox.left ) return HTLEFT;
			if( pt.x > rcClient.right - rcSizeBox.right ) return HTRIGHT;
		}

		RECT rcCaption = *prcCaption;
		if( pt.x >= rcClient.left + rcCaption.left && pt.x < rcClient.right - rcCaption.right
			&& pt.y >= rcCaption.top && pt.y < rcCaption.bottom ) {
#pragma TODO(��Ҫ�ų� �رա���С������󻯡�Option�˵��ȿؼ���λ��)
				//�����Щ�ط��ǰ�ť�Ļ�����ť�����ȴ�����˲���Ҫ�ų�
				//���������DUI�����Ի�Ļ���������ų�

				//CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(pt));
				//if( pControl && _tcscmp(pControl->GetClass(), _T("ButtonUI")) != 0 && 
				//	_tcscmp(pControl->GetClass(), _T("OptionUI")) != 0 &&
				//	_tcscmp(pControl->GetClass(), _T("TextUI")) != 0 )
					return HTCAPTION;
		}
		return HTCLIENT;
	}

	HWND CFWinUtil::GetProcessMainWindow(DWORD dwProcessId)
	{
		HWND hWnd = ::GetTopWindow(NULL);
		while (hWnd)
		{
			DWORD dwPid = 0;
			DWORD dwThreadId = GetWindowThreadProcessId(hWnd, &dwPid);
			UNREFERENCED_PARAMETER(dwThreadId);
			if (dwPid != 0 && dwPid == dwProcessId)
			{
				return hWnd;
			}
			hWnd = ::GetNextWindow(hWnd, GW_HWNDNEXT);
		}
		return NULL;
	}

    BOOL CFWinUtil::ActiveAndForegroundWindow(HWND hWnd)
    {
        //IPMSG ��Ҳ��һ�� SetForceForegroundWindow ������������ͬ

        BOOL bRet = TRUE;
        HWND   hForegdWnd   =   ::GetForegroundWindow();
        if (hForegdWnd == hWnd)
        {
            return TRUE;
        }

        DWORD   dwThisThreadID   =   ::GetWindowThreadProcessId(hWnd, NULL);
        DWORD   dwForeThreadID   =   ::GetWindowThreadProcessId(hForegdWnd,NULL);
        if (dwThisThreadID != dwForeThreadID)
        {
            API_VERIFY(::AttachThreadInput(dwThisThreadID,dwForeThreadID,TRUE));

            //������ǰ��ֵ��Ȼ������Ϊ0
            DWORD sp_time = 0;
            SystemParametersInfo( SPI_GETFOREGROUNDLOCKTIMEOUT,0,&sp_time,0);
            SystemParametersInfo( SPI_SETFOREGROUNDLOCKTIMEOUT,0,(LPVOID)NULL, SPIF_SENDCHANGE); //0);

			API_VERIFY(::BringWindowToTop( hWnd ));
            API_VERIFY(::SetForegroundWindow(hWnd));

            SystemParametersInfo( SPI_SETFOREGROUNDLOCKTIMEOUT,0,&sp_time, SPIF_SENDCHANGE); //0);
            API_VERIFY(::AttachThreadInput(dwThisThreadID,dwForeThreadID,FALSE));
            //ShowWindow(hWnd,...)?
        }
        else
        {
			API_VERIFY(::BringWindowToTop( hWnd ));
            API_VERIFY(::SetForegroundWindow(hWnd));
            //ShowWindow(hWnd,...)?
        }
        SwitchToThisWindow(hWnd, TRUE);

//#pragma TODO(how to active a minimize window without change it min/max status)
//        if (IsIconic(hWnd))
//        {
//            API_VERIFY(ShowWindow(hWnd,SW_RESTORE));
//        }
//        else
//        {
//            API_VERIFY(ShowWindow(hWnd,SW_SHOW));
//        }

		//�Ƿ���Ҫ��Щ����
#if 0
		if ( TRUE ==  ::IsIconic ( hWnd ) )
		{
			::SendMessage ( hWnd, WM_SYSCOMMAND, SC_RESTORE, 0 );
		}
#endif 

        return bRet;
    }

    LPCDLGTEMPLATE CFWinUtil::LoadDialog(HMODULE hModuleRes, LPCTSTR szResource, HINSTANCE * phInstFoundIn)
    {
        UNREFERENCED_PARAMETER(phInstFoundIn);
        // Find the dialog resource.
        HRSRC hRSRC = FindResourceEx ( hModuleRes ,RT_DIALOG,szResource,MAKELANGID ( LANG_NEUTRAL,SUBLANG_NEUTRAL )) ;
        if ( NULL == hRSRC )
        {
            return ( NULL ) ;
        }
        // Now load it.
        HGLOBAL hResource = LoadResource ( hModuleRes , hRSRC ) ;
        FTLASSERT ( NULL != hResource ) ;
        if ( NULL == hResource )
        {
            return ( NULL ) ;
        }
        LPCDLGTEMPLATE lpDlgTemplate = (LPCDLGTEMPLATE)LockResource ( hResource ) ;
        return lpDlgTemplate;
    }

	LPCTSTR CFWinUtil::GetColorString(ColorType clrType, int nColorIndex)
	{
		switch (clrType)
		{
		case ctCtrlColor:
			{
				switch(nColorIndex)
				{
					HANDLE_CASE_RETURN_STRING(CTLCOLOR_MSGBOX);
					HANDLE_CASE_RETURN_STRING(CTLCOLOR_EDIT);
					HANDLE_CASE_RETURN_STRING(CTLCOLOR_LISTBOX);
					HANDLE_CASE_RETURN_STRING(CTLCOLOR_BTN);
					HANDLE_CASE_RETURN_STRING(CTLCOLOR_DLG);
					HANDLE_CASE_RETURN_STRING(CTLCOLOR_SCROLLBAR);
					HANDLE_CASE_RETURN_STRING(CTLCOLOR_STATIC);
					HANDLE_CASE_RETURN_STRING(CTLCOLOR_MAX);
				}
				break;
			}
		case ctSysColor:
			{
				switch(nColorIndex)
				{
					HANDLE_CASE_RETURN_STRING(COLOR_SCROLLBAR);
					HANDLE_CASE_RETURN_STRING(COLOR_BACKGROUND);
					HANDLE_CASE_RETURN_STRING(COLOR_ACTIVECAPTION);
					HANDLE_CASE_RETURN_STRING(COLOR_INACTIVECAPTION);
					HANDLE_CASE_RETURN_STRING(COLOR_MENU);
					HANDLE_CASE_RETURN_STRING(COLOR_WINDOW);
					HANDLE_CASE_RETURN_STRING(COLOR_WINDOWFRAME);
					HANDLE_CASE_RETURN_STRING(COLOR_MENUTEXT);
					HANDLE_CASE_RETURN_STRING(COLOR_WINDOWTEXT);
					HANDLE_CASE_RETURN_STRING(COLOR_CAPTIONTEXT);
					HANDLE_CASE_RETURN_STRING(COLOR_ACTIVEBORDER);
					HANDLE_CASE_RETURN_STRING(COLOR_INACTIVEBORDER);
					HANDLE_CASE_RETURN_STRING(COLOR_APPWORKSPACE);
					HANDLE_CASE_RETURN_STRING(COLOR_HIGHLIGHT);
					HANDLE_CASE_RETURN_STRING(COLOR_HIGHLIGHTTEXT);
					HANDLE_CASE_RETURN_STRING(COLOR_BTNFACE);
					HANDLE_CASE_RETURN_STRING(COLOR_BTNSHADOW);
					HANDLE_CASE_RETURN_STRING(COLOR_GRAYTEXT);
					HANDLE_CASE_RETURN_STRING(COLOR_BTNTEXT);
					HANDLE_CASE_RETURN_STRING(COLOR_INACTIVECAPTIONTEXT);
					HANDLE_CASE_RETURN_STRING(COLOR_BTNHIGHLIGHT);
#if(WINVER >= 0x0400)
					HANDLE_CASE_RETURN_STRING(COLOR_3DDKSHADOW);
					HANDLE_CASE_RETURN_STRING(COLOR_3DLIGHT);
					HANDLE_CASE_RETURN_STRING(COLOR_INFOTEXT);
					HANDLE_CASE_RETURN_STRING(COLOR_INFOBK);
#endif /* WINVER >= 0x0400 */

#if(WINVER >= 0x0500)
					HANDLE_CASE_RETURN_STRING(COLOR_HOTLIGHT);
					HANDLE_CASE_RETURN_STRING(COLOR_GRADIENTACTIVECAPTION);
					HANDLE_CASE_RETURN_STRING(COLOR_GRADIENTINACTIVECAPTION);
#endif /* WINVER >= 0x0501 */

#if(WINVER >= 0x0501)
					HANDLE_CASE_RETURN_STRING(COLOR_MENUHILIGHT);
					HANDLE_CASE_RETURN_STRING(COLOR_MENUBAR);
#endif /* WINVER >= 0x0500 */
				}
				break;
			}
		}
		FTLTRACEEX(FTL::tlWarning, TEXT("Unknown Color type(%d) and Index(%d)\n"), clrType, nColorIndex);
		return TEXT("Unknown");
	}

    LPCTSTR CFWinUtil::GetScrollBarCodeString(UINT nSBCode)
    {
        switch(nSBCode)
        {
            HANDLE_CASE_RETURN_STRING((SB_LINEUP|SB_LINELEFT));
            HANDLE_CASE_RETURN_STRING((SB_LINEDOWN|SB_LINERIGHT));
            HANDLE_CASE_RETURN_STRING((SB_PAGEUP|SB_PAGELEFT));
            HANDLE_CASE_RETURN_STRING((SB_PAGEDOWN|SB_PAGERIGHT));
            HANDLE_CASE_RETURN_STRING(SB_THUMBPOSITION);
            HANDLE_CASE_RETURN_STRING(SB_THUMBTRACK);
            HANDLE_CASE_RETURN_STRING((SB_TOP|SB_LEFT));
            HANDLE_CASE_RETURN_STRING((SB_BOTTOM|SB_RIGHT));
            HANDLE_CASE_RETURN_STRING(SB_ENDSCROLL);
        default:
            FTLTRACEEX(FTL::tlWarning, TEXT("Unknown ScrollBar Code, %d\n"), nSBCode);
            return TEXT("Unknown");
        }
    }

	LPCTSTR CFWinUtil::GetVirtualKeyString(int nVirtKey)
	{
		switch (nVirtKey)
		{
			HANDLE_CASE_RETURN_STRING(VK_LBUTTON);
			HANDLE_CASE_RETURN_STRING(VK_RBUTTON);
			HANDLE_CASE_RETURN_STRING(VK_CANCEL);
			HANDLE_CASE_RETURN_STRING(VK_MBUTTON);
#if(_WIN32_WINNT >= 0x0500)
			HANDLE_CASE_RETURN_STRING(VK_XBUTTON1);
			HANDLE_CASE_RETURN_STRING(VK_XBUTTON2);
#endif /* _WIN32_WINNT >= 0x0500 */
			
			// * 0x07 : unassigned
			HANDLE_CASE_RETURN_STRING(VK_BACK);
			HANDLE_CASE_RETURN_STRING(VK_TAB);
			HANDLE_CASE_RETURN_STRING(VK_CLEAR);
			HANDLE_CASE_RETURN_STRING(VK_RETURN);
			HANDLE_CASE_RETURN_STRING(VK_SHIFT);
			HANDLE_CASE_RETURN_STRING(VK_CONTROL);
			HANDLE_CASE_RETURN_STRING(VK_MENU);
			HANDLE_CASE_RETURN_STRING(VK_PAUSE);
			HANDLE_CASE_RETURN_STRING(VK_CAPITAL);
			HANDLE_CASE_RETURN_STRING((VK_KANA|VK_HANGEUL|VK_HANGUL)); //have some old name
			HANDLE_CASE_RETURN_STRING(VK_JUNJA);
			HANDLE_CASE_RETURN_STRING(VK_FINAL);
			HANDLE_CASE_RETURN_STRING((VK_HANJA | VK_KANJI));
			HANDLE_CASE_RETURN_STRING(VK_ESCAPE);
			HANDLE_CASE_RETURN_STRING(VK_CONVERT);
			HANDLE_CASE_RETURN_STRING(VK_NONCONVERT);
			HANDLE_CASE_RETURN_STRING(VK_ACCEPT);
			HANDLE_CASE_RETURN_STRING(VK_MODECHANGE);
			HANDLE_CASE_RETURN_STRING(VK_SPACE);
			HANDLE_CASE_RETURN_STRING(VK_PRIOR);
			HANDLE_CASE_RETURN_STRING(VK_NEXT);
			HANDLE_CASE_RETURN_STRING(VK_END);
			HANDLE_CASE_RETURN_STRING(VK_HOME);
			HANDLE_CASE_RETURN_STRING(VK_LEFT);
			HANDLE_CASE_RETURN_STRING(VK_UP);
			HANDLE_CASE_RETURN_STRING(VK_RIGHT);
			HANDLE_CASE_RETURN_STRING(VK_DOWN);
			HANDLE_CASE_RETURN_STRING(VK_SELECT);
			HANDLE_CASE_RETURN_STRING(VK_PRINT);
			HANDLE_CASE_RETURN_STRING(VK_EXECUTE);
			HANDLE_CASE_RETURN_STRING(VK_SNAPSHOT);
			HANDLE_CASE_RETURN_STRING(VK_INSERT);
			HANDLE_CASE_RETURN_STRING(VK_DELETE);
			HANDLE_CASE_RETURN_STRING(VK_HELP);

			//VK_0 - VK_9 are the same as ASCII '0' - '9' (0x30 - 0x39)
			HANDLE_CASE_RETURN_STRING('0');
			HANDLE_CASE_RETURN_STRING('1');
			HANDLE_CASE_RETURN_STRING('2');
			HANDLE_CASE_RETURN_STRING('3');
			HANDLE_CASE_RETURN_STRING('4');
			HANDLE_CASE_RETURN_STRING('5');
			HANDLE_CASE_RETURN_STRING('6');
			HANDLE_CASE_RETURN_STRING('7');
			HANDLE_CASE_RETURN_STRING('8');
			HANDLE_CASE_RETURN_STRING('9');
			//VK_A - VK_Z are the same as ASCII 'A' - 'Z' (0x41 - 0x5A)
			HANDLE_CASE_RETURN_STRING('A');
			HANDLE_CASE_RETURN_STRING('B');
			HANDLE_CASE_RETURN_STRING('C');
			HANDLE_CASE_RETURN_STRING('D');
			HANDLE_CASE_RETURN_STRING('E');
			HANDLE_CASE_RETURN_STRING('F');
			HANDLE_CASE_RETURN_STRING('G');
			HANDLE_CASE_RETURN_STRING('H');
			HANDLE_CASE_RETURN_STRING('I');
			HANDLE_CASE_RETURN_STRING('J');
			HANDLE_CASE_RETURN_STRING('K');
			HANDLE_CASE_RETURN_STRING('L');
			HANDLE_CASE_RETURN_STRING('M');
			HANDLE_CASE_RETURN_STRING('N');
			HANDLE_CASE_RETURN_STRING('O');
			HANDLE_CASE_RETURN_STRING('P');
			HANDLE_CASE_RETURN_STRING('Q');
			HANDLE_CASE_RETURN_STRING('R');
			HANDLE_CASE_RETURN_STRING('S');
			HANDLE_CASE_RETURN_STRING('T');
			HANDLE_CASE_RETURN_STRING('U');
			HANDLE_CASE_RETURN_STRING('V');
			HANDLE_CASE_RETURN_STRING('W');
			HANDLE_CASE_RETURN_STRING('X');
			HANDLE_CASE_RETURN_STRING('Y');
			HANDLE_CASE_RETURN_STRING('Z');

			HANDLE_CASE_RETURN_STRING(VK_LWIN);
			HANDLE_CASE_RETURN_STRING(VK_RWIN);
			HANDLE_CASE_RETURN_STRING(VK_APPS);
			HANDLE_CASE_RETURN_STRING(VK_SLEEP);
			HANDLE_CASE_RETURN_STRING(VK_NUMPAD0);
			HANDLE_CASE_RETURN_STRING(VK_NUMPAD1);
			HANDLE_CASE_RETURN_STRING(VK_NUMPAD2);
			HANDLE_CASE_RETURN_STRING(VK_NUMPAD3);
			HANDLE_CASE_RETURN_STRING(VK_NUMPAD4);
			HANDLE_CASE_RETURN_STRING(VK_NUMPAD5);
			HANDLE_CASE_RETURN_STRING(VK_NUMPAD6);
			HANDLE_CASE_RETURN_STRING(VK_NUMPAD7);
			HANDLE_CASE_RETURN_STRING(VK_NUMPAD8);
			HANDLE_CASE_RETURN_STRING(VK_NUMPAD9);
			HANDLE_CASE_RETURN_STRING(VK_MULTIPLY);
			HANDLE_CASE_RETURN_STRING(VK_ADD);
			HANDLE_CASE_RETURN_STRING(VK_SEPARATOR);
			HANDLE_CASE_RETURN_STRING(VK_SUBTRACT);
			HANDLE_CASE_RETURN_STRING(VK_DECIMAL);
			HANDLE_CASE_RETURN_STRING(VK_DIVIDE);
			HANDLE_CASE_RETURN_STRING(VK_F1);
			HANDLE_CASE_RETURN_STRING(VK_F2);
			HANDLE_CASE_RETURN_STRING(VK_F3);
			HANDLE_CASE_RETURN_STRING(VK_F4);
			HANDLE_CASE_RETURN_STRING(VK_F5);
			HANDLE_CASE_RETURN_STRING(VK_F6);
			HANDLE_CASE_RETURN_STRING(VK_F7);
			HANDLE_CASE_RETURN_STRING(VK_F8);
			HANDLE_CASE_RETURN_STRING(VK_F9);
			HANDLE_CASE_RETURN_STRING(VK_F10);
			HANDLE_CASE_RETURN_STRING(VK_F11);
			HANDLE_CASE_RETURN_STRING(VK_F12);
			HANDLE_CASE_RETURN_STRING(VK_F13);
			HANDLE_CASE_RETURN_STRING(VK_F14);
			HANDLE_CASE_RETURN_STRING(VK_F15);
			HANDLE_CASE_RETURN_STRING(VK_F16);
			HANDLE_CASE_RETURN_STRING(VK_F17);
			HANDLE_CASE_RETURN_STRING(VK_F18);
			HANDLE_CASE_RETURN_STRING(VK_F19);
			HANDLE_CASE_RETURN_STRING(VK_F20);
			HANDLE_CASE_RETURN_STRING(VK_F21);
			HANDLE_CASE_RETURN_STRING(VK_F22);
			HANDLE_CASE_RETURN_STRING(VK_F23);
			HANDLE_CASE_RETURN_STRING(VK_F24);
			HANDLE_CASE_RETURN_STRING(VK_NUMLOCK);
			HANDLE_CASE_RETURN_STRING(VK_SCROLL);
			HANDLE_CASE_RETURN_STRING((VK_OEM_NEC_EQUAL | VK_OEM_FJ_JISHO));
			HANDLE_CASE_RETURN_STRING(VK_OEM_FJ_MASSHOU);
			HANDLE_CASE_RETURN_STRING(VK_OEM_FJ_TOUROKU);
			HANDLE_CASE_RETURN_STRING(VK_OEM_FJ_LOYA);
			HANDLE_CASE_RETURN_STRING(VK_OEM_FJ_ROYA);
			HANDLE_CASE_RETURN_STRING(VK_LSHIFT);
			HANDLE_CASE_RETURN_STRING(VK_RSHIFT);
			HANDLE_CASE_RETURN_STRING(VK_LCONTROL);
			HANDLE_CASE_RETURN_STRING(VK_RCONTROL);
			HANDLE_CASE_RETURN_STRING(VK_LMENU);
			HANDLE_CASE_RETURN_STRING(VK_RMENU);
#if(_WIN32_WINNT >= 0x0500)
			HANDLE_CASE_RETURN_STRING(VK_BROWSER_BACK);
			HANDLE_CASE_RETURN_STRING(VK_BROWSER_FORWARD);
			HANDLE_CASE_RETURN_STRING(VK_BROWSER_REFRESH);
			HANDLE_CASE_RETURN_STRING(VK_BROWSER_STOP);
			HANDLE_CASE_RETURN_STRING(VK_BROWSER_SEARCH);
			HANDLE_CASE_RETURN_STRING(VK_BROWSER_FAVORITES);
			HANDLE_CASE_RETURN_STRING(VK_BROWSER_HOME);
			HANDLE_CASE_RETURN_STRING(VK_VOLUME_MUTE);
			HANDLE_CASE_RETURN_STRING(VK_VOLUME_DOWN);
			HANDLE_CASE_RETURN_STRING(VK_VOLUME_UP);
			HANDLE_CASE_RETURN_STRING(VK_MEDIA_NEXT_TRACK);
			HANDLE_CASE_RETURN_STRING(VK_MEDIA_PREV_TRACK);
			HANDLE_CASE_RETURN_STRING(VK_MEDIA_STOP);
			HANDLE_CASE_RETURN_STRING(VK_MEDIA_PLAY_PAUSE);
			HANDLE_CASE_RETURN_STRING(VK_LAUNCH_MAIL);
			HANDLE_CASE_RETURN_STRING(VK_LAUNCH_MEDIA_SELECT);
			HANDLE_CASE_RETURN_STRING(VK_LAUNCH_APP1);
			HANDLE_CASE_RETURN_STRING(VK_LAUNCH_APP2);
#endif /* _WIN32_WINNT >= 0x0500 */
			HANDLE_CASE_RETURN_STRING(VK_OEM_1);		// ';:' for US
			HANDLE_CASE_RETURN_STRING(VK_OEM_PLUS);		// '+' any country
			HANDLE_CASE_RETURN_STRING(VK_OEM_COMMA);	// ',' any country
			HANDLE_CASE_RETURN_STRING(VK_OEM_MINUS);	// '-' any country
			HANDLE_CASE_RETURN_STRING(VK_OEM_PERIOD);	// '.' any country
			HANDLE_CASE_RETURN_STRING(VK_OEM_2);		// '/?' for US
			HANDLE_CASE_RETURN_STRING(VK_OEM_3);		// '`~' for US
			HANDLE_CASE_RETURN_STRING(VK_OEM_4);		//  '[{' for US
			HANDLE_CASE_RETURN_STRING(VK_OEM_5);		//  '\|' for US
			HANDLE_CASE_RETURN_STRING(VK_OEM_6);		//  ']}' for US
			HANDLE_CASE_RETURN_STRING(VK_OEM_7);		//  ''"' for US
			HANDLE_CASE_RETURN_STRING(VK_OEM_8);		//  'AX' key on Japanese AX kbd
			HANDLE_CASE_RETURN_STRING(VK_OEM_AX);		//  "<>" or "\|" on RT 102-key kbd.
			HANDLE_CASE_RETURN_STRING(VK_OEM_102);		//  Help key on ICO
			HANDLE_CASE_RETURN_STRING(VK_ICO_HELP);		//  00 key on ICO
			HANDLE_CASE_RETURN_STRING(VK_ICO_00);
#if(WINVER >= 0x0400)
			HANDLE_CASE_RETURN_STRING(VK_PROCESSKEY);
#endif /* WINVER >= 0x0400 */
			HANDLE_CASE_RETURN_STRING(VK_ICO_CLEAR);
#if(_WIN32_WINNT >= 0x0500)
			HANDLE_CASE_RETURN_STRING(VK_PACKET);
#endif /* _WIN32_WINNT >= 0x0500 */

			//Nokia/Ericsson definitions
			HANDLE_CASE_RETURN_STRING(VK_OEM_RESET);
			HANDLE_CASE_RETURN_STRING(VK_OEM_JUMP);
			HANDLE_CASE_RETURN_STRING(VK_OEM_PA1);
			HANDLE_CASE_RETURN_STRING(VK_OEM_PA2);
			HANDLE_CASE_RETURN_STRING(VK_OEM_PA3);
			HANDLE_CASE_RETURN_STRING(VK_OEM_WSCTRL);
			HANDLE_CASE_RETURN_STRING(VK_OEM_CUSEL);
			HANDLE_CASE_RETURN_STRING(VK_OEM_ATTN);
			HANDLE_CASE_RETURN_STRING(VK_OEM_FINISH);
			HANDLE_CASE_RETURN_STRING(VK_OEM_COPY);
			HANDLE_CASE_RETURN_STRING(VK_OEM_AUTO);
			HANDLE_CASE_RETURN_STRING(VK_OEM_ENLW);
			HANDLE_CASE_RETURN_STRING(VK_OEM_BACKTAB);

			HANDLE_CASE_RETURN_STRING(VK_ATTN);
			HANDLE_CASE_RETURN_STRING(VK_CRSEL);
			HANDLE_CASE_RETURN_STRING(VK_EXSEL);
			HANDLE_CASE_RETURN_STRING(VK_EREOF);
			HANDLE_CASE_RETURN_STRING(VK_PLAY);
			HANDLE_CASE_RETURN_STRING(VK_ZOOM);
			HANDLE_CASE_RETURN_STRING(VK_NONAME);
			HANDLE_CASE_RETURN_STRING(VK_PA1);
			HANDLE_CASE_RETURN_STRING(VK_OEM_CLEAR);
		default:
			FTLTRACEEX(FTL::tlWarning, TEXT("Unknown VirtualKey, 0x%x\n"), nVirtKey);
			return TEXT("Unknown");
		}
	}

    LPCTSTR CFWinUtil::GetNotifyCodeString(HWND hWnd, UINT nCode, LPTSTR pszCommandNotify, int nLength, 
		TranslateWndClassProc pTransProc/* = g_pTranslateWndClassProc*/)
    {
		UNREFERENCED_PARAMETER(hWnd);
		UNREFERENCED_PARAMETER(pszCommandNotify);
		UNREFERENCED_PARAMETER(nLength);
		UNREFERENCED_PARAMETER(pTransProc);

        switch(nCode)
        {
            // generic to all controls
            HANDLE_CASE_RETURN_STRING(NM_FIRST);
            HANDLE_CASE_RETURN_STRING(NM_OUTOFMEMORY);
            HANDLE_CASE_RETURN_STRING(NM_CLICK);
            HANDLE_CASE_RETURN_STRING(NM_DBLCLK);
            HANDLE_CASE_RETURN_STRING(NM_RETURN);
            HANDLE_CASE_RETURN_STRING(NM_RCLICK);
            HANDLE_CASE_RETURN_STRING(NM_RDBLCLK);
            HANDLE_CASE_RETURN_STRING(NM_SETFOCUS);
            HANDLE_CASE_RETURN_STRING(NM_KILLFOCUS);
            HANDLE_CASE_RETURN_STRING(NM_CUSTOMDRAW);
            HANDLE_CASE_RETURN_STRING(NM_HOVER);
            HANDLE_CASE_RETURN_STRING(NM_NCHITTEST);
            HANDLE_CASE_RETURN_STRING(NM_KEYDOWN);
            HANDLE_CASE_RETURN_STRING(NM_RELEASEDCAPTURE);
            HANDLE_CASE_RETURN_STRING(NM_SETCURSOR);
            HANDLE_CASE_RETURN_STRING(NM_CHAR);
#if (_WIN32_IE >= 0x0401)
            HANDLE_CASE_RETURN_STRING(NM_TOOLTIPSCREATED);
#endif //_WIN32_IE >= 0x0401
#if (_WIN32_IE >= 0x0500)
            HANDLE_CASE_RETURN_STRING(NM_LDOWN);
            HANDLE_CASE_RETURN_STRING(NM_RDOWN);
            HANDLE_CASE_RETURN_STRING(NM_THEMECHANGED);
#endif //_WIN32_IE >= 0x0500

#if (_WIN32_WINNT >= 0x0600)
            HANDLE_CASE_RETURN_STRING(NM_FONTCHANGED);
            //HANDLE_CASE_RETURN_STRING(NM_CUSTOMTEXT);
            HANDLE_CASE_RETURN_STRING((NM_TVSTATEIMAGECHANGING | NM_CUSTOMTEXT)); //Same ID
#endif
            HANDLE_CASE_RETURN_STRING(NM_LAST);

            // listview
            HANDLE_CASE_RETURN_STRING(LVN_ITEMCHANGING);
            HANDLE_CASE_RETURN_STRING(LVN_ITEMCHANGED);
            HANDLE_CASE_RETURN_STRING(LVN_INSERTITEM);
            HANDLE_CASE_RETURN_STRING(LVN_DELETEITEM);
            HANDLE_CASE_RETURN_STRING(LVN_DELETEALLITEMS);
            HANDLE_CASE_RETURN_STRING(LVN_BEGINLABELEDITA);
            HANDLE_CASE_RETURN_STRING(LVN_BEGINLABELEDITW);
            HANDLE_CASE_RETURN_STRING(LVN_ENDLABELEDITA);
            HANDLE_CASE_RETURN_STRING(LVN_ENDLABELEDITW);
            HANDLE_CASE_RETURN_STRING(LVN_COLUMNCLICK);
            HANDLE_CASE_RETURN_STRING(LVN_BEGINDRAG);
            HANDLE_CASE_RETURN_STRING(LVN_BEGINRDRAG);
#if (_WIN32_IE >= 0x0300)
            HANDLE_CASE_RETURN_STRING(LVN_ODCACHEHINT);
            HANDLE_CASE_RETURN_STRING(LVN_ODFINDITEMA);
            HANDLE_CASE_RETURN_STRING(LVN_ODFINDITEMW);
            HANDLE_CASE_RETURN_STRING(LVN_ITEMACTIVATE);
            HANDLE_CASE_RETURN_STRING(LVN_ODSTATECHANGED);
#endif      // _WIN32_IE >= 0x0300


#if (_WIN32_IE >= 0x0400)
            HANDLE_CASE_RETURN_STRING(LVN_HOTTRACK);
#endif
            HANDLE_CASE_RETURN_STRING(LVN_GETDISPINFOA);
            HANDLE_CASE_RETURN_STRING(LVN_GETDISPINFOW);
            HANDLE_CASE_RETURN_STRING(LVN_SETDISPINFOA);
            HANDLE_CASE_RETURN_STRING(LVN_SETDISPINFOW);
            HANDLE_CASE_RETURN_STRING(LVN_KEYDOWN);
            HANDLE_CASE_RETURN_STRING(LVN_MARQUEEBEGIN);
            HANDLE_CASE_RETURN_STRING(LVN_GETINFOTIPA);
            HANDLE_CASE_RETURN_STRING(LVN_GETINFOTIPW);
#if (_WIN32_IE >= 0x0500)
#  ifndef LVN_INCREMENTALSEARCHA    
#    define LVN_INCREMENTALSEARCHA  (LVN_FIRST-62)
#  endif 
            HANDLE_CASE_RETURN_STRING(LVN_INCREMENTALSEARCHA);

#  ifndef LVN_INCREMENTALSEARCHW
#    define LVN_INCREMENTALSEARCHW  (LVN_FIRST-63)
#  endif 
            HANDLE_CASE_RETURN_STRING(LVN_INCREMENTALSEARCHW);
#endif //_WIN32_IE >= 0x0500 

#if _WIN32_WINNT >= 0x0600
            HANDLE_CASE_RETURN_STRING(LVN_COLUMNDROPDOWN);
            HANDLE_CASE_RETURN_STRING(LVN_COLUMNOVERFLOWCLICK);
#endif // _WIN32_WINNT >= 0x0600
#if (_WIN32_WINNT >= 0x0501)
            HANDLE_CASE_RETURN_STRING(LVN_BEGINSCROLL);
            HANDLE_CASE_RETURN_STRING(LVN_ENDSCROLL);
#endif
#if _WIN32_WINNT >= 0x0600
            HANDLE_CASE_RETURN_STRING(LVN_LINKCLICK);
            HANDLE_CASE_RETURN_STRING(LVN_GETEMPTYMARKUP);
#endif //_WIN32_WINNT >= 0x0600

            HANDLE_CASE_RETURN_STRING(LVN_LAST);

            // Property sheet reserved      (0U-200U) -  (0U-299U) - see prsht.h

            HANDLE_CASE_RETURN_STRING(PSN_SETACTIVE);
            HANDLE_CASE_RETURN_STRING(PSN_KILLACTIVE);  //PSN_VALIDATE
            HANDLE_CASE_RETURN_STRING(PSN_APPLY);
            HANDLE_CASE_RETURN_STRING(PSN_RESET);       //PSN_CANCEL
            HANDLE_CASE_RETURN_STRING(PSN_HELP);
            HANDLE_CASE_RETURN_STRING(PSN_WIZBACK);
            HANDLE_CASE_RETURN_STRING(PSN_WIZNEXT);
            HANDLE_CASE_RETURN_STRING(PSN_WIZFINISH);
            HANDLE_CASE_RETURN_STRING(PSN_QUERYCANCEL);
#if (_WIN32_IE >= 0x0400)
            HANDLE_CASE_RETURN_STRING(PSN_GETOBJECT);
#endif // 0x0400
#if (_WIN32_IE >= 0x0500)
            HANDLE_CASE_RETURN_STRING(PSN_TRANSLATEACCELERATOR);
            HANDLE_CASE_RETURN_STRING(PSN_QUERYINITIALFOCUS);
#endif // 0x0500

            // header
            HANDLE_CASE_RETURN_STRING(HDN_ITEMCHANGINGA);
            HANDLE_CASE_RETURN_STRING(HDN_ITEMCHANGINGW);
            HANDLE_CASE_RETURN_STRING(HDN_ITEMCHANGEDA);
            HANDLE_CASE_RETURN_STRING(HDN_ITEMCHANGEDW);
            HANDLE_CASE_RETURN_STRING(HDN_ITEMCLICKA);
            HANDLE_CASE_RETURN_STRING(HDN_ITEMCLICKW);
            HANDLE_CASE_RETURN_STRING(HDN_ITEMDBLCLICKA);
            HANDLE_CASE_RETURN_STRING(HDN_ITEMDBLCLICKW);
            HANDLE_CASE_RETURN_STRING(HDN_DIVIDERDBLCLICKA);
            HANDLE_CASE_RETURN_STRING(HDN_DIVIDERDBLCLICKW);
            HANDLE_CASE_RETURN_STRING(HDN_BEGINTRACKA);
            HANDLE_CASE_RETURN_STRING(HDN_BEGINTRACKW);
            HANDLE_CASE_RETURN_STRING(HDN_ENDTRACKA);
            HANDLE_CASE_RETURN_STRING(HDN_ENDTRACKW);
            HANDLE_CASE_RETURN_STRING(HDN_TRACKA);
            HANDLE_CASE_RETURN_STRING(HDN_TRACKW);
#if (_WIN32_IE >= 0x0300)
            HANDLE_CASE_RETURN_STRING(HDN_GETDISPINFOA);
            HANDLE_CASE_RETURN_STRING(HDN_GETDISPINFOW);
            HANDLE_CASE_RETURN_STRING(HDN_BEGINDRAG);
            HANDLE_CASE_RETURN_STRING(HDN_ENDDRAG);
#endif //_WIN32_IE
#if (_WIN32_IE >= 0x0500)
            HANDLE_CASE_RETURN_STRING(HDN_FILTERCHANGE);
            HANDLE_CASE_RETURN_STRING(HDN_FILTERBTNCLICK);
#endif //0x0500
#if (_WIN32_IE >= 0x0600)
            HANDLE_CASE_RETURN_STRING(HDN_BEGINFILTEREDIT);
            HANDLE_CASE_RETURN_STRING(HDN_ENDFILTEREDIT);
#endif
#if _WIN32_WINNT >= 0x0600
            HANDLE_CASE_RETURN_STRING(HDN_ITEMSTATEICONCLICK);
            HANDLE_CASE_RETURN_STRING(HDN_ITEMKEYDOWN);
            HANDLE_CASE_RETURN_STRING(HDN_DROPDOWN);
            HANDLE_CASE_RETURN_STRING(HDN_OVERFLOWCLICK);
#endif

            // treeview
            HANDLE_CASE_RETURN_STRING(TVN_SELCHANGINGA);
            HANDLE_CASE_RETURN_STRING(TVN_SELCHANGINGW);
            HANDLE_CASE_RETURN_STRING(TVN_SELCHANGEDA);
            HANDLE_CASE_RETURN_STRING(TVN_SELCHANGEDW);
            HANDLE_CASE_RETURN_STRING(TVN_GETDISPINFOA);
            HANDLE_CASE_RETURN_STRING(TVN_GETDISPINFOW);
            HANDLE_CASE_RETURN_STRING(TVN_SETDISPINFOA);
            HANDLE_CASE_RETURN_STRING(TVN_SETDISPINFOW);
            HANDLE_CASE_RETURN_STRING(TVN_ITEMEXPANDINGA);
            HANDLE_CASE_RETURN_STRING(TVN_ITEMEXPANDINGW);
            HANDLE_CASE_RETURN_STRING(TVN_ITEMEXPANDEDA);
            HANDLE_CASE_RETURN_STRING(TVN_ITEMEXPANDEDW);
            HANDLE_CASE_RETURN_STRING(TVN_BEGINDRAGA);
            HANDLE_CASE_RETURN_STRING(TVN_BEGINDRAGW);
            HANDLE_CASE_RETURN_STRING(TVN_BEGINRDRAGA);
            HANDLE_CASE_RETURN_STRING(TVN_BEGINRDRAGW);
            HANDLE_CASE_RETURN_STRING(TVN_DELETEITEMA);
            HANDLE_CASE_RETURN_STRING(TVN_DELETEITEMW);
            HANDLE_CASE_RETURN_STRING(TVN_BEGINLABELEDITA);
            HANDLE_CASE_RETURN_STRING(TVN_BEGINLABELEDITW);
            HANDLE_CASE_RETURN_STRING(TVN_ENDLABELEDITA);
            HANDLE_CASE_RETURN_STRING(TVN_ENDLABELEDITW);
            HANDLE_CASE_RETURN_STRING(TVN_KEYDOWN);
#if (_WIN32_IE >= 0x0400)
            HANDLE_CASE_RETURN_STRING(TVN_GETINFOTIPA);
            HANDLE_CASE_RETURN_STRING(TVN_GETINFOTIPW);
            HANDLE_CASE_RETURN_STRING(TVN_SINGLEEXPAND);
#endif // 0x400
#if (_WIN32_IE >= 0x0600)
            HANDLE_CASE_RETURN_STRING(TVN_ITEMCHANGINGA);
            HANDLE_CASE_RETURN_STRING(TVN_ITEMCHANGINGW);
            HANDLE_CASE_RETURN_STRING(TVN_ITEMCHANGEDA);
            HANDLE_CASE_RETURN_STRING(TVN_ITEMCHANGEDW);
            HANDLE_CASE_RETURN_STRING(TVN_ASYNCDRAW);
#endif // 0x0600

            //tooltips
            HANDLE_CASE_RETURN_STRING(TTN_GETDISPINFOA);
            HANDLE_CASE_RETURN_STRING(TTN_GETDISPINFOW);
            HANDLE_CASE_RETURN_STRING(TTN_SHOW);
            HANDLE_CASE_RETURN_STRING(TTN_POP);
            HANDLE_CASE_RETURN_STRING(TTN_LINKCLICK);

            //tab control
            HANDLE_CASE_RETURN_STRING(TCN_KEYDOWN);
            HANDLE_CASE_RETURN_STRING(TCN_SELCHANGE);
            HANDLE_CASE_RETURN_STRING(TCN_SELCHANGING);
#if (_WIN32_IE >= 0x0400)
            HANDLE_CASE_RETURN_STRING(TCN_GETOBJECT);
#endif      // _WIN32_IE >= 0x0400
#if (_WIN32_IE >= 0x0500)
            HANDLE_CASE_RETURN_STRING(TCN_FOCUSCHANGE);
#endif      // _WIN32_IE >= 0x0500


            // Shell reserved               (0U-580U) -  (0U-589U)

            // common dialog (new)

            // toolbar
            HANDLE_CASE_RETURN_STRING(TBN_GETBUTTONINFOA);
            HANDLE_CASE_RETURN_STRING(TBN_BEGINDRAG);
            HANDLE_CASE_RETURN_STRING(TBN_ENDDRAG);
            HANDLE_CASE_RETURN_STRING(TBN_BEGINADJUST);
            HANDLE_CASE_RETURN_STRING(TBN_ENDADJUST);
            HANDLE_CASE_RETURN_STRING(TBN_RESET);
            HANDLE_CASE_RETURN_STRING(TBN_QUERYINSERT);
            HANDLE_CASE_RETURN_STRING(TBN_QUERYDELETE);
            HANDLE_CASE_RETURN_STRING(TBN_TOOLBARCHANGE);
            HANDLE_CASE_RETURN_STRING(TBN_CUSTHELP);
#if (_WIN32_IE >= 0x0300)
            HANDLE_CASE_RETURN_STRING(TBN_DROPDOWN);
#endif //0x0300
#if (_WIN32_IE >= 0x0400)
            HANDLE_CASE_RETURN_STRING(TBN_GETOBJECT);
#endif //0x0400

            HANDLE_CASE_RETURN_STRING(TBN_HOTITEMCHANGE);
            HANDLE_CASE_RETURN_STRING(TBN_DRAGOUT);
            HANDLE_CASE_RETURN_STRING(TBN_DELETINGBUTTON);
            HANDLE_CASE_RETURN_STRING(TBN_GETDISPINFOA);
            HANDLE_CASE_RETURN_STRING(TBN_GETDISPINFOW);
            HANDLE_CASE_RETURN_STRING(TBN_GETINFOTIPA);
            HANDLE_CASE_RETURN_STRING(TBN_GETINFOTIPW);
            HANDLE_CASE_RETURN_STRING(TBN_GETBUTTONINFOW);
#if (_WIN32_IE >= 0x0500)
            HANDLE_CASE_RETURN_STRING(TBN_RESTORE);
            //HANDLE_CASE_RETURN_STRING(TBN_SAVE);
            HANDLE_CASE_RETURN_STRING(TBN_INITCUSTOMIZE);
#endif // (_WIN32_IE >= 0x0500)

            // updown
            HANDLE_CASE_RETURN_STRING(UDN_DELTAPOS); //ע�⣺UDN_DELTAPOS �� TBN_SAVE ��ֵһ����TBN_XXX ��ֵ������ TBN_LAST��

            // datetimepick
            HANDLE_CASE_RETURN_STRING(DTN_USERSTRINGA);
            HANDLE_CASE_RETURN_STRING(DTN_USERSTRINGW);
            HANDLE_CASE_RETURN_STRING(DTN_WMKEYDOWNA);
            HANDLE_CASE_RETURN_STRING(DTN_WMKEYDOWNW);
            HANDLE_CASE_RETURN_STRING(DTN_FORMATA);
            HANDLE_CASE_RETURN_STRING(DTN_FORMATW);
            HANDLE_CASE_RETURN_STRING(DTN_FORMATQUERYA);
            HANDLE_CASE_RETURN_STRING(DTN_FORMATQUERYW);

            // monthcal
            HANDLE_CASE_RETURN_STRING(MCN_SELCHANGE);
            HANDLE_CASE_RETURN_STRING(MCN_GETDAYSTATE);
            HANDLE_CASE_RETURN_STRING(MCN_SELECT);
            //HANDLE_CASE_RETURN_STRING(MCN_VIEWCHANGE);

            // datetimepick2
            HANDLE_CASE_RETURN_STRING(DTN_DATETIMECHANGE);
            HANDLE_CASE_RETURN_STRING(DTN_DROPDOWN);
            HANDLE_CASE_RETURN_STRING(DTN_CLOSEUP);

            // combo box ex
            HANDLE_CASE_RETURN_STRING(CBEN_GETDISPINFOA);
            HANDLE_CASE_RETURN_STRING(CBEN_INSERTITEM);
            HANDLE_CASE_RETURN_STRING(CBEN_DELETEITEM);
            HANDLE_CASE_RETURN_STRING(CBEN_BEGINEDIT);
            HANDLE_CASE_RETURN_STRING(CBEN_ENDEDITA);
            HANDLE_CASE_RETURN_STRING(CBEN_ENDEDITW);
            HANDLE_CASE_RETURN_STRING(CBEN_GETDISPINFOW);
            HANDLE_CASE_RETURN_STRING(CBEN_DRAGBEGINA);
            HANDLE_CASE_RETURN_STRING(CBEN_DRAGBEGINW);

            // rebar
            HANDLE_CASE_RETURN_STRING(RBN_HEIGHTCHANGE);
#if (_WIN32_IE >= 0x0400)
            HANDLE_CASE_RETURN_STRING(RBN_GETOBJECT);
            HANDLE_CASE_RETURN_STRING(RBN_LAYOUTCHANGED);
            HANDLE_CASE_RETURN_STRING(RBN_AUTOSIZE);
            HANDLE_CASE_RETURN_STRING(RBN_BEGINDRAG);
            HANDLE_CASE_RETURN_STRING(RBN_ENDDRAG);
            HANDLE_CASE_RETURN_STRING(RBN_DELETINGBAND);
            HANDLE_CASE_RETURN_STRING(RBN_DELETEDBAND);
            HANDLE_CASE_RETURN_STRING(RBN_CHILDSIZE);
#if (_WIN32_IE >= 0x0500)
            HANDLE_CASE_RETURN_STRING(RBN_CHEVRONPUSHED);
#endif      // _WIN32_IE >= 0x0500
#if (_WIN32_IE >= 0x0600)
            HANDLE_CASE_RETURN_STRING(RBN_SPLITTERDRAG);
#endif      // _WIN32_IE >= 0x0600
#if (_WIN32_IE >= 0x0500)
            HANDLE_CASE_RETURN_STRING(RBN_MINMAX);
#endif      // _WIN32_IE >= 0x0500
#if (_WIN32_WINNT >= 0x0501)
            HANDLE_CASE_RETURN_STRING(RBN_AUTOBREAK);
#endif //0x0501
#endif //0x0400


#if (_WIN32_IE >= 0x0400)
            // internet address
            HANDLE_CASE_RETURN_STRING(IPN_FIELDCHANGED);

            // status bar
            HANDLE_CASE_RETURN_STRING(SBN_SIMPLEMODECHANGE);

            // Pager Control
            HANDLE_CASE_RETURN_STRING(PGN_SCROLL);
            HANDLE_CASE_RETURN_STRING(PGN_CALCSIZE);
            HANDLE_CASE_RETURN_STRING(PGN_HOTITEMCHANGE);
#endif //_WIN32_IE >= 0x0400

#if (_WIN32_IE >= 0x0500)
            //WMN_FIRST
#endif //_WIN32_IE >= 0x0500

#if (_WIN32_WINNT >= 0x0501)
            //BCN_FIRST
#  ifndef NM_GETCUSTOMSPLITRECT
#    define NM_GETCUSTOMSPLITRECT       (BCN_FIRST + 0x0003)
#  endif
            HANDLE_CASE_RETURN_STRING(NM_GETCUSTOMSPLITRECT);
            HANDLE_CASE_RETURN_STRING(BCN_HOTITEMCHANGE);
#endif //_WIN32_WINNT >= 0x0501

#if (_WIN32_WINNT >= 0x600)
            HANDLE_CASE_RETURN_STRING(BCN_DROPDOWN);
#endif // _WIN32_WINNT >= 0x600


#if (_WIN32_WINNT >= 0x0600)
            // trackbar
            HANDLE_CASE_RETURN_STRING(TRBN_THUMBPOSCHANGING);
#endif //_WIN32_WINNT >= 0x0600
        }

        FTLTRACEEX(FTL::tlWarning, TEXT("Unknown Notify Code, %d\n"), nCode);
        return TEXT("Unknown");
    }

    LPCTSTR CFWinUtil::GetCommandNotifyString(HWND hWnd, UINT nCode, LPTSTR pszCommandNotify, int nLength, 
		TranslateWndClassProc pTransProc/* = g_pTranslateWndClassProc*/)
    {
		CHECK_POINTER_RETURN_VALUE_IF_FAIL(pszCommandNotify, NULL);
		BOOL bRet = FALSE;
		HRESULT hr = E_FAIL;

		pszCommandNotify[0] = 0;
		TCHAR szClassName[FTL_MAX_CLASS_NAME_LENGTH] = {0};
		API_VERIFY( 0 != GetClassName(hWnd, szClassName, _countof(szClassName)));
		if (pTransProc)
		{
			TCHAR szNewClassName[FTL_MAX_CLASS_NAME_LENGTH] = {0};
			API_VERIFY((*pTransProc)(szClassName, szNewClassName, _countof(szNewClassName)));
			if (bRet)
			{
				FTLTRACEEX(FTL::tlInfo, TEXT("Translate Window Class Name From %s to %s\n"), szClassName, szNewClassName);
				StringCchCopy(szClassName, _countof(szClassName), szNewClassName);
			}
		}
		if (0 == lstrcmpi(szClassName, TEXT("Button")))
		{
			switch(nCode)
			{
				HANDLE_CASE_TO_STRING(pszCommandNotify, nLength, BN_CLICKED);
				HANDLE_CASE_TO_STRING(pszCommandNotify, nLength, BN_PAINT);
				HANDLE_CASE_TO_STRING(pszCommandNotify, nLength, BN_HILITE);	//BN_PUSHED
				HANDLE_CASE_TO_STRING(pszCommandNotify, nLength, BN_UNHILITE);	//BN_UNPUSHED
				HANDLE_CASE_TO_STRING(pszCommandNotify, nLength, BN_DISABLE);
				HANDLE_CASE_TO_STRING(pszCommandNotify, nLength, BN_DOUBLECLICKED);	//BN_DBLCLK
				HANDLE_CASE_TO_STRING(pszCommandNotify, nLength, BN_SETFOCUS);
				HANDLE_CASE_TO_STRING(pszCommandNotify, nLength, BN_KILLFOCUS);
			default:
				break;
			}
		}
		else if (0 == lstrcmpi(szClassName, TEXT("ComboBox")))
		{
			//Combo Box Notification Codes
			switch(nCode)
			{
				HANDLE_CASE_TO_STRING(pszCommandNotify, nLength, CBN_ERRSPACE);
				HANDLE_CASE_TO_STRING(pszCommandNotify, nLength, CBN_SELCHANGE);
				HANDLE_CASE_TO_STRING(pszCommandNotify, nLength, CBN_DBLCLK);
				HANDLE_CASE_TO_STRING(pszCommandNotify, nLength, CBN_SETFOCUS);
				HANDLE_CASE_TO_STRING(pszCommandNotify, nLength, CBN_KILLFOCUS);
				HANDLE_CASE_TO_STRING(pszCommandNotify, nLength, CBN_EDITCHANGE);
				HANDLE_CASE_TO_STRING(pszCommandNotify, nLength, CBN_EDITUPDATE);
				HANDLE_CASE_TO_STRING(pszCommandNotify, nLength, CBN_DROPDOWN);
				HANDLE_CASE_TO_STRING(pszCommandNotify, nLength, CBN_CLOSEUP);
				HANDLE_CASE_TO_STRING(pszCommandNotify, nLength, CBN_SELENDOK);
				HANDLE_CASE_TO_STRING(pszCommandNotify, nLength, CBN_SELENDCANCEL);
			default:
				break;
			}
        }
		else if (0 == lstrcmpi(szClassName, TEXT("ListBox")))
		{
			//Combo Box Notification Codes
			switch(nCode)
			{
				HANDLE_CASE_TO_STRING(pszCommandNotify, nLength, LBN_SELCHANGE);
				HANDLE_CASE_TO_STRING(pszCommandNotify, nLength, LBN_DBLCLK);
				HANDLE_CASE_TO_STRING(pszCommandNotify, nLength, LBN_SELCANCEL);
				HANDLE_CASE_TO_STRING(pszCommandNotify, nLength, LBN_SETFOCUS);
				HANDLE_CASE_TO_STRING(pszCommandNotify, nLength, LBN_KILLFOCUS);
			default:
				break;
			}
		}
		else if (0 == lstrcmpi(szClassName, TEXT("Edit")))
		{
			switch(nCode)
			{
				HANDLE_CASE_TO_STRING(pszCommandNotify, nLength, EN_SETFOCUS);
				HANDLE_CASE_TO_STRING(pszCommandNotify, nLength, EN_KILLFOCUS);
				HANDLE_CASE_TO_STRING(pszCommandNotify, nLength, EN_CHANGE);
				HANDLE_CASE_TO_STRING(pszCommandNotify, nLength, EN_UPDATE);
				HANDLE_CASE_TO_STRING(pszCommandNotify, nLength, EN_ERRSPACE);
				HANDLE_CASE_TO_STRING(pszCommandNotify, nLength, EN_MAXTEXT);
				HANDLE_CASE_TO_STRING(pszCommandNotify, nLength, EN_HSCROLL);
				HANDLE_CASE_TO_STRING(pszCommandNotify, nLength, EN_VSCROLL);
#if(_WIN32_WINNT >= 0x0500)
				HANDLE_CASE_TO_STRING(pszCommandNotify, nLength, EN_ALIGN_LTR_EC);
				HANDLE_CASE_TO_STRING(pszCommandNotify, nLength, EN_ALIGN_RTL_EC);
#endif /* _WIN32_WINNT >= 0x0500 */
			default:
				break;
			}
		}
		else if(0 == lstrcmpi(szClassName, TEXT("RichEdit20W")))
		{
#ifdef _RICHEDIT_
			switch (nCode)
			{
				HANDLE_CASE_TO_STRING(pszCommandNotify, nLength, EN_MSGFILTER);
				HANDLE_CASE_TO_STRING(pszCommandNotify, nLength, EN_REQUESTRESIZE);
				HANDLE_CASE_TO_STRING(pszCommandNotify, nLength, EN_SELCHANGE);
				HANDLE_CASE_TO_STRING(pszCommandNotify, nLength, EN_DROPFILES);
				HANDLE_CASE_TO_STRING(pszCommandNotify, nLength, EN_PROTECTED);
				HANDLE_CASE_TO_STRING(pszCommandNotify, nLength, EN_CORRECTTEXT);
				HANDLE_CASE_TO_STRING(pszCommandNotify, nLength, EN_STOPNOUNDO);
				HANDLE_CASE_TO_STRING(pszCommandNotify, nLength, EN_IMECHANGE);
				HANDLE_CASE_TO_STRING(pszCommandNotify, nLength, EN_SAVECLIPBOARD);
				HANDLE_CASE_TO_STRING(pszCommandNotify, nLength, EN_OLEOPFAILED);
				HANDLE_CASE_TO_STRING(pszCommandNotify, nLength, EN_OBJECTPOSITIONS);
				HANDLE_CASE_TO_STRING(pszCommandNotify, nLength, EN_LINK);
				HANDLE_CASE_TO_STRING(pszCommandNotify, nLength, EN_DRAGDROPDONE);
				HANDLE_CASE_TO_STRING(pszCommandNotify, nLength, EN_PARAGRAPHEXPANDED);
				HANDLE_CASE_TO_STRING(pszCommandNotify, nLength, EN_PAGECHANGE);
				HANDLE_CASE_TO_STRING(pszCommandNotify, nLength, EN_LOWFIRTF);
				HANDLE_CASE_TO_STRING(pszCommandNotify, nLength, EN_ALIGNLTR);
				HANDLE_CASE_TO_STRING(pszCommandNotify, nLength, EN_ALIGNRTL);
			default:
				//StringCchCopy(pszCommandNotify,nLength,TEXT("Unknown RichEdit Notify"));
				break;
			}
#endif	//_RICHEDIT_
		}

		if ( 0 == pszCommandNotify[0] )
		{
			FTLTRACEEX(FTL::tlWarning, TEXT("Warning -- Unknown Command Code %d For Class %s\n"), nCode, szClassName);
			COM_VERIFY(StringCchPrintf(pszCommandNotify, nLength, TEXT("Unknown Command Code %d For Class %s"), nCode, szClassName));
			//FTLASSERT(FALSE);
		}
		return pszCommandNotify;
    }

	LPCTSTR CFWinUtil::GetWindowDescriptionInfo(FTL::CFStringFormater& formater, HWND hWnd)
	{
		BOOL bRet = FALSE;
		HRESULT hr = E_FAIL;
		if (::IsWindow(hWnd))
		{
			TCHAR szClass[FTL_MAX_CLASS_NAME_LENGTH] = {0};
			API_VERIFY(0 != GetClassName(hWnd, szClass, _countof(szClass)));

			TCHAR szName[FTL_MAX_CLASS_NAME_LENGTH] = {0};
			API_VERIFY_EXCEPT1(0 != GetWindowText(hWnd, szName, _countof(szName)), ERROR_SUCCESS);

			RECT rcWindow = {0};
			API_VERIFY(GetWindowRect(hWnd, &rcWindow));

			COM_VERIFY(formater.Format(TEXT("0x%x(%d), Class=%s, Name=%s, WinPos=(%d,%d)-(%d,%d) %dx%d"),
				hWnd, hWnd, szClass, szName, 
				rcWindow.left, rcWindow.top, rcWindow.right, rcWindow.bottom,
				rcWindow.right - rcWindow.left, rcWindow.bottom - rcWindow.top));
		}
		else
		{
			COM_VERIFY(formater.Format(TEXT("0x%x(%d) NOT valid Window"), hWnd, hWnd));
		}

		return formater.GetString();
	}

    LPCTSTR CFWinUtil::GetWindowClassString(FTL::CFStringFormater& formater, HWND hWnd,LPCTSTR pszDivide/* = TEXT("|") */)
    {
        FTLASSERT(::IsWindow(hWnd));
        ULONG_PTR clsStyle = ::GetClassLongPtr(hWnd,GCL_STYLE);
        API_ASSERT(clsStyle != 0);
        ULONG_PTR oldClsStyle = clsStyle;

        HANDLE_COMBINATION_VALUE_TO_STRING(formater, clsStyle, CS_VREDRAW, pszDivide);
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, clsStyle, CS_HREDRAW, pszDivide);
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, clsStyle, CS_DBLCLKS, pszDivide);
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, clsStyle, CS_OWNDC, pszDivide);
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, clsStyle, CS_CLASSDC, pszDivide);

        //* ���Ӵ��������� CS_PARENTDC ����,�������丸���ڵ���ʾ�豸�������Ͻ��л��� -- �� Edit/Button
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, clsStyle, CS_PARENTDC, pszDivide);

        HANDLE_COMBINATION_VALUE_TO_STRING(formater, clsStyle, CS_NOCLOSE, pszDivide);
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, clsStyle, CS_SAVEBITS, pszDivide);
		//���ֽڱ߽��϶�λ���ڵ��û������λ�� -- ��ʲô�ã�
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, clsStyle, CS_BYTEALIGNCLIENT, pszDivide);
		//���ֽڱ߽��϶�λ���ڵ�λ�� -- ��ʲô�ã�
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, clsStyle, CS_BYTEALIGNWINDOW, pszDivide);
		//Ӧ�ó���ȫ�ֵĴ�����--���Ա�Exe��Dllע�ᣬ�Խ���������ģ�鶼��Ч��ͨ�����ṩUI��DLL��ע��
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, clsStyle, CS_GLOBALCLASS, pszDivide);
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, clsStyle, CS_IME, pszDivide);
#if(_WIN32_WINNT >= 0x0501)
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, clsStyle, CS_DROPSHADOW, pszDivide);
#endif /* _WIN32_WINNT >= 0x0501 */

        FTLASSERT( 0 == clsStyle);
        //HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, XXXXXXXXX, pszDivide);
        if (0 != clsStyle)
        {
            FTLTRACEEX(FTL::tlWarning, TEXT("%s:Check Class String For HWND(0x%x) Not Complete, total=0x%x, remain=0x%x\n"),
                __FILE__LINE__, hWnd, oldClsStyle, clsStyle);
        }

        return formater.GetString();
    }

    LPCTSTR CFWinUtil::GetWindowStyleString(FTL::CFStringFormater& formater, HWND hWnd,LPCTSTR pszDivide/* = TEXT("|") */)
    {
        BOOL bRet = FALSE;

        FTLASSERT(::IsWindow(hWnd));
        LONG_PTR    lStyle = ::GetWindowLongPtr(hWnd, GWL_STYLE);
        LONG_PTR    lOldStyle = lStyle;
        //HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, WS_OVERLAPPED, pszDivide);
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, WS_POPUP, pszDivide);
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, WS_CHILDWINDOW, pszDivide); //WS_CHILD
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, WS_MINIMIZE, pszDivide);
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, WS_VISIBLE, pszDivide);
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, WS_DISABLED, pszDivide);
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, WS_CLIPSIBLINGS, pszDivide); //�ֵ��Ӵ��ڻ���ü�(ֻ����WS_CHILD)
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, WS_CLIPCHILDREN, pszDivide); //�������в������Ӵ���(�ü���Ƶ���Ŵ���)
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, WS_MAXIMIZE, pszDivide);
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, WS_CAPTION, pszDivide);
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, WS_BORDER, pszDivide);
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, WS_DLGFRAME, pszDivide);
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, WS_VSCROLL, pszDivide);
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, WS_HSCROLL, pszDivide);
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, WS_SYSMENU, pszDivide);
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, WS_THICKFRAME, pszDivide);
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, WS_GROUP, pszDivide);      //WS_MINIMIZEBOX
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, WS_TABSTOP, pszDivide);    //WS_MAXIMIZEBOX

        //ͨ�õ����Ͷ����� 0x00010000L�� ���ֱ�׼�ؼ����е� Style С�� 0x00010000L

        TCHAR szClassName[256+1] = {0}; //The maximum length for lpszClassName is 256.
        API_VERIFY(0 != ::GetClassName(hWnd, szClassName, _countof(szClassName)));
        
        //BUTTON
        if (0 ==  lstrcmpi(szClassName, WC_BUTTON))
        {
            HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, BS_LEFTTEXT, pszDivide);

			if (0 != (lStyle & BS_TYPEMASK))
			{
#if _WIN32_WINNT >= 0x0600
				HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, BS_DEFCOMMANDLINK, pszDivide);
				HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, BS_COMMANDLINK, pszDivide);
				HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, BS_DEFSPLITBUTTON, pszDivide);
				HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, BS_SPLITBUTTON, pszDivide);
#endif //_WIN32_WINNT >= 0x0600
				HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, BS_OWNERDRAW, pszDivide); //��ť���Ի�
				HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, BS_PUSHBOX, pszDivide);
				HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, BS_AUTORADIOBUTTON, pszDivide);
				HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, BS_USERBUTTON, pszDivide);
				HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, BS_GROUPBOX, pszDivide);
				HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, BS_AUTO3STATE, pszDivide);
				HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, BS_3STATE, pszDivide);
				HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, BS_RADIOBUTTON, pszDivide);
				HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, BS_AUTOCHECKBOX, pszDivide);
				HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, BS_CHECKBOX, pszDivide);
				HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, BS_DEFPUSHBUTTON, pszDivide);
			}
			else
			{
				//HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, BS_PUSHBUTTON, pszDivide);
				formater.AppendFormat(TEXT("%s%s"), TEXT("BS_PUSHBUTTON"), pszDivide);
			}

            HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, BS_FLAT, pszDivide);
            HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, BS_NOTIFY, pszDivide);
            HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, BS_MULTILINE, pszDivide);
            HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, BS_PUSHLIKE, pszDivide);
            HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, BS_VCENTER, pszDivide);
            HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, BS_BOTTOM, pszDivide);
            HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, BS_TOP, pszDivide);
            HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, BS_CENTER, pszDivide);
            HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, BS_RIGHT, pszDivide);
            HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, BS_LEFT, pszDivide);

			if ( 0 != (lStyle & (BS_ICON|BS_TEXT)))
			{
				HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, BS_BITMAP, pszDivide);
				HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, BS_ICON, pszDivide);
			}
			else
			{
				//HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, BS_TEXT, pszDivide);
				formater.AppendFormat(TEXT("%s%s"), TEXT("BS_TEXT"), pszDivide);
			}
        }

		//Combo Box styles
		if (0 ==  lstrcmp(szClassName, WC_COMBOBOX))
		{
			HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, CBS_DROPDOWNLIST, pszDivide);
			HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, CBS_DROPDOWN, pszDivide);
			HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, CBS_SIMPLE, pszDivide);

			HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, CBS_OWNERDRAWFIXED, pszDivide);
			HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, CBS_OWNERDRAWVARIABLE, pszDivide);
			HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, CBS_AUTOHSCROLL, pszDivide);
			HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, CBS_OEMCONVERT, pszDivide);
			HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, CBS_SORT, pszDivide);
			HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, CBS_HASSTRINGS, pszDivide);
			HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, CBS_NOINTEGRALHEIGHT, pszDivide);
			HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, CBS_DISABLENOSCROLL, pszDivide);
			HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, CBS_UPPERCASE, pszDivide);
			HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, CBS_LOWERCASE, pszDivide);
		}

		if (0 ==  lstrcmp(szClassName, WC_EDIT)                     //Edit Control Styles
            || 0 == lstrcmp(szClassName, TEXT("RICHEDIT50W")))      //Rich Edit Control Styles
		{
			#define ES_ALIGNMASK	(ES_LEFT | ES_CENTER | ES_RIGHT)
			if ( 0 != (lStyle & ES_ALIGNMASK))
			{
				HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, ES_RIGHT, pszDivide);
				HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, ES_CENTER, pszDivide);
			}
			else
			{
				//HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, ES_LEFT, pszDivide);
				formater.AppendFormat(TEXT("%s%s"), TEXT("ES_LEFT"), pszDivide);
			}

			HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, ES_MULTILINE, pszDivide);
			HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, ES_UPPERCASE | ES_NOOLEDRAGDROP, pszDivide);
			HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, ES_LOWERCASE, pszDivide);
			HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, ES_PASSWORD, pszDivide);
			HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, ES_AUTOVSCROLL, pszDivide);
			HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, ES_AUTOHSCROLL, pszDivide);
			HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, ES_NOHIDESEL, pszDivide);

			HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, ES_OEMCONVERT, pszDivide);
			HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, ES_READONLY, pszDivide);
			HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, ES_WANTRETURN, pszDivide);
            HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, ES_NUMBER | ES_DISABLENOSCROLL, pszDivide); //������ֵһ����Ϊʲô
            HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, ES_SUNKEN, pszDivide); 
            HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, ES_SAVESEL, pszDivide);

            HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, ES_NOIME, pszDivide);
            HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, ES_VERTICAL, pszDivide);
            HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, ES_SELECTIONBAR, pszDivide);
		}

        //ListView
        if (0 ==  lstrcmp(szClassName, WC_LISTVIEW))
        {
			if (0 != (lStyle & LVS_TYPEMASK))
			{
				HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, LVS_LIST, pszDivide);
				HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, LVS_SMALLICON, pszDivide);
				HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, LVS_REPORT, pszDivide);
			}
			else
			{
				//HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, LVS_ICON, pszDivide);
				formater.AppendFormat(TEXT("%s%s"), TEXT("LVS_ICON"), pszDivide);
			}

			HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, LVS_SINGLESEL, pszDivide);
            HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, LVS_SHOWSELALWAYS, pszDivide);
            HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, LVS_SORTASCENDING, pszDivide);
            HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, LVS_SORTDESCENDING, pszDivide);
            HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, LVS_SHAREIMAGELISTS, pszDivide);
            HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, LVS_NOLABELWRAP, pszDivide);
            HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, LVS_AUTOARRANGE, pszDivide);
            HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, LVS_EDITLABELS, pszDivide);

			//���б�(�����б�)���� -- ����ʾ��ʱ��Ż�ȡ�������Ϣ�����Դ������UI��Դ������
            HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, LVS_OWNERDATA, pszDivide);
            HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, LVS_NOSCROLL, pszDivide);
            HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, LVS_TYPESTYLEMASK, pszDivide);

			if (0 != (lStyle & LVS_ALIGNMASK ))
			{
				HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, LVS_ALIGNLEFT, pszDivide);
			}
			else
			{
				//HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, LVS_ALIGNTOP, pszDivide);
				formater.AppendFormat(TEXT("%s%s"), TEXT("LVS_ALIGNTOP"), pszDivide);
			}

			HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, LVS_NOSORTHEADER, pszDivide);
            HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, LVS_NOCOLUMNHEADER, pszDivide);
            HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, LVS_OWNERDRAWFIXED, pszDivide);
        }

		//Scroll Bar Styles
		if (0 ==  lstrcmp(szClassName, WC_SCROLLBAR))
		{
			HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, SBS_SIZEGRIP, pszDivide);
			HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, SBS_SIZEBOX, pszDivide);
			HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, SBS_BOTTOMALIGN|SBS_RIGHTALIGN|SBS_SIZEBOXBOTTOMRIGHTALIGN, pszDivide);
			HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, SBS_TOPALIGN|SBS_LEFTALIGN|SBS_SIZEBOXTOPLEFTALIGN, pszDivide);
			if (0 != (lStyle & SBS_VERT))
			{
				HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, SBS_VERT, pszDivide);
			}
			else
			{
				//HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, SBS_HORZ, pszDivide);
				formater.AppendFormat(TEXT("%s%s"), TEXT("SBS_HORZ"), pszDivide);
			}
		}

		//SysHeader32 Box styles
		if (0 ==  lstrcmp(szClassName, WC_HEADER))
		{
			//HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, HDS_HORZ, pszDivide);
			formater.AppendFormat(TEXT("%s%s"), TEXT("HDS_HORZ"), pszDivide);

			HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, HDS_BUTTONS, pszDivide);
			HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, HDS_HOTTRACK, pszDivide);
			HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, HDS_HIDDEN, pszDivide);
			HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, HDS_DRAGDROP, pszDivide);
			HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, HDS_FULLDRAG, pszDivide);
#if (_WIN32_IE >= 0x0500)
			HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, HDS_FILTERBAR, pszDivide);
#endif //_WIN32_IE >= 0x0500

#if (_WIN32_WINNT >= 0x0501)
			HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, HDS_FLAT, pszDivide);
#endif //_WIN32_WINNT >= 0x0501
#if _WIN32_WINNT >= 0x0600
			HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, HDS_CHECKBOXES, pszDivide);
			HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, HDS_NOSIZING, pszDivide);
			HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, HDS_OVERFLOW, pszDivide);
#endif
		}

        FTLASSERT( 0 == lStyle);
		//HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, XXXXXXXXX, pszDivide);
        if (0 != lStyle)
        {
            FTLTRACEEX(FTL::tlWarning, TEXT("%s:Check Style String For \"%s\" Not Complete, total=0x%08x, remain=0x%08x\n"),
                __FILE__LINE__, szClassName, lOldStyle, lStyle);
        }
        return formater.GetString();
    }

    LPCTSTR CFWinUtil::GetWindowExStyleString(FTL::CFStringFormater& formater, HWND hWnd, LPCTSTR pszDivide /* = TEXT */)
    {
        FTLASSERT(::IsWindow(hWnd));
        LONG_PTR    lExStyle = ::GetWindowLongPtr(hWnd, GWL_EXSTYLE);
        LONG_PTR    lOldExStyle = lExStyle;

        HANDLE_COMBINATION_VALUE_TO_STRING(formater, lExStyle, WS_EX_DLGMODALFRAME, pszDivide);
		//���ô���(Child Window)����������ʱ����������巢�� WM_PARENTNOTIFY 
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, lExStyle, WS_EX_NOPARENTNOTIFY, pszDivide);
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, lExStyle, WS_EX_TOPMOST, pszDivide);
		//���Խ��� drag-and-drop �ļ�(���յ� WM_DROPFILES ��Ϣ)
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, lExStyle, WS_EX_ACCEPTFILES, pszDivide);
		//͸���ģ������ڸ����·��Ĵ��壬�����Ϣ�ᴩ͸�˴���(����Ӧ������Ӱ)���������·������д��嶼������Ϻ��յ� WM_PAINT ��Ϣ
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, lExStyle, WS_EX_TRANSPARENT, pszDivide);	
#if(WINVER >= 0x0400)
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, lExStyle, WS_EX_MDICHILD, pszDivide);

		//ͨ�����ڸ���������(floating toolbar) -- С�ı����������������������� Alt+Tab �б���
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, lExStyle, WS_EX_TOOLWINDOW, pszDivide);
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, lExStyle, WS_EX_WINDOWEDGE, pszDivide);
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, lExStyle, WS_EX_CLIENTEDGE, pszDivide);	//��3D�ͻ�����ۣ�����һ������(sunken edge)
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, lExStyle, WS_EX_CONTEXTHELP, pszDivide);
		if (0 != (lExStyle & WS_EX_RIGHT) )
		{
			HANDLE_COMBINATION_VALUE_TO_STRING(formater, lExStyle, WS_EX_RIGHT, pszDivide);
		}
		else
		{
			//HANDLE_COMBINATION_VALUE_TO_STRING(formater, lExStyle, WS_EX_LEFT, pszDivide);
			formater.AppendFormat(TEXT("%s%s"), TEXT("WS_EX_LEFT"), pszDivide);
		}
		if (0 != (lExStyle & WS_EX_RTLREADING))
		{
			HANDLE_COMBINATION_VALUE_TO_STRING(formater, lExStyle, WS_EX_RTLREADING, pszDivide);
		}
		else
		{
			//HANDLE_COMBINATION_VALUE_TO_STRING(formater, lExStyle, WS_EX_LTRREADING, pszDivide);
			formater.AppendFormat(TEXT("%s%s"), TEXT("WS_EX_LTRREADING"), pszDivide);
		}
		if (0 != (lExStyle & WS_EX_LEFTSCROLLBAR))
		{
			HANDLE_COMBINATION_VALUE_TO_STRING(formater, lExStyle, WS_EX_LEFTSCROLLBAR, pszDivide);
		}
		else
		{
			//HANDLE_COMBINATION_VALUE_TO_STRING(formater, lExStyle, WS_EX_RIGHTSCROLLBAR, pszDivide);
			formater.AppendFormat(TEXT("%s%s"), TEXT("WS_EX_RIGHTSCROLLBAR"), pszDivide);
		}
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, lExStyle, WS_EX_CONTROLPARENT, pszDivide);
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, lExStyle, WS_EX_STATICEDGE, pszDivide);
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, lExStyle, WS_EX_APPWINDOW, pszDivide);	//������ʱ���������ϻ���� Top-Level����
#endif /* WINVER >= 0x0400 */

#if(_WIN32_WINNT >= 0x0500)
        //�����ֲ㴰��
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, lExStyle, WS_EX_LAYERED, pszDivide);
#endif /* _WIN32_WINNT >= 0x0500 */

#if(WINVER >= 0x0500)
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, lExStyle, WS_EX_NOINHERITLAYOUT, pszDivide);
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, lExStyle, WS_EX_LAYOUTRTL, pszDivide);
#endif /* WINVER >= 0x0500 */

#if(_WIN32_WINNT >= 0x0501)
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, lExStyle, WS_EX_COMPOSITED, pszDivide);
#endif /* _WIN32_WINNT >= 0x0501 */

#if(_WIN32_WINNT >= 0x0500)
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, lExStyle, WS_EX_NOACTIVATE, pszDivide);
#endif /* _WIN32_WINNT >= 0x0500 */

        FTLASSERT( 0 == lExStyle);
        if (0 != lExStyle)
        {
            FTLTRACEEX(FTL::tlWarning, TEXT("%s: Check ExStyle String Not Complete, total=0x%08x, remain=0x%08x\n"),
                __FILE__LINE__, lOldExStyle, lExStyle);
        }
        return formater.GetString();
    }

	LPCTSTR CFWinUtil::GetWindowPosFlagsString(FTL::CFStringFormater& formater, UINT flags, LPCTSTR pszDivide /* = TEXT */)
	{
		UINT    nOldFlags = flags;

		HANDLE_COMBINATION_VALUE_TO_STRING(formater, flags, SWP_NOSIZE, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, flags, SWP_NOMOVE, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, flags, SWP_NOZORDER, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, flags, SWP_NOREDRAW, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, flags, SWP_NOACTIVATE, pszDivide);
		
		//�����µı߿�����(���� DUILib��ȥ�� WS_CAPTION ��)
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, flags, SWP_FRAMECHANGED, pszDivide);

		HANDLE_COMBINATION_VALUE_TO_STRING(formater, flags, SWP_SHOWWINDOW, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, flags, SWP_HIDEWINDOW, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, flags, SWP_NOCOPYBITS, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, flags, SWP_NOOWNERZORDER, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, flags, SWP_NOSENDCHANGING, pszDivide);


#if(WINVER >= 0x0400)
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, flags, SWP_DEFERERASE, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, flags, SWP_ASYNCWINDOWPOS, pszDivide);
#endif /* WINVER >= 0x0400 */

#ifndef SWP_NOCLIENTSIZE 
#  define SWP_NOCLIENTSIZE  0x0800		//Undocumented flags
#endif 
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, flags, SWP_NOCLIENTSIZE, pszDivide);

#ifndef SWP_NOCLIENTMOVE
#  define SWP_NOCLIENTMOVE	0x1000		//Undocumented flags
#endif 
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, flags, SWP_NOCLIENTMOVE, pszDivide);

#ifndef SWP_STATECHANGED  // window state (e.g. minimized, normalized, maximized) is changing or has changed,
#  define SWP_STATECHANGED  0x8000		//Undocumented flags
#endif 
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, flags, SWP_STATECHANGED, pszDivide);

#ifndef SWP_UNKNOWN_0X01000000
#  define SWP_UNKNOWN_0X01000000  0x01000000	//Undocumented flags
#endif 
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, flags, SWP_UNKNOWN_0X01000000, pszDivide);

#ifndef SWP_UNKNOWN_0X10000000
#  define SWP_UNKNOWN_0X10000000  0x10000000	//Undocumented flags
#endif 
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, flags, SWP_UNKNOWN_0X10000000, pszDivide);

#ifndef SWP_UNKNOWN_0X20000000
#  define SWP_UNKNOWN_0X20000000  0x20000000	//Undocumented flags
#endif 
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, flags, SWP_UNKNOWN_0X20000000, pszDivide);

#ifndef SWP_UNKNOWN_0X40000000
#  define SWP_UNKNOWN_0X40000000  0x40000000	//Undocumented flags
#endif 
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, flags, SWP_UNKNOWN_0X40000000, pszDivide);

		FTLASSERT( 0 == flags);
		if (0 != flags)
		{
			FTLTRACEEX(FTL::tlWarning, TEXT("%s: WARNING, Check Set Window Pos String Not Complete, total=0x%08x, remain=0x%08x\n"),
				__FILE__LINE__, nOldFlags, flags);
		}
		return formater.GetString();
	}

	LPCTSTR CFWinUtil::GetOwnerDrawState(FTL::CFStringFormater& formater, UINT itemState, LPCTSTR pszDivide)
	{
		UINT    oldItemState = itemState;

		HANDLE_COMBINATION_VALUE_TO_STRING(formater, itemState, ODS_SELECTED, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, itemState, ODS_GRAYED, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, itemState, ODS_DISABLED, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, itemState, ODS_CHECKED, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, itemState, ODS_FOCUS, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, itemState, ODS_DEFAULT, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, itemState, ODS_COMBOBOXEDIT, pszDivide);
#if(WINVER >= 0x0500)
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, itemState, ODS_HOTLIGHT, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, itemState, ODS_INACTIVE, pszDivide);
#if(_WIN32_WINNT >= 0x0500)
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, itemState, ODS_NOACCEL, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, itemState, ODS_NOFOCUSRECT, pszDivide);
#endif /* _WIN32_WINNT >= 0x0500 */
#endif /* WINVER >= 0x0500 */

		FTLASSERT( 0 == itemState);
		if (0 != itemState)
		{
			FTLTRACEEX(FTL::tlWarning, TEXT("%s: GetOwnerDrawState Not Complete, total=0x%08x, remain=0x%08x\n"),
				__FILE__LINE__, oldItemState, itemState);
		}
		return formater.GetString();
	}


    LPCTSTR CFWinUtil::GetOwnerDrawAction(CFStringFormater& formater, UINT itemAction, LPCTSTR pszDivide)
    {
        //UINT    oldItemAction = itemAction;

        HANDLE_COMBINATION_VALUE_TO_STRING(formater, itemAction, ODA_DRAWENTIRE, pszDivide);
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, itemAction, ODA_SELECT, pszDivide);
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, itemAction, ODA_FOCUS, pszDivide);

        FTLASSERT(0 == itemAction);
        return formater.GetString();
    }
    ///////////////////////////////////////////////////////////////////////////////////////////////////

	LPCTSTR CFWinHookUtil::GetCBTCodeInfo(CFStringFormater& formater, int nCode, WPARAM wParam, LPARAM lParam)
	{
		//FTLTRACE(TEXT("Enter GetCBTCodeInfo,TickCount=%d, nCode=%d\n"),GetTickCount(), nCode);
		switch(nCode)
		{
			HANDLE_CASE_TO_STRING_FORMATER(formater, HCBT_MOVESIZE);
			HANDLE_CASE_TO_STRING_FORMATER(formater, HCBT_MINMAX);
			HANDLE_CASE_TO_STRING_FORMATER(formater, HCBT_QS);
			HANDLE_CASE_TO_STRING_FORMATER(formater, HCBT_CREATEWND);
			HANDLE_CASE_TO_STRING_FORMATER(formater, HCBT_DESTROYWND);
		case HCBT_ACTIVATE:
			{
				CFStringFormater formaterActivae;
				CFStringFormater formaterActivateStruct;
				formaterActivateStruct.Format(TEXT("%s"), TEXT(""));

				HWND hWndActive = (HWND)wParam;
				CFWinUtil::GetWindowDescriptionInfo(formaterActivae, hWndActive);

				CBTACTIVATESTRUCT * pCBTActivateStruct = (CBTACTIVATESTRUCT*)lParam;
				if (pCBTActivateStruct)
				{
					CFStringFormater formaterActivaeInStruct;
					formaterActivateStruct.Format(TEXT("fMouse=%d, hWndActive=%s"),
						pCBTActivateStruct->fMouse, 
						CFWinUtil::GetWindowDescriptionInfo(formaterActivaeInStruct, pCBTActivateStruct->hWndActive));
				}
				formater.Format(TEXT("HCBT_ACTIVATE -- Active=%s, Struct=%s"), formaterActivae.GetString(), formaterActivateStruct.GetString());
				break;
			}
			HANDLE_CASE_TO_STRING_FORMATER(formater,HCBT_CLICKSKIPPED);
			HANDLE_CASE_TO_STRING_FORMATER(formater,HCBT_KEYSKIPPED);
			HANDLE_CASE_TO_STRING_FORMATER(formater,HCBT_SYSCOMMAND);
		case HCBT_SETFOCUS:
			{
				HWND hWndGetFocus = (HWND)wParam;
				HWND hWndLostFocus = (HWND)lParam;
				CFStringFormater formaterGetFocus, formaterLostFocus;
				formater.Format(TEXT("HCBT_SETFOCUS -- GetFocus=%s, LostFocus=%s"), 
					CFWinUtil::GetWindowDescriptionInfo(formaterGetFocus, hWndGetFocus),
					CFWinUtil::GetWindowDescriptionInfo(formaterLostFocus, hWndLostFocus));
				break;
			}

		default:
			break;
		}
		return formater.GetString();
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////

    __declspec(selectany) HHOOK CFMessageBoxHook::s_hHook = NULL;
    __declspec(selectany) HWND  CFMessageBoxHook::s_ProphWnd = NULL;
    __declspec(selectany) LPCTSTR CFMessageBoxHook::s_pszOKString = NULL;

    CFMessageBoxHook::CFMessageBoxHook(DWORD dwThreadId, LPCTSTR pszOKString)
    {
        BOOL bRet = FALSE;
        CFMessageBoxHook::s_pszOKString = pszOKString;
        s_hHook = ::SetWindowsHookEx(WH_CBT, (HOOKPROC)CFMessageBoxHook::CBTProc
            , NULL, dwThreadId);
        API_VERIFY(NULL != s_hHook);
    }

    CFMessageBoxHook::~CFMessageBoxHook(void)
    {
        if (CFMessageBoxHook::s_ProphWnd)
        {
            RemoveProp(CFMessageBoxHook::s_ProphWnd,PREV_WND_PROC_NAME);
            CFMessageBoxHook::s_ProphWnd = NULL;
        }
        if (CFMessageBoxHook::s_hHook)
        {
            ::UnhookWindowsHookEx(CFMessageBoxHook::s_hHook);
            CFMessageBoxHook::s_hHook = NULL;
        }
    }


    LRESULT CFMessageBoxHook::CBTProc(int nCode, WPARAM wParam, LPARAM lParam)
    {
        if (NULL == CFMessageBoxHook::s_hHook)
            return 0;
        BOOL bRet = FALSE;

        if (nCode == HCBT_CREATEWND){ //HCBT_CREATEWND = 3
            HWND hWnd = (HWND)wParam;
            TCHAR className[MAX_PATH];
            ::GetClassName(hWnd, className, _countof(className));
            if (_tcscmp(className, _T("#32770")) == 0)
            {
                WNDPROC prevWndProc = (WNDPROC)::SetWindowLongPtr(hWnd, GWLP_WNDPROC, (LONG_PTR)CFMessageBoxHook::WndProc);
                API_VERIFY(::SetProp(hWnd, PREV_WND_PROC_NAME, (HANDLE)prevWndProc));
                if (bRet)
                {
                    CFMessageBoxHook::s_ProphWnd = hWnd;
                }
            }
        }
        return ::CallNextHookEx(s_hHook, nCode, wParam, lParam);
    }

    LRESULT CFMessageBoxHook::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
    {
        WNDPROC prevWndProc = (WNDPROC)::GetProp(hWnd, PREV_WND_PROC_NAME);
        FTLASSERT(prevWndProc != NULL);

        if (message == WM_INITDIALOG)
        {
            BOOL bRet = FALSE;
            API_VERIFY(SetDlgItemText(hWnd, IDOK, CFMessageBoxHook::s_pszOKString));
        }
        return ::CallWindowProc(prevWndProc, hWnd, message, wParam, lParam);
    }

#if 0
    template <typename ControlT , typename ConverterFun>
    CControlPropertyHandleT<ControlT, ConverterFun>::
        CControlPropertyHandleT(ControlT& control)//, ConverterFun& fun)
        :m_control(control)
        //,m_fun(fun)
    {
    }

    template <typename ControlT , typename ConverterFun>
    INT CControlPropertyHandleT<ControlT, ConverterFun>::AddProperty(INT value)
    {
        INT index = m_control.AddString(ConverterFun(value));
        m_control.SetItemData(value);
        return index;
    }

#endif


}//FTL


#endif //FTL_WINDOW_HPP