#include "StdAfx.h"
#include "WowAHDefine.h"
#include <ftlSystem.h>

WowItemInfo::WowItemInfo()
{
	m_nId = 0;
	m_nTypeId = 0;
	m_nDataItem = 0;
	m_ItemSellTimeInfo = istiInvalid;
	m_nLastRefreshTickCount = FTL::CFSystemUtil::GetTickCount64();
	m_nExpirationTickCount = 0;
	m_nQuantity = 0;
	m_nPriceBid = 0;
	m_nPriceBuyout = 0;

	m_strItemName = _T("");
	m_strUrl = _T("");
	m_strSeller = _T("");
}

LPCTSTR WowItemInfo::GetSellTimeInfoString(ItemSellTimeInfo timeInfo)
{
	switch (timeInfo)
	{
		HANDLE_CASE_RETURN_STRING_EX(istiVeryLong, TEXT("�ǳ���"));
		HANDLE_CASE_RETURN_STRING_EX(istiLong, TEXT("��"));
		HANDLE_CASE_RETURN_STRING_EX(istiMiddle, TEXT("��"));
		HANDLE_CASE_RETURN_STRING_EX(istiShort, TEXT("��"));
	default:
		//FTLASSERT(FALSE);
		break;
	}
	return _T("δ֪");
}

void WowItemInfo::UpdateRefreshTime()
{
	m_nLastRefreshTickCount = FTL::CFSystemUtil::GetTickCount64();
}

void WowItemInfo::SetItemSellTimeInfo(ItemSellTimeInfo timeInfo)
{
	if (m_ItemSellTimeInfo != timeInfo)
	{
		if (istiInvalid != m_ItemSellTimeInfo )
		{
			//�������������͵ı��(�� verylong -> long)���������ܵ�ʣ��ʱ��
			//ע�⣺���һ��ʼ���� istiShort�� ���޷�����ʣ��ʱ��

			//TODO: �����ϴθ��µ�ʱ�䣬�����ʱ���Ӷ�����ȷ�Ĺ���
			ULONGLONG nCurTickCount = FTL::CFSystemUtil::GetTickCount64();

			ULONGLONG nDiffTickCount = nCurTickCount - m_nLastRefreshTickCount;

			switch (timeInfo)
			{
			case istiLong:	//�ոձ䵽12Сʱ
				m_nExpirationTickCount = nCurTickCount + 12 * 3600 * 1000 - nDiffTickCount;
				break;
			case istiMiddle://�ոձ䵽2Сʱ
				m_nExpirationTickCount = nCurTickCount + 2 * 3600 * 1000 - nDiffTickCount;
				break;
			case istiShort: //��Сʱ����
				m_nExpirationTickCount = nCurTickCount + 2 * 3600 * 1000 - nDiffTickCount;
				break;
			default:
				FTLASSERT(FALSE);
				break;
			}
			FTLTRACE(TEXT("nCurTickCount=%lld, nDiffTickCount = %lld, m_nExpirationTickCount=%lld\n"), 
				nCurTickCount, nDiffTickCount, m_nExpirationTickCount);

		}
		else
		{
			//��ʼ���ã��򵥸�ֵ����������������
		}
		m_ItemSellTimeInfo = timeInfo;
	}
	
}

VOID WowItemInfo::Dump()
{
	CString strExpirationTime;
	if (m_nExpirationTickCount > 0)
	{
		ULONGLONG nRemaineTicket = (m_nExpirationTickCount - FTL::CFSystemUtil::GetTickCount64()) / 1000;
		LONG nHour = (LONG)(nRemaineTicket / 3600);
		LONG nMinute = (LONG)((nRemaineTicket % 3600) / 60 );
		LONG nSecond = (LONG)(nRemaineTicket % 60);
		strExpirationTime.Format(TEXT("%d:%d:%d"), 
			nHour, nMinute, nSecond);
	}
	else
	{
		strExpirationTime = TEXT("Unknown");
	}
	FTLTRACE(
		TEXT("Item: Name=%s, strUrl=%s, Seller=%s, ")
		TEXT("id=%d, typeId=%d, SellTimeInfo=%s, Expiration Remain:%s, ")
		TEXT("DataItem=%d, PriceBid=%d, PriceBuyout=%d, Quantity=%d\n"),

		m_strItemName, m_strUrl, m_strSeller, 
		m_nId, m_nTypeId, GetSellTimeInfoString(m_ItemSellTimeInfo), strExpirationTime,
		m_nDataItem, m_nPriceBid, m_nPriceBuyout, m_nQuantity);
}