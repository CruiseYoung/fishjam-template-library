#ifndef FTL_WINDOW_HPP
#define FTL_WINDOW_HPP
#pragma once

#ifdef USE_EXPORT
#  include "ftlwindow.h"
#endif

#include <prsht.h>

namespace FTL
{
    CFMessageInfo::CFMessageInfo(UINT msg) : CFConvertInfoT<CFMessageInfo,UINT>(msg)
    {
    }

    LPCTSTR CFMessageInfo::ConvertInfo()
    {
        if (NULL == m_bufInfo[0])
        {
            switch(m_Info)
            {
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_CREATE);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_DESTROY);  //�رմ���ʱ,�����������,����PostQuitMessage,������̻�������(ֻ��û�д���)
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_MOVE);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_SIZE);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_ACTIVATE); //���ڱ������ʧȥ����״̬, �����ڲ鿴�����Ƿ���С������ǰһ����
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_SETFOCUS);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_KILLFOCUS);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_ENABLE); //����ֹ�򼤻�ʱ
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_SETREDRAW); //���ô����Ƿ����ػ� 
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_SETTEXT);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_GETTEXT);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_GETTEXTLENGTH); //�õ���һ�������йص��ı��ĳ��ȣ����������ַ���
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_PAINT);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_CLOSE);
#ifndef _WIN32_WCE
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_QUERYENDSESSION); //���û�ѡ������Ի��������Լ�����ExitWindows����
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_QUERYOPEN);     //���û����ڻָ���ǰ�Ĵ�Сλ��ʱ���Ѵ���Ϣ���͸�ĳ��ͼ��
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_ENDSESSION);    //��ϵͳ���̷���WM_QUERYENDSESSION��Ϣ�󣬴���Ϣ���͸�Ӧ�ó���֪ͨ���Ի��Ƿ����
#endif
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_QUIT);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_ERASEBKGND); //�����ڱ������뱻����ʱ�����ڴ��ڸı��Сʱ��
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_SYSCOLORCHANGE);    //��ϵͳ��ɫ�ı�ʱ�����ʹ���Ϣ�����ж�������
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_SHOWWINDOW);

#ifndef WM_CTLCOLOR
#  define WM_CTLCOLOR                             0x0019
#endif
                //�Ӵ������ڻ��ƿͻ���֮ǰ�����䷢�͸������ڣ�ί�и�����Ϊ�Լ�׼��һ����ˢ��ͬʱ�����ڣ���Ի���Ҳ���Լ����͸���Ϣ(Ϊʲô��)
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_CTLCOLOR);  

                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_WININICHANGE);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_DEVMODECHANGE);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_ACTIVATEAPP);//������һApp�Ĵ��彫����ʱ
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_FONTCHANGE);    //��ϵͳ��������Դ��仯ʱ���ʹ���Ϣ�����ж�������
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_TIMECHANGE);    //��ϵͳ��ʱ��仯ʱ���ʹ���Ϣ�����ж�������
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_CANCELMODE);    //ȡ��ϵͳģʽʱ
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_SETCURSOR);     //��������������ĳ���������ƶ����������û�б�����ʱ���ͷ���Ϣ��ĳ������
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_MOUSEACTIVATE);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_CHILDACTIVATE); //�ʹ���Ϣ��MDI�Ӵ��ڵ��û�����˴��ڵı��������򵱴��ڱ�����ƶ����ı��С
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_QUEUESYNC);     //����Ϣ�ɻ��ڼ������"ѵ��?"�����ͣ�ͨ�� WH_JOURNALPALYBACK ��hook���������û�������Ϣ
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_GETMINMAXINFO); //����Ϣ���͸����ڵ�����Ҫ�ı��С��λ��

                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_PAINTICON);     //���͸���С�����ڵ���ͼ�꽫Ҫ���ػ�
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_ICONERASEBKGND);//����Ϣ���͸�ĳ����С�����ڣ��������ڻ�ͼ��ǰ���ı������뱻�ػ�
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_NEXTDLGCTL);    //����Ϣ���͸�ĳ����С�����ڣ��������ڻ�ͼ��ǰ���ı������뱻�ػ�
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_SPOOLERSTATUS); //ÿ����ӡ�����ж����ӻ����һ����ҵʱ��������Ϣ
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_DRAWITEM);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_MEASUREITEM);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_DELETEITEM);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_VKEYTOITEM);    //��һ��LBS_WANTKEYBOARDINPUT���ķ�������������������ӦWM_KEYDOWN��Ϣ
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_CHARTOITEM);    //��һ��LBS_WANTKEYBOARDINPUT�����б���͸���������������ӦWM_CHAR��Ϣ
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_SETFONT);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_GETFONT);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_SETHOTKEY);     //Ӧ�ó����ʹ���Ϣ��һ��������һ���ȼ������
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_GETHOTKEY);     //Ӧ�ó����ʹ���Ϣ���ж��ȼ���ĳ�������Ƿ��й���
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_QUERYDRAGICON); //�͸���С�����ڣ����˴��ڽ�Ҫ���ϷŶ���������û�ж���ͼ�꣬Ӧ�ó����ܷ���һ��ͼ�����ľ����
                //���û��Ϸ�ͼ��ʱϵͳ��ʾ���ͼ�����
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_COMPAREITEM);   //���ʹ���Ϣ���ж�combobox��listbox�����ӵ�������λ��
#if(WINVER >= 0x0500)
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_GETOBJECT);
#endif
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_COMPACTING);    //��ʾ�ڴ��Ѿ�������
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_COMMNOTIFY);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_WINDOWPOSCHANGING);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_WINDOWPOSCHANGED);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_POWER);         //��ϵͳ��Ҫ������ͣ״̬ʱ���ʹ���Ϣ
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_COPYDATA);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_CANCELJOURNAL); //��ĳ���û�ȡ��������־����״̬���ύ����Ϣ������
#if(WINVER >= 0x0400)
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_NOTIFY);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_INPUTLANGCHANGEREQUEST);    //���û�ѡ��ĳ���������ԣ����������Ե��ȼ��ı�
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_INPUTLANGCHANGE);   //��ƽ̨�ֳ��Ѿ����ı���ʹ���Ϣ����Ӱ����������
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_TCARD);         //�������Ѿ���ʼ��windows��������ʱ���ʹ���Ϣ��Ӧ�ó���
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_HELP);          //����Ϣ��ʾ�û�������F1�����ĳ���˵��Ǽ���ģ��ͷ��ʹ���Ϣ���˴��ڹ����Ĳ˵���
                //  ����ͷ��͸��н���Ĵ��ڣ������ǰ��û�н��㣬�ͰѴ���Ϣ���͸���ǰ����Ĵ���
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_USERCHANGED);   //���û��Ѿ�������˳����ʹ���Ϣ�����еĴ��ڣ����û�������˳�ʱϵͳ�����û��ľ���
                //  ������Ϣ�����û���������ʱϵͳ���Ϸ��ʹ���Ϣ
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_NOTIFYFORMAT);  //ͨ������Ϣ���жϿؼ���ʹ��ANSI����UNICODE�ṹ
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_CONTEXTMENU);   //���û�ĳ�������е����һ���Ҽ��ͷ��ʹ���Ϣ���������
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_STYLECHANGING); //������SETWINDOWLONG���� ��Ҫ�ı� һ������ ���ڵķ��ʱ���ʹ���Ϣ���Ǹ�����
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_STYLECHANGED);  //������SETWINDOWLONG���� �ı� һ���������ڵķ����ʹ���Ϣ���Ǹ�����
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_DISPLAYCHANGE); //����ʾ���ķֱ��ʸı���ʹ���Ϣ�����еĴ���
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_GETICON);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_SETICON);
#endif
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_NCCREATE);      //��ĳ�����ڵ�һ�α�����ʱ������Ϣ��WM_CREATE��Ϣ����ǰ����
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_NCDESTROY);     //����Ϣ֪ͨĳ�����ڣ��ǿͻ�����������
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_NCCALCSIZE);    //��ĳ�����ڵģ��ǣ����ͻ�������뱻����ʱ���ʹ���Ϣ
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_NCHITTEST);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_NCPAINT);       //�����ʹ���Ϣ��ĳ�����ڵ��������ڣ��Ŀ�ܱ��뱻����ʱ
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_NCACTIVATE);    //����Ϣ���͸�ĳ������ �������ķǿͻ�����Ҫ���ı�����ʾ�Ǽ���ǷǼ���״̬
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_GETDLGCODE);    //���ʹ���Ϣ��ĳ����Ի����������Ŀؼ���widdows���Ʒ�λ����TAB��ʹ�������˿ؼ�
                //  ͨ����ӦWM_GETDLGCODE��Ϣ��Ӧ�ó�����԰�������һ�����������ؼ����ܴ�����
#ifndef _WIN32_WCE
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_SYNCPAINT);
#endif
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_NCMOUSEMOVE);   //�ǿͻ������ƶ�ʱ���ʹ���Ϣ���������
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_NCLBUTTONDOWN);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_NCLBUTTONUP);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_NCLBUTTONDBLCLK);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_NCRBUTTONDOWN);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_NCRBUTTONUP);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_NCRBUTTONDBLCLK);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_NCMBUTTONDOWN);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_NCMBUTTONUP);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_NCMBUTTONDBLCLK);

#if(_WIN32_WINNT >= 0x0500)
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_NCXBUTTONDOWN);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_NCXBUTTONUP);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_NCXBUTTONDBLCLK);
#endif

#if(_WIN32_WINNT >= 0x0501)
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_INPUT);
#endif
                //HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_KEYFIRST);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_KEYDOWN);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_KEYUP);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_CHAR);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_DEADCHAR); //���� translatemessage ��������WM_KEYUP��Ϣʱ���ʹ���Ϣ��ӵ�н���Ĵ���
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_SYSKEYDOWN);    //���û���סALT��ͬʱ����������ʱ�ύ����Ϣ��ӵ�н���Ĵ���
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_SYSKEYUP);      //���û��ͷ�һ����ͬʱALT ��������ʱ�ύ����Ϣ��ӵ�н���Ĵ���
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_SYSCHAR);       //��WM_SYSKEYDOWN��Ϣ��TRANSLATEMESSAGE����������ύ����Ϣ��ӵ�н���Ĵ�
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_SYSDEADCHAR);   //
#if(_WIN32_WINNT >= 0x0501)
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_UNICHAR);
                //HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_KEYLAST);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),UNICODE_NOCHAR);
#else
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_KEYLAST);
#endif

#if(WINVER >= 0x0400)
#endif
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_IME_STARTCOMPOSITION);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_IME_ENDCOMPOSITION);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_IME_COMPOSITION);
                //HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_IME_KEYLAST);

                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_INITDIALOG); //��һ���Ի��������ʾǰ���ʹ���Ϣ������ͨ���ô���Ϣ��ʼ���ؼ���ִ����������
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_COMMAND);

                //������������Ļ��������ʾ���ڵ�ģʽ(SC_SCREENSAVE/SC_MONITORPOWER),����0
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_SYSCOMMAND);    //���û�ѡ�񴰿ڲ˵���һ��������û�ѡ����󻯻���С��ʱ�Ǹ����ڻ��յ�����Ϣ

                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_TIMER);     //�����˶�ʱ���¼�
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_HSCROLL);   //ˮƽ����������һ�������¼�
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_VSCROLL);   //��ֱ����������һ�������¼�
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_INITMENU);  //��һ���˵���Ҫ������ʱ���ʹ���Ϣ�����������û��˵����е�ĳ�����ĳ���˵�����
                //  �������������ʾǰ���Ĳ˵�
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_INITMENUPOPUP);
#ifdef WM_SYSTIMER
#error Already define WM_SYSTIMER
#endif 

#ifndef WM_SYSTIMER
#define WM_SYSTIMER 0x0118
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_SYSTIMER);  //UnDocument Message
#endif

                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_MENUSELECT); //���û�ѡ��һ���˵���ʱ���ʹ���Ϣ���˵��������ߣ�һ���Ǵ��ڣ�
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_MENUCHAR);  //���˵��ѱ������û�������ĳ��������ͬ�ڼ��ټ��������ʹ���Ϣ���˵���������
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_ENTERIDLE); //��һ��ģ̬�Ի����˵��������״̬ʱ(������һ��������ǰ����Ϣ��û����Ϣ�����ж��еȴ�)���ʹ���Ϣ�����������ߣ�
#if(WINVER >= 0x0500)
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_MENURBUTTONUP);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_MENUDRAG);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_MENUGETOBJECT);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_UNINITMENUPOPUP);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_MENUCOMMAND);
#  if(_WIN32_WINNT >= 0x0500)
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_CHANGEUISTATE);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_UPDATEUISTATE);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_QUERYUISTATE);
#  endif //_WIN32_WINNT >= 0x0500

#endif //WINVER >= 0x0500

                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_CTLCOLORMSGBOX);    //��windows������Ϣ��ǰ���ʹ���Ϣ����Ϣ��������ߴ��ڣ�ͨ����Ӧ������Ϣ�������ߴ��ڿ���
                //  ͨ��ʹ�ø����������ʾ�豸�ľ����������Ϣ����ı��ͱ�����ɫ
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_CTLCOLOREDIT);      //��һ���༭�Ϳؼ���Ҫ������ʱ���ʹ���Ϣ�����ĸ����� -- �������ñ༭����ı��ͱ�����ɫ
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_CTLCOLORLISTBOX);   //��һ���б��ؼ���Ҫ������ǰ���ʹ���Ϣ�����ĸ����� -- ���������б����ı��ͱ�����ɫ
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_CTLCOLORBTN);       //��һ����ť  �ؼ���Ҫ������ʱ���ʹ���Ϣ�����ĸ����� -- �������ð�Ŧ���ı��ͱ�����ɫ
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_CTLCOLORDLG);       //��һ���Ի���ؼ���Ҫ������ǰ���ʹ���Ϣ�����ĸ����� -- �������öԻ�����ı�������ɫ
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_CTLCOLORSCROLLBAR); //��һ���������ؼ���Ҫ������ʱ���ʹ���Ϣ�����ĸ����� -- �������ù������ı�����ɫ
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_CTLCOLORSTATIC);    //��һ����̬�ؼ���Ҫ������ʱ���ʹ���Ϣ�����ĸ����� -- �������þ�̬�ؼ����ı��ͱ�����ɫ
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),MN_GETHMENU);


                //HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_MOUSEFIRST);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_MOUSEMOVE);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_LBUTTONDOWN);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_LBUTTONUP);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_LBUTTONDBLCLK);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_RBUTTONDOWN);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_RBUTTONUP);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_RBUTTONDBLCLK);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_MBUTTONDOWN);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_MBUTTONUP);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_MBUTTONDBLCLK);

#if (_WIN32_WINNT >= 0x0400) || (_WIN32_WINDOWS > 0x0400)
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_MOUSEWHEEL);        //���������ת��ʱ���ʹ���Ϣ����ǰ�н���Ŀؼ�
#endif

#if (_WIN32_WINNT >= 0x0500)                
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_XBUTTONDOWN);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_XBUTTONUP);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_XBUTTONDBLCLK);
#endif

                //HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_MOUSELAST);

                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_PARENTNOTIFY); //��MDI�Ӵ��ڱ����������٣����û�����һ��������������Ӵ�����ʱ���ʹ���Ϣ�����ĸ�����
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_ENTERMENULOOP); //���ʹ���Ϣ֪ͨӦ�ó���������� �Ѿ������˲˵�ѭ��ģʽ -- ?
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_EXITMENULOOP);  //���ʹ���Ϣ֪ͨӦ�ó���������� �Ѿ��˳��˲˵�ѭ��ģʽ -- ?

#if(WINVER >= 0x0400)
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_NEXTMENU);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_SIZING);        //���û����ڵ������ڴ�Сʱ���ʹ���Ϣ�����ڣ�ͨ������ϢӦ�ó�����Լ��Ӻ��޸Ĵ��ڴ�С��λ��
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_CAPTURECHANGED);//����ʧȥ��������ʱ
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_MOVING);        //���û����ƶ�����ʱ���ʹ���Ϣ
#endif

#if(WINVER >= 0x0400)
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_POWERBROADCAST);//�͸�Ӧ�ó�����֪ͨ���йص�Դ�����¼�
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_DEVICECHANGE);  //���豸��Ӳ�����øı�ʱ���ʹ���Ϣ��Ӧ�ó�����豸��������
#endif

                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_MDICREATE);     //Ҫ�󴴽�һ��MDI �Ӵ���
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_MDIDESTROY);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_MDIACTIVATE);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_MDIRESTORE);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_MDINEXT);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_MDIMAXIMIZE);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_MDITILE);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_MDICASCADE);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_MDIICONARRANGE);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_MDIGETACTIVE);


                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_MDISETMENU);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_ENTERSIZEMOVE);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_EXITSIZEMOVE);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_DROPFILES);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_MDIREFRESHMENU);

#if(WINVER >= 0x0400)
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_IME_SETCONTEXT);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_IME_NOTIFY);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_IME_CONTROL);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_IME_COMPOSITIONFULL);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_IME_SELECT);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_IME_CHAR);
#endif

#if(WINVER >= 0x0500)
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_IME_REQUEST);
#endif

#if(WINVER >= 0x0400)
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_IME_KEYDOWN);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_IME_KEYUP);
#endif

#if((_WIN32_WINNT >= 0x0400) || (WINVER >= 0x0500))
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_MOUSEHOVER); //
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_MOUSELEAVE); //����ƿ�ʱ����Ҫ�� ON_MESSAGE ��Ӧ
#endif

#if(WINVER >= 0x0500)
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_NCMOUSEHOVER);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_NCMOUSELEAVE);
#endif


#if(_WIN32_WINNT >= 0x0501)
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_WTSSESSION_CHANGE);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_TABLET_FIRST);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_TABLET_LAST);
#endif

                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_CUT);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_COPY);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_PASTE);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_CLEAR);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_UNDO);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_RENDERFORMAT);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_RENDERALLFORMATS);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_DESTROYCLIPBOARD);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_DRAWCLIPBOARD); //������������ݱ仯ʱ���ʹ���Ϣ��������۲����ĵ�һ�����ڣ��������ü�����۲촰������ʾ�������������
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_PAINTCLIPBOARD);//�����������CF_OWNERDIPLAY��ʽ�����ݲ��Ҽ�����۲촰�ڵĿͻ�����Ҫ�ػ�
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_VSCROLLCLIPBOARD);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_SIZECLIPBOARD);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_ASKCBFORMATNAME);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_CHANGECBCHAIN); //��һ�����ڴӼ�����۲�������ȥʱ���ʹ���Ϣ��������۲����ĵ�һ������
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_HSCROLLCLIPBOARD);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_QUERYNEWPALETTE);   //����Ϣ��ʹ�������յ�����ʱͬʱ�л���ʵ�������߼���ɫ��
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_PALETTEISCHANGING);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_PALETTECHANGED);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_HOTKEY);        //���û�������REGISTERHOTKEY����ע����ȼ�ʱ�ύ����Ϣ

#if(WINVER >= 0x0400)
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_PRINT);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_PRINTCLIENT);
#endif

#if(_WIN32_WINNT >= 0x0500)
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_APPCOMMAND);
#endif

#if(_WIN32_WINNT >= 0x0501)
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_THEMECHANGED);
#endif

#if(WINVER >= 0x0400)
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_HANDHELDFIRST);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_HANDHELDLAST);
#ifdef __AFXPRIV_H__
                //#  ifdef USING_MFC
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_QUERYAFXWNDPROC);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_SIZEPARENT);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_SETMESSAGESTRING);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_IDLEUPDATECMDUI);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_INITIALUPDATE);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_COMMANDHELP);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_HELPHITTEST);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_EXITHELPMODE);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_RECALCPARENT);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_SIZECHILD);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_KICKIDLE);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_QUERYCENTERWND);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_DISABLEMODAL);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_FLOATSTATUS);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_ACTIVATETOPLEVEL);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_RESERVED_036F);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_RESERVED_0370);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_RESERVED_0371);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_RESERVED_0372);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_SOCKET_NOTIFY);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_SOCKET_DEAD);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_POPMESSAGESTRING);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_HELPPROMPTADDR);
                //HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_OCC_LOADFROMSTREAM);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_OCC_LOADFROMSTORAGE);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_OCC_INITNEW);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_OCC_LOADFROMSTREAM_EX);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_OCC_LOADFROMSTORAGE_EX);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_QUEUE_SENTINEL);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_RESERVED_037C);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_RESERVED_037D);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_RESERVED_037E);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_FORWARDMSG);
#  else
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_AFXFIRST);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_AFXLAST);

#  endif //end __AFXPRIV_H__

#endif

                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_PENWINFIRST);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_PENWINLAST);
#  ifdef _DDEHEADER_INCLUDED_
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_DDE_INITIATE);  //һ��DDE�ͻ������ύ����Ϣ��ʼһ�������������ĻỰ����Ӧ�Ǹ�ָ���ĳ����������
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_DDE_TERMINATE);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_DDE_ADVISE);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_DDE_UNADVISE);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_DDE_ACK);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_DDE_DATA);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_DDE_REQUEST);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_DDE_POKE);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_DDE_EXECUTE);
                //HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_DDE_LAST);
#  endif
#if(WINVER >= 0x0400)
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_APP);
#endif
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),DM_GETDEFID);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),DM_SETDEFID);
#if(WINVER >= 0x0400)
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),DM_REPOSITION);
#endif
            default:
                StringCchPrintf(m_bufInfo,_countof(m_bufInfo),
                    TEXT("Unknown Message %d(0x%08x)"),m_Info,m_Info);
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
                    MONITOR_DEFAULTTONEAREST);

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

    BOOL CFWinUtil::ActiveAndForegroundWindow(HWND hWnd)
    {
        //IPMSG ��Ҳ��һ�� SetForceForegroundWindow ������������ͬ

        BOOL bRet = TRUE;
        HWND   hForegdWnd   =   ::GetForegroundWindow();
        if (hForegdWnd == hWnd)
        {
            return TRUE;
        }

        DWORD   dwCurID   =   ::GetCurrentThreadId();   
        DWORD   dwForeID   =   ::GetWindowThreadProcessId(hForegdWnd,NULL);
        if (dwCurID != dwForeID)
        {
            API_VERIFY(::AttachThreadInput(dwCurID,dwForeID,TRUE));

            //������ǰ��ֵ��Ȼ������Ϊ0
            DWORD sp_time = 0;
            SystemParametersInfo( SPI_GETFOREGROUNDLOCKTIMEOUT,0,&sp_time,0);
            SystemParametersInfo( SPI_SETFOREGROUNDLOCKTIMEOUT,0,(LPVOID)0,0);

            API_VERIFY(::SetForegroundWindow(hWnd));

            SystemParametersInfo( SPI_SETFOREGROUNDLOCKTIMEOUT,0,&sp_time,0);
            API_VERIFY(::AttachThreadInput(dwCurID,dwForeID,FALSE));
            //ShowWindow(hWnd,...)?
        }
        else
        {
#pragma TODO(how to active a minimize window without change it min/max status)
            API_VERIFY(::SetForegroundWindow(hWnd));
            //ShowWindow(hWnd,...)?
        }
        SwitchToThisWindow(hWnd, TRUE);

        //if (IsWindowVisible(hWnd))
        //{
        //}
        //else
        //{
        //}

        //if (IsIconic(hWnd))
        //{
        //    API_VERIFY(ShowWindow(hWnd,SW_RESTORE));
        //}
        //else
        //{
        //    API_VERIFY(ShowWindow(hWnd,SW_SHOW));
        //}

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

    LPCTSTR CFWinUtil::GetNotifyCodeString(UINT nCode)
    {
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
            HANDLE_CASE_RETURN_STRING(NM_TOOLTIPSCREATED);
#if (_WIN32_IE >= 0x0500)
            HANDLE_CASE_RETURN_STRING(NM_LDOWN);
            HANDLE_CASE_RETURN_STRING(NM_RDOWN);
            HANDLE_CASE_RETURN_STRING(NM_THEMECHANGED);
#endif

#if (_WIN32_IE >= 0x0600)
            HANDLE_CASE_RETURN_STRING(NM_FONTCHANGED);
            HANDLE_CASE_RETURN_STRING(NM_CUSTOMTEXT);
            HANDLE_CASE_RETURN_STRING(NM_TVSTATEIMAGECHANGING);
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
            HANDLE_CASE_RETURN_STRING(LVN_INCREMENTALSEARCHA);
            HANDLE_CASE_RETURN_STRING(LVN_INCREMENTALSEARCHW);
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
            HANDLE_CASE_RETURN_STRING(MCN_VIEWCHANGE);

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

    LPCTSTR CFWinUtil::GetWindowStyleString(FTL::CFStringFormater& formater, HWND hWnd,LPCTSTR pszDivide/* = TEXT("|") */)
    {
        BOOL bRet = FALSE;

        FTLASSERT(::IsWindow(hWnd));
        LONG_PTR    lStyle = ::GetWindowLongPtr(hWnd, GWL_STYLE);
        LONG_PTR    lOldStyle = lStyle;
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, WS_OVERLAPPED, pszDivide);
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, WS_POPUP, pszDivide);
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, WS_CHILD, pszDivide);
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, WS_MINIMIZE, pszDivide);
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, WS_VISIBLE, pszDivide);
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, WS_DISABLED, pszDivide);
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, WS_CLIPSIBLINGS, pszDivide);
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, WS_CLIPCHILDREN, pszDivide);
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

#if _WIN32_WINNT >= 0x0600
            HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, BS_DEFCOMMANDLINK, pszDivide);
            HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, BS_COMMANDLINK, pszDivide);
            HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, BS_DEFSPLITBUTTON, pszDivide);
            HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, BS_SPLITBUTTON, pszDivide);
#endif //_WIN32_WINNT >= 0x0600
            HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, BS_OWNERDRAW, pszDivide);
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
            HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, BS_PUSHBUTTON, pszDivide);
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
            HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, BS_BITMAP, pszDivide);
            HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, BS_ICON, pszDivide);
            HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, BS_TEXT, pszDivide);
        }

        //ListView
        if (0 ==  lstrcmp(szClassName, WC_LISTVIEW))
        {
            HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, LVS_LIST, pszDivide);
            //HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, LVS_ICON, pszDivide);
            HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, LVS_REPORT, pszDivide);
            HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, LVS_SMALLICON, pszDivide);
            HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, LVS_SINGLESEL, pszDivide);
            HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, LVS_SHOWSELALWAYS, pszDivide);
            HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, LVS_SORTASCENDING, pszDivide);
            HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, LVS_SORTDESCENDING, pszDivide);
            HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, LVS_SHAREIMAGELISTS, pszDivide);
            HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, LVS_NOLABELWRAP, pszDivide);
            HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, LVS_AUTOARRANGE, pszDivide);
            HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, LVS_EDITLABELS, pszDivide);
            HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, LVS_OWNERDATA, pszDivide);
            HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, LVS_NOSCROLL, pszDivide);
            HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, LVS_TYPESTYLEMASK, pszDivide);
            //HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, LVS_ALIGNMASK, pszDivide);
            HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, LVS_ALIGNTOP, pszDivide);
            HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, LVS_ALIGNLEFT, pszDivide);
            HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, LVS_NOSORTHEADER, pszDivide);
            HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, LVS_NOCOLUMNHEADER, pszDivide);
            HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, LVS_OWNERDRAWFIXED, pszDivide);
        }



        FTLASSERT( 0 == lStyle);
        if (0 != lStyle)
        {
            FTLTRACEEX(FTL::tlWarning, TEXT("%s:Check Style String Not Complete, total=0x%08x, remain=0x%08x\n"),
                __FILE__LINE__, lOldStyle, lStyle);
        }
        return formater.GetString();
    }

    LPCTSTR CFWinUtil::GetWindowExStyleString(FTL::CFStringFormater& formater, HWND hWnd, LPCTSTR pszDivide /* = TEXT */)
    {
        FTLASSERT(::IsWindow(hWnd));
        LONG_PTR    lExStyle = ::GetWindowLongPtr(hWnd, GWL_EXSTYLE);
        LONG_PTR    lOldExStyle = lExStyle;

        HANDLE_COMBINATION_VALUE_TO_STRING(formater, lExStyle, WS_EX_DLGMODALFRAME, pszDivide);
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, lExStyle, WS_EX_NOPARENTNOTIFY, pszDivide);
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, lExStyle, WS_EX_TOPMOST, pszDivide);
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, lExStyle, WS_EX_ACCEPTFILES, pszDivide);
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, lExStyle, WS_EX_TRANSPARENT, pszDivide);
#if(WINVER >= 0x0400)
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, lExStyle, WS_EX_MDICHILD, pszDivide);
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, lExStyle, WS_EX_TOOLWINDOW, pszDivide);
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, lExStyle, WS_EX_WINDOWEDGE, pszDivide);
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, lExStyle, WS_EX_CLIENTEDGE, pszDivide);
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, lExStyle, WS_EX_CONTEXTHELP, pszDivide);
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, lExStyle, WS_EX_RIGHT, pszDivide);
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, lExStyle, WS_EX_LEFT, pszDivide);
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, lExStyle, WS_EX_RTLREADING, pszDivide);
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, lExStyle, WS_EX_LTRREADING, pszDivide);
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, lExStyle, WS_EX_LEFTSCROLLBAR, pszDivide);
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, lExStyle, WS_EX_RIGHTSCROLLBAR, pszDivide);
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, lExStyle, WS_EX_CONTROLPARENT, pszDivide);
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, lExStyle, WS_EX_STATICEDGE, pszDivide);
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, lExStyle, WS_EX_APPWINDOW, pszDivide);
#endif /* WINVER >= 0x0400 */

#if(_WIN32_WINNT >= 0x0500)
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