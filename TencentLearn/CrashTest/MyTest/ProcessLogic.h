#pragma once
class CProcessLogic
{
public:
	CProcessLogic();
	~CProcessLogic();
	BOOL OutText(TCHAR w[], TString& cRes);// �����������������
	BOOL GetExeProcessId(LPCTSTR ExeName, vecID& vecInfo); // ��ȡExeName ��Ӧ��ID�������в�ֹһ��ID����������������Ҫ��ID
	BOOL GetParentProcessId(DWORD dwNowId, DWORD& dwParentId); // ��ȡ��ǰ�������ID��Ӧ�ĸ�����ID
};

