// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>



// TODO:  在此处引用程序需要的其他头文件
#include <windows.h>
#include <TlHelp32.h>
#include <process.h>
#include <Psapi.h>
#include <Shlwapi.h>
#include "winternl.h"
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "FileLog.h"
#pragma comment(lib, "ntdll.lib")
#pragma comment(lib, "psapi.lib")
#pragma comment(lib, "shlwapi.lib")


#ifdef UNICODE
#define TString std::wstring
#else

#endif

#define COUT std::wcout 
#define ENDL std::endl

#define SString std::string
typedef std::pair<TString, TString> strstrPair;  // 存放window Name  以及 ClassName
typedef std::pair<HWND, strstrPair> phpPair;  // 存放 父窗口 ，以及名字集合
typedef std::map<HWND, phpPair> mhhMAP; // map类型{key（子窗口） : value（父窗口）}
typedef std::vector<DWORD> vecID;


typedef struct tagThreadParam
{
	DWORD dwProcessID;
	HANDLE hProcess;
	HWND  hWnd;
}ThreadParam;
