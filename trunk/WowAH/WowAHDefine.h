#pragma once

#include <ftlBase.h>
#include <ftlSharePtr.h>
using namespace FTL;

#include <map>
#include <functional>

//�û����õ���Ҫ��������Ŀ����Ϣ
struct UserConfigItemInfo
{
	LONG		nTypeId;				//5466  -- /wow/zh/item/5466
	CString		strName;				//��Ʒ�� -- Ы��
	CString     strUrl;					//http://content.battlenet.com.cn/wow/icons/36/spell_nature_nullifypoison.jpg
	INT			nBuyPriceBid;			//����
	INT			nBuyPriceBuyout;		//һ�ڼ�
	//INT			nSellPriceBid;
	//INT			nSellPriceBuyout;
};

//typedef CFSharePtr<UserConfigItemInfo>	UserConfigItemInfoPtr;


enum ItemSellTimeInfo
{
	istiInvalid,		//��Чֵ�����ڳ�ʼ��

	//����ֵ��Ҫȷ��
	istiVeryLong,		//�ǳ���(����24Сʱ)
	istiLong,			//��(��2Сʱ��12Сʱ֮��)
	istiMiddle,			//��(2Сʱ����)
	istiShort,			//��(����30����)
};

//�������е���Ϣ
class WowItemInfo
{
public:
	WowItemInfo();
	LPCTSTR				GetSellTimeInfoString(ItemSellTimeInfo timeInfo);
	VOID				Dump();

	void UpdateRefreshTime();

	LONG GetId() const { return m_nId; }
	void SetId(LONG val) { m_nId = val; }
	LONG GetTypeId() const { return m_nTypeId; }
	void SetTypeId(LONG val) { m_nTypeId = val; }
	LONG GetDataItem() const { return m_nDataItem; }
	void SetDataItem(LONG val) { m_nDataItem = val; }
	ItemSellTimeInfo GetItemSellTimeInfo() const { return m_ItemSellTimeInfo; }
	void SetItemSellTimeInfo(ItemSellTimeInfo timeInfo);
	LONG GetQuantity() const { return m_nQuantity; }
	void SetQuantity(LONG val) { m_nQuantity = val; }
	LONG GetPriceBid() const { return m_nPriceBid; }
	void SetPriceBid(LONG val) { m_nPriceBid = val; }
	LONG GetPriceBuyout() const { return m_nPriceBuyout; }
	void SetPriceBuyout(LONG val) { m_nPriceBuyout = val; }
	const CString& GetUrl() const { return m_strUrl; }
	void SetUrl(const CString& val) { m_strUrl = val; }
	const CString& GetSeller() const { return m_strSeller; }
	void SetSeller(const CString& val) { m_strSeller = val; }
	const CString& GetItemName() const { return m_strItemName; }
	void SetItemName(const CString& val) { m_strItemName = val; }
	
	
private:

	LONG				m_nId;		//ÿһ���Ψһ��ʶ
	LONG				m_nTypeId;	//���ͣ�Ы�� ��Ӧ�� 5466, �� UserConfigItemInfo ����
	LONG				m_nDataItem;	//
	ItemSellTimeInfo	m_ItemSellTimeInfo;
	LONG				m_nQuantity;			//����
	LONG				m_nPriceBid;			//�����ۣ���� * 10000 + ���� * 100 + ͭ��
	LONG				m_nPriceBuyout;			//һ�ڼۣ���� * 10000 + ���� * 100 + ͭ�ң����ֵΪ0��ʾû��һ�ڼ�
	CString				m_strItemName;  
	CString				m_strUrl;
	CString				m_strSeller;

	ULONGLONG			m_nLastRefreshTickCount;//�ϴ�ˢ�µ�ʱ���
	ULONGLONG			m_nExpirationTickCount;	//��������ĵ���ʱ���
};

typedef CFSharePtr<WowItemInfo>	WowItemInfoPtr;


//ǰ������
class CWowItemManager;
class CBitItemPageAnalyze;
