#pragma once

#include "WowAHDefine.h"
#include <ftlThread.h>

class CWowItemManager
{
public:
	//id -> WowItemInfoPtr
	typedef std::map<LONG, WowItemInfoPtr>		ItemInAHInfoContainer;

	CWowItemManager(void);
	~CWowItemManager(void);

	WowItemInfoPtr	GetItemInAHInfo(const CString& strAuctionIdInfo);

	//Ŀǰ��ʵ�ֲ������� -- �Ժ���ܻ���չ��ֻ����ض�����
	VOID			ClearItems(ItemStatus	itemStatus);

	//����������ʧ��?
	VOID			RemoveItemInfo(LONG nAuctionId);
	VOID			DumpAllItemInfo();
private:
	ItemInAHInfoContainer		m_allWowItems;
	FTL::CFCriticalSection		m_csLock;
};
