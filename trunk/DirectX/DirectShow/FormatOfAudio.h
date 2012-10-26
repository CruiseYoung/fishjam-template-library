#pragma once

//���뼼������ PCM(Pluse Coding Modulation) -- ���������ƣ���LPCM�Ĺ�ϵ��
//MP3һ����1152������Ϊһ֡������ģ�֪�������ʣ������������׼����1�����֡
//  ֡/�� = SampleRate/1152/chanel ? �� 44100/1152/2 ?

/*********************************************************************************************
* AAC(Advanced Audio Coding) -- �߼���Ƶ���룬AC3����������2.0��5.1��Ƶ������
* AC3(Audio Coding3) -- ��Ƶ����3,�� DOLBY_AC3 ��ͬ��ʣ������Զ�6�����������Ϊ448kbps�ĵ����������б���
* APE -- һ������ѹ����ʽ��ѹ����Զ����������ʽ(50-70%��ѹ����)�����ܹ����������������Ʒ�ʵ��������ͼ��ղ�.
* DTS
* LPCM -- �������������ƣ���һ�ַ�ѹ����Ƶ���ֻ�������������롣
*         ������Ϊ��������ֵx������Сֵx������ bps�� �� 44100 x 16 x 2(˫����) ==1411.2 Kbps,�ȼ���128Kbps��MP3.
* ??? -- MPEG-1 Audio Layer II(HDV�б�����Ƶʱ�ĸ�ʽ)
* MP3(MPEG-1 Audio Layer-3, ��MPEG1���������뷽��) -- ��������ѹ�������ʻ������䣬�������α༭�����ʻἱ���½���
*   ���ֻ�ܱ���2��������������ý��Ļ��������������������߲��š�
*   DS���� MPEG-1 Layer III���ƺ�����������(���ֻ�����22050Hz)������Vista/Win7����license����
*      http://support.microsoft.com/kb/937141/en-us
*   ��Ҫ(���ַ�ʽ�Ƿ����ƽ⣬���ú�����������, VFW_E_START_TIME_AFTER_END(0x80040228))��
*     "HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Drivers32" ������ 
*       "msacm.l3codec"="l3codecp.acm" �� "msacm.l3acm"="l3codecp.acm"(ע�⣺����?)
*     "HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows NT\CurrentVersion\drivers.desc] ������ 
*       "l3codecp.acm"="Fraunhofer IIS MPEG Layer-3 Codec (professional)"
*     ע�⣺��Ĭ��ֻ�� "l3codeca.acm"="Fraunhofer IIS MPEG Layer-3 Codec"��
*   ���õķ�ʽ��ʹ�� "LAME MP3 Encoder" (http://lame.sourceforge.net/),���а����˿�ֱ��ʹ�õ� DShow Filter.
* mp3PRO -- ����mp3���뼼���ĸ��������������ļ������������SBR��mp3PRO��ʵ��һ��mp3�ź�����SBR�ź����Ļ�����������롣
*   �ٷ�����64kbps��mp3PRO��������128kbps��mp3��
* MPC -- ����������Ҫ�����ڸ�Ƶ����,��Ŀǰ���ʺ������������͵�������롣
* MPEG1/MPEG2 -- MEDIASUBTYPE_MPEG1Audio/MEDIASUBTYPE_MPEG2Audio
* Ogg Vorbis -- ��ȫ��ѣ��ųƿ�������Խϵ͵�����������ʵ�ֱ�MP3���õ����ʣ�MP3ɱ�֣�������֧�ֶ�������
*   ������ý��Ļ�������.
* RA(RealAudio) -- �������(֧�ֱ߶��߷�),���ҿ��Ը������ڵĴ����������Լ������ʣ��ڱ�֤������ǰ���¾�����������ʡ�
*   ����֧�ֶ�����Ƶ���룬����ATRAC3��֧��ʹ������Э���������ļ�����ʵ�����ַ(ֻ�������������ṩ����)
* WAV -- WAV��һ���ļ���ʽ������RIFF�淶��Windowsƽ̨�£�������Ƶ������ܹ��ṩ���֧��
* WMA -- Windows Media Audio�������ļ���ʽ����Ҫ������磬֧��������(�߶��߷�)��΢�������кܺõ���Ӳ��֧�֡�
*   ֧�ַ����ƹ���(��ͨ��Windows Media Rights Manager���뱣�����������Ʋ���ʱ��Ͳ��Ŵ��������ڲ��ŵĻ�����)
*   ֧����������
*
*********************************************************************************************/

/*********************************************************************************************
* Audio ��ý�����ͣ�AM_MEDIA_TYPE::pbFormat��
* formattypeΪFORMAT_WaveFormatEx ʱ���� WAVEFORMATEX ��ʽ
*   PCM�ļ�ÿ�����������С: nSamplesPerSec * (wBitsPerSample / 8) * nChannels, �� 44100 * (16/8) * 2
*     ע�⣺ wBitsPerSample ����Ϊ 0
*   typedef struct tWAVEFORMATEX
*   {
*     WORD        wFormatTag;         // format type���� WAVE_FORMAT_PCM, WAVE_FORMAT_EXTENSIBLE
*     WORD        nChannels;          // ������number of channels (i.e. monaural-1, stereo-2...)
*     DWORD       nSamplesPerSec;     // ÿ��Ĳ����ʣ�ͨ��Ϊ 44100,22050,11025 �ȣ�
*       ���ڷ�Χ̫��ͨ����Ҫʹ�� log10(freq)/pow(10.0,pos) �������ж���ת����������ʾ�͵�����
*     DWORD       nAvgBytesPerSec;    // ÿ���ƽ���ֽ�����nSamplesPerSec * nBlockAlign
*     WORD        nBlockAlign;        // (nChannels * wBitsPerSample) / 8����ʾÿһ���������ֽڴ�С������ֵ�� 1,2,4
*     WORD        wBitsPerSample;     // ÿ��������λ������8��16��, �����Գ����� 0 ?
*     WORD        cbSize;             // the count in bytes of the size of extra information (after cbSize)
*   }WAVEFORMATEX, *PWAVEFORMATEX, NEAR *NPWAVEFORMATEX, FAR *LPWAVEFORMATEX;
* 
* ý������(IMediaSample::GetPointer)
*   
*********************************************************************************************/

