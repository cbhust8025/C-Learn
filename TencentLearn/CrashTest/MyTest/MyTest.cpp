// MyTest.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "VectorHelper.h"
#include "ProcessLogic.h"
#include "MapHelper.h"
#include "FileLog.h"

#define TRAY_EXE	(_TEXT("QQPCTray.exe"))  // 找到管家的主进程

/*      全局变量声明      */
DWORD dwTrayID;  // 声明托盘tray的ID
void StartMap();  // 声明测试函数
void MsgGoMap();  // 声明消息测试函数
MapHelper TrayMapHelper(TRUE);  // 实例化一个托盘map操作对象,内含一个空map
MapHelper MainMapHelper(FALSE);  // 实例化一个主界面map操作对象,内含一个空map
VectorHelper vecHelper;  // 实例化一个vector操作对象
CProcessLogic cProcessLogic; // 实例化一个进程操作对象
/*      全局变量声明      */

int _tmain(int argc, _TCHAR* argv[])  // 入口
{
	// 初始化日志
	TLOG_INIT(NULL);
	TLOG_UPDATELEVEL(MODULELOG_LEVEL_INFO);

	TLOG_INFO(_T("开始测试！"));
	//COUT << "开始测试~！" << ENDL;

	//PostMessage(FindWindow(_T("AtlAxWin80"), NULL), 0x37F, NULL, NULL);
	StartMap(); // 开始将托盘tray和主界面的进程信息进行map
	TLOG_INFO(_T("输出托盘TrayMap:"));
	COUT << "输出托盘TrayMap:" << ENDL;
	TrayMapHelper.printMap();

	TLOG_INFO(_T("输出主界面Map:"));
	COUT << "输出主界面Map:" << ENDL;
	MainMapHelper.printMap();

	TLOG_INFO(_T("遍历制好的map，开始给每个窗口发送消息:"));
	//COUT << "遍历制好的map，开始给每个窗口发送消息:" << ENDL;
	MsgGoMap();

	TLOG_INFO(_T("结束测试~！"));
	//COUT << "结束测试~！" << ENDL;

	TLOG_UNINIT();
    return 0;  // 主程序结束，结束进程，退出
}

unsigned int CALLBACK ThreadFunc(void* vParam)
{
	// 对每个窗口开一个线程发送0x0000-0xFFFF消息
	ThreadParam *pthParam = (ThreadParam *)vParam;
	DWORD dwExitCode = 0; // 错误返回码
	DWORD dwMessage = 0;  // 发送的message，从0000-ffff
	TCHAR wszBuffer[MAX_PATH] = { 0 };

	if (pthParam->hWnd)
	{
		// 如果窗口句柄不为0
		__try
		{
			do {
				// 从0000 开始发消息，直到0xffff结束
				GetExitCodeProcess(pthParam->hProcess, &dwExitCode); // 获取返回码
				//#define STATUS_PENDING    ((DWORD   )0x00000103L)    
				if (dwExitCode != STILL_ACTIVE) 
				{
					// 如果线程已结束
					wnsprintf(wszBuffer, MAX_PATH, TEXT("PostMessage succeed! hWnd =%08X msg =%d"), pthParam->hWnd, dwMessage);
					TLOG_INFO(wszBuffer);
					COUT << "Process Crashed! hWnd = " << pthParam->hWnd << "msg = " << dwMessage << ENDL;
					break;
				}
				if (PostMessageA(pthParam->hWnd, dwMessage, 0, 0))
				{
					wnsprintf(wszBuffer, MAX_PATH, TEXT("PostMessage succeed! hWnd =%08X msg =%d"), pthParam->hWnd, dwMessage);
					TLOG_INFO(wszBuffer);
					COUT << "PostMessage succeed! hWnd = " << pthParam->hWnd << " msg = " << dwMessage << ENDL;
				}
				else
				{
					GetExitCodeProcess(pthParam->hProcess, &dwExitCode);
					if (dwExitCode != STILL_ACTIVE)
					{
						// 如果线程已结束
						wnsprintf(wszBuffer, MAX_PATH, TEXT("PostMessage succeed! hWnd =%08X msg =%d"), pthParam->hWnd, dwMessage);
						TLOG_INFO(wszBuffer);
						COUT << "Process Crashed! hWnd = " << pthParam->hWnd << "msg = " << dwMessage << ENDL;
						break;
					}

				}
				dwMessage++;
			} while (dwMessage != 0xFFFF);
		}
		__except (1)
		{
			TLOG_INFO(_T("Error Param Cause Crashed!\n"));
		}
	}

	return 0;
}

BOOL CALLBACK EnumChildFunc(HWND hWnd, LPARAM lpTargetID)
{
	// 当前回调的是主窗口的各种子窗口，或者是从子窗口回调过来的子窗口，制成一个map
	BOOL bRes = FALSE;
	TCHAR szWndName[MAX_PATH + 1] = { 0 };
	TCHAR szClassName[MAX_PATH + 1] = { 0 };
	phpPair phpValue;
	strstrPair ppNames;
	TString strWndName;
	TString strClassName;  // 保存窗口名称，以及类名称
	DWORD dwTargetID = *(DWORD*)lpTargetID;  // 将当前传入的ID设置成目标ID，也就是我们要找的窗口对应的进程ID

	GetWindowText(hWnd, szWndName, MAX_PATH);
	GetClassName(hWnd, szClassName, MAX_PATH);
	cProcessLogic.OutText(szWndName, strWndName);
	cProcessLogic.OutText(szClassName, strClassName);
	if (dwTargetID == dwTrayID)
	{
		// 如果目标ID是托盘进程
		bRes = TrayMapHelper.AddToMap(hWnd, GetParent(hWnd), strWndName, strClassName);
		if (bRes)
		{
			TLOG_INFO(_T("TrayMap Add Item Successful!"));
			//COUT << "TrayMap Add Item Successful!" << ENDL;
		}
	}
	else
	{
		// 如果目标ID是主界面进程
		bRes = MainMapHelper.AddToMap(hWnd, GetParent(hWnd), strWndName, strClassName);
		if (bRes)
		{
			TLOG_INFO(_T("MainMap Add Item Successful!"));
			//COUT << "MainMap Add Item Successful!" << ENDL;
		}
	}

	TLOG_INFO(_T("Call the EnumChildFunc!"));
	//COUT << "Call the EnumChildFunc!    " ;
	EnumChildWindows(hWnd, (WNDENUMPROC)EnumChildFunc, (LPARAM)&lpTargetID);  // 对子窗口的每个子窗口调用EnumChildFunc(父窗口， ) 函数
	return TRUE;
}

BOOL CALLBACK EnumFunc(HWND hWnd, LPARAM lpTargetID)
{// 对每个桌面主窗口调用的回调函数,如果当前的窗口句柄等于我们传入的lpID则表示当前的窗口是我们所需要的主窗口
	//COUT << "Call the EnumFunc! " << *(DWORD *)lpID << "   ";
	BOOL bRes = FALSE;
	TCHAR szWndName[MAX_PATH + 1] = { 0 };
	TCHAR szClassName[MAX_PATH + 1] = { 0 };
	TString strWndName;
	TString strClassName;  // 保存窗口名称，以及类名称
	DWORD dwNowProcessID = 0; // 保存我们提取当前窗口对应的进程ID
	DWORD dwTargetID = *(DWORD*)lpTargetID;  // 将当前传入的ID设置成目标ID，也就是我们要找的窗口对应的进程ID

	if (dwTargetID > 0)  // 如果目标ID不为空
	{
		GetWindowThreadProcessId(hWnd, &dwNowProcessID);  // 提取当前窗口对应的进程ID
		if (dwNowProcessID == dwTargetID) 
		{ 
			//两者相同说明我们找到了目标ID下面的所有主窗口
			COUT << hWnd << "  " << GetParent(hWnd) << ENDL;
			EnumChildWindows(hWnd, (WNDENUMPROC)EnumChildFunc, (LPARAM)&dwTargetID);  // 对主窗口的每个子窗口调用EnumChildFunc(父窗口， ) 函数
			GetWindowText(hWnd, szWndName, MAX_PATH);
			GetClassName(hWnd, szClassName, MAX_PATH);
			cProcessLogic.OutText(szWndName, strWndName);
			cProcessLogic.OutText(szClassName, strClassName);
			if (dwTargetID == dwTrayID) 
			{
				// 如果目标ID是托盘进程
				bRes = TrayMapHelper.AddToMap(hWnd, GetParent(hWnd), strWndName, strClassName);
				if (bRes)
				{
					TLOG_INFO(_T("TrayMap Add Item Successful!"));
					//COUT << "TrayMap Add Item Successful!" << ENDL;
				}
			}
			else 
			{
				// 如果目标ID是主界面进程
				bRes = MainMapHelper.AddToMap(hWnd, GetParent(hWnd), strWndName, strClassName);
				if (bRes)
				{
					TLOG_INFO(_T("MainMap Add Item Successful!"));
					//COUT << "MainMap Add Item Successful!" << ENDL;
				}
			}
		}
	}

	return TRUE;
}

void StartMap()
{
	BOOL bRes = FALSE;
	vecID vecIDs;
	cProcessLogic.GetExeProcessId(TRAY_EXE, vecIDs);
	vecID vecParentIDs(vecIDs.size(), 0);
	vecHelper.PrintVector(vecIDs);
	TCHAR wszBuffer[MAX_PATH] = { 0 };

	for (int i = 0;i < vecIDs.size();i++)
	{
		cProcessLogic.GetParentProcessId(vecIDs[i], vecParentIDs[i]);
		//COUT << vecIDs[i] << ": " << vecParentIDs[i] << ENDL;
		wnsprintf(wszBuffer, MAX_PATH, TEXT("vecIDs[%d] : %d --- vecParentIDs[%d] : %d "), i, vecIDs[i], i, vecParentIDs[i]);
		TLOG_INFO(wszBuffer);
	}
	bRes = vecHelper.GetCommonID(vecIDs, vecParentIDs, dwTrayID);
	if (!bRes)
	{
		TLOG_INFO(_T("未找到管家托盘的进程ID！"));
		//COUT << "未找到管家托盘的进程ID！" << ENDL;
		return;
	}
	for (int i = 0; i < vecIDs.size();i++)
	{
		if (!EnumWindows((WNDENUMPROC)EnumFunc, (LPARAM)&vecIDs[i]))  // 遍历tray以及主界面
		{
			TLOG_INFO(_T("EnumWindows failed! error ="));
			//COUT << "EnumWindows failed! error =" << GetLastError() << ENDL;
		}
	}
	return;
}

void MsgGoMap()
{// 遍历两个map,开线程发送信息, 保存进日志
	BOOL bRes = FALSE;
	ThreadParam thParam; // 实例化结构体，保存进程ID、进程句柄、窗口句柄
	DWORD dwProcessId = 0; // 保存遍历的时候存入进程ID
	mhhMAP mapWndMap;
	TCHAR wszBuffer[MAX_PATH] = { 0 };

	// 遍历托盘map
	bRes = TrayMapHelper.GetMap(mapWndMap);
	if (bRes)
	{// 成功复制traymap
		TLOG_INFO(_T("Find The TrayMap Successful! Start To PostMessage!"));
		for (auto iter = mapWndMap.cbegin();iter != mapWndMap.cend();iter++)
		{
			GetWindowThreadProcessId(iter->first, &dwProcessId); // 获取当前窗口对应的进程ID
			thParam.dwProcessID = dwProcessId;
			thParam.hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, dwProcessId);
			thParam.hWnd = iter->first;

			//COUT << dwProcessId << "   " << it->first << ENDL;
			wnsprintf(wszBuffer, MAX_PATH, TEXT("Found target process windows.pid = %d hWnd = %08X\n"), dwProcessId, iter->first);
			TLOG_INFO(wszBuffer);

			ThreadFunc((void*)&thParam);
		}
	}
	else
	{// traymap为空或者复制map出错
		TLOG_INFO(_T("Find The TrayMap Failed! Exit!"));
	}

	// 遍历主界面map
	bRes = MainMapHelper.GetMap(mapWndMap);
	if (bRes)
	{// 成功复制Mainmap
		TLOG_INFO(_T("Find The MainMap Successful! Start To PostMessage!"));
		for (auto iter = mapWndMap.cbegin();iter != mapWndMap.cend();iter++)
		{
			GetWindowThreadProcessId(iter->first, &dwProcessId); // 获取当前窗口对应的进程ID
			thParam.dwProcessID = dwProcessId;
			thParam.hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, dwProcessId);
			thParam.hWnd = iter->first;

			//COUT << dwProcessId << "   " << iter->first << ENDL;
			wnsprintf(wszBuffer, MAX_PATH, TEXT("Found target process windows.pid = %d hWnd = %08X \n"), dwProcessId, iter->first);
			TLOG_INFO(wszBuffer);
			ThreadFunc((void*)&thParam);
		}
	}
	else
	{// Mainmap为空或者复制map出错
		TLOG_INFO(_T("Find The MainMap Failed! Exit!"));
	}
	return;
}