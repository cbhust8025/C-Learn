#include "stdafx.h"
#include "ProcessLogic.h"


CProcessLogic::CProcessLogic()
{
}


CProcessLogic::~CProcessLogic()
{
}

BOOL CProcessLogic::OutText(TCHAR w[], TString& cRes) {
	// 声明输出进程名函数,将进程名存入string 中
	int index = 0;

	while (w[index] != '\0') {
		cRes.push_back(w[index]);
		index++;
	}
	return w[0] == '\0' ? TRUE : FALSE;
}

BOOL CProcessLogic::GetExeProcessId(LPCTSTR ExeName, vecID& vecInfo)
{// 获取ExeName 对应的ID，可能有不止一个ID，返回所有我们想要的ID
	BOOL bRes = FALSE;
	HANDLE vR1;  // 定义一个void类型指针
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
	else 
	{
		dwResult = 0;
		COUT << "Process Error!" << ENDL;
	}

	return vecInfo.size() > 0 ? TRUE : FALSE;
}

BOOL CProcessLogic::GetParentProcessId(DWORD dwNowId, DWORD& dwParentId)
{// 获取当前传入进程ID对应的父进程ID
	HANDLE hProcess = NULL;  // 定义一个空句柄，保存打开当前进程ID对应的进程句柄
	PROCESS_BASIC_INFORMATION pbiInfo; //  实例化一个PROCESS_BASIC_INFORMATION类型对象，后面用于OpenProcess函数
	LONG lStatus; // 定义状态变量
	BOOL bFlag = FALSE; // 定义是否成功找到父进程ID
	hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwNowId); // 打开ID对应的进程，返回其句柄
	// 注意打开进程句柄时要管理员权限运行VS2015,否则会返回错误代码5--拒绝访问。

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
			COUT << "pbiInfo.Reserved3: " << (DWORD)pbiInfo.Reserved3 << ENDL;
			COUT << "pbiInfo.UniqueProcessId: " << pbiInfo.UniqueProcessId << ENDL;
			dwParentId = (DWORD)pbiInfo.Reserved3;
			bFlag = TRUE;
		}
		CloseHandle(hProcess);  // 销毁句柄
	}

	if(bFlag)
	{
		TLOG_INFO(TEXT("GetParentID Successful!"));
		COUT << "GetParentID Successful!" << ENDL;
	}
	else
	{
		TLOG_INFO(TEXT("GetParentID Failed!"));
		COUT << "GetParentID Failed!" << ENDL;
	}
	return bFlag;
}

