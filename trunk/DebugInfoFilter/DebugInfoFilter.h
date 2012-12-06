#pragma once

#include "DebugInfoFilter_h.h"
#include <ftlfile.h>

#define FILTER_NAME		L"Debug Info Filter"

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
	public CPersistStream,
	public IDebugInfoFilter
{
public:

	DECLARE_IUNKNOWN;
	static CUnknown * WINAPI CreateInstance(LPUNKNOWN punk, HRESULT *phr);

	STDMETHODIMP NonDelegatingQueryInterface(REFIID riid, void ** ppv);

	//���ظ÷�����֪��Filter������ Graph
	virtual HRESULT STDMETHODCALLTYPE JoinFilterGraph(__in_opt  IFilterGraph *pGraph, __in_opt  LPCWSTR pName);

	//�������صĻ��ി�麯��
    virtual HRESULT CheckInputType(const CMediaType* mtIn); //UpStream Filter�жϱ�Filter�Ƿ�֧�ָ�ý������
    virtual HRESULT Transform(IMediaSample *pSample);

	//virtual HRESULT Transform(IMediaSample *pIn, IMediaSample *pOut);
	//virtual HRESULT CheckInputType(const CMediaType *mtIn);
	//virtual HRESULT CheckTransform(const CMediaType *mtIn, const CMediaType *mtOut);
	//virtual HRESULT DecideBufferSize(IMemAllocator *pAlloc,
	//	ALLOCATOR_PROPERTIES *pProperties);
    //�����������������ڳ�ʼ������ֹ��
    virtual HRESULT StartStreaming();
    virtual HRESULT StopStreaming();
    virtual HRESULT CompleteConnect(PIN_DIRECTION dir,IPin *pReceivePin);

	// ISpecifyPropertyPages interface
    STDMETHOD(GetPages)(/* [out] */ CAUUID *pPages);

    //IDebugInfoFilter
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

	CDebugInfoFilter(LPUNKNOWN pUnk, HRESULT *phr);
    ~CDebugInfoFilter( );
	HRESULT SetMediaType(PIN_DIRECTION direction,const CMediaType *pmt);
    //// override these to receive indication of when we change
    //// to Pause/Play (Active) or Stop (Inactive) state.
    //HRESULT Active();
    //HRESULT Inactive();
public:
};