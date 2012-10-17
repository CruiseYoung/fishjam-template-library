#include "Stdafx.h"
#include "MainFrame.h"
#pragma comment(lib,"dsound.lib")


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
    ON_WM_CREATE()
    ON_WM_DESTROY()
END_MESSAGE_MAP()


CMainFrame::CMainFrame(void)
{
    m_pdSound = NULL;
	m_pdSoundPrimaryBuffer = NULL;
    m_pdSoundBuffer = NULL;
    m_pdSoundNotify = NULL;
    m_pdSoundCapture = NULL;
} 

CMainFrame::~CMainFrame(void)
{
    SAFE_RELEASE(m_pdSoundCapture);
    SAFE_RELEASE(m_pdSoundNotify);
    SAFE_RELEASE(m_pdSoundBuffer);
	SAFE_RELEASE(m_pdSoundPrimaryBuffer);
    SAFE_RELEASE(m_pdSound);
}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    HRESULT hr = E_FAIL;

    if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
        return -1;

    //������DirectSound���� -- ���ж�����������Ҫʹ��ö�ٵķ�ʽ���ҵ����ʵ�����
    DX_VERIFY(DirectSoundCreate(
		NULL,//ʹ��NULL����ʹ��Ĭ�ϵ�����(�ȼ��� &DSDEVID_DefaultPlayback ?)
        &m_pdSound, NULL));
    
    //�趨Э���ȼ�
    //  Normal -- ��Ӧ���н���ʱ�����Բ���������������������Ӧ�ã�DSound�ᴴ��һ��������22kHz����������8λ��Ĭ��������
    //  Priority -- ӵ������Ӳ������ڣ����Ըı�����Ƶ�����趨����Ҫ��Ӳ����ɸ߼��ڴ����(��ѹ��),����Ҫ
    //    �ı�����������ݸ�ʽ(��16λ)ʱʹ��
    //  Exclusive -- ͬPriority,��ֻ�е�Ӧ����ǰ̨ʱ�������ü�
    //  Write_Primary -- ��ȫ����ʱ��ʹ��(���Լ�д������Ƶ������ʱ)
    DX_VERIFY(m_pdSound->SetCooperativeLevel(m_hWnd, DSSCL_NORMAL));

    //�������壬׼�����������ڲ���

    //�����ĸ�ʽ
    WAVEFORMATEX waveFormate = {0};
    waveFormate.wFormatTag = WAVE_FORMAT_PCM;
    waveFormate.nChannels = 1;  //MONO
    waveFormate.nSamplesPerSec = 11025; //11kHz
	waveFormate.wBitsPerSample = 8;
    waveFormate.nBlockAlign = (waveFormate.wBitsPerSample / 8) * waveFormate.nChannels; // 1
    waveFormate.nAvgBytesPerSec = waveFormate.nSamplesPerSec * waveFormate.nBlockAlign; // 11025
    waveFormate.cbSize = 0;

	//������
	DSBUFFERDESC  dsPrimaryBufferDesc = {0};
	dsPrimaryBufferDesc.dwSize = sizeof(dsPrimaryBufferDesc);
	dsPrimaryBufferDesc.dwFlags = DSBCAPS_PRIMARYBUFFER | DSBCAPS_CTRLVOLUME; //| DSBCAPS_CTRL3D
	dsPrimaryBufferDesc.dwBufferBytes = 0;  //����������ʱ����ֵ������ 0
	dsPrimaryBufferDesc.dwReserved = 0;
	dsPrimaryBufferDesc.lpwfxFormat = &waveFormate;
	DX_VERIFY(m_pdSound->CreateSoundBuffer(&dsPrimaryBufferDesc, &m_pdSoundPrimaryBuffer, NULL));

	//��Ӧ�ó��������뽹��ʱ��DS����øú�������������ĸ�ʽ( �� DSSCL_NORMAL ʱ)
	//DX_VERIFY(m_pdSoundPrimaryBuffer->SetFormat(&waveFormate));

	//��������
    DSBUFFERDESC  dsBufferDesc = {0};
    dsBufferDesc.dwSize = sizeof(dsBufferDesc);
    dsBufferDesc.dwFlags =  DSBCAPS_STATIC | DSBCAPS_LOCHARDWARE; 

	//�������Ĵ�С, 22050��ʾ ��11hKz, 8λ��������ʱ��������2������ݣ�
    dsBufferDesc.dwBufferBytes = waveFormate.nAvgBytesPerSec * 2;  // 2 seconds, �˴���ֵΪ 22050
	dsBufferDesc.dwReserved = 0;
    dsBufferDesc.lpwfxFormat = &waveFormate;

	//3DЧ��ʱ��������ģ���㷨(�� DS3DALG_HRTF_LIGHT )���������� DSBCAPS_CTRL3D ʱ����Ҫ���ø�ֵ
	dsBufferDesc.guid3DAlgorithm = DS3DALG_DEFAULT;
    DX_VERIFY(m_pdSound->CreateSoundBuffer(&dsBufferDesc, &m_pdSoundBuffer, NULL));

    //���ȡWAV���ļ���ʹ���������������������
	//�����������̫��Ļ������봴�������ŷ������������������е�һС�飬����һС�鲥������Ժ�
	//  �ټ��������С�����ݽ���������һֱ����������̣�ֱ��������������ϡ�
    //m_pdSoundBuffer->Lock();
    //write
    //m_pdSoundBuffer->Unlock();
    return 0;
}

void CMainFrame::OnDestroy()
{

    CFrameWnd::OnDestroy();
}
