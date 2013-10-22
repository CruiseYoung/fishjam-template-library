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
	//BYTE -> KB -> MB -> GB -> TB -> PB -> EB -> ZB -> YB
    #define BYTES_PER_KILOBYTE      (1024)
    #define BYTES_PER_MEGABYTE      (BYTES_PER_KILOBYTE * 1000)
    #define BYTES_PER_GIGABYTE      (BYTES_PER_MEGABYTE * 1000)
    #define BYTES_PER_TERABYTE      (BYTES_PER_GIGABYTE * 1000)
    #define BYTES_PER_PETABYTE      (BYTES_PER_TERABYTE * 1000)
	// ? 
	//#define BYTES_PER_ZETATABYTE	(BYTES_PER_PETABYTE * 1000)

	/**************************************************************************************************
	* 文本文件：
	*   ANSI[没有标识字符] -- 采用当前语言编码保存(如中文936, 韩文949)，文件头没有标识符，在不同语言的OS下会显示成乱码。
	*   UTF-8 without signature[65001] -- UTF8 编码，文件头没有标识符(记事本不支持)
	*   UTF-8 with signature[65001, 0xEFBBBF] -- UTF8编码，文件头有标识符，英文对应的是 0xXX
	*   Unicode[1200, 0xFFFE] -- Unicode 编码，英文对应的是 0xXX00
	*   Unicode Big-Endian[1201, 0xFEFF] -- Unicode 编码，英文对应的是 0x00XX
	*   UTF-7[65000] -- UTF7编码？文件头没有标识符
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
	* OVERLAPPED -- 异步I/O是创建高性能可伸缩的应用程序的秘诀，因为它允许单个线程处理来自不同客户机的请求。
	*   注意：
	*   1.不能随便结束发出 I/O 请求的线程
	*     Windows 对异步I/O请求规定了一个限制，即如果线程将一个异步 I/O 请求发送给设备驱动程序，然后终止运行，
	*     那么该 I/O 请求就会丢失，并且在I/O请求运行结束时，没有线程得到这个通知。
	*
    * 共享内存分为两种(注意多线程、进程访问时的同步)：
    *   1.本地共享内存 -- linux 下是 shmget, Windows下是 内存映射文件
	*     ATL 通过 CAtlFileMapping 对文件映射进行了封装
    *   2.分布式共享内存(DSM) -- 在网络上扩展了虚拟内存的概念，通过全局/共享内存中的数据进行透明的进程间通信。
    *     如多台计算机被群集(cluster)为一套逻辑系统，系统中的内存在群集中共享应用程序通过共享内存通信。
    *  
    * CreateFile 可以用来打开各式各样的资源，包括文件(硬盘，软盘，光盘或其他)，串口和并口，Named pipes，console等，
    *   可以在 dwFlagsAndAttributes 变量上设置 FILE_FLAG_OVERLAPPED 属性。
    *   OVERLAPPED结构执行两个功能：
    *     1.识别每一个目前正在进行的overlapped操作；
    *     2.在代码和系统之间提供一个共享区域，参数可以在该区域中双向传递。
    *   GetOverlappedResult
    * 
    *   虽然你要求一个overlapped操作，但它不一定就是overlapped，如果数据已经被放进cache中，或操作系统认为它可以很快地
    *   取得那份数据，那么文件操作就会在ReadFile或WriteFile返回之前完成，而这两个函数将返回TRUE。这种情况下，文件handle
    *   处于激发状态，而对文件的操作可被视为overlaped一样。另外，如果你要求一个文件操作为overlapped，而操作系统把
    *   这个“操作请求”放到队列中等待执行，那么ReadFile和WriteFile都会返回FALSE，这时你必须调用GetLastError并确定
    *   它传回ERROR_IO_PENDING，那意味着“overlapped I/O请求”被放进队列中等待执行
    * 
    * Console 文件
    *   m_hOutput = GetStdHandle (STD_OUTPUT_HANDLE);
    *   if (m_hOutput == INVALID_HANDLE_VALUE) {
    *     API_VERIFY(AllocConsole ());
    *     m_hOutput = GetStdHandle (STD_OUTPUT_HANDLE);
    *   }
    *   SetConsoleTitle (TEXT("ActiveX Debug Output"));  
    *   
    * 通过文件句柄获取文件信息 GetFileInformationByHandle
	*
	* CreateFile 时如果有 FILE_FLAG_SEQUENTIAL_SCAN， 表示优化Cache，适用于从头到尾顺序访问，不会随机跳
	* GetTempFileName -- 生成指定目录下临时文件的名字，通常用于生成临时文件
	*
	* 读写文件(CFile) 时的性能
	*   1.设置流使用缓冲区：setvbuf(fileLoad.m_pStream, NULL, _IOFBF, 32768);
	*
	**************************************************************************************************/

	/**************************************************************************************************
	* CFileDialog
	*   多选文件:
	*     1.设置 OFN_ALLOWMULTISELECT
	*     2.设置文件名的缓冲区
	*       const int c_cbBuffSize = (MAX_TRANSLATE_FILES_COUNT * (MAX_PATH + 1)) + 1;
	*     	fileDlg.GetOFN().lpstrFile = strMultiPath.GetBuffer(c_cbBuffSize);
	*       fileDlg.GetOFN().nMaxFile = c_cbBuffSize;
	*     3.遍历获取文件
	*       POSITION startPosition = fileDlg.GetStartPosition();
	*       while (startPosition) { CString strPath = fileDlg.GetNextPathName(startPosition); }
	*     4.释放缓冲区资源 strMultiPath.ReleaseBuffer();
	*
	*  初始化路径
	*    TCHAR szDefaultSavePath[MAX_PATH] = {0};
	*    SHGetSpecialFolderPath(NULL, szDefaultSavePath, CSIDL_MYDOCUMENTS , TRUE);
	*    dlg.m_ofn.lpstrInitialDir = szDefaultSavePath;

    **************************************************************************************************/

	class CFFileUtil
	{
	public:
        //获取 CreateFile(xxx, dwDesiredAccess, xxx) 中的 dwDesiredAccess 参数
        FTLINLINE static LPCTSTR GetFileDesiredAccessFlagsString(FTL::CFStringFormater& formater, DWORD dwDesiredAccess, LPCTSTR pszDivide = TEXT("|"));

        //调试方法，输出所有能获取到的系统目录
        FTLINLINE static BOOL DumpAllAPISystemFolderPath(DWORD dwSHGetFolderPathFlags = SHGFP_TYPE_DEFAULT);

		FTLINLINE static TextFileEncoding GetTextFileEncoding(LPCTSTR pszFilePath);
		FTLINLINE static HANDLE CreateLocalWriteFile(__inout LPTSTR pszFilePath, DWORD dwMaxSize, CreateLocalFileFlags flags);

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
		FTLINLINE virtual DWORD GetPosition() const;
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

		FTLINLINE DWORD SeekToEnd();
		FTLINLINE DWORD SeekToBegin();

		FTLINLINE virtual CFFile * Duplicate() const;

		FTLINLINE virtual DWORD Seek(LONG lOff, UINT nFrom);
		FTLINLINE virtual BOOL SetLength(DWORD dwNewLen);
		FTLINLINE virtual DWORD GetLength() const;

		FTLINLINE virtual DWORD Read(void* lpBuf, DWORD nCount, LPOVERLAPPED lpOverlapped = NULL);
		FTLINLINE virtual BOOL Write(const void* lpBuf, DWORD nCount, DWORD* pdwWritten);
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

    class CFFileUTF8Encoding
    {
    public:
        FTLINLINE static BOOL WriteEncodingString(CFFile* pFile, const CAtlString& strValue, DWORD* pnBytesWritten);
    };

    class CFFileUnicodeEncoding
    {
    public:
        FTLINLINE static BOOL WriteEncodingString(CFFile* pFile, const CAtlString& strValue, DWORD* pnBytesWritten);
    };

    template <typename TEncoding>
    class CFTextFile : public CFFile
    {
    public:
        typedef CFTextFile< TEncoding > thisClass;
        FTLINLINE CFTextFile(TextFileEncoding fileEncoding);
        FTLINLINE BOOL WriteFileHeader();
        FTLINLINE BOOL WriteString(const CAtlString&strValue, DWORD* pnBytesWritten);
    private:
        TextFileEncoding    m_fileEncoding;
    };


    //有 ATLPath 实现了很多功能, 如 RemoveFileSpec(删除路径最后的文件名)
    class CFPath
    {
    public:
		//创建指定路径中的全部目录
		FTLINLINE static BOOL CreateDirTree(LPTSTR szPath);
    private:
        
    };

    class ICopyDirCallback
    {
    public:
        virtual VOID OnBegin(LONGLONG nTotalSize, LONG nFileCount) = 0;
        virtual VOID OnCopyFile(LPCTSTR pszSrcFile, LPCTSTR pszTargetFile, LONG nIndex, LONGLONG nFileSize, LONGLONG nCopiedSize) = 0;
        virtual VOID OnError(LPCTSTR pszSrcFile, LPCTSTR pszTargetFile, DWORD dwError) = 0;
        virtual VOID OnEnd(BOOL bSuccess, LONGLONG nTotalCopiedSize, LONG nCopiedFileCount) = 0;
    };

    class CFDirectoryCopier
    {
    public:
        FTLINLINE CFDirectoryCopier();
        FTLINLINE ~CFDirectoryCopier();
        FTLINLINE BOOL SetCallback(ICopyDirCallback* pCallback);
        FTLINLINE BOOL Start(LPCTSTR pszSourcePath, LPCTSTR pszTargetPath, LPCTSTR pszFilter = _T("*.*"));
        FTLINLINE BOOL Stop();
    protected:
        ICopyDirCallback*               m_pCallback;
        CFThread<DefaultThreadTraits>   m_threadCopy;
        static DWORD __stdcall _CopierThreadProc(LPVOID lpThreadParameter);
        DWORD _InnerCopierThreadProc();
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