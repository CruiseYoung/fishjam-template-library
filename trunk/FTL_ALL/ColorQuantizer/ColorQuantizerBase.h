#pragma once

#include <ftlBase.h>
#include "ColorQuantizer.h"

namespace FTL
{
    class CFColorQuantizerBase : public IFColorQuantizer{
    public:
        CFColorQuantizerBase();
        virtual ~CFColorQuantizerBase();
    public:
        //IFColorQuantizer
        virtual BOOL SetBmpInfo(UINT nWidth, UINT nHeight, UINT nBpp, BYTE* pBmpData, UINT nBmpDataSize);
        virtual COLORREF*   GetPalette(UINT* pResultCount);
        virtual BOOL ProcessQuantizer(UINT colorCount, UINT *pResultClrCount);
    protected:
        virtual BOOL OnPrepare();
        virtual void OnFinish();
        virtual BOOL OnProcessQuantizer(UINT colorCount, UINT *pResultClrCount) = 0;
    protected:
        UINT m_nWidth;
        UINT m_nHeight;
        UINT m_nBpp;
        UINT m_nBmpDataSize;    //�ܵ��ֽ���
        UINT m_nBmpLineBytes;   //ÿ�е��ֽ���(4�ֽڶ���)
        UINT m_nPaletteItemCount;

        BYTE*       m_pBmpData;
        COLORREF*   m_pResultPalette;
        UCHAR*      m_pResultBuffer;
    };
}
