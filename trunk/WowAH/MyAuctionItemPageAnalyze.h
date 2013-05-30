#pragma once

#include "ItemPageAnalyzeBase.h"

//�ҵ����� -- MyAuction.xht
class CMyAuctionItemPageAnalyze : public CItemPageAnalyzeBase
{
public:
	CMyAuctionItemPageAnalyze(CWowItemManager* pWowItemManager);
	~CMyAuctionItemPageAnalyze(void);

	//����
	virtual HRESULT	ParseItemPage(CComPtr<IHTMLDocument3>& spHtmlDoc, const CString& strSearchItemName);

	//����ÿһ�еļ۸�
	HRESULT _GetRowItemInfo(const CString& strTableName, CComQIPtr<IHTMLElementCollection>& spChildElements, WowItemInfoPtr& spItemInfo, const CString& strSearchItemName);

private:

	HRESULT _InnerGetAuctionsActiveRowItemInfo(CComQIPtr<IHTMLElementCollection>& spChildElements, 
		WowItemInfoPtr& spItemInfo, const CString& strSearchItemName);
	HRESULT _InnerGetAuctionsSoldRowItemInfo(CComQIPtr<IHTMLElementCollection>& spChildElements, 
		WowItemInfoPtr& spItemInfo, const CString& strSearchItemName);
	HRESULT _InnerGetAuctionsEndedRowItemInfo(CComQIPtr<IHTMLElementCollection>& spChildElements, 
		WowItemInfoPtr& spItemInfo, const CString& strSearchItemName);

};
