#ifndef FTL_DIRECTX_HPP
#define FTL_DIRECTX_HPP
#pragma once

#ifdef USE_EXPORT
#  include "ftlDirectx.h"
#endif

#ifdef _DXERR_H_
#pragma comment( lib, "DxErr.lib" )
#else
#include <errors.h>
#pragma comment( lib, "Quartz.lib")
#endif 
#include <evcode.h>
#include <algorithm>
#include "ftlFunctional.h"

namespace FTL
{

    CFDirectXErrorInfo::CFDirectXErrorInfo(HRESULT error) 
        : CFConvertInfoT<CFDirectXErrorInfo,HRESULT>(error)
    {
    }

    LPCTSTR CFDirectXErrorInfo::ConvertInfo()
    {
#ifdef DXUT_H
        //���ʹ����DXUT���
        switch (m_Info)
        {
            HANDLE_CASE_RETURN_STRING(DXUTERR_NODIRECT3D);
            HANDLE_CASE_RETURN_STRING(DXUTERR_NOCOMPATIBLEDEVICES);
            HANDLE_CASE_RETURN_STRING(DXUTERR_MEDIANOTFOUND);
            HANDLE_CASE_RETURN_STRING(DXUTERR_NONZEROREFCOUNT);
            HANDLE_CASE_RETURN_STRING(DXUTERR_CREATINGDEVICE);
            HANDLE_CASE_RETURN_STRING(DXUTERR_RESETTINGDEVICE);
            HANDLE_CASE_RETURN_STRING(DXUTERR_CREATINGDEVICEOBJECTS);
            HANDLE_CASE_RETURN_STRING(DXUTERR_RESETTINGDEVICEOBJECTS);
            HANDLE_CASE_RETURN_STRING(DXUTERR_INCORRECTVERSION);
        }
#endif 

        //û��ִ�е����˵������DXUT�Ĵ���
        if (NULL == m_bufInfo[0])
        {


#ifdef _DXERR_H_
            const TCHAR* strErrorName = DXGetErrorString( m_Info );
            const TCHAR* strDescription = DXGetErrorDescription( m_Info );
            {
                StringCchPrintf(m_bufInfo, _countof(m_bufInfo),
                    TEXT("%s,Name(%s)"), 
                    strDescription,strErrorName);
            }
#else
            DWORD res = AMGetErrorText(m_Info, m_bufInfo, _countof(m_bufInfo));
            if (res == 0)
            {
                StringCchPrintf(m_bufInfo, _countof(m_bufInfo),
                    TEXT("Unknown Error"));
            }
#endif //no define _DXERR_H_
        }
        return m_bufInfo;
    }

#define GET_EVENT_CODE_INFO_ENTRY(lEvent, classEventCodeInfo ) \
	case (lEvent): { StringCchCopy(m_bufInfo, _countof(m_bufInfo), classEventCodeInfo().GetCodeInfo(lEvent, TEXT(#lEvent), m_lParam1, m_lParam2)); break; }

	class CFDefaultEventCodeInfo
	{
	public:
		virtual LPCTSTR GetCodeInfo(LONG /*lEvent*/, LPCTSTR pszEventCodeName, LONG_PTR /*lparam1*/, LONG_PTR /*lParam2*/)
		{
			return pszEventCodeName;
		}
	protected:
		CFStringFormater	m_strFormater;
	};


	CFDirectShowEventInfo::CFDirectShowEventInfo(LONG lEvent, LONG_PTR lParam1, LONG_PTR lParam2)
		: CFConvertInfoT<CFDirectShowEventInfo,LONG>(lEvent)
		, m_lParam1(lParam1)
		, m_lParam2(lParam2)
	{
	
	}

	LPCTSTR CFDirectShowEventInfo::ConvertInfo()
	{
		if (NULL == m_bufInfo[0])
		{
			switch(m_Info)
			{
				GET_EVENT_CODE_INFO_ENTRY(EC_COMPLETE, CFDefaultEventCodeInfo);
				GET_EVENT_CODE_INFO_ENTRY(EC_USERABORT, CFDefaultEventCodeInfo);
				GET_EVENT_CODE_INFO_ENTRY(EC_ERRORABORT, CFDefaultEventCodeInfo);
				GET_EVENT_CODE_INFO_ENTRY(EC_TIME, CFDefaultEventCodeInfo);
				GET_EVENT_CODE_INFO_ENTRY(EC_REPAINT, CFDefaultEventCodeInfo);
				GET_EVENT_CODE_INFO_ENTRY(EC_STREAM_ERROR_STOPPED, CFDefaultEventCodeInfo);
				GET_EVENT_CODE_INFO_ENTRY(EC_STREAM_ERROR_STILLPLAYING, CFDefaultEventCodeInfo);
				GET_EVENT_CODE_INFO_ENTRY(EC_ERROR_STILLPLAYING, CFDefaultEventCodeInfo);
				GET_EVENT_CODE_INFO_ENTRY(EC_PALETTE_CHANGED, CFDefaultEventCodeInfo);
				GET_EVENT_CODE_INFO_ENTRY(EC_VIDEO_SIZE_CHANGED, CFDefaultEventCodeInfo);
				GET_EVENT_CODE_INFO_ENTRY(EC_QUALITY_CHANGE, CFDefaultEventCodeInfo);
				GET_EVENT_CODE_INFO_ENTRY(EC_SHUTTING_DOWN, CFDefaultEventCodeInfo);
				GET_EVENT_CODE_INFO_ENTRY(EC_CLOCK_CHANGED, CFDefaultEventCodeInfo);
				GET_EVENT_CODE_INFO_ENTRY(EC_PAUSED, CFDefaultEventCodeInfo);
				GET_EVENT_CODE_INFO_ENTRY(EC_OPENING_FILE, CFDefaultEventCodeInfo);
				GET_EVENT_CODE_INFO_ENTRY(EC_BUFFERING_DATA, CFDefaultEventCodeInfo);
				GET_EVENT_CODE_INFO_ENTRY(EC_FULLSCREEN_LOST, CFDefaultEventCodeInfo);
				GET_EVENT_CODE_INFO_ENTRY(EC_ACTIVATE, CFDefaultEventCodeInfo);
				GET_EVENT_CODE_INFO_ENTRY(EC_NEED_RESTART, CFDefaultEventCodeInfo);
				GET_EVENT_CODE_INFO_ENTRY(EC_WINDOW_DESTROYED, CFDefaultEventCodeInfo);
				GET_EVENT_CODE_INFO_ENTRY(EC_DISPLAY_CHANGED, CFDefaultEventCodeInfo);
				GET_EVENT_CODE_INFO_ENTRY(EC_STARVATION, CFDefaultEventCodeInfo);
				GET_EVENT_CODE_INFO_ENTRY(EC_OLE_EVENT, CFDefaultEventCodeInfo);
				GET_EVENT_CODE_INFO_ENTRY(EC_NOTIFY_WINDOW, CFDefaultEventCodeInfo);
				GET_EVENT_CODE_INFO_ENTRY(EC_STREAM_CONTROL_STOPPED, CFDefaultEventCodeInfo);
				GET_EVENT_CODE_INFO_ENTRY(EC_STREAM_CONTROL_STARTED, CFDefaultEventCodeInfo);
				GET_EVENT_CODE_INFO_ENTRY(EC_END_OF_SEGMENT, CFDefaultEventCodeInfo);
				GET_EVENT_CODE_INFO_ENTRY(EC_SEGMENT_STARTED, CFDefaultEventCodeInfo);
				GET_EVENT_CODE_INFO_ENTRY(EC_LENGTH_CHANGED, CFDefaultEventCodeInfo);
				GET_EVENT_CODE_INFO_ENTRY(EC_DEVICE_LOST, CFDefaultEventCodeInfo);
				GET_EVENT_CODE_INFO_ENTRY(EC_SAMPLE_NEEDED, CFDefaultEventCodeInfo);
				GET_EVENT_CODE_INFO_ENTRY(EC_PROCESSING_LATENCY, CFDefaultEventCodeInfo);
				GET_EVENT_CODE_INFO_ENTRY(EC_SAMPLE_LATENCY, CFDefaultEventCodeInfo);
				GET_EVENT_CODE_INFO_ENTRY(EC_SCRUB_TIME, CFDefaultEventCodeInfo);
				GET_EVENT_CODE_INFO_ENTRY(EC_STEP_COMPLETE, CFDefaultEventCodeInfo);
				GET_EVENT_CODE_INFO_ENTRY(EC_TIMECODE_AVAILABLE, CFDefaultEventCodeInfo);
				GET_EVENT_CODE_INFO_ENTRY(EC_EXTDEVICE_MODE_CHANGE, CFDefaultEventCodeInfo);
				GET_EVENT_CODE_INFO_ENTRY(EC_STATE_CHANGE, CFDefaultEventCodeInfo);
				GET_EVENT_CODE_INFO_ENTRY(EC_GRAPH_CHANGED, CFDefaultEventCodeInfo);
				GET_EVENT_CODE_INFO_ENTRY(EC_CLOCK_UNSET, CFDefaultEventCodeInfo);
				GET_EVENT_CODE_INFO_ENTRY(EC_VMR_RENDERDEVICE_SET, CFDefaultEventCodeInfo);
				GET_EVENT_CODE_INFO_ENTRY(EC_VMR_SURFACE_FLIPPED, CFDefaultEventCodeInfo);
				GET_EVENT_CODE_INFO_ENTRY(EC_VMR_RECONNECTION_FAILED, CFDefaultEventCodeInfo);
				GET_EVENT_CODE_INFO_ENTRY(EC_PREPROCESS_COMPLETE, CFDefaultEventCodeInfo);
				GET_EVENT_CODE_INFO_ENTRY(EC_CODECAPI_EVENT, CFDefaultEventCodeInfo);

				//WindowsMedia SDK filter-specific events
				GET_EVENT_CODE_INFO_ENTRY(EC_WMT_INDEX_EVENT, CFDefaultEventCodeInfo);
				GET_EVENT_CODE_INFO_ENTRY(EC_WMT_EVENT, CFDefaultEventCodeInfo);
				GET_EVENT_CODE_INFO_ENTRY(EC_BUILT, CFDefaultEventCodeInfo);
				GET_EVENT_CODE_INFO_ENTRY(EC_UNBUILT, CFDefaultEventCodeInfo);

				//Other miscellaneous events used by various components
				GET_EVENT_CODE_INFO_ENTRY(EC_SKIP_FRAMES, CFDefaultEventCodeInfo);
				GET_EVENT_CODE_INFO_ENTRY(EC_PLEASE_REOPEN, CFDefaultEventCodeInfo);
				GET_EVENT_CODE_INFO_ENTRY(EC_STATUS, CFDefaultEventCodeInfo);
				GET_EVENT_CODE_INFO_ENTRY(EC_MARKER_HIT, CFDefaultEventCodeInfo);
				GET_EVENT_CODE_INFO_ENTRY(EC_LOADSTATUS, CFDefaultEventCodeInfo);
				GET_EVENT_CODE_INFO_ENTRY(EC_FILE_CLOSED, CFDefaultEventCodeInfo);
				GET_EVENT_CODE_INFO_ENTRY(EC_ERRORABORTEX, CFDefaultEventCodeInfo);
				GET_EVENT_CODE_INFO_ENTRY(EC_EOS_SOON, CFDefaultEventCodeInfo);
				GET_EVENT_CODE_INFO_ENTRY(EC_CONTENTPROPERTY_CHANGED, CFDefaultEventCodeInfo);
				GET_EVENT_CODE_INFO_ENTRY(EC_BANDWIDTHCHANGE, CFDefaultEventCodeInfo);
				GET_EVENT_CODE_INFO_ENTRY(EC_VIDEOFRAMEREADY, CFDefaultEventCodeInfo);
				
				//GET_EVENT_CODE_INFO_ENTRY(XXXX, CFDefaultEventCodeInfo);
			default:
				{
					if (m_Info >= EC_USER)
					{
						//private window classes
						LONG userMsg = m_Info - EC_USER;
						StringCchPrintf(m_bufInfo,_countof(m_bufInfo),
							TEXT("0x%08x = (EC_USER+%d[0x%x])"),m_Info, userMsg, userMsg );
					}
					else
					{
						StringCchPrintf(m_bufInfo,_countof(m_bufInfo),
							TEXT("Unknown Event Code %d(0x%08x)"),m_Info,m_Info);
					}
				}
				break;
			}
		}
		return m_bufInfo;
	}

    MediaSampleProperty::MediaSampleProperty()
    {
        IsSyncPoint = FALSE;
        IsPreroll = FALSE;
        IsDiscontinuity = FALSE;
        Size = 0;
        ActualDataLength = 0;
        MediaTimeStart = 0;
        MediaTimeEnd = 0;
        TimeStart = 0;
        TimeEnd = 0;
        //pMediaType = NULL;
    }

    //! DirectShow�Ĺ��ܺ�������������û�в���
    HRESULT DirectShowUtility::AddGraphToRot(IUnknown* pUnkGraph,DWORD* pdwRegister)
    {
        CHECK_POINTER_RETURN_VALUE_IF_FAIL(pUnkGraph,E_POINTER);
        CHECK_POINTER_RETURN_VALUE_IF_FAIL(pdwRegister,E_POINTER);

        HRESULT hr = E_FAIL;
        CComPtr<IMoniker> pMoniker = NULL;
        CComPtr<IRunningObjectTable> pROT = NULL;

        COM_VERIFY(GetRunningObjectTable(0, &pROT));
        if (SUCCEEDED(hr)) 
        {
            FTLASSERT(pROT);
            WCHAR wsz[256] = { 0 };
            COM_VERIFY(StringCchPrintfW(wsz,_countof(wsz), L"FilterGraph %08x pid %08x\0",//FilterGraph %08p pid %p",
                (DWORD_PTR)pUnkGraph, GetCurrentProcessId()));
            COM_VERIFY(CreateItemMoniker(L"!", wsz, &pMoniker));
            if (SUCCEEDED(hr)) 
            {
                //ROTFLAGS_REGISTRATIONKEEPSALIVE ������ֻ֤Ҫû�е���Remove��������ʹ����GE��ע��Ҳ��Ч
                COM_VERIFY(pROT->Register(ROTFLAGS_REGISTRATIONKEEPSALIVE, pUnkGraph, pMoniker, pdwRegister));
                //pMoniker->Release();
            }
            //pROT->Release();
        }
        return hr;
    }

    HRESULT DirectShowUtility::RemoveGraphFromRot(DWORD dwRegister)
    {
        HRESULT hr = E_FAIL;
        CComPtr<IRunningObjectTable> pROT = NULL;
        COM_VERIFY(GetRunningObjectTable(0, &pROT));
        if (SUCCEEDED(hr)) 
        {
            COM_VERIFY(pROT->Revoke(dwRegister));
            //pROT->Release();
        }
        return hr;
    }

    //ע�⣺IVideoWindow::put_MessageDrain ָ�����������ղ�����Video���ڵ����ͼ�����Ϣ
    //����(��δ����)��ToggleFullScreen(m_pVW,hMain,&hVideo); �� ToggleFullScreen(m_pVW,hVideo,NULL); 
    HRESULT DirectShowUtility::ToggleFullScreen(IVideoWindow *pVW,HWND hDrain, HWND *phOldDrain)
    {
        CHECK_POINTER_RETURN_VALUE_IF_FAIL(pVW,E_POINTER);

        HRESULT hr=S_OK;
        LONG lMode = OAFALSE;

        // Read current state
        DX_VERIFY(pVW->get_FullScreenMode(&lMode));

        if (lMode == OAFALSE)
        {
            // Save current message drain
            if (phOldDrain)
            {
                DX_VERIFY(pVW->get_MessageDrain((OAHWND *) phOldDrain));
            }
            // Set message drain to application main window
            DX_VERIFY(pVW->put_MessageDrain((OAHWND) hDrain));

            // Switch to full-screen mode
            lMode = OATRUE;
            DX_VERIFY(pVW->put_FullScreenMode(lMode));
        }
        else
        {
            // Switch back to windowed mode
            lMode = OAFALSE;
            DX_VERIFY(pVW->put_FullScreenMode(lMode));

            // Undo change of message drain
            DX_VERIFY(pVW->put_MessageDrain((OAHWND) hDrain));

            // Reset video window
            DX_VERIFY(pVW->SetWindowForeground(-1));

            // Reclaim keyboard focus for player application
            UpdateWindow(hDrain);
            SetForegroundWindow(hDrain);
            SetFocus(hDrain);
        }
        return hr;
    }

    //����һ��ָ��CLSID��Filter
    HRESULT DirectShowUtility::AddFilterByCLSID(IGraphBuilder* pGraph, REFGUID clsid,
        LPCWSTR pName, IBaseFilter** ppFilter)
    {
        CHECK_POINTER_RETURN_VALUE_IF_FAIL(pGraph,E_POINTER);
        CHECK_POINTER_RETURN_VALUE_IF_FAIL(ppFilter,E_POINTER);

        HRESULT hr = E_FAIL;
        *ppFilter = NULL;
        CComPtr<IBaseFilter> pTmpFilter = NULL;
        hr = CoCreateInstance(clsid,0,CLSCTX_INPROC_SERVER,IID_IBaseFilter,
            reinterpret_cast<void**>(&pTmpFilter));
        if (SUCCEEDED(hr))
        {
            DX_VERIFY(pGraph->AddFilter(pTmpFilter,pName));
            if (SUCCEEDED(hr))
            {
                *ppFilter = pTmpFilter.Detach();
            }
        }
        return hr;
    }

    //�õ�Filter��δ���ӵĵ�һ������majorType��Pin(��� majorTyte Ϊ MEDIATYPE_NULL ���ж�majorType)
    HRESULT DirectShowUtility::GetUnconnectedPin(IBaseFilter* pFilter,PIN_DIRECTION dir, REFGUID majorType, IPin ** ppPin)
    {
        CHECK_POINTER_RETURN_VALUE_IF_FAIL(pFilter,E_POINTER);
        CHECK_POINTER_RETURN_VALUE_IF_FAIL(ppPin,E_POINTER);

        HRESULT hr = E_FAIL;
        *ppPin = NULL;
        CComPtr<IEnumPins> pEnumPins = NULL; //CComPtr

        DX_VERIFY(pFilter->EnumPins(&pEnumPins));
        if (SUCCEEDED(hr))
        {
            CComPtr<IPin> pTmpPin = NULL;
            while ((hr = pEnumPins->Next(1,&pTmpPin,NULL)) == S_OK)
            {
                PIN_DIRECTION thisDir = PINDIR_INPUT;
                DX_VERIFY(pTmpPin->QueryDirection(&thisDir));
                if (thisDir == dir)
                {
                    CComPtr<IPin> pConnectedPin = NULL;
                    DX_VERIFY(pTmpPin->ConnectedTo(&pConnectedPin));//VFW_E_NOT_CONNECTED
                    if (SUCCEEDED(hr))  //�Ѿ��������ˣ�������Ҫ�ģ��ͷŵ�
                    {
                        //SAFE_RELEASE(pConnectedPin);
                        pConnectedPin.Release();
                    }
                    else
                    {
                        if(majorType == GUID_NULL)
                        {
                            //�ҵ��˵�һ�����㷽��ͬʱ��δ���ӵ�Pin����Ҫ���÷���ֵ������
                            *ppPin = pTmpPin.Detach();
                            //SAFE_RELEASE(pEnumPins);
                            return S_OK;
                        }
                        else
                        {
                            CComPtr<IEnumMediaTypes> pEnumTypes = NULL;
                            DX_VERIFY(pTmpPin->EnumMediaTypes(&pEnumTypes));
                            if (SUCCEEDED(hr))
                            {
                                AM_MEDIA_TYPE* pType = NULL;
                                while(*ppPin == NULL && pEnumTypes->Next(1, &pType, 0) == S_OK)
                                {
                                    if(pType->majortype == majorType)
                                    {
                                        *ppPin = pTmpPin.Detach();
#if 0
                                        DeleteMediaType(pType);
#else 
                                        FTLASSERT(FALSE);
#endif 
                                        return S_OK;
                                    }
#if 0
                                    DeleteMediaType(pType);
#else
                                    FTLASSERT(FALSE);
#endif 
                                }
                                //SAFE_RELEASE(pEnumTypes);
                            }
                        }//majorType != GUID_NULL
                    }
                }
                pTmpPin.Release();
            }//while ѭ��
            //SAFE_RELEASE(pEnumPins);
            if (S_FALSE == hr)  //ö�����˶�û���ҵ����ʵ�
            {
                hr = VFW_S_NO_MORE_ITEMS;
            }
        }
        return hr;
    }

    //!���Filter��ָ������ĵ� dirIndex ��Pin -- �����Ƿ�����
    HRESULT DirectShowUtility::GetPin(IBaseFilter* pFilter, PIN_DIRECTION dirRequest ,DWORD dirIndex, IPin** ppFoundPin)
    {
        CHECK_POINTER_RETURN_VALUE_IF_FAIL(pFilter,E_INVALIDARG);
        CHECK_POINTER_RETURN_VALUE_IF_FAIL(ppFoundPin,E_INVALIDARG);

        HRESULT hr = E_FAIL;
        IEnumPins* pEnum = NULL;
        *ppFoundPin = NULL;
        DWORD dirCount = 0;

        DX_VERIFY(pFilter->EnumPins(&pEnum));
        if (SUCCEEDED(hr)) 
        {
            IPin* pPin = NULL;
            while ( S_OK == (hr = pEnum->Next(1, &pPin, 0))) 
            {
                PIN_DIRECTION dir;
                pPin->QueryDirection(&dir);
                if (dir == dirRequest) 
                {
                    if (dirCount == dirIndex)
                    {
                        *ppFoundPin = pPin;
                        break;
                    }
                    dirCount++;                        
                }
                SAFE_RELEASE(pPin);
            }
            SAFE_RELEASE(pEnum);
        }
        if (*ppFoundPin)
        {
            hr = S_OK;
        }
        else if(S_FALSE == hr)  //Enum->Next no more ITEM
        {
            hr = VFW_S_NO_MORE_ITEMS;
        }
        return hr;
    }


    //!�����������Pin��Filter(��һ��δ�õ�����Pin) -- IGraphBuilder::Connect ��������������Pin
    HRESULT DirectShowUtility::ConnectFilters(IGraphBuilder* pGraph, IPin *pOut, IBaseFilter* pDest)
    {
        CHECK_POINTER_RETURN_VALUE_IF_FAIL(pGraph,E_POINTER);
        CHECK_POINTER_RETURN_VALUE_IF_FAIL(pOut,E_POINTER);
        CHECK_POINTER_RETURN_VALUE_IF_FAIL(pDest,E_POINTER);
        HRESULT hr = E_FAIL;
#ifdef FTL_DEBUG
        //���������Pin
        PIN_DIRECTION pinDir = PINDIR_INPUT;
        COM_VERIFY(pOut->QueryDirection(&pinDir));
        FTLASSERT(pinDir == PINDIR_OUTPUT);
#endif
        IPin* pIn = NULL;
        hr = GetUnconnectedPin(pDest,PINDIR_INPUT,MEDIATYPE_NULL,&pIn);
        if (FAILED(hr))
        {
            FTLASSERT(NULL == pIn);
            return hr;
        }
        //�����Pin���ӵ�����Pin
        hr = pGraph->Connect(pOut,pIn);
        SAFE_RELEASE(pIn);
        return hr;
    }

    //! ������������Filter(��һ��Filter��δ�����Pin -> �ڶ���Filterδ�õ�����Pin)
    HRESULT DirectShowUtility::ConnectFilters(IGraphBuilder* pGraph, IBaseFilter* pSrc, IBaseFilter* pDest)
    {
        CHECK_POINTER_RETURN_VALUE_IF_FAIL(pGraph,E_POINTER);
        CHECK_POINTER_RETURN_VALUE_IF_FAIL(pSrc,E_POINTER);
        CHECK_POINTER_RETURN_VALUE_IF_FAIL(pDest,E_POINTER);
        HRESULT hr = E_FAIL;

        //�ҵ�ԴFilter�����Pin
        IPin *pOut = NULL;
        hr = GetUnconnectedPin(pSrc,PINDIR_OUTPUT,MEDIATYPE_NULL,&pOut);
        if (SUCCEEDED(hr))
        {
            FTLASSERT(NULL != pOut);
            hr = ConnectFilters(pGraph,pOut,pDest);
            SAFE_RELEASE(pOut);
        }
        return hr;
    }

    //! ��Filter Graph �е�Filter�в����ض���ĳ�ֽӿ�(��DV��Ƶ����� IIPDVDec ��)
    HRESULT DirectShowUtility::FindFilterInterface(IFilterGraph* pGraph,REFIID riid, void** ppUnk)
    {
        CHECK_POINTER_RETURN_VALUE_IF_FAIL(pGraph,E_POINTER);
        CHECK_POINTER_RETURN_VALUE_IF_FAIL(ppUnk,E_POINTER);
        HRESULT hr = E_FAIL;
        IEnumFilters *pEnumFilters = NULL;
        DX_VERIFY(pGraph->EnumFilters(&pEnumFilters));
        if (SUCCEEDED(hr))
        {
            FTLASSERT(pEnumFilters);
            IBaseFilter* pFilter = NULL;
            while ((hr = pEnumFilters->Next(1,&pFilter,NULL)) == S_OK)
            {
                DX_VERIFY(pFilter->QueryInterface(riid,ppUnk));
                SAFE_RELEASE(pFilter);
                if (SUCCEEDED(hr))
                {
                    break;
                }
            }
            SAFE_RELEASE(pEnumFilters);
            if (S_FALSE == hr) //ѭ���궼û���ҵ�
            {
                hr = E_NOINTERFACE;
            }
        }
        return hr;
    }

    //!����ָ��Filter��Pin��ʵ�ֵ��ض��ӿ�
    HRESULT DirectShowUtility::FindPinInterface(IBaseFilter* pFilter, REFIID riid,void** ppUnk)
    {
        CHECK_POINTER_RETURN_VALUE_IF_FAIL(pFilter,E_POINTER);
        CHECK_POINTER_RETURN_VALUE_IF_FAIL(ppUnk,E_POINTER);
        HRESULT hr = E_FAIL;
        IEnumPins *pEnumPins = NULL;
        DX_VERIFY(pFilter->EnumPins(&pEnumPins));
        if (SUCCEEDED(hr))
        {
            FTLASSERT(pEnumPins);
            IPin *pPin = NULL;
            while ((hr = pEnumPins->Next(1,&pPin,NULL)) == S_OK)
            {
                DX_VERIFY_EXCEPT1(pPin->QueryInterface(riid,ppUnk),E_NOINTERFACE);
                SAFE_RELEASE(pPin);
                if (SUCCEEDED(hr))
                {
                    break;
                }
            }
            SAFE_RELEASE(pEnumPins);
            if (S_FALSE == hr)
            {
                hr = E_NOINTERFACE;
            }
        }
        return hr;
    }

    //! ��Graph�е� Filter �� Pin �ϲ���ָ���ӿ�
    HRESULT DirectShowUtility::FindInterfaceAnywhere(IFilterGraph *pGraph, REFIID riid, void** ppUnk)
    {
        CHECK_POINTER_RETURN_VALUE_IF_FAIL(pGraph,E_POINTER);
        CHECK_POINTER_RETURN_VALUE_IF_FAIL(ppUnk,E_POINTER);
        *ppUnk = NULL;
        HRESULT hr = E_FAIL;
        IEnumFilters *pEnumFilters = NULL;
        DX_VERIFY(pGraph->EnumFilters(&pEnumFilters));
        if (SUCCEEDED(hr))
        {
            FTLASSERT(pEnumFilters);
            IBaseFilter* pFilter = NULL;
            while ((hr = pEnumFilters->Next(1,&pFilter,NULL)) == S_OK)
            {
                DX_VERIFY_EXCEPT1(pFilter->QueryInterface(riid,ppUnk),E_NOINTERFACE);
                if (FAILED(hr)) //Filter ��û���ҵ��ӿ�,�����ϵ�Pin����
                {
                    hr = FindPinInterface(pFilter,riid,ppUnk);
                }
                SAFE_RELEASE(pFilter);
                if (SUCCEEDED(hr))
                {
                    FTLASSERT(NULL != ppUnk);
                    break;
                }
            }
            SAFE_RELEASE(pEnumFilters);
            if (S_FALSE == hr)
            {
                hr = E_NOINTERFACE;
            }
        }
        return hr;
    }

    //! ��ȡָ��Filter�����ŵ� ��/�� һ��Filter
    //! ע�⣺ֻ���ҵ�һ������Ҫ��(����)��
    HRESULT DirectShowUtility::GetNextFilter(IBaseFilter* pFilter, PIN_DIRECTION dir, IBaseFilter** ppNext)
    {
        CHECK_POINTER_RETURN_VALUE_IF_FAIL(pFilter,E_POINTER);
        CHECK_POINTER_RETURN_VALUE_IF_FAIL(ppNext,E_POINTER);
        *ppNext = NULL;
        HRESULT hr = E_FAIL;
        IEnumPins *pEnumPins = NULL;
        DX_VERIFY(pFilter->EnumPins(&pEnumPins));
        if (SUCCEEDED(hr))
        {
            IPin *pPin = NULL;
            while ((hr = pEnumPins->Next(1,&pPin,NULL)) == S_OK)
            {
                PIN_DIRECTION thisDir = PINDIR_INPUT;
                DX_VERIFY(pPin->QueryDirection(&thisDir));
                if (thisDir == dir) //�ҵ����ʷ����
                {
                    IPin* pConnectedPin = NULL;
                    DX_VERIFY_EXCEPT1(pPin->ConnectedTo(&pConnectedPin),VFW_E_NOT_CONNECTED);
                    if (SUCCEEDED(hr)) //�������ϵģ�����Pin����Ϣ����ȡ���ڵ�Filter
                    {
                        PIN_INFO pinInfo = {0};
                        DX_VERIFY(pConnectedPin->QueryPinInfo(&pinInfo));
                        *ppNext = pinInfo.pFilter;

                        SAFE_RELEASE(pConnectedPin);
                        SAFE_RELEASE(pPin);
                        SAFE_RELEASE(pEnumPins);

                        return hr;
                    }
                }
                SAFE_RELEASE(pPin);
            }
            SAFE_RELEASE(pEnumPins);
        }
        return hr;
    }

    //! �����Filter Graph�����е�Filter
    HRESULT DirectShowUtility::ClearAllFilters(IFilterGraph *pGraph)
    {
        CHECK_POINTER_RETURN_VALUE_IF_FAIL(pGraph,E_POINTER);
        HRESULT hr = E_FAIL;
#ifdef FTL_DEBUG
        //������ֹͣ״̬
        IMediaControl *pControl = NULL;
        FILTER_STATE state = State_Paused;
        DX_VERIFY(pGraph->QueryInterface(IID_IMediaControl,(void**)&pControl));
        DX_VERIFY(pControl->GetState(INFINITE,(OAFilterState*)&state));
        FTLASSERT(State_Stopped == state); 
#endif
        IEnumFilters* pEnumFilters = NULL;
        DX_VERIFY(pGraph->EnumFilters(&pEnumFilters));
        if (SUCCEEDED(hr))
        {
            IBaseFilter* pFilter = NULL;
            while((hr = pEnumFilters->Next(1,&pFilter,NULL)) == S_OK)
            {
                DX_VERIFY(pGraph->RemoveFilter(pFilter));
                DX_VERIFY(pEnumFilters->Reset());
                SAFE_RELEASE(pFilter);
            }
            SAFE_RELEASE(pEnumFilters);
            if (S_FALSE == hr)  //ѭ�����
            {
                hr = S_OK;
            }
        }
        return hr;
    }

    //��ָ��Filter��ʼ�����ָ�����������Filter -- ��ݹ�
    HRESULT DirectShowUtility::ClearDirFilters(IFilterGraph* pGraph, IBaseFilter* pFilter, PIN_DIRECTION dir)
    {
        CHECK_POINTER_RETURN_VALUE_IF_FAIL(pGraph,E_POINTER);
        CHECK_POINTER_RETURN_VALUE_IF_FAIL(pFilter,E_POINTER);
        HRESULT hr = E_FAIL;
        IEnumPins *pEnumPins = NULL;
        BOOL bPass = TRUE;
        //ö��Filter�ϵ�����Pin
        DX_VERIFY(pFilter->EnumPins(&pEnumPins));
        if (SUCCEEDED(hr))
        {   
            IPin *pPin = NULL;
			DX_VERIFY(pEnumPins->Reset());
            while (bPass && ((hr = pEnumPins->Next(1,&pPin,NULL)) == S_OK))
            {
                PIN_DIRECTION thisDir = PINDIR_INPUT;
                DX_VERIFY(pPin->QueryDirection(&thisDir));
                if (dir == thisDir)  //��������ɾ�������Pin�����Ƿ�������
                {
                    IPin *pConnectedPin = NULL;
                    DX_VERIFY_EXCEPT1(pPin->ConnectedTo(&pConnectedPin),VFW_E_NOT_CONNECTED);
                    if (SUCCEEDED(hr)) //�����ӵ�Pin����ȡ��Pin���ڵ�Filter���ݹ����
                    {
                        PIN_INFO pinInfo = {0};
                        DX_VERIFY(pConnectedPin->QueryPinInfo(&pinInfo));

                        DX_VERIFY(ClearDirFilters(pGraph,pinInfo.pFilter,dir));

                        //�Ͽ���ǰPin����
                        DX_VERIFY(pGraph->Disconnect(pConnectedPin));
                        DX_VERIFY(pGraph->Disconnect(pPin));
                        DX_VERIFY(pGraph->RemoveFilter(pinInfo.pFilter));

                        SAFE_RELEASE(pinInfo.pFilter);
                        SAFE_RELEASE(pConnectedPin);
                    }
                }
                SAFE_RELEASE(pPin);
            }
			if (S_FALSE == hr)
			{
				//IEnumPins::Next ö�����
				hr = S_OK;
			}
            SAFE_RELEASE(pEnumPins);
        }
        return hr;
    }

    //! �ڽ��и�ʽת��ʱ���������ҪԤ�����������ò�ʹ�òο�ʱ�ӣ���ʹ�������е�Sample������ٶȴ��͡�
    //  ʹ�� IFilterGraph::SetDefaultSyncSource �ָ�ȱʡ��
    HRESULT DirectShowUtility::SetNoClock(IFilterGraph *pGraph, IReferenceClock ** pOldClock/* = NULL*/)
    {
        CHECK_POINTER_READABLE_RETURN_VALUE_IF_FAIL(pGraph,E_POINTER);

        HRESULT hr = E_FAIL;
        IMediaFilter *pFilter = NULL;
        DX_VERIFY(pGraph->QueryInterface(IID_IMediaFilter,(void**)&pFilter));
        if (SUCCEEDED(hr))
        {
            if (NULL != pOldClock)
            {
                DX_VERIFY(pFilter->GetSyncSource(pOldClock));
            }
            DX_VERIFY(pFilter->SetSyncSource(NULL));
            SAFE_RELEASE(pFilter);
        }
        return hr;
    }

    HRESULT DirectShowUtility::RenderOutputPins(IGraphBuilder *pGB, IBaseFilter *pFilter)
    {
        HRESULT         hr = S_OK;
        IEnumPins *     pEnumPin = NULL;
        IPin *          pConnectedPin = NULL;
        IPin *          pPin = NULL;
        PIN_DIRECTION   PinDirection;
        ULONG           ulFetched;

        // Enumerate all pins on the filter
        DX_VERIFY(pFilter->EnumPins( &pEnumPin ));
        if(SUCCEEDED(hr))
        {
            // Step through every pin, looking for the output pins
            while (S_OK == (hr = pEnumPin->Next( 1L, &pPin, &ulFetched)))
            {
                // Is this pin connected?  We're not interested in connected pins.
                DX_VERIFY_EXCEPT1(pPin->ConnectedTo(&pConnectedPin),VFW_E_NOT_CONNECTED);
                if (pConnectedPin)
                {
                    pConnectedPin->Release();
                    pConnectedPin = NULL;
                }
                // If this pin is not connected, render it.
                if (VFW_E_NOT_CONNECTED == hr)
                {
                    DX_VERIFY(pPin->QueryDirection( &PinDirection ));
                    if ( ( S_OK == hr ) && ( PinDirection == PINDIR_OUTPUT ) )
                    {
                        DX_VERIFY(pGB->Render(pPin));
                    }
                }
                pPin->Release();
                // If there was an error, stop enumerating
                if (FAILED(hr))
                    break;
            }
        }
        // Release pin enumerator
        pEnumPin->Release();
        return hr;
    }


    HRESULT DirectShowUtility::GetMediaSampleProperty(IMediaSample* pSample,MediaSampleProperty* pProperty)
    {
        CHECK_POINTER_RETURN_VALUE_IF_FAIL(pSample,E_POINTER);    
        CHECK_POINTER_RETURN_VALUE_IF_FAIL(pProperty,E_POINTER);

        HRESULT hr = E_FAIL;
        pProperty->Size = pSample->GetSize();
        pProperty->ActualDataLength = pSample->GetActualDataLength();
        pProperty->IsSyncPoint = (S_OK == pSample->IsSyncPoint());
        pProperty->IsPreroll = (S_OK == pSample->IsPreroll());
        pProperty->IsPreroll = (S_OK == pSample->IsPreroll());
        pProperty->IsDiscontinuity = (S_OK == pSample->IsDiscontinuity());

        hr = pSample->GetMediaTime(&pProperty->MediaTimeStart,&pProperty->MediaTimeEnd); //VFW_E_MEDIA_TIME_NOT_SET
        DX_VERIFY(pSample->GetTime(&pProperty->TimeStart,&pProperty->TimeEnd));
        return S_OK;
    }

    //TODO:�ӿ����
    HRESULT DirectShowUtility::GetMediaTime(IMediaSeeking *pMS)
    {
        CHECK_POINTER_RETURN_VALUE_IF_FAIL(pMS,E_POINTER);
        HRESULT hr;

        // Is media time supported for this file?
        DX_VERIFY(pMS->IsFormatSupported(&TIME_FORMAT_MEDIA_TIME));

        // Read the time format to restore later
        GUID guidOriginalFormat;
        DX_VERIFY(pMS->GetTimeFormat(&guidOriginalFormat));
        if (FAILED(hr))
            return hr;

        // Ensure media time format for easy display
        DX_VERIFY(pMS->SetTimeFormat(&TIME_FORMAT_MEDIA_TIME));
        if (FAILED(hr))
            return hr;

        // Read the file's duration
        LONGLONG llDuration=0;
        DX_VERIFY(pMS->GetDuration(&llDuration));
        if (FAILED(hr))
            return hr;

        // Return to the original format
        if (guidOriginalFormat != TIME_FORMAT_MEDIA_TIME)
        {
            DX_VERIFY(pMS->SetTimeFormat(&guidOriginalFormat));
            if (FAILED(hr))
                return hr;
        }

        // Convert the LONGLONG duration into human-readable format
        unsigned long nTotalMS = (unsigned long) ((float) llDuration / 10000.0); // 100ns -> ms
        int nSeconds = nTotalMS / (int) 1000;
        int nMinutes = nSeconds / (int) 60;
        nSeconds %= 60;

        // Update the display
        TCHAR szDuration[64];
        hr = StringCchPrintf(szDuration, NUMELMS(szDuration), _T("Duration: %d min %2d sec\0"), nMinutes, nSeconds);

        REFERENCE_TIME rtNow;
        // Read the current stream position
        DX_VERIFY(pMS->GetCurrentPosition(&rtNow));
        return hr;
    }

    //��Filter��ץͼ
    HRESULT DirectShowUtility::SnapShotBitmap(IBasicVideo* pBasicVideo, LPCTSTR pszOutFile)
    {
        CHECK_POINTER_READABLE_RETURN_VALUE_IF_FAIL(pBasicVideo,E_POINTER);
        CHECK_POINTER_ISSTRING_PTR_RETURN_VALUE_IF_FAIL(pszOutFile,E_POINTER);
        HRESULT hr = E_FAIL;
        long bitmapSize = 0;
        DX_VERIFY(pBasicVideo->GetCurrentImage(&bitmapSize,NULL));
        if (SUCCEEDED(hr))
        {
            //�����ڴ�
            BYTE* pBuffer = new BYTE[bitmapSize];
            DX_VERIFY(pBasicVideo->GetCurrentImage(&bitmapSize,(long*)pBuffer));
            if (SUCCEEDED(hr))
            {
                LPBITMAPINFOHEADER lpBitmapInfoHeader = (LPBITMAPINFOHEADER)pBuffer;
                int nColors = 1 << lpBitmapInfoHeader->biBitCount;
                if (nColors > 256 ) //�����ɫ������256���Ͳ���Ҫ��ɫ����
                {
                    nColors = 0;
                }
                BITMAPFILEHEADER bmpFileHeader = {0};
                bmpFileHeader.bfType = ((DWORD)('M' << 8) | 'B' ); //���Ϊ "BM"
                bmpFileHeader.bfSize = bitmapSize + sizeof(bmpFileHeader);
                bmpFileHeader.bfOffBits = (DWORD)(sizeof(BITMAPFILEHEADER) 
                    + lpBitmapInfoHeader->biSize + nColors * sizeof(RGBQUAD));

                HANDLE hFile = ::CreateFile(pszOutFile,GENERIC_WRITE,0,NULL,
                    CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
                if (INVALID_HANDLE_VALUE != hFile)
                {
                    BOOL bRet = FALSE;
                    API_VERIFY(::WriteFile(hFile,&bmpFileHeader,sizeof(BITMAPFILEHEADER),NULL,NULL));
                    API_VERIFY(::WriteFile(hFile,pBuffer,bitmapSize,NULL,NULL));
                    SAFE_CLOSE_HANDLE(hFile,INVALID_HANDLE_VALUE);
                    if (!bRet)
                    {
                        hr = HRESULT_FROM_WIN32(GetLastError());
                    }
                }
            }
            SAFE_DELETE_ARRAY(pBuffer);
        }
        return hr;
    }

    HRESULT DirectShowUtility::InitCaptureGraphBuilder(IGraphBuilder **ppGraph, 
        ICaptureGraphBuilder2 ** ppCapture)
    {
        CHECK_POINTER_RETURN_VALUE_IF_FAIL(ppGraph,E_POINTER);
        CHECK_POINTER_RETURN_VALUE_IF_FAIL(ppCapture,E_POINTER);

        HRESULT hr = E_FAIL;
        *ppGraph = NULL;
        *ppCapture = NULL;
        IGraphBuilder *pGraph = NULL;
        ICaptureGraphBuilder2 * pCapture = NULL;

		// Create the capture graph builder
        DX_VERIFY(CoCreateInstance(CLSID_CaptureGraphBuilder2, NULL,
            CLSCTX_INPROC,	// CLSCTX_INPROC_SERVER
			IID_ICaptureGraphBuilder2, (void**)&pCapture));
        if (SUCCEEDED(hr))
        {
			// Create the filter graph
            DX_VERIFY(CoCreateInstance(CLSID_FilterGraph, NULL,
				CLSCTX_INPROC, //CLSCTX_INPROC_SERVER,
                IID_IGraphBuilder,(void**)&pGraph));
            if (SUCCEEDED(hr))
            {
				//Attach the filter graph to the capture graph
                DX_VERIFY(pCapture->SetFiltergraph(pGraph));
                if (SUCCEEDED(hr))
                {
                    *ppCapture = pCapture;
                    *ppGraph = pGraph;
                    return S_OK;
                }
            }
        }
        SAFE_RELEASE(pGraph);
        SAFE_RELEASE(pCapture);
        return hr;
    }

//#ifdef USE_DIRECTX_9
    //////////////////////////////////////////////////////////////////////////
    //                  Direct3DUtility
    //////////////////////////////////////////////////////////////////////////
    DWORD Direct3DUtility::EnterMsgLoop( bool (*ptr_display)(float timeDelta) )
    {
        MSG msg;
        ::ZeroMemory(&msg, sizeof(MSG));
        static float lastTime = (float)timeGetTime();

        while(TRUE)
        {
            if(::PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
            {
                if( WM_QUIT == msg.message )
                {
                    break;
                }
                //�������Ϣ��Ҫ����
                ::TranslateMessage(&msg);
                ::DispatchMessage(&msg);
            }
            else
            {
                //û����Ϣ��������������Ϸ�߼���AI�ȴ���
                float currTime = (float)timeGetTime();
                float timeDelta = (currTime - lastTime)*0.001f;
                ptr_display(timeDelta); // call display function
                lastTime = currTime;
            }
        }
        return 0;//msg.wParam;

#if 0
        //������ʹ�� DXUT ���Զ�����Ϣѭ��,ͨ����ʹ�� DXUTMainLoop 
        // Custom main loop    
        HWND hWnd = DXUTGetHWND();
        BOOL bGotMsg;
        MSG  msg;
        msg.message = WM_NULL;
        PeekMessage( &msg, NULL, 0U, 0U, PM_NOREMOVE );

        while( WM_QUIT != msg.message )
        {        
            // Use PeekMessage() so we can use idle time to render the scene
            bGotMsg = ( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) != 0 );
            if( bGotMsg )
            {
                // Translate and dispatch the message
                if( 0 == TranslateAccelerator( hWnd, NULL, &msg ) )
                {
                    TranslateMessage( &msg );
                    DispatchMessage( &msg );
                }
            }
            else
            {
                // Render a frame during idle time (no messages are waiting)
                DXUTRender3DEnvironment();
            }
        }
        return DXUTGetExitCode();
#endif 
    }

    DWORD Direct3DUtility::DumpAdapterModes(IDirect3D9* pD3D)
    {
        const D3DFORMAT allowedAdapterFormatArray[] = 
        {   
            D3DFMT_R8G8B8 ,
            D3DFMT_A8R8G8B8 ,
            D3DFMT_X8R8G8B8 ,
            D3DFMT_R5G6B5 ,
            D3DFMT_X1R5G5B5 ,
            D3DFMT_A1R5G5B5 ,
            D3DFMT_A4R4G4B4 ,
            D3DFMT_R3G3B2 ,
            D3DFMT_A8 ,
            D3DFMT_A8R3G3B2 ,
            D3DFMT_X4R4G4B4 ,
            D3DFMT_A2B10G10R10 ,
            D3DFMT_A8B8G8R8 ,
            D3DFMT_X8B8G8R8 ,
            D3DFMT_G16R16 ,
            D3DFMT_A2R10G10B10 ,
            D3DFMT_A16B16G16R16 ,

            D3DFMT_A8P8 ,
            D3DFMT_P8 ,

            D3DFMT_L8 ,
            D3DFMT_A8L8 ,
            D3DFMT_A4L4 ,

            D3DFMT_V8U8 ,
            D3DFMT_L6V5U5 ,
            D3DFMT_X8L8V8U8 ,
            D3DFMT_Q8W8V8U8 ,
            D3DFMT_V16U16 ,
            D3DFMT_A2W10V10U10 ,

            D3DFMT_UYVY ,
            D3DFMT_R8G8_B8G8 ,
            D3DFMT_YUY2 ,
            D3DFMT_G8R8_G8B8 ,
            D3DFMT_DXT1 ,
            D3DFMT_DXT2 ,
            D3DFMT_DXT3 ,
            D3DFMT_DXT4 ,
            D3DFMT_DXT5 ,

            D3DFMT_D16_LOCKABLE ,
            D3DFMT_D32 ,
            D3DFMT_D15S1 ,
            D3DFMT_D24S8 ,
            D3DFMT_D24X8 ,
            D3DFMT_D24X4S4 ,
            D3DFMT_D16 ,

            D3DFMT_D32F_LOCKABLE ,
            D3DFMT_D24FS8 ,

            D3DFMT_L16 ,

            D3DFMT_VERTEXDATA ,
            D3DFMT_INDEX16 ,
            D3DFMT_INDEX32 ,

            D3DFMT_Q16W16V16U16 ,

            D3DFMT_MULTI2_ARGB8 ,

            // Floating point surface formats

            // s10e5 formats (16-bits per channel)
            D3DFMT_R16F ,
            D3DFMT_G16R16F ,
            D3DFMT_A16B16G16R16F ,

            // IEEE s23e8 formats (32-bits per channel)
            D3DFMT_R32F ,
            D3DFMT_G32R32F ,
            D3DFMT_A32B32G32R32F ,

            D3DFMT_CxV8U8 ,
        };

        HRESULT hr = E_FAIL;
        DWORD dwCount = 0;
        const UINT allowedAdapterFormatArrayCount  = _countof(allowedAdapterFormatArray);

        USES_CONVERSION;

        //��ȡ�������ĸ���
        UINT numAdapters = pD3D->GetAdapterCount();
        for (UINT adapterOrdinal = 0; adapterOrdinal < numAdapters; adapterOrdinal++)
        {
            D3DADAPTER_IDENTIFIER9 adapterIdentifier = {0};
            DX_VERIFY(pD3D->GetAdapterIdentifier(adapterOrdinal, 0, &adapterIdentifier));

            FTLTRACE(TEXT("Adapter Identifier[%d]:driver=%s, desc=%s, deviceName=%s\n\t")
                TEXT("version=[%d,%d], vendorId=%d, deviceid=%d,subsysid=%d, Revision=%d, WHQLLevel=%d\n"),
                adapterOrdinal,
                A2T(adapterIdentifier.Driver), A2T(adapterIdentifier.Description), A2T(adapterIdentifier.DeviceName),
                adapterIdentifier.DriverVersion.HighPart, adapterIdentifier.DriverVersion.LowPart,
                adapterIdentifier.VendorId, adapterIdentifier.DeviceId,adapterIdentifier.SubSysId,
                adapterIdentifier.Revision,adapterIdentifier.WHQLLevel);

            for( UINT iFormatList = 0; iFormatList < allowedAdapterFormatArrayCount; iFormatList++ )
            {
                D3DFORMAT allowedAdapterFormat = allowedAdapterFormatArray[iFormatList];
                UINT numAdapterModes = pD3D->GetAdapterModeCount( adapterOrdinal, allowedAdapterFormat );
                for (UINT mode = 0; mode < numAdapterModes; mode++)
                {
                    D3DDISPLAYMODE displayMode = {0};
                    DX_VERIFY( pD3D->EnumAdapterModes( adapterOrdinal, allowedAdapterFormat, mode, &displayMode ));
                    dwCount++;

                    //FTLTRACEEX(FTL::tlInfo, "")
                }
            }
        }

        FTLTRACE(TEXT("DumpAdapterModes, ModeCount = %d\n"), dwCount);
        return dwCount;
    }


	CFDShowHardwareMgr::CFDShowHardwareMgr()
	{

	}

	CFDShowHardwareMgr::~CFDShowHardwareMgr()
	{
		Clear();
	}

	HRESULT CFDShowHardwareMgr::Refresh(const CLSID* pDevClsid)
	{
		HRESULT hr = E_FAIL;
		CComPtr<ICreateDevEnum> pSysDevEnum;
		COM_VERIFY(CoCreateInstance(CLSID_SystemDeviceEnum,NULL,CLSCTX_INPROC,IID_ICreateDevEnum,(void**)&pSysDevEnum));
		if (SUCCEEDED(hr))
		{
			CComPtr<IEnumMoniker> pEnumCat;
			COM_VERIFY_EXCEPT1(pSysDevEnum->CreateClassEnumerator(*pDevClsid, &pEnumCat,0), S_FALSE); //���������û�ж�����᷵��S_FALSE
			if (pEnumCat)
			{
				//if create class Enumerator Success, then clear old, otherwise will remain
				Clear();

				COM_VERIFY(pEnumCat->Reset());
				CComPtr<IMoniker> pMoniker;
				ULONG ulFetched = 0;
				while (pEnumCat->Next(1, &pMoniker, &ulFetched) == S_OK)
				{
					COM_VERIFY(_AddNewMoniker(pMoniker));
					pMoniker.Release();
				}

				//If CreateClassEnumerator Success then result is Success, even there are no Item
				hr = S_OK;
			}
		}
		return hr;
	}

	HRESULT CFDShowHardwareMgr::Clear()
	{
		std::for_each(m_Hardwares.begin(), m_Hardwares.end(), FTL::ObjecteDeleter<HardwareMonikerInfo*>());
		m_Hardwares.clear();
		return S_OK;
	}

	HRESULT CFDShowHardwareMgr::_AddNewMoniker(IMoniker* pMoniker)
	{
		CHECK_POINTER_RETURN_VALUE_IF_FAIL(pMoniker, E_POINTER);

		HRESULT hr = E_FAIL;
		CComPtr<IPropertyBag> spPropertyBag;
		COM_VERIFY(pMoniker->BindToStorage(0, 0, IID_IPropertyBag, (void **)&spPropertyBag));
		if (spPropertyBag)
		{
			VARIANT var;
			var.vt = VT_BSTR;
			COM_VERIFY(spPropertyBag->Read(L"FriendlyName", &var, NULL));
			if (S_OK == hr)
			{
				HardwareMonikerInfo* pHardwareMonikerInfo = new HardwareMonikerInfo();
				//pHardwareMonikerInfo->strFriendlyName = var.bstrVal;
				StringCchCopyW(pHardwareMonikerInfo->wachFriendlyName, _countof(pHardwareMonikerInfo->wachFriendlyName), 
					var.bstrVal);
				pHardwareMonikerInfo->pMoniker = pMoniker;
				pMoniker->AddRef();
				//m_Hardwares[pHardwareMonikerInfo->strFriendlyName] = pHardwareMonikerInfo;
				m_Hardwares.push_back(pHardwareMonikerInfo);
				SysFreeString(var.bstrVal);
			}
		}
		return hr;
	}

	HRESULT CFDShowHardwareMgr::GetBindObject(LPCWSTR pszName, REFIID riidResult, void **ppvResult)
	{
		HRESULT hr = E_FAIL;
		HardwareMonikerInfoContainerIter iter = m_Hardwares.begin();
		for ( ;	iter != m_Hardwares.end(); ++iter)
		{
			if (lstrcmpW(pszName, (*iter)->wachFriendlyName) == 0)
			{
				//Found
				break;
			}
			else
			{
				//Because IPropertyBag::Read(L"FriendlyName") limit 32 TCHAR, so just check part of the string
				if (lstrlen((*iter)->wachFriendlyName) > DS_FRIENDLY_NAME_MAX_LENGTH - 2
					&& NULL != wcsstr(pszName, (*iter)->wachFriendlyName))
				{
					//Found
					break;
				}
			}
		}
		if (iter != m_Hardwares.end())
		{
			//Found
			IMoniker* pMoniker = (*iter)->pMoniker;
			COM_VERIFY(pMoniker->BindToObject(NULL, NULL, riidResult, (void**)ppvResult));
		}
		return hr;
	}

//#endif //USE_DIRECTX_9
}

#endif //FTL_DIRECTX_HPP