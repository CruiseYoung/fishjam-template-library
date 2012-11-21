#ifndef DIRECTSHOW_SAMPLES_H
#define DIRECTSHOW_SAMPLES_H

#error(���Բ�����FTL) -- SysEnum --
void CBaseDialog::RedrawControl(int nID)
{
    // Find the dialog rect and the control rect, both relative to the display
    RECT rcDlg, rcControl;
    GetWindowRect(m_hDlg, &rcDlg);
    GetWindowRect(GetDlgItem(nID), &rcControl);

    // Adjust the dialog rect by the size of the border and caption 
    rcDlg.top += NonClientTop();
    rcDlg.left += NonClientWidth();

    // Find the control rect relative to the dialog position
    OffsetRect(&rcControl, - rcDlg.left, - rcDlg.top);

    InvalidateRect(m_hDlg, &rcControl, TRUE);
    UpdateWindow(m_hDlg);
}

/************************************************************************************************
* ����DirectShow�е�Sample
* Capture
*  +-AMCap      -- ����Ƶ�ɼ�������(��Ҫʹ��ICaptureGraphBuilder2)��֧����ƵԤ����֧�ֲɼ�AVI�ļ���֧��MPEG2�������֧���Ȳ�Ρ�
*                  ͨ�������õ� CCrossbar ��������Crossbar -- �ݹ麯��BuildRoutingList
*  +-AudioCap   -- δ�� -- ��Ƶ�ɼ������ӣ�ö����Ƶ�ɼ��豸��֧�ָ��ֲ������ã�������Wav�ļ����ܱ�����¼��
*  +-CaptureSharedTimerDriven -- ʹ��Vista/Win7��WASAPI���ڹ���ģʽ��ʹ��Timer Driven���ģʽ�ķ�ʽ��׽��Ƶ����Ϊ�ļ���
* 
* Editing
*  +-CompressView -- δ�� -- ��ʾ��ν�ý���ļ�ת��ΪAVI��ʽ(ͨ��ϵͳö��ѹ���������)���ĸ���Ч��
* 
* DMO
*  +-dmodemo    -- ʹ�ø��� DirectSound Audio Effect DMOs Ч����ͨ�� DirectSound ����Wav�ļ�
* 
* Filters
*  +-Ball       -- ʹ��δѹ����ʽ����Ƶ����ʾ�˶�С��� Live Source Filter��ÿһ��Sample����һ��BMPͼƬ
*  +-Dump       -- �ѽ��յ���Sample�����Txt�ļ��� Render Filter(֧��IFileSinkFilter�ӿڣ�GE���Զ�����SaveFile�Ի���)
*  +-EZRGB24    -- ��image���й��˺���ɫת��Ч����Transform Filter(֧������ҳ������ɫ)��ֻ֧��RGB24
*  +-Gargle     -- �޸�ͨ��������Ƶ�ʵ� In-Place Transform Filter(����������ҳ����Ƶ��)
*  +-Grabber    -- 
*  +-InfTee     -- δ�� -- ��CBaseFilter�̳еģ���������ָ�Ϊ�ܶ������
*  +-PushSource -- ���� Video Source Filter(�ֱ���ͼ��ͼ�� �� ��Ļ��ͼ), û��ʵ�� ��ͣ��֡�� ���ƵĹ���
*  +-Synth		-- ��ƵSourceFilter(Audio Synthesizer)
*  +-WavDest    -- �ɼ�����ʱ����PCM����ȫ��д���ļ��������ļ�ͷλ�ò���һ��Wave��ʽ�����ݿ顣
* 
* Misc
*  +- ASFCopy   -- δ�� -- ʹ��ָ����ϵͳ���ý��ļ�ת��ΪASF��ʽ���ļ���������֤����ö��ASF���õȡ�
*                  ���ж�����ASF�Ĵ�����(ASFErr.h �� nserror.h)��
*                  ����̨��ʽ���磺ASFCopy.exe /v /p 22 C:\WINDOWS\Media\tada.wav c:\testasf.asf
*  +- SysEnum   -- ö��ϵͳ��ע��ĸ���Filter������GraphEdt��ʾ����Filterʱö�ٵĹ���
*                  ����ʹ�� CLSID_ActiveMovieCategories �õ���DShow��ص���������(��ѹ������ѹ��Filter��)
*                  չʾ��ʹ���Զ���� _SetWindowLongPtr ģ�庯������� SetWindowLongPtr �����������ķ��� 
*                  ע�⣺��RedrawControl������ʾ�˼���Control���Dialogλ�õķ���
* Players
*  +-AudioBox   -- ʹ��MFC����Ƶ���ų����в����б�ʵ�� IServiceProvider �ӿ� ��WMF�ļ�����֧��
*  +-BGMusic    -- ʹ��Win32����MP3�����ֲ��ų����� SwapSourceFilter �������ж�̬�ı� Source Filter ������
*                  (ֹͣGraph -> �Ƴ��ɵ�SourceFilter -> �����µ�SourceFilter -> ����OutputPin ����Render)
*  +-Cutscene   -- ������ģʽ�� Movie Player��ͨ�������д���ý���ļ�������ȫ������(������һ��0��С��"����"�����������������)
*  +-PlayWnd    -- ʹ��Win32����Ƶ/��Ƶý���ļ����ų���ֻ�ܲ���һ���ļ�
*  +-PlayWndASF -- δ�� -- ý�岥����,���Բ�����DRM������WMF���͵��ļ� -- �ο�DRM�Ĵ���ʽ
*************************************************************************************************/

#endif //DIRECTSHOW_SAMPLES_H