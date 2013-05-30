#include "StdAfx.h"
#include "BitItemPageAnalyze.h"
#include "ftlWebInterfaceDetect.h"

//��ǰ����
#define TABLE_NAME_ACTIVE_BIDS		TEXT("bids-active")

//Ӯ������
#define TABLE_NAME_WON_BIDS			TEXT("bids-won")

//����ʧ��
#define TABLE_NAME_LOST_BIDS		TEXT("bids-lost")


namespace ActiveBids{
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

namespace WonBids
{
	enum BitItemIndex
	{
		biiTypeUrl = 1,
		

		biiMinCount,
	};
};

namespace LostBids
{
	enum BitItemIndex
	{
		biiTypeUrl = 1,
		biiName = 1,
		//biiSeller = 5,
		biiQuantity = 3,
		//biiTimeInfo = 8,

		biiPriceBidGold = 8,		//������
		biiPriceBidSilver = 9,		//��������
		biiPriceBidCopper = 10,		//����ͭ��

		//biiPriceBuyoutGold = 17,	//һ�ڼ۽��
		//biiPriceBuyoutSilver = 18,	//һ�ڼ�����
		//biiPriceBuyoutCopper = 19,	//һ�ڼ�ͭ��

		biiMinCount,

	};
};


CBitItemPageAnalyze::CBitItemPageAnalyze(CWowItemManager* pWowItemManager)
:CItemPageAnalyzeBase(pWowItemManager)
{
}

CBitItemPageAnalyze::~CBitItemPageAnalyze(void)
{
}

HRESULT CBitItemPageAnalyze::ParseItemPage( CComPtr<IHTMLDocument3>& spHtmlDoc, const CString& strSearchItemName )
{
	CHECK_POINTER_RETURN_VALUE_IF_FAIL(spHtmlDoc, E_POINTER);
	HRESULT hr = E_FAIL;

	LPCTSTR pszTableName[] = {
		//TABLE_NAME_ACTIVE_BIDS,
		//TABLE_NAME_WON_BIDS,
		TABLE_NAME_LOST_BIDS,
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

HRESULT CBitItemPageAnalyze::_InnerGetActiveBidsRowItemInfo(CComQIPtr<IHTMLElementCollection>& spChildElements, 
															WowItemInfoPtr& spItemInfo, 
															const CString& strSearchItemName)
{
	HRESULT hr = E_FAIL;
	long nLength = 0;
	COM_VERIFY(spChildElements->get_length(&nLength));
	FTLASSERT(nLength > ActiveBids::biiMinCount);
	if (nLength > ActiveBids::biiMinCount)
	{
		CString strItemName = _GetCollectionItemValue(spChildElements, ActiveBids::biiName, ivtInnerText);
		if (strSearchItemName.IsEmpty() || strSearchItemName == strItemName)
		{
			//����ȫ����ǰ����Ŀ��������Ҫ��������Ŀ
			CString strUrl = _GetCollectionItemValue(spChildElements, ActiveBids::biiTypeUrl, ivtToString);
			spItemInfo->SetUrl(strUrl);
			int nPosSlash = strUrl.ReverseFind(_T('/'));
			if (nPosSlash > 0)
			{
				CString strTypeId = strUrl.Mid(nPosSlash + 1);
				spItemInfo->SetTypeId(StrToLong(strTypeId));
			}

			spItemInfo->SetItemName(strItemName);
			spItemInfo->SetSeller(_GetCollectionItemValue(spChildElements, ActiveBids::biiSeller, ivtInnerText));
			spItemInfo->SetQuantity(StrToLong(_GetCollectionItemValue(spChildElements, ActiveBids::biiQuantity, ivtInnerText)));

			CString strTimeInfo = _GetCollectionItemValue(spChildElements, ActiveBids::biiTimeInfo, ivtInnerText, TRUE);
			spItemInfo->SetItemSellTimeInfo(_ConvertTimeInfo(strTimeInfo));

			spItemInfo->SetPriceBid(_GetPriceInfo(spChildElements, ActiveBids::biiPriceBidGold, 
				ActiveBids::biiPriceBidSilver, ActiveBids::biiPriceBidCopper));
			spItemInfo->SetPriceBuyout(_GetPriceInfo(spChildElements, ActiveBids::biiPriceBuyoutGold, 
				ActiveBids::biiPriceBuyoutSilver, ActiveBids::biiPriceBuyoutCopper));

			//���Ÿ���ʱ�� -- ��֤ SetItemSellTimeInfo ʱ�ܻ�ȡ���ϴθ��µ�ʱ����Ϣ
			spItemInfo->UpdateRefreshTime();
		}
	}
	else
	{
		hr = E_FAIL;
		FTLASSERT(FALSE && _T("Web Page Change"));
	}
	return hr;
}

HRESULT CBitItemPageAnalyze::_InnerGetWonBidsRowItemInfo(CComQIPtr<IHTMLElementCollection>& spChildElements, 
															WowItemInfoPtr& spItemInfo, 
															const CString& strSearchItemName)
{
	HRESULT hr = E_FAIL;
	long nLength = 0;

	CFHTMLElementCollectionDumper dumper(spChildElements, CFOutputWindowInfoOutput::Instance(), 0);

	COM_VERIFY(spChildElements->get_length(&nLength));
	FTLASSERT(nLength > WonBids::biiMinCount);
	if (nLength > WonBids::biiMinCount)
	{
#if 0
		CString strItemName = _GetCollectionItemValue(spChildElements, WonBids::biiName, ivtInnerText, TRUE);
		if (strSearchItemName.IsEmpty() || strSearchItemName == strItemName)
		{
			//����ȫ����ǰ����Ŀ��������Ҫ��������Ŀ
			CString strUrl = _GetCollectionItemValue(spChildElements, WonBids::biiTypeUrl, ivtToString);
			spItemInfo->SetUrl(strUrl);
			int nPosSlash = strUrl.ReverseFind(_T('/'));
			if (nPosSlash > 0)
			{
				CString strTypeId = strUrl.Mid(nPosSlash + 1);
				spItemInfo->SetTypeId(StrToLong(strTypeId));
			}

			spItemInfo->SetItemName(strItemName);
			//spItemInfo->SetSeller(_GetCollectionItemValue(spChildElements, LostBids::biiSeller, ivtInnerText));
			spItemInfo->SetQuantity(StrToLong(_GetCollectionItemValue(spChildElements, WonBids::biiQuantity, ivtInnerText)));

			//CString strTimeInfo = _GetCollectionItemValue(spChildElements, LostBids::biiTimeInfo, ivtInnerText, TRUE);
			//spItemInfo->SetItemSellTimeInfo(_ConvertTimeInfo(strTimeInfo));

			spItemInfo->SetPriceBid(_GetPriceInfo(spChildElements, WonBids::biiPriceBidGold, 
				WonBids::biiPriceBidSilver, WonBids::biiPriceBidCopper));
			//spItemInfo->SetPriceBuyout(_GetPriceInfo(spChildElements, LostBids::biiPriceBuyoutGold, 
			//	LostBids::biiPriceBuyoutSilver, LostBids::biiPriceBuyoutCopper));

			//���Ÿ���ʱ�� -- ��֤ SetItemSellTimeInfo ʱ�ܻ�ȡ���ϴθ��µ�ʱ����Ϣ
			//spItemInfo->UpdateRefreshTime();
		}
#endif
	}
	else
	{
		hr = E_FAIL;
		FTLASSERT(FALSE && _T("Web Page Change"));
	}

	return hr;
}

HRESULT CBitItemPageAnalyze::_InnerGetLostBidsRowItemInfo(CComQIPtr<IHTMLElementCollection>& spChildElements, 
															WowItemInfoPtr& spItemInfo, 
															const CString& strSearchItemName)
{
	HRESULT hr = E_FAIL;
	long nLength = 0;
	COM_VERIFY(spChildElements->get_length(&nLength));
	FTLASSERT(nLength > LostBids::biiMinCount);
	if (nLength > LostBids::biiMinCount)
	{
		CString strItemName = _GetCollectionItemValue(spChildElements, LostBids::biiName, ivtInnerText, TRUE);
		if (strSearchItemName.IsEmpty() || strSearchItemName == strItemName)
		{
			//����ȫ����ǰ����Ŀ��������Ҫ��������Ŀ
			CString strUrl = _GetCollectionItemValue(spChildElements, LostBids::biiTypeUrl, ivtToString);
			spItemInfo->SetUrl(strUrl);
			int nPosSlash = strUrl.ReverseFind(_T('/'));
			if (nPosSlash > 0)
			{
				CString strTypeId = strUrl.Mid(nPosSlash + 1);
				spItemInfo->SetTypeId(StrToLong(strTypeId));
			}

			spItemInfo->SetItemName(strItemName);
			//spItemInfo->SetSeller(_GetCollectionItemValue(spChildElements, LostBids::biiSeller, ivtInnerText));
			spItemInfo->SetQuantity(StrToLong(_GetCollectionItemValue(spChildElements, LostBids::biiQuantity, ivtInnerText)));

			//CString strTimeInfo = _GetCollectionItemValue(spChildElements, LostBids::biiTimeInfo, ivtInnerText, TRUE);
			//spItemInfo->SetItemSellTimeInfo(_ConvertTimeInfo(strTimeInfo));

			spItemInfo->SetPriceBid(_GetPriceInfo(spChildElements, LostBids::biiPriceBidGold, 
				LostBids::biiPriceBidSilver, LostBids::biiPriceBidCopper));
			//spItemInfo->SetPriceBuyout(_GetPriceInfo(spChildElements, LostBids::biiPriceBuyoutGold, 
			//	LostBids::biiPriceBuyoutSilver, LostBids::biiPriceBuyoutCopper));

			//���Ÿ���ʱ�� -- ��֤ SetItemSellTimeInfo ʱ�ܻ�ȡ���ϴθ��µ�ʱ����Ϣ
			spItemInfo->UpdateRefreshTime();
		}
	}
	else
	{
		hr = E_FAIL;
		FTLASSERT(FALSE && _T("Web Page Change"));
	}
	return hr;
}

HRESULT CBitItemPageAnalyze::_GetRowItemInfo(const CString& strTableName,
											 CComQIPtr<IHTMLElementCollection>& spChildElements, 
												WowItemInfoPtr& spItemInfo, 
												const CString& strSearchItemName)
{
	HRESULT hr = E_FAIL;
	long nLength = 0;
	COM_VERIFY(spChildElements->get_length(&nLength));
	if (strTableName.Compare(TABLE_NAME_ACTIVE_BIDS) == 0)
	{
		COM_VERIFY(_InnerGetActiveBidsRowItemInfo(spChildElements, spItemInfo, strSearchItemName));
	}
	else if(strTableName.Compare(TABLE_NAME_WON_BIDS) == 0)
	{
		COM_VERIFY(_InnerGetWonBidsRowItemInfo(spChildElements, spItemInfo, strSearchItemName));
	}
	else if(strTableName.Compare(TABLE_NAME_LOST_BIDS) == 0)
	{
		CFHTMLElementCollectionDumper collDumper(spChildElements, CFOutputWindowInfoOutput::Instance(), 0);
		COM_VERIFY(_InnerGetLostBidsRowItemInfo(spChildElements, spItemInfo, strSearchItemName));
	}



	return hr;
}
