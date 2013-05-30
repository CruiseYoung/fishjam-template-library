#pragma once

#include "ItemPageAnalyzeBase.h"

//��ʼ���� -- CreateMyAuction.xht
class CCreateMyAuctionPageAnalyze : public CItemPageAnalyzeBase
{
	enum CreateMyAuctionItemIndex
	{
		//siiTypeUrl = 1,
		//siiName = 2,
		//siiSeller = 5,
		//siiQuantity = 7,
		//siiTimeInfo = 14,

		//siiPriceBidGold = 17,		//������
		//siiPriceBidSilver = 18,		//��������
		//siiPriceBidCopper = 19,		//����ͭ��

		//siiPriceBuyoutGold = 21,		//һ�ڼ۽��
		//siiPriceBuyoutSilver = 22,	//һ�ڼ�����
		//siiPriceBuyoutCopper = 23,	//һ�ڼ�ͭ��

		siiMinCount,
	};
public:
	CCreateMyAuctionPageAnalyze(CWowItemManager* pWowItemManager);
	~CCreateMyAuctionPageAnalyze(void);

	//����
	virtual HRESULT	ParseItemPage(CComPtr<IHTMLDocument3>& spHtmlDoc, const CString& strSearchItemName);

	//����ÿһ�еļ۸�
	HRESULT _GetRowItemInfo(const CString& strTableName, CComQIPtr<IHTMLElementCollection>& spChildElements, WowItemInfoPtr& spItemInfo, const CString& strSearchItemName);

private:
	HRESULT _InnerGetInventory_0_RowItemInfo(CComQIPtr<IHTMLElementCollection>& spChildElements, 
		WowItemInfoPtr& spItemInfo, const CString& strSearchItemName);

};
