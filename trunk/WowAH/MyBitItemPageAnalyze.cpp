#include "StdAfx.h"
#include "MyBitItemPageAnalyze.h"
#include <ftlWebInterfaceDetect.h>

//��ǰ����
#define TABLE_NAME_BIDS_ACTIVE		TEXT("bids-active")

//Ӯ������
#define TABLE_NAME_BIDS_WON			TEXT("bids-won")

//����ʧ��
#define TABLE_NAME_BIDS_LOST		TEXT("bids-lost")


CMyBitItemPageAnalyze::CMyBitItemPageAnalyze(CWowItemManager* pWowItemManager)
:CItemPageAnalyzeBase(pWowItemManager)
{
}

CMyBitItemPageAnalyze::~CMyBitItemPageAnalyze(void)
{
}

HRESULT CMyBitItemPageAnalyze::ParseItemPage( CComPtr<IHTMLDocument3>& spHtmlDoc, const CString& strSearchItemName )
{
	CHECK_POINTER_RETURN_VALUE_IF_FAIL(spHtmlDoc, E_POINTER);
	HRESULT hr = E_FAIL;

	LPCTSTR pszTableName[] = {
		TABLE_NAME_BIDS_ACTIVE,
		TABLE_NAME_BIDS_WON,
		TABLE_NAME_BIDS_LOST,
	};
	COM_VERIFY(_INnerParseAllTables(spHtmlDoc, pszTableName, _countof(pszTableName), strSearchItemName));

	return hr;
}

namespace BidsActive{
	enum BitItemIndex
	{
		biiTypeUrl = 1,
		biiName = 2,
		biiSeller = 5,
		biiQuantity = 6,
		biiTimeInfo = 8,

		biiPriceBidGold = 13,		//������
		biiPriceBidSilver = 14,		//��������
		biiPriceBidCopper = 15,		//����ͭ��

		biiPriceBuyoutGold = 17,	//һ�ڼ۽��
		biiPriceBuyoutSilver = 18,	//һ�ڼ�����
		biiPriceBuyoutCopper = 19,	//һ�ڼ�ͭ��

		biiMinCount,
	};
};
HRESULT CMyBitItemPageAnalyze::_InnerGetMyBidsActiveRowItemInfo(CComQIPtr<IHTMLElementCollection>& spChildElements, 
															WowItemInfoPtr& spItemInfo, 
															const CString& strSearchItemName)
{
	HRESULT hr = E_FAIL;
	long nLength = 0;
	COM_VERIFY(spChildElements->get_length(&nLength));
	FTLASSERT(nLength >= BidsActive::biiMinCount);
	if (nLength >= BidsActive::biiMinCount)
	{
		spItemInfo->SetItemStatus(isMyBidsActive);
		CString strItemName = _GetCollectionItemValue(spChildElements, BidsActive::biiName, ivtInnerText);
		if (strSearchItemName.IsEmpty() || strSearchItemName == strItemName)
		{
			//����ȫ����ǰ����Ŀ��������Ҫ��������Ŀ
			CString strUrl = _GetCollectionItemValue(spChildElements, BidsActive::biiTypeUrl, ivtToString);
			spItemInfo->SetUrl(strUrl);
			int nPosSlash = strUrl.ReverseFind(_T('/'));
			if (nPosSlash > 0)
			{
				CString strTypeId = strUrl.Mid(nPosSlash + 1);
				spItemInfo->SetTypeId(StrToLong(strTypeId));
			}

			spItemInfo->SetItemName(strItemName);
			spItemInfo->SetSeller(_GetCollectionItemValue(spChildElements, BidsActive::biiSeller, ivtInnerText));
			spItemInfo->SetQuantity(StrToLong(_GetCollectionItemValue(spChildElements, BidsActive::biiQuantity, ivtInnerText)));

			CString strTimeInfo = _GetCollectionItemValue(spChildElements, BidsActive::biiTimeInfo, ivtInnerText, TRUE);
			spItemInfo->SetItemSellTimeInfo(_ConvertTimeInfo(strTimeInfo));

			spItemInfo->SetPriceBid(_GetPriceInfo(spChildElements, BidsActive::biiPriceBidGold, 
				BidsActive::biiPriceBidSilver, BidsActive::biiPriceBidCopper));
			spItemInfo->SetPriceBuyout(_GetPriceInfo(spChildElements, BidsActive::biiPriceBuyoutGold, 
				BidsActive::biiPriceBuyoutSilver, BidsActive::biiPriceBuyoutCopper));

			//���Ÿ���ʱ�� -- ��֤ SetItemSellTimeInfo ʱ�ܻ�ȡ���ϴθ��µ�ʱ����Ϣ
			//spItemInfo->UpdateRefreshTime();
		}
	}
	else
	{
		hr = E_FAIL;
		FTLASSERT(FALSE && _T("Web Page Change"));
	}
	return hr;
}

namespace BidsWon
{
	enum BitItemIndex
	{
		biiTypeUrl = 1,
		biiName = 1,
		biiQuantity = 3,
		biiRemainDayInMail = 5,

		//�������۳��ļ۸�?
		biiAuctionSoldGold = 8,
		biiAuctionSoldSilver = 9,
		biiAuctionSoldCopper = 10,

		biiMinCount,
	};
};
HRESULT CMyBitItemPageAnalyze::_InnerGetMyBidsWonRowItemInfo(CComQIPtr<IHTMLElementCollection>& spChildElements, 
															WowItemInfoPtr& spItemInfo, 
															const CString& strSearchItemName)
{
	HRESULT hr = E_FAIL;
	long nLength = 0;

	//CFHTMLElementCollectionDumper dumper(spChildElements, CFOutputWindowInfoOutput::Instance(), 0);

	COM_VERIFY(spChildElements->get_length(&nLength));
	FTLASSERT(nLength >= BidsWon::biiMinCount);
	if (nLength >= BidsWon::biiMinCount)
	{
		spItemInfo->SetItemStatus(isMyBidsWon);

		CString strItemName = _GetCollectionItemValue(spChildElements, BidsWon::biiName, ivtInnerText, TRUE);
		if (strSearchItemName.IsEmpty() || strSearchItemName == strItemName)
		{
			//����ȫ����ǰ����Ŀ��������Ҫ��������Ŀ
			CString strUrl = _GetCollectionItemValue(spChildElements, BidsWon::biiTypeUrl, ivtToString);
			spItemInfo->SetUrl(strUrl);
			int nPosSlash = strUrl.ReverseFind(_T('/'));
			if (nPosSlash > 0)
			{
				CString strTypeId = strUrl.Mid(nPosSlash + 1);
				spItemInfo->SetTypeId(StrToLong(strTypeId));
			}

			spItemInfo->SetItemName(strItemName);
			//spItemInfo->SetSeller(_GetCollectionItemValue(spChildElements, BidsWon::biiSeller, ivtInnerText));
			spItemInfo->SetQuantity(StrToLong(_GetCollectionItemValue(spChildElements, BidsWon::biiQuantity, ivtInnerText)));

			//CString strTimeInfo = _GetCollectionItemValue(spChildElements, BidsWon::biiTimeInfo, ivtInnerText, TRUE);
			//spItemInfo->SetItemSellTimeInfo(_ConvertTimeInfo(strTimeInfo));
			spItemInfo->SetRemainDayInMail(StrToLong(_GetCollectionItemValue(spChildElements, BidsWon::biiRemainDayInMail, ivtInnerText)));

			//TODO: SetPriceBid -> SetAuctionSold
			spItemInfo->SetPriceBid(_GetPriceInfo(spChildElements, BidsWon::biiAuctionSoldGold, 
				BidsWon::biiAuctionSoldSilver, BidsWon::biiAuctionSoldCopper));
			//spItemInfo->SetPriceBuyout(_GetPriceInfo(spChildElements, BidsWon::biiPriceBuyoutGold, 
			//	BidsWon::biiPriceBuyoutSilver, BidsWon::biiPriceBuyoutCopper));

			//���Ÿ���ʱ�� -- ��֤ SetItemSellTimeInfo ʱ�ܻ�ȡ���ϴθ��µ�ʱ����Ϣ
			//spItemInfo->UpdateRefreshTime();
		}
	}
	else
	{
		hr = E_FAIL;
		FTLASSERT(FALSE && _T("Web Page Change"));
	}

	return hr;
}

namespace BidsLost
{
	enum BitItemIndex
	{
		biiTypeUrl = 1,
		biiName = 1,
		biiQuantity = 3,

		biiPriceBidGold = 8,		//������
		biiPriceBidSilver = 9,		//��������
		biiPriceBidCopper = 10,		//����ͭ��

		biiMinCount,

	};
};
HRESULT CMyBitItemPageAnalyze::_InnerGetMyBidsLostRowItemInfo(CComQIPtr<IHTMLElementCollection>& spChildElements, 
															WowItemInfoPtr& spItemInfo, 
															const CString& strSearchItemName)
{
	HRESULT hr = E_FAIL;
	long nLength = 0;
	COM_VERIFY(spChildElements->get_length(&nLength));
	FTLASSERT(nLength >= BidsLost::biiMinCount);
	if (nLength >= BidsLost::biiMinCount)
	{
		spItemInfo->SetItemStatus(isMyBidsLost);
		CString strItemName = _GetCollectionItemValue(spChildElements, BidsLost::biiName, ivtInnerText, TRUE);
		if (strSearchItemName.IsEmpty() || strSearchItemName == strItemName)
		{
			//����ȫ����ǰ����Ŀ��������Ҫ��������Ŀ
			CString strUrl = _GetCollectionItemValue(spChildElements, BidsLost::biiTypeUrl, ivtToString);
			spItemInfo->SetUrl(strUrl);
			int nPosSlash = strUrl.ReverseFind(_T('/'));
			if (nPosSlash > 0)
			{
				CString strTypeId = strUrl.Mid(nPosSlash + 1);
				spItemInfo->SetTypeId(StrToLong(strTypeId));
			}

			spItemInfo->SetItemName(strItemName);
			//spItemInfo->SetSeller(_GetCollectionItemValue(spChildElements, BidsLost::biiSeller, ivtInnerText));
			spItemInfo->SetQuantity(StrToLong(_GetCollectionItemValue(spChildElements, BidsLost::biiQuantity, ivtInnerText)));

			//CString strTimeInfo = _GetCollectionItemValue(spChildElements, BidsLost::biiTimeInfo, ivtInnerText, TRUE);
			//spItemInfo->SetItemSellTimeInfo(_ConvertTimeInfo(strTimeInfo));

			spItemInfo->SetPriceBid(_GetPriceInfo(spChildElements, BidsLost::biiPriceBidGold, 
				BidsLost::biiPriceBidSilver, BidsLost::biiPriceBidCopper));
			//spItemInfo->SetPriceBuyout(_GetPriceInfo(spChildElements, BidsLost::biiPriceBuyoutGold, 
			//	BidsLost::biiPriceBuyoutSilver, BidsLost::biiPriceBuyoutCopper));
		}
	}
	else
	{
		hr = E_FAIL;
		FTLASSERT(FALSE && _T("Web Page Change"));
	}
	return hr;
}

HRESULT CMyBitItemPageAnalyze::_GetRowItemInfo(const CString& strTableName,
											 CComQIPtr<IHTMLElementCollection>& spChildElements, 
												WowItemInfoPtr& spItemInfo, 
												const CString& strSearchItemName)
{
	HRESULT hr = E_FAIL;
	if (strTableName.Compare(TABLE_NAME_BIDS_ACTIVE) == 0)
	{
		COM_VERIFY(_InnerGetMyBidsActiveRowItemInfo(spChildElements, spItemInfo, strSearchItemName));
	}
	else if(strTableName.Compare(TABLE_NAME_BIDS_WON) == 0)
	{
		COM_VERIFY(_InnerGetMyBidsWonRowItemInfo(spChildElements, spItemInfo, strSearchItemName));
	}
	else if(strTableName.Compare(TABLE_NAME_BIDS_LOST) == 0)
	{
		COM_VERIFY(_InnerGetMyBidsLostRowItemInfo(spChildElements, spItemInfo, strSearchItemName));
	}

	return hr;
}
