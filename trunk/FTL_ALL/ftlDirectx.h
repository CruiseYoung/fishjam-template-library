
#ifndef FTL_DIRECTX_H
#define FTL_DIRECTX_H

#pragma once

#ifndef FTL_BASE_H
#  error ftldirectx.h requires ftlbase.h to be included first
#endif

//΢��ϵͳ�ṩ��DirectShow Filter:  http://technet.microsoft.com/zh-cn/library/dd375464

#include <DShow.h>
#pragma comment( lib, "Quartz.lib" )

#include <d3d9.h>
#include <list>

//#ifdef _D3D9_H_
//��������� <d3d9.h> �ļ����Ͷ���ʹ�� D3D9 �ĺ�(ʹ��D3D�Ĵ������)
//#define USE_DIRECTX_9 
//#endif 

//#ifdef USE_DIRECTX_9
//#  include <dxerr.h>
//#  pragma comment( lib, "dxerr9.lib" )
//#endif //USE_DIRECTX_9

namespace FTL
{
//Class
    struct MediaSampleProperty;
    class CFDirectXErrorInfo;

//Function
//  DirectShowUtility(namespace)

}//namespace FTL

namespace FTL
{
    ////////////////////////////////////////////////////////////////////////////////////////
    //      Macro
    ////////////////////////////////////////////////////////////////////////////////////////

    //ARGB -- ���λ��Alpha��ʾ�Ƿ�͸��
    #define ARGB16BIT555(a,r,g,b)   ( ((a %2)<<15) + ((r%32)<<10) + ((g%32)<<5) + (b%32) )
    //XRGB -- ���λû��ʹ��
    #define XRGB16BIT555(r,g,b)     ( ((r%32)<<10) + ((g%32)<<5) + (b%32) )
    //RGB565 -- ��ɫ��6λ(���۶���ɫ������)
    #define RGB16BIT565(r,g,b)      ( ((r%32)<<11) + ((g%64)<<6) + (b%32) )

    //�ж�DirectInput���Ƿ���ָ���ļ棬�÷���if(DI_KEY_DOWN(keyState,DIK_ESCAPE))
    #define DI_KEY_DOWN(s,n) ( s[n] & 0x80 )
    #define DI_MOUSE_KEY_DOWN(m,n) ( m.rgbButtons[n] & 0x80 )

#  define DX_VERIFY(x)   \
    hr = (x);\
    if(S_OK != hr)\
    {\
        REPORT_ERROR_INFO(FTL::CFDirectXErrorInfo, hr,x);\
    }

#  define DX_VERIFY_EXCEPT1(x,hr1)   \
    hr = (x);\
    if(S_OK != hr && (hr1) != hr)\
    {\
        REPORT_ERROR_INFO(FTL::CFDirectXErrorInfo, hr,x);\
    }

#  define DX_VERIFY_EXCEPT2(x,hr1,hr2)   \
    hr = (x);\
    if(S_OK != hr && (hr1) != hr && (hr2) != hr)\
    {\
        REPORT_ERROR_INFO(FTL::CFDirectXErrorInfo, hr,x);\
    }


//#  define DX_VERIFY(x)   \
//    hr = (x);\
//    if(S_OK != hr)\
//    {\
//        DXUTTrace(__FILE__,(DWORD)__LINE__, hr,L#x);\
//    }


    /******************************************************************************************************************
    ******************************************************************************************************************/
    FTLEXPORT class CFDirectXErrorInfo : public CFConvertInfoT<CFDirectXErrorInfo,HRESULT>
    {
        DISABLE_COPY_AND_ASSIGNMENT(CFDirectXErrorInfo);
    public:
        FTLINLINE explicit CFDirectXErrorInfo(HRESULT error);
        FTLINLINE virtual LPCTSTR ConvertInfo();
    };

	//DirectShow �� IMediaEvent::GetEvent 
	FTLEXPORT class CFDirectShowEventInfo : public CFConvertInfoT<CFDirectShowEventInfo, LONG>
	{
	public:
		FTLINLINE explicit CFDirectShowEventInfo(LONG lEvent, LONG_PTR lParam1, LONG_PTR lParam2);
		FTLINLINE virtual LPCTSTR ConvertInfo();
	public:
		LONG_PTR m_lParam1;
		LONG_PTR m_lParam2;
	};

    struct MediaSampleProperty
    {
    public:
        FTLINLINE MediaSampleProperty();
    public:
        BOOL            IsSyncPoint;
        BOOL            IsPreroll;
        BOOL            IsDiscontinuity;
        LONG            Size;
        LONG            ActualDataLength;
        LONGLONG        MediaTimeStart;
        LONGLONG        MediaTimeEnd;
        REFERENCE_TIME  TimeStart;
        REFERENCE_TIME  TimeEnd;
        //AM_MEDIA_TYPE*  pMediaType;
    };

    //! DirectShow�Ĺ��ܺ�������������û�в���
    class CFDirectShowUtility
    {
	public:
		//��IGraphBuilder ע�ᵽ Running Object Table (ROT)�У�Ȼ���ͨ�� GraphEdt ���в鿴
        FTLINLINE static HRESULT AddGraphToRot(IUnknown* pUnkGraph,DWORD* pdwRegister);
        FTLINLINE static HRESULT RemoveGraphFromRot(DWORD dwRegister);

		//����MediaSample������
		FTLINLINE static HRESULT CopyMediaSample(IMediaSample *pSource, IMediaSample *pDest);

        //ע�⣺IVideoWindow::put_MessageDrain ָ�����������ղ�����Video���ڵ����ͼ�����Ϣ
        //����(��δ����)��ToggleFullScreen(m_pVW,hMain,&hVideo); �� ToggleFullScreen(m_pVW,hVideo,NULL); 
        FTLINLINE static HRESULT ToggleFullScreen(IVideoWindow *pVW,HWND hDrain, HWND *phOldDrain);

        //����һ��ָ��CLSID��Filter
        FTLINLINE static HRESULT AddFilterByCLSID(IGraphBuilder* pGraph, REFGUID clsid,
            LPCWSTR pName, IBaseFilter** ppFilter);

        //�õ�Filter��δ���ӵĵ�һ������majorType��Pin(��� majorTyte Ϊ MEDIATYPE_NULL ���ж�majorType)
        FTLINLINE static HRESULT GetUnconnectedPin(IBaseFilter* pFilter,PIN_DIRECTION dir, REFGUID majorType, IPin ** ppPin);

        //!���Filter��ָ������ĵ� dirIndex ��Pin -- �����Ƿ�����
        FTLINLINE static HRESULT GetPin(IBaseFilter* pFilter, PIN_DIRECTION dirRequest ,DWORD dirIndex, IPin** ppFoundPin);

        //!�����������Pin��Filter(��һ��δ�õ�����Pin) -- IGraphBuilder::Connect ��������������Pin
        FTLINLINE static HRESULT ConnectFilters(IGraphBuilder* pGraph, IPin *pOut, IBaseFilter* pDest);

        //! ������������Filter(��һ��Filter��δ�����Pin -> �ڶ���Filterδ�õ�����Pin)
        FTLINLINE static HRESULT ConnectFilters(IGraphBuilder* pGraph, IBaseFilter* pSrc, IBaseFilter* pDest);

        //! ��Filter Graph �е�Filter�в����ض���ĳ�ֽӿ�(��DV��Ƶ����� IIPDVDec ��)
        FTLINLINE static HRESULT FindFilterInterface(IFilterGraph* pGraph, REFIID riid, void** ppUnk);

        //!����ָ��Filter��Pin��ʵ�ֵ��ض��ӿ�(�� IID_IReferenceClock ��)
        FTLINLINE static HRESULT FindPinInterface(IBaseFilter* pFilter, REFIID riid, void** ppUnk);

        //! ��Graph�е� Filter �� Pin �ϲ���ָ���ӿ�
        FTLINLINE static HRESULT FindInterfaceAnywhere(IFilterGraph *pGraph, REFIID riid, void** ppUnk);

        //! ��ȡָ��Filter�����ŵ� ��/�� һ��Filter
        //! ע�⣺ֻ���ҵ�һ������Ҫ��(����)��
        FTLINLINE static HRESULT GetNextFilter(IBaseFilter* pFilter, PIN_DIRECTION dir, IBaseFilter** ppNext);

        //! �����Filter Graph�����е�Filter
        FTLINLINE static HRESULT ClearAllFilters(IFilterGraph *pGraph);

        //��ָ��Filter��ʼ���Ͽ�ָ�����������Filter(���ҿ��Դ�Graph���Ƴ�) -- ��ݹ�
        FTLINLINE static HRESULT DisconnectDirFilters(IFilterGraph* pGraph, IBaseFilter* pFilter, 
            PIN_DIRECTION dir, BOOL bRemoveFromGraph = FALSE);

        //! �ڽ��и�ʽת��ʱ���������ҪԤ�����������ò�ʹ�òο�ʱ�ӣ���ʹ�������е�Sample������ٶȴ��͡�
        //  ʹ�� IFilterGraph::SetDefaultSyncSource �ָ�ȱʡ��
        FTLINLINE static HRESULT SetNoClock(IFilterGraph *pGraph, IReferenceClock ** pOldClock = NULL);

        FTLINLINE static HRESULT RenderOutputPins(IGraphBuilder *pGB, IBaseFilter *pFilter);

        FTLINLINE static HRESULT GetMediaSampleProperty(IMediaSample* pSample,MediaSampleProperty* pProperty);

        //TODO:�ӿ����
        FTLINLINE static HRESULT GetMediaTime(IMediaSeeking *pMS);
        
		FTLINLINE static HRESULT GrapFrameFromMediaFile(LPCTSTR pszFilePath, HBITMAP& hBmp, LONGLONG nUnits = 0);
        //��Filter��ץͼ
		FTLINLINE static HRESULT SnapShotBitmap(IBasicVideo* pBasicVideo, HBITMAP& hBmpSnap);
        FTLINLINE static HRESULT SnapShotBitmap(IBasicVideo* pBasicVideo, LPCTSTR pszOutFile);

        //! ��ʼ����������Ƶ����� Capture Graph Builder
        FTLINLINE static HRESULT InitCaptureGraphBuilder(IGraphBuilder **ppGraph, 
            ICaptureGraphBuilder2 ** ppCapture);

	public:
		//��ȡ WAVEFORMATEX::wFormatTag ��Ӧ���ַ���Ϣ
        FTLINLINE static LPCTSTR GetWaveFormatExTagString(WORD wWaveFormatTag);

    };//DirectShowUtility

//#ifdef USE_DIRECTX_9
    class Direct3DUtility
    {
    public:
        //��װ��Ӧ�ó������Ϣѭ��������Ҫ����һ����ʾ�����ĺ���ָ�룬�ڿ��е�ʱ�������ʾ������BusyLoop��
        //����timeDeltaΪÿһ֡�ļ��ʱ�䣬��������ÿ���֡��
        FTLINLINE static DWORD EnterMsgLoop( bool (*ptr_display)(float timeDelta));

        //����ʱʹ�õĺ���
        FTLINLINE static DWORD DumpAdapterModes(IDirect3D9* pD3D);
    };

	struct HardwareMonikerInfo
	{
		//std::wstring	strFriendlyName;
		CLSID			clsid;
		WCHAR			wachFriendlyName[120];	//ʵ����DShowֻ���ҵ�32�ַ���(ע���)
		IMoniker*		pMoniker;
		HardwareMonikerInfo()
		{
			clsid = CLSID_NULL;
			wachFriendlyName[0] = 0;
			pMoniker = NULL;
		}
		~HardwareMonikerInfo()
		{
			SAFE_RELEASE(pMoniker);
		}
	};

	//typedef CFSharePtr<HardwareMonikerInfo>	HardwareMonikerInfoPtr;

	typedef std::list<HardwareMonikerInfo*> HardwareMonikerInfoContainer;
	typedef HardwareMonikerInfoContainer::iterator	HardwareMonikerInfoContainerIter;

	class CFDShowHardwareMgr
	{
	public:
		FTLINLINE CFDShowHardwareMgr();
		FTLINLINE ~CFDShowHardwareMgr();

		FTLINLINE HRESULT Refresh(const CLSID* pDevClsid);
		FTLINLINE HRESULT Clear();

        //һ����˵ riidResult ���� IID_IBaseFilter
		FTLINLINE HRESULT GetBindObject(LPCWSTR pszName, REFIID riidResult, void **ppvResult);

		//TODO: Enum
		HardwareMonikerInfoContainer& GetAllHardwares()
		{
			return m_Hardwares;
		}
	protected:
		FTLINLINE HRESULT _AddNewMoniker(IMoniker* pMoniker);
	private:
		HardwareMonikerInfoContainer	m_Hardwares;

		enum
		{
			//DirectShowö������ö�ٳ����� FriendlyName ����󳤶�(����������),���ճ��޷���ȡȫ�����Ƶ�����
			//IMMDeviceEnumerator �ӿ��ܶ�ȡ������������
			DS_FRIENDLY_NAME_MAX_LENGTH = 32,
		};
	};

//#endif //USE_DIRECTX_9
}
#endif //FTL_DIRECTX_H

#ifndef USE_EXPORT
#  include "ftlDirectx.hpp"
#endif