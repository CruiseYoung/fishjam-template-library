#pragma once

class CUserInfoManager : public FTL::CFSingletonT<CUserInfoManager>
{
public:
	CUserInfoManager(void);
	~CUserInfoManager(void);

public:
	const CString& GetUserName() const { return m_strUserName; }
	void SetUserName(CString val) { m_strUserName = val; }
	LONG GetCurrentMoney() const { return m_nCurrentMoney; }
	void SetCurrentMoney(LONG val) { m_nCurrentMoney = val; }

private:
	CString		m_strUserName;				//�û���
	LONG		m_nCurrentMoney;			//��ǰ�Ľ�Ǯ��
};
