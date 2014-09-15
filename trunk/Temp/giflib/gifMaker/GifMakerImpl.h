#pragma once

#include "gifMaker.h"

#include <ftlBase.h>
#include <ftlThread.h>
#include <ftlGdi.h>
#include <ftlDebug.h>
#include <ftlAlgorithm.h>
#include <ftlSharePtr.h>

#include <ColorQuantizer/WuColorQuantizer.h>
#include <ColorQuantizer/OctreeColorQuantizer.h>
#include "Quantize.h"

#include "../giflib/lib/gif_lib.h"

#define GIF_VERIFY(x) \
    nRet = (x);\
    if(nRet == GIF_ERROR){\
        FTLTRACE(TEXT("ERROR: %s(%d)"), TEXT(#x), 0); \
    }


class SnapBiampInfo{
public:
    SnapBiampInfo();
    SnapBiampInfo(int nIndex, BYTE* pBmpData, DWORD dwTicket, const RECT& rcTarget, int nBpp, BOOL bAttachMem);
    ~SnapBiampInfo();
public:
    int     nIndex;
    BYTE*   pBmpData;
    DWORD   dwTicket;
    RECT    rcTarget; 
    int     nBpp;
};
typedef CFSharePtr<SnapBiampInfo>   SnapBiampInfoPtr;

class CGifMakerImpl : public IGifMaker
{
public:
    CGifMakerImpl(void);
    virtual ~CGifMakerImpl();
    virtual CompressType SetCompressType(CompressType type);

    virtual BOOL BeginMakeGif(int nWidth, int nHeight, int bpp, LPCTSTR pszFileName);
    virtual BOOL AddGifImage(BYTE* pBmpData, int nLength, DWORD dwTicket);
    virtual BOOL EndMakeGif(DWORD dwTicket, DWORD dwWaitTimeOut = INFINITE);
    virtual void Release();

private:
    FTL::CFThread<>* m_pThreadMaker;
    FTL::CFProducerResumerQueue<SnapBiampInfoPtr>   m_WaitingBitmapInfoQueue;
    static DWORD  __stdcall MakerThreadProc(LPVOID lpThreadParameter);
    DWORD  _innerMakerThreadProc();

    CompressType    m_compressType;
    int m_nWidth, m_nHeight, m_nBpp;
    int m_nPreBmpBytes;     //��һ��ͼƬ�Ĵ�С
    int m_nDiffResultSize;
    int m_nGifColorRes;     //8
    int m_nGifNumLevels;    //256
    int m_nImgCount;
    int m_nGifBufferSize;
    BOOL m_bFirstImage;
    BOOL m_bDelayImage;
    BOOL m_bWriteFirst;

    BYTE* m_pPreBmp;
    BYTE* m_pDiffResult;
    //BYTE* m_pGiffDiffBuffer;
    RECT  m_rcDiff;

    GifFileType* m_pGifFile;
    GifColorType* m_pColorMap256;   //������Gif����ɫ����ֵ
    GifByteType*  m_pGifBuffer;     //���������������ֵ

    FTL::IFColorQuantizer*  m_pColorQuantizer;

    DWORD m_dwLastTicket;
    //CQuantizer* m_pQuantizer;
    

    BOOL _WriteGifData(SnapBiampInfoPtr& pSnapBitmapInfo);

    BYTE* _DuplicateBmpRect(BYTE* pSrcBmpData, int nSrcWidth, int nSrcHeight, int nBpp, RECT rcSrc);
    VOID _FreeDuplicateBmpData(BYTE* pData);

};
