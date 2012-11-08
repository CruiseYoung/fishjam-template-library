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
extern const GUID CLSID_GraphInfoProperty;

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

//class CDebugInfoInputPin : public CTransInPlaceInputPin
//{
//public:
//	CDebugInfoInputPin(CTransInPlaceFilter *pFilter, HRESULT *phr) 
//		: CTransInPlaceInputPin(TEXT("Input"), pFilter, phr, TEXT("Input"))
//	{
//
//	}
//
//};

//class CDebugInfoOutputPin : public CTransInPlaceOutputPin
//{
//
//};

struct DebugInfoParam
{
	DebugInfoParam()
	{
		m_nInputPinTypeIndex = -1;
		m_nOutputPinTypeIndex = -1;
		m_bDumpSample = FALSE;
		m_dwDumpSampleStartIndex = 0;
		m_dwDumpSampleLimitCount = 10;
		ZeroMemory(&m_bufDumpFilePath,sizeof(TCHAR) * _countof(m_bufDumpFilePath));
	}
	int                 m_nInputPinTypeIndex;       //�û�ѡ�������Pin��ý����������
	int                 m_nOutputPinTypeIndex;      //�û�ѡ������Pin��ý����������
	BOOL                m_bDumpSample;              //�Ƿ����Dump
	DWORD				m_dwDumpSampleStartIndex;   //��ʼDumpʱ�����
	DWORD               m_dwDumpSampleLimitCount;   //Dumpʱ�����Ƹ���
	TCHAR               m_bufDumpFilePath[MAX_PATH];//Dump�����ļ�·��
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
    
	//���ظ÷�����֪��Filter������ Graph
	virtual HRESULT STDMETHODCALLTYPE JoinFilterGraph(__in_opt  IFilterGraph *pGraph, __in_opt  LPCWSTR pName);

    //�������صĻ��ി�麯��
    virtual HRESULT CheckInputType(const CMediaType* mtIn); //UpStream Filter�жϱ�Filter�Ƿ�֧�ָ�ý������
    virtual HRESULT Transform(IMediaSample *pSample);

    //��ѡ���صĻ����麯��
    //STDMETHODIMP Stop();
    //STDMETHODIMP Run(REFERENCE_TIME tStart);
    //STDMETHODIMP Pause();

    //�����������������ڳ�ʼ������ֹ��
    virtual HRESULT StartStreaming();
    virtual HRESULT StopStreaming();

    virtual HRESULT CompleteConnect(PIN_DIRECTION dir,IPin *pReceivePin);

    //ISpecifyPropertyPages
    STDMETHOD(GetPages)(/* [out] */ CAUUID *pPages);

    //IFilterDebugInfo
    STDMETHOD(GetConnectedPin)(BOOL bIsInput, IPin** ppPin);
	STDMETHOD(SetAcceptMediaType)(AM_MEDIA_TYPE* pMediaType);
    STDMETHOD(GetFilterDebugParam)(/* [out][in] */FilterDebugParam* pFilterDebugParam);
    STDMETHOD(SetFilterDebugParam)(/* [in] */FilterDebugParam* pFilterDebugParam);

	STDMETHOD(GetFilterRunningInfo)(/*[in, out]*/ FilterRunningInfo* pFilterRunningInfo);
	STDMETHOD(SetFilterRunningInfo)(/*[in]*/ FilterRunningInfo* pFilterRunningInfo);

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
	static				INT		s_InstanceCount;
	static				BOOL	s_HasAddtoRot;
    CCritSec            m_DebugInfoLock;
	DWORD				m_dwRegister;
	REFERENCE_TIME		m_llLastTimeStart;			//��һ��MediaSample�Ŀ�ʼʱ��
	REFERENCE_TIME		m_llLastTimeEnd;			//��һ��MediaSample�Ľ���ʱ��
	FTL::CFElapseCounter	m_ElapseCounter;
	DebugInfoParam			m_DebugInfoParam;
	DWORD               m_dwCurrentDumpSampleIndex;
	DWORD				m_dwSampleCount;
	CMediaType*			m_pAcceptMediaType;
	
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
