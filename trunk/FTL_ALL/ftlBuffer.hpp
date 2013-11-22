#ifndef FTL_BUFFER_HPP
#define FTL_BUFFER_HPP
#pragma once

#ifdef USE_EXPORT
#  include "ftlBuffer.h"
#endif

namespace FTL
{
	template <typename T>
	CFRWBufferT<T>::CFRWBufferT()
	{
		m_pbWriteBuffer = NULL;
		m_bWriteBufferIsAttached = FALSE;
		m_nWriteBufferSize = 0;
		m_nWriteBufferPos = 0;
		
		m_pbReadBuffer = NULL;
		m_bReadBufferIsAttached = FALSE;
		m_nReadBufferSize = 0;
		m_nReadBufferPos = 0;
		m_nReadBufferBytes = 0;
	}

	template <typename T>
	CFRWBufferT<T>::~CFRWBufferT()
	{
		if(!m_bWriteBufferIsAttached)
		{
			SAFE_DELETE_ARRAY(m_pbWriteBuffer);
		}
		if(!m_bReadBufferIsAttached)
		{
			SAFE_DELETE_ARRAY(m_pbReadBuffer);
		}
	}

	template <typename T>
	BOOL CFRWBufferT<T>::Attach(PBYTE pReadBuffer, LONG nReadBufferSize, PBYTE pWriteBuffer, LONG nWriteBufferSize)
	{
		BOOL bRet = TRUE;
		if(pReadBuffer && nReadBufferSize > 0)
		{
			if(!m_bReadBufferIsAttached)
			{
				API_VERIFY(SetReadBufferSize(0));
			}
			if(bRet)
			{
                LONG nDataSize = m_nReadBufferBytes - m_nReadBufferPos;
                if (m_pbReadBuffer && nDataSize > 0)
                {
                    //TODO: copy old data
                    //Checked::memcpy_s(m_pbReadBuffer, nReadSize, pbTemp + m_nReadBufferPos, nDataSize);
                }
				m_pbReadBuffer = pReadBuffer;
				m_nReadBufferSize = nReadBufferSize;
				m_bReadBufferIsAttached = TRUE;
			}
		}
		
		if(pWriteBuffer && nWriteBufferSize > 0)
		{
			if(!m_bWriteBufferIsAttached)
			{
				API_VERIFY(SetWriteBufferSize(0));
			}
            //TODO: Copy old data
			m_pbWriteBuffer = pWriteBuffer;
			m_nWriteBufferSize = nWriteBufferSize;
			m_bWriteBufferIsAttached = TRUE;
		}
		
		return bRet;
	}
	template <typename T>
    BOOL  CFRWBufferT<T>::Detach(PBYTE* ppReadBuffer, LONG* pReadBufferSize, PBYTE* ppWriteBuffer, LONG* pWriteBufferSize)
	{
		BOOL bRet = TRUE;
		
		return bRet;
	}


	template <typename T>
	BOOL CFRWBufferT<T>::SetReadBufferSize(LONG nReadSize)
	{
		BOOL bRet = TRUE;
        FTLASSERT(!m_bReadBufferIsAttached);

		//MS Bug: http://support.microsoft.com/kb/154895/en-us, ���Ը��ĵ�,  (�°汾�Ѿ�û����?)

		if (nReadSize != m_nReadBufferSize)
		{
			//����û�б���ȡ�����ݴ�С
			LONG nDataSize = m_nReadBufferBytes - m_nReadBufferPos;
			FTLASSERT(nDataSize >= 0);

			if (nReadSize < nDataSize ) //m_dwReadBufferPos )
			{
				//���д������õĻ����С������û�б���ȡ��û�а취����
				bRet = FALSE;
			}
			else
			{
				if (nReadSize == 0)
				{
					//����ɾ����ȡBuffer
					SAFE_DELETE_ARRAY(m_pbReadBuffer);
					m_nReadBufferPos = 0;
					m_nReadBufferBytes = 0;
				}
				else if (m_pbReadBuffer == NULL)
				{
					m_pbReadBuffer = new BYTE[nReadSize];
					m_nReadBufferPos = 0; //TODO: VC2008����CInternetFile::SetReadBufferSize ʵ���� dwReadSize, ������Bug
					m_nReadBufferBytes = 0;
				}
				else
				{
					//�����µ�Buffer -- �ȱ���ԭ����ָ�룬�����µ�Buffer�󿽱�����
					LPBYTE pbTemp = m_pbReadBuffer;
					m_pbReadBuffer = new BYTE[nReadSize];

					//ԭ����Buffer�������ݣ��������µ�Buffer�У�
					if (nDataSize > 0) // && nDataSize <= nReadSize) //�˴������ٺ� nReadSize �Ƚ� 
					{
						//MS ������Bug�������� m_nReadBufferBytes ��ֵ����
						Checked::memcpy_s(m_pbReadBuffer, nReadSize, pbTemp + m_nReadBufferPos, nDataSize);
						m_nReadBufferPos = 0;
						m_nReadBufferBytes = nDataSize;
					}
					else
					{
						m_nReadBufferBytes = 0;
						m_nReadBufferPos = 0;//dwReadSize;
					}
					delete [] pbTemp;
				}

				m_nReadBufferSize = nReadSize;
			}
		}
		return bRet;
	}

	template <typename T>
	BOOL CFRWBufferT<T>::SetWriteBufferSize(LONG nWriteSize)
	{
		BOOL bRet = TRUE;
        FTLASSERT(!m_bWriteBufferIsAttached);

		if (nWriteSize != m_nWriteBufferSize)
		{
			if (m_nWriteBufferPos > nWriteSize)
			{
				//Buffer���������ݱ��µ�Buffer��
				API_VERIFY(FlushFromBuffer(NULL));
				if (!bRet)
				{
					return bRet;
				}
			}
			if (nWriteSize == 0)
			{
				SAFE_DELETE_ARRAY(m_pbWriteBuffer);
				m_nWriteBufferPos = 0;
			}
			else if (m_pbWriteBuffer == NULL)
			{
				m_pbWriteBuffer = new BYTE[nWriteSize];
				m_nWriteBufferPos = 0;
			}
			else
			{
				//������ǰ������
				LPBYTE pbTemp = m_pbWriteBuffer;
				m_pbWriteBuffer = new BYTE[nWriteSize];
				FTLASSERT(m_nWriteBufferPos <= nWriteSize);

				Checked::memcpy_s(m_pbWriteBuffer, nWriteSize, pbTemp, m_nWriteBufferPos);
				delete [] pbTemp;
			}

			m_nWriteBufferSize = nWriteSize;
		}

		return bRet;
	}

	template <typename T>
	BOOL CFRWBufferT<T>::ReadFromBuffer(PBYTE pBuffer, LONG nCount, LONG* pRead)
	{
		LONG nBytes = 0;
		BOOL bRet = TRUE;

		T* pT = static_cast<T*>(this);

		if (m_pbReadBuffer == NULL)
		{
			//û�л��壬ֱ�Ӵ���
			API_VERIFY(pT->ReadReal(pBuffer, nCount, &nBytes));
			//API_VERIFY(InternetReadFile(m_hRequest, (LPVOID) pBuffer, nCount, &nBytes));
			if (bRet && pRead)
			{
				*pRead = nBytes;
			}
			return bRet;
		}

		//�������Buffer���ڻ���Buffer
		if (nCount >= m_nReadBufferSize)
		{
			//�ȿ��������е�����
			LONG nDataSize = m_nReadBufferBytes - m_nReadBufferPos;
			FTLASSERT(nDataSize >= 0);
			if(nDataSize > 0)
			{
				Checked::memcpy_s(pBuffer, nCount, m_pbReadBuffer + m_nReadBufferPos, nDataSize);
				//��ʱ��Ч�����Ѿ�ȫ����ȡ��
			}
			m_nReadBufferPos = 0;
			m_nReadBufferBytes = 0;

			//�����������ȡ����
			API_VERIFY(pT->ReadReal(pBuffer + nDataSize, nCount - nDataSize, &nBytes));
			//API_VERIFY(InternetReadFile(m_hRequest, pBuffer + nDataSize, nCount - nDataSize, &nBytes));
			if (!bRet)
			{
				return bRet;
			}
			nBytes += nDataSize;
		}
		else  //����BufferС�ڻ���Buffer�����ȴӻ���Buffer�ж�ȡ
		{
			//�����Buffer��ʣ������ݱ������������ -- �Ƚ���Buffer�е�ȫ�����ݶ���������
			LONG nDataSize = m_nReadBufferBytes - m_nReadBufferPos;
			if (nDataSize <= nCount)  //			if (m_nReadBufferPos + nCount >= m_nReadBufferBytes)
			{
				if (nDataSize > 0)
				{
					Checked::memcpy_s(pBuffer, nCount, m_pbReadBuffer + m_nReadBufferPos, nDataSize);
				}
				m_nReadBufferPos = 0;
				m_nReadBufferBytes = 0;

				//�����������ȡ���� -- ���ֱ�Ӷ�ȡ������Buffer�У�������Ϊ��Buffer�е���Ч�����Ѿ�ȫ�����꣬���ͷ��ʼ����
				API_VERIFY(pT->ReadReal(m_pbReadBuffer, m_nReadBufferSize, &nBytes));
				//API_VERIFY(InternetReadFile(m_hRequest, m_pbReadBuffer, m_nReadBufferSize, &nBytes));
				if (bRet)
				{
					//���Ӹն�ȡ�������ݵ�����Buffer
					LONG nMoveSize = FTL_MIN(nCount - nDataSize, nBytes); 
					Checked::memcpy_s(pBuffer + nDataSize, nCount - nDataSize, 
						m_pbReadBuffer, nMoveSize);
					m_nReadBufferPos = nMoveSize;
					m_nReadBufferBytes = nBytes;

					nBytes = (nMoveSize + nDataSize);
				}
			}
			else
			{
				//�����Buffer��ʣ������ݱ���������ݶ࣬�򱾴�ֻ���ڴ��п���
				Checked::memcpy_s(pBuffer, nCount, m_pbReadBuffer + m_nReadBufferPos, nCount);
				m_nReadBufferPos += nCount;
				nBytes = nCount;
			}
		}
		if (pRead)
		{
			*pRead = nBytes;
		}
		return bRet;
	}

	template<typename T>
	BOOL CFRWBufferT<T>::WriteToBuffer(const PBYTE pBuffer, LONG nCount, LONG* pWrite)
	{
		BOOL bRet = TRUE;
		LONG nBytes = 0;
		T* pT = static_cast<T*>(this);

		if (m_pbWriteBuffer == NULL)
		{
			API_VERIFY(pT->WriteReal(pBuffer, nCount, &nBytes));
			//û�л�������ֱ�ӷ���
			//API_VERIFY(InternetWriteFile(m_hRequest, pBuffer, nCount, &nBytes));
			if (!bRet)
			{
				return bRet;
			}
			FTLASSERT(nCount == nBytes);
		}
		else
		{
			//ʹ�û�����
			if ((m_nWriteBufferPos + nCount) > m_nWriteBufferSize && m_nWriteBufferPos > 0)
			{
				//��� �ɵ����ݳ��� + �µ����ݳ��� �� �������Ĵ�С�������ϰѾɵ����ݷ���ȥ��������Pos
				API_VERIFY(pT->WriteReal(m_pbWriteBuffer,	m_nWriteBufferPos, &nBytes));
				//API_VERIFY(InternetWriteFile( m_hRequest, m_pbWriteBuffer,	m_nWriteBufferPos, &dwBytes));
				if (!bRet)
				{
					return bRet;
				}
				//FTLASSERT(nCount == dwBytes);
				m_nWriteBufferPos = 0;
			}

			//�����Ҫ���͵����ݱȻ������Ļ�����ֱ�ӷ��ͣ�������л���
			if (nCount >= m_nWriteBufferSize)
			{
				API_VERIFY(pT->WriteReal(pBuffer, nCount, &nBytes));
				//API_VERIFY(InternetWriteFile(m_hRequest, (LPVOID) pBuffer, nCount, &dwBytes));
			}
			else
			{
				FTLASSERT(m_nWriteBufferPos + nCount <= m_nWriteBufferSize);
				//if (m_nWriteBufferPos + nCount < m_nWriteBufferSize)
				Checked::memcpy_s(m_pbWriteBuffer + m_nWriteBufferPos, 
					m_nWriteBufferSize - m_nWriteBufferPos, pBuffer, nCount);
				m_nWriteBufferPos += nCount;
			}
		}

		if (pWrite)
		{
			*pWrite = nBytes;
		}
		return bRet;
	}

	template <typename T>
	BOOL CFRWBufferT<T>::FlushFromBuffer(LONG* pWrite)
	{
		BOOL bRet = TRUE;
		if (m_pbWriteBuffer != NULL && m_nWriteBufferPos > 0)
		{
			T* pT = static_cast<T*>(this);

			LONG nBytes = 0;
			API_VERIFY(pT->WriteReal(m_pbWriteBuffer, m_nWriteBufferPos, &nBytes));
			//API_VERIFY(InternetWriteFile(m_hRequest, m_pbWriteBuffer, m_nWriteBufferPos, &dwBytes));
			if (!bRet)
			{
				return bRet;
			}
			//FTLASSERT( dwBytes == m_nWriteBufferPos);
			m_nWriteBufferPos = 0;

			if (pWrite)
			{
				*pWrite = nBytes;
			}
		}
		return bRet;
	}
}

#endif //FTL_BUFFER_HPP