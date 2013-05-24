#pragma once

#include "WowAHDefine.h"

class CItemPageAnalyzeBase
{
	enum ItemIndex
	{
		iiTypeUrl = 1,
		iiName = 2,
		iiSeller = 5,
		iiQuantity = 7,
		iiTimeInfo = 14,

		iiPriceBidGold = 17,		//������
		iiPriceBidSilver = 18,		//��������
		iiPriceBidCopper = 19,		//����ͭ��
		
		iiPriceBuyoutGold = 21,		//һ�ڼ۽��
		iiPriceBuyoutSilver = 22,	//һ�ڼ�����
		iiPriceBuyoutCopper = 23,	//һ�ڼ�ͭ��

		iiMinCount,
	};
	
	enum ItemValueType
	{
		ivtInnerText,	
		ivtToString,

	};
public:
	CItemPageAnalyzeBase(void);
	~CItemPageAnalyzeBase(void);

	virtual HRESULT	ParseItemPage(CComPtr<IHTMLDocument3>& spHtmlDoc);
	HRESULT _InnerParseTable(CComPtr<IHTMLTable>& spTable);

	//����ÿһ�еļ۸�
	HRESULT _InnerParsePriceRowItem(long nIndex, CComQIPtr<IHTMLElement>& spPriceElementItem);
	
	ItemInAHInfoPtr	_GetItemInAnInfo(const CString& strIdInfo);
	CString _GetCollectionItemValue(CComQIPtr<IHTMLElementCollection>& spChildElements, INT index, ItemValueType valueType);
	HRESULT _GetPriceRowItemInfo(CComQIPtr<IHTMLElementCollection>& spChildElements, ItemInAHInfoPtr& spItemInfo);

	ItemSellTimeInfo _ConvertTimeInfo(const CString& strTimeInfo);
	LONG _GetPriceInfo(CComQIPtr<IHTMLElementCollection>& spChildElements, INT nGoldIndex, INT nSilverIndex, INT nCopperIndex);
protected:
	ItemInAHInfoContainer	m_Items;
};

