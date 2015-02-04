#ifndef IE_EXTENSION_SUPPORT_H
#define IE_EXTENSION_SUPPORT_H
#pragma once

class CIEExtensionSupport
{
public:
    //��������Ĺ������л�ȡ IE �������ָ��,ͨ�� IObjectWithSite ��(SetSite)(IUnknown* pUnkSite)����
    static HRESULT DemoGetWebBrowserInDeskBand(IUnknown* pUnkSite)
    {
        //Get the parent window.
        IOleWindow  *pOleWindow = NULL;
        HWND hWndParent = NULL;
        if(SUCCEEDED(pUnkSite->QueryInterface(IID_IOleWindow, (LPVOID*)&pOleWindow)))
        {
            pOleWindow->GetWindow(&hWndParent);
            pOleWindow->Release();
        }
        //Get and keep the IInputObjectSite pointer.
        IInputObjectSite* pSite = NULL;
        pUnkSite->QueryInterface(IID_IInputObjectSite, (LPVOID*)&pSite);

        //��ȡWebBrower�ӿ�
        IWebBrowser2* s_pFrameWB = NULL;
        IOleCommandTarget* pCmdTarget = NULL;
        HRESULT hr = pUnkSite->QueryInterface(IID_IOleCommandTarget, (LPVOID*)&pCmdTarget);
        if (SUCCEEDED(hr))
        {
            IServiceProvider* pSP;
            hr = pCmdTarget->QueryInterface(IID_IServiceProvider, (LPVOID*)&pSP);

            pCmdTarget->Release();

            if (SUCCEEDED(hr))
            {
                hr = pSP->QueryService(SID_SWebBrowserApp, IID_IWebBrowser2, (LPVOID*)&s_pFrameWB);
                pSP->Release();
                _ASSERT(s_pFrameWB);
                //ʹ�ýӿ�
                s_pFrameWB->Release();
                s_pFrameWB = NULL;
            }
        }
    }
};
#endif //IE_EXTENSION_SUPPORT_H