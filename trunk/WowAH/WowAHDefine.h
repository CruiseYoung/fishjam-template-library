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
	istiVeryLong,		//�ǳ���(����24Сʱ)
	istiLong,			//��(��2Сʱ��12Сʱ֮��)
	istiShort,			//��(2Сʱ����) ��
};

//�������е���Ϣ
struct ItemInAHInfo
{
	ItemInAHInfo();

	LONG				m_nId;		//ÿһ���Ψһ��ʶ
	LONG				m_nTypeId;	//���ͣ�Ы�� ��Ӧ�� 5466, �� UserConfigItemInfo ����
	LONG				m_nDataItem;	//
	ItemSellTimeInfo	m_ItemSellTimeInfo;
	DWORD				m_nLastCheckTime;		//�ϴμ���ʱ���
	DWORD				m_nRemainTime;			//���������׼ȷֵ
	LONG				m_nQuantity;			//����
	LONG				m_nPriceBid;			//�����ۣ���� * 10000 + ���� * 100 + ͭ��
	LONG				m_nPriceBuyout;			//һ�ڼۣ���� * 10000 + ���� * 100 + ͭ��

	CString				m_strItemName;  
	CString				m_strUrl;
	CString				m_strSeller;

	VOID				Dump();
	LPCTSTR				GetSellTimeInfoString(ItemSellTimeInfo timeInfo);
};

typedef CFSharePtr<ItemInAHInfo>	ItemInAHInfoPtr;

typedef std::map<LONG, ItemInAHInfoPtr>		ItemInAHInfoContainer;