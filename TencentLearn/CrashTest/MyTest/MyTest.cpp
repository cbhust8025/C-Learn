// MyTest.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "VectorHelper.h"
#include "ProcessLogic.h"
#include "MapHelper.h"
#include "FileLog.h"

#define TRAY_EXE	(_TEXT("QQPCTray.exe"))  // �ҵ��ܼҵ�������

/*      ȫ�ֱ�������      */
DWORD dwTrayID;  // ��������tray��ID
void StartMap();  // �������Ժ���
void MsgGoMap();  // ������Ϣ���Ժ���
MapHelper TrayMapHelper(TRUE);  // ʵ����һ������map��������,�ں�һ����map
MapHelper MainMapHelper(FALSE);  // ʵ����һ��������map��������,�ں�һ����map
VectorHelper vecHelper;  // ʵ����һ��vector��������
CProcessLogic cProcessLogic; // ʵ����һ�����̲�������
/*      ȫ�ֱ�������      */

int _tmain(int argc, _TCHAR* argv[])  // ���
{
	// ��ʼ����־
	TLOG_INIT(NULL);
	TLOG_UPDATELEVEL(MODULELOG_LEVEL_INFO);

	TLOG_INFO(_T("��ʼ���ԣ�"));
	//COUT << "��ʼ����~��" << ENDL;

	//PostMessage(FindWindow(_T("AtlAxWin80"), NULL), 0x37F, NULL, NULL);
	StartMap(); // ��ʼ������tray��������Ľ�����Ϣ����map
	TLOG_INFO(_T("�������TrayMap:"));
	COUT << "�������TrayMap:" << ENDL;
	TrayMapHelper.printMap();

	TLOG_INFO(_T("���������Map:"));
	COUT << "���������Map:" << ENDL;
	MainMapHelper.printMap();

	TLOG_INFO(_T("�����ƺõ�map����ʼ��ÿ�����ڷ�����Ϣ:"));
	//COUT << "�����ƺõ�map����ʼ��ÿ�����ڷ�����Ϣ:" << ENDL;
	MsgGoMap();

	TLOG_INFO(_T("��������~��"));
	//COUT << "��������~��" << ENDL;

	TLOG_UNINIT();
    return 0;  // ������������������̣��˳�
}

unsigned int CALLBACK ThreadFunc(void* vParam)
{
	// ��ÿ�����ڿ�һ���̷߳���0x0000-0xFFFF��Ϣ
	ThreadParam *pthParam = (ThreadParam *)vParam;
	DWORD dwExitCode = 0; // ���󷵻���
	DWORD dwMessage = 0;  // ���͵�message����0000-ffff
	TCHAR wszBuffer[MAX_PATH] = { 0 };

	if (pthParam->hWnd)
	{
		// ������ھ����Ϊ0
		__try
		{
			do {
				// ��0000 ��ʼ����Ϣ��ֱ��0xffff����
				GetExitCodeProcess(pthParam->hProcess, &dwExitCode); // ��ȡ������
				//#define STATUS_PENDING    ((DWORD   )0x00000103L)    
				if (dwExitCode != STILL_ACTIVE) 
				{
					// ����߳��ѽ���
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
						// ����߳��ѽ���
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
	// ��ǰ�ص����������ڵĸ����Ӵ��ڣ������Ǵ��Ӵ��ڻص��������Ӵ��ڣ��Ƴ�һ��map
	BOOL bRes = FALSE;
	TCHAR szWndName[MAX_PATH + 1] = { 0 };
	TCHAR szClassName[MAX_PATH + 1] = { 0 };
	phpPair phpValue;
	strstrPair ppNames;
	TString strWndName;
	TString strClassName;  // ���洰�����ƣ��Լ�������
	DWORD dwTargetID = *(DWORD*)lpTargetID;  // ����ǰ�����ID���ó�Ŀ��ID��Ҳ��������Ҫ�ҵĴ��ڶ�Ӧ�Ľ���ID

	GetWindowText(hWnd, szWndName, MAX_PATH);
	GetClassName(hWnd, szClassName, MAX_PATH);
	cProcessLogic.OutText(szWndName, strWndName);
	cProcessLogic.OutText(szClassName, strClassName);
	if (dwTargetID == dwTrayID)
	{
		// ���Ŀ��ID�����̽���
		bRes = TrayMapHelper.AddToMap(hWnd, GetParent(hWnd), strWndName, strClassName);
		if (bRes)
		{
			TLOG_INFO(_T("TrayMap Add Item Successful!"));
			//COUT << "TrayMap Add Item Successful!" << ENDL;
		}
	}
	else
	{
		// ���Ŀ��ID�����������
		bRes = MainMapHelper.AddToMap(hWnd, GetParent(hWnd), strWndName, strClassName);
		if (bRes)
		{
			TLOG_INFO(_T("MainMap Add Item Successful!"));
			//COUT << "MainMap Add Item Successful!" << ENDL;
		}
	}

	TLOG_INFO(_T("Call the EnumChildFunc!"));
	//COUT << "Call the EnumChildFunc!    " ;
	EnumChildWindows(hWnd, (WNDENUMPROC)EnumChildFunc, (LPARAM)&lpTargetID);  // ���Ӵ��ڵ�ÿ���Ӵ��ڵ���EnumChildFunc(�����ڣ� ) ����
	return TRUE;
}

BOOL CALLBACK EnumFunc(HWND hWnd, LPARAM lpTargetID)
{// ��ÿ�����������ڵ��õĻص�����,�����ǰ�Ĵ��ھ���������Ǵ����lpID���ʾ��ǰ�Ĵ�������������Ҫ��������
	//COUT << "Call the EnumFunc! " << *(DWORD *)lpID << "   ";
	BOOL bRes = FALSE;
	TCHAR szWndName[MAX_PATH + 1] = { 0 };
	TCHAR szClassName[MAX_PATH + 1] = { 0 };
	TString strWndName;
	TString strClassName;  // ���洰�����ƣ��Լ�������
	DWORD dwNowProcessID = 0; // ����������ȡ��ǰ���ڶ�Ӧ�Ľ���ID
	DWORD dwTargetID = *(DWORD*)lpTargetID;  // ����ǰ�����ID���ó�Ŀ��ID��Ҳ��������Ҫ�ҵĴ��ڶ�Ӧ�Ľ���ID

	if (dwTargetID > 0)  // ���Ŀ��ID��Ϊ��
	{
		GetWindowThreadProcessId(hWnd, &dwNowProcessID);  // ��ȡ��ǰ���ڶ�Ӧ�Ľ���ID
		if (dwNowProcessID == dwTargetID) 
		{ 
			//������ͬ˵�������ҵ���Ŀ��ID���������������
			COUT << hWnd << "  " << GetParent(hWnd) << ENDL;
			EnumChildWindows(hWnd, (WNDENUMPROC)EnumChildFunc, (LPARAM)&dwTargetID);  // �������ڵ�ÿ���Ӵ��ڵ���EnumChildFunc(�����ڣ� ) ����
			GetWindowText(hWnd, szWndName, MAX_PATH);
			GetClassName(hWnd, szClassName, MAX_PATH);
			cProcessLogic.OutText(szWndName, strWndName);
			cProcessLogic.OutText(szClassName, strClassName);
			if (dwTargetID == dwTrayID) 
			{
				// ���Ŀ��ID�����̽���
				bRes = TrayMapHelper.AddToMap(hWnd, GetParent(hWnd), strWndName, strClassName);
				if (bRes)
				{
					TLOG_INFO(_T("TrayMap Add Item Successful!"));
					//COUT << "TrayMap Add Item Successful!" << ENDL;
				}
			}
			else 
			{
				// ���Ŀ��ID�����������
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
		TLOG_INFO(_T("δ�ҵ��ܼ����̵Ľ���ID��"));
		//COUT << "δ�ҵ��ܼ����̵Ľ���ID��" << ENDL;
		return;
	}
	for (int i = 0; i < vecIDs.size();i++)
	{
		if (!EnumWindows((WNDENUMPROC)EnumFunc, (LPARAM)&vecIDs[i]))  // ����tray�Լ�������
		{
			TLOG_INFO(_T("EnumWindows failed! error ="));
			//COUT << "EnumWindows failed! error =" << GetLastError() << ENDL;
		}
	}
	return;
}

void MsgGoMap()
{// ��������map,���̷߳�����Ϣ, �������־
	BOOL bRes = FALSE;
	ThreadParam thParam; // ʵ�����ṹ�壬�������ID�����̾�������ھ��
	DWORD dwProcessId = 0; // ���������ʱ��������ID
	mhhMAP mapWndMap;
	TCHAR wszBuffer[MAX_PATH] = { 0 };

	// ��������map
	bRes = TrayMapHelper.GetMap(mapWndMap);
	if (bRes)
	{// �ɹ�����traymap
		TLOG_INFO(_T("Find The TrayMap Successful! Start To PostMessage!"));
		for (auto iter = mapWndMap.cbegin();iter != mapWndMap.cend();iter++)
		{
			GetWindowThreadProcessId(iter->first, &dwProcessId); // ��ȡ��ǰ���ڶ�Ӧ�Ľ���ID
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
	{// traymapΪ�ջ��߸���map����
		TLOG_INFO(_T("Find The TrayMap Failed! Exit!"));
	}

	// ����������map
	bRes = MainMapHelper.GetMap(mapWndMap);
	if (bRes)
	{// �ɹ�����Mainmap
		TLOG_INFO(_T("Find The MainMap Successful! Start To PostMessage!"));
		for (auto iter = mapWndMap.cbegin();iter != mapWndMap.cend();iter++)
		{
			GetWindowThreadProcessId(iter->first, &dwProcessId); // ��ȡ��ǰ���ڶ�Ӧ�Ľ���ID
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
	{// MainmapΪ�ջ��߸���map����
		TLOG_INFO(_T("Find The MainMap Failed! Exit!"));
	}
	return;
}