#include "StdAfx.h"
#include "CreateMyAuctionPageAnalyze.h"
#include <ftlWebInterfaceDetect.h>

//#define TABLE_NAME_ITEM_FILTERS		_TEXT("item-filters")
#define TABLE_NAME_INVENTORY_0		_TEXT("inventory-0")


namespace Inventory_0{
	enum BitItemIndex
	{
		biiTypeUrl = 1,
		biiName = 2,
		//biiSeller = 5,
		//biiQuantity = 6,
		//biiTimeInfo = 8,

		//biiPriceBidGold = 13,		//������
		//biiPriceBidSilver = 14,		//��������
		//biiPriceBidCopper = 15,		//����ͭ��

		//biiPriceBuyoutGold = 17,	//һ�ڼ۽��
		//biiPriceBuyoutSilver = 18,	//һ�ڼ�����
		//biiPriceBuyoutCopper = 19,	//һ�ڼ�ͭ��

		biiMinCount,
	};
};

CCreateMyAuctionPageAnalyze::CCreateMyAuctionPageAnalyze(CWowItemManager* pWowItemManager)
	:CItemPageAnalyzeBase(pWowItemManager)
{
}

CCreateMyAuctionPageAnalyze::~CCreateMyAuctionPageAnalyze(void)
{
}

HRESULT	CCreateMyAuctionPageAnalyze::ParseItemPage(CComPtr<IHTMLDocument3>& spHtmlDoc, 
											const CString& strSearchItemName)
{
	CHECK_POINTER_RETURN_VALUE_IF_FAIL(spHtmlDoc, E_POINTER);
	HRESULT hr = E_FAIL;

	LPCTSTR pszTableName[] = {
		TABLE_NAME_INVENTORY_0,
	};

	for (int i = 0; i < _countof(pszTableName); ++i)
	{
		CComPtr<IHTMLElement> spBidsElement;
		COM_VERIFY(spHtmlDoc->getElementById(CComBSTR(pszTableName[i]), &spBidsElement));
		FTLASSERT(spBidsElement);
		if (spBidsElement)
		{
			CComPtr<IHTMLElement> spElementBids;
			COM_VERIFY(CFWebBrowserUtil::FindChildElement(spBidsElement, femByTag, TEXT("table"), 
				0, rwBreadthFirst, &spElementBids));

			CComQIPtr<IHTMLTable>	spTableBids = spElementBids;
			FTLASSERT(spTableBids);
			if (spTableBids)
			{
				COM_VERIFY(_InnerParseTable(spTableBids, pszTableName[i], strSearchItemName));
			}
		}

	}

	return hr;
}

HRESULT CCreateMyAuctionPageAnalyze::_GetRowItemInfo(const CString& strTableName, CComQIPtr<IHTMLElementCollection>& spChildElements, 
												   WowItemInfoPtr& spItemInfo, 
												   const CString& strSearchItemName)
{
	HRESULT hr = E_FAIL;
	if (strTableName.Compare(TABLE_NAME_INVENTORY_0) == 0)
	{
		COM_VERIFY(_InnerGetInventory_0_RowItemInfo(spChildElements, spItemInfo, strSearchItemName));
	}
	else
	{
		FTLASSERT(FALSE);
	}
	return hr;
}

HRESULT CCreateMyAuctionPageAnalyze::_InnerGetInventory_0_RowItemInfo(CComQIPtr<IHTMLElementCollection>& spChildElements, 
															WowItemInfoPtr& spItemInfo, 
															const CString& strSearchItemName)
{
	HRESULT hr = E_FAIL;
	long nLength = 0;
	CFHTMLElementCollectionDumper	dumper(spChildElements, CFOutputWindowInfoOutput::Instance(), 0);

	COM_VERIFY(spChildElements->get_length(&nLength));
	FTLASSERT(nLength > Inventory_0::biiMinCount);
	if (nLength > Inventory_0::biiMinCount)
	{
		//CString strItemName = _GetCollectionItemValue(spChildElements, LostBids::biiName, ivtInnerText, TRUE);
		//if (strSearchItemName.IsEmpty() || strSearchItemName == strItemName)
		//{
		//	//����ȫ����ǰ����Ŀ��������Ҫ��������Ŀ
		//	CString strUrl = _GetCollectionItemValue(spChildElements, LostBids::biiTypeUrl, ivtToString);
		//	spItemInfo->SetUrl(strUrl);
		//	int nPosSlash = strUrl.ReverseFind(_T('/'));
		//	if (nPosSlash > 0)
		//	{
		//		CString strTypeId = strUrl.Mid(nPosSlash + 1);
		//		spItemInfo->SetTypeId(StrToLong(strTypeId));
		//	}

		//	spItemInfo->SetItemName(strItemName);
		//	//spItemInfo->SetSeller(_GetCollectionItemValue(spChildElements, LostBids::biiSeller, ivtInnerText));
		//	spItemInfo->SetQuantity(StrToLong(_GetCollectionItemValue(spChildElements, LostBids::biiQuantity, ivtInnerText)));

		//	//CString strTimeInfo = _GetCollectionItemValue(spChildElements, LostBids::biiTimeInfo, ivtInnerText, TRUE);
		//	//spItemInfo->SetItemSellTimeInfo(_ConvertTimeInfo(strTimeInfo));

		//	spItemInfo->SetPriceBid(_GetPriceInfo(spChildElements, LostBids::biiPriceBidGold, 
		//		LostBids::biiPriceBidSilver, LostBids::biiPriceBidCopper));
		//	//spItemInfo->SetPriceBuyout(_GetPriceInfo(spChildElements, LostBids::biiPriceBuyoutGold, 
		//	//	LostBids::biiPriceBuyoutSilver, LostBids::biiPriceBuyoutCopper));

		//	//���Ÿ���ʱ�� -- ��֤ SetItemSellTimeInfo ʱ�ܻ�ȡ���ϴθ��µ�ʱ����Ϣ
		//	spItemInfo->UpdateRefreshTime();
		//}
	}
	else
	{
		hr = E_FAIL;
		FTLASSERT(FALSE && _T("Web Page Change"));
	}

	return hr;
}