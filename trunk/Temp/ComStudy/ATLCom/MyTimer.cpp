// MyTimer.cpp : Implementation of CMyTimer

#include "stdafx.h"
#include "MyTimer.h"
#include <time.h>

// CMyTimer

//���ӵ�
static CMyTimer * g_pMyTimer;
void static CALLBACK TimerProc(HWND hwnd, UINT uMsg, UINT idEvent, DWORD dwTime )
{
    /********************* VT_DATE ��ʾʱ��ķ��� *********************
    DATE ��һ�� 8 �ֽڵĸ��������� double ����
    �����������ֱ�ʾ����1899/12/30 ��㿪ʼ�����ڵ�������
    ����С�����ֱ�ʾ���������������Ѿ��ȹ��˼���֮����
    *******************************************************************/
    time_t lt;
    ::time( &lt );	// ȡ���� 1970/1/1 0:0:0 ����������ǰ��������

    lt += 8*60*60;	// ����ʱ��ȸ���������8��Сʱ

    long day=(long)(lt/(60*60*24)) + 25569;	// 1899/12/30 �����ڵ�����
    // 25569 ��1899/12/30 �� 1970/1/1 ��������
    // ��Ϊ VARIANT(VT_DATE) ��ʱ���ǰ��� 1899/12/30 ��ʼ�����

    DATE dtSrc = day + (lt % 86400) / 86400.;	// 86400 ��һ���������

    VARIANT v;
    v.vt = VT_DATE;
    v.date = dtSrc;
    g_pMyTimer->Fire_Timer( v );
}

STDMETHODIMP CMyTimer::SetTimer(LONG uElapse)
{
    // uElapse �����ĵ�λ�Ǻ���(1/1000 ��)

    KillTimer();	// ����Ѿ�������ʱ�ӣ�����ɾ��
    m_idTimer = ::SetTimer( NULL, 0, uElapse, TimerProc );
    g_pMyTimer = this;

    return m_idTimer? S_OK : E_FAIL;	// ��0��ʾ�ɹ�
}

STDMETHODIMP CMyTimer::KillTimer(void)
{
    if( m_idTimer )
    {
        ::KillTimer( NULL, m_idTimer );
        m_idTimer = 0;
        g_pMyTimer = NULL;
    }

    return S_OK;
}
