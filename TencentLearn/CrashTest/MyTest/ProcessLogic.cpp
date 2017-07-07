#include "stdafx.h"
#include "ProcessLogic.h"


CProcessLogic::CProcessLogic()
{
}


CProcessLogic::~CProcessLogic()
{
}

BOOL CProcessLogic::OutText(TCHAR w[], TString& cRes) {
	// �����������������,������������string ��
	int index = 0;

	while (w[index] != '\0') {
		cRes.push_back(w[index]);
		index++;
	}
	return w[0] == '\0' ? TRUE : FALSE;
}

BOOL CProcessLogic::GetExeProcessId(LPCTSTR ExeName, vecID& vecInfo)
{// ��ȡExeName ��Ӧ��ID�������в�ֹһ��ID����������������Ҫ��ID
	BOOL bRes = FALSE;
	HANDLE vR1;  // ����һ��void����ָ��
	DWORD dwResult;  // ����һ��unsigned long���ͱ���
	PROCESSENTRY32 pcEntry;  // ����һ�����̶���
	BOOL bFind = FALSE;  // ��ʶ�Ƿ��ҵ���������Ҫ�Ľ���--QQ���Թܼ�

	memset(&pcEntry, 0, sizeof(PROCESSENTRY32)); // �����ڴ�����̶���
												 // ����dwSize�����Ҵ�Сһ��Ϊsizeof(PROCESSENTRY32)����������ã������Process32First��ʧ��
	pcEntry.dwSize = sizeof(PROCESSENTRY32);
	vR1 = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0); // ��ȡ��ǰ���̵Ŀ��գ����ؾ��
	COUT << "vR1: " << vR1 << ENDL;
	if (vR1 != (HANDLE)-1 && Process32First(vR1, &pcEntry)) 
	{
		// �����ǰ���̵ľ����Ϊ-1�����ʾ������Get����ǰ���̶���ϵͳ����--0��
		TLOG_INFO(TEXT("Process Normal!"));
		COUT << "Process Normal!" << ENDL;
		do 
		{
			bRes = lstrcmpi(pcEntry.szExeFile, ExeName);
			if (!bRes)
			{
				// ��ÿ�������ж���������Ƿ�ΪQQPCTray.exe,���ܻ��ҵ�����QQPCTray.exe������������ʾʱ��
				//TString sOut;
				//OutText(pcEntry.szExeFile, sOut);
				//COUT << "vR1: " << vR1 << ENDL;
				//COUT << "pEth32ID: " << pcEntry.th32ProcessID << ENDL;
				//COUT << "pEszEF: " << sOut << ENDL;
				vecInfo.push_back(pcEntry.th32ProcessID);
			}
		} while (Process32Next(vR1, &pcEntry));  // ������ԵĻ���ȡ��һ�����̶���(0--4--376--548�ȵ�)
	}
	else 
	{
		dwResult = 0;
		COUT << "Process Error!" << ENDL;
	}

	return vecInfo.size() > 0 ? TRUE : FALSE;
}

BOOL CProcessLogic::GetParentProcessId(DWORD dwNowId, DWORD& dwParentId)
{// ��ȡ��ǰ�������ID��Ӧ�ĸ�����ID
	HANDLE hProcess = NULL;  // ����һ���վ��������򿪵�ǰ����ID��Ӧ�Ľ��̾��
	PROCESS_BASIC_INFORMATION pbiInfo; //  ʵ����һ��PROCESS_BASIC_INFORMATION���Ͷ��󣬺�������OpenProcess����
	LONG lStatus; // ����״̬����
	BOOL bFlag = FALSE; // �����Ƿ�ɹ��ҵ�������ID
	hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwNowId); // ��ID��Ӧ�Ľ��̣���������
	// ע��򿪽��̾��ʱҪ����ԱȨ������VS2015,����᷵�ش������5--�ܾ����ʡ�

	// ָ�����͵Ľ�����Ϣ������ĳ������
	if (hProcess != NULL)  
	{// ������Դ򿪽��̣����ؽ��̾����
		lStatus = NtQueryInformationProcess(hProcess,  // ������̾��
			ProcessBasicInformation,  // ����������
			(PVOID)& pbiInfo, // ������
			sizeof(PROCESS_BASIC_INFORMATION),  //����������С
			NULL);
		// lStatus == S_OK(0)��ʱ�򣬱�ʾд�뻺�����ɹ�
		if (S_OK == lStatus)
		{// ��������ɹ�

			COUT << "pbiInfo.PebBaseAddress: " << pbiInfo.PebBaseAddress << ENDL;
			COUT << "pbiInfo.Reserved1: " << (DWORD)pbiInfo.Reserved1 << ENDL;
			COUT << "pbiInfo.Reserved2: " << (DWORD)pbiInfo.Reserved2 << ENDL;
			COUT << "pbiInfo.Reserved3: " << (DWORD)pbiInfo.Reserved3 << ENDL;
			COUT << "pbiInfo.UniqueProcessId: " << pbiInfo.UniqueProcessId << ENDL;
			dwParentId = (DWORD)pbiInfo.Reserved3;
			bFlag = TRUE;
		}
		CloseHandle(hProcess);  // ���پ��
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

