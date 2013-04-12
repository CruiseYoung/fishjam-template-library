#ifndef FTL_BUFFER_H
#define FTL_BUFFER_H

#pragma once

#ifndef FTL_BASE_H
#  error ftlBuffer.h requires ftlbase.h to be included first
#endif

namespace FTL
{
	/*****************************************************************************************************
	* 
	*****************************************************************************************************/

	//����Ķ�д������� -- ���� CInternetFile::Read/Write ��д����(ԭ�д�����Bug�������˸���)
	template <typename T>
	class CFRWBufferT 
	{
	public:
		//T* pT = static_cast<T*>(this);
		FTLINLINE CFRWBufferT();
		FTLINLINE virtual ~CFRWBufferT();

		FTLINLINE BOOL SetReadBufferSize(LONG nReadSize);
		FTLINLINE BOOL SetWriteBufferSize(LONG nWriteSize);

		//Ϊ�˷�ֹ���Ƴ�ͻ�����������֣���ʹ��̫������ Read/Write/Flush ��
		FTLINLINE BOOL ReadFromBuffer(PBYTE pBuffer, LONG nCount, LONG* pRead);
		FTLINLINE BOOL WriteToBuffer(const PBYTE pBuffer, LONG nCount, LONG* pWrite);
		FTLINLINE BOOL FlushFromBuffer(LONG* pWrite);
	protected:
		//BOOL ReadReal(PBYTE pBuffer, LONG nCount, LONG* pWrite) = 0;
		//BOOL WriteReal(const PBYTE pBuffer, LONG nCount, LONG* pWrite) = 0;
	protected:
		//˵����Read �� Write ��һ�� m_nReadBufferBytes(���������ָ Buffer ����Ч���ݵĽ�β) -- ���ʵ���߼���Write��һ��, MS ��������Bug
		LONG	m_nWriteBufferSize;
		LONG	m_nWriteBufferPos;
		LPBYTE	m_pbWriteBuffer;

		LONG	m_nReadBufferSize;
		LONG	m_nReadBufferPos;
		LONG	m_nReadBufferBytes;
		LPBYTE	m_pbReadBuffer;

	};
}

#endif //FTL_BUFFER_H

#ifndef USE_EXPORT
#  include "ftlBuffer.hpp"
#endif