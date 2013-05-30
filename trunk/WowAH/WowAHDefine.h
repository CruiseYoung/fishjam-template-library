#pragma once

#include <ftlBase.h>
#include <ftlSharePtr.h>
using namespace FTL;

#include <map>
#include <functional>

#ifndef INVALID_LONG
#  define INVALID_LONG		(LONG)(-1)
#endif 

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

//enum ItemSellerType{
//	istInvalid = -1,
//	istOthers,
//	istMine,
//};

enum ItemStatus
{
	isInvalid = -1,

	isOtherSelling,			//���������ڳ���

	//����ҳ
	isMyBidsActive,
	isMyBidsWon,
	isMyBidsLost,

	//����ҳ
	isMyAuctionActive,		//�����е�����
	isMyAuctionSold,		//���۳�
	isMyAuctionEnded,		//�ѽ���������
};

//�������е���Ϣ
class WowItemInfo
{
public:
	WowItemInfo();
	static LPCTSTR GetSellTimeInfoString(ItemSellTimeInfo timeInfo);
	static LPCTSTR GetItemStatusString(ItemStatus itemStatus);

	BOOL				IsValidItem();
	VOID				Dump(LONG nIndex, LONG nCount);

	//void UpdateRefreshTime();

	ItemStatus GetItemStatus() const { return m_itemStatus; }
	void SetItemStatus(ItemStatus val) { m_itemStatus = val; }
	//ItemSellerType GetItemSellerType() const { return m_ItemSellerType; }
	//void SetItemSellerType(ItemSellerType val) { m_ItemSellerType = val; }
	LONG GetAuctionId() const { return m_nAuctionId; }
	void SetAuctionId(LONG val) { m_nAuctionId = val; }
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
	LONG GetAuctionSoldPrice() const { return m_nAuctionSoldPrice; }
	void SetAuctionSoldPrice(LONG val) { m_nAuctionSoldPrice = val; }
	LONG GetRemainDayInMail() const { return m_nRemainDayInMail; }
	void SetRemainDayInMail(LONG val) { m_nRemainDayInMail = val; }
	LONG GetMailId() const { return m_nMailId; }
	void SetMailId(LONG val) { m_nMailId = val; }

	const CString& GetItemName() const { return m_strItemName; }
	void SetItemName(const CString& strItemName);
	const CString& GetUrl() const { return m_strUrl; }
	void SetUrl(const CString& strUrl);
	const CString& GetSeller() const { return m_strSeller; }
	void SetSeller(const CString& val) { m_strSeller = val; }
	CString GetPurchaser() const { return m_strPurchaser; }
	void SetPurchaser(CString val) { m_strPurchaser = val; }

	
private:
	ItemStatus			m_itemStatus;
	//ItemSellerType		m_ItemSellerType;
	LONG				m_nAuctionId;		//ÿһ���������Ψһ��ʶ
	LONG				m_nTypeId;			//���ͣ�Ы�� ��Ӧ�� 5466, �� UserConfigItemInfo ����
	LONG				m_nDataItem;		//
	ItemSellTimeInfo	m_ItemSellTimeInfo;
	LONG				m_nQuantity;			//����
	LONG				m_nPriceBid;			//�����ۣ���� * 10000 + ���� * 100 + ͭ��
	LONG				m_nPriceBuyout;			//һ�ڼۣ���� * 10000 + ���� * 100 + ͭ�ң����ֵΪ0��ʾû��һ�ڼ�
	LONG				m_nAuctionSoldPrice;	//�����۳��ļ۸�
	LONG				m_nRemainDayInMail;		//�����ﱣ�������
	LONG				m_nMailId;				//����ID
	CString				m_strItemName;  
	CString				m_strUrl;
	CString				m_strSeller;			//������
	CString				m_strPurchaser;			//������
	ULONGLONG			m_nLastRefreshTickCount;//�ϴ�ˢ�µ�ʱ���
	ULONGLONG			m_nExpirationTickCount;	//��������ĵ���ʱ���
};

typedef CFSharePtr<WowItemInfo>	WowItemInfoPtr;


//ǰ������
class CWowItemManager;

