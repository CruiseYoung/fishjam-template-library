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
    * �����ڴ��Ϊ����(ע����̡߳����̷���ʱ��ͬ��)��
    *   1.���ع����ڴ� -- linux ���� shmget, Windows���� �ڴ�ӳ���ļ�
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
    **************************************************************************************************/
	
    class CFConsoleFile
    {
    public:
        FTLINLINE CFConsoleFile();
        virtual ~CFConsoleFile();
    };

    class CFFile
    {
	public:
		enum SeekPosition { begin = 0x0, current = 0x1, end = 0x2 };
		// Constructors, destructor
		CFFile();
		CFFile(HANDLE hFile);
		virtual ~CFFile();

		// Attributes
		HANDLE m_hFile;

		// Operations
		virtual DWORD GetPosition() const;
		virtual CString GetFileName() const;
		virtual CString GetFilePath() const;
		virtual BOOL SetFilePath(CString strNewName);

		virtual BOOL Open(LPCTSTR pszFileName, 
			DWORD dwAccess = GENERIC_WRITE | GENERIC_READ,
			DWORD dwShareMode = FILE_SHARE_READ | FILE_SHARE_WRITE,
			LPSECURITY_ATTRIBUTES lpSA = NULL,
			DWORD dwCreationDisposition = CREATE_ALWAYS,
			DWORD dwAttributes = FILE_ATTRIBUTE_NORMAL,
			HANDLE hTemplateFile = NULL
			);

		static BOOL Rename(CString strOldName, CString strNewName);
		static BOOL Remove(CString strFileName);

		DWORD SeekToEnd();
		DWORD SeekToBegin();

		virtual CFFile * Duplicate() const;

		virtual DWORD Seek(LONG lOff, UINT nFrom);
		virtual BOOL SetLength(DWORD dwNewLen);
		virtual DWORD GetLength() const;

		virtual DWORD Read(void* lpBuf, DWORD nCount, LPOVERLAPPED lpOverlapped = NULL);
		virtual BOOL Write(const void* lpBuf, DWORD nCount);
		BOOL WriteEndOfLine();

		virtual BOOL LockRange(DWORD dwPos, DWORD dwCount) throw(CXFileException);
		virtual BOOL UnlockRange(DWORD dwPos, DWORD dwCount) throw(CXFileException);

		virtual BOOL Flush();
		virtual BOOL Close();

		enum BufferCommand { 
			bufferRead, 
			bufferWrite, 
			bufferCommit, 
			bufferCheck, 
			bufferBlocking,
			bufferDirect
		};
		virtual UINT GetBufferPtr(UINT nCommand, UINT nCount = 0, void** ppBufStart = NULL, void** ppBufMax = NULL);

	protected:
		CString m_strFileName;	// stores the file name for the current file
    };

    //�� ATLPath ʵ���˺ܶ๦��, �� RemoveFileSpec(ɾ��·�������ļ���)
    class CFPath
    {
    public:
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