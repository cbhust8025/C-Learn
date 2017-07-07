#pragma once
class CProcessLogic
{
public:
	CProcessLogic();
	~CProcessLogic();
	BOOL OutText(TCHAR w[], TString& cRes);// 声明输出进程名函数
	BOOL GetExeProcessId(LPCTSTR ExeName, vecID& vecInfo); // 获取ExeName 对应的ID，可能有不止一个ID，返回所有我们想要的ID
	BOOL GetParentProcessId(DWORD dwNowId, DWORD& dwParentId); // 获取当前传入进程ID对应的父进程ID
};

