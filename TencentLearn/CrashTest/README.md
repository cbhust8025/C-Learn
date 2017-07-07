# 测试引起Crash的消息和窗口
> 工程项目放在[MyTest](https://github.com/cbhust8025/C-Learn/tree/master/TencentLearn/CrashTest/MyTest) 文件夹下,下面就项目中涉及到的一些变量或者函数进行解释。

## 变量类型
* DWORD类型：事实上是一个unsigned long型变量，常用来存储一些进程ID、错误返回码等等不为负的值。
```C++
typedef unsigned long       DWORD;
```

* string类型和wstring类型，wstring类型的使用范围更广（与string由char构成不同，是由wchar_t），输出的时候需要使用wcout。TCHAR类型即为wchar_t
```
typedef wchar_t     TCHAR;

typedef basic_string<char, char_traits<char>, allocator<char> >
	string;
typedef basic_string<wchar_t, char_traits<wchar_t>, allocator<wchar_t> >
	wstring;
  
#ifdef UNICODE
#define TString std::wstring
#else

#endif
```
* HANDLE类型：句柄类型，是资源的一种表示，操作系统进程要控制线程来操作资源，需要通过HANDLE(句柄)来找到相对应的资源。句柄按照资源类型不同可以划分为：
>* 图标句柄（HICON）
>* 光标句柄（HCURSOR）
>* 窗口句柄（HWND）
>* 应用程序实例句柄（HINSTANCE）

```C++
//
// Handle to an Object
//

#ifdef STRICT
typedef void *HANDLE;
#if 0 && (_MSC_VER > 1000)
#define DECLARE_HANDLE(name) struct name##__; typedef struct name##__ *name
#else
#define DECLARE_HANDLE(name) struct name##__{int unused;}; typedef struct name##__ *name
#endif
#else
typedef PVOID HANDLE;
#define DECLARE_HANDLE(name) typedef HANDLE name
#endif
typedef HANDLE *PHANDLE;
```

* HWND类型：每个窗口都会有一个ID，我们称之为窗口句柄（属于一种HANDLE类型），实质上是一个结构体类型，定义如下。
```C++
#define DECLARE_HANDLE(name) struct name##__{int unused;}; typedef struct name##__ *name

DECLARE_HANDLE            (HWND);
```

* MAX_PATH：包含在minwindef.h中的最大字符串长度的宏定义：
```C++
#define MAX_PATH          260
```

* LPCTSTR类型： 实质上是一个const WCHAR\* 的指针
```C++
//
// UNICODE (Wide Character) types
//

#ifndef _MAC
typedef wchar_t WCHAR;    // wc,   16-bit UNICODE character
#else
// some Macintosh compilers don't define wchar_t in a convenient location, or define it as a char
typedef unsigned short WCHAR;    // wc,   16-bit UNICODE character
#endif

typedef LPWSTR PTSTR, LPTSTR;
```

* PROCESSENTRY32类型：实质上一个结构体类型，用来存放快照进程信息。（存放进程信息和调用成员输出进程信息）用来 Process32First指向第一个进程信息，并将进程信息抽取到PROCESSENTRY32中。用Process32Next指向下一条进程信息。
```C++
typedef struct tagPROCESSENTRY32
{
    DWORD   dwSize;
    DWORD   cntUsage;
    DWORD   th32ProcessID;          // this process
    ULONG_PTR th32DefaultHeapID;
    DWORD   th32ModuleID;           // associated exe
    DWORD   cntThreads;
    DWORD   th32ParentProcessID;    // this process's parent process
    LONG    pcPriClassBase;         // Base priority of process's threads
    DWORD   dwFlags;
    CHAR    szExeFile[MAX_PATH];    // Path
} PROCESSENTRY32;
```
```C++
BOOL bRes = FALSE;
HANDLE vR1;  // 定义一个句柄
DWORD dwResult;  // 定义一个unsigned long类型变量
PROCESSENTRY32 pcEntry;  // 定义一个进程对象
BOOL bFind = FALSE;  // 标识是否找到我们所需要的进程--QQ电脑管家

memset(&pcEntry, 0, sizeof(PROCESSENTRY32)); // 分配内存给进程对象
// 设置dwSize，并且大小一般为sizeof(PROCESSENTRY32)，如果不设置，则调用Process32First会失败
pcEntry.dwSize = sizeof(PROCESSENTRY32);
vR1 = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0); // 获取当前进程的快照，返回句柄
COUT << "vR1: " << vR1 << ENDL;
if (vR1 != (HANDLE)-1 && Process32First(vR1, &pcEntry)) 
{
  // 如果当前进程的句柄不为-1，则表示正常，Get到当前进程对象（系统进程--0）
  TLOG_INFO(TEXT("Process Normal!"));
  COUT << "Process Normal!" << ENDL;
  do 
  {
    bRes = lstrcmpi(pcEntry.szExeFile, ExeName);
    if (!bRes)
    {
      // 对每个进程判断其进程名是否为QQPCTray.exe,可能会找到两个QQPCTray.exe（主界面在显示时）
      //TString sOut;
      //OutText(pcEntry.szExeFile, sOut);
      //COUT << "vR1: " << vR1 << ENDL;
      //COUT << "pEth32ID: " << pcEntry.th32ProcessID << ENDL;
      //COUT << "pEszEF: " << sOut << ENDL;
      vecInfo.push_back(pcEntry.th32ProcessID);
    }
  } while (Process32Next(vR1, &pcEntry));  // 如果可以的话获取下一个进程对象(0--4--376--548等等)
}
```
>* dwSize(DWORD): 在调用Process32First之前必须设置，并且一般设置为sizeof(PROCESSENTRY32)，否则调用Process32First会失败。
```C++
PROCESSENTRY32 pcEntry;  // 定义一个进程快照
memset(&pcEntry, 0, sizeof(PROCESSENTRY32)); // 分配内存给进程对象
// 设置dwSize，并且大小一般为sizeof(PROCESSENTRY32)，如果不设置，则调用Process32First会失败
pcEntry.dwSize = sizeof(PROCESSENTRY32);
```

>* cntUsage(DWORD):此进程的引用计数，这个成员已经不再被使用，所以总是被设置为0。 -----NOT USE

>* th32ProcessID(DWORD): 当前进程的PID值。

>* th32DefaultHeapID(ULONG_PTR): 进程默认堆ID，这个成员已经不再被使用，所以总是被设置为0。-----NOT USE

>* th32ModuleID(DWORD): 当前进程的模块ID，这个成员已经不再被使用，所以总是被设置为0。-----NOT USE

>* cntThreads(DWORD): 此进程开启的线程计数。

>* th32ParentProcessID(DWORD): 此进程的父进程ID。

>* pcPriClassBase(LONG): 线程优先权，当前进程创建的任何一个线程的基础优先级，即在当前进程内创建线程的话，其基本优先级的值。

>* dwFlags: 这个成员已经不再使用，所以总是被设置为0。 -----NOT　USE

>* szExeFile\[MAX_PATH\](CHAR): 进程的可执行文件名称。要获得可执行文件的完整路径，应调用Module32First函数，再检查其返回的MODULEENTRY32结构的szExePath成员。但是，如果被调用进程是一个64位程序，您必须调用QueryFullProcessImageName函数去获取64位进程的可执行文件完整路径名。

## Window API 函数：
* EnumWindows/EnumChildWindows: 遍历系统0x0000000下面的所有主窗口/遍历某个窗口句柄下面的所有子窗口,对每个窗口调用其中的回调函数（第二个参数）, 第三个参数用于传递需要传递数据。
```C++
WINUSERAPI
BOOL
WINAPI
EnumWindows(
    _In_ WNDENUMPROC lpEnumFunc,  // 回调函数
    _In_ LPARAM lParam);  // 参数
// EnumWindows((WNDENUMPROC)EnumFunc, (LPARAM)&vecIDs[i])

WINUSERAPI
BOOL
WINAPI
EnumChildWindows(
    _In_opt_ HWND hWndParent,   // 想要遍历的子窗口的父窗口
    _In_ WNDENUMPROC lpEnumFunc,  // 回调函数
    _In_ LPARAM lParam);  // 参数
//EnumChildWindows(hWnd, (WNDENUMPROC)EnumChildFunc, (LPARAM)&dwTargetID);
```
>* 回调函数定义: 需要在返回类型和函数名中间添加```CALLBACK```，来定义回调函数，参数一定，不能修改。
```C++
BOOL CALLBACK EnumFunc(HWND hWnd, LPARAM lpTargetID);

BOOL CALLBACK EnumChildFunc(HWND hWnd, LPARAM lpTargetID)
```

* GetWindowText/GetClassName： 获取窗口名以及窗口对应的类名。
```C++
WINUSERAPI
int
WINAPI
GetWindowTextW(
    _In_ HWND hWnd,
    _Out_writes_(nMaxCount) LPWSTR lpString,
    _In_ int nMaxCount);
#ifdef UNICODE
#define GetWindowText  GetWindowTextW
#else
#define GetWindowText  GetWindowTextA
#endif // !UNICODE

WINUSERAPI
int
WINAPI
GetClassNameW(
    _In_ HWND hWnd,
    _Out_writes_to_(nMaxCount, return) LPWSTR lpClassName,
    _In_ int nMaxCount
    );
#ifdef UNICODE
#define GetClassName  GetClassNameW
#else
#define GetClassName  GetClassNameA
#endif // !UNICODE
```

* GetWindowThreadProcessId: 获取当前窗口句柄所对应的进程ID
```C++
WINUSERAPI
DWORD
WINAPI
GetWindowThreadProcessId(
    _In_ HWND hWnd,
    _Out_opt_ LPDWORD lpdwProcessId);

// GetWindowThreadProcessId(hWnd, &dwNowProcessID);  // 提取当前窗口对应的进程ID
```

* GetParent/SetParent: 获取/设置父窗口句柄。
```C++
WINUSERAPI
HWND
WINAPI
GetParent(
    _In_ HWND hWnd);

WINUSERAPI
HWND
WINAPI
SetParent(
    _In_ HWND hWndChild,
    _In_opt_ HWND hWndNewParent);
```

* GetExitCodeProcess： 获取进程退出码
```C++
WINBASEAPI
BOOL
WINAPI
GetExitCodeProcess(
    _In_ HANDLE hProcess,
    _Out_ LPDWORD lpExitCode
    );
    
DWORD dwExitCode = 0; // 错误返回码
GetExitCodeProcess(pthParam->hProcess, &dwExitCode);
```

* \_sntprintf\_s: 输出字符串到buffer中。(本项目中，利用这个向日志中写入带格式化输出的字符串)
```C++
#define _sntprintf_s    _snwprintf_s
__DEFINE_CPP_OVERLOAD_SECURE_FUNC_0_2_ARGLIST(
    _Success_(return >= 0)
    int, _snwprintf_s, _vsnwprintf_s,
    _Post_z_                      wchar_t,        _Buffer,
    _In_                          size_t,         _BufferCount,
    _In_z_ _Printf_format_string_ wchar_t const*, _Format
    )

TCHAR szBuffer[MAX_PATH] = { 0 };
_sntprintf_s(szBuffer, MAX_PATH, TEXT("NOW WND( %08X ): Parent WND( %08X ) -- WindowName: %s -- ClassName: %s\n"),
			iter->first, iter->second.first, iter->second.second.first.c_str(), iter->second.second.second.c_str());
TLOG_INFO(szBuffer);
```

* NtQueryInformationProcess: 可以用来获取指定进程的父进程ID。---winternl.h
>* 注意：打开进程句柄时，需要以管理员权限打开编译器。
```C++
__kernel_entry NTSTATUS
NTAPI 
NtQueryInformationProcess (
    IN HANDLE ProcessHandle,  // 传入进程句柄
    IN PROCESSINFOCLASS ProcessInformationClass,  // 缓冲区类型
    OUT PVOID ProcessInformation,  // 缓冲区
    IN ULONG ProcessInformationLength,  //　缓冲区大小
    OUT PULONG ReturnLength OPTIONAL
    );
```
```C++
HANDLE hProcess = NULL;  // 定义一个空句柄，保存打开当前进程ID对应的进程句柄
PROCESS_BASIC_INFORMATION pbiInfo; //  实例化一个PROCESS_BASIC_INFORMATION类型对象，后面用于OpenProcess函数
LONG lStatus; // 定义状态变量
// 注意打开进程句柄时要管理员权限运行VS2015,否则会返回错误代码5--拒绝访问。
hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwNowId); // 打开ID对应的进程，返回其句柄

// 指定类型的进程信息拷贝到某个缓冲
if (hProcess != NULL)  
{// 如果可以打开进程，返回进程句柄，
  lStatus = NtQueryInformationProcess(hProcess,  // 传入进程句柄
    ProcessBasicInformation,  // 缓冲区类型
    (PVOID)& pbiInfo, // 缓冲区
    sizeof(PROCESS_BASIC_INFORMATION),  //　缓冲区大小
    NULL);
  // lStatus == S_OK(0)的时候，表示写入缓冲区成功
  if (S_OK == lStatus)
  {// 如果操作成功

    COUT << "pbiInfo.PebBaseAddress: " << pbiInfo.PebBaseAddress << ENDL;
    COUT << "pbiInfo.Reserved1: " << (DWORD)pbiInfo.Reserved1 << ENDL;
    COUT << "pbiInfo.Reserved2: " << (DWORD)pbiInfo.Reserved2 << ENDL;
    COUT << "pbiInfo.Reserved3: " << (DWORD)pbiInfo.Reserved3 << ENDL;   //----保存父进程ID 
    COUT << "pbiInfo.UniqueProcessId: " << pbiInfo.UniqueProcessId << ENDL;
    dwParentId = (DWORD)pbiInfo.Reserved3;
    bFlag = TRUE;
  }
  CloseHandle(hProcess);  // 销毁句柄
}
```

* ThreadFunc: 与EnumWindows这种类似，开一个独立的线程运行回调函数。
```C++
typedef struct tagThreadParam
{
	DWORD dwProcessID;
	HANDLE hProcess;
	HWND  hWnd;
}ThreadParam;


ThreadParam thParam; // 实例化结构体，保存进程ID、进程句柄、窗口句柄
threadFunc((void*)&thParam);

unsigned int CALLBACK threadFunc(void* vParam);
```

* 日志操作:复制工程中的FileLog.h、FileLog.cpp两个文件到我们想要写入日志的工程中。在工程的初始化函数，或者主函数开始的地方初始化日志对象：
```C++
// 初始化日志
TLOG_INIT(NULL);
TLOG_UPDATELEVEL(MODULELOG_LEVEL_INFO);  //设置级别

// 反初始化日志
TLOG_UNINIT();
```

使用下列语句分别写入对应的信息到日志，可以使用格式化输出
```C++

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
```

* lstrcmpi: 比较两个字符串，此比较不区分大小写
```C++
// 函数功能：比较两个字符串，此比较不区分大小写。
int lstrcmp(LPCTSTR lpString1,LPCTSTR lpString2);
// 参数：
// lpString1：指向将被比较的第一个字符串。
// lpString2：指向将被比较的第二个字符串。
// 返回值：若第一个字符串比第二个字符串小则返回值为负；若第一个字符串比第二个字符串大则返回值为正；若两个字符串相等则返回值为0。
// 注意：lstrcmp函数比较两个字符串是通过检查第一个字符是否不同，然后检查第二个字符，直到发现不同之处或是到了字符串的末尾。

```
