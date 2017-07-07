#include "stdafx.h"
#include "FileLog.h"
#ifndef _DISABLE_MODULELOG

#if (_WIN32_IE < 0x0601)
#undef _WIN32_IE
#define _WIN32_IE  0x0601
#endif

#include <ShlObj.h>


////////////////////////////////////////////////////////////////////////////////////////////////////
/// CCSLock
class CCSLock
{
public:
	CCSLock(CRITICAL_SECTION& oCS)
		: m_oCS(oCS)
	{
		::EnterCriticalSection(&m_oCS);
	}

	~CCSLock()
	{
		::LeaveCriticalSection(&m_oCS);
	}

private:
	CRITICAL_SECTION& m_oCS;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// CModuleLog

volatile int CModuleLog::s_nLogLevel = 0;

#define TheLogger (*(CModuleLog::Instance()))

const LPCTSTR CModuleLog::s_szLevelName[] = 
{
	_T("DISABLE"),
	_T("FATAL"),
	_T("ERROR"),
	_T("WARN"),
	_T("INFO"),
	_T("TRACE"),
	_T("DEBUG")
};

LPCTSTR CModuleLog::GetLevelName(int nLevel)
{
	if (nLevel < MODULELOG_LEVEL_DISABLE || nLevel > MODULELOG_LEVEL_DEBUG)
	{
		return s_szLevelName[0];
	}

	return s_szLevelName[nLevel];
}

int CModuleLog::GetLevelFromName(LPCTSTR lpszName)
{
	if (lpszName == NULL || *lpszName == _T('\0'))
		return 0;

	for (int i=0; i<sizeof(s_szLevelName)/sizeof(LPCTSTR); ++i)
	{
		if (_tcsicmp(lpszName, s_szLevelName[i]) == 0)
			return i;
	}

	return 0;
}


BOOL CModuleLog::IsNeedLog(int nLevel)
{
	return (nLevel <= s_nLogLevel);
}

void CModuleLog::Output(int nLevel, LPCTSTR lpszFormat, ...)
{
	va_list args;
	va_start(args, lpszFormat);

	TheLogger.OutputFmt(nLevel, lpszFormat, args);
}

int CModuleLog::InitLog(LPCTSTR lpszLogDir)
{
	return TheLogger.Init(lpszLogDir);
}

void CModuleLog::UninitLog()
{
	TheLogger.Uninit();
}

void CModuleLog::UpdateLogLevel(int nLevel)
{
	TheLogger.UpdateLevel(nLevel);
}

void CModuleLog::SetLogCallback(MODULELOG_CALLBACK pfnCallback)
{
	TheLogger.SetCallback(pfnCallback);
}

void CModuleLog::EraseTimeoutLogs(ULONG ulHours)
{
	TheLogger.EraseTimeout(ulHours);
}

CModuleLog::CModuleLog()
: m_hLogFile(INVALID_HANDLE_VALUE),
m_fnCallback(NULL),
m_dwRollWrite(0),
m_lCallback(0)
{
	memset(m_szModuleName, 0, sizeof(m_szModuleName));
	memset(m_szFilePath, 0, sizeof(m_szModuleName));
	memset(m_szLogIni, 0, sizeof(m_szModuleName));
	::InitializeCriticalSectionAndSpinCount(&m_oCS, 4000);
}

CModuleLog::~CModuleLog()
{
	Uninit();
	::DeleteCriticalSection(&m_oCS);
}

BOOL CModuleLog::GetLogNameInfo(TCHAR lpszFileName[], TCHAR lpszModuleName[])
{
	HMODULE hModule;
	if (!GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, (LPCTSTR)CModuleLog::InitLog, &hModule))
		return FALSE;

	TCHAR szLogFilePath[MAX_PATH] = {0};
	LPTSTR lpszLogFileName;
	GetModuleFileName(hModule, szLogFilePath, MAX_PATH - 1);
	LPTSTR lpPos = _tcsrchr(szLogFilePath, _T('\\'));
	if (lpPos != NULL)
	{
		lpszLogFileName = lpPos + 1;
	}
	else
	{
		lpszLogFileName = szLogFilePath;
	}

	int nFileLen = (int)_tcslen(lpszLogFileName);
	if (nFileLen >= MAX_FILE_NAME - 1)
	{
		nFileLen = MAX_FILE_NAME - 2;
		lpszLogFileName[32] = _T('\0');
	}

	_tcsncpy_s(lpszModuleName, MAX_FILE_NAME, lpszLogFileName, _TRUNCATE);

	{
		LPTSTR lpNext = lpszLogFileName;
		while (*lpNext != _T('\0'))
		{
			if (*lpNext == _T('.'))
				*lpNext = _T('_');
			++lpNext;
		}
	}

	SYSTEMTIME stTime;
	GetLocalTime(&stTime);
	_sntprintf_s(lpszLogFileName + nFileLen, MAX_PATH -(lpszLogFileName - szLogFilePath) - nFileLen, _TRUNCATE, _T("_%02u-%02u-%02u_%u.log"), stTime.wHour, stTime.wMinute, stTime.wSecond, GetCurrentProcessId());

	_tcsncpy_s(lpszFileName, MAX_FILE_NAME, lpszLogFileName, _TRUNCATE);

	return TRUE;
}

#define DEFAULT_SECTION			_T("Default")
#define KEY_LEVEL				_T("Level")
#define KEY_DEFAULTLEVEL		_T("FATAL")
#define KEY_SAMPLELEVEL			_T("SampleLevel")
#define KEY_LEVELUSEDEFAULT		_T("DEFAULT")

void CModuleLog::CreateSampleLogIni()
{
	TheLogger.CreateSampleIni();
}

void CModuleLog::CreateSampleIni()
{
	if (m_szLogIni[0] == _T('\0'))
		return;

	TCHAR szDir[MAX_PATH];
	_tcscpy_s(szDir, MAX_PATH, m_szLogIni);

	{
		LPTSTR lpPos = _tcsrchr(szDir, _T('\\'));
		if (lpPos == NULL)
			return;

		*lpPos = _T('\0');
		::CreateDirectory(szDir, NULL);
		*lpPos = _T('\\');
	}

	TCHAR szLevel[32] = {0};
	::GetPrivateProfileString(DEFAULT_SECTION, KEY_LEVEL, _T(""), szLevel, 32, szDir);
	if (szLevel[0] == _T('\0'))
	{
		//文件不存在
		::WritePrivateProfileString(DEFAULT_SECTION, KEY_SAMPLELEVEL, _T("DISABLE, FATAL, ERROR, WARN, INFO, TRACE, DEBUG"), szDir);
		::WritePrivateProfileString(DEFAULT_SECTION, KEY_LEVEL, KEY_DEFAULTLEVEL, szDir);
	}
}

void CModuleLog::UpdateLevel(int nLevel)
{
	if (nLevel < 0)
	{
		nLevel = GetLevelFromFile();
	}

	s_nLogLevel = nLevel;
}

void CModuleLog::SetCallback(MODULELOG_CALLBACK pfnCallback)
{
	m_fnCallback = pfnCallback;
}

void CModuleLog::EraseTimeout(ULONG ulHours)
{
	SYSTEMTIME sysTime;
	::GetSystemTime(&sysTime);

	FILETIME fileTime;
	::SystemTimeToFileTime(&sysTime, &fileTime);

	unsigned __int64 ui64CurrentTime = (*(unsigned __int64 *)&fileTime);
	unsigned __int64 uiKeepTime = ((unsigned __int64)ulHours) * 60 * 60 * 1000*1000*10;
	if (ui64CurrentTime < uiKeepTime)
		return;

	unsigned __int64 ui64Time = ui64CurrentTime - uiKeepTime;

	TCHAR szFind[MAX_PATH];

	WIN32_FIND_DATA findFileData;

	_tcscpy_s(szFind, MAX_PATH, m_szFilePath);
	LPTSTR lpPos = _tcsrchr(szFind, _T('\\'));
	if (lpPos == NULL)
		return;
	++lpPos;
	*lpPos = _T('\0');

	int nDirLen = (int)(lpPos - szFind);

	_tcscpy_s(lpPos, MAX_PATH - nDirLen, _T("*.log"));

	HANDLE hFind = ::FindFirstFile(szFind, &findFileData);
	if (INVALID_HANDLE_VALUE == hFind)
	{
		return;
	}


	//遍历文件夹
	do
	{
		if ((findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0)
		{
			//不是目录, 是文件
			if ((*(unsigned __int64 *)&findFileData.ftLastWriteTime) < ui64Time)
			{
				_tcscpy_s(lpPos, MAX_PATH - nDirLen, findFileData.cFileName);
				DeleteFile(szFind);	//被占用的文件无法删除.
				TLOG_INFO(_T("Erase timeout log: %s[%llu, %llu]"), findFileData.cFileName, (*(unsigned __int64 *)&findFileData.ftLastWriteTime), ui64Time);
			}
		}
	}while (::FindNextFile(hFind, &findFileData));

	::FindClose(hFind);
}

int CModuleLog::GetLevelFromFile()
{
	TCHAR szLevel[32] = {0};
	::GetPrivateProfileString(m_szModuleName, KEY_LEVEL, _T(""), szLevel, 32, m_szLogIni);

	if (szLevel[0] == _T('\0') || _tcsicmp(szLevel, KEY_LEVELUSEDEFAULT) == 0)
	{
		::GetPrivateProfileString(DEFAULT_SECTION, KEY_LEVEL, _T(""), szLevel, 32, m_szLogIni);
		if (szLevel[0] == _T('\0'))
		{
			return GetLevelFromName(KEY_DEFAULTLEVEL);
		}

		::WritePrivateProfileString(m_szModuleName, KEY_LEVEL, KEY_LEVELUSEDEFAULT, m_szLogIni);
	}

	return GetLevelFromName(szLevel);
}

int CModuleLog::Init(LPCTSTR lpszLogDir)
{
	TCHAR szLogDir[MAX_PATH] = {0};
	int nDirLen = -1;
	if (lpszLogDir == NULL || lpszLogDir[0] == _T('\0'))
	{
		::GetModuleFileName(NULL, szLogDir, MAX_PATH);
		LPTSTR lpszDir = _tcsrchr(szLogDir, _T('\\'));
		if (lpszDir != NULL)
		{
			*lpszDir = _T('\0');
		}

		nDirLen = (int)_tcslen(szLogDir);
		static const TCHAR SUB_DIR[] = _T("\\Log\\");
		if (_tcsncpy_s(szLogDir+nDirLen, MAX_PATH - nDirLen, SUB_DIR, _TRUNCATE) < 0)
			return -1;
		nDirLen += (int)_tcslen(SUB_DIR);
	}
	else
	{
		_tcsncpy_s(szLogDir, MAX_PATH, lpszLogDir, _TRUNCATE);
		nDirLen = (int)_tcslen(szLogDir);
		if (nDirLen >= MAX_PATH - 20)
			return -1;

		if (szLogDir[nDirLen - 1] != _T('\\'))
		{
			szLogDir[nDirLen] = _T('\\');
			++nDirLen;
		}
	}

	if (!::PathFileExists(szLogDir))
	{
		::SHCreateDirectoryEx(NULL, szLogDir, NULL);
	}

	TCHAR szLogFileName[MAX_FILE_NAME];
	if (!GetLogNameInfo(szLogFileName, m_szModuleName))
		return -1;

	if (_tcsncpy_s(szLogDir + nDirLen, MAX_PATH-nDirLen, _T("log.ini"), _TRUNCATE) < 0)
		return -1;

	if (_tcsncpy_s(m_szLogIni, MAX_PATH, szLogDir, _TRUNCATE) < 0)
		return -1;

	int nLogLevel = GetLevelFromFile();

	if (_tcsncpy_s(szLogDir + nDirLen, MAX_PATH-nDirLen, szLogFileName, _TRUNCATE) < 0)
		return -1;

	_tcsncpy_s(m_szFilePath, MAX_PATH, szLogDir, _TRUNCATE);
	s_nLogLevel = nLogLevel;
	return nLogLevel;
}

void CModuleLog::Uninit()
{
	s_nLogLevel = 0;
	m_fnCallback = NULL;

	CCSLock oLock(m_oCS);
	if (m_hLogFile != INVALID_HANDLE_VALUE)
	{
		::CloseHandle(m_hLogFile);
		m_hLogFile = INVALID_HANDLE_VALUE;
	}
}

void CModuleLog::OutputFmt(int nLevel, LPCTSTR lpszFormat, va_list args)
{
#define MAX_LINE_LEN	1024

	SYSTEMTIME stTime;
	GetLocalTime(&stTime);
	TCHAR szBuffer[MAX_LINE_LEN];
	int nLen = _sntprintf_s(szBuffer, MAX_LINE_LEN, _TRUNCATE, _T("[%5s][%5u][%02u:%02u:%02u.%03u]: "), GetLevelName(nLevel), ::GetCurrentThreadId(), stTime.wHour, stTime.wMinute, stTime.wSecond, stTime.wMilliseconds);
	int nLen2 = _vsntprintf_s(szBuffer + nLen, MAX_LINE_LEN - nLen - 2, _TRUNCATE, lpszFormat, args);

	if (nLen2 < 0)
	{
		nLen2 = MAX_LINE_LEN - 3;
	}
	else
	{
		nLen2 += nLen;
	}

	if (::InterlockedCompareExchange(&m_lCallback, 1, 0) == 0)
	{
		if (m_fnCallback != NULL)
		{
			// 将日志返回给callback, 进行上报等处理, 
			// 第四个参数可以将详细参数上报上来, 为了将第四个参数进行聚合统计, 需要将前置部分去除
			m_fnCallback(nLevel, m_szModuleName, lpszFormat, szBuffer + nLen);
		}

		::InterlockedExchange(&m_lCallback, 0);
	}

	szBuffer[nLen2++] = _T('\r');
	szBuffer[nLen2++] = _T('\n');

	OutputToFile(szBuffer, nLen2);
}

void CModuleLog::OutputToFile(LPCTSTR lpszString, int nLen)
{
	CCSLock oLock(m_oCS);

	if (m_hLogFile == INVALID_HANDLE_VALUE)
	{
		//直到有信息输出时才建立文件. 减少无内容文件的创建
		m_hLogFile = ::CreateFile(m_szFilePath, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, 0, NULL);
		if (m_hLogFile == INVALID_HANDLE_VALUE)
		{
			s_nLogLevel = 0;
			return;
		}
		m_dwRollWrite = 0;

#ifdef _UNICODE
		USHORT usUnicodeTag = 0xFEFF;
		DWORD dwWrite=0;
		::WriteFile(m_hLogFile, &usUnicodeTag, sizeof(usUnicodeTag), &dwWrite, NULL);
#endif
	}

	DWORD dwWrite=0;
	::WriteFile(m_hLogFile, lpszString, (DWORD)nLen*sizeof(TCHAR), &dwWrite, NULL);
	m_dwRollWrite += dwWrite;

	static const DWORD MAX_ROLL_SIZE = 1024 * 1024 * 2;	//日志文件最大2M
	if (m_dwRollWrite > MAX_ROLL_SIZE)
	{
		//不能让文件太大了. 循环处理.
		::SetFilePointer(m_hLogFile, sizeof(USHORT), 0, FILE_BEGIN);
		::WriteFile(m_hLogFile, lpszString, (DWORD)nLen*sizeof(TCHAR), &dwWrite, NULL);
		m_dwRollWrite = 0;
	}
}

#endif
