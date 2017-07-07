#include "stdafx.h"
#include "MapHelper.h"


MapHelper::MapHelper(BOOL bFlag): bTrayFlag(bFlag)
{
}


MapHelper::~MapHelper()
{
}

BOOL MapHelper::IsTrayMap()
{// 返回True表示当前的map为托盘tray的map，否则为主界面的map
	return bTrayFlag;
}

BOOL MapHelper::printMap(const mhhMAP& mhhmap)
{
	TCHAR szBuffer[MAX_PATH] = { 0 };

	for (auto iter = mhhmap.begin(); iter != mhhmap.end(); iter++)
	{
		_sntprintf_s(szBuffer, MAX_PATH, TEXT("NOW WND( %08X ): Parent WND( %08X ) -- WindowName: %s -- ClassName: %s\n"),
			iter->first, iter->second.first, iter->second.second.first.c_str(), iter->second.second.second.c_str());
		TLOG_INFO(szBuffer);
	}
	return mhhmap.size() > 0 ? TRUE : FALSE;
}

BOOL MapHelper::printMap()
{
	TCHAR szBuffer[MAX_PATH] = { 0 };

	for (auto iter = WndMap.begin(); iter != WndMap.end(); iter++)
	{		
		_sntprintf_s(szBuffer, MAX_PATH, TEXT("NOW WND( %08X ): Parent WND( %08X ) -- WindowName: %s -- ClassName: %s\n"),
			iter->first, iter->second.first, iter->second.second.first.c_str(), iter->second.second.second.c_str());
		TLOG_INFO(szBuffer);
		//TLOG_INFO(_T("NOW WND( %s ): Parent WND( %s ) -- WindowName: %s -- ClassName: %s",
		//	it->first, it->second.first, it->second.second.first, it->second.second.second));
		//COUT << "Now WND(" << it->first << "): " << "Parent WND(" << it->second.first << ") " << ENDL;
			/*<< "--WindowName:" << it->second.second.first << " --ClassName:" << it->second.second.second << ENDL;*/
	}

	return WndMap.size() > 0 ? TRUE : FALSE;
}

BOOL MapHelper::GetMap(mhhMAP& mhhmap)
{
	phpPair phpValue;
	strstrPair ppNames;

	for (auto it = WndMap.cbegin();it != WndMap.cend();it++)
	{
		ppNames.first = it->second.second.first;
		ppNames.second = it->second.second.second;
		phpValue.first = it->second.first;
		phpValue.second = ppNames;

		mhhmap[it->first] = phpValue;
	}
	return WndMap.size() > 0 ? TRUE : FALSE;
}

BOOL MapHelper::AddToMap(HWND NowhWnd, HWND ParhWnd, TString strWndName, TString strClassName)
{
	// 为WndMap增添一个元素 key（当前窗口） :values(父窗口、窗口名称，类名称)
	if (!NowhWnd)
		return FALSE;
	phpPair phpValue;
	strstrPair ppNames;

	ppNames.first = strWndName;
	ppNames.second = strClassName;
	phpValue.first = ParhWnd;
	phpValue.second = ppNames;
	WndMap[NowhWnd] = phpValue;
	return TRUE;
}