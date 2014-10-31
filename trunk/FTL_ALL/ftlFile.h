#ifndef FTL_FILE_H
#define FTL_FILE_H
#pragma once

#ifndef FTL_BASE_H
#  error ftlfile.h requires ftlbase.h to be included first
#endif
#include <atlstr.h>

namespace FTL
{
//Class
    class CFStructuredStorageFile;
//Function

}//namespace FTL

#include <WinIoctl.h>
#include "ftlsystem.h"
#include "ftlThread.h"

namespace FTL
{
	//BYTE -> KB -> MB -> GB -> TB -> PB -> EB -> ZB -> YB
    #define BYTES_PER_KILOBYTE      (1024)
    #define BYTES_PER_MEGABYTE      (BYTES_PER_KILOBYTE * 1000)
    #define BYTES_PER_GIGABYTE      (BYTES_PER_MEGABYTE * 1000)
    #define BYTES_PER_TERABYTE      (BYTES_PER_GIGABYTE * 1000)
    #define BYTES_PER_PETABYTE      (BYTES_PER_TERABYTE * 1000)
	// ? 
	//#define BYTES_PER_ZETATABYTE	(BYTES_PER_PETABYTE * 1000)

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
	};

	enum CreateLocalFileFlags{
		clfCreateIfNotExist,
		clfAutoRename,
		clfOverwrite,
	};

	const BYTE TEXT_FILE_HEADER_UTF8[]				= { 0xEF, 0xBB, 0xBF };
	const BYTE TEXT_FILE_HEADER_UNICODE[]			= { 0xFF, 0xFE };
	const BYTE TEXT_FILE_HEADER_UNICODE_BIG_ENDIAN[]= { 0xFE, 0xFF };

    /**************************************************************************************************
    * IOCP -- 
    * 
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
	*
	* ��д�ļ�(CFile) ʱ������
	*   1.������ʹ�û�������setvbuf(fileLoad.m_pStream, NULL, _IOFBF, 32768);
	*
	**************************************************************************************************/

	/**************************************************************************************************
	* CFileDialog
    *   �����ַ�����ʽ
    *     MFC -- CString strFilter = _T("ImageFile(*.bmp;*.jpg;*.jpeg;*.png)|*.bmp;*.jpg;*.jpeg;*.png|All Files(*.*)|*.*||");
    *     WTL -- CString strFilter = _T("ImageFile(*.bmp;*.jpg;*.jpeg;*.png)|*.bmp;*.jpg;*.jpeg;*.png|All Files(*.*)|*.*||");
    *            strFilter.Replace(TEXT('|'), TEXT('\0')); //ע�⣺���ֱ�����ַ����������� "\0" �ǲ��е�
	*   ��ѡ�ļ�:
	*     1.���� OFN_ALLOWMULTISELECT
	*     2.�����ļ����Ļ�����
	*       const int c_cbBuffSize = (MAX_TRANSLATE_FILES_COUNT * (MAX_PATH + 1)) + 1;
	*     	fileDlg.GetOFN().lpstrFile = strMultiPath.GetBuffer(c_cbBuffSize);
	*       fileDlg.GetOFN().nMaxFile = c_cbBuffSize;
	*     3.������ȡ�ļ�
	*       POSITION startPosition = fileDlg.GetStartPosition();
	*       while (startPosition) { CString strPath = fileDlg.GetNextPathName(startPosition); }
	*     4.�ͷŻ�������Դ strMultiPath.ReleaseBuffer();
	*
	*  ��ʼ��·��
	*    TCHAR szDefaultSavePath[MAX_PATH] = {0};
	*    SHGetSpecialFolderPath(NULL, szDefaultSavePath, CSIDL_MYDOCUMENTS , TRUE);
	*    dlg.m_ofn.lpstrInitialDir = szDefaultSavePath;

    **************************************************************************************************/

	class CFFileUtil
	{
	public:
        //��ȡ CreateFile(xxx, dwDesiredAccess, xxx) �е� dwDesiredAccess ����
        FTLINLINE static LPCTSTR GetFileDesiredAccessFlagsString(FTL::CFStringFormater& formater, DWORD dwDesiredAccess, LPCTSTR pszDivide = TEXT("|"));

        //���Է�������������ܻ�ȡ����ϵͳĿ¼
        FTLINLINE static BOOL DumpAllAPISystemFolderPath(DWORD dwSHGetFolderPathFlags = SHGFP_TYPE_DEFAULT);

		FTLINLINE static TextFileEncoding GetTextFileEncoding(LPCTSTR pszFilePath);
		FTLINLINE static HANDLE CreateLocalWriteFile(__inout LPTSTR pszFilePath, DWORD dwMaxSize, CreateLocalFileFlags flags);

        //������ xxx(��������).xxx ���ļ����ֽ�� �ļ��� + ���� �ķ�ʽ
        //������Ϲ淶���򷵻�TRUE
        FTLINLINE static BOOL SpitFileNameAndIndex(LPCTSTR pszFileName, LPTSTR pszFilePrefix, DWORD dwMaxSize, int& nIndex);

        FTLINLINE static BOOL DumpMemoryToFile(PVOID pBuffer, DWORD dwSize, LPCTSTR pszFilePath);
	};

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
		FTLINLINE virtual LONGLONG GetPosition() const;
		FTLINLINE virtual CString GetFileName() const;
		FTLINLINE virtual CString GetFilePath() const;
		FTLINLINE virtual BOOL SetFilePath(CString strNewName);

		FTLINLINE virtual BOOL Create(LPCTSTR pszFileName, 
			DWORD dwAccess = GENERIC_WRITE | GENERIC_READ,
			DWORD dwShareMode = FILE_SHARE_READ | FILE_SHARE_WRITE,
			LPSECURITY_ATTRIBUTES lpSA = NULL,
			DWORD dwCreationDisposition = CREATE_ALWAYS,
			DWORD dwAttributes = FILE_ATTRIBUTE_NORMAL,
			HANDLE hTemplateFile = NULL
			);

		FTLINLINE static BOOL Rename(CString strOldName, CString strNewName);
		FTLINLINE static BOOL Remove(CString strFileName);

		FTLINLINE LONGLONG SeekToEnd();
		FTLINLINE LONGLONG SeekToBegin();

		FTLINLINE virtual CFFile * Duplicate() const;

		FTLINLINE virtual LONGLONG Seek(LONGLONG lOff, UINT nFrom);
		FTLINLINE virtual BOOL SetLength(LONGLONG newLen);
		FTLINLINE virtual LONGLONG GetLength() const;

		FTLINLINE virtual BOOL Read(void* lpBuf, DWORD nCount, DWORD* pdwRead, LPOVERLAPPED lpOverlapped = NULL);
		FTLINLINE virtual BOOL Write(const void* lpBuf, DWORD nCount, DWORD* pdwWritten, LPOVERLAPPED lpOverlapped = NULL);
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

    class CFFileAnsiEncoding
    {
    public:
        FTLINLINE static BOOL WriteEncodingString(CFFile* pFile, const CAtlString& strValue, 
            DWORD* pnBytesWritten, LPOVERLAPPED lpOverlapped = NULL);
    };
    class CFFileUTF8Encoding
    {
    public:
        FTLINLINE static BOOL WriteEncodingString(CFFile* pFile, const CAtlString& strValue, 
            DWORD* pnBytesWritten, LPOVERLAPPED lpOverlapped = NULL);
    };

    class CFFileUnicodeEncoding
    {
    public:
        FTLINLINE static BOOL WriteEncodingString(CFFile* pFile, const CAtlString& strValue, 
            DWORD* pnBytesWritten, LPOVERLAPPED lpOverlapped = NULL);
    };

    template <typename TEncoding>
    class CFTextFile : public CFFile
    {
    public:
        typedef CFTextFile< TEncoding > thisClass;
        FTLINLINE CFTextFile(TextFileEncoding fileEncoding);
        FTLINLINE BOOL WriteFileHeader(LPOVERLAPPED lpOverlapped = NULL);
        FTLINLINE BOOL WriteString(const CAtlString&strValue, DWORD* pnBytesWritten = NULL, LPOVERLAPPED lpOverlapped = NULL);
    private:
        TextFileEncoding    m_fileEncoding;
    };

    typedef CFTextFile<CFFileAnsiEncoding>    CFAnsiFile;
    typedef CFTextFile<CFFileUnicodeEncoding> CFUnicodeFile;

    //�� ATLPath ʵ���˺ܶ๦��, �� RemoveFileSpec(ɾ��·�������ļ���)
    class CFPath
    {
    public:
		//����ָ��·���е�ȫ��Ŀ¼
		FTLINLINE static BOOL CreateDirTree(LPCTSTR szPath);
        FTLINLINE static BOOL GetRelativePath(LPCTSTR pszFullPath, LPCTSTR pszParentPath, LPTSTR pszRelateivePath, UINT cchMax);

        //����·��ת�ɶ�·��(���Ͽ�������δ����) -- ������ GUI ��ص�ʵ��
        FTLINLINE static CAtlString GetShortPath(LPCTSTR pszFullPath, long nMaxLength);
    private:
        
    };


    enum FileFindResultHandle
    {
        rhContinue,
        rhSkipDir,  //just use for skip special dir
        rhStop,
    };

    class IFileFindCallback
    {
    public:
        virtual FileFindResultHandle OnFindFile(LPCTSTR pszFilePath, const WIN32_FIND_DATA& findData, LPVOID pParam) = 0;
    };

    //ͬ������ָ��Ŀ¼ -- �����˵��Ʒ�ʽ������ݹ鷽ʽ������
    class CFFileFinder
    {
    public:
        FTLINLINE CFFileFinder();
        FTLINLINE VOID SetCallback(IFileFindCallback* pCallBack, LPVOID pParam);
        FTLINLINE BOOL Find(LPCTSTR pszDirPath, LPCTSTR pszFilter = _T("*.*"), BOOL bRecursive = TRUE);
    protected:
        IFileFindCallback*  m_pCallback;
        LPVOID              m_pParam;
        CAtlString          m_strDirPath;
        CAtlString          m_strFilter;
        typedef std::deque<CAtlString>  FindDirsContainer;      //�ݹ鷽ʽʱ������Ҫ���ҵ���Ŀ¼
        FindDirsContainer  m_FindDirs;
        typedef std::list<CAtlString>   FindFiltersContainer;   //����ͨ���ֺ����ֿ��Ķ�����չ��
        FindFiltersContainer    m_FindFilters;
        FTLINLINE   BOOL _isMatchFilterFile(LPCTSTR pszFileName);
    };


    class ICopyDirCallback
    {
    public:
        enum CallbackType
        {
            cbtPrepareSourceFiles,         //prepare source files
            cbtBegin,
            cbtCopyFile,
            cbtEnd,
            cbtError,
        };
        virtual VOID OnBeginPrepareSourceFiles(LPCTSTR pszSrcDir, LPCTSTR pszDstDir) = 0;
        virtual VOID OnBegin(LONGLONG nTotalSize, LONG nFileCount) = 0;
        virtual VOID OnCopyFile(LPCTSTR pszSrcFile, LPCTSTR pszTargetFile, LONG nIndex, LONGLONG nFileSize, LONGLONG nCopiedSize) = 0;
        virtual VOID OnEnd(BOOL bSuccess, LONGLONG nTotalCopiedSize, LONG nCopiedFileCount) = 0;
        virtual VOID OnError(LPCTSTR pszSrcFile, LPCTSTR pszTargetFile, DWORD dwError) = 0;
    };
    class CFDirectoryCopier : public IFileFindCallback
    {
    public:
        FTLINLINE CFDirectoryCopier();
        FTLINLINE ~CFDirectoryCopier();
        FTLINLINE BOOL SetCallback(ICopyDirCallback* pCallback);
        FTLINLINE BOOL SetCopyEmptyFolder(BOOL bCopyEmptyFolder);
        FTLINLINE BOOL Start(LPCTSTR pszSourcePath, LPCTSTR pszDestPath, 
            LPCTSTR pszFilter = _T("*.*"), 
            BOOL bFailIfExists = FALSE, 
            BOOL bRecursive = TRUE);
        FTLINLINE BOOL IsPaused();
        FTLINLINE BOOL PauseOrResume();
        FTLINLINE BOOL Stop();
        FTLINLINE BOOL WaitToEnd(DWORD dwMilliseconds = INFINITE);
    public:
        FTLINLINE virtual FileFindResultHandle OnFindFile(LPCTSTR pszFilePath, const WIN32_FIND_DATA& findData, LPVOID pParam);
    protected:
        ICopyDirCallback*               m_pCallback;

        LONGLONG                        m_nCurCopyFileSize;
        LONGLONG                        m_nTotalSize;
        LONGLONG                        m_nCopiedFileSize;
        LONGLONG                        m_nCurFileTransferred;

        LONG                            m_nFileCount;
        LONG                            m_nCopyFileIndex;
        LONG                            m_nTotalCopiedFileCount;

        BOOL                            m_bFailIfExists;
        BOOL                            m_bRecursive;
        BOOL                            m_bCopyEmptyFolder;
        BOOL                            m_bCancelForCopyFileEx;

        CAtlString                      m_strCurSrcFilePath;
        CAtlString                      m_strCurDstFilePath;
        CAtlString                      m_strSrcDirPath;
        CAtlString                      m_strDstDirPath;
        CAtlString                      m_strFilter;


        struct SourceFileInfo{
            CAtlString  strFullPath;
            BOOL        isDirectory;
            LONGLONG    nFileSize;
        };
        typedef std::list<SourceFileInfo>   SourceFileInfoContainer;

        SourceFileInfoContainer         m_sourceFiles;
        CFThread<DefaultThreadTraits>   m_threadCopy;

        FTLINLINE VOID   _InitValue();
        FTLINLINE static DWORD __stdcall _CopierThreadProc(LPVOID lpThreadParameter);
        FTLINLINE DWORD _InnerCopierThreadProc();


        FTLINLINE static DWORD CALLBACK _CopyFileProgressRoutine(LARGE_INTEGER TotalFileSize,
            LARGE_INTEGER TotalBytesTransferred,
            LARGE_INTEGER StreamSize,
            LARGE_INTEGER StreamBytesTransferred,
            DWORD dwStreamNumber,
            DWORD dwCallbackReason,
            HANDLE hSourceFile,
            HANDLE hDestinationFile,
            LPVOID lpData
            );

        FTLINLINE VOID _NotifyCallBack(ICopyDirCallback::CallbackType type, DWORD dwError = 0);

        FTLINLINE FTLThreadWaitType _PrepareSourceFiles();
        FTLINLINE FTLThreadWaitType _CopyFiles();
        
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