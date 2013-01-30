#ifndef FTL_FILE_H
#define FTL_FILE_H
#pragma once

#ifndef FTL_BASE_H
#  error ftlfile.h requires ftlbase.h to be included first
#endif

namespace FTL
{
//Class
    class CFStructuredStorageFile;
//Function

}//namespace FTL

#include <WinIoctl.h>
#include "ftlsystem.h"

namespace FTL
{
    #define BYTES_PER_KILOBYTE      (1024)
    #define BYTES_PER_MEGABYTE      (BYTES_PER_KILOBYTE * 1000)
    #define BYTES_PER_GIGABYTE      (BYTES_PER_MEGABYTE * 1000)
    #define BYTES_PER_TERABYTE      (BYTES_PER_GIGABYTE * 1000)
    #define BYTES_PER_PETABYTE      (BYTES_PER_TERABYTE * 1000)

	/**************************************************************************************************
	* �ı��ļ���
	*   ANSI[û�б�ʶ�ַ�] -- ���õ�ǰ���Ա��뱣��(������936, ����949)���ļ�ͷû�б�ʶ�����ڲ�ͬ���Ե�OS�»���ʾ�����롣
	*   UTF-8 without signature[65001] -- UTF8 ���룬�ļ�ͷû�б�ʶ��(���±���֧��)
	*   UTF-8 with signature[65001, 0xEFBBBF] -- UTF8���룬�ļ�ͷ�б�ʶ����Ӣ�Ķ�Ӧ���� 0xXX
	*   Unicode[1200, 0xFFFE] -- Unicode ���룬Ӣ�Ķ�Ӧ���� 0xXX00
	*   Unicode Big-Endian[1201, 0xFEFF] -- Unicode ���룬Ӣ�Ķ�Ӧ���� 0x00XX
	*   UTF-7[65000] -- UTF7���룿�ļ�ͷû�б�ʶ��
	**************************************************************************************************/
	enum TextFileEncoding
	{
		tfeError = -1,
		tfeUnknown = 0,

		tfeUTF8,					//0xEF BB BF
		tfeUnicode,					//0xFF FE
		tfeUnicodeBigEndian,		//0xFE FF
	}
	const BYTE TEXT_FILE_HEADER_UTF8[]				= { 0xEF, 0xBB, 0xBF };
	const BYTE TEXT_FILE_HEADER_UNICODE[]			= { 0xFF, 0xFE };
	const BYTE TEXT_FILE_HEADER_UNICODE_BIG_ENDIAN	= { 0xFE, 0xFF };

    /**************************************************************************************************
	* OVERLAPPED -- �첽I/O�Ǵ��������ܿ�������Ӧ�ó�����ؾ�����Ϊ���������̴߳������Բ�ͬ�ͻ���������
	*   ע�⣺
	*   1.�������������� I/O ������߳�
	*     Windows ���첽I/O����涨��һ�����ƣ�������߳̽�һ���첽 I/O �����͸��豸��������Ȼ����ֹ���У�
	*     ��ô�� I/O ����ͻᶪʧ��������I/O�������н���ʱ��û���̵߳õ����֪ͨ��
	*
    * �����ڴ��Ϊ����(ע����̡߳����̷���ʱ��ͬ��)��
    *   1.���ع����ڴ� -- linux ���� shmget, Windows���� �ڴ�ӳ���ļ�
	*     ATL ͨ�� CAtlFileMapping ���ļ�ӳ������˷�װ
    *   2.�ֲ�ʽ�����ڴ�(DSM) -- ����������չ�������ڴ�ĸ��ͨ��ȫ��/�����ڴ��е����ݽ���͸���Ľ��̼�ͨ�š�
    *     ���̨�������Ⱥ��(cluster)Ϊһ���߼�ϵͳ��ϵͳ�е��ڴ���Ⱥ���й���Ӧ�ó���ͨ�������ڴ�ͨ�š�
    *  
    * CreateFile ���������򿪸�ʽ��������Դ�������ļ�(Ӳ�̣����̣����̻�����)�����ںͲ��ڣ�Named pipes��console�ȣ�
    *   ������ dwFlagsAndAttributes ���������� FILE_FLAG_OVERLAPPED ���ԡ�
    *   OVERLAPPED�ṹִ���������ܣ�
    *     1.ʶ��ÿһ��Ŀǰ���ڽ��е�overlapped������
    *     2.�ڴ����ϵͳ֮���ṩһ���������򣬲��������ڸ�������˫�򴫵ݡ�
    *   GetOverlappedResult
    * 
    *   ��Ȼ��Ҫ��һ��overlapped������������һ������overlapped����������Ѿ����Ž�cache�У������ϵͳ��Ϊ�����Ժܿ��
    *   ȡ���Ƿ����ݣ���ô�ļ������ͻ���ReadFile��WriteFile����֮ǰ��ɣ�������������������TRUE����������£��ļ�handle
    *   ���ڼ���״̬�������ļ��Ĳ����ɱ���Ϊoverlapedһ�������⣬�����Ҫ��һ���ļ�����Ϊoverlapped��������ϵͳ��
    *   ������������󡱷ŵ������еȴ�ִ�У���ôReadFile��WriteFile���᷵��FALSE����ʱ��������GetLastError��ȷ��
    *   ������ERROR_IO_PENDING������ζ�š�overlapped I/O���󡱱��Ž������еȴ�ִ��
    * 
    * Console �ļ�
    *   m_hOutput = GetStdHandle (STD_OUTPUT_HANDLE);
    *   if (m_hOutput == INVALID_HANDLE_VALUE) {
    *     API_VERIFY(AllocConsole ());
    *     m_hOutput = GetStdHandle (STD_OUTPUT_HANDLE);
    *   }
    *   SetConsoleTitle (TEXT("ActiveX Debug Output"));  
    *   
    * ͨ���ļ������ȡ�ļ���Ϣ GetFileInformationByHandle
	*
	* CreateFile ʱ����� FILE_FLAG_SEQUENTIAL_SCAN�� ��ʾ�Ż�Cache�������ڴ�ͷ��β˳����ʣ����������
	* GetTempFileName -- ����ָ��Ŀ¼����ʱ�ļ������֣�ͨ������������ʱ�ļ�
    **************************************************************************************************/
	class CFFileUtil
	{
	public:
		FTLINLINE static TextFileEncoding GetTextFileEncoding(LPCTSTR pszFileName);
	}

#if 0
    class CFConsoleFile
    {
    public:
        FTLINLINE CFConsoleFile();
        virtual ~CFConsoleFile();
    };
#endif 

    class CFFile
    {
	public:
		enum SeekPosition { begin = 0x0, current = 0x1, end = 0x2 };
		// Constructors, destructor
		FTLINLINE CFFile();
		FTLINLINE CFFile(HANDLE hFile);
		FTLINLINE virtual ~CFFile();

		// Attributes
		HANDLE m_hFile;

		// Operations
		FTLINLINE virtual DWORD GetPosition() const;
		FTLINLINE virtual CString GetFileName() const;
		FTLINLINE virtual CString GetFilePath() const;
		FTLINLINE virtual BOOL SetFilePath(CString strNewName);

		FTLINLINE virtual BOOL Open(LPCTSTR pszFileName, 
			DWORD dwAccess = GENERIC_WRITE | GENERIC_READ,
			DWORD dwShareMode = FILE_SHARE_READ | FILE_SHARE_WRITE,
			LPSECURITY_ATTRIBUTES lpSA = NULL,
			DWORD dwCreationDisposition = CREATE_ALWAYS,
			DWORD dwAttributes = FILE_ATTRIBUTE_NORMAL,
			HANDLE hTemplateFile = NULL
			);

		FTLINLINE static BOOL Rename(CString strOldName, CString strNewName);
		FTLINLINE static BOOL Remove(CString strFileName);

		FTLINLINE DWORD SeekToEnd();
		FTLINLINE DWORD SeekToBegin();

		FTLINLINE virtual CFFile * Duplicate() const;

		FTLINLINE virtual DWORD Seek(LONG lOff, UINT nFrom);
		FTLINLINE virtual BOOL SetLength(DWORD dwNewLen);
		FTLINLINE virtual DWORD GetLength() const;

		FTLINLINE virtual DWORD Read(void* lpBuf, DWORD nCount, LPOVERLAPPED lpOverlapped = NULL);
		FTLINLINE virtual BOOL Write(const void* lpBuf, DWORD nCount);
		FTLINLINE BOOL WriteEndOfLine();

		FTLINLINE virtual BOOL LockRange(DWORD dwPos, DWORD dwCount);
		FTLINLINE virtual BOOL UnlockRange(DWORD dwPos, DWORD dwCount);

		FTLINLINE virtual BOOL Flush();
		FTLINLINE virtual BOOL Close();

		enum BufferCommand { 
			bufferRead, 
			bufferWrite, 
			bufferCommit, 
			bufferCheck, 
			bufferBlocking,
			bufferDirect
		};
		FTLINLINE virtual UINT GetBufferPtr(UINT nCommand, UINT nCount = 0, void** ppBufStart = NULL, void** ppBufMax = NULL);

	protected:
		CString m_strFileName;	// stores the file name for the current file
    };

    //�� ATLPath ʵ���˺ܶ๦��, �� RemoveFileSpec(ɾ��·�������ļ���)
    class CFPath
    {
    public:
		FTLINLINE static BOOL CreateDir(LPTSTR szPath);
    private:
        
    };

    class CFStructuredStorageFile
    {
    public:
        const static DWORD STRUCTURED_STORAGE_FILE_DEFAULT_MODE = STGM_READWRITE | STGM_SHARE_EXCLUSIVE;
        FTLINLINE CFStructuredStorageFile();
        FTLINLINE virtual ~CFStructuredStorageFile();
        FTLINLINE HRESULT CreateDocFile(LPCTSTR pszFilePath, DWORD mode = STGM_CREATE | STRUCTURED_STORAGE_FILE_DEFAULT_MODE);
        FTLINLINE HRESULT OpenDocFile(LPCTSTR pszFilePath,DWORD mode = STRUCTURED_STORAGE_FILE_DEFAULT_MODE);
        FTLINLINE void Close();
        FTLINLINE IStorage * Attach(IStorage* pNews);
        FTLINLINE IStorage * Detach();
        // storage-level access:
        FTLINLINE HRESULT CreateStorage(LPCTSTR pName,BOOL bEnter = TRUE, DWORD mode = STRUCTURED_STORAGE_FILE_DEFAULT_MODE);
        FTLINLINE HRESULT EnterStorage(LPCTSTR pName,DWORD mode = STRUCTURED_STORAGE_FILE_DEFAULT_MODE);
        FTLINLINE HRESULT ExitStorage();
        //bool CreateStream(const CString & name, COleStreamFile &sf, DWORD mode = CFile::modeReadWrite | CFile::shareExclusive);
        //bool OpenStream(const CString & name, COleStreamFile &sf, DWORD mode = CFile::modeReadWrite | CFile::shareExclusive);
        FTLINLINE HRESULT CreateStream(LPCTSTR pName,IStream** ppChildStream,DWORD mode = STRUCTURED_STORAGE_FILE_DEFAULT_MODE);
        FTLINLINE HRESULT OpenStream(LPCTSTR pName,IStream** ppChildStream,DWORD mode = STRUCTURED_STORAGE_FILE_DEFAULT_MODE);
        FTLINLINE HRESULT DestroyElement(LPCTSTR pName);
        // status info:
        FTLINLINE IStorage* GetRootStorage() const;
        FTLINLINE IStorage* GetCurrentStorage() const;
        FTLINLINE BOOL IsOpen() const;
        //CString GetPath(const CString & SepChar) const;
        //CString GetFilename() const;
    private:
        struct StorageData
        {
            IStorage *Stg;
            StorageData *ParentStg;
            StorageData()
            {
                Stg = NULL;
                ParentStg = NULL;
            }
        };
        IStorage    *m_pRootStg;
        StorageData *m_pCurrentStg;
    };

    class CFileSystemFinder
    {

    };

}//namespace FTL

#endif //FTL_FILE_H

#ifndef USE_EXPORT
#  include "ftlFile.hpp"
#endif