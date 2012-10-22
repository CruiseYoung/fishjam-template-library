#pragma once

/*************************************************************************************************
* CAMSchedule -- ��ʵ�������һ��List��Node������CAdvisePacket, �����������
* CAutoLock + CCritSec -- ��Χ����������Դ����
* CBaseAllocator -- ����IMediaSample�б�ĳ�����࣬����ʵ��Alloc��Free����,Addʱ�Ǽ����б����ǰ��(ʹ��Ч�ʸ�?)
*    ��ʵ��ʱʹ����һЩС����(lazy signalling mechanism),ֻ�ڱ�Ҫʱ�Ž���OS�ȴ�ģʽ���Ӷ���߶��߳�ʱ��Ч��
* +- CMemAllocator -- Ĭ�ϵ�Sample������(ʹ��new�����洢���з����ڴ�)����������/�ͷ�Sample���ڴ棬ά��Sample���е�
*     ͨ��ȫ�ֺ��� CreateMemoryAllocator ���� CLSID_MemoryAllocator
* CBaseReferenceClock -- �̳��� IReferenceClock �ӿڣ�
* CBaseWindow -- �������� Render �Ĵ��壬�������Լ��Ĺ����̡߳�
* CDrawImage -- 
* CEnumPins -- ö��Filter�ϵ�PIn
* CEnumMediaTypes -- Pin�ϵ���ѡý������ö��
* CGenericList -- ͨ�õ�Listģ�����
* CImageDisplay -- Video Renderer ʹ�õĹ����࣬���Ի�ȡ��ʾý��ĸ�ʽ����Ϣ��
* CMediaSample -- ��װ��ָ�������ڴ��ָ�뼰��ص����ԡ�Sample��Pin�ϵ�Sample��������������ά��
*   ��һ������Sample������(CBaseAllocator����?)����Ҫʹ��ʱ��ͨ��GetBuffer���һ����Sample(�����ǰû�п�Sample�Ļ����ȴ�)��
*   ʹ����Ϻ�Release��������ü�����Ϊ0�����Զ�����IMemAllocator::ReleaseBuffer�����Լ����¼���
*   ����Sample�����С�ֱ������IMemAllocator::Decommitʱ��Sample����Ż�����������
* CPosPassThru -- �� Render �� Transform Filter �д��� IMediaPosition IMediaSeeking ���࣬��������λ�ÿ��ƣ�
*    CreatePosPassThru �������� ISeekingPassThru �ӿ�ʵ��
* CRefTime
*   COARefTime -- �� Double/REFERENCE_TIME �ȸ�ʽ��ת���ĸ�����
*************************************************************************************************/
class CDirectShowUtil
{
public:
    CDirectShowUtil(void);
    ~CDirectShowUtil(void);
    static BOOL SetFilterMerit(LPCTSTR inClsid, DWORD inMerit);
};
 