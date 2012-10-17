#pragma once

#include "DirectShowDemo_h.h"
#include <ftlfile.h>

/*******************************************************************************************************
* ��дFilterʱ�����齫Ӧ�ÿ�ܺ�Ӧ���߼����룬�����һ���ṹ��������չ�����õ�Filter��Ŀ
*   ������ ==> Filter���(<-> Ӧ���߼��飬ͨ����һ��������Ӧ���߼����ƻ���ָ��) ==> �����
*                 
* 
*******************************************************************************************************/

//extern const IID IID_IFilterDebugInfo;
//extern const GUID CLSID_FilterDebugInfo;
extern const GUID CLSID_FilterDebugInfoProperty;

extern const AMOVIESETUP_FILTER sudDebugInfoFilter;

//#ifdef __cplusplus 
//extern "C" 
//{
//#endif
//    DECLARE_INTERFACE_(IFilterDebugInfo,IUnknown)
//    {
//        STDMETHOD(GetConnectedPin) (THIS_ 
//            BOOL bIsInput,
//            IPin** ppPin
//           ) PURE;
//    };
//
//#ifdef __cplusplus 
//}
//#endif

class CDebugInfoInputPin : public CTransInPlaceInputPin
{

};

class CDebugInfoOutputPin : public CTransInPlaceOutputPin
{

};

class CDebugInfoFilter : public CTransInPlaceFilter,
    public ISpecifyPropertyPages,
    public IFilterDebugInfo,
    //public IFileSinkFilter,
    public CPersistStream
{
public:
    static CUnknown * WINAPI CreateInstance(LPUNKNOWN lpunk, HRESULT *phr);
    STDMETHOD(NonDelegatingQueryInterface)(REFIID riid, void ** ppv);
    
    //�������صĻ��ി�麯��
    virtual HRESULT CheckInputType(const CMediaType* mtIn); //UpStream Filter�жϱ�Filter�Ƿ�֧�ָ�ý������
    virtual HRESULT Transform(IMediaSample *pSample);

    //��ѡ���صĻ����麯��
    //STDMETHODIMP Stop();
    //STDMETHODIMP Run(REFERENCE_TIME tStart);
    //STDMETHODIMP Pause();

    //�����������������ڳ�ʼ������ֹ��
    HRESULT StartStreaming();
    HRESULT StopStreaming();

    HRESULT CompleteConnect(PIN_DIRECTION dir,IPin *pReceivePin);

    //ISpecifyPropertyPages
    STDMETHOD(GetPages)(/* [out] */ CAUUID *pPages);

    //IFilterDebugInfo
    STDMETHOD(GetConnectedPin)(BOOL bIsInput, IPin** ppPin);
    STDMETHOD(GetFilterDebugParam)(/* [out][in] */FilterDebugParam* pFilterDebugParam);
    STDMETHOD(SetFilterDebugParam)(/* [in] */FilterDebugParam* pFilterDebugParam);

    // CPersistStream overrides
    HRESULT WriteToStream(IStream *pStream);
    HRESULT ReadFromStream(IStream *pStream);
    int SizeMax();
    STDMETHODIMP GetClassID(CLSID *pClsid);

   // //IFileSinkFilter
   // STDMETHOD(SetFileName)( 
   //     /* [in] */ LPCOLESTR pszFileName,
   //     /* [unique][in] */ const AM_MEDIA_TYPE *pmt);

   //STDMETHOD(GetCurFile)( 
   //     /* [out] */ LPOLESTR *ppszFileName,
   //     /* [out] */ AM_MEDIA_TYPE *pmt);

private:
    CCritSec            m_DebugInfoLock;
    int                 m_nInputPinTypeIndex;       //�û�ѡ�������Pin��ý����������
    int                 m_nOutputPinTypeIndex;      //�û�ѡ������Pin��ý����������
    BOOL                m_bDumpSample;              //�Ƿ����Dump
	DWORD				m_dwDumpSampleStartIndex;   //��ʼDumpʱ�����
    DWORD               m_dwDumpSampleLimitCount;   //Dumpʱ�����Ƹ���
    DWORD               m_dwCurrentDumpSampleIndex;
    TCHAR               m_bufDumpFilePath[MAX_PATH];//Dump�����ļ�·��

    CFStructuredStorageFile  m_StorageFile;
    //CFThread<>          m_DumpThread;
    //static DWORD __stdcall DumpThreadFun(void* pParam);

    //HWND m_hWnd;
    //HDC m_hDC;
    //long m_nWidth;
    //long m_nHeight;
    //BITMAPINFOHEADER m_BMI;
    //DWORD * m_pCheckers;

    //void _Clear( );

    CDebugInfoFilter( IUnknown * pUnk, HRESULT * phr );
    ~CDebugInfoFilter( );

    DECLARE_IUNKNOWN;

    HRESULT SetMediaType(PIN_DIRECTION direction,const CMediaType *pmt);
    //// override these to receive indication of when we change
    //// to Pause/Play (Active) or Stop (Inactive) state.
    //HRESULT Active();
    //HRESULT Inactive();
public:
};
