// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>



// TODO:  �ڴ˴����ó�����Ҫ������ͷ�ļ�
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
typedef std::pair<TString, TString> strstrPair;  // ���window Name  �Լ� ClassName
typedef std::pair<HWND, strstrPair> phpPair;  // ��� ������ ���Լ����ּ���
typedef std::map<HWND, phpPair> mhhMAP; // map����{key���Ӵ��ڣ� : value�������ڣ�}
typedef std::vector<DWORD> vecID;


typedef struct tagThreadParam
{
	DWORD dwProcessID;
	HANDLE hProcess;
	HWND  hWnd;
}ThreadParam;
