////////////////////////////////////////////////////////
//

#ifndef __MODULELOG_H_8B5D4B84_
#define __MODULELOG_H_8B5D4B84_

#ifndef _WIN32_WINNT		// 允许使用特定于 Windows XP 或更高版本的功能。
#define _WIN32_WINNT 0x0501	// 将此值更改为相应的值，以适用于 Windows 的其他版本。
#endif						

#include <tchar.h>
#include <Windows.h>

#define MODULELOG_LEVEL_DISABLE		0
#define MODULELOG_LEVEL_FATAL		1
#define MODULELOG_LEVEL_ERROR		2
#define MODULELOG_LEVEL_WARN		3
#define MODULELOG_LEVEL_INFO		4
#define MODULELOG_LEVEL_TRACE		5
#define MODULELOG_LEVEL_DEBUG		6

typedef void (*MODULELOG_CALLBACK)(int nLevel, LPCTSTR lpszModule, LPCTSTR lpszFormat, LPCTSTR lpszResult);

#ifndef _DISABLE_MODULELOG
class CModuleLog
{
public:
	static CModuleLog *Instance()
	{
		static const ULONG_PTR BeingCreate = 1;
		static volatile ULONG_PTR s_pInstance = 0;

		if (s_pInstance != 0 && s_pInstance != BeingCreate)
			return reinterpret_cast<CModuleLog*>(s_pInstance);
#ifdef _M_X64
		if (InterlockedCompareExchange64((volatile LONGLONG *)&s_pInstance, BeingCreate, 0) == 0)
#else
		if (InterlockedCompareExchange((volatile LONG *)&s_pInstance, BeingCreate, 0) == 0)
#endif
		{
			//
			//  Create the instance
			//

			static CModuleLog sInst;
#ifdef _M_X64
			InterlockedExchange64((LONGLONG *)&s_pInstance, (LONGLONG)(ULONG_PTR)&sInst);
#else
			InterlockedExchange((LONG *)&s_pInstance, (LONG)(ULONG_PTR)&sInst);
#endif

			return reinterpret_cast<CModuleLog *>(s_pInstance);
		}
		else
		{
			//
			//  Wait for the instance to complete create
			//

			while (s_pInstance == BeingCreate)
				Sleep(0);

			return reinterpret_cast<CModuleLog *>(s_pInstance);
		}
	}

	~CModuleLog();

public:
	static int InitLog(LPCTSTR lpszLogDir = NULL);
	static void UninitLog();
	static void Output(int nLevel, LPCTSTR lpszFormat, ...);
	static BOOL IsNeedLog(int nLevel);
	static LPCTSTR GetLevelName(int nLevel);
	static int GetLevelFromName(LPCTSTR lpszName);
	static void CreateSampleLogIni();
	static void UpdateLogLevel(int nLevel);
	static void SetLogCallback(MODULELOG_CALLBACK pfnCallback);
	static void EraseTimeoutLogs(ULONG ulHours);

	static const int MAX_FILE_NAME = 64;
private:
	static const LPCTSTR s_szLevelName[];
	static volatile int s_nLogLevel;

private:
	int Init(LPCTSTR lpszLogDir);
	void Uninit();
	int GetLevelFromFile();
	void UpdateLevel(int nLevel);
	void SetCallback(MODULELOG_CALLBACK pfnCallback);
	void EraseTimeout(ULONG ulHours);
	void CreateSampleIni();
	BOOL GetLogNameInfo(TCHAR lpszFileName[], TCHAR lpszModuleName[]);
	void OutputFmt(int nLevel, LPCTSTR lpszFormat, va_list args);
	void OutputToFile(LPCTSTR lpszString, int nLen);
	CModuleLog();

private:
	MODULELOG_CALLBACK m_fnCallback;
	HANDLE m_hLogFile;
	DWORD m_dwRollWrite;
	volatile LONG m_lCallback;
	TCHAR m_szModuleName[MAX_FILE_NAME];
	TCHAR m_szFilePath[MAX_PATH];
	TCHAR m_szLogIni[MAX_PATH];
	CRITICAL_SECTION m_oCS;
};

#define TLOG_INIT(x) CModuleLog::InitLog(x)
#define TLOG_UNINIT() CModuleLog::UninitLog()
#define TLOG_UPDATELEVEL(x) CModuleLog::UpdateLogLevel(x)
#define TLOG_SETCALLBACK(x) CModuleLog::SetLogCallback(x)
#define TLOG_ERASETIMEOUT(x) CModuleLog::EraseTimeoutLogs(x)
#define TLOG_CREATESAMPLEINI() CModuleLog::CreateSampleLogIni()

#define TLOG_FATAL(format, ...) \
	if (CModuleLog::IsNeedLog(MODULELOG_LEVEL_FATAL))\
{\
	CModuleLog::Output(MODULELOG_LEVEL_FATAL, format, __VA_ARGS__);\
}\

#define TLOG_ERROR(format, ...) \
	if (CModuleLog::IsNeedLog(MODULELOG_LEVEL_ERROR))\
{\
	CModuleLog::Output(MODULELOG_LEVEL_ERROR, format, __VA_ARGS__);\
}\

#define TLOG_WARN(format, ...) \
	if (CModuleLog::IsNeedLog(MODULELOG_LEVEL_WARN))\
{\
	CModuleLog::Output(MODULELOG_LEVEL_WARN, format, __VA_ARGS__);\
}\

#define TLOG_INFO(format, ...) \
	if (CModuleLog::IsNeedLog(MODULELOG_LEVEL_INFO))\
{\
	CModuleLog::Output(MODULELOG_LEVEL_INFO, format, __VA_ARGS__);\
}\

//Release版本有可能需要禁用LowLevel级别的日志, 减少性能影响(虽然性能影响非常小)
#ifndef WIA_DISABLE_LOWLEVEL_MODULELOG
#define TLOG_TRACE(format, ...) \
	if (CModuleLog::IsNeedLog(MODULELOG_LEVEL_TRACE))\
{\
	CModuleLog::Output(MODULELOG_LEVEL_TRACE, format, __VA_ARGS__);\
}\

#define TLOG_DEBUG(format, ...) \
	if (CModuleLog::IsNeedLog(MODULELOG_LEVEL_DEBUG))\
{\
	CModuleLog::Output(MODULELOG_LEVEL_DEBUG, format, __VA_ARGS__);\
}\

#else
#define TLOG_TRACE __noop
#define TLOG_DEBUG __noop
#endif
#else
#define TLOG_INIT __noop
#define TLOG_UNINIT __noop
#define TLOG_UPDATELEVEL __noop
#define TLOG_SETCALLBACK __noop
#define TLOG_ERASETIMEOUT __noop
#define TLOG_CREATESAMPLEINI __noop

#define TLOG_FATAL __noop
#define TLOG_ERROR __noop
#define TLOG_WARN __noop
#define TLOG_INFO __noop
#define TLOG_TRACE __noop
#define TLOG_DEBUG __noop

#endif //_DISABLE_MODULELOG


#endif //__MODULELOG_H_8B5D4B84_